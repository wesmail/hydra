//*-- AUTHOR : Vladimir Pechenov
//*-- Modified : 7/11/2000 by V.Pechenov


////////////////////////////////////////////////////////////////
// HMdcClusSim
//
// Data object for Mdc cluster.
//
// Int_t getNTracks() - num. of tracks in claster
//                (track have 4 or more hits in each module)
// Int_t getTrack(indx) - return track number
//
// Short_t getNHitsInTrack(indx) - num. of hits in cluster
//                               from track getTrack(indx)
//
////////////////////////////////////////////////////////////////

#include "hmdcclussim.h"
#include "hades.h"
#include "hmdctrackddef.h"
#include "hmdccal1sim.h"
#include "hevent.h"
#include "hcategory.h"
#include <iostream.h>

ClassImp(HMdcClusSim)

Int_t HMdcClusSim::getNTracks() {
  if(nTracks>=0) return nTracks;
  HCategory *fCalCat=gHades->getCurrentEvent()->getCategory(catMdcCal1);
  if( !fCalCat ) {
    nTracks=0;
    return 0;
  }
  HLocation loc;
  loc.set(4,sec,0,0,0);
  Int_t nTr=0;
  Int_t lsTr[100];
  UChar_t  nLay[100][2];
  UChar_t nHt[100];
  for(Int_t lay=0; lay<12; lay++) {
    Int_t mod=seg*2+lay/6;
    Int_t cell=-1;
    Short_t add=1<<(lay%6);
    loc[1]=mod;
    loc[2]=lay%6;
    while((cell=next(lay,cell)) >= 0) {
      UChar_t nTimes=getTime(lay,cell);
      loc[3]=cell;
      HMdcCal1Sim *sim=(HMdcCal1Sim*)fCalCat->getSlot(loc);
      Int_t track=0;
      for(Int_t t=0; t<2; t++) {
        if(t==0 && (nTimes&1)) track=sim->getNTrack1();
        else if(t==1 && (nTimes&2)) track=sim->getNTrack2();
        else continue;
        Int_t indx=nTr;
        for(Int_t n=0; n<nTr; n++) {
          if(lsTr[n]==track) {
            indx=n;
            break;
          }
        }
        if(indx==nTr) {
          lsTr[indx]=track;
          nLay[indx][0]=nLay[indx][1]=0;
          nHt[indx]=0;
          nTr++;
        }
        nLay[indx][mod&1] |= add;
        nHt[indx]++;
      }
    }
  }
  nTracks=0;
  for(Int_t n=0; n<nTr; n++) {
    if(nTracks==5) {
      Error("setTrack"," The number of tracks in cluster >10");
      break;
    }
    Int_t nLm1=HMdcBArray::getNSet(nLay[n][0]);
    Int_t nLm2=HMdcBArray::getNSet(nLay[n][1]);
    if( (minCl[0]>0 && nLm1<4) || (minCl[1]>0 && nLm2<4) ) continue;
    listTr[nTracks]=lsTr[n];
    nHits[nTracks]=nHt[n];
    nTracks++;
  }
  return nTracks;
}

Int_t HMdcClusSim::getTrack(Int_t indx) {
  return (indx>=0 && indx<getNTracks()) ? listTr[indx] : -1;
}

Short_t HMdcClusSim::getNHitsInTrack(Int_t indx) {
  return (indx>=0 && indx<getNTracks()) ? nHits[indx] : 0;
}

void HMdcClusSim::printCont() {
  Int_t nTr=getNTracks();  // If nTracks wasn't calc. it will be done 
  printf("  GEANT: %i tracks in cluster:",nTr);
  for(Int_t n=0; n<nTr; n++) {
    printf(" %i(%i)",listTr[n],nHits[n]);
  }
  printf("\n");
  HCategory *fCalCat=gHades->getCurrentEvent()->getCategory(catMdcCal1);
  if( !fCalCat ) return;
  HLocation loc;
  for(Int_t lay=0; lay<12; lay++) {
    Int_t mod=seg*2+lay/6;
    Int_t nCells=getNCells(lay);
    if(!nCells) continue;
    printf("  M.%i L.%i  %i cells:",mod+1,lay%6+1,nCells);
    Int_t cell=-1;
    while((cell=next(lay,cell)) >= 0) {
      Int_t nTimes=getTime(lay,cell);
      printf(" %i(%i)[",cell+1,HMdcBArray::getNSet(nTimes));
      loc.set(4,sec,mod,lay%6,cell);
      HMdcCal1Sim *sim=(HMdcCal1Sim*)fCalCat->getSlot(loc);
      if(nTimes&1) printf("%i",sim->getNTrack1());
      if(nTimes&2) printf(",%i",sim->getNTrack2());
      printf("]");
    }
    printf("\n");
  }
}
