//*-- Author : Anar Rustamov
//*-- Modified : 24/11/2004 by V. Pechenov
//*-- Modified : 02/12/2004 by V. Pechenov

///////////////////////////////////////////////////////////////////////////
//
//  HMetaMatch
//
//  Keep indexes of hits for one track, matching qualities, and
//  indexes of track objects (HSplineTrack, HKickTrackB,...).
//
//  Indexis of hits:
//    trkCandInd                      -  index of HTrkCand object
//    showerHitInd                    -  index of HShowerHitTof object
//    tofHitInd                       -  index of HTofHit or HTofCluster object
//    aRichIndTable[RICH_TAB_SIZE]    -  arr.of indexes of HRichHit objects
//    aRichIPUIndTable[RICH_TAB_SIZE] -  arr.of indexes of HRichHitIPU objects
//
//    Arrais aRichIndTable and aRichIPUIndTable are sorted by matching quality 
//    and can keep up to 3 ring indexes.
//
// Quality MDC SHOWER matching:
//   Xs,Ys - shower hit position in coordinate system of shower module
//   dXs,dYs - hit position errors (== HShowerHit::getSigmaX(), getSigmaY())
//   Xm,Ym - mdc segment cross point with shower module in coordinate system 
//           of corresponding shower module 
//   showerSigmaXOffset & showerSigmaYOffset - from HMetaMatchPar cont.
//   showerSigmaXMdc & showerSigmaYMdc - from HMetaMatchPar cont.
//   qualityShower = 
//     sqrt[((Xs - Xm - showerSigmaXOffset)/sqrt(dXs^2 + showerSigmaXMdc^2))^2 +
//          ((Ys - Ym - showerSigmaYOffset)/sqrt(dYs^2 + showerSigmaYMdc^2))^2]
//
// Tof:
//   Xt,Yt - toh hit (or tof cluster position in coordinate system of tof module
//   Xm,Ym - mdc segment cross point with tof module in coordinate system 
//           of corresponding tof module
//   tofSigmaXOffset, tofSigmaYOffset - from HMetaMatchPar cont.
//   tofSigmaX, tofSigmaY - from HMetaMatchPar cont.
//   qualityTof = sqrt[ ((Xt - Xm - tofSigmaXOffset)/tofSigmaX)^2 +
//                      ((Yt - Ym - tofSigmaYOffset)/tofSigmaY)^2 ]
//
///////////////////////////////////////////////////////////////////////////

#include "hmetamatch.h"
using namespace std;

HMetaMatch::HMetaMatch() {
  trkCandInd=-1;
  showerHitInd=tofHitInd=-1; 
  nCandForMeta=-1;
  nextCandForMeta=-1;
  firstCandForMeta=-1;
  splineInd=-1;
  
  qualityTOF=-1.;
  qualitySHOWER=-1.;
  rtInd=-1;
  kickInd=kick123Ind=-1;
  rungeKuttaInd=-1;
  sector=-1;
  for(Int_t i = 0; i < RICH_TAB_SIZE; i++) aRichIndTable[i] = -1;
  for(Int_t i = 0; i < RICH_TAB_SIZE; i++) aRichIPUIndTable[i] = -1;
  nRichId=nRichIPUId=0;
  flag=0;
  tofClusterSize=0;
  indMMForTofClst=-1;
  nextMMForTofHit=-1;
  numMMForSameMeta=0;
  nextMMForSameMeta=-1;
  firstMMForSameMeta=-1;
  xSegCr=0.;
  ySegCr=0.;
}

HMetaMatch::HMetaMatch(Short_t sec, Short_t tkInd, Short_t ind) {
  sector=sec;
  firstCandForMeta=ind;
  trkCandInd=tkInd;
  
  qualityTOF=-1.;
  qualitySHOWER=-1.;
  
  showerHitInd=tofHitInd=-1; 
  nCandForMeta=-1;
  nextCandForMeta=-1;
  splineInd=-1;
  rtInd=-1;
  kickInd=kick123Ind=-1;
  rungeKuttaInd=-1;
  for(Int_t i = 0; i < RICH_TAB_SIZE; i++) aRichIndTable[i] = -1;
  for(Int_t i = 0; i < RICH_TAB_SIZE; i++) aRichIPUIndTable[i] = -1;
  nRichId=nRichIPUId=0;
  flag=0;
  tofClusterSize=0;
  indMMForTofClst=-1;
  nextMMForTofHit=-1;
  numMMForSameMeta=0;
  nextMMForSameMeta=-1;
  firstMMForSameMeta=-1;
  xSegCr=0.;
  ySegCr=0.;
}
  
