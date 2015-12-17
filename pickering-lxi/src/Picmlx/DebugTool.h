/*******************************************************************************
*  Debug tool module header file.
*
*  Copyright (C) 1988 - 2011, Pickering Interfaces ltd.
*  
*  Author: Michal Heczko
*  Contact: support@pickeringswitch.com
*  
*******************************************************************************/

#ifndef _DEBUGTOOL_H_
#define _DEBUGTOOL_H_

#if defined(DEBUG) || defined(_DEBUG)

#include <System.h>
#include <Picmlx.h>

#ifdef __cplusplus
extern "C" {
#endif

void PICMLX_API __DEBUGMSG(pbool includeSysError,pchar *message,...);

void PICMLX_API __DEBUGMSGHEX(pchar *message,pchar *format,pchar *buffer,pu32 size);

/*
void PICMLX_API __DEBUG_MEM_USAGE();
*/

void __DEBUG_INIT();

void __DEBUG_FREE();

#ifdef __cplusplus
};
#endif

#endif

#endif // #ifndef _DEBUGTOOL_H_
