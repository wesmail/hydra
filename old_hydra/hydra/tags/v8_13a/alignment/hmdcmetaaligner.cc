# include <math.h>
# include <stdlib.h>
# include <iomanip> 
# include <fstream> 
# include <TMath.h>
# include <TROOT.h>
# include <TF1.h>
# include "hmdcmetaaligner.h"
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
# include "../tof/tofdef.h"
# include "../shower/showerdef.h"

ClassImp(HMdcMetaAligner)

  //*-- AUTHOR : Hector Alvarez-Pol  
//*-- Date: 08/2001
//*-- Last Update: 25/08/01
//*-- Copyright: GENP (Univ. Santiago de Compostela)
  
//_HADES_CLASS_DESCRIPTION 
  ////////////////////////////////////////////////////////////////////////
  //
  // HMdcMetaAligner   (ongoing work)
  //
  // Performs the TOF/SHOWER software alignment (first part).
  //
  // First, a set of Hits from MDCs is used to follow the tracks
  // up to the SHOWER/TOF. The Hit selection repeates the scheme
  // from hmdcalignernew.cc. Then the differences in X, Y 
  // gives information about the position
  //
  ////////////////////////////////////////////////////////////////////////

  Int_t HMdcMetaAligner::fRecCount=0;
TFile* HMdcMetaAligner::fOutRoot=0;
Int_t MA_DEBUG=0;

HMdcMetaAligner::HMdcMetaAligner(void) : HReconstructor()
{  
  //
  // Default constructor. 
  //

  fLoc.setNIndex(5);
  fHits = new HMdcMetaHit(4);
  fLoc.set(5,0,0,1,2,3); //dummy sector 0
  fNumMods = 4;
  initDefaults();
}
 




HMdcMetaAligner::HMdcMetaAligner(Int_t sector, Int_t modA, Int_t modB) 
  : HReconstructor()
{  
  //
  // Constructor including module election. Alignment procedure 
  // proyects hits of modA and modB on META coordinate system and compares
  //
  
  fLoc.setNIndex(3);  
  fHits = new HMdcMetaHit(2); 
  fLoc.set(3,sector,modA,modB);
  fNumMods = 2;
  initDefaults();
}





HMdcMetaAligner::HMdcMetaAligner(Text_t* name, Text_t* title, 
				 Int_t sector, Int_t modA, Int_t modB,
				 Int_t modC, Int_t modD)
  : HReconstructor(name, title)
{  
  //
  // Constructor including module election (and name and title, which 
  // seems to be very important). Alignment procedure 
  // proyects hits of modA and modB on META coordinate system and compares
  //

  if(modC == -1) {  
    fHits = new HMdcMetaHit(2);
    fLoc.setNIndex(3);
    fLoc.set(3,sector,modA,modB); 
    fNumMods = 2;
  } 
  else if(modD == -1){ 
    fHits = new HMdcMetaHit(3);
    fLoc.setNIndex(4);
    fLoc.set(4,sector,modA,modB,modC); 
    fNumMods = 3;
  }
  else {        
    fHits = new HMdcMetaHit(4);
    fLoc.setNIndex(5);
    fLoc.set(5,sector,modA,modB,modC,modD); 
    fNumMods = 4;
  }
  initDefaults();
}





HMdcMetaAligner::HMdcMetaAligner(Text_t* name, Text_t* title, 
				 Int_t sector, Int_t modA)
  : HReconstructor(name, title)
{  
  //
  // Constructor for only one MDC
  //
  
  fHits=new HMdcMetaHit(1);   
  fLoc.setNIndex(3);
  fLoc.set(3,sector,modA,0);  //the last one is a dummy
  fNumMods = 1; 
  initDefaults();
}





void HMdcMetaAligner::initDefaults(void)
{
  //
  // Inits common defaults
  //

  if(fNumMods>1){
    fRotMat = new HGeomRotation[fNumMods-1];
    fTranslation = new HGeomVector[fNumMods-1];
    fEuler = new HGeomVector[fNumMods-1];  
  } 
  fMRotMat = new HGeomRotation[9];
  fMTranslation = new HGeomVector[9];
  fMEuler = new HGeomVector[9];
  fMRotTofRel = new HGeomRotation[8]; //Rel params for TOF Unique CS
  fMTransTofRel = new HGeomVector[8]; //Rel params for TOF Unique CS
  if(fNumMods>1) fDiscart = new Int_t[fNumMods-1];
  fError = new Double_t[6];
  fSDiscart = 0;
  fTDiscart = new Int_t[8];
  if(fNumMods>1){
    fHitsMdc = new Int_t[fNumMods];
    fHitsFoundInWindow = new Int_t[fNumMods-1];
    fHitsFoundAndUnique = new Int_t[fNumMods-1];
  }
  else  fHitsMdc = new Int_t[1];
  fHitsFoundInTofWindow = new Int_t[8];
  fHitsFoundInTofAndUnique = new Int_t[8];
  if(fNumMods>1){    
    for(Int_t i=0;i<fNumMods-1;i++){
      fHitsFoundInWindow[i] = 0;
      fHitsFoundAndUnique[i] = 0;
      fDiscart[i] = 0;
      fHitsMdc[i] = 0;
      fHitsMdc[i+1]=0;
    }
  } 
  for(Int_t i=0;i<fNumMods;i++) fHitsMdc[i]=0; 
  for(Int_t i=0;i<8;i++){
    fHitsFoundInTofWindow[i] = 0;
    fHitsFoundInTofAndUnique[i] = 0;
    fTDiscart[i] = 0;
  }
  fHitsFoundInShowerWindow =0;
  fHitsFoundInShowerAndUnique = 0;
  fNEntries = 0; 
  fCount = 0;  
  fTCount = 0;
  fSCount = 0;
  fCountCut = 0;
  fMin = 0;
  fHitCat = NULL; 
  fShowerHitCat = NULL;
  fTofHitCat = NULL;
  fAuto =kFALSE;    
  fAutoMeta =kFALSE; 
  fHistoOff =kFALSE;  
  fUseUnitErrors =kFALSE;  
  fXArea = 100;  
  fYArea = 100;
  fXSigmas = 1.64;    //90% of gauss distribution
  fYSigmas = 1.64;  
  fS0Sigmas = 1.64;  
  fS1Sigmas = 1.64;
  fUseCut = kTRUE;  
  fUseSlopeCut = kFALSE;  
  fUseTarget = kFALSE;
  fUseUniqueRSInTof = kFALSE;
  fUniqueTofDet = 4;   //Reference Tof module 


  fSlopeCut = 0.1;
  fOutRoot = NULL; 
  fFix = 0;
  fHistNum = 2;       //allows the fast definition of new sets of histo
  fMetaHistNum = 4;   //allows the fast definition of new sets of histo
  initMinimization();
}





HMdcMetaAligner::~HMdcMetaAligner(void)
{    
  //
  // Destructor.
  //
  if(fRotMat) delete[] fRotMat;
  if(fTranslation) delete[] fTranslation;
  if(fEuler) delete[] fEuler; 
  if(fError) delete[] fError;
  if(fHitsMdc) delete[] fHitsMdc;
  if(fHitsFoundInWindow) delete[] fHitsFoundInWindow;
  if(fHitsFoundAndUnique) delete[] fHitsFoundAndUnique;
  if(fHits) delete fHits;
}





void HMdcMetaAligner::initMinimization(void){
  //
  // Minimization defaults 
  //

  //CAMBIOS AQUI POR MAS DE DOS MODULOS
  fIterCriteria = 0.1;   
  fWeights = new Float_t[4];
  fSteps = new Float_t[6];
  fLimits = new Float_t[6];
  setWeights(400,160,0.004,0.003);
  setSteps(0.01,0.01,0.01,0.1,0.1,0.1);
  setLimits(0.,0.,0.,0.,0.,0.);
}





void HMdcMetaAligner::setOutputAscii(TString name)
{
  //
  // Sets ascii output for debugging.
  //
  fNameAscii=name;
}





void HMdcMetaAligner::setOutputRoot(TString name)
{
  //
  // Sets ascii output for debugging.
  //
  fNameRoot=name;
}





