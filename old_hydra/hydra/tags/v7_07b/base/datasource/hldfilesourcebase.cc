//*-- Author : Ilse Koenig
//*-- Modified on 1/3/2000 by R. Holzmann
//*-- Modified : 26/11/98 by Ilse Koenig
//*-- Modified by D.B (Dump of events)
//////////////////////////////////////////////////////////
//HLdFileSource
//
//  This "data source" reads the LMD data from files on disk.
//  (further information in base class HldSource)
//
///////////////////////////////////////////////////////////////////////////////

#include "hldfilesourcebase.h"
#include "hrecevent.h"
#include "hldfilevt.h"
#include "hades.h"
#include "hruntimedb.h"
#include "hmessagemgr.h"
#include <iostream>
#include <unistd.h>
#include <limits.h>

ClassImp(HldFileDesc)
ClassImp(HldFileSourceBase)

HldFileSourceBase::HldFileSourceBase() : HldSource() {
  // Default constructor for a Lmd file data source.
  iter=0;
  fCurrentFile=0;
  fEventNr = 0;
  iter = NULL;
  fEventLimit = INT_MAX;
}


HldFileSourceBase::~HldFileSourceBase(void) {
  // Destructor for a LMD file data source
  if (fReadEvent)
  {
    delete fReadEvent;
    fReadEvent=0;
    }
  if (iter)
  {
    delete iter;
    iter = NULL;
  }
}


HldFileSourceBase::HldFileSourceBase(HldFileSourceBase &so) {
  fFileList.AddAll(&so.fFileList);
  if (so.fCurrentFile) {
    fCurrentFile=(HldFileDesc *)fFileList.FindObject(so.fCurrentFile->GetName());
    iter=0;
  }
  fReadEvent=so.fReadEvent; //FIXME: Do a real copy!!
  fEventNr = 0;
  fEventLimit = INT_MAX;
}


Bool_t HldFileSourceBase::init(void)
{
  // initializes the unpackers and creates the fReadEvent of the first file
  Bool_t r=kFALSE;
  if (!(*fEventAddr)) {
    (*fEventAddr) = new HRecEvent;
  }
  if (isDumped || isScanned)  return getNextFile();  
  iter=fUnpackerList->MakeIterator();
  if (initUnpacker()==kFALSE) return kFALSE;
  r=getNextFile();

  HldUnpack *unpacker;
  while ( (unpacker=(HldUnpack *)iter->Next()) != 0 ) {
    fReadEvent->appendSubEvtIdx( (*unpacker) );
  }
  INFO_msg(10,HMessageMgr::DET_ALL,"initailized");
  return r;
}

Int_t HldFileSourceBase::getCurrentRunId(void) {
  Int_t r=-1;
  HldFileDesc *file=(fCurrentFile)?fCurrentFile:(HldFileDesc *)fFileList.First();
  if (file) {
    r=file->getRunId();
  }

  return r;
}


Int_t HldFileSourceBase::getCurrentRefId(void) {
  Int_t r=-1;
  HldFileDesc *file=(fCurrentFile) ?
                    fCurrentFile : (HldFileDesc *)fFileList.First();
  if (file) {
    r=file->getRefId();
  } 

  return r;
}


EDsState HldFileSourceBase::getNextEvent() {
  // loops over the event files in the runtime database
  // Tries to read a new event from the LMD file and put the info into the 
  // HEvent structure:

  if(isDumped) {
    if (dumpEvt()==kDsError) {
      if (getNextFile()==kFALSE) return kDsEndData;
      else return kDsEndFile;
    }
    return kDsOk;
  }

  if (isScanned) {
    if (scanEvt()==kDsError) {
      if (getNextFile()==kFALSE) return kDsEndData;
      else return kDsEndFile;
    }
    return kDsOk; 
  }  

  fEventNr++;

  if ( !((HldFilEvt*)fReadEvent)->execute() || !(fEventNr<fEventLimit)) {
    //End of current file
    fEventNr = 0;
    if (getNextFile()==kFALSE) return kDsEndData;
    else return kDsEndFile;
  }

  decodeHeader((*fEventAddr)->getHeader());
  iter->Reset();
  HldUnpack *unpacker;
  while ( (unpacker=(HldUnpack *)iter->Next())!=NULL) {
    if (!unpacker->execute()) return kDsError;
  }
  return kDsOk;
}
 


void HldFileSourceBase::addFile(Text_t *fileName, Text_t *refFile) { 
  Int_t r=-1;
  TString rf=fCurrentDir+"/";
  rf+=refFile;
  r=getRunId(rf.Data());
  if (r==-1) Error("addFile","File %s not found\n",rf.Data());
  else addFile(fileName,r);
}