HMetaMatch::HMetaMatch(HMetaMatch* fMetaMatch, Short_t ind) {
  // ind - index of this object
  fMetaMatch->nextCandForMeta=ind;
  
  sector=fMetaMatch->sector;
  trkCandInd=fMetaMatch->trkCandInd;
  firstCandForMeta=fMetaMatch->firstCandForMeta;
  nCandForMeta=-1;
  
  qualityTOF=-1.;
  qualitySHOWER=-1.;
  
  showerHitInd=tofHitInd=-1;
  nextCandForMeta=-1;
  splineInd=-1;
  rtInd=-1;
  kickInd=kick123Ind=-1;
  rungeKuttaInd=-1;
  nRichId=fMetaMatch->nRichId;
  nRichIPUId=fMetaMatch->nRichIPUId;
  for(Int_t i = 0; i < RICH_TAB_SIZE; i++) {
    aRichIndTable[i] = fMetaMatch->aRichIndTable[i];
    aRichIPUIndTable[i] = fMetaMatch->aRichIPUIndTable[i];
  }
  flag=0;
  tofClusterSize=0;
  indMMForTofClst=-1;
  nextMMForTofHit=-1;
  numMMForSameMeta=0;
  nextMMForSameMeta=-1;
  firstMMForSameMeta=-1;
  xSegCr=0.;
  ySegCr=0.;
}

HMetaMatch& HMetaMatch::operator=(HMetaMatch& met) {
  trkCandInd=met.getTrkCandInd();
  showerHitInd=met.getShowerHitInd();
  nCandForMeta=met.getNCandForMeta();
  nextCandForMeta=met.getNextCandForMeta();
  firstCandForMeta=met.getFirstCandForMeta();
  qualityTOF=met.getQualityTOF();
  qualitySHOWER=met.getQualitySHOWER();
  splineInd=met.getSplineInd();
  rtInd=met.getRtInd();
  kickInd=met.getKickInd();
  kick123Ind=met.getKick123Ind();
  rungeKuttaInd=met.getRungeKuttaInd();
  sector=met.getSector();
  nRichId=met.getNCandForRich();
  nRichIPUId=met.getNCandForIPU();

  for(int i=0; i<RICH_TAB_SIZE; i++) {
    aRichIndTable[i]=met.getARichInd(i);
    aRichIPUIndTable[i]=met.getARichIPUInd(i);
  }
  indMMForTofClst=met.indMMForTofClst;
  nextMMForTofHit=met.nextMMForTofHit;
  numMMForSameMeta=met.numMMForSameMeta;
  nextMMForSameMeta=met.nextMMForSameMeta;
  firstMMForSameMeta=met.firstMMForSameMeta;
  xSegCr=met.xSegCr;
  ySegCr=met.ySegCr;
  return *this;
}

void HMetaMatch::setSplineInd(Short_t splind) {
  splineInd=splind;
  if(splineInd>=0) setSplineAccept();
  else unsetSplineAccept();
}

void HMetaMatch::setRtInd(Short_t rtind) {
  rtInd=rtind;
  if(rtInd>=0) setRtAccept();
  else unsetRtAccept();
}
void HMetaMatch::setKickInd(Short_t kicktr) {
  kickInd=kicktr;
  if(kickInd>=0) setKickAccept();
  else unsetKickAccept();
}

void HMetaMatch::setKick123Ind(Short_t kicktr){
  kick123Ind=kicktr;
  if(kick123Ind>=0) setKick123Accept();
  else unsetKick123Accept();
}

void HMetaMatch::setRungeKuttaInd(Short_t rktr){
  rungeKuttaInd=rktr;
  if(rungeKuttaInd>=0) setRungeKuttaAccept();
  else unsetRungeKuttaAccept();
}

void HMetaMatch::print(void) {
  printf("HMetaMatch:  %i sector, index of trkCand=%i\n",sector+1,trkCandInd);
  printf("  nCandForMeta=%i firstCandForMeta=%i nextCandForMeta=%i\n",
      nCandForMeta,firstCandForMeta,nextCandForMeta);
  if(showerHitInd>=0) printf("  showerHitInd=%i  qualitySHOWER=%g\n",
    showerHitInd,qualitySHOWER);
  if(tofHitInd>=0) printf("  tofHitInd=%i  qualityTOF=%g  tofClusterSize=%i\n", 
      tofHitInd,qualityTOF,tofClusterSize);
  if(indMMForTofClst>=0) printf(
    "  index of MM for tof cluster =%i, index of MM for next rot in tof cluster =%i\n",
    indMMForTofClst,nextMMForTofHit);
  printf("  The same meta hit: num.of MM obj. =%i, next MM =%i, first MM=%i\n",
      numMMForSameMeta,nextMMForSameMeta,firstMMForSameMeta);
  if(nRichId>0) {
    printf("  richHitInd =");
    for(Int_t n=0;n<nRichId;n++) printf(" %i",aRichIndTable[n]);
    printf("\n");
  }
  if(nRichIPUId>0) {
    printf("  richHitIPUInd =");
    for(Int_t n=0;n<nRichIPUId;n++) printf("  %i",aRichIPUIndTable[n]);
    printf("\n");
  }
  printf("  Accepted tracks:");
  if(isSplineAccepted()) printf(" spline");
  if(isRtAccepted()) printf(" rt");
  if(isKickAcceptde()) printf(" kick");
  if(isKick123Accepted()) printf(" kick123");
  printf("\n");
  printf("  Tracks:");
  if(splineInd>=0)    printf("  splineInd=%i",splineInd);
  if(rtInd>=0)        printf("  rtInd=%i",rtInd);
  if(kickInd>=0)      printf("  kickInd=%i",kickInd);
  if(kick123Ind>=0)   printf("  kick123Ind=%i",kick123Ind);
  if(rungeKuttaInd>=0)printf("  rungeKuttaInd=%i",rungeKuttaInd);
  printf("\n");
}
   
