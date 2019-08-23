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
  
  virtual void print(void) const;
  
  ClassDef(HMdcTrkCand,1) // MDC track candidate
};

#endif  /* HMDCTRKCAND_H */
