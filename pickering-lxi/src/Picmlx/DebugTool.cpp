#if defined(DEBUG) || defined(_DEBUG)

#include <DebugTool.h>
#include <System.h>
#include <stdio.h>

#ifndef LINUX
#include <share.h>
#endif // LINUX

FILE *debugFile;

void PICMLX_API __DEBUGMSG(pbool includeSysError,pchar *message,...) {
    va_list args;
    pchar msgStr[512] = {0};
    pchar msgStr2[512] = {0};

    va_start(args,message);
    if(includeSysError) {
        vsnprintf_s(msgStr,sizeof(msgStr) - 1,message,args);
        snprintf(msgStr2,sizeof(msgStr2) - 1,"%s, LastSysError: %d",msgStr,GetLastError());
    } else {    
        vsnprintf_s(msgStr2,sizeof(msgStr2) - 1,message,args);
    }
    if((message[0] == ':') && (message[1] == ':'))
        fprintf(debugFile,"  %s\n",msgStr2);
    else
        fprintf(debugFile,"%s\n",msgStr2);
    fflush(debugFile);
    va_end(args);
}

void PICMLX_API __DEBUGMSGHEX(pchar *message,pchar *format,pchar *buffer,pu32 size) {
    pchar msgStr[10] = {0};
    pchar msgStr2[SOCKET_BUFFER_SIZE] = {0};
    pchar msgStr3[SOCKET_BUFFER_SIZE] = {0};

    for(pu32 i = 0; i < size; i++) {
        snprintf(msgStr,sizeof(msgStr) - 1,format,*((pu8*)(buffer + i)));
        strcat_s(msgStr3,sizeof(msgStr3),msgStr);
    }
    snprintf(msgStr2,sizeof(msgStr2) - 1,message,msgStr3);
    if((message[0] == ':') && (message[1] == ':'))
        fprintf(debugFile,"  %s\n",msgStr2);
    else
        fprintf(debugFile,"%s\n",msgStr2);
    fflush(debugFile);
}

/*
 *
 */
void __DEBUG_INIT() {
#ifdef DEBUGFILE
#ifdef WIN32
    debugFile = _fsopen(DEBUGFILE,"w+",_SH_DENYWR);
#else
    fopen_s(&debugFile, "/var/logs/clientbridge.log", "w+");
#endif
#else
    debugFile = stderr;
#endif
    if(debugFile) {
        fprintf(debugFile,"Debug initiated.\n");
    }
    fflush(debugFile);
}

void __DEBUG_FREE() {
    if(debugFile) {
        fprintf(debugFile,"Debug closed.\n");
    }
    fflush(debugFile);
#ifdef DEBUGFILE    
    fclose(debugFile);  
#endif
}

#endif
