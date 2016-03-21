#include <PipslxModule.h>
#include <PipslxFids.h>
#include <PicmlxModuleAPI.h>
#include <Picmlx.h>
#include <PipslxResources.h>

#ifdef WIN32
	extern handle_t __libPipslx_Instance;
#endif

enum PipslxServerErrorCodes {  
  ERR_PS_BAD_FORMAT_DATA = 0x02000000,
  ERR_PS_SWITCH_FAULT,
  ERR_PS_UNKNOWN_CMD,
  ERR_PS_BUSY,
  ERR_PS_SEQUPINPROGRESS,
  ERR_PS_SEQDOWNINPROGRESS,
  ERR_PS_CHANTESTINPROGRESS,
  ERR_PS_EMERGENCY_STOP
};

/* ##################################################################################################### */

pu32 pipslx_module_c::convertServerError(pu32 code) {
  switch(code) {
    case ERR_PS_BAD_FORMAT_DATA: return ER_PIPSLX_BAD_FORMAT_DATA;
    case ERR_PS_SWITCH_FAULT: return ER_PIPSLX_SWITCH_FAULT;
    case ERR_PS_UNKNOWN_CMD: return ER_PIPSLX_UNKNOWN_CMD;
    case ERR_PS_BUSY: return ER_PIPSLX_BUSY;
    case ERR_PS_SEQUPINPROGRESS: return ER_PIPSLX_SEQUPINPROGRESS;
    case ERR_PS_SEQDOWNINPROGRESS: return ER_PIPSLX_SEQDOWNINPROGRESS;
    case ERR_PS_CHANTESTINPROGRESS: return ER_PIPSLX_CHANTESTINPROGRESS;
    case ERR_PS_EMERGENCY_STOP: return ER_PIPSLX_EMERGENCY_STOP;
    default: return code;
  };
}

pu32 pipslx_module_c::error2Resource(pu32 code) {
  switch(code) {
    case ER_PIPSLX_INVALID_SESSION: return RES_PIPSLX_INVALID_SESSION;
    case ER_PIPSLX_FUNC_NOT_LOCAL: return RES_PIPSLX_FUNC_NOT_LOCAL;
    case ER_PIPSLX_NOT_CONNECTED: return RES_PIPSLX_NOT_CONNECTED;
    case ER_PIPSLX_NOT_INIT: return RES_PIPSLX_NOT_INIT;
    case ER_PIPSLX_SWITCH_FAULT: return RES_PIPSLX_SWITCH_FAULT;
    case ER_PIPSLX_UNKNOWN_CMD: return RES_PIPSLX_UNKNOWN_CMD;
    case ER_PIPSLX_BUSY: return RES_PIPSLX_BUSY;
    case ER_PIPSLX_SEQUPINPROGRESS: return RES_PIPSLX_SEQUPINPROGRESS;
    case ER_PIPSLX_SEQDOWNINPROGRESS: return RES_PIPSLX_SEQDOWNINPROGRESS;
    case ER_PIPSLX_CHANTESTINPROGRESS: return RES_PIPSLX_CHANTESTINPROGRESS;
    case ER_PIPSLX_EMERGENCY_STOP: return RES_PIPSLX_EMERGENCY_STOP;
    default:
      return RES_UNKNOWN_ERROR;
  };
}

/* ##################################################################################################### */

