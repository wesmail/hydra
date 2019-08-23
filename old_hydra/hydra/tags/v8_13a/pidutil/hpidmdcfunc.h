#ifndef HPidMdcFunc_h
#define HPidMdcFunc_h

// -----------------------------------------------------------------------------

#include <TObject.h>

// -----------------------------------------------------------------------------

//class HPidTrackCand;
class HMdcSeg;
class HMdcHit;
class HCategory;
class HMdcClusInf;
class HRichHitSim;
class HKickTrackSim;

// -----------------------------------------------------------------------------

class HPidMdcFunc : public TObject
{
public:
  HPidMdcFunc();
  ~HPidMdcFunc();

  static Float_t    getMdcChiSquare(HMdcSeg *pMdcSeg);
  //  static Float_t    getMdcChiSquare(const HPidTrackCand *pTrack);
  /*  static Int_t    getMdcClsSize(const HPidTrackCand *pTrack,Int_t module);
      static Int_t    getMdcNWires(const HPidTrackCand *pTrack,Int_t module);
      static Int_t    getMdcLevelCls(const HPidTrackCand *pTrack,Int_t module);
      static Int_t    getIntCharge(const HPidTrackCand *pTrack);
  */
    static HCategory* getMdcHitCat(void);
    static HCategory* getClusInfCat(void);
  
  static Int_t    getMdcClsSize(HMdcSeg *seg,Int_t module);
  static Int_t    getMdcNWires(HMdcSeg *seg,Int_t module);
  static Int_t    getMdcLevelCls(HMdcSeg *seg,Int_t module);

  static Int_t    getMdcClsSize(HMdcHit *hit);
  static Int_t    getMdcNWires(HMdcHit *hit);
  static Int_t    getMdcLevelCls(HMdcHit *hit);
  
  static HMdcHit*     getMdcHit(HMdcSeg *seg,Int_t module);
  static HMdcClusInf* getClusInfObj(Int_t clsindex);
  static HMdcClusInf* getMdcClusInfoBySegment(HMdcSeg* pSeg,Int_t module);
  static Int_t getMdcClusInfoIndexBySegment(HMdcSeg* pSeg,Int_t module);
  
  static Int_t    calculateBin(Float_t mdc_angle,Int_t option);
  static Int_t    calculateLevelBin(Int_t level);
  /*    static Int_t    getGeantId(HRichHitSim* pRichHitSim,
   HKickTrackSim *pKickTrack);*/

  ClassDef(HPidMdcFunc,0)
};

// -----------------------------------------------------------------------------

#endif /* !HPidMdcFunc_h */
