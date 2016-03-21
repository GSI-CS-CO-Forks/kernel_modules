/*******************************************************************************
*  Discovery support header file.
*
*  Copyright (C) 1988 - 2011, Pickering Interfaces ltd.
*  
*  Author: Michal Heczko
*  Contact: support@pickeringswitch.com
*  
*******************************************************************************/

#ifndef _DISCOVERY_H_
#define _DISCOVERY_H_

#include <System.h>
#include <BaseList.h>
#include <Picmlx.h>

class device_list_c : public base_list_c {
public:
    device_list_c() : base_list_c(TRUE) {}
    void clear() { base_list_c::clear(); }
    device_entry_t *getDevice(pu32 index) { return (device_entry_t*)base_list_c::getData(index); }
    void insert(device_entry_t *device) { base_list_c::insert(device,sizeof(device_entry_t)); }
    device_entry_t *search(device_entry_t *data) { return (device_entry_t*)base_list_c::search(data); }
};

#endif // #ifdef _DISCOVERY_H_
