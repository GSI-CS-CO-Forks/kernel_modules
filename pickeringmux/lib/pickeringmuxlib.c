static __attribute__((unused)) char *rcsid(){return "$Id: pickeringmuxlib.c,v 1.9 2006/03/21 15:06:57 sdeghaye Exp $";}
const char * const libpickeringmux_version_s = "libpickeringmux version: " GIT_VERSION;

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

/**
 * @brief generate a device handle for subsequent reference
 * @param aSlot		- pickeringmux cPCI slot
 *
 * @return and opaque MuxHandle for the device
 * 		at aSlot on success, NULL on failure.
 *
 * The returned handle points to information on the device,
 * which is kept statically by the library.
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

/**
 * @brief connect input and output channels of the mux
 * @param aSlot		- pickeringmux cPCI slot
 * @param anInput	- analog input channel
 * @param anOutput	- analog output channel
 *
 * @return 0 on success, <0 on failure, sets errno
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

/**
 * @brief destroy the connection going to an analog channel
 * @param aSlot		- pickeringmux cPCI slot
 * @param anOutput	- analog output channel
 *
 * @return 0 on success, <0 on failure, sets errno
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
    
/**
 * @brief reset the multiplexor
 * @param aSlot		- pickeringmux cPCI slot
 *
 * @return 0 on success, <0 on failure, sets errno
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

/**
 * @brief width of a hardware module
 * @param aSlot		- pickeringmux cPCI slot
 * @return module width on success, <0 on failure, sets errno
 *
 * Returns the width of the hardware module, installed in
 *		cPCI slot 'aSlot'. Module width can be up to 2 slots. 
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


/**
 * @brief obtain maximum output channel # of a module
 * @param aSlot		- pickeringmux cPCI slot
 * @return max output (e.g. 16 in a x to 16 mux), <0 on failure, sets errno
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

/**
 * @brief obtain maximum input channel # of a module
 * @param aSlot		- pickeringmux cPCI slot
 * @return max output (e.g. 22 in a 22 to 8 mux), <0 on failure, sets errno
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

/**
 * @brief get status of module outputs
 * @param aSlot		- pickeringmux cPCI slot
 * @param outputs	- a vector [1..maxout]
 * @return module width on success, <0 on failure, sets errno
 *
 *    Entry outputs[i-1] receives the input it is connected to, or
 *    a zero value if disconnected (this description is
 *    reverse-engineered from the code, take with a grain of salt)
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

/**
 * @brief set channel attenuation
 * @param aSlot		- pickeringmux cPCI slot
 * @param aChannel	- analog channel to which attn applies
 * @param anAttn	- attenuation to apply (0 to 63, in dB)
 *
 * @return module width on success, <0 on failure, sets errno
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


/**
 * @brief decode libpickeringmux error codes
 *
 * @param aMessage	- pointer to receive decoded string
 * @param anError	- error code
 *
 * Almost all library functions return a specific error code < 0,
 * which this function decodes into a human-readable diagnostic or its
 * errno equivalent
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
