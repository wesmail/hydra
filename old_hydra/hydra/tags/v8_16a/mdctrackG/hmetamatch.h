#ifndef HMETAMATCH_H
#define HMETAMATCH_H
#include "hreconstructor.h"
#include "TString.h"
#include "TObject.h"

#define RICH_TAB_SIZE 3

class HMetaMatch:public TObject {
private:
   Short_t trkCandInd;         // index of HTrkCand object
   Short_t showerHitInd;       // index of HShowerHitTof object
   Short_t tofHitInd;          // index of HTofHit or HTofCluster object
   Short_t nCandForMeta;       // number of MetaMatch obj. for the same HTrkCand
                               // >0 for first MetaMatch obj. in this chain
                               // <0 one of the next MetaMatch obj.
   Short_t nextCandForMeta;    // index of next MetaMatch obj. for the same HTrkCand
                               // = -1 no next object
   Short_t firstCandForMeta;   // index of the first MetaMatch obj. for the same HTrkCand in this chain
   Float_t qualityTOF;         // MDC TOF matching quality
   Float_t qualitySHOWER;      // MDC SHOWER matching quality
   Float_t dxMeta;             // Deviation in X-coordinate in META between hit and segment
   Float_t dyMeta;             // Deviation in X-coordinate in META between hit and segment

   Short_t splineInd;          // index of HSplineTrack object
   Short_t rtInd;              // index of HRtMetaTrack object
   Short_t kickInd;            // index of HKickTrackB in catKickTrackB
   Short_t kick123Ind;         // index of HKickTrack123B in catKickTrack123B
   Short_t rungeKuttaInd;      // index of HRKTrackB in catRKTrackB
   Char_t sector;              // sector number

   Short_t nRichId;            // number of matched rings in aRichIndTable[]
   Short_t nRichIPUId;         // number of matched rings in aRichIPUIndTable[]
   Int_t aRichIndTable[RICH_TAB_SIZE];     // arr.of indexes of HRichHit objects
   Int_t aRichIPUIndTable[RICH_TAB_SIZE];  // arr.of indexes of HRichHitIPU objects
                                           // arrais are sorted by matching quality

   Char_t flag;                // User flags bit array (8 flags, one bit / flag)
                               // First bit for spline track
                               // Second bit for Rt track
                               // Third bit for kick track
                               // Fourth bit for kick123 track
                               // Sixth bit reserved for rungeKutta track (but not yet implemented)
                               // If flag for corresponding track =1,
                               // this track is accepted

   Char_t tofClusterSize;      // tofClusterSize / tofHitInd
                               //   ==0          / HTofHit index in catTofHit
                               //   >=1          / HTofCluster index in catTofCluster
                               //   ==-1         / HTofHit index; hit belong TofCluster
                               //                /   with size >2
   Short_t indMMForTofClst;    // index of MetaMatch obj. with tofTofClusterSize>1
   Short_t nextMMForTofHit;    // index of MetaMatch obj. with next stripe(TofHit) from TofCluster

   Short_t numMMForSameMeta;   // number of MetaMatch obj. for the same meta hit
   Short_t nextMMForSameMeta;  // index of the next MetaMatch obj. for the same meta hit
   Short_t firstMMForSameMeta; // index of the first MetaMatch obj. for the same meta hit in this chain

   Float_t xSegCr;             // x,y (z=0) - cross point of MdcSeg with meta
   Float_t ySegCr;             // detector (tof or shower) in module coor.sys.
                               // of this meta hit

public:
   HMetaMatch();
   ~HMetaMatch(){;}
   HMetaMatch(Short_t sec, Short_t tkInd, Short_t ind);
   HMetaMatch(HMetaMatch* fMetaMatch, Short_t ind);
   HMetaMatch& operator=(HMetaMatch& met);
   void setTrkCandInd(Short_t trkind){trkCandInd=trkind;}
   void setMetaHitInd(Short_t shower, Short_t tof){
     showerHitInd=shower; tofHitInd=tof;}
   void setShowerHitInd(Short_t shower) {showerHitInd=shower;}
   void setTofHitInd(Short_t tof) {tofHitInd=tof;}
   void setTofClusterSize(Char_t clsz) {tofClusterSize=clsz;}
   void setNCandForMeta(Short_t nmeta){nCandForMeta=nmeta;}              // old func.name
   void setNextCandForMeta( Short_t nextmeta){nextCandForMeta=nextmeta;} // old func.name
   void setFirstCandForMeta(Short_t firstmeta){firstCandForMeta=firstmeta;} // old func.name
   void setNumMMForSameTrCnd(Short_t nmeta){nCandForMeta=nmeta;}            // new one
   void setNextMMForSameTrCnd( Short_t nextmeta){nextCandForMeta=nextmeta;} // new one
   void setFirstMMForSameTrCnd(Short_t firstmeta){firstCandForMeta=firstmeta;} // new one
   void setQualityTOF(Float_t QUALITYTOF) {qualityTOF=QUALITYTOF;}
   void setQualitySHOWER(Float_t QUALITYSHOWER){qualitySHOWER=QUALITYSHOWER;}
   void setdxMeta(Float_t DXMETA) {dxMeta=DXMETA;}
   void setdyMeta(Float_t DYMETA) {dyMeta=DYMETA;}
   void setMdcSegCross(Float_t x, Float_t y) {xSegCr=x; ySegCr=y;}

   void setSplineInd( Short_t splind);
   void setRtInd( Short_t rtind);
   void setKickInd( Short_t kicktr);
   void setKick123Ind( Short_t kicktr);
   void setRungeKuttaInd( Short_t rktr);

