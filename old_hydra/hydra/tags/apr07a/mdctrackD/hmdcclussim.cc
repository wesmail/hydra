//*-- AUTHOR : Vladimir Pechenov
//*-- Modified : 05/06/2002 by V.Pechenov
//*-- Modified : 30/05/2001 by V.Pechenov
//*-- Modified : 7/11/2000 by V.Pechenov


//_HADES_CLASS_DESCRIPTION 
////////////////////////////////////////////////////////////////
// HMdcClusSim
//
// Data object for Mdc cluster.
//
// Int_t getNTracks() - num. of tracks which have hits in cluster
//     (5 tracks maximum)
// Int_t getTrack(Int_t indx) - return track number.
//      indx= 0, ..., (getNTracks()-1).
//      List of tracks is sorted: first track (indx=0) 
//      have most number of hits, ...
//
// Short_t getNTimesInTrack(Int_t indx) - num. of hits
//      in cluster from track getTrack(indx)
//
// Int_t getNLayersInTrack(Int_t indx, Int_t m=-1) num. layers 
//      with hits from track getTrack(indx):
//      m=0 - in first module of segment,
//      m=1 - in second module of segment,
//      m!=0 && m!=1 - in segment
//
////////////////////////////////////////////////////////////////
using namespace std;
#include "hmdcclussim.h"
#include "hades.h"
#include "hmdctrackddef.h"
#include "hmdccal1sim.h"
#include "hevent.h"
#include "hmatrixcategory.h"
#include <iostream> 
#include <iomanip>

ClassImp(HMdcClusSim)

