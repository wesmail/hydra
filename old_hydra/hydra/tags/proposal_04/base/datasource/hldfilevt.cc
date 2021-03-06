// File: hldevt.cc
//
//*-- Modified : 27/09/2000 by Ilse Koenig
//*-- Author : Walter Karig <W.Karig@gsi.de>
//*-- Modified : 97/12/11 18:19:30 by Walter Karig
//*-- Modified : 25/03/98 by Manuel Sanchez Garcia

//////////////////////////////////////////////////////
//HldEvent
//
//  Class used for reading LMD events from file
//
/////////////////////////////////////////////////////
using namespace std;
#include <iostream>
#include <iomanip> 
#include "hldfilevt.h"
#include <unistd.h>
#include "TError.h"


//ClassImp(HldFilEvt) 

#define MAX_SUBEVT_ID 999

Bool_t HldFilEvt::setFile(const Text_t *name) {
  if (access(name,F_OK)!=0) return kFALSE; //File does not exist
  delete file;
  file=new std::ifstream(name);
  return kTRUE;
}

Bool_t HldFilEvt::read() {
  if (file) {
    if (file->eof()) {
      return kFALSE;
    }
    if (pData) {
      delete[] pData;
      pData = 0;
    }
    if (file->read((char *)(pHdr), getHdrSize())) {
      if (isSwapped()) swapHdr();
      
      if (getSize() > getHdrSize()) {
	#warning "Hardcoded maximum event size is 1000000"
	if (getDataLen() > 1000000) return kFALSE;
        pData = new UInt4[getDataLen()];
	if (file->read((char*)(pData), getDataSize()))
	  file->ignore(getPaddedSize() - getSize());
      }
    }
    return file->good();
  } 
  return kFALSE;
}


Bool_t HldFilEvt::readSubEvt(size_t i) {
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
  


Bool_t HldFilEvt::execute() {
  if (read()) {
    for (size_t idx = 0; idx < lastSubEvtIdx; idx++) *subEvtTable[idx].p = 0;
    for (size_t i = 0; i < maxSubEvts && readSubEvt(i); i++) { // loop subevts
      Bool_t unpacked=kFALSE;
      for (size_t idx = 0; idx < lastSubEvtIdx; idx++) { // loop unpackers
        if (subEvt[i].getId() == subEvtTable[idx].id) {
          subEvt[i].swapData();
          *subEvtTable[idx].p = subEvt + i;
//        cout << "pass a pointer to subevt to its unpacker" << endl;
          unpacked=kTRUE;
        }
      }
      if (isWritable && !unpacked ){
                   // to assure that the swapData() can work
          if(((subEvt[i].getWordSize())==1) || ((subEvt[i].getWordSize())==2) || ((subEvt[i].getWordSize())==4) ){ 
			subEvt[i].swapData();
          }
          else{
            Warning("execute","Corrupted SubEvent, SubId %x",subEvt[i].getId());
          }
	  }

    }
    return kTRUE;
  } else {
    return kFALSE;
  }
} 

Bool_t HldFilEvt::swap() {
//only swapping correctly the header  
  if (read()) {
   return kTRUE;
  } else {
   return kFALSE;
  }
}






