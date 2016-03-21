/*******************************************************************************
*  Base two side list class with basic function for operate with list data.
*
*  Copyright (C) 1988 - 2011 Pickering Interfaces Ltd.
*  
*  Author: Michal Heczko
*  Contact: support@pickeringswitch.com
*   
*******************************************************************************/

#ifndef _BASE_LIST_H_
#define _BASE_LIST_H_

#include <System.h>
#include <stdio.h>
#include <memory.h>

// List data entry
typedef struct _data_entry {
    void *data;         // 
    long size;          //
    _data_entry *next;  //
    _data_entry *prev;  //
} data_entry_s;

// Base two side list class with basic functions.
class base_list_c {
private:
    data_entry_s *root;
    long count;
    pbool deleteData;
protected:
    void clear();  
    void *getData(long index);
    pbool insert(void *data,long size);
    pbool remove(long index);
    pbool remove(void *data);
    void *search(void *data);
public:
    base_list_c(pbool dd) { root = NULL; count = 0; deleteData = dd; }
    ~base_list_c() { clear(); }
    long getCount() { return this->count; }
};

#endif // #ifdef _BASE_LIST_H_
