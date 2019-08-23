//*-- Author : Manuel Sanchez 
//*-- Modified : 30/03/98 by Manuel Sanchez
//*-- Copyright : GENP (Univ.Santiago de Compostela)
//*-- Modified : 26/11/98 by Ilse Koenig

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
#include "hrecevent.h"
#include "hades.h"

ClassImp(HldSource)

HldSource::HldSource(void) {
  // Default constructor for a Lmd data source.
  fReadEvent=0;
  fUnpackerList=new TList();  
}

HldSource::~HldSource(void) {
  //Destructor for a LMD data source
  if (fUnpackerList) fUnpackerList->Delete();
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
