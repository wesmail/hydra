using namespace std;
# include <math.h>
# include <stdlib.h>
# include <iostream>
# include <iomanip> 
# include <fstream> 
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
// selected window). Then, several algorithms are defined to
// obtain the alignment parameters. 
//
////////////////////////////////////////////////////////////////////////
  
Int_t HMdcAligner::fRecCount=0;
TFile* HMdcAligner::fOutRoot=0;
Int_t A_DEBUG=0;


//Poner en un header para que sea utilizable en otros modulos, si se requiere.
Double_t breitW(Double_t *x, Double_t *par)
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
			 Int_t modA, Int_t modB, Int_t modC, Int_t modD)
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

  Char_t title1[50], tmp[50];
  sprintf(title1,"%s%i%s%i","Sector_",sector,"_ModA_",modA);
  sprintf(tmp,"%s%s%i%s%i","All","_Sector_",sector,"_ModA_",modA);    
  fAlignAll = new TTree(tmp,title1);
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

  zones_X = new Float_t[9];
  zones_Y = new Float_t[9];
  zones_DX = new Float_t[9];
  zones_DY = new Float_t[9];
  resZon_X = new Float_t[9];
  resZon_Y = new Float_t[9];
  for(Int_t n=0;n<9;n++){
    zones_X[n]=0.;
    zones_Y[n]=0.;
    zones_DX[n]=0.;
    zones_DY[n]=0.;
    resZon_X[n]=0.;
    resZon_Y[n]=0.;
  }
  setZones();

  fNEntries = 0; 
  fCount = 0;
  fCountCut = 0;

  fCloseToSolution = kFALSE;  
  fAuto = kFALSE;  
  fHistoOff = kFALSE;  
  fUseUnitErrors = kFALSE;  
  fUseModErrors = kFALSE;  
  fDoNotUseCov = kFALSE;   
  fUseSharpCut = kFALSE;   
  fUseCut = kTRUE;  
  fUseSlopeCut = kFALSE;
  fUseTarget = kFALSE;
  fUseZones = kTRUE;
  fSetHomogeneousDistribution = kFALSE;
  fMaxEntriesPerZone = 1000;
  fEntriesPerZone = new Int_t[90];
  for(Int_t zo=0;zo<90;zo++) fEntriesPerZone[zo] = 0;
  
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
  fHistZonNum = 4; //allows the fast definition of new sets of Zones histos
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
  fPosError = new Float_t[8]; 
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

  /*
  cout << "fEuler " << fEuler << " fTranslation " << fTranslation << endl; 
  cout << "ang " << ang << " tra " << tra << endl; 
  */

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





HGeomVector HMdcAligner::getLocalTarget(HGeomRotation rot,HGeomVector tran)
{
  //
  // Sets Local Target position for the module where is not defined
  // (that is, for the MDC in first position in the constructor).
  // Requires the definition and use of the target on the module
  // (WARNING: Use it only for 2 MDCs up to now.
  // It is mandatory to enter in the second position in the
  // constructor the MDC where the target is defined from) 
  //
  HGeomVector  target = rot.inverse()*
      (fTargetPos-tran);
  return target;
}





void HMdcAligner::setZones(Float_t* X, Float_t* Y,
			   Float_t* DX, Float_t* DY)
{
  //
  // Sets the center coordinates and side lengths of the rectangles
  // where tracks are accepted for studying rotations
  // 
    //ZONES
  // Every rectangular zone is defined around (x,y) with side (dx,dy).
  //(tested in macros)
  //i.e. Int_t zones_X[9] = {-500,0,500,-400,0,400,-250,0,250}
  //     Int_t zones_Y[9] = {400,400,400,-100,-100,-100,-600,-600,-600}
  //     Int_t zones_DX[9] = {150,150,150,100,100,100,50,50,50}
  //     Int_t zones_DY[9] = {150,150,150,100,100,100,50,50,50}
  //Suggested with symmetrical (extreme) values 
  //p.e.MDCII 
  //     Int_t zones_X[9] = {-200,0,200,-200,0,200,-200,0,200}
  //     Int_t zones_Y[9] = {200,200,200,0,0,0,-200,-200,-200}
  //     Int_t zones_DX[9] = {100,100,100,75,75,75,40,40,40}
  //     Int_t zones_DY[9] = {100,100,100,75,75,75,40,40,40}
  //p.e.MDCIII 
  //     Int_t zones_X[9] = {-350,0,350,-350,0,350,-350,0,350}
  //     Int_t zones_Y[9] = {350,350,350,0,0,0,-350,-350,-350}
  //     Int_t zones_DX[9] = {150,150,150,100,100,100,60,60,60}
  //     Int_t zones_DY[9] = {150,150,150,100,100,100,60,60,60}
  //p.e.MDCIV
  //     Int_t zones_X[9] = {-400,0,400,-400,0,400,-400,0,400}
  //     Int_t zones_Y[9] = {400,400,400,0,0,0,-400,-400,-400}
  //     Int_t zones_DX[9] = {200,200,200,150,150,150,100,100,100}
  //     Int_t zones_DY[9] = {200,200,200,150,150,150,100,100,100}

  if(X==0){
    if(fLoc[1]==0){      
      zones_X[0]=-100; zones_X[1]=0; zones_X[2]=100;
      zones_X[3]=-100; zones_X[4]=0; zones_X[5]=100;
      zones_X[6]=-100; zones_X[7]=0; zones_X[8]=100;

      zones_Y[0]=100; zones_Y[1]=100; zones_Y[2]=100;
      zones_Y[3]=0; zones_Y[4]=0; zones_Y[5]=0 ;
      zones_Y[6]=-100; zones_Y[7]=-100; zones_Y[8]=-100;

      zones_DX[0]=100; zones_DX[1]=100; zones_DX[2]=100; 
      zones_DX[3]=75; zones_DX[4]=75; zones_DX[5]=75; 
      zones_DX[6]=50; zones_DX[7]=50; zones_DX[8]=50;

      zones_DY[0]=100; zones_DY[1]=100; zones_DY[2]=100; 
      zones_DY[3]=75; zones_DY[4]=75; zones_DY[5]=75; 
      zones_DY[6]=50; zones_DY[7]=50; zones_DY[8]=50;
    }
    if(fLoc[1]==1){
      zones_X[0]=-200; zones_X[1]=0; zones_X[2]=200;
      zones_X[3]=-200; zones_X[4]=0; zones_X[5]=200;
      zones_X[6]=-200; zones_X[7]=0; zones_X[8]=200;

      zones_Y[0]=200; zones_Y[1]=200; zones_Y[2]=200;
      zones_Y[3]=0; zones_Y[4]=0; zones_Y[5]=0 ;
      zones_Y[6]=-200; zones_Y[7]=-200; zones_Y[8]=-200;

      zones_DX[0]=100; zones_DX[1]=100; zones_DX[2]=100; 
      zones_DX[3]=75; zones_DX[4]=75; zones_DX[5]=75; 
      zones_DX[6]=40; zones_DX[7]=40; zones_DX[8]=40;

      zones_DY[0]=100; zones_DY[1]=100; zones_DY[2]=100; 
      zones_DY[3]=75; zones_DY[4]=75; zones_DY[5]=75; 
      zones_DY[6]=40; zones_DY[7]=40; zones_DY[8]=40;
    }
    if(fLoc[1]==2){
      zones_X[0]=-350; zones_X[1]=0; zones_X[2]=350;
      zones_X[3]=-350; zones_X[4]=0; zones_X[5]=350;
      zones_X[6]=-350; zones_X[7]=0; zones_X[8]=350;

      zones_Y[0]=350; zones_Y[1]=350; zones_Y[2]=350;
      zones_Y[3]=0; zones_Y[4]=0; zones_Y[5]=0 ;
      zones_Y[6]=-350; zones_Y[7]=-350; zones_Y[8]=-350;
       
      zones_DX[0]=150; zones_DX[1]=150; zones_DX[2]=150; 
      zones_DX[3]=100; zones_DX[4]=100; zones_DX[5]=100; 
      zones_DX[6]=60; zones_DX[7]=60; zones_DX[8]=60;

      zones_DY[0]=150; zones_DY[1]=150; zones_DY[2]=150; 
      zones_DY[3]=100; zones_DY[4]=100; zones_DY[5]=100; 
      zones_DY[6]=60; zones_DY[7]=60; zones_DY[8]=60;
            
      //for comparison
      /*
	zones_X[0]=-500; zones_X[1]=0; zones_X[2]=500;
      zones_X[3]=-400; zones_X[4]=0; zones_X[5]=400;
      zones_X[6]=-250; zones_X[7]=0; zones_X[8]=250;

      zones_Y[0]=400; zones_Y[1]=400; zones_Y[2]=400;
      zones_Y[3]=-100; zones_Y[4]=-100; zones_Y[5]=-100 ;
      zones_Y[6]=-600; zones_Y[7]=-600; zones_Y[8]=-600;

      zones_DX[0]=150; zones_DX[1]=150; zones_DX[2]=150; 
      zones_DX[3]=100; zones_DX[4]=100; zones_DX[5]=100; 
      zones_DX[6]=50; zones_DX[7]=50; zones_DX[8]=50;

      zones_DY[0]=150; zones_DY[1]=150; zones_DY[2]=150; 
      zones_DY[3]=100; zones_DY[4]=100; zones_DY[5]=100; 
      zones_DY[6]=50; zones_DY[7]=50; zones_DY[8]=50;
      */
    }
    if(fLoc[1]==3){
      zones_X[0]=-400; zones_X[1]=0; zones_X[2]=400;
      zones_X[3]=-400; zones_X[4]=0; zones_X[5]=400;
      zones_X[6]=-400; zones_X[7]=0; zones_X[8]=400;

      zones_Y[0]=400; zones_Y[1]=400; zones_Y[2]=400;
      zones_Y[3]=0; zones_Y[4]=0; zones_Y[5]=0 ;
      zones_Y[6]=-400; zones_Y[7]=-400; zones_Y[8]=-400;

      zones_DX[0]=200; zones_DX[1]=200; zones_DX[2]=200; 
      zones_DX[3]=150; zones_DX[4]=150; zones_DX[5]=150; 
      zones_DX[6]=100; zones_DX[7]=100; zones_DX[8]=100;

      zones_DY[0]=200; zones_DY[1]=200; zones_DY[2]=200; 
      zones_DY[3]=150; zones_DY[4]=150; zones_DY[5]=150; 
      zones_DY[6]=100; zones_DY[7]=100; zones_DY[8]=100;
    }
  } 
  else {
    zones_X = X;
    zones_Y = Y;
    zones_DX = DX;
    zones_DY = DY;
  }
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
  } 

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
  BvsAinACS_Zon = new TH2F*[fHistZonNum];
  //BvsAinACS_Zon2 = new TH2F*[fHistZonNum];
  BvsAinACS_X_Zone = new TH1F*[fHistZonNum*9];  
  BvsAinACS_Y_Zone = new TH1F*[fHistZonNum*9]; 

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
    Int_t bin=150;
    Int_t binS=150;
    //Int_t min=-100,max=100,minS=-1,maxS=1;
    
    for(Int_t index=0;index<fHistNum;index++){
      sprintf(tmp,"%s%i%s%i%s%i%s%i","AvsBinBCS_X_",index,"_Sector_",sector,
	      "_ModA_",modA,"_ModB_",modB);
      AvsBinBCS_X[index] = new TH1F(tmp,title,bin,-fXArea,fXArea);  
      sprintf(tmp,"%s%i%s%i%s%i%s%i","AvsBinBCS_Y_",index,"_Sector_",sector,
	      "_ModA_",modA,"_ModB_",modB);
      AvsBinBCS_Y[index] = new TH1F(tmp,title,bin,-fYArea,fYArea);  
      sprintf(tmp,"%s%i%s%i%s%i%s%i","AvsBinBCS_XSlope_",index,"_Sector_",
	      sector,"_ModA_",modA,"_ModB_",modB);
      AvsBinBCS_XSlope[index] = new TH1F(tmp,title,binS,-fSArea,fSArea);
      sprintf(tmp,"%s%i%s%i%s%i%s%i","AvsBinBCS_YSlope_",index,"_Sector_",
	      sector,"_ModA_",modA,"_ModB_",modB);
      AvsBinBCS_YSlope[index] = new TH1F(tmp,title,binS,-fSArea,fSArea);

      sprintf(tmp,"%s%i%s%i%s%i%s%i","BvsAinACS_X_",index,"_Sector_",sector,
	      "_ModA_",modA,"_ModB_",modB);
      BvsAinACS_X[index] = new TH1F(tmp,title,bin,-fXArea,fXArea);  
      sprintf(tmp,"%s%i%s%i%s%i%s%i","BvsAinACS_Y_",index,"_Sector_",sector,
	      "_ModA_",modA,"_ModB_",modB);
      BvsAinACS_Y[index] = new TH1F(tmp,title,bin,-fYArea,fYArea);  
      sprintf(tmp,"%s%i%s%i%s%i%s%i","BvsAinACS_XSlope_",index,"_Sector_",
	      sector,"_ModA_",modA,"_ModB_",modB);
      BvsAinACS_XSlope[index] = new TH1F(tmp,title,binS,-fSArea,fSArea);
      sprintf(tmp,"%s%i%s%i%s%i%s%i","BvsAinACS_YSlope_",index,"_Sector_",
	      sector,"_ModA_",modA,"_ModB_",modB);
      BvsAinACS_YSlope[index] = new TH1F(tmp,title,binS,-fSArea,fSArea);
    }
  }
  //Histogramas comunes

  //Zones: 2D histograms for zones
  for(Int_t plot=0;plot<fHistZonNum;plot++){
    sprintf(tmp,"%s%i%s%i%s%i%s%i","BvsAinACS_Zon_P",plot,"_Sector_",
	    sector,"_ModA_",modA,"_ModB_",modB);
    BvsAinACS_Zon[plot] = new TH2F(tmp,title,800,-80,80,800,-90,70);
    //sprintf(tmp,"%s%i%s%i%s%i%s%i","BvsAinACS_Zon2_P",plot,"_Sector_",
    //	    sector,"_ModA_",modA,"_ModB_",modB);
    //BvsAinACS_Zon2[plot] = new TH2F(tmp,title,800,-80,80,800,-90,70);
  }
  
  Int_t binZ=200,minZ=-100,maxZ=100;

  for(Int_t plot=0;plot<fHistZonNum;plot++){
    for(Int_t index=0;index<9;index++){
      sprintf(title,"%s%i","zone_",index);
      sprintf(tmp,"%s%i%s%i","res_X_zone_",index,"_R",plot);
      BvsAinACS_X_Zone[index+9*plot] = new TH1F(tmp,title,binZ,minZ,maxZ);  
      sprintf(tmp,"%s%i%s%i","res_Y_zone_",index,"_R",plot);
      BvsAinACS_Y_Zone[index+9*plot] = new TH1F(tmp,title,binZ,minZ,maxZ); 
    }
  }

  sprintf(tmp,"%s%i%s%i%s%i","rotZ_Sector",sector,
	  "_ModA_",modA,"_ModB_",modB);
  hisrotZ = new TH1F(tmp,title,1000,-0.25,0.25);
  //sprintf(tmp,"%s%i%s%i%s%i","rotZW_Sector",sector,
  //	  "_ModA_",modA,"_ModB_",modB);
  //hisrotZW = new TH1F(tmp,title,500,-0.5,0.5);
  sprintf(tmp,"%s%i%s%i%s%i","rotZ_Zon_Sector",sector,
	  "_ModA_",modA,"_ModB_",modB);
  hisrotZZon = new TH1F(tmp,title,500,-0.25,0.25);
  sprintf(tmp,"%s%i%s%i%s%i","rotY_Sector",sector,
	  "_ModA_",modA,"_ModB_",modB);
  hisrotY = new TH1F(tmp,title,1000,-0.5,0.5);
  //sprintf(tmp,"%s%i%s%i%s%i","rotYW_Sector",sector,
  //	  "_ModA_",modA,"_ModB_",modB);
  //hisrotYW = new TH1F(tmp,title,1000,-1,1);
  sprintf(tmp,"%s%i%s%i%s%i","rotY_Zon_Sector",sector,
	  "_ModA_",modA,"_ModB_",modB);
  hisrotYZon = new TH1F(tmp,title,1000,-0.5,0.5);
  sprintf(tmp,"%s%i%s%i%s%i","rotYPos_Sector",sector,
	  "_ModA_",modA,"_ModB_",modB);
  hisrotYPos = new TH1F(tmp,title,1000,-1,1);
  sprintf(tmp,"%s%i%s%i%s%i","rotYNeg_Sector",sector,
	  "_ModA_",modA,"_ModB_",modB);
  hisrotYNeg = new TH1F(tmp,title,1000,-1,1);
  sprintf(tmp,"%s%i%s%i%s%i","rotY_ZonPos_Sector",sector,
	  "_ModA_",modA,"_ModB_",modB);
  hisrotYZonPos = new TH1F(tmp,title,1000,-1,1);
  sprintf(tmp,"%s%i%s%i%s%i","rotY_ZonNeg_Sector",sector,
	  "_ModA_",modA,"_ModB_",modB);
  hisrotYZonNeg = new TH1F(tmp,title,1000,-1,1);
  sprintf(tmp,"%s%i%s%i%s%i","rotX_Sector",sector,
	  "_ModA_",modA,"_ModB_",modB);
  hisrotX = new TH1F(tmp,title,1000,-0.5,0.5);
  //sprintf(tmp,"%s%i%s%i%s%i","rotXW_Sector",sector,
  //	  "_ModA_",modA,"_ModB_",modB);
  //hisrotXW = new TH1F(tmp,title,1000,-1,1);
  sprintf(tmp,"%s%i%s%i%s%i","rotX_Zon_Sector",sector,
	  "_ModA_",modA,"_ModB_",modB);
  hisrotXZon = new TH1F(tmp,title,1000,-0.5,0.5);
  sprintf(tmp,"%s%i%s%i%s%i","rotXPos_Sector",sector,
	  "_ModA_",modA,"_ModB_",modB);
  hisrotXPos = new TH1F(tmp,title,1000,-1,1);
  sprintf(tmp,"%s%i%s%i%s%i","rotXNeg_Sector",sector,
	  "_ModA_",modA,"_ModB_",modB);
  hisrotXNeg = new TH1F(tmp,title,1000,-1,1);
  sprintf(tmp,"%s%i%s%i%s%i","rotX_ZonPos_Sector",sector,
	  "_ModA_",modA,"_ModB_",modB);
  hisrotXZonPos = new TH1F(tmp,title,1000,-1,1);
  sprintf(tmp,"%s%i%s%i%s%i","rotX_ZonNeg_Sector",sector,
	  "_ModA_",modA,"_ModB_",modB);
  hisrotXZonNeg = new TH1F(tmp,title,1000,-1,1);

  graphXchi2 = new TGraph(40);
  graphXchi2->SetName("graphXchi2");
  graphXchi2->SetTitle("graphXchi2");
  graphXchi2X = new TGraph(40);
  graphXchi2X->SetName("graphXchi2X");
  graphXchi2X->SetTitle("graphXchi2X");
  graphXchi2Y = new TGraph(40);
  graphXchi2Y->SetName("graphXchi2Y");
  graphXchi2Y->SetTitle("graphXchi2Y");
  graphYchi2 = new TGraph(40);
  graphYchi2->SetName("graphYchi2");
  graphYchi2->SetTitle("graphYchi2");
  graphYchi2X = new TGraph(40);
  graphYchi2X->SetName("graphYchi2X");
  graphYchi2X->SetTitle("graphYchi2X");
  graphYchi2Y = new TGraph(40);
  graphYchi2Y->SetName("graphYchi2Y");
  graphYchi2Y->SetTitle("graphYchi2Y");

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





