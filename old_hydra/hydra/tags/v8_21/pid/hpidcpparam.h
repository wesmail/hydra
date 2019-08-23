#ifndef  HPIDCPPARAM_H
#define  HPIDCPPARAM_H

#include "TObject.h"
#include "TH1F.h"
#include "TString.h"

class HPidCPParam:public TObject  {
  
 protected:

  TH1F * hhcls; // 8 theta;3 phi;3 levels;mdc module 0-1;2 single-double;
  TH1F * hhnwires; // 8 theta;3 phi;3 levels;mdc module 0-1;2 single-double;
  TH1F * hhcharge; // 8 theta;3 phi;3 levels;2 single-double

  TH1F * hhcls_p; // 8 theta;3 phi;3 levels;mdc module 0-1;2 single-double;
  TH1F * hhnwires_p; // 8 theta;3 phi;3 levels;mdc module 0-1;2 single-double;
  TH1F * hhcharge_p; // 8 theta;3 phi;3 levels;2 single-double
  TString name_of_parfile;
  Bool_t setHist;   
 public:
  HPidCPParam(const Char_t *name_of_paramfile="cp_histos_test.root");
  ~HPidCPParam();
  // Bool_t readHistos(TH1F* cls,TH1F* nwires,TH1F* charge,TH1F* cls_p ,TH1F* nwires_p,TH1F* charge_p);
  Float_t getProbSingle(Int_t thbin,Int_t phibin,Int_t level_bin0,Int_t level_bin1,Int_t cls0,Int_t cls1,Int_t nwires0,Int_t nwires1, Int_t charge);
  Float_t getProbSingle(Float_t theta,Float_t phi,Int_t level,Int_t level1,Int_t cls0,Int_t cls1,Int_t nwires0,Int_t nwires1, Int_t charge);
  
  Float_t getProbSingle(Int_t thbin,Int_t phibin,Int_t level_bin0,Int_t level_bin1,Int_t cls0,Int_t cls1,Int_t nwires0,Int_t nwires1);
  Float_t getProbSingle(Float_t theta,Float_t phi,Int_t level,Int_t level1,Int_t cls0,Int_t cls1,Int_t nwires0,Int_t nwires1);

  Float_t getProbSingle(Int_t thbin,Int_t phibin,Int_t level_bin,Int_t cls,Int_t nwires,Int_t module);
  Float_t getProbSingle(Float_t theta,Float_t phi,Int_t level,Int_t cls,Int_t nwires,Int_t module);
  ClassDef(HPidCPParam,0) 
    };


#endif /* !HPIDCPPARAM_H */
