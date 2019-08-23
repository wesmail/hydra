# include <math.h>
# include <stdlib.h>
# include <iomanip.h>
# include <fstream.h>
# include <TMath.h>
# include <TROOT.h>
# include <TF1.h>
# include "hmdcaligner.h"
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

ClassImp(HMdcAligner)
  
//*-- AUTHOR : Hector Alvarez-Pol  
//*-- Date: 03/2001
//*-- Last Update: 25/05/01
//*-- Copyright: GENP (Univ. Santiago de Compostela)
  
////////////////////////////////////////////////////////////////////////
//
// HMdcAligner   (ongoing work)
//
// Performs the MDC software alignment (first part).
//
// Based on hits in MDCs (up to four), gets the relative
// geometrical parameters (three euler angles and translation 
// vector) vs the most external MDC. 
// After initialization (where euler angles can be obtained 
// from standard parameter containers) the execute() function
// makes histograms for the initial parameters and fill the
// tree with accepted pairs (one hit and only one in the
// selected window). Then, a set of parameters which minimizes
// a set of (gaussian?) distributions are obtained using TMinuit. 
// This distributions are the differences in X, Y and the slope
// projections in both MDCs (after track propagation and 
// transformations).
//
////////////////////////////////////////////////////////////////////////
  
Int_t HMdcAligner::fRecCount=0;
TFile* HMdcAligner::fOutRoot=0;
Int_t A_DEBUG=0;

HMdcAligner::HMdcAligner(void) : HReconstructor()
{  
  //
  // Default constructor. 
  //

  fLoc.setNIndex(5);
  fHits = new TClonesArray("HMdcHit",4);   
  fLoc.set(5,0,0,1,2,3); //dummy sector 0
  fNumMods = 4;
  initDefaults();
}

HMdcAligner::HMdcAligner(Int_t sector, Int_t modA, Int_t modB) 
  : HReconstructor()
{  
  //
  // Constructor including module election. Alignment procedure 
  // proyects hits of modA in modB coordinate system and compares
  //

  fLoc.setNIndex(3);
  fHits = new TClonesArray("HMdcHit",2);   
  fLoc.set(3,sector,modA,modB);
  fNumMods = 2;
  initDefaults();
}

HMdcAligner::HMdcAligner(Text_t* name, Text_t* title, Int_t sector, 
			 Int_t modA, Int_t modB, Int_t modC=-1, Int_t modD=-1)
  : HReconstructor(name, title)
{  
  //
  // Constructor including module election (and name and title, which 
  // seems to be very important). Alignment procedure 
  // proyects hits of modA in modB coordinate system and compares
  //

  if(modC == -1) {
    fHits=new TClonesArray("HMdcHit",2);      
    fLoc.setNIndex(3);
    fLoc.set(3,sector,modA,modB); 
    fNumMods = 2;
  } 
  else if(modD == -1){
    fHits=new TClonesArray("HMdcHit",3);     
    fLoc.setNIndex(4);
    fLoc.set(4,sector,modA,modB,modC); 
    fNumMods = 3;
  }
  else {    
    fHits=new TClonesArray("HMdcHit",4);      
    fLoc.setNIndex(5);
    fLoc.set(5,sector,modA,modB,modC,modD); 
    fNumMods = 4;
  }
  initDefaults();
}

HMdcAligner::HMdcAligner(Text_t* name, Text_t* title, 
			 Int_t sector, Int_t modA)
  : HReconstructor(name, title)
{  
  //
  // Constructor for only one MDC
  //
  
  fHits=new TClonesArray("HMdcHit",1);   
  fLoc.setNIndex(2);
  fLoc.set(2,sector,modA);
  fNumMods = 1;  
  fConstTukey = 10;

  Char_t title[50], tmp[50];
  sprintf(title,"%s%i%s%i","Sector_",sector,"_ModA_",modA);
  sprintf(tmp,"%s%s%i%s%i","All","_Sector_",sector,"_ModA_",modA);    
  fAlignAll = new TTree(tmp,title);
  fAlignAll->Branch("hits",&fHits);  
}

void HMdcAligner::initDefaults(void)
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
  fUseCut = kTRUE;  
  fUseSlopeCut = kFALSE;
  fUseTarget = kFALSE;

  fMin = 0;
  fFix = 0;
  fXArea = 100;  
  fYArea = 100;
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

HMdcAligner::~HMdcAligner(void)
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





void HMdcAligner::initMinimization(void){
  //
  // Minimization defaults 
  //

  fIterCriteria = 0.01;   
  fWeights = new Float_t[4];
  fSteps = new Float_t[18];
  fLimits = new Float_t[18];
  setWeights(400,160,0.004,0.003);
  setSteps(0.01,0.01,0.01,0.1,0.1,0.1,
	   0.01,0.01,0.01,0.1,0.1,0.1,
	   0.01,0.01,0.01,0.1,0.1,0.1);
  setLimits(0.,0.,0.,0.,0.,0.,
	    0.,0.,0.,0.,0.,0.,
	    0.,0.,0.,0.,0.,0.);
}





Int_t HMdcAligner::getRelParams(HGeomVector*& ang,HGeomVector*& tra)
  //  				HGeomVector*& ang2,HGeomVector*& tra2,
  //  				HGeomVector*& ang3,HGeomVector*& tra3)
{    
  //
  // Serves pointers to the relative transformation parameters
  //
  /*
  if (fNumMods>3){
    ang3 = &fEuler[2];         //MDC A - MDC D
    tra3 = &fTranslation[2];  
    ang2 = &fEuler[1];         //MDC B - MDC D
    tra2 = &fTranslation[1];
    ang = &fEuler[0];          //MDC C - MDC D
    tra = &fTranslation[0];
    if(A_DEBUG>2){
      cout << " HMdcAligner :: getRelParams(). Filling " << endl;
      cout << "ang3: " << ang3 << endl;
      cout << "tra3: " << tra3 << endl;
      cout << "ang2: " << ang2 << endl;
      cout << "tra2: " << tra2 << endl;      
      cout << "ang: " << ang << endl;
      cout << "tra: " << tra << endl;
    }
  }
  else if(fNumMods>2){
    ang2 = &fEuler[1];          //MDC A - MDC C 
    tra2 = &fTranslation[1];
    ang = &fEuler[0];           //MDC B - MDC C
    tra = &fTranslation[0];
    if(A_DEBUG>2){
      cout << " HMdcAligner :: getRelParams(). Filling " << endl;
      cout << "ang2: " << ang2 << endl;
      cout << "tra2: " << tra2 << endl;  
      cout << "ang: " << ang << endl;
      cout << "tra: " << tra << endl;
    }
  }
  else{
    ang = &fEuler[0];           //MDC A - MDC B
    tra = &fTranslation[0];
    if(A_DEBUG>2){
      cout << " HMdcAligner :: getRelParams(). Filling " << endl;
      cout << "ang: " << ang << endl;
      cout << "tra: " << tra << endl;
    }
  }
  */
  ang = fEuler; 
  tra = fTranslation;  

  cout << "fEuler " << fEuler << " fTranslation " << fTranslation << endl; 
  cout << "ang " << ang << " tra " << tra << endl; 


  return 0;
}





void HMdcAligner::setOutputAscii(TString name)
{
  //
  // Sets ascii output for debugging.
  //

    fNameAscii=name;
}





void HMdcAligner::setOutputRoot(TString name)
{
  //
  // Sets ascii output for debugging.
  //

    fNameRoot=name;
}





void HMdcAligner::setSigmas(Float_t XSigmas, Float_t YSigmas, 
			    Float_t S0Sigmas, Float_t S1Sigmas)
{
  //
  // Sets number of sigmas in cuts.
  //

  fXSigmas = XSigmas;    //1.64 -> 90% of gauss distribution
  fYSigmas = YSigmas;    //1.96 -> 95% of gauss distribution
  fS0Sigmas = S0Sigmas;   //2.58 -> 99% of gauss distribution
  fS1Sigmas = S1Sigmas;   //3.29 -> 99.9% of gauss distribution
}





void HMdcAligner::setTukeyConstant(Float_t cte)
{
  //
  // Sets the Tukey constant in the weigth determination
  // for 1 MDC target finder
  //
  fConstTukey = cte;
}





