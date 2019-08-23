# ifndef HMDCALIGNER_H
# define HMDCALIGNER_H

# include "hreconstructor.h"
# include "hmdcgeompar.h"
# include "hcategory.h"
# include "hiterator.h"
# include "hgeomrotation.h"
# include "hgeomvector.h"
# include "hgeomvertexfit.h"
# include "TH1.h"
# include "TGraph.h"
# include "TH2.h"
# include "TNtuple.h"
# include "TMinuit.h"
# include "TFile.h"
# include "TString.h"
# include "hlocation.h"
# include "hmdchit.h"


class TFile;
void fcnalign(Int_t &npar, Double_t *gin, Double_t &f, Double_t *par, Int_t iflag);
void fcnalign34(Int_t &npar, Double_t *gin, Double_t &f, Double_t *par, Int_t iflag);
void fcnRot(Int_t &npar, Double_t *gin, Double_t &f, Double_t *par, Int_t iflag);
//void fcnRotDist(Int_t &npar, Double_t *gin, Double_t &f, Double_t *par, Int_t iflag);
class HMdcAligner : public HReconstructor {

 protected:    
  HMdcGeomPar* fMdcGeomPar;   //! geometrical MDC parameters 

  TString fNameAscii;         //! file name
  TString fNameRoot;          //! file name

  HCategory* fHitCat;         //! category of Hits in the MDCs
  HIterator* fIter1;          //! Iterator on the category  
  HIterator* fIter2;          //! Iterator on the category
  HIterator* fIter3;          //! Iterator on the category  
  HIterator* fIter4;          //! Iterator on the category
  TClonesArray* fHits;        //! Hits matched 
  HLocation fLoc;             //! MDC location (sector,modules) 
  HGeomRotation* fRotMat;     //! rotation matrix (MDCB)=(matrix)*(MDCA)+(T)
  HGeomVector* fTranslation;  //! Internal translation
  HGeomVector* fEuler;        //! Euler angles    
  //HGeomRotation* fRotInv;   //! inv rot matrix (MDCA)=(matrix)*(MDCB)+(T)
  //HGeomVector* fTrasInv;    //! Inverse internal translation
  //HGeomVector* fEulerInv;   //! Inverse Euler angles  
  Double_t* fError;           //! Errors in tranformation
  HGeomVector fTargetPos;     //! Target position
  HGeomVector theLocalTarget; //! Target position in second MDC
  Float_t fXArea;             //! Window dimension   
  Float_t fYArea;             //! Window dimension  
  Float_t fSArea;            //! Window dimension   
  Int_t* fDiscart;            //! Discarted hits 
  Int_t* fHitsMdc;            //! Hits in MDC
  Int_t* fHitsFoundInWindow;  //! Number of matches in the window
  Int_t* fHitsFoundAndUnique; //! Number of unique matches in the window
  Float_t* zones_X;           //! X coordinate of Zones center 
  Float_t* zones_Y;           //! Y coordinate of Zones center 
  Float_t* zones_DX;          //! X side length of Zones  
  Float_t* zones_DY;          //! Y side length of Zones  
  Float_t* resZon_X;          //! X position of the projection vs zone center
  Float_t* resZon_Y;          //! Y position of the projection vs zone center
  Int_t fNEntries;            //! Number of events
  Int_t fCount;               //! Valid matched hit set for alignment
  Int_t fCountCut;            //! Valid matched hit set after cuts
  Bool_t fManual;             //! Manual input on flag
  Bool_t fAuto;               //! Automatical input on flag
  Bool_t fHistoOff;           //! Control histograms on flag  
  Int_t fMin;                 //! Control minimization
  Float_t fIterCriteria;      //! Iteration criteria in minimization
  Int_t fNumMods;             //! Number of modules to align
  Int_t fHistNum;             //! Number of Histo  
  Int_t fHistZonNum;          //! Number of Zones Histos  
  Bool_t fUseCut;             //! Cut in N sigmas  
  Bool_t fUseCutRot;          //! Cut in N sigmas used in Rotation around X or Y axes  
  Bool_t fUseCutRotZ;         //! Cut in N sigmas used in Rotation around Z axis
  Bool_t fUseSlopeCut;        //! Cut perp incidence in MDC 
  Bool_t fUseTarget;          //! Use target instead of hit slopes
  Bool_t fUseZones;           //! Use Zones to determine rotations
  Float_t fSlopeCut;          //! Cut parameter for perp incidence in MDC 
  Float_t fSigmaX;            //! Covariance matrix elements
  Float_t fSigmaY;            //!
  Float_t fSigmaS0;           //!
  Float_t fSigmaS1;           //!
  Float_t fRhoxSx;            //!
  Float_t fRhoySy;            //!
  Float_t fRhoxSy;            //!  
  Float_t fXSigmas;           //! Number of sigmas for the X residual cut
  Float_t fYSigmas;           //! Number of sigmas for the Y residual cut  
  Float_t fS0Sigmas;          //! Number of sigmas for the S0 residual cut  
  Float_t fS1Sigmas;          //! Number of sigmas for the S1 residual cut  
  Float_t fConstTukey;        //! Tukey constant in 1MDC target finder
  Int_t fFix;                 //! Fixing parameters in minimization
  Bool_t fUseUnitErrors;      //! Sets unit errors for all point in the fit
  Bool_t fUseModErrors;    //! Sets manually the errors for the Hit points
  Bool_t fDoNotUseCov;        //! Sets off the covariance in the trans. Finder
  Bool_t fUseSharpCut;        //! Sets off the covariance in fitHistograms()
  Int_t fMinFlag;             //! Just a flag for minimization 
  Bool_t fSetHomogeneousDistribution; //!(Quasi)homogeneous distribution flag
  Bool_t fCloseToSolution;    //! A flag for better fitting close to solution 
  Int_t fMaxEntriesPerZone;   //!
  Int_t* fEntriesPerZone;     //!
  Double_t fFunctionMin;      //! Functional minimum
  Float_t* fPosError;         //! Errors in the module coordinates

