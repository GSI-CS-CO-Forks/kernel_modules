#include <Sockets.h>
#include <DebugTool.h>
#include <stdio.h>
#include <errno.h>

#if (defined(WIN32) || defined(WIN64)) && !defined(LABVIEWRT)
#include <winsock2.h>
#include <ws2tcpip.h>
#endif // #ifdef WIN32

#ifdef LINUX
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/ioctl.h>
#include <string.h>  
#include <unistd.h>
#include <fcntl.h>
#define TIMEVAL timeval
#endif // #ifdef LINUX

#ifndef ECONNREFUSED
#define ECONNREFUSED    107
#endif

#ifndef FAR
#define FAR
#endif

#ifdef LABVIEWRT
extern ViPSession __defaultResourceManager;
#endif // #ifdef LABVIEWRT

const int PORT_LENGTH = 6;

// Need synchro in Picmlx.h with PicmlxErrorCodes
enum ConnectErrorCodes {
    // Connext function error codes
    ER_CLI_CONNECT = 0x0000100D,
	ER_CLI_CONNECT_REFUSED,
	ER_CLI_NAME_NOT_KNOWN
};

/*
* Common socket class for all other client or server sockets
*/

socket_c::socket_c(pu32 bufferSize) {
    this->bufferSize = bufferSize;
    buffer = NULL;
    connected = FALSE;
    created = FALSE;
    offset = 0;
    recvBytes = 0;
    handle = INVALID_SOCKET;
}

socket_c::~socket_c() {
    if(created) {
        delete[] buffer;
        created = FALSE;
    }
}

// Functions for read from internal socket buffer

void socket_c::readArray(void *buffer,pu32 itemCount,pu32 itemSize) {
    register pu32 count;

    if(created) {
        count = readUInt32();
        itemSize = min(itemCount,count) * itemSize;     
        ::memcpy(buffer,this->buffer + offset,itemSize);
        offset += itemSize;
    }
}

pu32 socket_c::readArray(void *buffer,pu32 *itemCount,pu32 itemSize)
{
    register pu32 count = 0;

    if(created) {
        count = readUInt32();
        itemSize = min(*itemCount,count) * itemSize;     
        ::memcpy(buffer,this->buffer + offset,itemSize);
        offset += itemSize;
        *itemCount = count;
    }
    return count;
}

pchar socket_c::readChar() {
    pchar value;

    if(created) {
        ::memcpy(&value,buffer + offset,CHAR_SIZE);
        offset += CHAR_SIZE;
        return value;
    }
    return 0;
}

pdouble socket_c::readDouble() {
    pchar netbuf[DOUBLE_SIZE] = {0};

    if(created) {
#ifdef STRONGARM
        ::memcpy(netbuf + DOUBLE_SIZE / 2,buffer + offset,DOUBLE_SIZE / 2);
        ::memcpy(netbuf,buffer + offset + DOUBLE_SIZE / 2,DOUBLE_SIZE / 2);
#else
        ::memcpy(netbuf,buffer + offset,DOUBLE_SIZE);
#endif
        offset += DOUBLE_SIZE;
    }
    return *((double*)netbuf);
}

pfloat socket_c::readFloat() {
    pfloat value;

    if(created) {
        ::memcpy(&value,buffer + offset,FLOAT_SIZE);
        offset += FLOAT_SIZE;
        return value;
    }
    return 0.0;
}

ps32 socket_c::readInt32() {
    ps32 value;

    if(created) {
        ::memcpy(&value,buffer + offset,INT_SIZE);
        offset += INT_SIZE;
        return value;
    }
    return 0;
}

ps64 socket_c::readInt64() {
    ps64 value;

    if(created) {
        ::memcpy(&value,buffer + offset,INT64_SIZE);
        offset += INT64_SIZE;
        return value;
    }
    return 0;
}

void socket_c::readString(pchar *buffer,pu32 length) {
    register pu32 len;

    if(created && (buffer != NULL)) {  
        len = readUInt32();
        length = min(length,len) * CHAR_SIZE;
        ::memcpy(buffer,this->buffer + offset,length);
        offset += length;
    }
}

pu32 socket_c::readUInt32() {
    pu32 value;

    if(created) {
        ::memcpy(&value,buffer + offset,DWORD_SIZE);
        offset += DWORD_SIZE;
        return value;
    }
    return 0;
}

