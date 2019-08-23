using namespace std;
# include <math.h>
# include <stdlib.h>
# include <iostream>
# include <iomanip> 
# include <fstream> 
# include <TMath.h>
# include <TROOT.h>
# include <TF1.h>
# include "hmdcgeomparf.h"
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
# include "hparrootfileio.h"
# include "mdcsdef.h"

ClassImp(HMdcGeomParF)  

//*-- AUTHOR : Hector Alvarez-Pol  
//*-- Date: 03/2001
//*-- Last Update: 30/03/01
//*-- Copyright: GENP (Univ. Santiago de Compostela)
  
////////////////////////////////////////////////////////////////////////
//
// HMdcGeomParF    (ongoing work)
//
// Performs the MDC software alignment and writes a modified 
// HMdcGeomPar in the root file.
//
// The execute() function makes nothing. All functionality is 
// implemented in the finalize() function, where it takes the  
// TTree results from HMdcAligner. Performs calls to TMinuit
// trying to minimize the distance to Z axis. For the obtained
// parameters, makes the control histograms and returns ascii
// file and new geometrical parameters container.
//
////////////////////////////////////////////////////////////////////////

   Int_t GP_DEBUG=0;

HMdcGeomParF :: HMdcGeomParF(void) : HReconstructor()
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





HMdcGeomParF :: HMdcGeomParF(Int_t sector, Int_t modA, Int_t modB) 
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





HMdcGeomParF :: HMdcGeomParF(Text_t* name, Text_t* title, Int_t sector, 
			     Int_t modA, Int_t modB, 
			     Int_t modC, Int_t modD)
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





HMdcGeomParF::HMdcGeomParF(Text_t* name, Text_t* title, 
			 Int_t sector, Int_t modA)
  : HReconstructor(name, title)
{  
  //
  // Constructor for only one MDC
  //
  if(sector==6){ //finding target for all MDCs in a frustum 
    fHits=new TClonesArray("HMdcHit",1); 
    fLoc.setNIndex(3);
    fLoc.set(3,sector,modA,0);  //the last one is a dummy
    fNumMods = 0; 
    initDefaults();
  }
  else{
    fHits=new TClonesArray("HMdcHit",1); 
    fLoc.setNIndex(3);
    fLoc.set(3,sector,modA,0);  //the last one is a dummy
    fNumMods = 1; 
    initDefaults();
  }
}





void HMdcGeomParF::initDefaults(void)
{
  //
  // Inits common defaults
  //

  if(fNumMods>1){
    fEulerResult = new HGeomVector[fNumMods-1];
    fTransResult = new HGeomVector[fNumMods-1];
    
    fRotMat = new HGeomRotation[fNumMods-1];
    fTranslation = new HGeomVector[fNumMods-1];
    fEuler = new HGeomVector[fNumMods-1];  
    fError = new Double_t[(fNumMods-1)*6];
  } 
  else if(fNumMods==1){
    fRotMat = new HGeomRotation[1];
    fTranslation = new HGeomVector[1];    
    fEuler = new HGeomVector[1];  
  } 
  else {////finding target for all MDCs in a frustum 
    fRotMat = new HGeomRotation[6];
    fTranslation = new HGeomVector[6];    
    fEuler = new HGeomVector[6];  
  }
  fManual = kFALSE;
  fHistoOff = kFALSE;  
  fUseCut = kTRUE;
  fUseYPosCut = kFALSE;
  fUseInitValue = kFALSE;
  fUseUnitErrors = kFALSE;
  fUseAngleCut = kFALSE;  //1MDC fit
  fTFFor2Mods = kFALSE;   //2MDCs targetFinder call
  fAngleCut = 0.5;       //for MDCII 0.5 means 350mm from Phys. Center free
  fConstTukey = 10; 
  fStepTukey = 1;
  fResultTukey = 1;
  fEndTukey = 0.4;

  fMin = 1; 
  fHistNum = 2;
  fZLoCut = -100;  
  fZHiCut = 100;
  fTree = NULL;  
  fOutFile = NULL;    
  fFix = 0;
  initMinimization();
}





HMdcGeomParF :: ~HMdcGeomParF(void)
{    
  //
  // Destructor. Makes nothing
  //

  if(fRotMat) delete[] fRotMat;
  if(fTranslation) delete[] fTranslation;
  if(fEuler) delete[] fEuler; 
  if(fHits) delete fHits;
}





void HMdcGeomParF::initMinimization(void){
  fIterCriteria = 0.1;   
  fSteps = new Float_t[6];
  fLimits = new Float_t[6];
  setSteps(0.01,0.01,0.01,0.1,0.1,0.1);
  setLimits(0.,0.,0.,0.,0.,0.);
}





void HMdcGeomParF::setRelParams(HGeomVector*& ang, HGeomVector*& tra)
  //				HGeomVector*& ang2,HGeomVector*& tra2,
  //				HGeomVector*& ang3,HGeomVector*& tra3)
{    
  //
  // Sets the pointers to previous results of HMdcAligner.
  // This function should be called in the macro
  //
  /*
  if(fNumMods>3) {
    fEulerResult[2] = ang3;        //MDC A - MDC D
    fTransResult[2] = tra3;
    fEulerResult[1] = ang2;        //MDC B - MDC D
    fTransResult[1] = tra2;    
    fEulerResult[0] = ang;         //MDC C - MDC D
    fTransResult[0] = tra;
  }
  else if(fNumMods>2) {
    fEulerResult[1] = ang2;        //MDC A - MDC C
    fTransResult[1] = tra2;    
    fEulerResult[0] = ang;         //MDC B - MDC C
    fTransResult[0] = tra;
  }
  else {
    fEulerResult[0] = ang;         //MDC A - MDC B
    fTransResult[0] = tra;    
  }
  */

  fEulerResult = ang;
  fTransResult = tra;

  /*
  cout << "fEulerResult " << fEulerResult << " fTransResult " << fTransResult << endl; 
  cout << "ang " << ang << " tra " << tra << endl; 
  */
}





void HMdcGeomParF::setOutputAscii(TString name)
{
  //
  // Sets ascii output for results and debugging.
  //

  fNameAscii=name;
}





void HMdcGeomParF::setOutputRoot(TString name)
{
  //
  // Sets Root output
  //

  fNameRoot=name;
}





void HMdcGeomParF::setTukeyConstant(Float_t cte, Float_t step, Float_t res, Float_t end)
{
  //
  // Sets the Tukey constant in the weigth determination
  // for 1 MDC target finder
  // The function arguments are: 
  //   cte   the initial Tukey constant
  //   step  the step between tukey calculations
  //   res   the tukey value for which the calculations on the output file are made 
  //   end   the last accepted Tukey value
  //
  fConstTukey = cte;  
  fStepTukey = step;
  fResultTukey = res;
  fEndTukey = end;
}





void HMdcGeomParF::setInitValue(Float_t x, Float_t y, Float_t z)
{
  //
  // Sets the init value
  // for 1MDC target determination
  //
  fUseInitValue = kTRUE;
  fInitValue.setX(x);
  fInitValue.setY(y);
  fInitValue.setZ(z);
}





void HMdcGeomParF::setTargetFinderFor2Mods(void)
{
  //
  // Sets targetFinder call for two modules
  //
  fTFFor2Mods = kTRUE;
}





void HMdcGeomParF :: setHistograms(void)
{
  //
  // Inits histograms
  //

  Int_t sector = fLoc[0];
  Int_t modA = fLoc[1];
  Int_t modB = fLoc[2];
  Int_t modC = -1;
  Int_t modD = -1;

  static Char_t newDirName[255];
  if(fNumMods>3) {
    modC = fLoc[3];
    modD = fLoc[4];
    sprintf(newDirName,"%s%s%i%s%i%s%i%s%i%s%i","GeomParF_","S_",sector,
	    "_ModA_",modA,"_ModB_",modB,"_ModC_",modC,"_ModD_",modD);
  }
  else if(fNumMods>2) {
    modC = fLoc[3];    
    sprintf(newDirName,"%s%s%i%s%i%s%i%s%i","GeomParF_",
	    "S_",sector,"_ModA_",modA,"_ModB_",modB,"_ModC_",modC);
  }
  else if(fNumMods>1) sprintf(newDirName,"%s%s%i%s%i%s%i","GeomParF_",
			      "S_",sector,"_ModA_",modA,"_ModB_",modB);
  
  else if(fNumMods==1) sprintf(newDirName,"%s%s%i%s%i","GeomParF_",
	       "S_",sector,"_ModA_",modA);

  else sprintf(newDirName,"%s%s%i","GeomParF_","_Mods_",modA);

  fOutFile->mkdir(newDirName,newDirName);
  fOutFile->cd(newDirName);
  
  fPlanex = new TH2F*[fHistNum];
  fPlaney = new TH2F*[fHistNum];
  fPlanez = new TH2F*[fHistNum];
  fProzY = new TH1F*[fHistNum];
  fProzX = new TH1F*[fHistNum];
  fProyZ = new TH1F*[fHistNum];
  fProyX =  new TH1F*[fHistNum];
  fProxZ = new TH1F*[fHistNum];
  fProxY = new TH1F*[fHistNum];
  fTargets = new TH1F*[fHistNum];
  fTarTuk = new TH1F*[4];
  fTarTukW = new TH1F*[4];
  fDisCTuk = new TH3F*[4];
  if(fNumMods==0) {
    fTarMDC = new TH1F*[fHistNum*6];
    fTarCut = new TH1F*[fHistNum];   
    fTarMDCCut = new TH1F*[fHistNum*6];
  }
  fZVsTheta = new TH2F*[fHistNum];
  fZVsRho = new TH2F*[fHistNum];
  fDisZ = new TH1F*[fHistNum];
  fDisCenter = new TH1F*[fHistNum];

  Char_t title[50], tmp[50];

  for(Int_t index=0;index<fHistNum;index++){
    if(fNumMods>3) {
      modC = fLoc[3];
      modD = fLoc[4];
      sprintf(title,"%s%i%s%i%s%i%s%i%s%i%s%i","_",index,"_Sector_",sector,
	      "_ModA_",modA,"_ModB_",modB,"_ModC_",modC,"_ModD_",modD);
    }
    else if(fNumMods>2){
      modC = fLoc[3];    
      sprintf(title,"%s%i%s%i%s%i%s%i%s%i","_",index,"_Sector_",sector,
	      "_ModA_",modA,"_ModB_",modB,"_ModC_",modC);
    }
    else if(fNumMods>1) sprintf(title,"%s%i%s%i%s%i%s%i","_",index,"_Sector_",
				sector,"_ModA_",modA,"_ModB_",modB);

    else  if(fNumMods==1) sprintf(title,"%s%i%s%i%s%i","_",index,"_Sector_",
				sector,"_ModA_",modA);
    
    else  sprintf(title,"%s%i%s%i","_",index,"_Mods_",modA);
    
    sprintf(tmp,"%s%s","fPlanex",title);
    fPlanex[index] = new TH2F(tmp,title,100,-1000,1000,100,-1000,1000);  
    sprintf(tmp,"%s%s","fPlaney",title);
    fPlaney[index] = new TH2F(tmp,title,100,-1000,1000,100,-1000,1000);   
    sprintf(tmp,"%s%s","fPlanez",title);
    fPlanez[index] = new TH2F(tmp,title,100,-1000,1000,100,-1000,1000);
    
    sprintf(tmp,"%s%s","fTargets",title);
    fTargets[index] = new TH1F(tmp,title,2000,-1000,1000);     
    sprintf(tmp,"%s%s","fZVsTheta",title);
    fZVsTheta[index] = new TH2F(tmp,title,200,0,3.1,200,-1000,1000);  
    sprintf(tmp,"%s%s","fZVsRho",title);
    fZVsRho[index] = new TH2F(tmp,title,200,-100,100,200,-1000,1000);       
    
    sprintf(tmp,"%s%s","fDisZ",title);
    fDisZ[index] = new TH1F(tmp,title,200,-100,100);  
    sprintf(tmp,"%s%s","fDisCenter",title);
    fDisCenter[index] = new TH1F(tmp,title,200,0,1000);     
    
    sprintf(tmp,"%s%s","fProzY",title);
    fProzY[index] = new TH1F(tmp,title,200,-1000,1000);
    sprintf(tmp,"%s%s","fProzX",title);
    fProzX[index] = new TH1F(tmp,title,200,-1000,1000);
    sprintf(tmp,"%s%s","fProyZ",title);
    fProyZ[index] = new TH1F(tmp,title,100,-500,500);
    sprintf(tmp,"%s%s","fProyX",title);
    fProyX[index] = new TH1F(tmp,title,100,-500,500);
    sprintf(tmp,"%s%s","fProxZ",title);
    fProxZ[index] = new TH1F(tmp,title,100,-500,500);
    sprintf(tmp,"%s%s","fProxY",title);
    fProxY[index] = new TH1F(tmp,title,100,-200,200);

    if(fNumMods==0){    
      sprintf(tmp,"%s%s","fTarCut",title);
      fTarCut[index] = new TH1F(tmp,title,2000,-1000,1000);     

      for(Int_t i=0;i<6;i++){
	sprintf(tmp,"%s%i%s%s","fTarMDC_",i,"_",title);
	fTarMDC[6*index+i] = new TH1F(tmp,title,2000,-1000,1000);
	sprintf(tmp,"%s%i%s%s","fTarMDCCut_",i,"_",title);
	fTarMDCCut[6*index+i] = new TH1F(tmp,title,2000,-1000,1000);
      }
    }
  }
  //fOutFile->cd(oldDirName);

  for(Int_t index=0;index<4;index++){
    	sprintf(tmp,"%s%i","fTarTuk_Tk_",index+1);
	fTarTuk[index] = new TH1F(tmp,title,1000,-100,100);
    	sprintf(tmp,"%s%i","fTarTukW_Tk_",index+1);
	fTarTukW[index] = new TH1F(tmp,title,1000,-100,100);
    	sprintf(tmp,"%s%i","fDisCTuk_Tk_",index+1);
	fDisCTuk[index] = new TH3F(tmp,title,100,-10,10,100,-10,10,100,-10,10);
    }
  //fDisCTuk = new TH3F("fDisCTuk_Tk_1","fDisCTuk_Tk_1",100,-10,10,100,-10,10,100,-10,10);
  
  fOutFile->cd();
}





