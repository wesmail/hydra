using namespace std;
# include <math.h>
# include <stdlib.h>
# include <iostream>
# include <iomanip> 
# include <fstream> 
# include <TMath.h>
# include <TROOT.h>
# include <TF1.h>
# include "hmdcaligner3M.h"
# include "hades.h"
# include "hdetector.h"
# include "hmdcdetector.h"
# include "hmdcmodulegeometry.h"
# include "hmdcgeomstruct.h"
# include "hevent.h"
# include "hmatrixcategory.h"
# include "hmatrixcatiter.h"
# include "hmdchit.h"
# include "hmdchitaux.h"
# include "hruntimedb.h"
# include "hspectrometer.h"
# include "mdcsdef.h"
 

ClassImp(HMdcAligner3M)
  
  //*-- AUTHOR : Hector Alvarez-Pol  
//*-- Date: 05/2004
//*-- Last Update: 31/05/04
//*-- Copyright: GENP (Univ. Santiago de Compostela)
//_HADES_CLASS_DESCRIPTION 
  ////////////////////////////////////////////////////////////////////////
  //
  // HMdcAligner3M
  //
  // (Fast version)  MDC aligner using the 3 modules method.
  //
  //
  ////////////////////////////////////////////////////////////////////////
  
  Int_t HMdcAligner3M::fRecCount=0;
TFile* HMdcAligner3M::fOutRoot=0;
Int_t AA_DEBUG=0;


//Poner en un header para que sea utilizable en otros modulos, si se requiere.
Double_t breitW3M(Double_t *x, Double_t *par)
{
  //
  // Breit Wigner (following
  //http://ikpe1101.ikp.kfa-juelich.de/briefbook_data_analysis/node23.html#22
  //for instance)    
  //par[2] = gamma, width of the BW function
  //par[1] = mean, 
  //par[0] = scale factor

  Double_t fitval = par[0]*(1/(2*TMath::Pi())) * 
    ( par[2]/((par[2]*par[2]/4)+(x[0]-par[1])*(x[0]-par[1])) );
  return fitval;
}


HMdcAligner3M::HMdcAligner3M(void) : HReconstructor()
{  
  //
  // Default constructor. 
  //

  fLoc.setNIndex(5);
  fHits = new TClonesArray("HMdcHit",4);   
  fLoc.set(5,0,0,1,2,3); //dummy sector 0
  fNumMods = 4;
  fMode=0;
  initDefaults();
}


HMdcAligner3M::HMdcAligner3M(const Text_t* name,const Text_t* title, Int_t sector,
			     Int_t modA, Int_t modB, Int_t modC)
  : HReconstructor(name, title)
{  
  //
  // Constructor including module election (and name and title, which 
  // seems to be very important). Alignment procedure 
  // proyects hits of modA in modB coordinate system and compares
  //

  
  fHits=new TClonesArray("HMdcHit",3);     
  fLoc.setNIndex(4);
  fLoc.set(4,sector,modA,modB,modC); 
  fNumMods = 3;
  
  fMode=0;
  initDefaults();
}


HMdcAligner3M::HMdcAligner3M(const Text_t* name,const Text_t* title, Int_t sectorA,
			     Int_t modA, Int_t sectorB, Int_t modB, Int_t modC, 
			     Int_t mode)
  : HReconstructor(name, title)
{  
  //
  // Constructor including module election (and name and title, which 
  // seems to be very important). This constructor deals with modules in
  // TWO DIFFERENT sectors: selects module A in sector A and modules B and C 
  // in sector B
  // mode takes values 1 or 2 for cosmics or pp ...
  //
  fHits=new TClonesArray("HMdcHit",3);     
  fLoc.setNIndex(5);
  fLoc.set(5,sectorA,modA,sectorB,modB,modC); 
  fNumMods = 3;
  fMode=mode;

  initDefaults();
}



void HMdcAligner3M::initDefaults(void)
{
  //
  // Inits common defaults
  //

  fRotMat = new HGeomRotation[fNumMods-1];
  fTranslation = new HGeomVector[fNumMods-1];
  fEuler = new HGeomVector[fNumMods-1];  
  fError = new Double_t[(fNumMods-1)*6];

  fDiscart = new Int_t[fNumMods-1];
  fHitsMdc = new Int_t[fNumMods];
  fHitsFoundInWindow = new Int_t[fNumMods-1];
  fHitsFoundAndUnique = new Int_t[fNumMods-1];

  fNEntries = 0; 
  fCount = 0;
  fCountCut = 0;

  fAuto = kFALSE;  
  fHistoOff = kFALSE;  
  fUseUnitErrors = kFALSE;  
  fUseModErrors = kFALSE;  
  fDoNotUseCov = kFALSE;   
  fUseSharpCut = kFALSE;   
  fUseCut = kTRUE;    
  fUseCutRot = kFALSE;  
  fUseCutRotZ = kFALSE; 
  fUseSlopeCut = kFALSE;
  
  for(Int_t num=0;num<fNumMods-1;num++){
    fDiscart[num]=0;
    fHitsMdc[num]=0;
    fHitsFoundInWindow[num]=0;
    fHitsFoundAndUnique[num]=0;
    fHitsMdc[num+1]=0;
  }
  //init values for resolution  
  if(fLoc[2] == 1){
    if(fLoc[1] == 0){
      fSigmaX = 0.400;fSigmaY = 0.300;fSigmaS0 = 0.008;fSigmaS1 = 0.005; 
      fRhoxSx = -0.17;fRhoySy = -0.22;fRhoxSy = 0.00;
    }
    else if(fLoc[1] == 2){
      fSigmaX = 0.400;fSigmaY = 0.300;fSigmaS0 = 0.008;fSigmaS1 = 0.005; 
      fRhoxSx = 0.23;fRhoySy = 0.23;fRhoxSy = 0.01;     
    }
    else if(fLoc[1] == 3){
      fSigmaX = 0.400;fSigmaY = 0.300;fSigmaS0 = 0.008;fSigmaS1 = 0.005; 
      fRhoxSx = 0.29;fRhoySy = 0.27;fRhoxSy = 0.00;
    }
  }

  fMin = 0;
  fFix = 0;
  fXArea = 100;  
  fYArea = 100;
  fSArea = 1;  
  fXSigmas = 1.64;    //90% of gauss distribution
  fYSigmas = 1.64;  
  fS0Sigmas = 1.64;  
  fS1Sigmas = 1.64;
  fSlopeCut = 0.1;
  fHistNum = 4;       //allows the fast definition of new sets of histo

  fHitCat = NULL; 
  fOutRoot = NULL; 
  initMinimization();
}


HMdcAligner3M::~HMdcAligner3M(void)
{    
  //
  // Destructor.
  //

  delete[] fRotMat;
  delete[] fTranslation;
  delete[] fEuler;   
  delete[] fError;
  delete[] fDiscart;
  delete[] fHitsMdc;
  delete[] fHitsFoundInWindow;
  delete[] fHitsFoundAndUnique;
  delete fHits;
}





void HMdcAligner3M::initMinimization(void){
  //
  // Minimization defaults 
  //

  fIterCriteria = 0.01;   
  fWeights = new Float_t[4];
  fSteps = new Float_t[12];
  fLimits = new Float_t[12];
  fPosError = new Float_t[6]; 
  setWeights(400,160,0.004,0.003);
  setSteps(0.01,0.01,0.01,0.1,0.1,0.1,
	   0.01,0.01,0.01,0.1,0.1,0.1);
  setLimits(0.,0.,0.,0.,0.,0.,
	    0.,0.,0.,0.,0.,0.);
}





Int_t HMdcAligner3M::getRelParams(HGeomVector*& ang,HGeomVector*& tra) 
{    
  //
  // Serves pointers to the relative transformation parameters
  //
  
  ang = fEuler; 
  tra = fTranslation;  
  
  return 0;
}





void HMdcAligner3M::setOutputAscii(TString name) 
{
  //
  // Sets ascii output for debugging.
  //

  fNameAscii=name;
}





void HMdcAligner3M::setOutputRoot(TString name)
{
  //
  // Sets root output
  //

  fNameRoot=name;
}





void HMdcAligner3M::setSigmas(Float_t XSigmas, Float_t YSigmas, 
			      Float_t S0Sigmas, Float_t S1Sigmas)
{
  //
  // Sets number of sigmas in cuts.
  //

  fXSigmas = XSigmas;    //1.64 -> 90% of gauss distribution
  fYSigmas = YSigmas;    //1.96 -> 95% of gauss distribution
  fS0Sigmas = S0Sigmas;  //2.58 -> 99% of gauss distribution
  fS1Sigmas = S1Sigmas;  //3.29 -> 99.9% of gauss distribution
}





void HMdcAligner3M::setHistograms(void)
{
  //
  // Inits histograms
  //
  
  Int_t sector = fLoc[0];
  Int_t modA = fLoc[1];
  Int_t modB = fLoc[2];    
  Int_t modC = fLoc[3];     
  if (fMode>0){
    sector = fLoc[0]*10;
    modA = fLoc[1];
    sector += fLoc[2]; //sector = sectorA*10+sectorB
    modB = fLoc[3];      
    modC = fLoc[4];       
  }  

  fRecCount++;  
  Char_t title[50], tmp[50];
  if(!fOutRoot) fOutRoot = new TFile(fNameRoot,"UPDATE");
  
  //Histograms for studying residuals in projections 
  
  BvsCinCCS_X = new TH1F*[fHistNum];  
  BvsCinCCS_Y = new TH1F*[fHistNum];
  BvsCinCCS_XSlope = new TH1F*[fHistNum];  
  BvsCinCCS_YSlope = new TH1F*[fHistNum];
  AvsCinCCS_X = new TH1F*[fHistNum];  
  AvsCinCCS_Y = new TH1F*[fHistNum];
  AvsCinCCS_XSlope = new TH1F*[fHistNum];  
  AvsCinCCS_YSlope = new TH1F*[fHistNum]; 
  CvsBinBCS_X = new TH1F*[fHistNum];  
  CvsBinBCS_Y = new TH1F*[fHistNum];
  CvsBinBCS_XSlope = new TH1F*[fHistNum];  
  CvsBinBCS_YSlope = new TH1F*[fHistNum];  
  CvsAinACS_X = new TH1F*[fHistNum];  
  CvsAinACS_Y = new TH1F*[fHistNum];
  CvsAinACS_XSlope = new TH1F*[fHistNum];  
  CvsAinACS_YSlope = new TH1F*[fHistNum];   

  XChi2Hist = new TH1F*[fHistNum]; 
  YChi2Hist = new TH1F*[fHistNum]; 
  TotalChi2 = new TH1F*[fHistNum]; 
  SqrDistToA = new TH1F*[fHistNum]; 
  SqrDistToB = new TH1F*[fHistNum];  
  SqrDistToC = new TH1F*[fHistNum];    
  SqrDist = new TH1F*[fHistNum];
  
  DiffBCvsAinACS_XSlope = new TH1F*[fHistNum]; 
  DiffBCvsAinACS_YSlope = new TH1F*[fHistNum]; 
  DiffBCvsBinACS_XSlope = new TH1F*[fHistNum];
  DiffBCvsBinACS_YSlope = new TH1F*[fHistNum]; 
  DiffBCvsCinACS_XSlope = new TH1F*[fHistNum]; 
  DiffBCvsCinACS_YSlope = new TH1F*[fHistNum]; 
  DiffACvsAinACS_XSlope = new TH1F*[fHistNum]; 
  DiffACvsAinACS_YSlope = new TH1F*[fHistNum]; 
  DiffACvsBinACS_XSlope = new TH1F*[fHistNum];
  DiffACvsBinACS_YSlope = new TH1F*[fHistNum]; 
  DiffACvsCinACS_XSlope = new TH1F*[fHistNum]; 
  DiffACvsCinACS_YSlope = new TH1F*[fHistNum]; 
  DiffBCvsAinACS_XSlopeLow = new TH1F*[fHistNum]; 
  DiffBCvsAinACS_YSlopeLow = new TH1F*[fHistNum]; 
  DiffBCvsBinACS_XSlopeLow = new TH1F*[fHistNum];
  DiffBCvsBinACS_YSlopeLow = new TH1F*[fHistNum]; 
  DiffBCvsCinACS_XSlopeLow = new TH1F*[fHistNum]; 
  DiffBCvsCinACS_YSlopeLow = new TH1F*[fHistNum]; 
  DiffACvsAinACS_XSlopeLow = new TH1F*[fHistNum]; 
  DiffACvsAinACS_YSlopeLow = new TH1F*[fHistNum]; 
  DiffACvsBinACS_XSlopeLow = new TH1F*[fHistNum];
  DiffACvsBinACS_YSlopeLow = new TH1F*[fHistNum]; 
  DiffACvsCinACS_XSlopeLow = new TH1F*[fHistNum]; 
  DiffACvsCinACS_YSlopeLow = new TH1F*[fHistNum];     
  DiffBCvsAinACS_XSlopeUp = new TH1F*[fHistNum]; 
  DiffBCvsAinACS_YSlopeUp = new TH1F*[fHistNum]; 
  DiffBCvsBinACS_XSlopeUp = new TH1F*[fHistNum];
  DiffBCvsBinACS_YSlopeUp = new TH1F*[fHistNum]; 
  DiffBCvsCinACS_XSlopeUp = new TH1F*[fHistNum]; 
  DiffBCvsCinACS_YSlopeUp = new TH1F*[fHistNum]; 
  DiffACvsAinACS_XSlopeUp = new TH1F*[fHistNum]; 
  DiffACvsAinACS_YSlopeUp = new TH1F*[fHistNum]; 
  DiffACvsBinACS_XSlopeUp = new TH1F*[fHistNum];
  DiffACvsBinACS_YSlopeUp = new TH1F*[fHistNum]; 
  DiffACvsCinACS_XSlopeUp = new TH1F*[fHistNum]; 
  DiffACvsCinACS_YSlopeUp = new TH1F*[fHistNum];     
   

  AvsBinBCS_X = new TH1F*[fHistNum];  
  AvsBinBCS_Y = new TH1F*[fHistNum];
  AvsBinBCS_XSlope = new TH1F*[fHistNum];  
  AvsBinBCS_YSlope = new TH1F*[fHistNum];  
  BvsAinACS_X = new TH1F*[fHistNum];  
  BvsAinACS_Y = new TH1F*[fHistNum];
  BvsAinACS_XSlope = new TH1F*[fHistNum];  
  BvsAinACS_YSlope = new TH1F*[fHistNum];
  BCvsAinACS_X = new TH1F*[fHistNum];    
  BCvsAinACS_Y = new TH1F*[fHistNum];
  BCvsACinACS_XSlope = new TH1F*[fHistNum];  
  BCvsACinACS_YSlope = new TH1F*[fHistNum];
  ABvsCinCCS_X = new TH1F*[fHistNum];    
  ABvsCinCCS_Y = new TH1F*[fHistNum];
  ABvsCinCCS_XSlope = new TH1F*[fHistNum];  
  ABvsCinCCS_YSlope = new TH1F*[fHistNum];
  ACvsBinBCS_X = new TH1F*[fHistNum];    
  ACvsBinBCS_Y = new TH1F*[fHistNum];
  ACvsBinBCS_XSlope = new TH1F*[fHistNum];  
  ACvsBinBCS_YSlope = new TH1F*[fHistNum];
  
  sprintf(title,"%s%i%s%i%s%i%s%i","Sector_",sector,"_ModA_",modA,
	  "_ModB_",modB,"_ModC_",modC);
  sprintf(tmp,"%s%s%i%s%i%s%i%s%i","All","_Sector_",sector,
	  "_ModA_",modA,"_ModB_",modB,"_ModC_",modC);   
  fAlignAll = new TTree(tmp,title);
  fAlignAll->Branch("hits",&fHits,64000);
  
  sprintf(tmp,"%s%s%i%s%i%s%i%s%i","AllCut","_Sector_",sector,
	  "_ModA_",modA,"_ModB_",modB,"_ModC_",modC);   
  fAlignAllCut = new TTree(tmp,title);
  fAlignAllCut->Branch("hits",&fHits,64000);
  
  static Char_t newDirName[255];
  sprintf(newDirName,"%s%s%i%s%i%s%i%s%i","Aligner3M_","S_",sector,
	  "_ModA_",modA,"_ModB_",modB,"_ModC_",modC);
  fOutRoot->mkdir(newDirName,newDirName);
  fOutRoot->cd(newDirName);
    
  //binning
  Int_t bin=200, binS=200, binChi=200, binDist=200;
  Int_t min=-100,max=100,minS=-1,maxS=1;
  Int_t minChi=0, maxChi=10, minDist=0,maxDist=100;
  
  for(Int_t index=0;index<fHistNum;index++){
    sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i","BvsCinCCS_X_",index,"_Sector_",sector,
	    "_ModA_",modA,"_ModB_",modB,"_ModC_",modC);
    BvsCinCCS_X[index] = new TH1F(tmp,title,bin,min,max);  
    sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i","BvsCinCCS_Y_",index,"_Sector_",sector,
	    "_ModA_",modA,"_ModB_",modB,"_ModC_",modC);
    BvsCinCCS_Y[index] = new TH1F(tmp,title,bin,min,max);  
    sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i","BvsCinCCS_XSlope_",index,"_Sector_",
	    sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC);
    BvsCinCCS_XSlope[index] = new TH1F(tmp,title,binS,minS,maxS);
    sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i","BvsCinCCS_YSlope_",index,"_Sector_",
	    sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC);
    BvsCinCCS_YSlope[index] = new TH1F(tmp,title,binS,minS,maxS);
    if(index==2){
      sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i","BvsCinCCS_Polar_",index,"_Sector_",
	      sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC);
      BvsCinCCS_Polar = new TH1F(tmp,title,200,-0.1,0.1);
      sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i","BvsCinCCS_Polar_Stripe1",index,
	      "_Sector_",sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC);
      BvsCinCCS_Polar_Stripe1 = new TH1F(tmp,title,200,-0.1,0.1);
      sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i","BvsCinCCS_Polar_Stripe2",index,
	      "_Sector_",sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC);
      BvsCinCCS_Polar_Stripe2 = new TH1F(tmp,title,200,-0.1,0.1);
      sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i","BvsCinCCS_Polar_Stripe3",index,
	      "_Sector_",sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC);
      BvsCinCCS_Polar_Stripe3 = new TH1F(tmp,title,200,-0.1,0.1);
      sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i","BvsCinCCS_Polar_Stripe4",index,
	      "_Sector_",sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC);
      BvsCinCCS_Polar_Stripe4 = new TH1F(tmp,title,200,-0.1,0.1);
      sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i","BvsCinCCS_Polar_Stripe5",index,
	      "_Sector_",sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC);
      BvsCinCCS_Polar_Stripe5 = new TH1F(tmp,title,200,-0.1,0.1);
      
      sprintf(tmp,"%s%s%i%s%i%s%i%s%i","DiffACvsAinACS_YSlope_Stripe1",
	      "_Sector_",sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC);
      DiffACvsAinACS_YSlope_Stripe1 = new TH1F(tmp,title,200,-0.1,0.1);
      sprintf(tmp,"%s%s%i%s%i%s%i%s%i","DiffACvsAinACS_YSlope_Stripe2",
	      "_Sector_",sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC);
      DiffACvsAinACS_YSlope_Stripe2 = new TH1F(tmp,title,200,-0.1,0.1);
      sprintf(tmp,"%s%s%i%s%i%s%i%s%i","DiffACvsAinACS_YSlope_Stripe3",
	      "_Sector_",sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC);
      DiffACvsAinACS_YSlope_Stripe3 = new TH1F(tmp,title,200,-0.1,0.1);
      sprintf(tmp,"%s%s%i%s%i%s%i%s%i","DiffACvsAinACS_YSlope_Stripe4",
	      "_Sector_",sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC);
      DiffACvsAinACS_YSlope_Stripe4 = new TH1F(tmp,title,200,-0.1,0.1);
      sprintf(tmp,"%s%s%i%s%i%s%i%s%i","DiffACvsAinACS_YSlope_Stripe5",
	      "_Sector_",sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC);
      DiffACvsAinACS_YSlope_Stripe5 = new TH1F(tmp,title,200,-0.1,0.1);
      
      sprintf(tmp,"%s%s%i%s%i%s%i%s%i","DiffACvsBinACS_YSlope_Stripe1",
	      "_Sector_",sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC);
      DiffACvsBinACS_YSlope_Stripe1 = new TH1F(tmp,title,200,-0.1,0.1);
      sprintf(tmp,"%s%s%i%s%i%s%i%s%i","DiffACvsBinACS_YSlope_Stripe2",
	      "_Sector_",sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC);
      DiffACvsBinACS_YSlope_Stripe2 = new TH1F(tmp,title,200,-0.1,0.1);
      sprintf(tmp,"%s%s%i%s%i%s%i%s%i","DiffACvsBinACS_YSlope_Stripe3",
	      "_Sector_",sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC);
      DiffACvsBinACS_YSlope_Stripe3 = new TH1F(tmp,title,200,-0.1,0.1);
      sprintf(tmp,"%s%s%i%s%i%s%i%s%i","DiffACvsBinACS_YSlope_Stripe4",
	      "_Sector_",sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC);
      DiffACvsBinACS_YSlope_Stripe4 = new TH1F(tmp,title,200,-0.1,0.1);
      sprintf(tmp,"%s%s%i%s%i%s%i%s%i","DiffACvsBinACS_YSlope_Stripe5",
	      "_Sector_",sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC);
      DiffACvsBinACS_YSlope_Stripe5 = new TH1F(tmp,title,200,-0.1,0.1);
      
      sprintf(tmp,"%s%s%i%s%i%s%i%s%i","DiffACvsCinACS_YSlope_Stripe1",
	      "_Sector_",sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC);
      DiffACvsCinACS_YSlope_Stripe1 = new TH1F(tmp,title,200,-0.1,0.1);
      sprintf(tmp,"%s%s%i%s%i%s%i%s%i","DiffACvsCinACS_YSlope_Stripe2",
	      "_Sector_",sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC);
      DiffACvsCinACS_YSlope_Stripe2 = new TH1F(tmp,title,200,-0.1,0.1);
      sprintf(tmp,"%s%s%i%s%i%s%i%s%i","DiffACvsCinACS_YSlope_Stripe3",
	      "_Sector_",sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC);
      DiffACvsCinACS_YSlope_Stripe3 = new TH1F(tmp,title,200,-0.1,0.1);
      sprintf(tmp,"%s%s%i%s%i%s%i%s%i","DiffACvsCinACS_YSlope_Stripe4",
	      "_Sector_",sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC);
      DiffACvsCinACS_YSlope_Stripe4 = new TH1F(tmp,title,200,-0.1,0.1);
      sprintf(tmp,"%s%s%i%s%i%s%i%s%i","DiffACvsCinACS_YSlope_Stripe5",
	      "_Sector_",sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC);
      DiffACvsCinACS_YSlope_Stripe5 = new TH1F(tmp,title,200,-0.1,0.1);  
    }
    sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i","AvsCinCCS_X_",index,"_Sector_",sector,
	    "_ModA_",modA,"_ModB_",modB,"_ModC_",modC);
    AvsCinCCS_X[index] = new TH1F(tmp,title,bin,min,max);  
    sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i","AvsCinCCS_Y_",index,"_Sector_",sector,
	    "_ModA_",modA,"_ModB_",modB,"_ModC_",modC);
    AvsCinCCS_Y[index] = new TH1F(tmp,title,bin,min,max);  
    sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i","AvsCinCCS_XSlope_",index,"_Sector_",
	    sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC);
    AvsCinCCS_XSlope[index] = new TH1F(tmp,title,binS,minS,maxS);
    sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i","AvsCinCCS_YSlope_",index,"_Sector_",
	    sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC);
    AvsCinCCS_YSlope[index] = new TH1F(tmp,title,binS,minS,maxS);
    if(index==2){
      sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i","AvsCinCCS_Polar_",index,"_Sector_",
	      sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC);
      AvsCinCCS_Polar = new TH1F(tmp,title,200,-0.1,0.1);
      sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i","AvsCinCCS_Polar_Stripe1",index,
	      "_Sector_",sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC);
      AvsCinCCS_Polar_Stripe1 = new TH1F(tmp,title,200,-0.1,0.1);
      sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i","AvsCinCCS_Polar_Stripe2",index,
	      "_Sector_",sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC);
      AvsCinCCS_Polar_Stripe2 = new TH1F(tmp,title,200,-0.1,0.1);
      sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i","AvsCinCCS_Polar_Stripe3",index,
	      "_Sector_",sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC);
      AvsCinCCS_Polar_Stripe3 = new TH1F(tmp,title,200,-0.1,0.1);
      sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i","AvsCinCCS_Polar_Stripe4",index,
	      "_Sector_",sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC);
      AvsCinCCS_Polar_Stripe4 = new TH1F(tmp,title,200,-0.1,0.1);
      sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i","AvsCinCCS_Polar_Stripe5",index,
	      "_Sector_",sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC);
      AvsCinCCS_Polar_Stripe5 = new TH1F(tmp,title,200,-0.1,0.1);
    }
    sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i","CvsBinBCS_X_",index,"_Sector_",sector,
	    "_ModA_",modA,"_ModB_",modB,"_ModC_",modC);
    CvsBinBCS_X[index] = new TH1F(tmp,title,bin,min,max);  
    sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i","CvsBinBCS_Y_",index,"_Sector_",sector,
	    "_ModA_",modA,"_ModB_",modB,"_ModC_",modC);
    CvsBinBCS_Y[index] = new TH1F(tmp,title,bin,min,max);  
    sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i","CvsBinBCS_XSlope_",index,"_Sector_",
	    sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC);
    CvsBinBCS_XSlope[index] = new TH1F(tmp,title,binS,minS,maxS);
    sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i","CvsBinBCS_YSlope_",index,"_Sector_",
	    sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC);
    CvsBinBCS_YSlope[index] = new TH1F(tmp,title,binS,minS,maxS);
    
    sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i","AvsBinBCS_X_",index,"_Sector_",sector,
	    "_ModA_",modA,"_ModB_",modB,"_ModC_",modC);
    AvsBinBCS_X[index] = new TH1F(tmp,title,bin,min,max);  
    sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i","AvsBinBCS_Y_",index,"_Sector_",sector,
	    "_ModA_",modA,"_ModB_",modB,"_ModC_",modC);
    AvsBinBCS_Y[index] = new TH1F(tmp,title,bin,min,max);  
    sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i","AvsBinBCS_XSlope_",index,"_Sector_",
	    sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC);
    AvsBinBCS_XSlope[index] = new TH1F(tmp,title,binS,minS,maxS);
    sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i","AvsBinBCS_YSlope_",index,"_Sector_",
	    sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC);
    AvsBinBCS_YSlope[index] = new TH1F(tmp,title,binS,minS,maxS);
    
    sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i","CvsAinACS_X_",index,"_Sector_",sector,
	    "_ModA_",modA,"_ModB_",modB,"_ModC_",modC);
    CvsAinACS_X[index] = new TH1F(tmp,title,bin,min,max);  
    sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i","CvsAinACS_Y_",index,"_Sector_",sector,
	    "_ModA_",modA,"_ModB_",modB,"_ModC_",modC);
    CvsAinACS_Y[index] = new TH1F(tmp,title,bin,min,max);  
    sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i","CvsAinACS_XSlope_",index,"_Sector_",
	    sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC);
    CvsAinACS_XSlope[index] = new TH1F(tmp,title,binS,minS,maxS);
    sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i","CvsAinACS_YSlope_",index,"_Sector_",
	    sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC);
    CvsAinACS_YSlope[index] = new TH1F(tmp,title,binS,minS,maxS);
    
    sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i","BvsAinACS_X_",index,"_Sector_",sector,
	    "_ModA_",modA,"_ModB_",modB,"_ModC_",modC);
    BvsAinACS_X[index] = new TH1F(tmp,title,bin,min,max);  
    sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i","BvsAinACS_Y_",index,"_Sector_",sector,
	    "_ModA_",modA,"_ModB_",modB,"_ModC_",modC);
    BvsAinACS_Y[index] = new TH1F(tmp,title,bin,min,max);  
    sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i","BvsAinACS_XSlope_",index,"_Sector_",
	    sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC);
    BvsAinACS_XSlope[index] = new TH1F(tmp,title,binS,minS,maxS);
    sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i","BvsAinACS_YSlope_",index,"_Sector_",
	    sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC);
    BvsAinACS_YSlope[index] = new TH1F(tmp,title,binS,minS,maxS);
    
    sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i","BCvsAinACS_X_",index,"_Sector_",
	    sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC);
    BCvsAinACS_X[index] = new TH1F(tmp,title,10*bin,min,max);  
    sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i","BCvsAinACS_Y_",index,"_Sector_",
	    sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC);
    BCvsAinACS_Y[index] = new TH1F(tmp,title,10*bin,min,max);  
    sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i","BCvsACinACS_XSlope_",index,"_Sector_",
	    sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC);
    BCvsACinACS_XSlope[index] = new TH1F(tmp,title,10*binS,minS,maxS);
    sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i","BCvsACinACS_YSlope_",index,"_Sector_",
	    sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC);
    BCvsACinACS_YSlope[index] = new TH1F(tmp,title,10*binS,minS,maxS);
      
    sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i","ABvsCinCCS_X_",index,"_Sector_",
	    sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC);
    ABvsCinCCS_X[index] = new TH1F(tmp,title,10*bin,min,max);  
    sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i","ABvsCinCCS_Y_",index,"_Sector_",
	    sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC);
    ABvsCinCCS_Y[index] = new TH1F(tmp,title,10*bin,min,max);  
    sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i","ABvsCinCCS_XSlope_",index,"_Sector_",
	    sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC);
    ABvsCinCCS_XSlope[index] = new TH1F(tmp,title,10*binS,minS,maxS);
    sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i","ABvsCinCCS_YSlope_",index,"_Sector_",
	    sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC);
    ABvsCinCCS_YSlope[index] = new TH1F(tmp,title,10*binS,minS,maxS);
    
    sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i","ACvsBinBCS_X_",index,"_Sector_",
	    sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC);
    ACvsBinBCS_X[index] = new TH1F(tmp,title,10*bin,min,max);  
    sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i","ACvsBinBCS_Y_",index,"_Sector_",
	    sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC);
    ACvsBinBCS_Y[index] = new TH1F(tmp,title,10*bin,min,max);  
    sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i","ACvsBinBCS_XSlope_",index,"_Sector_",
	    sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC);
    ACvsBinBCS_XSlope[index] = new TH1F(tmp,title,10*binS,minS,maxS);
    sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i","ACvsBinBCS_YSlope_",index,"_Sector_",
	    sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC);
    ACvsBinBCS_YSlope[index] = new TH1F(tmp,title,10*binS,minS,maxS);
    
    
    sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i","DiffBCvsAinACS_XSlope_",index,
	    "_Sector_",sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC);
    DiffBCvsAinACS_XSlope[index] = new TH1F(tmp,title,10*binS,minS,maxS);
    sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i","DiffBCvsAinACS_YSlope_",index,
	    "_Sector_",sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC);
    DiffBCvsAinACS_YSlope[index] = new TH1F(tmp,title,10*binS,minS,maxS);
    sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i","DiffBCvsBinACS_XSlope_",index,
	    "_Sector_",sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC);
    DiffBCvsBinACS_XSlope[index] = new TH1F(tmp,title,10*binS,minS,maxS);
    sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i","DiffBCvsBinACS_YSlope_",index,
	    "_Sector_",sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC);
    DiffBCvsBinACS_YSlope[index] = new TH1F(tmp,title,10*binS,minS,maxS);
    sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i","DiffBCvsCinACS_XSlope_",index,
	    "_Sector_",sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC);
    DiffBCvsCinACS_XSlope[index] = new TH1F(tmp,title,10*binS,minS,maxS);
    sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i","DiffBCvsCinACS_YSlope_",index,
	    "_Sector_",sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC);
    DiffBCvsCinACS_YSlope[index] = new TH1F(tmp,title,10*binS,minS,maxS);
    sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i","DiffACvsAinACS_XSlope_",index,
	    "_Sector_",sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC);
    DiffACvsAinACS_XSlope[index] = new TH1F(tmp,title,10*binS,minS,maxS);
    sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i","DiffACvsAinACS_YSlope_",index,
	    "_Sector_",sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC);
    DiffACvsAinACS_YSlope[index] = new TH1F(tmp,title,10*binS,minS,maxS);
    sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i","DiffACvsBinACS_XSlope_",index,
	    "_Sector_",sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC);
    DiffACvsBinACS_XSlope[index] = new TH1F(tmp,title,10*binS,minS,maxS);
    sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i","DiffACvsBinACS_YSlope_",index,
	    "_Sector_",sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC);
    DiffACvsBinACS_YSlope[index] = new TH1F(tmp,title,10*binS,minS,maxS);
    sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i","DiffACvsCinACS_XSlope_",index,
	    "_Sector_",sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC);
    DiffACvsCinACS_XSlope[index] = new TH1F(tmp,title,10*binS,minS,maxS);
    sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i","DiffACvsCinACS_YSlope_",index,
	    "_Sector_",sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC);
    DiffACvsCinACS_YSlope[index] = new TH1F(tmp,title,10*binS,minS,maxS);
    
    sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i","DiffBCvsAinACS_XSlopeLow_",index,
	    "_Sector_",sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC);
    DiffBCvsAinACS_XSlopeLow[index] = new TH1F(tmp,title,10*binS,minS,maxS);
    sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i","DiffBCvsAinACS_YSlopeLow_",index,
	    "_Sector_",sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC);
    DiffBCvsAinACS_YSlopeLow[index] = new TH1F(tmp,title,10*binS,minS,maxS);
    sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i","DiffBCvsBinACS_XSlopeLow_",index,
	    "_Sector_",sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC);
    DiffBCvsBinACS_XSlopeLow[index] = new TH1F(tmp,title,10*binS,minS,maxS);
    sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i","DiffBCvsBinACS_YSlopeLow_",index,
	    "_Sector_",sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC);
    DiffBCvsBinACS_YSlopeLow[index] = new TH1F(tmp,title,10*binS,minS,maxS);
    sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i","DiffBCvsCinACS_XSlopeLow_",index,
	    "_Sector_",sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC);
    DiffBCvsCinACS_XSlopeLow[index] = new TH1F(tmp,title,10*binS,minS,maxS);
    sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i","DiffBCvsCinACS_YSlopeLow_",index,
	    "_Sector_",sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC);
    DiffBCvsCinACS_YSlopeLow[index] = new TH1F(tmp,title,10*binS,minS,maxS);
    sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i","DiffACvsAinACS_XSlopeLow_",index,
	    "_Sector_",sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC);
    DiffACvsAinACS_XSlopeLow[index] = new TH1F(tmp,title,10*binS,minS,maxS);
    sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i","DiffACvsAinACS_YSlopeLow_",index,
	    "_Sector_",sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC);
    DiffACvsAinACS_YSlopeLow[index] = new TH1F(tmp,title,10*binS,minS,maxS);
    sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i","DiffACvsBinACS_XSlopeLow_",index,
	    "_Sector_",sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC);
    DiffACvsBinACS_XSlopeLow[index] = new TH1F(tmp,title,10*binS,minS,maxS);
    sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i","DiffACvsBinACS_YSlopeLow_",index,
	    "_Sector_",sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC);
    DiffACvsBinACS_YSlopeLow[index] = new TH1F(tmp,title,10*binS,minS,maxS);
    sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i","DiffACvsCinACS_XSlopeLow_",index,
	    "_Sector_",sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC);
    DiffACvsCinACS_XSlopeLow[index] = new TH1F(tmp,title,10*binS,minS,maxS);
    sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i","DiffACvsCinACS_YSlopeLow_",index,
	    "_Sector_",sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC);
    DiffACvsCinACS_YSlopeLow[index] = new TH1F(tmp,title,10*binS,minS,maxS);
    
    sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i","DiffBCvsAinACS_XSlopeUp_",index,
	    "_Sector_",sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC);
    DiffBCvsAinACS_XSlopeUp[index] = new TH1F(tmp,title,10*binS,minS,maxS);
    sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i","DiffBCvsAinACS_YSlopeUp_",index,
	    "_Sector_",sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC);
    DiffBCvsAinACS_YSlopeUp[index] = new TH1F(tmp,title,10*binS,minS,maxS);
    sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i","DiffBCvsBinACS_XSlopeUp_",index,
	    "_Sector_",sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC);
    DiffBCvsBinACS_XSlopeUp[index] = new TH1F(tmp,title,10*binS,minS,maxS);
    sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i","DiffBCvsBinACS_YSlopeUp_",index,
	    "_Sector_",sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC);
    DiffBCvsBinACS_YSlopeUp[index] = new TH1F(tmp,title,10*binS,minS,maxS);
    sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i","DiffBCvsCinACS_XSlopeUp_",index,
	    "_Sector_",sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC);
    DiffBCvsCinACS_XSlopeUp[index] = new TH1F(tmp,title,10*binS,minS,maxS);
    sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i","DiffBCvsCinACS_YSlopeUp_",index,
	    "_Sector_",sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC);
    DiffBCvsCinACS_YSlopeUp[index] = new TH1F(tmp,title,10*binS,minS,maxS);
    sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i","DiffACvsAinACS_XSlopeUp_",index,
	    "_Sector_",sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC);
    DiffACvsAinACS_XSlopeUp[index] = new TH1F(tmp,title,10*binS,minS,maxS);
    sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i","DiffACvsAinACS_YSlopeUp_",index,
	    "_Sector_",sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC);
    DiffACvsAinACS_YSlopeUp[index] = new TH1F(tmp,title,10*binS,minS,maxS);
    sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i","DiffACvsBinACS_XSlopeUp_",index,
	    "_Sector_",sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC);
    DiffACvsBinACS_XSlopeUp[index] = new TH1F(tmp,title,10*binS,minS,maxS);
    sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i","DiffACvsBinACS_YSlopeUp_",index,
	    "_Sector_",sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC);
    DiffACvsBinACS_YSlopeUp[index] = new TH1F(tmp,title,10*binS,minS,maxS);
    sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i","DiffACvsCinACS_XSlopeUp_",index,
	    "_Sector_",sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC);
    DiffACvsCinACS_XSlopeUp[index] = new TH1F(tmp,title,10*binS,minS,maxS);
    sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i","DiffACvsCinACS_YSlopeUp_",index,
	    "_Sector_",sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC);
    DiffACvsCinACS_YSlopeUp[index] = new TH1F(tmp,title,10*binS,minS,maxS);      
    
    sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i","XChi2Hist_",index,"_Sector_",
	    sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC);
    XChi2Hist[index] = new TH1F(tmp,title,binChi,minChi,maxChi);       
    sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i","YChi2Hist_",index,"_Sector_",
	    sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC);
    YChi2Hist[index] = new TH1F(tmp,title,binChi,minChi,maxChi);       
    sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i","TotalChi2_",index,"_Sector_",
	    sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC);
    TotalChi2[index] = new TH1F(tmp,title,binChi,minChi,maxChi);      
    sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i","SqrDistToA_",index,"_Sector_",
	    sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC); 
    SqrDistToA[index] = new TH1F(tmp,title,binDist,minDist,maxDist);      
    sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i","SqrDistToB_",index,"_Sector_",
	    sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC);  
    SqrDistToB[index] = new TH1F(tmp,title,binDist,minDist,maxDist);       
    sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i","SqrDistToC_",index,"_Sector_",
	    sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC);  
    SqrDistToC[index] = new TH1F(tmp,title,binDist,minDist,maxDist);       
    sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i","SqrDist_",index,"_Sector_",
	    sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC);    
    SqrDist[index] = new TH1F(tmp,title,binDist,minDist,maxDist);
  }
    
  //Histogramas comunes

  graphCont = new TGraph(40);
  graphCont->SetName("graphCont");
  graphCont->SetTitle("graphCont");

  sprintf(tmp,"%s%s%i%s%i%s%i","fResX","_Sector_",sector,
	  "_ModA_",modA,"_ModB_",modB);
  fResX = new TH1F(tmp,title,2000,-1000,1000);  
  sprintf(tmp,"%s%s%i%s%i%s%i","fResY","_Sector_",sector,
	  "_ModA_",modA,"_ModB_",modB);
  fResY = new TH1F(tmp,title,2000,-1000,1000);   
  sprintf(tmp,"%s%s%i%s%i%s%i","fResS0","_Sector_",sector,
	  "_ModA_",modA,"_ModB_",modB);
  fResS0 = new TH1F(tmp,title,2000,-1,1); 
  sprintf(tmp,"%s%s%i%s%i%s%i","fResS1","_Sector_",sector,
	  "_ModA_",modA,"_ModB_",modB);
  fResS1 = new TH1F(tmp,title,2000,-1,1);     
  sprintf(tmp,"%s%s%i%s%i%s%i","fAccResX","_Sector_",sector,
	  "_ModA_",modA,"_ModB_",modB);

  fOutRoot->cd();
}





