#ifndef HMDCTRKCAND_H
#define HMDCTRKCAND_H

#include <TObject.h>

class HMdcTrkCand : public TObject {
protected:
  Char_t sector;             // sector number
  Short_t seg1Ind;           // index of HMdcSeg container (inner segment)
  Short_t seg2Ind;           // index of HMdcSeg container (outer segment)
  Short_t nCandForSeg1;      // number of HMdcTrkCand objects 
                             // for this inner segmen
                             // =0 - no outer segment
                             // >0 this container is first
                             // =-1 one of the next conteiners
  Short_t nextCandInd;       // index of next HMdcTrkCand object
                             // =-1 - this object is latest
  Short_t firstCandInd;      // index of the first HMdcTrkCand object with
                             // the same inner segment
  Float_t dedxInner;         // mean value of t2-t1 for inner segment
  Float_t dedxSigmaInner;    // sigma of t2-t1 distribution in inner segment
  UChar_t dedxNWireInner;    // number of wires in inner segment before truncated mean procedure
  UChar_t dedxNWireCutInner; // number of wires in inner segment cutted by truncated mean procedure
  Float_t dedxOuter;         // mean value of t2-t1 for outer segment
  Float_t dedxSigmaOuter;    // sigma of t2-t1 distribution in outer segment
  UChar_t dedxNWireOuter;    // number of wires in outer segment before truncated mean procedure
  UChar_t dedxNWireCutOuter; // number of wires in outer segment cutted by truncated mean procedure

  Short_t splineTrkInd;      // index of object filled by Anar's method
  Short_t rtTrkInd;          // index of object filled by Manuel's method
 
private: 
  HMdcTrkCand* pFirstCand;   //! first HMdcTrkCand object with
                             //! the same inner segment

public:
  HMdcTrkCand(void) {clear();}
  HMdcTrkCand(Char_t sec, Short_t s1ind, Int_t ind);
  HMdcTrkCand(HMdcTrkCand* fTrkCand, Short_t s2ind, Short_t ind);
  ~HMdcTrkCand(void) {}
  void clear(void);
  
  void setSec(Char_t s) {sector=s;}
  void setSeg1Ind(Short_t ind) {seg1Ind=ind;}
  void setSeg2Ind(Short_t ind) {seg2Ind=ind;}
  void setNCandForSeg1(Short_t n) {nCandForSeg1=n;}
  void setNextCandInd(Short_t n) {nextCandInd=n;}
  void setSplineTrkInd(Short_t ind) {splineTrkInd=ind;}
  void setRtTrkInd(Short_t ind) {rtTrkInd=ind;}
  void addSeg2Ind(Short_t ind) {seg2Ind=ind; nCandForSeg1=1;}

  void setdedxInnerSeg(Float_t dedx){dedxInner=dedx;}
  void setdedxOuterSeg(Float_t dedx){dedxOuter=dedx;}
  void getdedxSeg(Int_t seg,Float_t dedx)
  {
      if(seg==0)     {dedxInner=dedx;}
      else if(seg==1){dedxOuter=dedx;}
  }
  void setSigmadedxInnerSeg(Float_t sig){dedxSigmaInner=sig;}
  void setSigmadedxOuterSeg(Float_t sig){dedxSigmaOuter=sig;}
  void getSigmadedxSeg(Int_t seg,Float_t sig)
  {
      if(seg==0)     {dedxSigmaInner=sig;}
      else if(seg==1){dedxSigmaOuter=sig;}
  }
  void setNWirededxInnerSeg(UChar_t nw){dedxNWireInner=nw;}
  void setNWirededxOuterSeg(UChar_t nw){dedxNWireOuter=nw;}
  void getNWirededxSeg(Int_t seg,UChar_t nw)
  {
      if(seg==0)     {dedxNWireInner=nw;}
      else if(seg==1){dedxNWireOuter=nw;}
  }
  void setNWireCutdedxInnerSeg(UChar_t nw){dedxNWireCutInner=nw;}
  void setNWireCutdedxOuterSeg(UChar_t nw){dedxNWireCutOuter=nw;}
  void getNWireCutdedxSeg(Int_t seg,UChar_t nw)
  {
      if(seg==0)     {dedxNWireCutInner=nw;}
      else if(seg==1){dedxNWireCutOuter=nw;}
  }


  Char_t getSec(void) const {return sector;}
  Short_t getSeg1Ind(void) const {return seg1Ind;}
  Short_t getSeg2Ind(void) const {return seg2Ind;}
  Short_t getSegInd(Int_t seg) const {
    return (seg==0) ? seg1Ind : ((seg==1) ? seg2Ind:-1); }
  Short_t getNCandForSeg1(void) const {return nCandForSeg1;}
  Short_t getNextCandInd(void) const {return nextCandInd;}
  Short_t getFirstCandInd(void) const {return firstCandInd;}
  Short_t getSplineTrkInd(void) const {return splineTrkInd;}
  Short_t getRtTrkInd(void) const {return rtTrkInd;}
  Float_t getdedxInnerSeg(){return dedxInner;}
  Float_t getdedxOuterSeg(){return dedxOuter;}
  Float_t getdedxSeg(Int_t seg)
  {
      if(seg==0)     {return dedxInner;}
      else if(seg==1){return dedxOuter;}
      else {return -1;}
  }
  Float_t getSigmadedxInnerSeg(){return dedxSigmaInner;}
  Float_t getSigmadedxOuterSeg(){return dedxSigmaOuter;}
  Float_t getSigmadedxSeg(Int_t seg)
  {
      if(seg==0)     {return dedxSigmaInner;}
      else if(seg==1){return dedxSigmaOuter;}
      else {return -1;}
  }
  UChar_t getNWirededxInnerSeg(){return dedxNWireInner;}
  UChar_t getNWirededxOuterSeg(){return dedxNWireOuter;}
  UChar_t getNWirededxSeg(Int_t seg)
  {
      if(seg==0)     {return dedxNWireInner;}
      else if(seg==1){return dedxNWireOuter;}
      else {return 0;}
  }
  UChar_t getNWireCutdedxInnerSeg(){return dedxNWireCutInner;}
  UChar_t getNWireCutdedxOuterSeg(){return dedxNWireCutOuter;}
  UChar_t getNWireCutdedxSeg(Int_t seg)
  {
      if(seg==0)     {return dedxNWireCutInner;}
      else if(seg==1){return dedxNWireCutOuter;}
      else {return 0;}
  }
  virtual void print(void) const;
  
  ClassDef(HMdcTrkCand,2) // MDC track candidate
};

#endif  /* HMDCTRKCAND_H */
