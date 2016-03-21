/*******************************************************************************
*  Base module header file.
*
*  Copyright (C) 1988 - 2011, Pickering Interfaces ltd.
*  
*  Author: Michal Heczko
*  Contact: support@pickeringswitch.com
*  
*******************************************************************************/

#ifndef _BASE_MODULE_H_
#define _BASE_MODULE_H_

#include <System.h>
#include <stdio.h>

#if(defined(LINUX) || defined(LABVIEWRT))
#include <map>
using namespace std;
#endif

class base_module_c {
private:
    module_t handle;
protected:
#if(defined(LINUX) || defined(LABVIEWRT))
    map<int,const pchar*> *errorList;
#endif // #if(defined(LINUX) || defined(LABVIEWRT))
    virtual void loadFunctions(module_t libararyHandle) {}
public:
    base_module_c(const pchar *libName) {
        handle = NULL;
        if(libName != NULL)
            handle = LOADLIB(libName);    
#if(defined(LINUX) || defined(LABVIEWRT))
        errorList = new map<int,const pchar*>;
#endif // #if(defined(LINUX) || defined(LABVIEWRT))
    }
    ~base_module_c() {
#if(defined(LINUX) || defined(LABVIEWRT))
        if(handle != NULL)
            delete errorList;
#endif // #if(defined(LINUX) || defined(LABVIEWRT))
        FREELIB(handle);
    }
    void init() { /*if(handle != NULL) */loadFunctions(handle); }
};

#endif // #ifndef _BASE_MODULE_H_
