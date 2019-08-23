#ifndef  HMDCCPCUTSFINDER_H
#define  HMDCCPCUTSFINDER_H
#include "TNtuple.h"
#include "TObject.h"
#include "TH1F.h"

class HMdcCPCutsFinder:public TObject  {
 protected:
  Float_t container[3][8][3][4][5];   
  Int_t h;
  TNtuple *sing;
  TNtuple *doub;

  TNtuple *mdc_sing;
  TNtuple *mdc_doub;
  
  Int_t mdc_cluster_max,mdc_wires_max,rich_charge_max; 
  Int_t mdc_cluster_step,mdc_wires_step,rich_charge_step;
  TH1F * hcls[8][3][3][2][2]; // 8 theta;3 phi;3 levels;mdc module 0-1;2 single-double;
  TH1F * hnwires[8][3][3][2][2]; // 8 theta;3 phi;3 levels;mdc module 0-1;2 single-double;
  TH1F * hcharge[8][3][3][2]; // 8 theta;3 phi;3 levels;2 single-double

  TH1F * hcls_p[8][3][3][2][2]; // 8 theta;3 phi;3 levels;mdc module 0-1;2 single-double;
  TH1F * hnwires_p[8][3][3][2][2]; // 8 theta;3 phi;3 levels;mdc module 0-1;2 single-double;
  TH1F * hcharge_p[8][3][3][2]; // 8 theta;3 phi;3 levels;2 single-double




 public:
  Int_t lev_nbin,theta_bin_size,phi_bin_size,theta_nbin,phi_nbin;
  Float_t theta_min,theta_max,phi_min,phi_max;
  HMdcCPCutsFinder();
  HMdcCPCutsFinder(Int_t,Int_t,Int_t);
  ~HMdcCPCutsFinder();
  Bool_t readSingles(TNtuple *);
  Bool_t readDoubles(TNtuple *);
  
  Bool_t readMdcSingles(TNtuple *);
  Bool_t readMdcDoubles(TNtuple *);
  

  Bool_t findCuts();
  void findPidCuts(Char_t* outputname="cp_histos.root");
  
  void findPidCutsFromMdc(Char_t* outputname="cp_histos.root");
  
  void printContainer();
  void calculateOverallRejection();
  void printContainer2File(const Char_t* );
  Int_t calculateBin(Int_t,Int_t,Int_t);
  Int_t calculateMdcThetaBin(Float_t);
  Int_t calculateMdcPhiBin(Float_t);
  Int_t calculateMdcLevBin(Float_t l){return (Int_t) l-4;};
  Bool_t inBin(Float_t,Float_t,Float_t,Int_t,Int_t,Int_t);
ClassDef(HMdcCPCutsFinder,0) // Select simulated TOF hits
};

#endif /* !HMDCCPCUTSFINDER_H */
