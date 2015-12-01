/**************************************************************************/
/* Acdx test                                                              */
/* Julian Lewis 15th Nov 2002                                             */
/**************************************************************************/

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <errno.h>        /* Error numbers */
#include <sys/file.h>
#include <a.out.h>
#include <ctype.h>

#include <drm.h>
#include <acdxdrvr.h>
#include <acdxdrvrP.h>

/**************************************************************************/
/* Code section from here on                                              */
/**************************************************************************/

/* Print news on startup if not zero */

#define NEWS 1

#define HISTORIES 24
#define CMD_BUF_SIZE 128

static char history[HISTORIES][CMD_BUF_SIZE];
static char *cmdbuf = &(history[0][0]);
static int  cmdindx = 0;
static char prompt[32];
static char *pname = NULL;
static int  acdx;

#include "Cmds.h"
#include "GetAtoms.c"
#include "PrintAtoms.c"
#include "DoCmd.c"
#include "Cmds.c"
#include "AcdxOpen.c"
#include "AcdxCmds.c"

static char git_version[] = "git_version: " GIT_VERSION;
/**************************************************************************/
/* Prompt and do commands in a loop                                       */
/**************************************************************************/

int main(int argc,char *argv[]) {

char *cp, *ep;
char host[49];
char tmpb[CMD_BUF_SIZE];

   printf("acdxtest: See <news> command\n");
   printf("acdxtest: Type h for help\n");

   pname = argv[0];
   printf("%s: Compiled %s %s\n",pname,__DATE__,__TIME__);
   printf("%s: %s\n",pname, git_version);

   acdx = AcdxOpen();
   if (acdx == 0) {
      printf("\nWARNING: Could not open driver");
      printf("\n\n");
   } else {
      printf("ACDX Driver opened OK\n\n");
   }

   bzero((void *) host,49);
   gethostname(host,48);

   while (True) {

      cmdbuf = &(history[cmdindx][0]);
      if (strlen(cmdbuf)) printf("{%s} ",cmdbuf);
      fflush(stdout);

      if (acdx) sprintf(prompt,"%s:Acdx[%02d]",host,cmdindx+1);
      else     sprintf(prompt,"%s:NoDriver:Acdx[%02d]",host,cmdindx+1);
      printf("%s",prompt);

      bzero((void *) tmpb,CMD_BUF_SIZE);
      if (gets(tmpb)==NULL) exit(1);

      cp = tmpb;
      if (*cp == '!') {
	 cmdindx = strtoul(++cp,&ep,0) -1;
	 cp = ep;
	 if (cmdindx >= HISTORIES) cmdindx = 0;
	 cmdbuf = &(history[cmdindx][0]);
	 continue;
      } else if (*cp == '.') {
	 printf("Execute:%s\n",cmdbuf); fflush(stdout);
      } else if ((*cp == '\n') || (*cp == '\0')) {
	 cmdindx++;
	 if (cmdindx >= HISTORIES) { printf("\n"); cmdindx = 0; }
	 cmdbuf = &(history[cmdindx][0]);
	 continue;
      } else if (*cp == '?') {
	 printf("History:\n");
	 printf("\t!<1..24> Goto command\n");
	 printf("\tCR       Goto next command\n");
	 printf("\t.        Execute current command\n");
	 printf("\this      Show command history\n");
	 continue;
      } else {
	 cmdindx++; if (cmdindx >= HISTORIES) { printf("\n"); cmdindx = 0; }
	 strcpy(cmdbuf,tmpb);
      }
      bzero((void *) val_bufs,sizeof(val_bufs));
      GetAtoms(cmdbuf);
      DoCmd(0);
   }
}
