#ifndef  HMDCCPSELECTOR_H
#define  HMDCCPSELECTOR_H

#include "hreconstructor.h"
#include "hkinesim.h"
#include <TVector3.h>
class HGeantKine;
class HCategory;
class HMdcHitSim;
class HMdcSegSim;
class HMdcClusInf;
class HRichGeometryPar;
class HRichPadTab;
class HRichPad;
class HRichHitSim;
class HGeantMdc;
#include <TH1.h>
#include <TNtuple.h>
#include <TString.h>

class HMdcCPSelector : public HReconstructor {
 protected: 

  
  Int_t theta_bin_size;
  Int_t phi_bin_size;
  
  
  
  TNtuple * singles;
  TNtuple * doubles;
  TNtuple * conv_mdcgeant;
  TNtuple * conv_richmirrgeant;
  TNtuple * conv_mdcseg;
  TNtuple * conv_richhit;
  TNtuple * mdc_singles;
  TNtuple * mdc_doubles;
  TH1F * hsystem;
  TNtuple * rich_help;

  

  HIterator *iterator; //! Iterator on raw data
  HCategory *fGeantCat;  //! Pointer to the HGeantKine category
  HCategory *fGeantRichMirrorCat;  //! Pointer to the HGeantKine category
  HCategory *fMdcSegCat;  //! Pointer to the category
  HCategory *fMdcHitCat;  //! Pointer to the category
  HCategory *fRichHitCat;  //! Pointer to the category
  HCategory *fGeantMdc;  //! Pointer to the category
 
  
  HCategory *fClusInf; //! Pointer to the category
  HIterator *iterator_clusinf;  //! Iterator on HMdcClusInf
 

  HIterator *iterator_kine; //! Iterator on HGeantKine
  HIterator *iterator_mdcseg; //! Iterator on HMdcSegSim
  HIterator *iterator_mdchit; //! Iterator on HMdcHitSim
  HIterator *iterator_mdchit_2; //! Iterator on HMdcHitSim
  HIterator *iterator_mdcgeant; //! Iterator on HGeantMdc
  HIterator *iterator_mdcgeant_2; //! Iterator on HGeantMdc

  HIterator *iterator_richhit; //! Iterator on HRichHitSim
  HIterator *iterator_richmirror; //! Iterator on HGeantRichMirror
  

  HRichGeometryPar *fRichGeoPar;
  HRichPadTab *pPadsPar;
  HRichPad *pPad; 

  Int_t  nThreshold,index,dummy,dummy1;
  
  
  Int_t pTrack_parent,nEvCounter;
  Int_t pID;
  TVector3 vec;
  
  char *nameHistoFile;
  Bool_t version_flag;
  Bool_t all_flag;
  void checkCandidates(TNtuple*,Int_t,Int_t& , Int_t&,Int_t&);
 public: 
  HMdcCPSelector(void);
  HMdcCPSelector(Text_t *name,Text_t *title,char *histoname="histograms_convanal.root");
  ~HMdcCPSelector(void);
  HKineSim * kineGeant;
  Bool_t init(void);
  Bool_t finalize(void);
  Int_t execute(void);
  void findConvInMdcSeg();
  void findConvInRichHit();
  void setHistoName(char* name){nameHistoFile=name;}
  HGeantKine* getObj(Int_t nTrack);
  HCategory * getMdcHitCat(){return  fMdcHitCat;} 
  HCategory * getClusInfCat(){return  fClusInf;} 
  
  Bool_t checkConv(HGeantKine *kine); 
  Bool_t checkIfLept(HGeantKine *kine); 
  void analyzeDouble(HMdcSegSim *mdchit);
  void analyzeSingle(HMdcSegSim *mdchit);
  void checkRich(HMdcSegSim *mdchit,Int_t mdccase);
  void findConvPairsInGeantMdc();
  void findConvPairsInGeantRichMirr();
  Bool_t isConvPairsInGeantMdc(Int_t track,Int_t sec,Int_t option=0);
  Bool_t checkRing(HRichHitSim *richhit);  
  Bool_t fillHistoSingle(HMdcSegSim *mdchit); 
  Bool_t fillHistoDouble(HMdcSegSim *mdchit);

  Int_t  getClusterSizeInSeg(HMdcSegSim *mdchit);
  Int_t  getNWiresInSeg(HMdcSegSim *mdchit);
  Int_t  getNMergedClsInSeg(HMdcSegSim *mdchit);
  Float_t getSigma1InHit(HMdcHitSim *mdchit);
  Float_t getSigma1InSeg(HMdcSegSim *segsim);
  Float_t getSigma2InHit(HMdcHitSim *mdchit);
  Float_t getSigma2InSeg(HMdcSegSim *segsim);
  Float_t getAlphaInHit(HMdcHitSim *mdchit);
  Float_t getAlphaInSeg(HMdcSegSim *segsim);

  Int_t  getLevelClFindingInSeg(HMdcSegSim *segsim);
  Int_t  getLevelClFindingInHit(HMdcHitSim *mdchit);
  Int_t getClusterSizeFromHit(HMdcHitSim *);
  Int_t getNWiresFromHit(HMdcHitSim *);
  Int_t getNMergedClustersFromHit(HMdcHitSim *);

  Float_t getGeantPhi(Float_t px,Float_t py);
  Float_t getGeantTheta(Float_t px,Float_t py,Float_t pz);
  
  HMdcHitSim * getMdcHitFromSeg(HMdcSegSim *);
  HMdcClusInf* getClusInfObj(Int_t clsindex);
  void writeHisto2File();
  
  void  setOldVersionModus(){version_flag=kTRUE;}
  void  setAllElectronSinglesModus(){all_flag=kTRUE;}
  ClassDef(HMdcCPSelector,0)
    };
    
#endif /* !HMDCCPSELECTOR_H */






