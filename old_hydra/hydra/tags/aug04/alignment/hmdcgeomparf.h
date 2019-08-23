# ifndef HMDCGEOMPARF_H
# define HMDCGEOMPARF_H

# include "hmdcgeompar.h"
# include "hcategory.h"
# include "hiterator.h"
# include "hreconstructor.h"
# include "hgeomrotation.h"
# include "hgeomvector.h"
# include "hgeomvertexfit.h"
# include "TH1.h"
# include "TH2.h"
# include "TH3.h"
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
  
  HMdcGeomPar* fMdcGeomPar;   //! geometrical MDC parameters 

  TString fNameAscii;         //! file name
  TString fNameRoot;          //! file name

  HCategory* fHitCat;         //! category of Hits in the MDCs (for one MDC)
  HIterator* fIter1;          //! Iterator on the category  

  TClonesArray* fHits;        //! Hits matched in HMdcAligner  
  TTree* fTree;               //! TTree with the data pairs
  HLocation fLoc;             //! MDC location (sector,moduleA,moduleB...) 
  TFile *fOutFile;            //! Output file   
  HGeomRotation* fRotMat;     //! rotation matrix (MDCB)=(fRotMat)*(MDCA)+(T) 
  HGeomVector* fTransResult;  //! Internal translation from HMdcAligner
  HGeomVector* fTranslation;  //! Internal translation
  HGeomVector* fEulerResult;  //! Euler angles from HMdcAligner
  HGeomVector* fEuler;      //! Euler angles  
  Double_t* fError;         //! Errors in tranformation
  HGeomRotation fRotLab;      //! rotation matrix (MDCB)=(fRotLab)*(MDCA)+(T)
  HGeomVector fTransLab;      //! Internal translation
  HGeomVector fEulerLab;      //! Euler angles  
  Double_t fErrorLab[6];      //! Errors in tranformation

  Float_t fZLoCut;            //! Window dimension   
  Float_t fZHiCut;

  Int_t fNumMods;             //! Number of MDCs
  Int_t fHistNum;          //! Number of histogram families
  Int_t fFix;                 //! Fixing parameters in minimization
  Int_t fNEntries;            //! Number of events
  Int_t fCount;               //! Valid hits for alignment
  Int_t fMin;                 //! Minimization flag
  Bool_t fManual;             //! Manual parameters input flag
  Bool_t fHistoOff;           //! Control histograms on flag
  Bool_t fUseCut;             //! Control the Tree used (with or without cuts)
  Bool_t fUseInitValue;       //! Selects the init value mode
  Bool_t fUseAngleCut;        //! Select Hits for 1MDC alignment
  Bool_t fUseYPosCut;         //! Select Hits for 1MDC alignment
  Bool_t fUseUnitErrors;      //! 
  Bool_t fTFFor2Mods;         //! 
  Float_t fAngleCut;          //! Min. theo. angle allowed in 1MDC alignment
  Float_t fYPosCut;           //! Min. Hit Y coord. allowed in 1MDC alignment
  Float_t fIterCriteria;      //! Iteration criteria in minimization
  Float_t fConstTukey;        //! Tukey constant in 1 MDC target finder
  Float_t fStepTukey;         //! Size of the step between different Tukey constant
  Float_t fResultTukey;       //! Tukey constant for which the calculations are made
  Float_t fEndTukey;          //! Last Tukey constant checked in the method 

  HGeomVector fInitValue;     //! Init value for targetFinder()
 public:

  TH2F** fPlanex;              //! projection on planes
  TH2F** fPlaney;              //!
  TH2F** fPlanez;              //!

  TH1F** fTarTuk;             //! z proj. of min. distance to Z axis point
  TH1F** fTarTukW;            //! z proj. of min. distance to Z axis point
  TH3F** fDisCTuk;            //! distance to (0,0,0)

  TH1F** fTargets;             //! z proj. of min. distance to Z axis point
  TH1F** fTarMDC;              //! z proj. of min. distance to Z axis point
  TH1F** fTarCut;              //! z proj. of min. distance to Z axis point
  TH1F** fTarMDCCut;           //! z proj. of min. distance to Z axis point
  //TH3F tridi;          
  TH2F** fZVsTheta;            //! z vs. theta
  TH2F** fZVsRho;              //! z vs. rho

  TH1F** fDisZ;                //! distance to Z axis
  TH1F** fDisCenter;           //! distance to (0,0,0)

  TH1F** fProzY;               //! z proj for fPlaney
  TH1F** fProzX;               //! z proj for fPlanex
  TH1F** fProyZ;               //! y proj for fPlanez
  TH1F** fProyX;               //! y proj for fPlanex
  TH1F** fProxZ;               //! x proj for fPlanez
  TH1F** fProxY;               //! x proj for fPlaney
 
  //Float_t* fWeights;          //! Weights in the minimization fcn 
  Float_t* fSteps;            //! Step for minimization 
  Float_t* fLimits;           //! Limits for minimization 
      
