#ifndef  HMDCCPSELECTOR_H
#define  HMDCCPSELECTOR_H

#include "hreconstructor.h"

class HGeantKine;
class HCategory;
class HMdcSegSim;
class TArrayI;
class TNtuple;
class TH1F;

class HMdcCPSelector : public HReconstructor {
 protected: 
  Int_t theta_bin_size;
  Int_t phi_bin_size;
  
  TNtuple * mdc_singles;
  TNtuple * mdc_doubles;
  TH1F * hsystem;
 
  TArrayI* arr_doub_mdcseg;
  TArrayI* arr_sing_mdcseg;
  

  HCategory *fMdcSegCat;  //! Pointer to the category
  HCategory *fMdcHitCat;  //! Pointer to the category
  HCategory *fGeantCat;   //! Pointer to the category
  HCategory *fGeantMdcCat;   //! Pointer to the category

  HCategory *fPidPart; 
  HIterator *fPidPartIter;
  
  HCategory *fClusInf; //! Pointer to the category
  HIterator *iterator_clusinf;  //! Iterator on HMdcClusInf
  HIterator *iterator_kine; //! Iterator on HGeantKine
  HIterator *iterator_mdcseg; //! Iterator on HMdcSegSim
  HIterator *iterator_mdchit; //! Iterator on HMdcHitSim
  
  Int_t  nThreshold,index,dummy,dummy1;
  Int_t pTrack_parent,nEvCounter;
  Int_t pID;
  Int_t special_mode;//=0 it select singles and doubles normaly ;=1 for doubles it consider also such singles that has coresponding pair lepton passing possition  of Hit in Geant   
  
  
  char *nameHistoFile;
  
  Int_t mod_mdcseg;

 public: 
  HMdcCPSelector(void);
  HMdcCPSelector(Text_t *name,Text_t *title,char *histoname="histograms_convanal.root",Int_t mode=1);
  ~HMdcCPSelector(void);
  void resetArray(TArrayI *ar); 
  Bool_t checkArray(Int_t member,TArrayI *ar);
  
  Bool_t init(void);
  Bool_t finalize(void);
  Int_t execute(void);
  void findConvInMdcSeg();
  void findConvInMdcSeg(HMdcSegSim* mdcseg);
  void findConvPairsInPidParticle();
  void setHistoName(char* name){nameHistoFile=name;}
  void setSpecialMode(Int_t mode){special_mode=mode;}
  HCategory * getMdcHitCat(){return  fMdcHitCat;} 
  HCategory * getClusInfCat(){return  fClusInf;} 
  
  Bool_t fillHistoSingle(HMdcSegSim *mdchit); 
  Bool_t fillHistoDouble(HMdcSegSim *mdchit);
  void writeHisto2File();
  ClassDef(HMdcCPSelector,0)
    };
    
#endif /* !HMDCCPSELECTOR_H */





