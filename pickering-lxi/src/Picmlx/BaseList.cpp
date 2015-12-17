#include <BaseList.h>
#include <DebugTool.h>

/* Private class functions */

void base_list_c::clear() {
    data_entry_s *temp;

    while(root != NULL) {
        temp = (data_entry_s*)root;
        root = ((data_entry_s*)root)->next;
        if(deleteData) {
            /* DEBUG RECORD BEGIN */
#if defined(DEBUG) || defined(_DEBUG)
            __DEBUGMSG(FALSE,"::base_list_c::clear - deleting data (size: %d)",temp->size);
#endif    
            /* DEBUG RECORD END */
            /*#ifdef LINUX
            delete temp->data;
            delete temp;
            #else
            delete[temp->size] temp->data;
            delete[sizeof(data_entry_s*)] temp;
            #endif*/
            delete (pchar *)temp->data;
            delete temp;
        }
    }
    count = 0;
}

void *base_list_c::getData(long index) {
    data_entry_s *temp;
    long i;

    if(!count || (index < 0) || (index >= count)) {
        return NULL;
    }
    temp = (data_entry_s*)root;
    i = 0;
    while((i < index) && (temp != NULL)) {
        temp = temp->next;
        i++;
    }
    return temp->data;
}

pbool base_list_c::insert(void *data,long size) {
    data_entry_s *node = new data_entry_s;
    if(node == NULL) {
        return false;
    }
    /* DEBUG RECORD BEGIN */
#if defined(DEBUG) || defined(_DEBUG)
    __DEBUGMSG(FALSE,"::base_list_c::insert - inserting data (size: %d)",size);
#endif    
    /* DEBUG RECORD END */
    //node->data = new pchar[size];
    node->data = (pchar*)data;
    //memcpy(node->data,data,size);
    node->size = size;
    node->next = (data_entry_s*)root;
    node->prev = NULL;
    if(root != NULL) {
        ((data_entry_s*)root)->prev = node;
    }
    root = node;
    count++;
    return true;
}

pbool base_list_c::remove(long index) {
    data_entry_s *temp;
    long i;

    if(!count || (index < 0) || (index >= count)) {
        return false;
    }
    i = 0;
    temp = (data_entry_s*)root;
    while((i < index) && (temp != NULL)) {
        temp = temp->next;
        i++;
    }
    if(temp == NULL) {
        return false;
    }
    if(temp->prev != NULL) {
        temp->prev->next = temp->next;
    } else {
        root = temp->next;
    }
    if(temp->next != NULL) {
        temp->next->prev = temp->prev;
    }
    if(deleteData) {
        /* DEBUG RECORD BEGIN */
#if defined(DEBUG) || defined(_DEBUG)
        __DEBUGMSG(FALSE,"::base_list_c::remove - deleting data (size: %d)",temp->size);
#endif    
        /* DEBUG RECORD END */
        /*#ifdef LINUX    
        delete temp->data;
        delete temp;
        #else
        delete[temp->size] temp->data;
        delete[sizeof(data_entry_s*)] temp;
        #endif*/
        delete (pchar *)temp->data;
        delete temp;
    }
    count--;
    return true;
}

pbool base_list_c::remove(void *data) {
    data_entry_s *temp;
    long index;

    if(count == 0) {
        return false;
    }
    index = 0;
    temp = (data_entry_s*)root;
    while(temp != NULL) {
        if(!memcmp(data,temp->data,temp->size)) {
            if(temp->prev != NULL) {
                temp->prev->next = temp->next; 
            } else {
                root = temp->next;
            }
            if(temp->next != NULL) {
                temp->next->prev = temp->prev;
            }
            if(deleteData) {
                /* DEBUG RECORD BEGIN */
#if defined(DEBUG) || defined(_DEBUG)
                __DEBUGMSG(FALSE,"::base_list_c::remove - deleting data (size: %d)",temp->size);
#endif    
                /* DEBUG RECORD END */
                /*#ifdef LINUX
                delete temp->data;
                delete temp;
                #else
                delete[temp->size] temp->data;
                delete[sizeof(data_entry_s*)] temp;
                #endif*/
                delete (pchar *)temp->data;
                delete temp;
            }
            count--;
            break;
        }
        temp = temp->next;
        index++;
    } 
    return true;
}

void *base_list_c::search(void *data) {
    data_entry_s *temp;

    if(count == 0) {
        return NULL;
    }
    temp = (data_entry_s*)root;
    while(temp != NULL) {
        if(!memcmp(data,temp->data,temp->size)) {
            break;
        }
        temp = temp->next;
    }
    return temp;
}