pu64 socket_c::readUInt64() {
    pu64 value;

    if(created) {
        ::memcpy(&value,buffer + offset,UINT64_SIZE);
        offset += UINT64_SIZE;
        return value;
    }
    return 0;
}

// Functions for write from internal socket buffer

void socket_c::writeArray(void *array,pu32 itemCount,pu32 itemSize) {
    register pu32 size;

    if(created) {
        size = itemCount * itemSize;
        writeUInt32(itemCount);
        ::memcpy(buffer + offset,array,size);
        offset += size;
    }
}

void socket_c::writeChar(pchar value) {
    if(created) {
        ::memcpy(buffer + offset,&value,CHAR_SIZE);
        offset += CHAR_SIZE;
    }
}

void socket_c::writeDouble(pdouble value) {
    pchar netbuf[DOUBLE_SIZE] = {0};

    if(created) {
        *((double*)netbuf) = value;
#ifdef STRONGARM
        ::memcpy(buffer + offset,netbuf + DOUBLE_SIZE / 2,DOUBLE_SIZE / 2);
        ::memcpy(buffer + offset + DOUBLE_SIZE / 2,netbuf,DOUBLE_SIZE / 2);
#else
        ::memcpy(buffer + offset,netbuf,DOUBLE_SIZE);
#endif
        offset += DOUBLE_SIZE;
    }
}

void socket_c::writeFloat(pfloat value) {
    if(created) {
        ::memcpy(buffer + offset,&value,FLOAT_SIZE);
        offset += FLOAT_SIZE;
    }
}

void socket_c::writeInt32(ps32 value) {
    if(created) {
        ::memcpy(buffer + offset,&value,INT_SIZE);
        offset += INT_SIZE;
    }
}

void socket_c::writeInt64(ps64 value) {
    if(created) {
        ::memcpy(buffer + offset,&value,INT64_SIZE);
        offset += INT64_SIZE;
    }
}

void socket_c::writeString(pchar *buffer,pu32 length) {
    register pu32 size;

    if(created && (buffer != NULL)) {
        size = min(length,strlen(buffer) + 1) * CHAR_SIZE;
        writeUInt32(size);
        ::memcpy(this->buffer + offset,buffer,size);
        offset += size;
    }
}

void socket_c::writeUInt32(pu32 value) {
    if(created) {
        ::memcpy(buffer + offset,&value,DWORD_SIZE);
        offset += DWORD_SIZE;
    }
}

void socket_c::writeUInt64(pu64 value) {
    if(created) {
        ::memcpy(buffer + offset,&value,UINT64_SIZE);
        offset += UINT64_SIZE;
    }
}

#if defined(WIN32) || defined(WIN64)
int socket_c::WSAErrorToBSD(int errorCode) {
	switch(errorCode) {
		case WSAECONNREFUSED:
			return ECONNREFUSED;
		default:
			return errorCode;
	}

}
#endif

pbool socket_c::setBlockingMode(pbool enable) {
#ifdef LINUX
    int flag;
#endif // #ifdef LINUX
    int mode;

    if (created) {
        mode = !enable;
#if (defined(WIN32) || defined(WIN64)) && !defined(LABVIEWRT)
        return ioctlsocket(handle, FIONBIO, (u_long FAR*) & mode) != SOCKET_ERROR;
#elif LINUX // #ifdef WIN32
        flag = fcntl(handle, F_GETFL, 0);
        if (enable) {
            flag = flag && ~O_NONBLOCK;
        } else {
            flag = flag | O_NONBLOCK;
        }
        return !fcntl(handle, F_SETFL, flag);
#endif // #elif LINUX
    }
    return false;
}

pu32 socket_c::GetConnectResult() {
	return connectResult;
}

/* ##################################################################################################### */

/*
* Client socket class functions
*/

socket_client_c::socket_client_c(socket_type_e type,pu32 bufferSize):socket_c(bufferSize)
{
    this->type = type;
    this->created = this->type == SOCKET_VISA;
    if(this->created) {
        this->bufferSize = bufferSize;
        this->buffer = new pchar[this->bufferSize];
    }
}