void HMdcMetaAligner::setSigmas(Float_t XSigmas, Float_t YSigmas, 
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





void HMdcMetaAligner::setHistograms(void)
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

  fOutRoot->cd();

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
  if(fNumMods>1) {
    AvsBinBCS_X = new TH1F*[fHistNum];  
    AvsBinBCS_Y = new TH1F*[fHistNum];
    AvsBinBCS_XSlope = new TH1F*[fHistNum];  
    AvsBinBCS_YSlope = new TH1F*[fHistNum];  
    BvsAinACS_X = new TH1F*[fHistNum];  
    BvsAinACS_Y = new TH1F*[fHistNum];
    BvsAinACS_XSlope = new TH1F*[fHistNum];  
    BvsAinACS_YSlope = new TH1F*[fHistNum];
  }
  fResShowerX = new TH1F*[fMetaHistNum]; 
  fResShowerY = new TH1F*[fMetaHistNum];    
  if(fUseUniqueRSInTof==kTRUE){
    fUniqueResTofX = new TH1F*[fMetaHistNum];    
    fUniqueResTofY = new TH1F*[fMetaHistNum];
  }
  fResTofX = new TH1F*[fMetaHistNum*8]; //0,...,7 for fMetaHistNum==0
  fResTofY = new TH1F*[fMetaHistNum*8]; //8,...,15 for fMetaHistNum==1 ...
  fAllResTofX = new TH1F*[8];    
  fAllResTofY = new TH1F*[8];

  if (fNumMods>3){    
    Int_t modC = fLoc[3];
    Int_t modD = fLoc[4];
    sprintf(title,"%s%i%s%i%s%i%s%i%s%i","Sector_",sector,"_ModA_",modA,
	    "_ModB_",modB,"_ModC_",modC,"_ModD_",modD);
    sprintf(tmp,"%s%s%i%s%i%s%i%s%i%s%i","Meta","_Sector_",sector,
	    "_ModA_",modA,"_ModB_",modB,"_ModC_",modC,"_ModD_",modD);
    fAlignMeta = new TTree(tmp,title);
    fAlignMeta->Branch("hits","HMdcMetaHit",&fHits,64000);

    sprintf(tmp,"%s%s%i%s%i%s%i%s%i%s%i","MetaCut","_Sector_",sector,
	    "_ModA_",modA,"_ModB_",modB,"_ModC_",modC,"_ModD_",modD);
    fAlignMetaCut = new TTree(tmp,title);
    fAlignMetaCut->Branch("hits","HMdcMetaHit",&fHits,64000);
    
    static Char_t newDirName[255];
    sprintf(newDirName,"%s%s%i%s%i%s%i%s%i%s%i","MetaAligner_","S_",sector,
	    "_ModA_",modA,"_ModB_",modB,"_ModC_",modC,"_ModD_",modD);
    fOutRoot->mkdir(newDirName,newDirName);
    fOutRoot->cd(newDirName);
    
    //binning
    Int_t bin=200, binS=200, binChi=200, binDist=200;
    Int_t min=-100,max=100,minS=-1,maxS=1;
    Int_t minChi=0, maxChi=10, minDist=0,maxDist=100;
    
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
    sprintf(tmp,"%s%s%i%s%i%s%i%s%i","Meta","_Sector_",sector,
	    "_ModA_",modA,"_ModB_",modB,"_ModC_",modC);   
    fAlignMeta = new TTree(tmp,title);
    fAlignMeta->Branch("hits","HMdcMetaHit",&fHits,64000);

    sprintf(tmp,"%s%s%i%s%i%s%i%s%i","MetaCut","_Sector_",sector,
	    "_ModA_",modA,"_ModB_",modB,"_ModC_",modC);   
    fAlignMetaCut = new TTree(tmp,title);
    fAlignMetaCut->Branch("hits","HMdcMetaHit",&fHits,64000);

    static Char_t newDirName[255];
    sprintf(newDirName,"%s%s%i%s%i%s%i%s%i","MetaAligner_","S_",sector,
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
  else if(fNumMods>1){    
    sprintf(title,"%s%i%s%i%s%i","Sector_",sector,
	    "_ModA_",modA,"_ModB_",modB);
    sprintf(tmp,"%s%s%i%s%i%s%i","Meta","_Sector_",sector,
	    "_ModA_",modA,"_ModB_",modB);
    fAlignMeta = new TTree(tmp,title);
    fAlignMeta->Branch("hits","HMdcMetaHit",&fHits,64000);

    sprintf(tmp,"%s%s%i%s%i%s%i","MetaCut","_Sector_",sector,
	    "_ModA_",modA,"_ModB_",modB);
    fAlignMetaCut = new TTree(tmp,title);
    fAlignMetaCut->Branch("hits","HMdcMetaHit",&fHits,64000);

    static Char_t newDirName[255];
    sprintf(newDirName,"%s%s%i%s%i%s%i","MetaAligner_","S_",sector,
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
  else{   //1MDC
    sprintf(title,"%s%i%s%i","Sector_",sector,"_ModA_",modA);
    sprintf(tmp,"%s%s%i%s%i","Meta","_Sector_",sector,"_ModA_",modA);    
    fAlignMeta = new TTree(tmp,title);
    fAlignMeta->Branch("hits","HMdcMetaHit",&fHits,64000); 

    sprintf(tmp,"%s%s%i%s%i","MetaCut","_Sector_",sector,
	    "_ModA_",modA);   
    fAlignMetaCut = new TTree(tmp,title);
    fAlignMetaCut->Branch("hits","HMdcMetaHit",&fHits,64000);

    static Char_t newDirName[255];
    sprintf(newDirName,"%s%s%i%s%i","MetaAligner_","S_",sector,
	    "_ModA_",modA);
    fOutRoot->mkdir(newDirName,newDirName);
    fOutRoot->cd(newDirName);
  }

  //Histogramas comunes
  if(fNumMods>1){
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
  }
       
  //binning
  Int_t Sbin=250,Tbin=250;
  Int_t Smin=-125,Smax=125,Tmin=-125,Tmax=125;
 
  sprintf(tmp,"%s%s%i","ShowerPlaneProj","_Sector_",sector);
  ShowerPlaneProj = new TH2F(tmp,title,500,-1000,1000,500,-800,1200);
  
  for(Int_t index=0;index<fMetaHistNum;index++){
    sprintf(tmp,"%s%i%s%i","fResShowerX_",index,"_Sector_",sector);
    fResShowerX[index] = new TH1F(tmp,title,Sbin,Smin,Smax);
    sprintf(tmp,"%s%i%s%i","fResShowerY_",index,"_Sector_",sector);
    fResShowerY[index] = new TH1F(tmp,title,Sbin,Smin,Smax);

    if(fUseUniqueRSInTof==kTRUE){
      sprintf(tmp,"%s%i%s%i","fUniqueResTofX_",index,"_Sector_",sector);
      fUniqueResTofX[index] = new TH1F(tmp,title,Tbin,Tmin,Tmax);
      sprintf(tmp,"%s%i%s%i","fUniqueResTofY_",index,"_Sector_",sector);
      fUniqueResTofY[index] = new TH1F(tmp,title,Tbin,Tmin,Tmax);
    }
    
    for (Int_t det=0;det<8;det++){//0,...,7 for fMetaHistNum==0
      //8,...,15 for fMetaHistNum==1 ...
      sprintf(tmp,"%s%i%s%i%s%i","fResTof",det,"X_",index,"_Sector_",sector);
      fResTofX[(8*index)+det] = new TH1F(tmp,title,Tbin,Tmin,Tmax);
      sprintf(tmp,"%s%i%s%i%s%i","fResTof",det,"Y_",index,"_Sector_",sector);
      fResTofY[(8*index)+det] = new TH1F(tmp,title,Tbin,Tmin,Tmax);
    }
  }

  //binning
  Int_t SbinA=200,TbinA=200;
  Int_t SminA=-1000,SmaxA=1000,TminA=-1000,TmaxA=1000;
 
  sprintf(tmp,"%s%s%i","fAllResShowerX","_Sector_",sector);
  fAllResShowerX = new TH1F(tmp,title,SbinA,SminA,SmaxA);
  sprintf(tmp,"%s%s%i","fAllResShowerY","_Sector_",sector);
  fAllResShowerY = new TH1F(tmp,title,SbinA,SminA,SmaxA);
 
  if(fUseUniqueRSInTof==kTRUE){    
    sprintf(tmp,"%s%s%i","fUniqueAllResTofX","_Sector_",sector);
    fUniqueAllResTofX = new TH1F(tmp,title,TbinA,TminA,TmaxA);
    sprintf(tmp,"%s%s%i","fUniqueAllResTofY","_Sector_",sector);
    fUniqueAllResTofY = new TH1F(tmp,title,TbinA,TminA,TmaxA);
  } 
  
  for (Int_t det=0;det<8;det++){
    sprintf(tmp,"%s%i%s%i","fAllResTof",det,"X_Sector_",sector);
    fAllResTofX[det] = new TH1F(tmp,title,Tbin,Tmin,Tmax);
    sprintf(tmp,"%s%i%s%i","fAllResTof",det,"Y_Sector_",sector);
    fAllResTofY[det] = new TH1F(tmp,title,Tbin,Tmin,Tmax);
  }

  fOutRoot->cd();
}




void HMdcMetaAligner::fitMetaHistograms(Int_t index)
{
  //
  //Fits to a gaussian the two relevant histograms 
  //(residuals in X and Y for Meta Histograms)
  //and obtains the fit parameters for further data selection
  //
  
  //Till now, only working for one MDCs
  //Antes de hacerlo para mas, pensar que es necesario cortar!
  
  if(fNumMods==1){
    Float_t XNewArea[9], YNewArea[9];
    Float_t XNewMean[9], YNewMean[9];
    
    TF1 *f1X = new TF1("f1X","gaus",-fXArea,fXArea);
    TF1 *f1Y = new TF1("f1Y","gaus",-fYArea,fYArea);
    TF1 *totalX  = new TF1("totalX","gaus(0)+pol2(3)",-fXArea,fXArea);
    TF1 *totalY  = new TF1("totalY","gaus(0)+pol2(3)",-fYArea,fYArea);
    Double_t par[6]; 
    
    
    if(MA_DEBUG>1) cout << endl 
			<<"**** fitMetaHistograms() results ****" << endl;   
    if(MA_DEBUG>1) cout << endl 
			<<"**** Gauss fit: mean, sigma ****" << endl 
			<<"**** Gauss+pol: mean, sigma ****" 
			<< endl;   
    
    
    fResShowerX[index]->Fit("f1X","RQNW");
    Float_t fitPar0     = f1X->GetParameter(0);  // constant
    Float_t fitPar1     = f1X->GetParameter(1);  // mean
    Float_t fitPar2     = f1X->GetParameter(2);  // sigma
    if(MA_DEBUG>1) cout << " fResShowerX[" << index << "]: "
			<< fitPar1 << ", " << fitPar2<< ", " ;
    f1X->GetParameters(&par[0]);
    par[3] =  par[4] = par[5] = 0.; 
    totalX->SetParameters(par);
    fResShowerX[index]->Fit("totalX","RQNW"); 
    fitPar0     = totalX->GetParameter(0); 
    fitPar1     = totalX->GetParameter(1);
    fitPar2     = totalX->GetParameter(2);
    if(MA_DEBUG>1) cout << fitPar1 << ", " << fitPar2 << endl;
    XNewArea[8] = fXSigmas * fitPar2;
    XNewMean[8] = fitPar1;
    
    fResShowerY[index]->Fit("f1Y","RQNW");
    fitPar0     = f1Y->GetParameter(0);  // constant
    fitPar1     = f1Y->GetParameter(1);  // mean
    fitPar2     = f1Y->GetParameter(2);  // sigma    
    if(MA_DEBUG>1) cout << " fResShowerY[" << index << "]: "
			<< fitPar1 << ", " << fitPar2<< ", " ;
    f1Y->GetParameters(&par[0]);
    totalY->SetParameters(par);     
    fResShowerY[index]->Fit("totalY","RQNW");
    fitPar1     = totalY->GetParameter(1);
    fitPar2     = totalY->GetParameter(2);
    if(MA_DEBUG>1) cout << fitPar1 << ", " << fitPar2 << endl;
    YNewArea[8] = fYSigmas * fitPar2;
    YNewMean[8] = fitPar1;
    
    if(fUseUniqueRSInTof==kTRUE){    
      fUniqueResTofX[index]->Fit("f1X","RQNW");
      Float_t fitPar0     = f1X->GetParameter(0);  // constant
      Float_t fitPar1     = f1X->GetParameter(1);  // mean
      Float_t fitPar2     = f1X->GetParameter(2);  // sigma
      if(MA_DEBUG>1) cout << " fUniqueResTofX[" << index << "]: "
			  << fitPar1 << ", " << fitPar2<< ", " ;
      f1X->GetParameters(&par[0]);
      par[3] =  par[4] = par[5] = 0.; 
      totalX->SetParameters(par);
      fUniqueResTofX[index]->Fit("totalX","RQNW"); 
      fitPar0     = totalX->GetParameter(0); 
      fitPar1     = totalX->GetParameter(1);
      fitPar2     = totalX->GetParameter(2);
      if(MA_DEBUG>1) cout << fitPar1 << ", " << fitPar2 << endl;
      XNewArea[0] = fXSigmas * fitPar2;
      XNewMean[0] = fitPar1;
      
      fUniqueResTofY[index]->Fit("f1Y","RQNW");
      fitPar0     = f1Y->GetParameter(0);  // constant
      fitPar1     = f1Y->GetParameter(1);  // mean
      fitPar2     = f1Y->GetParameter(2);  // sigma    
      if(MA_DEBUG>1) cout << " fUniqueResTofY[" << index << "]: "
			  << fitPar1 << ", " << fitPar2<< ", " ;
      f1Y->GetParameters(&par[0]);
      totalY->SetParameters(par);     
      fUniqueResTofY[index]->Fit("totalY","RQNW");
      fitPar1     = totalY->GetParameter(1);
      fitPar2     = totalY->GetParameter(2);
      if(MA_DEBUG>1) cout << fitPar1 << ", " << fitPar2 << endl;
      YNewArea[0] = fYSigmas * fitPar2;
      YNewMean[0] = fitPar1;
    }
    else{
      //0,...,7 for fMetaHistNum==0
      //8,...,15 for fMetaHistNum==1 ...
      for(Int_t detnum = 0;detnum<8;detnum++){
	fResTofX[(8*index)+detnum]->Fit("f1X","RQNW");
	fitPar0     = f1X->GetParameter(0);  // constant
	fitPar1     = f1X->GetParameter(1);  // mean
	fitPar2     = f1X->GetParameter(2);  // sigma
	if(MA_DEBUG>1) cout << " fResTof0X[" << (8*index)+detnum << "]: "
			    << fitPar1 << ", " << fitPar2<< ", " ;
	f1X->GetParameters(&par[0]);
	par[3] =  par[4] = par[5] = 0.; 
	totalX->SetParameters(par);
	fResTofX[(8*index)+detnum]->Fit("totalX","RQNW"); 
	fitPar0     = totalX->GetParameter(0); 
	fitPar1     = totalX->GetParameter(1);
	fitPar2     = totalX->GetParameter(2);
	if(MA_DEBUG>1) cout << fitPar1 << ", " << fitPar2 << endl;
	XNewArea[detnum] = fXSigmas * fitPar2;
	XNewMean[detnum] = fitPar1;
	
	fResTofY[(8*index)+detnum]->Fit("f1Y","RQNW");
	fitPar0     = f1Y->GetParameter(0);  // constant
	fitPar1     = f1Y->GetParameter(1);  // mean
	fitPar2     = f1Y->GetParameter(2);  // sigma    
	if(MA_DEBUG>1) cout << " fResTof0Y[" << (8*index)+detnum << "]: "
			    << fitPar1 << ", " << fitPar2<< ", " ;
	f1Y->GetParameters(&par[0]);
	totalY->SetParameters(par);     
	fResTofY[(8*index)+detnum]->Fit("totalY","RQNW");
	fitPar1     = totalY->GetParameter(1);
	fitPar2     = totalY->GetParameter(2);
	if(MA_DEBUG>1) cout << fitPar1 << ", " << fitPar2 << endl;
	YNewArea[detnum] = fYSigmas * fitPar2;
	YNewMean[detnum] = fitPar1;
      } 
    }
    Stat_t entries = fAlignMeta->GetEntries();
    
    HMdcHit* hitA; 
    HMdcHit* hitB=0;   
    HGeomVector* lHit;
    HGeomVector projMP;
    Bool_t passed=kFALSE;
    Float_t res_X, res_Y;
    Int_t det=-10;
    
    for (Int_t i=0;i<entries;i++) {     
      fAlignMeta->GetEntry(i);
      hitA = (HMdcHit*) fHits->getMdcHitA();
      if(fNumMods>2) hitB = (HMdcHit*) fHits->getMdcHitB();
      //if(fNumMods>2) hitC = (HMdcHit*) fHits->At(2);
      //if(fNumMods>3) hitD = (HMdcHit*) fHits->At(3);
      lHit = fHits->getLocalHitA();
      //if(lHit->getZ()>8)lHitB = fHits->getLocalHitB();
      //if(fHits->getDetector()>8)lHitB = fHits->getLocalHitB();
      
      //Algo va mal con los Int_t de HMdcMetaHit en el TTree!!
      det =  fHits->getDetector();

      //Int_t det =  lHit->getZ();
      if(MA_DEBUG>0) cout << "detector: " << det << endl;
      
      //Constructing all possible projections
      //The histos represent (value of local hit - value of projected hit)
      //In all cases (Meta part)
      if(fNumMods>1){
	if(det<9)
	  projMP = findProjMetaPoint(hitA,hitB,fRotMat[0],
				     fTranslation[0],
				     fMRotMat[det],
				     fMTranslation[det]);
	else  projMP = findProjMetaPoint(hitA,hitB,fRotMat[0],
					 fTranslation[0],
					 fMRotMat[det-9],
					 fMTranslation[det-9]);
      }
      else {      
	if(det<9)
	  projMP = findProjMetaPoint(hitA,fMRotMat[det],
				     fMTranslation[det]);
	else  projMP = findProjMetaPoint(hitA,fMRotMat[det-9],
					 fMTranslation[det-9]);
      }
      
      res_X = lHit->getX() - projMP.getX();
      res_Y = lHit->getY() - projMP.getY();
      
      passed = kFALSE;
      if(fUseUniqueRSInTof==kTRUE && det<8){
	//Now passing the cut if is the "global" well behaved
	//hits on the histograms fUniqueResTofX, fUniqueResTofY
	if(fabs(res_X - XNewMean[0]) < XNewArea[0] && 
	   fabs(res_Y - YNewMean[0]) < YNewArea[0]) passed = kTRUE;
      }
      else{
	if(fabs(res_X - XNewMean[det]) < XNewArea[det] && 
	   fabs(res_Y - YNewMean[det]) < YNewArea[det]) passed = kTRUE;
      }
      
      //strong condition: cutting in all histograms 
      if(passed==kTRUE){
	if(fUseSlopeCut){
	  // This cut makes the sample near indep. of Z translations
	  // and results useful for X and Y minimization.
	  // The cut is effective only in MDCB, because fTranslation
	  // is represented in MDCB coordinates. Then, tracks passing
	  // the cut are almost parallel to Z direction
	  if(( fabs(hitA->getXDir()) < fSlopeCut) && 
	     ( fabs(hitA->getYDir()) < fSlopeCut)){
	    if(MA_DEBUG>3) cout << " -- CUT PASSED (fSlopeCut=" 
				<< fSlopeCut << " ) --" << endl;
	    
	    //Aqui no hago nada ??? Comprobar que realmente es lo correcto
	    //Creo que fHits contiene todo lo que necesito tal como
	    //viene de fAlignMeta. Solo hay que llenar el Cut si cumple el if
	    fHits->print();
	    fAlignMetaCut->Fill();
	    fCountCut++;
	  }
	}
	else{
	  if(MA_DEBUG>3) cout << " ---------  CUT PASSED ------------" << endl;
	  
	  fAlignMetaCut->Fill();
	  fCountCut++;
	}
      }
    }
  }
}





void HMdcMetaAligner::fitHistograms(Int_t index)
{
  //
  //Fits to a gaussian the four relevant histograms
  //and obtains the fit parameters for further data selection
  //
  
  //Till now, only working for two MDCs
  //Antes de hacerlo para mas, pensar que es necesario cortar!
  
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

    if(MA_DEBUG>1) cout << endl 
			<<"**** fitHistograms() results ****" << endl;   
    if(MA_DEBUG>1) cout << endl 
			<<"**** Gauss fit: mean, sigma ****" << endl 
			<<"**** Gauss+pol: mean, sigma ****" 
			<< endl;   

    AvsBinBCS_X[index]->Fit("f1X","RQNW");
    Float_t fitPar0     = f1X->GetParameter(0);  // constant
    Float_t fitPar1     = f1X->GetParameter(1);  // mean
    Float_t fitPar2     = f1X->GetParameter(2);  // sigma
    if(MA_DEBUG>1) cout << " AvsBinBCS_X[" << index << "]: "
			<< fitPar1 << ", " << fitPar2<< ", " ;
    f1X->GetParameters(&par[0]);
    par[3] =  par[4] = par[5] = 0.; 
    totalX->SetParameters(par);
    AvsBinBCS_X[index]->Fit("totalX","RQNW"); 
    fitPar0     = totalX->GetParameter(0); 
    fitPar1     = totalX->GetParameter(1);
    fitPar2     = totalX->GetParameter(2);
    if(MA_DEBUG>1) cout << fitPar1 << ", " << fitPar2 << endl;
    XNewAreaA = fXSigmas * fitPar2;
    XNewMeanA = fitPar1;

    AvsBinBCS_Y[index]->Fit("f1Y","RQNW");
    fitPar0     = f1Y->GetParameter(0);  // constant
    fitPar1     = f1Y->GetParameter(1);  // mean
    fitPar2     = f1Y->GetParameter(2);  // sigma    
    if(MA_DEBUG>1) cout << " AvsBinBCS_Y[" << index << "]: "
			<< fitPar1 << ", " << fitPar2<< ", " ;
    f1Y->GetParameters(&par[0]);
    totalY->SetParameters(par);     
    AvsBinBCS_Y[index]->Fit("totalY","RQNW");
    fitPar1     = totalY->GetParameter(1);
    fitPar2     = totalY->GetParameter(2);
    if(MA_DEBUG>1) cout << fitPar1 << ", " << fitPar2 << endl;
    YNewAreaA = fYSigmas * fitPar2;
    YNewMeanA = fitPar1;

    AvsBinBCS_XSlope[index]->Fit("f1S","RQNW");
    fitPar0     = f1S->GetParameter(0);  // constant
    fitPar1     = f1S->GetParameter(1);  // mean
    fitPar2     = f1S->GetParameter(2);  // sigma    
    if(MA_DEBUG>1) cout << " AvsBinBCS_XSlope[" << index << "]: "
			<< fitPar1 << ", " << fitPar2<< ", " ;
    f1S->GetParameters(&par[0]);
    totalS->SetParameters(par);     
    AvsBinBCS_XSlope[index]->Fit("totalS","RQNW");
    fitPar1     = totalS->GetParameter(1);
    fitPar2     = totalS->GetParameter(2);
    if(MA_DEBUG>1) cout << fitPar1 << ", " << fitPar2 << endl;
    S0NewAreaA = fS0Sigmas * fitPar2;
    S0NewMeanA = fitPar1;
   
    AvsBinBCS_YSlope[index]->Fit("f1S","RQNW");
    fitPar0     = f1S->GetParameter(0);  // constant
    fitPar1     = f1S->GetParameter(1);  // mean
    fitPar2     = f1S->GetParameter(2);  // sigma    
    if(MA_DEBUG>1) cout << " AvsBinBCS_YSlope[" << index << "]: "
			<< fitPar1 << ", " << fitPar2<< ", " ;
    f1S->GetParameters(&par[0]);
    totalS->SetParameters(par);     
    AvsBinBCS_YSlope[index]->Fit("totalS","RQNW");
    fitPar1     = totalS->GetParameter(1);
    fitPar2     = totalS->GetParameter(2);
    if(MA_DEBUG>1) cout << fitPar1 << ", " << fitPar2 << endl;
    S1NewAreaA = fS1Sigmas * fitPar2;
    S1NewMeanA = fitPar1;

    BvsAinACS_X[index]->Fit("f1X","RQNW");
    fitPar0     = f1X->GetParameter(0);  // constant
    fitPar1     = f1X->GetParameter(1);  // mean
    fitPar2     = f1X->GetParameter(2);  // sigma    
    if(MA_DEBUG>1) cout << " BvsAinACS_X[" << index << "]: "
			<< fitPar1 << ", " << fitPar2<< ", " ;
    f1X->GetParameters(&par[0]);
    totalX->SetParameters(par);     
    BvsAinACS_X[index]->Fit("totalX","RQNW");
    fitPar1     = totalX->GetParameter(1);
    fitPar2     = totalX->GetParameter(2);
    if(MA_DEBUG>1) cout << fitPar1 << ", " << fitPar2 << endl;
    XNewAreaB = fXSigmas * fitPar2;
    XNewMeanB = fitPar1;

    BvsAinACS_Y[index]->Fit("f1Y","RQNW");
    fitPar0     = f1Y->GetParameter(0);  // constant
    fitPar1     = f1Y->GetParameter(1);  // mean
    fitPar2     = f1Y->GetParameter(2);  // sigma    
    if(MA_DEBUG>1) cout << " BvsAinACS_Y[" << index << "]: "
			<< fitPar1 << ", " << fitPar2<< ", " ;    
    f1Y->GetParameters(&par[0]);
    totalY->SetParameters(par);     
    BvsAinACS_Y[index]->Fit("totalY","RQNW");
    fitPar1     = totalY->GetParameter(1);
    fitPar2     = totalY->GetParameter(2);
    if(MA_DEBUG>1) cout << fitPar1 << ", " << fitPar2 << endl;
    YNewAreaB = fYSigmas * fitPar2;
    YNewMeanB = fitPar1;

    BvsAinACS_XSlope[index]->Fit("f1S","RQNW");
    fitPar0     = f1S->GetParameter(0);  // constant
    fitPar1     = f1S->GetParameter(1);  // mean
    fitPar2     = f1S->GetParameter(2);  // sigma    
    if(MA_DEBUG>1) cout << " BvsAinACS_XSlope[" << index << "]: "
			<< fitPar1 << ", " << fitPar2<< ", " ;
    f1S->GetParameters(&par[0]);
    totalS->SetParameters(par);     
    BvsAinACS_XSlope[index]->Fit("totalS","RQNW");
    fitPar1     = totalS->GetParameter(1);
    fitPar2     = totalS->GetParameter(2);
    if(MA_DEBUG>1) cout << fitPar1 << ", " << fitPar2 << endl;
    S0NewAreaB = fS0Sigmas * fitPar2;
    S0NewMeanB = fitPar1;

    BvsAinACS_YSlope[index]->Fit("f1S","RQNW");
    fitPar0     = f1S->GetParameter(0);  // constant
    fitPar1     = f1S->GetParameter(1);  // mean
    fitPar2     = f1S->GetParameter(2);  // sigma    
    if(MA_DEBUG>1) cout << " BvsAinACS_YSlope[" << index << "]: "
			<< fitPar1 << ", " << fitPar2<< ", " ;    
    f1S->GetParameters(&par[0]);
    totalS->SetParameters(par);     
    BvsAinACS_YSlope[index]->Fit("totalS","RQNW");
    fitPar1     = totalS->GetParameter(1);
    fitPar2     = totalS->GetParameter(2);
    if(MA_DEBUG>1) cout << fitPar1 << ", " << fitPar2 << endl;
    S1NewAreaB = fS1Sigmas * fitPar2;
    S1NewMeanB = fitPar1;

    Stat_t entries = fAlignMeta->GetEntries();

    HMdcHit* hitA; 
    HMdcHit* hitB; 
    HGeomVector projPoint; 
    Float_t* projSlopes = new Float_t[2];
    Float_t* origSlopes = new Float_t[2];
    HGeomRotation rotaux;
    HGeomVector transaux;

    for (Int_t i=0;i<entries;i++) {     
      fAlignMeta->GetEntry(i);
      hitA = (HMdcHit*) fHits->getMdcHitA();
      hitB = (HMdcHit*) fHits->getMdcHitB();
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

      if(MA_DEBUG>3){
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
	    if(MA_DEBUG>3) cout << " -- CUT PASSED (fSlopeCut=" 
				<< fSlopeCut << " ) --" << endl;
	    
	    //Aqui no hago nada ??? Comprobar que realmente es lo correcto
	    //Creo que fHits contiene todo lo que necesito tal como
	    //viene de fAlignMeta. Solo hay que llenar el Cut si cumple el if
	    fHits->print();
	    fAlignMetaCut->Fill();
	    fCountCut++;
	  }
	}
	else{
	  if(MA_DEBUG>3) cout << " ---------  CUT PASSED ------------" << endl;

	  fAlignMetaCut->Fill();
	  fCountCut++;
	}
      }
    }
  }
}





TTree* HMdcMetaAligner::getTree(void)
{
  //
  // return a tree to the minimization functional.
  //
  if(fNumMods == 1)   return fAlignMeta;
  else {
    if(fUseCut) return fAlignMetaCut;
    return fAlignMeta;
  }
}




void HMdcMetaAligner::setTree(void)
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
    sprintf(tmp,"%s%s%i%s%i%s%i%s%i%s%i","Meta","_Sector_",sector,
	    "_ModA_",modA,"_ModB_",modB,"_ModC_",modC,"_ModD_",modD);    
    sprintf(tmp2,"%s%s%i%s%i%s%i%s%i%s%i","MetaCut","_Sector_",sector,
	    "_ModA_",modA,"_ModB_",modB,"_ModC_",modC,"_ModD_",modD);
  }
  else if(fNumMods>2){
    Int_t modC = fLoc[3];
    sprintf(title,"%s%i%s%i%s%i%s%i","Sector_",sector,"_ModA_",modA,
	    "_ModB_",modB,"_ModC_",modC);
    sprintf(tmp,"%s%s%i%s%i%s%i%s%i","Meta","_Sector_",sector,
	    "_ModA_",modA,"_ModB_",modB,"_ModC_",modC);    
    sprintf(tmp2,"%s%s%i%s%i%s%i%s%i","MetaCut","_Sector_",sector,
	    "_ModA_",modA,"_ModB_",modB,"_ModC_",modC);
  }
  else if(fNumMods>1){  
    sprintf(title,"%s%i%s%i%s%i","Sector_",sector,
	    "_ModA_",modA,"_ModB_",modB);
    sprintf(tmp,"%s%s%i%s%i%s%i","Meta","_Sector_",sector,
	    "_ModA_",modA,"_ModB_",modB);    
    sprintf(tmp2,"%s%s%i%s%i%s%i","MetaCut","_Sector_",sector,
	    "_ModA_",modA,"_ModB_",modB);
  }
  else{
    sprintf(title,"%s%i%s%i","Sector_",sector,
	    "_ModA_",modA);
    sprintf(tmp,"%s%s%i%s%i","Meta","_Sector_",sector,
	    "_ModA_",modA);    
    sprintf(tmp2,"%s%s%i%s%i","MetaCut","_Sector_",sector,
	    "_ModA_",modA);    
  }
  fAlignMeta = new TTree(tmp,title);
  fAlignMeta->Branch("hits","HMdcMetaHit",&fHits,64000);  
  fAlignMetaCut = new TTree(tmp2,title);
  fAlignMetaCut->Branch("hits","HMdcMetaHit",&fHits,64000);
}