public:
  HMdcGeomParF(void);    
  HMdcGeomParF(Int_t sector, Int_t modA, Int_t modB);  
  HMdcGeomParF(Text_t* name, Text_t* title, Int_t sector, 
	       Int_t modA, Int_t modB,Int_t modC=-1, Int_t modD=-1);  
  HMdcGeomParF(Text_t* name, Text_t* title, Int_t sector, Int_t modA);
  ~HMdcGeomParF(void);  

  Bool_t init(void);    
  Bool_t reinit(void);  
  Bool_t initDelayed(void);
  Bool_t finalize(void);
  Int_t  execute(void);
  void execute1(void);
  void execute0(void);
 
  void initDefaults(void);
  void initMinimization(void);

  void setParContainers(void);
  void setGeomAuxPar(void);
  HGeomVector findEulerAngles(HGeomRotation rot);
  void setHistograms(void);
  void setRelParams(HGeomVector*& ang,HGeomVector*& tra);
		    //		    HGeomVector*& ang2,HGeomVector*& tra2,
		    //		    HGeomVector*& ang3,HGeomVector*& tra3);
  void setGeomParManOn(void);
  void setControlHistoOff(void);
  void setMinimization(Int_t select=1);
  void setUnitErrors(void);
  void setFix(Int_t fix);
  void setNoCut(void);
  void setAngleCut(Float_t aCut);
  void setYPosCut(Float_t aYCut);
  void setTukeyConstant(Float_t cte,Float_t step=1, Float_t res=1, Float_t end=0.4);
  void setInitValue(Float_t x, Float_t y, Float_t z);
  void setTargetFinderFor2Mods(void);

  void fillRotMatrix(Int_t i,Float_t prim,Float_t segu, Float_t terc); 
  void fillTranslation(Int_t i,Float_t x,Float_t y, Float_t z); 
  void fillRotLab(Float_t prim,Float_t segu, Float_t terc);
  void fillTransLab(Float_t x,Float_t y, Float_t z);
  void setEulerAngles(Int_t, Float_t f,Float_t s, Float_t t); 
  void setEulerLabAngles(Float_t f,Float_t s, Float_t t);
  void setSearchLimits(Float_t x, Float_t y);
  void setIterCriteria(Float_t cri);
  void setSteps(Float_t s0,Float_t s1,Float_t s2,
		Float_t s3, Float_t s4, Float_t s5);
  void setLimits(Float_t l0,Float_t l1,Float_t l2,
		 Float_t l3, Float_t l4, Float_t l5); 

  void minfit(Int_t fix, Float_t fE, Float_t sE, 
	      Float_t tE, Float_t fT, Float_t sT, Float_t tT);
  void findZComponent(void);

  TTree* getTree(void){return fTree;}
  HGeomRotation getRotMatrix(Int_t i){return fRotMat[i];}
  HGeomVector getTranslation(Int_t i){return fTranslation[i];}
  HGeomRotation getRotLab(void){return fRotLab;}
  HGeomVector getTransLab(void){return fTransLab;}
  Bool_t targetFinder(ofstream *fout);

  void fillHistograms(Int_t index);
  void storeInFile(void);
  void writeGeomParContainer(void);

  void setOutputAscii(TString name);
  void setOutputRoot(TString name);

  ClassDef(HMdcGeomParF,1)  // finder for the MDC geometrical parameters 
};

# endif /* !HMDCGEOMPARF_H */
 