Bool_t HMdcGeomParF :: init(void)
{
  //
  //Inits all containers
  //
  
  if(fNumMods < 2){
    //Inicialization of hit category needed for 1 MDC!!!!!
    fHitCat=gHades->getCurrentEvent()->getCategory(catMdcHit);
    if (!fHitCat) {
      fHitCat=gHades->getSetup()->getDetector("Mdc")->buildCategory(catMdcHit);
      if (!fHitCat) return kFALSE;
      else gHades->getCurrentEvent()->addCategory(catMdcHit,fHitCat,"Mdc");
    }
    
    fIter1 = (HIterator *)fHitCat->MakeIterator(); 
    
    //Aqui hay que construir el fTree (equiv. al fAlign en HMdcAlignernew
    Int_t sector = fLoc[0];
    Int_t modA = fLoc[1];    
    
    Char_t title[50], tmp[50];
    
    sprintf(title,"%s%i%s%i","Sector_",sector,"_ModA_",modA);
    sprintf(tmp,"%s%s%i%s%i","All","_Sector_",sector,"_ModA_",modA);    
    if (fNumMods==0){
      sprintf(title,"%s%i","_ModA_",modA);
      sprintf(tmp,"%s%s%i","All","_ModA_",modA);  
    }
    fTree = new TTree(tmp,title);
    fTree->Branch("hits",&fHits);     
    //1MDC end
  }

  setParContainers();
  return kTRUE;
}





Bool_t HMdcGeomParF :: reinit(void)
{
  //
  //
  //

  if(fManual == kFALSE) setGeomAuxPar();
  return kTRUE;
}





Bool_t HMdcGeomParF :: initDelayed(void)
{
  //
  // Initialization called in finalize(). It is used for
  // the initialization of trees and transformations
  // obtained from other tasks in the execute() time 
  //

  Int_t sector = fLoc[0];
  Int_t modA = fLoc[1];
  Int_t modB = fLoc[2];
  Int_t modC = -1;
  Int_t modD = -1;
  Char_t tmp[255];

  if(fNumMods>3){
    modC = fLoc[3];
    modD = fLoc[4];
    if(fUseCut) 
      sprintf(tmp,"%s%s%i%s%i%s%i%s%i%s%i","AllCut","_Sector_",sector,
	      "_ModA_",modA,"_ModB_",modB,"_ModC_",modC,"_ModD_",modD);
    else sprintf(tmp,"%s%s%i%s%i%s%i%s%i%s%i","All","_Sector_",sector,
		 "_ModA_",modA,"_ModB_",modB,"_ModC_",modC,"_ModD_",modD);
  }
  else if(fNumMods>2){
    modC = fLoc[3];    
    if(fUseCut) 
      sprintf(tmp,"%s%s%i%s%i%s%i%s%i","AllCut","_Sector_",sector,
	      "_ModA_",modA,"_ModB_",modB,"_ModC_",modC);
    else sprintf(tmp,"%s%s%i%s%i%s%i%s%i","All","_Sector_",sector,
		 "_ModA_",modA,"_ModB_",modB,"_ModC_",modC);
  }
  else {
    if(fUseCut) sprintf(tmp,"%s%s%i%s%i%s%i","AllCut","_Sector_",sector,
			"_ModA_",modA,"_ModB_",modB);
    else sprintf(tmp,"%s%s%i%s%i%s%i","All","_Sector_",sector,
		 "_ModA_",modA,"_ModB_",modB);
  }
  
  if(fEulerResult && fTransResult) {
    for(Int_t i=0;i<fNumMods-1;i++){
      setEulerAngles(i,fEulerResult[i].getX(),
		     fEulerResult[i].getY(),
		     fEulerResult[i].getZ());
      fillRotMatrix(i,fEulerResult[i].getX(),
		    fEulerResult[i].getY(),
		    fEulerResult[i].getZ());
      fillTranslation(i,fTransResult[i].getX(),
		      fTransResult[i].getY(),
		      fTransResult[i].getZ());
    }
  }
  else if(fNumMods>1) cout << "FATAL ERROR in HMdcGeomParF::initDelayed(): "
			   << "no relative transformation found!" << endl;
  
  //Opening file and getting the TTree  
  if(!fOutFile) fOutFile = new TFile(fNameRoot,"UPDATE");
  

  if(fNumMods>1) { //all but 1 MDC target finder
    fTree = (TTree *)fOutFile->Get(tmp);
    if (!fTree) cout << "ERROR in HMdcGeomParF::initDelayed(): " 
		     << "No Tree found in file " << endl;
  }

  return kTRUE;
}





void HMdcGeomParF :: setGeomParManOn(void)
{
  //
  // Disables the filling of geometrical parameters from
  // the containers. User should introduce parameters in the macro
  // by hand, using setEulerLabAngles(), fillRotLab(), fillTrasLab()
  //

  fManual =kTRUE;  
  cout << "WARNING in HMdcGeomParF::setGeomParManOn(): " 
       << "introducing manually Geometrical" << endl; 
  cout << "Parameters without containers. Parameters " 
       << "should be in the macro" << endl;

}





void HMdcGeomParF :: setControlHistoOff(void)
{
  //
  // Disables control histograms output (saving memory)
  //

  fHistoOff = kTRUE;  
}





void HMdcGeomParF :: setMinimization(Int_t select)
{
  //
  // Selects minimization strategy
  //
  // select = 0 Disables minimization
  // select = 1 Distance to Z axis and target centering
  //

  fMin = select;  
}





void HMdcGeomParF::setUnitErrors(void)
{  
  //
  // introduce unit errors in Hits
  //

  fUseUnitErrors = kTRUE;  
}





void HMdcGeomParF::setFix(Int_t fix)
{  
  //
  // Fix a parameter set in minimization
  // Z translation is always fixed for Minuit.
  // 1...6 parameter number (Minuit parameters begin in 0, then param+1)
  // 20    fixes translations
  // 30    fixes rotations

  fFix = fix;  
}





void HMdcGeomParF::setNoCut(void)
{  
  //
  // Set the TTree from the HMdcAligner selection
  // When fUseCut=kFALSE it is used a TClonesArray 
  // of Hits without cuts in the distributions peak fit

  fUseCut = kFALSE;  
}





void HMdcGeomParF::setAngleCut(Float_t aCut=0.5)
{  
  //
  // Sets a cut in the 1 MDC target finder. 
  // Introducing a positive valu: the cut
  // eliminates in the fit the low incidence angle tracks, 
  // trying to get more resolution in Z target determination
  // Introducing a negative value: the cut
  // eliminates in the fit the large incidence angle tracks,
  // trying to eliminate fails in reconstruction due to
  // larger distances between planes than in parameters
  // or any tracking systematics in the reconstruction
 
  fUseAngleCut = kTRUE;
  fAngleCut = aCut;
}





void HMdcGeomParF::setYPosCut(Float_t aYCut=0.)
{  
  //
  // Sets a cut in the 1 MDC target finder. 
  // The cut eliminates in the fit the tracks closer
  // to the beam line, which pushes the target estimate
  // towards positive values of Z.
 
  fUseYPosCut = kTRUE;
  fYPosCut = aYCut;
}





void HMdcGeomParF :: setParContainers(void)
{
  //
  // Loads the parameter containers it uses later
  //

  fMdcGeomPar=(HMdcGeomPar*)gHades
    ->getRuntimeDb()->getContainer("MdcGeomPar");

}