socket_client_c::socket_client_c(pu32 board,socket_type_e type,pu32 bufferSize,pbool intf):socket_c(bufferSize) {  
    this->type = type;

#if (defined(WIN32) || defined(LINUX)) && !defined(LABVIEWRT)
    struct addrinfo hints = {0};
    hints.ai_family = AF_INET;
    switch(this->type) {
    case SOCKET_UDP:
        hints.ai_socktype = SOCK_DGRAM;
        hints.ai_protocol = IPPROTO_UDP;
        break;
    case SOCKET_RAW:
        hints.ai_socktype = SOCK_RAW;
        hints.ai_protocol = IPPROTO_RAW;
        break;
    default: // SOCKET_TCP
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;
        break;
    };    
    created = (handle = ::socket(hints.ai_family,hints.ai_socktype,hints.ai_protocol)) != INVALID_SOCKET;
#else // #if (defined(WIN32) || defined(LINUX)) && !defined(LABVIEWRT)
    this->created = this->type == SOCKET_VISA;
#endif // #else

    if(this->created) {
#ifndef LABVIEWRT
        if(intf) {
            char localName[HOSTNAME_MAX_LEN];
            if(!::gethostname(localName,HOSTNAME_MAX_LEN)) {
                struct addrinfo *intfs[10] = {0};
                struct addrinfo *results = NULL;
                struct addrinfo *ptr = NULL;
                pu32 adaptersCount = 0;
                pu32 result = ::getaddrinfo(localName,NULL,&hints,&results);
                /* DEBUG RECORD BEGIN */
#if defined(DEBUG) || defined(_DEBUG)
                __DEBUGMSG(TRUE,"::socket_client_c::getaddrinfo: %d",result);
#endif
                /* DEBUG RECORD END */
                // Discover how many adapters are present if none, than stop creating socket
                for(ptr = results; ptr != NULL; ptr = ptr->ai_next)
                {
                    intfs[adaptersCount] = ptr;
                    /* DEBUG RECORD BEGIN */
#if defined(DEBUG) || defined(_DEBUG)
                    char* tmp = inet_ntoa(((struct sockaddr_in *)intfs[adaptersCount])->sin_addr);
                    //__DEBUGMSG(FALSE,"::socket_client_c::getaddrinfo[%d]: %s",adaptersCount,tmp);
#endif
                    /* DEBUG RECORD END */
                    adaptersCount++;
                }
                if(adaptersCount > 0) {
                    // If board == 0 then let the system decide what is default network device, otherwise select form available list
                    if(board > 0)
                    {
                        if(adaptersCount < (board - 1))
                        {
                            created = false;
                            return;
                        }
                        board--;
                        /* DEBUG RECORD BEGIN */
#if defined(DEBUG) || defined(_DEBUG)
                        __DEBUGMSG(FALSE,"::socket_client_c::board: %d",board);
#endif
                        /* DEBUG RECORD END */
                        if(!(result = bind(handle,intfs[board]->ai_addr,intfs[board]->ai_addrlen)))
                        {
                            /* DEBUG RECORD BEGIN */
#if defined(DEBUG) || defined(_DEBUG)
                            __DEBUGMSG(TRUE,"::socket_client_c::bind: %d",result);
#endif
                            /* DEBUG RECORD END */
                            this->buffer = new pchar[this->bufferSize];
                        }
                        else
                        {
                            /* DEBUG RECORD BEGIN */
#if defined(DEBUG) || defined(_DEBUG)
                            __DEBUGMSG(TRUE,"::socket_client_c::bind: %d",result);
#endif
                            /* DEBUG RECORD END */
                            closesocket(handle);
                            created = false;
                        }
                    }
                    else
                    {
                        this->buffer = new pchar[this->bufferSize];
                    }
                }
                else {
                    closesocket(handle);
                    created = false;
                }
            } else {
                closesocket(handle);
                created = false;
            }    
        } else
            this->buffer = new pchar[this->bufferSize];
#else
        this->bufferSize = bufferSize;
        this->buffer = new pchar[this->bufferSize];
#endif
    }
#if defined(DEBUG) || defined(_DEBUG)
    __DEBUGMSG(FALSE,"::socket_client_c::created: %d",created);
#endif
}

socket_client_c::socket_client_c(socket_type_e type,pu32 bufferSize,socket_t handle,pbool accepted):socket_c(bufferSize) {
    this->type = type;
    this->bufferSize = bufferSize;
    this->handle = handle;
    created = true;
    connected = accepted;
    this->buffer = new pchar[bufferSize];
}