void HMdcAligner::setHistograms(void)
{
  //
  // Inits histograms
  //
  
  Int_t sector = fLoc[0];
  Int_t modA = fLoc[1];
  Int_t modB = fLoc[2];
  fRecCount++;  
  Char_t title[50], tmp[50];
  if(!fOutRoot) fOutRoot = new TFile(fNameRoot,"UPDATE");

//Histograms for studying residuals in projections 

  if(fNumMods>3) {
    CvsDinDCS_X = new TH1F*[fHistNum];
    CvsDinDCS_Y = new TH1F*[fHistNum];     
    CvsDinDCS_XSlope = new TH1F*[fHistNum];
    CvsDinDCS_YSlope = new TH1F*[fHistNum];
    BvsDinDCS_X = new TH1F*[fHistNum];     
    BvsDinDCS_Y = new TH1F*[fHistNum];     
    BvsDinDCS_XSlope = new TH1F*[fHistNum];  
    BvsDinDCS_YSlope = new TH1F*[fHistNum];  
    AvsDinDCS_X = new TH1F*[fHistNum];  
    AvsDinDCS_Y = new TH1F*[fHistNum];
    AvsDinDCS_XSlope = new TH1F*[fHistNum];  
    AvsDinDCS_YSlope = new TH1F*[fHistNum];  
    DvsCinCCS_X = new TH1F*[fHistNum];  
    DvsCinCCS_Y = new TH1F*[fHistNum];
    DvsCinCCS_XSlope = new TH1F*[fHistNum];  
    DvsCinCCS_YSlope = new TH1F*[fHistNum];    
    DvsAinACS_X = new TH1F*[fHistNum];  
    DvsAinACS_Y = new TH1F*[fHistNum];
    DvsAinACS_XSlope = new TH1F*[fHistNum];  
    DvsAinACS_YSlope = new TH1F*[fHistNum]; 
    DvsBinBCS_X = new TH1F*[fHistNum];  
    DvsBinBCS_Y = new TH1F*[fHistNum];
    DvsBinBCS_XSlope = new TH1F*[fHistNum];  
    DvsBinBCS_YSlope = new TH1F*[fHistNum]; 

    SqrDistToD = new TH1F*[fHistNum];     
  }
  if(fNumMods>2) {
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
  } 
  AvsBinBCS_X = new TH1F*[fHistNum];  
  AvsBinBCS_Y = new TH1F*[fHistNum];
  AvsBinBCS_XSlope = new TH1F*[fHistNum];  
  AvsBinBCS_YSlope = new TH1F*[fHistNum];  
  BvsAinACS_X = new TH1F*[fHistNum];  
  BvsAinACS_Y = new TH1F*[fHistNum];
  BvsAinACS_XSlope = new TH1F*[fHistNum];  
  BvsAinACS_YSlope = new TH1F*[fHistNum];
 
  if (fNumMods>3){    
    Int_t modC = fLoc[3];
    Int_t modD = fLoc[4];
    sprintf(title,"%s%i%s%i%s%i%s%i%s%i","Sector_",sector,"_ModA_",modA,
	    "_ModB_",modB,"_ModC_",modC,"_ModD_",modD);
    sprintf(tmp,"%s%s%i%s%i%s%i%s%i%s%i","All","_Sector_",sector,
	    "_ModA_",modA,"_ModB_",modB,"_ModC_",modC,"_ModD_",modD);
    fAlignAll = new TTree(tmp,title);
    fAlignAll->Branch("hits",&fHits,64000);

    sprintf(tmp,"%s%s%i%s%i%s%i%s%i%s%i","AllCut","_Sector_",sector,
	    "_ModA_",modA,"_ModB_",modB,"_ModC_",modC,"_ModD_",modD);
    fAlignAllCut = new TTree(tmp,title);
    fAlignAllCut->Branch("hits",&fHits,64000);
    
    static Char_t newDirName[255];
    sprintf(newDirName,"%s%s%i%s%i%s%i%s%i%s%i","Aligner_","S_",sector,
	    "_ModA_",modA,"_ModB_",modB,"_ModC_",modC,"_ModD_",modD);
    fOutRoot->mkdir(newDirName,newDirName);
    fOutRoot->cd(newDirName);
    
    //binning
    Int_t bin=200, binS=200, binChi=200, binDist=200;
    Int_t min=-100,max=100,minS=-1,maxS=1;
    Int_t minChi=0, maxChi=20, minDist=0,maxDist=50;
    
    for(Int_t index=0;index<fHistNum;index++){
      sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i%s%i","CvsDinDCS_X_",index,"_Sector_",
	      sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC,"_ModD_",modD);
      CvsDinDCS_X[index] = new TH1F(tmp,title,bin,min,max);  
      sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i%s%i","CvsDinDCS_Y_",index,"_Sector_",
	      sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC,"_ModD_",modD);
      CvsDinDCS_Y[index] = new TH1F(tmp,title,bin,min,max);  
      sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i%s%i","CvsDinDCS_XSlope_",index,"_Sector_",
	      sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC,"_ModD_",modD);
      CvsDinDCS_XSlope[index] = new TH1F(tmp,title,binS,minS,maxS);
      sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i%s%i","CvsDinDCS_YSlope_",index,"_Sector_",
	      sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC,"_ModD_",modD);
      CvsDinDCS_YSlope[index] = new TH1F(tmp,title,binS,minS,maxS);
      
      sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i%s%i","BvsDinDCS_X_",index,"_Sector_",
	      sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC,"_ModD_",modD);
      BvsDinDCS_X[index] = new TH1F(tmp,title,bin,min,max);  
      sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i%s%i","BvsDinDCS_Y_",index,"_Sector_",
	      sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC,"_ModD_",modD);
      BvsDinDCS_Y[index] = new TH1F(tmp,title,bin,min,max);  
      sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i%s%i","BvsDinDCS_XSlope_",index,"_Sector_",
	      sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC,"_ModD_",modD);
      BvsDinDCS_XSlope[index] = new TH1F(tmp,title,binS,minS,maxS);
      sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i%s%i","BvsDinDCS_YSlope_",index,"_Sector_",
	      sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC,"_ModD_",modD);
      BvsDinDCS_YSlope[index] = new TH1F(tmp,title,binS,minS,maxS);
      
      sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i%s%i","AvsDinDCS_X_",index,"_Sector_",
	      sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC,"_ModD_",modD);
      AvsDinDCS_X[index] = new TH1F(tmp,title,bin,min,max);  
      sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i%s%i","AvsDinDCS_Y_",index,"_Sector_",
	      sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC,"_ModD_",modD);
      AvsDinDCS_Y[index] = new TH1F(tmp,title,bin,min,max);  
      sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i%s%i","AvsDinDCS_XSlope_",index,"_Sector_",
	      sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC,"_ModD_",modD);
      AvsDinDCS_XSlope[index] = new TH1F(tmp,title,binS,minS,maxS);
      sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i%s%i","AvsDinDCS_YSlope_",index,"_Sector_",
	      sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC,"_ModD_",modD);
      AvsDinDCS_YSlope[index] = new TH1F(tmp,title,binS,minS,maxS);
      
      sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i%s%i","DvsCinCCS_X_",index,"_Sector_",
	      sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC,"_ModD_",modD);
      DvsCinCCS_X[index] = new TH1F(tmp,title,bin,min,max);  
      sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i%s%i","DvsCinCCS_Y_",index,"_Sector_",
	      sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC,"_ModD_",modD);
      DvsCinCCS_Y[index] = new TH1F(tmp,title,bin,min,max);  
      sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i%s%i","DvsCinCCS_XSlope_",index,"_Sector_",
	      sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC,"_ModD_",modD);
      DvsCinCCS_XSlope[index] = new TH1F(tmp,title,binS,minS,maxS);
      sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i%s%i","DvsCinCCS_YSlope_",index,"_Sector_",
	      sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC,"_ModD_",modD);
      DvsCinCCS_YSlope[index] = new TH1F(tmp,title,binS,minS,maxS);
      
      sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i%s%i","BvsCinCCS_X_",index,"_Sector_",
	      sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC,"_ModD_",modD);
      BvsCinCCS_X[index] = new TH1F(tmp,title,bin,min,max);  
      sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i%s%i","BvsCinCCS_Y_",index,"_Sector_",
	      sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC,"_ModD_",modD);
      BvsCinCCS_Y[index] = new TH1F(tmp,title,bin,min,max);  
      sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i%s%i","BvsCinCCS_XSlope_",index,"_Sector_",
	      sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC,"_ModD_",modD);
      BvsCinCCS_XSlope[index] = new TH1F(tmp,title,binS,minS,maxS);
      sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i%s%i","BvsCinCCS_YSlope_",index,"_Sector_",
	      sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC,"_ModD_",modD);
      BvsCinCCS_YSlope[index] = new TH1F(tmp,title,binS,minS,maxS);
      
      sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i%s%i","AvsCinCCS_X_",index,"_Sector_",
	      sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC,"_ModD_",modD);
      AvsCinCCS_X[index] = new TH1F(tmp,title,bin,min,max);  
      sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i%s%i","AvsCinCCS_Y_",index,"_Sector_",
	      sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC,"_ModD_",modD);
      AvsCinCCS_Y[index] = new TH1F(tmp,title,bin,min,max);  
      sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i%s%i","AvsCinCCS_XSlope_",index,"_Sector_",
	      sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC,"_ModD_",modD);
      AvsCinCCS_XSlope[index] = new TH1F(tmp,title,binS,minS,maxS);
      sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i%s%i","AvsCinCCS_YSlope_",index,"_Sector_",
	      sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC,"_ModD_",modD);
      AvsCinCCS_YSlope[index] = new TH1F(tmp,title,binS,minS,maxS);
      
      sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i%s%i","DvsBinBCS_X_",index,"_Sector_",
	      sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC,"_ModD_",modD);
      DvsBinBCS_X[index] = new TH1F(tmp,title,bin,min,max);  
      sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i%s%i","DvsBinBCS_Y_",index,"_Sector_",
	      sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC,"_ModD_",modD);
      DvsBinBCS_Y[index] = new TH1F(tmp,title,bin,min,max);  
      sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i%s%i","DvsBinBCS_XSlope_",index,"_Sector_",
	      sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC,"_ModD_",modD);
      DvsBinBCS_XSlope[index] = new TH1F(tmp,title,binS,minS,maxS);
      sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i%s%i","DvsBinBCS_YSlope_",index,"_Sector_",
	      sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC,"_ModD_",modD);
      DvsBinBCS_YSlope[index] = new TH1F(tmp,title,binS,minS,maxS);
      
      sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i%s%i","CvsBinBCS_X_",index,"_Sector_",
	      sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC,"_ModD_",modD);
      CvsBinBCS_X[index] = new TH1F(tmp,title,bin,min,max);  
      sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i%s%i","CvsBinBCS_Y_",index,"_Sector_",
	      sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC,"_ModD_",modD);
      CvsBinBCS_Y[index] = new TH1F(tmp,title,bin,min,max);  
      sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i%s%i","CvsBinBCS_XSlope_",index,"_Sector_",
	      sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC,"_ModD_",modD);
      CvsBinBCS_XSlope[index] = new TH1F(tmp,title,binS,minS,maxS);
      sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i%s%i","CvsBinBCS_YSlope_",index,"_Sector_",
	      sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC,"_ModD_",modD);
      CvsBinBCS_YSlope[index] = new TH1F(tmp,title,binS,minS,maxS);
      
      sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i%s%i","AvsBinBCS_X_",index,"_Sector_",
	      sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC,"_ModD_",modD);
      AvsBinBCS_X[index] = new TH1F(tmp,title,bin,min,max);  
      sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i%s%i","AvsBinBCS_Y_",index,"_Sector_",
	      sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC,"_ModD_",modD);
      AvsBinBCS_Y[index] = new TH1F(tmp,title,bin,min,max);  
      sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i%s%i","AvsBinBCS_XSlope_",index,"_Sector_",
	      sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC,"_ModD_",modD);
      AvsBinBCS_XSlope[index] = new TH1F(tmp,title,binS,minS,maxS);
      sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i%s%i","AvsBinBCS_YSlope_",index,"_Sector_",
	      sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC,"_ModD_",modD);
      AvsBinBCS_YSlope[index] = new TH1F(tmp,title,binS,minS,maxS);
      
      sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i%s%i","DvsAinACS_X_",index,"_Sector_",
	      sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC,"_ModD_",modD);
      DvsAinACS_X[index] = new TH1F(tmp,title,bin,min,max);  
      sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i%s%i","DvsAinACS_Y_",index,"_Sector_",
	      sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC,"_ModD_",modD);
      DvsAinACS_Y[index] = new TH1F(tmp,title,bin,min,max);  
      sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i%s%i","DvsAinACS_XSlope_",index,"_Sector_",
	      sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC,"_ModD_",modD);
      DvsAinACS_XSlope[index] = new TH1F(tmp,title,binS,minS,maxS);
      sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i%s%i","DvsAinACS_YSlope_",index,"_Sector_",
	      sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC,"_ModD_",modD);
      DvsAinACS_YSlope[index] = new TH1F(tmp,title,binS,minS,maxS);
      
      sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i%s%i","CvsAinACS_X_",index,"_Sector_",
	      sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC,"_ModD_",modD);
      CvsAinACS_X[index] = new TH1F(tmp,title,bin,min,max);  
      sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i%s%i","CvsAinACS_Y_",index,"_Sector_",
	      sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC,"_ModD_",modD);
      CvsAinACS_Y[index] = new TH1F(tmp,title,bin,min,max);  
      sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i%s%i","CvsAinACS_XSlope_",index,"_Sector_",
	      sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC,"_ModD_",modD);
      CvsAinACS_XSlope[index] = new TH1F(tmp,title,binS,minS,maxS);
      sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i%s%i","CvsAinACS_YSlope_",index,"_Sector_",
	      sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC,"_ModD_",modD);
      CvsAinACS_YSlope[index] = new TH1F(tmp,title,binS,minS,maxS);
      
      sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i%s%i","BvsAinACS_X_",index,"_Sector_",
	      sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC,"_ModD_",modD);
      BvsAinACS_X[index] = new TH1F(tmp,title,bin,min,max);  
      sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i%s%i","BvsAinACS_Y_",index,"_Sector_",
	      sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC,"_ModD_",modD);
      BvsAinACS_Y[index] = new TH1F(tmp,title,bin,min,max);  
      sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i%s%i","BvsAinACS_XSlope_",index,"_Sector_",
	      sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC,"_ModD_",modD);
      BvsAinACS_XSlope[index] = new TH1F(tmp,title,binS,minS,maxS);
      sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i%s%i","BvsAinACS_YSlope_",index,"_Sector_",
	      sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC,"_ModD_",modD);
      BvsAinACS_YSlope[index] = new TH1F(tmp,title,binS,minS,maxS);

      sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i%s%i","XChi2Hist_",index,"_Sector_",
	      sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC,"_ModD_",modD);
      XChi2Hist[index] = new TH1F(tmp,title,binChi,minChi,maxChi);       
      sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i%s%i","YChi2Hist_",index,"_Sector_",
	      sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC,"_ModD_",modD);
      YChi2Hist[index] = new TH1F(tmp,title,binChi,minChi,maxChi);       
      sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i%s%i","TotalChi2_",index,"_Sector_",
	      sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC,"_ModD_",modD);
      TotalChi2[index] = new TH1F(tmp,title,binChi,minChi,maxChi);      
      sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i%s%i","SqrDistToA_",index,"_Sector_",
	      sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC,"_ModD_",modD); 
      SqrDistToA[index] = new TH1F(tmp,title,binDist,minDist,maxDist);      
      sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i%s%i","SqrDistToB_",index,"_Sector_",
	      sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC,"_ModD_",modD);  
      SqrDistToB[index] = new TH1F(tmp,title,binDist,minDist,maxDist);       
      sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i%s%i","SqrDistToC_",index,"_Sector_",
	      sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC,"_ModD_",modD);  
      SqrDistToC[index] = new TH1F(tmp,title,binDist,minDist,maxDist);        
      sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i%s%i","SqrDistToD_",index,"_Sector_",
	      sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC,"_ModD_",modD);  
      SqrDistToD[index] = new TH1F(tmp,title,binDist,minDist,maxDist);            
      sprintf(tmp,"%s%i%s%i%s%i%s%i%s%i%s%i","SqrDist_",index,"_Sector_",
	      sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC,"_ModD_",modD);    
      SqrDist[index] = new TH1F(tmp,title,binDist,minDist,maxDist);
    }
  }
  else if(fNumMods>2){    
    Int_t modC = fLoc[3];    
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
    sprintf(newDirName,"%s%s%i%s%i%s%i%s%i","Aligner_","S_",sector,
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
  }
  else{    
    sprintf(title,"%s%i%s%i%s%i","Sector_",sector,
	    "_ModA_",modA,"_ModB_",modB);
    sprintf(tmp,"%s%s%i%s%i%s%i","All","_Sector_",sector,
	    "_ModA_",modA,"_ModB_",modB);
    fAlignAll = new TTree(tmp,title);
    fAlignAll->Branch("hits",&fHits);

    sprintf(tmp,"%s%s%i%s%i%s%i","AllCut","_Sector_",sector,
	    "_ModA_",modA,"_ModB_",modB);
    fAlignAllCut = new TTree(tmp,title);
    fAlignAllCut->Branch("hits",&fHits);

    static Char_t newDirName[255];
    sprintf(newDirName,"%s%s%i%s%i%s%i","Aligner_","S_",sector,
	    "_ModA_",modA,"_ModB_",modB);
    fOutRoot->mkdir(newDirName,newDirName);
    fOutRoot->cd(newDirName);
    
    //binning
    Int_t bin=200;
    Int_t binS=200;
    Int_t min=-100,max=100,minS=-1,maxS=1;
    
    for(Int_t index=0;index<fHistNum;index++){
      sprintf(tmp,"%s%i%s%i%s%i%s%i","AvsBinBCS_X_",index,"_Sector_",sector,
	      "_ModA_",modA,"_ModB_",modB);
      AvsBinBCS_X[index] = new TH1F(tmp,title,bin,min,max);  
      sprintf(tmp,"%s%i%s%i%s%i%s%i","AvsBinBCS_Y_",index,"_Sector_",sector,
	      "_ModA_",modA,"_ModB_",modB);
      AvsBinBCS_Y[index] = new TH1F(tmp,title,bin,min,max);  
      sprintf(tmp,"%s%i%s%i%s%i%s%i","AvsBinBCS_XSlope_",index,"_Sector_",sector,
	      "_ModA_",modA,"_ModB_",modB);
      AvsBinBCS_XSlope[index] = new TH1F(tmp,title,binS,minS,maxS);
      sprintf(tmp,"%s%i%s%i%s%i%s%i","AvsBinBCS_YSlope_",index,"_Sector_",sector,
	      "_ModA_",modA,"_ModB_",modB);
      AvsBinBCS_YSlope[index] = new TH1F(tmp,title,binS,minS,maxS);

      sprintf(tmp,"%s%i%s%i%s%i%s%i","BvsAinACS_X_",index,"_Sector_",sector,
	      "_ModA_",modA,"_ModB_",modB);
      BvsAinACS_X[index] = new TH1F(tmp,title,bin,min,max);  
      sprintf(tmp,"%s%i%s%i%s%i%s%i","BvsAinACS_Y_",index,"_Sector_",sector,
	      "_ModA_",modA,"_ModB_",modB);
      BvsAinACS_Y[index] = new TH1F(tmp,title,bin,min,max);  
      sprintf(tmp,"%s%i%s%i%s%i%s%i","BvsAinACS_XSlope_",index,"_Sector_",sector,
	      "_ModA_",modA,"_ModB_",modB);
      BvsAinACS_XSlope[index] = new TH1F(tmp,title,binS,minS,maxS);
      sprintf(tmp,"%s%i%s%i%s%i%s%i","BvsAinACS_YSlope_",index,"_Sector_",sector,
	      "_ModA_",modA,"_ModB_",modB);
      BvsAinACS_YSlope[index] = new TH1F(tmp,title,binS,minS,maxS);
    }
  }
  //Histogramas comunes

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





void HMdcAligner::fitHistograms(Int_t index)
{
  //
  //Fits to a gaussian the four relevant histograms
  //and obtains the fit parameters for further data selection
  //Only valid for two MDCs

  //Ahora solo para dos MDCs. En el caso de mas de dos, es posible
  //realizarlo, pero deberia estudiar sus beneficios cuidadosamente
  if(fNumMods==2){
    
    Float_t XNewAreaA, XNewAreaB, YNewAreaA, YNewAreaB;
    Float_t S0NewAreaA, S0NewAreaB, S1NewAreaA, S1NewAreaB;
    Float_t XNewMeanA, XNewMeanB, YNewMeanA, YNewMeanB;
    Float_t S0NewMeanA, S0NewMeanB, S1NewMeanA, S1NewMeanB;

    TF1 *f1X = new TF1("f1X","gaus",-fXArea,fXArea);
    TF1 *f1Y = new TF1("f1Y","gaus",-fYArea,fYArea);
    TF1 *f1S = new TF1("f1S","gaus",-1,1);
    TF1 *totalX  = new TF1("totalX","gaus(0)+pol2(3)",-fXArea,fXArea);
    TF1 *totalY  = new TF1("totalY","gaus(0)+pol2(3)",-fYArea,fYArea);
    TF1 *totalS  = new TF1("totalS","gaus(0)+pol2(3)",-1,1);
    Double_t par[6]; 

    if(A_DEBUG>1) cout << endl 
		       <<"**** fitHistograms() results ****" << endl;   
    if(A_DEBUG>1) cout << endl 
		       <<"**** Gauss fit: mean, sigma ****" << endl 
		       <<"**** Gauss+pol: mean, sigma ****" 
		       << endl;   

    AvsBinBCS_X[index]->Fit("f1X","RQNW");
    Float_t fitPar0     = f1X->GetParameter(0);  // constant
    Float_t fitPar1     = f1X->GetParameter(1);  // mean
    Float_t fitPar2     = f1X->GetParameter(2);  // sigma
    if(A_DEBUG>1) cout << " AvsBinBCS_X[" << index << "]: "
		       << fitPar1 << ", " << fitPar2<< ", " ;
    f1X->GetParameters(&par[0]);
    par[3] =  par[4] = par[5] = 0.; 
    totalX->SetParameters(par);
    AvsBinBCS_X[index]->Fit("totalX","RQNW"); 
    fitPar0     = totalX->GetParameter(0); 
    fitPar1     = totalX->GetParameter(1);
    fitPar2     = totalX->GetParameter(2);
    if(A_DEBUG>1) cout << fitPar1 << ", " << fitPar2 << endl;
    XNewAreaA = fXSigmas * fitPar2;
    XNewMeanA = fitPar1;

    AvsBinBCS_Y[index]->Fit("f1Y","RQNW");
    fitPar0     = f1Y->GetParameter(0);  // constant
    fitPar1     = f1Y->GetParameter(1);  // mean
    fitPar2     = f1Y->GetParameter(2);  // sigma    
    if(A_DEBUG>1) cout << " AvsBinBCS_Y[" << index << "]: "
		       << fitPar1 << ", " << fitPar2<< ", " ;
    f1Y->GetParameters(&par[0]);
    totalY->SetParameters(par);     
    AvsBinBCS_Y[index]->Fit("totalY","RQNW");
    fitPar1     = totalY->GetParameter(1);
    fitPar2     = totalY->GetParameter(2);
    if(A_DEBUG>1) cout << fitPar1 << ", " << fitPar2 << endl;
    YNewAreaA = fYSigmas * fitPar2;
    YNewMeanA = fitPar1;

    AvsBinBCS_XSlope[index]->Fit("f1S","RQNW");
    fitPar0     = f1S->GetParameter(0);  // constant
    fitPar1     = f1S->GetParameter(1);  // mean
    fitPar2     = f1S->GetParameter(2);  // sigma    
    if(A_DEBUG>1) cout << " AvsBinBCS_XSlope[" << index << "]: "
		       << fitPar1 << ", " << fitPar2<< ", " ;
    f1S->GetParameters(&par[0]);
    totalS->SetParameters(par);     
    AvsBinBCS_XSlope[index]->Fit("totalS","RQNW");
    fitPar1     = totalS->GetParameter(1);
    fitPar2     = totalS->GetParameter(2);
    if(A_DEBUG>1) cout << fitPar1 << ", " << fitPar2 << endl;
    S0NewAreaA = fS0Sigmas * fitPar2;
    S0NewMeanA = fitPar1;
   
    AvsBinBCS_YSlope[index]->Fit("f1S","RQNW");
    fitPar0     = f1S->GetParameter(0);  // constant
    fitPar1     = f1S->GetParameter(1);  // mean
    fitPar2     = f1S->GetParameter(2);  // sigma    
    if(A_DEBUG>1) cout << " AvsBinBCS_YSlope[" << index << "]: "
		       << fitPar1 << ", " << fitPar2<< ", " ;
    f1S->GetParameters(&par[0]);
    totalS->SetParameters(par);     
    AvsBinBCS_YSlope[index]->Fit("totalS","RQNW");
    fitPar1     = totalS->GetParameter(1);
    fitPar2     = totalS->GetParameter(2);
    if(A_DEBUG>1) cout << fitPar1 << ", " << fitPar2 << endl;
    S1NewAreaA = fS1Sigmas * fitPar2;
    S1NewMeanA = fitPar1;

    BvsAinACS_X[index]->Fit("f1X","RQNW");
    fitPar0     = f1X->GetParameter(0);  // constant
    fitPar1     = f1X->GetParameter(1);  // mean
    fitPar2     = f1X->GetParameter(2);  // sigma    
    if(A_DEBUG>1) cout << " BvsAinACS_X[" << index << "]: "
		       << fitPar1 << ", " << fitPar2<< ", " ;
    f1X->GetParameters(&par[0]);
    totalX->SetParameters(par);     
    BvsAinACS_X[index]->Fit("totalX","RQNW");
    fitPar1     = totalX->GetParameter(1);
    fitPar2     = totalX->GetParameter(2);
    if(A_DEBUG>1) cout << fitPar1 << ", " << fitPar2 << endl;
    XNewAreaB = fXSigmas * fitPar2;
    XNewMeanB = fitPar1;

    BvsAinACS_Y[index]->Fit("f1Y","RQNW");
    fitPar0     = f1Y->GetParameter(0);  // constant
    fitPar1     = f1Y->GetParameter(1);  // mean
    fitPar2     = f1Y->GetParameter(2);  // sigma    
    if(A_DEBUG>1) cout << " BvsAinACS_Y[" << index << "]: "
		       << fitPar1 << ", " << fitPar2<< ", " ;    
    f1Y->GetParameters(&par[0]);
    totalY->SetParameters(par);     
    BvsAinACS_Y[index]->Fit("totalY","RQNW");
    fitPar1     = totalY->GetParameter(1);
    fitPar2     = totalY->GetParameter(2);
    if(A_DEBUG>1) cout << fitPar1 << ", " << fitPar2 << endl;
    YNewAreaB = fYSigmas * fitPar2;
    YNewMeanB = fitPar1;

    BvsAinACS_XSlope[index]->Fit("f1S","RQNW");
    fitPar0     = f1S->GetParameter(0);  // constant
    fitPar1     = f1S->GetParameter(1);  // mean
    fitPar2     = f1S->GetParameter(2);  // sigma    
    if(A_DEBUG>1) cout << " BvsAinACS_XSlope[" << index << "]: "
		       << fitPar1 << ", " << fitPar2<< ", " ;
    f1S->GetParameters(&par[0]);
    totalS->SetParameters(par);     
    BvsAinACS_XSlope[index]->Fit("totalS","RQNW");
    fitPar1     = totalS->GetParameter(1);
    fitPar2     = totalS->GetParameter(2);
    if(A_DEBUG>1) cout << fitPar1 << ", " << fitPar2 << endl;
    S0NewAreaB = fS0Sigmas * fitPar2;
    S0NewMeanB = fitPar1;

    BvsAinACS_YSlope[index]->Fit("f1S","RQNW");
    fitPar0     = f1S->GetParameter(0);  // constant
    fitPar1     = f1S->GetParameter(1);  // mean
    fitPar2     = f1S->GetParameter(2);  // sigma    
    if(A_DEBUG>1) cout << " BvsAinACS_YSlope[" << index << "]: "
		       << fitPar1 << ", " << fitPar2<< ", " ;    
    f1S->GetParameters(&par[0]);
    totalS->SetParameters(par);     
    BvsAinACS_YSlope[index]->Fit("totalS","RQNW");
    fitPar1     = totalS->GetParameter(1);
    fitPar2     = totalS->GetParameter(2);
    if(A_DEBUG>1) cout << fitPar1 << ", " << fitPar2 << endl;
    S1NewAreaB = fS1Sigmas * fitPar2;
    S1NewMeanB = fitPar1;

    Stat_t entries = fAlignAll->GetEntries();

    HMdcHit* hitA; 
    HMdcHit* hitB; 
    HGeomVector projPoint; 
    Float_t* projSlopes = new Float_t[2];
    Float_t* origSlopes = new Float_t[2];
    HGeomRotation rotaux;
    HGeomVector transaux;

    for (Int_t i=0;i<entries;i++) {     
      fAlignAll->GetEntry(i);
      hitA = (HMdcHit*) fHits->At(0);
      hitB = (HMdcHit*) fHits->At(1);
      //if(fNumMods>2) hitC = (HMdcHit*) fHits->At(2);
      //if(fNumMods>3) hitD = (HMdcHit*) fHits->At(3);
      
      Float_t resInAvsBinBCS_X, resInAvsBinBCS_Y;
      Float_t resInAvsBinBCS_XSlope, resInAvsBinBCS_YSlope;
      Float_t resInBvsAinACS_X, resInBvsAinACS_Y;
      Float_t resInBvsAinACS_XSlope, resInBvsAinACS_YSlope;
   
      //projecting on MDC B
      projPoint = findProjPoint(hitA,fRotMat[0],fTranslation[0],projSlopes);     
      resInAvsBinBCS_X = hitB->getX() - projPoint.getX();
      resInAvsBinBCS_Y = hitB->getY() - projPoint.getY();      
      origSlopes = transformToSlopes(hitB);
      resInAvsBinBCS_XSlope = origSlopes[0] - projSlopes[0];
      resInAvsBinBCS_YSlope = origSlopes[1] - projSlopes[1];
      
      //then projecting on MDC A
      rotaux = fRotMat[0].inverse();
      transaux = -(rotaux * fTranslation[0]);
      projPoint = findProjPoint(hitB,rotaux,transaux,projSlopes);
      resInBvsAinACS_X = hitA->getX() - projPoint.getX();
      resInBvsAinACS_Y = hitA->getY() - projPoint.getY();
      origSlopes = transformToSlopes(hitA);
      resInBvsAinACS_XSlope = origSlopes[0] - projSlopes[0];
      resInBvsAinACS_YSlope = origSlopes[1] - projSlopes[1];
     
      //strong condition: cutting in all histograms 

      if(A_DEBUG>3){
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

      if(fabs(resInAvsBinBCS_X - XNewMeanA) < XNewAreaA && 
	 fabs(resInAvsBinBCS_Y - YNewMeanA) < YNewAreaA &&
	 fabs(resInAvsBinBCS_XSlope - S0NewMeanA) < S0NewAreaA &&
	 fabs(resInAvsBinBCS_YSlope - S1NewMeanA) < S1NewAreaA &&
	 fabs(resInBvsAinACS_X - XNewMeanB) < XNewAreaB && 
	 fabs(resInBvsAinACS_Y - YNewMeanB) < YNewAreaB &&
	 fabs(resInBvsAinACS_XSlope - S0NewMeanB) < S0NewAreaB &&
	 fabs(resInBvsAinACS_YSlope - S1NewMeanB) < S1NewAreaB ) {
	if(fUseSlopeCut){
	  // This cut makes the sample near indep. of Z translations
	  // and results useful for X and Y minimization.
	  // The cut is effective only in MDCB, because fTranslation
	  // is represented in MDCB coordinates. Then, tracks passing
	  // the cut are almost parallel to Z direction
	  if(( fabs(hitB->getXDir()) < fSlopeCut) && 
	     ( fabs(hitB->getYDir()) < fSlopeCut)){
	    if(A_DEBUG>3) cout << " -- CUT PASSED (fSlopeCut=" 
			       << fSlopeCut << " ) --" << endl;
	    new((*fHits)[0])HMdcHit(*hitA);	  
	    new((*fHits)[1])HMdcHit(*hitB);      
	    fAlignAllCut->Fill();
	    fHits->Clear();   	      
	    fCountCut++;
	  }
	}
	else{
	  if(A_DEBUG>3) cout << " ---------  CUT PASSED ------------" << endl;
	  new((*fHits)[0])HMdcHit(*hitA);	  
	  new((*fHits)[1])HMdcHit(*hitB);      
	  fAlignAllCut->Fill();
	  fHits->Clear();   	      
	  fCountCut++;
	}
      }
    }
  }
}






void HMdcAligner::setTree(void)
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
  
  if (fNumMods>3){
    Int_t modC = fLoc[3];
    Int_t modD = fLoc[4];
    sprintf(title,"%s%i%s%i%s%i%s%i%s%i","Sector_",sector,"_ModA_",modA,
	    "_ModB_",modB,"_ModC_",modC,"_ModD_",modD);
    sprintf(tmp,"%s%s%i%s%i%s%i%s%i%s%i","All","_Sector_",sector,
	    "_ModA_",modA,"_ModB_",modB,"_ModC_",modC,"_ModD_",modD);    
    sprintf(tmp2,"%s%s%i%s%i%s%i%s%i%s%i","AllCut","_Sector_",sector,
	    "_ModA_",modA,"_ModB_",modB,"_ModC_",modC,"_ModD_",modD);
  }
  else if(fNumMods>2){
    Int_t modC = fLoc[3];
    sprintf(title,"%s%i%s%i%s%i%s%i","Sector_",sector,"_ModA_",modA,
	    "_ModB_",modB,"_ModC_",modC);
    sprintf(tmp,"%s%s%i%s%i%s%i%s%i","All","_Sector_",sector,
	    "_ModA_",modA,"_ModB_",modB,"_ModC_",modC);    
    sprintf(tmp2,"%s%s%i%s%i%s%i%s%i","AllCut","_Sector_",sector,
	    "_ModA_",modA,"_ModB_",modB,"_ModC_",modC);
  }
  else{  
    sprintf(title,"%s%i%s%i%s%i","Sector_",sector,
	    "_ModA_",modA,"_ModB_",modB);
    sprintf(tmp,"%s%s%i%s%i%s%i","All","_Sector_",sector,
	    "_ModA_",modA,"_ModB_",modB);    
    sprintf(tmp2,"%s%s%i%s%i%s%i","AllCut","_Sector_",sector,
	    "_ModA_",modA,"_ModB_",modB);
  }
  fAlignAll = new TTree(tmp,title);
  fAlignAll->Branch("hits",&fHits);  
  fAlignAllCut = new TTree(tmp2,title);
  fAlignAllCut->Branch("hits",&fHits);
}





Bool_t HMdcAligner::init(void)
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
  fIter4 = (HIterator *)fHitCat->MakeIterator(); 

  setParContainers(); 

  if(fNumMods == 1) return kTRUE;

  if(fHistoOff!=kTRUE) setHistograms();
  else setTree();
 
  return kTRUE;
}