void HMdcAligner::fitHistograms(Int_t index)
{
  //
  //Fits to a gaussian the four relevant histograms
  //and obtains the fit parameters for further data selection
  //Only valid for two MDCs or three MDCs

  if(fNumMods==2 || fNumMods==3){
    
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
    TF1 *total3X  = new TF1("total3X",breitW,-fXArea,fXArea,3);
    TF1 *total3Y  = new TF1("total3Y",breitW,-fYArea,fYArea,3);
    TF1 *total3S  = new TF1("total3S",breitW,-fSArea,fSArea,3);
    Double_t par[6]; 

    if(A_DEBUG>1) cout << endl 
		       <<"**** fitHistograms() results ****" << endl;   
    if(A_DEBUG>1) cout << endl 
		       <<"**** Gauss fit: mean, sigma ****" << endl 
		       <<"**** Gauss+pol: mean, sigma ****" 
		       << endl;   

    if(fNumMods==3) ABvsCinCCS_X[index]->Fit("f1X","RQNW"); 
    else AvsBinBCS_X[index]->Fit("f1X","RQNW");
    Float_t fitPar0     = f1X->GetParameter(0);  // constant
    Float_t fitPar1     = f1X->GetParameter(1);  // mean
    Float_t fitPar2     = f1X->GetParameter(2);  // sigma
    if(A_DEBUG>1) 
      cout << " AvsBinBCS_X[" << index << "]: "
	   << fitPar1 << ", " << fitPar2<< ", " ;
    f1X->GetParameters(&par[0]);
    par[3] =  par[4] = par[5] = 0.; 
    totalX->SetParameters(par);
    if(fNumMods==3) ABvsCinCCS_X[index]->Fit("totalX","RQN");
    else AvsBinBCS_X[index]->Fit("totalX","RQN"); 
    fitPar0     = totalX->GetParameter(0); 
    fitPar1     = totalX->GetParameter(1);
    fitPar2     = totalX->GetParameter(2);
    if(A_DEBUG>1) cout << fitPar1 << ", " << fitPar2 << endl;
    XNewAreaA = fXSigmas * fitPar2;
    XNewMeanA = fitPar1;

    if(fNumMods==3) ABvsCinCCS_Y[index]->Fit("f1Y","RQNW");
    else AvsBinBCS_Y[index]->Fit("f1Y","RQNW");
    fitPar0     = f1Y->GetParameter(0);  // constant
    fitPar1     = f1Y->GetParameter(1);  // mean
    fitPar2     = f1Y->GetParameter(2);  // sigma    
    if(A_DEBUG>1) cout << " AvsBinBCS_Y[" << index << "]: "
		       << fitPar1 << ", " << fitPar2<< ", " ;
    f1Y->GetParameters(&par[0]);
    totalY->SetParameters(par);
    if(fNumMods==3) ABvsCinCCS_Y[index]->Fit("totalY","RQN");
    else AvsBinBCS_Y[index]->Fit("totalY","RQN");
    fitPar1     = totalY->GetParameter(1);
    fitPar2     = totalY->GetParameter(2);
    if(A_DEBUG>1) cout << fitPar1 << ", " << fitPar2 << endl;
    YNewAreaA = fYSigmas * fitPar2;
    YNewMeanA = fitPar1;

    if(fNumMods==3) ABvsCinCCS_XSlope[index]->Fit("f1S","RQNW");
    else AvsBinBCS_XSlope[index]->Fit("f1S","RQNW");
    fitPar0     = f1S->GetParameter(0);  // constant
    fitPar1     = f1S->GetParameter(1);  // mean
    fitPar2     = f1S->GetParameter(2);  // sigma    
    if(A_DEBUG>1) cout << " AvsBinBCS_XSlope[" << index << "]: "
		       << fitPar1 << ", " << fitPar2<< ", " ;
    f1S->GetParameters(&par[0]);
    totalS->SetParameters(par);     
    if(fNumMods==3) ABvsCinCCS_XSlope[index]->Fit("totalS","RQN");
    else AvsBinBCS_XSlope[index]->Fit("totalS","RQN");
    fitPar1     = totalS->GetParameter(1);
    fitPar2     = totalS->GetParameter(2);
    if(A_DEBUG>1) cout << fitPar1 << ", " << fitPar2 << endl;
    S0NewAreaA = fS0Sigmas * fitPar2;
    S0NewMeanA = fitPar1;
   
    if(fNumMods==3) ABvsCinCCS_YSlope[index]->Fit("f1S","RQNW");
    else AvsBinBCS_YSlope[index]->Fit("f1S","RQNW");
    fitPar0     = f1S->GetParameter(0);  // constant
    fitPar1     = f1S->GetParameter(1);  // mean
    fitPar2     = f1S->GetParameter(2);  // sigma    
    if(A_DEBUG>1) cout << " AvsBinBCS_YSlope[" << index << "]: "
		       << fitPar1 << ", " << fitPar2<< ", " ;
    f1S->GetParameters(&par[0]);
    totalS->SetParameters(par);     
    if(fNumMods==3) ABvsCinCCS_YSlope[index]->Fit("totalS","RQN");
    else AvsBinBCS_YSlope[index]->Fit("totalS","RQN");
    fitPar1     = totalS->GetParameter(1);
    fitPar2     = totalS->GetParameter(2);
    if(A_DEBUG>1) cout << fitPar1 << ", " << fitPar2 << endl;
    S1NewAreaA = fS1Sigmas * fitPar2;
    S1NewMeanA = fitPar1;

    if(fNumMods==3) BCvsAinACS_X[index]->Fit("f1X","RQN");
    else BvsAinACS_X[index]->Fit("f1X","RQN");
    fitPar0     = f1X->GetParameter(0);  // constant
    fitPar1     = f1X->GetParameter(1);  // mean
    fitPar2     = f1X->GetParameter(2);  // sigma    
    if(A_DEBUG>1) cout << " BvsAinACS_X[" << index << "]: "
		       << fitPar1 << ", " << fitPar2<< ", " ;
    f1X->GetParameters(&par[0]);
    totalX->SetParameters(par);     
    if(fNumMods==3) BCvsAinACS_X[index]->Fit("totalX","RQN");
    else BvsAinACS_X[index]->Fit("totalX","RQN");
    fitPar1     = totalX->GetParameter(1);
    fitPar2     = totalX->GetParameter(2);
    if(A_DEBUG>1) cout << fitPar1 << ", " << fitPar2 << endl;
    XNewAreaB = fXSigmas * fitPar2;
    XNewMeanB = fitPar1;

    //Double gauss
    f1X->GetParameters(&par[0]);
    par[3]=par[0]/10;par[4]=par[1];par[5]=par[2]*4;
    total2X->SetParameters(par);  
    if(fNumMods==3) BCvsAinACS_X[index]->Fit("total2X","RQN");
    else BvsAinACS_X[index]->Fit("total2X","RQN");
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
    if(fNumMods==3) BCvsAinACS_X[index]->Fit(total3X,"RQN");
    else BvsAinACS_X[index]->Fit(total3X,"RQN");
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

    if(fNumMods==3) BCvsAinACS_Y[index]->Fit("f1Y","RQN");
    else BvsAinACS_Y[index]->Fit("f1Y","RQN");
    fitPar0     = f1Y->GetParameter(0);  // constant
    fitPar1     = f1Y->GetParameter(1);  // mean
    fitPar2     = f1Y->GetParameter(2);  // sigma    
    if(A_DEBUG>1) 
      cout << " BvsAinACS_Y[" << index << "]: "
	   << fitPar1 << ", " << fitPar2<< ", " ;    
    f1Y->GetParameters(&par[0]);
    totalY->SetParameters(par);     
    if(fNumMods==3) BCvsAinACS_Y[index]->Fit("totalY","RQN");
    else BvsAinACS_Y[index]->Fit("totalY","RQN");
    fitPar1     = totalY->GetParameter(1);
    fitPar2     = totalY->GetParameter(2);
    if(A_DEBUG>1) 
      cout << fitPar1 << ", " << fitPar2 << endl;
    YNewAreaB = fYSigmas * fitPar2;
    YNewMeanB = fitPar1;

    //Double gauss
    f1Y->GetParameters(&par[0]);
    par[3]=par[0]/10;par[4]=par[1];par[5]=par[2]*4;
    total2Y->SetParameters(par);  
    if(fNumMods==3) BCvsAinACS_Y[index]->Fit("total2Y","RQN");
    else BvsAinACS_Y[index]->Fit("total2Y","RQN");
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
    if(fNumMods==3) BCvsAinACS_Y[index]->Fit(total3Y,"RQN");
    else BvsAinACS_Y[index]->Fit(total3Y,"RQN");
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


    if(fNumMods==3) BCvsACinACS_XSlope[index]->Fit("f1S","RQN");
    else BvsAinACS_XSlope[index]->Fit("f1S","RQN");
    fitPar0     = f1S->GetParameter(0);  // constant
    fitPar1     = f1S->GetParameter(1);  // mean
    fitPar2     = f1S->GetParameter(2);  // sigma    
    if(A_DEBUG>1) cout << " BvsAinACS_XSlope[" << index << "]: "
		       << fitPar1 << ", " << fitPar2<< ", " ;
    f1S->GetParameters(&par[0]);
    totalS->SetParameters(par);     
    if(fNumMods==3) BCvsACinACS_XSlope[index]->Fit("totalS","RQN");
    else BvsAinACS_XSlope[index]->Fit("totalS","RQN");
    fitPar1     = totalS->GetParameter(1);
    fitPar2     = totalS->GetParameter(2);
    if(A_DEBUG>1) cout << fitPar1 << ", " << fitPar2 << endl;
    S0NewAreaB = fS0Sigmas * fitPar2;
    S0NewMeanB = fitPar1;

    //Double gauss
    f1S->GetParameters(&par[0]);
    par[3]=par[0]/10;par[4]=par[1];par[5]=par[2]*4;
    total2S->SetParameters(par);  
    if(fNumMods==3) BCvsACinACS_XSlope[index]->Fit("total2S","RQN");
    else BvsAinACS_XSlope[index]->Fit("total2S","RQN");
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
    if(fNumMods==3) BCvsACinACS_XSlope[index]->Fit(total3S,"RQN");
    else BvsAinACS_XSlope[index]->Fit(total3S,"RQN");
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

    if(fNumMods==3) BCvsACinACS_YSlope[index]->Fit("f1S","RQN");
    else BvsAinACS_YSlope[index]->Fit("f1S","RQN");
    fitPar0     = f1S->GetParameter(0);  // constant
    fitPar1     = f1S->GetParameter(1);  // mean
    fitPar2     = f1S->GetParameter(2);  // sigma    
    if(A_DEBUG>1) cout << " BvsAinACS_YSlope[" << index << "]: "
		       << fitPar1 << ", " << fitPar2<< ", " ;    
    f1S->GetParameters(&par[0]);
    totalS->SetParameters(par);     
    if(fNumMods==3) BCvsACinACS_YSlope[index]->Fit("totalS","RQN");
    else BvsAinACS_YSlope[index]->Fit("totalS","RQN");
    fitPar1     = totalS->GetParameter(1);
    fitPar2     = totalS->GetParameter(2);
    if(A_DEBUG>1) cout << fitPar1 << ", " << fitPar2 << endl;
    S1NewAreaB = fS1Sigmas * fitPar2;
    S1NewMeanB = fitPar1;

    //Double gauss
    f1S->GetParameters(&par[0]);
    par[3]=par[0]/10;par[4]=par[1];par[5]=par[2]*4;
    total2S->SetParameters(par);  
    if(fNumMods==3) BCvsACinACS_YSlope[index]->Fit("total2S","RQN");
    else BvsAinACS_YSlope[index]->Fit("total2S","RQN");
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
    if(fNumMods==3) BCvsACinACS_YSlope[index]->Fit(total3S,"RQN");
    else BvsAinACS_YSlope[index]->Fit(total3S,"RQN");
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
    
    if(fNumMods==3){
      rotaux = fRotMat[1].inverse();
      transaux = -(rotaux * fTranslation[1]);
      rotaux2 = fRotMat[0].inverse() * fRotMat[1];
      transaux2 = fRotMat[0].inverse()*fTranslation[1]-
	fRotMat[0].inverse()*fTranslation[0];
    }
    else{
      rotaux = fRotMat[0].inverse();
      transaux = -(rotaux * fTranslation[0]);
    }
    
    for (Int_t i=0;i<entries;i++) { 
      fHits->Clear();
      fAlignAll->GetEntry(i); 
      hitA = (HMdcHit*) fHits->At(0);
      hitB = (HMdcHit*) fHits->At(1);
      if(fNumMods>2) hitC = (HMdcHit*) fHits->At(2);

      if(fNumMods==3){
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
      else{
	projPoint = findProjPoint(hitB,rotaux,transaux,projSlopes);
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
    }
    //INTRODUCING THE COVARIANCES OF THE VARIABLES      
    Bool_t cutPassed=kFALSE;
    //Covariances (correlation factors)
    //Int_t modA = fLoc[1];
    //Int_t modB = fLoc[2]; 
    Int_t modC = fLoc[3]; 

    Float_t rhoxSx, rhoxSy, rhoySy, rhoxy, rho; 
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

    //Needed to introduce (quasi)homogenous distributions
    Float_t offsetY = 575;
    Float_t sizeBin = 100;
    if(modC==2){
      offsetY = 910;
      sizeBin = 191;
    }
      for(Int_t zo=0;zo<90;zo++) fEntriesPerZone[zo] = 0;//Reset


    for (Int_t i=0;i<entries;i++) {     
      fHits->Clear();
      fAlignAll->GetEntry(i);
      hitA = (HMdcHit*) fHits->At(0);
      hitB = (HMdcHit*) fHits->At(1);
      if(fNumMods>2) hitC = (HMdcHit*) fHits->At(2);
      //if(fNumMods>3) hitD = (HMdcHit*) fHits->At(3);
      
      Float_t resInAvsBinBCS_X, resInAvsBinBCS_Y;
      Float_t resInAvsBinBCS_XSlope, resInAvsBinBCS_YSlope;
      Float_t resInBvsAinACS_X, resInBvsAinACS_Y;
      Float_t resInBvsAinACS_XSlope, resInBvsAinACS_YSlope;
   
      if(fNumMods==3){

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
      }
      else   {
	//projecting on MDC B
	projPoint = findProjPoint(hitA,fRotMat[0],fTranslation[0],projSlopes);
	resInAvsBinBCS_X = hitB->getX() - projPoint.getX();
	resInAvsBinBCS_Y = hitB->getY() - projPoint.getY();      
	transformToSlopes(hitB,origSlopes);
	resInAvsBinBCS_XSlope = origSlopes[0] - projSlopes[0];
	resInAvsBinBCS_YSlope = origSlopes[1] - projSlopes[1];
	
	//then projecting on MDC A
	//rotaux = fRotMat[0].inverse();
	//transaux = -(rotaux * fTranslation[0]);
	projPoint = findProjPoint(hitB,rotaux,transaux,projSlopes);
	resInBvsAinACS_X = hitA->getX() - projPoint.getX();
	resInBvsAinACS_Y = hitA->getY() - projPoint.getY();
	transformToSlopes(hitA,origSlopes);
	resInBvsAinACS_XSlope = origSlopes[0] - projSlopes[0];
	resInBvsAinACS_YSlope = origSlopes[1] - projSlopes[1];
      }

      
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

	/*
	if(modA==0){
	  //cov(x,S(x))=-0.0154, rho(x,S(x))=-0.224,
 	  //cov(y,S(y))=-0.0132, rho(y,S(y))=-0.307,
	  //rhoxSx = -0.224;
	  //rhoySy = -0.307;

	  // Just the exponent of the probability function	  
	  exponent = 
	    ( 1/(1-(rhoxSx*rhoxSx)) ) *
	    ( ( ((resInBvsAinACS_X-XNewMeanB)*(resInBvsAinACS_X-XNewMeanB))/
		((XNewAreaB/fXSigmas)*(XNewAreaB/fXSigmas)) ) +
	      ( ((resInBvsAinACS_XSlope-S0NewMeanB)*
		 (resInBvsAinACS_XSlope-S0NewMeanB))/
		((S0NewAreaB/fS0Sigmas)*(S0NewAreaB/fS0Sigmas)) ) -
	      ( 2 * (resInBvsAinACS_X-XNewMeanB) *
		(resInBvsAinACS_XSlope-S0NewMeanB) * 
		(rhoxSx*rhoxSx)/((XNewAreaB/fXSigmas)*
				 (S0NewAreaB/fS0Sigmas)) ) ) +
	    ( 1/(1-(rhoySy*rhoySy)) ) *
	    ( ( ((resInBvsAinACS_Y-YNewMeanB)*(resInBvsAinACS_Y-YNewMeanB))/
		((YNewAreaB/fYSigmas)*(YNewAreaB/fYSigmas)) ) +
	      ( ((resInBvsAinACS_YSlope-S1NewMeanB)*
		 (resInBvsAinACS_YSlope-S1NewMeanB))/
		((S1NewAreaB/fS1Sigmas)*(S1NewAreaB/fS1Sigmas)) ) -
	      ( 2 * (resInBvsAinACS_Y-YNewMeanB) *
		(resInBvsAinACS_YSlope-S1NewMeanB) * 
		(rhoxSx*rhoxSx)/((YNewAreaB/fYSigmas)*
				 (S1NewAreaB/fS1Sigmas)) ) );
	  
	  if(exponent<pow(fXSigmas,4)) cutPassed=kTRUE;
	}
	else if(modA==2 || modA==3){
	  //cov(x,S(x))=0.0139, rho(x,S(x))=-0.109,
 	  //cov(y,S(y))=0.0130, rho(y,S(y))=-0.131,
 	  //cov(x,S(y))=0.0174, rho(x,S(y))=-0.136,	  
	  //rhoxSx = -0.109;
	  //rhoySy = -0.131;
	  //rhoxSy = -0.136;
	  
	*/

	if(fNumMods==3){
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
	else {
	  rho = 1 - rhoxSy*rhoxSy - rhoxSx*rhoxSx 
	    - rhoySy*rhoySy - rhoxSx*rhoxSx*rhoySy*rhoySy;
	  
	  exponent =  (1/rho) *
	    ( ( (-2 * rhoxSy*rhoxSy * (resInBvsAinACS_X-XNewMeanB) * 
		 (resInBvsAinACS_YSlope-S1NewMeanB)) /
		((XNewAreaB/fXSigmas)*(S1NewAreaB/fS1Sigmas)) ) +
	      (1 - rhoySy*rhoySy) *
	      ( (((resInBvsAinACS_X-XNewMeanB)*(resInBvsAinACS_X-XNewMeanB))/
		 ((XNewAreaB/fXSigmas)*(XNewAreaB/fXSigmas))) - 
		((2 * rhoxSx * (resInBvsAinACS_X-XNewMeanB) * 
		  (resInBvsAinACS_XSlope-S0NewMeanB)) / 
		 ((XNewAreaB/fXSigmas)*(S0NewAreaB/fS0Sigmas))) ) +
	      (1 - rhoxSy*rhoxSy - rhoySy*rhoySy) *
	      (((resInBvsAinACS_XSlope-S0NewMeanB)*
		(resInBvsAinACS_XSlope-S0NewMeanB))/
	       ((S0NewAreaB/fS0Sigmas)*(S0NewAreaB/fS0Sigmas))) +
	      (1 - rhoxSx*rhoxSx) *
	      ( (((resInBvsAinACS_Y-YNewMeanB)*(resInBvsAinACS_Y-YNewMeanB))/
		 ((YNewAreaB/fYSigmas)*(YNewAreaB/fYSigmas))) - 
		((2 * rhoySy * (resInBvsAinACS_Y-YNewMeanB) * 
		  (resInBvsAinACS_YSlope-S1NewMeanB)) / 
		 ((YNewAreaB/fYSigmas)*(S1NewAreaB/fS1Sigmas))) ) +
	      (1 - rhoxSy*rhoxSy - rhoxSx*rhoxSx) *
	      (((resInBvsAinACS_Y-YNewMeanB)*(resInBvsAinACS_Y-YNewMeanB))/
	       ((YNewAreaB/fYSigmas)*(YNewAreaB/fYSigmas))) );
	  
	  if(exponent<pow(fXSigmas,4)) cutPassed=kTRUE;
	}
      }
      
      //discarting hits in overcrowded zones to obtain a (quasi)homogeneous 
      if(fSetHomogeneousDistribution==kTRUE){
	if(cutPassed==kTRUE){
	  Int_t binZone = (int)floor((hitC->getY()+offsetY)/sizeBin);
	  binZone += 10 * (int)floor(fabs((hitC->getX()+(sizeBin/2))/sizeBin));
	  if((hitC->getX()+(sizeBin/2))<0) binZone += 50;
	  //check if the binZone is between the limits (0,89)
	  //cout << "Hit x e y: " <<hitC->getX()<< "  "<<hitC->getY()<< endl;
	  //cout << "binZone: " << binZone << endl;
	  if(binZone>-1 && binZone<90){
	    fEntriesPerZone[binZone]++;
	    if(binZone == 22 || binZone == 35 || binZone == 48 || 
	       binZone == 62 || binZone == 75 || binZone == 88){
	      if(fEntriesPerZone[binZone]>(fMaxEntriesPerZone/4))
		cutPassed=kFALSE;
	    }
	    if(binZone == 89 || binZone == 76 || binZone == 63 || 
	       binZone == 50 || binZone == 10 || binZone == 23 || 
	       binZone == 36 || binZone == 49 ){
	      if(fEntriesPerZone[binZone]>(fMaxEntriesPerZone/2))
		cutPassed=kFALSE;
	    }
	    if(binZone == 11 || binZone == 24 || binZone == 37 || 
	       binZone == 77 || binZone == 64 || binZone == 51){
	      if(fEntriesPerZone[binZone]>((3*fMaxEntriesPerZone)/4))
		cutPassed=kFALSE;
	    }
	    else {	      
	      if(fEntriesPerZone[binZone]>fMaxEntriesPerZone)
		cutPassed=kFALSE;
	    }
	  }
	}	  
      }    

      if(cutPassed==kTRUE){
	if(fUseSlopeCut){
	  if(fNumMods==2){
	    if(fSlopeCut>0){
	      // This cut makes the sample near indep. of Z translations
	      // and results useful for X and Y minimization.
	      // The cut is effective only in MDCB, because fTranslation
	      // is represented in MDCB coordinates. Then, tracks passing
	      // the cut are almost parallel to Z direction
	      
	      if(( fabs(hitB->getXDir()) < fSlopeCut) && 
		 ( fabs(hitB->getYDir()) < fSlopeCut)){
		if(A_DEBUG>3) cout << " -- CUT PASSED (fSlopeCut=" 
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
	
	      if(( fabs(hitB->getXDir()) > -fSlopeCut) && 
		 ( fabs(hitB->getYDir()) > -fSlopeCut)){
		if(A_DEBUG>3) cout << " -- CUT PASSED (fSlopeCut=" 
				   << fSlopeCut << " ) --" << endl;
		//new((*fHits)[0])HMdcHit(*hitA);	  
		//new((*fHits)[1])HMdcHit(*hitB);      
		fAlignAllCut->Fill();
		fHits->Clear();   	      
		fCountCut++;
	      }
	    }
	  }
	  else if(fNumMods==3){
	    if(fSlopeCut>0){
	      // This cut makes the sample near indep. of Z translations
	      // and results useful for X and Y minimization.
	      
	      if(( fabs(hitA->getXDir()+hitB->getXDir()+hitC->getXDir()/3) 
		   < fSlopeCut) && 
		 ( fabs(hitA->getYDir()+hitB->getYDir()+hitC->getYDir()/3) 
		   < fSlopeCut)){
		if(A_DEBUG>3) cout << " -- CUT PASSED (fSlopeCut=" 
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
		if(A_DEBUG>3) cout << " -- CUT PASSED (fSlopeCut=" 
				   << fSlopeCut << " ) --" << endl;
		//new((*fHits)[0])HMdcHit(*hitA);	  
		//new((*fHits)[1])HMdcHit(*hitB);      
		fAlignAllCut->Fill();
		fHits->Clear();   	      
		fCountCut++;
	      }
	    }
	  }
	}
	else{
	  if(A_DEBUG>3) cout << " ---------  CUT PASSED ------------" << endl;
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





void HMdcAligner::setCloseToSolution(void)
{
  //
  //Turn on the fCloseToSolution flag to introduce better fits
  //

  fCloseToSolution =kTRUE;
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





void HMdcAligner::setMinimization(Int_t select)
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
  // select = 10 Pure geometrical rotation finder 
  // select = 11 Translation finder before and after select=10
  // select = 12 Translation finder between steps in select=10
  // select = 14 Pure rotation determination by sampling
  // select = 15 Translation finder before and after select=14
  // select = 16 Translation finder between steps in select=14  
  //
  // select = 500 + (10,11,12,14,15,16) Iterative minimization
  //            version of the previously described algorithms 
  //
  // select = 100 (fcnalign34) Chi square sum in minimization
  // select = 101 (fcnalign34) Distances line-hit
  //  
  // select = 105 (fcnalign34) Minimizing angles between 2 lines 
  //              made from Hits (testing)
  // select = 106 (fcnalign34) Error with MINOS!
  //
  // select = 777 Testing the contour
  //
  // select = 200 ROTATION FINDER USING MINUIT (testing)

  fMin = select;  
}





void HMdcAligner::setUnitErrors(void)
{  
  //
  // introduce unit errors in Hits
  //

  fUseUnitErrors = kTRUE;  
}





void HMdcAligner::setOffCov(void)
{  
  //
  // Sets off the covariance in the translationFinder() function
  //

  fDoNotUseCov = kTRUE;  
}





void HMdcAligner::setSharpCut(void)
{  
  //
  // Sets off the covariance in fitHistograms()
  //

  fUseSharpCut = kTRUE;  
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





void HMdcAligner::setHomogeneousDistribution(Int_t maxEntries)
{
  //
  //Discart Hits in overcrowded zones to create a (quasi)homogeneous 
  //distribution
  //
  fSetHomogeneousDistribution = kTRUE;
  fMaxEntriesPerZone = maxEntries;
}                





void HMdcAligner::setFix(Int_t fix)
{  
  //
  // Fix a parameter set in minimization
  // New scheme: 18 bits (binary number) specifying the parameters
  // fixed (1) and released (0). Lower bit is first parameter
  //

  fFix = fix;  
}





void HMdcAligner::setNoCut(void)
{  
  //
  // Disables the cuts after fitting the histograms
  //

  fUseCut = kFALSE;  
}





void HMdcAligner::setNoZones(void)
{  
  //
  // Disables the use of zones in the different
  // rotation finders. Instead of zones weigthed hits are used
  //

  fUseZones = kFALSE;  
}





void HMdcAligner::setSlopeCut(Float_t SlopeCut)
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
  HMdcHit* pHitCD = new HMdcHit;
  HMdcHit* pHitBCD = new HMdcHit;
  HMdcHit* pHitABCD = new HMdcHit;

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
	  mergeHits(pHitD,pHitC,fRotMat[0],fTranslation[0],pHitCD);
	  
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
		mergeHits(pHitCD,pHitB,fRotMat[1],fTranslation[1],pHitBCD);
		
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
		      mergeHits(pHitBCD,pHitA,fRotMat[2],
				fTranslation[2],pHitABCD);
		      
		      //Filling the TClonesArray for storage in TTree
		      //Will be used only if 
		      fHits->Clear();
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

  if(pHitCD!=0) delete pHitCD;
  if(pHitBCD!=0) delete pHitBCD;
  if(pHitABCD!=0) delete pHitABCD;
  if(projSlopes!=0) delete[] projSlopes;

}
  


void HMdcAligner::execute3(void)
{    
  // 
  // New execute for more than two MDCs
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
	  mergeHits(pHitC,pHitB,fRotMat[0],fTranslation[0],pHitBC);
	  
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

  if(pHitBC!=0) delete pHitBC;
  if(pHitABC!=0) delete pHitABC;
  if(projSlopes!=0) delete[] projSlopes;
}





void HMdcAligner::execute2(void)
{    
  // 
  // Adapting the old execute
  //

  HMdcHit* pHitA; 
  HMdcHit* pHitB; 
  HMdcHit* pHitAB = new HMdcHit;

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
	  mergeHits(pHitB,pHitA,fRotMat[0],
		    fTranslation[0],pHitAB);
	  
	  //Filling the TClonesArray for storage in TTree
	  //Will be used only if 
	  fHits->Clear();
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

  if(pHitAB!=0) delete pHitAB;  
  if(projSlopes!=0) delete[] projSlopes;
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



void HMdcAligner::mergeHits(HMdcHit* hitB, HMdcHit* hitA,
			    HGeomRotation rot,HGeomVector tra, 
			    HMdcHit* mergeHit){
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
  //  return hitB;

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





void HMdcAligner::transformToSlopes(HMdcHit* pHit, Float_t* slopes){
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
  if(A_DEBUG>0){
    cout << endl << "Sector: " << sector << endl;
    cout << "Module A: " << modA << "  Module B: " << modB ;
    if(fNumMods>2) cout << "  Module C: " << modC;    
    if(fNumMods>3) cout << "  Module D: " << modD;
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
  if(A_DEBUG>0){
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
    if(fSetHomogeneousDistribution){    
      *fout << "Homogeneous distribution; entries per zone: " << endl;
      for (Int_t co=0;co<10;co++) 
	*fout<<fEntriesPerZone[89-co]<<"    "<<fEntriesPerZone[79-co]<<"    "
	     <<fEntriesPerZone[69-co]<<"    "<<fEntriesPerZone[59-co]<<"    "
	     <<fEntriesPerZone[9-co]<<"    "<<fEntriesPerZone[19-co]<<"    "
	     <<fEntriesPerZone[29-co]<<"    "<<fEntriesPerZone[39-co]<<"    "
	     <<fEntriesPerZone[49-co]<<endl;
    }
    *fout << endl << endl;
  }
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
  // select = 0  Disables minimization
  // select = 1  Angle reduction by minuit
  // select = 2  Analytic translation minimization
  // select = 3  (1+2) Angle reduction+analytic translation
  // select = 4  Translation reduction by minuit
  // select = 5  6 parameters reduction by minuit
  // select = 10 Pure geometrical rotation finder 
  // select = 11 Translation finder before and after select=10
  // select = 12 Translation finder between steps in select=10
  // select = 14 Pure rotation determination by sampling
  // select = 15 Translation finder before and after select=14
  // select = 16 Translation finder between steps in select=14  
  //
  // select = 100 (fcnalign34) Chi square sum in minimization
  // select = 101 (fcnalign34) Distances line-hit
  //  
  // select = 105 (fcnalign34) Minimizing angles between 2 lines 
  //              made from Hits (testing)
  //
  //
  // select = 200 ROTATION FINDER USING MINUIT (testing)
  if(fMin==1 || fMin==3 || fMin==4 || fMin==5 || 
     fMin==100 || fMin==101 || fMin==105 || fMin==777){
    
    //intentando que minimize sobre distintos conjuntos de hits aceptados
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
	if(fMin==777) break;
	
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
      
      //intentando que minimize sobre distintos conjuntos de hits aceptados
      if(fMin==777) break;
      
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
	
	if(A_DEBUG==0){     
	  cout << i << "IterCri2: " <<   IterCri2 << endl;
	}
      }
      IterCounter2++;
      if(IterCounter2>100) {
	cout << "WARNING in HMdcAligner :: finalize" << endl;
	cout << "Sector: " << sector << " ModuleA: " 
	     << modA <<  " ModuleB: " << modB << endl;
	cout << "More than 100 double iterations without results!" <<endl;
	break;
      }    
      
      if(IterCri2>fIterCriteria){
	fillHistograms(2);  
	fitHistograms(2);
	
	if (*fout) {
	  *fout << "Valid hits for alignment after cuts: " 
		<< fCountCut << endl << endl;    
	  if(fSetHomogeneousDistribution){    
	    *fout << "Homogeneous distribution; entries per zone: " << endl;
	    for (Int_t co=0;co<10;co++) 
	      *fout<<fEntriesPerZone[89-co]<<"    "<<fEntriesPerZone[79-co]<<"    "
		   <<fEntriesPerZone[69-co]<<"    "<<fEntriesPerZone[59-co]<<"    "
		   <<fEntriesPerZone[9-co]<<"    "<<fEntriesPerZone[19-co]<<"    "
		   <<fEntriesPerZone[29-co]<<"    "<<fEntriesPerZone[39-co]<<"    "
		   <<fEntriesPerZone[49-co]<<endl;
	    *fout << endl;
	  }
	  *fout << endl << endl;
	}
	if(A_DEBUG>0) cout << "Valid hits for alignment after cuts: " 
			   << fCountCut << endl << endl;
      }     
      
    }while(IterCri2>fIterCriteria);
    //

    //Uncomment for a MINOS error evaluation after the MINUIT minimization
    //Very slow and unsuccess
    /*
    if(fMin==105){
      fMin=106;
      minfit(fFix,fEuler,fTranslation);
      if (*fout){
	*fout << "MINOS: a better error estimation! "  << endl;
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
      minfit(0,fEuler,fTranslation);
            if (*fout){
	*fout << "MINOS RELEASING ALL FIXED PARAMETERS!! "  << endl;
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

      fMin=105;
    }
    */

  }

  HGeomVector result;

  if(fMin==2 || fMin==3){ 
    //select = 2  Analytic translation minimization
    //select = 3  (1+2) Angle reduction+analytic translation
    
    //Analytic translation minimization
    result = translationFinder();    
  }  

  if(fMin==10 ||fMin==11 ||fMin==12){       
    //select = 10 Pure geometrical rotation finder 
    //select = 11 Translation finder before and after select=10
    //select = 12 Translation finder between steps in select=10

    //Analytic translation minimization
    if(fMin==11||fMin==12) result = translationFinder();
    //geometrical rotation finder (Z)
    findRotZGeometrically();
    //Analytic translation minimization
    if(fMin==11) result = translationFinder();
    //geometrical rotation finder (X)
    findRotXGeometrically();
    //Analytic translation minimization
    if(fMin==11) result = translationFinder();
    //geometrical rotation finder (Y)
    findRotYGeometrically();
    //Analytic translation minimization
    if(fMin==11||fMin==12) result = translationFinder();
    
    divideInZones(3,fRotMat[0],fTranslation[0]); //just for histograming
    //Output
    if (*fout){
      *fout << endl <<"Parameters after GEOMETRIC rotation determination "
	    << endl;
      for(Int_t i=0;i<fNumMods-1;i++){
      *fout << fEuler[i].getX() << ",  " << fEuler[i].getY() << ",  " 
	    << fEuler[i].getZ() << ",  " << fTranslation[i].getX() << ",  "
	    << fTranslation[i].getY() << ",  " << fTranslation[i].getZ() 
	    << endl;
      }	
      *fout << endl;	
    }
  }

  if(fMin==14 || fMin==15 || fMin==16){ 
    // select = 14 Pure rotation determination by sampling
    // select = 15 Translation finder before and after select=14
    // select = 16 Translation finder between steps in select=14

    //Analytic translation minimization
    if(fMin==15||fMin==16) result = translationFinder();
    //geometrical rotation finder (Z)
    findRotZGeometrically();
    //Analytic translation minimization
    if(fMin==15) result = translationFinder();
    //RotXYBySampling
    findRotXYBySampling(1);    
    //Analytic translation minimization
    if(fMin==15) result = translationFinder();
    findRotXYBySampling(2);
    //Analytic translation minimization
    if(fMin==15||fMin==16) result = translationFinder();
   }

  
  //ITERATIVE VERSIONS
  
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
    *fout << "Iterative procedure; iteration number " << IterCounter << endl;
    
    if(fMin==510||fMin==511||fMin==512){ 
      //ITERATIVE EXTENSION TO
      //select = 10 Pure geometrical rotation finder 
      //select = 11 Translation finder before and after select=10
      //select = 12 Translation finder between steps in select=10

      //Analytic translation minimization
      if(fMin==511||fMin==512) result = translationFinder();
      //geometrical rotation finder (Z)
      findRotZGeometrically(IterCounter);
      //Analytic translation minimization
      if(fMin==511) result = translationFinder();
      //geometrical rotation finder (X)
      findRotXGeometrically(IterCounter);
      //Analytic translation minimization
      if(fMin==511) result = translationFinder();
      //geometrical rotation finder (Y)
      findRotYGeometrically(IterCounter);
      //Analytic translation minimization
      if(fMin==511||fMin==512) result = translationFinder();
      
      divideInZones(3,fRotMat[0],fTranslation[0]); //just for histograming
      //Output
      if (*fout){
	*fout << endl <<"Parameters after GEOMETRIC rotation determination "
	      << endl;
	for(Int_t i=0;i<fNumMods-1;i++){
	  *fout << fEuler[i].getX() << ",  " << fEuler[i].getY() << ",  " 
		<< fEuler[i].getZ() << ",  " << fTranslation[i].getX() << ",  "
		<< fTranslation[i].getY() << ",  " << fTranslation[i].getZ() 
		<< endl;
	}	
	*fout << endl;	
      }
    }
    
    
    if(fMin==514||fMin==515||fMin==516){ 
      //  ITERATIVE EXTENSION TO
      // select = 14 Pure rotation determination by sampling
      // select = 15 Translation finder before and after select=14
      // select = 16 Translation finder between steps in select=14

      //Analytic translation minimization
      if(fMin==515||fMin==516) result = translationFinder();
      //geometrical rotation finder (Z)
      findRotZGeometrically(IterCounter);
      //Analytic translation minimization
      if(fMin==515) result = translationFinder();
      //RotXYBySampling
      findRotXYBySampling(1,IterCounter);
     //Analytic translation minimization
      if(fMin==515) result = translationFinder();
      findRotXYBySampling(2,IterCounter);
     //Analytic translation minimization
      if(fMin==515||fMin==516) result = translationFinder();
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
    }
    IterCounter++;      
    if(IterCounter>25) {
      
      cout << endl << "More than 25 iterations without convergence!" <<endl;
      *fout << endl << "More than 25 iterations without convergence!" <<endl;
      break;
    }
  }while(IterCri>fIterCriteria);
  
  if(fMin == 200){    
    //Analytic translation minimization
    result = translationFinder();
    
    //MINUIT rotations 
    if (*fout) *fout << "Minimization strategy: " 
		     << fMin <<" with MINUIT" << endl;
    HGeomVector OldEuler[(fNumMods-1)];
    HGeomVector OldTranslation[(fNumMods-1)];
    Int_t IterCounter =0;
    Float_t IterCri;    

    do{
      divideInZones(IterCounter,fRotMat[0],fTranslation[0]);

      IterCri = 0;
      for(Int_t i=0;i<fNumMods-1;i++){
	OldEuler[i] = fEuler[i];
	OldTranslation[i] = fTranslation[i];
      }      
      Double_t first=0.;
      Double_t second=0.;
      minfitRot(fFix,first,second);
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
      
      //      for(Int_t i=0;i<fNumMods-1;i++){
      //	fillRotMatrix(i,fEuler[i].getX(),fEuler[i].getY(),fEuler[i].getZ());
      //      }    
      
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
      if(IterCounter>3) {
	cout << "WARNING in HMdcAligner :: finalize" << endl;
	cout << "Sector: " << sector << " ModuleA: " 
	     << modA <<  " ModuleB: " << modB << endl;
	cout << "More than 3 iterations (MINUIT ROT) without results!" <<endl;
	break;
      }
    }while(IterCri>fIterCriteria);
  }


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
  if(A_DEBUG>0){
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





void HMdcAligner::fillHistograms (Int_t index, Int_t select){
  //
  // Performs the graphical output from obtained parameters
  //   

  HMdcHit* hitA;      
  HMdcHit* hitB;      
  HMdcHit* hitC=NULL;      
  HMdcHit* hitD=NULL; 
  HMdcHit* hitBC=new HMdcHit; 
  HMdcHit* hitAB=new HMdcHit; 
  HMdcHit* hitAC=new HMdcHit; 

  HGeomVector projPoint;
  Float_t* projSlopes = new Float_t[2];
  Float_t* origSlopes = new Float_t[2];
  Float_t* SlopesAinA = new Float_t[2];
  Float_t* SlopesBinA = new Float_t[2];
  Float_t* SlopesCinA = new Float_t[2];
  HGeomRotation rotaux;
  HGeomVector transaux;
  HGeomVector transf[4];  
  HGeomVector a,b,c,d;
  Float_t errorx[4]; 
  Float_t errory[4];
  Stat_t entries; 
  if(fNumMods==3){  
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
  }

  if(fNumMods==2){
    if (index==2){
      AvsBinBCS_X[2]->Reset();
      AvsBinBCS_Y[2]->Reset();
      AvsBinBCS_XSlope[2]->Reset();
      AvsBinBCS_YSlope[2]->Reset();
      BvsAinACS_X[2]->Reset();
      BvsAinACS_Y[2]->Reset();
      BvsAinACS_XSlope[2]->Reset();
      BvsAinACS_YSlope[2]->Reset();
    }
  }

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
      transformToSlopes(hitD,origSlopes);
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
      transformToSlopes(hitC,origSlopes);
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
      transformToSlopes(hitB,origSlopes);
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
      transformToSlopes(hitA,origSlopes);
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
      transformToSlopes(hitC,origSlopes);
      BvsCinCCS_XSlope[index]->Fill(origSlopes[0] - projSlopes[0]);
      BvsCinCCS_YSlope[index]->Fill(origSlopes[1] - projSlopes[1]);
      if(index==2){
	BvsCinCCS_Polar->Fill(atanf(origSlopes[1]) - atanf(projSlopes[1]));
	if(origSlopes[1]>0.2) 
	  BvsCinCCS_Polar_Stripe1->Fill(atanf(origSlopes[1]) 
					- atanf(projSlopes[1]));
	else if(origSlopes[1]>0.) 
	  BvsCinCCS_Polar_Stripe2->Fill(atanf(origSlopes[1]) 
					- atanf(projSlopes[1]));
	else if(origSlopes[1]>-0.2) 
	  BvsCinCCS_Polar_Stripe3->Fill(atanf(origSlopes[1]) 
                                       - atanf(projSlopes[1]));
	else if(origSlopes[1]>-0.4) 
	  BvsCinCCS_Polar_Stripe4->Fill(atanf(origSlopes[1]) 
					- atanf(projSlopes[1]));
	else  
	  BvsCinCCS_Polar_Stripe5->Fill(atanf(origSlopes[1]) 
					- atanf(projSlopes[1]));
      }
      projPoint = findProjPoint(hitA,fRotMat[1],fTranslation[1],projSlopes);
      AvsCinCCS_X[index]->Fill(hitC->getX() - projPoint.getX());
      AvsCinCCS_Y[index]->Fill(hitC->getY() - projPoint.getY());
      AvsCinCCS_XSlope[index]->Fill(origSlopes[0] - projSlopes[0]);
      AvsCinCCS_YSlope[index]->Fill(origSlopes[1] - projSlopes[1]);

      if(index==2){
	AvsCinCCS_Polar->Fill(atanf(origSlopes[1]) - atanf(projSlopes[1]));
	if(origSlopes[1]>0.2) 
	  AvsCinCCS_Polar_Stripe1->Fill(atanf(origSlopes[1]) 
					- atanf(projSlopes[1]));
	else if(origSlopes[1]>0.) 
	  AvsCinCCS_Polar_Stripe2->Fill(atanf(origSlopes[1]) 
					- atanf(projSlopes[1]));
	else if(origSlopes[1]>-0.2) 
	  AvsCinCCS_Polar_Stripe3->Fill(atanf(origSlopes[1]) 
					- atanf(projSlopes[1]));
	else if(origSlopes[1]>-0.4) 
	  AvsCinCCS_Polar_Stripe4->Fill(atanf(origSlopes[1]) 
					- atanf(projSlopes[1]));
	else  
	  AvsCinCCS_Polar_Stripe5->Fill(atanf(origSlopes[1]) 
					- atanf(projSlopes[1]));
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
      transformToSlopes(hitA,origSlopes);
      CvsAinACS_XSlope[index]->Fill(origSlopes[0] - projSlopes[0]);
      CvsAinACS_YSlope[index]->Fill(origSlopes[1] - projSlopes[1]);
      //to be used later on
      SlopesCinA[0] = projSlopes[0];
      SlopesCinA[1] = projSlopes[1];

      rotaux = (fRotMat[1].inverse())*fRotMat[0];  
      transaux = (fRotMat[1].inverse())*(fTranslation[0]-fTranslation[1]);
      projPoint = findProjPoint(hitB,rotaux,transaux,projSlopes);      
      BvsAinACS_X[index]->Fill(hitA->getX() - projPoint.getX());
      BvsAinACS_Y[index]->Fill(hitA->getY() - projPoint.getY());
      BvsAinACS_XSlope[index]->Fill(origSlopes[0] - projSlopes[0]);
      BvsAinACS_YSlope[index]->Fill(origSlopes[1] - projSlopes[1]);   
      //to be used later on
      SlopesBinA[0] = projSlopes[0];
      SlopesBinA[1] = projSlopes[1];

      //new projection, merge of MDC C and MDC B on MDC A 
      //(testing MDC A resolution after alignment, for instance)
      rotaux = fRotMat[1].inverse();
      transaux = -(rotaux * fTranslation[1]);
      mergeHits(hitC,hitB,fRotMat[0],fTranslation[0],hitBC);
      projPoint = findProjPoint(hitBC,rotaux,transaux,projSlopes);
      BCvsAinACS_X[index]->Fill(hitA->getX() - projPoint.getX());
      BCvsAinACS_Y[index]->Fill(hitA->getY() - projPoint.getY());
      
      //checking the slopes from two hits in modules C and A
      mergeHits(hitC,hitA,fRotMat[1],fTranslation[1],hitAC);
      projPoint = findProjPoint(hitAC,rotaux,transaux,origSlopes);
      //
      //      if(hitC->getY()<-100){
      BCvsACinACS_XSlope[index]->Fill(origSlopes[0] - projSlopes[0]);
      BCvsACinACS_YSlope[index]->Fill(origSlopes[1] - projSlopes[1]);   
      // }
      
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
      rotaux = fRotMat[0].inverse() * fRotMat[1];
      transaux = fRotMat[0].inverse()*fTranslation[1]-
	fRotMat[0].inverse()*fTranslation[0];
      mergeHits(hitB,hitA,rotaux,transaux,hitAB);
      projPoint = findProjPoint(hitAB,fRotMat[0],fTranslation[0],projSlopes);
      transformToSlopes(hitC,origSlopes);
      ABvsCinCCS_X[index]->Fill(hitC->getX() - projPoint.getX());
      ABvsCinCCS_Y[index]->Fill(hitC->getY() - projPoint.getY());
      ABvsCinCCS_XSlope[index]->Fill(origSlopes[0] - projSlopes[0]);
      ABvsCinCCS_YSlope[index]->Fill(origSlopes[1] - projSlopes[1]);   

      //new projection, merge of MDC A and MDC C on MDC B
      rotaux = fRotMat[0].inverse();
      transaux = -(rotaux * fTranslation[0]);
      mergeHits(hitC,hitA,fRotMat[1],fTranslation[1],hitAC);
      projPoint = findProjPoint(hitAC,rotaux,transaux,projSlopes);
      transformToSlopes(hitB,origSlopes);
      ACvsBinBCS_X[index]->Fill(hitB->getX() - projPoint.getX());
      ACvsBinBCS_Y[index]->Fill(hitB->getY() - projPoint.getY());
      ACvsBinBCS_XSlope[index]->Fill(origSlopes[0] - projSlopes[0]);
      ACvsBinBCS_YSlope[index]->Fill(origSlopes[1] - projSlopes[1]);   
    }
    if(fNumMods==2) { 
      //first projecting on MDC B
      projPoint = findProjPoint(hitA,fRotMat[0],fTranslation[0],projSlopes);
      AvsBinBCS_X[index]->Fill(hitB->getX() - projPoint.getX());
      AvsBinBCS_Y[index]->Fill(hitB->getY() - projPoint.getY());
      transformToSlopes(hitB,origSlopes);
      AvsBinBCS_XSlope[index]->Fill(origSlopes[0] - projSlopes[0]);
      AvsBinBCS_YSlope[index]->Fill(origSlopes[1] - projSlopes[1]);

      //then projecting on MDC A
      rotaux = fRotMat[0].inverse();
      transaux = -(rotaux * fTranslation[0]);
      projPoint = findProjPoint(hitB,rotaux,transaux,projSlopes);
      BvsAinACS_X[index]->Fill(hitA->getX() - projPoint.getX());
      BvsAinACS_Y[index]->Fill(hitA->getY() - projPoint.getY());
      transformToSlopes(hitA,origSlopes);
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
      if(fUseUnitErrors==kFALSE && fUseModErrors==kFALSE ){   
	errorx[0] = (hitA->getErrX()!=0)? hitA->getErrX() : 0.2;
	errorx[1] = (hitB->getErrX()!=0)? hitB->getErrX() : 0.2;      
	errorx[2] = (hitC->getErrX()!=0)? hitC->getErrX() : 0.2;   
	if(fNumMods>3) errorx[3] = (hitD->getErrX()!=0)? hitD->getErrX():0.2;
	errory[0] = (hitA->getErrY()!=0)? hitA->getErrY() : 0.1;
	errory[1] = (hitB->getErrY()!=0)? hitB->getErrY() : 0.1;      
	errory[2] = (hitC->getErrY()!=0)? hitC->getErrY() : 0.1;   
	if(fNumMods>3) errory[3] = (hitD->getErrY()!=0)? hitD->getErrY():0.1;
      }    
      else if(fUseModErrors==kTRUE){
	errorx[0]=fPosError[0];errory[0]=fPosError[1];    
	errorx[1]=fPosError[2];errory[1]=fPosError[3];    
	errorx[2]=fPosError[4];errory[2]=fPosError[5];    
	if(fNumMods>3){errorx[3]=fPosError[6];errory[3]=fPosError[7];}  
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
  if(hitBC!=0) delete hitBC; 
  if(hitAB!=0) delete hitAB; 
  if(hitAC!=0) delete hitAC;
  if(projSlopes!=0) delete[] projSlopes;
  if(origSlopes!=0) delete[] origSlopes;
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
  if(fHistoOff!=kTRUE) {
    fOutRoot->cd(newDirName);
    
    //Enter in the file the HMdcGeomRotation HMdcGeomVector
    for(Int_t i=0;i<fNumMods-1;i++){
      fRotMat[i].Write();
      fTranslation[i].Write();
    }  
    //storing the graph (it is not automatic!)
    //graph->Write();
    graphXchi2->Write();   
    graphYchi2->Write();
    graphXchi2X->Write();   
    graphXchi2Y->Write();
    graphYchi2X->Write();
    graphYchi2Y->Write();

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





void HMdcAligner::setSearchLimits(Float_t x, Float_t y, Float_t s){
  //
  // Limits of the square defined in the search procedure
  //
 
  fXArea = x; 
  fYArea = y; 
  fSArea = s; 
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





void HMdcAligner::setModErrors(Float_t errXModA,Float_t errYModA,
			       Float_t errXModB,Float_t errYModB,
			       Float_t errXModC,Float_t errYModC,
			       Float_t errXModD,Float_t errYModD){  
  //
  // Set the module errors in the fcn()
  //
  fUseModErrors=kTRUE;
  fPosError[0]=errXModA;fPosError[1]=errYModA;
  fPosError[2]=errXModB;fPosError[3]=errYModB;
  fPosError[4]=errXModC;fPosError[5]=errYModC;
  fPosError[6]=errXModD;fPosError[7]=errYModD;
}





void HMdcAligner::setSteps(Float_t s0,Float_t s1,Float_t s2,
			   Float_t s3, Float_t s4, Float_t s5,
			   Float_t s6, Float_t s7, Float_t s8,
			   Float_t s9, Float_t s10, Float_t s11,
			   Float_t s12, Float_t s13, Float_t s14,
			   Float_t s15, Float_t s16, Float_t s17){
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
			    Float_t l6, Float_t l7, Float_t l8,
			    Float_t l9, Float_t l10, Float_t l11,
			    Float_t l12, Float_t l13, Float_t l14,
			    Float_t l15, Float_t l16, Float_t l17){
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



void HMdcAligner::findRotXYBySampling(Int_t angleIter,Int_t iter){
  //
  //
  //if angleIter==1 work on X, if angleIter==2 work on Y

  ofstream *fout2=NULL;
  if (fNameAscii) fout2 = new ofstream(fNameAscii.Data(), ios::app);

  if(angleIter==1 && iter<1) divideInZones(1,fRotMat[0],fTranslation[0]);
  else if(angleIter==2 && iter<1) divideInZones(2,fRotMat[0],fTranslation[0]);
  else divideInZones(-3,fRotMat[0],fTranslation[0]);

  HGeomRotation correct, origRot, newRot;
  HGeomVector euler, result;
  Float_t testCorrectAngle, correctAngle;
  Float_t chi2X[40];
  Float_t chi2Y[40];
  Float_t angle_eval[40];
  Int_t counter;
  
  //for(Int_t angleIter=1;angleIter<3;angleIter++){      
  origRot = fRotMat[0];
  counter=0;
  testCorrectAngle = -0.020;
  for(Int_t i=0;i<40;i++){
    chi2X[i] = 0;
    chi2Y[i] = 0;
    angle_eval[i] = 0;
  }
  do{
    angle_eval[counter] = testCorrectAngle;
    if(angleIter==1){
      correct.setUnitMatrix();
      correct.setElement(cos(testCorrectAngle),4);
      correct.setElement(sin(testCorrectAngle),5);
      correct.setElement(-sin(testCorrectAngle),7);
      correct.setElement(cos(testCorrectAngle),8);
    }
    else {  
      correct.setUnitMatrix();
      correct.setElement(cos(testCorrectAngle),0);
      correct.setElement(sin(testCorrectAngle),2);
      correct.setElement(-sin(testCorrectAngle),6);
      correct.setElement(cos(testCorrectAngle),8);
    }
    newRot = origRot * correct;
    
    divideInZones(-3,newRot,fTranslation[0]);
      
    //revisar esta llamada. Tiene algun efecto? Hace falta? 
    //result = translationFinderZones(newRot,fTranslation[0]);
    //result = translationFinder(newRot,fTranslation[0]);
    //fillTranslation(0,result.getX(),result.getY(),result.getZ());
        
    //Propongo cambiar esta por una llamada que si centre la zona
    //4 (medio) mediante los offsets (cambio a -3)
    //divideInZones(-3,newRot,result);
    
    for (Int_t zo = 0;zo<9;zo++){
      chi2Y[counter] += (zones_Y[zo]-resZon_Y[zo])*(zones_Y[zo]-resZon_Y[zo]);
      chi2X[counter] += (zones_X[zo]-resZon_X[zo])*(zones_X[zo]-resZon_X[zo]);
    }
    
    testCorrectAngle=testCorrectAngle+0.001;	
    counter++;
  }while(testCorrectAngle<0.020);
  
  //Determinar aqui el segundo o tercer angulo a partir del chi2
  
  if (*fout2){
    *fout2 << endl<< "  Results Table -- angle, chi2X, chi2Y"<< endl;
    for(Int_t ele=0;ele<40;ele++)
      *fout2  << angle_eval[ele] << "            "
	      << chi2X[ele] << "            "
	      << chi2Y[ele] << endl;
    *fout2  << endl;
  }
  
  TF1 *f1  = new TF1("f1","pol2",angle_eval[0],angle_eval[40]);
  
  for(Int_t ele=0;ele<40;ele++){
    if(angleIter==1){
      graphXchi2Y->SetPoint(ele, angle_eval[ele], chi2Y[ele]);
      graphXchi2X->SetPoint(ele, angle_eval[ele], chi2X[ele]);
      graphXchi2->SetPoint(ele, angle_eval[ele], chi2X[ele]+2*chi2Y[ele]);
    }
    else {
      graphYchi2X->SetPoint(ele, angle_eval[ele], chi2X[ele]);
      graphYchi2Y->SetPoint(ele, angle_eval[ele], chi2Y[ele]);
      graphYchi2->SetPoint(ele, angle_eval[ele], chi2X[ele]+2*chi2Y[ele]);
    }
  }
  
  if(angleIter==1){	  
    graphXchi2Y->Fit("f1","RQNW");
    correctAngle = -(f1->GetParameter(1))/(2 * f1->GetParameter(2));
    graphXchi2X->Fit("f1","RQNW");
    if(*fout2){
      *fout2 << "Correction angle from chi2Y: " << correctAngle << endl
	     << "Correction angle from chi2X: " 
	     <<  -(f1->GetParameter(1))/(2 * f1->GetParameter(2)) << endl;
    }
    graphXchi2->Fit("f1","RQNW");
    correctAngle = -(f1->GetParameter(1))/(2 * f1->GetParameter(2));
  }
  else{	  
    graphYchi2X->Fit("f1","RQNW");
    correctAngle = -(f1->GetParameter(1))/(2 * f1->GetParameter(2));
    graphYchi2Y->Fit("f1","RQNW");
    if(*fout2){
      *fout2 << "Correction angle from chi2X: " << correctAngle << endl
	     << "Correction angle from chi2Y: " 
	     <<  -(f1->GetParameter(1))/(2 * f1->GetParameter(2)) << endl;
    }
    graphYchi2->Fit("f1","RQNW");
    correctAngle = -(f1->GetParameter(1))/(2 * f1->GetParameter(2));
  }
  
  //redoing around the minimum
  //TF1 *f2  = new TF1("f2","pol2",correctAngle-0.005,correctAngle+0.005);
  //graph->Fit("f2","RQNW");
  //fitPar1     = f2->GetParameter(1);
  //fitPar2     = f2->GetParameter(2);
  //new minimum
  //correctAngle= (-fitPar1)/(2 *fitPar2);
  
  if (*fout2) 
    *fout2 <<endl<<" The winner angle is " << correctAngle <<endl<<endl; 
  
  if(angleIter==1){  
    correct.setUnitMatrix();
    correct.setElement(cos(correctAngle),4);
    correct.setElement(sin(correctAngle),5);
    correct.setElement(-sin(correctAngle),7);
    correct.setElement(cos(correctAngle),8);
  }
  else {  
    correct.setUnitMatrix();
    correct.setElement(cos(correctAngle),0);
    correct.setElement(sin(correctAngle),2);
    correct.setElement(-sin(correctAngle),6);
    correct.setElement(cos(correctAngle),8);
  }
  newRot = origRot * correct;
  
  // I think this returns a position a little bit out
  // of the better estimate (focus defect in 2D plots).
  //I have found three mm of difference in z, probably due to: 
  //the rotation is found from chi2Y, while translationFinder()
  //tries to obtain the best for X and Y in the lower part (more statist.) 
  //now, analytical translation minimization after the rotation
  //divideInZones(-3,newRot,fTranslation[0]);
  //if(angleIter==1) result = translationFinderZones(newRot,fTranslation[0]);
  //else 
  //  if(fMin!=14 || fMin!=514)
  //    result = translationFinder();
  
  fRotMat[0] = newRot;
  euler = findEulerAngles(fRotMat[0]);    
  setEulerAngles(0,euler(0),euler(1),euler(2));
  
  if (*fout2){
    if(fMin==14 || fMin==514)
      *fout2 << endl << "Parameters after rotation around ";    
    else   
      *fout2 << endl << "Parameters after rotation (including ANALYTIC " 
	     << " translation minimization) around " ;
    if(angleIter==1) *fout2 << endl << " X axis"<< endl;
    else  *fout2 << endl << " Y axis"<< endl;
    for(Int_t i=0; i<fNumMods-1;i++){
      *fout2 << fEuler[i].getX() << ",  " << fEuler[i].getY() << ",  " 
	     << fEuler[i].getZ() << ",  " << fTranslation[i].getX() << ",  "
	     << fTranslation[i].getY() << ",  " << fTranslation[i].getZ() 
	     << endl;
    }	
    *fout2 << endl;	
  }
  
  if(angleIter==1) divideInZones(2,fRotMat[0],fTranslation[0]);
  else   divideInZones(3,fRotMat[0],fTranslation[0]);
}





void HMdcAligner::findRotXGeometrically(Int_t iter){
  //
  //
  //

  Float_t* rotResults2=0; 
  HGeomRotation correct2;
  HGeomRotation newRotation; 
  HGeomVector euler;
  ofstream *fout2=NULL;
  if (fNameAscii) fout2 = new ofstream(fNameAscii.Data(), ios::app);
  
  if(iter<1) divideInZones(1,fRotMat[0],fTranslation[0]);
  else divideInZones(-3,fRotMat[0],fTranslation[0]);
  
  rotResults2 = rotYXFinder(1,1);  
  
  correct2.setUnitMatrix();
  //Using rotResults2[2]:mean of fit around maximum
  correct2.setElement(cos(rotResults2[2]),4);
  correct2.setElement(sin(rotResults2[2]),5);
  correct2.setElement(-sin(rotResults2[2]),7);
  correct2.setElement(cos(rotResults2[2]),8);   
  
  newRotation = fRotMat[0] * correct2;
  if(!fUseZones){
    fRotMat[0] = newRotation;      
    euler = findEulerAngles(newRotation);    
    setEulerAngles(0,euler(0),euler(1),euler(2));
  }

  *fout2 << endl << "  *** Using all hits ***" << endl;
  *fout2 << " Second angle; rotX = " <<rotResults2[0]<<" in "<< rotResults2[6]
	 << " entries, with Variance " << rotResults2[3] << endl;
  *fout2 << " Negatives in root: " << rotResults2[7]; 
  *fout2 << "    Discarted (inner) hits: " << rotResults2[8] << endl;
  *fout2 << "usingfit; rotX = " << rotResults2[1] 
	 << " with sigma " << rotResults2[4] << endl;
  *fout2 << "usingfitcen; rotX = " << rotResults2[2] 
	 << " with sigma " << rotResults2[5] << endl;
  
  //Zones
  rotResults2 = rotYXFinder(0,1);
  
  correct2.setUnitMatrix();
  correct2.setElement(cos(rotResults2[0]),4);
  correct2.setElement(sin(rotResults2[0]),5);
  correct2.setElement(-sin(rotResults2[0]),7);
  correct2.setElement(cos(rotResults2[0]),8);
  
  newRotation = fRotMat[0] * correct2;
  if(fUseZones){
    fRotMat[0] = newRotation;
    //Also the fEuler[] should change to keep track of
    //the relative transformation euler parameters
    euler = findEulerAngles(newRotation);    
    setEulerAngles(0,euler(0),euler(1),euler(2));
  }  
  
  *fout2 << endl << "  *** Using Zones ***" << endl;
  *fout2 << " Second angle; rotX = " <<rotResults2[0]<<" in "<< rotResults2[6]
	 << " entries, with Variance " << rotResults2[3] << endl;
  *fout2 << " Negatives in root: " << rotResults2[7]; 
  *fout2 << "    Discarted (inner) hits: " << rotResults2[8] << endl;
}


void HMdcAligner::findRotYGeometrically(Int_t iter){
  //
  //
  //
  Float_t* rotResults3=0;   
  HGeomRotation correct3;
  HGeomRotation newRotation;  
  HGeomVector euler;
  ofstream *fout2=NULL;
  if (fNameAscii) fout2 = new ofstream(fNameAscii.Data(), ios::app);

  if(iter<1) divideInZones(2,fRotMat[0],fTranslation[0]);
  else divideInZones(-3,fRotMat[0],fTranslation[0]);

  rotResults3 = rotYXFinder(1);
  
  correct3.setUnitMatrix();
  correct3.setElement(cos(rotResults3[2]),0);
  correct3.setElement(sin(rotResults3[2]),2);
  correct3.setElement(-sin(rotResults3[2]),6);
  correct3.setElement(cos(rotResults3[2]),8);
  
  newRotation = fRotMat[0] * correct3;
  if(!fUseZones){
    fRotMat[0] = newRotation;      
    euler = findEulerAngles(newRotation);    
    setEulerAngles(0,euler(0),euler(1),euler(2));
  }
  
  *fout2 << endl << "  *** Using all hits ***" << endl;
  *fout2 << " Third angle; rotY = " <<rotResults3[0]<<" in "<< rotResults3[6]
	 << " entries, with Variance " << rotResults3[3] << endl;
  *fout2 << " Negatives in root: " << rotResults3[7]; 
  *fout2 << "    Discarted (inner) hits: " << rotResults3[8] << endl;
  *fout2 << "Additional info: " << endl;
  *fout2 << "usingfit; rotY = " << rotResults3[1] 
	 << " with sigma " << rotResults3[4] << endl;
  *fout2 << "usingfitcen; rotY = " << rotResults3[2] 
	 << " with sigma " << rotResults3[5] << endl;
  
  //Zones
  rotResults3 = rotYXFinder();  
  
  correct3.setUnitMatrix();
  correct3.setElement(cos(rotResults3[0]),0);
  correct3.setElement(sin(rotResults3[0]),2);
  correct3.setElement(-sin(rotResults3[0]),6);
  correct3.setElement(cos(rotResults3[0]),8);
  
  newRotation = fRotMat[0] * correct3;
  if(fUseZones){
    fRotMat[0] = newRotation;
    euler = findEulerAngles(newRotation);    
    setEulerAngles(0,euler(0),euler(1),euler(2));
  }
  
  *fout2 << endl << "  *** Using Zones ***" << endl;
  *fout2 << " Third angle; rotY = " <<rotResults3[0]<<" in "<< rotResults3[6]
	 << " entries, with Variance " << rotResults3[3] << endl;
  *fout2 << " Negatives in root: " << rotResults3[7]; 
  *fout2 << "    Discarted (inner) hits: " << rotResults3[8] << endl;
}





void HMdcAligner::findRotZGeometrically(Int_t iter){
  //
  //  
  //
  Float_t* rotResults=0;    
  HGeomRotation correct1; 
  HGeomRotation newRotation;
  HGeomVector euler;
  ofstream *fout2=NULL;
  if (fNameAscii) fout2 = new ofstream(fNameAscii.Data(), ios::app);

  if(iter<1) divideInZones(0,fRotMat[0],fTranslation[0]);
  else divideInZones(-3,fRotMat[0],fTranslation[0]); 

  rotResults = rotZFinder(1); 

  //Using rotResults2[2]: mean of fit around maximum
  correct1.setUnitMatrix();    
  correct1.setElement(cos(rotResults[2]),0);
  correct1.setElement(-sin(rotResults[2]),1);
  correct1.setElement(sin(rotResults[2]),3);
  correct1.setElement(cos(rotResults[2]),4);
  
  newRotation = fRotMat[0] * correct1;
  if(!fUseZones){
    fRotMat[0] = newRotation;      
    euler = findEulerAngles(newRotation);    
    setEulerAngles(0,euler(0),euler(1),euler(2));
  }
  
  *fout2 << endl << "  *** Using all hits ***" << endl;
  *fout2 << " First angle; rotZ = " << rotResults[0] 
	 <<" in " << rotResults[6]
	 << " entries, with Variance " << rotResults[3] << endl;
  *fout2 << "usingfit; rotZ = " << rotResults[1] 
	 << " with sigma " << rotResults[4] << endl;
  *fout2 << "usingfitcen; rotZ = " << rotResults[2] 
	 << " with sigma " << rotResults[5] << endl;
  
  //Zones
  rotResults = rotZFinder();  
  
  correct1.setElement(cos(rotResults[0]),0);
  correct1.setElement(-sin(rotResults[0]),1);
  correct1.setElement(sin(rotResults[0]),3);
  correct1.setElement(cos(rotResults[0]),4);
    
  newRotation = fRotMat[0] * correct1;
  if(fUseZones){
    fRotMat[0] = newRotation;
    //Also the fEuler[] should change to keep track of
    //the relative transformation euler parameters
    euler = findEulerAngles(newRotation);    
    setEulerAngles(0,euler(0),euler(1),euler(2));
  }
  
  *fout2 << endl << "  *** Using Zones ***" << endl;
  *fout2 << " First angle; rotZ = " << rotResults[0] <<" in " << rotResults[6]
	 << " entries, with Variance " << rotResults[3] << endl;
}





HGeomVector HMdcAligner::translationFinderZones(HGeomRotation rot,HGeomVector tran){
  //
  // Copy of translation Finder for Zones
  //

  Float_t a=0,b=0,c=0,d=0,e=0,f=0,g=0,h=0;
  Float_t thetaX=0,thetaY=0;
  Float_t resX=0,resY=0,resXstar=0,resYstar=0;
  Float_t Wx=1,Wy=1;
  Stat_t used=0;
  HGeomRotation rotaux;
  HGeomVector transaux;
  HGeomVector resultaux,result;  

  HGeomVector TargetPos = getLocalTarget(rot,tran);
  //projecting on MDC A
  rotaux = rot.inverse();
  transaux = -(rotaux * tran);
  for (Int_t i=0;i<9;i++) { 

    resX = zones_X[i] - resZon_X[i];
    resY = zones_Y[i] - resZon_Y[i];

    used++;
    Float_t s0=0,s1=0;

    if(fUseTarget) {
      s0 = (resZon_X[i]-TargetPos.getX())/(-TargetPos.getZ());
      s1 = (resZon_Y[i]-TargetPos.getY())/(-TargetPos.getZ());
    }
    else cout << "FATAL ERROR in translationFinderZones()" << endl;

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

  result = -(rot * resultaux);

  cout << "translationFinderZones(): used " << used 
       << " from  9" << endl;
  result.print();

  return result;
}





HGeomVector HMdcAligner::translationFinder(Int_t XYorBoth,
					   Int_t XLowLim, Int_t XUppLim,
					   Int_t YLowLim, Int_t YUppLim){
  //
  // Iterative call to the translationFinder algorithm for different
  // accepted Hit distributions
  
  HGeomVector result;
  
  if(!fUseCut) {//if the complete sample is used
    result = translationFinderRW(XYorBoth, XLowLim, XUppLim, 
				 YLowLim, YUppLim);
    return result; 
  }
  else{ 
    
    Int_t IterCounter = 0;
    Float_t IterCri = 0;      
    HGeomVector OldTranslation[(fNumMods-1)];
    
    ofstream *fout2=NULL;
    if (fNameAscii) fout2 = new ofstream(fNameAscii.Data(), ios::app);
    
    if (*fout2) *fout2 << endl << "ITERATIVE translationFinder!" << endl;
    
    do{
      IterCri = 0;      
      for(Int_t i=0;i<fNumMods-1;i++){
	OldTranslation[i] = fTranslation[i];
      }
      result = translationFinderRW(XYorBoth, XLowLim, XUppLim, 
				   YLowLim, YUppLim);

      fillTranslation(0,result.getX(),result.getY(),result.getZ());

      if (*fout2){
	*fout2 << "Parameters after minimization "  << endl;
	for(Int_t i=0;i<fNumMods-1;i++){
	  *fout2 << fTranslation[i].getX() << ",  " 
		 << fTranslation[i].getY() << ",  " 
		 << fTranslation[i].getZ() << endl;
	}		       
	*fout2 << "Using  " << fCountCut << "  combinations." << endl;
      }
      
      for(Int_t i=0;i<fNumMods-1;i++){    
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
	cout << "WARNING in HMdcAligner::translationFinder()" << endl;
	cout << "More than 10 iterations without results!" <<endl;
	break;
      }
      
      if(IterCri>fIterCriteria){
	fillHistograms(2);  
	fitHistograms(2);
      }
      
    }while(IterCri>fIterCriteria);
    
    if (*fout2){
      *fout2 << "Parameters after ANALYTIC translation minimization "  << endl;
      for(Int_t i=0;i<fNumMods-1;i++){
	*fout2 << fEuler[i].getX() << ",  " << fEuler[i].getY() << ",  " 
	       << fEuler[i].getZ() << ",  " << fTranslation[i].getX() << ",  " 
	       << fTranslation[i].getY() << ",  " << fTranslation[i].getZ() << endl;
      }	
      *fout2 << endl;	
    }
    
    fout2->close();
    delete fout2;
    
    return result; 
  }
}





HGeomVector HMdcAligner::translationFinderRW(Int_t XYorBoth,
					   Int_t XLowLim, Int_t XUppLim,
					   Int_t YLowLim, Int_t YUppLim){
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
  // Parameters can now be introduced to find the translation for
  // reduced set of hits, those between lower and upper limits in
  // each direction, selectable in the first param:
  // XYorBoth=-1 No Cut           XYorBoth=0 Cut on X
  // XYorBoth=1 Cut on Y          XYorBoth=2 Cut on both
  //

  HMdcHit* hitA;      
  HMdcHit* hitB;      
  HMdcHit* hitC;      
  HMdcHit* hitD; 
  HGeomVector projPoint;
  Float_t* origSlopes = new Float_t[2];
  Float_t* projSlopes = new Float_t[2];
  HGeomRotation rotaux;
  HGeomVector transaux;
  HGeomVector resultaux,result;  
  Float_t a=0,b=0,c=0,d=0,e=0,f=0,g=0,h=0;
  Float_t thetaX=0,thetaY=0;
  Float_t resX=0,resY=0,resS0=0,resS1=0,resXstar=0,resYstar=0;
  Float_t Wx=1,Wy=1,WxSx=0,WxSy=0,WySy=0;
  Stat_t entries, used=0;

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

    //continue (skip) if not in the correct interval
    if(XYorBoth!=-1){
      if(XYorBoth==0 && 
	 (hitA->getX() < XLowLim || hitA->getX() > XUppLim)) continue;
      if(XYorBoth==1 && 
	 (hitA->getY() < YLowLim || hitA->getY() > YUppLim)) continue;
      if(XYorBoth==2 && (
	 (hitA->getX() < XLowLim || hitA->getX() > XUppLim) || 
	 (hitA->getY() < YLowLim || hitA->getY() > YUppLim) )) continue;
    }
    used++;

    projPoint = findProjPoint(hitB,rotaux,transaux,projSlopes);
    transformToSlopes(hitA,origSlopes);

    resX = hitA->getX() - projPoint.getX();
    resY = hitA->getY() - projPoint.getY();
    resS0 = origSlopes[0]-projSlopes[0];
    resS1 = origSlopes[1]-projSlopes[1];

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

    resXstar = resX + transaux.getX() - thetaX * transaux.getZ();
    resYstar = resY + transaux.getY() - thetaY * transaux.getZ();

    //Introducing the covariance 
    if(fUseUnitErrors!=kTRUE){
      //Inverse covariance matrix
      Float_t det = ( (fSigmaX*fSigmaX+fSigmaY*fSigmaY+
		       fSigmaS0*fSigmaS0+fSigmaS1*fSigmaS1) *
		      (1 - fRhoxSy*fRhoxSy - fRhoxSx*fRhoxSx - 
		       fRhoySy*fRhoySy + fRhoxSx*fRhoxSx*fRhoySy*fRhoySy) );
      
      Wx = (fSigmaY*fSigmaS0*fSigmaS1-
	    fRhoySy*fRhoySy*fSigmaY*fSigmaY*fSigmaS1*fSigmaS1*fSigmaS0)/det;
      Wy = (fSigmaX*fSigmaS0*fSigmaS1-
	    fRhoxSy*fRhoxSy*fSigmaX*fSigmaX*fSigmaS1*fSigmaS1*fSigmaS0-
	    fRhoxSx*fRhoxSx*fSigmaX*fSigmaX*fSigmaS0*fSigmaS0*fSigmaS1)/det;
      WxSx = -(fRhoxSx*fSigmaX*fSigmaS0*fSigmaY*fSigmaS1-
	       fRhoySy*fRhoySy*fSigmaY*fSigmaY*fSigmaS1*
	       fSigmaS1*fRhoxSx*fSigmaX*fSigmaS0)/det;
      WySy = -(fSigmaX*fSigmaS0*fRhoySy*fSigmaY*fSigmaS1-
	       fRhoxSx*fRhoxSx*fSigmaX*fSigmaX*fSigmaS0*fSigmaS0*
	       fRhoySy*fSigmaY*fSigmaS1)/det;
      WxSy = -(fRhoxSx*fSigmaX*fSigmaS0*fSigmaY*fRhoxSy*fSigmaX*fSigmaS1)/det;

      if(fDoNotUseCov){
	resXstar = resX + transaux.getX() - thetaX * transaux.getZ();
	resYstar = resY + transaux.getY() - thetaY * transaux.getZ();
      }
      else{
	resXstar = resX + transaux.getX() - thetaX * transaux.getZ() 
	  + (WxSx/Wx) * resS0 + (WxSy/Wx) * resS1 ; 
	resYstar = resY + transaux.getY() - thetaY * transaux.getZ() 
	  + (WxSy/Wy) * resS1 ;
      }
    }
    else{
      Wx = 1;
      Wy = 1;
      resXstar = resX + transaux.getX() - thetaX * transaux.getZ();
      resYstar = resY + transaux.getY() - thetaY * transaux.getZ();
    }
    
    a+=1*Wx;
    b+=1*Wy;
    c+=-thetaX*Wx;
    d+=-thetaY*Wy;
    e+=thetaX*thetaX*Wx+thetaY*thetaY*Wy;
    f+=resXstar*Wx;
    g+=resYstar*Wy;
    h+=-(thetaX*resXstar*Wx+thetaY*resYstar*Wy);
    
  }
  
  Float_t den = a*b*e-c*b*c-a*d*d;
  resultaux.setX((f*b*e+g*d*c-b*c*h-f*d*d)/den);
  resultaux.setY((a*g*e+f*d*c-g*c*c-d*h*a)/den);
  resultaux.setZ((a*b*h-f*b*c-g*d*a)/den);
  //again we should invert the transformation    

  result = -(fRotMat[0] * resultaux);

  cout << "translationFinder("; 
  if(XYorBoth!=-1) cout << XYorBoth << ", " << XLowLim<< "," << XUppLim ;
  if(XYorBoth>0) cout << ", " << YLowLim<< "," << YUppLim; 
  cout << "): used " << used << " from  "<< entries << endl;
  result.print();


  if(projSlopes!=0) delete[] projSlopes;
  return result;

  //quiza luego pueda hacer tambien el inverso,
  //esto es, proyectar en MDC B y comprobar que parametros
  //salen de la minimizacion analitica.
}



void HMdcAligner::divideInZones(Int_t loop,HGeomRotation rot,HGeomVector tran){
  //
  // Divide in several zones and find for each one
  // the mean residual position 
  // See setZones() for the definition
  //
  HMdcHit* hitA;      
  HMdcHit* hitB;      
  HGeomVector projPoint;
  Float_t* projSlopes = new Float_t[2];
  HGeomRotation rotaux;
  HGeomVector transaux;
  Stat_t entries;
  Float_t xhit=0,yhit=0,xpro=0,ypro=0; 
  //Bool_t doNotStore=kFALSE;
  Bool_t doNotCenter=kFALSE;
  
  //histograms
  //  TF1 *f1 = new TF1("f1","gaus",-25,25);
  TF1 *f1 = new TF1("f1","gaus",-10,10);
  
  //projecting on MDC A
  rotaux = rot.inverse();
  transaux = -(rotaux * tran);
 
  if(loop<0){
    if(loop==-1) doNotCenter=kTRUE; 
    loop = 3;
  }

  for(Int_t index =0;index<9;index++){
    BvsAinACS_X_Zone[index+(9*loop)]->Reset();
    BvsAinACS_Y_Zone[index+(9*loop)]->Reset();
    BvsAinACS_Zon[loop]->Reset();
    //BvsAinACS_Zon2[loop]->Reset();
  }

  //After testing the cutted distribution, shows problems due to 
  //the asymmetry introduced by the cuts (and the low statistics).
  //this is mainly true in the rotXYFinder()  
  //if(fUseCut) entries= fAlignAllCut->GetEntries();
  //else  entries= fAlignAll->GetEntries();
  entries= fAlignAll->GetEntries();
  

  for (Int_t i=0;i<entries;i++) {  
    //After testing the cutted distribution, shows problems due to 
    //the asymmetry introduced by the cuts (and the low statistics).
    //this is mainly true in the rotXYFinder()    
    //if(fUseCut) fAlignAllCut->GetEntry(i);
    //else fAlignAll->GetEntry(i);
    fAlignAll->GetEntry(i);
    
    hitA = (HMdcHit*) fHits->At(0);
    hitB = (HMdcHit*) fHits->At(1);
    
    projPoint = findProjPoint(hitB,rotaux,transaux,projSlopes);
    xhit = hitA->getX();
    xpro = projPoint.getX();
    yhit = hitA->getY();
    ypro = projPoint.getY();	
    
    for(Int_t index=0;index<9;index++){
      if( xhit > zones_X[index]-zones_DX[index]/2 && 
	  xhit < zones_X[index]+zones_DX[index]/2 && 
	  yhit > zones_Y[index]-zones_DY[index]/2 && 
	  yhit < zones_Y[index]+zones_DY[index]/2 ) {
    	BvsAinACS_X_Zone[index+(9*loop)]->Fill(xhit-xpro);
	BvsAinACS_Y_Zone[index+(9*loop)]->Fill(yhit-ypro);
	BvsAinACS_Zon[loop]->Fill(xhit-xpro+zones_X[index]/10,
				  yhit-ypro+zones_Y[index]/10);
      } 
    }
  }
  
  Float_t offsetX=0.,offsetY=0.;
  
  for(Int_t index =0;index<9;index++){
    BvsAinACS_X_Zone[index+(9*loop)]->Fit("f1","RQN");
    resZon_X[index] = zones_X[index] - f1->GetParameter(1);//loc-pro -> pro-loc
    if(index==4) offsetX= f1->GetParameter(1); 
    BvsAinACS_Y_Zone[index+(9*loop)]->Fit("f1","RQN");
    resZon_Y[index] = zones_Y[index] - f1->GetParameter(1);//loc-pro -> pro-loc
    if(index==4) offsetY= f1->GetParameter(1); 
    
    if(A_DEBUG>3){
      cout << "DEBUG: index=" << index << "  resZon_X[index]=" 
	   << resZon_X[index] << "  resZon_Y[index]=" << resZon_Y[index] << endl;
      cout << "  zones_X[index]=" << zones_X[index] 
	   << "  zones_Y[index]=" << zones_Y[index] << endl;
    }
  }
    
  //trying to center perfectly the central zone 
  //the previous offset parameters are defined just for this centering 
  //do not perform this if sampling (doNotCenter==kTRUE or loop==-1)
  if(doNotCenter==kFALSE) {
    for(Int_t index =0;index<9;index++){
      if(A_DEBUG>3){
	cout << "offsetX = " << offsetX<< endl;
	cout << "offsetY = " << offsetY<< endl;
      }
      resZon_X[index] = resZon_X[index] + offsetX;
      resZon_Y[index] = resZon_Y[index] + offsetY; 
      if(A_DEBUG>3){
	cout << "resZon_X["<< index<< "] = " << resZon_X[index] << endl;
	cout << "resZon_Y["<< index<< "] = " << resZon_Y[index] << endl;
      }
    }
  }

  //for(Int_t index =0;index<9;index++)
  //  BvsAinACS_Zon2[loop]->Fill(-resZon_X[index]+zones_X[index]
  //			       +zones_X[index]/10,
  //			       -resZon_Y[index]+zones_Y[index]
  //			       +zones_Y[index]/10);
  // 

  if(projSlopes!=0) delete[] projSlopes;
  if(f1!=0) delete f1;

}



Float_t* HMdcAligner::rotZFinder(Int_t noZones){
  //
  // Finding the rotation in local XY plane (around local Z)
  //

  HMdcHit* hitA;      
  HMdcHit* hitB;      
  HGeomVector projPoint;
  Float_t* projSlopes = new Float_t[2];
  HGeomRotation rotaux;
  HGeomVector transaux;  
  Float_t* rotvalues = new Float_t[7]; 
  Float_t rotZ=0., rotZmean=0., rotZfit=0., rotZfitcen=0.;
  Float_t rotZsqr=0., sigmafit=0., sigmafitcen=0.;
  //Float_t rotZWmean=0., rotZWsqr=0.;  
  Float_t xhit=0,yhit=0,xpro=0,ypro=0;//,weight=1; 
  Stat_t count=0;//,wcount=0;

  //projecting on MDC A
  rotaux = fRotMat[0].inverse();
  transaux = -(rotaux * fTranslation[0]);

  if(noZones != 0){
    Stat_t entries;
    hisrotZ->Reset();

    //After testing the cutted distribution, shows problems due to 
    //the asymmetry introduced by the cuts (and the low statistics).
    //this is mainly true in the rotXYFinder()
    //if(fUseCut) entries= fAlignAllCut->GetEntries();
    //else  entries= fAlignAll->GetEntries();
    entries= fAlignAll->GetEntries();

    count = entries;    
    for (Int_t i=0;i<entries;i++) {//specify argument in function for all hits

      //After testing the cutted distribution, shows problems due to 
      //the asymmetry introduced by the cuts (and the low statistics).
      //this is mainly true in the rotXYFinder()
      //if(fUseCut) fAlignAllCut->GetEntry(i);
      //else fAlignAll->GetEntry(i);
      fAlignAll->GetEntry(i);

      hitA = (HMdcHit*) fHits->At(0);
      hitB = (HMdcHit*) fHits->At(1);
      
      projPoint = findProjPoint(hitB,rotaux,transaux,projSlopes);
      xhit = hitA->getX();
      xpro = projPoint.getX();
      yhit = hitA->getY();
      ypro = projPoint.getY();
      
      rotZ = atan2(xhit*ypro-yhit*xpro,xhit*xpro+yhit*ypro);
      //weight: for y<500 is w=11/15+4/7500*y for a weigth 1/5 for lower y's)
      //if(fLoc[1]>1){
      //if(yhit>500) weight=1;
      //else weight = (11/15) + (4/7500)*yhit;
      //}
      //else weight = (11/15) + (4/7500)*yhit; //calcular este coeficiente!

      hisrotZ->Fill(rotZ);
      //hisrotZW->Fill(rotZ,weight);

      rotZmean += rotZ;
      rotZsqr  += rotZ*rotZ;
      //rotZWmean += rotZ*weight;
      //rotZWsqr  += rotZ*rotZ*weight*weight;
      //wcount += weight; 
    }
    rotZmean = rotZmean / count;
    //rotZWmean= rotZWmean / wcount;
    cout << endl << endl;
    cout << "rotZ (first rotation)"<<endl;
    cout << "rotZmean=" << rotZmean;
    //cout << "rotZWmean=" << rotZWmean << endl;
    
    TF1 *f1 = new TF1("f1","gaus",-0.2,0.2);
    hisrotZ->Fit("f1","RQN");
    rotZfit = f1->GetParameter(1);
    sigmafit = f1->GetParameter(2);
    TF1 *f2 = new TF1("f2","gaus",rotZfit-1.64*sigmafit,rotZfit+1.64*sigmafit);
    hisrotZ->Fit("f2","RQN");
    rotZfitcen = f2->GetParameter(1);
    sigmafitcen = f2->GetParameter(2);
    
    if(fCloseToSolution){  
      //Warning: the method depends on the number of bins in the histos    
      //if maximum is one of the center bins
      rotZfitcen=0;
      Float_t entriesTot=0;
      for(Int_t co=498;co<504;co++){
	rotZfitcen += hisrotZ->GetBinContent(co)*hisrotZ->GetBinCenter(co);
	entriesTot += hisrotZ->GetBinContent(co);
      }
      rotZfitcen = rotZfitcen/entriesTot;
    }
    else{
      if((f2->GetChisquare()/f2->GetNDF())>(f1->GetChisquare()/f1->GetNDF())){
	rotZfitcen = f1->GetParameter(1);
	sigmafitcen = f1->GetParameter(2);
      }
    }
    
    rotvalues[0] = rotZmean;
    rotvalues[1] = rotZfit;
    rotvalues[2] = rotZfitcen;
    rotvalues[3] = (rotZsqr / count) - (rotZmean*rotZmean); //variance
    rotvalues[4] = sigmafit;
    rotvalues[5] = sigmafitcen;    
    rotvalues[6] = count;  

    if(f1!=0) delete f1;
    if(f2!=0) delete f2;
  }  
  else {
    count = 9;
    hisrotZZon->Reset();

    for (Int_t i=0;i<9;i++) {//without argument use the fit on zones
      xhit = zones_X[i];
      xpro = resZon_X[i];
      yhit = zones_Y[i];
      ypro = resZon_Y[i];
      if(xhit==0 && yhit==0) count--;
      else{
	//cout << endl <<"Datos in rotZFinder(): " << xhit << " " 
	//     << xpro << " " << yhit << " " << ypro << endl; 
	rotZ = atan2(xhit*ypro-yhit*xpro,xhit*xpro+yhit*ypro);
	hisrotZZon->Fill(rotZ);
	rotZmean += rotZ;
	rotZsqr  += rotZ*rotZ;
      }
    }
    rotZmean = rotZmean / count; //mean
    rotvalues[0] = rotZmean;
    rotvalues[1] = 0.;
    rotvalues[2] = 0.;
    rotvalues[3] = (rotZsqr / count) - (rotZmean*rotZmean); //variance
    rotvalues[4] = 0.;    
    rotvalues[5] = 0.;
    rotvalues[6] = count;        
  }

  if(projSlopes!=0) delete[] projSlopes;

  return rotvalues;
}





Float_t* HMdcAligner::rotYXFinder(Int_t noZones, Int_t YorX){
  //
  // Finding the rotation in local XZ plane (around local Y)
  // or around local YZ plane (around local X)
  // If second argument=0, then rotations around local Y are found
  //

  HMdcHit* hitA;      
  HMdcHit* hitB;      
  HGeomVector projPoint;
  Float_t* projSlopes = new Float_t[2];
  HGeomRotation rotaux;
  HGeomVector transaux;   
  Float_t* rotvalues = new Float_t[9]; 
  Float_t rot=0., rotmean=0., rotfit=0., rotfitcen=0.;
  Float_t rotsqr=0., sigmafit=0., sigmafitcen=0.;
  //Float_t rotfitPos=0., rotfitNeg=0.,sigmafitPos=0.,sigmafitNeg=0.; 
  //Float_t rotWmean=0., rotWsqr=0.;  
  Float_t fnum=0, snum=0, den=0, totalpos=0, totalneg=0;
  Float_t total=0, a=0, c=0, hit=0, pro=0;//, yhit=0, weight=1; 
  Int_t snumneg=0, innerhits=0;
  Stat_t count=0;//,wcount=0;
  
  HGeomVector localTarget = fRotMat[0].inverse() * 
    (fTargetPos-fTranslation[0]);
  Float_t xTar = localTarget.getX();
  Float_t yTar = localTarget.getY();
  Float_t zTar = localTarget.getZ();
  
  //projecting on MDC A
  rotaux = fRotMat[0].inverse();
  transaux = -(rotaux * fTranslation[0]);
  
  if(noZones != 0){
    Stat_t entries;

    //Reseting histos to allow iteration
    if(YorX !=0) {
      hisrotXNeg->Reset();
      hisrotXPos->Reset();
      hisrotX->Reset();
    }
    else{   
      hisrotYNeg->Reset();
      hisrotYPos->Reset();
      hisrotY->Reset();
    }
    //After testing the cutted distribution, shows problems due to 
    //the asymmetry introduced by the cuts (and the low statistics).
    //this is mainly true in the rotXYFinder()
    //if(fUseCut) entries= fAlignAllCut->GetEntries();
    //else  entries= fAlignAll->GetEntries();
    entries= fAlignAll->GetEntries();

    count = entries;    
    for (Int_t i=0;i<entries;i++) { 
      //After testing the cutted distribution, shows problems due to 
      //the asymmetry introduced by the cuts (and the low statistics).
      //this is mainly true in the rotXYFinder()
      //if(fUseCut) fAlignAllCut->GetEntry(i);
      //else fAlignAll->GetEntry(i);
      fAlignAll->GetEntry(i);

      hitA = (HMdcHit*) fHits->At(0);
      hitB = (HMdcHit*) fHits->At(1);
      
      projPoint = findProjPoint(hitB,rotaux,transaux,projSlopes);
      if(YorX !=0) {
	hit = hitA->getY();     
	pro = projPoint.getY();
      }
      else{
	hit = hitA->getX();
	pro = projPoint.getX();
      }
      //Anyway for weighting       
      //yhit = hitA->getY();

      if(fabs(pro)>100) {	//external hit
	if(YorX !=0) a = hit-yTar;
	else a = hit-xTar;
	c = -zTar;
	fnum = (a*pro) / (c*hit);
	snum = 1 + ((a*a)/(c*c))  - ((pro*pro)/(hit*hit));
	den = 1 + (a*a)/(c*c);
	if(snum>0) {	        //positive root 
	  totalneg = (fnum - sqrt(snum)) / den;  
	  if(totalneg>1 || totalneg < -1) 
	    cout << "FATAL ERROR IN rotYXFinder()" << endl;
	  else{
	    rot = asin(totalneg);      
	    if(YorX !=0) hisrotXNeg->Fill(rot);
	    else hisrotYNeg->Fill(rot);
	  }
	  totalpos = (fnum + sqrt(snum)) / den;
	  if(totalpos>1 || totalpos < -1) 
	    cout << "FATAL ERROR IN rotYXFinder()" << endl;
	  else{
	    rot = asin(totalpos);	  
	    if(YorX !=0) hisrotXPos->Fill(rot);
	    else hisrotYPos->Fill(rot);
	  }
	  if(fabs(totalpos)>fabs(totalneg)) total = totalneg;
	  else total = totalpos;
	  rot = asin(total);

	  //weight      
	  //if(fLoc[1]>1){
	  //  if(yhit>500) weight=1;
	  //  else weight = (11/15) + (4/7500)*yhit;
	  // }
	  //else weight = (11/15) + (4/7500)*yhit; //calcular este coeficiente!

	  if(YorX !=0) hisrotX->Fill(rot);
	  else hisrotY->Fill(rot);
	  //if(YorX !=0) hisrotXW->Fill(rot,weight);
	  //else hisrotYW->Fill(rot,weight);

	  rotmean += rot;	  
	  rotsqr  += rot*rot;
	  //rotWmean += rot*weight;
	  //rotWsqr  += rot*rot*weight*weight;
	  //wcount += weight; 
	}
	else {      //negative root 
	  snumneg++;   
	  count--;
	}
      }
      else {        //inner hit
	innerhits++;       
	count--;
      }
    }   
    rotmean = rotmean / count;
    //rotWmean= rotWmean / wcount;
    cout << endl << endl;
    cout << "rot (second and third rotation)"<<endl;
    cout << "rotmean=" << rotmean;
    //cout << "rotWmean=" << rotWmean << endl;

    
    if(!fCloseToSolution){
      TF1 *f1 = new TF1("f1","gaus",-0.2,0.2);
      
      if(YorX !=0) hisrotX->Fit("f1","RQN");
      else hisrotY->Fit("f1","RQN");
      rotfit = f1->GetParameter(1); 
      sigmafit = f1->GetParameter(2); 
      
      TF1 *f2 = new TF1("f2","gaus",rotfit-1.64*sigmafit,rotfit+1.64*sigmafit);
      
      if(YorX !=0) hisrotX->Fit("f2","RQN");    
      else hisrotY->Fit("f2","RQN");
      rotfitcen = f1->GetParameter(1); 
      sigmafitcen = f1->GetParameter(2); 

      /*
      if(YorX !=0) hisrotXPos->Fit("f1","RQN");
      else hisrotYPos->Fit("f1","RQN");
      rotfitPos = f1->GetParameter(1); 
      sigmafitPos = f1->GetParameter(2); 
      if(YorX !=0) hisrotXNeg->Fit("f1","RQN");
      else hisrotYNeg->Fit("f1","RQN");
      rotfitNeg = f1->GetParameter(1); 
      sigmafitNeg = f1->GetParameter(2); 
      */
      
      //trying to decide if fit is good
      if(f2->GetChisquare()/f2->GetNDF()<5.){
	rotfitcen = f2->GetParameter(1); 
	sigmafitcen = f2->GetParameter(2); 
      }
      else if(YorX ==0){      
	//Warning: the method depends on the number of bins in the histos    
	//Only for Y histograms, sometimes are problematic
	cout << "WARNING:  Bad fit in rotXYFinder()" << endl;
	cout << "Mean of center bins" << endl;
	cout << "DUMMY FIT SIGMA IN THE TEXT OUTPUT (999)!!!!!" << endl;
	rotfitcen=0;
	Float_t entriesTot=0;
	for(Int_t co=491;co<511;co++){
	  rotfitcen += hisrotY->GetBinContent(co)*hisrotY->GetBinCenter(co);
	  entriesTot += hisrotY->GetBinContent(co);
	}
	rotfitcen = rotfitcen/entriesTot;
	sigmafitcen = 999;
      }
      else{      
	//Warning: the method depends on the number of bins in the histos    
	cout << "WARNING:  Bad second fit in rotXYFinder()" << endl;
	cout << "Mean of center bins" << endl;
	cout << "DUMMY FIT SIGMA IN THE TEXT OUTPUT (999)!!!!!" << endl;
	rotfitcen=0;
	Float_t entriesTot=0;
	for(Int_t co=498;co<504;co++){
	  rotfitcen += hisrotX->GetBinContent(co)*hisrotX->GetBinCenter(co);
	  entriesTot += hisrotX->GetBinContent(co);
	}
	rotfitcen = rotfitcen/entriesTot;
	sigmafitcen = 999;   

	if(f1!=0) delete f1;
	if(f2!=0) delete f2;
      }
    }
    else{ 
      if(YorX !=0){
	//Warning: the method depends on the number of bins in the histos    
	rotfitcen=0;
	Float_t entriesTot=0;
	for(Int_t co=498;co<504;co++){
	  rotfitcen += hisrotX->GetBinContent(co)*hisrotX->GetBinCenter(co);
	  entriesTot += hisrotX->GetBinContent(co);
	}
	rotfitcen = rotfitcen/entriesTot;
      }
      else{
	TF1 *f3 = new TF1("f3","gaus",-0.03,0.01);
	TF1 *f4 = new TF1("f4","gaus",-0.02,0.02);
	TF1 *f5 = new TF1("f5","gaus",-0.01,0.03);
	
	hisrotY->Fit("f3","RQN");
	hisrotY->Fit("f4","RQN");
	hisrotY->Fit("f5","RQN");
	
	if( (f3->GetChisquare()/f3->GetNDF())<(f4->GetChisquare()/f4->GetNDF()) && 
	    (f3->GetChisquare()/f3->GetNDF())<(f5->GetChisquare()/f5->GetNDF()) &&
	    fabs(f3->GetParameter(1))<0.02){
	  rotfitcen = f3->GetParameter(1); 
	  sigmafitcen = f3->GetParameter(2);
	}
	else if( (f5->GetChisquare()/f5->GetNDF())<(f4->GetChisquare()/f4->GetNDF()) &&
		 fabs(f5->GetParameter(1))<0.02){
	  rotfitcen = f5->GetParameter(1); 
	  sigmafitcen = f5->GetParameter(2);
	}
	else{	
	  rotfitcen = f4->GetParameter(1); 
	  sigmafitcen = f4->GetParameter(2);
	}
	
	if(f3!=0) delete f3;
	if(f4!=0) delete f4;
	if(f5!=0) delete f5;
      }
    }
    //limit to the solution in an uncontrolled case (trying to save the run)
    if(rotfitcen>0.05) {
      cout <<"WARNING:  Uncontrolled or far from solution in  rotXYFinder()" 
	   << endl;
      rotfitcen=0.01;sigmafitcen = 88888;
    }
    if(rotfitcen<-0.05) {     
      cout <<"WARNING:  Uncontrolled or far from solution in  rotXYFinder()" 
	   << endl;
      rotfitcen=-0.01;sigmafitcen = 88888;
    }


    rotvalues[0] = rotmean;
    rotvalues[1] = rotfit;
    rotvalues[2] = rotfitcen;
    rotvalues[3] = (rotsqr / count) - (rotmean*rotmean); //variance
    rotvalues[4] = sigmafit;
    rotvalues[5] = sigmafitcen;    
    rotvalues[6] = count;
    rotvalues[7] = snumneg;
    rotvalues[8] = innerhits;
    
  }
  else{    //Zones
    //Introducing oP = offsetPlane;
    Float_t oP= 0.;

    //Reseting histos to allow iteration
    hisrotXZonNeg->Reset();
    hisrotXZonPos->Reset();
    hisrotXZon->Reset();
    hisrotYZonNeg->Reset();
    hisrotYZonPos->Reset();
    hisrotYZon->Reset();
    
    count = 9;
    for(Int_t i=0;i<9;i++){
      if(YorX !=0){
	hit = zones_Y[i];
	pro = resZon_Y[i];
      }
      else{	
	hit = zones_X[i];
	pro = resZon_X[i];
      } 
      if(fabs(pro)>100){        //external hit
	if(YorX !=0) a = hit-yTar;
	else a = hit-xTar;
	c = -zTar;
	//Introducing oP = offsetPlane; modifies pro -> (a/c * oP + pro)
	pro = ((a/c) * oP + pro);
	fnum = (a*pro) / (c*hit);
	snum = 1 + ((a*a)/(c*c))  - ((pro*pro)/(hit*hit));
	den = 1 + (a*a)/(c*c);
	if(snum>0) {            //positive root 
	  totalneg = (fnum - sqrt(snum)) / den;  
	  if(totalneg>1 || totalneg < -1)
	    cout << "FATAL ERROR IN rotYXFinder()" << endl;
	  else{
	    rot = asin(totalneg);	    
	    if(YorX !=0) hisrotXZonNeg->Fill(rot);
	    else hisrotYZonNeg->Fill(rot);
	  }	  
	  totalpos = (fnum + sqrt(snum)) / den; 
	  if(totalpos>1 || totalpos < -1) 
	    cout << "FATAL ERROR IN rotYXFinder()" << endl;
	  else{
	    rot = asin(totalpos);	  
	    if(YorX !=0) hisrotXZonPos->Fill(rot);
	    else hisrotYZonPos->Fill(rot);
	  }
	  if(fabs(totalpos)>fabs(totalneg)) total = totalneg;
	  else total = totalpos;
	  rot = asin(total);
	  if(YorX !=0) hisrotXZon->Fill(rot);
	  else hisrotYZon->Fill(rot);
	  rotmean += rot;      
	  rotsqr  += rot*rot;
	}
	else {     //negative root
	  snumneg++;   
	  count--;
	}
      }
      else {        //inner hit
	innerhits++;  
	count--;
      }
    }
    rotmean = rotmean / count;

    rotvalues[0] = rotmean;  //mean
    rotvalues[1] = 0.;
    rotvalues[2] = 0.;
    rotvalues[3] = (rotsqr / count) - (rotmean*rotmean); //variance
    rotvalues[4] = 0.;
    rotvalues[5] = 0.;
    rotvalues[6] = count;
    rotvalues[7] = snumneg;
    rotvalues[8] = innerhits;
    
  }
  
  if(projSlopes!=0) delete[] projSlopes;

  return rotvalues;
}



void HMdcAligner::minfit(Int_t fix, HGeomVector* fE, HGeomVector* fT){ 
  //
  // Minuit menu
  // Argument fix correspon to fFix value (see setFix())
  // Other arguments are init values for the parameters!
  //

  Double_t args[2]={0,0}; 
  Int_t err = 0; 
  Float_t* limitL;
  Float_t* limitH;
  limitL = new Float_t[18];
  limitH = new Float_t[18];
  Double_t parresult[18];
  Double_t oldparresult[18];

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
  if(fNumMods==2) minuit->SetFCN(fcnalign);     
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
  if(A_DEBUG<3){ 
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
	cout << "WARNING in HMdcAligner :: minfit -> Method (105)" << endl;
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





void HMdcAligner::minfitRot(Int_t fix, Double_t first, Double_t second){
  //
  // Minuit menu for rotations
  // Arguments are init values for the parameters!
  //

  Double_t args[2]={0,0}; 
  Int_t err = 0; 
  Double_t start_val[2] = {first,second};
  Double_t errors[2];

  if(fix==0) setMinFlag(0);
  else setMinFlag(1);

  TMinuit *minuit=new TMinuit(2);
  //  if(fMin==14)   minuit=new TMinuit(1);
  //else minuit=new TMinuit(2);


  //setting the minimization function
  minuit->SetFCN(fcnRot);
  //if(fMin==14)minuit->SetFCN(fcnRotDist);  
  //else minuit->SetFCN(fcnRot);      
  minuit->SetObjectFit(this);
  
  Char_t pname[10];
  Int_t parnum;
  //  fMin==14?parnum=1:parnum=2;
  parnum=2;
  for(Int_t i=0;i<parnum;i++){
    sprintf(pname,"%s%i","par",i);
    minuit->mnparm(i,pname,start_val[i],0.1,0,0,err); 
  }

  //To remove all printout
  //  if(A_DEBUG<3){ 
  // minuit->SetPrintLevel(-1);
  //}

  minuit->mnexcm("MIGRAD",args,0,err); 
  Double_t parresult[parnum];
  for(Int_t i=0;i<parnum;i++){
    minuit->GetParameter(i,parresult[i],errors[i]);
  }
  
  if(A_DEBUG>3){
    cout << "RESULTS FROM MINUIT ROT" << endl;
    cout << "Phi=" << parresult[0] << "+-"  << errors[0] << endl;
    cout << "Theta=" << parresult[1] << "+-"  << errors[1] << endl;
  }

  //  HGeomVector euler;
  /*
  if(fMin==14){      
    HGeomRotation correct;  
    if(fix==0){    
      correct.setElement(cos(parresult[0]),4);
      correct.setElement(sin(parresult[0]),5);
      correct.setElement(-sin(parresult[0]),7);
      correct.setElement(cos(parresult[0]),8);
      
      fRotMat[0] = fRotMat[0] * correct;
      
      //Also the fEuler[] should change to keep track of
      //the relative transformation euler parameters
      euler = findEulerAngles(fRotMat[0]);    
      setEulerAngles(0,euler(0),euler(1),euler(2));
    }
    else{
      correct.setElement(cos(parresult[0]),0);
      correct.setElement(sin(parresult[0]),2);
      correct.setElement(-sin(parresult[0]),6);
      correct.setElement(cos(parresult[0]),8);

      fRotMat[0] = fRotMat[0] * correct;
      
      //Also the fEuler[] should change to keep track of
      //the relative transformation euler parameters
      euler = findEulerAngles(fRotMat[0]);    
      setEulerAngles(0,euler(0),euler(1),euler(2));
    }
  }
  else{ 
  */

    Float_t a=0,b=0,c=0,A=0,B=0,C=0;
    Float_t num=0,num2=0,den=0,psi=0;
    HGeomRotation correction; 
    HGeomRotation newRotation;
    
    HGeomVector Target = getLocalTarget(fRotMat[0],fTranslation[0]);  
    Float_t xTar = Target.getX();
    Float_t yTar = Target.getY();
    Float_t zTar = Target.getZ();
    
    //Calculating psi (TODO: for all zones!!!!) 
    a = resZon_X[0]-xTar; 
    b = resZon_Y[0]-yTar;  
    c =            -zTar; 
    A = b*resZon_X[0]-a*resZon_Y[0];
    B = c*resZon_X[0]           ;
    C = c*resZon_Y[0]           ;
    
    num = resZon_X[0]* ( (-A)*sin(parresult[1])-B*sin(parresult[0])*cos(parresult[1])+
			 C*cos(parresult[0])*cos(parresult[1]) ) -
      resZon_Y[0] * (B*cos(parresult[0])+C*sin(parresult[0]) );
    num2 = resZon_Y[0]* ( (-A)*sin(parresult[1])-B*sin(parresult[0])*cos(parresult[1])+
			  C*cos(parresult[0])*cos(parresult[1]) ) +
      resZon_X[0] * (B*cos(parresult[0])+C*sin(parresult[0]) );
    den = a*sin(parresult[1])*cos(parresult[0]) + b*sin(parresult[1])*sin(parresult[0]) + c*cos(parresult[1]);
    
    psi =  atan2(num/den,num2/den);
    cout << "Psi "<< psi << endl;
    
    const double rad2deg = 57.29577951308;
    correction.setEulerAngles(parresult[0]*rad2deg,
			      parresult[1]*rad2deg,psi*rad2deg);
    
  //A partir de la correccion tenemos la nueva matriz
    newRotation = fRotMat[0] * correction;
    cout << "The new Rotation is"<< endl;
    newRotation.print();
    
    fRotMat[0] =newRotation;
    
    HGeomVector euler = findEulerAngles(newRotation);   
    
    for(Int_t i=0;i<fNumMods-1;i++){
      fEuler[i].setX(euler.getX());
      fEuler[i].setY(euler.getY());
      fEuler[i].setZ(euler.getZ());
    }
    //}
  
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
  Float_t* errors;
  errors = new Float_t[8];
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
  
  if(numMods>3){
    rotmat[2].setEulerAngles(par[12]*180./TMath::Pi(),
			     par[13]*180./TMath::Pi(),
			     par[14]*180./TMath::Pi()); 
    transla[2].setX(par[15]); 
    transla[2].setY(par[16]); 
    transla[2].setZ(par[17]);
  }
  
  //  if (A_DEBUG>2){
    cout << "HMdcAligner::fcnalign()   Parameters: " 
	 << par[0] << "," << par[1] << "," <<  par[2] << "," 
	 << par[3] << ","  << par[4] << ","  << par[5] << ","
	 << par[6] << "," << par[7] << "," <<  par[8] << "," 
	 << par[9] << ","  << par[10] << ","  << par[11] ;
    if(numMods>3) 
      cout << par[12] << "," << par[13] << "," <<  par[14] << "," 
	   << par[15] << ","  << par[16] << ","  << par[17] ;
    cout << endl;
    // }
  
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
    if(UseUnitErrors==kFALSE && UseModErrors==kFALSE ){
      errorx[0] = (hitA->getErrX()<0.01)? hitA->getErrX() : 0.2;
      errorx[1] = (hitB->getErrX()<0.01)? hitB->getErrX() : 0.2;      
      errorx[2] = (hitC->getErrX()<0.01)? hitC->getErrX() : 0.2;   
      if(numMods>3) errorx[3] = (hitD->getErrX()<0.01)? hitD->getErrX():0.2;
      errory[0] = (hitA->getErrY()<0.01)? hitA->getErrY() : 0.1;
      errory[1] = (hitB->getErrY()<0.01)? hitB->getErrY() : 0.1;      
      errory[2] = (hitC->getErrY()<0.01)? hitC->getErrY() : 0.1;   
      if(numMods>3) errory[3] = (hitD->getErrY()<0.01)? hitD->getErrY():0.1;
    }
    else if(UseModErrors==kTRUE){
      errorx[0]=errors[0];errory[0]=errors[1];    
      errorx[1]=errors[2];errory[1]=errors[3];    
      errorx[2]=errors[4];errory[2]=errors[5];    
      if(numMods>3){errorx[3]=errors[6];errory[3]=errors[7];}  
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
    
    //Converting all hits in each MDC to a common reference system
    //The common system is that of the last module in the constructor
    //(which is the coordinate system of the farther MDC from target
    //provided you use the usual order in the constructors)

    if(numMods>3){
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
    
    //1- Functional minimizing the angle between the straight lines
    //constructed from the hits of three modules

    if(strategy == 105){
      //minimizing sin^2(angle)
      //use in the constructor the middle module (that physically 
      //between the others two) in the third position

      //Float_t part1,part2,part3,part4,part5,sincua,numerator,denominator;

      //      for(Int_t i=0;i<numMods;i++){
      //	cout << "transf[" << i <<"]   ";
      //	transf[i].print();
      //      }
      //following notation in notes: A is transf[0], B is is transf[2],
      //                             C is transf[1]      
      //
      //part1 = (transf[0].getY()-transf[2].getY())*transf[1].getZ() -
      //	(transf[1].getY()-transf[2].getY())*transf[0].getZ();
      //part2 = (transf[1].getX()-transf[2].getX())*transf[0].getZ() -
      //(transf[0].getX()-transf[2].getX())*transf[1].getZ();
      //part3 = (transf[1].getY()-transf[2].getY())*
      //(transf[0].getX()-transf[2].getX()) -
      //(transf[0].getY()-transf[2].getY())*
      //(transf[1].getX()-transf[2].getX());
      //
      //part4=(transf[0].getX()-transf[2].getX())*
      //(transf[0].getX()-transf[2].getX())+
      //(transf[0].getY()-transf[2].getY())*
      //(transf[0].getY()-transf[2].getY())+
      //(transf[0].getZ())*(transf[0].getZ());
      //part5=(transf[1].getX()-transf[2].getX())*
      //(transf[1].getX()-transf[2].getX())+
      //(transf[1].getY()-transf[2].getY())*
      //(transf[1].getY()-transf[2].getY())+
      //(transf[1].getZ())*(transf[1].getZ());
      //
      //numerator = part1*part1 + part2*part2 + part3*part3;
      //denominator = part4*part5;
      //
      //sincua = numerator/denominator;

      //cout << "  sincua " << sincua << endl;

      //New notation: modules A,C,B, defined the coordinate system in C
      //vector made from Hit in modules A and C: (vecA_x, vecA_y, vecA_z)
      //vector made from Hit in modules C and B: (vecB_x, vecB_y, vecB_z)
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
      
      /*
      cout << "mod2VecA=" << mod2VecA << "  mod2VecB="<< mod2VecB
	   << "   2/...=" << (2/(mod2VecA*mod2VecA*mod2VecB*mod2VecB)) << endl
	   << "vecA.getX()="<< vecA.getX()
	   << "  vecA.getY()="<< vecA.getY() 
	   << "  vecA.getZ()="<< vecA.getZ()<<endl	   
	   << "vecB.getX()="<< vecB.getX()
	   << "  vecB.getY()="<< vecB.getY() 
	   << "  vecB.getZ()="<< vecB.getZ()<<endl
	   << "vecAxVecB.getX()="<< vecAxVecB.getX()
	   << "  vecAxVecB.getY()="<< vecAxVecB.getY() 	   
	   << "  vecAxVecB.getZ()="<< vecAxVecB.getZ()<<endl
	   << "rotmat[1](0)=" << rotmat[1](0)
	   << "rotmat[1](3)=" << rotmat[1](3)
	   << "rotmat[1](6)=" << rotmat[1](6) <<endl
	   << "term1=" << vecAxVecB.getX()*(rotmat[1](3)*vecB.getZ()-
					    rotmat[1](6)*vecB.getY()) 
	   << "  term2=" << vecAxVecB.getY()*(rotmat[1](6)*vecB.getX()-
					    rotmat[1](0)*vecB.getZ()) 
	   << "  term3=" << vecAxVecB.getZ()*(rotmat[1](0)*vecB.getY()-
					    rotmat[1](3)*vecB.getX())<<endl
	   << "mod2VecA*mod2VecB=" << mod2VecA*mod2VecB
	   << "  lastterm="<< (rotmat[1](0)*vecA.getX() + 
			       rotmat[1](3)*vecA.getY() + 
			       rotmat[1](6)*vecA.getZ() ) <<endl
	   << "mod2VecAxVecB="<< mod2VecAxVecB << endl;
      */
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
      /*
      cout << "partialx_a=" <<partialx_a<< "  partialy_a=" <<partialy_a<<endl
	   << "partialx_b=" <<partialx_b<< "  partialy_b=" <<partialy_b<<endl
	   << "partialx_c=" <<partialx_c<< "  partialy_c=" <<partialy_c<<endl;
      
      cout << "errorx[0]="<<errorx[0]<< "  errory[0]="<<errory[0]<<endl 
	   << "errorx[1]="<<errorx[1]<< "  errory[1]="<<errory[1]<<endl 
	   << "errorx[2]="<<errorx[2]<< "  errory[2]="<<errory[2]<<endl; 

      cout << "sincua2="<<sincua*sincua 
	   << "  varianzainsincua="<<varianzainsincua<<endl;
      */
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
      
      if(A_DEBUG>4) 
	cout << "Xwt=" << Xwt << " Xss=" << Xss 
	     << " Xsx=" <<  Xsx << " Xsy=" << Xsy
	     << "Ywt=" << Ywt << " Yss=" << Yss 
	     << " Ysx=" <<  Ysx << " Ysy=" << Ysy << endl;
    }
    
    Xsxoss = Xsx/Xss;    
    Ysxoss = Ysx/Yss;
    
    if(A_DEBUG>4) 
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
      
      if(A_DEBUG>4) 
	cout << "Xt=" << Xt << " Xst2=" << Xst2 
	     << " bx (partial)=" << bx << "Yt=" << Yt 
	     << " Yst2=" << Yst2 
	     << " by (partial)=" << by << endl; 
    } 
    
    bx /= Xst2;
    ax = (Xsy-(Xsx*bx))/Xss;
    by /= Yst2;
    ay = (Ysy-(Ysx*by))/Yss;
     
    if(A_DEBUG>4)  
      cout << "bx=" << bx << " ax=" << ax
	   << "by=" << by << " ay=" << ay << endl;
    
    sigax = sqrt((1.0+Xsx*Xsx/(Xss*Xst2))/Xss);
    sigbx = sqrt(1.0/Xst2);
    sigay = sqrt((1.0+Ysx*Ysx/(Yss*Yst2))/Yss);
    sigby = sqrt(1.0/Yst2);
    
    if(A_DEBUG>4) 
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
  } 
  f = chisq; 
  //if(A_DEBUG>2){
    cout  << "chisqr= " << chisq << " out of " 
	  << entries << " combinations."<< endl; 
    //}
} 




void fcnRot(Int_t &npar, Double_t *gin, Double_t &f, 
	    Double_t *par, Int_t iflag){ 
  
  //
  // functional to minimize to obtain rotations 
  //
  Double_t chisq = 0.; 
  Double_t phi,theta; 
  phi = par[0]; 
  theta = par[1]; 
  Float_t *xpzon=new Float_t[9];
  Float_t *ypzon=new Float_t[9];
  Float_t *xcen=new Float_t[9];
  Float_t *ycen=new Float_t[9];
  HGeomVector tran;
  HGeomRotation rot;
  HGeomVector Target;

  
  HMdcAligner* pAlign = (HMdcAligner*)(gMinuit->GetObjectFit());
  pAlign = (HMdcAligner*)(gMinuit->GetObjectFit());
  xcen = pAlign->getZonesX();
  ycen = pAlign->getZonesY();
  xpzon = pAlign->getResZonX();
  ypzon = pAlign->getResZonY();
  rot = pAlign->getRotMatrix(0);
  tran = pAlign->getTranslation(0);
  Target = pAlign->getLocalTarget(rot,tran);
  
  Float_t A=0,B=0,C=0;
  Float_t a=0,b=0,c=0;    
  
  Float_t xTar = Target.getX();
  Float_t yTar = Target.getY();
  Float_t zTar = Target.getZ();
  Float_t term1,term2,term3,term4,total;  
  
  if(A_DEBUG>4) {
    cout << endl <<"######## New call to fcnZ() #######" << endl;
    cout << "phi=" << phi << "  theta=" << theta;
    cout << "Target in:" << xTar << ", " << yTar << ", " << zTar<< endl;
  }
  
  for (Int_t i=0; i<9;i++) {
    
    if(A_DEBUG>4)
      cout << "## Loop " << i << " ##" << endl;
    
    a = xpzon[i]-xTar; 
    b = ypzon[i]-yTar;  
    c =         -zTar; 
    A = b*xpzon[i]-a*ypzon[i];
    B = c*xpzon[i]           ;
    C = c*ypzon[i]           ;

    if(A_DEBUG>4) {
      cout << "a = " << a << "  b = " << b << "  c = " << c <<endl;
      cout << "A = " << A << "  B = " << B << "  C = " << C <<endl;
      cout << "xcen[i] = " << xcen[i] <<"  ycen[i] = " << ycen[i] <<endl;
    }

    term1 = -A*sin(theta)-B*sin(phi)*cos(theta)+C*cos(phi)*cos(theta);
    term2 = B*cos(phi) + C*sin(phi);
    term3 = xcen[i]*xcen[i] + ycen[i]*ycen[i];
    term4 = a*sin(theta)*cos(phi)+b*sin(theta)*sin(phi)+c*cos(theta);
    total = term1*term1 + term2*term2 - term3*term4*term4;


    if(A_DEBUG>4) {    
      cout << "term1 = " << term1 << "  term2 = " << term2 << "  term3 = " 
	   << term3 << "  term4 = " << term4 << endl;
      cout << "total = " << total << endl;
    }

    chisq += total*total; 

    if(A_DEBUG>4) 
            cout << "chisq = " << chisq  << endl;

    //chisq es igual a chisq2! COMPROBADO!
    //chisq2 +=  A*A*sin(theta)*sin(theta) + 
    //  cos(theta)*cos(theta)*(B*sin(phi)-C*cos(phi))*(B*sin(phi)-C*cos(phi)) +
    //  2*A*sin(theta)*cos(theta)*(B*sin(phi)-C*cos(phi)) +
    //  (B*cos(phi)+C*sin(phi))*(B*cos(phi)+C*sin(phi)) -
    //  (xcen[i]*xcen[i]+ycen[i]*ycen[i])*
    //  (a*sin(theta)*cos(phi)+b*sin(theta)*sin(phi)+c*cos(theta))*
    //  (a*sin(theta)*cos(phi)+b*sin(theta)*sin(phi)+c*cos(theta));
    //cout << "chisq = " << chisq << "chisq2 = " << chisq2 << endl;
  } 
  f = chisq; 

  if(A_DEBUG>4) 
     cout << "     funcional = "<< f << endl;
} 



/*
void fcnRotDist(Int_t &npar, Double_t *gin, Double_t &f, 
		Double_t *par, Int_t iflag){ 
  
  //
  // functional to minimize to obtain rotations 
  //
  Double_t chisq = 0.;
  Float_t *xpzon=new Float_t[9];
  Float_t *ypzon=new Float_t[9];
  Float_t *xcen=new Float_t[9];
  Float_t *ycen=new Float_t[9];
  HGeomRotation origRot,newRot,correct;  
  HGeomVector origTrans, newTrans;
  Int_t flag;

  HMdcAligner* pAlign = (HMdcAligner*)(gMinuit->GetObjectFit());
  flag = pAlign->getMinFlag();
  origRot = pAlign->getRotMatrix(0);
  origTrans = pAlign->getTranslation(0);
  
  if(flag==0) {
    correct.setElement(cos(par[0]),4);
    correct.setElement(sin(par[0]),5);
    correct.setElement(-sin(par[0]),7);
    correct.setElement(cos(par[0]),8);
  }
  else{
    correct.setElement(cos(par[0]),0);
    correct.setElement(sin(par[0]),2);
    correct.setElement(-sin(par[0]),6);
    correct.setElement(cos(par[0]),8);
  }  

  newRot = origRot * correct;
  
  //if(A_DEBUG>5){
  cout << "%%%%%%%%%%%%%%% fcnRotDist %%%%%%%%%%%%%%"<<endl;
  cout << "par[0] = " << par[0] <<  "  correct and new matrix are" << endl;
  correct.print();
  newRot.print();
  //}

  // pAlign->calculo(newRot,origTrans,newTrans);

  pAlign->divideInZones(-1,newRot,origTrans);
  
  //newTrans = pAlign->translationFinderZones(newRot,origTrans);
  
  //pAlign->divideInZones(-1,newRot,newTrans);

  xcen = pAlign->getZonesX();
  ycen = pAlign->getZonesY();
  xpzon = pAlign->getResZonX();
  ypzon = pAlign->getResZonY();  


  for (Int_t zo = 0;zo<9;zo++){
    if(flag==0) chisq += (ycen[zo] - ypzon[zo])*(ycen[zo] - ypzon[zo])   +par[0];
    else chisq += (xcen[zo] - xpzon[zo])*(xcen[zo] - xpzon[zo])   -par[0];
    //if(A_DEBUG>5){
    cout << "xcen[zo]=" <<  xcen[zo]  << " ycen[zo]=" << ycen[zo] <<  
      "xpzon[zo]=" <<  xpzon[zo]  << " ypzon[zo]=" << ypzon[zo]; 
    cout <<  "   chisq=" << chisq << endl;
    //}
  }
  
  //  if(A_DEBUG>4) 
  cout << "Final chisq = " << chisq  << endl;
  
  f = chisq; 
  
  if(A_DEBUG>4) 
    cout << "     funcional = "<< f << endl;
} 
*/