void HMdcClusSim::calcTrList(void) {
  HCategory *fCalCat=gHades->getCurrentEvent()->getCategory(catMdcCal1);
  if( !fCalCat ) {
    nTracks=0;
    return;
  }
  
  HLocation loc;
  loc.set(4,sec,0,0,0);
  Int_t nTr=0;
  Int_t lsTr[100];
  Int_t ind[100];
  UChar_t  nLay[100][2];
  UChar_t nHt[100];
  for(Int_t lay=0; lay<12; lay++) {
    Int_t modInd=lay/6;
    Int_t cell=-1;
    Short_t add=1<<(lay%6);
    loc[1]=modInd+seg*2;
    loc[2]=lay%6;
    while((cell=next(lay,cell)) >= 0) {
      UChar_t nTms=getTime(lay,cell);
      loc[3]=cell;
      HMdcCal1Sim *sim=(HMdcCal1Sim*)fCalCat->getObject(loc);
      if(!sim) {
        Error("getNTracks()",
            " Can't get object HMdcCal1Sim for address [%i][%i][%i][ %i ]",
            loc[0]+1,loc[1]+1,loc[2]+1,loc[3]+1);
        continue;
      }
      Int_t track=0;
      for(Int_t t=0; t<2; t++) {
        if(t==0 && (nTms&1)) track=sim->getNTrack1();
        else if(t==1 && (nTms&2)) track=sim->getNTrack2();
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
          ind[indx]=indx;
          nTr++;
        }
        nLay[indx][modInd] |= add;
        nHt[indx]++;
      }
    }
  }
  //Sorting:
  if(nTr>1) {
    Bool_t sort=kTRUE;
    while(sort) {
      sort=kFALSE;
      for(Int_t n=0; n<nTr-1; n++) {
        if(nHt[ind[n]]<nHt[ind[n+1]]) {
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
    listTr[n]=lsTr[ind[n]];
    nTimes[n]=nHt[ind[n]];
    nLayers[n][0]=nLay[ind[n]][0];
    nLayers[n][1]=nLay[ind[n]][1];
  }
}

void HMdcClusSim::calcTrListMod(HMdcList12GroupCells& list,Int_t m) {
  HCategory *fCalCat=gHades->getCurrentEvent()->getCategory(catMdcCal1);
  if( !fCalCat ) {
    nTracksM[0]=nTracksM[1]=0;
    return;
  }
  Int_t mod=m&1;
  
  HLocation loc;
  loc.set(4,sec,0,0,0);
  Int_t nTr=0;
  Int_t lsTr[100];
  Int_t ind[100];
  UChar_t nHt[100];
  for(Int_t lay=mod*6; lay<(mod+1)*6; lay++) {
    Int_t modInd=lay/6;
    Int_t cell=-1;
    loc[1]=modInd+seg*2;
    loc[2]=lay%6;
    while((cell=list.next(lay,cell)) >= 0) {
      UChar_t nTms=list.getTime(lay,cell);
      loc[3]=cell;
      HMdcCal1Sim *sim=(HMdcCal1Sim*)fCalCat->getObject(loc);
      if(!sim) {
        Error("getNTracks()",
            " Can't get object HMdcCal1Sim for address [%i][%i][%i][ %i ]",
            loc[0]+1,loc[1]+1,loc[2]+1,loc[3]+1);
        continue;
      }
      Int_t track=0;
      for(Int_t t=0; t<2; t++) {
        if(t==0 && (nTms&1)) track=sim->getNTrack1();
        else if(t==1 && (nTms&2)) track=sim->getNTrack2();
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
          nHt[indx]=0;
          ind[indx]=indx;
          nTr++;
        }
        nHt[indx]++;
      }
    }
  }
  //Sorting:
  if(nTr>1) {
    Bool_t sort=kTRUE;
    while(sort) {
      sort=kFALSE;
      for(Int_t n=0; n<nTr-1; n++) {
        if(nHt[ind[n]]<nHt[ind[n+1]]) {
          sort=kTRUE;
          Int_t i=ind[n];
          ind[n]=ind[n+1];
          ind[n+1]=i;
        }
      }
    }
  }
  nTracksM[mod]=(nTr>5) ? 5:nTr;
  for(Int_t n=0; n<nTracksM[mod]; n++) {
    listTrM[mod][n]=lsTr[ind[n]];
    nTimesM[mod][n]=nHt[ind[n]];
  }
}

Int_t HMdcClusSim::getNLayersInTrack(Int_t indx, Int_t m) const {
  if(indx<0 || indx>=nTracks) return 0;
  if(m==0 || m==1) return HMdcBArray::getNSet(nLayers[indx][m]);
  else return HMdcBArray::getNSet(nLayers[indx][0])+
              HMdcBArray::getNSet(nLayers[indx][1]);
}

void HMdcClusSim::printCont(Bool_t fl) const {
  printf("  GEANT: %i tracks in cluster:",nTracks);
  for(Int_t n=0; n<nTracks; n++) printf(" %i(%i)",listTr[n],nTimes[n]);
  printf("\n");
  for(Int_t m=0;m<2;m++) {
    if(nTracksM[m]<=0) continue;
    printf("         %i tracks in Mod%i:",nTracksM[m],m+seg*2+1);
    for(Int_t n=0; n<nTracksM[m]; n++) 
      printf(" %i(%i)",listTrM[m][n],nTimesM[m][n]);
    printf("\n");
  }
//  HMatrixCategory *fCalCat=0;
//   if(gHades) fCalCat=
//       (HMatrixCategory*) gHades->getCurrentEvent()->getCategory(catMdcCal1);
//  if(!fCalCat) {
    if(fl) HMdcClus::printCont(fl);
//    return;
//  }
//   HLocation loc;
//   for(Int_t lay=0; lay<12; lay++) {
//     Int_t mod=seg*2+lay/6;
//     Int_t nCells=getNCells(lay);
//     if(!nCells) continue;
//     printf("  M.%i L.%i  %i cells:",mod+1,lay%6+1,nCells);
//     Int_t cell=-1;
//     while((cell=next(lay,cell)) >= 0) {
//       Int_t nTms=getTime(lay,cell);
//       printf(" %i(%i)[",cell+1,HMdcBArray::getNSet(nTms));
//       loc.set(4,sec,mod,lay%6,cell);
//       HMdcCal1Sim *sim=(HMdcCal1Sim*)fCalCat->getObject(loc);
//       if(nTms&1) printf("%i",sim->getNTrack1());
//       if(nTms&2) printf(",%i",sim->getNTrack2());
//       printf("]");
//     }
//     printf("\n");
//   }
}
