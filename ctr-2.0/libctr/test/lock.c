#include <sys/ipc.h>
#include <sys/sem.h>

#define LOCK_LIB_KEY 13426587

int lock_lib()
{
	key_t key;
	int semid;

	struct sembuf sops[2];
	int nsops, cc;

	key = LOCK_LIB_KEY;

	semid = semget(key, 1, 0666);
	if (semid < 0) {

		semid = semget(key, 1, 0666 | IPC_CREAT);
		if (semid < 0)
			return -1;

		sops[0].sem_num = 0;
		sops[0].sem_op = 0;
		sops[0].sem_flg = SEM_UNDO;

		cc = semop(semid, sops, 1);
		if (cc < 0)
			return -1;
	}

	nsops = 2;

	sops[0].sem_num = 0;
	sops[0].sem_op = 0;
	sops[0].sem_flg = SEM_UNDO;

	sops[1].sem_num = 0;
	sops[1].sem_op = 1;
	sops[1].sem_flg = SEM_UNDO | IPC_NOWAIT;

	cc = semop(semid, sops, nsops);
	if (cc < 0)
		return cc;

	return 0;
}

int unlock_lib()
{
	key_t key;
	int semid;

	struct sembuf sops[2];
	int nsops, cc;

	key = LOCK_LIB_KEY;

	semid = semget(key, 1, 0666);
	if (semid <= 0)
		return -1;

	nsops = 1;

	sops[0].sem_num = 0;
	sops[0].sem_op = -1;
	sops[0].sem_flg = SEM_UNDO | IPC_NOWAIT;

	cc = semop(semid, sops, nsops);
	if (cc < 0)
		return cc;

	return 0;
}
