# ifndef HMDCGEOMPARF_H
# define HMDCGEOMPARF_H

# include "hmdcgeompar.h"
# include "hcategory.h"
# include "hiterator.h"
# include "hreconstructor.h"
# include "hgeomrotation.h"
# include "hgeomvector.h"
# include "TH1.h"
# include "TH2.h"
# include "TTree.h"
# include "TMinuit.h"
# include "TFile.h"
# include "TString.h"
# include "hlocation.h"
# include "hmdchit.h"


class TFile;
void fcnGP(Int_t &npar, Double_t *gin, Double_t &f, Double_t *par, Int_t iflag);

class HMdcGeomParF : public HReconstructor {

 protected:    
  HMdcGeomPar* fMdcGeomPar;   // ! geometrical MDC parameters 

  TString fNameAscii;         // ! file name
  TString fNameRoot;         // ! file name

  TClonesArray* fHits;        // ! Hits matched in HMdcAligner  
  TTree* fTree;               // ! TTree with the data pairs
  HLocation fLocA;            // ! MDC A location (sector,module) 
  HLocation fLocB;            // ! MDC B location (sector,module)
  TFile *fOutFile;            // ! Output file   
  HGeomRotation fRotMat;      // ! rotation matrix (MDCB)=(fRotMat)*(MDCA)+(T) 
  HGeomVector* fTransResult;  // ! Internal translation from HMdcAligner
  HGeomVector fTranslation;   // ! Internal translation
  HGeomVector* fEulerResult;  // ! Euler angles from HMdcAligner
  Double_t fEuler[3];         // ! Euler angles  
  Double_t fError[6];         // ! Errors in tranformation
  HGeomRotation fRotLab;      // ! rotation matrix (MDCB)=(fRotLab)*(MDCA)+(T)  
  HGeomVector fTransLab;      // ! Internal translation
  Double_t fEulerLab[3];      // ! Euler angles  
  Double_t fErrorLab[6];      // ! Errors in tranformation

  Float_t fZLoCut;            // ! Window dimension   
  Float_t fZHiCut;

  Int_t fNEntries;            // ! Number of events
  Int_t fCount;               // ! Valid hits for alignment
  Bool_t fManual;             // ! Manual parameters input flag
  Bool_t fHistograms;         // ! Control histograms on flag
  Float_t fIterCriteria;      // ! Iteration criteria in minimization

 public:

  TH2F* fFPlanex;              // ! projection on planes
  TH2F* fFPlaney;
  TH2F* fFPlanez;

  TH1F* fFTargets;             // ! z proj. of min. distance to Z axis point
  //TH3F tridi;          
  TH2F* fFZVsTheta;            // ! z vs. theta
  TH2F* fFZVsRho;              // ! z vs. rho

  TH1F* fFDisZ;                // ! distance to Z axis
  TH1F* fFDisCenter;           // ! distance to (0,0,0)

  TH1F* fFProzY;               // ! z proj for hresy
  TH1F* fFProzX;               // ! z proj for hresx
  TH1F* fFProyZ;               // ! y proj for hresz
  TH1F* fFProyX;               // ! y proj for hresx
  TH1F* fFProxZ;               // ! x proj for hresz
  TH1F* fFProxY;               // ! x proj for hresy
 
  //TH1F* fFProzYAC;             // ! same but reduced to -fZCut<Z<fZCut
  //TH1F* fFProzXAC;
  //TH1F* fFProyZAC;
  //TH1F* fFProyXAC;
  //TH1F* fFProxZAC;
  //TH1F* fFProxYAC;

  TH2F* fPlanex;              // ! projection on planes
  TH2F* fPlaney;
  TH2F* fPlanez;

  TH1F* fTargets;             // ! z proj. of min. distance to Z axis point
  //TH3F tridi;          
  TH2F* fZVsTheta;            // ! z vs. theta
  TH2F* fZVsRho;              // ! z vs. rho

  TH1F* fDisZ;                // ! distance to Z axis
  TH1F* fDisCenter;           // ! distance to (0,0,0)

  TH1F* fProzY;               // ! z proj for hresy
  TH1F* fProzX;               // ! z proj for hresx
  TH1F* fProyZ;               // ! y proj for hresz
  TH1F* fProyX;               // ! y proj for hresx
  TH1F* fProxZ;               // ! x proj for hresz
  TH1F* fProxY;               // ! x proj for hresy
 
  //TH1F* fProzYAC;             // ! same but reduced to -fZCut<Z<fZCut
  //TH1F* fProzXAC;
  //TH1F* fProyZAC;
  //TH1F* fProyXAC;
  //TH1F* fProxZAC;
  //TH1F* fProxYAC;

public:
  HMdcGeomParF(void);    
  HMdcGeomParF(Int_t sector, Int_t modA, Int_t modB);  
  HMdcGeomParF(Text_t* name, Text_t* title, Int_t sector, Int_t modA, Int_t modB);  

  ~HMdcGeomParF(void);  

  Bool_t init(void);    
  Bool_t reinit(void);  
  Bool_t initDelayed(void);
  Bool_t finalize(void);
  Int_t  execute(void);

  void setModstoAlign(Int_t sector, Int_t modA, Int_t modB);

  void setParContainers();
  void setGeomAuxPar();
  void setHistograms(void);
  void setEulerAngles(Float_t f, Float_t s, Float_t t);
  void setEulerLabAngles(Float_t f, Float_t s, Float_t t);
  TTree* getTree(void){return fTree;}
  //TClonesArray* getHits(void){return fHits;}

  void fillRotMatrix(Float_t prim, Float_t segu, Float_t terc);
  void fillRotLab(Float_t prim, Float_t segu, Float_t terc);
  void fillTranslation(Float_t x, Float_t y, Float_t z);
  void fillTransLab(Float_t x, Float_t y, Float_t z);

  void findZComponent(void);
  void writeGeomParContainer(void);
  void storeInFile(void);

  void setRelParams(HGeomVector* ang, HGeomVector* tra);

  HGeomRotation getRotMatrix(void){return fRotMat;}
  HGeomVector getTranslation(void){return fTranslation;}
  HGeomRotation getRotLab(void){return fRotLab;}
  HGeomVector getTransLab(void){return fTransLab;}

  void fillAuxHisto(void);
  void setSearchLimits(Float_t x, Float_t y);
  void setGeomParManOn(void);    
  void setControlHistoOff(void);
  void setIterCriteria(Float_t criteria);
  void minfit(Int_t fix, Float_t fE, Float_t sE, Float_t tE, Float_t fT, Float_t sT, Float_t tT);

  void setOutputAscii(TString name);   
  void setOutputRoot(TString name);   
  
  ClassDef(HMdcGeomParF,1)  
};

# endif /* !HMDCGEOMPARF_H */
 