pipslx_module_c::pipslx_module_c(const pchar *libName):base_module_c(libName) {
  // Create error message list
  #if(defined(LINUX) || defined(LABVIEWRT))
    errorList->insert(make_pair(RES_UNKNOWN_ERROR,"Unknown error code."));
    //errorList->insert(make_pair(RES_UNKNOWN_RESOURCE,"Unknown resource ID."));
    errorList->insert(make_pair(RES_PIPSLX_INVALID_SESSION,"Session ID is invalid => !!! Do not use it anymore !!! Replaced by ER_PICMLX_INVALID_SESSION."));
    errorList->insert(make_pair(RES_PIPSLX_FUNC_NOT_LOCAL,"Function not supported on localhost."));
    errorList->insert(make_pair(RES_PIPSLX_NOT_CONNECTED,"You are not connected to remote host."));
    errorList->insert(make_pair(RES_PIPSLX_SWITCH_FAULT,"Switch fault."));
    errorList->insert(make_pair(RES_PIPSLX_BAD_FORMAT_DATA,"Data or command format is bad."));
    errorList->insert(make_pair(RES_PIPSLX_UNKNOWN_CMD,"Unknown command."));
    errorList->insert(make_pair(RES_PIPSLX_BUSY,"Device is busy."));
    errorList->insert(make_pair(RES_PIPSLX_SEQUPINPROGRESS,"Up sequence is in progress."));
    errorList->insert(make_pair(RES_PIPSLX_SEQDOWNINPROGRESS,"Down sequence is in progress."));
    errorList->insert(make_pair(RES_PIPSLX_CHANTESTINPROGRESS,"Channel test is running."));
    errorList->insert(make_pair(RES_PIPSLX_EMERGENCY_STOP,"Emergency stop button engaged."));
    errorList->insert(make_pair(RES_PIPSLX_NOT_INIT,"Library wasn't initialized!"));    
  #endif // #if(defined(LINUX) || defined(LABVIEWRT))
}

pipslx_module_c::~pipslx_module_c() {
  //base_module_c::~base_module_c();
}

/* ##################################################################################################### */

pbool pipslx_module_c::isError(session_t sid) {
  pu32 code;

  if(getLastError(sid,&code))
    return true;
  return (pbool)code;
}

pu32 pipslx_module_c::getLastError(session_t sid,pu32 *code) {
  return PICMLX_GetLastErrorCode(sid,code);
}

pu32 pipslx_module_c::getLastErrorMessage(session_t sid,pchar *buffer,pu32 size) {
  pu32 code,result,length;
  pchar fmtStr[512];
  
  if((buffer == NULL) || (((ps32)size) <= 0)) {
    return ER_PICMLX_CLI_PARAM_SIZE;
  }  
  result = getLastError(sid,&code);
  if(!result)
    if(code >= 0) {
      code = errorCode2Message(code,buffer,size);          
		  sprintf_s(fmtStr,sizeof(fmtStr),"%s [SysError: %d]",buffer,GetLastError()); //sprintf_s(fmtStr,sizeof(fmtStr),"%s, (SystemErrorCode: %d)",buffer,GetLastError());
      length = (int)strlen(fmtStr);
      if(size <= length) {
        fmtStr[size - 1] = 0;
      }
      strcpy_s(buffer,size,fmtStr);
    }  
  return result;
}

pu32 pipslx_module_c::errorCode2Message(pu32 code,pchar *buffer,pu32 size) {
  pu32 resCode;

  if((buffer == NULL) || (((ps32)size) <= 0))
    return ER_PICMLX_CLI_PARAM_SIZE;  
  buffer[0] = 0;
  resCode = error2Resource(code);  
  if(resCode != RES_UNKNOWN_ERROR) {
    #ifdef WIN32
      LOADSTRING(__libPipslx_Instance,resCode,buffer,size);
    #elif LINUX // #ifdef WIN32
      LOADSTRING(errorList,resCode,buffer,size);
    #endif // #elif LINUX
  } else {
    return PICMLX_ErrorCodeToMessage(code,buffer,size);
  }  
  return NO_ERROR;
}

/* ##################################################################################################### */

pu32 pipslx_module_c::Open(session_t sid) {
  register pu32 result;
  session_data_s sesData = {0};

  result = PICMLX_BeginTransmition(sid,&sesData);
  if(!result) {
    if(sesData.remote) {
      if(sesData.connected) {
        if(!PICMLX_SendPacket(PIPSLX_FID_OPEN,NULL)) {
          sesData.lastError = PICMLX_RecvPacket(pipslx_module_c::convertServerError,NULL);
        }
      } else {
        sesData.lastError = ER_PICMLX_CLI_NOT_CONNECTED;
      }
    } else {      
      sesData.lastError = ER_PICMLX_CLI_FUNC_NOT_LOCAL;
    }
    result = sesData.lastError;
  }
  PICMLX_EndTransmition(&sesData);
  return result;
}