   void setSector( Char_t sec) {sector=sec;}
   void setNCandForRich( Short_t nrich){nRichId=nrich;}
   void setNCandForIPU( Short_t nrich){nRichIPUId=nrich;}
   void setARichInd( Short_t id,  Short_t richind) {
     if(id>=0&& id<RICH_TAB_SIZE) aRichIndTable[id] = richind;}
   void setARichIPUInd( Short_t id,  Short_t richind) {
     if(id>=0&& id<RICH_TAB_SIZE) aRichIPUIndTable[id] = richind;}
   void setIndMMForTofClst(Short_t ind) {indMMForTofClst=ind;}
   void setNextMMForTofHit(Short_t ind) {nextMMForTofHit=ind;}
   void setNumMMForSameMeta(Short_t n) {numMMForSameMeta=n;}
   void setNextMMForSameMeta(Short_t ind) {nextMMForSameMeta=ind;}
   void setFirstMMForSameMeta(Short_t ind) {firstMMForSameMeta=ind;}

   Short_t getTrkCandInd(void) const {return trkCandInd;}
   Short_t getNCandForMeta(void) const {return nCandForMeta;}         // old name
   Short_t getFirstCandForMeta(void) const {return firstCandForMeta;} // old name
   Short_t getNextCandForMeta(void) const {return nextCandForMeta;}   // old name
   Short_t getNumMMForSameTrCnd(void) const {return nCandForMeta;}       //new one
   Short_t getFirstMMForSameTrCnd(void) const {return firstCandForMeta;} //new one
   Short_t getNextMMForSameTrCnd(void) const {return nextCandForMeta;}   //new one

   Float_t getQualityTOF(void){return qualityTOF;}
   Float_t getQualitySHOWER(void) {return qualitySHOWER;}
   Float_t getdxMeta(void) {return dxMeta;}
   Float_t getdyMeta(void) {return dyMeta;}

   Short_t getSplineInd(void) const {return splineInd;}
   Short_t getRtInd(void) const {return rtInd;}
   Short_t getKickInd(void) const {return kickInd;}
   Short_t getKick123Ind(void) const {return kick123Ind;}
   Short_t getRungeKuttaInd(void) const {return rungeKuttaInd;}
   Char_t getSector(void) const {return sector;}
   Bool_t isOverlap(void) const {
     return (showerHitInd>-1 && tofHitInd>-1) ? kTRUE:kFALSE; }
   Short_t getMetaHitInd(void) const {
     return (getSystem()==1) ? tofHitInd:showerHitInd;} //???
   Int_t getSystem(void) const {
     return (tofHitInd>-1) ? 1 : ((showerHitInd>-1) ? 0:-1); } //???
   Short_t getShowerHitInd(void) const {return showerHitInd;}
   Short_t getTofHitInd(void) const {return tofHitInd;}
   Char_t getTofClusterSize(void) const {return tofClusterSize;}
   void getMdcSegCross(Float_t& x, Float_t& y) {x=xSegCr; y=ySegCr;}
   Float_t getXSegCross(void) const {return xSegCr;}
   Float_t getYSegCross(void) const {return ySegCr;}

   Short_t getNCandForRich(void) const {return nRichId;}
   Short_t getNCandForIPU(void) const {return nRichIPUId;}
   Short_t getARichInd(const Short_t id) const {
     return (id<RICH_TAB_SIZE) ? aRichIndTable[id]:-1;}
   Short_t getARichIPUInd(const Short_t id) const {
     return  (id<RICH_TAB_SIZE) ? aRichIPUIndTable[id]:-1;}

   Short_t getIndMMForTofClst(void) const {return indMMForTofClst;}
   Short_t getNextMMForTofHit(void) const {return nextMMForTofHit;}
   Short_t getNumMMForSameMeta(void) const {return numMMForSameMeta;}
   Short_t getNextMMForSameMeta(void) const {return nextMMForSameMeta;}
   Short_t getFirstMMForSameMeta(void) const {return firstMMForSameMeta;}

   inline void incrNumMMForSameTrkCnd(void);
   void setSplineAccept(void)    {flag |=  1;}  // First bit for spline track
   void setRtAccept(void)        {flag |=  2;}  // Second bit for Rt track
   void setKickAccept(void)      {flag |=  4;}  // Third bit for kick track
   void setKick123Accept(void)   {flag |=  8;}  // Fourth bit for kick123 track
   void setRungeKuttaAccept(void){flag |= 16;}  // Fifth bit for kick123 track

   void unsetSplineAccept(void)    {flag &=  ~1;}
   void unsetRtAccept(void)        {flag &=  ~2;}
   void unsetKickAccept(void)      {flag &=  ~4;}
   void unsetKick123Accept(void)   {flag &=  ~8;}
   void unsetRungeKuttaAccept(void){flag &= ~16;}

   Bool_t isSplineAccepted(void)   const  {return (flag&1)  ? kTRUE:kFALSE;}
   Bool_t isRtAccepted(void)       const  {return (flag&2)  ? kTRUE:kFALSE;}
   Bool_t isKickAccepted(void)     const  {return (flag&4)  ? kTRUE:kFALSE;}
   Bool_t isKick123Accepted(void)  const  {return (flag&8)  ? kTRUE:kFALSE;}
   Bool_t isRungeKuttaAccepted(void) const{return (flag&16) ? kTRUE:kFALSE;}

   void print(void);

   ClassDef(HMetaMatch,5)
};

void HMetaMatch::incrNumMMForSameTrkCnd(void) {
  if(nCandForMeta<=0) nCandForMeta=1;
  else nCandForMeta++;
}

#endif