Bool_t HMdcAligner::reinit(void)
{
  //
  // Call the functions returning auxiliar geometrical parameters
  //

  if(fNumMods == 1) return kTRUE; 
  if(fAuto == kFALSE) setGeomAuxPar();
  //Esta linea llamaba a la funcion cuando no debia hacerlo.
  //Afortunadamente no hace nada la funcion sin argumentos.
  //else if(fAuto == kTRUE) setGeomAuxParSim();
  else if(fAuto == kTRUE) ;
  return kTRUE;
}





void HMdcAligner::setGeomParAutoOn(void)
{
  //
  //Turn on the automatic geometrical parameter input
  //Use it for inserting manually the parameters in the macro
  //

  fAuto =kTRUE;  
  cout << "WARNING in HMdcAligner::setGeomParAutoOn(): " 
       << "introducing manually Geometrical" << endl; 
  cout << "Parameters without containers. " 
       << "Parameters should be in the macro" << endl;
}





void HMdcAligner::setControlHistoOff(void)
{  
  //
  // Disables control histograms output (saving memory)
  //

  fHistoOff = kTRUE;  
}





void HMdcAligner::setMinimization(Int_t select=1)
{  
  //
  // Selects minimization strategy
  // 
  // select = 0  Disables minimization
  // select = 1  Angle reduction by minuit
  // select = 2  Analytic translation minimization
  // select = 3  (1+2) Angle reduction+analytic translation
  // select = 4  Translation reduction by minuit
  // select = 5  6 parameters reduction by minuit
  // select = 100 (fcnalign34) Chi square sum in minimization
  // select = 101 (fcnalign34) Distances line-hit
  // 

  fMin = select;  
}





void HMdcAligner::setUnitErrors(void)
{  
  //
  // introduce unit errors in Hits
  //

  fUseUnitErrors = kTRUE;  
}





void HMdcAligner::setTargetOn(HGeomVector target)
{
  //
  // Target position is defined and used for
  // the hit definition in 
  // (WARNING: Use it only for 2 MDCs up to now.
  // It is mandatory to enter in the second position in the
  // constructor the MDC where the target is defined from) 
  //
  
  fUseTarget = kTRUE;
  fTargetPos = target;
}                





void HMdcAligner::setFix(Int_t fix)
{  
  //
  // Fix a parameter set in minimization
  //
  // 1...19 parameter number (Minuit parameters begin in 0, then param+1)
  // 20    fixes translations (both fcn)
  // 30    fixes rotations (both fcn)
  // 40    fixes Z traslations (both fcn)
  // 110   fixes all first and third angles

  fFix = fix;  
}





void HMdcAligner::setNoCut(void)
{  
  //
  // Disables the cuts after fitting the histograms
  //

  fUseCut = kFALSE;  
}





void HMdcAligner::setSlopeCut(Float_t SlopeCut=0.1)
{  
  //
  // Enables the Slope cuts after fitting the histograms.
  // This cut makes the sample near indep. of Z translations
  // and results useful for X and Y minimization.
  // The cut is effective only in MDCB, because fTranslation
  // is represented in MDCB coordinates. Then, tracks passing
  // the cut are almost parallel to Z direction
  //

  fUseSlopeCut = kTRUE;  
  fSlopeCut = SlopeCut;
}

void HMdcAligner::setParContainers(void)
{
  //
  // Loads the parameter containers it uses later
  //

  fMdcGeomPar=(HMdcGeomPar*)gHades->getRuntimeDb()->getContainer("MdcGeomPar");
}