 public:
  TH1F* AvsCinCCS_Polar_Stripe1; //!
  TH1F* AvsCinCCS_Polar_Stripe2; //!
  TH1F* AvsCinCCS_Polar_Stripe3; //!
  TH1F* AvsCinCCS_Polar_Stripe4; //!
  TH1F* AvsCinCCS_Polar_Stripe5; //!
  TH1F* BvsCinCCS_Polar_Stripe1; //!
  TH1F* BvsCinCCS_Polar_Stripe2; //!
  TH1F* BvsCinCCS_Polar_Stripe3; //!
  TH1F* BvsCinCCS_Polar_Stripe4; //!
  TH1F* BvsCinCCS_Polar_Stripe5; //!

  TH1F** CvsDinDCS_X;  //! Histograms for studying residuals in projections 
  TH1F** CvsDinDCS_Y;       //!  Key: CvsDinDCS_X means that the plot is
  TH1F** CvsDinDCS_XSlope;  //! the X position of a Hit in MDC D minus
  TH1F** CvsDinDCS_YSlope;  //! the X position of a Hit in MDC C
  TH1F** BvsDinDCS_X;       //! projected on MDC D coordinate system
  TH1F** BvsDinDCS_Y;       //! (that is inDCS ;-)
  TH1F** BvsDinDCS_XSlope;  //!
  TH1F** BvsDinDCS_YSlope;  //!
  TH1F** AvsDinDCS_X;       //!
  TH1F** AvsDinDCS_Y;       //!
  TH1F** AvsDinDCS_XSlope;  //!
  TH1F** AvsDinDCS_YSlope;  //! 
  TH1F** DvsCinCCS_X;       //!
  TH1F** DvsCinCCS_Y;       //!
  TH1F** DvsCinCCS_XSlope;  //! 
  TH1F** DvsCinCCS_YSlope;  //!
  TH1F** BvsCinCCS_X;       //!
  TH1F** BvsCinCCS_Y;       //!
  TH1F** BvsCinCCS_XSlope;  //!
  TH1F** BvsCinCCS_YSlope;  //!
  TH1F** AvsCinCCS_X;       //!
  TH1F** AvsCinCCS_Y;       //!
  TH1F** AvsCinCCS_XSlope;  //!
  TH1F** AvsCinCCS_YSlope;  //!
  TH1F** DvsBinBCS_X;       //!
  TH1F** DvsBinBCS_Y;       //!
  TH1F** DvsBinBCS_XSlope;  //!
  TH1F** DvsBinBCS_YSlope;  //!
  TH1F** CvsBinBCS_X;       //!
  TH1F** CvsBinBCS_Y;       //!
  TH1F** CvsBinBCS_XSlope;  //!
  TH1F** CvsBinBCS_YSlope;  //!
  TH1F** AvsBinBCS_X;       //!
  TH1F** AvsBinBCS_Y;       //!
  TH1F** AvsBinBCS_XSlope;  //!
  TH1F** AvsBinBCS_YSlope;  //!
  TH1F** DvsAinACS_X;       //!
  TH1F** DvsAinACS_Y;       //!
  TH1F** DvsAinACS_XSlope;  //!
  TH1F** DvsAinACS_YSlope;  //!
  TH1F** CvsAinACS_X;       //!
  TH1F** CvsAinACS_Y;       //!
  TH1F** CvsAinACS_XSlope;  //!
  TH1F** CvsAinACS_YSlope;  //!
  TH1F** BvsAinACS_X;       //!
  TH1F** BvsAinACS_Y;       //!
  TH1F** BvsAinACS_XSlope;  //!
  TH1F** BvsAinACS_YSlope;  //!  
  TH1F** BCvsAinACS_X;       //!
  TH1F** BCvsAinACS_Y;       //!
  TH1F** BCvsACinACS_XSlope;  //!
  TH1F** BCvsACinACS_YSlope;  //!
  TH1F** ABvsCinCCS_X;       //!
  TH1F** ABvsCinCCS_Y;       //!
  TH1F** ABvsCinCCS_XSlope;  //!
  TH1F** ABvsCinCCS_YSlope;  //!
  TH1F** ACvsBinBCS_X;       //!
  TH1F** ACvsBinBCS_Y;       //!
  TH1F** ACvsBinBCS_XSlope;  //!
  TH1F** ACvsBinBCS_YSlope;  //!
  TH1F** XChi2Hist;          //! chi2 from XZ plane fit 
  TH1F** YChi2Hist;          //! chi2 from YZ plane fit
  TH1F** TotalChi2;         //! sum of chi2 from both fits
  TH1F** SqrDistToA;        //! square distance from fitted str.line to A
  TH1F** SqrDistToB;        //! square distance from fitted str.line to B
  TH1F** SqrDistToC;        //! square distance from fitted str.line to C
  TH1F** SqrDistToD;        //! square distance from fitted str.line to D
  TH1F** SqrDist;           //! Sum of square distances
  TH1F* fResX;              //! All residual combinations X    
  TH1F* fResY;              //! All residual combinations Y
  TH1F* fResS0;             //! All residual combinations S0
  TH1F* fResS1;             //! All residual combinations S1
  TH1F** BvsAinACS_X_Zone;  //!
  TH1F** BvsAinACS_Y_Zone;  //!
  TH2F** BvsAinACS_Zon;     //! 2D residuals in MDC selected zones
  //TH2F** BvsAinACS_Zon2;    //!
  TH1F* hisrotZ;            //! angles from analysis of rotations
  //TH1F* hisrotZW;           //! angles from analysis of rotations
  TH1F* hisrotZZon;         //! angles from analysis of rotations
  TH1F* hisrotY;            //!
  //TH1F* hisrotYW;           //!
  TH1F* hisrotYZon;         //!
  TH1F* hisrotX;            //!
  //TH1F* hisrotXW;           //!
  TH1F* hisrotXZon;         //!
  TH1F* hisrotYPos;         //!
  TH1F* hisrotXPos;         //!
  TH1F* hisrotYNeg;         //!
  TH1F* hisrotXNeg;         //!
  TH1F* hisrotYZonPos;      //!
  TH1F* hisrotXZonPos;      //!
  TH1F* hisrotYZonNeg;      //!
  TH1F* hisrotXZonNeg;      //!
  TGraph* graphXchi2X;            //!
  TGraph* graphXchi2Y;            //!
  TGraph* graphXchi2;            //!
  TGraph* graphYchi2X;            //!
  TGraph* graphYchi2Y;            //!
  TGraph* graphYchi2;            //!

