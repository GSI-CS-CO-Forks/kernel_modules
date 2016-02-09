/* $Id: pickeringmuxdrv.h,v 1.7 2008/12/04 15:52:29 nmn Exp $ */
#ifndef PICKERINGMUXDRV_H
#define PICKERINGMUXDRV_H

#define PICKERINGMUX_DIR_NAME "pickeringmux"
#define PICKERINGMUX_FILE_TEMPLATE PICKERINGMUX_DIR_NAME"/slot%d-%d"
#define PICKERINGMUX_PATH_TEMPLATE "/dev/"PICKERINGMUX_FILE_TEMPLATE

#define MAX_SLOT 32 /* Max 32 slots a PCI bus*/
#define MAX_BUS   4 /* Max 4 PCI buses supported */

/* IOCTL commands */
#include <linux/ioctl.h>
#define PICKERINGMUX_MAGIC 'H' /* See Documentation/ioctl-number.h */
#define PICKERINGMUX_CONNECT    _IOW(PICKERINGMUX_MAGIC, 1, PickeringMuxConnectData)
#define PICKERINGMUX_DISCONNECT _IOW(PICKERINGMUX_MAGIC, 2, int)
#define PICKERINGMUX_RESET      _IO(PICKERINGMUX_MAGIC, 3)
#define PICKERINGMUX_MAXOUTPUT  _IOR(PICKERINGMUX_MAGIC, 4, int)
#define PICKERINGMUX_MAXINPUT   _IOR(PICKERINGMUX_MAGIC, 5, int)
#define PICKERINGMUX_STATUS     _IOR(PICKERINGMUX_MAGIC, 6, int)
#define PICKERINGMUX_WIDTH      _IOR(PICKERINGMUX_MAGIC, 7, int)
#define PICKERING_SET_ATTN      _IOR(PICKERINGMUX_MAGIC, 8, PickeringAttnData)

/* Data structure for the Connect */
typedef struct
{
  int input;
  int output;
} PickeringMuxConnectData;

/* Data structure for the SetAttn */
typedef struct
{
  int channel;
  int attn;
} PickeringAttnData;

/* This part of the header file is use in the driver only */
#ifdef __KERNEL__

#ifdef CONFIG_DEVFS_FS
#include <linux/devfs_fs_kernel.h>
#endif
#include <linux/cdev.h>

#define PICK_MUX_OE_SET 0x0010
#define PICK_MUX_CLK_PERIOD 100

#define PICK_MUX_CLK_POS 1
#define PICK_MUX_DATA_POS 2
#define PICK_MUX_STRB1_POS 3
#define PICK_MUX_STRB2_POS 5

/* Module descriptor */
typedef struct 
{
  unsigned long las0addr;
  int las0len;
  void *las0;
  unsigned long las1addr;
  int las1len;    
  void *las1;
  unsigned short type;
  unsigned char bus;	/* PCI bus number of the device */
  unsigned int device;	/* PCI device number of the device */
  unsigned int *inputs;	/* input number currently connected to the outputs */
  unsigned int *loopData; /* Data held by the mux registers (Serial architecture)*/
  struct cdev charDev; /* Char device structure */
  /* We probably need to add a semaphore here */ 
} PickeringMuxDescr;

/* Structure to store relay mapping of serial architecture */
/* A relay is identified by its index in the loops data */
typedef struct
{
  int yIsoRelay; /* This is the relay that isolates an entry from a 4*4 matrix (see doc).*/
  int yxRelay;   /* This is the relay in the 4*4 matrix that connects a X to a Y. */
  int xIsoRelay; /* This is the relay that isolates an output from a 4*4 matrix.*/
} RelayMap;

/* Prototype */
void pickeringMuxSendData(PickeringMuxDescr *muxDesc, int nbrLoop, int c);


#endif /* __KERNEL__ */

#endif