void HMdcAligner3M::fitHistograms(Int_t index)
{
  //
  //Fits to a gaussian the four relevant histograms
  //and obtains the fit parameters for further data selection
  
  Float_t XNewAreaA, XNewAreaB, YNewAreaA, YNewAreaB;
  Float_t S0NewAreaA, S0NewAreaB, S1NewAreaA, S1NewAreaB;
  Float_t XNewMeanA, XNewMeanB, YNewMeanA, YNewMeanB;
  Float_t S0NewMeanA, S0NewMeanB, S1NewMeanA, S1NewMeanB;
  
  TF1 *f1X = new TF1("f1X","gaus",-fXArea,fXArea);
  TF1 *f1Y = new TF1("f1Y","gaus",-fYArea,fYArea);
  TF1 *f1S = new TF1("f1S","gaus",-fSArea,fSArea);
  TF1 *totalX  = new TF1("totalX","gaus(0)+pol2(3)",-fXArea,fXArea);
  TF1 *totalY  = new TF1("totalY","gaus(0)+pol2(3)",-fYArea,fYArea);
  TF1 *totalS  = new TF1("totalS","gaus(0)+pol2(3)",-fSArea,fSArea);
  TF1 *total2X  = new TF1("total2X","gaus(0)+gaus(3)",-fXArea,fXArea);
  TF1 *total2Y  = new TF1("total2Y","gaus(0)+gaus(3)",-fYArea,fYArea);
  TF1 *total2S  = new TF1("total2S","gaus(0)+gaus(3)",-fSArea,fSArea);
  TF1 *total3X  = new TF1("total3X",breitW3M,-fXArea,fXArea,3);
  TF1 *total3Y  = new TF1("total3Y",breitW3M,-fYArea,fYArea,3);
  TF1 *total3S  = new TF1("total3S",breitW3M,-fSArea,fSArea,3);
  Double_t par[6]; 
  
  if(AA_DEBUG>1) cout << endl 
		      <<"**** fitHistograms() results ****" << endl;   
  if(AA_DEBUG>1) cout << endl 
		      <<"**** Gauss fit: mean, sigma ****" << endl 
		      <<"**** Gauss+pol: mean, sigma ****" 
		      << endl;   
  
  ABvsCinCCS_X[index]->Fit("f1X","RQNW"); 
  Float_t fitPar0     = f1X->GetParameter(0);  // constant
  Float_t fitPar1     = f1X->GetParameter(1);  // mean
  Float_t fitPar2     = f1X->GetParameter(2);  // sigma
  if(AA_DEBUG>1) 
    cout << " AvsBinBCS_X[" << index << "]: "
	 << fitPar1 << ", " << fitPar2<< ", " ;
  f1X->GetParameters(&par[0]);
  par[3] =  par[4] = par[5] = 0.; 
  totalX->SetParameters(par);

  ABvsCinCCS_X[index]->Fit("totalX","RQN");
  fitPar0     = totalX->GetParameter(0); 
  fitPar1     = totalX->GetParameter(1);
  fitPar2     = totalX->GetParameter(2);
  if(AA_DEBUG>1) cout << fitPar1 << ", " << fitPar2 << endl;
  XNewAreaA = fXSigmas * fitPar2;
  XNewMeanA = fitPar1;
  
  ABvsCinCCS_Y[index]->Fit("f1Y","RQNW");
  fitPar0     = f1Y->GetParameter(0);  // constant
  fitPar1     = f1Y->GetParameter(1);  // mean
  fitPar2     = f1Y->GetParameter(2);  // sigma    
  if(AA_DEBUG>1) cout << " AvsBinBCS_Y[" << index << "]: "
		      << fitPar1 << ", " << fitPar2<< ", " ;
  f1Y->GetParameters(&par[0]);
  totalY->SetParameters(par);

  ABvsCinCCS_Y[index]->Fit("totalY","RQN");
  fitPar1     = totalY->GetParameter(1);
  fitPar2     = totalY->GetParameter(2);
  if(AA_DEBUG>1) cout << fitPar1 << ", " << fitPar2 << endl;
  YNewAreaA = fYSigmas * fitPar2;
  YNewMeanA = fitPar1;

  ABvsCinCCS_XSlope[index]->Fit("f1S","RQNW");
  fitPar0     = f1S->GetParameter(0);  // constant
  fitPar1     = f1S->GetParameter(1);  // mean
  fitPar2     = f1S->GetParameter(2);  // sigma    
  if(AA_DEBUG>1) cout << " AvsBinBCS_XSlope[" << index << "]: "
		      << fitPar1 << ", " << fitPar2<< ", " ;
  f1S->GetParameters(&par[0]);
  totalS->SetParameters(par);     
  
  ABvsCinCCS_XSlope[index]->Fit("totalS","RQN");
  fitPar1     = totalS->GetParameter(1);
  fitPar2     = totalS->GetParameter(2);
  if(AA_DEBUG>1) cout << fitPar1 << ", " << fitPar2 << endl;
  S0NewAreaA = fS0Sigmas * fitPar2;
  S0NewMeanA = fitPar1;
  
  ABvsCinCCS_YSlope[index]->Fit("f1S","RQNW");
  fitPar0     = f1S->GetParameter(0);  // constant
  fitPar1     = f1S->GetParameter(1);  // mean
  fitPar2     = f1S->GetParameter(2);  // sigma    
  if(AA_DEBUG>1) cout << " AvsBinBCS_YSlope[" << index << "]: "
		      << fitPar1 << ", " << fitPar2<< ", " ;
  f1S->GetParameters(&par[0]);
  totalS->SetParameters(par);     
  
  ABvsCinCCS_YSlope[index]->Fit("totalS","RQN");
  fitPar1     = totalS->GetParameter(1);
  fitPar2     = totalS->GetParameter(2);
  if(AA_DEBUG>1) cout << fitPar1 << ", " << fitPar2 << endl;
  S1NewAreaA = fS1Sigmas * fitPar2;
  S1NewMeanA = fitPar1;
  
  BCvsAinACS_X[index]->Fit("f1X","RQN");
  fitPar0     = f1X->GetParameter(0);  // constant
  fitPar1     = f1X->GetParameter(1);  // mean
  fitPar2     = f1X->GetParameter(2);  // sigma    
  if(AA_DEBUG>1) cout << " BvsAinACS_X[" << index << "]: "
		      << fitPar1 << ", " << fitPar2<< ", " ;
  f1X->GetParameters(&par[0]);
  totalX->SetParameters(par);     
    
  BCvsAinACS_X[index]->Fit("totalX","RQN");
  fitPar1     = totalX->GetParameter(1);
  fitPar2     = totalX->GetParameter(2);
  if(AA_DEBUG>1) cout << fitPar1 << ", " << fitPar2 << endl;
  XNewAreaB = fXSigmas * fitPar2;
  XNewMeanB = fitPar1;

  //Double gauss
  f1X->GetParameters(&par[0]);
  par[3]=par[0]/10;par[4]=par[1];par[5]=par[2]*4;
  total2X->SetParameters(par);  
   
  BCvsAinACS_X[index]->Fit("total2X","RQN");
  fitPar1     = total2X->GetParameter(1);
  fitPar2     = total2X->GetParameter(2);
  
  if(total2X->GetChisquare()!=0.)
    if( (total2X->GetChisquare()/total2X->GetNDF()) <
	(totalX->GetChisquare()/totalX->GetNDF()) ){//take the better fit
      if(total2X->GetParameter(5)<total2X->GetParameter(2)){
	XNewAreaB = fXSigmas * total2X->GetParameter(5);
	XNewMeanB = total2X->GetParameter(4);
      }
      else {
	XNewAreaB = fXSigmas * fitPar2;
	XNewMeanB = fitPar1;
      }
    }
  
  //BW
  total2X->GetParameters(&par[0]);
  par[0]=par[0]*10*par[2];par[2]=par[2]*2;
  total3X->SetParameters(par);
  BCvsAinACS_X[index]->Fit(total3X,"RQN");
  fitPar1     = total3X->GetParameter(1);
  fitPar2     = total3X->GetParameter(2);
  
  if(total3X->GetChisquare()!=0.)
    if( (total3X->GetChisquare()/total3X->GetNDF()) <
	(total2X->GetChisquare()/total2X->GetNDF()) && 
	(total3X->GetChisquare()/total3X->GetNDF()) <
	(totalX->GetChisquare()/totalX->GetNDF()) ){//take the better fit
      XNewAreaB = fXSigmas * (fitPar2/2.355);//converting to sigma equivalent
      XNewMeanB = fitPar1;    
    }
  
  ofstream *fout=NULL;
  if (fNameAscii) fout = new ofstream(fNameAscii.Data(), ios::app);
  if (*fout){
    *fout << endl; 
    *fout << "Fitting X hist:"  << " chi square " << 
      " chi square/NDF" << endl; 
    *fout << "gaus:         "  << f1X->GetChisquare() << "    " <<
      f1X->GetChisquare()/f1X->GetNDF() << endl; 
    *fout << "gaus + pol2:  "  << totalX->GetChisquare() <<  "    " <<
      totalX->GetChisquare()/totalX->GetNDF() << endl; 
    *fout << "double gauss: "  << total2X->GetChisquare() <<  "    " <<
      total2X->GetChisquare()/total2X->GetNDF() << endl; 
    *fout << "Breit Wigner: "  << total3X->GetChisquare() <<  "    " <<
      total3X->GetChisquare()/total3X->GetNDF() << endl; 	    
  }
  
  BCvsAinACS_Y[index]->Fit("f1Y","RQN");
  fitPar0     = f1Y->GetParameter(0);  // constant
  fitPar1     = f1Y->GetParameter(1);  // mean
  fitPar2     = f1Y->GetParameter(2);  // sigma    
  if(AA_DEBUG>1) 
    cout << " BvsAinACS_Y[" << index << "]: "
	 << fitPar1 << ", " << fitPar2<< ", " ;    
  f1Y->GetParameters(&par[0]);
  totalY->SetParameters(par);     
  
  BCvsAinACS_Y[index]->Fit("totalY","RQN");
  fitPar1     = totalY->GetParameter(1);
  fitPar2     = totalY->GetParameter(2);
  if(AA_DEBUG>1) 
    cout << fitPar1 << ", " << fitPar2 << endl;
  YNewAreaB = fYSigmas * fitPar2;
  YNewMeanB = fitPar1;
  
  //Double gauss
  f1Y->GetParameters(&par[0]);
  par[3]=par[0]/10;par[4]=par[1];par[5]=par[2]*4;
  total2Y->SetParameters(par);  
  BCvsAinACS_Y[index]->Fit("total2Y","RQN");
  fitPar1     = total2Y->GetParameter(1);
  fitPar2     = total2Y->GetParameter(2);
  
  if(total2Y->GetChisquare()!=0.)
    if( (total2Y->GetChisquare()/total2Y->GetNDF()) <
	(totalY->GetChisquare()/totalY->GetNDF()) ){//take the better fit
      if(total2Y->GetParameter(5)<total2Y->GetParameter(2)){
	YNewAreaB = fYSigmas * total2Y->GetParameter(5);
	YNewMeanB = total2Y->GetParameter(4);
      }
      else {
	YNewAreaB = fYSigmas * fitPar2;
	YNewMeanB = fitPar1;
      }
    }
  
  //BW
  total2Y->GetParameters(&par[0]);
  par[0]=par[0]*10*par[2];par[2]=par[2]*2;
  total3Y->SetParameters(par);  
  BCvsAinACS_Y[index]->Fit(total3Y,"RQN");
  fitPar1     = total3Y->GetParameter(1);
  fitPar2     = total3Y->GetParameter(2);
  
  if(total3Y->GetChisquare()!=0.)
    if( (total3Y->GetChisquare()/total3Y->GetNDF()) <
	(total2Y->GetChisquare()/total2Y->GetNDF()) && 
	(total3Y->GetChisquare()/total3Y->GetNDF()) <
	(totalY->GetChisquare()/totalY->GetNDF()) ){//take the better fit
      YNewAreaB = fYSigmas * (fitPar2/2.355);
      YNewMeanB = fitPar1;
    }
  
  if (*fout){
    *fout << endl; 
    *fout << "Fitting Y hist:"  << " chi square " << 
      " chi square/NDF" << endl; 
    *fout << "gaus:         "  << f1Y->GetChisquare() <<  "    " <<
      f1Y->GetChisquare()/f1Y->GetNDF() << endl; 
    *fout << "gaus + pol2:  "  << totalY->GetChisquare() <<  "    " <<
      totalY->GetChisquare()/totalY->GetNDF() << endl; 
    *fout << "double gauss: "  << total2Y->GetChisquare() <<  "    " <<
      total2Y->GetChisquare()/total2Y->GetNDF() << endl; 
    *fout << "Breit Wigner: "  << total3Y->GetChisquare() <<  "    " <<
      total3Y->GetChisquare()/total3Y->GetNDF() << endl; 	    
  }
  
  
  BCvsACinACS_XSlope[index]->Fit("f1S","RQN");
  fitPar0     = f1S->GetParameter(0);  // constant
  fitPar1     = f1S->GetParameter(1);  // mean
  fitPar2     = f1S->GetParameter(2);  // sigma    
  if(AA_DEBUG>1) cout << " BvsAinACS_XSlope[" << index << "]: "
		      << fitPar1 << ", " << fitPar2<< ", " ;
  f1S->GetParameters(&par[0]);
  totalS->SetParameters(par);     
  
  BCvsACinACS_XSlope[index]->Fit("totalS","RQN");
  fitPar1     = totalS->GetParameter(1);
  fitPar2     = totalS->GetParameter(2);
  if(AA_DEBUG>1) cout << fitPar1 << ", " << fitPar2 << endl;
  S0NewAreaB = fS0Sigmas * fitPar2;
  S0NewMeanB = fitPar1;
  
  //Double gauss
  f1S->GetParameters(&par[0]);
  par[3]=par[0]/10;par[4]=par[1];par[5]=par[2]*4;
  total2S->SetParameters(par);  
  BCvsACinACS_XSlope[index]->Fit("total2S","RQN");
  fitPar1     = total2S->GetParameter(1);
  fitPar2     = total2S->GetParameter(2);
  
  if(total2S->GetChisquare()!=0.)
    if( (total2S->GetChisquare()/total2S->GetNDF()) <
	(totalS->GetChisquare()/totalS->GetNDF()) ){//take the better fit
      if(total2S->GetParameter(5)<total2S->GetParameter(2)){
	S0NewAreaB = fS0Sigmas * total2S->GetParameter(5);
	S0NewMeanB = total2S->GetParameter(4);
      }
      else {
	S0NewAreaB = fS0Sigmas * fitPar2;
	S0NewMeanB = fitPar1;
      }
    }
  
  //BW
  total2S->GetParameters(&par[0]);
  par[0]=par[0]*10*par[2];par[2]=par[2]*2;
  total3S->SetParameters(par);  
  BCvsACinACS_XSlope[index]->Fit(total3S,"RQN");
  fitPar1     = total3S->GetParameter(1);
  fitPar2     = total3S->GetParameter(2);
  
  if(total3S->GetChisquare()!=0.)
    if( (total3S->GetChisquare()/total3S->GetNDF()) <
	(total2S->GetChisquare()/total2S->GetNDF()) && 
	(total3S->GetChisquare()/total3S->GetNDF()) <
	(totalS->GetChisquare()/totalS->GetNDF()) ){//take the better fit
      S0NewAreaB = fS0Sigmas * (fitPar2/2.355);
      S0NewMeanB = fitPar1;
    }
  
  if (*fout){
    *fout << endl; 
    *fout << "Fitting S0 hist:"  << " chi square " << 
      " chi square/NDF" << endl; 
    *fout << "gaus:         "  << f1S->GetChisquare() <<  "    " <<
      f1S->GetChisquare()/f1S->GetNDF() << endl; 
    *fout << "gaus + pol2:  "  << totalS->GetChisquare() <<  "    " <<
      totalS->GetChisquare()/totalS->GetNDF() << endl; 
    *fout << "double gauss: "  << total2S->GetChisquare() <<  "    " <<
      total2S->GetChisquare()/total2S->GetNDF() << endl; 
    *fout << "Breit Wigner: "  << total3S->GetChisquare() <<  "    " <<
      total3S->GetChisquare()/total3S->GetNDF() << endl; 	    
  }
  
  BCvsACinACS_YSlope[index]->Fit("f1S","RQN");
  fitPar0     = f1S->GetParameter(0);  // constant
  fitPar1     = f1S->GetParameter(1);  // mean
  fitPar2     = f1S->GetParameter(2);  // sigma    
  if(AA_DEBUG>1) cout << " BvsAinACS_YSlope[" << index << "]: "
		      << fitPar1 << ", " << fitPar2<< ", " ;    
  f1S->GetParameters(&par[0]);
  totalS->SetParameters(par);     
  
  BCvsACinACS_YSlope[index]->Fit("totalS","RQN");
  fitPar1     = totalS->GetParameter(1);
  fitPar2     = totalS->GetParameter(2);
  if(AA_DEBUG>1) cout << fitPar1 << ", " << fitPar2 << endl;
  S1NewAreaB = fS1Sigmas * fitPar2;
  S1NewMeanB = fitPar1;
  
  //Double gauss
  f1S->GetParameters(&par[0]);
  par[3]=par[0]/10;par[4]=par[1];par[5]=par[2]*4;
  total2S->SetParameters(par);  
  BCvsACinACS_YSlope[index]->Fit("total2S","RQN");
  fitPar1     = total2S->GetParameter(1);
  fitPar2     = total2S->GetParameter(2);
  
  if(total2S->GetChisquare()!=0.)
    if( (total2S->GetChisquare()/total2S->GetNDF()) <
	(totalS->GetChisquare()/totalS->GetNDF()) ){//take the better fit
      if(total2S->GetParameter(5)<total2S->GetParameter(2)){
	S1NewAreaB = fS1Sigmas * total2S->GetParameter(5);
	S1NewMeanB = total2S->GetParameter(4);
      }
      else {
	S1NewAreaB = fS1Sigmas * fitPar2;
	S1NewMeanB = fitPar1;
      }
    }
    
  //BW
  total2S->GetParameters(&par[0]);
  par[0]=par[0]*10*par[2];par[2]=par[2]*2;
  total3S->SetParameters(par);  
  BCvsACinACS_YSlope[index]->Fit(total3S,"RQN");
  fitPar1     = total3S->GetParameter(1);
  fitPar2     = total3S->GetParameter(2);
  
  if(total3S->GetChisquare()!=0.)
    if( (total3S->GetChisquare()/total3S->GetNDF()) <
	(total2S->GetChisquare()/total2S->GetNDF()) && 
	(total3S->GetChisquare()/total3S->GetNDF()) <
	(totalS->GetChisquare()/totalS->GetNDF()) ){//take the better fit
      S1NewAreaB = fS1Sigmas * (fitPar2/2.355);  
      S1NewMeanB = fitPar1;
    }
  
  if (*fout){
    *fout << endl; 
    *fout << "Fitting S1 hist:"  << " chi square " << 
      " chi square/NDF" << endl; 
    *fout << "gaus:         "  << f1S->GetChisquare() <<  "    " <<
      f1S->GetChisquare()/f1S->GetNDF() << endl; 
    *fout << "gaus + pol2:  "  << totalS->GetChisquare() <<  "    " <<
      totalS->GetChisquare()/totalS->GetNDF() << endl; 
    *fout << "double gauss: "  << total2S->GetChisquare() <<  "    " <<
      total2S->GetChisquare()/total2S->GetNDF() << endl; 
    *fout << "Breit Wigner: "  << total3S->GetChisquare() <<  "    " <<
      total3S->GetChisquare()/total3S->GetNDF() << endl; 	    
    
    *fout << " Fit sigmas: " << XNewAreaB/fXSigmas << "  " 
	  << YNewAreaB/fYSigmas << "  " << S0NewAreaB/fS0Sigmas 
	  << "  " << S1NewAreaB/fS1Sigmas << endl;
  }
  
  Stat_t entries = fAlignAll->GetEntries();
  
  HMdcHit* hitA; 
  HMdcHit* hitB; 
  HMdcHit* hitC=NULL;  
  HMdcHit* hitBC=new HMdcHit; 
  HMdcHit* hitAB=new HMdcHit; 
  
  HGeomVector projPoint; 
  Float_t* projSlopes = new Float_t[2];
  Float_t* origSlopes = new Float_t[2];
  HGeomRotation rotaux;
  HGeomVector transaux;
  HGeomRotation rotaux2;
  HGeomVector transaux2;
  
  TH2F* xSxcov;            
  TH2F* ySycov;            
  TH2F* xycov;             
  TH2F* xSycov;            
  TH2F* ySxcov;            
  TH2F* SxSycov;        
  
  //Covariances and correlations  
  xSxcov = new TH2F("xSxcov","xSxcov",100,XNewMeanB-(2*(XNewAreaB/fXSigmas)),
		    XNewMeanB+(2*(XNewAreaB/fXSigmas)),
		    100,S0NewMeanB-(2*(S0NewAreaB/fS0Sigmas)),
		    S0NewMeanB+(2*(S0NewAreaB/fS0Sigmas)));
  ySycov = new TH2F("ySycov","ySycov",100,YNewMeanB-(2*(YNewAreaB/fYSigmas)),
		    YNewMeanB+(2*(YNewAreaB/fYSigmas)),
		    100,S1NewMeanB-(2*(S1NewAreaB/fS1Sigmas)),
		    S1NewMeanB+(2*(S1NewAreaB/fS1Sigmas)));
  xycov = new TH2F("xycov","xycov",100,XNewMeanB-(2*(XNewAreaB/fXSigmas)),
		   XNewMeanB+(2*(XNewAreaB/fXSigmas)),
		   100,YNewMeanB-(2*(YNewAreaB/fYSigmas)),
		   YNewMeanB+(2*(YNewAreaB/fYSigmas)));
  xSycov = new TH2F("xSycov","xSycov",100,XNewMeanB-(2*(XNewAreaB/fXSigmas)),
		    XNewMeanB+(2*(XNewAreaB/fXSigmas)),
		    100,S1NewMeanB-(2*(S1NewAreaB/fS1Sigmas)),
		    S1NewMeanB+(2*(S1NewAreaB/fS1Sigmas)));
  ySxcov = new TH2F("ySxcov","ySxcov",100,YNewMeanB-(2*(YNewAreaB/fYSigmas)),
		    YNewMeanB+(2*(YNewAreaB/fYSigmas)),
		    100,S0NewMeanB-(2*(S0NewAreaB/fS0Sigmas)),
		    S0NewMeanB+(2*(S0NewAreaB/fS0Sigmas)));
  SxSycov = new TH2F("SxSycov","SxSycov",100,S0NewMeanB-
		     (2*(S0NewAreaB/fS0Sigmas)),
		     S0NewMeanB+(2*(S0NewAreaB/fS0Sigmas)),
		     100,S1NewMeanB-(2*(S1NewAreaB/fS1Sigmas)),
		     S1NewMeanB+(2*(S1NewAreaB/fS1Sigmas)));
  
  rotaux = fRotMat[1].inverse();
  transaux = -(rotaux * fTranslation[1]);
  rotaux2 = fRotMat[0].inverse() * fRotMat[1];
  transaux2 = fRotMat[0].inverse()*fTranslation[1]-
    fRotMat[0].inverse()*fTranslation[0];
  
  
  for (Int_t i=0;i<entries;i++) { 
    fHits->Clear();
    fAlignAll->GetEntry(i); 
    hitA = (HMdcHit*) fHits->At(0);
    hitB = (HMdcHit*) fHits->At(1);
    hitC = (HMdcHit*) fHits->At(2);
    
    mergeHits(hitC,hitB,fRotMat[0],fTranslation[0],hitBC);
    projPoint = findProjPoint(hitBC,rotaux,transaux,projSlopes);
    transformToSlopes(hitA,origSlopes);
    
    xSxcov->Fill(hitA->getX()-projPoint.getX(),
		 origSlopes[0]-projSlopes[0]);
    ySycov->Fill(hitA->getY()-projPoint.getY(),
		 origSlopes[1]-projSlopes[1]);
    xycov->Fill(hitA->getX()-projPoint.getX(),
		hitA->getY()-projPoint.getY());
    xSycov->Fill(hitA->getX()-projPoint.getX(),
		 origSlopes[1]-projSlopes[1]);
    ySxcov->Fill(hitA->getY()-projPoint.getY(),
		 origSlopes[0]-projSlopes[0]);
    SxSycov->Fill(origSlopes[0]-projSlopes[0],
		  origSlopes[1]-projSlopes[1]); 
  }
  
  //INTRODUCING THE COVARIANCES OF THE VARIABLES      
  Bool_t cutPassed=kFALSE;
  //Covariances (correlation factors)
  //Int_t modA = fLoc[1];
  //Int_t modB = fLoc[2]; 
  Int_t modC = fLoc[3]; 
  if (fMode>0) modC = fLoc[4];      
  
  
  Float_t rhoxSx, rhoxSy, rhoySy, rhoxy; 
  Float_t exponent;
  
  rhoxSx=xSxcov->GetCorrelationFactor();
  rhoySy=ySycov->GetCorrelationFactor();
  rhoxSy=xSycov->GetCorrelationFactor();
  rhoxy = xycov->GetCorrelationFactor();
  
  if(fout) 
    *fout << endl << "Correlation factors (xy,SxSy,xSx,ySy,xSy,ySx): " 
	  << endl << xycov->GetCorrelationFactor() << "  " 
	  << SxSycov->GetCorrelationFactor() << "  " 
	  << rhoxSx << "  " << rhoySy << "  " << rhoxSy << "  "
	  << ySxcov->GetCorrelationFactor() << endl << endl; 
  
  // close ascii file
  fout->close();
  delete fout;
  delete xSxcov;            
  delete ySycov;            
  delete xycov;             
  delete xSycov;            
  delete ySxcov;            
  delete SxSycov; 
  
  //filling the resolution and correlation for their later use
  fSigmaX = XNewAreaB/fXSigmas;
  fSigmaY = YNewAreaB/fYSigmas;
  fSigmaS0 = S0NewAreaB/fS1Sigmas;
  fSigmaS1 = S1NewAreaB/fS0Sigmas; 
  fRhoxSx = rhoxSx;
  fRhoySy = rhoySy;
  fRhoxSy = rhoxSy;
  
  //Reseting the tree if this function is called to generate iteratively
  //selected Hits distributions
  fAlignAllCut->Reset();
  fCountCut=0;
  
  for (Int_t i=0;i<entries;i++) {     
    fHits->Clear();
    fAlignAll->GetEntry(i);
    hitA = (HMdcHit*) fHits->At(0);
    hitB = (HMdcHit*) fHits->At(1);
    hitC = (HMdcHit*) fHits->At(2);
    
    Float_t resInAvsBinBCS_X, resInAvsBinBCS_Y;
    Float_t resInAvsBinBCS_XSlope, resInAvsBinBCS_YSlope;
    Float_t resInBvsAinACS_X, resInBvsAinACS_Y;
    Float_t resInBvsAinACS_XSlope, resInBvsAinACS_YSlope;
    
    //projecting 
    //rotaux2 = fRotMat[0].inverse() * fRotMat[1];
    //transaux2 = fRotMat[0].inverse()*fTranslation[1]-
    //  fRotMat[0].inverse()*fTranslation[0];
    mergeHits(hitB,hitA,rotaux2,transaux2,hitAB);
    projPoint = findProjPoint(hitAB,fRotMat[0],fTranslation[0],projSlopes);
    transformToSlopes(hitC,origSlopes);
    resInAvsBinBCS_X = hitC->getX() - projPoint.getX();
    resInAvsBinBCS_Y = hitC->getY() - projPoint.getY();      
    resInAvsBinBCS_XSlope = origSlopes[0] - projSlopes[0];
    resInAvsBinBCS_YSlope = origSlopes[1] - projSlopes[1];
    
    //then projecting on MDC A
    //rotaux = fRotMat[1].inverse();
    //transaux = -(rotaux * fTranslation[1]);
    mergeHits(hitC,hitB,fRotMat[0],fTranslation[0],hitBC);
    projPoint = findProjPoint(hitBC,rotaux,transaux,projSlopes);
    resInBvsAinACS_X = hitA->getX() - projPoint.getX();
    resInBvsAinACS_Y = hitA->getY() - projPoint.getY();
    transformToSlopes(hitA,origSlopes);
    resInBvsAinACS_XSlope = origSlopes[0] - projSlopes[0];
    resInBvsAinACS_YSlope = origSlopes[1] - projSlopes[1];

    if(AA_DEBUG>3){
      cout << endl <<"Cuts in fitHistograms(): " << endl;
      cout << fabs(resInAvsBinBCS_X - XNewMeanA) << " vs " 
	   << XNewAreaA << endl;
      cout << fabs(resInAvsBinBCS_Y - YNewMeanA) << " vs " 
	   << YNewAreaA << endl;	
      cout << fabs(resInAvsBinBCS_XSlope - S0NewMeanA) << " vs "     
	   << S0NewAreaA << endl;	
      cout << fabs(resInAvsBinBCS_YSlope - S1NewMeanA) << " vs "     
	   << S1NewAreaA << endl;	
      cout << fabs(resInBvsAinACS_X - XNewMeanB) << " vs "     
	   << XNewAreaB << endl;	 	
      cout << fabs(resInBvsAinACS_Y - YNewMeanB) << " vs "     
	   << YNewAreaB << endl;	 	
      cout << fabs(resInBvsAinACS_XSlope - S0NewMeanB) << " vs "     
	   << S0NewAreaB << endl;	 	
      cout << fabs(resInBvsAinACS_YSlope - S1NewMeanB) << " vs "     
	   << S1NewAreaB << endl;	
    }
    
    cutPassed=kFALSE;
    
    //CUTTING IN THE FOUR DISTRIBUTIONS WITHOUT COVARIANCE
    //strong condition: cutting in all histograms 
    if(fUseSharpCut){
      if(fabs(resInAvsBinBCS_X - XNewMeanA) < XNewAreaA && 
	 fabs(resInAvsBinBCS_Y - YNewMeanA) < YNewAreaA &&
	 fabs(resInAvsBinBCS_XSlope - S0NewMeanA) < S0NewAreaA &&
	 fabs(resInAvsBinBCS_YSlope - S1NewMeanA) < S1NewAreaA &&
	 fabs(resInBvsAinACS_X - XNewMeanB) < XNewAreaB && 
	 fabs(resInBvsAinACS_Y - YNewMeanB) < YNewAreaB &&
	 fabs(resInBvsAinACS_XSlope - S0NewMeanB) < S0NewAreaB &&
	 fabs(resInBvsAinACS_YSlope - S1NewMeanB) < S1NewAreaB ) {
	cutPassed=kTRUE;
      }
    }
    //CUTTING IN THE FOUR DISTRIBUTIONS WITH COVARIANCE
    else{      
      exponent =  (1/(1-rhoxy*rhoxy)) * 
	( ((resInBvsAinACS_X-XNewMeanB)*(resInBvsAinACS_X-XNewMeanB))/
	  ((XNewAreaB/fXSigmas)*(XNewAreaB/fXSigmas)) -
	  ((2*rhoxy)*
	   ((resInBvsAinACS_X-XNewMeanB)*(resInBvsAinACS_Y-YNewMeanB)))/
	  ((XNewAreaB/fXSigmas)*(YNewAreaB/fYSigmas)) +
	  ((resInBvsAinACS_Y-YNewMeanB)*(resInBvsAinACS_Y-YNewMeanB))/
	  ((YNewAreaB/fYSigmas)*(YNewAreaB/fYSigmas)) );
      
      if(exponent<pow(fXSigmas,2)) cutPassed=kTRUE;  
    }
    
    if(cutPassed==kTRUE){
      if(fUseSlopeCut){
	if(fSlopeCut>0){
	  // This cut makes the sample near indep. of Z translations
	  // and results useful for X and Y minimization.
	  
	  if(( fabs(hitA->getXDir()+hitB->getXDir()+hitC->getXDir()/3) 
	       < fSlopeCut) && 
	     ( fabs(hitA->getYDir()+hitB->getYDir()+hitC->getYDir()/3) 
	       < fSlopeCut)){
	    if(AA_DEBUG>3) cout << " -- CUT PASSED (fSlopeCut=" 
				<< fSlopeCut << " ) --" << endl;
	    //new((*fHits)[0])HMdcHit(*hitA);	  
	    //new((*fHits)[1])HMdcHit(*hitB);      
	    fAlignAllCut->Fill();
	    fHits->Clear();   	      
	    fCountCut++;
	  }
	}
	else{
	  // This cut results useful for Z minimization.
	  // The cut is effective only in MDCB, because fTranslation
	  // is represented in MDCB coordinates.
	  
	  if(( fabs(hitA->getXDir()+hitB->getXDir()+hitC->getXDir()/3) 
	       > -fSlopeCut) && 
	     ( fabs(hitA->getYDir()+hitB->getYDir()+hitC->getYDir()/3) 
	       > -fSlopeCut)){
	    if(AA_DEBUG>3) cout << " -- CUT PASSED (fSlopeCut=" 
				<< fSlopeCut << " ) --" << endl;
	    //new((*fHits)[0])HMdcHit(*hitA);	  
	    //new((*fHits)[1])HMdcHit(*hitB);      
	    fAlignAllCut->Fill();
	    fHits->Clear();   	      
	    fCountCut++;
	  }
	}
      }
      else{
	if(AA_DEBUG>3) cout << " ---------  CUT PASSED ------------" << endl;
	//new((*fHits)[0])HMdcHit(*hitA);	  
	//new((*fHits)[1])HMdcHit(*hitB);      
	fAlignAllCut->Fill();
	fHits->Clear();   	      
	fCountCut++;
      }
    }
  }
  delete f1X;
  delete f1Y;
  delete f1S;
  delete totalX;
  delete totalY;
  delete totalS;
  delete[] projSlopes;
  delete[] origSlopes; 
}