  TGraph* graphCont;            //!
  
  TH1F* DiffACvsAinACS_YSlope_Stripe1; //!
  TH1F* DiffACvsAinACS_YSlope_Stripe2; //!
  TH1F* DiffACvsAinACS_YSlope_Stripe3; //!
  TH1F* DiffACvsAinACS_YSlope_Stripe4; //!
  TH1F* DiffACvsAinACS_YSlope_Stripe5; //!
  TH1F* DiffACvsBinACS_YSlope_Stripe1; //!
  TH1F* DiffACvsBinACS_YSlope_Stripe2; //!
  TH1F* DiffACvsBinACS_YSlope_Stripe3; //!
  TH1F* DiffACvsBinACS_YSlope_Stripe4; //!
  TH1F* DiffACvsBinACS_YSlope_Stripe5; //!
  TH1F* DiffACvsCinACS_YSlope_Stripe1; //!
  TH1F* DiffACvsCinACS_YSlope_Stripe2; //!
  TH1F* DiffACvsCinACS_YSlope_Stripe3; //!
  TH1F* DiffACvsCinACS_YSlope_Stripe4; //!
  TH1F* DiffACvsCinACS_YSlope_Stripe5; //!

  TH1F** DiffBCvsAinACS_XSlope;   //!
  TH1F** DiffBCvsAinACS_YSlope;   //!
  TH1F** DiffBCvsBinACS_XSlope;   //!
  TH1F** DiffBCvsBinACS_YSlope;   //!
  TH1F** DiffBCvsCinACS_XSlope;   //!
  TH1F** DiffBCvsCinACS_YSlope;   //!
  TH1F** DiffACvsAinACS_XSlope;   //!
  TH1F** DiffACvsAinACS_YSlope;   //!
  TH1F** DiffACvsBinACS_XSlope;   //!
  TH1F** DiffACvsBinACS_YSlope;   //!
  TH1F** DiffACvsCinACS_XSlope;   //!
  TH1F** DiffACvsCinACS_YSlope;   //!
  TH1F** DiffBCvsAinACS_XSlopeLow;   //!
  TH1F** DiffBCvsAinACS_YSlopeLow;   //!
  TH1F** DiffBCvsBinACS_XSlopeLow;   //!
  TH1F** DiffBCvsBinACS_YSlopeLow;   //!
  TH1F** DiffBCvsCinACS_XSlopeLow;   //!
  TH1F** DiffBCvsCinACS_YSlopeLow;   //!
  TH1F** DiffACvsAinACS_XSlopeLow;   //!
  TH1F** DiffACvsAinACS_YSlopeLow;   //!
  TH1F** DiffACvsBinACS_XSlopeLow;   //!
  TH1F** DiffACvsBinACS_YSlopeLow;   //!
  TH1F** DiffACvsCinACS_XSlopeLow;   //!
  TH1F** DiffACvsCinACS_YSlopeLow;   //!
  TH1F** DiffBCvsAinACS_XSlopeUp;   //!
  TH1F** DiffBCvsAinACS_YSlopeUp;   //!
  TH1F** DiffBCvsBinACS_XSlopeUp;   //!
  TH1F** DiffBCvsBinACS_YSlopeUp;   //!
  TH1F** DiffBCvsCinACS_XSlopeUp;   //!
  TH1F** DiffBCvsCinACS_YSlopeUp;   //!
  TH1F** DiffACvsAinACS_XSlopeUp;   //!
  TH1F** DiffACvsAinACS_YSlopeUp;   //!
  TH1F** DiffACvsBinACS_XSlopeUp;   //!
  TH1F** DiffACvsBinACS_YSlopeUp;   //!
  TH1F** DiffACvsCinACS_XSlopeUp;   //!
  TH1F** DiffACvsCinACS_YSlopeUp;   //!
  TH1F* AvsCinCCS_Polar;   //!
  TH1F* BvsCinCCS_Polar;   //!
  
