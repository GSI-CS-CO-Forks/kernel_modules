/*******************************************************************************
*  Critical section header file.
*
*  Copyright (C) 1988 - 2011 Pickering Interfaces Ltd.
*  
*  Author: Michal Heczko
*  Contact: support@pickeringswitch.com
*  
*******************************************************************************/

#ifndef _SAFETY_LOCK_H_
#define _SAFETY_LOCK_H_

#include <System.h>
#include <DebugTool.h>

class safety_lock_c {
private:
    safety_lock_t *handle;
    pu32 lockCount;
public:
    safety_lock_c() { CREATE_SAFETY_LOCK(handle); lockCount = 0; };
    ~safety_lock_c() { DESTROY_SAFETY_LOCK(handle); };  
    void lock() {
        lockCount++;
        /* DEBUG RECORD BEGIN */
#if defined(DEBUG) || defined(_DEBUG)
        __DEBUGMSG(FALSE,"safety_lock_c::lock - trying lock");
#endif
        /* DEBUG RECORD END */
        LOCK_SAFETY_LOCK(handle);
        /* DEBUG RECORD BEGIN */
#if defined(DEBUG) || defined(_DEBUG)
        __DEBUGMSG(FALSE,"safety_lock_c::lockCount: %d",lockCount);
        __DEBUGMSG(FALSE,"safety_lock_c::locked");
#endif
        /* DEBUG RECORD END */
    };  
    void unlock() {
        lockCount--;
        /* DEBUG RECORD BEGIN */
#if defined(DEBUG) || defined(_DEBUG)
        __DEBUGMSG(FALSE,"safety_lock_c::trying unlock");      
#endif
        /* DEBUG RECORD END */
        UNLOCK_SAFETY_LOCK(handle);
        /* DEBUG RECORD BEGIN */
#if defined(DEBUG) || defined(_DEBUG)
        __DEBUGMSG(FALSE,"safety_lock_c::lockCount: %d",lockCount);
        __DEBUGMSG(FALSE,"safety_lock_c::unlock - unlocked");
#endif
        /* DEBUG RECORD END */
    };  
};

#endif // #ifndef _SAFETY_LOCK_H_