socket_client_c::~socket_client_c() {
    //if(created && connected) {
    disconnect();
    //}
    //socket_c::~socket_c();
}

pbool socket_client_c::connect(pchar *address,pu32 port,pu32 /* options (reserved) */ recvTimeout) {

#ifndef LABVIEWRT
    if ((type == SOCKET_TCP) && created && !connected) {
        TIMEVAL timeout = {0};
        struct addrinfo hints = {0};
        struct addrinfo *res, *rp;
        fd_set write, err;
        socklen_t blen = 0;
        char c_port[PORT_LENGTH] = {0};
        int sfd, s, j, result, flag, val;

        flag = result = val = sfd = s = -1;

        // Socket settings
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_flags = 0;
        hints.ai_protocol = IPPROTO_TCP;
        
        sprintf_s(c_port, PORT_LENGTH, "%d", port);

        // Getting address information
        if ((result = getaddrinfo(address, c_port, &hints, &res)) != 0) {
#if defined(DEBUG) || defined(_DEBUG)
            __DEBUGMSG(TRUE, "::geaddrinfo(): Cannot obtain any info from: %s:%s", address, c_port);
            __DEBUGMSG(TRUE, "::geaddrinfo(): Error: %s", gai_strerror(result));
#endif        
			connectResult = ER_CLI_NAME_NOT_KNOWN;
			return connected = FALSE;
        }

        j = 0;
        for (rp = res; rp != NULL; rp = rp->ai_next, j++) {
            if ((sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol)) == -1) {
#if defined(DEBUG) || defined(_DEBUG)
                __DEBUGMSG(TRUE, "::socket() returned -1 (iter: %d)", j);
#endif            
                continue;
            }
            closesocket(this->handle);
            this->handle = sfd;

#if defined(DEBUG) || defined(_DEBUG)
            __DEBUGMSG(TRUE, "::socket() socketfd: %d (iter: %d)", this->handle, j);
#endif            
            // Setting non-blocking mode for socket fd
            if (!setBlockingMode(false)) {
                connectResult = ER_CLI_CONNECT;
				return connected = FALSE;
            }
            
            if ((result = ::connect(this->handle, rp->ai_addr, rp->ai_addrlen)) == 0) {
                // This should never hapen because sockfd is in non-blocking mode
#if defined(DEBUG) || defined(_DEBUG)
                __DEBUGMSG(TRUE, "::connect() successfull (socketfd: %d)", this->handle);
#endif            
                break; /* Success */
            }
#if defined(DEBUG) || defined(_DEBUG)
            __DEBUGMSG(TRUE, "::connect() not successfull [RC: %d](socketfd: %d)", result, this->handle);
            __DEBUGMSG(TRUE, "::connect() errno: %d (Error: %s)", errno, strerror(errno));
#endif            
        }

        freeaddrinfo(res);

        timeout.tv_sec = recvTimeout / 1000;
        timeout.tv_usec = (timeout.tv_sec == 0) ? timeout.tv_usec = recvTimeout * 1000 : timeout.tv_usec = (recvTimeout % 1000) * 1000;

        if (result != 0) {
            FD_ZERO(&write);
            FD_ZERO(&err);
            FD_SET(handle, &write);
            FD_SET(handle, &err);

#if defined(WIN32) || defined(WIN64)
            errno = WSAGetLastError();
            if (errno != WSAEWOULDBLOCK) {
#else
            if (errno != EINPROGRESS) {
#endif
				connectResult = ER_CLI_CONNECT; //connect failed
				return connected = FALSE; //connect failed
            }
#if defined(DEBUG) || defined(_DEBUG)
            __DEBUGMSG(TRUE, "::waiting for connect: %d", result);
#endif
            // check if the socket is ready
            //select(FD_SETSIZE, NULL, &write, NULL, &timeout);
            result = select(this->handle + 1, NULL, &write, &err, &timeout);

            // Timed out 
            if (result == 0) {
#if defined(DEBUG) || defined(_DEBUG)
                __DEBUGMSG(TRUE, "::select() timed out %s:%s [RC: %d]", address, c_port, result);
#endif
                connectResult = ER_CLI_CONNECT;
				return connected = FALSE;
            }
            // Error ocured 
            else if (result == -1) {
#if defined(DEBUG) || defined(_DEBUG)                
                __DEBUGMSG(TRUE, "::select() finished with error [RC: %d]", result);
                //PDEP
                __DEBUGMSG(TRUE, "::errno: %d (Error: %s)", errno, strerror(errno));
#endif
                connectResult = ER_CLI_CONNECT;
				return connected = FALSE;
            }
            // OK but check the result of connect
            else {
                blen = sizeof(int);
#if (defined(DEBUG) || defined(_DEBUG)) && defined WIN32
                 __DEBUGMSG(TRUE, "::val: %d,", val);
#endif
                if ((result = getsockopt(this->handle, SOL_SOCKET, SO_ERROR, (char *)&val, &blen)) == 0) {
                    if (val != 0) {
#if defined(WIN32) || defined(WIN64)
						errno = WSAErrorToBSD(WSAECONNREFUSED);
#elif defined LINUX
						errno = val;
#endif

		
#if (defined(DEBUG) || defined(_DEBUG)) && defined LINUX
                        __DEBUGMSG(TRUE, "::getsockopt(sfd: %d, opt_level: SOL_SOCKET, opt_name: SO_ERROR) [RC: %d]", this->handle, result);
	                    __DEBUGMSG(TRUE, "::SO_ERROR: %d, Error: %s", val, strerror(val));
#elif (defined(DEBUG) || defined(_DEBUG)) && defined(WIN32) || defined(WIN64) 
                        __DEBUGMSG(TRUE, "::getsockopt(sfd: %d, opt_level: SOL_SOCKET, opt_name: SO_ERROR) [RC: %d]", this->handle, result);
						__DEBUGMSG(TRUE, "::Win32 SO_ERROR: %d, Error: %s", errno, strerror(errno));
#endif                        
                        closesocket(this->handle);
						if( errno == ECONNREFUSED ) { 
							connectResult = ER_CLI_CONNECT_REFUSED;
							return connected = FALSE;
						}
						else { 
							connectResult = ER_CLI_CONNECT;
							return connected = FALSE;
						}
                    }
                    // Success.
#if defined(DEBUG) || defined(_DEBUG)                         
                    __DEBUGMSG(TRUE, "::getsockopt(sfd: %d, opt_level: SOL_SOCKET, opt_name: SO_ERROR) Connect finished with success. [RC: %d]", this->handle, result);
#endif                    

                } else {
#if defined(DEBUG) || defined(_DEBUG)                     
                    __DEBUGMSG(TRUE, "::getsockopt(sfd: %d, opt_level: SOL_SOCKET, opt_name: SO_ERROR) [RC: %d]", result);
                    __DEBUGMSG(TRUE, "::errno: %d, Error: %s", errno, strerror(errno));
#endif                    
                    closesocket(this->handle);
					connectResult = ER_CLI_CONNECT;
					return connected = FALSE;
                }
            }

            if (!FD_ISSET(handle, &write)) {
                connectResult = ER_CLI_CONNECT;
				return connected = FALSE;
			}
        }
#if defined(DEBUG) || defined(_DEBUG)
        __DEBUGMSG(TRUE, "::returning to blocking mode: %d", result);
#endif
        // return to blocking mode - ********* SEE COMMENTS ABOVE
        if (!setBlockingMode(true)) {
            connectResult = ER_CLI_CONNECT;
			return connected = FALSE;
        }

		connectResult = NO_ERROR;
        return connected = TRUE;
    }
#else
    pchar visaRes[128] = {0};

#if defined(DEBUG) || defined(_DEBUG)
    __DEBUGMSG(TRUE, "::socket_client_c::connect: VISA connection: created: %d, connected: %d", created, connected);
#endif
    if (created && !connected) {
        viSetAttribute(*__defaultResourceManager, VI_ATTR_TMO_VALUE, (ViAttrState) recvTimeout);
        sprintf_s(visaRes, sizeof (visaRes), "TCPIP0::%s::%d::SOCKET", address, port);
        connected = !(viOpen(*__defaultResourceManager, (ViRsrc) visaRes, VI_NULL, VI_NULL, &handle) < VI_SUCCESS);
#if defined(DEBUG) || defined(_DEBUG)
        __DEBUGMSG(TRUE, "::viOpen(): %s - connected: %d", visaRes, connected);
#endif
    }
#endif

    return connected;

}

