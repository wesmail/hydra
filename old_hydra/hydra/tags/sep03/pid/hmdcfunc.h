#ifndef HMDCFUNC
#define HMDCFUNC

#include <TObject.h>
class HPidParticle;
class HPidTrackCand;
class HMdcSeg;
class HMdcHit;
class HCategory;
class HMdcClusInf;
class HRichHitSim;
class HKickTrackSim;

class HMdcFunc//:public TObject
{


 private:
  HCategory * fMdcHitCat;
  HCategory * fClusInf;


 public:
  HMdcFunc(void){fMdcHitCat=NULL;fClusInf=NULL;};
  virtual ~HMdcFunc(void){;};

  Int_t  getMdcClsSize(HPidParticle *part,Int_t module);
  Int_t  getMdcNWires(HPidParticle *part,Int_t module);
  Int_t  getMdcLevelCls(HPidParticle *part,Int_t module);

  Int_t  getIntCharge(HPidParticle *part);

  Int_t  getMdcClsSize(HMdcSeg *seg,Int_t module);
  Int_t  getMdcNWires(HMdcSeg *seg,Int_t module);
  Int_t  getMdcLevelCls(HMdcSeg *seg,Int_t module);

  Int_t  getMdcClsSize(HMdcHit *hit);
  Int_t  getMdcNWires(HMdcHit *hit);
  Int_t  getMdcLevelCls(HMdcHit *hit);

  HPidTrackCand* getPidTrackCand(HPidParticle *part);
  HMdcHit* getMdcHit(HMdcSeg *seg,Int_t module);

  HCategory * getMdcHitCat(){return  fMdcHitCat;}
  HCategory * getClusInfCat(){return  fClusInf;}
  void setMdcHitCat(HCategory * cat){fMdcHitCat=cat;}
  void setClusInfCat(HCategory * cat){fClusInf=cat;}
  HMdcClusInf* getClusInfObj(Int_t clsindex);
  Int_t calculateBin(Float_t mdc_angle,Int_t option);
  Int_t calculateLevelBin(Int_t level);
  Float_t getNormalMdcPhi(Int_t iSector, Float_t fPhiMdc);
  Float_t getNormalMdcTheta(Float_t fThetaMdc);
  Int_t getGeantId(HRichHitSim * pRichHitSim,HKickTrackSim * pKickTrack);
  ClassDef(HMdcFunc,0)
    };
#endif /* !HMDCFUNC */