void HMdcAligner3M::setTree(void)
{
  //
  // Inits TNtuple
  //
  
  Int_t sector = fLoc[0];
  Int_t modA = fLoc[1];
  Int_t modB = fLoc[2];
  fRecCount++; 
  Char_t title[50], tmp[50], tmp2[50];
  if(!fOutRoot) fOutRoot = new TFile(fNameRoot,"UPDATE");
  
  Int_t modC = fLoc[3];   
  if (fMode>0){
    sector = fLoc[0]*10;
    modA = fLoc[1];
    sector += fLoc[2]; //sector = sectorA*10+sectorB
    modB = fLoc[3];      
    modC = fLoc[4];       
  }
  sprintf(title,"%s%i%s%i%s%i%s%i","Sector_",sector,"_ModA_",modA,
	  "_ModB_",modB,"_ModC_",modC);
  sprintf(tmp,"%s%s%i%s%i%s%i%s%i","All","_Sector_",sector,
	  "_ModA_",modA,"_ModB_",modB,"_ModC_",modC);    
  sprintf(tmp2,"%s%s%i%s%i%s%i%s%i","AllCut","_Sector_",sector,
	  "_ModA_",modA,"_ModB_",modB,"_ModC_",modC);
  
  fAlignAll = new TTree(tmp,title);
  fAlignAll->Branch("hits",&fHits);  
  fAlignAllCut = new TTree(tmp2,title);
  fAlignAllCut->Branch("hits",&fHits);
}





