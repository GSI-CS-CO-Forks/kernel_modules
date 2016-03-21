/*******************************************************************************
 *  Power Sequencer 60-200 functions IDs
 *
 *  Copyright (C) 1988 - 2011, Pickering Interfaces ltd.
 *  
 *  Author: michal.heczko
 *  Contact: michal.heczko@pickeringmail.cz
 *  
 *******************************************************************************/

#ifndef _PIPSLXFIDS_H_
#define _PIPSLXFIDS_H_

enum PipslxFunctionIDs {
  PIPSLX_FID_OPEN = 0x00020000,
  PIPSLX_FID_OPENFORREAD,
  PIPSLX_FID_CLOSE,
  PIPSLX_FID_GETCHANSTATE,
  PIPSLX_FID_SETCHANSTATE,
  PIPSLX_FID_GETCHANTIME,
  PIPSLX_FID_SETCHANTIME,
  PIPSLX_FID_GETCHANENABLED,
  PIPSLX_FID_SETCHANENABLED,
  PIPSLX_FID_CHANTEST,
  PIPSLX_FID_SEQUENCE,
  PIPSLX_FID_SETTLETIME,
  PIPSLX_FID_SHUTDOWN,
  PIPSLX_FID_GETCHANCOUNT
};

/*enum PipslFunctionIDs {
  PIPSLX_FID_OPEN = 0x00020000,
  PIPSLX_FID_OPENFORREAD,
  PIPSLX_FID_CLOSE,
  PIPSLX_FID_GETCHANSTATE,
  PIPSLX_FID_SETCHANSTATE,
  PIPSLX_FID_GETCHANTIME,
  PIPSLX_FID_SETCHANTIME,
  PIPSLX_FID_GETCHANENABLED,
  PIPSLX_FID_SETCHANENABLED,
  PIPSLX_FID_CHANTEST,
  PIPSLX_FID_SEQUENCE,
  PIPSLX_FID_SETTLETIME,
  PIPSLX_FID_SHUTDOWN,
  PIPSLX_FID_GETCHANCOUNT
};*/

#endif // _FIDS_H_