void HMdcAligner::setGeomAuxPar(void)
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
  HGeomVector euler;

  HGeomTransform transformA;
  transformA = fMdcGeomPar->getModule(sector,moduleA)->getLabTransform();
  HGeomTransform transformB;
  transformB = fMdcGeomPar->getModule(sector,moduleB)->getLabTransform();
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
    
    if(A_DEBUG>0){
      cout << endl <<"Debugging output in HMdcAligner::setGeomAuxPar" << endl;
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
   
    if(A_DEBUG>0){
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
   
    if(A_DEBUG>0){
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
   
    if(A_DEBUG>0){
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
    cout << "* HMdcAligner::setGeomAuxPar: from geom params:  *" << endl;
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
    HGeomTransform transformC;
    transformC = fMdcGeomPar->getModule(sector,moduleC)->getLabTransform();
    HGeomRotation rotC;
    rotC = transformC.getRotMatrix();
    HGeomVector vectorC;
    vectorC = transformC.getTransVector();
    
    if(A_DEBUG>0){
      cout << endl <<"Debugging output in HMdcAligner::setGeomAuxPar" << endl;
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
       
    if(A_DEBUG>0){
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
   
    if(A_DEBUG>0){
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
    cout << "* HMdcAligner::setGeomAuxPar: from geom params:  *" << endl;
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

    if(A_DEBUG>0){
      cout << endl <<"Debugging output in HMdcAligner::setGeomAuxPar" << endl;
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
    
    if(A_DEBUG>0){
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
    cout << "* HMdcAligner::setGeomAuxPar: from geom params:  *" << endl;
    cout << "* Sector: "<< sector << "  ModA: " << moduleA 
	 << " ModB: " << moduleB << endl;
    cout << "* Rotation(0): " << fEuler[0].getX() << ", " 
	 << fEuler[0].getY()  << ", " << fEuler[0].getZ() << endl;   
    cout << "* Translation: " << relvector.getX() << ", " 
	 << relvector.getY() << " , " <<  relvector.getZ() << endl;
    cout << "**************************************************" << endl;
  }
}


HGeomVector HMdcAligner::findEulerAngles(HGeomRotation rot){
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
  
  if(A_DEBUG>0){
    cout << endl <<"Euler angles: " << euler[0] << ", " 
	 << euler[1]  << ", " << euler[2] << endl;
  }
  
  HGeomRotation test;  
  test.setEulerAngles(euler[0]*180./TMath::Pi(),
		      euler[1]*180./TMath::Pi(),
		      euler[2]*180./TMath::Pi()); 
  
  if(A_DEBUG>0){
    cout << endl <<"Rotation from Euler angles (first attemp): " << endl;
    test.print();
  }
  
  //Now solving the problem when euler[1]<0
  Double_t eps = 0.0001; //check precision
  
  if( (fabs(test(0)-rot(0))>eps) || (fabs(test(1)-rot(1))>eps) || 
      (fabs(test(3)-rot(3))>eps) || (fabs(test(4)-rot(4))>eps) ) {
    if(A_DEBUG>0) cout << endl << "Bad election in first euler angle! " 
		       << "Trying again. "<< endl;
    euler[1] = - euler[1]; 
    sinaux = sin(euler[1]);
    euler[0] = atan2(rot(5)/sinaux,rot(2)/sinaux);
    euler[2] = atan2(rot(7)/sinaux,-rot(6)/sinaux);
    
    test.setEulerAngles(euler[0]*180./TMath::Pi(),
			euler[1]*180./TMath::Pi(),
			euler[2]*180./TMath::Pi());  
    
    if(A_DEBUG>0){
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





void HMdcAligner::setGeomAuxParSim(Int_t ind, Float_t eu1, Float_t eu2, 
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
  if(A_DEBUG>0){
    cout << "Transformation[" << ind << "]:" << endl;
    cout <<" Euler angles: " << eu1 << ", " 
	 << eu2  << ", " << eu3 << endl;
    cout << " Translation: " << tr1 << ", " 
	 << tr2  << ", " << tr3 << endl;
  }
}





Int_t HMdcAligner::execute(void)
{    
  // 
  // Iteration in the hit category. Fills histograms
  // and TTree and calculates relevant quantities
  //  
  if(fNumMods==1) execute1();
  if(fNumMods==2) execute2();
  if(fNumMods==3) execute3();
  if(fNumMods==4) execute4();
  
  return 0; 
}





void HMdcAligner::execute4(void)
{    
  // 
  // New execute for four MDCs
  // Find the projection of Hits in the most external MDCs 
  // on the inners (besides this can be modified, changing the
  // order of the MDCs in the constructor). Then merge the hits
  // and continue to find till the last MDC. If successfull,
  // fills a TClonesArray of Hits in a TTree for further 
  // analysis and minimization
  // 
  
  HMdcHit* pHitA;   
  HMdcHit* pHitB;   
  HMdcHit* pHitC;   
  HMdcHit* pHitD;
  HMdcHit* pHitCD;
  HMdcHit* pHitBCD;
  HMdcHit* pHitABCD;

  Int_t sector = fLoc[0];
  Int_t modA = fLoc[1];
  Int_t modB = fLoc[2]; 
  Int_t modC = fLoc[3]; 
  Int_t modD = fLoc[4];
  
  HLocation local;
  local.setNIndex(2);
  local.set(2,sector,modD); 
  
  HGeomVector projPoint; 
  Float_t* projSlopes = new Float_t[2];

  Bool_t usedA = kFALSE;   
  Bool_t usedB = kFALSE;
  Bool_t usedC = kFALSE;
  Bool_t invalidA = kFALSE; 
  Bool_t invalidB = kFALSE; 
  Bool_t invalidC = kFALSE; 
  
  fNEntries++;
  
  fIter1->Reset(); 
  fIter1->gotoLocation(local); 
  while ( (pHitD =(HMdcHit*)fIter1->Next()) != 0 ){ 
    
    fHitsMdc[0]++;     //Hits found in MDC D
    usedA = kFALSE;    //reinit control flags
    usedB = kFALSE;
    usedC = kFALSE;
    invalidA = kFALSE; 
    invalidB = kFALSE; 
    invalidC = kFALSE; 
    
    if(A_DEBUG>3)   {
      cout << "    ----- SECTOR " << sector << " -----"<< endl; 
      cout << "Module " << modD << ", fHitsMdc " << fHitsMdc[0] 
	   << endl;
    }  
    
    //   Calculation of cross point and slopes in MDC C  
    // The rotation matrix here used is inverse of fRotMat
    // (see CONVENTION in HMdcAligner::setGeomAuxPar())
    // That is:      X(C) = fRotMat[0]-1 * (X(D) - fTranslation[0])
    // ==> X(C) = fRotMat[0]-1 * X(D) - fRotMat[0]-1 * fTranslation[0]
    // because we are calculating the projection of MDC D Hits on MDC C
    //
    HGeomRotation rotInv = fRotMat[0].inverse();
    HGeomVector trasInv = -(rotInv * fTranslation[0]);
    projPoint = findProjPoint(pHitD,rotInv,trasInv,projSlopes);

    //Iteration on the second MDC (MDCC) for each hit in the first (MDCD)
    local.set(2,sector,modC); 
    fIter2->Reset(); 
    fIter2->gotoLocation(local); 
    while ( (pHitC =(HMdcHit*)fIter2->Next()) != 0 ){ 	

      //hits found in MDCC, provided there is a Hit in MDCD
      fHitsMdc[1]++;     
      
      if(A_DEBUG>3)  
	cout << "Module " << modC << ", fHitsMdc " << fHitsMdc[1] << endl; 
      
      if(isInsideWindow(1,pHitC,projPoint,projSlopes)&&
	 (invalidB!=kTRUE)&&(invalidA!=kTRUE)){
	
	if(usedC == kFALSE){  
	  
	  usedC = kTRUE;
	  // MDCC hits found in window (only used if unique)
	  fHitsFoundInWindow[0]++;
	  fHitsFoundAndUnique[0]++;

	  //merging hits of MDCC and MDCD on MDCD coordinate system
	  pHitCD = mergeHits(pHitD,pHitC,fRotMat[0],fTranslation[0]);
	  
	  //   Calculation of cross point and slopes in MDC B  
	  // The rotation matrix here used is inverse of fRotMat
	  // (see CONVENTION in HMdcAligner::setGeomAuxPar())
	  // That is:      X(B) = fRotMat[1]-1 * (X(D) - fTranslation[1])
	  // ==> X(B) = fRotMat[1]-1 * X(D) - fRotMat[1]-1 * fTranslation[1]
	  // because we are calculating the projection of MDCD Hits on MDCB
	  // (in this case the merging of MDCD and MDCC, but in MDCD 
	  // coordinate system anyway)
	  rotInv = fRotMat[1].inverse();
	  trasInv = -(rotInv * fTranslation[1]);
	  projPoint = findProjPoint(pHitCD,rotInv,trasInv,projSlopes);
	  
	  //Iteration on the third MDC (MDCB) for each couple MDCD-MDCC
	  local.set(2,sector,modB); 
	  fIter3->Reset(); 
	  fIter3->gotoLocation(local); 
	  while ( (pHitB =(HMdcHit*)fIter3->Next()) != 0 ){ 	

	    //hits found in MDCB, provided there is a Hit in MDCD 
	    //and one in MDC. If there is more than one in MDCC the 
	    //number is also increased but the hit will not be used
	    fHitsMdc[2]++;
	      
	    if(A_DEBUG>3)  
	      cout << "Module " << modB << ", fHitsMdc " 
		   << fHitsMdc[2] << endl; 
          
	    if(isInsideWindow(0,pHitB,projPoint,projSlopes)&&(invalidA!=kTRUE)){
	      if(usedB == kFALSE){  
		
		usedB = kTRUE;
		// MDCB hits found in window (only used if unique)
		fHitsFoundInWindow[1]++;
		fHitsFoundAndUnique[1]++;

		//merging hits of MDCB and MDCC-MDCD on MDCD coordinate system
		pHitBCD = mergeHits(pHitCD,pHitB,fRotMat[1],fTranslation[1]);
		
		//   Calculation of cross point and slopes in MDC A  
		// The rotation matrix here used is inverse of fRotMat
		// (see CONVENTION in HMdcAligner::setGeomAuxPar())
		// That is:      X(A) = fRotMat[2]-1 * (X(D) - fTranslation[2])
		// ==> X(A)=fRotMat[2]-1*X(D)-fRotMat[2]-1*fTranslation[2]
		// because we are calculating the projection of MDCD Hits 
		// on MDCA (in this case the merging of MDCD, MDCC and 
		// MDCB, but in MDCD coordinate system anyway)
		rotInv = fRotMat[2].inverse();
		trasInv = -(rotInv * fTranslation[2]);
		projPoint = findProjPoint(pHitBCD,rotInv,trasInv,projSlopes);
		
		//Iteration on the fourth MDC (MDCA) for each MDCD-MDCC-MDCB 
		local.set(2,sector,modA); 
		fIter4->Reset(); 
		fIter4->gotoLocation(local); 
		while ( (pHitA =(HMdcHit*)fIter4->Next()) != 0 ){ 	
		  
		  //hits found in MDCA, provided there is a Hit in MDCD 
		  //MDC and MDCB. If there is more than one in MDCC or MDCB 
		  //the number is also increased but the hit will not be used
		  fHitsMdc[3]++;

		  if(A_DEBUG>3)  
		    cout << "Module " << modA << ", fHitsMdc " 
			 << fHitsMdc[3] << endl;

		  if(isInsideWindow(0,pHitA,projPoint,projSlopes)){
		    if(usedA == kFALSE){  
		      
		      usedA = kTRUE;
		      // MDCA hits found in window (only used if unique)
		      fHitsFoundInWindow[2]++;
		      fHitsFoundAndUnique[2]++;

		      //Real number of matched hits
		      fCount++;

		      //merging all HITS (curiosity or can be used??)
		      pHitABCD = mergeHits(pHitBCD,pHitA,fRotMat[2],
					   fTranslation[2]);
		      
		      //Filling the TClonesArray for storage in TTree
		      //Will be used only if 
		      new((*fHits)[0])HMdcHit(*pHitA);
		      new((*fHits)[1])HMdcHit(*pHitB);	  
		      new((*fHits)[2])HMdcHit(*pHitC);
		      new((*fHits)[3])HMdcHit(*pHitD);	
		      
		    } //  End of  if(usedA == kFALSE){
		    
		    else{          //that is, if usedA == kTRUE
		      //More than one candidate on window!! Discart 
		      if(invalidA == kFALSE){
			fCount--;  
			invalidA = kTRUE;
			fDiscart[2]++;	
			fHitsFoundAndUnique[2]--;
		      }
		    } // End of  else{          //that is, if usedA == kTRUE
		    
		  } // End of  if(isInsideWindow(pHitA,projPoint,projSlopes)){
		} // End of  while( (pHitA =(HMdcHit*)fIter4->Next()) != 0 ){ 
	      } // End of  if(usedB == kFALSE){  
	      
	      else{          //that is, if usedB == kTRUE
		//More than one candidate on window!! Discart 
		if(invalidB == kFALSE){
		  invalidB = kTRUE;
		  fDiscart[1]++;	
		  fHitsFoundAndUnique[1]--;
		}
	      } // End of  else{          //that is, if usedB == kTRUE
	      
	    } // End of  if(isInsideWindow(pHitB,projPoint,projSlopes)){
	  } // End of  while( (pHitB =(HMdcHit*)fIter3->Next()) != 0 ){ 
	} // End of if(usedC == kFALSE){  

	else{          //that is, if usedC == kTRUE
	  //More than one candidate on window!! Discart 
	  if(invalidC == kFALSE){
	    invalidC = kTRUE;
	    fDiscart[0]++;	
	    fHitsFoundAndUnique[0]--;
	  }
	} // End of  else{          //that is, if usedC == kTRUE

      } // End of  if(isInsideWindow(pHitC,projPoint,projSlopes)){
    } // End of  while( (pHitC =(HMdcHit*)fIter2->Next()) != 0 ){ 

    if(usedC == kTRUE && invalidC != kTRUE && 
       usedB == kTRUE && invalidB != kTRUE && 
       usedA == kTRUE && invalidA != kTRUE){
      fAlignAll->Fill();
      fHits->Clear();
      if(fCount%100 ==0) cout << "."<< flush;
    }
    
  } // End of  while ( (pHitD =(HMdcHit*)fIter1->Next()) != 0 ){ 
}
  


void HMdcAligner::execute3(void)
{    
  // 
  // New execute for more than two MDCs
  //
  
  HMdcHit* pHitA;         
  HMdcHit* pHitB;       
  HMdcHit* pHitC; 
  HMdcHit* pHitBC;
  HMdcHit* pHitABC;

  Int_t sector = fLoc[0];
  Int_t modA = fLoc[1];
  Int_t modB = fLoc[2]; 
  Int_t modC = fLoc[3]; 

  HLocation local;
  local.setNIndex(2);
  local.set(2,sector,modC); 
  
  HGeomVector projPoint; 
  Float_t* projSlopes = new Float_t[2];

  Bool_t usedA = kFALSE;   
  Bool_t usedB = kFALSE;
  Bool_t invalidA = kFALSE; 
  Bool_t invalidB = kFALSE; 

  fNEntries++;

  fIter1->Reset(); 
  fIter1->gotoLocation(local); 
  while ( (pHitC =(HMdcHit*)fIter1->Next()) != 0 ){ 
    
    fHitsMdc[0]++;     //Hits found in MDC C
    usedA = kFALSE;    //reinit control flags
    usedB = kFALSE;
    invalidA = kFALSE; 
    invalidB = kFALSE;  
    
    if(A_DEBUG>3)   {
      cout << "     ----- SECTOR " << sector << " -----"<< endl; 
      cout << "Module " << modC << ", fHitsMdc " << fHitsMdc[0] 
	   << endl;
    }  
    
    //   Calculation of cross point and slopes in MDC B  
    // The rotation matrix here used is inverse of fRotMat
    // (see CONVENTION in HMdcAligner::setGeomAuxPar())
    // That is:      X(B) = fRotMat[0]-1 * (X(C) - fTranslation[0])
    // ==> X(B) = fRotMat[0]-1 * X(C) - fRotMat[0]-1 * fTranslation[0]
    // because we are calculating the projection of MDC C Hits on MDC B
    //
    HGeomRotation rotInv = fRotMat[0].inverse();
    HGeomVector trasInv = -(rotInv * fTranslation[0]);
    projPoint = findProjPoint(pHitC,rotInv,trasInv,projSlopes);

    //Iteration on the second MDC (MDCB) for each hit in the first (MDCC)
    local.set(2,sector,modB); 
    fIter2->Reset(); 
    fIter2->gotoLocation(local); 
    while ( (pHitB =(HMdcHit*)fIter2->Next()) != 0 ){ 	

      //hits found in MDCB, provided there is a Hit in MDCC
      fHitsMdc[1]++;     
    
      if(A_DEBUG>3)  
	cout << "Module " << modB << ", fHitsMdc " << fHitsMdc[1] << endl; 
         
      if(isInsideWindow(1,pHitB,projPoint,projSlopes)&&(invalidA!=kTRUE)){

	if(usedB == kFALSE){  

	  usedB = kTRUE;
	  // MDCB hits found in window (only used if unique)
	  fHitsFoundInWindow[0]++;
	  fHitsFoundAndUnique[0]++;

	  //merging hits of MDCB and MDCC on MDCC coordinate system
	  pHitBC = mergeHits(pHitC,pHitB,fRotMat[0],fTranslation[0]);
	  
	  //   Calculation of cross point and slopes in MDC A  
	  // The rotation matrix here used is inverse of fRotMat
	  // (see CONVENTION in HMdcAligner::setGeomAuxPar())
	  // That is:      X(A) = fRotMat[1]-1 * (X(C) - fTranslation[1])
	  // ==> X(A) = fRotMat[1]-1 * X(C) - fRotMat[1]-1 * fTranslation[1]
	  // because we are calculating the projection of MDCC Hits on MDCA
	  // (in this case the merging of MDCC and MDCB, but in MDCC 
	  // coordinate system anyway)
	  rotInv = fRotMat[1].inverse();
	  trasInv = -(rotInv * fTranslation[1]);
	  projPoint = findProjPoint(pHitBC,rotInv,trasInv,projSlopes);
	
	  //Iteration on the third MDC (MDCA) for each couple MDCC-MDCB
	  local.set(2,sector,modA); 
	  fIter3->Reset(); 
	  fIter3->gotoLocation(local); 
	  while ( (pHitA =(HMdcHit*)fIter3->Next()) != 0 ){ 	

	    //hits found in MDCA, provided there is a Hit in MDCC 
	    //and one in MDB. If there is more than one in MDCB the 
	    //number is also increased but the hit will not be used
	    fHitsMdc[2]++;
	    
	    if(A_DEBUG>3)  
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
		pHitABC = mergeHits(pHitBC,pHitA,fRotMat[1],
				     fTranslation[1]);
		
		//Filling the TClonesArray for storage in TTree
		//Will be used only if 
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
    } // End of  while( (pHitB =(HMdcHit*)fIter2->Next()) != 0 ){ 
    
    if(usedB == kTRUE && invalidB != kTRUE && 
       usedA == kTRUE && invalidA != kTRUE){
      fAlignAll->Fill();
      fHits->Clear();
      if(fCount%100 ==0) cout << "."<< flush;
    }
    
  } // End of  while ( (pHitC =(HMdcHit*)fIter1->Next()) != 0 ){ 
}





void HMdcAligner::execute2(void)
{    
  // 
  // Adapting the old execute
  //

  HMdcHit* pHitA; 
  HMdcHit* pHitB; 
  HMdcHit* pHitAB;

  Int_t sector = fLoc[0];
  Int_t modA = fLoc[1];
  Int_t modB = fLoc[2]; 
 
  HLocation local;
  local.setNIndex(2);
  local.set(2,sector,modB); 

  HGeomVector projPoint; 
  Float_t* projSlopes = new Float_t[2];

  Bool_t usedA = kFALSE; 
  Bool_t invalidA = kFALSE;

  fNEntries++;

  fIter1->Reset(); 
  fIter1->gotoLocation(local); 
  while ( (pHitB =(HMdcHit*)fIter1->Next()) != 0 ){ 

    fHitsMdc[0]++;   //Hits found in MDC B
    usedA = kFALSE;                   //reinit control flag
    invalidA = kFALSE;               
    
    if(A_DEBUG>3)   {
      cout << "     ----- SECTOR " << sector << " -----"<< endl; 
      cout << "Module " << modB << ", fHitsMdc " << fHitsMdc[0] 
	   << endl;
    }  
    
    //   Calculation of cross point and slopes in MDC A  
    // The rotation matrix here used is inverse of fRotMat
    // (see CONVENTION in HMdcAligner::setGeomAuxPar())
    // That is:      X(A) = fRotMat[0]-1 * (X(B) - fTranslation[0])
    // ==> X(A) = fRotMat[0]-1 * X(B) - fRotMat[0]-1 * fTranslation[0]
    // because we are calculating the projection of MDC B Hits on MDC A
    //
    HGeomRotation rotInv = fRotMat[0].inverse();
    HGeomVector trasInv = -(rotInv * fTranslation[0]);
    projPoint = findProjPoint(pHitB,rotInv,trasInv,projSlopes);
    
    //Iteration on the second MDC (MDCA) for each hit in the first (MDCB)
    local.set(2,sector,modA); 
    fIter2->Reset(); 
    fIter2->gotoLocation(local); 
    while ( (pHitA =(HMdcHit*)fIter2->Next()) != 0 ){ 	

      //hits found in MDCA, provided there is a Hit in MDCB
      fHitsMdc[1]++;     
    
      if(A_DEBUG>3)  
	cout << "Module " << modA << ", fHitsMdc " 
	     << fHitsMdc[1] << endl; 
   
      if(isInsideWindow(1,pHitA,projPoint,projSlopes)){
	
	if(usedA == kFALSE){  
	  
	  usedA = kTRUE;
	  // MDCA hits found in window (only used if unique)
	  fHitsFoundInWindow[0]++;
	  fHitsFoundAndUnique[0]++;
	  
	  //Real number of matched hits
	  fCount++;
	  
	  //merging all HITS (curiosity or can be used??)
	  pHitAB = mergeHits(pHitB,pHitA,fRotMat[0],
			     fTranslation[0]);
	  
	  //Filling the TClonesArray for storage in TTree
	  //Will be used only if 
	  new((*fHits)[0])HMdcHit(*pHitA);	  
	  new((*fHits)[1])HMdcHit(*pHitB);
	  
	} //  End of  if(usedA == kFALSE){
	
	else{          //that is, if usedA == kTRUE
	  //More than one candidate on window!! Discart 
	  if(invalidA == kFALSE){
	    fCount--;  
	    invalidA = kTRUE;
	    fDiscart[0]++;	
	    fHitsFoundAndUnique[0]--;
	  }
	} // End of  else{          //that is, if usedA == kTRUE
	      
      } // End of  if(isInsideWindow(pHitA,projPoint,projSlopes)){
    } // End of  while( (pHitA =(HMdcHit*)fIter2->Next()) != 0 ){ 
    
    if(usedA == kTRUE && invalidA != kTRUE){
      fAlignAll->Fill();
      fHits->Clear();
      if(fCount%100 ==0) cout << "."<< flush;
    }
    
  } // End of  while ( (pHitB =(HMdcHit*)fIter1->Next()) != 0 ){ 
} 





void HMdcAligner::execute1(void)
{    
  // 
  // Only one MDC. Trying to find out the point where all
  // tracks point to. 
  //

  HMdcHit* pHitA; 

  Int_t sector = fLoc[0];
  Int_t modA = fLoc[1];
 
  HLocation local;
  local.setNIndex(2);
  local.set(2,sector,modA); 

  fIter1->Reset(); 
  fIter1->gotoLocation(local); 
  while ( (pHitA =(HMdcHit*)fIter1->Next()) != 0 ){ 

    //Enter here cuts if necessary

    new((*fHits)[0])HMdcHit(*pHitA);	
  
    fAlignAll->Fill();
    fHits->Clear();

  } // End of  while ( (pHitA =(HMdcHit*)fIter1->Next()) != 0 ){ 
}
 




void HMdcAligner::transfEuler(HGeomRotation eulrot,HGeomVector eulvec,
			       HGeomVector oldV, HGeomVector newV){
   //
   // Transformation from one coordinate system to a new one
   //     newV = eulrot * oldV + eulvec
   // (NOT USED)
   newV = eulrot * oldV + eulvec;
 }




 
 void HMdcAligner::transfEulerInv(HGeomRotation eulrot,HGeomVector eulvec,
				  HGeomVector oldV, HGeomVector newV){
   //
  // Transformation from one coordinate system to a new one
  //     newV = eulrot * (oldV - eulvec)
  // (NOT USED)
  newV = eulrot * (oldV - eulvec);
}
 
 



HGeomVector HMdcAligner::findProjPoint(HMdcHit* pHit, HGeomRotation rot,
				       HGeomVector tra, Float_t* slopes)
{
  //
  // Find the projection point of a Hit on another MDC
  //
  // Given a relative rotation and translation from MDC A to MDC B
  // (see CONVENTION in HMdcAligner::setGeomAuxPar())
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
  
  // 15/11/01: Introducing the possibility of target-MDC
  // definition of the track, instead of Hit alone
  if(fUseTarget) {
    if(pHit->getModule() != fLoc[2]){
      // the target is finded and defined for MDC B.
      // If MDC requires to use the target, then should
      // be calculated using fTargetPos and the relative
      // transformation between the modules
      HGeomVector targetInOtherMdc,transaux;
      HGeomRotation rotaux;
      rotaux = rot.inverse();
      transaux = -(rotaux * tra);
      targetInOtherMdc = rotaux * fTargetPos + transaux;
      s0 = (x-targetInOtherMdc.getX())/(-targetInOtherMdc.getZ());
      s1 = (y-targetInOtherMdc.getY())/(-targetInOtherMdc.getZ());
    }
    else{
      s0 = (x-fTargetPos.getX())/(-fTargetPos.getZ());
      s1 = (y-fTargetPos.getY())/(-fTargetPos.getZ());
    }
  }                 
  else{
    xDir = pHit->getXDir(); 
    yDir = pHit->getYDir();
    aux = sqrt(1 - xDir * xDir - yDir * yDir);    
    if(aux == 0.){ s0=1; s1=1;} //non-sense values
    else{
      s0 = xDir/aux;
      s1 = yDir/aux;
    }
  }     
 
  if(A_DEBUG>3){
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
    cout << "ERROR in HMdcAligner::findProjPoint()" << endl;
    return newvec;
  }  
  slopes[0] = (s0*rot(0) + s1*rot(1) + rot(2)) / den;
  slopes[1] = (s0*rot(3) + s1*rot(4) + rot(5)) / den;

  if(A_DEBUG>3){
    cout << "Projected MDC HIT: " << xsearch << " " << ysearch 
	 <<  " " << slopes[0] << " " << slopes[1] << endl;
  }

  newvec.setX(xsearch);
  newvec.setY(ysearch);
  newvec.setZ(zsearch);
  return newvec;
}





Bool_t HMdcAligner::isInsideWindow(Int_t plot, HMdcHit* pHit, HGeomVector point,
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

  if(plot){
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

  if(A_DEBUG>3) cout << "MDC HIT: " 
		     << pHit->getX() << " " << pHit->getY();
  
  if( (pHit->getX()>xlolimit) && (pHit->getX()<xuplimit) && 
      (pHit->getY()>ylolimit) && (pHit->getY()<yuplimit)) {
    if(A_DEBUG>3) cout <<  "  inside window" << endl;
    return kTRUE;
  }

  else {    
    if(A_DEBUG>3) cout <<  "  outside window" << endl;
    return kFALSE;
  }

  /*
  //New check
  Float_t corr2X = 0.0484; //(corr x-x')^2 from Bea paper
  Float_t corr2Y = 0.0841; //corr y-y')^2 from Bea paper
  Float_t sigma2X = 0.04; //(sigma(X))^2 from Bea paper
  Float_t sigma2Y = 0.01; //(sigma(Y))^2 from Bea paper
  Float_t sigma2S0 = 0.01; //(sigma(S0))^2 from Bea paper
  Float_t sigma2S1 = 0.01; //(sigma(S1))^2 from Bea paper
  Float_t alpha = 3.29; //0.999%
  Float_t resX2 = (x - point.getX())(x - point.getX());
  Float_t resY2 = (y - point.getY())(y - point.getY());
  Float_t resS02 = (s0 - slope[0]) * (s0 - slope[0]);
  Float_t resS12 = (s1 - slope[1]) * (s1 - slope[1]);
  Float_t partX = (1/(1-corr2X)) * ( (resX2/sigma2X) + (resS02/sigma2S0) 
			     - (2*(x - point.getX())*(s0 - slope[0])*
				corr2X/(sigma2X*sigma2S0)) );
  Float_t partY = (1/(1-corr2Y)) * ( (resY2/sigma2Y) + (resS12/sigma2S1) 
			     - (2*(y - point.getY())*(s1 - slope[1])*
				corr2Y/(sigma2Y*sigma2S1)) );
  if(partX+partY < alpha) {    
    if(A_DEBUG>3) cout <<  "  inside window" << endl;
    return kTRUE;
  }
  else {    
    if(A_DEBUG>3) cout <<  "  outside window" << endl;
    return kFALSE;
  }
  */
}



HMdcHit* HMdcAligner::mergeHits(HMdcHit* hitB, HMdcHit* hitA,
				 HGeomRotation rot,HGeomVector tra){
  //
  // Propagates hitA in hitB coordinate system and merges 
  // the information in a new hit in hitB coordinate system
  // The rot and tra verifies the CONVENTION 
  // (see HMdcAligner::setGeomAuxPar()), that is:
  //     X(B) = rot X(A) + tra
  // Normally B is reference MDC, which is farther from target

  //HMdcHit* pHit = new HMdcHit();

  //Now, only mean values. Later, propagate the covariance and add the
  //multiple scattering effect on hitA
  /*
  pHit->setX(,);
  pHit->setY(,);  
  pHit->setXDir(,);
  pHit->setYDir(,);

  return pHit;
  */
  //Only for test purposes, return hitB
  return hitB;
}





Float_t* HMdcAligner::transformToSlopes(HMdcHit* pHit){
  //
  //Transform hit angular components in slopes
  //
  
  Float_t* slopes = new Float_t[2];
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
  return slopes;
}





void HMdcAligner::findAbsolutePosition(HGeomRotation* rot, HGeomVector* vect){
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
  // are the relative transformations we know (see HMdcAligner)
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
  Int_t modC=-1;
  Int_t modD=-1;
  if(fNumMods>2) modC = fLoc[3];
  if(fNumMods>3) modD = fLoc[4];
  
  HGeomTransform transformA,transformB,transformC,transformD;
  transformA = fMdcGeomPar->getModule(sector,modA)->getLabTransform();
  HGeomRotation rotOrigA,rotOrigB,rotOrigC,rotOrigD;
  rotOrigA = transformA.getRotMatrix();
  HGeomVector vectorOrigA,vectorOrigB,vectorOrigC,vectorOrigD;
  vectorOrigA = transformA.getTransVector();    
  
  transformB = fMdcGeomPar->getModule(sector,modB)->getLabTransform();    
  rotOrigB = transformB.getRotMatrix();
  vectorOrigB = transformB.getTransVector();        
  
  if(fNumMods>2){
    transformC = fMdcGeomPar->getModule(sector,modC)
      ->getLabTransform(); 
    rotOrigC = transformC.getRotMatrix();    
    vectorOrigC = transformC.getTransVector(); 
  }
  if(fNumMods>3){ 
    transformD = fMdcGeomPar->getModule(sector,modD)
      ->getLabTransform(); 
    rotOrigD = transformD.getRotMatrix();    
    vectorOrigD = transformD.getTransVector(); 
  }
  
  if(fNumMods>3) {   
    rot[0] = rotOrigD * fRotMat[0];
    vect[0] = rotOrigD * fTranslation[0] + vectorOrigD;
    rot[1] = rotOrigD * fRotMat[1];
    vect[1] = rotOrigD * fTranslation[1] + vectorOrigD;     
    rot[2] = rotOrigD * fRotMat[2];
    vect[2] = rotOrigD * fTranslation[2] + vectorOrigD;    
    if(A_DEBUG >2){
      rot[0].print();
      vect[0].print();      
      rot[1].print();
      vect[1].print();      
      rot[2].print();
      vect[2].print();
    }
  }
  else if(fNumMods>2){
    rot[0] = rotOrigC * fRotMat[0];
    vect[0] = rotOrigC * fTranslation[0] + vectorOrigC;     
    rot[1] = rotOrigC * fRotMat[1];
    vect[1] = rotOrigC * fTranslation[1] + vectorOrigC;    
    if(A_DEBUG >2){
      rot[0].print();
      vect[0].print();      
      rot[1].print();
      vect[1].print();
    }
  }
  else{
    rot[0] = rotOrigB * fRotMat[0];
    vect[0]  = rotOrigB * fTranslation[0] + vectorOrigB;
    if(A_DEBUG >2){
      rot[0].print();
      vect[0].print();
    }
  }
}





Bool_t HMdcAligner::finalize(void)
{   
  // 
  // Statistical information and Minimization procedure
  //

  //first the vertex finder for 1 MDC
  if(fNumMods==1) {  
    Int_t sector = fLoc[0];
    Int_t modA = fLoc[1];
    HGeomVector aPoint; 
    HGeomVector aVector;
    HGeomVector theTarget;
    HGeomVector oldTarget;
    Float_t third;
    Float_t part1, part2, part3, numera,denomi, distRel, disTarget;
    Float_t weigth = 1;
    Float_t t; 
    //    Float_t Factor = 0.1;
    Float_t angularError = 0.05 ; // around 3 degrees
    HMdcHit* pHitA;  
    Stat_t entries; 
    Int_t cont=0;

    
    //The Manuel's Vertex finder applied to all the events
    HGeomVertexFit* targetFinder= new HGeomVertexFit();  
    targetFinder->reset();

    entries = fAlignAll->GetEntries();

    for (Int_t i=0;i<entries;i++) { 
      
      fAlignAll->GetEntry(i);
      pHitA = (HMdcHit*) fHits->At(0);
      
      aPoint.setX(pHitA->getX());
      aPoint.setY(pHitA->getY());
      aPoint.setZ(0.);
      
      third = sqrt(1-pHitA->getXDir()*pHitA->getXDir()
		   -pHitA->getYDir()*pHitA->getYDir());
      
      aVector.setX(pHitA->getXDir());
      aVector.setY(pHitA->getYDir());
      aVector.setZ(third);
      cont++;

      //Add the Hit with weigth 1
      targetFinder->addLine(aPoint,aVector,1);       
    }
    targetFinder->getVertex(theTarget);
    cout << "###### TARGET FINDER RESULT #######" << endl;
    cout << "Init Value (no weigth)" << endl;
    cout << " Sector: "  << sector 
	 << "  Module: " << modA << endl;
    theTarget.print();
    cout << "Entries with weigth: " << cont;     

    while(fConstTukey>0.4){
      targetFinder->reset();
      cout << endl << "fConstTukey = " <<  fConstTukey << endl;
      
      //recursive while does not converge
      do{    
	targetFinder->reset();
	oldTarget = theTarget;
	cont =0;

	for (Int_t i=0;i<entries;i++) { 
	  fAlignAll->GetEntry(i);
	  pHitA = (HMdcHit*) fHits->At(0);
	  
	  aPoint.setX(pHitA->getX());
	  aPoint.setY(pHitA->getY());
	  aPoint.setZ(0.);
	  
	  third = sqrt(1-pHitA->getXDir()*pHitA->getXDir()
		       -pHitA->getYDir()*pHitA->getYDir());
	  
	  aVector.setX(pHitA->getXDir());
	  aVector.setY(pHitA->getYDir());
	  aVector.setZ(third);

	  //cout <<"point and vector are:"<< endl;
	  //aPoint.print();
	  //aVector.print();
  
	  //finding weigths    
	  part1 = ((theTarget.getX()-aPoint.getX())*(aVector.getY())) 
	    - ((theTarget.getY()-aPoint.getY())*(aVector.getX()));
	  part2 = ((theTarget.getY()-aPoint.getY())*(aVector.getZ())) 
	    - ((theTarget.getZ()-aPoint.getZ())*(aVector.getY()));
	  part3 = ((theTarget.getZ()-aPoint.getZ())*(aVector.getX())) 
	    - ((theTarget.getX()-aPoint.getX())*(aVector.getZ()));
	  numera = (part1*part1)+(part2*part2)+(part3*part3);
	  denomi = aVector.getX()*aVector.getX() 
	    + aVector.getY()*aVector.getY() 
	    + aVector.getZ()*aVector.getZ(); 
	  disTarget = sqrt(numera/denomi); 
	
	  //cout << "dist. to target: " <<  disTarget << cout;

	  //An exponential weigth
	  //	weigth = exp(-Factor*disTarget);
	  
	  //Tukey bisquared weigths (NIM A394(1997)225)
	  //the error is an approx. angular error (rads)
	  //times the approx. distance to the target
	  t = disTarget/(angularError*theTarget.getZ());
	  if(fabs(t)<fConstTukey){
	    weigth = (1-t*t/(fConstTukey*fConstTukey)) 
	      * (1-t*t/(fConstTukey*fConstTukey));
	    cont++;
	  }
	  else weigth = 0;

	  //cout << "t: " << t << "weigth: " <<  weigth<< endl;

	  //Add the Hit with weigth 
	  targetFinder->addLine(aPoint,aVector,weigth);       
	}
	targetFinder->getVertex(theTarget);
	theTarget.print();    
	cout << "Entries with weigth: " << cont;     

	distRel = sqrt( (theTarget.getX()-oldTarget.getX())*
			(theTarget.getX()-oldTarget.getX()) +
			(theTarget.getY()-oldTarget.getY())*
			(theTarget.getY()-oldTarget.getY()) +
			(theTarget.getZ()-oldTarget.getZ())*
			(theTarget.getZ()-oldTarget.getZ()) );
	//  cout << "     " << distRel ;   
      }while(distRel>0.001);    

      if(fConstTukey>1)fConstTukey = fConstTukey-1.;
      else fConstTukey = fConstTukey -0.5; 
    }
    return kTRUE;
  }
  
  Int_t sector = fLoc[0];
  Int_t modA = fLoc[1];
  Int_t modB = fLoc[2];
  Int_t modC=-1;
  Int_t modD=-1;
  if(fNumMods>2) modC = fLoc[3];
  if(fNumMods>3) modD = fLoc[4];
  
  //first statistical information
  ofstream *fout=NULL;
  if (fNameAscii) fout = new ofstream(fNameAscii.Data(), ios::app);
  if (*fout){
    *fout << endl << "Sector: " << sector << endl;
    *fout << "Module A: " << modA << "  Module B: " << modB ;
    if(fNumMods>2) *fout << "  Module C: " << modC;    
    if(fNumMods>3) *fout << "  Module D: " << modD;
    *fout << endl << endl; 
    *fout << "Number of events: "  << fNEntries << endl;  
    *fout << "Window (mm): " << fXArea <<"," << fYArea << endl;
    *fout << "Interpret smaller MDC index as farther away from target" 
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
  if(A_DEBUG>0){
    cout << endl << "Sector: " << sector << endl;
    cout << "Module A: " << modA << "  Module B: " << modB ;
    if(fNumMods>2) cout << "  Module C: " << modC;    
    if(fNumMods>3) cout << "  Module D: " << modD;
    cout << endl << endl; 
    cout << "Number of events: "  << fNEntries << endl;  
    cout << "Window (mm): " << fXArea <<"," << fYArea << endl;
    cout << "Interpret smaller MDC index as farther away from target" 
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
    *fout << "Interpret smaller MDC index as farther away from target" 
	  << endl << endl;
    for(Int_t i=0;i<fNumMods-1;i++){
      *fout << "Module" << i <<  endl;      
      for(Int_t j=0;j<9;j++) *fout << absRot[i](j) << "   " ;
      *fout << endl;
      for(Int_t j=0;j<3;j++) *fout << absVect[i](j) << "   " ;
      *fout << endl << endl;
    }
  }
  if(A_DEBUG>0){
    cout << endl << "Individual transformations before " 
	 << "minimization (init values): "  << endl;	
    cout << "Interpret smaller MDC index as farther away from target" 
	 << endl << endl;
    for(Int_t i=0;i<fNumMods-1;i++){
      cout << "Module" << i <<  endl;      
      absRot[i].print();
      absVect[i].print();
    }
  }

  //filling common histograms
  fillHistograms(0);  
  fitHistograms(0);
  fillHistograms(1,1);  
  if (*fout) *fout << "Valid hits for alignment after cuts: " 
		   << fCountCut << endl << endl;
  if(A_DEBUG>0) cout << "Valid hits for alignment after cuts: " 
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
   
  //minimization strategy
  if(!fMin){ //if no minimization is performed  
    if (*fout) *fout << "Minimization strategy = 0: No minimization" << endl;
    storeInFile();
    fout->close();
    delete fout;
    return kTRUE; 
  }
  
  if(fMin && fMin!=2){ //Minuit
    if (*fout) *fout << "Minimization strategy: " << fMin <<" with MINUIT" << endl;
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
	
	if(A_DEBUG==0){     
	  cout << i << "IterCri: " <<   IterCri << endl;
	}
      }
      IterCounter++;
      if(IterCounter>10) {
	cout << "WARNING in HMdcAligner :: finalize" << endl;
	cout << "Sector: " << sector << " ModuleA: " 
	     << modA <<  " ModuleB: " << modB << endl;
	cout << "More than 10 iterations without results!" <<endl;
	break;
      }
    }while(IterCri>fIterCriteria);
  }

  if(fMin==2||fMin==3){ //Analytic translation minimization
    HGeomVector result;
    result = translationFinder();
    fillTranslation(0,result.getX(),result.getY(),result.getZ());
    if (*fout){
      *fout << "Parameters after ANALYTIC translation minimization "  << endl;
      for(Int_t i=0;i<fNumMods-1;i++){
	*fout << fEuler[i].getX() << ",  " << fEuler[i].getY() << ",  " 
	      << fEuler[i].getZ() << ",  " << fTranslation[i].getX() << ",  " 
	      << fTranslation[i].getY() << ",  " << fTranslation[i].getZ() << endl;
      }	
      *fout << endl;	
    }
  }

  //getting absolute parameters
  findAbsolutePosition(absRot,absVect);  
  if(*fout){    
    *fout << endl <<"Individual transformations in " 
	  << "minimization (init values): "  << endl;	  
    *fout << "Interpret smaller MDC index as farther away from target" 
	  << endl << endl;
    for(Int_t i=0;i<fNumMods-1;i++){
      *fout << "Module" << i <<  endl;      
      for(Int_t j=0;j<9;j++) *fout << absRot[i](j) << "   " ;
      *fout << endl;
      for(Int_t j=0;j<3;j++) *fout << absVect[i](j) << "   " ;
      *fout << endl << endl;
    }
  }    
  if(A_DEBUG>0){
    cout << endl << "Individual transformations in " 
	 << "minimization: "  << endl;  
    cout << "Interpret smaller MDC index as farther away from target"  
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





void HMdcAligner::fillHistograms (Int_t index, Int_t select = 0){ 
  //
  // Performs the graphical output from obtained parameters
  //   

  HMdcHit* hitA;      
  HMdcHit* hitB;      
  HMdcHit* hitC=NULL;      
  HMdcHit* hitD=NULL; 
  HGeomVector projPoint;
  Float_t* projSlopes = new Float_t[2];
  Float_t* origSlopes = new Float_t[2];
  HGeomRotation rotaux;
  HGeomVector transaux;
  HGeomVector transf[4];  
  HGeomVector a,b,c,d;
  Float_t errorx[4]; 
  Float_t errory[4];
  Stat_t entries; 
  
  if(select != 0) entries = fAlignAllCut->GetEntries();
  else entries = fAlignAll->GetEntries();

  for (Int_t i=0;i<entries;i++) { 
    if(select != 0) fAlignAllCut->GetEntry(i);  
    else fAlignAll->GetEntry(i); 
    hitA = (HMdcHit*) fHits->At(0);
    hitB = (HMdcHit*) fHits->At(1);
    if(fNumMods>2) hitC = (HMdcHit*) fHits->At(2);
    if(fNumMods>3) hitD = (HMdcHit*) fHits->At(3);
    

    //Constructing all possible projections
    //The histos represent (value of local hit - value of projected hit)

    if(fNumMods==4) {
      //first projecting on MDC D
      projPoint = findProjPoint(hitC,fRotMat[0],fTranslation[0],projSlopes);
      CvsDinDCS_X[index]->Fill(hitD->getX() - projPoint.getX());
      CvsDinDCS_Y[index]->Fill(hitD->getY() - projPoint.getY());
      origSlopes = transformToSlopes(hitD);
      CvsDinDCS_XSlope[index]->Fill(origSlopes[0] - projSlopes[0]);
      CvsDinDCS_YSlope[index]->Fill(origSlopes[1] - projSlopes[1]);

      projPoint = findProjPoint(hitB,fRotMat[1],fTranslation[1],projSlopes);
      BvsDinDCS_X[index]->Fill(hitD->getX() - projPoint.getX());
      BvsDinDCS_Y[index]->Fill(hitD->getY() - projPoint.getY());
      BvsDinDCS_XSlope[index]->Fill(origSlopes[0] - projSlopes[0]);
      BvsDinDCS_YSlope[index]->Fill(origSlopes[1] - projSlopes[1]);

      projPoint = findProjPoint(hitA,fRotMat[2],fTranslation[0],projSlopes);
      AvsDinDCS_X[index]->Fill(hitD->getX() - projPoint.getX());
      AvsDinDCS_Y[index]->Fill(hitD->getY() - projPoint.getY());
      AvsDinDCS_XSlope[index]->Fill(origSlopes[0] - projSlopes[0]);
      AvsDinDCS_YSlope[index]->Fill(origSlopes[1] - projSlopes[1]);

      //second projecting on MDC C
      rotaux = fRotMat[0].inverse();
      transaux = -(rotaux * fTranslation[0]);
      projPoint = findProjPoint(hitD,rotaux,transaux,projSlopes);
      DvsCinCCS_X[index]->Fill(hitC->getX() - projPoint.getX());
      DvsCinCCS_Y[index]->Fill(hitC->getY() - projPoint.getY());
      origSlopes = transformToSlopes(hitC);
      DvsCinCCS_XSlope[index]->Fill(origSlopes[0] - projSlopes[0]);
      DvsCinCCS_YSlope[index]->Fill(origSlopes[1] - projSlopes[1]);

      rotaux = (fRotMat[0].inverse())*fRotMat[1];  
      transaux = (fRotMat[0].inverse())*(fTranslation[1]-fTranslation[0]);
      projPoint = findProjPoint(hitB,rotaux,transaux,projSlopes);
      BvsCinCCS_X[index]->Fill(hitC->getX() - projPoint.getX());
      BvsCinCCS_Y[index]->Fill(hitC->getY() - projPoint.getY());
      BvsCinCCS_XSlope[index]->Fill(origSlopes[0] - projSlopes[0]);
      BvsCinCCS_YSlope[index]->Fill(origSlopes[1] - projSlopes[1]);
      
      rotaux = (fRotMat[0].inverse())*fRotMat[2];  
      transaux = (fRotMat[0].inverse())*(fTranslation[2]-fTranslation[0]);
      projPoint = findProjPoint(hitA,rotaux,transaux,projSlopes);
      AvsCinCCS_X[index]->Fill(hitC->getX() - projPoint.getX());
      AvsCinCCS_Y[index]->Fill(hitC->getY() - projPoint.getY());
      AvsCinCCS_XSlope[index]->Fill(origSlopes[0] - projSlopes[0]);
      AvsCinCCS_YSlope[index]->Fill(origSlopes[1] - projSlopes[1]);

      //third projecting on MDC B       
      rotaux = fRotMat[1].inverse();
      transaux = -(rotaux * fTranslation[1]);
      projPoint = findProjPoint(hitD,rotaux,transaux,projSlopes);
      DvsBinBCS_X[index]->Fill(hitB->getX() - projPoint.getX());
      DvsBinBCS_Y[index]->Fill(hitB->getY() - projPoint.getY());
      origSlopes = transformToSlopes(hitB);
      DvsBinBCS_XSlope[index]->Fill(origSlopes[0] - projSlopes[0]);
      DvsBinBCS_YSlope[index]->Fill(origSlopes[1] - projSlopes[1]);

      rotaux = (fRotMat[1].inverse())*fRotMat[0];  
      transaux = (fRotMat[1].inverse())*(fTranslation[0]-fTranslation[1]);
      projPoint = findProjPoint(hitC,rotaux,transaux,projSlopes);
      CvsBinBCS_X[index]->Fill(hitB->getX() - projPoint.getX());
      CvsBinBCS_Y[index]->Fill(hitB->getY() - projPoint.getY());
      CvsBinBCS_XSlope[index]->Fill(origSlopes[0] - projSlopes[0]);
      CvsBinBCS_YSlope[index]->Fill(origSlopes[1] - projSlopes[1]);

      rotaux = (fRotMat[1].inverse())*fRotMat[2];  
      transaux = (fRotMat[1].inverse())*(fTranslation[2]-fTranslation[1]);
      projPoint = findProjPoint(hitA,rotaux,transaux,projSlopes);
      AvsBinBCS_X[index]->Fill(hitB->getX() - projPoint.getX());
      AvsBinBCS_Y[index]->Fill(hitB->getY() - projPoint.getY());
      AvsBinBCS_XSlope[index]->Fill(origSlopes[0] - projSlopes[0]);
      AvsBinBCS_YSlope[index]->Fill(origSlopes[1] - projSlopes[1]);

      //last, projecting on MDC A 
      rotaux = fRotMat[2].inverse();
      transaux = -(rotaux * fTranslation[2]);
      projPoint = findProjPoint(hitD,rotaux,transaux,projSlopes);
      DvsAinACS_X[index]->Fill(hitA->getX() - projPoint.getX());
      DvsAinACS_Y[index]->Fill(hitA->getY() - projPoint.getY());
      origSlopes = transformToSlopes(hitA);
      DvsAinACS_XSlope[index]->Fill(origSlopes[0] - projSlopes[0]);
      DvsAinACS_YSlope[index]->Fill(origSlopes[1] - projSlopes[1]);
	
      rotaux = (fRotMat[2].inverse())*fRotMat[0];  
      transaux = (fRotMat[2].inverse())*(fTranslation[0]-fTranslation[2]);
      projPoint = findProjPoint(hitC,rotaux,transaux,projSlopes);
      CvsAinACS_X[index]->Fill(hitA->getX() - projPoint.getX());
      CvsAinACS_Y[index]->Fill(hitA->getY() - projPoint.getY());
      CvsAinACS_XSlope[index]->Fill(origSlopes[0] - projSlopes[0]);
      CvsAinACS_YSlope[index]->Fill(origSlopes[1] - projSlopes[1]);

      rotaux = (fRotMat[2].inverse())*fRotMat[1];  
      transaux = (fRotMat[2].inverse())*(fTranslation[1]-fTranslation[2]);
      projPoint = findProjPoint(hitB,rotaux,transaux,projSlopes);
      BvsAinACS_X[index]->Fill(hitA->getX() - projPoint.getX());
      BvsAinACS_Y[index]->Fill(hitA->getY() - projPoint.getY());
      BvsAinACS_XSlope[index]->Fill(origSlopes[0] - projSlopes[0]);
      BvsAinACS_YSlope[index]->Fill(origSlopes[1] - projSlopes[1]);
    }
    if(fNumMods==3){
      //first projecting on MDC C
      projPoint = findProjPoint(hitB,fRotMat[0],fTranslation[0],projSlopes);
      BvsCinCCS_X[index]->Fill(hitC->getX() - projPoint.getX());
      BvsCinCCS_Y[index]->Fill(hitC->getY() - projPoint.getY());
      origSlopes = transformToSlopes(hitC);
      BvsCinCCS_XSlope[index]->Fill(origSlopes[0] - projSlopes[0]);
      BvsCinCCS_YSlope[index]->Fill(origSlopes[1] - projSlopes[1]);

      projPoint = findProjPoint(hitA,fRotMat[1],fTranslation[1],projSlopes);
      AvsCinCCS_X[index]->Fill(hitC->getX() - projPoint.getX());
      AvsCinCCS_Y[index]->Fill(hitC->getY() - projPoint.getY());
      AvsCinCCS_XSlope[index]->Fill(origSlopes[0] - projSlopes[0]);
      AvsCinCCS_YSlope[index]->Fill(origSlopes[1] - projSlopes[1]);

      //then projecting on MDC B      
      rotaux = fRotMat[0].inverse();
      transaux = -(rotaux * fTranslation[0]);
      projPoint = findProjPoint(hitC,rotaux,transaux,projSlopes);      
      CvsBinBCS_X[index]->Fill(hitB->getX() - projPoint.getX());
      CvsBinBCS_Y[index]->Fill(hitB->getY() - projPoint.getY());
      origSlopes = transformToSlopes(hitB);
      CvsBinBCS_XSlope[index]->Fill(origSlopes[0] - projSlopes[0]);
      CvsBinBCS_YSlope[index]->Fill(origSlopes[1] - projSlopes[1]);

      rotaux = (fRotMat[0].inverse())*fRotMat[1];  
      transaux = (fRotMat[0].inverse())*(fTranslation[1]-fTranslation[0]);
      projPoint = findProjPoint(hitA,rotaux,transaux,projSlopes);   
      AvsBinBCS_X[index]->Fill(hitB->getX() - projPoint.getX());
      AvsBinBCS_Y[index]->Fill(hitB->getY() - projPoint.getY());
      AvsBinBCS_XSlope[index]->Fill(origSlopes[0] - projSlopes[0]);
      AvsBinBCS_YSlope[index]->Fill(origSlopes[1] - projSlopes[1]);

   
      //last, projecting on MDC A      
      rotaux = fRotMat[1].inverse();
      transaux = -(rotaux * fTranslation[1]);
      projPoint = findProjPoint(hitC,rotaux,transaux,projSlopes); 
      CvsAinACS_X[index]->Fill(hitA->getX() - projPoint.getX());
      CvsAinACS_Y[index]->Fill(hitA->getY() - projPoint.getY());
      origSlopes = transformToSlopes(hitA);
      CvsAinACS_XSlope[index]->Fill(origSlopes[0] - projSlopes[0]);
      CvsAinACS_YSlope[index]->Fill(origSlopes[1] - projSlopes[1]);

      rotaux = (fRotMat[1].inverse())*fRotMat[0];  
      transaux = (fRotMat[1].inverse())*(fTranslation[0]-fTranslation[1]);
      projPoint = findProjPoint(hitB,rotaux,transaux,projSlopes);      
      BvsAinACS_X[index]->Fill(hitA->getX() - projPoint.getX());
      BvsAinACS_Y[index]->Fill(hitA->getY() - projPoint.getY());
      BvsAinACS_XSlope[index]->Fill(origSlopes[0] - projSlopes[0]);
      BvsAinACS_YSlope[index]->Fill(origSlopes[1] - projSlopes[1]);   

    }
    if(fNumMods==2) { 
      //first projecting on MDC B
      projPoint = findProjPoint(hitA,fRotMat[0],fTranslation[0],projSlopes);
      AvsBinBCS_X[index]->Fill(hitB->getX() - projPoint.getX());
      AvsBinBCS_Y[index]->Fill(hitB->getY() - projPoint.getY());
      origSlopes = transformToSlopes(hitB);
      AvsBinBCS_XSlope[index]->Fill(origSlopes[0] - projSlopes[0]);
      AvsBinBCS_YSlope[index]->Fill(origSlopes[1] - projSlopes[1]);

      //then projecting on MDC A
      rotaux = fRotMat[0].inverse();
      transaux = -(rotaux * fTranslation[0]);
      projPoint = findProjPoint(hitB,rotaux,transaux,projSlopes);
      BvsAinACS_X[index]->Fill(hitA->getX() - projPoint.getX());
      BvsAinACS_Y[index]->Fill(hitA->getY() - projPoint.getY());
      origSlopes = transformToSlopes(hitA);
      BvsAinACS_XSlope[index]->Fill(origSlopes[0] - projSlopes[0]);
      BvsAinACS_YSlope[index]->Fill(origSlopes[1] - projSlopes[1]);
    }
    //more histograms
    if(fNumMods>2) {
      //First, calculate the straigth line
      //and all insteresting quantities    
      if(fNumMods>3){
	d.setX(hitD->getX());
	d.setY(hitD->getY());
	d.setZ(0.);
      }          
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
      if(fUseUnitErrors==kFALSE){   
	errorx[0] = (hitA->getErrX()!=0)? hitA->getErrX() : 0.2;
	errorx[1] = (hitB->getErrX()!=0)? hitB->getErrX() : 0.2;      
	errorx[2] = (hitC->getErrX()!=0)? hitC->getErrX() : 0.2;   
	if(fNumMods>3) errorx[3] = (hitD->getErrX()!=0)? hitD->getErrX():0.2;
	errory[0] = (hitA->getErrY()!=0)? hitA->getErrY() : 0.1;
	errory[1] = (hitB->getErrY()!=0)? hitB->getErrY() : 0.1;      
	errory[2] = (hitC->getErrY()!=0)? hitC->getErrY() : 0.1;   
	if(fNumMods>3) errory[3] = (hitD->getErrY()!=0)? hitD->getErrY():0.1;
      }    
      else 
	for(Int_t i=0;i<fNumMods;i++){
	  errorx[i]=1.0;
	  errory[i]=1.0;      
	}
      
      if(A_DEBUG>3){
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
      
      if(A_DEBUG>3){
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
	
	if(A_DEBUG>3) cout << "Xwt=" << Xwt << " Xss=" << Xss 
			   << " Xsx=" <<  Xsx << " Xsy=" << Xsy
			   << " Ywt=" << Ywt << " Yss=" << Yss 
			   << " Ysx=" <<  Ysx << " Ysy=" << Ysy << endl;
      }
      
      Xsxoss = Xsx/Xss;    
      Ysxoss = Ysx/Yss;
      
      if(A_DEBUG>3) cout << "Xsxoss=" << Xsxoss << "  Ysxoss=" 
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
	
	if(A_DEBUG>3) cout << "Xt=" << Xt << " Xst2=" << Xst2 
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
      
      if(A_DEBUG>3) cout << "bx=" << bx << " ax=" << ax
			 << "by=" << by << " ay=" << ay << endl;
      
      sigax = sqrt((1.0+Xsx*Xsx/(Xss*Xst2))/Xss);
      sigbx = sqrt(1.0/Xst2);
      sigay = sqrt((1.0+Ysx*Ysx/(Yss*Yst2))/Yss);
      sigby = sqrt(1.0/Yst2);
      
      if(A_DEBUG>3) cout << "sigbx=" << sigbx << " sigax=" << sigax
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
	if(A_DEBUG>3) cout << "XChi2=" << XChi2 << " YChi2=" << YChi2 << endl;
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
	if(A_DEBUG>3) 
	  cout << "Square Distance point " << i << " - line: " 
	       << sdistance << endl;
	if(i==0)SqrDistToA[index]->Fill(sdist);	  
	if(i==1)SqrDistToB[index]->Fill(sdist);	  
	if(i==2)SqrDistToC[index]->Fill(sdist);	  
	if(i==3)SqrDistToD[index]->Fill(sdist);	
      }
      SqrDist[index]->Fill(sdistance);
    } 
    else{   //2 MDCs
      
    }
  } // end of for (Int_t i=0; i<nentries;i++) {
}

void HMdcAligner::storeInFile(void)
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

  fAlignAll->Write();
  fAlignAllCut->Write();

  //  const char* oldDirName = gDirectory->GetPath();
  static Char_t newDirName[255];
  if(fNumMods == 2)  sprintf(newDirName,"%s%s%i%s%i%s%i","Aligner_",
			     "S_",sector,"_ModA_",modA,"_ModB_",modB);
  if(fNumMods == 3)  sprintf(newDirName,"%s%s%i%s%i%s%i%s%i","Aligner_",
			     "S_",sector,"_ModA_",modA,"_ModB_",modB,
			     "_ModC_",modC);
  if(fNumMods == 4)  sprintf(newDirName,"%s%s%i%s%i%s%i%s%i%s%i",
			     "Aligner_","S_",sector,"_ModA_",modA,
			     "_ModB_",modB,"_ModC_",modC,"_ModD_",modD);

  fOutRoot->cd(newDirName);

  //Enter in the file the HMdcGeomRotation HMdcGeomVector
   for(Int_t i=0;i<fNumMods-1;i++){
    fRotMat[i].Write();
    fTranslation[i].Write();
  }  
  fOutRoot->cd();
  fRecCount--;
  if(!fRecCount) {
    cout << "Antes de Write()" << endl;
    fOutRoot->Write();
    fOutRoot->Close();
  }
} 

void HMdcAligner::fillRotMatrix(Int_t ind, Float_t prim,
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
  const double rad2deg = 57.29577951308;
  fRotMat[ind].setEulerAngles(prim*rad2deg,segu*rad2deg,terc*rad2deg);
} 

void HMdcAligner::fillTranslation(Int_t ind,Float_t x,
				 Float_t y, Float_t z){ 
  //
  // Translation from MDC A to MDC B
  //
 
  fTranslation[ind].setX(x); 
  fTranslation[ind].setY(y);
  fTranslation[ind].setZ(z); 
}

void HMdcAligner::setEulerAngles(Int_t ind,Float_t f,
				 Float_t s, Float_t t){ 
  //
  // Euler angles in transformation from MDC A to MDC B
  //
 
  fEuler[ind].setX(f); 
  fEuler[ind].setY(s); 
  fEuler[ind].setZ(t); 
} 

void HMdcAligner::setSearchLimits(Float_t x, Float_t y){ 
  //
  // Limits of the square defined in the search procedure
  //
 
  fXArea = x; 
  fYArea = y; 
} 

void HMdcAligner::setIterCriteria(Float_t cri){ 
  //
  // Set the criteria for iteration in the minimization (see finalize())
  //
 
  fIterCriteria = cri; 
}

void HMdcAligner::setWeights(Float_t w0,Float_t w1,Float_t w2,Float_t w3){ 
  //
  // Set the weights in the fcn()
  //
 
  fWeights[0]= w0; 
  fWeights[1]= w1; 
  fWeights[2]= w2; 
  fWeights[3]= w3; 

}

void HMdcAligner::setSteps(Float_t s0,Float_t s1,Float_t s2,
			   Float_t s3, Float_t s4, Float_t s5,
			   Float_t s6=0., Float_t s7=0., Float_t s8=0.,
			   Float_t s9=0., Float_t s10=0., Float_t s11=0.,
			   Float_t s12=0., Float_t s13=0., Float_t s14=0.,
			   Float_t s15=0., Float_t s16=0., Float_t s17=0.){ 
  //
  // Set the steps in the minimization
  //
 
  fSteps[0]= s0; fSteps[1]= s1; fSteps[2]= s2; 
  fSteps[3]= s3; fSteps[4]= s4; fSteps[5]= s5;
  fSteps[6]= s6; fSteps[7]= s7; fSteps[8]= s8; 
  fSteps[9]= s9; fSteps[10]= s10; fSteps[11]= s11;
  fSteps[12]= s12; fSteps[13]= s13; fSteps[14]= s14; 
  fSteps[15]= s15; fSteps[16]= s16; fSteps[17]= s17;

  if(A_DEBUG>3) cout << "Steps in the minimization adjusted to " 
		     << s0 << ", " << s1 << ", " << s2 << ", " 
		     << s3 << ", " << s4 << ", " << s5 << ", " 
		     << s6 << ", " << s7 << ", " << s8 << ", " 
		     << s9 << ", " << s10 << ", " << s11 << ", " 
		     << s12 << ", " << s13 << ", " << s14 << ", " 
		     << s15 << ", " << s16 << ", " << s17 << endl;
}

void HMdcAligner::setLimits(Float_t l0,Float_t l1,Float_t l2,
			    Float_t l3, Float_t l4, Float_t l5,
			    Float_t l6=0., Float_t l7=0., Float_t l8=0.,
			    Float_t l9=0., Float_t l10=0., Float_t l11=0.,
			    Float_t l12=0., Float_t l13=0., Float_t l14=0.,
			    Float_t l15=0., Float_t l16=0., Float_t l17=0.){ 
  //
  // Set the criteria for iteration in the minimization (see finalize())
  //
 
  fLimits[0]= l0; fLimits[1]= l1; fLimits[2]= l2; 
  fLimits[3]= l3; fLimits[4]= l4; fLimits[5]= l5; 
  fLimits[6]= l6; fLimits[7]= l7; fLimits[8]= l8; 
  fLimits[9]= l9; fLimits[10]= l10; fLimits[11]= l11; 
  fLimits[12]= l12; fLimits[13]= l13; fLimits[14]= l14; 
  fLimits[15]= l15; fLimits[16]= l16; fLimits[17]= l17; 

  if(A_DEBUG>3) cout << "Limits in the minimization adjusted to " 
		     << l0 << ", " << l1 << ", " << l2 << ", " 
		     << l3 << ", " << l4 << ", " << l5 << ", " 
		     << l6 << ", " << l7 << ", " << l8 << ", " 
		     << l9 << ", " << l10 << ", " << l11 << ", " 
		     << l12 << ", " << l13 << ", " << l14 << ", " 
		     << l15 << ", " << l16 << ", " << l17 << endl;
}

HGeomVector HMdcAligner::translationFinder(void){
  //
  // Analytical minimization of the function
  // X and Y residuals
  // Results are the relative translation 
  // vector components
  //
  // Matrix:
  //
  //    a 0 c      translation[0]     f
  //    0 b d   *  translation[1]  =  g
  //    c d e      translation[2]     h
  //
  // results in
  //
  // translation[0] = (fbe+gdc-bch-fdd)/(abe-bcc-add)
  // translation[1] = (age+fdc-gcc-dha)/(abe-bcc-add)
  // translation[2] = (abh-fbc-gda)/(abe-bcc-add)
  //

  HMdcHit* hitA;      
  HMdcHit* hitB;      
  HMdcHit* hitC;      
  HMdcHit* hitD; 
  HGeomVector projPoint;
  Float_t* projSlopes = new Float_t[2];
  HGeomRotation rotaux;
  HGeomVector transaux;
  HGeomVector resultaux,result;  
  Float_t a=0,b=0,c=0,d=0,e=0,f=0,g=0,h=0;
  Float_t thetaX=0,thetaY=0;
  Float_t resX=0,resY=0,resXstar=0,resYstar=0;
  Float_t Wx=1,Wy=1;
  Stat_t entries;
  if(fUseCut) entries= fAlignAllCut->GetEntries();
  else  entries= fAlignAll->GetEntries();
  
  //projecting on MDC A
  rotaux = fRotMat[0].inverse();
  transaux = -(rotaux * fTranslation[0]);
  
  for (Int_t i=0;i<entries;i++) { 
    if(fUseCut) fAlignAllCut->GetEntry(i);
    else fAlignAll->GetEntry(i);
    hitA = (HMdcHit*) fHits->At(0);
    hitB = (HMdcHit*) fHits->At(1);
    if(fNumMods>2) hitC = (HMdcHit*) fHits->At(2);
    if(fNumMods>3) hitD = (HMdcHit*) fHits->At(3);
    
    projPoint = findProjPoint(hitB,rotaux,transaux,projSlopes);
    resX = hitA->getX() - projPoint.getX();
    resY = hitA->getY() - projPoint.getY();
    
    Float_t s0=0,s1=0;
    if(fUseTarget) {
      s0 = (hitB->getX()-fTargetPos.getX())/(-fTargetPos.getZ());
      s1 = (hitB->getY()-fTargetPos.getY())/(-fTargetPos.getZ());
    }
    else{
      Float_t xDir = hitB->getXDir(); 
      Float_t yDir = hitB->getYDir();
      //if using hit!
      Float_t aux = sqrt(1 - xDir * xDir - yDir * yDir);    
      if(aux == 0.){ s0=1; s1=1;} //non-sense values
      else{
	s0 = xDir/aux;
	s1 = yDir/aux;
      }    
    }
    
    thetaX = (s0*rotaux(0) + s1*rotaux(1) + rotaux(2))/
      (rotaux(8) + s0*rotaux(6) + s1*rotaux(7));
    thetaY = (s0*rotaux(3) + s1*rotaux(4) + rotaux(5))/ 
      (rotaux(8) + s0*rotaux(6) + s1*rotaux(7));

    resXstar = resX + transaux.getX() - thetaX * transaux.getZ() ;
    resYstar = resY + transaux.getY() - thetaY * transaux.getZ() ;

    if(fUseUnitErrors){
      a+=1;
      b+=1;
      c+=-thetaX;
      d+=-thetaY;
      e+=thetaX*thetaX+thetaY*thetaY;
      f+=resXstar;
      g+=resYstar;
      h+=-(thetaX*resXstar+thetaY*resYstar);
    }
    else{      
      a+=Wx;
      b+=Wy;
      c+=-thetaX/Wx;
      d+=-thetaY/Wy;
      e+=thetaX*thetaX/Wx+thetaY*thetaY/Wy;
      f+=resXstar/Wx;
      g+=resYstar/Wy;
      h+=-(thetaX*resXstar/Wx+thetaY*resYstar/Wy);
    }
  }

  Float_t den = a*b*e-c*b*c-a*d*d;
  resultaux.setX((f*b*e+g*d*c-b*c*h-f*d*d)/den);
  resultaux.setY((a*g*e+f*d*c-g*c*c-d*h*a)/den);
  resultaux.setZ((a*b*h-f*b*c-g*d*a)/den);
  //again we should invert the transformation    

  result = -(fRotMat[0] * resultaux);
  return result;

  //quiza luego pueda hacer tambien el inverso,
  //esto es, proyectar en MDC B y comprobar que parametros
  //salen de la minimizacion analitica.
}

void HMdcAligner::minfit(Int_t fix, HGeomVector* fE, HGeomVector* fT){ 
  //
  // Minuit menu
  // Argument fix correspon to fFix value (see setFix())
  // Other arguments are init values for the parameters!
  //

  Double_t args[8]; 
  Int_t err = 0; 
  Float_t* limitL;
  Float_t* limitH;
  limitL = new Float_t[18];
  limitH = new Float_t[18];
 
  //This depends on MDCA and MDCB 
  //DANGEROUS! Can read memory out of scope
  Double_t start_val[]={fE[0].getX(),fE[0].getY(),fE[0].getZ(),
			fT[0].getX(),fT[0].getY(),fT[0].getZ(),
			fE[1].getX(),fE[1].getY(),fE[1].getZ(),
			fT[1].getX(),fT[1].getY(),fT[1].getZ(),
			fE[2].getX(),fE[2].getY(),fE[2].getZ(),
			fT[2].getX(),fT[2].getY(),fT[2].getZ()};
  
  //setting limits
  for(int i=0;i<18;i++){
    if(fLimits[i]==0){
      limitL[i]=0;
      limitH[i]=0;
    }
    else { 
      limitL[i]=start_val[i]-fLimits[i];      
      limitH[i]=start_val[i]+fLimits[i];
      cout << " LIMITS IN THE MINIMIZATION " << endl;
      cout << " (from 0 to 17) Par " << i << " between " 
	   << limitL[i] << " and " << limitH[i] << endl;
    }
  }
  
  TMinuit *minuit=new TMinuit((fNumMods-1)*6);
   
  //setting the minimization function
  if(fNumMods==2)minuit->SetFCN(fcnalign); 
  else minuit->SetFCN(fcnalign34); 

  minuit->SetObjectFit(this);
  
  if(A_DEBUG>0){
    cout << "HMdcAligner::minfit()" <<endl;    
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
  }

  //FIXING 
  if(fMin==4||fix==30) {    //fixing angles
    for(Int_t i=0;i<(fNumMods-1);i++){
      minuit->FixParameter(6*i+0);     
      minuit->FixParameter(6*i+1); 
      minuit->FixParameter(6*i+2); 
    }
  }
  if(fMin==1||fMin==3||fix==20) {    //fixing translations
    for(Int_t i=0;i<(fNumMods-1);i++){
      minuit->FixParameter(6*i+3);     
      minuit->FixParameter(6*i+4); 
      minuit->FixParameter(6*i+5); 
    }
  }
  if(fix==40){    //when cut in slopes, fixing the Z translation
    for(Int_t i=0;i<(fNumMods-1);i++){
      minuit->FixParameter(6*i+5); 
    }
  }
  if(fix>0 && fix<19){
    minuit->FixParameter(fix+1); 
  }
  if(fix==110){    
    for(Int_t i=0;i<(fNumMods-1);i++){
      minuit->FixParameter(6*i+0);       
      minuit->FixParameter(6*i+2); 
    }
  }

  //To remove all printout
  if(A_DEBUG<3){ 
    minuit->SetPrintLevel(-1);
  }

  //args is the array of the numerical arguments of the command 
  //the third field is the number of arguments especified 
  //For MIGRAD arguments are maxcalls and tolerance, both optionals 
  minuit->mnexcm("MIGRAD",args,0,err); 
  //minuit->mnexcm("SIMPLEX",args,0,err); 
  
  Double_t parresult[18];
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

}




void fcnalign(Int_t &npar, Double_t *gin, Double_t &f, 
	      Double_t *par, Int_t iflag){ 

  //
  // This function contains the functional to minimize
  // Only for 2 MDCs

  Double_t chisq = 0.; 
  Double_t zproy, resx, resy, ress0, ress1; 
  HGeomRotation rotmat;
  Double_t prim, segu, terc; 
  Float_t pxa, pya, ps0a, ps1a, pxb, pyb, ps0b, ps1b;
  Float_t xDir,yDir,aux;

  prim = par[0]; 
  segu = par[1]; 
  terc = par[2]; 

  HGeomVector translation(par[3],par[4],par[5]);

  if (A_DEBUG>2){
    cout << "HMdcAligner::fcnalign()   Parameters: " 
	 << par[0] << "," << par[1] << "," <<  par[2] << "," 
	 << par[3] << ","  << par[4] << ","  << par[5] << endl;
  }
  
  rotmat.setEulerAngles(prim*180./TMath::Pi(),
			segu*180./TMath::Pi(),
			terc*180./TMath::Pi()); 
  /*
0  rotmat[0][0]=(cos(prim)*cos(segu)*cos(terc))-(sin(prim)*sin(terc)); 
1  rotmat[1][0]=(-cos(prim)*cos(segu)*sin(terc))-(sin(prim)*cos(terc)); 
2  rotmat[2][0]=(cos(prim) * sin(segu)); 
3  rotmat[0][1]=(sin(prim)*cos(segu)*cos(terc))+(cos(prim)*sin(terc)); 
4  rotmat[1][1]=(-sin(prim)*cos(segu)*sin(terc))+(cos(prim)*cos(terc)); 
5  rotmat[2][1]=(sin(prim) * sin(segu)); 
6  rotmat[0][2]=( - sin(segu) * cos(terc)); 
7  rotmat[1][2]=(sin(segu) * sin(terc)); 
8  rotmat[2][2]=(cos(segu)); 
  */

  TTree* pData; 
  HMdcAligner* pAlign;  
  TClonesArray* theHits;
  pAlign = (HMdcAligner*)(gMinuit->GetObjectFit());
  pData = pAlign->getTree();
  Int_t strategy = pAlign->getStrategy();

  Stat_t entries = pData->GetEntries();

  Float_t* weights;
  weights = new Float_t[4];
  weights = pAlign->getWeights();

  HMdcHit *hitA;
  HMdcHit *hitB;

  //loop on hits 
  for (Int_t i=0;i<entries;i++) { 
    pData->GetEntry(i);  
    theHits = pAlign->getHits();

    //filling from the ntuple
    hitA = (HMdcHit*) theHits->At(0);
    hitB = (HMdcHit*) theHits->At(1);

    pxa = hitA->getX();
    pya = hitA->getY();       
    xDir = hitA->getXDir();
    yDir = hitA->getYDir(); 
    //If using HMdcHit, it is neccesary this change
    aux = sqrt(1 - xDir * xDir - yDir * yDir);
    if(aux == 0.) {
      ps0a=1; 
      ps1a=1; 
      cout<< "ERROR #1 in HMdcAligner::fcnalign().";
    } 
    else{
      ps0a = xDir/aux;
      ps1a = yDir/aux;
    }
    pxb = hitB->getX();
    pyb = hitB->getY();
    xDir = hitB->getXDir();
    yDir = hitB->getYDir();
    //If using HMdcHit, it is neccesary this change
    aux = sqrt(1 - xDir * xDir - yDir * yDir);
    if(aux == 0.) {
      ps0b=1; 
      ps1b=1;      
      cout<< "ERROR #2 in HMdcAligner::fcnalign().";
    }
    else{
      ps0b = xDir/aux;
      ps1b = yDir/aux;
    }

    //Inverse matrix 
    /*    
    det = 4*((pV00 * pV11 * pV22 * pV33) - (pV00 * pV11 * pV23 * pV23) 
          - (pV22 * pV33 * pV01 * pV01) + (pV01 * pV01 * pV23 * pV23)); 
    
    VI00 = (pV11 * pV22 * pV33 - pV11 * pV23 * pV23)/det; 
    VI11 = (pV00 * pV22 * pV33 - pV00 * pV23 * pV23)/det; 
    VI22 = (pV00 * pV11 * pV33 - pV33 * pV01 * pV01)/det; 
    VI33 = (pV00 * pV11 * pV22 - pV22 * pV01 * pV01)/det; 
    VI01 = (pV23 * pV23 * pV01 - pV01 * pV22 * pV33)/det; 
    VI23 = (pV01 * pV01 * pV23 - pV23 * pV00 * pV11)/det; 
    */

    HGeomRotation rotInv = rotmat.inverse();
    HGeomVector trasInv = -(rotInv * translation);

    // Remember that now MDCB is the origin and the hits 
    // are projected in MDCA (and inverse transform. are used)
    zproy = -(rotInv(6)*pxb + rotInv(7)*pyb + trasInv(2)) / 
      (rotInv(8) + rotInv(6)*ps0b + rotInv(7)*ps1b);  
    
    resx = pxa - (pxb*rotInv(0) + pyb*rotInv(1) + trasInv(0) + 
		  zproy*(ps0b*rotInv(0) + ps1b*rotInv(1) + rotInv(2))); 
     
    resy = pya - (pxb*rotInv(3) + pyb*rotInv(4) + trasInv(1) + 
		  zproy*(ps0b*rotInv(3) + ps1b*rotInv(4) + rotInv(5))); 
     
    ress0 = ps0a - ((rotInv(0)*ps0b + rotInv(1)*ps1b + rotInv(2)) / 
		    (rotInv(6)*ps0b + rotInv(7)*ps1b + rotInv(8))); 
     
    ress1 = ps1a - ((rotInv(3)*ps0b + rotInv(4)*ps1b + rotInv(5)) / 
		    (rotInv(6)*ps0b + rotInv(7)*ps1b + rotInv(8))); 

    //Some extra debbuging (always needed ;-) )
    if(A_DEBUG >4){
      cout << "  ++++++++  VALUES IN fcnalign()  +++++++++ " << endl;
      cout << "HITA: " << pxa << ", " << pya << ", " 
	   << ps0a << ", " << ps1a << endl;
      cout << "HITB: " << pxb << ", " << pyb << ", " 
	   << ps0b << ", " << ps1b << endl;
      cout << "Dist: " << resx << ", " << resy << ", " 
	   << ress0 << ", " << ress1 << endl;
    }
 
    //Choose the chisquare funcion 
    //chisq += resx*resx*VI00 + 2*resx*ress0*VI01 + resy*resy*VI22 
    //+ 2*resy*ress1*VI23 + ress0*ress0*VI11 + ress1*ress1*VI33; 
    //chisq += resx*resx/400. + resy*resy/213. + ress0*ress0/0.0037 
    //+ ress1*ress1/0.0027; 
    
    if(strategy==1||strategy==3) chisq += ress0*ress0/weights[2] 
				   + ress1*ress1/weights[3]; 
    
    if(strategy==4) chisq += resx*resx/weights[0] + resy*resy/weights[1];
    
    if(strategy==5) chisq += resx*resx/weights[0] + resy*resy/weights[1] 
		      + ress0*ress0/weights[2] + ress1*ress1/weights[3]; 
  } 
    f = chisq; 
    if(A_DEBUG>2){
      cout  << "chisqr= " << chisq << " out of " 
	    << entries << " combinations."<< endl; 
    }
} 


 
void fcnalign34(Int_t &npar, Double_t *gin, Double_t &f, 
	      Double_t *par, Int_t iflag){ 

  //
  // This function contains the functional to minimize
  // Use this if three or four MDCs are present in the sector

  Double_t chisq = 0.; 
  HGeomRotation rotmat[3];
  HGeomVector transla[3];
  HGeomVector a,b,c,d;
  HGeomVector transf[4];
  Float_t errorx[4]; 
  Float_t errory[4];

  HMdcAligner* pAlign = (HMdcAligner*)(gMinuit->GetObjectFit());
  TClonesArray* theHits;
  pAlign = (HMdcAligner*)(gMinuit->GetObjectFit());
  TTree* pData= pAlign->getTree();
  Stat_t entries = pData->GetEntries();  
  Int_t strategy = pAlign->getStrategy();

  Float_t* weights;
  weights = new Float_t[4];
  weights = pAlign->getWeights();
  Int_t numMods = pAlign->getNumMods();
  Bool_t UseUnitErrors = pAlign->getUseUnitErrors();

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
  
  if(numMods>3){
    rotmat[2].setEulerAngles(par[12]*180./TMath::Pi(),
			     par[13]*180./TMath::Pi(),
			     par[14]*180./TMath::Pi()); 
    transla[2].setX(par[15]); 
    transla[2].setY(par[16]); 
    transla[2].setZ(par[17]);
  }
  
  if (A_DEBUG>2){
    cout << "HMdcAligner::fcnalign()   Parameters: " 
	 << par[0] << "," << par[1] << "," <<  par[2] << "," 
	 << par[3] << ","  << par[4] << ","  << par[5]
	 << par[6] << "," << par[7] << "," <<  par[8] << "," 
	 << par[9] << ","  << par[10] << ","  << par[11] ;
    if(numMods>3) 
      cout << par[12] << "," << par[13] << "," <<  par[14] << "," 
	   << par[15] << ","  << par[16] << ","  << par[17] ;
    cout << endl;
  }
  
  HMdcHit *hitA;
  HMdcHit *hitB;
  HMdcHit *hitC;
  HMdcHit *hitD=NULL;


  //loop on hits 
  for (Int_t i=0;i<entries;i++) { 
    pData->GetEntry(i);  
    theHits = pAlign->getHits();
    //data = pData->GetArgs();
    //filling from the ntuple
    hitA = (HMdcHit*) theHits->At(0);      
    hitB = (HMdcHit*) theHits->At(1);      
    hitC = (HMdcHit*) theHits->At(2);
    if(numMods>3) hitD = (HMdcHit*) theHits->At(3);

    if(numMods>3){
      d.setX(hitD->getX());
      d.setY(hitD->getY());
      d.setZ(0.);
    }          
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
    if(UseUnitErrors==kFALSE){
      errorx[0] = (hitA->getErrX()!=0)? hitA->getErrX() : 0.2;
      errorx[1] = (hitB->getErrX()!=0)? hitB->getErrX() : 0.2;      
      errorx[2] = (hitC->getErrX()!=0)? hitC->getErrX() : 0.2;   
      if(numMods>3) errorx[3] = (hitD->getErrX()!=0)? hitD->getErrX():0.2;      
      errory[0] = (hitA->getErrY()!=0)? hitA->getErrY() : 0.1;
      errory[1] = (hitB->getErrY()!=0)? hitB->getErrY() : 0.1;      
      errory[2] = (hitC->getErrY()!=0)? hitC->getErrY() : 0.1;   
      if(numMods>3) errory[3] = (hitD->getErrY()!=0)? hitD->getErrY():0.1;      
    }
    else 
      for(Int_t i=0;i<numMods;i++){
	errorx[i]=1.0;
	errory[i]=1.0;      
      }

    if(A_DEBUG>4){
      for(Int_t i=0;i<numMods;i++){
	cout << "errorx[" << i <<"] = " << errorx[i] << endl;	
	cout << "errory[" << i <<"] = " << errory[i]  << endl;
      }
    }

    //convert all hits in each MDC to a common reference system
    //which is the coordinate system of the farther MDC from target
    //(provided you use the usual order in the constructors)

    if(numMods>4){
      transf[3] = d;
      transf[2] = rotmat[0] * c + transla[0];
      transf[1] = rotmat[1] * b + transla[1];
      transf[0] = rotmat[2] * a + transla[2];
    }
    else{            
      transf[2] = c;
      transf[1] = rotmat[0] * b + transla[0];
      transf[0] = rotmat[1] * a + transla[1];
    }

    if(A_DEBUG>4){
      for(Int_t i=0;i<numMods;i++){
	cout << "transf[" << i <<"]   ";
	transf[i].print();
      }
    }

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
      
      if(A_DEBUG>4) cout << "Xwt=" << Xwt << " Xss=" << Xss 
			 << " Xsx=" <<  Xsx << " Xsy=" << Xsy
			 << "Ywt=" << Ywt << " Yss=" << Yss 
			 << " Ysx=" <<  Ysx << " Ysy=" << Ysy << endl;
    }

    Xsxoss = Xsx/Xss;    
    Ysxoss = Ysx/Yss;

     if(A_DEBUG>4) cout << "Xsxoss=" << Xsxoss << "  Ysxoss=" 
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
      
      if(A_DEBUG>4) cout << "Xt=" << Xt << " Xst2=" << Xst2 
			 << " bx (partial)=" << bx << "Yt=" << Yt 
			 << " Yst2=" << Yst2 
			 << " by (partial)=" << by << endl; 
    } 
    
    bx /= Xst2;
    ax = (Xsy-(Xsx*bx))/Xss;
    by /= Yst2;
    ay = (Ysy-(Ysx*by))/Yss;
     
    if(A_DEBUG>4)  cout << "bx=" << bx << " ax=" << ax
			<< "by=" << by << " ay=" << ay << endl;

    sigax = sqrt((1.0+Xsx*Xsx/(Xss*Xst2))/Xss);
    sigbx = sqrt(1.0/Xst2);
    sigay = sqrt((1.0+Ysx*Ysx/(Yss*Yst2))/Yss);
    sigby = sqrt(1.0/Yst2);

    if(A_DEBUG>4) cout << "sigbx=" << sigbx << " sigax=" << sigax
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
      if(A_DEBUG>4) {
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
      if(A_DEBUG>4) cout << "Square Distance point " << i << " - line: " 
			 << sdistance << endl;
      //chisq should be inside the loop for adding the distances
      //of all points which contribute to the fit
      if(strategy==101) chisq += sdistance; 
    } 
    if(strategy==100) chisq += XChi2+YChi2;
    //if(XChi2+YChi2<1.)  chisq += XChi2+YChi2;
    //else  chisq += 1.;
  } 
  f = chisq; 
  if(A_DEBUG>2){
    cout  << "chisqr= " << chisq << " out of " 
	  << entries << " combinations."<< endl; 
  }
} 