pbool socket_client_c::disconnect() {
    if(created /*&& connected*/) {

#ifndef LABVIEWRT   
        ::shutdown(handle,2);
        ::closesocket(handle);      
#else // #ifndef LABVIEWRT
        viClose(handle);        
#endif // #else

        connected = false;
        //created = false;
        return TRUE;
    }
    return FALSE;
}

void socket_client_c::getHostName(pchar *buffer,pu32 bufferSize) {

#ifndef LABVIEWRT
    pchar hostName[128];

    if(::gethostname(hostName,sizeof(hostName)) != SOCKET_ERROR) {
        struct hostent *phe = ::gethostbyname(hostName);
        if(phe != 0) {
            struct in_addr addr;

            ::memcpy(&addr,phe->h_addr_list[0],sizeof(struct in_addr));
            pchar *address = ::inet_ntoa(addr);
            ::memcpy(buffer,address,bufferSize * CHAR_SIZE);
            //delete address;
        }
    }
#else // #ifndef LABVIEWRT
    if(viGetAttribute(handle,VI_ATTR_TCPIP_ADDR,buffer) < VI_SUCCESS) {
        buffer[0] = 0;
    }
#endif // else

}

void socket_client_c::getPeerName(pchar *buffer,pu32 bufferSize) {
    if(connected) {

#ifndef LABVIEWRT
        sockaddr_in sin;
        socklen_t size = sizeof(sockaddr_in);
        ::getpeername(handle,(sockaddr*)&sin,&size);
        pchar *addr = ::inet_ntoa(sin.sin_addr);
        ::memcpy(buffer,addr,bufferSize * CHAR_SIZE);
        //delete addr;
#else // #ifndef LABVIEWRT
        if(viGetAttribute(handle,VI_ATTR_TCPIP_HOSTNAME,buffer) < VI_SUCCESS) {
            buffer[0] = 0;
        }
#endif // #else

    }
}

