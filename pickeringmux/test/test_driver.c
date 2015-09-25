
static __attribute__((unused)) char test_version[] = "version: " GIT_VERSION;

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include "pickeringmuxdrv.h"
#include "pciioconfig/crateconfig.h"

static void printUsage(char *basename);
static void printStatus(int *status, int size);
static char waitUserInput();
void dumpMemory(void *addr, int size);

/* Mode flags */
static int testMode;
static int tempoMode;
static int statusMode;
static int attnMode;
static int resetMode;
static int closeMode;
static int openMode;


int main(int argc, char *argv[])
{
  int fd;
  int rc;
  int in, out;
  int maxInput;
  int maxOutput;
  int bus;
  int device;
  int slot;
  int *status;
  char buf[100];
  char c;
  PickeringMuxConnectData connectData;
  
  if(argc < 2)
  {
    printUsage(argv[0]);    
    exit(0);
  }
  
  rc = sscanf(argv[1], "%d\n", &slot);
  if(rc != 1) /* We should decode just one character */
  {
    printf("Bad Argument (%d): %s\nUsage: %s <slot number>\n", rc, argv[1], argv[0]);
    exit(0);
  }
  
  /* Open the module */
  rc = getPCIPosition(slot, &bus, &device);
  if(rc != 0)
  {
    return -1;
  }
  sprintf(buf, PICKERINGMUX_PATH_TEMPLATE, bus, device);
  fd = open(buf, O_RDWR);
  if(fd == -1)
  {
    perror("open");
    return -1;
  }

  /* Get the ranges */
  rc = ioctl(fd, PICKERINGMUX_MAXINPUT, &maxInput);
  if(rc != 0)
  {
    perror("ioctl(maxInput)");
    exit(-1);
  }
  else
  {
    printf("Max Input: %d\n", maxInput);
  }
  
  rc = ioctl(fd, PICKERINGMUX_MAXOUTPUT, &maxOutput);  
  if(rc != 0)
  {
    perror("ioctl(maxOutput)");
    exit(-1);
  }
  else
  {
    printf("Max Output: %d\n", maxOutput);
  }
  
  /* Allocate the buffer for the status */
  status = (int *)malloc(512/*sizeof(int) * maxOutput*/);
  if(status == NULL)
  {
    printf("Error: Unable to allocate memory!\n");
    close(fd);
    exit(-1);
  }
  
  /* Detect the mode */
  if(argc >= 3)
  {
    /* mode auto */
    if(!strcmp(argv[2], "-t"))
    {
      testMode = 1;
    }
    /* mode auto */
    else if(!strcmp(argv[2], "-ta"))
    {
      testMode = 1;
      tempoMode = 1;
    }    
    /* status mode */
    else if(!strcmp(argv[2], "-s"))
    {
      statusMode = 1;
    }
    /* reset mode */
    else if(!strcmp(argv[2], "-r"))
    {
      resetMode = 1;
    }
    /* open mode */
    else if(!strcmp(argv[2], "-o"))
    {
      openMode = 1;
    }
    /* clsoe mode */
    else if(!strcmp(argv[2], "-c"))
    {
      closeMode = 1;
    }
    /* attenuator mode */
    else if(!strcmp(argv[2], "-a"))
    {
      attnMode = 1;
    }
    else
    {
      printUsage(argv[0]);    
      exit(0);
    }      
  }
    
  /* status mode */
  if(statusMode)
  {
    rc = ioctl(fd, PICKERINGMUX_STATUS, status);  
    if(rc != 0)
    {
      perror("ioctl(status)");
      exit(-1);
    }   
    printStatus(status, maxOutput);
  }
  
  /* reset mode */
  if(resetMode)
  {
    rc = ioctl(fd, PICKERINGMUX_RESET, status);  
    if(rc != 0)
    {
      perror("ioctl(status)");
      exit(-1);
    }   
    printStatus(status, maxOutput);
  }
  
  /* attenuator mode */
  if(attnMode)
  {
    int channel;
    int attn;
    PickeringAttnData data;
  
    rc = sscanf(argv[3], "%d\n", &channel);
    if(rc != 1) /* We should decode just one character */
    {
      printf("Bad Argument (%d): %s\nUsage: %s -a <channel number> <attenuation>\n", rc, argv[3], argv[0]);
      exit(0);
    }  
    rc = sscanf(argv[4], "%d\n", &attn);
    if(rc != 1) /* We should decode just one character */
    {
      printf("Bad Argument (%d): %s\nUsage: %s -a <channel number> <attenuation>\n", rc, argv[4], argv[0]);
      exit(0);
    }
    data.channel = channel;
    data.attn = attn;
    rc = ioctl(fd, PICKERING_SET_ATTN, &data);
    if(rc != 0)
    {
      perror("ioctl(attn)");
      exit(-1);
    }
      
    rc = ioctl(fd, PICKERINGMUX_STATUS, status);  
    if(rc != 0)
    {
      perror("ioctl(status)");
      exit(-1);
    }   
    printStatus(status, maxOutput);
  }
 
  /* test mode */
  if(testMode)
  {
    for(out = 1; out <= maxOutput; out++)
    {
      for(in = 1; in <= maxInput; in++)
      {
        printf("Press Enter to route input %d to output %d or q to quit\n", in, out);
        c = waitUserInput();
        if(c == 'q')
        {
          close(fd);
          free(status);
          return 0;
        }
        connectData.input = in;
        connectData.output = out;
        
        rc = ioctl(fd, PICKERINGMUX_CONNECT, &connectData);  
        if(rc != 0)
        {
          perror("ioctl(connect)");
          exit(-1);
        }      
        
        rc = ioctl(fd, PICKERINGMUX_STATUS, status);  
        if(rc != 0)
        {
          perror("ioctl(status)");
          exit(-1);
        }   
        printStatus(status, maxOutput);
        
        printf("Press Enter to disconnect input %d from output %d\n", in, out);
        c = waitUserInput();
  
        rc = ioctl(fd, PICKERINGMUX_DISCONNECT, &out);  
        if(rc != 0)
        {
          perror("ioctl(disconnect)");
          exit(-1);
        }      
      }
    }
  }
  
  /* open mode */
  if(openMode)
  {
    if(argc == 4)
    {
      int output;
      /* Decoding the output no */
      rc = sscanf(argv[3], "%d\n", &output);
      if(rc != 1) /* We should decode just one character */
      {
        printf("Bad Argument (%d): %s\nUsage: %s -o <output>\n", rc, argv[3], argv[0]);
        exit(0);
      }
      /* opening the output */
      rc = ioctl(fd, PICKERINGMUX_DISCONNECT, &output);  
      if(rc != 0)
      {
          perror("ioctl(disconnect)");     
      }
      /* updating the status */
      rc = ioctl(fd, PICKERINGMUX_STATUS, status);  
      if(rc != 0)
      {
        perror("ioctl(status)");
      }   
      printStatus(status, maxOutput);
    }
  }
  
  /* close mode */
  if(closeMode)
  {
    if(argc == 5)
    {
      int input;
      int output;
      /* Decoding the input no */
      rc = sscanf(argv[3], "%d\n", &input);
      if(rc != 1) /* We should decode just one character */
      {
        printf("Bad Argument (%d): %s\n", rc, argv[3]);
        printUsage(argv[0]);
        exit(0);
      }
      /* Decoding the output no */
      rc = sscanf(argv[4], "%d\n", &output);
      if(rc != 1) /* We should decode just one character */
      {
        printf("Bad Argument (%d): %s\n", rc, argv[4]);
        printUsage(argv[0]);
        exit(0);
      }
      /* closing the input on the output */
      connectData.input = input;
      connectData.output = output;        
      rc = ioctl(fd, PICKERINGMUX_CONNECT, &connectData);  
      if(rc != 0)
      {
        perror("ioctl(connect)");     
      }
      /* updating the status */
      rc = ioctl(fd, PICKERINGMUX_STATUS, status);  
      if(rc != 0)
      {
        perror("ioctl(status)");
      }   
      printStatus(status, maxOutput);
    }
  }
  
  /* Clean up */
  printf("Bye !\n");
  close(fd);
  free(status);
  return 0;
}

