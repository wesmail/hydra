#ifndef HMDCFUNC1
#define HMDCFUNC1

#include <TObject.h>
class HPidDilepton;
class HPidParticle;
class HPidParticleSim;
class HPidDilepton;
class HPidTrackCand;
class HMdcSeg;
class HMdcSegSim;
class HMdcHit;
class HMdcHitSim;
class HCategory;
class HMdcClusInf;
class HRichHitSim;
class HKickTrackSim;
class HGeantKine;
class TArrayI;

class HMdcFunc1:public TObject {
  
 private: 
  HCategory * fMdcSegCat;
  HCategory * fMdcHitCat;
  HCategory * fClusInf;
  HCategory * fGeantKineCat;
  HCategory * fGeantMdcCat;
  HCategory * fGeantRichMirrorCat;
  HCategory * fGeantTofCat;
  HCategory * fGeantShowCat;

 public: 
  HMdcFunc1(void){fMdcSegCat=NULL;fMdcHitCat=NULL;fClusInf=NULL;fGeantKineCat=NULL;fGeantMdcCat=NULL;fGeantRichMirrorCat=NULL;fGeantTofCat=NULL;fGeantShowCat=NULL;};
  ~HMdcFunc1(void){;}; 
  
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
  HCategory * getGeantRichMirrorCat(){return fGeantRichMirrorCat;}
  HCategory * getGeantTofCat(){return fGeantTofCat;}
  HCategory * getGeantShowCat(){return fGeantShowCat;} 
   
  void setMdcSegCat(HCategory * cat){fMdcSegCat=cat;} 
   void setMdcHitCat(HCategory * cat){fMdcHitCat=cat;} 
   void setClusInfCat(HCategory * cat){fClusInf=cat;}
   void setGeantKineCat(HCategory * cat){fGeantKineCat=cat;}
   void setGeantMdcCat(HCategory * cat){fGeantMdcCat=cat;}
   void setGeantRichMirrorCat(HCategory * cat){fGeantRichMirrorCat=cat;}
   void setGeantTofCat(HCategory * cat){fGeantTofCat=cat;}
   void setGeantShowCat(HCategory * cat){fGeantShowCat=cat;}

  HMdcClusInf* getClusInfObj(Int_t clsindex);
  Int_t calculateBin(Float_t mdc_angle,Int_t option);
  Int_t calculateLevelBin(Int_t level);
  Float_t getNormalMdcPhi(Int_t iSector, Float_t fPhiMdc);
  Float_t getNormalMdcTheta(Float_t fThetaMdc);
  Float_t getKickAngle(HPidParticle * part); 
  Int_t getGeantId(HRichHitSim * pRichHitSim,HKickTrackSim * pKickTrack);
  Int_t getGeantCommonTrack(HRichHitSim * pRichHitSim,HKickTrackSim * pKickTrack);
  Int_t getGeantCommonTrack(HPidParticleSim *part);

  Bool_t isCompt(HGeantKine *kine);
  Bool_t isConv(HGeantKine *kine);
  Bool_t isPi0Conv(HGeantKine *kine);
  Bool_t isPi0Dalitz(HGeantKine *kine);
  Bool_t isEtaDalitz(HGeantKine *kine);
  Bool_t isLepton(HGeantKine *kine);
  Bool_t isSingle(HMdcSegSim *seg);
  Bool_t isDouble(HMdcSegSim *seg,Int_t conv_mode=0);
  Int_t  isSingleDouble(HMdcSegSim *seg,Int_t track,Int_t conv_mode=0);
  Bool_t isSingle(HMdcHitSim *hit);
  Bool_t isDouble(HMdcHitSim *hit,Int_t conv_mode=0);



  Bool_t isMdcSegInGeant(HMdcSegSim *seg,Int_t track);
  Int_t  getPairTrack(Int_t track1); 
  HGeantKine*  getPairTrack(HGeantKine* kine1);
  Float_t getPairOpenAngle(HGeantKine* kine1,HGeantKine* kine2);
  Float_t getPairInvMass(HGeantKine* kine1,HGeantKine* kine2);

  void resetArray(TArrayI * arr); 
  void putInArray(Int_t cls,TArrayI * arr,Int_t index); 
  Bool_t isInArray(Int_t cls,TArrayI * arr); 

  HGeantKine* getKineObj(Int_t pTrack);
  Bool_t isInGeantMdc(Int_t tr);
  Bool_t isInGeantRichMirror(Int_t tr);
  Bool_t isInGeantTof(Int_t tr);
  Bool_t isInGeantShower(Int_t tr);
  Bool_t isHadesAccepted(Int_t tr);
  Bool_t couldBeAccepted(Int_t tr);
  Bool_t couldBeAccepted(HGeantKine *kine);


  Bool_t passCprPdfCuts(HPidDilepton* pDil,Float_t prob,Char_t * name_of_pdf_file,Int_t mode_rs=0,Option_t *rej_condition="OR");
  Bool_t passCprPdfCuts(HPidParticle* pPart,Float_t prob,Char_t * name_of_pdf_file,Int_t mode_rs=0,Option_t *rej_condition="OR");
  Int_t  corect_nwires(Int_t nwires,Int_t the_bin,Int_t phi,Int_t level);
  Int_t  corect_cls(Int_t cls,Int_t the_bin,Int_t phi,Int_t level);

  Int_t whatCommonHits(HPidDilepton* dil);

  ClassDef(HMdcFunc1,0)
    };
#endif /* !HMDCFUNC1 */
