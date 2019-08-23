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
//      (5 tracks maximum) (noise don't increase tracks counter)
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
//      if listTracks[4]=-99 then nTimes[4] - number of noised wires
//
//
// Meaning of trackStatus[tr] bits:
// ------------------------------------
//  Bit 8: =0 - GEANT bug was found in this track
//  Bit 7: =0 - track don't reach META
//  Bit 6: =0 - not reconstructable track. It can be if
//              track not present in mdc,
//              track cross more then one sector,
//              numder of segments <1 or >2,
//              upstream direction of at list one segment,
//              track doesn't cross inner or outer segment,
//              num. of segments with digi.wires <1 or >2,
//              num.digi.wires in track < 5*num.segments,
//              at list one seg. has <3 layers or <5 wires
//  Bit 5: =0 - segment has <3 layers or <5 digi.wires
//  Bit 4: =0 - at list one mdc in segment has
//              <3 fired layers or <5 wires
//  Bit 3: =0 - segment can't be found due to high level
//              of cluster finder
//  Bit 2: =0 - fake contribution in cluster
//  Bit 1: =0 - fake inner-outer segments matching
//              (for outer segment only!)
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
  nTracks        = 0;
  nTracksM[0]    = 0;
  nTracksM[1]    = 0;
  listTr[4]      = -99;    // [4] - for noise wires
  nTimes[4]      = 0;      // ...
  nLayers[4][0]  = 0;
  nLayers[4][1]  = 0;
  nDigiTimes[4]  = 0;
  trackStatus[4] = 0;
  xGeant[4]      = 0.;
  yGeant[4]      = 0.;
  nNotFakeTracks = -1;
  ioSMatchStatus = -1;
}

void HMdcClusSim::cleanRest(void) {
  for(Int_t n=nTracks;n<4;n++) {
    listTr[n]      = 0;
    nTimes[n]      = 0;
    nLayers[n][0]  = 0;
    nLayers[n][1]  = 0;
    nDigiTimes[n]  = 0;
    trackStatus[n] = 0;
    xGeant[n]      = 0.;
    yGeant[n]      = 0.;
  }
}

Bool_t HMdcClusSim::resetTrack(Int_t ind, Int_t tr, Short_t nTm,
    UChar_t lM1, UChar_t lM2) {
  // reset geant track;
  if(indOk(ind)) {
    listTr[ind]     = tr;    
    nTimes[ind]     = nTm;    
    nLayers[ind][0] = lM1;
    nLayers[ind][1] = lM2;
    if(nTracks <= ind) nTracks = ind+1;
    return kTRUE;
  }
  return kFALSE;
}

Int_t HMdcClusSim::addTrack(Int_t tr, Short_t nTm,
    UChar_t lM1, UChar_t lM2) {
  // add geant track. Return index of track in array
  if(nTracks>=5) return -1;
  listTr[nTracks]     = tr;    
  nTimes[nTracks]     = nTm;    
  nLayers[nTracks][0] = lM1;
  nLayers[nTracks][1] = lM2;
  xGeant[nTracks]     = -10000.;
  yGeant[nTracks]     = -10000.;
  nTracks++;
  return nTracks-1;
}

void HMdcClusSim::setNDigiTimes(Int_t ind,Int_t nt) {
  if(indOk(ind)) {
    if(nt<0)        nDigiTimes[ind] = 0;
    else if(nt>255) nDigiTimes[ind] = 255;
    else            nDigiTimes[ind] = nt;
  }
}

void HMdcClusSim::setXYGeant(Int_t ind,Float_t xg,Float_t yg) {
  if(indOk(ind)) {
    xGeant[ind]=xg;
    yGeant[ind]=yg;
  }
}

void HMdcClusSim::setTrackStatus(Int_t ind,UChar_t status) {
  if(indOk(ind)) trackStatus[ind] = status;
}