void HMdcGeomParF :: setGeomAuxPar(void)
{
  //
  // Defines the parameter containers it uses later
  //
  // The transformations are:  X = R X' + T
  // in the geometrical parameters.
  // That is:
  //   X = R(A) X(A) + T(A)      
  //   X = R(B) X(B) + T(B)  ...
  //
  
  Int_t sector = fLoc[0];
  Int_t moduleA = fLoc[1]; 
  Int_t moduleB = fLoc[2];  
  Int_t moduleC = -1;
  Int_t moduleD = -1;  
  HGeomVector euler;
  HGeomTransform transform;  

  if(GP_DEBUG>0){
    cout << endl <<"Debugging output in HMdcGeomParF::setGeomAuxPar" << endl;
    cout << "Original transformation from container" << endl;
    cout << " ------ SECTOR " << sector << " ------ " << endl;
  }    
  if(fNumMods>3){    
    moduleD = fLoc[4];  
    transform = fMdcGeomPar->getModule(sector,moduleD)->getLabTransform();
    if(GP_DEBUG>0){
      cout << "Reference: MDC D (Module " << moduleD << ")" << endl;
      transform.print(); 
    }  
  }
  else if (fNumMods>2){
    moduleC = fLoc[3];
    transform = fMdcGeomPar->getModule(sector,moduleC)->getLabTransform();
    if(GP_DEBUG>0){
      cout << "Reference: MDC C (Module " << moduleC << ")" << endl;
      transform.print(); 
    }   
  }
  else if (fNumMods>1){
    transform = fMdcGeomPar->getModule(sector,moduleB)->getLabTransform();
    if(GP_DEBUG>0){
      cout << "Reference: MDC B (Module " << moduleB << ")" << endl;
      transform.print(); 
    }  
  }
  else {
    if(fNumMods==1)
      transform = fMdcGeomPar->getModule(sector,moduleA)->getLabTransform();
    else {//all sectors, 1 frustum
      //transform = fMdcGeomPar->getModule(0,moduleA)->getLabTransform();
      for(Int_t i=0; i<6;i++){
	if(fMdcGeomPar->getModule(i,moduleA)!=0){
	  transform = fMdcGeomPar->getModule(i,moduleA)->getLabTransform();
	  fRotMat[i] = transform.getRotMatrix();
	  fTranslation[i] = transform.getTransVector();	
	  cout << "**************************************************" << endl;
	  cout << "*  Target finder in a complete frustum          *" << endl;
	  cout << "**************************************************" << endl;
	}
      }
    }
    if(GP_DEBUG>0){
      cout << "Reference: MDC A (Module " << moduleA << ")" << endl;
      transform.print(); 
    }  
  }
  HGeomRotation rot = transform.getRotMatrix();
  HGeomVector vect = transform.getTransVector();
  
  //finding rotation euler angles 
  euler = findEulerAngles(rot);    
  
  fillRotLab(euler.getX(),euler.getY(),euler.getZ());
  fillTransLab(vect.getX(),vect.getY(),vect.getZ());
  setEulerLabAngles(euler.getX(),euler.getY(),euler.getZ());
  
  cout << "**************************************************" << endl;
  cout << "* HMdcGeomParF::setGeomAuxPar: from geom params:  *" << endl;

  if(fNumMods>1) cout << "* Sector: "<< sector 
		      << "  Reference Module: " <<fNumMods-1<<endl; 
  else cout << "* Sector: "<< sector 
		      << "  Reference Module: " << moduleA<<endl; 
  cout << "* Rotation (Lab): " << euler.getX() << ", " << euler.getY()  
       << ", " << euler.getZ() << endl;
  cout << "* Translation (Lab): " << vect.getX() << ", " 
       << vect.getY() << " , " <<  vect.getZ() << endl;
  cout << "**************************************************" << endl;
}  