Bool_t HMdcAligner3M::init(void)
{
  //
  // Inits hitaux category and calls setParContainers() 
  //
  
  //Inicialization of hit category
  fHitCat=gHades->getCurrentEvent()->getCategory(catMdcHit);
  if (!fHitCat) {
    fHitCat=gHades->getSetup()->getDetector("Mdc")->buildCategory(catMdcHit);
    if (!fHitCat) return kFALSE;
    else gHades->getCurrentEvent()->addCategory(catMdcHit,fHitCat,"Mdc");
  }
  
  fIter1 = (HIterator *)fHitCat->MakeIterator(); 
  fIter2 = (HIterator *)fHitCat->MakeIterator(); 
  fIter3 = (HIterator *)fHitCat->MakeIterator(); 
  
  setParContainers(); 
  
  if(fHistoOff!=kTRUE) setHistograms();
  else setTree();
  
  return kTRUE;
}





Bool_t HMdcAligner3M::reinit(void)
{
  //
  // Call the functions returning auxiliar geometrical parameters
  //
  
  if(fAuto == kFALSE) setGeomAuxPar();
  //Esta linea llamaba a la funcion cuando no debia hacerlo.
  //Afortunadamente no hace nada la funcion sin argumentos.
  //else if(fAuto == kTRUE) setGeomAuxParSim();
  else if(fAuto == kTRUE) ;
  return kTRUE;
}





void HMdcAligner3M::setGeomParAutoOn(void)
{
  //
  //Turn on the automatic geometrical parameter input
  //Use it for inserting manually the parameters in the macro
  //

  fAuto =kTRUE;  
  cout << "WARNING in HMdcAligner3M::setGeomParAutoOn(): " 
       << "introducing manually Geometrical" << endl; 
  cout << "Parameters without containers. " 
       << "Parameters should be in the macro" << endl;
}





void HMdcAligner3M::setControlHistoOff(void)
{  
  //
  // Disables control histograms output (saving memory)
  //

  fHistoOff = kTRUE;  
}





void HMdcAligner3M::setMinimization(Int_t select)
{  
  //
  // Selects minimization strategy. Only valids in this reduction!
  //   
  // select = 100 (fcnalign3) Chi square sum in minimization
  // select = 101 (fcnalign3) Distances line-hit
  //  
  // select = 105 (fcnalign3) Minimizing angles between 2 lines 
  //              made from Hits (testing)
  // select = 106 (fcnalign3) Error with MINOS!


  fMin = select;  
}





void HMdcAligner3M::setUnitErrors(void)
{  
  //
  // introduce unit errors in Hits
  //

  fUseUnitErrors = kTRUE;  
}





void HMdcAligner3M::setOffCov(void)
{  
  //
  // Sets off the covariance in the translationFinder() function
  //

  fDoNotUseCov = kTRUE;  
}





void HMdcAligner3M::setSharpCut(void)
{  
  //
  // Sets off the covariance in fitHistograms()
  //

  fUseSharpCut = kTRUE;  
}




void HMdcAligner3M::setFix(Int_t fix)
{  
  //
  // Fix a parameter set in minimization
  // New scheme: 18 bits (binary number) specifying the parameters
  // fixed (1) and released (0). Lower bit is first parameter
  //

  fFix = fix;  
}





void HMdcAligner3M::setNoCut(void)
{  
  //
  // Disables the cuts after fitting the histograms
  //

  fUseCut = kFALSE;  
}





void HMdcAligner3M::setSlopeCut(Float_t SlopeCut)
{  
  //
  // Enables the Slope cuts after fitting the histograms.
  // This cut makes the sample near indep. of Z translations
  // and results useful for X and Y minimization.
  // For 2 MDCs: the cut is effective only in MDCB, because fTranslation
  // is represented in MDCB coordinates. Then, tracks passing
  // the cut are almost parallel to Z direction if fSlopeCut is positive
  // For 3 MDCs: a mean value for the three slopes. tracks passing
  // the cut are almost parallel to Z direction if fSlopeCut is positive

  fUseSlopeCut = kTRUE;  
  fSlopeCut = SlopeCut;
}





void HMdcAligner3M::setParContainers(void)
{
  //
  // Loads the parameter containers it uses later
  //

  fMdcGeomPar=(HMdcGeomPar*)gHades->getRuntimeDb()->getContainer("MdcGeomPar");
}





void HMdcAligner3M::setGeomAuxPar(void)
{
  //
  // Defines the parameter containers it uses later
  //
  // The transformations are:  X = R X' + T
  // in the geometrical parameters.
  // To obtain relative transformations: X(B) = M X(A) + V
  // we follow:
  //   X = R(A) X(A) + T(A)      
  //   X = R(B) X(B) + T(B)  
  //
  //   X(B) = [R(B)]E-1 R(A) X(A) + [R(B)]E-1 [T(A)-T(B)] 
  //
  //   M = [R(B)]E-1 R(A)
  //   V = [R(B)]E-1 [T(A)-T(B)] 
  //
  // From M it is easy to get the relative Euler angles 
  //
  // CONVENTION: fRotMat[i] and fTranslation[i] store the
  // matrix M and vector V of the transformations made like this:
  // 4 modules: [0]  X(D) = fRotMat[0] X(C) + fTranslation[0]
  //            [1]  X(D) = fRotMat[1] X(B) + fTranslation[1]
  //            [2]  X(D) = fRotMat[2] X(A) + fTranslation[2]
  //
  // 3 modules: [0]  X(C) = fRotMat[0] X(B) + fTranslation[0]
  //            [1]  X(C) = fRotMat[1] X(A) + fTranslation[1]
  //
  // 2 modules: [0]  X(B) = fRotMat[0] X(A) + fTranslation[0]
  //

  Int_t sector = fLoc[0];
  Int_t moduleA = fLoc[1];
  Int_t moduleB = fLoc[2];
  Int_t sectorB=0;    
  if (fMode>0){
    sector = fLoc[0];
    moduleA = fLoc[1];
    sectorB = fLoc[2];
    moduleB = fLoc[3];           
  }  
  HGeomVector euler;

  HGeomTransform transformA;
  transformA = fMdcGeomPar->getModule(sector,moduleA)->getLabTransform();
  HGeomTransform transformB;
  if(fMode>0) transformB = fMdcGeomPar->getModule(sectorB,moduleB)->getLabTransform();
  else transformB = fMdcGeomPar->getModule(sector,moduleB)->getLabTransform();
  HGeomRotation rotA;
  rotA = transformA.getRotMatrix();
  HGeomVector vectorA;
  vectorA = transformA.getTransVector();
  HGeomRotation rotB;
  rotB = transformB.getRotMatrix();
  HGeomVector vectorB;
  vectorB = transformB.getTransVector();

  if(fNumMods>3){
    Int_t moduleC = fLoc[3];  
    Int_t moduleD = fLoc[4];
    HGeomTransform transformC;
    transformC = fMdcGeomPar->getModule(sector,moduleC)->getLabTransform();
    HGeomTransform transformD;
    transformD = fMdcGeomPar->getModule(sector,moduleD)->getLabTransform();
    HGeomRotation rotC;
    rotC = transformC.getRotMatrix();
    HGeomVector vectorC;
    vectorC = transformC.getTransVector();
    HGeomRotation rotD;
    rotD = transformD.getRotMatrix();
    HGeomVector vectorD;
    vectorD = transformD.getTransVector();
    
    if(AA_DEBUG>0){
      cout << endl <<"Debugging output in HMdcAligner3M::setGeomAuxPar" << endl;
      cout << "Original transformation from container" << endl;
      cout << " ------ SECTOR " << sector << " ------ " << endl;
      cout << "MDC A (Module " << moduleA << ")" << endl;
      transformA.print();  
      cout << "MDC B (Module " << moduleB << ")" << endl;
      transformB.print();      
      cout << "MDC C (Module " << moduleC << ")" << endl;
      transformC.print();  
      cout << "MDC D (Module " << moduleD << ")" << endl;
      transformD.print();
    }
     
    //From the previous transformation, get the relative transformation 
    //   M = [R(D)]E-1 R(C)
    //   V = [R(D)]E-1 [T(C)-T(D)] 
    
    HGeomRotation rotDinv = rotD.inverse();
    HGeomRotation relrot = rotDinv * rotC;
    HGeomVector relvector = rotDinv * (vectorC - vectorD);
   
    if(AA_DEBUG>0){
      //Printing relative transformation (MDCC -> MDCD)
      cout << endl <<"Relative transformation:  (MDCC -> MDCD)" << endl;
      relrot.print();
      relvector.print();
    }
     
    //Filling the first rotation (MDCC and MDCD)
    euler = findEulerAngles(relrot);    
    fillRotMatrix(0,euler.getX(),euler.getY(),euler.getZ());
    fillTranslation(0,relvector.getX(),relvector.getY(),relvector.getZ());
    setEulerAngles(0,euler.getX(),euler.getY(),euler.getZ());

    //The same for (MDCB and MDCD)
    relrot = rotDinv * rotB;
    relvector =  rotDinv * (vectorB - vectorD);
   
    if(AA_DEBUG>0){
      //Printing relative transformation (MDCB -> MDCD)
      cout << endl <<"Relative transformation: (MDCB -> MDCD)" << endl;
      relrot.print();
      relvector.print();
    }
     
    //Filling the second rotation (MDCB and MDCD)
    euler = findEulerAngles(relrot);    
    fillRotMatrix(1,euler(0),euler(1),euler(2));
    fillTranslation(1,relvector.getX(),relvector.getY(),relvector.getZ());
    setEulerAngles(1,euler(0),euler(1),euler(2));


    //The same for (MDCA and MDCD)
    relrot = rotDinv * rotA;
    relvector =  rotDinv * (vectorA - vectorD);
   
    if(AA_DEBUG>0){
      //Printing relative transformation (MDCA -> MDCD)
      cout << endl <<"Relative transformation: (MDCA -> MDCD)" << endl;
      relrot.print();
      relvector.print();
    }
     
    //Filling the second rotation (MDCA and MDCD)
    euler = findEulerAngles(relrot);    
    fillRotMatrix(2,euler(0),euler(1),euler(2));
    fillTranslation(2,relvector.getX(),relvector.getY(),relvector.getZ());
    setEulerAngles(2,euler(0),euler(1),euler(2));
    
    cout << "**************************************************" << endl;
    cout << "* HMdcAligner3M::setGeomAuxPar: from geom params:  *" << endl;
    cout << "* Sector: "<< sector << "  ModA: " << moduleA 
	 << " ModB: " << moduleB <<  "  ModC: " << moduleC 
	 << " ModD: " << moduleD << endl;
    for(Int_t c=0;c<3;c++){
      cout << "* Rotation(" << c << "): " << fEuler[c].getX() << ", " 
	   << fEuler[c].getY()  << ", " << fEuler[c].getZ() << endl;
      cout << "* Translation(" << c << "): " << fTranslation[c].getX() 
	   << ", " << fTranslation[c].getY() 
	   << ", " <<  fTranslation[c].getZ() << endl;
    }
    cout << "**************************************************" << endl;
  }
  else if(fNumMods>2){  
    Int_t moduleC = fLoc[3]; 
    if (fMode>0){     
      sector = fLoc[0]*10;
      sector += fLoc[2]; //sector = sectorA*10+sectorB
      moduleC = fLoc[4];       
    }

    HGeomTransform transformC;
    if(fMode>0) transformC = fMdcGeomPar->getModule(sectorB,moduleC)->getLabTransform();
    else transformC = fMdcGeomPar->getModule(sector,moduleC)->getLabTransform();
    HGeomRotation rotC;
    rotC = transformC.getRotMatrix();
    HGeomVector vectorC;
    vectorC = transformC.getTransVector();
    
    if(AA_DEBUG>0){
      cout << endl <<"Debugging output in HMdcAligner3M::setGeomAuxPar" << endl;
      cout << "Original transformation from container" << endl;
      cout << " ------ SECTOR " << sector << " ------ " << endl;
      cout << "MDC A (Module " << moduleA << ")" << endl;
      transformA.print();  
      cout << "MDC B (Module " << moduleB << ")" << endl;
      transformB.print();    
      cout << "MDC C (Module " << moduleC << ")" << endl;
      transformC.print();
    }
        
    //From the previous transformation, get the relative transformation 
    //   M = [R(B)]E-1 R(A)
    //   V = [R(B)]E-1 [T(A)-T(B)] 
    
    HGeomRotation rotCinv = rotC.inverse();
    HGeomRotation relrot = rotCinv * rotB;
    HGeomVector relvector = rotCinv * (vectorB - vectorC);
       
    if(AA_DEBUG>0){
      //Printing relative transformation (MDCB -> MDCC)
      cout << endl <<"Relative transformation: (MDCB -> MDCC)" << endl;
      relrot.print();
      relvector.print();
    }
     
    //Filling the second rotation (MDCB and MDCC)
    euler = findEulerAngles(relrot);    
    fillRotMatrix(0,euler(0),euler(1),euler(2));
    fillTranslation(0,relvector.getX(),relvector.getY(),relvector.getZ());
    setEulerAngles(0,euler(0),euler(1),euler(2));

    //The same for (MDCA and MDCC)
    relrot = rotCinv * rotA;
    relvector =  rotCinv * (vectorA - vectorC);
   
    if(AA_DEBUG>0){
      //Printing relative transformation (MDCA -> MDCC)
      cout << endl <<"Relative transformation: (MDCA -> MDCC)" << endl;
      relrot.print();
      relvector.print();
    }
     
    //Filling the second rotation (MDCA and MDCC)
    euler = findEulerAngles(relrot);    
    fillRotMatrix(1,euler(0),euler(1),euler(2));
    fillTranslation(1,relvector.getX(),relvector.getY(),relvector.getZ());
    setEulerAngles(1,euler(0),euler(1),euler(2));
    
    cout << "**************************************************" << endl;
    cout << "* HMdcAligner3M::setGeomAuxPar: from geom params:  *" << endl;
    cout << "* Sector: "<< sector << "  ModA: " << moduleA 
	 << " ModB: " << moduleB <<  "  ModC: " << moduleC << endl;
    for(Int_t c=0;c<2;c++){
      cout << "* Rotation(" << c << "): " << fEuler[c].getX() << ", " 
	   << fEuler[c].getY()  << ", " << fEuler[c].getZ() << endl;
      cout << "* Translation(" << c << "): " << fTranslation[c].getX() 
	   << ", " << fTranslation[c].getY() 
	   << ", " <<  fTranslation[c].getZ() << endl;
    }
    cout << "**************************************************" << endl;
  }  
  else{

    if(AA_DEBUG>0){
      cout << endl <<"Debugging output in HMdcAligner3M::setGeomAuxPar" << endl;
      cout << "Original transformation from container" << endl;
      cout << " ------ SECTOR " << sector << " ------ " << endl;
      cout << "MDC A (Module " << moduleA << ")" << endl;
      transformA.print();  
      cout << "MDC B (Module " << moduleB << ")" << endl;
      transformB.print();
    }
    
    //From the previous transformation, get the relative transformation 
    //   M = [R(B)]E-1 R(A)
    //   V = [R(B)]E-1 [T(A)-T(B)] 
    
    HGeomRotation rotBinv = rotB.inverse();
    HGeomRotation relrot = rotBinv * rotA;
    HGeomVector relvector =  rotBinv * (vectorA - vectorB);
    
    if(AA_DEBUG>0){
      //Printing relative transformation
      cout << endl <<"Relative transformation: " << endl;
      relrot.print();
      relvector.print();
    }
         
    //Filling the first rotation (MDCA and MDCB)
    euler = findEulerAngles(relrot);    
    fillRotMatrix(0,euler(0),euler(1),euler(2));
    fillTranslation(0,relvector.getX(),relvector.getY(),relvector.getZ());
    setEulerAngles(0,euler(0),euler(1),euler(2));
    
    cout << "**************************************************" << endl;
    cout << "* HMdcAligner3M::setGeomAuxPar: from geom params:  *" << endl;
    cout << "* Sector: "<< sector << "  ModA: " << moduleA 
	 << " ModB: " << moduleB << endl;
    cout << "* Rotation(0): " << fEuler[0].getX() << ", " 
	 << fEuler[0].getY()  << ", " << fEuler[0].getZ() << endl;   
    cout << "* Translation: " << relvector.getX() << ", " 
	 << relvector.getY() << " , " <<  relvector.getZ() << endl;
    cout << "**************************************************" << endl;
  }
}


HGeomVector HMdcAligner3M::findEulerAngles(HGeomRotation rot){
  //
  // From the relative rotation, get the euler angles (radians)
  //
  // From an Euler rotation (see Dahlinger paper for the convention)   
  // the euler angles are:
  // euler[0] = atan2(rot(5)/sin(euler[1]),rot(2)/sin(euler[1]))
  // euler[1] = acos (rot(8))    with possible change of sign
  // euler[2] = atan2(rot(7)/sin(euler[1]),-rot(6)/sin(euler[1]))
  //

  Double_t euler[3];
  HGeomVector eul;

  //Checking if rot(8) is in the acos() domain
  if(rot(8)< 0.99999 && rot(8)>-0.99999) euler[1] = acos(rot(8));
  else euler[1]=0;
  Double_t sinaux;
  if(euler[1] == 0.){
    //euler[0] and euler[2] are equivalent. Write all in euler[0]
    euler[0]= (TMath::Pi()/2)+acos(rot(0));
    euler[2]=-(TMath::Pi()/2);
  } 
  else{  //IS AN EULER MATRIX
    sinaux = sin(euler[1]);
    euler[0] = atan2(rot(5)/sinaux,rot(2)/sinaux);
    euler[2] = atan2(rot(7)/sinaux,-rot(6)/sinaux);
  }
  
  if(AA_DEBUG>0){
    cout << endl <<"Euler angles: " << euler[0] << ", " 
	 << euler[1]  << ", " << euler[2] << endl;
  }
  
  HGeomRotation test;  
  test.setEulerAngles(euler[0]*180./TMath::Pi(),
		      euler[1]*180./TMath::Pi(),
		      euler[2]*180./TMath::Pi()); 
  
  if(AA_DEBUG>0){
    cout << endl <<"Rotation from Euler angles (first attemp): " << endl;
    test.print();
  }
  
  //Now solving the problem when euler[1]<0
  Double_t eps = 0.0001; //check precision
  
  if( (fabs(test(0)-rot(0))>eps) || (fabs(test(1)-rot(1))>eps) || 
      (fabs(test(3)-rot(3))>eps) || (fabs(test(4)-rot(4))>eps) ) {
    if(AA_DEBUG>0) cout << endl << "Bad election in first euler angle! " 
			<< "Trying again. "<< endl;
    euler[1] = - euler[1]; 
    sinaux = sin(euler[1]);
    euler[0] = atan2(rot(5)/sinaux,rot(2)/sinaux);
    euler[2] = atan2(rot(7)/sinaux,-rot(6)/sinaux);
    
    test.setEulerAngles(euler[0]*180./TMath::Pi(),
			euler[1]*180./TMath::Pi(),
			euler[2]*180./TMath::Pi());  
    
    if(AA_DEBUG>0){
      cout << "Rotation from Euler angles (second attemp): " << endl;
      test.print();
    }
    
    //testing if euler angles are rigth now
    if( (fabs(test(0)-rot(0))>eps) || (fabs(test(1)-rot(1))>eps) || 
	(fabs(test(3)-rot(3))>eps) || (fabs(test(4)-rot(4))>eps) ){
      cout << endl << "FATAL ERROR: Bad election in euler angles! "<< endl;
      cout << "Original rot matrix: "<< endl;
      rot.print();
      cout << "From obtained euler angles: " << endl;
      test.print();
      //What to do??
    }
  }
  eul.setX(euler[0]); 
  eul.setY(euler[1]); 
  eul.setZ(euler[2]); 
  return eul;
}





void HMdcAligner3M::setGeomAuxParSim(Int_t ind, Float_t eu1, Float_t eu2, 
				     Float_t eu3, Float_t tr1, 
				     Float_t tr2, Float_t tr3)
{
  //
  // Entering geometrical parameters.
  //
  // To be used introducing in the macro the parameters for
  // testing, optimization ...
  //
  
  cout << "WARNING: Introducing automatically Geometrical" << endl; 
  cout << "Parameters without containers" << endl;
  
  fillRotMatrix(ind,eu1,eu2,eu3);
  fillTranslation(ind,tr1,tr2,tr3);  
  setEulerAngles(ind,eu1,eu2,eu3);
  if(AA_DEBUG>0){
    cout << "Transformation[" << ind << "]:" << endl;
    cout <<" Euler angles: " << eu1 << ", " 
	 << eu2  << ", " << eu3 << endl;
    cout << " Translation: " << tr1 << ", " 
	 << tr2  << ", " << tr3 << endl;
  }
}





Int_t HMdcAligner3M::execute(void)
{    
  // 
  // Iteration in the hit category. Fills histograms
  // and TTree and calculates relevant quantities
  //  

  execute3();
  
  return 0; 
}

  



