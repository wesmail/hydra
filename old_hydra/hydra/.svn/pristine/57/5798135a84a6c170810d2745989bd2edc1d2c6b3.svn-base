// modified  3-12-1998 D.B
// modified  15/3/99 by R. Holzmann
// modified  23/3/99 by D.B
//_HADES_CLASS_DESCRIPTION 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//HLdremoteSource
//
//  This is a particular kind of "data source"  (see HDataSource) which takes 
//the event's data from  directly from a Data event builder.using RPC.
//
//  The data are accessed via a RPC buffer pointer and 
//  need to be unpacked.  For This user must specify the 
// node of the SERVER as argument in the constructor of this class. 
// Unpacking data is then similar to other data source.(see HldSource).
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
using namespace std;
#include "hldremotesource.h"
#include "hldremotevt.h"
#include "hrecevent.h"
#include "hruntimedb.h"
#include "hldevt.h"
#include "hldsubevt.h"
#include "hldunpack.h"  
#include <iostream> 
#include <iomanip>

ClassImp(HldRemoteSource)

HldRemoteSource::HldRemoteSource(void): HldSource() {
	refId=-1;
}

HldRemoteSource::HldRemoteSource(Text_t *nodeName): HldSource(){
  // test Server IO
   
   fReadEvent=new HldRemotEvt(nodeName);
  currNodeName=nodeName;
  iter=0;
   iter=fUnpackerList->MakeIterator();
  isDumped=kFALSE;   
  refId=-1;
}


HldRemoteSource::~HldRemoteSource(void) {
  //Destructor for a remote data source
   if (fUnpackerList) fUnpackerList->Delete();
  fUnpackerList=0;
  if (fReadEvent) delete fReadEvent;
  fReadEvent=0;
  if(iter) delete iter;
}


Bool_t HldRemoteSource::init(void) {
  //Calls the init() function for each unpacker.
 if (!(*fEventAddr)) {
   (*fEventAddr) = new HRecEvent;
 }
 if(isDumped || isScanned) return kTRUE;
 iter=fUnpackerList->MakeIterator();
 if (initUnpacker()==kFALSE) {
    printf(" problem init unpacker \n");
    return kFALSE;
 }
 HldUnpack* unpacker;
 while ( (unpacker=(HldUnpack *)iter->Next())!=NULL) {
       fReadEvent->appendSubEvtIdx(*unpacker);      
 }  
 return kTRUE;  
}

Int_t HldRemoteSource::getCurrentRunId(void) {
  ((HldRemotEvt*)fReadEvent)->read();
  return fReadEvent->getRunNr();
}


EDsState HldRemoteSource::getNextEvent(Bool_t doUnpack) {
 // test Server IO
  if(isDumped) return dumpEvt();
  if(isScanned) return scanEvt();

 iter->Reset();
 HldUnpack *unpacker;
 if (!fReadEvent) return kDsError;
  if (fReadEvent) fReadEvent->execute();
	if(doUnpack){
    decodeHeader((*fEventAddr)->getHeader());
    while ( (unpacker=(HldUnpack *)iter->Next())!=NULL) {
      if (!unpacker->execute()) return kDsError;
    }
	}
return kDsOk;
}