HGeomVector HMdcGeomParF::findEulerAngles(HGeomRotation rot){
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
  
  if(GP_DEBUG>0){
    cout << endl <<"Euler angles: " << euler[0] << ", " 
	 << euler[1]  << ", " << euler[2] << endl;
  }
  
  HGeomRotation test;  
  test.setEulerAngles(euler[0]*180./TMath::Pi(),
		      euler[1]*180./TMath::Pi(),
		      euler[2]*180./TMath::Pi()); 
  
  if(GP_DEBUG>0){
    cout << endl <<"Rotation from Euler angles (first attemp): " << endl;
    test.print();
  }
  
  //Now solving the problem when euler[1]<0
  Double_t eps = 0.0001; //check precision
  
  if( (fabs(test(0)-rot(0))>eps) || (fabs(test(1)-rot(1))>eps) || 
      (fabs(test(3)-rot(3))>eps) || (fabs(test(4)-rot(4))>eps) ) {
    if(GP_DEBUG>0) cout << endl << "Bad election in first euler angle! " 
		       << "Trying again. "<< endl;
    euler[1] = - euler[1]; 
    sinaux = sin(euler[1]);
    euler[0] = atan2(rot(5)/sinaux,rot(2)/sinaux);
    euler[2] = atan2(rot(7)/sinaux,-rot(6)/sinaux);
    
    test.setEulerAngles(euler[0]*180./TMath::Pi(),
			euler[1]*180./TMath::Pi(),
			euler[2]*180./TMath::Pi());  
    
    if(GP_DEBUG>0){
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





Int_t HMdcGeomParF :: execute(void)
{  
  //
  // Makes absolutely nothing in case of more
  // than one MDCs. Calls execute1 in case of one MDC.
  //

  if(fNumMods==1) execute1();
  if(fNumMods==0) execute0();
  return 0;
}





void HMdcGeomParF::execute1(void)
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
  //next line has been change to get rid of the Dubna fitter results with
  //chi square=-1, that is, the results of the cluster finder
  while ((pHitA =(HMdcHit*)fIter1->Next()) != 0  && (pHitA->getChi2()>-0.1)){ 

    //Enter here cuts if necessary
    //if(fUseCut=kTrue)

    new((*fHits)[0])HMdcHit(*pHitA);	
  
    fTree->Fill();
    fHits->Clear();

  } // End of  while ( (pHitA =(HMdcHit*)fIter1->Next()) != 0 ){ 
}





void HMdcGeomParF::execute0(void)
{    
  // 
  // Filling Hits of a frustum. Trying to find out the point where all
  // tracks point to. 
  //

  HMdcHit* pHitA; 

  Int_t modA = fLoc[1];
 
  HLocation local;
  local.setNIndex(2);
    
  for(Int_t i=0;i<6;i++){
    local.set(2,i,modA); 
    
    fIter1->Reset(); 
    fIter1->gotoLocation(local); 
    //next line has been change to get rid of the Dubna fitter results with
    //chi square=-1, that is, the results of the cluster finder
    while ((pHitA =(HMdcHit*)fIter1->Next()) != 0 && (pHitA->getChi2()>-0.1)){ 
      
      //Enter here cuts if necessary
      //if(fUseCut=kTRUE)
      
      new((*fHits)[0])HMdcHit(*pHitA);	
      
      fTree->Fill();
      fHits->Clear();
      
    } // End of  while ( (pHitA =(HMdcHit*)fIter1->Next()) != 0 ){ 
  } // End of   for(Int_t i=0;i<6;i++){
}





Bool_t HMdcGeomParF :: finalize(void)
{    
  // 
  // Makes initialization, minimization and
  // fills the control histograms for final values
  //

  //Initialization!
  initDelayed();

  Int_t sector = fLoc[0];
  Int_t modA = fLoc[1];
  Int_t modB = fLoc[2];
  Int_t modC = -1;  
  Int_t modD = -1;

  if(fNumMods>2) modC = fLoc[3]; 
  if(fNumMods>3) modD = fLoc[4]; 

  //Some control histo
  if(fHistoOff!=kTRUE) setHistograms();
  if(fHistoOff!=kTRUE) fillHistograms(0);

  ofstream *fout=NULL;
  if (fNameAscii) fout = new ofstream(fNameAscii.Data(), ios::app);
  if (*fout){
    *fout << endl << " Laboratory transformation parameters " << endl;
    *fout << endl << "Sector: " << sector << endl;
    *fout << "Module A: " << modA; 
    if(fNumMods>1) *fout << "  Module B: " << modB;
    if(fNumMods>2) *fout << "  Module C: " << modC;    
    if(fNumMods>3) *fout << "  Module D: " << modD;
    *fout << endl << endl;     
    *fout << "Transformation before minimization (init values): "  << endl;
    *fout << fEulerLab(0) << ", " << fEulerLab(1) << ", " 
	  << fEulerLab(2) << ", " << fTransLab(0) << ", " 
	  << fTransLab(1)<< ", " << fTransLab(2)<< endl;
  }

  if(fNumMods<2 || (fNumMods==2 && fTFFor2Mods)) {
    targetFinder(fout);
    if(fHistoOff!=kTRUE) fillHistograms(1);
    storeInFile();
    //writeGeomParContainer();
    fout->close();
    delete fout;
    return kTRUE;
  }

  //minimization strategy
  if(!fMin){ //if  no minimization is performed  
    if (*fout) *fout << "Minimization strategy = 0: No minimization" << endl;
    storeInFile();
    //writeGeomParContainer();
    fout->close();    
    delete fout;
    return kTRUE; 
  }
  
  Float_t OldEulerLab[3];
  Float_t OldTransLab[3];
  Int_t IterCounter =0;
  Float_t IterCri;

  do{
    IterCri = 0;
    for(Int_t i=0;i<3;i++){
      OldEulerLab[i] = fEulerLab(i);
      OldTransLab[i] = fTransLab(i);
    }
    minfit(fFix,fEulerLab(0),fEulerLab(1),fEulerLab(2),
	   fTransLab(0),fTransLab(1),fTransLab(2));
    if (*fout){
      *fout << "Parameters after minimization "  << endl;
      *fout << fEulerLab(0) << "+-" << fError[0] << "," 
	    << fEulerLab(1) << "+-" << fError[1] << "," 
	    << fEulerLab(2) << "+-" << fError[2] << "," 
	    << fTransLab(0) << "+-" << fError[3] << "," 
	    << fTransLab(1) << "+-" << fError[4] << "," 
	    << fTransLab(2) << "+-" << fError[5] << endl;
    }
    for(Int_t i=0;i<3;i++){
      IterCri += fabs((fEulerLab(i)-OldEulerLab[i])/fEulerLab(i));
      IterCri += fabs((fTransLab(i)-OldTransLab[i])/fTransLab(i));
      }
    IterCounter++;
    if(IterCounter>10) {
      cout << "WARNING in HMdcGeomParF :: execute" << endl;
      cout << "Sector: " << sector << " ModuleA: " << modA 
	   <<  " ModuleB: " << modB; 
      if(fNumMods>2) *fout << "  Module C: " << modC;    
      if(fNumMods>3) *fout << "  Module D: " << modD;
      cout << endl;
      cout << "More than 10 iterations without results!" <<endl;
      break;
    }
  }while(IterCri>fIterCriteria);
  
  
  //Still last parameters (third component of traslation 
  //or fTransLab(2)) is not known 
  findZComponent();      
  
  if (*fout){
    *fout << "Parameters after Z centering "  << endl;
    *fout << fEulerLab(0) << "+-" << fError[0] << "," 
	  << fEulerLab(1) << "+-" << fError[1] << "," 
	  << fEulerLab(2) << "+-" << fError[2] << "," 
	  << fTransLab(0) << "+-" << fError[3] << "," 
	  << fTransLab(1) << "+-" << fError[4] << "," 
	  << fTransLab(2) << "+-" << fError[5] << endl;
  }
 
 //third, graphical results with the minimization results
  if(fHistoOff!=kTRUE) fillHistograms(1);
  
  fout->close();
  delete fout;

  storeInFile();
  //writeGeomParContainer();

  return kTRUE;
}





Bool_t HMdcGeomParF::targetFinder(ofstream *fout)
{
  //
  // target finder algorithm
  //
  
  Int_t sector = fLoc[0];
  Int_t modA = fLoc[1];
  HGeomVector aPoint; 
  HGeomVector aVector;
  HGeomVector theTarget,finalTarget;
  HGeomVector oldTarget;
  Float_t third=0;
  Float_t part1, part2, part3, numera,denomi, distRel, disTarget;
  Float_t weigth = 1;
  Float_t t; 
  Float_t angularError = 0.015 ; // 0.05 -> 3 degrees 0.015 -> 0.85 degrees
  Float_t distTarMDC,distToModulePC,distAllowed;
  Bool_t accepted=kTRUE;
  HMdcHit* pHitA;   
  HMdcHit* pHitB;  
  Stat_t entries; 
  Int_t sec=0;
  Int_t iternum=0;
  Int_t cont=0;
  //Float_t weigthCorr;
  //Float_t thetaHitTar, theta;
  //for graphics
  Float_t xcZ, ycZ, phiA, thetaA, ztar;
  Float_t x,y,z;
  HGeomVector a, b, ainb;
  Int_t loopTukHist=4;
  Float_t localTuk=-1;

  //calculating the limit distance around MDC Phys. center if cuts
  if(fNumMods==0)
    distTarMDC = sqrt(fTranslation[1].getX()*fTranslation[1].getX()+
		      fTranslation[1].getY()*fTranslation[1].getY()+   
		      fTranslation[1].getZ()*fTranslation[1].getZ());
  else
    distTarMDC = sqrt(fTransLab.getX()*fTransLab.getX()+
		      fTransLab.getY()*fTransLab.getY()+   
		      fTransLab.getZ()*fTransLab.getZ());
  
  distAllowed = fabs(fAngleCut*distTarMDC);
  
  cout << "distAllowed " << distAllowed << endl;

  //KOENIGs
  //  Float_t pxa, pya, xDir, yDir, aux,ps0a,ps1a;
  
  //The Manuel's Vertex finder applied to all the events
  HGeomVertexFit* targetFinder= new HGeomVertexFit();  
  targetFinder->reset();
  
  entries = fTree->GetEntries();

  //if no init value is given, a first iteration on all Hits
  //without any weigth gives the first estimate.  
  if(fUseInitValue) theTarget = fInitValue;
  else{
    if(fNumMods == 2){
      TBranch *branch = fTree->GetBranch("hits");
      branch->SetAddress(&fHits);
      entries = fTree->GetEntries();
    }
    //loop on hits   
    for (Int_t i=0;i<entries;i++) { 
      if(fNumMods == 2){
	fHits->Clear();
	fTree->GetEntry(i);
	pHitB = (HMdcHit*) fHits->At(1);
	pHitA = (HMdcHit*) fHits->At(0);
	
	HGeomVector a,b,ainb;
	//filling from the tree
	a.setX(pHitA->getX());
	a.setY(pHitA->getY());       
	b.setX(pHitB->getX());
	b.setY(pHitB->getY());
	a.setZ(0.);
	b.setZ(0.);
	//coord. of MDC A in MDC B system:
	// X(B) = M X(A) + V    
	ainb = fRotMat[0]*a+fTranslation[0];
	// point coord. in lab system  
	//alab = fRotLab*ainb+fTransLab;
	//blab = fRotLab*b+fTransLab;
	
	aPoint = b;
	aVector.setX(b.getX() - ainb.getX());
	aVector.setY(b.getY() - ainb.getY());
	aVector.setZ(b.getZ() - ainb.getZ());	
      }
      else{
	fTree->GetEntry(i);
	pHitA = (HMdcHit*) fHits->At(0);
	
	sec = pHitA->getSector();
	aPoint.setX(pHitA->getX());
	aPoint.setY(pHitA->getY());
	aPoint.setZ(0.);
	
	Float_t trump = 1-pHitA->getXDir()*pHitA->getXDir()
	  -pHitA->getYDir()*pHitA->getYDir();
	
	if(trump<0) cout << "WARNING: Trying to sqrt x when x<0";	
	else third = sqrt(trump);
	
	aVector.setX(pHitA->getXDir());
	aVector.setY(pHitA->getYDir());
	aVector.setZ(third);
	
	if(fNumMods==0) {//target finder of frustum 
	  aPoint = fRotMat[sec]*aPoint+fTranslation[sec];
	  aVector = fRotMat[sec]*aVector;
	}
      }
      //KOENIG
      //a few Hits for W. Koenig
      /*
	pxa =pHitA->getX();
	pya = pHitA->getY();       
	xDir = pHitA->getXDir();
	yDir = pHitA->getYDir(); 
	//If using HMdcHit, it is neccesary this change
	aux = sqrt(1 - xDir * xDir - yDir * yDir);
	if(aux == 0.) {
	ps0a=1; 
	ps1a=1; 
	cout<< "ERROR #1 in HMdcGeomParF::fcnGP().";
	} 
	else{
	ps0a = xDir/aux;
	ps1a = yDir/aux;
	}
	
	*fout << pxa << "  " << pya << "  " << ps0a  << "  " << ps1a <<endl;
	*/

      cont++;
      //Add the Hit with weigth 1
      targetFinder->addLine(aPoint,aVector,1);       
    }
    targetFinder->getVertex(theTarget);
  }
  
  cout << "###### TARGET FINDER RESULT #######" << endl;
  cout << "Init Value (no weigth)" << endl;
  cout << " Sector: "  << sector 
       << "  Module: " << modA << endl;
  theTarget.print();
  if(!fUseInitValue) cout << "Total entries : " << cont;     
      
  if(fout) {
    *fout << " Init value: " << endl << theTarget.getX() << ", " 
	  << theTarget.getY()<< ", " << theTarget.getZ() << endl;
  }
  
  while(fConstTukey>=fEndTukey){
    targetFinder->reset();
    cout << endl << "fConstTukey = " <<  fConstTukey << endl;
    
    //recursive while does not converge
    do{    
      targetFinder->reset();
      oldTarget = theTarget;
      cont = 0;

      if(fNumMods == 2){
	TBranch *branch = fTree->GetBranch("hits");
	branch->SetAddress(&fHits);
	entries = fTree->GetEntries();
      }
      //loop on hits 
      for (Int_t i=0;i<entries;i++) { 
	if(fNumMods == 2){
	  fHits->Clear();
	  fTree->GetEntry(i);
	  pHitB = (HMdcHit*) fHits->At(1);
	  pHitA = (HMdcHit*) fHits->At(0);
	  
	  HGeomVector a,b,ainb,alab,blab;
	  //filling from the tree
	  a.setX(pHitA->getX());
	  a.setY(pHitA->getY());       
	  b.setX(pHitB->getX());
	  b.setY(pHitB->getY());
	  a.setZ(0.);
	  b.setZ(0.);
	  //coord. of MDC A in MDC B system:
	  // X(B) = M X(A) + V  
	  ainb = fRotMat[0]*a+fTranslation[0];
	  // point coord. in lab system  
	  //alab = fRotLab*ainb+fTransLab;
	  //blab = fRotLab*b+fTransLab;
	  
	  aPoint = b;
	  aVector.setX(b.getX() - ainb.getX());
	  aVector.setY(b.getY() - ainb.getY());
	  aVector.setZ(b.getZ() - ainb.getZ());
	}
	else{
	  fTree->GetEntry(i);
	  pHitA = (HMdcHit*) fHits->At(0);
	  
	  sec = pHitA->getSector();
	  aPoint.setX(pHitA->getX());
	  aPoint.setY(pHitA->getY());
	  aPoint.setZ(0.);
	  
	   Float_t trump = 1-pHitA->getXDir()*pHitA->getXDir()
	    -pHitA->getYDir()*pHitA->getYDir();
	  
	  if(trump<0) cout << "WARNING: Trying to sqrt x when x<0";	
	  else third = sqrt(trump);
	  
	  aVector.setX(pHitA->getXDir());
	  aVector.setY(pHitA->getYDir());
	  aVector.setZ(third);
	}
	//aPoint.print();
	//aVector.print();

	if(fUseAngleCut) {
	  distToModulePC=sqrt(aPoint.getX()*aPoint.getX()
			      +aPoint.getY()*aPoint.getY());

	  //cout << "distToModulePC " << distToModulePC << endl;

	  if(fAngleCut>0 && distToModulePC<distAllowed) accepted=kFALSE;
	  else if(fAngleCut<0 && distToModulePC>distAllowed) accepted=kFALSE;
	  else accepted=kTRUE;	
	}
	
	if(fUseYPosCut) {	  
	  if(pHitA->getY()<fYPosCut) accepted=kFALSE;
	  else accepted=kTRUE;	
	}

	if(fNumMods==0) {//target finder of frustum 
	  aPoint = fRotMat[sec]*aPoint+fTranslation[sec];
	  aVector = fRotMat[sec]*aVector;
	}

	//aPoint.print();
	//aVector.print();
	//theTarget.print();

	//finding weigths  (distance to target)  
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
	
	//An exponential weigth
	//	weigth = exp(-Factor*disTarget);
	
	//Tukey bisquared weigths (NIM A394(1997)225)
	//the error is an approx. angular error (rads)
	//times the approx. distance to the target
	t = disTarget/(angularError*distTarMDC);
	if(fabs(t)<fConstTukey && accepted){
	  weigth = (1-t*t/(fConstTukey*fConstTukey)) 
	    * (1-t*t/(fConstTukey*fConstTukey));
	  cont++;

	  //Introducing an additional weight to compensate the assymetry
	  //in the Z direction. Those Hits with polar angle larger than
	  //the theorical polar angle obtained from their MDC position and
	  //the target position are corrected. The correction is based on
	  //a symmetry of the MDCs determining the angle
	  /*
	    if(fConstTukey<3.9){
	    //thetaHitTar is the polar angle calculated from the
	    //Hit coordinates and the target position
	    thetaHitTar = atan(sqrt(  (aPoint.getX()-theTarget.getX())*
	    (aPoint.getX()-theTarget.getX())+
	    (aPoint.getY()-theTarget.getY())*
	    (aPoint.getY()-theTarget.getY())  )
	    /(aPoint.getZ()-theTarget.getZ()));
	    
	    theta = acos((aVector.getZ())/
	    sqrt((aVector.getX())*(aVector.getX())+
	    (aVector.getY())*(aVector.getY())+
	    (aVector.getZ())*(aVector.getZ())));	    
	    //cout << "_______ thetaHitTar: " << thetaHitTar <<endl;
	    //cout << "_______ theta: " << theta <<endl;
	    //cout << "___________________________________" << endl;
	    
	    if(theta < thetaHitTar){
	    weigthCorr = sin(theta)/sin(2*thetaHitTar-theta);
	    weigth = weigth*weigthCorr;	
	    //cout << "_______ weigthCorr: " << weigthCorr <<endl;
	    //cout << "___________________________________" << endl;
	    }
	    }
	  */
	  
	  //cout << "weigth " << weigth << endl;
	  //Add the Hit with weigth 


	  //filling distribution graphical information 
	  //The information in the histograms fTarTuk, fTarTukW and fDisCTuk has changed;
	  //now they represent the information of the four last Tukey constants, even being
	  //their names on the Browser 4,3,2,1.
	  if(fConstTukey<=fEndTukey+4*fStepTukey && fConstTukey>fEndTukey && iternum==2){
	    if(localTuk == -1.) {
	      localTuk = fConstTukey;
	      loopTukHist = 4;
	    }
	    else 
	      if(localTuk != fConstTukey)
		loopTukHist--;	    
	    
	    xcZ = aPoint.getX()-(aVector.getX())*
	      aPoint.getZ()/(aVector.getZ());
	    ycZ = aPoint.getY()-(aVector.getY())*
	      aPoint.getZ()/(aVector.getZ());
     
	    phiA = atan2(aVector.getY(),aVector.getX());
	    thetaA = acos((aVector.getZ())/
			 sqrt((aVector.getX())*(aVector.getX())+
			      (aVector.getY())*(aVector.getY())+
			      (aVector.getZ())*(aVector.getZ())));

	    ztar = -(ycZ * sin(phiA) + xcZ*cos(phiA))*cos(thetaA)/sin(thetaA);
	    
	    fTarTuk[loopTukHist]->Fill(ztar);
	    fTarTukW[loopTukHist]->Fill(ztar,weigth);

	    //v+(r-v) x a con v vertice, r punto y a vector 
	    z=(aPoint.getX()*aVector.getY())-(aPoint.getY()*aVector.getX());
	    x=(aPoint.getY()*aVector.getZ())-(aPoint.getZ()*aVector.getY());
	    y=(aPoint.getZ()*aVector.getX())-(aPoint.getX()*aVector.getZ());
	    fDisCTuk[loopTukHist]->Fill(x,y,z);
	  }

	  targetFinder->addLine(aPoint,aVector,weigth);   
	}
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
      iternum++;
      if(iternum>100)   {    
	cout << "More than 100 iterations without results!" <<endl;
	break;
      }
    }while(distRel>0.001);    
    iternum=0;

    if(fout) {
      *fout << " Tukey = " << fConstTukey << "   " << theTarget.getX() 
	    << ", " << theTarget.getY()<< ", " << theTarget.getZ() 
	    << "  Entries: " << cont << endl;
    }
    if(fConstTukey==fResultTukey) finalTarget=theTarget;
    if(fStepTukey!=1)fConstTukey = fConstTukey-fStepTukey;
    else{
      if(fConstTukey>1)fConstTukey = fConstTukey-1.;
      else fConstTukey = fConstTukey -0.5; 
    }
  }
  
  if(fNumMods==0) {//target finder of frustum 
    *fout << "Target estimate in LAB coordinates  " <<endl;
    *fout << finalTarget.getX() << ", " << finalTarget.getY()<< ", " 
	  << finalTarget.getZ() << endl;
    
    //New transformations (relative vector vs. the new target)
    //Here (for fNumMods=0) fTranslation are lab transformations
    for(Int_t i=0;i<6;i++){
      *fout << "Old fTranslation[" << i << "]:";
      *fout << fTranslation[i].getX() << ", " 
	    << fTranslation[i].getY() << ", " 
	    << fTranslation[i].getZ() << endl;
      fTranslation[i] = fTranslation[i]-finalTarget;
      *fout << "New fTranslation[" << i << "]:";
      *fout << fTranslation[i].getX() << ", " 
	    << fTranslation[i].getY() << ", " 
	    << fTranslation[i].getZ() << endl;
    }
  }
  else{
    //I have a vector which defines the target 
    //position in MDC coordinates. Then, change the vector to
    //Lab coordinates and that is the new MDC center coordinates 
    //in the LAB system!
    if(fUseAngleCut) *fout << endl <<"Using an Angle Cut (" << fAngleCut 
			   << ") = " << distAllowed
			   << "mm. around MDC phys. center " <<endl;
    
    *fout  << endl <<"Original MDC translation vector: " <<endl;
    *fout << fTransLab.getX() << ", " << fTransLab.getY()<< ", " 
	  << fTransLab.getZ() << endl;
    *fout << "Target estimate in MDC coordinates  " << endl;
    *fout << finalTarget.getX() << ", " << finalTarget.getY()<< ", " 
	  << finalTarget.getZ() << endl;
    
    finalTarget = fRotLab*finalTarget+fTransLab;
    
    *fout << "Target estimate in LAB coordinates  " <<endl;
    *fout << finalTarget.getX() << ", " << finalTarget.getY()<< ", " 
	<< finalTarget.getZ() << endl;
    
    fTransLab = fTransLab-finalTarget; 
    
    *fout  << endl <<"New estimate of the MDC translation vector: " <<endl;
    *fout << fTransLab.getX() << ", " << fTransLab.getY()<< ", " 
	<< fTransLab.getZ() << endl;
  }
  
  return kTRUE;
}





void HMdcGeomParF :: fillHistograms(Int_t index){
  //
  // Filling histograms
  //
      
  HMdcHit* hitA;
  HMdcHit* hitB=NULL;
  HMdcHit* hitC=NULL;
  HMdcHit* hitD=NULL;
      
  Float_t xcZ=0, ycZ=0, xcY=0, zcY=0, ycX=0, zcX=0;
  Float_t phi=0, theta=0,ztar=0, rtar=0, xtar=0, ytar=0, canaux;
  Float_t myxtar=0, myytar=0, myztar=0, myrho=0, distZ=0;
  Float_t part1, part2, part3, numera, denomi, discenter;
  Float_t distTarMDC,distToModulePC,distAllowed;

  TBranch *branch = fTree->GetBranch("hits");
  branch->SetAddress(&fHits);
  Stat_t entries = fTree->GetEntries();

  //loop on hits   
  for (Int_t i=0;i<entries;i++) {     
    fHits->Clear();
    fTree->GetEntry(i);
    if(fNumMods>1) hitB = (HMdcHit*) fHits->At(1);
    hitA = (HMdcHit*) fHits->At(0);
    
    if(fNumMods>2){      
      HGeomVector a,b,c,d;
      HGeomVector transf[4];  
      Float_t errorx[4]; 
      Float_t errory[4];
      
      //First, calculate the straigth line     
      hitC = (HMdcHit*) fHits->At(2);
      if(fNumMods>3){
	hitD = (HMdcHit*) fHits->At(3);
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
      
      Float_t ax=0.0, ay=0.0, bx=0.0, by=0.0;     
      Float_t sigax=0.0, sigay=0.0, sigbx=0.0, sigby=0.0; 
      Float_t Xwt, Xt, Xsxoss, Xsx=0.0, Xsy=0.0, Xst2=0.0, Xss=0.0;
      Float_t Ywt, Yt, Ysxoss, Ysx=0.0, Ysy=0.0, Yst2=0.0, Yss=0.0;
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
      }
      
      Xsxoss = Xsx/Xss;    
      Ysxoss = Ysx/Yss;
      
      for(Int_t i=0;i<fNumMods;i++){
	//Plane XZ
	Xt = (transf[i].getZ()-Xsxoss)/errorx[i];
	Xst2 += Xt * Xt;
	bx += Xt * transf[i].getX()/errorx[i];            
	//Plane YZ
	Yt = (transf[i].getZ()-Ysxoss)/errory[i];
	Yst2 += Yt * Yt;
	by += Yt * transf[i].getY()/errory[i];
      } 

      // X = ax + bx Z
      // Y = ay + by Z  
      bx /= Xst2;
      ax = (Xsy-(Xsx*bx))/Xss;
      by /= Yst2;
      ay = (Ysy-(Ysx*by))/Yss;

      sigax = sqrt((1.0+Xsx*Xsx/(Xss*Xst2))/Xss);
      sigbx = sqrt(1.0/Xst2);
      sigay = sqrt((1.0+Ysx*Ysx/(Yss*Yst2))/Yss);
      sigby = sqrt(1.0/Yst2);
      
      //Cuts with Z=0 is (X,Y)=(ax,bx)
      fPlanez[index]->Fill(ax,bx);

      //Cuts with Y=0 is (X,Z)=(ax-bx*ay/by,-ay/by)
      fPlaney[index]->Fill(ax-bx*ay/by,-ay/by);
 
      //Cuts with X=0 is (Y,Z)=(ay-by*ax/bx,-ax/bx)
      fPlanex[index]->Fill(ay-by*ax/bx,-ax/bx);

      //Projection
      fProzY[index]->Fill(-ay/by);      
      fProzX[index]->Fill(-ax/bx);
      fProyZ[index]->Fill(bx);      
      fProyX[index]->Fill(ay-by*ax/bx);      
      fProxZ[index]->Fill(ax);
      fProxY[index]->Fill(ax-bx*ay/by);
 
      // phi and theta: track spherical coordinates in a system
      // with Z axis parallel to Z laboratory
      phi = atan2(by,bx);
      theta = atan(sqrt(bx*bx+by*by));
      
      //coordinates of the point where the track passes closer to beam
      Float_t common = bx*bx + by*by;
      xtar = (by*by*ax - ay*bx*by) / common;
      ytar = (ay*bx*bx - ax*bx*by) / common;
      ztar = -(ay*by + ax*bx) / common;

      rtar = (ay*bx-ax*by) / sqrt(common);
       //rtar = ycZ * cos(phi) - xcZ*sin(phi);
      
      discenter = sqrt((ax*ax*(by*by+1)+ay*ay*(bx*bx+1)-2*ax*ay*bx*by) /
	bx*bx+by*by+1);
      
      fZVsTheta[index]->Fill(theta,ztar); //z vs. theta
      fZVsRho[index]->Fill(rtar,ztar); //z vs. rho
      fTargets[index]->Fill(ztar);
      fDisZ[index]->Fill(rtar);
      fDisCenter[index]->Fill(discenter);
    }
    if(fNumMods==2){ 
      HGeomVector a,b,ainb,alab,blab;
      
      //filling from the tree
      a.setX(hitA->getX());
      a.setY(hitA->getY());       
      b.setX(hitB->getX());
      b.setY(hitB->getY());
      a.setZ(0.);
      b.setZ(0.);
      
      if(GP_DEBUG>3){
	cout << "MDC HIT A: "  << endl; 
	  a.print();  
	cout << "MDC HIT B: "  << endl; 
	  b.print();  
      }
      
      //coord. of MDC A in MDC B system:
      // X(B) = M X(A) + V    
      ainb = fRotMat[0]*a+fTranslation[0];
      
      if(GP_DEBUG>3){
	cout << "MDC HIT A IN MDC B CS: "<< endl; 
	ainb.print();
      }
      
      // point coord. in lab system
      alab = fRotLab*ainb+fTransLab;
      blab = fRotLab*b+fTransLab;
      
      if(GP_DEBUG>3){
	cout << "MDC HIT A IN LAB CS: " << endl;
	alab.print();
	cout << "MDC HIT B IN LAB CS: " << endl;
	blab.print();
      }
      
      //Cuts with Z=0
      xcZ = alab.getX()-(blab.getX()-alab.getX())*
	alab.getZ()/(blab.getZ()-alab.getZ());
      ycZ = alab.getY()-(blab.getY()-alab.getY())*
	alab.getZ()/(blab.getZ()-alab.getZ());     
      fPlanez[index]->Fill(xcZ,ycZ);
      
      //Cuts with Y=0
      xcY = alab.getX() -(blab.getX()-alab.getX())*
	alab.getY()/(blab.getY()-alab.getY());
      zcY = alab.getZ() -(blab.getZ()-alab.getZ())*
	alab.getY()/(blab.getY()-alab.getY());
      fPlaney[index]->Fill(xcY,zcY);
      
      //Cuts with X=0
      ycX = alab.getY() -(blab.getY()-alab.getY())*
	alab.getX()/(blab.getX()-alab.getX());
      zcX = alab.getZ() -(blab.getZ()-alab.getZ())*
	alab.getX()/(blab.getX()-alab.getX());
      fPlanex[index]->Fill(ycX,zcX);
      
      //Projection
      fProzY[index]->Fill(zcY);      
      fProzX[index]->Fill(zcX);
      fProyZ[index]->Fill(ycZ);      
      fProyX[index]->Fill(ycX);      
      fProxZ[index]->Fill(xcZ);
      fProxY[index]->Fill(xcY);
      
      //more info
      phi = atan2(blab.getY()-alab.getY(),blab.getX()-alab.getX());
      theta = acos((blab.getZ()-alab.getZ())/
		   sqrt((blab.getX()-alab.getX())*(blab.getX()-alab.getX())+
			(blab.getY()-alab.getY())*(blab.getY()-alab.getY())+
			(blab.getZ()-alab.getZ())*(blab.getZ()-alab.getZ())));
      //coordinates of the point where the track passes closer to beam
      ztar = -(ycZ * sin(phi) + xcZ*cos(phi))*cos(theta)/sin(theta);
      xtar = ((blab.getX()-alab.getX())*(ztar-alab.getZ())/
	      (blab.getZ()-alab.getZ())) + alab.getX();
      ytar = ((blab.getY()-alab.getY())*(ztar-alab.getZ())/
	      (blab.getZ()-alab.getZ())) + alab.getY();
      rtar = ycZ * cos(phi) - xcZ*sin(phi);
      
      fZVsTheta[index]->Fill(theta,ztar); //z vs. theta
      fZVsRho[index]->Fill(rtar,ztar); //z vs. rho
      
      fTargets[index]->Fill(ztar);
      
      //again, with my own calculation (not used in histo but used for testing)
      canaux = ((alab.getX()-blab.getX())/(blab.getY()-alab.getY())) - 
	((blab.getY()-alab.getY())/(blab.getX()-alab.getX()));
      myxtar = (alab.getY()-
		(alab.getX()*(blab.getY()-alab.getY())/
		 (blab.getX()-alab.getX()))) / canaux;
      myytar = (myxtar*(blab.getY()-alab.getY())/(blab.getX()-alab.getX())) + 
	(alab.getY()-alab.getX()*(blab.getY()-alab.getY())/
	 (blab.getX()-alab.getX()));
      myztar = (myxtar*(blab.getZ()-alab.getZ())/(blab.getX()-alab.getX())) + 
	(alab.getZ()-alab.getX()*(blab.getZ()-alab.getZ())/
	 (blab.getX()-alab.getX()));
      myrho = sqrt(myxtar*myxtar + myytar*myytar);
      
      //distance to beam
      distZ = ((alab.getX()*(blab.getY()-alab.getY()))-
	       (alab.getY()*(blab.getX()-alab.getX()))) /
	sqrt(((blab.getY()-alab.getY())*(blab.getY()-alab.getY())) + 
	     ((blab.getX()-alab.getX())*(blab.getX()-alab.getX())));
      fDisZ[index]->Fill(distZ);
      
      //Distance to (0,0,0)
      part1 = ((-alab.getX())*(blab.getY()-alab.getY())) - 
	((-alab.getY())*(blab.getX()-alab.getX()));
      part2 = ((-alab.getY())*(blab.getZ()-alab.getZ())) - 
	((-alab.getZ())*(blab.getY()-alab.getY()));
      part3 = ((-alab.getZ())*(blab.getX()-alab.getX())) - 
	((-alab.getX())*(blab.getZ()-alab.getZ()));
      numera = (part1*part1)+(part2*part2)+(part3*part3);
      denomi = (blab.getX()-alab.getX())*(blab.getX()-alab.getX())+
	(blab.getY()-alab.getY())*(blab.getY()-alab.getY())+
	(blab.getZ()-alab.getZ())*(blab.getZ()-alab.getZ()); 
      discenter = sqrt(numera/denomi);
      fDisCenter[index]->Fill(discenter);
      
      if(zcY>fZLoCut && zcY<fZHiCut){
      }
    }

    else{ //general case (1MDC or point plus direction)

      HGeomVector aPoint,aVector;
      HGeomVector aPointLab,aVectorLab;
      Int_t sec; 
      Float_t third=0;
      
      //filling from the tree
      aPoint.setX(hitA->getX());
      aPoint.setY(hitA->getY());       
      aPoint.setZ(0.);
      
      third = sqrt(1-hitA->getXDir()*hitA->getXDir()
		   -hitA->getYDir()*hitA->getYDir());
      
      aVector.setX(hitA->getXDir());
      aVector.setY(hitA->getYDir());
      aVector.setZ(third);
      
      sec = hitA->getSector();

      if(GP_DEBUG>3){
	cout << "MDC HIT A. Point: "  << endl; 
	aPoint.print();  
	cout << "Vector: "  << endl; 
	aVector.print();  
      }

      if(fNumMods==0) {//target finder of frustum 
	aPointLab = fRotMat[sec]*aPoint+fTranslation[sec];
	aVectorLab = fRotMat[sec]*aVector;
      }
      else{
	// point coord. in lab system
	aPointLab = fRotLab*aPoint+fTransLab;
	aVectorLab = fRotLab*aVector;
      }

      //Cuts with Z=0
      xcZ = aPointLab.getX()-(aVectorLab.getX())*
	aPointLab.getZ()/(aVectorLab.getZ());
      ycZ = aPointLab.getY()-(aVectorLab.getY())*
	aPointLab.getZ()/(aVectorLab.getZ());     
      fPlanez[index]->Fill(xcZ,ycZ);
      
      //Cuts with Y=0
      xcY = aPointLab.getX() -(aVectorLab.getX())*
	aPointLab.getY()/(aVectorLab.getY());
      zcY = aPointLab.getZ() -(aVectorLab.getZ())*
	aPointLab.getY()/(aVectorLab.getY());
      fPlaney[index]->Fill(xcY,zcY);
      
      //Cuts with X=0
      ycX = aPointLab.getY() -(aVectorLab.getY())*
	aPointLab.getX()/(aVectorLab.getX());
      zcX = aPointLab.getZ() -(aVectorLab.getZ())*
	aPointLab.getX()/(aVectorLab.getX());
      fPlanex[index]->Fill(ycX,zcX);
      
      //Projection
      fProzY[index]->Fill(zcY);      
      fProzX[index]->Fill(zcX);
      fProyZ[index]->Fill(ycZ);      
      fProyX[index]->Fill(ycX);      
      fProxZ[index]->Fill(xcZ);
      fProxY[index]->Fill(xcY);
      
      //more info
      phi = atan2(aVectorLab.getY(),aVectorLab.getX());
      theta = acos((aVectorLab.getZ())/
		   sqrt((aVectorLab.getX())*(aVectorLab.getX())+
			(aVectorLab.getY())*(aVectorLab.getY())+
			(aVectorLab.getZ())*(aVectorLab.getZ())));
      //coordinates of the point where the track passes closer to beam
      ztar = -(ycZ * sin(phi) + xcZ*cos(phi))*cos(theta)/sin(theta);
      xtar = ((aVectorLab.getX())*(ztar-aPointLab.getZ())/
	      (aVectorLab.getZ())) + aPointLab.getX();
      ytar = ((aVectorLab.getY())*(ztar-aPointLab.getZ())/
	      (aVectorLab.getZ())) + aPointLab.getY();
      rtar = ycZ * cos(phi) - xcZ*sin(phi);
      
      fZVsTheta[index]->Fill(theta,ztar); //z vs. theta
      fZVsRho[index]->Fill(rtar,ztar); //z vs. rho
      
      fTargets[index]->Fill(ztar);

      if(fNumMods==0)
	fTarMDC[6*index+sec]->Fill(ztar);
      
      if(fUseAngleCut){
	if(fNumMods==0)
	  distTarMDC = sqrt(fTranslation->getX()*fTranslation->getX()+
			    fTranslation->getY()*fTranslation->getY()+   
			    fTranslation->getZ()*fTranslation->getZ());
	else
	  distTarMDC = sqrt(fTransLab.getX()*fTransLab.getX()+
			    fTransLab.getY()*fTransLab.getY()+   
			    fTransLab.getZ()*fTransLab.getZ());
	
	distAllowed = fabs(fAngleCut*distTarMDC);
	
	distToModulePC=sqrt(aPoint.getX()*aPoint.getX()
			    +aPoint.getY()*aPoint.getY());
	    	
	if(fAngleCut>0 && distToModulePC<distAllowed) ;
	else if(fAngleCut<0 && distToModulePC>distAllowed) ;
	else {
	  fTarCut[index]->Fill(ztar);
	  if(fNumMods==0){	  
	    fTarMDCCut[6*index+sec]->Fill(ztar);
	  }
	}
      }

      //again, with my own calculation (not used in histo but used for testing)
      canaux = ((aVectorLab.getX())/(aVectorLab.getY())) - 
	((aVectorLab.getY())/(aVectorLab.getX()));
      myxtar = (aPointLab.getY()-
		(aPointLab.getX()*(aVectorLab.getY())/
		 (aVectorLab.getX()))) / canaux;
      myytar = (myxtar*(aVectorLab.getY())/(aVectorLab.getX())) + 
	(aPointLab.getY()-aPointLab.getX()*(aVectorLab.getY())/
	 (aVectorLab.getX()));
      myztar = (myxtar*(aVectorLab.getZ())/(aVectorLab.getX())) + 
	(aPointLab.getZ()-aPointLab.getX()*(aVectorLab.getZ())/
	 (aVectorLab.getX()));
      myrho = sqrt(myxtar*myxtar + myytar*myytar);
      
      // if(myrho !=rtar || myxtar!=xtar ||  myytar!=ytar ||
      // 	 myztar!=ztar) {
      //	cout << "calculations differ!!!!!!!!" << endl;
      //	cout << "rho:" << rtar << " "<< myrho << endl;
      //	cout << "xtar:" << xtar << " "<< myxtar<< endl;
      //	cout << "xtar:" << ytar << " "<< myytar<< endl;
      //	cout << "xtar:" << ztar << " "<< myztar<< endl;
      //      }      


      //distance to beam
      distZ = ((aPointLab.getX()*(aVectorLab.getY()))-
	       (aPointLab.getY()*(aVectorLab.getX()))) /
	sqrt(((aVectorLab.getY())*(aVectorLab.getY())) + 
	     ((aVectorLab.getX())*(aVectorLab.getX())));
      fDisZ[index]->Fill(distZ);
      
      part1 = (-aPointLab.getX()*(aVectorLab.getY())) 
	- (-aPointLab.getY()*(aVectorLab.getX()));
      part2 = (-aPointLab.getY()*(aVectorLab.getZ())) 
	- (-aPointLab.getZ()*(aVectorLab.getY()));
      part3 = (-aPointLab.getZ()*(aVectorLab.getX())) 
	- (-aPointLab.getX()*(aVectorLab.getZ()));
      numera = (part1*part1)+(part2*part2)+(part3*part3);
      denomi = aVectorLab.getX()*aVectorLab.getX() 
	+ aVectorLab.getY()*aVectorLab.getY() 
	+ aVectorLab.getZ()*aVectorLab.getZ(); 
      discenter = sqrt(numera/denomi); 
      fDisCenter[index]->Fill(discenter);
      
    }

  
  } // end of for (Int_t i=0; i<nentries;i++) {
}





void HMdcGeomParF :: storeInFile(void)
{  
  //
  // Stores all histos and tree in the Root file
  //
  
  Int_t sector = fLoc[0];
  Int_t modA = fLoc[1];
  Int_t modB = fLoc[2];
  Int_t modC =-1;  
  Int_t modD =-1;  

  //const char* oldDirName = gDirectory->GetPath();
  static Char_t newDirName[255];
  if(fNumMods>3){
    modC =  fLoc[3];
    modD =  fLoc[4];
    sprintf(newDirName,"%s%s%i%s%i%s%i%s%i%s%i","GeomParF_","S_",sector,
	    "_ModA_",modA,"_ModB_",modB,"_ModC_",modC,"_ModD_",modD);
  }
  if(fNumMods>2){
    modC =  fLoc[3];
    sprintf(newDirName,"%s%s%i%s%i%s%i%s%i","GeomParF_","S_",sector,
	    "_ModA_",modA,"_ModB_",modB,"_ModC_",modC);
  }
  else  if(fNumMods>1){
    sprintf(newDirName,"%s%s%i%s%i%s%i","GeomParF_","S_",sector,
		"_ModA_",modA,"_ModB_",modB);
  }
  else if(fNumMods==1) {
    sprintf(newDirName,"%s%s%i%s%i","GeomParF_","S_",sector,
	    "_ModA_",modA);
  }
  else sprintf(newDirName,"%s%s%i","GeomParF_","_Mods_",modA);

  fOutFile->cd(newDirName);
  //fOutFile->cd(oldDirName);
  fOutFile->cd();
  fOutFile->Write();
  fOutFile->Close();
}





void HMdcGeomParF :: writeGeomParContainer(void){ 
  //
  // Writing the new geometrical parameters from minimization results
  //
  // Getting module A parameters from module B 
  // parameters (fRotLab fTransLab)
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
  Int_t moduleA = fLoc[1];
  Int_t moduleB = fLoc[2];
  Int_t moduleC =-1;
  Int_t moduleD =-1;
  
  HGeomRotation rotA,rotB,rotC;
  HGeomVector vectorA,vectorB,vectorC;

  cout << endl << "   ########  FINAL RESULT   ########   "<< endl;
  cout << " Sector: " << sector << "  ModA: " << moduleA 
       << "  ModB: " << moduleB ;
  cout << endl;
  
  if(fNumMods>3){
    moduleC = fLoc[3];
    moduleD = fLoc[4];
    cout << "  ModC: " << moduleC;    
    cout << "  ModD: " << moduleD << endl;

    rotA = fRotLab * fRotMat[2];
    vectorA = fRotLab * fTranslation[2] + fTransLab;
    rotB = fRotLab * fRotMat[1];
    vectorB = fRotLab * fTranslation[1] + fTransLab;
    rotC = fRotLab * fRotMat[0];
    vectorC = fRotLab * fTranslation[0] + fTransLab;    

    cout << "Params for module A: " << endl; 
    rotA.print(); 
    vectorA.print();   
    cout << "Params for module B: " << endl; 
    rotB.print(); 
    vectorB.print();   
    cout << "Params for module C: " << endl; 
    rotC.print(); 
    vectorC.print();   
    cout << "Params for module D: " << endl; 
    fRotLab.print();
    fTransLab.print();

    fMdcGeomPar->getModule(sector,moduleB)
      ->getLabTransform().setRotMatrix(rotB);
    fMdcGeomPar->getModule(sector,moduleB)
      ->getLabTransform().setTransVector(vectorB);
    fMdcGeomPar->getModule(sector,moduleC)
      ->getLabTransform().setRotMatrix(rotC);
    fMdcGeomPar->getModule(sector,moduleC)
      ->getLabTransform().setTransVector(vectorC);
    fMdcGeomPar->getModule(sector,moduleD)
      ->getLabTransform().setRotMatrix(fRotLab);
    fMdcGeomPar->getModule(sector,moduleD)
      ->getLabTransform().setTransVector(fTransLab);
  }
  if(fNumMods>2){
    moduleC = fLoc[3];
    cout << "  ModC: " << moduleC << endl;

    rotA = fRotLab * fRotMat[1];
    vectorA = fRotLab * fTranslation[1] + fTransLab;    
    rotB = fRotLab * fRotMat[0];
    vectorB = fRotLab * fTranslation[0] + fTransLab;

    cout << "Params for module A: " << endl; 
    rotA.print(); 
    vectorA.print();   
    cout << "Params for module B: " << endl; 
    rotB.print(); 
    vectorB.print();   
    cout << "Params for module C: " << endl; 
    fRotLab.print();
    fTransLab.print();

    fMdcGeomPar->getModule(sector,moduleB)
      ->getLabTransform().setRotMatrix(rotB);
    fMdcGeomPar->getModule(sector,moduleB)
      ->getLabTransform().setTransVector(vectorB);    
    fMdcGeomPar->getModule(sector,moduleC)
      ->getLabTransform().setRotMatrix(fRotLab);
    fMdcGeomPar->getModule(sector,moduleC)
      ->getLabTransform().setTransVector(fTransLab);
  }
  else if(fNumMods>1){
    rotA = fRotLab * fRotMat[0];
    vectorA = fRotLab * fTranslation[0] + fTransLab;    
    fMdcGeomPar->getModule(sector,moduleB)
      ->getLabTransform().setRotMatrix(fRotLab);
    fMdcGeomPar->getModule(sector,moduleB)
      ->getLabTransform().setTransVector(fTransLab);
  }
  else{
    rotA = fRotLab;
    vectorA = fTransLab; 
  }
  if(fNumMods>0){
    fMdcGeomPar->getModule(sector,moduleA)
      ->getLabTransform().setRotMatrix(rotA);
    fMdcGeomPar->getModule(sector,moduleA)
      ->getLabTransform().setTransVector(vectorA);
  }
  if(fNumMods==2) {
    cout << "Params for module A: " << endl; 
    rotA.print(); 
    vectorA.print();   
    cout << "Params for module B: " << endl; 
    fRotLab.print();
    fTransLab.print();
  }

  if(GP_DEBUG>1){
    if(fMdcGeomPar->hasChanged()==kTRUE) cout << " Flag es kTRUE "<< endl;
    else cout << " Flag es kFALSE "<< endl;
    cout << "Input version[1] = " << fMdcGeomPar->getInputVersion(1)<< endl;
    cout << "Input version[2] = " << fMdcGeomPar->getInputVersion(2)<< endl;
  }
  
  fMdcGeomPar->setChanged();
  
  Int_t v = fMdcGeomPar->getInputVersion(1);
  if (v>0) fMdcGeomPar->setInputVersion(++v,1); 
  else {
    v=fMdcGeomPar->getInputVersion(2);
    fMdcGeomPar->setInputVersion(++v,2);     
  } 
  
  if(GP_DEBUG>1){
    if(fMdcGeomPar->hasChanged()==kTRUE) cout << " Ahora flag es kTRUE "<< endl;
    else cout << " Ahora flag es kFALSE "<< endl;
    cout << "Input version[1] = " << fMdcGeomPar->getInputVersion(1)<< endl;
    cout << "Input version[2] = " << fMdcGeomPar->getInputVersion(2)<< endl;
  }
}





void HMdcGeomParF :: fillRotMatrix(Int_t i,Float_t prim,
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
  fRotMat[i].setEulerAngles(prim*rad2deg,segu*rad2deg,terc*rad2deg);
} 




void HMdcGeomParF :: fillTranslation(Int_t i,Float_t x,Float_t y, Float_t z){ 
  //
  // Translation from MDC A to MDC B
  //
 
  fTranslation[i].setX(x); 
  fTranslation[i].setY(y);
  fTranslation[i].setZ(z); 

}





void   HMdcGeomParF :: fillRotLab(Float_t prim,Float_t segu, Float_t terc){
  //
  // Fill a matrix using the Euler angles of the laboratory transformation
  // This matrix correspond to R
  // X(LAB) = R X(B) + T
  //
  const double rad2deg = 57.29577951308;  
  fRotLab.setEulerAngles(prim*rad2deg,segu*rad2deg,terc*rad2deg);

}





void HMdcGeomParF :: fillTransLab(Float_t x,Float_t y, Float_t z){
  //
  // Lab Translation 
  //

  fTransLab.setX(x); 
  fTransLab.setY(y);
  fTransLab.setZ(z); 

}
  




void HMdcGeomParF :: setEulerAngles(Int_t ind, Float_t f,Float_t s, Float_t t){
  //
  // Setting the Euler angles of the relative transformation
  //
 
  fEuler[ind].setX(f); 
  fEuler[ind].setY(s); 
  fEuler[ind].setZ(t); 
} 




  
void HMdcGeomParF :: setEulerLabAngles(Float_t f,Float_t s, Float_t t){ 
  //
  // Setting the Euler angles of the laboratory transformation
  //
 
  fEulerLab.setX(f); 
  fEulerLab.setY(s); 
  fEulerLab.setZ(t); 
} 





void HMdcGeomParF :: setSearchLimits(Float_t x, Float_t y){ 
  //
  // Limits in Z 
  //
 
  fZLoCut = x; 
  fZHiCut = y; 
} 





void HMdcGeomParF :: setIterCriteria(Float_t cri){ 
  //
  // Set the criteria for iteration in the minimization (see finalize())
  //
 
  fIterCriteria= cri; 
}





void HMdcGeomParF :: setSteps(Float_t s0,Float_t s1,Float_t s2,
			      Float_t s3, Float_t s4, Float_t s5){ 
  //
  // Set the steps in the minimization
  //
 
  fSteps[0]= s0; 
  fSteps[1]= s1; 
  fSteps[2]= s2; 
  fSteps[3]= s3;   
  fSteps[4]= s4; 
  fSteps[5]= s5;
  cout << "Steps in the minimization adjusted to " << s0 << ", " 
       << s1 << ", " << s2 << ", " << s3 
       << ", " << s4 << ", " << s5 << endl;
}





void HMdcGeomParF :: setLimits(Float_t l0,Float_t l1,Float_t l2,
			       Float_t l3, Float_t l4, Float_t l5){ 
  //
  // Set the criteria for iteration in the minimization (see finalize())
  //
 
  fLimits[0]= l0; 
  fLimits[1]= l1; 
  fLimits[2]= l2; 
  fLimits[3]= l3; 
  fLimits[4]= l4;   
  fLimits[5]= l5; 
}





void HMdcGeomParF :: minfit(Int_t fix, Float_t fE, Float_t sE, 
			    Float_t tE, Float_t fT, Float_t sT, Float_t tT){
  //
  // Minuit menu
  // Argument fix correspon to fFix value (see setFix())
  // Other arguments are init values for the parameters!
  //

  Double_t args[8]; 
  Int_t err = 0; 
  Float_t* limitL;
  Float_t* limitH;
  limitL = new Float_t[6];
  limitH = new Float_t[6];
 
  //This depends on MDCA and MDCB 
  Double_t start_val[]={fE, sE, tE, fT, sT, tT};
  
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
  minuit->SetFCN(fcnGP); 
  minuit->SetObjectFit(this);

  if(GP_DEBUG>0){
    cout << "HMdcGeomParF::minfit()" <<endl;
    cout << "Start Values for initialization: " << start_val[0] << "," 
	 << start_val[1] <<  "," << start_val[2] << "," << start_val[3] 
	 <<  "," << start_val[4] << "," << start_val[5] << endl;
  }

  minuit->mnparm(0,"par0",start_val[0],fSteps[0],limitL[0],limitH[0],err); 
  minuit->mnparm(1,"par1",start_val[1],fSteps[1],limitL[1],limitH[1],err); 
  minuit->mnparm(2,"par2",start_val[2],fSteps[2],limitL[2],limitH[2],err); 
  minuit->mnparm(3,"par3",start_val[3],fSteps[3],limitL[3],limitH[3],err); 
  minuit->mnparm(4,"par4",start_val[4],fSteps[4],limitL[4],limitH[4],err); 
  minuit->mnparm(5,"par5",start_val[5],fSteps[5],limitL[5],limitH[5],err);

  //Z Always fixed!!!
  minuit->FixParameter(5);
  //FIXING 
  if(fix>0 && fix<7) minuit->FixParameter(fix+1);
  //FIXING 
  if(fix == 30) minuit->FixParameter(0);     
  if(fix == 30) minuit->FixParameter(1); 
  if(fix == 30) minuit->FixParameter(2); 
    //FIXING 
  if(fix == 20) minuit->FixParameter(3);     
  if(fix == 20) minuit->FixParameter(4); 
  if(fix == 20) minuit->FixParameter(5); 
  

  //To remove all printout  
  if(GP_DEBUG<3){ 
    minuit->SetPrintLevel(-1);
  }
  
  //args is the array of the numerical arguments of the command 
  //the third field is the number of arguments esp3cified 
  //For MIGRAD arguments are maxcalls and tolerance, both optionals 
  minuit->mnexcm("MIGRAD",args,0,err); 
  //minuit->mnexcm("SIMPLEX",args,0,err); 
  
  Double_t parresult[6];
  minuit->GetParameter(0,parresult[0],fErrorLab[0]);
  minuit->GetParameter(1,parresult[1],fErrorLab[1]);
  minuit->GetParameter(2,parresult[2],fErrorLab[2]);
  minuit->GetParameter(3,parresult[3],fErrorLab[3]);
  minuit->GetParameter(4,parresult[4],fErrorLab[4]);
  minuit->GetParameter(5,parresult[5],fErrorLab[5]);
  fEulerLab.setX(parresult[0]);
  fEulerLab.setY(parresult[1]);
  fEulerLab.setZ(parresult[2]); 
  fTransLab.setX(parresult[3]);  
  fTransLab.setY(parresult[4]);
  fTransLab.setZ(parresult[5]);
  fillRotLab(fEulerLab(0),fEulerLab(1),fEulerLab(2));

  if (err != 0) cout << endl <<"MINIMIZATION EXITED WITH ERROR " 
		     << err << endl;
}





void fcnGP(Int_t &npar, Double_t *gin, Double_t &f, 
	   Double_t *par, Int_t iflag){  
  //
  //This function contains the functional to minimize 
  //

  Double_t chisq = 0.;       
  HGeomRotation rotlab;
  Double_t translab[3];   
  HGeomRotation rotrel;
  HGeomVector transrel;      

  Float_t pxa, pya, ps0a, ps1a, pxb, pyb, ps0b, ps1b;
  Float_t fxalab, fyalab, fzalab, fxblab, fyblab, fzblab;
  Float_t xanew, yanew, zanew;
  Double_t prim, segu, terc; 
  Float_t xDir,yDir,aux;

  prim = par[0]; 
  segu = par[1]; 
  terc = par[2]; 
  translab[0] = par[3]; 
  translab[1] = par[4]; 
  translab[2] = par[5]; 

  if (GP_DEBUG>2){
    cout << "HMdcGeomParF::fcnGP()  Parameters: " << par[0] << "," 
	 << par[1] << "," <<  par[2] << "," << par[3] 
	 << ","  << par[4] << ","  << par[5] << endl;
  }
  
  rotlab.setEulerAngles(prim*180./TMath::Pi(),
			segu*180./TMath::Pi(),
			terc*180./TMath::Pi()); 
  /*
0  rotlab[0][0]=(cos(prim)*cos(segu)*cos(terc))-(sin(prim)*sin(terc));
1  rotlab[0][1]=(-cos(prim)*cos(segu)*sin(terc))-(sin(prim)*cos(terc));
2  rotlab[0][2]=(cos(prim) * sin(segu));
3  rotlab[1][0]=(sin(prim)*cos(segu)*cos(terc))+(cos(prim)*sin(terc));
4  rotlab[1][1]=(-sin(prim)*cos(segu)*sin(terc))+(cos(prim)*cos(terc));
5  rotlab[1][2]=(sin(prim) * sin(segu));
6  rotlab[2][0]=( - sin(segu) * cos(terc));
7  rotlab[2][1]=(sin(segu) * sin(terc));
8  rotlab[2][2]=(cos(segu));
  */

  TTree* pData; 
  HMdcGeomParF* pGeomParF; 
  TClonesArray* theHits;  
  theHits = new TClonesArray("HMdcHit",2);
  pGeomParF = (HMdcGeomParF*)(gMinuit->GetObjectFit());
  pData = pGeomParF->getTree();
  rotrel = pGeomParF->getRotMatrix(0);
  transrel = pGeomParF->getTranslation(0);

  TBranch *branch = pData->GetBranch("hits");
  branch->SetAddress(&theHits);

  Stat_t entries = pData->GetEntries();

  TH1F *targets =new TH1F("que","pasa",800,-200,200);
  TF1 *f1 = new TF1("f1","gaus",-100,100);

  HMdcHit* hitA;
  HMdcHit* hitB;
  
  //loop on hits   
  for (Int_t i=0;i<entries;i++) {     
    theHits->Clear();
    pData->GetEntry(i);
    //data = pData->GetArgs();
    //filling from the Tree    
    //hitB = (HMdcHit*) theHits->At(0);
    //hitA = (HMdcHit*) theHits->At(1);    
    //TESTING FOR NEW
    hitB = (HMdcHit*) theHits->At(1);
    hitA = (HMdcHit*) theHits->At(0);    
   
    //filling from the ntuple
    pxa = hitA->getX();
    pya = hitA->getY();       
    xDir = hitA->getXDir();
    yDir = hitA->getYDir(); 
    //If using HMdcHit, it is neccesary this change
    aux = sqrt(1 - xDir * xDir - yDir * yDir);
    if(aux == 0.) {
      ps0a=1; 
      ps1a=1; 
      cout<< "ERROR #1 in HMdcGeomParF::fcnGP().";
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
      cout<< "ERROR #2 in HMdcGeomParF::fcnGP().";
    }
    else{
      ps0b = xDir/aux;
      ps1b = yDir/aux;
    }

    //coord. of MDC A in MDC B system:
    // X(B) = M X(A) + V
    //Remember: rotrel is a copy of fRotMat
    //  rot(0) rot(3) rot(6)     fRotMat[0][0] fRotMat[0][1] fRotMat[0][2]
    //  rot(1) rot(4) rot(7)  =  fRotMat[1][0] fRotMat[1][1] fRotMat[1][2]
    //  rot(2) rot(5) rot(8)     fRotMat[2][0] fRotMat[2][1] fRotMat[2][2]

    xanew = rotrel(0)*pxa+rotrel(1)*pya+transrel(0);
    yanew = rotrel(3)*pxa+rotrel(4)*pya+transrel(1);
    zanew = rotrel(6)*pxa+rotrel(7)*pya+transrel(2);

    // point coord. in lab system
    fxalab = rotlab(0)*xanew+rotlab(1)*yanew+rotlab(2)*zanew+translab[0];
    fyalab = rotlab(3)*xanew+rotlab(4)*yanew+rotlab(5)*zanew+translab[1];
    fzalab = rotlab(6)*xanew+rotlab(7)*yanew+rotlab(8)*zanew+translab[2];
    
    fxblab = rotlab(0)*pxb+rotlab(1)*pyb+translab[0];
    fyblab = rotlab(3)*pxb+rotlab(4)*pyb+translab[1];
    fzblab = rotlab(6)*pxb+rotlab(7)*pyb+translab[2];

    //probando minimizar la distancia al eje Z!
    Float_t distZ = ( (fxalab*(fyblab-fyalab))-(fyalab*(fxblab-fxalab)) ) /
      sqrt( ((fyblab-fyalab)*(fyblab-fyalab)) + 
	    ((fxblab-fxalab)*(fxblab-fxalab)) );

    if(GP_DEBUG >3){
      cout << "  ++++++++  VALUES IN fcnGP()  +++++++++ " << endl;
      cout << "HITA: " << pxa << ", " << pya << ", " 
	   << ps0a << ", " << ps1a << endl;
      cout << "HITB: " << pxb << ", " << pyb << ", " 
	   << ps0b << ", " << ps1b << endl;      
      cout << "HITA IN MDC B CS: " << xanew << ", " << yanew << ", " 
	   << zanew << endl;
      cout << "HITA IN LAB CS: " << fxalab << ", " << fyalab << ", " 
	   << fzalab << endl;
      cout << "HITB IN LAB CS: " << fxblab << ", " << fyblab << ", " 
	   << fzblab << endl;
      cout << "Distance: " << distZ  << endl;
    }

    //Cuts with Z=0
    Float_t xcZ = fxalab-(fxblab-fxalab)*fzalab/(fzblab-fzalab);
    Float_t ycZ = fyalab-(fyblab-fyalab)*fzalab/(fzblab-fzalab);     
    
    //Cuts with Y=0
    //    Float_t xcY = fxalab -(fxblab-fxalab)*fyalab/(fyblab-fyalab);
    //    Float_t zcY = fzalab -(fzblab-fzalab)*fyalab/(fyblab-fyalab);
    
    //Cuts with X=0
    //    Float_t ycX = fyalab -(fyblab-fyalab)*fxalab/(fxblab-fxalab);
    //    Float_t zcX = fzalab -(fzblab-fzalab)*fxalab/(fxblab-fxalab);
    
    //more info
    Float_t phi = atan2(fyblab-fyalab,fxblab-fxalab);
    Float_t theta = acos((fzblab-fzalab)/sqrt((fxblab-fxalab)*(fxblab-fxalab)+
				    (fyblab-fyalab)*(fyblab-fyalab)+
				    (fzblab-fzalab)*(fzblab-fzalab)));
    //coordinates of the point where the track passes closer to beam
    Float_t ztar = -(ycZ * sin(phi) + xcZ*cos(phi))*cos(theta)/sin(theta);

    targets->Fill(ztar);


    //chisq += distZ*distZ/50.;

  }

  targets->Fit("f1","RQN");
  Float_t fitPar1     = f1->GetParameter(1);  // mean

  f = fitPar1;
     
  if(GP_DEBUG>2){
    cout  << "chisqr= " << chisq << " out of " << entries 
	  << " combinations."<< endl; 
  }
}





void  HMdcGeomParF :: findZComponent(void)
{
  //
  //Finding the third component of the translation
  //

  HGeomVector a,b,ainb,alab,blab;
  Float_t xcZ, ycZ;
  Float_t phi, theta,ztar, rtar, xtar, ytar;

  Int_t sector = fLoc[0];
  Int_t modA = fLoc[1];
  Int_t modB = fLoc[2];  

  Char_t title[50], tmp[50];
  sprintf(title,"%s%i%s%i%s%i","Sector_",sector,"_ModA_",modA,"_ModB_",modB);
  sprintf(tmp,"%s%s%i%s%i%s%i","Mtarget","_Sector_",sector,"_ModA_",modA,"_ModB_",modB);
  TH1F *targets2 =new TH1F(tmp,title,800,-200,200);

  TBranch *branch = fTree->GetBranch("hits");
  branch->SetAddress(&fHits);

  Stat_t entries = fTree->GetEntries();

  HMdcHit* hitA;
  HMdcHit* hitB;

  //loop on hits   
  for (Int_t i=0;i<entries;i++) { 
    fHits->Clear();
    fTree->GetEntry(i);
    //    data = fTree->GetArgs();
    
    //filling from the Tree    
    //hitB = (HMdcHit*) fHits->At(0);
    //hitA = (HMdcHit*) fHits->At(1);    
    //TESTING FOR NEW
    hitB = (HMdcHit*) fHits->At(1);
    hitA = (HMdcHit*) fHits->At(0);
 
    //filling from the tree
    a.setX(hitA->getX());
    a.setY(hitA->getY());       
    b.setX(hitB->getX());
    b.setY(hitB->getY());
    a.setZ(0.);
    b.setZ(0.);

    //coord. of MDC A in MDC B system:
    // X(B) = M X(A) + V    
    ainb = fRotMat[0]*a+fTranslation[0];

    // point coord. in lab system
    alab = fRotLab*ainb+fTransLab;
    blab = fRotLab*b+fTransLab;

    //Cuts with Z=0
    xcZ = alab.getX()-(blab.getX()-alab.getX())*
      alab.getZ()/(blab.getZ()-alab.getZ());
    ycZ = alab.getY()-(blab.getY()-alab.getY())*
      alab.getZ()/(blab.getZ()-alab.getZ());     
    phi = atan2(blab.getY()-alab.getY(),blab.getX()-alab.getX());
    theta = acos((blab.getZ()-alab.getZ())/
		 sqrt((blab.getX()-alab.getX())*(blab.getX()-alab.getX())+
		      (blab.getY()-alab.getY())*(blab.getY()-alab.getY())+
		      (blab.getZ()-alab.getZ())*(blab.getZ()-alab.getZ())));
    
    //coordinates of the point where the track passes closer to beam
    ztar = -(ycZ * sin(phi) + xcZ*cos(phi))*cos(theta)/sin(theta);
    xtar = ((blab.getX()-alab.getX())*(ztar-alab.getZ())/
	    (blab.getZ()-alab.getZ())) + alab.getX();
    ytar = ((blab.getY()-alab.getY())*(ztar-alab.getZ())/
	    (blab.getZ()-alab.getZ())) + alab.getY();
    rtar = ycZ * cos(phi) - xcZ*sin(phi);
    
    if((xtar*xtar + ytar*ytar)<10 )      targets2->Fill(ztar);    
  }
  TF1 *f1 = new TF1("f1","gaus",-200,200);
  targets2->Fit("f1","RQN");
  //Float_t fitPar0     = f1->GetParameter(0);  // constant
  Float_t fitPar1     = f1->GetParameter(1);  // mean
  //Float_t fitPar2     = f1->GetParameter(2);  // sigma
  
  fTransLab.setZ(fTransLab(2)-fitPar1);
  
}