void HMdcAligner3M::execute3(void)
{    
  // 
  // New execute for more than two MDCs
  // UPDATED TO COSMICS AND pp REACTIONS (fMode>0)
  //
  
  HMdcHit* pHitA;         
  HMdcHit* pHitB;       
  HMdcHit* pHitC; 
  HMdcHit* pHitBC = new HMdcHit;
  HMdcHit* pHitABC = new HMdcHit;
  
  Int_t sector = fLoc[0];
  Int_t modA = fLoc[1];
  Int_t modB = fLoc[2]; 
  Int_t modC = fLoc[3];
  Int_t sectorB=0; 
  if (fMode>0){
    sector = fLoc[0];
    modA = fLoc[1];
    sectorB = fLoc[2];
    modB = fLoc[3];      
    modC = fLoc[4];       
  }
  
  HLocation local;
  local.setNIndex(2);
  if(fMode>1) local.set(2,sectorB,modC); 
  else local.set(2,sector,modC); 

  HGeomVector projPoint; 
  Float_t* projSlopes = new Float_t[2];

  Bool_t usedA = kFALSE;   
  Bool_t usedB = kFALSE;
  Bool_t invalidA = kFALSE; 
  Bool_t invalidB = kFALSE; 

  fNEntries++;

  HGeomRotation rotInv = fRotMat[0].inverse();
  HGeomVector trasInv = -(rotInv * fTranslation[0]);
  HGeomRotation rotInv2 = fRotMat[1].inverse();
  HGeomVector trasInv2 = -(rotInv2 * fTranslation[1]);

  fIter1->Reset(); 
  fIter1->gotoLocation(local); 
  //next line has been change to get rid of the Dubna fitter results with
  //chi square=-1, that is, the results of the cluster finder
  while ((pHitC =(HMdcHit*)fIter1->Next()) != 0){ 
    if(pHitC->getChi2()>0)
      {
	fHitsMdc[0]++;     //Hits found in MDC C
	usedA = kFALSE;    //reinit control flags
	usedB = kFALSE;
	invalidA = kFALSE; 
	invalidB = kFALSE;  
    
	if(AA_DEBUG>3)   {
	  cout << "     ----- SECTOR " << sector << " -----"<< endl; 
	  cout << "Module " << modC << ", fHitsMdc " << fHitsMdc[0] 
	       << endl;
	}  
    
	//   Calculation of cross point and slopes in MDC B  
	// The rotation matrix here used is inverse of fRotMat
	// (see CONVENTION in HMdcAligner3M::setGeomAuxPar())
	// That is:      X(B) = fRotMat[0]-1 * (X(C) - fTranslation[0])
	// ==> X(B) = fRotMat[0]-1 * X(C) - fRotMat[0]-1 * fTranslation[0]
	// because we are calculating the projection of MDC C Hits on MDC B
	//
	projPoint = findProjPoint(pHitC,rotInv,trasInv,projSlopes);

	//Iteration on the second MDC (MDCB) for each hit in the first (MDCC)
	if(fMode>1) local.set(2,sectorB,modB); 
	else local.set(2,sector,modB); 
	fIter2->Reset(); 
	fIter2->gotoLocation(local); 
	//next line has been change to get rid of the Dubna fitter results with
	//chi square=-1, that is, the results of the cluster finder
	while ((pHitB =(HMdcHit*)fIter2->Next()) != 0){ 	
	  if(pHitB->getChi2()>0)
	    {
	      //hits found in MDCB, provided there is a Hit in MDCC
	      fHitsMdc[1]++;     
    
	      if(AA_DEBUG>3)  
		cout << "Module " << modB << ", fHitsMdc " << fHitsMdc[1] << endl; 
         
	      if(isInsideWindow(1,pHitB,projPoint,projSlopes)&&(invalidA!=kTRUE)){

		if(usedB == kFALSE){  

		  usedB = kTRUE;
		  // MDCB hits found in window (only used if unique)
		  fHitsFoundInWindow[0]++;
		  fHitsFoundAndUnique[0]++;

		  //merging hits of MDCB and MDCC on MDCC coordinate system
		  mergeHits(pHitC,pHitB,fRotMat[0],fTranslation[0],pHitBC);
	  
		  //   Calculation of cross point and slopes in MDC A  
		  // The rotation matrix here used is inverse of fRotMat
		  // (see CONVENTION in HMdcAligner3M::setGeomAuxPar())
		  // That is:      X(A) = fRotMat[1]-1 * (X(C) - fTranslation[1])
		  // ==> X(A) = fRotMat[1]-1 * X(C) - fRotMat[1]-1 * fTranslation[1]
		  // because we are calculating the projection of MDCC Hits on MDCA
		  // (in this case the merging of MDCC and MDCB, but in MDCC 
		  // coordinate system anyway)
		  projPoint = findProjPoint(pHitBC,rotInv2,trasInv2,projSlopes);
	
		  //Iteration on the third MDC (MDCA) for each couple MDCC-MDCB
		  local.set(2,sector,modA); 
		  fIter3->Reset(); 
		  fIter3->gotoLocation(local);
		  //next line has been change to get rid of the Dubna fitter results with
		  //chi square=-1, that is, the results of the cluster finder 
		  while ((pHitA =(HMdcHit*)fIter3->Next()) != 0 ){ 	
		    if(pHitA->getChi2()>0)
		      {
			//hits found in MDCA, provided there is a Hit in MDCC 
			//and one in MDB. If there is more than one in MDCB the 
			//number is also increased but the hit will not be used
			fHitsMdc[2]++;
	    
			if(AA_DEBUG>3)  
			  cout << "Module " << modB << ", fHitsMdc " 
			       << fHitsMdc[1] << endl; 
    
			if(isInsideWindow(0,pHitA,projPoint,projSlopes)){
			  if(usedA == kFALSE){  
		
			    usedA = kTRUE;
			    // MDCA hits found in window (only used if unique)
			    fHitsFoundInWindow[1]++;
			    fHitsFoundAndUnique[1]++;
		
			    //Real number of matched hits
			    fCount++;
		
			    //merging all HITS (curiosity or can be used??)
			    mergeHits(pHitBC,pHitA,fRotMat[1],
				      fTranslation[1],pHitABC );
		
			    //Filling the TClonesArray for storage in TTree
			    //Will be used only if 
			    fHits->Clear();	
			    new((*fHits)[0])HMdcHit(*pHitA);	  
			    new((*fHits)[1])HMdcHit(*pHitB);
			    new((*fHits)[2])HMdcHit(*pHitC);	
		
			  } //  End of  if(usedA == kFALSE){
	      
			  else{          //that is, if usedA == kTRUE
			    //More than one candidate on window!! Discart 
			    if(invalidA == kFALSE){
			      fCount--;  
			      invalidA = kTRUE;
			      fDiscart[1]++;	
			      fHitsFoundAndUnique[1]--;
			    }
			  } // End of  else{          //that is, if usedA == kTRUE
	      
			} // End of  if(isInsideWindow(pHitA,projPoint,projSlopes)){
		      }
		  } // End of  while( (pHitA =(HMdcHit*)fIter3->Next()) != 0 ){ 
		} // End of  if(usedB == kFALSE){  
	
		else{          //that is, if usedB == kTRUE
		  //More than one candidate on window!! Discart 
		  if(invalidB == kFALSE){
		    invalidB = kTRUE;
		    fDiscart[0]++;	
		    fHitsFoundAndUnique[0]--;
		  }
		} // End of  else{          //that is, if usedB == kTRUE
	      
	      } // End of  if(isInsideWindow(pHitB,projPoint,projSlopes)&&...){
	    }
	} // End of  while( (pHitB =(HMdcHit*)fIter2->Next()) != 0 ){ 
    
	if(usedB == kTRUE && invalidB != kTRUE && 
	   usedA == kTRUE && invalidA != kTRUE){
	  fAlignAll->Fill();
	  fHits->Clear();
	  if(fCount%100 ==0) cout << "."<< flush;
	}
      }
  } // End of  while ( (pHitC =(HMdcHit*)fIter1->Next()) != 0 ){ 

  if(pHitBC!=0) delete pHitBC;
  if(pHitABC!=0) delete pHitABC;
  if(projSlopes!=0) delete[] projSlopes;
}





HGeomVector HMdcAligner3M::findProjPoint(HMdcHit* pHit, HGeomRotation rot,
					 HGeomVector tra, Float_t* slopes)
{
  //
  // Find the projection point of a Hit on another MDC
  //
  // Given a relative rotation and translation from MDC A to MDC B
  // (see CONVENTION in HMdcAligner3M::setGeomAuxPar())
  //  X(B) = rot X(A) + tra
  // this function obtains the projection in MDC B of a Hit in MDC A
  //
  // When the user wants to obtain the projection in MDC A of a Hit in 
  // MDC B, should provide the inverse transformations
  // Example: X(A) = rot-1 * X(B) - rot-1 * tra 
  //               = newrot * X(B) + newtra
  // where:
  //    newrot = rot-1
  //    newtra = - (rot-1 * tra)
  // The function returns also the slopes in the new coordinate system 

  HGeomVector newvec;
  Float_t x, y, s0=0, s1=0;
  Float_t xDir, yDir, aux, den;  
  Float_t zsearch, xsearch, ysearch;

  x = pHit->getX();              //getting the hit info     
  y = pHit->getY();
  
  xDir = pHit->getXDir(); 
  yDir = pHit->getYDir();
  aux = sqrt(1 - xDir * xDir - yDir * yDir);    
  if(aux == 0.){ s0=1; s1=1;} //non-sense values
  else{
    s0 = xDir/aux;
    s1 = yDir/aux;
  }
  
  if(AA_DEBUG>3){
    cout << "VALID MDC HIT: " << x << " " << y 
	 <<  " " << s0 << " " << s1 << endl;
  }
  zsearch = -(x*rot(6) + y*rot(7) + tra(2)) / 
    (rot(8) + s0*rot(6) + s1*rot(7)); 
  
  xsearch = x*rot(0) + y*rot(1) + tra(0) + 
    zsearch*(s0*rot(0) + s1*rot(1) + rot(2));
  
  ysearch = x*rot(3) + y*rot(4) + tra(1) + 
    zsearch*(s0*rot(3) + s1*rot(4) + rot(5));
  
  //For getting the histograms in slopes and also for new cuts!!
  den = s0*rot(6) + s1*rot(7) + rot(8);
  if (den == 0) {
    cout << "ERROR in HMdcAligner3M::findProjPoint()" << endl;
    return newvec;
  }  
  slopes[0] = (s0*rot(0) + s1*rot(1) + rot(2)) / den;
  slopes[1] = (s0*rot(3) + s1*rot(4) + rot(5)) / den;
  
  if(AA_DEBUG>3){
    cout << "Projected MDC HIT: " << xsearch << " " << ysearch 
	 <<  " " << slopes[0] << " " << slopes[1] << endl;
  }
  
  newvec.setX(xsearch);
  newvec.setY(ysearch);
  newvec.setZ(zsearch);
  return newvec;
}





Bool_t HMdcAligner3M::isInsideWindow(Int_t plot, HMdcHit* pHit, HGeomVector point,
				     Float_t* slope){
  //
  //Check if the hit is inside a window around point and slope  
  //old check based on square cuts 
  //New one based on equiprobability elipse (Beatriz paper)
  //

  Float_t xlolimit,xuplimit,ylolimit,yuplimit;   
  
  xlolimit = point.getX() - fXArea;
  xuplimit = point.getX() + fXArea;
  ylolimit = point.getY() - fYArea;
  yuplimit = point.getY() + fYArea;

  if(plot && (fHistoOff==kFALSE)){
    Float_t x, y, s0, s1;
    Float_t xDir, yDir, aux;  
    
    x = pHit->getX();              //getting the hit info     
    y = pHit->getY();
    xDir = pHit->getXDir(); 
    yDir = pHit->getYDir();
    //if using hit!
    aux = sqrt(1 - xDir * xDir - yDir * yDir);    
    if(aux == 0.){ s0=1; s1=1;} //non-sense values
    else{
      s0 = xDir/aux;
      s1 = yDir/aux;
    }
    
    fResX->Fill(x - point.getX());
    fResY->Fill(y - point.getY());
    fResS0->Fill(s0 - slope[0]);
    fResS1->Fill(s1 - slope[1]);
  }

  if(AA_DEBUG>3) cout << "MDC HIT: " 
		      << pHit->getX() << " " << pHit->getY();
  
  if( (pHit->getX()>xlolimit) && (pHit->getX()<xuplimit) && 
      (pHit->getY()>ylolimit) && (pHit->getY()<yuplimit)) {
    if(AA_DEBUG>3) cout <<  "  inside window" << endl;
    return kTRUE;
  }

  else {    
    if(AA_DEBUG>3) cout <<  "  outside window" << endl;
    return kFALSE;
  }

}



void HMdcAligner3M::mergeHits(HMdcHit* hitB, HMdcHit* hitA,
			      HGeomRotation rot,HGeomVector tra, 
			      HMdcHit* mergeHit){
  //
  // Propagates hitA in hitB coordinate system and merges 
  // the information in a new hit in hitB coordinate system
  // The rot and tra verifies the CONVENTION 
  // (see HMdcAligner3M::setGeomAuxPar()), that is:
  //     X(B) = rot X(A) + tra
  // Normally B is reference MDC, which is farther from target

  //Testing a merge in function of the coordinates 

  *mergeHit = *hitB;

  HGeomVector pointA(hitA->getX(),hitA->getY(),0);
  HGeomVector pointB(hitB->getX(),hitB->getY(),0);
  HGeomVector pointAinB = rot * pointA +tra;

  //calculating slopes and filling xDir and yDir
  Float_t slopeX = (pointAinB.getX() - pointB.getX())/pointAinB.getZ();
  //cout << slopeX ;
  Float_t slopeY = (pointAinB.getY() - pointB.getY())/pointAinB.getZ();
  //cout << "  " << slopeY ;
  Float_t aux = sqrt(slopeX*slopeX + slopeY*slopeY+1);
  //cout << "  " << aux << "  " << slopeX/aux << "  " << slopeY/aux << endl;

  
  mergeHit->setXYDir(slopeX/aux,0.1,slopeY/aux,0.1); 
  //the error is a non-meaning quantity just now.

  //newHit->print();
}





void HMdcAligner3M::transformToSlopes(HMdcHit* pHit, Float_t* slopes){
  //
  //Transform hit angular components in slopes
  //
  
  //  Float_t* slopes = new Float_t[2];
  Float_t xDir,yDir;
  Float_t aux;
  
  xDir = pHit->getXDir(); 
  yDir = pHit->getYDir();
  aux = sqrt(1 - xDir * xDir - yDir * yDir);    
  if(aux == 0.){ slopes[0]=1; slopes[1]=1;} //non-sense values
  else{
    slopes[0] = xDir/aux;
    slopes[1] = yDir/aux;
  }
}





void HMdcAligner3M::findAbsolutePosition(HGeomRotation* rot, HGeomVector* vect){
  //
  // Getting module A parameters from module B parameters (fRotLab fTransLab)
  // and from relative parameters (fRotMat and fTranslation)
  // The transformations are:  X = R X' + T
  // in the geometrical parameters. We have here:
  //   X = R(B) X(B) + T(B)  
  // and we know
  //   X(B) = M x(A) + V 
  // where
  //   M = [R(B)]E-1 R(A)
  //   V = [R(B)]E-1 [T(A)-T(B)] 
  // are the relative transformations we know (see HMdcAligner3M)
  // We want to find out the transformation
  //   X = R(A) X(A) + T(A)    
  // using:
  //   X = R(B) X(B) + T(B) = R(B) (M x(A) + V)  + T(B) =>
  //   X = R(B) M x(A) + R(B) V  + T(B) =>
  //   R(A) = R(B) M  and
  //   T(A) = R(B) V  + T(B)
  //

  Int_t sector = fLoc[0];
  Int_t modA = fLoc[1];
  Int_t modB = fLoc[2];
  Int_t modC = fLoc[3];
  
  Int_t sectorB=0;
  if (fMode>0){
    sector = fLoc[0];
    modA = fLoc[1];
    sectorB = fLoc[2];
    modB = fLoc[3];      
    modC = fLoc[4];       
  }
  
  HGeomTransform transformA,transformB,transformC;
  transformA = fMdcGeomPar->getModule(sector,modA)->getLabTransform();
  HGeomRotation rotOrigA,rotOrigB,rotOrigC,rotOrigD;
  rotOrigA = transformA.getRotMatrix();
  HGeomVector vectorOrigA,vectorOrigB,vectorOrigC,vectorOrigD;
  vectorOrigA = transformA.getTransVector();    
  
  if (fMode>0) transformB = fMdcGeomPar->getModule(sectorB,modB)->getLabTransform();    
  else transformB = fMdcGeomPar->getModule(sector,modB)->getLabTransform(); 
  rotOrigB = transformB.getRotMatrix();
  vectorOrigB = transformB.getTransVector();        
  

  if (fMode>0) transformC = fMdcGeomPar->getModule(sectorB,modC)
		 ->getLabTransform(); 
  else  transformC = fMdcGeomPar->getModule(sector,modC)
	  ->getLabTransform();
  rotOrigC = transformC.getRotMatrix();    
  vectorOrigC = transformC.getTransVector(); 
  
  rot[0] = rotOrigC * fRotMat[0];
  vect[0] = rotOrigC * fTranslation[0] + vectorOrigC;     
  rot[1] = rotOrigC * fRotMat[1];
  vect[1] = rotOrigC * fTranslation[1] + vectorOrigC;    
  if(AA_DEBUG >2){
    rot[0].print();
    vect[0].print();      
    rot[1].print();
    vect[1].print();
  }
}





Bool_t HMdcAligner3M::finalize(void)
{   
  // 
  // Statistical information and Minimization procedure
  //

  Int_t sector = fLoc[0];
  Int_t modA = fLoc[1];
  Int_t modB = fLoc[2];
  Int_t modC = fLoc[3];
  
  if (fMode>0){
    sector = fLoc[0]*10;
    modA = fLoc[1];
    sector += fLoc[2]; //sector = sectorA*10+sectorB
    modB = fLoc[3];      
    modC = fLoc[4];       
  }
  
  //first statistical information
  ofstream *fout=NULL;
  if (fNameAscii) fout = new ofstream(fNameAscii.Data(), ios::app);
  if (*fout){
    *fout << endl << "Sector: " << sector << endl;
    *fout << "Module A: " << modA << "  Module B: " << modB ;
    *fout << "  Module C: " << modC;    
    *fout << endl << endl; 
    *fout << "Number of events: "  << fNEntries << endl;  
    *fout << "Window (mm): " << fXArea <<"," << fYArea << endl;
    *fout << "Interpret smaller MDC index as last in the previous list" 
	  << endl << endl;
    for(Int_t i=0;i<fNumMods;i++){
      *fout << "Passing Hits in MDC[" << i << "]: " << fHitsMdc[i] << endl;
    }
    for(Int_t i=0;i<fNumMods-1;i++){
      *fout << "Hits found in MDC[" << i << "] window: " 
	    << fHitsFoundInWindow[i] << endl;
      *fout << "Hits found in MDC[" << i << "] window and unique: " 
	    << fHitsFoundAndUnique[i] << endl;
      *fout << "Hits discarted in MDC[" << i << "] window: " 
	    << fDiscart[i] << endl;
    }
    *fout << "Valid hits for alignment: " << fCount << endl;
  }
  if(AA_DEBUG>0){
    cout << endl << "Sector: " << sector << endl;
    cout << "Module A: " << modA << "  Module B: " << modB ;
    if(fNumMods>2) cout << "  Module C: " << modC;    
    cout << endl << endl; 
    cout << "Number of events: "  << fNEntries << endl;  
    cout << "Window (mm): " << fXArea <<"," << fYArea << endl;
    cout << "Interpret smaller MDC index as last in the previous list" 
	 << endl << endl;
    for(Int_t i=0;i<fNumMods;i++){
      cout << "Passing Hits in MDC[" << i << "]: " << fHitsMdc[i] << endl;
    }
    for(Int_t i=0;i<fNumMods-1;i++){
      cout << "Hits found in MDC[" << i << "] window: " 
	   << fHitsFoundInWindow[i] << endl;
      cout << "Hits found in MDC[" << i << "] window and unique: " 
	   << fHitsFoundAndUnique[i] << endl;
      cout << "Hits discarted in MDC[" << i << "] window: " 
	   << fDiscart[i] << endl;
    }
    cout << "Valid hits for alignment: " << fCount << endl;
  }
  
  //getting the individual MDC parameters before any calculation
  HGeomRotation absRot[fNumMods-1];
  HGeomVector absVect[fNumMods-1];
  findAbsolutePosition(absRot,absVect);  
 
  if(*fout){    
    *fout << endl << "Individual transformations before " 
	  << "minimization (init values): "  << endl;	
    *fout << "Interpret smaller MDC index as last in the previous list" 
	  << endl << endl;
    for(Int_t i=0;i<fNumMods-1;i++){
      *fout << "Module" << i <<  endl;      
      for(Int_t j=0;j<9;j++) *fout << absRot[i](j) << "   " ;
      *fout << endl;
      for(Int_t j=0;j<3;j++) *fout << absVect[i](j) << "   " ;
      *fout << endl << endl;
    }
  }
  if(AA_DEBUG>0){
    cout << endl << "Individual transformations before " 
	 << "minimization (init values): "  << endl;	
    cout << "Interpret smaller MDC index as last in the previous list" 
	 << endl << endl;
    for(Int_t i=0;i<fNumMods-1;i++){
      cout << "Module" << i <<  endl;      
      absRot[i].print();
      absVect[i].print();
    }
  }

  //filling common histograms
  if(fHistoOff!=kTRUE) {
    fillHistograms(0);  
    fitHistograms(0);
    fillHistograms(1,1);  
  }
  if (*fout){
    *fout << "Valid hits for alignment after cuts: " 
	  << fCountCut << endl << endl;      
    *fout << endl << endl;
  }
  if(AA_DEBUG>0) cout << "Valid hits for alignment after cuts: " 
		      << fCountCut << endl << endl;
  if (*fout){
    *fout << "Transformation before minimization (init values): "  << endl;
    for(Int_t i=0;i<fNumMods-1;i++){
      *fout << fEuler[i].getX() << ",  " << fEuler[i].getY() << ",  " 
	    << fEuler[i].getZ() << ",  " << fTranslation[i].getX() << ",  " 
	    << fTranslation[i].getY() << ",  " << fTranslation[i].getZ() 
	    << endl;
    }
  }
   
  //minimization (105 in full program)   
  // select = 100 (fcnalign3) Chi square sum in minimization
  // select = 101 (fcnalign3) Distances line-hit
  // select = 105 (fcnalign3) Minimizing angles between 2 lines made from Hits

  if (*fout) *fout << "Multiple iteration on different accepted Hits distribution: " 
		   << fMin <<" with MINUIT" << endl;
  Int_t IterCounter2 =0;
  Float_t IterCri2;        
  HGeomVector OldEuler2[(fNumMods-1)];
  HGeomVector OldTranslation2[(fNumMods-1)];
  do{
    IterCri2 = 0;      
    for(Int_t i=0;i<fNumMods-1;i++){
      OldEuler2[i] = fEuler[i];
      OldTranslation2[i] = fTranslation[i];
    }
    //
    
    //Minuit
    if (*fout) *fout << "Minimization strategy: " 
		     << fMin <<" with MINUIT" << endl;
    HGeomVector OldEuler[(fNumMods-1)];
    HGeomVector OldTranslation[(fNumMods-1)];
    Int_t IterCounter =0;
    Float_t IterCri;    

    do{
      IterCri = 0;
      for(Int_t i=0;i<fNumMods-1;i++){
	OldEuler[i] = fEuler[i];
	OldTranslation[i] = fTranslation[i];
      }
      minfit(fFix,fEuler,fTranslation);
      if (*fout){
	*fout << "Parameters after minimization "  << endl;
	for(Int_t i=0;i<fNumMods-1;i++){
	  *fout << fEuler[i].getX() << "+-" << fError[i*6] << ",  " 
		<< fEuler[i].getY() << "+-" << fError[i*6+1] << ",  " 
		<< fEuler[i].getZ() << "+-" << fError[i*6+2] << ",  " 
		<< fTranslation[i].getX() << "+-" << fError[i*6+3] << ",  " 
		<< fTranslation[i].getY() << "+-" << fError[i*6+4] << ",  " 
		<< fTranslation[i].getZ() << "+-" << fError[i*6+5] << endl;
	}		  
	*fout << "Function value:  " <<  fFunctionMin 
	      << "  from  "; 
	if(fUseCut) *fout << fCountCut << "  combinations." << endl;
	else *fout <<  fCount << "  combinations." << endl;
	*fout << endl;	
      }
      
      for(Int_t i=0;i<fNumMods-1;i++){
	fillRotMatrix(i,fEuler[i].getX(),fEuler[i].getY(),fEuler[i].getZ());
      }    
      
      for(Int_t i=0;i<fNumMods-1;i++){    
	if(fEuler[i].getX()!=0) 
	  IterCri += fabs((fEuler[i].getX()-OldEuler[i].getX())/
			  fEuler[i].getX());
	if(fEuler[i].getY()!=0) 
	  IterCri += fabs((fEuler[i].getY()-OldEuler[i].getY())/
			  fEuler[i].getY());
	if(fEuler[i].getZ()!=0) 
	  IterCri += fabs((fEuler[i].getZ()-OldEuler[i].getZ())/
			  fEuler[i].getZ());
	if(fTranslation[i].getX()!=0) 
	  IterCri += fabs((fTranslation[i].getX()-OldTranslation[i].getX())/
			  fTranslation[i].getX());
	if(fTranslation[i].getY()!=0) 
	  IterCri += fabs((fTranslation[i].getY()-OldTranslation[i].getY())/
			  fTranslation[i].getY());
	if(fTranslation[i].getZ()!=0) 
	  IterCri += fabs((fTranslation[i].getZ()-OldTranslation[i].getZ())/
			  fTranslation[i].getZ());
	
	if(AA_DEBUG==0){     
	  cout << i << "IterCri: " <<   IterCri << endl;
	}
      }
      IterCounter++;
      if(IterCounter>10) {
	cout << "WARNING in HMdcAligner3M :: finalize" << endl;
	cout << "Sector: " << sector << " ModuleA: " 
	     << modA <<  " ModuleB: " << modB << endl;
	cout << "More than 10 iterations without results!" <<endl;
	break;
      }
    }while(IterCri>fIterCriteria);
    
    for(Int_t i=0;i<fNumMods-1;i++){    
      if(fEuler[i].getX()!=0) 
	IterCri2 += fabs((fEuler[i].getX()-OldEuler2[i].getX())/
			 fEuler[i].getX());
      if(fEuler[i].getY()!=0) 
	IterCri2 += fabs((fEuler[i].getY()-OldEuler2[i].getY())/
			 fEuler[i].getY());
      if(fEuler[i].getZ()!=0) 
	IterCri2 += fabs((fEuler[i].getZ()-OldEuler2[i].getZ())/
			 fEuler[i].getZ());
      if(fTranslation[i].getX()!=0) 
	IterCri2 += fabs((fTranslation[i].getX()-OldTranslation2[i].getX())/
			 fTranslation[i].getX());
      if(fTranslation[i].getY()!=0) 
	IterCri2 += fabs((fTranslation[i].getY()-OldTranslation2[i].getY())/
			 fTranslation[i].getY());
      if(fTranslation[i].getZ()!=0) 
	IterCri2 += fabs((fTranslation[i].getZ()-OldTranslation2[i].getZ())/
			 fTranslation[i].getZ());
      
      if(AA_DEBUG==0){     
	cout << i << "IterCri2: " <<   IterCri2 << endl;
      }
    }
    IterCounter2++;
    if(IterCounter2>100) {
      cout << "WARNING in HMdcAligner3M :: finalize" << endl;
      cout << "Sector: " << sector << " ModuleA: " 
	   << modA <<  " ModuleB: " << modB << endl;
      cout << "More than 100 Double_t iterations without results!" <<endl;
      break;
    }    
    
    if(IterCri2>fIterCriteria){
      fillHistograms(2);  
      fitHistograms(2);
      
      if (*fout) {
	*fout << "Valid hits for alignment after cuts: " 
	      << fCountCut << endl << endl;    
	*fout << endl << endl;
      }
      if(AA_DEBUG>0) cout << "Valid hits for alignment after cuts: " 
			  << fCountCut << endl << endl;
    }     
    
  }while(IterCri2>fIterCriteria);

  //getting absolute parameters
  findAbsolutePosition(absRot,absVect);  
  if(*fout){    
    *fout << endl <<"Individual transformations in " 
	  << "minimization (init values): "  << endl;	  
    *fout << "Interpret smaller MDC index as last in the previous list" 
	  << endl << endl;
    for(Int_t i=0;i<fNumMods-1;i++){
      *fout << "Module" << i <<  endl;      
      for(Int_t j=0;j<9;j++) *fout << absRot[i](j) << "   " ;
      *fout << endl;
      for(Int_t j=0;j<3;j++) *fout << absVect[i](j) << "   " ;
      *fout << endl << endl;
    }
  }    
  if(AA_DEBUG>0){
    cout << endl << "Individual transformations in " 
	 << "minimization: "  << endl;  
    cout << "Interpret smaller MDC index as last in the previous list"  
	 << endl << endl;
    for(Int_t i=0;i<fNumMods-1;i++){
      cout << "Module" << i <<  endl;      
      absRot[i].print();
      absVect[i].print();	
    }
  }      
  
  //Recalculate all histograms for the new parameters!!!
  fillHistograms(2);
  fillHistograms(3,1);
  storeInFile();
  
  // close ascii file
  fout->close();
  delete fout;
  
  return kTRUE; 
}