/******************************************************************************
 *
 */
void doOpen()
{
  
}

/******************************************************************************
 *
 */
static void printUsage(char *basename)
{
  printf("Usage: %s <slot number> [option]\n\nOption\n======\n", basename);
  printf("the -t  option goes through the matrix\n");
  printf("the -ta option goes through the matrix automatically\n");
  printf("the -s  option gives the status of the module\n");
  printf("the -r  option resets the module\n");
  printf("the -c  <input> <output> option closes the <input> on <output>\n");
  printf("the -o  <output> option opens the <output>\n");
  printf("the -a  <ch> <attn> option sets the <attn> attenuation on channel <ch>\n");
}

/******************************************************************************
 *
 */
static void printStatus(int *status, int size)
{
  int i;
  
  printf("Status:\n");
  for(i = 0; i < size; i++)
  {
    printf("%d  ", status[i]);
    if(((i + 1) % 4) == 0)
    {
      printf("\n");
    }
  }
  printf("\n");
}

/******************************************************************************
 *
 */
static char waitUserInput()
{
  if(tempoMode)
  {
    sleep(1);
    return '\n';
  }
  else
  {
    return getchar();
  }
}

/******************************************************************************
 *
 */
void dumpMemory(void *addr, int size)
{
    int i, ligne = 0, pos;
    char decod[20], curByte, txt[100];
    
    printf("\n>>>>>>>> Memory Dump\n\n");
    while(size > ligne * 16)
    {
        memset(decod, 0, sizeof(decod));
        memset(txt, 0, sizeof(txt));
        pos = 0;
        
        pos = sprintf(txt, "%p : ", addr);
        for(i = 0; i < 16; i++)
        {
            if((ligne * 16) + i < size)
            {
                curByte = ((char*)addr)[i];
                pos += sprintf(txt + pos, "%.2X ",(unsigned char)curByte);
                if( 0x1F < curByte && curByte < 0x7F)
                    decod[i] = curByte;
                else
                    decod[i] = '.'; 
            }
            else
                pos += sprintf(txt + pos, "?? "); 
                
            if(!((i+1)% 4))
                pos += sprintf(txt + pos, " ");
        }
        pos += sprintf(txt + pos, "|%s\n", decod);
        printf("%s\n", txt);
        ligne++;
        addr = (char *)addr + 16;
    }
    printf("\n");
}
