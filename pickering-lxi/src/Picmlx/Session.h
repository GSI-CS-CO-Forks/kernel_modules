/*******************************************************************************
*  Session definition header file.
*
*  Copyright (C) 1988 - 2011 Pickering Interfaces Ltd.
*  
*  Author: Michal Heczko
*  Contact: support@pickeringswitch.com
*  
*******************************************************************************/

#ifndef _SESSION_H_
#define _SESSION_H_

#include <System.h>
#include <BaseList.h>
#include <Sockets.h>
#include <Picmlx.h>

// Predefined classes
class occupant_list_c;

// Session occupant informations
typedef struct _occupant_info {
    ps32 id;
    ps32 lastError;
} occupant_info_s;

// Session informations
typedef struct _session_info {
    session_t sid;
    pchar *addr;
    ps32 addr_len;  
    ps32 port;
    pbool connected;
    pbool remote;  
} session_info_s;

typedef struct _session_data {
    pbool remote;
    pbool connected;
    ps32 lastError;
} session_data_s;

// Session work entry
typedef struct _session_entry {
    session_info_s info;
    socket_client_c *socket;
    occupant_list_c *occupants;
} session_entry_s;

// Session occupants info list
class occupant_list_c : public base_list_c {
public:  
    occupant_list_c(pbool dd) : base_list_c(dd) {}
    //~occupant_list_c();
    occupant_info_s *getOccupant(ps32 id);
    void insert(occupant_info_s *data);
    pbool remove(ps32 id);
};

// Sessions info list
class session_list_c : public base_list_c {
public:
    session_list_c(pbool dd) : base_list_c(dd) {}
    //~session_list_c();
    session_entry_s *getSession(session_t sid);
    void insert(session_entry_s *data);
    pbool remove(session_t sid);
};

#endif // #ifdef _SESSION_H_