void HMdcAligner3M::fillHistograms (Int_t index, Int_t select){
  //
  // Performs the graphical output from obtained parameters
  //   

  HMdcHit* hitA;      
  HMdcHit* hitB;      
  HMdcHit* hitC=NULL;      
  HMdcHit* hitBC=new HMdcHit; 
  HMdcHit* hitAB=new HMdcHit; 
  HMdcHit* hitAC=new HMdcHit; 

  HGeomVector projPoint;
  Float_t* projSlopes = new Float_t[2];
  Float_t* origSlopes = new Float_t[2];
  Float_t* SlopesAinA = new Float_t[2];
  Float_t* SlopesBinA = new Float_t[2];
  Float_t* SlopesCinA = new Float_t[2];

  HGeomRotation rotaux,rotaux2,rotaux3,rotaux4;
  HGeomVector transaux,transaux2,transaux3,transaux4;
  HGeomVector transf[4];  
  HGeomVector a,b,c,d;

  Float_t errorx[4]; 
  Float_t errory[4];
  Stat_t entries; 

  
  if (index==2){
    BvsCinCCS_X[2]->Reset();
    BvsCinCCS_Y[2]->Reset();
    BvsCinCCS_XSlope[2]->Reset();
    BvsCinCCS_YSlope[2]->Reset();
    AvsCinCCS_X[2]->Reset();
    AvsCinCCS_Y[2]->Reset();
    AvsCinCCS_XSlope[2]->Reset();
    AvsCinCCS_YSlope[2]->Reset();
    AvsCinCCS_Polar->Reset();
    BvsCinCCS_Polar->Reset();
    AvsCinCCS_Polar_Stripe1->Reset();
    AvsCinCCS_Polar_Stripe2->Reset();
    AvsCinCCS_Polar_Stripe3->Reset();
    AvsCinCCS_Polar_Stripe4->Reset();
    AvsCinCCS_Polar_Stripe5->Reset();
    BvsCinCCS_Polar_Stripe1->Reset();
    BvsCinCCS_Polar_Stripe2->Reset();
    BvsCinCCS_Polar_Stripe3->Reset();
    BvsCinCCS_Polar_Stripe4->Reset();
    BvsCinCCS_Polar_Stripe5->Reset();
    
    CvsBinBCS_X[2]->Reset();
    CvsBinBCS_Y[2]->Reset();
    
    CvsBinBCS_XSlope[2]->Reset();
    CvsBinBCS_YSlope[2]->Reset();
    AvsBinBCS_X[2]->Reset();
    AvsBinBCS_Y[2]->Reset();
    AvsBinBCS_XSlope[2]->Reset();
    AvsBinBCS_YSlope[2]->Reset();
    CvsAinACS_X[2]->Reset();
    CvsAinACS_Y[2]->Reset();
    CvsAinACS_XSlope[2]->Reset();
    CvsAinACS_YSlope[2]->Reset();
    BvsAinACS_X[2]->Reset();
    BvsAinACS_Y[2]->Reset();
    BvsAinACS_XSlope[2]->Reset();
    BvsAinACS_YSlope[2]->Reset();
    BCvsAinACS_X[2]->Reset();
    BCvsAinACS_Y[2]->Reset();
    BCvsACinACS_XSlope[2]->Reset();
    BCvsACinACS_YSlope[2]->Reset();
    ABvsCinCCS_X[2]->Reset();
    ABvsCinCCS_Y[2]->Reset();
    ABvsCinCCS_XSlope[2]->Reset();
    ABvsCinCCS_YSlope[2]->Reset();
    ACvsBinBCS_X[2]->Reset();
    ACvsBinBCS_Y[2]->Reset();
    ACvsBinBCS_XSlope[2]->Reset();
    ACvsBinBCS_YSlope[2]->Reset();
    
    DiffBCvsAinACS_XSlope[2]->Reset();
    DiffBCvsAinACS_YSlope[2]->Reset();
    DiffBCvsBinACS_XSlope[2]->Reset();
    DiffBCvsBinACS_YSlope[2]->Reset();
    DiffBCvsCinACS_XSlope[2]->Reset();
    DiffBCvsCinACS_YSlope[2]->Reset();
    DiffACvsAinACS_XSlope[2]->Reset();
    DiffACvsAinACS_YSlope[2]->Reset();
    DiffACvsBinACS_XSlope[2]->Reset();
    DiffACvsBinACS_YSlope[2]->Reset();
    DiffACvsCinACS_XSlope[2]->Reset();
    DiffACvsCinACS_YSlope[2]->Reset();
    DiffBCvsAinACS_XSlopeLow[2]->Reset();
    DiffBCvsAinACS_YSlopeLow[2]->Reset();
    DiffBCvsBinACS_XSlopeLow[2]->Reset();
    DiffBCvsBinACS_YSlopeLow[2]->Reset();
    DiffBCvsCinACS_XSlopeLow[2]->Reset();
    DiffBCvsCinACS_YSlopeLow[2]->Reset();
    DiffACvsAinACS_XSlopeLow[2]->Reset();
    DiffACvsAinACS_YSlopeLow[2]->Reset();
    DiffACvsBinACS_XSlopeLow[2]->Reset();
    DiffACvsBinACS_YSlopeLow[2]->Reset();
    DiffACvsCinACS_XSlopeLow[2]->Reset();
    DiffACvsCinACS_YSlopeLow[2]->Reset();
    DiffBCvsAinACS_XSlopeUp[2]->Reset();
    DiffBCvsAinACS_YSlopeUp[2]->Reset();
    DiffBCvsBinACS_XSlopeUp[2]->Reset();
    DiffBCvsBinACS_YSlopeUp[2]->Reset();
    DiffBCvsCinACS_XSlopeUp[2]->Reset();
    DiffBCvsCinACS_YSlopeUp[2]->Reset();
    DiffACvsAinACS_XSlopeUp[2]->Reset();
    DiffACvsAinACS_YSlopeUp[2]->Reset();
    DiffACvsBinACS_XSlopeUp[2]->Reset();
    DiffACvsBinACS_YSlopeUp[2]->Reset();
    DiffACvsCinACS_XSlopeUp[2]->Reset();
    DiffACvsCinACS_YSlopeUp[2]->Reset();
    DiffACvsAinACS_YSlope_Stripe1->Reset();
    DiffACvsAinACS_YSlope_Stripe2->Reset();
    DiffACvsAinACS_YSlope_Stripe3->Reset();
    DiffACvsAinACS_YSlope_Stripe4->Reset();
    DiffACvsAinACS_YSlope_Stripe5->Reset();
    DiffACvsBinACS_YSlope_Stripe1->Reset();
    DiffACvsBinACS_YSlope_Stripe2->Reset();
    DiffACvsBinACS_YSlope_Stripe3->Reset();
    DiffACvsBinACS_YSlope_Stripe4->Reset();
    DiffACvsBinACS_YSlope_Stripe5->Reset();
    DiffACvsCinACS_YSlope_Stripe1->Reset();
    DiffACvsCinACS_YSlope_Stripe2->Reset();
    DiffACvsCinACS_YSlope_Stripe3->Reset();
    DiffACvsCinACS_YSlope_Stripe4->Reset();
    DiffACvsCinACS_YSlope_Stripe5->Reset();
  }
  
  if(select != 0) entries = fAlignAllCut->GetEntries();
  else entries = fAlignAll->GetEntries();
  
  for (Int_t i=0;i<entries;i++) { 
    if(select != 0) fAlignAllCut->GetEntry(i);  
    else fAlignAll->GetEntry(i); 
    hitA = (HMdcHit*) fHits->At(0);
    hitB = (HMdcHit*) fHits->At(1);
    hitC = (HMdcHit*) fHits->At(2);
    
    //Constructing all possible projections
    //The histos represent (value of local hit - value of projected hit)
    
    //first projecting on MDC C
    projPoint = findProjPoint(hitB,fRotMat[0],fTranslation[0],projSlopes);
    BvsCinCCS_X[index]->Fill(hitC->getX() - projPoint.getX());
    BvsCinCCS_Y[index]->Fill(hitC->getY() - projPoint.getY());
    transformToSlopes(hitC,origSlopes);
    BvsCinCCS_XSlope[index]->Fill(origSlopes[0] - projSlopes[0]);
    BvsCinCCS_YSlope[index]->Fill(origSlopes[1] - projSlopes[1]);
    if(index==2){
      Float_t diffAngle = atanf(origSlopes[1]) - atanf(projSlopes[1]); 
      BvsCinCCS_Polar->Fill(diffAngle);
      if(origSlopes[1]>0.2) 
	BvsCinCCS_Polar_Stripe1->Fill(diffAngle);
      else if(origSlopes[1]>0.) 
	BvsCinCCS_Polar_Stripe2->Fill(diffAngle);
      else if(origSlopes[1]>-0.2) 
	BvsCinCCS_Polar_Stripe3->Fill(diffAngle);
      else if(origSlopes[1]>-0.4) 
	BvsCinCCS_Polar_Stripe4->Fill(diffAngle);
      else  
	BvsCinCCS_Polar_Stripe5->Fill(diffAngle);
    }    

    projPoint = findProjPoint(hitA,fRotMat[1],fTranslation[1],projSlopes);
    AvsCinCCS_X[index]->Fill(hitC->getX() - projPoint.getX());
    AvsCinCCS_Y[index]->Fill(hitC->getY() - projPoint.getY());
    AvsCinCCS_XSlope[index]->Fill(origSlopes[0] - projSlopes[0]);
    AvsCinCCS_YSlope[index]->Fill(origSlopes[1] - projSlopes[1]);
    
    if(index==2){
      Float_t diffAngle = atanf(origSlopes[1]) - atanf(projSlopes[1]); 
      AvsCinCCS_Polar->Fill(diffAngle);
      if(origSlopes[1]>0.2) 
	AvsCinCCS_Polar_Stripe1->Fill(diffAngle);
      else if(origSlopes[1]>0.) 
	AvsCinCCS_Polar_Stripe2->Fill(diffAngle);
      else if(origSlopes[1]>-0.2) 
	AvsCinCCS_Polar_Stripe3->Fill(diffAngle);
      else if(origSlopes[1]>-0.4) 
	AvsCinCCS_Polar_Stripe4->Fill(diffAngle);
      else  
	AvsCinCCS_Polar_Stripe5->Fill(diffAngle);
    }
    
    //then projecting on MDC B      
    rotaux = fRotMat[0].inverse();
    transaux = -(rotaux * fTranslation[0]);
    projPoint = findProjPoint(hitC,rotaux,transaux,projSlopes);      
    CvsBinBCS_X[index]->Fill(hitB->getX() - projPoint.getX());
    CvsBinBCS_Y[index]->Fill(hitB->getY() - projPoint.getY());
    transformToSlopes(hitB,origSlopes);
    CvsBinBCS_XSlope[index]->Fill(origSlopes[0] - projSlopes[0]);
    CvsBinBCS_YSlope[index]->Fill(origSlopes[1] - projSlopes[1]);
    
    rotaux2 = rotaux*fRotMat[1];  
    transaux2 = (rotaux)*(fTranslation[1]-fTranslation[0]);
    projPoint = findProjPoint(hitA,rotaux2,transaux2,projSlopes);   
    AvsBinBCS_X[index]->Fill(hitB->getX() - projPoint.getX());
    AvsBinBCS_Y[index]->Fill(hitB->getY() - projPoint.getY());
    AvsBinBCS_XSlope[index]->Fill(origSlopes[0] - projSlopes[0]);
    AvsBinBCS_YSlope[index]->Fill(origSlopes[1] - projSlopes[1]);
    
    
    //last, projecting on MDC A      
    rotaux3 = fRotMat[1].inverse();
    transaux3 = -(rotaux3 * fTranslation[1]);
    projPoint = findProjPoint(hitC,rotaux3,transaux3,projSlopes); 
    CvsAinACS_X[index]->Fill(hitA->getX() - projPoint.getX());
    CvsAinACS_Y[index]->Fill(hitA->getY() - projPoint.getY());
    transformToSlopes(hitA,origSlopes);
    CvsAinACS_XSlope[index]->Fill(origSlopes[0] - projSlopes[0]);
    CvsAinACS_YSlope[index]->Fill(origSlopes[1] - projSlopes[1]);
    //to be used later on
    SlopesCinA[0] = projSlopes[0];
    SlopesCinA[1] = projSlopes[1];
    
    rotaux4 = (rotaux3)*fRotMat[0];  
    transaux4 = (rotaux3)*(fTranslation[0]-fTranslation[1]);
    projPoint = findProjPoint(hitB,rotaux4,transaux4,projSlopes);      
    BvsAinACS_X[index]->Fill(hitA->getX() - projPoint.getX());
    BvsAinACS_Y[index]->Fill(hitA->getY() - projPoint.getY());
    BvsAinACS_XSlope[index]->Fill(origSlopes[0] - projSlopes[0]);
    BvsAinACS_YSlope[index]->Fill(origSlopes[1] - projSlopes[1]);   
    //to be used later on
    SlopesBinA[0] = projSlopes[0];
    SlopesBinA[1] = projSlopes[1];
    
    //new projection, merge of MDC C and MDC B on MDC A 
    //(testing MDC A resolution after alignment, for instance)
    mergeHits(hitC,hitB,fRotMat[0],fTranslation[0],hitBC);
    projPoint = findProjPoint(hitBC,rotaux3,transaux3,projSlopes);
    BCvsAinACS_X[index]->Fill(hitA->getX() - projPoint.getX());
    BCvsAinACS_Y[index]->Fill(hitA->getY() - projPoint.getY());
    
    //checking the slopes from two hits in modules C and A
    mergeHits(hitC,hitA,fRotMat[1],fTranslation[1],hitAC);
    projPoint = findProjPoint(hitAC,rotaux3,transaux3,origSlopes);
    BCvsACinACS_XSlope[index]->Fill(origSlopes[0] - projSlopes[0]);
    BCvsACinACS_YSlope[index]->Fill(origSlopes[1] - projSlopes[1]);   
    
    //checking the difference between slopes made from positions
    //and Hit slopes
    transformToSlopes(hitA,SlopesAinA);
    
    DiffBCvsAinACS_XSlope[index]->Fill(SlopesAinA[0] - projSlopes[0]);
    DiffBCvsAinACS_YSlope[index]->Fill(SlopesAinA[1] - projSlopes[1]);   
    DiffBCvsBinACS_XSlope[index]->Fill(SlopesBinA[0] - projSlopes[0]);
    DiffBCvsBinACS_YSlope[index]->Fill(SlopesBinA[1] - projSlopes[1]);   
    DiffBCvsCinACS_XSlope[index]->Fill(SlopesCinA[0] - projSlopes[0]);
    DiffBCvsCinACS_YSlope[index]->Fill(SlopesCinA[1] - projSlopes[1]);   
    DiffACvsAinACS_XSlope[index]->Fill(SlopesAinA[0] - origSlopes[0]);
    DiffACvsAinACS_YSlope[index]->Fill(SlopesAinA[1] - origSlopes[1]);   
    DiffACvsBinACS_XSlope[index]->Fill(SlopesBinA[0] - origSlopes[0]);
    DiffACvsBinACS_YSlope[index]->Fill(SlopesBinA[1] - origSlopes[1]);   
    DiffACvsCinACS_XSlope[index]->Fill(SlopesCinA[0] - origSlopes[0]);
    DiffACvsCinACS_YSlope[index]->Fill(SlopesCinA[1] - origSlopes[1]);   
    
    if(SlopesAinA[1]>0.2) 
      DiffACvsAinACS_YSlope_Stripe1->Fill(SlopesAinA[1] - origSlopes[1]);
    else if(SlopesAinA[1]>0)
      DiffACvsAinACS_YSlope_Stripe2->Fill(SlopesAinA[1] - origSlopes[1]);
    else if(SlopesAinA[1]>-0.2)
      DiffACvsAinACS_YSlope_Stripe3->Fill(SlopesAinA[1] - origSlopes[1]);
    else if(SlopesAinA[1]>-0.4)
      DiffACvsAinACS_YSlope_Stripe4->Fill(SlopesAinA[1] - origSlopes[1]);
    else
      DiffACvsAinACS_YSlope_Stripe5->Fill(SlopesAinA[1] - origSlopes[1]);
    
    if(SlopesBinA[1]>0.2) 
      DiffACvsBinACS_YSlope_Stripe1->Fill(SlopesBinA[1] - origSlopes[1]);
    else if(SlopesBinA[1]>0)
      DiffACvsBinACS_YSlope_Stripe2->Fill(SlopesBinA[1] - origSlopes[1]);
    else if(SlopesBinA[1]>-0.2)
      DiffACvsBinACS_YSlope_Stripe3->Fill(SlopesBinA[1] - origSlopes[1]);
    else if(SlopesBinA[1]>-0.4)
      DiffACvsBinACS_YSlope_Stripe4->Fill(SlopesBinA[1] - origSlopes[1]);
    else
      DiffACvsBinACS_YSlope_Stripe5->Fill(SlopesBinA[1] - origSlopes[1]);
    
    if(SlopesCinA[1]>0.2) 
      DiffACvsCinACS_YSlope_Stripe1->Fill(SlopesCinA[1] - origSlopes[1]);
    else if(SlopesCinA[1]>0)
      DiffACvsCinACS_YSlope_Stripe2->Fill(SlopesCinA[1] - origSlopes[1]);
    else if(SlopesCinA[1]>-0.2)
      DiffACvsCinACS_YSlope_Stripe3->Fill(SlopesCinA[1] - origSlopes[1]);
    else if(SlopesCinA[1]>-0.4)
      DiffACvsCinACS_YSlope_Stripe4->Fill(SlopesCinA[1] - origSlopes[1]);
    else
      DiffACvsCinACS_YSlope_Stripe5->Fill(SlopesCinA[1] - origSlopes[1]);
    
    if(hitC->getY()<0){
      DiffBCvsAinACS_XSlopeLow[index]->Fill(SlopesAinA[0] - projSlopes[0]);
      DiffBCvsAinACS_YSlopeLow[index]->Fill(SlopesAinA[1] - projSlopes[1]);
      DiffBCvsBinACS_XSlopeLow[index]->Fill(SlopesBinA[0] - projSlopes[0]);
      DiffBCvsBinACS_YSlopeLow[index]->Fill(SlopesBinA[1] - projSlopes[1]);
      DiffBCvsCinACS_XSlopeLow[index]->Fill(SlopesCinA[0] - projSlopes[0]);
      DiffBCvsCinACS_YSlopeLow[index]->Fill(SlopesCinA[1] - projSlopes[1]);
      DiffACvsAinACS_XSlopeLow[index]->Fill(SlopesAinA[0] - origSlopes[0]);
      DiffACvsAinACS_YSlopeLow[index]->Fill(SlopesAinA[1] - origSlopes[1]);
      DiffACvsBinACS_XSlopeLow[index]->Fill(SlopesBinA[0] - origSlopes[0]);
      DiffACvsBinACS_YSlopeLow[index]->Fill(SlopesBinA[1] - origSlopes[1]);
      DiffACvsCinACS_XSlopeLow[index]->Fill(SlopesCinA[0] - origSlopes[0]);
      DiffACvsCinACS_YSlopeLow[index]->Fill(SlopesCinA[1] - origSlopes[1]);
    }
    else{
      DiffBCvsAinACS_XSlopeUp[index]->Fill(SlopesAinA[0] - projSlopes[0]);
      DiffBCvsAinACS_YSlopeUp[index]->Fill(SlopesAinA[1] - projSlopes[1]);
      DiffBCvsBinACS_XSlopeUp[index]->Fill(SlopesBinA[0] - projSlopes[0]);
      DiffBCvsBinACS_YSlopeUp[index]->Fill(SlopesBinA[1] - projSlopes[1]);  
      DiffBCvsCinACS_XSlopeUp[index]->Fill(SlopesCinA[0] - projSlopes[0]);
      DiffBCvsCinACS_YSlopeUp[index]->Fill(SlopesCinA[1] - projSlopes[1]);
      DiffACvsAinACS_XSlopeUp[index]->Fill(SlopesAinA[0] - origSlopes[0]);
      DiffACvsAinACS_YSlopeUp[index]->Fill(SlopesAinA[1] - origSlopes[1]);
      DiffACvsBinACS_XSlopeUp[index]->Fill(SlopesBinA[0] - origSlopes[0]);
      DiffACvsBinACS_YSlopeUp[index]->Fill(SlopesBinA[1] - origSlopes[1]);
      DiffACvsCinACS_XSlopeUp[index]->Fill(SlopesCinA[0] - origSlopes[0]);
      DiffACvsCinACS_YSlopeUp[index]->Fill(SlopesCinA[1] - origSlopes[1]);
    }
    
    //new projection, merge of MDC A and MDC B on MDC C 
    mergeHits(hitB,hitA,rotaux2,transaux2,hitAB);
    projPoint = findProjPoint(hitAB,fRotMat[0],fTranslation[0],projSlopes);
    transformToSlopes(hitC,origSlopes);
    ABvsCinCCS_X[index]->Fill(hitC->getX() - projPoint.getX());
    ABvsCinCCS_Y[index]->Fill(hitC->getY() - projPoint.getY());
    ABvsCinCCS_XSlope[index]->Fill(origSlopes[0] - projSlopes[0]);
    ABvsCinCCS_YSlope[index]->Fill(origSlopes[1] - projSlopes[1]);   
    
    //new projection, merge of MDC A and MDC C on MDC B
    mergeHits(hitC,hitA,fRotMat[1],fTranslation[1],hitAC);
    projPoint = findProjPoint(hitAC,rotaux,transaux,projSlopes);
    transformToSlopes(hitB,origSlopes);
    ACvsBinBCS_X[index]->Fill(hitB->getX() - projPoint.getX());
    ACvsBinBCS_Y[index]->Fill(hitB->getY() - projPoint.getY());
    ACvsBinBCS_XSlope[index]->Fill(origSlopes[0] - projSlopes[0]);
    ACvsBinBCS_YSlope[index]->Fill(origSlopes[1] - projSlopes[1]);   

    if(fMin!=105){
      c.setX(hitC->getX());
      c.setY(hitC->getY());
      c.setZ(0.);      
      b.setX(hitB->getX());
      b.setY(hitB->getY());
      b.setZ(0.);      
      a.setX(hitA->getX());
      a.setY(hitA->getY());
      a.setZ(0.);
      
      //ERRORS      
      if(fUseUnitErrors==kFALSE && fUseModErrors==kFALSE ){   
	errorx[0] = (hitA->getErrX()!=0)? hitA->getErrX() : 0.2;
	errorx[1] = (hitB->getErrX()!=0)? hitB->getErrX() : 0.2;      
	errorx[2] = (hitC->getErrX()!=0)? hitC->getErrX() : 0.2;   
	errory[0] = (hitA->getErrY()!=0)? hitA->getErrY() : 0.1;
	errory[1] = (hitB->getErrY()!=0)? hitB->getErrY() : 0.1;      
	errory[2] = (hitC->getErrY()!=0)? hitC->getErrY() : 0.1;   
      }    
      else if(fUseModErrors==kTRUE){
	errorx[0]=fPosError[0];errory[0]=fPosError[1];    
	errorx[1]=fPosError[2];errory[1]=fPosError[3];    
	errorx[2]=fPosError[4];errory[2]=fPosError[5];     
      }
      else 
	for(Int_t i=0;i<fNumMods;i++){
	  errorx[i]=1.0;
	  errory[i]=1.0;      
	}
      
      if(AA_DEBUG>3){
	for(Int_t i=0;i<fNumMods;i++){
	  cout << "errorx[" << i <<"] = " << errorx[i] << endl;	
	  cout << "errory[" << i <<"] = " << errory[i]  << endl;
	}
      }
      
      if(fNumMods>3){
	transf[3] = d;
	transf[2] = fRotMat[0] * c + fTranslation[0];
	transf[1] = fRotMat[1] * b + fTranslation[1];
	transf[0] = fRotMat[2] * a + fTranslation[2];
      }
      else{            
	transf[2] = c;
	transf[1] = fRotMat[0] * b + fTranslation[0];
	transf[0] = fRotMat[1] * a + fTranslation[1];
      }
      
      if(AA_DEBUG>3){
	for(Int_t i=0;i<fNumMods;i++){
	  cout << "transf[" << i <<"]   ";
	  transf[i].print();
	}
      }
      
      Float_t ax=0.0, ay=0.0, bx=0.0, by=0.0;     
      Float_t sigax=0.0, sigay=0.0, sigbx=0.0, sigby=0.0; 
      Float_t Xwt, Xt, Xsxoss, Xsx=0.0, Xsy=0.0, Xst2=0.0, Xss=0.0;
      Float_t Ywt, Yt, Ysxoss, Ysx=0.0, Ysy=0.0, Yst2=0.0, Yss=0.0;
      Float_t XChi2=0.0, YChi2=0.0, chipar=0.0;      
      for(Int_t i=0;i<fNumMods;i++){
	//Plane XZ
	Xwt = 1.0/(errorx[i]*errorx[i]);
	Xss += Xwt;
	Xsx += transf[i].getZ()*Xwt;     
	Xsy += transf[i].getX()*Xwt;      
	//Plane YZ
	Ywt = 1.0/(errory[i]*errory[i]);
	Yss += Ywt;
	Ysx += transf[i].getZ()*Ywt;     
	Ysy += transf[i].getY()*Ywt;
	
	if(AA_DEBUG>3) cout << "Xwt=" << Xwt << " Xss=" << Xss 
			    << " Xsx=" <<  Xsx << " Xsy=" << Xsy
			    << " Ywt=" << Ywt << " Yss=" << Yss 
			    << " Ysx=" <<  Ysx << " Ysy=" << Ysy << endl;
      }
      
      Xsxoss = Xsx/Xss;    
      Ysxoss = Ysx/Yss;
      
      if(AA_DEBUG>3) cout << "Xsxoss=" << Xsxoss << "  Ysxoss=" 
			  << Ysxoss << endl;
      
      for(Int_t i=0;i<fNumMods;i++){
	//Plane XZ
	Xt = (transf[i].getZ()-Xsxoss)/errorx[i];
	Xst2 += Xt * Xt;
	bx += Xt * transf[i].getX()/errorx[i];            
	//Plane YZ
	Yt = (transf[i].getZ()-Ysxoss)/errory[i];
	Yst2 += Yt * Yt;
	by += Yt * transf[i].getY()/errory[i];
	
	if(AA_DEBUG>3) cout << "Xt=" << Xt << " Xst2=" << Xst2 
			    << " bx (partial)=" << bx << "Yt=" << Yt 
			    << " Yst2=" << Yst2 
			    << " by (partial)=" << by << endl; 
      } 
      // X = ax + bx Z
      // Y = ay + by Z  
      bx /= Xst2;
      ax = (Xsy-(Xsx*bx))/Xss;
      by /= Yst2;
      ay = (Ysy-(Ysx*by))/Yss;
      
      if(AA_DEBUG>3) cout << "bx=" << bx << " ax=" << ax
			  << "by=" << by << " ay=" << ay << endl;
      
      sigax = sqrt((1.0+Xsx*Xsx/(Xss*Xst2))/Xss);
      sigbx = sqrt(1.0/Xst2);
      sigay = sqrt((1.0+Ysx*Ysx/(Yss*Yst2))/Yss);
      sigby = sqrt(1.0/Yst2);
      
      if(AA_DEBUG>3) cout << "sigbx=" << sigbx << " sigax=" << sigax
			  << "sigby=" << sigby << " sigay=" << sigay << endl;
      
      //Aqui falta evaluar la calidad del ajuste o bien encontrar
      //cuales son los errores que se esperan en los datos para un buen ajuste
      for(Int_t i=0;i<fNumMods;i++){
	//Plane XZ
	chipar = (transf[i].getX()-ax-bx*transf[i].getZ())/errorx[i];
	XChi2 += chipar*chipar;
	//Plane YZ
	chipar = (transf[i].getY()-ay-by*transf[i].getZ())/errory[i];
	YChi2 += chipar*chipar;
	if(AA_DEBUG>3) cout << "XChi2=" << XChi2 << " YChi2=" << YChi2 << endl;
      } 
      
      XChi2Hist[index]->Fill(XChi2);
      YChi2Hist[index]->Fill(YChi2);
      TotalChi2[index]->Fill(XChi2+YChi2);
      
      // Also can be defined by a vector V and a point P:
      // V=(bx,by,1) P=(ax,ay,0)
      // Let us calculate the square distance from the straigth 
      // line to the points (fit residuals) 
      Float_t part1=0.0,part2=0.0,part3=0.0,sdistance=0.0,sdist=0.0;
      for(Int_t i=0;i<fNumMods;i++){
	part1 = ( (transf[i].getX()-ax)*by ) - ( (transf[i].getY()-ay)*bx );
	part2 = ( (transf[i].getY()-ay)    ) - ( (transf[i].getZ()   )*by );
	part3 = ( (transf[i].getZ()   )*bx ) - ( (transf[i].getX()-ax)    );
	sdist = (part1*part1 + part2*part2 + part3*part3)/(1+bx*bx+by*by);
	sdistance += sdist;
	if(AA_DEBUG>3) 
	  cout << "Square Distance point " << i << " - line: " 
	       << sdistance << endl;
	if(i==0)SqrDistToA[index]->Fill(sdist);	  
	if(i==1)SqrDistToB[index]->Fill(sdist);	  
	if(i==2)SqrDistToC[index]->Fill(sdist);	  
	if(i==3)SqrDistToD[index]->Fill(sdist);	
      }
      SqrDist[index]->Fill(sdistance);
    }
    
  } // end of for (Int_t i=0; i<nentries;i++) {
  if(hitBC!=0) delete hitBC; 
  if(hitAB!=0) delete hitAB; 
  if(hitAC!=0) delete hitAC;
  if(projSlopes!=0) delete[] projSlopes;
  if(origSlopes!=0) delete[] origSlopes;
}





