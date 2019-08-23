//*-- AUTHOR : Vladimir Pechenov
//*-- Modified : 08/02/2007 by V.Pechenov
//*-- Modified : 05/06/2002 by V.Pechenov
//*-- Modified : 30/05/2001 by V.Pechenov
//*-- Modified : 7/11/2000 by V.Pechenov


//_HADES_CLASS_DESCRIPTION 
////////////////////////////////////////////////////////////////
// HMdcClusSim
//
// Data object for Mdc cluster.
//
//  Int_t getTrackIndex(Int_t tr) - return geant track "tr" index
//      in listTr[5] array or -1 if there no track "tr" in the array.
//
// Int_t getNTracks() - num. of tracks which have hits in cluster
//      (5 tracks maximum) (niese don't increase tracks counter)
//
// Int_t getTrack(Int_t indx) - return track number.
//      indx= 0, ..., (getNTracks()-1).
//      List of tracks is sorted: first track (indx=0) 
//      have most number of hits, ...
//      Track_number==-9 - wires produced due to geant bug
//      Track_number==gHades->getEmbeddingRealTrackId() -
//          real track embeded in simulation.
//
// Int_t getNoiseIndex(void) - return index of noise wires inf.
//      if index < 0 - no noise in cluster.
//      Track number for noise wires =-99
//      Example:
//      Int_t ind = clus->getNoiseIndex();
//      if(ind>0) {
//        clus->getTrack(ind);         // will return -99
//        clus->getNTimesInTrack(ind); // number of noise wires
//        ...
//      }
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
// Number of noise wires is kept in nTimes[4]:
//      if listTracks[4]=-99 then nTimes[4] - number of niesed wires
//
////////////////////////////////////////////////////////////////
using namespace std;
#include "hmdcclussim.h"
#include "hades.h"
#include "hmdctrackddef.h"
#include "hmdccal1sim.h"
#include "hevent.h"
#include "hmatrixcategory.h"
#include "hmdclistcells.h"
#include <iostream> 
#include <iomanip>

ClassImp(HMdcClusSim)


void HMdcClusSim::setDef(void) {
  nTracks=0;
  nTracksM[0]=nTracksM[1]=0;
  listTr[4]=-99;    // [4] - for noise wires
  nTimes[4]=0;      // ...
  nLayers[4][0]=nLayers[4][1]=0;
}

void HMdcClusSim::setTrack(Int_t ind, Int_t tr, Short_t nTm,
    UChar_t lM1, UChar_t lM2) {
  if(ind<0 || ind>4) return;
  listTr[ind]     = tr;    
  nTimes[ind]     = nTm;    
  nLayers[ind][0] = lM1;
  nLayers[ind][1] = lM2;
}

void HMdcClusSim::setNTracksM(Int_t modi, Int_t nTrs) {
  if(modi<0 || modi>1) return;
  nTracksM[modi] = nTrs;
}

void HMdcClusSim::setTrackM(Int_t modi, Int_t ind, Int_t tr, Short_t nTm) {
  if(modi<0 || modi>1) return;
  if(ind<0 || ind>4) return;
  listTrM[modi][ind] = tr;
  nTimesM[modi][ind] = nTm;
}


Int_t HMdcClusSim::getNoiseIndex(void) const {
  return (listTr[4] == -99 && nTimes[4] > 0) ? 4:-1;
}
    
Int_t HMdcClusSim::getTrackIndex(Int_t tr) const {
  for(Int_t i=0;i<nTracks;i++) if(listTr[i]==tr) return i;
  return -1;
}

Int_t HMdcClusSim::getTrackMod(Int_t m,Int_t indx) const {
  return (indx>=0 && indx<nTracksM[m&1]) ? listTrM[m&1][indx] : -1;
}

Short_t HMdcClusSim::getNTimesInMod(Int_t m,Int_t indx) const {
  return (indx<0 || indx>=nTracksM[m&1]) ? 0:nTimesM[m&1][indx];
}


Int_t   HMdcClusSim::getTrack(Int_t indx) const {
  return (indx>=0&&indx<nTracks) ? listTr[indx] : 0;
}

Short_t HMdcClusSim::getNTimesInTrack(Int_t indx) const {
  return (indx<0||indx>=nTracks) ? 0:nTimes[indx];
}

void HMdcClusSim::calcTrList(void) {
  HCategory *fCalCat=gHades->getCurrentEvent()->getCategory(catMdcCal1);
  if( !fCalCat ) return;

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
        if(track == 0) continue;
        Int_t indx=nTr;
        if(track<0 && track!=gHades->getEmbeddingRealTrackId()) {
          nTimes[4]++;
          nLayers[4][modInd] |= add;
        } else {
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
  nTracks=(nTr>4) ? 4:nTr;
  for(Int_t n=0; n<4; n++) {
    if(n<nTracks) {
      listTr[n]=lsTr[ind[n]];
      nTimes[n]=nHt[ind[n]];
      nLayers[n][0]=nLay[ind[n]][0];
      nLayers[n][1]=nLay[ind[n]][1];
    } else {
      listTr[n]=0;
      nTimes[n]=0;
      nLayers[n][0]=0;
      nLayers[n][1]=0;
    }
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
  if(fl) HMdcClus::printCont(fl);
}
