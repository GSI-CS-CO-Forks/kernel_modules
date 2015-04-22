/**************************************************************************/
/* Syn test                                                              */
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

#include <syndrvr.h>

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
static int  syn;

#include "Cmds.h"
#include "GetAtoms.inc"
#include "PrintAtoms.inc"
#include "DoCmd.inc"
#include "Cmds.inc"
#include "SynOpen.inc"
#include "SynCmds.inc"

/**************************************************************************/
/* Prompt and do commands in a loop                                       */
/**************************************************************************/

int main(int argc,char *argv[]) {

int rp, pr;
char *cp;
char host[49];
char tmpb[CMD_BUF_SIZE];

#if NEWS
   printf("syntest: See <news> command\n");
   printf("syntest: Type h for help\n");
#endif

   pname = argv[0];
   printf("%s: Compiled %s %s\n",pname,__DATE__,__TIME__);

   syn = SynOpen();
   if (syn == 0) {
      printf("\nWARNING: Could not open driver");
      printf("\n\n");
   } else {
      printf("Driver opened OK: Using SYN module: 1\n\n");
   }

   bzero((void *) host,49);
   gethostname(host,48);

   while (True) {

      if (syn) sprintf(prompt,"%s:Syn[%02d]",host,cmdindx+1);
      else     sprintf(prompt,"%s:NoDriver:Syn[%02d]",host,cmdindx+1);
      printf("%s",prompt);

      cmdbuf = &(history[cmdindx][0]);

      bzero((void *) tmpb,CMD_BUF_SIZE);
      if (gets(tmpb)==NULL) exit(1);

      cp = &(tmpb[0]);
      pr = 0;           /* Dont print a history */
      rp = 0;           /* Dont repeat a command */

      while ((*cp == '-')
      ||     (*cp == '+')
      ||     (*cp == '.')
      ||     (*cp == '!')) {

	 pr = 1;        /* Print command on */

	 if (*cp == '!') {
	    cmdindx = strtoul(++cp,&cp,0) -1;
	    if (cmdindx >= HISTORIES) cmdindx = 0;
	    if (cmdindx < 0) cmdindx = HISTORIES -1;
	    rp = 1;
	    break;
	 }

	 if (*cp == '-') {
	    if (--cmdindx < 0) cmdindx = HISTORIES -1;
	    cmdbuf = &(history[cmdindx][0]);
	 }

	 if (*cp == '+') {
	    if (++cmdindx >= HISTORIES) cmdindx = 0;
	    cmdbuf = &(history[cmdindx][0]);
	 }

	 if (*cp == '.') {
	    rp = 1;
	    break;
	 };
	 cp++;
      }
      if (pr) {
	 printf("{%s}\t ",cmdbuf); fflush(stdout);
	 if (!rp) continue;
      }
      if (!rp) strcpy(cmdbuf,tmpb);

      bzero((void *) val_bufs,sizeof(val_bufs));
      GetAtoms(cmdbuf);
      DoCmd(0);

      if ((!rp) && (++cmdindx >= HISTORIES)) cmdindx = 0;
   }
}