void HMdcAligner3M::storeInFile(void)
{  
  //
  // Stores all histos and tree in the Root file
  //

  //Esto provocaba los errores!!
  //TFile *fOutRoot = new TFile(fNameRoot,"RECREATE");

  Int_t sector = fLoc[0];
  Int_t modA = fLoc[1];
  Int_t modB = fLoc[2]; 
  Int_t modC=-1;
  Int_t modD=-1;
  if(fNumMods>2) modC = fLoc[3];
  if(fNumMods>3) modD = fLoc[4];

  if (fMode>0){
    sector = fLoc[0]*10;
    modA = fLoc[1];
    sector += fLoc[2]; //sector = sectorA*10+sectorB
    modB = fLoc[3];      
    modC = fLoc[4];       
  }
  
  fAlignAll->Write();
  fAlignAllCut->Write();

  //  const Char_t* oldDirName = gDirectory->GetPath();
  static Char_t newDirName[255];
  if(fNumMods == 2)  sprintf(newDirName,"%s%s%i%s%i%s%i","Aligner3M_",
			     "S_",sector,"_ModA_",modA,"_ModB_",modB);
  if(fNumMods == 3)  sprintf(newDirName,"%s%s%i%s%i%s%i%s%i","Aligner3M_",
			     "S_",sector,"_ModA_",modA,"_ModB_",modB,
			     "_ModC_",modC);
  if(fNumMods == 4)  sprintf(newDirName,"%s%s%i%s%i%s%i%s%i%s%i",
			     "Aligner3M_","S_",sector,"_ModA_",modA,
			     "_ModB_",modB,"_ModC_",modC,"_ModD_",modD);
  if(fHistoOff!=kTRUE) {
    fOutRoot->cd(newDirName);
    
    //Enter in the file the HMdcGeomRotation HMdcGeomVector
    for(Int_t i=0;i<fNumMods-1;i++){
      fRotMat[i].Write();
      fTranslation[i].Write();
    }  

    //storing the graph (it is not automatic!)
    graphCont->Write();

    fOutRoot->cd();
  }
  fRecCount--;
  if(!fRecCount) {
    //    cout << "Antes de Write()" << endl;
    fOutRoot->Write();
    fOutRoot->Close();
  }
} 





void HMdcAligner3M::fillRotMatrix(Int_t ind, Float_t prim,
				  Float_t segu, Float_t terc){ 
  // 
  // Fill a matrix using the Euler angles of the relative transformation 
  // 
  /*OLD 
    fRotMat[0][0]=(cos(prim) * cos(segu) * cos(terc)) - (sin(prim) * sin(terc)); 
    fRotMat[1][0]=( - cos(prim) * cos(segu) * sin(terc)) - (sin(prim) * cos(terc)); 
    fRotMat[2][0]=(cos(prim) * sin(segu)); 
    fRotMat[0][1]=(sin(prim) * cos(segu) * cos(terc)) + (cos(prim) * sin(terc)); 
    fRotMat[1][1]=( - sin(prim) * cos(segu) * sin(terc)) + (cos(prim) * cos(terc)); 
    fRotMat[2][1]=(sin(prim) * sin(segu)); 
    fRotMat[0][2]=( - sin(segu) * cos(terc)); 
    fRotMat[1][2]=(sin(segu) * sin(terc)); 
    fRotMat[2][2]=(cos(segu)); 
  */

  //ATT! Correspondencia entre HGeomRotation y la antigua transf. arriba
  //
  //  rot(0) rot(1) rot(2)     fRotMat[0][0] fRotMat[1][0] fRotMat[2][0]
  //  rot(3) rot(4) rot(5)  =  fRotMat[0][1] fRotMat[1][1] fRotMat[2][1]
  //  rot(6) rot(7) rot(8)     fRotMat[0][2] fRotMat[1][2] fRotMat[2][2]
  const Double_t rad2deg = 57.29577951308;
  fRotMat[ind].setEulerAngles(prim*rad2deg,segu*rad2deg,terc*rad2deg);
} 





void HMdcAligner3M::fillTranslation(Int_t ind,Float_t x,
				    Float_t y, Float_t z){ 
  //
  // Translation from MDC A to MDC B
  //
 
  fTranslation[ind].setX(x); 
  fTranslation[ind].setY(y);
  fTranslation[ind].setZ(z); 
}





void HMdcAligner3M::setEulerAngles(Int_t ind,Float_t f,
				   Float_t s, Float_t t){ 
  //
  // Euler angles in transformation from MDC A to MDC B
  //
 
  fEuler[ind].setX(f); 
  fEuler[ind].setY(s); 
  fEuler[ind].setZ(t); 
} 





void HMdcAligner3M::setSearchLimits(Float_t x, Float_t y, Float_t s){
  //
  // Limits of the square defined in the search procedure
  //
 
  fXArea = x; 
  fYArea = y; 
  fSArea = s; 
} 





void HMdcAligner3M::setIterCriteria(Float_t cri){ 
  //
  // Set the criteria for iteration in the minimization (see finalize())
  //
 
  fIterCriteria = cri; 
}





void HMdcAligner3M::setWeights(Float_t w0,Float_t w1,Float_t w2,Float_t w3){ 
  //
  // Set the weights in the fcn()
  //
 
  fWeights[0]= w0; 
  fWeights[1]= w1; 
  fWeights[2]= w2; 
  fWeights[3]= w3; 

}





void HMdcAligner3M::setModErrors(Float_t errXModA,Float_t errYModA,
				 Float_t errXModB,Float_t errYModB,
				 Float_t errXModC,Float_t errYModC){  
  //
  // Set the module errors in the fcn()
  //
  fUseModErrors=kTRUE;
  fPosError[0]=errXModA;fPosError[1]=errYModA;
  fPosError[2]=errXModB;fPosError[3]=errYModB;
  fPosError[4]=errXModC;fPosError[5]=errYModC;
}





void HMdcAligner3M::setSteps(Float_t s0,Float_t s1,Float_t s2,
			     Float_t s3, Float_t s4, Float_t s5,
			     Float_t s6, Float_t s7, Float_t s8,
			     Float_t s9, Float_t s10, Float_t s11){
  //
  // Set the steps in the minimization
  //
 
  fSteps[0]= s0; fSteps[1]= s1; fSteps[2]= s2; 
  fSteps[3]= s3; fSteps[4]= s4; fSteps[5]= s5;
  fSteps[6]= s6; fSteps[7]= s7; fSteps[8]= s8; 
  fSteps[9]= s9; fSteps[10]= s10; fSteps[11]= s11;

  if(AA_DEBUG>3) cout << "Steps in the minimization adjusted to " 
		      << s0 << ", " << s1 << ", " << s2 << ", " 
		      << s3 << ", " << s4 << ", " << s5 << ", " 
		      << s6 << ", " << s7 << ", " << s8 << ", " 
		      << s9 << ", " << s10 << ", " << s11 << endl;
}





void HMdcAligner3M::setLimits(Float_t l0,Float_t l1,Float_t l2,
			      Float_t l3, Float_t l4, Float_t l5,
			      Float_t l6, Float_t l7, Float_t l8,
			      Float_t l9, Float_t l10, Float_t l11){
  //
  // Set the criteria for iteration in the minimization (see finalize())
  //
 
  fLimits[0]= l0; fLimits[1]= l1; fLimits[2]= l2; 
  fLimits[3]= l3; fLimits[4]= l4; fLimits[5]= l5; 
  fLimits[6]= l6; fLimits[7]= l7; fLimits[8]= l8; 
  fLimits[9]= l9; fLimits[10]= l10; fLimits[11]= l11; 

  if(AA_DEBUG>3) cout << "Limits in the minimization adjusted to " 
		      << l0 << ", " << l1 << ", " << l2 << ", " 
		      << l3 << ", " << l4 << ", " << l5 << ", " 
		      << l6 << ", " << l7 << ", " << l8 << ", " 
		      << l9 << ", " << l10 << ", " << l11 << endl;
}





void HMdcAligner3M::minfit(Int_t fix, HGeomVector* fE, HGeomVector* fT){ 
  //
  // Minuit menu
  // Argument fix correspon to fFix value (see setFix())
  // Other arguments are init values for the parameters!
  //

  Double_t args[2]={0,0}; 
  Int_t err = 0; 
  Float_t* limitL;
  Float_t* limitH;
  limitL = new Float_t[12];
  limitH = new Float_t[12];
  Double_t parresult[12];
  Double_t oldparresult[12];

  //This depends on MDCA and MDCB 
  //In this case is only for three modules, and there is no risk!!
  Double_t start_val[]={fE[0].getX(),fE[0].getY(),fE[0].getZ(),
			fT[0].getX(),fT[0].getY(),fT[0].getZ(),
			fE[1].getX(),fE[1].getY(),fE[1].getZ(),
			fT[1].getX(),fT[1].getY(),fT[1].getZ()};
  
  //setting limits
  for(Int_t i=0;i<12;i++){
    if(fLimits[i]==0){
      limitL[i]=0;
      limitH[i]=0;
    }
    else { 
      limitL[i]=start_val[i]-fLimits[i];      
      limitH[i]=start_val[i]+fLimits[i];
      cout << " LIMITS IN THE MINIMIZATION " << endl;
      cout << " (from 0 to 11) Par " << i << " between " 
	   << limitL[i] << " and " << limitH[i] << endl;
    }
  }
  
  TMinuit *minuit=new TMinuit((fNumMods-1)*6);
   
  //setting the minimization function (only fcnalign3 for three modules...)
  if(fMin==105){
    fillHitArrayForMinimization();
    minuit->SetFCN(fcnalign3fast); 
  }
  else minuit->SetFCN(fcnalign3); 
  minuit->SetObjectFit(this);
  
  if(AA_DEBUG>0){
    cout << "HMdcAligner3M::minfit()" <<endl;    
    cout << "Start Values for initialization: "; 
    for(Int_t i=0;i<(fNumMods-1)*6;i++){
      cout << start_val[i] << "," ;
    }
    cout << endl;
  }

  Char_t pname[10];  
  for(Int_t i=0;i<(fNumMods-1)*6;i++){
    sprintf(pname,"%s%i","par",i);
    minuit->mnparm(i,pname,start_val[i],fSteps[i],limitL[i],limitH[i],err); 
    oldparresult[i] = start_val[i];
    if(err>0) cout << "ERROR IN MINUIT INITIALIZATION" << endl;
  }
  
  if(fix!=4096){
    //FIXING parameters
    Int_t bit=1;
    for(Int_t i=0;i<(fNumMods-1)*6;i++){
      if(fix & bit)
	minuit->FixParameter(i);
      bit<<=1;
    }
  }
  
  //some method dependent fixed parameters
  if(fMin==4) {    //fixing angles
    for(Int_t i=0;i<(fNumMods-1);i++){
      minuit->FixParameter(6*i+0);     
      minuit->FixParameter(6*i+1); 
      minuit->FixParameter(6*i+2); 
    }
  }
  if(fMin==1||fMin==3) {    //fixing translations
    for(Int_t i=0;i<(fNumMods-1);i++){
      minuit->FixParameter(6*i+3);     
      minuit->FixParameter(6*i+4); 
      minuit->FixParameter(6*i+5); 
    }
  }
  
  //To remove all printout
  if(AA_DEBUG<3){ 
    //minuit->SetPrintLevel(-1);
    minuit->SetPrintLevel(1);
  }
  
  Double_t aDummy=0;
  Int_t otherDummy=0;
  //args is the array of the numerical arguments of the command 
  //the third field is the number of arguments especified 
  //For MIGRAD arguments are maxcalls and tolerance, both optionals 
  if(fMin==105 && fix==4096){
    Int_t IterCounter =0;
    Float_t IterCri;    
    static Double_t pfix=0;
    
    ofstream *fout=NULL;
    if (fNameAscii) fout = new ofstream(fNameAscii.Data(), ios::app);
    if (*fout)
      *fout << endl << "Iterative fixing and releasing of two params in (105)";
    
    do{
      if (*fout) *fout<<endl<<endl<<" Iteration number " <<IterCounter<<endl;
      IterCri = 0;
      for(Int_t con=0;con<12;con++) oldparresult[con] = parresult[con];
      
      for(Int_t con=0;con<12;con++) {
	pfix = con+1;
	minuit->mnexcm("RELEASE",&pfix,1,err);     
      }
      pfix = 2;
      minuit->mnexcm("FIX",&pfix,1,err); 
      pfix = 8;
      minuit->mnexcm("FIX",&pfix,1,err); 
      minuit->mnexcm("MIGRAD",args,0,err); 
      minuit->mnstat(fFunctionMin,aDummy,aDummy,
		     otherDummy,otherDummy,otherDummy);
      for(Int_t i=0;i<(fNumMods-1)*6;i++) 
	minuit->GetParameter(i,parresult[i],fError[i]);
      
      if (*fout){
	*fout << "Fixing the angles: "<< endl;
	for(Int_t con=0;con<12;con++) 
	  *fout << parresult[con] << "+-" <<  fError[con] << ", ";
	*fout << "Function value:  " <<  fFunctionMin 
	      << "  from  "; 
	if(fUseCut) *fout << fCountCut << "  combinations." << endl;
	else *fout <<  fCount << "  combinations." << endl;
	*fout << endl;	
      }
      
      for(Int_t con=0;con<12;con++) {
	pfix = con+1;
	minuit->mnexcm("FIX",&pfix,1,err);     
      }
      pfix = 2;
      minuit->mnexcm("RELEASE",&pfix,1,err); 
      pfix = 8;
      minuit->mnexcm("RELEASE",&pfix,1,err); 
      minuit->mnexcm("MIGRAD",args,0,err); 
      minuit->mnstat(fFunctionMin,aDummy,aDummy,
		     otherDummy,otherDummy,otherDummy);      
      for(Int_t i=0;i<(fNumMods-1)*6;i++) 
	minuit->GetParameter(i,parresult[i],fError[i]);

      if (*fout){
	*fout << endl << "Fixing all but the angles: "<< endl;
	for(Int_t con=0;con<12;con++) 
	  *fout << parresult[con] << "+-" <<  fError[con] << ", ";
	*fout << "Function value:  " <<  fFunctionMin 
	      << "  from  "; 
	if(fUseCut) *fout << fCountCut << "  combinations." << endl;
	else *fout <<  fCount << "  combinations." << endl;
	*fout << endl;	
      }
      
      for(Int_t i=0;i<12;i++){    
	if(parresult[i]!=0) 
	  IterCri += fabs((parresult[i]-oldparresult[i])/parresult[i]);
      }
      
      IterCounter++;
      if(IterCounter>10) {
	cout << "WARNING in HMdcAligner3M :: minfit -> Method (105)" << endl;
	cout << "More than 10 iterations without results!" <<endl;
	break;
      }
    }while(IterCri>fIterCriteria);
    
  }
  else if(fMin == 106){
    minuit->mnexcm("MINOS",args,0,err); 
    if(err>0) cout << "ERROR IN MINUIT INITIALIZATION" << endl;
  }
  else if(fMin == 777){
    //minuit->mnexcm("MIGRAD",args,0,err); 
    graphCont = (TGraph*)minuit->Contour(100,1,7);
  }
  else{
    minuit->mnexcm("MIGRAD",args,0,err); 
    //minuit->mnexcm("SIMPLEX",args,0,err); 
    //minuit->mnexcm("MINOS",args,0,err); 
    if(err>0) cout << "ERROR IN MINUIT INITIALIZATION" << endl;
  }
  
  //getting the function minimum after each minimization step
  minuit->mnstat(fFunctionMin,aDummy,aDummy,otherDummy,otherDummy,otherDummy);
  for(Int_t i=0;i<(fNumMods-1)*6;i++){
    minuit->GetParameter(i,parresult[i],fError[i]);
  }
  
  for(Int_t i=0;i<fNumMods-1;i++){
    fEuler[i].setX(parresult[i*6]);
    fEuler[i].setY(parresult[i*6+1]);
    fEuler[i].setZ(parresult[i*6+2]);
    fTranslation[i].setX(parresult[i*6+3]);
    fTranslation[i].setY(parresult[i*6+4]);
    fTranslation[i].setZ(parresult[i*6+5]);
  }
  
  if (err != 0) cout << endl <<"MINIMIZATION EXITED WITH ERROR " 
		     << err << endl;
  
  if(limitL!=0)delete []limitL;
  if(limitH!=0)delete []limitH;
  
}





void  HMdcAligner3M::fillHitArrayForMinimization(void){
  //
  // For fast minimization, a Hit array with only relevant information
  // (x,y coordinates) for the Hits in accepted track combinations
  //
  
  HMdcHit* hitA;      
  HMdcHit* hitB;      
  HMdcHit* hitC;  
  
  Stat_t entries; 
  
  if(select != 0) entries = fAlignAllCut->GetEntries();
  else entries = fAlignAll->GetEntries();

  fHitsForMin = new Float_t[(Int_t)(3*2*entries)];

  for (Int_t i=0;i<entries;i++) { 
    if(select != 0) fAlignAllCut->GetEntry(i);  
    else fAlignAll->GetEntry(i); 
    hitA = (HMdcHit*) fHits->At(0);
    hitB = (HMdcHit*) fHits->At(1);
    hitC = (HMdcHit*) fHits->At(2);
    fHitsForMin[6*i]= hitA->getX();
    fHitsForMin[6*i+1]= hitA->getY();
    fHitsForMin[6*i+2]= hitB->getX();
    fHitsForMin[6*i+3]= hitB->getY();
    fHitsForMin[6*i+4]= hitC->getX();
    fHitsForMin[6*i+5]= hitC->getY();
  }

  finalEntries = entries;
}





Stat_t HMdcAligner3M::getHitArrayForMinimization(Float_t** buf){
  //
  // For fast minimization, a Hit array with only relevant information
  // (x,y coordinates) for the Hits in accepted track combinations
  //  

  *buf = fHitsForMin;
  return finalEntries;
}





