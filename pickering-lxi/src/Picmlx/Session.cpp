#include <Session.h>

// occupant_list_c class functions

/*occupant_list_c::~occupant_list_c() {
base_list_c::~base_list_c();
}*/

occupant_info_s *occupant_list_c::getOccupant(ps32 id) {
    occupant_info_s *item;
    long index;

    for(index = 0; index < getCount(); index++) {
        item = (occupant_info_s *)base_list_c::getData(index);
        if(item->id == id) {
            return item;
        }
    }
    return NULL;
}

void occupant_list_c::insert(occupant_info_s *data) {  
    base_list_c::insert((void*)data,sizeof(occupant_info_s));
}

pbool occupant_list_c::remove(ps32 id) {
    occupant_info_s *item;

    if((item = getOccupant(id)) != NULL) {    
        base_list_c::remove(item);
        return TRUE;
    }
    return FALSE;
}

// session_list_c class functions

/*session_list_c::~session_list_c() {
base_list_c::~base_list_c();
}*/

session_entry_s *session_list_c::getSession(session_t sid) {
    session_entry_s *item;
    long index;

    for(index = 0; index < getCount(); index++) {
        item = (session_entry_s *)base_list_c::getData(index);
        if(item->info.sid == sid) {
            return item;
        }
    }
    return NULL;
}

void session_list_c::insert(session_entry_s *data) {
    base_list_c::insert((void*)data,sizeof(session_entry_s));
}

pbool session_list_c::remove(session_t sid) {
    session_entry_s *item;

    if((item = getSession(sid)) != NULL) {    
        base_list_c::remove(item);
        return TRUE;
    }
    return FALSE;
}
