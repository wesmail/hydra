#ifndef HMETAMATCH_H
#define HMETAMATCH_H
#include "hreconstructor.h"
#include "TString.h"
#include "TObject.h"

#define RICH_TAB_SIZE 3

class HMetaMatch:public TObject {
private:
   Short_t trkCandInd;       // index of trkCand
   Short_t showerHitInd;     // index from META 
   Short_t tofHitInd;        // index from META 
   Short_t nCandForMeta;     // Number of META candidates,if<0 no META
   Short_t nextCandForMeta;  // Index of next MetaMatch object -1 no next object
   Short_t firstCandForMeta; // Index of first candidate
   Float_t qualityTOF;
   Float_t qualitySHOWER;

   Short_t splineInd;        // Index of Spline fitting
   Short_t rtInd;            // Index of rtmetatrack
   Short_t kickInd;          // Index of HKickTrackB in catKickTrackB
   Short_t kick123Ind;       // Index of HKickTrack123B in catKickTrack123B
   Char_t sector;            // sector number
   
   Short_t nRichId;
   Short_t nRichIPUId;
   Int_t aRichIndTable[RICH_TAB_SIZE];
   Int_t aRichIPUIndTable[RICH_TAB_SIZE];
   
   Char_t flag;              // user flag
public:
   HMetaMatch();
   ~HMetaMatch(){;}
   HMetaMatch(Short_t sec, Short_t tkInd, Short_t ind);
   HMetaMatch(HMetaMatch* fMetaMatch, Short_t ind);
   void setTrkCandInd(const Short_t trkind){trkCandInd=trkind;}
   void setMetaHitInd(const Short_t shower, Short_t tof){
     showerHitInd=shower; tofHitInd=tof;}
   void setShowerHitInd(const Short_t shower) {showerHitInd=shower;}
   void setTofHitInd(const Short_t tof) {tofHitInd=tof;}
   void setNCandForMeta(const Short_t nmeta){nCandForMeta=nmeta;}
   void setNextCandForMeta(const Short_t nextmeta){nextCandForMeta=nextmeta;}
   void setFirstCandForMeta(const Short_t firstmeta){firstCandForMeta=firstmeta;}
   void setQualityTOF(Float_t QUALITYTOF) {qualityTOF=QUALITYTOF;}
   void setQualitySHOWER(Float_t QUALITYSHOWER){qualitySHOWER=QUALITYSHOWER;}
   void setSplineInd(const Short_t splind){ splineInd=splind;}
   void setRtInd(const Short_t rtind){rtInd=rtind;}
   void setKickInd(const Short_t kicktr){kickInd=kicktr;}
   void setKick123Ind(const Short_t kicktr){kick123Ind=kicktr;}
   void setSector(const Char_t sec) {sector=sec;}
   void setNCandForRich(const Short_t nrich){nRichId=nrich;}
   void setNCandForIPU(const Short_t nrich){nRichIPUId=nrich;}
   void setARichInd(const Short_t id,const  Short_t richind) {
     if(id>=0&& id<RICH_TAB_SIZE) aRichIndTable[id] = richind;}
   void setARichIPUInd(const Short_t id,const  Short_t richind) {
     if(id>=0&& id<RICH_TAB_SIZE) aRichIPUIndTable[id] = richind;}

   Short_t getTrkCandInd(void) const {return trkCandInd;}
   Short_t getNCandForMeta(void) const {return nCandForMeta;}
   Short_t getFirstCandForMeta(void) const {return firstCandForMeta;}
   Short_t getNextCandForMeta(void) const {return nextCandForMeta;}
   Float_t getQualityTOF(void){return qualityTOF;}
   Float_t getQualitySHOWER(void) {return qualitySHOWER;}
   Short_t getSplineInd(void) const {return splineInd;}
   Short_t getRtInd(void) const {return rtInd;}
   Short_t getKickInd(void) const {return kickInd;}
   Short_t getKick123Ind(void) const {return kick123Ind;}
   Char_t getSector(void) const {return sector;}
   Bool_t isOverlap(void) const {
     return (showerHitInd>-1 && tofHitInd>-1) ? kTRUE:kFALSE; }
   Short_t getMetaHitInd(void) const {
     return (getSystem()==1) ? tofHitInd:showerHitInd;} //???
   Int_t getSystem(void) const {
     return (tofHitInd>-1) ? 1 : ((showerHitInd>-1) ? 0:-1); } //???
   Short_t getShowerHitInd(void) const {return showerHitInd;}
   Short_t getTofHitInd(void) const {return tofHitInd;}
   Short_t getNCandForRich(void) const {return nRichId;}
   Short_t getNCandForIPU(void) const {return nRichIPUId;}
   Short_t getARichInd(const Short_t id) const {
     return (id<RICH_TAB_SIZE) ? aRichIndTable[id]:-1;}
   Short_t getARichIPUInd(const Short_t id) const {
     return  (id<RICH_TAB_SIZE) ? aRichIPUIndTable[id]:-1;}
   
   ClassDef(HMetaMatch,1)
};
#endif