pu32 pipslx_module_c::OpenForRead(session_t sid) {
  register pu32 result;
  session_data_s sesData = {0};

  result = PICMLX_BeginTransmition(sid,&sesData);
  if(!result) {
    if(sesData.remote) {
      if(sesData.connected) {
        if(!PICMLX_SendPacket(PIPSLX_FID_OPENFORREAD,NULL)) {
          sesData.lastError = PICMLX_RecvPacket(pipslx_module_c::convertServerError,NULL);
        }
      } else {
        sesData.lastError = ER_PICMLX_CLI_NOT_CONNECTED;
      }
    } else {      
      sesData.lastError = ER_PICMLX_CLI_FUNC_NOT_LOCAL;
    }
    result = sesData.lastError;
  }
  PICMLX_EndTransmition(&sesData);
  return result;
}

pu32 pipslx_module_c::Close(session_t sid) {
  register pu32 result;
  session_data_s sesData = {0};

  result = PICMLX_BeginTransmition(sid,&sesData);
  if(!result) {
    if(sesData.remote) {
      if(sesData.connected) {
        if(!PICMLX_SendPacket(PIPSLX_FID_CLOSE,NULL)) {
          sesData.lastError = PICMLX_RecvPacket(pipslx_module_c::convertServerError,NULL);
        }
      } else {
        sesData.lastError = ER_PICMLX_CLI_NOT_CONNECTED;
      }
    } else {      
      sesData.lastError = ER_PICMLX_CLI_FUNC_NOT_LOCAL;
    }
    result = sesData.lastError;
  }
  PICMLX_EndTransmition(&sesData);
  return result;
}

pu32 pipslx_module_c::GetChanState(session_t sid,pu32 Chan,pbool* State) {
  register pu32 result;
  register pu32 chanCount;
  session_data_s sesData = {0};

  if((result = this->GetChanCount(sid,&chanCount)))
    return result;
  result = PICMLX_BeginTransmition(sid,&sesData);
  if(!result) {
    if((((ps32)Chan) <= 0) || (Chan > chanCount))
      sesData.lastError = ER_PIPSLX_BAD_FORMAT_DATA;
    else if(State == NULL)
      sesData.lastError = ER_PICMLX_CLI_PARAM_SIZE;
    else
      if(sesData.remote) {
        if(sesData.connected) {
          if(!PICMLX_SendPacket(PIPSLX_FID_GETCHANSTATE,"u",Chan)) {
            sesData.lastError = PICMLX_RecvPacket(pipslx_module_c::convertServerError,"b",State);
          }
        } else {
          sesData.lastError = ER_PICMLX_CLI_NOT_CONNECTED;
        }
      } else {      
        sesData.lastError = ER_PICMLX_CLI_FUNC_NOT_LOCAL;
      }
      result = sesData.lastError;
  }
  PICMLX_EndTransmition(&sesData);
  return result;
}

pu32 pipslx_module_c::SetChanState(session_t sid,pu32 Chan,pbool State) {
  register pu32 result;
  register pu32 chanCount;
  session_data_s sesData = {0};

  if((result = this->GetChanCount(sid,&chanCount)))
    return result;
  result = PICMLX_BeginTransmition(sid,&sesData);
  if(!result) {
    if((((ps32)Chan) <= 0) || (Chan > chanCount))
      sesData.lastError = ER_PIPSLX_BAD_FORMAT_DATA;
    else
      if(sesData.remote) {
        if(sesData.connected) {
          if(!PICMLX_SendPacket(PIPSLX_FID_SETCHANSTATE,"ub",Chan,State)) {
            sesData.lastError = PICMLX_RecvPacket(pipslx_module_c::convertServerError,NULL);
          }
        } else {
          sesData.lastError = ER_PICMLX_CLI_NOT_CONNECTED;
        }
      } else {      
        sesData.lastError = ER_PICMLX_CLI_FUNC_NOT_LOCAL;
      }
      result = sesData.lastError;
  }
  PICMLX_EndTransmition(&sesData);
  return result;
}