void socket_client_c::getLocalAddress(pchar *buffer,pu32 bufferSize) {
#ifndef LABVIEWRT
    pchar temp[60];
    hostent *host;

    if(::gethostname(temp,sizeof(temp)) == SOCKET_ERROR)
        return;
    if((host = gethostbyname(temp)) == NULL)
        return;
    pchar *addr = ::inet_ntoa(*((struct in_addr *)(host->h_addr)));
    ::memcpy(buffer,addr,bufferSize * CHAR_SIZE);
#else // #ifndef LABVIEWRT
    if(viGetAttribute(handle,VI_ATTR_TCPIP_HOSTNAME,buffer) < VI_SUCCESS) {
        buffer[0] = 0;
    }
#endif // #else
}

pbool socket_client_c::getRecvTimeout(pu32 *timeout) {
#ifdef LABVIEWRT
    return viGetAttribute(handle,VI_ATTR_TMO_VALUE,&timeout) == VI_SUCCESS;
#elif LINUX
    timeval tv = {0};
    pu32 size = sizeof(struct timeval);
    pbool result = ::getsockopt(handle,SOL_SOCKET,SO_RCVTIMEO,(pchar*)&tv,(socklen_t*)size) != SOCKET_ERROR;
    *timeout = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
    return result;
#else
    pu32 size = sizeof(pu32);
    return ::getsockopt(handle,SOL_SOCKET,SO_RCVTIMEO,(pchar*)timeout,(socklen_t*)&size) != SOCKET_ERROR;
#endif
}

pbool socket_client_c::getSendTimeout(pu32 *timeout) {
#ifdef LABVIEWRT
    return viGetAttribute(handle,VI_ATTR_TMO_VALUE,&timeout) == VI_SUCCESS;
#elif LINUX
    timeval tv = {0};
    pu32 size = sizeof(struct timeval);
    pbool result = ::getsockopt(handle,SOL_SOCKET,SO_SNDTIMEO,(pchar*)&tv,(socklen_t*)size) != SOCKET_ERROR;
    *timeout = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
    return result;
#else
    pu32 size = sizeof(pu32);
    return ::getsockopt(handle,SOL_SOCKET,SO_SNDTIMEO,(pchar*)timeout,(socklen_t*)&size) != SOCKET_ERROR;
#endif
}