Bool_t HMdcMetaAligner::init(void)
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
  
  fShowerHitCat=gHades->getCurrentEvent()->getCategory(catShowerHit);
  if (!fShowerHitCat) {
    fShowerHitCat=gHades->getSetup()->getDetector("Shower")
      ->buildCategory(catShowerHit);
    if (!fShowerHitCat) return kFALSE;
    else gHades->getCurrentEvent()
	   ->addCategory(catShowerHit,fShowerHitCat,"Shower");
  }

  fTofHitCat=gHades->getCurrentEvent()->getCategory(catTofHit);
  if (!fTofHitCat) {
    fTofHitCat=gHades->getSetup()->getDetector("Tof")
      ->buildCategory(catTofHit);
    if (!fTofHitCat) return kFALSE;
    else gHades->getCurrentEvent()->addCategory(catTofHit,fTofHitCat,"Tof");
  }

  fIter1 = (HIterator *)fHitCat->MakeIterator(); 
  fIter2 = (HIterator *)fHitCat->MakeIterator(); 
  fIter3 = (HIterator *)fHitCat->MakeIterator(); 
  fIter4 = (HIterator *)fHitCat->MakeIterator(); 
  fIter5 = (HIterator *)fShowerHitCat->MakeIterator(); 
  fIter6 = (HIterator *)fTofHitCat->MakeIterator(); 

  setParContainers();
 
  if(fHistoOff!=kTRUE) setHistograms();
  else setTree();
 
  return kTRUE;
}





Bool_t HMdcMetaAligner::reinit(void)
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





void HMdcMetaAligner::setGeomParAutoOn(void)
{
  //
  //Turn on the automatic geometrical parameter input
  //

  fAuto =kTRUE;  
  cout << "WARNING in HMdcMetaAligner::setGeomParManOn(): " 
       << "introducing manually Geometrical" << endl; 
  cout << "Parameters without containers. " 
       << "Parameters should be in the macro" << endl;
}





void HMdcMetaAligner::setMetaGeomParAutoOn(void)
{
  //
  //Turn on the automatic geometrical parameter input
  //

  fAutoMeta =kTRUE;  
  cout << "WARNING in HMdcMetaAligner::setMetaGeomParAutoOn(): " 
       << "introducing manually Geometrical" << endl; 
  cout << "Parameters without containers. " 
       << "Parameters should be in the macro" << endl;
}





void HMdcMetaAligner::setControlHistoOff(void)
{  
  //
  // Disables control histograms output (saving memory)
  //

  fHistoOff = kTRUE;  
}





void HMdcMetaAligner::setMinimization(Int_t select)
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
  // select = 500 + (2) Iterative minimization
  //            version of the previously described algorithms 
  //  in particular:
  // select = 502 Iterative version
  //
  fMin = select;  
}





void HMdcMetaAligner::setMinimizationOff(void)
{  
  //
  // Disables minimization
  //

  fMin = 0;  
}





void HMdcMetaAligner::setUnitErrors(void)
{  
  //
  // introduce unit errors in Hits
  //

  fUseUnitErrors = kTRUE;  
}





void HMdcMetaAligner::setUniqueRSInTof(Int_t det)
{  
  //
  // Only one Reference System is used for all TOF in one
  // sector
  //

  fUseUniqueRSInTof = kTRUE;  
  fUniqueTofDet = det;
}





void HMdcMetaAligner::setTargetOn(HGeomVector target)
{
  //
  // Target position is defined and used for
  // the hit definition in 
  // (WARNING. Use only with one MDC)
  //
  
  fUseTarget = kTRUE;
  fTargetPos = target;
}         





void HMdcMetaAligner::setFix(Int_t fix)
{  
  //
  // Fix a parameter set in minimization
  //
  //
  // 20    fixes translations 
  // 30    fixes rotations 
  // 40    fixes Z traslations 
  // 110   fixes all first and third angles

  fFix = fix;  
}





void HMdcMetaAligner::setNoCut(void)
{  
  //
  // Disables the cuts after fitting the histograms
  //

  fUseCut = kFALSE;  
}





void HMdcMetaAligner::setSlopeCut(Float_t SlopeCut)
{  
  //
  // Enables the Slope cuts after fitting the histograms.
  // This cut makes the sample near indep. of Z translations
  // and results useful for X and Y minimization.
  // The cut is effective only in MDCB, because fTranslation
  // is represented in MDCB coordinates. Then, tracks passing
  // the cut are almost parallel to Z direction

  fUseSlopeCut = kTRUE;  
  fSlopeCut = SlopeCut;
}





void HMdcMetaAligner::setParContainers(void)
{
  //
  // Loads the parameter containers it uses later
  //

  fMdcGeomPar=(HMdcGeomPar*)gHades->getRuntimeDb()->getContainer("MdcGeomPar");

  //El SHOWER no es standard. Su geometria es hija de HShowerParSet, 
  //el cual deriva directamente de HParSet, sin ser hijo (como en 
  //los otros casos) de HDetGeomPar.
  fShowerGeometry=(HShowerGeometry*)gHades->getRuntimeDb()
    ->getContainer("ShowerGeometry");
  if(!fShowerGeometry){    
    fShowerGeometry = new HShowerGeometry;
    if (!fShowerGeometry) {
      Error ("init","Unable to create Shower Geometry container");
      //return kFALSE;
    }
    gHades->getRuntimeDb()->addContainer(fShowerGeometry);
  }
  
  fTofGeomPar=(HTofGeomPar*)gHades->getRuntimeDb()->getContainer("TofGeomPar");
}