pu32 pipslx_module_c::GetChanTime(session_t sid,pu32 Chan,pu32 TimeType,pu32 *Time) {
  register pu32 result;
  register pu32 chanCount;
  session_data_s sesData = {0};

  if((result = this->GetChanCount(sid,&chanCount)))
    return result;
  result = PICMLX_BeginTransmition(sid,&sesData);
  if(!result) {
    if(Time == NULL)
      sesData.lastError = ER_PICMLX_CLI_PARAM_SIZE;
    else if((TimeType < 0) || (TimeType > 1) || (((ps32)Chan) <= 0) || (Chan > chanCount))
      sesData.lastError = ER_PIPSLX_BAD_FORMAT_DATA;
    else
      if(sesData.remote) {
        if(sesData.connected) {
          if(!PICMLX_SendPacket(PIPSLX_FID_GETCHANTIME,"uu",Chan,TimeType)) {
            sesData.lastError = PICMLX_RecvPacket(pipslx_module_c::convertServerError,"u",Time);
          }
        } else {
          sesData.lastError = ER_PICMLX_CLI_NOT_CONNECTED;
        }
      } else {      
        sesData.lastError = ER_PICMLX_CLI_FUNC_NOT_LOCAL;
      }
      result = sesData.lastError;
  }
  PICMLX_EndTransmition(&sesData);
  return result;
}

pu32 pipslx_module_c::SetChanTime(session_t sid,pu32 Chan,pu32 TimeType,pu32 Time) {
  register pu32 result;
  register pu32 chanCount;
  session_data_s sesData = {0};

  if((result = this->GetChanCount(sid,&chanCount)))
    return result;
  result = PICMLX_BeginTransmition(sid,&sesData);
  if(!result) {
    if((TimeType < 0) || (TimeType > 1) || (((ps32)Chan) <= 0) || (Chan > chanCount) || (((ps32)Time) <= 0) || (Time > 60000))
      sesData.lastError = ER_PIPSLX_BAD_FORMAT_DATA;    
    else 
      if(sesData.remote) {
        if(sesData.connected) {
          if(!PICMLX_SendPacket(PIPSLX_FID_SETCHANTIME,"uuu",Chan,TimeType,Time)) {
            sesData.lastError = PICMLX_RecvPacket(pipslx_module_c::convertServerError,NULL);
          }
        } else {
          sesData.lastError = ER_PICMLX_CLI_NOT_CONNECTED;
        }
      } else {      
        sesData.lastError = ER_PICMLX_CLI_FUNC_NOT_LOCAL;
      }
      result = sesData.lastError;
  }
  PICMLX_EndTransmition(&sesData);
  return result;
}

pu32 pipslx_module_c::GetChanEnabled(session_t sid,pu32 Chan,pbool *Enabled) {
  register pu32 result;
  register pu32 chanCount;
  session_data_s sesData = {0};

  if((result = this->GetChanCount(sid,&chanCount)))
    return result;
  result = PICMLX_BeginTransmition(sid,&sesData);
  if(!result) {
    if((((ps32)Chan) <= 0) || (Chan > chanCount))
      sesData.lastError = ER_PIPSLX_BAD_FORMAT_DATA;
    else if(Enabled == NULL)
      sesData.lastError = ER_PICMLX_CLI_PARAM_SIZE;
    else
      if(sesData.remote) {
        if(sesData.connected) {
          if(!PICMLX_SendPacket(PIPSLX_FID_GETCHANENABLED,"u",Chan)) {
            sesData.lastError = PICMLX_RecvPacket(pipslx_module_c::convertServerError,"b",Enabled);
          }
        } else {
          sesData.lastError = ER_PICMLX_CLI_NOT_CONNECTED;
        }
      } else {      
        sesData.lastError = ER_PICMLX_CLI_FUNC_NOT_LOCAL;
      }
      result = sesData.lastError;
  }
  PICMLX_EndTransmition(&sesData);
  return result;
}