pbool socket_client_c::recv() {
    if(connected) {

#ifndef LABVIEWRT
        if((type == SOCKET_TCP) && connected) {
            offset = 0;
            if((recvBytes = ::recv(handle,buffer,bufferSize,0)) == SOCKET_ERROR) {
                recvBytes = 0;
                return FALSE;
            }
        }
#else // #ifndef LABVIEWRT
        register ViUInt32 recvd;
        register ViUInt32 header[2] = {0};
        register ViUInt32 headerSize = DWORD_SIZE * 2;

        if((type == SOCKET_VISA) && connected) {
            offset = 0;
            if(viRead(handle,(ViPBuf)&header,headerSize,&recvd) < VI_SUCCESS) {
                return FALSE;
            }
            ::memcpy(buffer,header,headerSize);
            if(viRead(handle,(ViPBuf)(buffer + headerSize),header[1],(ViUInt32*)&recvBytes) < VI_SUCCESS) {
                return FALSE;
            }
            recvBytes += recvd;
        }
#endif // #else

        return TRUE;
    }
    return FALSE;
}

pbool socket_client_c::send() {
    if(connected) {    

#ifndef LABVIEWRT
        pu32 result;

        if(type == SOCKET_TCP) {
            result = ::send(handle,buffer,offset,0);      
            if((result == SOCKET_ERROR) && (result != offset)) {
                offset = 0;
                return FALSE;
            }
            //offset = 0;
        }
#else // #ifndef LABVIEWRT
        ViUInt32 sended;

        if(type == SOCKET_VISA) {
            if(viWrite(handle,(ViPBuf)buffer,offset,&sended) < VI_SUCCESS) {
                return FALSE;
            }
            if(sended != offset) {
                return FALSE;
            }
            //offset = 0;
        }
#endif // #else

        ::memset(buffer,0,SOCKET_BUFFER_SIZE);    
        return TRUE;
    }
    return FALSE;
}

pbool socket_client_c::setRecvTimeout(pu32 timeout) {
#ifdef LABVIEWRT
    return(connected ? viSetAttribute(handle,VI_ATTR_TMO_VALUE,timeout) == VI_SUCCESS : FALSE);
#elif LINUX
    timeval tv = {0};
    tv.tv_sec = timeout / 1000;
    tv.tv_usec = (timeout % 1000) * 1000;
    return ::setsockopt(handle,SOL_SOCKET,SO_RCVTIMEO,(pchar*)&tv,sizeof(struct timeval)) != SOCKET_ERROR;
#else
    pu32 size = sizeof(pu32);
    return ::setsockopt(handle,SOL_SOCKET,SO_RCVTIMEO,(pchar*)&timeout,(socklen_t)size) != SOCKET_ERROR;
#endif
}

pbool socket_client_c::setSendTimeout(pu32 timeout) {
#ifdef LABVIEWRT
    return(connected ? viSetAttribute(handle,VI_ATTR_TMO_VALUE,timeout) == VI_SUCCESS : FALSE);
#elif LINUX
    timeval tv = {0};
    tv.tv_sec = timeout / 1000;
    tv.tv_usec = (timeout % 1000) * 1000;
    return ::setsockopt(handle,SOL_SOCKET,SO_SNDTIMEO,(pchar*)&tv,sizeof(struct timeval)) != SOCKET_ERROR;
#else
    pu32 size = sizeof(pu32);
    return ::setsockopt(handle,SOL_SOCKET,SO_SNDTIMEO,(pchar*)&timeout,(socklen_t)size) != SOCKET_ERROR;
#endif
}

#if (defined(WIN32) || defined(LINUX)) && !defined(LABVIEWRT)

pbool socket_client_c::recvFrom(ps32 flags,pchar *address,pu32 *port) {
    if(type != SOCKET_TCP) {
        sockaddr_in sin;
        socklen_t sinSize = sizeof(sockaddr_in);

        offset = 0;
        recvBytes = ::recvfrom(handle,buffer,bufferSize,flags,(sockaddr*)&sin,&sinSize);
        if(recvBytes == SOCKET_ERROR) {
            recvBytes = 0;
            return FALSE;
        }
        if(address != NULL) {
            pchar *addr = ::inet_ntoa(sin.sin_addr);
            ::memcpy(address,addr,strlen(addr) + 1);
            //delete addr;
        }
        if(port != NULL) {
            *port = (ps32)sin.sin_port;
        }
        return TRUE;
    }
    return FALSE;
}