void HMdcMetaAligner::setGeomAuxPar(void)
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
  Int_t moduleC=-1,moduleD=-1;
  HGeomVector euler;

  HGeomTransform transformA;
  transformA = fMdcGeomPar->getModule(sector,moduleA)->getLabTransform();
  HGeomRotation rotA;
  rotA = transformA.getRotMatrix();
  HGeomVector vectorA;
  vectorA = transformA.getTransVector();

  HGeomRotation rotB,rotC,rotD;
  HGeomVector vectorB,vectorC, vectorD;

  if(fNumMods>3){
    moduleC = fLoc[3];  
    moduleD = fLoc[4];
    HGeomTransform transformB;
    transformB = fMdcGeomPar->getModule(sector,moduleB)->getLabTransform();
    HGeomTransform transformC;
    transformC = fMdcGeomPar->getModule(sector,moduleC)->getLabTransform();
    HGeomTransform transformD;
    transformD = fMdcGeomPar->getModule(sector,moduleD)->getLabTransform();
    rotB = transformB.getRotMatrix();
    vectorB = transformB.getTransVector();
    rotC = transformC.getRotMatrix();
    vectorC = transformC.getTransVector();
    rotD = transformD.getRotMatrix();
    vectorD = transformD.getTransVector();
    
    if(MA_DEBUG>0){
      cout << endl <<"Debugging output in HMdcMetaAligner::setGeomAuxPar" << endl;
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
   
    if(MA_DEBUG>0){
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
   
    if(MA_DEBUG>0){
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
   
    if(MA_DEBUG>0){
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
    cout << "* HMdcMetaAligner::setGeomAuxPar: from geom params:  *" << endl;
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
    moduleC = fLoc[3];  
    HGeomTransform transformB;
    transformB = fMdcGeomPar->getModule(sector,moduleB)->getLabTransform();
    HGeomTransform transformC;
    transformC = fMdcGeomPar->getModule(sector,moduleC)->getLabTransform();
    rotB = transformB.getRotMatrix();
    vectorB = transformB.getTransVector();
    rotC = transformC.getRotMatrix();
    vectorC = transformC.getTransVector();
    
    if(MA_DEBUG>0){
      cout << endl <<"Debugging output in HMdcMetaAligner::setGeomAuxPar" << endl;
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
       
    if(MA_DEBUG>0){
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
   
    if(MA_DEBUG>0){
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
    cout << "* HMdcMetaAligner::setGeomAuxPar: from geom params:  *" << endl;
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
  else if(fNumMods>1){
    HGeomTransform transformB;
    transformB = fMdcGeomPar->getModule(sector,moduleB)->getLabTransform();
    rotB = transformB.getRotMatrix();
    vectorB = transformB.getTransVector();
    if(MA_DEBUG>0){
      cout << endl <<"Debugging output in HMdcMetaAligner::setGeomAuxPar" 
	   << endl;
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
    
    if(MA_DEBUG>0){
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
    cout << "* HMdcMetaAligner::setGeomAuxPar: from geom params:  *" << endl;
    cout << "* Sector: "<< sector << "  ModA: " << moduleA 
	 << " ModB: " << moduleB << endl;
    cout << "* Rotation(0): " << fEuler[0].getX() << ", " 
	 << fEuler[0].getY()  << ", " << fEuler[0].getZ() << endl;   
    cout << "* Translation: " << relvector.getX() << ", " 
	 << relvector.getY() << " , " <<  relvector.getZ() << endl;
    cout << "**************************************************" << endl;
  }
  else{//1MDC
    if(MA_DEBUG>0){
      cout << endl <<"Debugging output in HMdcMetaAligner::setGeomAuxPar" 
	   << endl;
      cout << "Original transformation from container" << endl;
      cout << " ------ SECTOR " << sector << " ------ " << endl;
      cout << "MDC A (Module " << moduleA << ")" << endl;
      transformA.print(); 
      cout << " NO RELATIVE TRANSFORMATION (ONLY ONE MDC) " << endl;
    }

  }

  if (!fAutoMeta){
    // Relative transformation of Shower vs. MDCs
    // The transformations are:  X(SHOWER) = Rrel X(MDC) + Trel
    // where
    // X(SHOWER) = R(SHOWER)E-1 (X(LAB) - T(SHOWER))    
    // X(LAB) = R(MDC) X(MDC) + T(MDC)
    //
    // X(SHOWER)=[R(SHOWER)]E-1 R(MDC) X(MDC)+[R(SHOWER)]E-1 [T(MDC)-T(SHOWER)] 
    //
    // Rrel = [R(SHOWER)]E-1 R(MDC)
    // Trel = [R(SHOWER)]E-1 [T(MDC)-T(SHOWER)] 
    //
    // From M it is easy to get the relative Euler angles 
    
    HGeomTransform transformS = fShowerGeometry->getTransform(sector,0);
    HGeomRotation rotS;
    rotS = transformS.getRotMatrix();
    HGeomVector vectorS;
    vectorS = transformS.getTransVector();
    
    HGeomTransform transformT;
    HGeomRotation rotT[8];    
    HGeomVector vectorT[8];
    for(Int_t module=0;module<8;module++) {
      transformT = fTofGeomPar
	->getModule(sector,module)->getLabTransform();
      rotT[module] = transformT.getRotMatrix();
      vectorT[module] = transformT.getTransVector();
    }  
  
    HGeomRotation rotTinv;
    HGeomRotation rotSinv = rotS.inverse();
    HGeomRotation relSrot,relTrot;
    HGeomVector relSvector,relTvector;
    if (fNumMods>3){
      relSrot = rotSinv * rotD;
      relSvector = rotSinv * (vectorD - vectorS);
    }  
    else if(fNumMods>2){
      relSrot = rotSinv * rotC;
      relSvector =  rotSinv * (vectorC - vectorS);     
    }
    else if(fNumMods>1){    
      relSrot = rotSinv * rotB;
      relSvector =  rotSinv * (vectorB - vectorS);             
    }
    else{ //1 MDC
      relSrot = rotSinv * rotA;
      relSvector =  rotSinv * (vectorA - vectorS);             
    }  
    
    for(Int_t module=0;module<8;module++) {
      rotTinv = rotT[module].inverse();
      if (fNumMods>3){
	relTrot = rotTinv * rotD;
	relTvector = rotTinv * (vectorD - vectorT[module]); 
      }  
      else if(fNumMods>2){
	relTrot = rotTinv * rotC; 
	relTvector = rotTinv * (vectorC - vectorT[module]);
      }
      else if(fNumMods>1){    
	relTrot = rotTinv * rotB; 
	relTvector = rotTinv * (vectorB - vectorT[module]); 
      }
      else{    
	relTrot = rotTinv * rotA; 
	relTvector = rotTinv * (vectorA - vectorT[module]); 
      }
      
      euler = findEulerAngles(relTrot);  
      fillMetaRotMatrix(module,euler(0),euler(1),euler(2));
      fillMetaTranslation(module,relTvector.getX(),
			  relTvector.getY(),relTvector.getZ());
      setMetaEulerAngles(module,euler(0),euler(1),euler(2)); 
    }
    //Filling the Shower rotation 
    euler = findEulerAngles(relSrot);  
    fillMetaRotMatrix(8,euler(0),euler(1),euler(2));
    fillMetaTranslation(8,relSvector.getX(),relSvector.getY(),relSvector.getZ());
    setMetaEulerAngles(8,euler(0),euler(1),euler(2));
    //RELATIVE TOF TRANSFORMATIONS
    for(Int_t modu=0;modu<8;modu++) {
      //Introducing the fMRotTofRel fMTransTofRel for an unique CS in TOF
      //for instance, if fUniqueTofDet=REF:
      // From:
      // X_TOF_REF = fMR[REF] * X_MDC + fMT[REF]
      // X_TOF_# = fMR[#] * X_MDC + fMT[#]
      // one can obtain
      // X_TOF_REF = fMR[REF] * (fMR[#])E-1 * X_TOF_# 
      //             - fMR[REF] * (fMR[#])E-1 * fMT[#] + fMT[REF]
      //
      // X_TOF_REF=     fMRotTofRel[i]   * X_TOF_# +  fMTransTofRel[i]
      //
      fMRotTofRel[modu] = fMRotMat[fUniqueTofDet] * fMRotMat[modu].inverse();
      fMTransTofRel[modu] = fMTranslation[fUniqueTofDet] -
	fMRotTofRel[modu] * fMTranslation[modu];
      
      cout << "*************************************************" << endl;
      cout << "Relative TOF tranformations for module" << modu << endl;
      fMRotTofRel[modu].print();
      fMTransTofRel[modu].print();
      cout<< "**************************************************" << endl;
    }
    
    cout << "**************************************************" << endl;
    cout << "* HMdcMetaAligner::setGeomAuxPar: from geom params:  *" << endl;
    cout << "* Sector: "<< sector << "  Meta vs. "; 
    if(fNumMods==4) cout  << " MDC module: " << moduleD << endl;
    if(fNumMods==3) cout  << " MDC module: " << moduleC << endl;
    if(fNumMods==2) cout  << " MDC module: " << moduleB << endl;
    else cout  << " MDC module: " << moduleA << endl;
    cout << "From 0 to 7: TOF modules. 8 corresponds to SHOWER" << endl;;
    for(Int_t module=0;module<9;module++){
      cout << module << " Rotation: " << fMEuler[module].getX() << ", " 
	   << fMEuler[module].getY()  << ", " 
	   << fMEuler[module].getZ() << endl;   
      cout << module << " Translation: " 
	   << fMTranslation[module].getX() 
	   << ", "  << fMTranslation[module].getY() << " , " 
	   <<  fMTranslation[module].getZ() << endl;
      cout << "**************************************************" << endl;
    }
  }
}




HGeomVector HMdcMetaAligner::findEulerAngles(HGeomRotation rot){
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
  
  if(MA_DEBUG>0){
    cout << endl <<"Euler angles: " << euler[0] << ", " 
	 << euler[1]  << ", " << euler[2] << endl;
  }
  
  HGeomRotation test;  
  test.setEulerAngles(euler[0]*180./TMath::Pi(),
		      euler[1]*180./TMath::Pi(),
		      euler[2]*180./TMath::Pi()); 
  
  if(MA_DEBUG>0){
    cout << endl <<"Rotation from Euler angles (first attemp): " << endl;
    test.print();
  }
  
  //Now solving the problem when euler[1]<0
  Double_t eps = 0.0001; //check precision
  
  if( (fabs(test(0)-rot(0))>eps) || (fabs(test(1)-rot(1))>eps) || 
      (fabs(test(3)-rot(3))>eps) || (fabs(test(4)-rot(4))>eps) ) {
    if(MA_DEBUG>0) cout << endl << "Bad election in first euler angle! " 
			<< "Trying again. "<< endl;
    euler[1] = - euler[1]; 
    sinaux = sin(euler[1]);
    euler[0] = atan2(rot(5)/sinaux,rot(2)/sinaux);
    euler[2] = atan2(rot(7)/sinaux,-rot(6)/sinaux);
    
    test.setEulerAngles(euler[0]*180./TMath::Pi(),
			euler[1]*180./TMath::Pi(),
			euler[2]*180./TMath::Pi());  
    
    if(MA_DEBUG>0){
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





void HMdcMetaAligner::setGeomAuxParSim(Int_t ind, Float_t eu1, Float_t eu2,
				       Float_t eu3, Float_t tr1,
				       Float_t tr2, Float_t tr3){
  //
  // Entering geometrical parameters.
  //
  // To be used introducing in the macro the parameters for
  // testing, optimization ...
  //

  cout << "WARNING: Introducing automatically Geometrical" << endl; 
  cout << "Parameters without containers" << endl;

  if(ind!=0 || eu1!=0 || eu2!=0 || eu3!=0 || tr1!=0 || tr2!=0 || tr3!=0){
    fillRotMatrix(ind,eu1,eu2,eu3);
    fillTranslation(ind,tr1,tr2,tr3);  
    setEulerAngles(ind,eu1,eu2,eu3);
    if(MA_DEBUG>0){
      cout << "Transformation[" << ind << "]:" << endl;
      cout <<" Euler angles: " << eu1 << ", " 
	   << eu2  << ", " << eu3 << endl;
      cout << " Translation: " << tr1 << ", " 
	   << tr2  << ", " << tr3 << endl;
    }
  }
  else{
    // Parameters could enter automatically?
    cout << "  !!  Sorry, not implemented  !! " << endl;
  }
}





void HMdcMetaAligner::setMetaGeomAuxParSim(Int_t ind, Float_t eu1, Float_t eu2,
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

  if(ind!=0 || eu1!=0 || eu2!=0 || eu3!=0 || tr1!=0 || tr2!=0 || tr3!=0){
    fillMetaRotMatrix(ind,eu1,eu2,eu3);
    fillMetaTranslation(ind,tr1,tr2,tr3);  
    setMetaEulerAngles(ind,eu1,eu2,eu3);
    if(MA_DEBUG>0){
      cout << "MetaTransformation[" << ind << "]:" << endl;
      cout <<" Euler angles: " << eu1 << ", " 
	   << eu2  << ", " << eu3 << endl;
      cout << " Translation: " << tr1 << ", " 
	   << tr2  << ", " << tr3 << endl;
    }
  }
  else{
    // Parameters could enter automatically?
    cout << "  !!  Sorry, not implemented  !! " << endl;
  }
}





Int_t HMdcMetaAligner::execute(void)
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





void HMdcMetaAligner::execute4(void)
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
  HShowerHit* pHitS;    
  HTofHit* pHitT;
  HGeomVector* lSVec = new HGeomVector;
  HGeomVector* lTVec = new HGeomVector;

  Int_t sector = fLoc[0];
  Int_t modA = fLoc[1];
  Int_t modB = fLoc[2]; 
  Int_t modC = fLoc[3]; 
  Int_t modD = fLoc[4];
  
  HLocation local;
  local.setNIndex(2);
  local.set(2,sector,modD); 
  
  HGeomVector projPoint; 
  HGeomVector projMetaPoint; 
  Float_t* projSlopes = new Float_t[2];

  Bool_t usedA = kFALSE;   
  Bool_t usedB = kFALSE;
  Bool_t usedC = kFALSE;
  Bool_t invalidA = kFALSE; 
  Bool_t invalidB = kFALSE; 
  Bool_t invalidC = kFALSE; 
  Bool_t SUsed = kFALSE; 
  Bool_t SInvalid = kFALSE; 
  Bool_t TUsed = kFALSE; 
  Bool_t TInvalid = kFALSE; 

  Int_t detnum=-1;
  Int_t modTof =0;

  fNEntries++;
  
  fIter1->Reset(); 
  fIter1->gotoLocation(local); 
  while ( (pHitD =(HMdcHit*)fIter1->Next()) != 0 ){ 
    if(pHitD->getChi2()>0)
      {
	fHitsMdc[0]++;     //Hits found in MDC D
	usedA = kFALSE;    //reinit control flags
	usedB = kFALSE;
	usedC = kFALSE;
	invalidA = kFALSE; 
	invalidB = kFALSE; 
	invalidC = kFALSE; 
    
	if(MA_DEBUG>3)   {
	  cout << "    ----- SECTOR " << sector << " -----"<< endl; 
	  cout << "Module " << modD << ", fHitsMdc " << fHitsMdc[0] 
	       << endl;
	}  
    
	//   Calculation of cross point and slopes in MDC C  
	// The rotation matrix here used is inverse of fRotMat
	// (see CONVENTION in HMdcMetaAligner::setGeomAuxPar())
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
	  if(pHitC->getChi2()>0)
	    {
	      //hits found in MDCC, provided there is a Hit in MDCD
	      fHitsMdc[1]++;     
      
	      if(MA_DEBUG>3)  
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
		  // (see CONVENTION in HMdcMetaAligner::setGeomAuxPar())
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
		    if(pHitB->getChi2()>0)
		      {
			//hits found in MDCB, provided there is a Hit in MDCD 
			//and one in MDC. If there is more than one in MDCC the 
			//number is also increased but the hit will not be used
			fHitsMdc[2]++;
	      
			if(MA_DEBUG>3)  
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
			    // (see CONVENTION in HMdcMetaAligner::setGeomAuxPar())
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
			      if(pHitA->getChi2()>0)
				{
				  //hits found in MDCA, provided there is a Hit in MDCD 
				  //MDC and MDCB. If there is more than one in MDCC or MDCB 
				  //the number is also increased but the hit will not be used
				  fHitsMdc[3]++;

				  if(MA_DEBUG>3)  
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

		      
				      //Now,we enter in the META part!
				      //projectin on SHOWER plane
				      projMetaPoint = findProjMetaPoint(pHitC,pHitD,fRotMat[0],
									fTranslation[0],
									fMRotMat[8],
									fMTranslation[8]);
				      detnum =-1;
				      SUsed = kFALSE; 
				      SInvalid = kFALSE; 
				      TUsed = kFALSE; 
				      TInvalid = kFALSE; 
 
				      //SHOWER iteration      
				      fIter5->Reset();
				      while ( (pHitS =(HShowerHit*)fIter5->Next()) != 0 ){    
					if(isInsideMetaWindow(sector,pHitS,projMetaPoint,lSVec)){
					  //SHOWER Hit found in window
					  if(SUsed == kFALSE){  
					    SUsed = kTRUE;
					    fHitsFoundInShowerWindow++;
					    fHitsFoundInShowerAndUnique++;		      
					    fSCount++;	  
					    detnum =8;
					    //...
					  }
					  else{          //that is, if SUsed == kTRUE	
					    if(SInvalid == kFALSE){		
					      fSCount--;
					      SInvalid = kTRUE;
					      fHitsFoundInShowerAndUnique--;
					      fSDiscart++;
					      detnum =-1;
					    }
					  }
					}	    
				      }
				      //TOF iteration      
				      fIter6->Reset();
				      while ( (pHitT =(HTofHit*)fIter6->Next()) != 0 ){
					modTof = pHitT->getModule();
					projMetaPoint = findProjMetaPoint(pHitC,pHitD,fRotMat[0],
									  fTranslation[0],
									  fMRotMat[modTof],
									  fMTranslation[modTof]);
					if(isInsideMetaWindow(sector,pHitT,projMetaPoint,lTVec)){
					  //TOF Hit found in window
					  if(TUsed == kFALSE){  		
					    TUsed = kTRUE;		
					    fHitsFoundInTofWindow[modTof]++;
					    fHitsFoundInTofAndUnique[modTof]++;	
					    fTCount++;		
					    if(detnum == 8) detnum=detnum+1+modTof;
					    else detnum = modTof;
					    //...
					  }
					  else{          //that is, if TUsed == kTRUE	
					    //POR AHORA NO PERMITO DOS HITS EN TOF.CAMBIAR!!
					    if(TInvalid == kFALSE){		
					      fTCount--;
					      TInvalid = kTRUE;
					      fHitsFoundInTofAndUnique[modTof]--;
					      fTDiscart[modTof]++;
					      detnum = -1;
					    }
					  }
					}
				      }
				      fHits->setMdcHitA(pHitA);
				      fHits->setMdcHitB(pHitB);
				      fHits->setMdcHitC(pHitC);
				      fHits->setMdcHitD(pHitD);
				      if(detnum<0) fHits->setNumberOfMetaHits(0);
				      else {
					fHits->setNumberOfMetaHits(1);
					if(detnum>7) {
					  fHits->setNumberOfMetaHits(2);
					  fHits->setLocalHitA(lSVec);
					  fHits->setLocalHitB(lTVec);
					}
					else if(detnum==8) fHits->setLocalHitA(lSVec);
					else fHits->setLocalHitA(lTVec);
				      }
				      fHits->setDetector(detnum);
       
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
				} // end ot chi2 cut
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
		      } // end of chi2 cut
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
	    } // end of chi2 
	} // End of  while( (pHitC =(HMdcHit*)fIter2->Next()) != 0 ){ 

	if(usedC == kTRUE && invalidC != kTRUE && 
	   usedB == kTRUE && invalidB != kTRUE && 
	   usedA == kTRUE && invalidA != kTRUE){
	  fAlignMeta->Fill();
	  if(fCount%100 ==0) cout << "."<< flush;
	}
      } // end of chi2 cut
  } // End of  while ( (pHitD =(HMdcHit*)fIter1->Next()) != 0 ){ 
}
  


  void HMdcMetaAligner::execute3(void)
    {    
      // 
      // New execute for more than two MDCs
      //
  
      HMdcHit* pHitA;         
      HMdcHit* pHitB;       
      HMdcHit* pHitC; 
      HMdcHit* pHitBC;
      HMdcHit* pHitABC;
      HShowerHit* pHitS;    
      HTofHit* pHitT;
      HGeomVector* lSVec = new HGeomVector;
      HGeomVector* lTVec = new HGeomVector;

      Int_t sector = fLoc[0];
      Int_t modA = fLoc[1];
      Int_t modB = fLoc[2]; 
      Int_t modC = fLoc[3]; 

      HLocation local;
      local.setNIndex(2);
      local.set(2,sector,modC); 
  
      HGeomVector projPoint; 
      HGeomVector projMetaPoint; 
      Float_t* projSlopes = new Float_t[2];

      Bool_t usedA = kFALSE;   
      Bool_t usedB = kFALSE;
      Bool_t invalidA = kFALSE; 
      Bool_t invalidB = kFALSE; 
      Bool_t SUsed = kFALSE; 
      Bool_t SInvalid = kFALSE; 
      Bool_t TUsed = kFALSE; 
      Bool_t TInvalid = kFALSE; 

      Int_t detnum=-1;
      Int_t modTof =0;

      fNEntries++;

      fIter1->Reset(); 
      fIter1->gotoLocation(local); 
      while ( (pHitC =(HMdcHit*)fIter1->Next()) != 0 ){ 
	if(pHitC->getChi2()>0)
	  {
	    fHitsMdc[0]++;     //Hits found in MDC C
	    usedA = kFALSE;    //reinit control flags
	    usedB = kFALSE;
	    invalidA = kFALSE; 
	    invalidB = kFALSE;  
    
	    if(MA_DEBUG>3)   {
	      cout << "     ----- SECTOR " << sector << " -----"<< endl; 
	      cout << "Module " << modC << ", fHitsMdc " << fHitsMdc[0] 
		   << endl;
	    }  
    
	    //   Calculation of cross point and slopes in MDC B  
	    // The rotation matrix here used is inverse of fRotMat
	    // (see CONVENTION in HMdcMetaAligner::setGeomAuxPar())
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
	      if(pHitB->getChi2()>0)
		{
		  //hits found in MDCB, provided there is a Hit in MDCC
		  fHitsMdc[1]++;     
    
		  if(MA_DEBUG>3)  
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
		      // (see CONVENTION in HMdcMetaAligner::setGeomAuxPar())
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
			if(pHitA->getChi2()>0)
			  {
			    //hits found in MDCA, provided there is a Hit in MDCC 
			    //and one in MDB. If there is more than one in MDCB the 
			    //number is also increased but the hit will not be used
			    fHitsMdc[2]++;
	    
			    if(MA_DEBUG>3)  
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
		
				//Now,we enter in the META part!
				//projectin on SHOWER plane
				projMetaPoint = findProjMetaPoint(pHitB,pHitC,fRotMat[0],
								  fTranslation[0],
								  fMRotMat[8],
								  fMTranslation[8]);
				detnum =-1;
				SUsed = kFALSE; 
				SInvalid = kFALSE; 
				TUsed = kFALSE; 
				TInvalid = kFALSE; 
 
				//SHOWER iteration      
				fIter5->Reset();
				while ( (pHitS =(HShowerHit*)fIter5->Next()) != 0 ){    
				  if(isInsideMetaWindow(sector,pHitS,projMetaPoint,lSVec)){
				    //SHOWER Hit found in window
				    if(SUsed == kFALSE){  
				      SUsed = kTRUE;
				      fHitsFoundInShowerWindow++;
				      fHitsFoundInShowerAndUnique++;		      
				      fSCount++;	  
				      detnum =8;
				      //...
				    }
				    else{          //that is, if SUsed == kTRUE	
				      if(SInvalid == kFALSE){		
					fSCount--;
					SInvalid = kTRUE;
					fHitsFoundInShowerAndUnique--;
					fSDiscart++;
					detnum =-1;
				      }
				    }
				  }	    
				}
				//TOF iteration      
				fIter6->Reset();
				while ( (pHitT =(HTofHit*)fIter6->Next()) != 0 ){
				  modTof = pHitT->getModule();
				  projMetaPoint = findProjMetaPoint(pHitB,pHitC,fRotMat[0],
								    fTranslation[0],
								    fMRotMat[modTof],
								    fMTranslation[modTof]);
				  if(isInsideMetaWindow(sector,pHitT,projMetaPoint,lTVec)){
				    //TOF Hit found in window
				    if(TUsed == kFALSE){  		
				      TUsed = kTRUE;		
				      fHitsFoundInTofWindow[modTof]++;
				      fHitsFoundInTofAndUnique[modTof]++;	
				      fTCount++;		
				      if(detnum == 8) detnum=detnum+1+modTof;
				      else detnum = modTof;
				      //...
				    }
				    else{          //that is, if TUsed == kTRUE	
				      //POR AHORA NO PERMITO DOS HITS EN TOF.CAMBIAR!!
				      if(TInvalid == kFALSE){		
					fTCount--;
					TInvalid = kTRUE;
					fHitsFoundInTofAndUnique[modTof]--;
					fTDiscart[modTof]++;
					detnum = -1;
				      }
				    }
				  }
				}
				fHits->setMdcHitA(pHitA);
				fHits->setMdcHitB(pHitB);
				fHits->setMdcHitC(pHitC);
				if(detnum<0) fHits->setNumberOfMetaHits(0);
				else {
				  fHits->setNumberOfMetaHits(1);
				  if(detnum>7) {
				    fHits->setNumberOfMetaHits(2);
				    fHits->setLocalHitA(lSVec);
				    fHits->setLocalHitB(lTVec);
				  }
				  else if(detnum==8) fHits->setLocalHitA(lSVec);
				  else fHits->setLocalHitA(lTVec);
				}
				fHits->setDetector(detnum);
		
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
			  } // end of chi2 cut
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
		} // end of chi2 cut
	    } // End of  while( (pHitB =(HMdcHit*)fIter2->Next()) != 0 ){ 
    
	    if(usedB == kTRUE && invalidB != kTRUE && 
	       usedA == kTRUE && invalidA != kTRUE){
	      fAlignMeta->Fill();
	      if(fCount%100 ==0) cout << "."<< flush;
	    }
	  } // end of chi2 cut
      } // End of  while ( (pHitC =(HMdcHit*)fIter1->Next()) != 0 ){ 
    }





  void HMdcMetaAligner::execute2(void)
    {     
      // 
      // Adapting the old execute
      //

      HMdcHit* pHitA; 
      HMdcHit* pHitB; 
      HMdcHit* pHitAB;
      HShowerHit* pHitS;    
      HTofHit* pHitT;
      HGeomVector* lSVec = new HGeomVector;
      HGeomVector* lTVec = new HGeomVector;

      Int_t sector = fLoc[0];
      Int_t modA = fLoc[1];
      Int_t modB = fLoc[2]; 
 
      HLocation local;
      local.setNIndex(2);
      local.set(2,sector,modB); 

      HGeomVector projPoint;   
      HGeomVector projMetaPoint; 
      Float_t* projSlopes = new Float_t[2];

      Bool_t usedA = kFALSE; 
      Bool_t invalidA = kFALSE;
      Bool_t SUsed = kFALSE; 
      Bool_t SInvalid = kFALSE; 
      Bool_t TUsed = kFALSE; 
      Bool_t TInvalid = kFALSE; 

      Int_t detnum=-1;
      Int_t modTof =0;

      fNEntries++;

      fIter1->Reset(); 
      fIter1->gotoLocation(local); 

      while ( (pHitB =(HMdcHit*)fIter1->Next()) != 0 ){ 
	if(pHitB->getChi2()>0)
	  {
	    fHitsMdc[0]++;   //Hits found in MDC B
	    usedA = kFALSE;                   //reinit control flag
	    invalidA = kFALSE;               
	    SUsed = kFALSE; 
	    SInvalid = kFALSE; 
	    TUsed = kFALSE; 
	    TInvalid = kFALSE;

	    if(MA_DEBUG>3)   {
	      cout << "     ----- SECTOR " << sector << " -----"<< endl; 
	      cout << "Module " << modB << ", fHitsMdc " << fHitsMdc[0] 
		   << endl;
	    }  
    
	    //   Calculation of cross point and slopes in MDC A  
	    // The rotation matrix here used is inverse of fRotMat
	    // (see CONVENTION in HMdcMetaAligner::setGeomAuxPar())
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
	      if(pHitA->getChi2()>0)
		{
		  //hits found in MDCA, provided there is a Hit in MDCB
		  fHitsMdc[1]++;     
    
		  if(MA_DEBUG>3)  
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
	  
		      //merging all HITS
		      pHitAB = mergeHits(pHitB,pHitA,fRotMat[0],
					 fTranslation[0]);
	  
		      //Now,we enter in the META part!
		      //projectin on SHOWER plane
		      projMetaPoint = findProjMetaPoint(pHitA,pHitB,fRotMat[0],
							fTranslation[0],
							fMRotMat[8],
							fMTranslation[8]);
		      detnum =-1;
		      SUsed = kFALSE; 
		      SInvalid = kFALSE; 
		      TUsed = kFALSE; 
		      TInvalid = kFALSE; 
	  
		      //SHOWER iteration      
		      fIter5->Reset();
		      while ( (pHitS =(HShowerHit*)fIter5->Next()) != 0 ){    
			if(isInsideMetaWindow(sector,pHitS,projMetaPoint,lSVec)){
			  //SHOWER Hit found in window
			  if(SUsed == kFALSE){  
			    SUsed = kTRUE;
			    fHitsFoundInShowerWindow++;
			    fHitsFoundInShowerAndUnique++;		      
			    fSCount++;	  
			    detnum =8;
			    //...
			  }
			  else{          //that is, if SUsed == kTRUE	
			    if(SInvalid == kFALSE){		
			      fSCount--;
			      SInvalid = kTRUE;
			      fHitsFoundInShowerAndUnique--;
			      fSDiscart++;
			      detnum = -1;
			    }
			  }
			}	    
		      }
		      //TOF iteration      
		      fIter6->Reset();
		      while ( (pHitT =(HTofHit*)fIter6->Next()) != 0 ){
			modTof = pHitT->getModule();
			projMetaPoint = findProjMetaPoint(pHitA,pHitB,fRotMat[0],
							  fTranslation[0],
							  fMRotMat[modTof],
							  fMTranslation[modTof]);
			if(isInsideMetaWindow(sector,pHitT,projMetaPoint,lTVec)){
			  //TOF Hit found in window
			  if(TUsed == kFALSE){  		
			    TUsed = kTRUE;		
			    fHitsFoundInTofWindow[modTof]++;
			    fHitsFoundInTofAndUnique[modTof]++;	
			    fTCount++;		
			    if(detnum == 8) detnum=detnum+1+modTof;
			    else detnum = modTof;
			    //...
			  }
			  else{          //that is, if TUsed == kTRUE	
			    //POR AHORA NO PERMITO DOS HITS EN TOF.CAMBIAR!!
			    if(TInvalid == kFALSE){		
			      fTCount--;
			      TInvalid = kTRUE;
			      fHitsFoundInTofAndUnique[modTof]--;
			      fTDiscart[modTof]++;
			      detnum = -1;
			    }
			  }
			}
		      }
		      fHits->setMdcHitA(pHitA);
		      fHits->setMdcHitB(pHitB);	      
		      lSVec->setZ(detnum);
		      lTVec->setZ(detnum);	
		      fHits->setNumberOfMDCs(2);
		      fHits->setDetector(detnum);
		      if(detnum<0) fHits->setNumberOfMetaHits(0);
		      else {
			fHits->setNumberOfMetaHits(1);
			if(detnum>8) {
			  fHits->setNumberOfMetaHits(2);
			  fHits->setLocalHitA(lSVec);
			  fHits->setLocalHitB(lTVec);
			}
			else if(detnum==8) fHits->setLocalHitA(lSVec);
			else fHits->setLocalHitA(lTVec);
		      }
		      fHits->setDetector(detnum);

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
		}// end of chi2 cut
	    } // End of  while( (pHitA =(HMdcHit*)fIter2->Next()) != 0 ){ 
	    if(MA_DEBUG>3) cout << "Logic values: " << (Int_t)usedA << (Int_t)invalidA 
				<< (Int_t)SUsed << (Int_t)SInvalid << (Int_t)TUsed 
				<< (Int_t)TInvalid << endl;

	    if(usedA == kTRUE && invalidA != kTRUE && 
	       ((SUsed == kTRUE && SInvalid != kTRUE) ||
		(TUsed == kTRUE && TInvalid != kTRUE))  ){
	      if(MA_DEBUG>3) fHits->print();
	      fAlignMeta->Fill();
	      if(fCount%100 ==0) cout << "."<< flush;
	    }
	  } // end of chi2 cut
      } // End of  while ( (pHitB =(HMdcHit*)fIter1->Next()) != 0 ){ 
      delete lSVec;  
      delete lTVec;  
    }  




 
  void HMdcMetaAligner::execute1(void)
    {    
      // 
      // Only one MDC. 
      //

      HMdcHit* pHitA; 
      HShowerHit* pHitS;    
      HTofHit* pHitT;
      HGeomVector* lSVec = new HGeomVector;
      HGeomVector* lTVec = new HGeomVector;

      Int_t sector = fLoc[0];
      Int_t modA = fLoc[1];
 
      HLocation local;
      local.setNIndex(2);
      local.set(2,sector,modA); 

      HGeomVector projPoint;   
      HGeomVector projMetaPoint; 

      Bool_t SUsed = kFALSE; 
      Bool_t SInvalid = kFALSE; 
      Bool_t TUsed = kFALSE; 
      Bool_t TInvalid = kFALSE; 

      Int_t detnum=-1;
      Int_t modTof =0;

      fIter1->Reset(); 
      fIter1->gotoLocation(local); 
      while ( (pHitA =(HMdcHit*)fIter1->Next()) != 0 ){ 
	if(pHitA->getChi2()>0)
	  {
	    fHitsMdc[0]++;	  
	    fCount++;

	    //Now,we enter in the META part!
	    //projectin on SHOWER plane
	    projMetaPoint = findProjMetaPoint(pHitA,fMRotMat[8],
					      fMTranslation[8]);
	    detnum =-1;
	    SUsed = kFALSE; 
	    SInvalid = kFALSE; 
	    TUsed = kFALSE; 
	    TInvalid = kFALSE; 
	    //SHOWER iteration      
	    fIter5->Reset();
	    while ( (pHitS =(HShowerHit*)fIter5->Next()) != 0 ){    
	      if(isInsideMetaWindow(sector,pHitS,projMetaPoint,lSVec)){
		//SHOWER Hit found in window
		if(SUsed == kFALSE){  
		  SUsed = kTRUE;
		  fHitsFoundInShowerWindow++;
		  fHitsFoundInShowerAndUnique++;		      
		  fSCount++;	  
		  detnum =8;
		  //...
		}
		else{          //that is, if SUsed == kTRUE	
		  if(SInvalid == kFALSE){		
		    fSCount--;
		    SInvalid = kTRUE;
		    fHitsFoundInShowerAndUnique--;
		    fSDiscart++;
		    detnum = -1;
		  }
		}
	      }	    
	    }
	    //TOF iteration      
	    fIter6->Reset();
	    while ( (pHitT =(HTofHit*)fIter6->Next()) != 0 ){
	      modTof = pHitT->getModule();
	      projMetaPoint = findProjMetaPoint(pHitA,fMRotMat[modTof],
						fMTranslation[modTof]);
	      if(isInsideMetaWindow(sector,pHitT,projMetaPoint,lTVec)){
		//TOF Hit found in window
		if(TUsed == kFALSE){  		
		  TUsed = kTRUE;		
		  fHitsFoundInTofWindow[modTof]++;
		  fHitsFoundInTofAndUnique[modTof]++;	
		  fTCount++;		
		  if(detnum == 8) detnum=detnum+1+modTof;
		  else detnum = modTof;
		  //...
		}
		else{          //that is, if TUsed == kTRUE	
		  //POR AHORA NO PERMITO DOS HITS EN TOF.CAMBIAR!!
		  if(TInvalid == kFALSE){		
		    fTCount--;
		    TInvalid = kTRUE;
		    fHitsFoundInTofAndUnique[modTof]--;
		    fTDiscart[modTof]++;
		    detnum = -1;
		  }
		}
	      }
	    }

	    fHits->setMdcHitA(pHitA);
	    lSVec->setZ(detnum);
	    lTVec->setZ(detnum);
	    if(detnum<0) fHits->setNumberOfMetaHits(0);
	    else {
	      fHits->setNumberOfMetaHits(1);
	      if(detnum>8) {
		fHits->setNumberOfMetaHits(2);
		fHits->setLocalHitA(lSVec);
		fHits->setLocalHitB(lTVec);
	      }
	      else if(detnum==8) fHits->setLocalHitA(lSVec);
	      else fHits->setLocalHitA(lTVec);
	    }
	    fHits->setDetector(detnum);
    
	    if( ((SUsed == kTRUE && SInvalid != kTRUE) ||
		 (TUsed == kTRUE && TInvalid != kTRUE))  ){
	      if(MA_DEBUG>3) fHits->print();
	      fAlignMeta->Fill();
	      //
	      // It is possible to fill also fAlignMetaCut->Fill();
	      //with a condition on the Hits
	      //
	      if(fCount%100 ==0) cout << "."<< flush;
	    }
	  } // end of chi2 cut
      } // End of  while ( (pHitA =(HMdcHit*)fIter1->Next()) != 0 ){   
      delete lSVec;  
      delete lTVec;
    }
 



 
  void HMdcMetaAligner::transfEuler(HGeomRotation eulrot,HGeomVector eulvec,
				    HGeomVector oldV, HGeomVector newV){
    //
    // Transformation from one coordinate system to a new one
    //     newV = eulrot * oldV + eulvec
    //
    newV = eulrot * oldV + eulvec;
  }




 
  void HMdcMetaAligner::transfEulerInv(HGeomRotation eulrot,HGeomVector eulvec,
				       HGeomVector oldV, HGeomVector newV){
    //
    // Transformation from one coordinate system to a new one
    //     newV = eulrot * (oldV - eulvec)
    //
    newV = eulrot * (oldV - eulvec);
  }
 


 

  HGeomVector HMdcMetaAligner::findProjMetaPoint(HMdcHit* pHitA,
						 HGeomRotation rotM,
						 HGeomVector traM)
    {
      //
      // Find the projection point of a Hit on META plane
      // Uses a Hit to project on the META plane 
      // defined by rotM and traM 
      //  
      // Two methods are defined. One suposes target at fTargetPos
      // (fUseTarget =kTRUE) and the other uses the hit slopes.
      //

      HGeomVector point,direct,result;  
      HGeomVector pointInMeta,directInMeta;

      Float_t third;
      third = sqrt(1 - pHitA->getXDir()*pHitA->getXDir() - 
		   pHitA->getYDir()*pHitA->getYDir());

      point.setX(pHitA->getX());              //getting the hit info     
      point.setY(pHitA->getY());
      point.setZ(0.);

      if(fUseTarget){
	direct = point-fTargetPos;
      }
      else{
	direct.setX(pHitA->getXDir()); 
	direct.setY(pHitA->getYDir()); 
	direct.setZ(third); 
      }
      //transform to the new system
      pointInMeta = rotM * point + traM;
      directInMeta = rotM * direct;
  
      //    x-pM.getX()            y-pM.getY()           -pM.getZ()
      // ------------------- = ------------------- = -------------------
      //  direcInMeta.getX()   directInMeta.getY()   directInMeta.getZ()  

      //
      result.setX(pointInMeta.getX()-
		  pointInMeta.getZ()*directInMeta.getX()/directInMeta.getZ());
      result.setY(pointInMeta.getY()-
		  pointInMeta.getZ()*directInMeta.getY()/directInMeta.getZ());
      result.setZ(0.);

      return result;
    }





  HGeomVector HMdcMetaAligner::findProjMetaPoint(HMdcHit* pHitA,
						 HMdcHit* pHitB, 
						 HGeomRotation rot,
						 HGeomVector tra, 
						 HGeomRotation rotM,
						 HGeomVector traM)
    {
      //
      // Find the projection point of a Hit pair on META plane
      //
      // Given a relative rotation and translation from MDC A to MDC B
      // (see CONVENTION in HMdcMetaAligner::setGeomAuxPar())
      //  X(B) = rot X(A) + tra
      // this function obtains the MDC A coordinates in 
      // MDC B coordinate system using rot and tra.
      // Then uses both Hits to project on the META plane 
      // defined by rotM and traM 
      //  
      HGeomVector a,b,anew,ainmeta,binmeta;
      HGeomVector newvec;

      a.setX(pHitA->getX());              //getting the hit info     
      a.setY(pHitA->getY());
      a.setZ(0.);
      b.setX(pHitB->getX());              //getting the hit info     
      b.setY(pHitB->getY());
      b.setZ(0.);

      //coordinates of MDC A in MDC B
      anew = rot * a + tra;
      ainmeta = rotM * anew + traM;
      binmeta = rotM * b + traM;
    
      //straigth line from both hits and cross point with plane ZMETA=0
      //    x-aM.getX()            y-aM.getY()           z-aM.getZ()
      // ------------------- = ------------------- = -------------------
      // bM.getX()-aM.getX()   bM.getY()-aM.getY()   bM.getZ()-aM.getZ()  
      //
      // then for z=0:  
      // x=aM.getX()-aM.getZ()*(bM.getX()-aM.getX())/(bM.getZ()-aM.getZ())
      // y=aM.getY()-aM.getZ()*(bM.getY()-aM.getY())/(bM.getZ()-aM.getZ())
      //
      Float_t consta = ainmeta.getZ()/(binmeta.getZ()-ainmeta.getZ());
      Float_t xproj = ainmeta.getX()-(binmeta.getX()-ainmeta.getX())*consta;
      Float_t yproj = ainmeta.getY()-(binmeta.getY()-ainmeta.getY())*consta;

      if(MA_DEBUG>3)cout << "Projected on META: " << xproj << " " << yproj << endl;

      newvec.setX(xproj);
      newvec.setY(yproj);
      newvec.setZ(0.);
      return newvec;
    }





  HGeomVector HMdcMetaAligner::findProjPoint(HMdcHit* pHit, HGeomRotation rot,
					     HGeomVector tra, Float_t* slopes)
    {
      //
      // Find the projection point of a Hit on another MDC
      //
      // Given a relative rotation and translation from MDC A to MDC B
      // (see CONVENTION in HMdcMetaAligner::setGeomAuxPar())
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
      Float_t x, y, s0, s1;
      Float_t xDir, yDir, aux, den;  
      Float_t zsearch, xsearch, ysearch;

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
      
      if(MA_DEBUG>3){
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
	cout << "ERROR in HMdcMetaAligner::findProjPoint()" << endl;
	return newvec;
      }  
      slopes[0] = (s0*rot(0) + s1*rot(1) + rot(2)) / den;
      slopes[1] = (s0*rot(3) + s1*rot(4) + rot(5)) / den;

      if(MA_DEBUG>3){
	cout << "Projected MDC HIT: " << xsearch << " " << ysearch 
	     <<  " " << slopes[0] << " " << slopes[1] << endl;
      }

      newvec.setX(xsearch);
      newvec.setY(ysearch);
      newvec.setZ(zsearch);
      return newvec;
    }





  Int_t HMdcMetaAligner::isInsideMetaWindow(Int_t sector,
					    HTofHit* pHitT, 
					    HGeomVector proj,
					    HGeomVector* vec){
    //
    //Check if the TOF hit is inside a window around proj  point 
    //If it is, include in vec
  
    //montones de parametros que deberian estar en contenedores estan a mano!
    Float_t largeCellWidth = 30.0;
    Float_t smallCellWidth = 20.0;
    Float_t xTof = pHitT->getXpos(); //   /10?? Other units??  
    Int_t tofCell = (Int_t)pHitT->getCell();
    Int_t tofModule = (Int_t)pHitT->getModule();
    Float_t yTof;
    if(tofModule<4) yTof = largeCellWidth * (3.5 - tofCell);
    else  yTof = smallCellWidth * (3.5 - tofCell);
  
    if (pHitT->getSector() == sector) {    
      if(fUseUniqueRSInTof==kTRUE){  
	fUniqueAllResTofX->Fill(xTof-proj.getX());
	fUniqueAllResTofY->Fill(yTof-proj.getY());	
      }
      fAllResTofX[tofModule]->Fill(xTof-proj.getX());
      fAllResTofY[tofModule]->Fill(yTof-proj.getY());	
   
      if(MA_DEBUG>3)
	cout << "VALID TOF HIT: " << sector  << " " << tofModule << " " 
	     << tofCell << " " << xTof << " " << yTof << endl;
    
      if((fabs(xTof-proj.getX())<fXArea) 
	 && (fabs(yTof-proj.getY())<fYArea)){ 
	if(MA_DEBUG>3) cout <<  "  inside window" << endl;    
	vec->setX(xTof);
	vec->setY(yTof);
	vec->setZ(0);      
	return (tofModule+1);
      }
      else return 0;
    }
    else return 0;	     
  }  





  Int_t HMdcMetaAligner::isInsideMetaWindow(Int_t sector,
					    HShowerHit* pHitS, 
					    HGeomVector proj,
					    HGeomVector* vec){
    //
    //Check if the SHOWER hit is inside a window around proj  point 
    //If it is, include in vec

    //SHOWER
    //getting info from the address
    Int_t SAddress = pHitS->getAddress();
    Int_t SCol = SAddress%100;
    Int_t SRow = ((SAddress-SCol)/100)%100;
    Int_t SMod = ((SAddress-SRow-SCol)/10000)%10;
    //Int_t SSec = ((SAddress-SRow-SCol-SMod)/100000)%10; 
    //NO. SECTOR 0 = SECTOR 6  arriba!!!
    Int_t SSec =  (Int_t)pHitS->getSector();
    Float_t hitS[3];

    //getting the pad coordinates
    fShowerGeometry->getPadParam(0)->
      getPad(SRow, SCol)->getPadCenter(&hitS[0],&hitS[1]);
    hitS[2] = 0.;
    //cm to mm
    hitS[0] = hitS[0]*10.;
    hitS[1] = hitS[1]*10.;

    //  cout << "Antes:" << endl;
    //  cout << "hitS[0]" << hitS[0] << " hitS[1]" << hitS[1] << endl;
    pHitS->getXY(&hitS[0],&hitS[1]);
    //  cout << "Despues:" << endl;
    //  cout << "hitS[0]" << hitS[0] << " hitS[1]" << hitS[1] << endl;

    if(MA_DEBUG>3){
      cout << "VALID SHOWER HIT: " << SAddress << " " << SSec 
	   << " " << SMod << " " << SRow << " " << SCol 
	   <<  " " << hitS[0] << " " << hitS[1] << endl;
      //cout << "TESTING SHOWER HIT: " << (Int_t)pHitS->getSector() 
      //	 << " " << (Int_t)pHitS->getModule() << " " 
      //	 << (Int_t)pHitS->getRow() << " " 
      //	 << (Int_t)pHitS->getCol() << endl;
    }
  
    if(SMod == 0 && SSec == sector){	    
      ShowerPlaneProj->Fill(hitS[0],hitS[1]);
      fAllResShowerX->Fill(hitS[0]-proj.getX());
      fAllResShowerY->Fill(hitS[1]-proj.getY());
      if((fabs(hitS[0]-proj.getX())<fXArea) 
	 && (fabs(hitS[1]-proj.getY())<fYArea)){ 
	if(MA_DEBUG>3) cout <<  "  inside window" << endl;
	vec->setVector(hitS);
	return 8;
      }
      else return 0;
    }
    else return 0;	     
  }  





  Bool_t HMdcMetaAligner::isInsideWindow(Int_t plot, HMdcHit* pHit, 
					 HGeomVector point, Float_t* slope){
    //
    //Check if the hit is inside a window around point and slope
    //
  
    //old check. Implement new one PLEASE!  
    //New one base on equiprobability elipse (Beatriz paper)
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

    if(MA_DEBUG>3) cout << "MDC HIT: " 
			<< pHit->getX() << " " << pHit->getY();
  
    if( (pHit->getX()>xlolimit) && (pHit->getX()<xuplimit) && 
	(pHit->getY()>ylolimit) && (pHit->getY()<yuplimit)) {
      if(MA_DEBUG>3) cout <<  "  inside window" << endl;
      return kTRUE;
    }

    else {    
      if(MA_DEBUG>3) cout <<  "  outside window" << endl;
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
    if(MA_DEBUG>3) cout <<  "  inside window" << endl;
    return kTRUE;
    }
    else {    
    if(MA_DEBUG>3) cout <<  "  outside window" << endl;
    return kFALSE;
    }
    */
  }



  HMdcHit* HMdcMetaAligner::mergeHits(HMdcHit* hitB, HMdcHit* hitA,
				      HGeomRotation rot,HGeomVector tra){
    //
    // Propagates hitA in hitB coordinate system and merges 
    // the information in a new hit in hitB coordinate system
    // The rot and tra verifies the CONVENTION 
    // (see HMdcMetaAligner::setGeomAuxPar()), that is:
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





  Float_t* HMdcMetaAligner::transformToSlopes(HMdcHit* pHit){
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





  Bool_t HMdcMetaAligner::finalize(void)
    {   
      // 
      // Statistical information and Minimization procedure
      //
    
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
	*fout << "Module A: " << modA; 
	if(fNumMods>1) *fout << "  Module B: " << modB;
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
	if(fNumMods>1){
	  for(Int_t i=0;i<fNumMods-1;i++){
	    *fout << "Hits found in MDC[" << i << "] window: " 
		  << fHitsFoundInWindow[i] << endl;
	    *fout << "Hits found in MDC[" << i << "] window and unique: " 
		  << fHitsFoundAndUnique[i] << endl;
	    *fout << "Hits discarted in MDC[" << i << "] window: " 
		  << fDiscart[i] << endl;
	  }
	}
	*fout << "Hits found in SHOWER window: "
	      << fHitsFoundInShowerWindow << endl;
	*fout << "Hits found in SHOWER window and unique: "
	      << fHitsFoundInShowerAndUnique << endl;
	for(Int_t i=0;i<8;i++){
	  *fout << "Hits found in TOF[" << i << "] window: "
		<< fHitsFoundInTofWindow[i] << endl;
	  *fout << "Hits found in TOF[" << i << "] window and unique: "
		<< fHitsFoundInTofAndUnique[i] << endl;
	}
	*fout << "Valid hits for alignment: " << fCount << endl;
	*fout << "Valid hits for META alignment: SHOWER->" << fSCount 
	      << "   and TOF->" << fTCount << endl;
      }
      if(MA_DEBUG>0){
	cout << endl << "Sector: " << sector << endl;
	cout << "Module A: " << modA;
	if(fNumMods>1) cout << "  Module B: " << modB;
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
	if(fNumMods>1){
	  for(Int_t i=0;i<fNumMods-1;i++){
	    cout << "Hits found in MDC[" << i << "] window: " 
		 << fHitsFoundInWindow[i] << endl;
	    cout << "Hits found in MDC[" << i << "] window and unique: " 
		 << fHitsFoundAndUnique[i] << endl;
	    cout << "Hits discarted in MDC[" << i << "] window: " 
		 << fDiscart[i] << endl;
	  }    
	}
	cout << "Hits found in SHOWER window: "
	     << fHitsFoundInShowerWindow << endl;
	cout << "Hits found in SHOWER window and unique: "
	     << fHitsFoundInShowerAndUnique << endl;
	for(Int_t i=0;i<8;i++){
	  cout << "Hits found in TOF[" << i << "] window: "
	       << fHitsFoundInTofWindow[i] << endl;
	  cout << "Hits found in TOF[" << i << "] window and unique: "
	       << fHitsFoundInTofAndUnique[i] << endl;
	}
	cout << "Valid hits for alignment: " << fCount << endl;    
	cout << "Valid hits for META alignment: SHOWER->" << fSCount 
	     << "   and TOF->" << fTCount << endl;
      }

      //filling histograms before minimization
      fillHistograms(0);  
      fillMetaHistograms(0);  
      fitMetaHistograms(0);
      fillHistograms(1,1);
      fillMetaHistograms(1,1);  

      if (*fout) *fout << "Valid hits for META alignment after cuts: " 
		       << fCountCut << endl << endl;
      if(MA_DEBUG>0) cout << "Valid hits for META alignment after cuts: " 
			  << fCountCut << endl << endl;

      if (*fout){
	*fout << "Transformation before minimization (init values): "  << endl;
	for(Int_t i=0;i<9;i++){
	  *fout << "Detector " << i << ", "<< fMEuler[i].getX() << ",  " 
		<< fMEuler[i].getY() << ",  " 
		<< fMEuler[i].getZ() << ",  " << fMTranslation[i].getX() << ",  " 
		<< fMTranslation[i].getY() << ",  " << fMTranslation[i].getZ() 
		<< endl;
	}
      }
  
      //minimization strategy
      if(!fMin){ //if  no minimization is performed  
	storeInFile();
	fout->close();
	delete fout;
	return kTRUE; 
      }
  
      if(fMin && fMin!=2 && fMin!=502){ //Minuit
	if (*fout) *fout << "Minimization strategy: " << fMin <<" with MINUIT" << endl;
	HGeomVector OldEuler;
	HGeomVector OldTranslation;
	Int_t IterCounter = 0;
	Float_t IterCri;
    
	for(Int_t i=0;i<9;i++){
	  fDetector = i;
	  do{
	    OldEuler = fMEuler[i];
	    OldTranslation = fMTranslation[i];
	    IterCounter = 0;
	    IterCri = 0;
	    minfit(fFix,fMEuler[i],fMTranslation[i]);
	    if (*fout){
	      *fout << "Parameters after minimization "  << endl;
	      *fout << "Detector" << i <<  ">  "
		    <<fMEuler[i].getX()<<"+-"<<fError[i]<<",  " 
		    <<fMEuler[i].getY()<<"+-"<<fError[i]<<",  " 
		    <<fMEuler[i].getZ()<<"+-"<<fError[i]<<",  " 
		    <<fMTranslation[i].getX()<<"+-"<<fError[i]<<",  " 
		    <<fMTranslation[i].getY()<<"+-"<<fError[i]<<",  " 
		    <<fMTranslation[i].getZ()<<"+-"<<fError[i]<<endl;
	    }	
	    *fout << endl;	
	
	    if(fMEuler[i].getX()!=0) 
	      IterCri += fabs((fMEuler[i].getX()-OldEuler.getX())/
			      fMEuler[i].getX());
	    if(fMEuler[i].getY()!=0) 
	      IterCri += fabs((fMEuler[i].getY()-OldEuler.getY())/
			      fMEuler[i].getY());
	    if(fMEuler[i].getZ()!=0) 
	      IterCri += fabs((fMEuler[i].getZ()-OldEuler.getZ())/
			      fMEuler[i].getZ());
	    if(fMTranslation[i].getX()!=0) 
	      IterCri += fabs((fMTranslation[i].getX()-OldTranslation.getX())/
			      fMTranslation[i].getX());
	    if(fMTranslation[i].getY()!=0) 
	      IterCri += fabs((fMTranslation[i].getY()-OldTranslation.getY())/
			      fMTranslation[i].getY());
	    if(fMTranslation[i].getZ()!=0) 
	      IterCri += fabs((fMTranslation[i].getZ()-OldTranslation.getZ())/
			      fMTranslation[i].getZ());
	
	    cout << "Detector: " << i << "IterCri: " <<   IterCri << endl;	
	    IterCounter++;
	    if(IterCounter>10) {
	      cout << "WARNING in HMdcMetaAligner :: finalize" << endl;
	      cout << "Sector: sector. "  << "Detector: " <<fDetector << endl;
	      cout << "More than 10 iterations without results!" <<endl;
	      break;
	    }
	  }while(IterCri>fIterCriteria);    
	  fillMetaRotMatrix(i,fMEuler[i].getX(),fMEuler[i].getY(),fMEuler[i].getZ());
	}
      }
      if(fMin==2||fMin==3){ //Analytic translation minimization
	HGeomVector result;
	if (*fout)
	  *fout << "Parameters after ANALYTIC translation minimization "  << endl;
    
	if(fUseUniqueRSInTof==kTRUE){    
	  result = translationFinder(8);
	  fillMetaTranslation(8,result.getX(),result.getY(),result.getZ());
	  if (*fout)
	    *fout << "Detector " << 8 << ", "<< fMEuler[8].getX() << ",  " 
		  << fMEuler[8].getY() << ",  " << fMEuler[8].getZ() 
		  << ",  " << fMTranslation[8].getX() << ",  " 
		  << fMTranslation[8].getY() << ",  " 
		  << fMTranslation[8].getZ() << endl;	
	  result = transUniqueFinder();
      
	  fillMetaTranslation(99,result.getX(),result.getY(),result.getZ());
	  if (*fout)
	    *fout << "Detector TOF(Unique): "<< fMEuler[fUniqueTofDet].getX() 
		  << ",  " << fMEuler[fUniqueTofDet].getY() << ",  " 
		  << fMEuler[fUniqueTofDet].getZ() << ",  " 
		  << fMTranslation[fUniqueTofDet].getX() << ",  " 
		  << fMTranslation[fUniqueTofDet].getY() << ",  " 
		  << fMTranslation[fUniqueTofDet].getZ() << endl;	     
	  for(Int_t i=0;i<8;i++){
	    if (*fout)
	      *fout << "Detector " << i << ", "<< fMEuler[i].getX() << ",  " 
		    << fMEuler[i].getY() << ",  " << fMEuler[i].getZ() 
		    << ",  " << fMTranslation[i].getX() << ",  " 
		    << fMTranslation[i].getY() << ",  " 
		    << fMTranslation[i].getZ() << endl;
	  }
	}
	else{
	  for(Int_t i=0;i<9;i++){
	    result = translationFinder(i);
	    fillMetaTranslation(i,result.getX(),result.getY(),result.getZ());
	    if (*fout)
	      *fout << "Detector " << i << ", "<< fMEuler[i].getX() << ",  " 
		    << fMEuler[i].getY() << ",  " << fMEuler[i].getZ() 
		    << ",  " << fMTranslation[i].getX() << ",  " 
		    << fMTranslation[i].getY() << ",  " 
		    << fMTranslation[i].getZ() << endl;
	  }
	}
      }

      if(fMin==502){ //Analytic translation minimization
    
	HGeomVector OldTranslation;
	Int_t IterCounter = 0;
	Float_t IterCri;
    
	HGeomVector result;
	if (*fout)
	  *fout << "Parameters after ITERATIVE ANALYTIC translation minimization "  << endl;
    
	if(fUseUniqueRSInTof==kTRUE){ 
	  do{
	    OldTranslation = fMTranslation[8];
	    IterCri = 0;
	
	    result = translationFinder(8);
	    fillMetaTranslation(8,result.getX(),result.getY(),result.getZ());
	    if (*fout)
	      *fout << "Detector " << 8 << ", "<< fMEuler[8].getX() << ",  " 
		    << fMEuler[8].getY() << ",  " << fMEuler[8].getZ() 
		    << ",  " << fMTranslation[8].getX() << ",  " 
		    << fMTranslation[8].getY() << ",  " 
		    << fMTranslation[8].getZ() << endl;
	
	    if(fMTranslation[8].getX()!=0) 
	      IterCri += fabs((fMTranslation[8].getX()-OldTranslation.getX())/
			      fMTranslation[8].getX());
	    if(fMTranslation[8].getY()!=0) 
	      IterCri += fabs((fMTranslation[8].getY()-OldTranslation.getY())/
			      fMTranslation[8].getY());
	    if(fMTranslation[8].getZ()!=0) 
	      IterCri += fabs((fMTranslation[8].getZ()-OldTranslation.getZ())/
			      fMTranslation[8].getZ());
	
	    cout << "Detector: " << 8 << "IterCri: " <<   IterCri << endl;	
	    IterCounter++;
	    if(IterCounter>100) {
	      cout << "WARNING in HMdcMetaAligner :: finalize" << endl;
	      cout << "Sector: sector. "  << "Detector: " <<fDetector << endl;
	      cout << "More than 100 iterations without results!" <<endl;
	      break;
	    }
	  }while(IterCri>fIterCriteria);
	  IterCounter = 0;
	  do{
	    OldTranslation = fMTranslation[fUniqueTofDet];
	    IterCri = 0;
	
	    result = transUniqueFinder();
	
	    fillMetaTranslation(99,result.getX(),result.getY(),result.getZ());
	    if (*fout)
	      *fout << "Detector TOF(Unique): "<< fMEuler[fUniqueTofDet].getX() 
		    << ",  " << fMEuler[fUniqueTofDet].getY() << ",  " 
		    << fMEuler[fUniqueTofDet].getZ() << ",  " 
		    << fMTranslation[fUniqueTofDet].getX() << ",  " 
		    << fMTranslation[fUniqueTofDet].getY() << ",  " 
		    << fMTranslation[fUniqueTofDet].getZ() << endl;	     
	    for(Int_t i=0;i<8;i++){
	      if (*fout)
		*fout << "Detector " << i << ", "<< fMEuler[i].getX() << ",  " 
		      << fMEuler[i].getY() << ",  " << fMEuler[i].getZ() 
		      << ",  " << fMTranslation[i].getX() << ",  " 
		      << fMTranslation[i].getY() << ",  " 
		      << fMTranslation[i].getZ() << endl;
	    }
	
	    if(fMTranslation[fUniqueTofDet].getX()!=0) 
	      IterCri += fabs((fMTranslation[fUniqueTofDet].getX()-OldTranslation.getX())/
			      fMTranslation[fUniqueTofDet].getX());
	    if(fMTranslation[fUniqueTofDet].getY()!=0) 
	      IterCri += fabs((fMTranslation[fUniqueTofDet].getY()-OldTranslation.getY())/
			      fMTranslation[fUniqueTofDet].getY());
	    if(fMTranslation[fUniqueTofDet].getZ()!=0) 
	      IterCri += fabs((fMTranslation[fUniqueTofDet].getZ()-OldTranslation.getZ())/
			      fMTranslation[fUniqueTofDet].getZ());
	
	    cout << "Detector: " << fUniqueTofDet << "IterCri: " <<   IterCri << endl;	
	    IterCounter++;
	    if(IterCounter>100) {
	      cout << "WARNING in HMdcMetaAligner :: finalize" << endl;
	      cout << "Sector: sector. "  << "Detector: " <<fDetector << endl;
	      cout << "More than 100 iterations without results!" <<endl;
	      break;
	    }
	  }while(IterCri>fIterCriteria);      
	}

	else{
	  for(Int_t i=0;i<9;i++){
	    IterCounter = 0;
	    do{
	      OldTranslation = fMTranslation[i];
	      IterCri = 0;
	      if (*fout) *fout << "Iteration number " << IterCounter << endl;
	      result = translationFinder(i);
	      fillMetaTranslation(i,result.getX(),result.getY(),result.getZ());
	      if (*fout)
		*fout << "Detector " << i << ", "<< fMEuler[i].getX() << ",  " 
		      << fMEuler[i].getY() << ",  " << fMEuler[i].getZ() 
		      << ",  " << fMTranslation[i].getX() << ",  " 
		      << fMTranslation[i].getY() << ",  " 
		      << fMTranslation[i].getZ() << endl;	  
	  
	      if(fMTranslation[i].getX()!=0) 
		IterCri += fabs((fMTranslation[i].getX()-OldTranslation.getX())/
				fMTranslation[i].getX());
	      if(fMTranslation[i].getY()!=0) 
		IterCri += fabs((fMTranslation[i].getY()-OldTranslation.getY())/
				fMTranslation[i].getY());
	      if(fMTranslation[i].getZ()!=0) 
		IterCri += fabs((fMTranslation[i].getZ()-OldTranslation.getZ())/
				fMTranslation[i].getZ());
	  
	      cout << "Detector: " << i << "IterCri: " <<   IterCri << endl;	
	      IterCounter++;
	      if(IterCounter>100) {
		cout << "WARNING in HMdcMetaAligner :: finalize" << endl;
		cout << "Sector: sector. "  << "Detector: " <<fDetector << endl;
		cout << "More than 100 iterations without results!" <<endl;
		break;
	      }
	    }while(IterCri>fIterCriteria);	
	  }
	}
      }

      //Recalculate all histograms for the new parameters!!!
      fillMetaHistograms(2);
      fillMetaHistograms(3,1);
      storeInFile();
  
      // close ascii file
      fout->close();
      delete fout;
  
      return kTRUE; 
    }




  void HMdcMetaAligner::fillMetaHistograms (Int_t Mindex, Int_t select){
    //
    // Performs the graphical output from obtained parameters
    //   

    HMdcHit* hitA;      
    HMdcHit* hitB = NULL;      
    HMdcHit* hitC = NULL;      
    HMdcHit* hitD = NULL;      
    HGeomVector* lHit;
    HGeomVector* lHitB;
    HGeomVector projMP;
    Stat_t entries; 
    Int_t det =10;

    if(select != 0) entries = fAlignMetaCut->GetEntries();
    else entries = fAlignMeta->GetEntries();

    for (Int_t i=0;i<entries;i++) { 
      if(select != 0) fAlignMetaCut->GetEntry(i);  
      else fAlignMeta->GetEntry(i);
      if(MA_DEBUG>0) fHits->print();    
      hitA = (HMdcHit*) fHits->getMdcHitA();
      if(fNumMods>1)hitB = (HMdcHit*) fHits->getMdcHitB();
      if(fNumMods>2) hitC = (HMdcHit*) fHits->getMdcHitC();
      if(fNumMods>3) hitD = (HMdcHit*) fHits->getMdcHitD();
      lHit = fHits->getLocalHitA();
      //if(lHit->getZ()>8)lHitB = fHits->getLocalHitB();
      if(fHits->getDetector()>8)lHitB = fHits->getLocalHitB();

      //Algo va mal con los Int_t de HMdcMetaHit en el TTree!!
      det =  fHits->getDetector();

      //Int_t det =  lHit->getZ();
      if(MA_DEBUG>0) cout << "detector: " << det << endl;

      //Constructing all possible projections
      //The histos represent (value of local hit - value of projected hit)
      //In any case (Meta part)
      if(fNumMods>1){
	if(det<9)
	  projMP = findProjMetaPoint(hitA,hitB,fRotMat[0],
				     fTranslation[0],
				     fMRotMat[det],
				     fMTranslation[det]);
	else  projMP = findProjMetaPoint(hitA,hitB,fRotMat[0],
					 fTranslation[0],
					 fMRotMat[det-9],
					 fMTranslation[det-9]);
      }
      else {      
	if(det<9)
	  projMP = findProjMetaPoint(hitA,fMRotMat[det],
				     fMTranslation[det]);
	else  projMP = findProjMetaPoint(hitA,fMRotMat[det-9],
					 fMTranslation[det-9]);
      }

      if(det<0) cout << "WARNING: det<0 in fHits!" << endl;
      else{
	if(det>7) fResShowerX[Mindex]->Fill(lHit->getX()-projMP.getX());
	if(det>7) fResShowerY[Mindex]->Fill(lHit->getY()-projMP.getY());
	else{
	  //0,...,7 for fMetaHistNum==0
	  //8,...,15 for fMetaHistNum==1 ...
	  fResTofX[(8*Mindex)+det]->Fill(lHit->getX()-projMP.getX());
	  fResTofY[(8*Mindex)+det]->Fill(lHit->getY()-projMP.getY());
	  if(fUseUniqueRSInTof){
	    fUniqueResTofX[Mindex]->Fill(lHit->getX()-projMP.getX());
	    fUniqueResTofY[Mindex]->Fill(lHit->getY()-projMP.getY());	  
	  }	
	}
      }
    }
  }




  void HMdcMetaAligner::fillHistograms (Int_t index, Int_t select){
    //
    // Performs the graphical output from obtained parameters
    //   

    HMdcHit* hitA;      
    HMdcHit* hitB=NULL;      
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
  
    if(select != 0) entries = fAlignMetaCut->GetEntries();
    else entries = fAlignMeta->GetEntries();

    for (Int_t i=0;i<entries;i++) { 
      if(select != 0) fAlignMetaCut->GetEntry(i);  
      else fAlignMeta->GetEntry(i);     
      hitA = (HMdcHit*) fHits->getMdcHitA();
      if(fNumMods>1) hitB = (HMdcHit*) fHits->getMdcHitB();
      if(fNumMods>2) hitC = (HMdcHit*) fHits->getMdcHitC();
      if(fNumMods>3) hitD = (HMdcHit*) fHits->getMdcHitD();

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
      
	if(MA_DEBUG>3){
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
      
	if(MA_DEBUG>3){
	  for(Int_t i=0;i<fNumMods;i++){
	    cout << "transf[" << i <<"]   ";
	    transf[i].print();
	  }
	}

	Float_t a1=0.0, a2=0.0, b1=0.0, b2=0.0;     
	Float_t siga1=0.0, siga2=0.0, sigb1=0.0, sigb2=0.0; 
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
	
	  if(MA_DEBUG>3) cout << "Xwt=" << Xwt << " Xss=" << Xss 
			      << " Xsx=" <<  Xsx << " Xsy=" << Xsy
			      << "Ywt=" << Ywt << " Yss=" << Yss 
			      << " Ysx=" <<  Ysx << " Ysy=" << Ysy << endl;
	}
      
	Xsxoss = Xsx/Xss;    
	Ysxoss = Ysx/Yss;
      
	if(MA_DEBUG>3) cout << "Xsxoss=" << Xsxoss << "  Ysxoss=" 
			    << Ysxoss << endl;
      
	for(Int_t i=0;i<fNumMods;i++){
	  //Plane XZ
	  Xt = (transf[i].getZ()-Xsxoss)/errorx[i];
	  Xst2 += Xt * Xt;
	  b1 += Xt * transf[i].getX()/errorx[i];            
	  //Plane YZ
	  Yt = (transf[i].getZ()-Ysxoss)/errory[i];
	  Yst2 += Yt * Yt;
	  b2 += Yt * transf[i].getY()/errory[i];
	
	  if(MA_DEBUG>3) cout << "Xt=" << Xt << " Xst2=" << Xst2 
			      << " b1 (partial)=" << b1 << "Yt=" << Yt 
			      << " Yst2=" << Yst2 
			      << " b2 (partial)=" << b2 << endl; 
	} 
      
	b1 /= Xst2;
	a1 = (Xsy-(Xsx*b1))/Xss;
	b2 /= Yst2;
	a2 = (Ysy-(Ysx*b2))/Yss;
      
	if(MA_DEBUG>3) cout << "b1=" << b1 << " a1=" << a1
			    << "b2=" << b2 << " a2=" << a2 << endl;
      
	siga1 = sqrt((1.0+Xsx*Xsx/(Xss*Xst2))/Xss);
	sigb1 = sqrt(1.0/Xst2);
	siga2 = sqrt((1.0+Ysx*Ysx/(Yss*Yst2))/Yss);
	sigb2 = sqrt(1.0/Yst2);
      
	if(MA_DEBUG>3) cout << "sigb1=" << sigb1 << " siga1=" << siga1
			    << "sigb2=" << sigb2 << " siga2=" << siga2 << endl;
      
	//Aqui falta evaluar la calidad del ajuste o bien encontrar
	//cuales son los errores que se esperan en los datos para un buen ajuste
	for(Int_t i=0;i<fNumMods;i++){
	  //Plane XZ
	  chipar = (transf[i].getX()-a1-b1*transf[i].getZ())/errorx[i];
	  XChi2 += chipar*chipar;
	  //Plane YZ
	  chipar = (transf[i].getY()-a2-b2*transf[i].getZ())/errory[i];
	  YChi2 += chipar*chipar;
	  if(MA_DEBUG>3) cout << "XChi2=" << XChi2 << " YChi2=" << YChi2 << endl;
	} 
      
	XChi2Hist[index]->Fill(XChi2);
	YChi2Hist[index]->Fill(YChi2);
	TotalChi2[index]->Fill(XChi2+YChi2);

	// Also can be defined by a vector V and a point P:
	// V=(b1,b2,1) P=(a1,a2,0)
	// Let us calculate the square distance from the straigth 
	// line to the points (fit residuals) 
	Float_t part1=0.0,part2=0.0,part3=0.0,sdistance=0.0,sdist=0.0;
	for(Int_t i=0;i<fNumMods;i++){
	  part1 = ( (transf[i].getX()-a1)*b2 ) - ( (transf[i].getY()-a2)*b1 );
	  part2 = ( (transf[i].getY()-a2)    ) - ( (transf[i].getZ()   )*b2 );
	  part3 = ( (transf[i].getZ()   )*b1 ) - ( (transf[i].getX()-a1)    );
	  sdist = (part1*part1 + part2*part2 + part3*part3)/(1+b1*b1+b2*b2);
	  sdistance += sdist;
	  if(MA_DEBUG>3) 
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





  void HMdcMetaAligner::storeInFile(void)
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

      fOutRoot->cd();

      fAlignMeta->Write();
      fAlignMetaCut->Write();

      //  const char* oldDirName = gDirectory->GetPath();
      static Char_t newDirName[255];

      if(fNumMods == 1)  sprintf(newDirName,"%s%s%i%s%i","MetaAligner_",
				 "S_",sector,"_ModA_",modA);
      if(fNumMods == 2)  sprintf(newDirName,"%s%s%i%s%i%s%i","MetaAligner_",
				 "S_",sector,"_ModA_",modA,"_ModB_",modB);
      if(fNumMods == 3)  sprintf(newDirName,"%s%s%i%s%i%s%i%s%i","MetaAligner_",
				 "S_",sector,"_ModA_",modA,"_ModB_",modB,
				 "_ModC_",modC);
      if(fNumMods == 4)  sprintf(newDirName,"%s%s%i%s%i%s%i%s%i%s%i",
				 "MetaAligner_","S_",sector,"_ModA_",modA,
				 "_ModB_",modB,"_ModC_",modC,"_ModD_",modD);

      fOutRoot->cd(newDirName);

      //Enter in the file the HMdcGeomRotation HMdcGeomVector
      if(fNumMods>1){
	for(Int_t i=0;i<fNumMods-1;i++){
	  fRotMat[i].Write();
	  fTranslation[i].Write();
	}
      }
      //This breaks the output
      //  for(Int_t i=0;i<8;i++){
      //    fMRotMat[i].Write();
      //    fMTranslation[i].Write();
      //  }  
      fOutRoot->cd();
      fRecCount--;
      if(!fRecCount) {
	fOutRoot->Write();
	fOutRoot->Close();
      }
    } 





  void HMdcMetaAligner::fillMetaRotMatrix(Int_t ind, Float_t prim,
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
    fMRotMat[ind].setEulerAngles(prim*rad2deg,segu*rad2deg,terc*rad2deg);
  } 





  void HMdcMetaAligner::fillMetaTranslation(Int_t ind,Float_t x,
					    Float_t y, Float_t z){ 
    //
    // Translation from META to MDC 
    //

    if(ind==99){
      //TOF Unique CS (all modules refer to [fUniqueTofDet])
      fMTranslation[fUniqueTofDet].setX(x);
      fMTranslation[fUniqueTofDet].setY(y);
      fMTranslation[fUniqueTofDet].setZ(z); 
      HGeomVector temp;
      for(Int_t det=0;det<8;det++){
	//
	// From 
	// (1) X(TOF4)= fMRotMat[4] X(MDC)+fMTranslation[4]
	// (2) X(TOF#)= fMRotMat[#] X(MDC)+fMTranslation[#]
	// (3) X_TOF4=     fMRotTofRel[i]   * X_TOFi+     fMTransTofRel[i]
	//then
	//     fMTranslation[#] = fMRotTofRel[#].inverse() * 
	//                        (fMTranslation[4]-fMTransTofRel[#] )
	//
	temp = fMRotTofRel[det].inverse() * 
	  (fMTranslation[fUniqueTofDet]-fMTransTofRel[det]);

	//OLD AND WRONG
	//temp = fMTranslation[fUniqueTofDet] + 
	//	fMRotMat[fUniqueTofDet] * fMTransTofRel[det];
	fillMetaTranslation(det,temp.getX(),temp.getY(),temp.getZ());
      }  
    }
    else{
      fMTranslation[ind].setX(x); 
      fMTranslation[ind].setY(y);
      fMTranslation[ind].setZ(z); 
    }
  }





  void HMdcMetaAligner::fillRotMatrix(Int_t ind, Float_t prim,
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





  void HMdcMetaAligner::fillTranslation(Int_t ind,Float_t x,
					Float_t y, Float_t z){ 
    //
    // Translation from MDC A to MDC B
    //
 
    fTranslation[ind].setX(x); 
    fTranslation[ind].setY(y);
    fTranslation[ind].setZ(z); 
  }





  void HMdcMetaAligner::setMetaEulerAngles(Int_t ind,Float_t f,
					   Float_t s, Float_t t){ 
    //
    // Euler angles in transformation MDC B - META
    //
 
    fMEuler[ind].setX(f); 
    fMEuler[ind].setY(s); 
    fMEuler[ind].setZ(t); 
  } 





  void HMdcMetaAligner::setEulerAngles(Int_t ind,Float_t f,
				       Float_t s, Float_t t){ 
    //
    // Euler angles in transformation from MDC A to MDC B
    //
 
    fEuler[ind].setX(f); 
    fEuler[ind].setY(s); 
    fEuler[ind].setZ(t); 
  } 





  void HMdcMetaAligner::setSearchLimits(Float_t x, Float_t y){ 
    //
    // Limits of the square defined in the search procedure
    //
 
    fXArea = x; 
    fYArea = y; 
  } 





  void HMdcMetaAligner::setIterCriteria(Float_t cri){ 
    //
    // Set the criteria for iteration in the minimization (see finalize())
    //
 
    fIterCriteria = cri; 
  }





  void HMdcMetaAligner::setWeights(Float_t w0,Float_t w1,Float_t w2,Float_t w3){ 
    //
    // Set the weights in the fcn()
    //
 
    fWeights[0]= w0; 
    fWeights[1]= w1; 
    fWeights[2]= w2; 
    fWeights[3]= w3; 

  }





  void HMdcMetaAligner::setSteps(Float_t s0,Float_t s1,Float_t s2,
				 Float_t s3, Float_t s4, Float_t s5){
    //
    // Set the steps in the minimization
    //
 
    fSteps[0]= s0; fSteps[1]= s1; fSteps[2]= s2; 
    fSteps[3]= s3; fSteps[4]= s4; fSteps[5]= s5;

    if(MA_DEBUG>3) cout << "Steps in the minimization adjusted to " 
			<< s0 << ", " << s1 << ", " << s2 << ", " 
			<< s3 << ", " << s4 << ", " << s5 << endl;
  }





  void HMdcMetaAligner::setLimits(Float_t l0,Float_t l1,Float_t l2,
				  Float_t l3, Float_t l4, Float_t l5){
    //
    // Set the criteria for iteration in the minimization (see finalize())
    //
 
    fLimits[0]= l0; fLimits[1]= l1; fLimits[2]= l2; 
    fLimits[3]= l3; fLimits[4]= l4; fLimits[5]= l5; 

    if(MA_DEBUG>3) cout << "Limits in the minimization adjusted to " 
			<< l0 << ", " << l1 << ", " << l2 << ", " 
			<< l3 << ", " << l4 << ", " << l5 << endl;
  }





  HGeomVector HMdcMetaAligner::translationFinder(Int_t module){
    //
    // Analytical minimization of the function
    // X and Y residuals
    // Results are the relative translation 
    // vector components
    // (WARNING: Use it here just with one MDC and target)
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
    HMdcHit* hitB=NULL;
    HMdcHit* hitC;      
    HMdcHit* hitD;
    HGeomVector* lHit;
    //HGeomVector* lHitB;
    HGeomVector projPoint;
    HGeomRotation rotaux;
    HGeomVector transaux;
    HGeomVector result;  
    Float_t a=0,b=0,c=0,d=0,e=0,f=0,g=0,h=0;
    Float_t thetaX=0,thetaY=0;
    Float_t resX=0,resY=0,resXstar=0,resYstar=0;
    Float_t Wx=1,Wy=1;
    Int_t det;
    Stat_t entries;
    Float_t s0=0,s1=0;

    if(fUseCut) entries= fAlignMetaCut->GetEntries();
    else  entries= fAlignMeta->GetEntries();
  
    //projecting on MDC A
    if(module<9){
      rotaux = fMRotMat[module];
      transaux = fMTranslation[module];
    }

    //
    for (Int_t i=0;i<entries;i++){
      if(fUseCut) fAlignMetaCut->GetEntry(i);
      else fAlignMeta->GetEntry(i);
      lHit = (HGeomVector*) fHits->getLocalHitA();
      //if(lHit->getZ()>8)lHitB = fHits->getLocalHitB();
      //if(fHits->getDetector()>8)lHitB = (HGeomVector*) fHits->getLocalHitB();
      det = fHits->getDetector();

      if ( (det==module) ){
      
	hitA = (HMdcHit*) fHits->getMdcHitA();
	if(fNumMods>1) hitB = (HMdcHit*) fHits->getMdcHitB();
	if(fNumMods>2) hitC = (HMdcHit*) fHits->getMdcHitC();
	if(fNumMods>3) hitD = (HMdcHit*) fHits->getMdcHitD();
	//lHit = fHits->getLocalHitA();
	//if(lHit->getZ()>8)lHitB = fHits->getLocalHitB();
	//if(fHits->getDetector()>8)lHitB = fHits->getLocalHitB();
      
	if(fNumMods==1){
	  projPoint = findProjMetaPoint(hitA,rotaux,transaux);
	  resX = lHit->getX() - projPoint.getX();
	  resY = lHit->getY() - projPoint.getY();
	
	  if(fUseTarget) {
	    s0 = (hitA->getX()-fTargetPos.getX())/(-fTargetPos.getZ());
	    s1 = (hitA->getY()-fTargetPos.getY())/(-fTargetPos.getZ());
	  }
	  else{
	    Float_t xDir = hitA->getXDir(); 
	    Float_t yDir = hitA->getYDir();
	    //if using hit!
	    Float_t aux = sqrt(1 - xDir * xDir - yDir * yDir);    
	    if(aux == 0.){ s0=1; s1=1;} //non-sense values
	    else{
	      s0 = xDir/aux;
	      s1 = yDir/aux;
	    }    
	  }
	}
	else if(fNumMods==2){
	  projPoint = findProjMetaPoint(hitA,hitB,
					fRotMat[0],fTranslation[0],
					rotaux,transaux);
	  resX = lHit->getX() - projPoint.getX();
	  resY = lHit->getY() - projPoint.getY();
	  HMdcHit* hitAB = mergeHits(hitB,hitA,fRotMat[0],fTranslation[0]);
	  Float_t xDir = hitAB->getXDir(); 
	  Float_t yDir = hitAB->getYDir();
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
	  a+=1/Wx;
	  b+=1/Wy;
	  c+=-thetaX/Wx;
	  d+=-thetaY/Wy;
	  e+=thetaX*thetaX/Wx+thetaY*thetaY/Wy;
	  f+=resXstar/Wx;
	  g+=resYstar/Wy;
	  h+=-(thetaX*resXstar/Wx+thetaY*resYstar/Wy);
	}
      }
    }
  
    Float_t den = a*b*e-c*b*c-a*d*d;
    result.setX((f*b*e+g*d*c-b*c*h-f*d*d)/den);
    result.setY((a*g*e+f*d*c-g*c*c-d*h*a)/den);
    result.setZ((a*b*h-f*b*c-g*d*a)/den);
  
    return result;
  
  }





  HGeomVector HMdcMetaAligner::transUniqueFinder(void){
    //
    // Analytical minimization of the function
    // X and Y residuals fot Unique Reference System in TOF
    // Results are the relative translation 
    // vector components for Reference TOF System
    // (WARNING: Use it here just with one MDC and target)
    //
    // Matrix:
    //
    //    a b c      translation[0]     j
    //    b e f   *  translation[1]  =  k
    //    c f i      translation[2]     l
    //
    // results in
    //
    // translation[0] = (jei+bfl+kfc-cel-bki-ffj)/(aei+2fcb-cce-bbi-ffa)
    // translation[1] = (aki+jfc+cbl-cck-jbi-lfa)/(aei+2fcb-cce-bbi-ffa)
    // translation[2] = (ael+bkc+fbj-jec-bbl-kfa)/(aei+2fcb-cce-bbi-ffa)
    //
  
    HMdcHit* hitA;      
    HMdcHit* hitB;
    HMdcHit* hitC;      
    HMdcHit* hitD;
    HGeomVector* lHit;
    //HGeomVector* lHitB;
    HGeomVector projPoint;
    HGeomRotation rotaux;
    HGeomVector transaux;
    HGeomVector result;  
    Float_t a=0,b=0,c=0,e=0,f=0,ip=0,j=0,k=0,l=0;
    Float_t thetaX=0,thetaY=0;
    Float_t resX=0,resY=0,resXstar=0,resYstar=0;
    Float_t Wx=1,Wy=1;
    Float_t AX=0,AY=0,BX=0,BY=0,CX=0,CY=0;
    Int_t det;
    Stat_t entries;

    if(fUseCut) entries= fAlignMetaCut->GetEntries();
    else  entries= fAlignMeta->GetEntries();
  
    for (Int_t i=0;i<entries;i++){
      if(fUseCut) fAlignMetaCut->GetEntry(i);
      else fAlignMeta->GetEntry(i);
      lHit = (HGeomVector*) fHits->getLocalHitA();
      //if(lHit->getZ()>8)lHitB = fHits->getLocalHitB();
      //if(fHits->getDetector()>8)lHitB = (HGeomVector*) fHits->getLocalHitB();
      det = fHits->getDetector();

      if ( (det>0) && (det<8) ){
      
	hitA = (HMdcHit*) fHits->getMdcHitA();
	if(fNumMods>1) hitB = (HMdcHit*) fHits->getMdcHitB();
	if(fNumMods>2) hitC = (HMdcHit*) fHits->getMdcHitC();
	if(fNumMods>3) hitD = (HMdcHit*) fHits->getMdcHitD();
	//lHit = fHits->getLocalHitA();
	//if(lHit->getZ()>8)lHitB = fHits->getLocalHitB();
	//if(fHits->getDetector()>8)lHitB = fHits->getLocalHitB();
      
	rotaux = fMRotMat[det];
	transaux = fMTranslation[det];
      
	projPoint = findProjMetaPoint(hitA,rotaux,transaux);
      
	resX = lHit->getX() - projPoint.getX();
	resY = lHit->getY() - projPoint.getY();
      
	Float_t s0=0,s1=0;
	if(fUseTarget) {
	  s0 = (hitA->getX()-fTargetPos.getX())/(-fTargetPos.getZ());
	  s1 = (hitA->getY()-fTargetPos.getY())/(-fTargetPos.getZ());
	}
	else{
	  Float_t xDir = hitA->getXDir(); 
	  Float_t yDir = hitA->getYDir();
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
      
	//      cout << "thetaX: " << thetaX << " thetaY: " << thetaY << endl;
      
	//instead of invert the matrix, just the inverse elements are taken
	AX = thetaX*fMRotTofRel[det](2) - fMRotTofRel[det](0);
	BX = thetaX*fMRotTofRel[det](5) - fMRotTofRel[det](3);
	CX = thetaX*fMRotTofRel[det](8) - fMRotTofRel[det](6);
	AY = thetaY*fMRotTofRel[det](2) - fMRotTofRel[det](1);
	BY = thetaY*fMRotTofRel[det](5) - fMRotTofRel[det](4);
	CY = thetaY*fMRotTofRel[det](8) - fMRotTofRel[det](7);
      
	//cout << "AX: " << AX << " BX: " << BX << " CX: " << CX<< endl;
	//cout << "AY: " << AY << " BY: " << BY << " CY: " << CY<< endl;
      
	resXstar = resX - (fMTranslation[fUniqueTofDet].getX() * AX + 
			   fMTranslation[fUniqueTofDet].getY() * BX +
			   fMTranslation[fUniqueTofDet].getZ() * CX );
	resYstar = resY - (fMTranslation[fUniqueTofDet].getX() * AY + 
			   fMTranslation[fUniqueTofDet].getY() * BY +
			   fMTranslation[fUniqueTofDet].getZ() * CY );
      
	//cout << " resXstar: " << resXstar << " resYstar: " << resYstar;
      
	if(fUseUnitErrors){
	  a+= (AX*AX) + (AY*AY);
	  b+= (AX*BX) + (AY*BY);
	  c+= (AX*CX) + (AY*CY);
	  e+= (BX*BX) + (BY*BY);
	  f+= (BX*CX) + (BY*CY);
	  ip+= (CX*CX) + (CY*CY);
	  j+= (-resXstar*AX) + (-resYstar*AY);
	  k+= (-resXstar*BX) + (-resYstar*BY);
	  l+= (-resXstar*CX) + (-resYstar*CY);
	}
	else{      
	  a+= (AX*AX)/Wx + (AY*AY)/Wy;
	  b+= (AX*BX)/Wx + (AY*BY)/Wy;
	  c+= (AX*CX)/Wx + (AY*CY)/Wy;
	  e+= (BX*BX)/Wx + (BY*BY)/Wy;
	  f+= (BX*CX)/Wx + (BY*CY)/Wy;
	  ip+= (CX*CX)/Wx + (CY*CY)/Wy;
	  j+= (-resXstar*AX)/Wx + (-resYstar*AY)/Wy;
	  k+= (-resXstar*BX)/Wx + (-resYstar*BY)/Wy;
	  l+= (-resXstar*CX)/Wx + (-resYstar*CY)/Wy;
	}
      }
    }
    Float_t den = a*e*ip+2*f*c*b-c*c*e-b*b*ip-f*f*a;
    result.setX((j*e*ip+b*f*l+k*f*c-c*e*l-b*k*ip-f*f*j)/den);
    result.setY((a*k*ip+j*f*c+c*b*l-c*c*k-j*b*ip-l*f*a)/den);
    result.setZ((a*e*l+b*k*c+f*b*j-j*e*c-b*b*l-k*f*a)/den);

    //cout << "den=" << den <<endl;
    //result.print();  

    return result;
  
    //quiza luego pueda hacer tambien el inverso,
    //esto es, proyectar en MDC B y comprobar que parametros
    //salen de la minimizacion analitica.
  }





  void HMdcMetaAligner::minfit(Int_t fix, HGeomVector fE, HGeomVector fT){ 
    //
    // Minuit menu
    // Put first argument to 1 to fix angular values   
    // Put first argument to 2 to fix translation values   
    // Other arguments are init values for the parameters!
    //

    Double_t args[8]; 
    Int_t err = 0; 
    Float_t* limitL;
    Float_t* limitH;
    limitL = new Float_t[6];
    limitH = new Float_t[6];
 
    //This depends on MDCA and MDCB 
    //DANGEROUS! Can read memory out of scope
    Double_t start_val[]={fE.getX(),fE.getY(),fE.getZ(),
			  fT.getX(),fT.getY(),fT.getZ()};
  
    //seting limits
    for(int i=0;i<6;i++){
      if(fLimits[i]==0){
	limitL[i]=0;
	limitH[i]=0;
      }
      else { 
	limitL[i]=start_val[i]-fLimits[i];      
	limitH[i]=start_val[i]+fLimits[i];
	cout << " LIMITS IN THE MINIMIZATION " << endl;
	cout << " (from 0 to 5) Par " << i << " between " 
	     << limitL[i] << " and " << limitH[i] << endl;
      }
    }
  
    TMinuit *minuit=new TMinuit(6);
   
    //setting the object to minimize
    minuit->SetFCN(fcnMA); 
    minuit->SetObjectFit(this);
  
    if(MA_DEBUG>0){
      cout << "HMdcMetaAligner::minfit()" <<endl;    
      cout << "Start Values for initialization: "; 
      for(Int_t i=0;i<6;i++){
	cout << start_val[i] << "," ;
      }
      cout << endl;
    }

    Char_t pname[10];  
    for(Int_t i=0;i<6;i++){
      sprintf(pname,"%s%i","par",i);
      minuit->mnparm(i,pname,start_val[i],fSteps[i],limitL[i],limitH[i],err); 
    }

    //FIXING 
    if(fMin==4||fix == 30) {    //fixing angles
      minuit->FixParameter(0);     
      minuit->FixParameter(1); 
      minuit->FixParameter(2);
    }
    if(fMin==1||fMin==3||fix == 20) {    //fixing translations
      minuit->FixParameter(3);     
      minuit->FixParameter(4); 
      minuit->FixParameter(5); 
    }

    if(fix==40){    //when cut in slopes, fixing the Z translation
      minuit->FixParameter(5); 
    }
    if(fix>0 && fix<6){
      minuit->FixParameter(fix+1); 
    } 
    if(fix==110){    
      minuit->FixParameter(0);       
      minuit->FixParameter(2); 
    }
 
    //To remove all printout
    if(MA_DEBUG<3){ 
      minuit->SetPrintLevel(-1);
    }

    //args is the array of the numerical arguments of the command 
    //the third field is the number of arguments esp3cified 
    //For MIGRAD arguments are maxcalls and tolerance, both optionals 
    minuit->mnexcm("MIGRAD",args,0,err); 
    //minuit->mnexcm("SIMPLEX",args,0,err); 
  
    Double_t parresult[6];
    for(Int_t i=0;i<6;i++){
      minuit->GetParameter(i,parresult[i],fError[i]);
    }
  
    fMEuler[fDetector].setX(parresult[0]);
    fMEuler[fDetector].setY(parresult[1]);
    fMEuler[fDetector].setZ(parresult[2]);
    fMTranslation[fDetector].setX(parresult[3]);
    fMTranslation[fDetector].setY(parresult[4]);
    fMTranslation[fDetector].setZ(parresult[5]);

    if (err != 0) cout << endl <<"MINIMIZATION EXITED WITH ERROR " 
		       << err << endl;
  }





  void fcnMA(Int_t &npar, Double_t *gin, Double_t &f, 
	     Double_t *par, Int_t iflag){ 
    //
    // This function contains the functional to minimize
    //

    Double_t chisq = 0.;   
    HGeomRotation rotM;
    HGeomRotation rot;
    HGeomVector tra;    
    HGeomVector a,b,anew,ainmeta,binmeta;
    HGeomVector point,direct;  
    HGeomVector pointInMeta,directInMeta;

    Double_t prim, segu, terc;  
    Float_t xproj,yproj,consta;
    HGeomVector* vec;
    Int_t ldet;
    Float_t third;
    Int_t detector =-10;

    prim = par[0]; 
    segu = par[1]; 
    terc = par[2];   
    HGeomVector traM(par[3],par[4],par[5]);

    if (MA_DEBUG>2){
      cout << "HMdcMetaAligner::fcnalign()   Parameters: " << par[0] 
	   << "," << par[1] << "," <<  par[2] << "," << par[3] 
	   << "," << par[4] << ","  << par[5] << endl;
    }
 
    rotM.setEulerAngles(prim*180./TMath::Pi(),
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

    Int_t numMods;
    TTree* pData; 
    HMdcMetaAligner* pAlign;  
    HMdcMetaHit* theHits;
    pAlign = (HMdcMetaAligner*)(gMinuit->GetObjectFit());
    pData = pAlign->getTree();  
    numMods = pAlign->getNumMods();
    if(numMods>1){ 
      rot = pAlign->getRotMatrix();
      tra = pAlign->getTranslation();
    }
    detector = pAlign->getDetector();
    //  theHits = pAlign->getHits();
    Stat_t entries = pData->GetEntries();


    Float_t* weights;
    weights = new Float_t[4];
    weights = pAlign->getWeights();

    HMdcHit* hitA;
    HMdcHit* hitB=NULL;
  
    //loop on hits   
    for (Int_t i=0;i<entries;i++) {     
      pData->GetEntry(i);    
      theHits = pAlign->getHits();
      //data = pData->GetArgs();
      //filling from the Tree 
    
      vec = (HGeomVector*) theHits->getLocalHitA();
      //ldet = (Int_t) vec->getZ();
      //CUando arregle el problema con el tree de HMdcMetaHit, cambiar 
      ldet = (Int_t) theHits->getDetector();

      if(ldet==detector||(ldet-9)==detector) {
	hitA = (HMdcHit*) theHits->getMdcHitA();
	if(numMods>1) hitB = (HMdcHit*) theHits->getMdcHitB();    
	if(ldet>8) vec =  (HGeomVector*) theHits->getLocalHitB();  

	if(numMods>1){
	  //filling from the ntuple
	  a.setX(hitA->getX());              //getting the hit info     
	  a.setY(hitA->getY());
	  a.setZ(0.);
	  b.setX(hitB->getX());              //getting the hit info     
	  b.setY(hitB->getY());
	  b.setZ(0.);
	
	  anew = rot * a + tra;
	  ainmeta = rotM * anew + traM;
	  binmeta = rotM * b + traM;
	
	  //straigth line from both hits and cross point with plane ZMETA=0
	  //    x-aM.getX()            y-aM.getY()           z-aM.getZ()
	  // ------------------- = ------------------- = -------------------
	  // bM.getX()-aM.getX()   bM.getY()-aM.getY()   bM.getZ()-aM.getZ()  
	  //
	  // then for z=0:  
	  // x=aM.getX()-aM.getZ()*(bM.getX()-aM.getX())/(bM.getZ()-aM.getZ())
	  // y=aM.getY()-aM.getZ()*(bM.getY()-aM.getY())/(bM.getZ()-aM.getZ())
	  //
	  consta = ainmeta.getZ()/(binmeta.getZ()-ainmeta.getZ());
	  xproj = ainmeta.getX()-(binmeta.getX()-ainmeta.getX())*consta;
	  yproj = ainmeta.getY()-(binmeta.getY()-ainmeta.getY())*consta;
	
	  //Some extra debbuging (always needed ;-) )
	  if(MA_DEBUG >3){
	    cout << "  ++++++++  VALUES IN fcnMA()  +++++++++ " << endl;
	    cout << "HITA: " ;
	    a.print();
	    cout << "HITB: " ;
	    b.print();
	    cout << "HITA IN MDC B CS: ";
	    anew.print();
	    cout << "HITA IN META CS: ";
	    ainmeta.print();
	    cout << "HITB IN META CS: ";
	    binmeta.print();      
	    cout << "Dif X: " << xproj-(vec->getX())  
		 << " Dif Y: " << yproj-(vec->getY())<< endl;
	  }
	}
	else{ //1MDC
	  third = sqrt(1 - hitA->getXDir()*hitA->getXDir() - 
		       hitA->getYDir()*hitA->getYDir());
	
	  point.setX(hitA->getX());              //getting the hit info     
	  point.setY(hitA->getY());
	  point.setZ(0.);
	
	  direct.setX(hitA->getXDir()); 
	  direct.setY(hitA->getYDir()); 
	  direct.setZ(third); 
	
	  //transfor to the new system
	  pointInMeta = rotM * point + traM;
	  directInMeta = rotM * direct;
	
	  //    x-pM.getX()            y-pM.getY()           -pM.getZ()
	  // ------------------- = ------------------- = -------------------
	  //  direcInMeta.getX()   directInMeta.getY()   directInMeta.getZ()  
	
	  //
	
	  xproj = pointInMeta.getX()-
	    pointInMeta.getZ()*directInMeta.getX()/directInMeta.getZ();
	  yproj = pointInMeta.getY()-
	    pointInMeta.getZ()*directInMeta.getY()/directInMeta.getZ();
	}

	chisq += (xproj-(vec->getX()))*(xproj-(vec->getX()))/weights[0] + 
	  (yproj-(vec->getY()))*(yproj-(vec->getY()))/weights[1];

      }  //end of  if(...
    }  //end of  for (Int_t i=0;...
    f = chisq; 
    if(MA_DEBUG>2){
      cout  << "chisqr= " << chisq << " out of " << entries 
	    << " combinations."<< endl; 
    }
  } 








  
