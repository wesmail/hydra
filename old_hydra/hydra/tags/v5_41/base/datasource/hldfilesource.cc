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

#include "hldfilesource.h"
#include "hrecevent.h"
#include "hldfilevt.h"
#include "hades.h"
#include "hruntimedb.h"
#include <iostream.h>
#include <unistd.h>
#include <limits.h>

ClassImp(HldFileDesc)
ClassImp(HldFileSource)

HldFileSource::HldFileSource() : HldSource() {
  // Default constructor for a Lmd file data source.
  fReadEvent=new HldFilEvt;
  iter=0;
  fCurrentFile=0;
  fEventNr = 0;
  fEventLimit = INT_MAX;
}


HldFileSource::~HldFileSource(void) {
  // Destructor for a LMD file data source
  if (fReadEvent) delete fReadEvent;
  fReadEvent=0;
  if (iter) delete iter;
}


HldFileSource::HldFileSource(HldFileSource &so) {
  fFileList.AddAll(&so.fFileList);
  if (so.fCurrentFile) {
    fCurrentFile=(HldFileDesc *)fFileList.FindObject(so.fCurrentFile->GetName());
    iter=0;
  }
  fReadEvent=so.fReadEvent; //FIXME: Do a real copy!!
  fEventNr = 0;
  fEventLimit = INT_MAX;
}


Bool_t HldFileSource::init() {
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
  return r;
}


Int_t HldFileSource::getRunId(const Text_t *fileName) {
  HldFilEvt evt;
  Int_t r=-1;

  if (fCurrentFile && strcmp(fileName,fCurrentFile->GetName())==0) {
    r=fCurrentFile->getRunId();
  } else {
    if (!evt.setFile(fileName)) r=-1;
    else {
      evt.read();
      r=evt.getRunNr();
    }
  }
  return r;
}


Int_t HldFileSource::getCurrentRunId(void) {
  Int_t r=-1;
  HldFileDesc *file=(fCurrentFile)?fCurrentFile:(HldFileDesc *)fFileList.First();
  if (file) {
    r=file->getRunId();
  } 
    
  return r;
}


Int_t HldFileSource::getCurrentRefId(void) {
  Int_t r=-1;
  HldFileDesc *file=(fCurrentFile) ?
                    fCurrentFile : (HldFileDesc *)fFileList.First();
  if (file) {
    r=file->getRefId();
  } 

  return r;
}


EDsState HldFileSource::getNextEvent() {
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
 

Bool_t HldFileSource::getNextFile() {
  if (!fCurrentFile) {
    if (fFileList.IsEmpty()) return kFALSE;
    else fCurrentFile=(HldFileDesc *)fFileList.First();
  } else {
    fCurrentFile=(HldFileDesc *)fFileList.After(fCurrentFile);
    if (!fCurrentFile) return kFALSE;
  }

  ((HldFilEvt *)fReadEvent)->setFile(fCurrentFile->GetName());
  
  return kTRUE;
}


void HldFileSource::addFile(Text_t *fileName, Text_t *refFile) { 
  Int_t r=-1;
  TString rf=fCurrentDir+"/";
  rf+=refFile;
  r=getRunId(rf.Data());
  if (r==-1) Error("addFile","File %s not found\n",rf.Data());
  else addFile(fileName,r);
}


void HldFileSource::addFile(Text_t *name,Int_t refId) {
  HldFileDesc *desc=0;
  HldFileDesc *newDesc=0;
  TString fileName;
  fileName=fCurrentDir+"/"+name;
  desc=(HldFileDesc *)fFileList.FindObject(fileName);
  if (desc) {
    if (refId==-1) 
      newDesc=new HldFileDesc(fileName.Data(),desc->getRunId(),desc->getRefId());
    else
      newDesc=new HldFileDesc(fileName.Data(),desc->getRunId(),refId);
    fFileList.Add(newDesc);
    Warning("addFile","File %s added more than once\n",fileName.Data());
  } else {
    if (access(fileName.Data(),F_OK)!=0) { // File does not exist
      Error("addFile","File %s not found\n",fileName.Data());
    } else {
      newDesc=new HldFileDesc(fileName.Data(),getRunId(fileName),refId);
      fFileList.Add(newDesc);
    }
  }
}