void fcnalign3fast(Int_t &npar, Double_t *gin, Double_t &f, 
		   Double_t *par, Int_t iflag){ 
  //
  // This function contains the functional to minimize
  // Use this if three  MDCs are present in the sector
  //
  Double_t chisq = 0.; 
  HGeomRotation rotmat[2];

  Float_t errorx[3]={1.,1.,1.};
  Float_t errory[3]={1.,1.,1.};

  HMdcAligner3M* pAlign = (HMdcAligner3M*)(gMinuit->GetObjectFit());
  //  TClonesArray* theHits;
  pAlign = (HMdcAligner3M*)(gMinuit->GetObjectFit());
  //TTree* pData= pAlign->getTree();
  //Stat_t entries = pData->GetEntries();  

  Float_t* buffer = 0;
  Stat_t entries = pAlign->getHitArrayForMinimization(&buffer);

  Float_t* errors;
  errors = new Float_t[6];
  errors = pAlign->getErrors();
  //Bool_t UseUnitErrors = pAlign->getUseUnitErrors();
  Bool_t UseModErrors = pAlign->getUseModErrors(); //theHits = pAlign->getHits();
  //data = pData->GetArgs();
  //filling from the ntuple
  //hitA = (HMdcHit*) theHits->At(0);      
  //hitB = (HMdcHit*) theHits->At(1);      
  //hitC = (HMdcHit*) theHits->At(2);

  //filling matrix and vectors from parameters with the
  //same notation as in execute() and setGeomAuxPar() funcions
  //(here rotmat is equivalent to fRotMat ...)
  rotmat[0].setEulerAngles(par[0]*180./TMath::Pi(),
			   par[1]*180./TMath::Pi(),
			   par[2]*180./TMath::Pi()); 
  
  rotmat[1].setEulerAngles(par[6]*180./TMath::Pi(),
			   par[7]*180./TMath::Pi(),
			   par[8]*180./TMath::Pi()); 
  /* 
     cout << "HMdcAligner3M::fcnalign()   Parameters: " 
     << par[0] << "," << par[1] << "," <<  par[2] << "," 
     << par[3] << ","  << par[4] << ","  << par[5] << ","
     << par[6] << "," << par[7] << "," <<  par[8] << "," 
     << par[9] << ","  << par[10] << ","  << par[11] ;
     cout << endl;
  */
  
  //HMdcHit *hitA;
  //HMdcHit *hitB;
  //HMdcHit *hitC;

  if(UseModErrors==kTRUE){
    errorx[0]=errors[0];errory[0]=errors[1];    
    errorx[1]=errors[2];errory[1]=errors[3];    
    errorx[2]=errors[4];errory[2]=errors[5];    
  }
  else 
    cout << "ATT! The fast version of the functional only accepts global Hit errors..."
	 <<endl;	

  //Accelerating as maximum!!! 
  Double_t rot0_0 = rotmat[0](0);
  Double_t rot0_1 = rotmat[0](1);
  //Double_t rot0_2 = rotmat[0](2);
  Double_t rot0_3 = rotmat[0](3);
  Double_t rot0_4 = rotmat[0](4);
  //Double_t rot0_5 = rotmat[0](5);
  Double_t rot0_6 = rotmat[0](6);     
  Double_t rot0_7 = rotmat[0](7);
  //Double_t rot0_8 = rotmat[0](8);

  Double_t rot1_0 = rotmat[1](0);
  Double_t rot1_1 = rotmat[1](1);
  //Double_t rot1_2 = rotmat[1](2);
  Double_t rot1_3 = rotmat[1](3);
  Double_t rot1_4 = rotmat[1](4);
  //Double_t rot1_5 = rotmat[1](5);
  Double_t rot1_6 = rotmat[1](6);     
  Double_t rot1_7 = rotmat[1](7);
  //Double_t rot1_8 = rotmat[1](8);

  Double_t trans0_0 = par[3];
  Double_t trans0_1 = par[4];
  Double_t trans0_2 = par[5]; 

  Double_t trans1_0 = par[9];
  Double_t trans1_1 = par[10];
  Double_t trans1_2 = par[11]; 

  Float_t a_X = 0;
  Float_t a_Y = 0;
  Float_t b_X = 0;
  Float_t b_Y = 0;
  Float_t c_X = 0;
  Float_t c_Y = 0;

  Double_t vecA_X = 0;
  Double_t vecA_Y = 0;
  Double_t vecA_Z = 0;
  Double_t vecB_X = 0;
  Double_t vecB_Y = 0;
  Double_t vecB_Z = 0;

  Double_t vecAxVecB_X =0;
  Double_t vecAxVecB_Y =0;
  Double_t vecAxVecB_Z =0;
  Double_t mod2VecA = 0;
  Double_t mod2VecB = 0;
  Double_t mod2VecAxVecB = 0;
  Double_t commonPartial = 0; 
  Double_t mod2VecAB = 0;

  Double_t sincua = 0;
  Double_t partialx_a = 0;
  Double_t partialx_b = 0;
  Double_t partialx_c = 0;
  Double_t partialy_a = 0;
  Double_t partialy_b = 0;
  Double_t partialy_c = 0;
  Double_t varianzainsincua = 0;

  Int_t reg = 0;

  //lo siguiente a intentar es una funcion de HMdcAligner que se llama 
  //aqui que devuelve en su return el numero de elementos de un vector 
  //(nentries) y al que se le pase como argumento un array gigante que 
  //se llena en la funcion. Elimino las funciones de acceso al arbol y
  //de lleando de Hits a partir del TClones por un acceso directo a 
  //elementos de un array previamente llenado en un paso para todos los
  //Hits! La llamada a la funcion que rellena un buen trozo de memoria
  //con toda esta informacion se realiza en minfit, justo antes de la
  //llamada a func34fast (y en el mismo if())

  //loop on hits 
  for (Int_t i=0;i<entries;i++) { 
    //pData->GetEntry(i);  
    //theHits = pAlign->getHits();
    //data = pData->GetArgs();
    //filling from the ntuple
    //hitA = (HMdcHit*) theHits->At(0);      
    //hitB = (HMdcHit*) theHits->At(1);      
    //hitC = (HMdcHit*) theHits->At(2);
    reg = 6*i;
    c_X = buffer[reg+4];
    c_Y = buffer[reg+5];
    b_X = buffer[reg+2];
    b_Y = buffer[reg+3];
    a_X = buffer[reg];
    a_Y = buffer[reg+1];
    
    //Converting all hits in each MDC to a common reference system
    //The common system is that of the last module in the constructor
    //(which is the coordinate system of the farther MDC from target
    //provided you use the usual order in the constructors)

    //Accelerated version (does not use transf, directly vecA and vecB)
    
    vecA_X = rot1_0 * a_X + rot1_1 * a_Y + trans1_0 - c_X;
    vecA_Y = rot1_3 * a_X + rot1_4 * a_Y + trans1_1 - c_Y;
    vecA_Z = rot1_6 * a_X + rot1_7 * a_Y + trans1_2;  

    vecB_X = rot0_0 * b_X + rot0_1 * b_Y + trans0_0 - c_X;
    vecB_Y = rot0_3 * b_X + rot0_4 * b_Y + trans0_1 - c_Y;
    vecB_Z = rot0_6 * b_X + rot0_7 * b_Y + trans0_2;  

    vecAxVecB_X = vecA_Y * vecB_Z - vecA_Z * vecB_Y;
    vecAxVecB_Y = vecA_Z * vecB_X - vecA_X * vecB_Z;
    vecAxVecB_Z = vecA_X * vecB_Y - vecA_Y * vecB_X;

    mod2VecA = vecA_X * vecA_X + vecA_Y * vecA_Y + vecA_Z * vecA_Z;
    mod2VecB = vecB_X * vecB_X + vecB_Y * vecB_Y + vecB_Z * vecB_Z;
    mod2VecAxVecB = vecAxVecB_X * vecAxVecB_X 
      + vecAxVecB_Y * vecAxVecB_Y 
      + vecAxVecB_Z * vecAxVecB_Z;

    sincua = mod2VecAxVecB / (mod2VecA*mod2VecB);
    
    commonPartial = 2/(mod2VecA*mod2VecA*mod2VecB*mod2VecB);
    mod2VecAB = mod2VecA*mod2VecB;
    
    partialx_a = commonPartial *
      ( ( vecAxVecB_X * (rot1_3*vecB_Z-rot1_6*vecB_Y) +
	  vecAxVecB_Y * (rot1_6*vecB_X-rot1_0*vecB_Z) +
	  vecAxVecB_Z * (rot1_0*vecB_Y-rot1_3*vecB_X) ) *
	mod2VecAB - mod2VecB * (rot1_0*vecA_X + 
				rot1_3*vecA_Y + 
				rot1_6*vecA_Z ) * mod2VecAxVecB );
    
    partialy_a = commonPartial *
      ( ( vecAxVecB_X * (rot1_4*vecB_Z-rot1_7*vecB_Y) +
	  vecAxVecB_Y * (rot1_7*vecB_X-rot1_1*vecB_Z) +
	  vecAxVecB_Z * (rot1_1*vecB_Y-rot1_4*vecB_X) ) *
	mod2VecAB - mod2VecB * (rot1_1*vecA_X + 
				rot1_4*vecA_Y + 
				rot1_7*vecA_Z ) * mod2VecAxVecB );
    
    partialx_b = commonPartial *
      ( ( vecAxVecB_Y * (vecB_Z-vecA_Z) +
	  vecAxVecB_Z * (vecA_Y-vecB_Y) ) * mod2VecAB 
	+ ( vecA_X*mod2VecB + vecB_X*mod2VecA ) * mod2VecAxVecB );
    
    partialy_b = commonPartial *
      ( ( vecAxVecB_X*(vecA_Z-vecB_Z) +
	  vecAxVecB_Y*(vecB_X-vecA_X) ) * mod2VecAB 
	+ ( vecA_Y*mod2VecB + vecB_Y*mod2VecA ) * mod2VecAxVecB );
    
    partialx_c = commonPartial *
      ( ( vecAxVecB_X * (-rot0_3*vecA_Z+rot0_6*vecA_Y) +
	  vecAxVecB_Y * (-rot0_6*vecA_X+rot0_0*vecA_Z) +
	  vecAxVecB_Z * (-rot0_0*vecA_Y+rot0_3*vecA_X) ) *
	mod2VecAB - mod2VecA * (rot0_0*vecB_X + 
				rot0_3*vecB_Y + 
				rot0_6*vecB_Z ) * mod2VecAxVecB );
    
    partialy_c = commonPartial *
      ( ( vecAxVecB_X * (-rot0_4*vecA_Z+rot0_7*vecA_Y) +
	  vecAxVecB_Y * (-rot0_7*vecA_X+rot0_1*vecA_Z) +
	  vecAxVecB_Z * (-rot0_1*vecA_Y+rot0_4*vecA_X) ) *
	mod2VecAB - mod2VecA * (rot0_1*vecB_X + 
				rot0_4*vecB_Y + 
				rot0_7*vecB_Z ) * mod2VecAxVecB);
    
    varianzainsincua = (partialx_a*partialx_a*errorx[0]*errorx[0] +
			partialy_a*partialy_a*errory[0]*errory[0] +
			partialx_b*partialx_b*errorx[1]*errorx[1] +
			partialy_b*partialy_b*errory[1]*errory[1] +
			partialx_c*partialx_c*errorx[2]*errorx[2] +
			partialy_c*partialy_c*errory[2]*errorx[2]);

    chisq += (sincua*sincua)/varianzainsincua;
  }

  f = chisq; 
  
  cout  << "chisqr= " << chisq << " out of " 
  	<< entries << " combinations."<< endl; 
} 





void fcnalign3(Int_t &npar, Double_t *gin, Double_t &f, 
	       Double_t *par, Int_t iflag){ 
  //
  // This function contains the functional to minimize
  // Use this if three  MDCs are present in the sector
  //
  Double_t chisq = 0.; 
  HGeomRotation rotmat[2];
  HGeomVector transla[2];
  HGeomVector a,b,c;
  HGeomVector transf[3];
  Float_t errorx[3]; 
  Float_t errory[3];

  HMdcAligner3M* pAlign = (HMdcAligner3M*)(gMinuit->GetObjectFit());
  TClonesArray* theHits;
  pAlign = (HMdcAligner3M*)(gMinuit->GetObjectFit());
  TTree* pData= pAlign->getTree();
  Stat_t entries = pData->GetEntries();  
  Int_t strategy = pAlign->getStrategy(); 

  Float_t* weights;
  weights = new Float_t[4];
  weights = pAlign->getWeights();
  Float_t* errors;
  errors = new Float_t[6];
  errors = pAlign->getErrors();
  Int_t numMods = pAlign->getNumMods();
  Bool_t UseUnitErrors = pAlign->getUseUnitErrors();
  Bool_t UseModErrors = pAlign->getUseModErrors();

  //filling matrix and vectors from parameters with the
  //same notation as in execute() and setGeomAuxPar() funcions
  //(here rotmat is equivalent to fRotMat ...)
  rotmat[0].setEulerAngles(par[0]*180./TMath::Pi(),
			   par[1]*180./TMath::Pi(),
			   par[2]*180./TMath::Pi()); 
  transla[0].setX(par[3]); 
  transla[0].setY(par[4]); 
  transla[0].setZ(par[5]);
  
  rotmat[1].setEulerAngles(par[6]*180./TMath::Pi(),
			   par[7]*180./TMath::Pi(),
			   par[8]*180./TMath::Pi()); 
  transla[1].setX(par[9]); 
  transla[1].setY(par[10]); 
  transla[1].setZ(par[11]);
    
  cout << "HMdcAligner3M::fcnalign()   Parameters: " 
       << par[0] << "," << par[1] << "," <<  par[2] << "," 
       << par[3] << ","  << par[4] << ","  << par[5] << ","
       << par[6] << "," << par[7] << "," <<  par[8] << "," 
       << par[9] << ","  << par[10] << ","  << par[11] ;
  cout << endl;

  
  HMdcHit *hitA;
  HMdcHit *hitB;
  HMdcHit *hitC;

  if(UseModErrors==kTRUE){
    errorx[0]=errors[0];errory[0]=errors[1];    
    errorx[1]=errors[2];errory[1]=errors[3];    
    errorx[2]=errors[4];errory[2]=errors[5];    
  }
  
  //loop on hits 
  for (Int_t i=0;i<entries;i++) { 
    pData->GetEntry(i);  
    theHits = pAlign->getHits();
    //data = pData->GetArgs();
    //filling from the ntuple
    hitA = (HMdcHit*) theHits->At(0);      
    hitB = (HMdcHit*) theHits->At(1);      
    hitC = (HMdcHit*) theHits->At(2);

    c.setX(hitC->getX());
    c.setY(hitC->getY());
    c.setZ(0.);      
    b.setX(hitB->getX());
    b.setY(hitB->getY());
    b.setZ(0.);      
    a.setX(hitA->getX());
    a.setY(hitA->getY());
    a.setZ(0.);
    
    //ERRORS
    if(UseModErrors==kFALSE){
      if(UseUnitErrors==kFALSE){
	errorx[0] = (hitA->getErrX()<0.01)? hitA->getErrX() : 0.2;
	errorx[1] = (hitB->getErrX()<0.01)? hitB->getErrX() : 0.2;      
	errorx[2] = (hitC->getErrX()<0.01)? hitC->getErrX() : 0.2;   
	errory[0] = (hitA->getErrY()<0.01)? hitA->getErrY() : 0.1;
	errory[1] = (hitB->getErrY()<0.01)? hitB->getErrY() : 0.1;      
	errory[2] = (hitC->getErrY()<0.01)? hitC->getErrY() : 0.1;
      }
      else 
	for(Int_t i=0;i<numMods;i++){
	  errorx[i]=1.0;
	  errory[i]=1.0;      
	}
    }
    if(AA_DEBUG>4){
      for(Int_t i=0;i<numMods;i++){
	cout << "errorx[" << i <<"] = " << errorx[i] << endl;	
	cout << "errory[" << i <<"] = " << errory[i]  << endl;
      }
    }
    
    //Converting all hits in each MDC to a common reference system
    //The common system is that of the last module in the constructor
    //(which is the coordinate system of the farther MDC from target
    //provided you use the usual order in the constructors)
    
    
    transf[2] = c;
    transf[1] = rotmat[0] * b + transla[0];
    transf[0] = rotmat[1] * a + transla[1];
    
    if(AA_DEBUG>4){
      for(Int_t i=0;i<numMods;i++){
	cout << "transf[" << i <<"]   ";
	transf[i].print();
      }
    }
    
    //1- Functional minimizing the angle between the straight lines
    //constructed from the hits of three modules
    if(strategy == 105){

      
      //Accelerated version (does not use transf, directly vecA and vecB)
      HGeomVector vecA(transf[0].getX()-transf[2].getX(),
		       transf[0].getY()-transf[2].getY(),
		       transf[0].getZ());
      HGeomVector vecB(transf[1].getX()-transf[2].getX(),
		       transf[1].getY()-transf[2].getY(),
		       transf[1].getZ());
      
      HGeomVector vecAxVecB = vecA.vectorProduct(vecB);

      Float_t mod2VecA = vecA.scalarProduct(vecA);
      Float_t mod2VecB = vecB.scalarProduct(vecB);
      Float_t mod2VecAxVecB = vecAxVecB.scalarProduct(vecAxVecB);

      //Float_t sincua = (vecAxVecB.scalarProduct(vecAxVecB))/
      //	(vecA.scalarProduct(vecA)*vecB.scalarProduct(vecB));
      Float_t sincua = mod2VecAxVecB / (mod2VecA*mod2VecB);

      Float_t partialx_a =   (2/(mod2VecA*mod2VecA*mod2VecB*mod2VecB)) *
	( ( vecAxVecB.getX()*(rotmat[1](3)*vecB.getZ()-
			      rotmat[1](6)*vecB.getY()) +
	    vecAxVecB.getY()*(rotmat[1](6)*vecB.getX()-
			      rotmat[1](0)*vecB.getZ()) +
	    vecAxVecB.getZ()*(rotmat[1](0)*vecB.getY()-
			      rotmat[1](3)*vecB.getX()) ) *
	  (mod2VecA*mod2VecB) -  mod2VecB * (rotmat[1](0)*vecA.getX() + 
					     rotmat[1](3)*vecA.getY() + 
					     rotmat[1](6)*vecA.getZ() ) * 
	  mod2VecAxVecB );
      

      Float_t partialy_a = (2/(mod2VecA*mod2VecA*mod2VecB*mod2VecB)) *
	( ( vecAxVecB.getX()*(rotmat[1](4)*vecB.getZ()-
			      rotmat[1](7)*vecB.getY()) +
	    vecAxVecB.getY()*(rotmat[1](7)*vecB.getX()-
			      rotmat[1](1)*vecB.getZ()) +
	    vecAxVecB.getZ()*(rotmat[1](1)*vecB.getY()-
			      rotmat[1](4)*vecB.getX()) ) *
	  (mod2VecA*mod2VecB) -  mod2VecB * (rotmat[1](1)*vecA.getX() + 
					     rotmat[1](4)*vecA.getY() + 
					     rotmat[1](7)*vecA.getZ() ) * 
	  mod2VecAxVecB );
      
      Float_t partialx_b =  (2/(mod2VecA*mod2VecA*mod2VecB*mod2VecB)) *
	( ( vecAxVecB.getY()*(vecB.getZ()-vecA.getZ()) +
	    vecAxVecB.getZ()*(vecA.getY()-vecB.getY()) ) *
	  (mod2VecA*mod2VecB) + ( vecA.getX()*mod2VecB + 
				  vecB.getX()*mod2VecA ) *
	  mod2VecAxVecB );
      
      Float_t partialy_b = (2/(mod2VecA*mod2VecA*mod2VecB*mod2VecB)) *
	( ( vecAxVecB.getX()*(vecA.getZ()-vecB.getZ()) +
	    vecAxVecB.getY()*(vecB.getX()-vecA.getX()) ) *
	  (mod2VecA*mod2VecB) + ( vecA.getY()*mod2VecB + 
				  vecB.getY()*mod2VecA ) *
	  mod2VecAxVecB );
      
      Float_t partialx_c = (2/(mod2VecA*mod2VecA*mod2VecB*mod2VecB)) *
	( ( vecAxVecB.getX()*(-rotmat[0](3)*vecA.getZ()+
			      rotmat[0](6)*vecA.getY()) +
	    vecAxVecB.getY()*(-rotmat[0](6)*vecA.getX()+
			      rotmat[0](0)*vecA.getZ()) +
	    vecAxVecB.getZ()*(-rotmat[0](0)*vecA.getY()+
			      rotmat[0](3)*vecA.getX()) ) *
	  (mod2VecA*mod2VecB) -  mod2VecA * (rotmat[0](0)*vecB.getX() + 
					     rotmat[0](3)*vecB.getY() + 
					     rotmat[0](6)*vecB.getZ() ) * 
	  mod2VecAxVecB );
      
      Float_t partialy_c = (2/(mod2VecA*mod2VecA*mod2VecB*mod2VecB)) *
	( ( vecAxVecB.getX()*(-rotmat[0](4)*vecA.getZ()+
			      rotmat[0](7)*vecA.getY()) +
	    vecAxVecB.getY()*(-rotmat[0](7)*vecA.getX()+
			      rotmat[0](1)*vecA.getZ()) +
	    vecAxVecB.getZ()*(-rotmat[0](1)*vecA.getY()+
			      rotmat[0](4)*vecA.getX()) ) *
	  (mod2VecA*mod2VecB) -  mod2VecA * (rotmat[0](1)*vecB.getX() + 
					     rotmat[0](4)*vecB.getY() + 
					     rotmat[0](7)*vecB.getZ() ) * 
	  mod2VecAxVecB);
      
      Float_t varianzainsincua = (partialx_a*partialx_a*errorx[0]*errorx[0] +
				  partialy_a*partialy_a*errory[0]*errory[0] +
				  partialx_b*partialx_b*errorx[1]*errorx[1] +
				  partialy_b*partialy_b*errory[1]*errory[1] +
				  partialx_c*partialx_c*errorx[2]*errorx[2] +
				  partialy_c*partialy_c*errory[2]*errorx[2]);

      chisq += (sincua*sincua)/varianzainsincua;
    }

    //2- Functional minimizing the residuals obtained from the 
    //linear fit of the Hit coordinates in three or four modules
    
    else{
      //See notation in Numerical Recipes pag 665
      Float_t ax=0.0, ay=0.0, bx=0.0, by=0.0;     
      Float_t sigax=0.0, sigay=0.0, sigbx=0.0, sigby=0.0; 
      Float_t Xwt, Xt, Xsxoss, Xsx=0.0, Xsy=0.0, Xst2=0.0, Xss=0.0;
      Float_t Ywt, Yt, Ysxoss, Ysx=0.0, Ysy=0.0, Yst2=0.0, Yss=0.0;
      Float_t XChi2=0.0, YChi2=0.0, chipar=0.0;
      
      for(Int_t i=0;i<numMods;i++){
	//Plane XZ
	Xwt = 1.0/(errorx[i]*errorx[i]);
	Xss += Xwt;
	Xsx += transf[i].getZ()*Xwt;     
	Xsy += transf[i].getX()*Xwt;      
	//Plane YZ
	Ywt = 1.0/(errory[i]*errory[i]);
	Yss += Ywt;
	Ysx += transf[i].getZ()*Ywt;     
	Ysy += transf[i].getY()*Ywt;
	
	if(AA_DEBUG>4) 
	  cout << "Xwt=" << Xwt << " Xss=" << Xss 
	       << " Xsx=" <<  Xsx << " Xsy=" << Xsy
	       << "Ywt=" << Ywt << " Yss=" << Yss 
	       << " Ysx=" <<  Ysx << " Ysy=" << Ysy << endl;
      }
      
      Xsxoss = Xsx/Xss;    
      Ysxoss = Ysx/Yss;
      
      if(AA_DEBUG>4) 
	cout << "Xsxoss=" << Xsxoss << "  Ysxoss=" 
	     << Ysxoss << endl;
      
      for(Int_t i=0;i<numMods;i++){
	//Plane XZ
	Xt = (transf[i].getZ()-Xsxoss)/errorx[i];
	Xst2 += Xt * Xt;
	bx += Xt * transf[i].getX()/errorx[i];            
	//Plane YZ
	Yt = (transf[i].getZ()-Ysxoss)/errory[i];
	Yst2 += Yt * Yt;
	by += Yt * transf[i].getY()/errory[i];
	
	if(AA_DEBUG>4) 
	  cout << "Xt=" << Xt << " Xst2=" << Xst2 
	       << " bx (partial)=" << bx << "Yt=" << Yt 
	       << " Yst2=" << Yst2 
	       << " by (partial)=" << by << endl; 
      } 
      
      bx /= Xst2;
      ax = (Xsy-(Xsx*bx))/Xss;
      by /= Yst2;
      ay = (Ysy-(Ysx*by))/Yss;
      
      if(AA_DEBUG>4)  
	cout << "bx=" << bx << " ax=" << ax
	     << "by=" << by << " ay=" << ay << endl;
      
      sigax = sqrt((1.0+Xsx*Xsx/(Xss*Xst2))/Xss);
      sigbx = sqrt(1.0/Xst2);
      sigay = sqrt((1.0+Ysx*Ysx/(Yss*Yst2))/Yss);
      sigby = sqrt(1.0/Yst2);
      
      if(AA_DEBUG>4) 
	cout << "sigbx=" << sigbx << " sigax=" << sigax
	     << "sigby=" << sigby << " sigay=" << sigay << endl;
      
      //Aqui falta evaluar la calidad del ajuste o bien encontrar
      //cuales son los errores que se esperan en los datos para un buen ajuste
      
      for(Int_t i=0;i<numMods;i++){
	//Plane XZ
	chipar = (transf[i].getX()-ax-bx*transf[i].getZ())/errorx[i];
	XChi2 += chipar*chipar;
	//Plane YZ
	chipar = (transf[i].getY()-ay-by*transf[i].getZ())/errory[i];
	YChi2 += chipar*chipar;
	if(AA_DEBUG>4) {
	  cout << "DiffX: " << transf[i].getX()-ax-bx*transf[i].getZ() 
	       << "DiffY: " << transf[i].getY()-ay-by*transf[i].getZ() << endl;
	  cout << "chiparX" << (transf[i].getX()-ax-bx*transf[i].getZ())/errorx[i]
	       << "chiparY" << (transf[i].getY()-ay-by*transf[i].getZ())/errory[i] << endl;
	  cout << "XChi2=" << XChi2 << " YChi2=" << YChi2 << endl;
	}
      }
      
      
      // Also can be defined by a vector V and a point P:
      // V=(bx,by,1) P=(ax,ay,0)
      // Let us calculate the square distance from the straigth 
      // line to the points (fit residuals) 
      Float_t part1=0,part2=0,part3=0,sdistance=0;
      for(Int_t i=0;i<numMods;i++){
	part1 = ( (transf[i].getX()-ax)*by ) - ( (transf[i].getY()-ay)*bx );
	part2 = ( (transf[i].getY()-ay)    ) - ( (transf[i].getZ()   )*by );
	part3 = ( (transf[i].getZ()   )*bx ) - ( (transf[i].getX()-ax)    );
	sdistance += (part1*part1 + part2*part2 + part3*part3)
	  /(1+bx*bx+by*by);
	if(AA_DEBUG>4) cout << "Square Distance point " << i << " - line: " 
			    << sdistance << endl;
	//chisq should be inside the loop for adding the distances
	//of all points which contribute to the fit
	if(strategy==101) chisq += sdistance; 
      } 
      if(strategy==100) chisq += XChi2+YChi2;
      //if(XChi2+YChi2<1.)  chisq += XChi2+YChi2;
      //else  chisq += 1.;
    }
  }
  f = chisq; 
  //if(AA_DEBUG>2){
  cout  << "chisqr= " << chisq << " out of " 
	<< entries << " combinations."<< endl; 
  //}
} 