void HMetaMatch::Streamer(TBuffer &R__b) {
  // Stream an object of class HMetaMatch.
  UInt_t R__s, R__c;
  if (R__b.IsReading()) {
    Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
    TObject::Streamer(R__b);
    R__b >> trkCandInd;
    R__b >> showerHitInd;
    R__b >> tofHitInd;
    R__b >> nCandForMeta;
    R__b >> nextCandForMeta;
    R__b >> firstCandForMeta;
    R__b >> qualityTOF;
    R__b >> qualitySHOWER;
    R__b >> splineInd;
    R__b >> rtInd;
    R__b >> kickInd;
    R__b >> kick123Ind;
    R__b >> sector;
    R__b >> nRichId;
    R__b >> nRichIPUId;
    R__b.ReadStaticArray((int*)aRichIndTable);
    R__b.ReadStaticArray((int*)aRichIPUIndTable);
    R__b >> flag;

    if(R__v < 4) {
      if(splineInd>=0) setSplineAccept();
      if(rtInd>=0) setRtAccept() ;
      if(kickInd>=0) setKickAccept();
      if(kick123Ind>=0) setKick123Accept();
      qualityTOF=(qualityTOF<0.) ? -1. : sqrt(qualityTOF);
      qualitySHOWER=(qualitySHOWER<0.) ? -1. : sqrt(qualitySHOWER);
    }
    if (R__v > 1) {
      R__b >>tofClusterSize;
      if (R__v == 3) {
        Short_t isTofCluster;
        R__b >> isTofCluster;
        if(isTofCluster<0) unsetSplineAccept();
      }
    } else {
      tofClusterSize=0;
    }
    if (R__v > 3) {
      R__b >> indMMForTofClst;
      R__b >> nextMMForTofHit;
      R__b >> numMMForSameMeta;
      R__b >> nextMMForSameMeta;
      R__b >> firstMMForSameMeta;

      R__b >> rungeKuttaInd;
      R__b >> xSegCr;
      R__b >> ySegCr;
    } else {
      indMMForTofClst=-1;
      nextMMForTofHit=-1;
      numMMForSameMeta=1;
      if(showerHitInd<0 && tofHitInd<0) numMMForSameMeta=0;
      nextMMForSameMeta=-1;
      firstMMForSameMeta=-1;
      rungeKuttaInd=-1;
      xSegCr=0.;
      ySegCr=0.;
    }  
    R__b.CheckByteCount(R__s, R__c, HMetaMatch::IsA());
  } else {
    R__c = R__b.WriteVersion(HMetaMatch::IsA(), kTRUE);
    TObject::Streamer(R__b);
    R__b << trkCandInd;
    R__b << showerHitInd;
    R__b << tofHitInd;
    R__b << nCandForMeta;
    R__b << nextCandForMeta;
    R__b << firstCandForMeta;
    R__b << qualityTOF;
    R__b << qualitySHOWER;
    R__b << splineInd;
    R__b << rtInd;
    R__b << kickInd;
    R__b << kick123Ind;
    R__b << rungeKuttaInd;
    R__b << sector;
    R__b << nRichId;
    R__b << nRichIPUId;
    R__b.WriteArray(aRichIndTable, 3);
    R__b.WriteArray(aRichIPUIndTable, 3);
    R__b << flag;
    R__b << tofClusterSize;
    R__b << indMMForTofClst;
    R__b << nextMMForTofHit;
    R__b << numMMForSameMeta;
    R__b << nextMMForSameMeta;
    R__b << firstMMForSameMeta;
    R__b << xSegCr;
    R__b << ySegCr;
    R__b.SetByteCount(R__c, kTRUE);
  }
}


ClassImp(HMetaMatch)