pu32 pipslx_module_c::SetChanEnabled(session_t sid,pu32 Chan,pbool Enabled) {
  register pu32 result;
  register pu32 chanCount;
  session_data_s sesData = {0};

  if((result = this->GetChanCount(sid,&chanCount)))
    return result;
  result = PICMLX_BeginTransmition(sid,&sesData);
  if(!result) {
    if((((ps32)Chan) <= 0) || (Chan > chanCount))
      sesData.lastError = ER_PIPSLX_BAD_FORMAT_DATA;
    else
      if(sesData.remote) {
        if(sesData.connected) {
          if(!PICMLX_SendPacket(PIPSLX_FID_SETCHANENABLED,"ub",Chan,Enabled)) {
            sesData.lastError = PICMLX_RecvPacket(pipslx_module_c::convertServerError,NULL);
          }
        } else {
          sesData.lastError = ER_PICMLX_CLI_NOT_CONNECTED;
        }
      } else {      
        sesData.lastError = ER_PICMLX_CLI_FUNC_NOT_LOCAL;
      }
      result = sesData.lastError;
  }
  PICMLX_EndTransmition(&sesData);
  return result;
}

pu32 pipslx_module_c::ChanTest(session_t sid,pu32* BadChans,pu32* BadChansLen) {
  register pu32 result;
  register pu32 chanCount;
  register pu32 settleTime;
  register int timeout;
  register int timeoutOld;
  session_data_s sesData = {0};
  
  if((result = this->GetChanCount(sid,&chanCount)))
    return result;
  if((result = this->SettleTime(sid,&settleTime)))
    return result;
  result = PICMLX_BeginTransmition(sid,&sesData);
  if(!result) {
    if((BadChans == NULL) || (BadChansLen == NULL))
      sesData.lastError = ER_PICMLX_CLI_PARAM_SIZE;
    else if(((ps32)*BadChansLen) <= 0)
      sesData.lastError = ER_PICMLX_CLI_PARAM_SIZE;
    else {
      if(sesData.remote) {
        if(sesData.connected) {
          // set new socket receive timeout
          if(PICMLX_GetOption(ON_RECVTIMEO,&timeoutOld)) {
          timeout = (settleTime * chanCount) + 600;
          if(timeout > timeoutOld)
            PICMLX_SetOption(ON_RECVTIMEO,timeout);
          }
          if(!PICMLX_SendPacket(PIPSLX_FID_CHANTEST,"uu",*BadChansLen,timeout)) {
            sesData.lastError = PICMLX_RecvPacket(pipslx_module_c::convertServerError,"uau",BadChansLen,*BadChansLen,BadChans);
          }
          // set old socket receive timeout
          if(timeout > timeoutOld)
            PICMLX_SetOption(ON_RECVTIMEO,timeoutOld);
          } else {
            sesData.lastError = ER_PICMLX_CLI_NOT_CONNECTED;
          }          
       } else {      
         sesData.lastError = ER_PICMLX_CLI_FUNC_NOT_LOCAL;
       }          
    }
    result = sesData.lastError;
  }
  PICMLX_EndTransmition(&sesData);
  return result;
}

