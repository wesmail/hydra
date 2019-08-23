#ifndef  HLEPTONFILTER_H
#define  HLEPTONFILTER_H


#include "hfilter.h"
class HCategory;
class HLinearCategory;

class HIterator;
class HPidParticle;
class HPidTrackCand;
class HFilter;
class HParSet;
#include <TH1F.h>

class HLeptonFilter:public HFilter {
 protected: 
  

 
  
  Float_t tof_tof_mn,tof_tof_mx,tofino_tof_mn,tofino_tof_mx;
  Float_t tof_beta_mn,tof_beta_mx,tofino_beta_mn,tofino_beta_mx;
  Int_t npads,pattenqual;  
  Float_t avcharge,centroid,cut_on_pull;
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
  


  Float_t thetadiff,phidiff;
  Bool_t  mult_tofino_cut;
  Bool_t  filter_only_singles; 
  Bool_t  check_showlep_cond;
  HParSet *pShowPar;
  Bool_t  check_beta;
  Bool_t  check_mom_beta;  
  Bool_t  check_rings;
 
  Bool_t  check_pull; 
  Bool_t  check_mom_pull;
  Bool_t  check_mom;
  
 public: 
  HLeptonFilter(void);
  ~HLeptonFilter(void);

  Bool_t check(TObject* pidPart);
  
  void setDefaults(void);
  Bool_t checkConditions(HPidParticle *part);
  HPidTrackCand* getPidTrackCand(HPidParticle *part);
  Float_t getTof(HPidParticle *part);
  Bool_t isLeptonInShower(HPidParticle *part);
  Bool_t isInShowerTofPID(Int_t ad_SHT);
  Bool_t fullfillMatching(HPidParticle *part);
  Bool_t fullfillMultTofino(HPidParticle *part);
  Bool_t checkRingQuality(HPidParticle *part);
  Bool_t isInBetaWindow(HPidParticle *part);
  Bool_t isInTofWindow(HPidParticle *part);
  Bool_t isLepInSys0(HPidParticle *part);
  Bool_t checkPull(HPidParticle *part);
  Bool_t checkMomPull(HPidParticle *part);
  Bool_t isInMomBetaWindow(HPidParticle *part);

  Bool_t isInMomWindow(HPidParticle *part);
 
  void setBetaCutsMomDep(Float_t * array_mean_s0,Float_t * array_sigma_s0,Float_t * array_mean_s1,Float_t * array_sigma_s1,Float_t how_many_sigma=3.0);
 

  void setPullCutsMomDep(Float_t * array_mean_s0,Float_t * array_sigma_s0,Float_t * array_mean_s1,Float_t * array_sigma_s1,Float_t how_many_sigma=3.0);




  Float_t getShowThresh(Float_t *pPar, Float_t fMom, Float_t fScale);
  Float_t calcFromFit(Float_t *pPar, Float_t fMom);
  void setBetaCuts(Float_t tof_mn=0.0,Float_t  tof_mx=5.0,Float_t  tofino_mn=0.0,Float_t tofino_mx=5.0 ){tof_beta_mn=tof_mn;tof_beta_mx=tof_mx;tofino_beta_mn=tofino_mn;tofino_beta_mx=tofino_mx;check_beta=kTRUE;}
  void setTofCuts(Float_t tof_mn=0.0,Float_t  tof_mx=100.0,Float_t  tofino_mn=0.0,Float_t tofino_mx=100.0 ){tof_tof_mn=tof_mn;tof_tof_mx=tof_mx;tofino_tof_mn=tofino_mn;tofino_tof_mx=tofino_mx;}
  void setRichQRingCuts(Float_t avrCharge=0.0,Int_t  nPads=0,Int_t  patternQual=0,Float_t ring_Centroid=100){avcharge=avrCharge;npads=nPads;pattenqual=patternQual;centroid=ring_Centroid;check_rings=kTRUE;}

  void setMatchingCuts(Float_t theta_diff=100.0,Float_t  phi_diff=100.0){thetadiff=theta_diff;phidiff=phi_diff;}
  void setMultTofinoCut(Bool_t OnOff=kFALSE){mult_tofino_cut=OnOff;}
  void setShowerLepCond(Bool_t OnOff=kFALSE){check_showlep_cond=OnOff;}
  void setShowParamCont(HParSet *pHitFPar){pShowPar=pHitFPar;check_showlep_cond=kTRUE;}
  void setCutOnPull(Float_t pull){cut_on_pull=pull;check_pull=kTRUE;}

  void setMinMomCut(Float_t mom){mom_cut=mom;check_mom=kTRUE;}

  void filterOnlySingles (Bool_t fil_singles=kTRUE){filter_only_singles=fil_singles;} //default is not to filter singles (it means keep sing; kick out doubles)
  // -----------------------------------------------------------------------------
  inline Float_t HLeptonFilter::getMdcPhi(Int_t iSector, Float_t fPhiMdc)
    {
      // Convert MDC's phi angle to the coordinate system used in other detectors
      
      Float_t fPhi = TMath::RadToDeg() * fPhiMdc;
      
      switch(iSector)
	{
        case 0:
            break;
	    
        case 1:
        case 2:
        case 3:
        case 4:
	  fPhi += 60.0f * iSector;
	  break;
	  
        default:
	  fPhi -= 60.0f;
	  break;
	}
      
      return fPhi;
    }
  
  // -----------------------------------------------------------------------------
  
  inline Float_t HLeptonFilter::getMdcTheta(Float_t fThetaMdc)
    {
      // Convert MDC's theta angle to the coordinate system used in other detectors
      
      return  TMath::RadToDeg() * fThetaMdc;
    }
  
  // ----------------------------------------------------------
  
  ClassDef(HLeptonFilter,0)
    };

#endif /* !HLEPTONFILTER_H */