UChar_t HMdcClusSim::getTrackStatus(Int_t ind) const {
  if(indOk(ind)) return trackStatus[ind];
  return 0;
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
          sort     = kTRUE;
          Int_t i  = ind[n];
          ind[n]   = ind[n+1];
          ind[n+1] = i;
        }
      }
    }
  }
  nTracks=(nTr>4) ? 4:nTr;
  for(Int_t n=0; n<nTracks; n++) {
    listTr[n]      = lsTr[ind[n]];
    nTimes[n]      = nHt[ind[n]];
    nLayers[n][0]  = nLay[ind[n]][0];
    nLayers[n][1]  = nLay[ind[n]][1];
    nDigiTimes[n]  = 0;
    trackStatus[n] = 255;
    xGeant[n]      = -10000.;
    yGeant[n]      = -10000.;
  }
  cleanRest();
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
  if(indOk(indx)) {
    if(m==0 || m==1) return HMdcBArray::getNSet(nLayers[indx][m]);
    else return HMdcBArray::getNSet(nLayers[indx][0])+
                HMdcBArray::getNSet(nLayers[indx][1]);
  }
  return 0;
}

UChar_t HMdcClusSim::getNDigiTimesInTrack(Int_t indx) const {
  if(indOk(indx)) return nDigiTimes[indx];
  return 0;
}

Bool_t HMdcClusSim::getXYGeant(Int_t i,Float_t& xg, Float_t& yg) const {
  if(indOk(i) && xGeant[i]>-9000.) {
    xg = xGeant[i];
    yg = yGeant[i];
    return kTRUE;
  }
  return kFALSE;
}

void HMdcClusSim::sortTrListByContr(void) {
  // Sorting track list "listTr" by "FakeContribution" flag.
  // After sorting tracks with not FakeContribution status
  // will be placed at the begining of arrais.
  nNotFakeTracks = 0;
  for(Int_t t1=0;t1<nTracks;t1++) {
    if(isFakeContribution(t1)) {
      for(Int_t t2=t1+1;t2<nTracks;t2++) {
        if(isFakeContribution(t2)) continue;
        Int_t   v1 = listTr[t1];
        Short_t v2 = nTimes[t1];
        UChar_t v3 = nLayers[t1][0];
        UChar_t v4 = nLayers[t1][1];
        UChar_t v5 = nDigiTimes[t1];
        UChar_t v6 = trackStatus[t1];
        Float_t v7 = xGeant[t1];
        Float_t v8 = yGeant[t1];

        listTr[t1]      = listTr[t2];
        nTimes[t1]      = nTimes[t2];
        nLayers[t1][0]  = nLayers[t2][0];
        nLayers[t1][1]  = nLayers[t2][1];
        nDigiTimes[t1]  = nDigiTimes[t2];
        trackStatus[t1] = trackStatus[t2];
        xGeant[t1]      = xGeant[t2];
        yGeant[t1]      = yGeant[t2];

        listTr[t2]      = v1;
        nTimes[t2]      = v2;
        nLayers[t2][0]  = v3;
        nLayers[t2][1]  = v4;
        nDigiTimes[t2]  = v5;
        trackStatus[t2] = v6;
        xGeant[t2]      = v7;
        yGeant[t2]      = v8;
        nNotFakeTracks++;
        break;
      }
    } else nNotFakeTracks++;
  }
}