pu32 pipslx_module_c::Sequence(session_t sid,pu32 SequenceType) {
  register pu32 result;
  register pu32 chanCount;
  register pu32 settleTime;
  register int timeout;
  register int timeoutOld;
  register pu32 time;
  session_data_s sesData = {0};

  if((result = this->GetChanCount(sid,&chanCount)))
    return result;
  if((result = this->SettleTime(sid,&settleTime)))
    return result;
  timeout = 0;
  for(pu32 i = 1; i < chanCount; i++) {
    if((result = this->GetChanTime(sid,i,SequenceType,&time)))
      return result;
    if(timeout < (int)time)
      timeout = time;
  }
  timeout += 600;
  result = PICMLX_BeginTransmition(sid,&sesData);
  if(!result) {    
    if(sesData.remote) {
      if(sesData.connected) {
        // set new socket receive timeout
        if(PICMLX_GetOption(ON_RECVTIMEO,&timeoutOld)) {
          timeout = (settleTime * chanCount) + 600;
          if(timeout > timeoutOld)
            PICMLX_SetOption(ON_RECVTIMEO,timeout);
        }
        if(!PICMLX_SendPacket(PIPSLX_FID_SEQUENCE,"uu",SequenceType,timeout)) {
          sesData.lastError = PICMLX_RecvPacket(pipslx_module_c::convertServerError,NULL);
        }
        // set old socket receive timeout
        if(timeout > timeoutOld)
          PICMLX_SetOption(ON_RECVTIMEO,timeoutOld);
      } else {
        sesData.lastError = ER_PICMLX_CLI_NOT_CONNECTED;
      }
    } else {      
      sesData.lastError = ER_PICMLX_CLI_FUNC_NOT_LOCAL;
    }
    result = sesData.lastError;
  }
  PICMLX_EndTransmition(&sesData);
  return result;
}

pu32 pipslx_module_c::SettleTime(session_t sid,pu32* Time) {
  register pu32 result;

  session_data_s sesData = {0};

  result = PICMLX_BeginTransmition(sid,&sesData);
  if(!result) {
    if(sesData.remote) {
      if(sesData.connected) {
        if(!PICMLX_SendPacket(PIPSLX_FID_SETTLETIME,NULL)) {
          sesData.lastError = PICMLX_RecvPacket(pipslx_module_c::convertServerError,"u",Time);
        }
      } else {
        sesData.lastError = ER_PICMLX_CLI_NOT_CONNECTED;
      }
    } else {      
      sesData.lastError = ER_PICMLX_CLI_FUNC_NOT_LOCAL;
    }
    result = sesData.lastError;
  }
  PICMLX_EndTransmition(&sesData);
  return result;
}

pu32 pipslx_module_c::Shutdown(session_t sid) {
  register pu32 result;

  session_data_s sesData = {0};

  result = PICMLX_BeginTransmition(sid,&sesData);
  if(!result) {
    if(sesData.remote) {
      if(sesData.connected) {
        if(!PICMLX_SendPacket(PIPSLX_FID_SHUTDOWN,NULL)) {
          sesData.lastError = PICMLX_RecvPacket(pipslx_module_c::convertServerError,NULL);
        }
      } else {
        sesData.lastError = ER_PICMLX_CLI_NOT_CONNECTED;
      }
    } else {      
      sesData.lastError = ER_PICMLX_CLI_FUNC_NOT_LOCAL;
    }
    result = sesData.lastError;
  }
  PICMLX_EndTransmition(&sesData);
  return result;
}

pu32 pipslx_module_c::GetChanCount(session_t sid,pu32* ChanCount) {
  register pu32 result;

  session_data_s sesData = {0};

  result = PICMLX_BeginTransmition(sid,&sesData);
  if(!result) {
    if(sesData.remote) {
      if(sesData.connected) {
        if(!PICMLX_SendPacket(PIPSLX_FID_GETCHANCOUNT,NULL)) {
          sesData.lastError = PICMLX_RecvPacket(pipslx_module_c::convertServerError,"u",ChanCount);
        }
      } else {
        sesData.lastError = ER_PICMLX_CLI_NOT_CONNECTED;
      }
    } else {      
      sesData.lastError = ER_PICMLX_CLI_FUNC_NOT_LOCAL;
    }
    result = sesData.lastError;
  }
  PICMLX_EndTransmition(&sesData);
  return result;
}
