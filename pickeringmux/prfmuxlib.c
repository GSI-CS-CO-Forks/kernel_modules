static char *rcsid(){return "$Id: prfmuxlib.c,v 1.2 2004/03/08 10:42:33 sdeghaye Exp $";}

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "prfmuxdriver.h"
#include "prfmuxlib.h"

#define MAX_SLOT 32

static int fd[MAX_SLOT];

/***************************************************************************************************
 *
 */
static int checkFileDescr(int aSlot)
{
  if(fd[aSlot] == 0)
  {
    char buf[50];
  
    sprintf(buf, "/dev/%s/%s%d", PRFMUX_DIR_NAME, PRFMUX_FILE_NAME, aSlot);
    fd[aSlot] = open(buf, O_RDWR);
    if(fd[aSlot] == 0)
    {
      return PRFMUXLIB_SYSTEM_ERROR;
    }
    else
    {
      return fd[aSlot];
    }
  }
  else
  {
    return fd[aSlot];
  }
}

/***************************************************************************************************
 *
 */
int prfmuxConnect(int aSlot, int anInput, int anOutput)
{
  int muxFd;
  int rc;
  char buf[5];
  
  if(anInput > PRFMUX_MAX_INPUT || anOutput > PRFMUX_MAX_OUTPUT || aSlot > MAX_SLOT)
  {
    return PRFMUXLIB_BAD_PARAMETER;
  }
  else
  {
    muxFd = checkFileDescr(aSlot);
    if(muxFd == PRFMUXLIB_SYSTEM_ERROR)
    {
      return PRFMUXLIB_NO_SUCH_MUX;
    }
    sprintf(buf, "%d", anInput);
    rc = write(muxFd, buf, 1);
    if(rc == PRFMUXLIB_SYSTEM_ERROR)
    {
      return rc;
    }
    return 0;
  }
}

/***************************************************************************************************
 *
 */
int prfmuxOuputStatus(int aSlot, int anOutput)
{
  int muxFd;
  int rc;
  char buf[5];
  
  if(anOutput > PRFMUX_MAX_OUTPUT || aSlot > MAX_SLOT)
  {
    return PRFMUXLIB_BAD_PARAMETER;
  }
  else
  {
    muxFd = checkFileDescr(aSlot);
    if(muxFd == PRFMUXLIB_SYSTEM_ERROR)
    {
      return PRFMUXLIB_NO_SUCH_MUX;
    }
    rc = read(muxFd, buf, 1);
    if(rc == PRFMUXLIB_SYSTEM_ERROR)
    {
      sscanf(buf, "%d", &rc);
    }
    return 0;
  }
}

/***************************************************************************************************
 *
 */
void prfmuxPrintError(char *aMessage, int anError)
{
  switch(anError)
  {
  case PRFMUXLIB_BAD_PARAMETER:
    printf("%s: %s\n", aMessage, PRFMUXLIB_BAD_PARAMETER_STRING);
    break;
  case PRFMUXLIB_NO_SUCH_MUX:
    printf("%s: %s\n", aMessage, PRFMUXLIB_NO_SUCH_MUX_STRING);
    break;
  case PRFMUXLIB_SYSTEM_ERROR:
    perror(aMessage);
    break;
  default:
    printf("%s: prfmuxlib: No such error code (%d)\n", aMessage, anError);
    break;
  } 
}
