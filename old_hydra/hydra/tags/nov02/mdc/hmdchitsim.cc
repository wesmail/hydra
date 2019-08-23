//*-- AUTHOR : Vladimir Pechenov                                                         
//*-- Modified : 31.05.2001 by Vladimir Pechenov

///////////////////////////////////////////////////////////////////////////////
//  HMdcHitSim
// 
// This class is derived from HMdcHit for simulated events.
// Additional data elements are following:
//   nTracks - the number of tracks in the list of tracks numbers
//   listTracks[5] - the list of tracks numbers
//   nTimes[5] - num. of drift times from each track in this hit
//     (not cells because one cell can have 2 drift times from 2 tracks)
// This information can be accessed via the functions:
//    Int_t getNTracks(void)
//    Int_t getTrack(const Int_t n)      0 <= n < getNTracks()
//    UChar_t getNTimes(const Int_t n)
//
// The list of tracks is sorted. listTracks[0] is a track which have
// most number of drift times and etc.
// 
// The list of tracks is calculated after filling of HMdcHit container
// via function  Int_t calcNTracks(void) which return nTracks.
// 
///////////////////////////////////////////////////////////////////////////////

#include "hmdchitsim.h"
#include "hades.h"
#include "hmdcdef.h"
#include "hmdccal1sim.h"
#include "hevent.h"
#include "hcategory.h" 
#include "hmessagemgr.h"

ClassImp(HMdcHitSim)

void HMdcHitSim::setNTracks(const Int_t nTr, const Int_t* listTr, 
    const UChar_t* nTm) {
  if(nTr<=0) nTracks=-1;
  else {
    nTracks=(nTr<6) ? nTr:5;
    for(Int_t n=0; n<nTracks; n++) {
      listTracks[n]=listTr[n];
      nTimes[n]=nTm[n];
    }
  }
}

Int_t HMdcHitSim::calcNTracks(void) {
  HCategory *fCalCat=gHades->getCurrentEvent()->getCategory(catMdcCal1);
  if( !fCalCat ) {
    nTracks=0;
    return -1;
  }
  HLocation loc;
  loc.set(4,sector,module,0,0);
  Int_t lsTr[48];
  Int_t nTimesTot=0;
  for(Int_t layer=0; layer<6; layer++) {
    loc[2]=layer;
    for(Int_t n=0; n<4; n++) {
      UChar_t nCTimes=getSignId(layer,n);
      if(!nCTimes) continue;
      loc[3]=getCell(layer,n);
      HMdcCal1Sim *fCal1Sim=(HMdcCal1Sim*)fCalCat->getObject(loc);
      if(nCTimes&1) lsTr[nTimesTot++]=fCal1Sim->getNTrack1();
      if(nCTimes&2) lsTr[nTimesTot++]=fCal1Sim->getNTrack2();
    }
  }
  Int_t nTr=0;
  Int_t ind[48];
  UChar_t nTm[48];  
  for(Int_t n=0; n<nTimesTot; n++) {
    if(lsTr[nTr]<=0) continue;
    lsTr[nTr]=lsTr[n];
    nTm[nTr]=1;
    ind[nTr]=nTr;
    for(Int_t m=n+1; m<nTimesTot; m++) {
      if(lsTr[m] != lsTr[nTr]) continue;
      nTm[nTr]++;
      lsTr[m]=-1;
    }
    nTr++;
  }
  //Sorting:
  if(nTr>1) {
    Bool_t sort=kTRUE;
    while(sort) {
      sort=kFALSE;
      for(Int_t n=0; n<nTr-1; n++) {
        if(nTm[ind[n]]<nTm[ind[n+1]]) {
          sort=kTRUE;
          Int_t i=ind[n];
          ind[n]=ind[n+1];
          ind[n+1]=i;
        }
      }
    }
  }
  nTracks=(nTr>5) ? 5:nTr;
  for(Int_t n=0; n<nTracks; n++) {
    listTracks[n]=lsTr[ind[n]];
    nTimes[n]=nTm[ind[n]];
  }
  return nTracks;
}

void HMdcHitSim::print(void) {
    gHades->getMsg()->info(10,DET_MDC,GetName()
			   ,"Sec.%i Mod.%i, %i tracks: ",
			   sector,module,nTracks);
  for(Int_t n=0; n<nTracks; n++)
      gHades->getMsg()->info(10,DET_MDC,GetName()
			     ,"%i(%i) ",listTracks[n],nTimes[n]);
  INFO_msg(10,DET_MDC,"[trNum(nOfTimes)]");
  HMdcHit::print();
}
