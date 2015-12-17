/*******************************************************************************
*  Abstract communication socket, client and server socket header file.
*
*  Copyright (C) 1988 - 2011 Pickering Interfaces Ltd.
*  
*  Author: Michal Heczko
*  Contact: support@pickeringswitch.com
*  
*******************************************************************************/

#ifndef _SOCKETS_H_
#define _SOCKETS_H_

#include <System.h>

// Sockety type enumerator
typedef enum {
    SOCKET_TCP,
    SOCKET_UDP,
    SOCKET_RAW,
    SOCKET_VISA    // Reserved for LabView RT system
} socket_type_e;

class socket_client_c;
class socket_server_c;

/*
* Common socket class for all other client or server sockets
*/
class socket_c {
protected:
    socket_t handle;
    pchar *buffer;
    pu32 bufferSize;
    ps32 offset;
    ps32 recvBytes;
    pbool connected;
	pu32 connectResult;
    pbool created;
protected:  
    // Common functions for client and server sockets
    virtual pbool disconnect() { return false; }
    virtual void getHostName(pchar *buffer,pu32 bufferSize) {}
    virtual void getPeerName(pchar *buffer,pu32 bufferSize) {}
    virtual void getLocalAddress(pchar *buffer,pu32 bufferSize) {}
    virtual pbool getRecvTimeout(pu32 *timeout) { return false; }
    virtual pbool recv() { return false; }
    virtual pbool send() { return false; }
    virtual pbool setRecvTimeout(pu32 timeout) { return false; }
public:
    socket_c(pu32 bufferSize);
    virtual ~socket_c();
    // Functions for read from internal socket buffer
    void readArray(void *buffer,pu32 itemCount,pu32 itemSize);
    pu32 readArray(void *buffer,pu32 *itemCount,pu32 itemSize);
    pchar readChar();
    pdouble readDouble();
    pfloat readFloat();
    ps32 readInt32();
    ps64 readInt64();
    void readString(pchar *buffer,pu32 length);
    pu32 readUInt32();
    pu64 readUInt64();
    // Functions for write to internal socket buffer
    void writeArray(void *array,pu32 itemCount,pu32 itemSize);
    void writeChar(pchar value);
    void writeDouble(pdouble value);
    void writeFloat(pfloat value);
    void writeInt32(ps32 value);
    void writeInt64(ps64 value);
    void writeString(pchar *buffer,pu32 length);
    void writeUInt32(pu32 value);
    void writeUInt64(pu64 value);
#if defined(WIN32) || defined(WIN64)
	int WSAErrorToBSD(int errorCode);
#endif
	pbool setBlockingMode(pbool enable);
	pu32 GetConnectResult();
    // Socket support functions
    pchar *getBuffer() { return buffer; }
    pu32 getBufferSize() { return bufferSize; }
    socket_t getHandle() { return handle; }
    pu32 getOffset() { return offset; }
    pu32 getRecvBytesCount() { return recvBytes; }  
    pbool isCreated() { return created; } /// connected
    void setOffset(pu32 offset) { this->offset = offset; }
};

/*
* Client socket class
*/

class socket_client_c : public socket_c {
private:
    socket_type_e type;
public:
    socket_client_c(socket_type_e type,pu32 bufferSize);
    socket_client_c(pu32 board,socket_type_e type,pu32 bufferSize,pbool intf);
    socket_client_c(socket_type_e type,pu32 bufferSize,socket_t handle,pbool accepted);
    virtual ~socket_client_c();
    pbool connect(pchar *address,pu32 port,pu32 /* options (reserved) */ recvTimeout);
    pbool disconnect();
    void getHostName(pchar *buffer,pu32 bufferSize);
    void getPeerName(pchar *buffer,pu32 bufferSize);
    void getLocalAddress(pchar *buffer,pu32 bufferSize);
    pbool getRecvTimeout(pu32 *timeout);
    pbool getSendTimeout(pu32 *timeout);
    pbool recv();
    pbool send();  
    pbool setRecvTimeout(pu32 timeout);
    pbool setSendTimeout(pu32 timeout);

#if (defined(WIN32) || defined(LINUX)) && !defined(LABVIEWRT)
    pbool recvFrom(ps32 flags,pchar *address,pu32 *port);
    pbool sendTo(ps32 flags,const pchar *address,pu32 port);
    pbool setBroadcastMode(pbool enable);
#endif // #if (defined(WIN32) || defined(LINUX)) && !defined(LABVIEWRT)

};

#if (defined(WIN32) || defined(LINUX)) && !defined(LABVIEWRT)

class socket_server_c : public socket_c {
private:
    socket_type_e type;
    pbool listenning;
public:
    socket_server_c(socket_type_e type,pu32 bufferSize);
    virtual ~socket_server_c();
    socket_client_c *accept();
    pbool bind(pu32 port);
    pbool disconnect();
    pbool listen(pu32 port,pu32 backLog);
    pbool setBlockingMode(pbool enable);
};

#endif // #if (defined(WIN32) || defined(LINUX)) && !defined(LABVIEWRT)

#endif // #ifndef _SOCKETS_H_