  TTree* fAlignAll;         //! all data from hits in module pairs
  TTree* fAlignAllCut;      //! data after cuts 

  static Int_t fRecCount;     //! Number of reconstructors      
  static TFile *fOutRoot;     //! Output file   

  Float_t* fWeights;          //! Weights in the minimization fcn 
  Float_t* fSteps;            //! Step for minimization 
  Float_t* fLimits;           //! Limits for minimization 
      
public:
  HMdcAligner(void);    
  HMdcAligner(Int_t sector, Int_t modA, Int_t modB);  
  HMdcAligner(Text_t* name, Text_t* title, Int_t sector, 
	      Int_t modA, Int_t modB, Int_t modC=-1, Int_t modD=-1);
  HMdcAligner(Text_t* name, Text_t* title, Int_t sector, Int_t modA);
  ~HMdcAligner(void);  

  Bool_t init(void); 
  Bool_t reinit(void);
  Bool_t finalize(void);
  Int_t  execute(void);

  void execute4(void);    
  void execute3(void);
  void execute2(void);
  void execute1(void);

  void initDefaults();
  void initMinimization();
  HGeomVector findEulerAngles(HGeomRotation rot);

  void setParContainers();
  void setGeomAuxPar();
  void setGeomAuxParSim(Int_t ind, Float_t eu1, Float_t eu2, Float_t eu3, 
			Float_t tr1, Float_t tr2, Float_t tr3);
  void setHistograms(void);  
  void setTree(void);
  void setEulerAngles(Int_t ind,Float_t f, Float_t s, Float_t t);
  TTree* getTree(void){if(fUseCut) return fAlignAllCut;
  return fAlignAll;}
  void setRotMatrix(Int_t ind,HGeomRotation newRot){fRotMat[ind]=newRot;}
  void setMinFlag(Int_t flag){fMinFlag=flag;}

