/*******************************************************************************
*  Operation system dependent types and functions or definitions header file.
*
*  Copyright (C) 1988 - 2011, Pickering Interfaces ltd.
*  
*  Author: Michal Heczko
*  Contact: support@pickeringswitch.com
*  
*******************************************************************************/

#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

/* Fundamental system types for CB & SB using */
typedef char				  	ps8;      	// -128            -> 127
typedef unsigned char         	pu8;      	//  0              -> 255
typedef signed short int      	ps16;     	// -32 768         -> 32767
typedef unsigned short int    	pu16;     	//  0              -> 65535
typedef signed long int       	ps32; 	  	//  -2 147 483 648 -> 2 147 483 647
typedef unsigned long int     	pu32;		//  0              -> 4 294 967 295
typedef signed long long      	ps64;        
typedef unsigned long long    	pu64;
typedef float                 	pfloat;		// 3.4e+/-38   ( 7 digits)
typedef double                	pdouble;	// 1.7e +/-308 (15 digits)
typedef long double           	pldouble;   // 1.7e +/-308 (15 digits)

#ifdef STRONGARM
typedef long			              pbool;
#else
typedef char                    pbool;
#endif

/* Exteded set of basic types */
typedef char		         	      pchar; 		  //generic pchar, can be signed or unsigned
typedef ps32					      pwbool;

typedef void* handle_t;

#define RAW_BYTE_SIZE			sizeof(pu8)
#define INT_SIZE		  		sizeof(ps32)
#define CHAR_SIZE               sizeof(pchar)
#define	BOOL_SIZE         		sizeof(pbool)
#define	WBOOL_SIZE        		sizeof(pwbool)
#define WORD_SIZE         		sizeof(pu16)
#define DWORD_SIZE        		sizeof(pu32)
#define INT64_SIZE        		sizeof(ps64)
#define UINT64_SIZE       		sizeof(pu64)
#define FLOAT_SIZE        		sizeof(pfloat)
#define DOUBLE_SIZE       		sizeof(pdouble)
#define LLDOUBLE_SIZE     		sizeof(pldouble)
#define VOID_SIZE         		sizeof(void)
#define PVOID_SIZE        		sizeof(void*)

/* Definitions for all OS */

#define SOCKET_BUFFER_SIZE 2048
#define MAX_STRING_LENGTH 1024//(SOCKET_BUFFER_SIZE / 2)
#define DEFAULT_LISTEN_BACKLOG 100
#define MESH_HEADER_SIZE 2 * INT_SIZE
#define BROADCAST_ADDRESS "255.255.255.255"
#define HOSTNAME_MAX_LEN 256

/* Definitions for Windows OS*/

#if defined(WIN32) || defined(WIN64)

#ifdef LABVIEWRT /* Definitions for LabView RT OS base on WinAPI */

#include <windows.h>
#include <visa.h>        

typedef ViSession socket_t;  

#else /* Definitions for Windows OS*/

#include <winsock2.h>

typedef SOCKET socket_t;

typedef struct _ipinfo {
    pu8 ttl;
    pu8 tos;
    pu8 flags;
    pu8 size;
    pu8 *data;
} ipinfo_t;

typedef struct _icmpecho {
    unsigned long addr;
    unsigned long status;
    unsigned long rtt;
    unsigned short size;
    unsigned short reserved;
    void* data;
    struct _ipinfo opt;
} icmpecho_t;

#define snprintf sprintf_s

#define SLEEPEX(time,alterTable) (SleepEx((time),(alterTable)))

#endif // LABVIEWRT

typedef CRITICAL_SECTION safety_lock_t;
typedef HMODULE module_t;  
typedef int socklen_t;

#define getTickCount ::GetTickCount

#define CREATE_SAFETY_LOCK(safetyLock) {\
    (safetyLock) = new safety_lock_t;\
    ::InitializeCriticalSection((safetyLock));\
}

#define DESTROY_SAFETY_LOCK(safetyLock) {\
    ::DeleteCriticalSection((safetyLock));\
    delete (safetyLock);\
}

#define LOCK_SAFETY_LOCK(safetyLock) {\
    ::EnterCriticalSection((safetyLock));\
}

#define UNLOCK_SAFETY_LOCK(safetyLock) {\
    ::LeaveCriticalSection((safetyLock));\
}  

#define LOADLIB(libName) (LoadLibrary((libName)))

#define FREELIB(libHandle) {\
    if(libHandle != NULL) {\
    ::FreeLibrary((libHandle));\
    }\
}

#define GETLIBPROC(func,funcType,module,funcName)\
{\
    (func) = (funcType)::GetProcAddress((module),(funcName));\
}

