//*-- Author : Manuel Sanchez 
//*-- Modified : 30/03/98 by Manuel Sanchez
//*-- Copyright : GENP (Univ.Santiago de Compostela)
//*-- Modified : 26/11/98 by Ilse Koenig
//*-- Modified : 23/3/99 by D.B. (Dump of events)

//////////////////////////////////////////////////////////////////////////////
//HLdSource
//
//  Base class for a "data source" which delivery LMD data.
//  The derived class HldFileSource reads the data from a file on disk and
//  the derived class HldRemoteSource getrs the data directly from the
//  Data Adquisition.
//
//  The data in the file need to be unpacked. The user must specify the list 
//  of unpackers (see HldUnpack) to use, this way the user can select which 
//  parts of the data will be retrieved. That also give support to the case of 
//  LMD files not containing all the possible types of data (Rich data, 
//  Mdc data...)
//
//  An example would be:
//  HLmdFileSource *lmdSource=new HLmdFileSource();
//  lmdSource->addUnpacker(new HRichUnpacker);
//
//  This way you only get the Rich data in the files specified in the runtime
//  database
//
///////////////////////////////////////////////////////////////////////////////

#include "hldsource.h"
#include "hldevt.h"
#include "hldsubevt.h"
#include "hrecevent.h"
#include "hades.h"

ClassImp(HldSource)

HldSource::HldSource(void) {
  // Default constructor for a Lmd data source.
  fReadEvent=0;
  fUnpackerList=new TList();
  isDumped=kFALSE;   
  isScanned=kFALSE;
}

HldSource::~HldSource(void) {
  //Destructor for a LMD data source
  if (fUnpackerList) {
                      fUnpackerList->Delete();
                      delete fUnpackerList;
  }
  if (fReadEvent) delete fReadEvent;
}

void HldSource::addUnpacker(HldUnpack *unpacker) {
  // adds an unpacker to the list of unpackers for a LMD source
  fUnpackerList->Add(unpacker);
}

Bool_t HldSource::initUnpacker(void) {
  //Calls the init() function for each unpacker.
  TIter next(fUnpackerList);
  HldUnpack *unpacker;
  if (!(*fEventAddr)) {
    (*fEventAddr)=new HRecEvent;
  }
  while ( (unpacker=(HldUnpack *)next())!=NULL) {
    if (!unpacker->init()) return kFALSE;
  }
  return kTRUE;
}


Bool_t HldSource::finalizeUnpacker(void) {
  //Calls the init() function for each unpacker.
  TIter next(fUnpackerList);
  HldUnpack *unpacker;
  if (!(*fEventAddr)) {
    (*fEventAddr)=new HRecEvent;
  }
  while ( (unpacker=(HldUnpack *)next())!=NULL) {
    if (!unpacker->finalize()) return kFALSE;
  }
  return kTRUE;
}

Bool_t HldSource::finalize(void) {
  return finalizeUnpacker();
}

void HldSource::decodeHeader(void) {
  //Decodes the event's header
  HEventHeader *dest;
  dest=((*fEventAddr)->getHeader()); //CHECKME: 
  // dest= gHades->getCurrentEvent()->getHeader();
  dest->setEventSize(fReadEvent->getSize());
  dest->setEventDecoding(fReadEvent->getDecoding());
  dest->setId(fReadEvent->getId());
  dest->setEventSeqNumber(fReadEvent->getSeqNr());
  dest->setDate(fReadEvent->getDate());
  dest->setTime(fReadEvent->getTime());
  dest->setEventFileNumber(fReadEvent->getRunNr());
  dest->setEventPad(fReadEvent->getPad());
}


EDsState HldSource::scanIt(HldEvt *evt) {
  // dump the whole evt on the screen  
  Bool_t r=kTRUE;

  if (!evt->scanHdr()) r=kFALSE;

  UInt_t *data=evt->getData();
  if (!data) {printf("*** No subevent in this event ****\n"); 
              return kDsOk;
  } 
  
  UInt_t *end=evt->getEnd(); 
  HldSubEvt subevt;
  while ( data < end ){ 
    subevt= HldSubEvt(data);
    if (!subevt.scanIt()) r=kFALSE; 
    data= subevt.getPaddedEnd();
  }
  if (!r) return kDsError;
  return kDsOk;
}

EDsState HldSource::showIt(HldEvt *evt){
  // Scan the event.  
  printf("*******************************************************************\n
");
  evt->dumpHdr();
  printf("*******************************************************************\n"
);
  UInt_t *data=evt->getData();
  if (!data) {printf("*** No subevent in this event ****\n");
              return kDsOk;
  }

  UInt_t *end=evt->getEnd();
  HldSubEvt subevt;
     while ( data < end ){
                subevt= HldSubEvt(data);
                subevt.dumpIt();
                data= subevt.getPaddedEnd();
         }
  return kDsOk;    
}


EDsState HldSource::dumpEvt(void) {
 if (!fReadEvent) return kDsError; 
     else{
    if(!(fReadEvent->swap()) ) return kDsError;  
     return showIt(fReadEvent);   
    
    } 
}

EDsState HldSource::scanEvt(void) {
 if (!fReadEvent) return kDsError; 
     else{
    if(!(fReadEvent->swap()) ) return kDsError;  
     return scanIt(fReadEvent);   
    
    } 
}





