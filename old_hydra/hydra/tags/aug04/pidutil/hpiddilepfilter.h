#ifndef  HPIDDILEPFILTER_H
#define  HPIDDILEPFILTER_H

#include "hreconstructor.h"
#include "hpiddileptonfilter.h"
class HPidDilepton;
class HCategory;
class HLinearCategory;

class TArrayI;


class HIterator;
class HPidDiLeptonFilter; 

class HPidDiLepFilter: public HReconstructor {
 protected: 
  HCategory *fPidDilepton;
  HIterator *fPidDileptonIter;
  HCategory *fClusInfCat; 
  HCategory *fMdcHitCat;



  
  Bool_t  check_common_hits; 
  Bool_t  check_cpr_rej; 
  Bool_t  check_open_angle;
  Bool_t  skip_no_pairs;

  Option_t *singleSelectCond;// OR or AND possible  (OR is default)

  Int_t mode_data; // if mode_data=0 data are REAL if  mode_data=1 data are SIM (default REAL)

  Option_t *cutParams;
  Float_t open_angle_cut;
  Float_t prob_single_cut;
  Char_t *fCpPidParName;
  
  TArrayI * double_ids; //array of Id of particles  to be removed
  
  HPidDiLeptonFilter diLepFilter; 

 public: 
  HPidDiLepFilter(void);
  HPidDiLepFilter(Text_t *name,Text_t *title);
  ~HPidDiLepFilter(void);
 
  Bool_t init(void);
  Bool_t finalize(void);
  Int_t execute(void);
  
  void setOptions(Option_t *pParams="");
  void setCpPidParamName(Char_t * cp_pid_par_name){fCpPidParName=cp_pid_par_name;}
  void setOpeningAngleCut(Float_t angle){open_angle_cut=angle;} //angle in deg 
  void setProbSingleCut(Float_t prob){prob_single_cut=prob;}
  void setProbSingleCondition(Option_t *cond="OR"){singleSelectCond=cond;}
  void fillRM_array();
  
  
 

  ClassDef(HPidDiLepFilter,0)
    };

#endif /* !HPIDDILEPFILTER */