#define LOADSTRING(object,code,buffer,size) LoadString((HINSTANCE)(object),(code),(buffer),(size));

#define PROCESS_ID GetCurrentThreadId()  

#endif // WIN32

/* Definitions for Linux OS*/

#ifdef LINUX

#include <pthread.h>
#include <dlfcn.h>
#include <unistd.h>

typedef pthread_mutex_t safety_lock_t;
typedef void* module_t;
typedef int socket_t;

#ifndef INVALID_SOCKET
#define INVALID_SOCKET  (socket_t)(~0)
#endif

#ifndef SOCKET_ERROR
#define SOCKET_ERROR (-1)
#endif

#define min(a,b)((a) > (b) ? (b) : (a))
#define ioctlsocket ioctl
#define NO_ERROR 			0
#define TRUE          1
#define FALSE         0
#define __stdcall
#define _stdcall
#define DLL_PROCESS_ATTACH   1
#define DLL_THREAD_ATTACH    2
#define DLL_THREAD_DETACH    3
#define DLL_PROCESS_DETACH   0
#define DLL_PROCESS_VERIFIER 4
#define APIENTRY
#define RR_CACHE_SIZE 500  

#define _SH_DENYWR 0x20

#define closesocket close

#define vsnprintf_s vsnprintf

#define CREATE_SAFETY_LOCK(safetyLock) {\
    pthread_mutexattr_t attr;\
    (safetyLock) = new safety_lock_t;\
    ::pthread_mutexattr_init(&attr);\
    ::pthread_mutexattr_settype(&attr,PTHREAD_MUTEX_RECURSIVE);\
    ::pthread_mutex_init((safetyLock),&attr);\
    ::pthread_mutexattr_destroy(&attr);\
}

#define DESTROY_SAFETY_LOCK(safetyLock) {\
    ::pthread_mutex_destroy((safetyLock));\
    delete (safetyLock);\
}

#define LOCK_SAFETY_LOCK(safetyLock) {\
    ::pthread_mutex_lock((safetyLock));\
}

#define UNLOCK_SAFETY_LOCK(safetyLock) {\
    ::pthread_mutex_unlock((safetyLock));\
}

#define fopen_s(handle,file,attr) (*(handle) = fopen(file,attr))

#define ctime_s(timeStr,size,time) {\
    pchar *temp;\
    temp = ::ctime((time));\
    ::strcpy((timeStr),temp);\
    delete temp;\
}

#define gmtime_s(time_s,time_64) {\
struct tm *time;\
    time = ::gmtime((time_64));\
    ::memcpy((time_s),time,sizeof(time));\
    delete time;\
}

#define strcpy_s(dest,size,src) (::strcpy((dest),(src)))

#define strcat_s(dest,size,src) (::strcat((dest),(src)))  

#define LOADLIB(libName) (::dlopen((libName),RTLD_LOCAL | RTLD_LAZY))

#define FREELIB(libHandle) {\
    if(libHandle != NULL) {\
    ::dlclose((libHandle));\
    }\
}

#define GETLIBPROC(func,funcType,module,funcName)\
{\
    (func) = (funcType)dlsym((module),(funcName));\
}

#define LOADSTRING(object,code,buffer,size)\
{\
    map<int,const pchar*>::iterator item;\
    if((item = (object)->find((code))) != (object)->end()) {\
    strcpy_s((buffer),(size),item->second);\
	(buffer)[(size)-1] = 0;\
    } else {\
    (buffer)[0] = 0;\
    }\
}

#define PROCESS_ID ::getpid()

//#define SLEEPEX(time,alterTable) (::sched_yield())
#define SLEEPEX(time,alterTable) (::usleep((time) * 1000))

unsigned short checksum(pu8 *addr,int count);
pu32 getTickCount();
pchar *_strupr_s(pchar *str,int length);
pu32 GetLastError();
int sprintf_s(pchar *buffer,int size,const pchar *format,...);

#endif // LINUX

typedef pu32(__stdcall convert_func_t)(pu32);

/*#if((_MSC_VER < 1400) || (LINUX > 0))

#define strcpy_s(strDestination,sizeInBytes,strSource) (::strcpy((strDestination),(strSource)))
#define sprintf_s(buffer,sizeOfBuffer,format,...) {\
va_list Args;\
::va_start(Args,(format));\
::sprintf((buffer),(format),va_pass(Args);\
::va_end(Args);\
}
#define fopen_s(file,filename,mode) ((file) = ::fopen((filename),(mode)))

#endif // #if((_MSC_VER < 1400) || (LINUX > 0))*/

#endif // #ifndef _SYSTEM_H_