  Int_t getMinFlag(void){return fMinFlag;}
  HGeomRotation getRotMatrix(Int_t i){return fRotMat[i];}
  HGeomVector getTranslation(Int_t i){return fTranslation[i];}
  Int_t getStrategy(void){return fMin;}
  Float_t* getZonesX(void){return zones_X;}
  Float_t* getZonesY(void){return zones_Y;}
  Float_t* getResZonX(void){return resZon_X;}
  Float_t* getResZonY(void){return resZon_Y;}
  HGeomVector getLocalTarget(HGeomRotation rot,HGeomVector tran);
  TClonesArray* getHits(void){return fHits;}
  Float_t* getWeights(void){return fWeights;}    
  Float_t* getErrors(void){return fPosError;}
  Int_t getNumMods(void){return fNumMods;}
  Bool_t getUseUnitErrors(void){return fUseUnitErrors;}
  Bool_t getUseModErrors(void){return fUseModErrors;}
  Int_t getRelParams(HGeomVector*& ang,HGeomVector*& tra);
//HGeomVector*& ang2,HGeomVector*& tra2,
//HGeomVector*& ang3,HGeomVector*& tra3);

  void transfEuler(HGeomRotation eulrot,HGeomVector eulvec,
		   HGeomVector oldV, HGeomVector newV);
  void transfEulerInv(HGeomRotation eulrot,HGeomVector eulvec,
		      HGeomVector oldV, HGeomVector newV);
  void findAbsolutePosition(HGeomRotation* rot, HGeomVector* vect);
  HGeomVector findProjPoint(HMdcHit* pHit, HGeomRotation rot,
			    HGeomVector tra, Float_t* slopes);
  Bool_t isInsideWindow(Int_t plot, HMdcHit* pHit, HGeomVector point,
			Float_t* slope);
  void mergeHits(HMdcHit* hitB, HMdcHit* hitA,
		 HGeomRotation rot,HGeomVector tra,
		 HMdcHit* mergeHit);
  void transformToSlopes(HMdcHit* pHit, Float_t* slopes);
  void fillHistograms(Int_t index, Int_t select=0);  
  void fitHistograms(Int_t index);  
  void storeInFile(void);
  void fillRotMatrix(Int_t ind,Float_t prim,Float_t segu, Float_t terc);
  void fillTranslation(Int_t ind,Float_t x,Float_t y, Float_t z);
  void divideInZones(Int_t loop,HGeomRotation rot,HGeomVector tran);
  void setSearchLimits(Float_t x, Float_t y, Float_t s=1.);
  void setSigmas(Float_t XSigmas, Float_t YSigmas, 
		 Float_t S0Sigmas, Float_t S1Sigmas);
  void setTukeyConstant(Float_t cte);
  void setGeomParAutoOn(void);
  void setControlHistoOff(void);
  void setMinimization(Int_t select=1);
  void setUnitErrors(void);  
  void setOffCov(void);  
  void setSharpCut(void);  
  void setHomogeneousDistribution(Int_t maxEntries=1000);
  void setFix(Int_t fix);
  void setNoCut(void);
  void setCutRot(void);
  void setCutRotZ(void);
  void setNoZones(void);
  void setCloseToSolution(void);
  void setTargetOn(HGeomVector target);
  void setSlopeCut(Float_t SlopeCut=0.1);
  void setIterCriteria(Float_t criteria);  
  void setWeights(Float_t w0,Float_t w1,Float_t w2,Float_t w3);    
  void setModErrors(Float_t errXModA,Float_t errYModA,Float_t errXModB,
		    Float_t errYModB,Float_t errXModC,Float_t errYModC,
		    Float_t errXModD,Float_t errYModD);    
  void setSteps(Float_t s0,Float_t s1,Float_t s2,
		Float_t s3, Float_t s4, Float_t s5,
		Float_t s6=0., Float_t s7=0., Float_t s8=0.,
		Float_t s9=0., Float_t s10=0., Float_t s11=0.,
		Float_t s12=0., Float_t s13=0., Float_t s14=0.,
		Float_t s15=0., Float_t s16=0., Float_t s17=0.);
  void setLimits(Float_t l0,Float_t l1,Float_t l2,
		 Float_t l3,Float_t l4,Float_t l5,
		 Float_t l6=0., Float_t l7=0., Float_t l8=0.,
		 Float_t l9=0., Float_t l10=0., Float_t l11=0.,
		 Float_t l12=0., Float_t l13=0., Float_t l14=0.,
		 Float_t l15=0., Float_t l16=0., Float_t l17=0.);
  void setZones(Float_t* X=0,Float_t* Y=0, Float_t* DX=0, Float_t* DY=0);
  void minfit(Int_t fix, HGeomVector* fE, HGeomVector* fT);
  void minfitRot(Int_t fix, Double_t first, Double_t second=0);
  HGeomVector translationFinderZones(HGeomRotation rot,HGeomVector tran);  
  HGeomVector translationFinder(Int_t XYorBoth=-1,
				Int_t XLowLim=0, Int_t XUppLim=0,
				Int_t YLowLim=0, Int_t YUppLim=0);
  HGeomVector translationFinderRW(Int_t XYorBoth=-1,
				  Int_t XLowLim=0, Int_t XUppLim=0,
				  Int_t YLowLim=0, Int_t YUppLim=0);
  Float_t* rotZFinder(Int_t noZones=0);
  Float_t* rotYXFinder(Int_t noZones=0, Int_t YorX=0);

  void findRotXYBySampling(Int_t angleIter=1,Int_t iter=0);
  void findRotXGeometrically(Int_t iter=0);
  void findRotYGeometrically(Int_t iter=0);
  void findRotZGeometrically(Int_t iter=0);

  void setOutputAscii(TString name);
  void setOutputRoot(TString name);

  ClassDef(HMdcAligner,1)  // relative aligner of MDCs
};

# endif /* !HMDCALIGNER_H */





