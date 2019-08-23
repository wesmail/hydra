// File: hldremotevt.cc
// Authors: H.G & D.B
// Modified by D.B : 02-12-98
// Modified by D.B : 19-03-99

//////////////////////////////////////////////////////
//HldRemotEvent
//
//  Class used for reading LMD events from file
//
/////////////////////////////////////////////////////

#include "hldremotevt.h"
#include <sys/socket.h>
#include <iomanip.h>
#include "linux_server.h"
#include <iostream.h>

//ClassImp(HldRemotEvt) 

HldRemotEvt::HldRemotEvt(const char* node){ 
pHdr = (UInt4*) &hdr;
pData=0;
lastSubEvtIdx = 0;
if( !open(node)) exit(1); 
}

Bool_t  HldRemotEvt::open (const char *pcnode)
{
//here the handle is set once.
   CLIENT *rpchandle;
   rpchandle = clnt_create(pcnode, DAQPROG, DAQVERS, "tcp");
   if (rpchandle == NULL)
   {
      clnt_pcreateerror(pcnode);
      cout << "-E- connecting to server " << pcnode << endl;
      return kFALSE;
   }
   rpcHandle = (void *) rpchandle;
   pcNode = pcnode;
   cout << "-I- connection to server " << pcNode << " okay" << endl;
return kTRUE;
}

Bool_t  HldRemotEvt::getBuffer(void *handle){
// here using the handle i retrieve the actual  buffer.
rpcbuf* rpcbuffer;  
rpcbuffer = onlineevt_1( 0, (CLIENT *) handle);
   if (rpcbuffer == 0)
   {
      clnt_perror( (CLIENT *) handle, pcNode);
    cout << "-E- in RPC - Buffering  "  << endl;
      return kFALSE;
   }

rpcBuf = rpcbuffer;
return kTRUE;
}

UInt_t  HldRemotEvt::getLen(){ //Data Lenght (bytes)
if(rpcBuf) return  rpcBuf->rpcbuf_len;
return 0;
}


Bool_t HldRemotEvt::read() {
pData=0;
// takes the current buffer  
if (getBuffer(rpcHandle)){
                    pHdr = (UInt4*) rpcBuf->rpcbuf_val;
                    }else{return kFALSE;}
if(pHdr){
     if (isSwapped()) swapHdr();
     if (getSize() > getHdrSize()) {
       pData  = pHdr + getHdrLen();
       if (pData) return kTRUE;
     }
}
return kFALSE;
}

Bool_t HldRemotEvt::readSubEvt(size_t i)  {
  UInt4* p;
   if (i)
     p = subEvt[i-1].getPaddedEnd();
   else
     p = pData;
   if (p < getPaddedEnd())
     subEvt[i] = HldSubEvt(p);
   else
     return kFALSE;
   return kTRUE;
}
  

Int_t HldRemotEvt::reGetValue(Int_t iNumber)
{
if(rpcBuf){
    Int_t iValue;
    Int_t *pint;
    Int_t intSize = sizeof(int);
     if ( (iNumber < 1) || ( (UInt_t) iNumber > (this->getLen())/intSize) )
        {
           cout << "-E- event parameter number " << iNumber 
                    << " out of range" << endl;
            return(-1);
         }
pint = (Int_t *) rpcBuf->rpcbuf_val;
iValue = pint[--iNumber];
return iValue;}else{return -999;}
}

Bool_t HldRemotEvt::execute(){
 
  if(read()){ 
 
   for (size_t idx = 0; idx < lastSubEvtIdx; idx++)    *subEvtTable[idx].p = 0;
     for (size_t i = 0; i < maxSubEvts && readSubEvt(i); i++){
      for (size_t idx = 0; idx < lastSubEvtIdx; idx++) {
	//	  cout << "\tsubEvt["<<i<<"].getId()= " << subEvt[i].getId() 
	//	 << " table= " << subEvtTable[idx].id << endl;
     
       if (subEvt[i].getId() == subEvtTable[idx].id) {
          subEvt[i].swapData();
          *subEvtTable[idx].p = subEvt + i;
	  //   cout << "Put Subevt into table" << endl;
          }
       }
     }
    return kTRUE;
  } else {
 return kFALSE;
 }
}


Bool_t HldRemotEvt::swap(){
  //Only test the swap byte order in the event  
  if(read()){ 
    return kTRUE;
  } else {
 return kFALSE;
 }
}