void HMdcClusSim::printCont(Bool_t fl) const {
  printf("  GEANT: %i tracks in cluster",nTracks);
  if(nNotFakeTracks==0) printf(".  FAKE CLUSTER!\n");
  else  {
    if(nNotFakeTracks>=0 && nNotFakeTracks != nTracks) 
        printf(",  %i of them - fakes contribution",nTracks-nNotFakeTracks);
    if(ioSMatchStatus > 0)       printf(".  IO seg.matching - OK.\n");
    else if(ioSMatchStatus == 0) printf(".  IO seg.matching - FAKE!\n");
    else printf(".\n");
  }
  for(Int_t n=0; n<nTracks; n++) {
    if(listTr[n]==gHades->getEmbeddingRealTrackId())
        printf("%6i(%2iw.) Embeded real track\n",listTr[n],nTimes[n]);
    else if(listTr[n]<=0)
      printf("%6i(%2iw.) GEANT bug wires\n",listTr[n],nTimes[n]);
    else {
      printf("%6i(%2iw. from %2iw., lay.:",listTr[n],nTimes[n],nDigiTimes[n]);
      for(Int_t lay=0;lay<6;lay++) printf("%i",(nLayers[n][0]>>lay)&1);
      printf(" ");
      for(Int_t lay=0;lay<6;lay++) printf("%i",(nLayers[n][1]>>lay)&1);
      printf(")");
      if(xGeant[n]>-9000. && yGeant[n]>-9000.)
        printf(" dX=%.1f dY=%.1f",dX(n),dY(n));

      if(trackStatus[n] == 255)            printf(" Ok.");
      else {
        if((trackStatus[n]&2)   == 0)      printf(" FakeContribution");
        else {
          if((trackStatus[n]&1)      == 0) printf(" FakeIOSegMatch");
          if((trackStatus[n]&2)      == 0) printf(" FakeCluster");
          if((trackStatus[n]&4)      == 0) printf(" ClFnLevelTooHigh");
          if((trackStatus[n]&16)     == 0) printf(" SegNotRec");
          else if((trackStatus[n]&8) == 0) printf(" 1or2HitsNotRec");
          if((trackStatus[n]&32)     == 0) printf(" TrackNotRec");
          if((trackStatus[n]&64)     == 0) printf(" NoMeta");
          if((trackStatus[n]&128)    == 0) printf(" GEANT_Bug");
        }
      }
      printf("\n");
    }
  }
  if(nTracks<5 && nTimes[4]>0 && listTr[4]==-99) 
    printf("%6i(%2iw.) noised wires\n",listTr[4],nTimes[4]);
  for(Int_t m=0;m<2;m++) {
    if(nTracksM[m]<=0) continue;
    printf("         %i tracks in Mod%i:",nTracksM[m],m+seg*2+1);
    for(Int_t n=0; n<nTracksM[m]; n++) 
      printf(" %i(%i)",listTrM[m][n],nTimesM[m][n]);
    printf("\n");
  }
  if(fl) HMdcClus::printCont(fl);
}

void HMdcClusSim::Streamer(TBuffer &R__b) {
   // Stream an object of class HMdcClusSim.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      HMdcClus::Streamer(R__b);
      R__b >> nTracks;
      R__b.ReadStaticArray((int*)listTr);
      R__b.ReadStaticArray((short*)nTimes);
      R__b.ReadStaticArray((unsigned char*)nLayers);
      if (R__v > 1) {
        R__b.ReadStaticArray((unsigned char*)nDigiTimes);
        R__b.ReadStaticArray((unsigned char*)trackStatus);
        if(R__v > 2) {
          R__b.ReadStaticArray((float*)xGeant);
          R__b.ReadStaticArray((float*)yGeant);
          R__b >> nNotFakeTracks;
          R__b >> ioSMatchStatus;
        } else {
          R__b >> xGeant[0];
          R__b >> yGeant[0];
          setDefForOldVer(1);
        }
      } else setDefForOldVer(0);
      R__b.CheckByteCount(R__s, R__c, HMdcClusSim::IsA());
   } else {
      R__c = R__b.WriteVersion(HMdcClusSim::IsA(), kTRUE);
      HMdcClus::Streamer(R__b);
      R__b << nTracks;
      R__b.WriteArray(listTr, 5);
      R__b.WriteArray(nTimes, 5);
      R__b.WriteArray((unsigned char*)nLayers, 10);
      R__b.WriteArray((unsigned char*)nDigiTimes, 5);
      R__b.WriteArray((unsigned char*)trackStatus, 5);
      R__b.WriteArray((float*)xGeant, 5);
      R__b.WriteArray((float*)yGeant, 5);
      R__b << nNotFakeTracks;
      R__b << ioSMatchStatus;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

void HMdcClusSim::setDefForOldVer(Int_t ind) {
  for(Int_t i=ind;i<nTracks;i++) {
    xGeant[i] = -10000.;
    yGeant[i] = -10000.;
  }
  if(nTracks>=0) for(Int_t i=nTracks;i<5;i++) {
    xGeant[i] = 0.;
    yGeant[i] = 0.;
  }
  nNotFakeTracks = -1;
  ioSMatchStatus = -1;
  
}
