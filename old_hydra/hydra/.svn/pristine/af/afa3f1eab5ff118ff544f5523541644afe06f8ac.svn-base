#ifndef  HPIDLEPFILTER_H
#define  HPIDLEPFILTER_H

#include "hreconstructor.h"
#include "hleptonfilter.h"
class HCategory;
class HLinearCategory;
class HParSet;
class HIterator;
class HPidParticle;
class HPidTrackCand;
class HMdcLeptonFilter;
#include <TH1F.h>

class HPidLepFilter: public HReconstructor {
 protected: 
  HCategory *fPidPart;
  HIterator *fPidPartIter;
  HCategory *fShowerTofPID;
  HIterator *fShowerTofPIDIter;
  HCategory *fRichHitCat;
  HCategory *fKickTrackCat;
  
  HParSet *pHitFPar; // pointer to the Shower analysis parameters

  TH1F *htof_sh;
  TH1F *htof_sh_lep;
  TH1F *htof_tof;
  
  Float_t tof_tof_mn,tof_tof_mx,tofino_tof_mn,tofino_tof_mx;
  Float_t tof_beta_mn,tof_beta_mx,tofino_beta_mn,tofino_beta_mx;
  Float_t thetadiff,phidiff;

  Float_t avcharge,centroid,cut_on_pull;
  Int_t npads,pattenqual;  

  Float_t beta_sig_cut,pull_sig_cut; //how many sigma one have to cut
  Float_t mom_cut; //cut on the lepton momentum 

  Float_t array_beta_mean_s0[10]; //array of means of for beta cuts is sys0 for 10 bins in mom 0-100 ... 
  Float_t array_beta_sigma_s0[10];//array of sigmas of for beta cuts is sys0 for 10 bins in mom 0-100 ... 
  Float_t array_beta_mean_s1[10];
  Float_t array_beta_sigma_s1[10];

  Float_t array_pull_mean_s0[10];
  Float_t array_pull_sigma_s0[10];
  Float_t array_pull_mean_s1[10];
  Float_t array_pull_sigma_s1[10];
  


  Bool_t  mult_tofino_cut; 
  Bool_t  select_best_match_part; 
  Bool_t  check_showlep_cond; 
  
  Bool_t  check_rings; 
  Bool_t  check_beta;
  Bool_t  check_mom_beta; 
  Bool_t  check_pull;
  Bool_t  check_mom_pull; 
  Bool_t  check_mom;

  HLeptonFilter lepFilter; 
  Int_t mode; //mode=0 default select rich-kicktrack events; mode=1 apply also tof cut ;mode=2 fill also tof histograms 
 public: 
  HPidLepFilter(void);
  HPidLepFilter(Text_t *name,Text_t *title,Int_t run_mode=1);
  ~HPidLepFilter(void);

  Bool_t init(void);
  Bool_t reinit(void);
  Bool_t finalize(void);
  Int_t execute(void);
  void fillHisto(void);
  void initHisto(void);
  void setDefaults(void);
 
  Bool_t checkConditions(HPidParticle *part);
  HPidTrackCand* getPidTrackCand(HPidParticle *part);
  Float_t getTof(HPidParticle *part);
  Bool_t isLeptonInShower(HPidParticle *part);
  Bool_t isInShowerTofPID(Int_t ad_SHT);
  void setBetaCuts(Float_t tof_mn=0.0,Float_t  tof_mx=5.0,Float_t  tofino_mn=0.0,Float_t tofino_mx=5.0 ){tof_beta_mn=tof_mn;tof_beta_mx=tof_mx;tofino_beta_mn=tofino_mn;tofino_beta_mx=tofino_mx;check_beta=kTRUE;}
  void setTofCuts(Float_t tof_mn=0.0,Float_t  tof_mx=100.0,Float_t  tofino_mn=0.0,Float_t tofino_mx=100.0 ){tof_tof_mn=tof_mn;tof_tof_mx=tof_mx;tofino_tof_mn=tofino_mn;tofino_tof_mx=tofino_mx;}

 void setBetaCutsMomDep(Float_t * array_mean_s0,Float_t * array_sigma_s0,Float_t * array_mean_s1,Float_t * array_sigma_s1,Float_t how_many_sigma=3.0);
 void setPullCutsMomDep(Float_t * array_mean_s0,Float_t * array_sigma_s0,Float_t * array_mean_s1,Float_t * array_sigma_s1,Float_t how_many_sigma=3.0);
 



  void setRichQRingCuts(Float_t avrCharge=0.0,Int_t  nPads=0,Int_t  patternQual=0,Float_t ring_Centroid=100.0){avcharge=avrCharge;npads=nPads;pattenqual=patternQual;centroid=ring_Centroid;check_rings=kTRUE;}

  void setMatchingCuts(Float_t theta_diff=100.0,Float_t  phi_diff=100.0){thetadiff=theta_diff;phidiff=phi_diff;}
  void setMultTofinoCut(Bool_t OnOff=kFALSE){mult_tofino_cut=OnOff;}
  void setShowerLepCond(Bool_t OnOff=kFALSE){check_showlep_cond=OnOff;}
  void setSelecOfBestMatchPart(Bool_t OnOff=kTRUE){printf("warning HPidFilter::setSelecOfBestMatchPart not yet  implemented \n");select_best_match_part=OnOff;}

  void setCutOnPull(Float_t pull){cut_on_pull=pull;check_pull=kTRUE;}

  void setMinMomCut(Float_t mom){mom_cut=mom;check_mom=kTRUE;}

  Int_t findIndexOfBestMatching();
  ClassDef(HPidLepFilter,0)
    };

#endif /* !HPIDLEPFILTER_H */
