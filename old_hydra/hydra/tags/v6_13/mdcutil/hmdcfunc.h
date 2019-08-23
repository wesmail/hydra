#ifndef HMDCFUNC
#define HMDCFUNC

#include <TObject.h>
class HPidParticle;
class HPidParticleSim;
class HPidTrackCand;
class HMdcSeg;
class HMdcSegSim;
class HMdcHit;
class HCategory;
class HMdcClusInf;
class HRichHitSim;
class HKickTrackSim;
class HGeantKine;

class HMdcFunc:public TObject {
  
 private: 
  HCategory * fMdcSegCat;
  HCategory * fMdcHitCat;
  HCategory * fClusInf;
  HCategory * fGeantKineCat;
  HCategory * fGeantMdcCat;
  
 public: 
  HMdcFunc(void){fMdcSegCat=NULL;fMdcHitCat=NULL;fClusInf=NULL;fGeantKineCat=NULL;};
  ~HMdcFunc(void){;}; 
  
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
  
  HCategory * getMdcSegCat(){return  fMdcSegCat;}  
  HCategory * getMdcHitCat(){return  fMdcHitCat;} 
  HCategory * getClusInfCat(){return  fClusInf;} 
  HCategory * getGeantKineCat(){return fGeantKineCat;}
  HCategory * getGeantMdcCat(){return fGeantMdcCat;}
  
   void setMdcSegCat(HCategory * cat){fMdcSegCat=cat;} 
   void setMdcHitCat(HCategory * cat){fMdcHitCat=cat;} 
   void setClusInfCat(HCategory * cat){fClusInf=cat;}
   void setGeantKineCat(HCategory * cat){fGeantKineCat=cat;}
   void setGeantMdcCat(HCategory * cat){fGeantMdcCat=cat;}

  HMdcClusInf* getClusInfObj(Int_t clsindex);
  Int_t calculateBin(Float_t mdc_angle,Int_t option);
  Int_t calculateLevelBin(Int_t level);
  Float_t getNormalMdcPhi(Int_t iSector, Float_t fPhiMdc);
  Float_t getNormalMdcTheta(Float_t fThetaMdc);
  Int_t getGeantId(HRichHitSim * pRichHitSim,HKickTrackSim * pKickTrack);
  Int_t getGeantCommonTrack(HRichHitSim * pRichHitSim,HKickTrackSim * pKickTrack);
  Int_t getGeantCommonTrack(HPidParticleSim *part);

  Bool_t isPi0Conv(HGeantKine *kine);
  Bool_t isPi0Dalitz(HGeantKine *kine);
  Bool_t isLepton(HGeantKine *kine);
  Bool_t isSingle(HMdcSegSim *seg);
  Bool_t isDouble(HMdcSegSim *seg,Int_t conv_mode=0);
  Int_t  isSingleDouble(HMdcSegSim *seg,Int_t track,Int_t conv_mode=0);
  Bool_t isMdcSegInGeant(HMdcSegSim *seg,Int_t track);
  Int_t  getPairTrack(Int_t track1); 

  HGeantKine* getKineObj(Int_t pTrack);
  ClassDef(HMdcFunc,0)
    };
#endif /* !HMDCFUNC */
