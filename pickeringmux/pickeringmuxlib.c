static __attribute__((unused)) char *rcsid(){return "$Id: pickeringmuxlib.c,v 1.9 2006/03/21 15:06:57 sdeghaye Exp $";}

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "pickeringmuxdrv.h"
#include "pickeringmuxlib.h"
#include "pciioconfig/crateconfig.h"

typedef struct
{
  int fd;
  int maxInput;
  int maxOutput;
} MuxHandle;

static MuxHandle muxHanlde[MAX_SLOT];

/***************************************************************************************************
 *
 */
static MuxHandle * checkMuxHandle(int aSlot)
{
  int fd;
  
  if(aSlot > MAX_SLOT)
  {
    return NULL;
  }
  
  if(muxHanlde[aSlot].fd == 0)
  {
    char buf[50];
    int bus;
    int device;
    int rc;
    
    rc = getPCIPosition(aSlot, &bus, &device);
    if(rc != 0)
    {
      return NULL;
    }
    
    sprintf(buf, PICKERINGMUX_PATH_TEMPLATE, bus, device);
    fd = open(buf, O_RDWR);
    if(fd == -1)
    {
      return NULL;
    }
    muxHanlde[aSlot].fd = fd;
    
    /* Get the ranges */
    if(ioctl(fd, PICKERINGMUX_MAXINPUT, &muxHanlde[aSlot].maxInput) != 0)
    {
      perror("pickeringmuxlib ioctl(PICKERINGMUX_MAXINPUT)");
      return NULL;
    }
    
    if(ioctl(fd, PICKERINGMUX_MAXOUTPUT, &muxHanlde[aSlot].maxOutput) != 0)
    {
      perror("pickeringmuxlib ioctl(PICKERINGMUX_MAXOUTPUT)");
      return NULL;
    }
  }
  return &muxHanlde[aSlot];
}

/***************************************************************************************************
 *
 */
int pickeringMuxConnect(int aSlot, int anInput, int anOutput)
{
  MuxHandle *muxHandle;
  PickeringMuxConnectData connectData;
  int rc;
  
  muxHandle = checkMuxHandle(aSlot);
  if(muxHandle == NULL)
  {
    return PICKERINGMUX_NO_SUCH_MUX;
  }
  
  if(anInput > muxHandle->maxInput || anOutput > muxHandle->maxOutput)
  {
    return PICKERINGMUX_BAD_PARAMETER;
  }
 
  connectData.input = anInput;
  connectData.output = anOutput;
  
  rc = ioctl(muxHandle->fd, PICKERINGMUX_CONNECT, &connectData);
  if(rc != 0)
  {
    return rc;
  }
  return 0;
}

/***************************************************************************************************
 *
 */
int pickeringMuxDisconnect(int aSlot, int anOutput)
{
  MuxHandle *muxHandle;
  int rc;
  
  muxHandle = checkMuxHandle(aSlot);
  if(muxHandle == NULL)
  {
    return PICKERINGMUX_NO_SUCH_MUX;
  }
  
  if(anOutput > muxHandle->maxOutput)
  {
    return PICKERINGMUX_BAD_PARAMETER;
  }
  
  rc = ioctl(muxHandle->fd, PICKERINGMUX_DISCONNECT, &anOutput);
  if(rc != 0)
  {
    return rc;
  }
  return 0;
}
    
/***************************************************************************************************
 *
 */
int pickeringMuxReset(int aSlot)
{
  
  MuxHandle *muxHandle;
  int rc;
  
  muxHandle = checkMuxHandle(aSlot);
  if(muxHandle == NULL)
  {
    return PICKERINGMUX_NO_SUCH_MUX;
  }
  
  rc = ioctl(muxHandle->fd, PICKERINGMUX_RESET, NULL);
  if(rc != 0)
  {
    return rc;
  }
  return 0;
}

/*-----------------------------------------------------------------------------
 * FUNCTION:    pickeringMuxWidth.
 * DESCRIPTION: Returns the width of the hardware module, installed in
 *		slot 'aSlot'. Module width can be up to 2 slots. 
 * RETURNS:	module width             - if succeed.
 *		PICKERINGMUX_NO_SUCH_MUX - if slot is empty.
 *    PICKERINGMUX_SYSTEM_ERROR - if the driver returns an error.
 *-----------------------------------------------------------------------------
 */
int
pickeringMuxWidth(
		  int aSlot)	/* slot number to check */
{
  MuxHandle *muxHandle;
  int rc, width;
  
  muxHandle = checkMuxHandle(aSlot);

  if (muxHandle == NULL)
    return(PICKERINGMUX_NO_SUCH_MUX); /* error */
  
  rc = ioctl(muxHandle->fd, PICKERINGMUX_WIDTH, &width);
  if (rc != 0)
    return(PICKERINGMUX_SYSTEM_ERROR);			/* internal error */
  
  return(width);
}


/***************************************************************************************************
 *
 */
int pickeringMuxMaxOutput(int aSlot)
{
  MuxHandle *muxHandle = checkMuxHandle(aSlot);
  if(muxHandle == NULL)
  {
    return PICKERINGMUX_NO_SUCH_MUX;
  }
  return muxHandle->maxOutput;
}
    
/***************************************************************************************************
 *
 */
int pickeringMuxMaxInput(int aSlot)
{
  MuxHandle *muxHandle = checkMuxHandle(aSlot);
  if(muxHandle == NULL)
  {
    return PICKERINGMUX_NO_SUCH_MUX;
  }
  return muxHandle->maxInput;
}
    
/***************************************************************************************************
 *
 */
int pickeringMuxOuputStatus(int aSlot, const int *outputs)
{
  MuxHandle *muxHandle;
  int rc;
  
  muxHandle = checkMuxHandle(aSlot);
  if(muxHandle == NULL)
  {
    return PICKERINGMUX_NO_SUCH_MUX;
  }

  rc = ioctl(muxHandle->fd, PICKERINGMUX_STATUS, outputs);
  if(rc != 0)
  {
    return rc;
  }
  return 0;
}

/***************************************************************************************************
 *
 */
int  pickeringAttn(int aSlot, int aChannel, int anAttn)
{
  MuxHandle *muxHandle;
  PickeringAttnData data;
  int rc;
  
  muxHandle = checkMuxHandle(aSlot);
  if(muxHandle == NULL)
  {
    return PICKERINGMUX_NO_SUCH_MUX;
  }
  
  data.channel = aChannel;
  data.attn = anAttn;
  
  rc = ioctl(muxHandle->fd, PICKERING_SET_ATTN, &data);
  if(rc != 0)
  {
    return rc;
  }
  return 0;
}

/***************************************************************************************************
 *
 */
void pickeringMuxPrintError(char *aMessage, int anError)
{
  switch(anError)
  {
  case PICKERINGMUX_BAD_PARAMETER:
    printf("%s: %s\n", aMessage, PICKERINGMUX_BAD_PARAMETER_STRING);
    break;
  case PICKERINGMUX_NO_SUCH_MUX:
    printf("%s: %s\n", aMessage, PICKERINGMUX_NO_SUCH_MUX_STRING);
    break;
  case PICKERINGMUX_SYSTEM_ERROR:
    perror(aMessage);
    break;
  default:
    printf("%s: pickeringmuxlib: No such error code (%d)\n", aMessage, anError);
    break;
  } 
}
