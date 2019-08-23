#ifndef  HPIDDILEPTONFILTER_H
#define  HPIDDILEPTONFILTER_H


#include "hfilter.h"
class HCategory;
class HLinearCategory;

class HPidDilepton;

class HIterator;
class HPidParticle;
class HPidTrackCand;
class HFilter;
class HParSet;



class HPidDiLeptonFilter:public HFilter {
 protected:
  
  HCategory *fPidDilepton;
  HIterator *fPidDileptonIter; 

  Bool_t  check_common_hits; 
  Bool_t  check_cpr_rej; 
  Bool_t  check_open_angle;
  Bool_t  skip_no_pairs;

  Option_t *singleSelectCond;// OR or AND possible  (OR is default)

  Int_t mode_data; // if mode_data=0 data are REAL if  mode_data=1 data are SIM (default REAL)

  
  Float_t open_angle_cut;
  Float_t prob_single_cut; //cut on probability to be single
  Char_t *fCpPidParName;
  TArrayI * double_ids; //array of Id of particles  to be removed
  
 public: 
  HPidDiLeptonFilter(void);
  ~HPidDiLeptonFilter(void);
 
  void setDefaults();
  void setOptions(Option_t *pParams);
  Bool_t check(TObject* pidDiLep);
  void setCpPidParamName(Char_t * cp_pid_par_name){fCpPidParName=cp_pid_par_name;}
  void setOpeningAngleCut(Float_t angle){open_angle_cut=angle;check_open_angle=kTRUE;} //angle in deg 
  void setProbSingleCut(Float_t prob){prob_single_cut=prob;} 
  void setProbSingleCondition(Option_t *cond="OR"){singleSelectCond=cond;}


  void setPidDileptonCatIter(HIterator *fDileptonIter) {fPidDileptonIter=fDileptonIter;}  

  void setRMArray(TArrayI * array){double_ids=array;}
  
  //  void fillRM_array();
  Bool_t checkConditions(HPidDilepton *pidDiLep);
  Bool_t checkCommonHits(HPidDilepton* pidDiLep);
  Bool_t isParticleIdSingle(HPidDilepton* pidDiLep,TArrayI *rm_id);
  Bool_t checkOpenAngle(HPidDilepton* pidDiLep);
  

  ClassDef(HPidDiLeptonFilter,0)
    };

#endif /* !HPIDDILEPTONFILTER */
