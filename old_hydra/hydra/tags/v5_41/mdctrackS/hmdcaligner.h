# ifndef HMDCALIGNER_H
# define HMDCALIGNER_H

# include "hreconstructor.h"
# include "hmdcgeompar.h"
# include "hcategory.h"
# include "hiterator.h"
# include "hgeomrotation.h"
# include "hgeomvector.h"
# include "TH1.h"
# include "TNtuple.h"
# include "TMinuit.h"
# include "TFile.h"
# include "TString.h"
# include "hlocation.h"
# include "hmdchit.h"


class TFile;
void fcnalign(Int_t &npar, Double_t *gin, Double_t &f, Double_t *par, Int_t iflag);

class HMdcAligner : public HReconstructor {

 protected:    
  HMdcGeomPar* fMdcGeomPar;   // ! geometrical MDC parameters 

  TString fNameAscii;         // ! file name
  TString fNameRoot;          // ! file name

  //TFile* fOutRoot;            // ! ROOT file
  HCategory* fHitCat;         // ! category of Hits in the MDCs
  HIterator* fIter1;          // ! Iterator on the category  
  HIterator* fIter2;          // ! Iterator on the category
  TClonesArray* fHits;        // ! Hits matched 
  HLocation fLocA;            // ! MDC A location (sector,module) 
  HLocation fLocB;            // ! MDC B location (sector,module)
  HGeomRotation fRotMat;      // ! rotation matrix (MDCB)=(matrix)*(MDCA)+(T)
  HGeomVector fTranslation;   // ! Internal translation
  HGeomVector fEuler;         // ! Euler angles  
  Double_t fError[6];         // ! Errors in tranformation
  Float_t fXArea;             // ! Window dimension   
  Float_t fYArea;             // ! Window dimension  
  Int_t fDiscart;             // ! Discarted hits 
  Int_t fHitsMdcA;            // ! Hits in Mdc A
  Int_t fHitsFoundInWindow;   // ! Number of matches in the window
  Int_t fNEntries;            // ! Number of events
  Int_t fCount;               // ! Valid hits for alignment
  Bool_t fManual;             // ! Manual input on flag
  Bool_t fAuto;               // ! Automatical input on flag
  Bool_t fHistograms;         // ! Control histograms on flag
  Float_t fIterCriteria;      // ! Iteration criteria in minimization

 public:

  //HGeomVector fTranslation;    // ! Internal translation

  TH1F* fResX;                // ! X distance (all combinations)
  TH1F* fResY;                // ! Y distance (all combinations)
  TH1F* fResS0;               // ! X Slope proj distance (all comb.)
  TH1F* fResS1;               // ! Y Slope proj distance (all comb.)
  TH1F* fAccResX;             // ! X distance (accep. hit pairs)
  TH1F* fAccResY;             // ! Y distance (accep. hit pairs)
  TH1F* fAccResS0;            // ! X Slope proj distance (accep. hit pairs)
  TH1F* fAccResS1;            // ! Y Slope proj distance (accep. hit pairs)  
  TH1F* fFAccResX;            // ! X distance (final parameters!)
  TH1F* fFAccResY;            // ! Y distance (final parameters!)
  TH1F* fFAccResS0;           // ! X Slope proj distance (final parameters!)
  TH1F* fFAccResS1;           // ! Y Slope proj distance (final parameters!)
  TTree* fAlignAll;           // ! all data from hits in module pairs

  static Int_t fRecCount;     // ! Number of reconstructors      
  static TFile *fOutRoot;     // ! Output file   

  Float_t* fWeights;          // ! Weights in the minimization fcn 
  Float_t* fSteps;            // ! Step for minimization 
  Float_t* fLimits;           // ! Limits for minimization 
      
public:
  HMdcAligner(void);    
  HMdcAligner(Int_t sector, Int_t modA, Int_t modB);  
  HMdcAligner(Text_t* name, Text_t* title, Int_t sector, Int_t modA, Int_t modB);
  ~HMdcAligner(void);  

  Bool_t init(void); 
  Bool_t reinit(void);
  Bool_t finalize(void);
  Int_t  execute(void);

  void initMinimization();
  void setModstoAlign(Int_t sector, Int_t modA, Int_t modB);

  void setParContainers();
  void setGeomAuxPar();
  void setGeomAuxParSim(Float_t x=0,Float_t y=0, Float_t z=0, Float_t prim=0,Float_t segu=0, Float_t terc=0);
  void setHistograms(void);  
  void setTree(void);
  void setEulerAngles(Float_t f, Float_t s, Float_t t);
  TTree* getTree(void){return fAlignAll;}
  TClonesArray* getHits(void){return fHits;}
  Float_t* getWeights(void){return fWeights;}

  Int_t getRelParams(HGeomVector*& ang,HGeomVector*& tra);

  void fillHistograms(void);  
  void storeInFile(void);
  void fillRotMatrix(Float_t prim,Float_t segu, Float_t terc);
  void fillTranslation(Float_t x,Float_t y, Float_t z);
  void setSearchLimits(Float_t x, Float_t y);
  void setGeomParManOn(void);    
  void setGeomParAutoOn(void);
  void setControlHistoOff(void);
  void setIterCriteria(Float_t criteria);  
  void setWeights(Float_t w0,Float_t w1,Float_t w2,Float_t w3);    
  void setSteps(Float_t s0,Float_t s1,Float_t s2,Float_t s3, Float_t s4, Float_t s5);
  void setLimits(Float_t l0,Float_t l1,Float_t l2,Float_t l3, Float_t l4, Float_t l5);
  void minfit(Int_t fix, Float_t fE, Float_t sE, Float_t tE, Float_t fT, Float_t sT, Float_t tT);

  void setOutputAscii(TString name);
  void setOutputRoot(TString name);

  ClassDef(HMdcAligner,1)  
};

# endif /* !HMDCALIGNER_H */





