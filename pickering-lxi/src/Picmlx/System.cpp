#include <System.h>
#include <sys/time.h>
#include <ctype.h>
#include <errno.h>
#include <stdio.h>

unsigned short checksum(pu8 *addr,int count) {
    register long sum = 0;

    // Inner loop
    while(count > 1) {
        sum += *((unsigned short*)addr++);
        count -= 2;
    }
    // Add left-over byte, if any
    if(count > 0) {
        sum += *((pu8*)addr);
    }
    // Fold 32-bit sum to 16 bits
    while(sum >> 16) {
        sum = (sum & 0xffff) + (sum >> 16);
    }
    /*sum = (sum & 0xffff) + (sum >> 16);
    sum += (sum >> 16);*/
    return (unsigned short)~sum;
}

pu32 getTickCount() {  
    struct timeval tv;

    gettimeofday(&tv,NULL);
    // this will rollover ~ every 49.7 days
    return (pu32)(tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

pchar *_strupr_s(pchar *str,int length) {
    while(*str) {
        *str = toupper((pu8)*str);
        str++;
    }
    return str;
}

pu32 GetLastError() {
    return errno;
}

int sprintf_s(pchar *buffer,int size,const pchar *format,...) {
    va_list args;
    int result;

    ::va_start(args,format);
    result = ::vsnprintf(buffer,size,format,args);
    ::va_end(args);
    return result;
}