pbool socket_client_c::sendTo(ps32 flags,const pchar *address,pu32 port) {
    if(type != SOCKET_TCP) {
        sockaddr_in sin;
        hostent *hostInfo = ::gethostbyname(address);
        ps32 result;

        if(hostInfo == NULL)
            return FALSE;
        sin.sin_family = AF_INET;
        sin.sin_port = htons((pu16)port);
        ::memcpy(&sin.sin_addr,hostInfo->h_addr,hostInfo->h_length);
        // !!! Maybe in Linux it's necesary !!!
        //delete hostInfo;
        result = ::sendto(handle,buffer,offset,flags,(sockaddr*)&sin,sizeof(sockaddr_in));
        offset = 0;
        recvBytes = 0;
        return result != SOCKET_ERROR;      
    }
    return FALSE;
}

pbool socket_client_c::setBroadcastMode(pbool enable) {
    int value = (int)enable;

    if(created) {
        return setsockopt(handle,SOL_SOCKET,SO_BROADCAST,(pchar*)&value,sizeof(int)) != SOCKET_ERROR;
    }
    return false;
}

#endif // #if (defined(WIN32) || defined(LINUX)) && !defined(LABVIEWRT)

/*
* Server socket class functions
*/

#if (defined(WIN32) || defined(LINUX)) && !defined(LABVIEWRT)

socket_server_c::socket_server_c(socket_type_e type,pu32 bufferSize):socket_c(bufferSize) {
    int socketType;
    int protocol;

    this->type = type;
    this->listenning = false;
    switch(type) {     
    case SOCKET_UDP:
        socketType = SOCK_DGRAM;
        protocol = IPPROTO_UDP;
        break;
    case SOCKET_RAW:
        socketType = SOCK_RAW;
        protocol = IPPROTO_RAW;
        break;
    default: // SOCKET_TCP
        socketType = SOCK_STREAM;
        protocol = IPPROTO_TCP;
        break;
    };
    this->created = (handle = ::socket(AF_INET,socketType,protocol)) != INVALID_SOCKET;
    if(this->created) {
        buffer = new pchar[bufferSize];
    }  
}  

socket_server_c::~socket_server_c() {
    //if(created && listenning) {
    disconnect();
    //}
    //socket_c::~socket_c();
}

socket_client_c *socket_server_c::accept() {
    socket_t socket;
    sockaddr_in sin;
    socklen_t size = (int)sizeof(sockaddr_in);    

    if((type == SOCKET_TCP) && created && listenning) {
        if((socket = ::accept(handle,(sockaddr*)&sin,&size)) != INVALID_SOCKET) {
            return new socket_client_c(SOCKET_TCP,SOCKET_BUFFER_SIZE,socket,true);        
        }
    }
    return NULL;
}

pbool socket_server_c::bind(pu32 port) {
    sockaddr_in sin;

    if(type == SOCKET_TCP) {
        sin.sin_family = AF_INET;
        sin.sin_port = htons((u_short)port);
        sin.sin_addr.s_addr = INADDR_ANY;
        return ::bind(handle,(sockaddr*)&sin,sizeof(sockaddr_in)) != SOCKET_ERROR;
    }
    return false;
}

pbool socket_server_c::disconnect() {
    if(created /*&& connected*/) {
        // !!! Returns error 10057 !!!
        //::shutdown(handle,2);
        ::closesocket(handle);            
        connected = false;
        listenning = false;
        //created = false;
        return TRUE;
    }
    return FALSE;
}

pbool socket_server_c::listen(pu32 port,pu32 backLog) {
    if(type == SOCKET_TCP)
        if(bind(port))
            listenning = ::listen(handle,backLog) != SOCKET_ERROR;
    return listenning;
}

pbool socket_server_c::setBlockingMode(pbool enable) {
#ifdef LINUX
    int flag;
#endif // #ifdef LINUX
    int mode;

    if(created) {
        mode = !enable;
#ifdef WIN32
        return ioctlsocket(handle,FIONBIO,(u_long FAR*)&mode) != SOCKET_ERROR;
#elif LINUX // #ifdef WIN32
        flag = fcntl(handle,F_GETFL,0);
        if(enable) {
            flag = flag && ~O_NONBLOCK;
        } else {
            flag = flag | O_NONBLOCK;
        }
        return !fcntl(handle,F_SETFL,flag);
#endif // #elif LINUX
    }
    return false;
}

#endif // #if (defined(WIN32) || defined(LINUX)) && !defined(LABVIEWRT)
