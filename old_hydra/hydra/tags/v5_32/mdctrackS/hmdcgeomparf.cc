# include <math.h>
# include <stdlib.h>
# include <iomanip.h>
# include <fstream.h>
# include <TMath.h>
# include <TROOT.h>
# include <TF1.h>
# include "hmdcgeomparf.h"
# include "hades.h"
# include "hdataobject.h"
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
// As HMdcAligner, this version is intended to work with pairs
// of MDCs. Later, will be extended to three or four in a sector.
//
////////////////////////////////////////////////////////////////////////

   Int_t GP_DEBUG=0;

HMdcGeomParF :: HMdcGeomParF(void) : HReconstructor()
{  
  //
  // Default constructor. 
  //

  fLocA.setNIndex(2);
  fLocB.setNIndex(2);
  fManual =kFALSE;
  fZLoCut = -100;  
  fZHiCut = 100;
  fLocA.set(2,0,0); //MDC A  dummy
  fLocB.set(2,0,1); //MDC B  dummy
  fTree = NULL;
  fOutFile = NULL;
  fHits = new TClonesArray("HMdcHit",2);
}
 




HMdcGeomParF :: HMdcGeomParF(Int_t sector, Int_t modA, Int_t modB) : HReconstructor()
{  
  //
  // Constructor including module election. Alignment procedure 
  // proyects hits of modA in modB coordinate system and compares
  //

  fLocA.setNIndex(2);
  fLocB.setNIndex(2);
  fManual =kFALSE;
  fZLoCut = -100;  
  fZHiCut = 100;
  fLocA.set(2,sector,modA); //MDC A 
  fLocB.set(2,sector,modB); //MDC B     
  fTree = NULL;
  fOutFile = NULL;  
  fHits = new TClonesArray("HMdcHit",2);
}

HMdcGeomParF :: HMdcGeomParF(Text_t* name, Text_t* title, Int_t sector, Int_t modA, Int_t modB) : HReconstructor(name, title)
{  
  //
  // Constructor including module election. Alignment procedure 
  // proyects hits of modA in modB coordinate system and compares
  // USE THIS ONE IN MACROS
  //

  fLocA.setNIndex(2);
  fLocB.setNIndex(2);
  fManual =kFALSE;
  fZLoCut = -100;  
  fZHiCut = 100;
  fLocA.set(2,sector,modA); //MDC A 
  fLocB.set(2,sector,modB); //MDC B   
  fTree = NULL;
  fOutFile = NULL;  
  fHits = new TClonesArray("HMdcHit",2);
}





HMdcGeomParF :: ~HMdcGeomParF(void)
{    
  //
  // Destructor. Makes nothing
  //

}





void HMdcGeomParF :: setRelParams(HGeomVector* ang, HGeomVector* tra)
{    
  //
  // Sets the pointers to previous results of HMdcAligner.
  // This function should be called in the macro
  //
  fEulerResult = ang;
  fTransResult = tra;  
  //  cout << " HMdcGeomParF :: setRelParams(). Reading " << endl;
  //  cout << " fEulerResult: " << fEulerResult << endl; 
  //  cout << " fTransResults: " << fTransResult << endl;
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




void HMdcGeomParF :: setHistograms(void)
{
  //
  // Inits histograms
  //

  Int_t sector = fLocA[0];
  Int_t modA = fLocA[1];
  Int_t modB = fLocB[1];

  const char* oldDirName = gDirectory->GetPath();
  static Char_t newDirName[255];
  sprintf(newDirName,"%s%s%i%s%i%s%i","GeomParF_","S_",sector,"_ModA_",modA,"_ModB_",modB);
  fOutFile->mkdir(newDirName,newDirName);
  fOutFile->cd(newDirName);
  
  Char_t title[50], tmp[50];
  sprintf(title,"%s%i%s%i%s%i","Sector_",sector,"_ModA_",modA,"_ModB_",modB);
  sprintf(tmp,"%s%s%i%s%i%s%i","fFPlanex","_Sector_",sector,"_ModA_",modA,"_ModB_",modB);
  fFPlanex = new TH2F(tmp,title,100,-1000,1000,100,-1000,1000);  
  sprintf(tmp,"%s%s%i%s%i%s%i","fFPlaney","_Sector_",sector,"_ModA_",modA,"_ModB_",modB);
  fFPlaney = new TH2F(tmp,title,100,-1000,1000,100,-1000,1000);   
  sprintf(tmp,"%s%s%i%s%i%s%i","fFPlanez","_Sector_",sector,"_ModA_",modA,"_ModB_",modB);
  fFPlanez = new TH2F(tmp,title,100,-1000,1000,100,-1000,1000);
 
  sprintf(tmp,"%s%s%i%s%i%s%i","fFTargets","_Sector_",sector,"_ModA_",modA,"_ModB_",modB);
  fFTargets = new TH1F(tmp,title,2000,-1000,1000);     
  sprintf(tmp,"%s%s%i%s%i%s%i","fFZVsTheta","_Sector_",sector,"_ModA_",modA,"_ModB_",modB);
  fFZVsTheta = new TH2F(tmp,title,200,0,2,200,-1000,1000);  
  sprintf(tmp,"%s%s%i%s%i%s%i","fFZVsRho","_Sector_",sector,"_ModA_",modA,"_ModB_",modB);
  fFZVsRho = new TH2F(tmp,title,200,-100,100,200,-1000,1000);       

  sprintf(tmp,"%s%s%i%s%i%s%i","fFDisZ","_Sector_",sector,"_ModA_",modA,"_ModB_",modB);
  fFDisZ = new TH1F(tmp,title,200,-100,100);  
  sprintf(tmp,"%s%s%i%s%i%s%i","fFDisCenter","_Sector_",sector,"_ModA_",modA,"_ModB_",modB);
  fFDisCenter = new TH1F(tmp,title,200,0,1000);     
  
  sprintf(tmp,"%s%s%i%s%i%s%i","fFProzY","_Sector_",sector,"_ModA_",modA,"_ModB_",modB);
  fFProzY = new TH1F(tmp,title,200,-1000,1000);
  sprintf(tmp,"%s%s%i%s%i%s%i","fFProzX","_Sector_",sector,"_ModA_",modA,"_ModB_",modB);
  fFProzX = new TH1F(tmp,title,200,-1000,1000);
  sprintf(tmp,"%s%s%i%s%i%s%i","fFProyZ","_Sector_",sector,"_ModA_",modA,"_ModB_",modB);
  fFProyZ = new TH1F(tmp,title,100,-500,500);
  sprintf(tmp,"%s%s%i%s%i%s%i","fFProyX","_Sector_",sector,"_ModA_",modA,"_ModB_",modB);
  fFProyX = new TH1F(tmp,title,100,-500,500);
  sprintf(tmp,"%s%s%i%s%i%s%i","fFProxZ","_Sector_",sector,"_ModA_",modA,"_ModB_",modB);
  fFProxZ = new TH1F(tmp,title,100,-500,500);
  sprintf(tmp,"%s%s%i%s%i%s%i","fFProxY","_Sector_",sector,"_ModA_",modA,"_ModB_",modB);
  fFProxY = new TH1F(tmp,title,100,-200,200);

  //sprintf(tmp,"%s%s%i%s%i%s%i","fFProzYAC","_Sector_",sector,"_ModA_",modA,"_ModB_",modB);
  //fFProzYAC = new TH1F(tmp,title,200,-1000,1000);
  //sprintf(tmp,"%s%s%i%s%i%s%i","fFProzXAC","_Sector_",sector,"_ModA_",modA,"_ModB_",modB);
  //fFProzXAC = new TH1F(tmp,title,200,-1000,1000);
  //sprintf(tmp,"%s%s%i%s%i%s%i","fFProyZAC","_Sector_",sector,"_ModA_",modA,"_ModB_",modB);
  //fFProyZAC = new TH1F(tmp,title,100,-500,500);
  //sprintf(tmp,"%s%s%i%s%i%s%i","fFProyXAC","_Sector_",sector,"_ModA_",modA,"_ModB_",modB);
  //fFProyXAC = new TH1F(tmp,title,100,-500,500);
  //sprintf(tmp,"%s%s%i%s%i%s%i","fFProxZAC","_Sector_",sector,"_ModA_",modA,"_ModB_",modB);
  //fFProxZAC = new TH1F(tmp,title,100,-500,500);
  //sprintf(tmp,"%s%s%i%s%i%s%i","fFProxYAC","_Sector_",sector,"_ModA_",modA,"_ModB_",modB);
  //fFProxYAC = new TH1F(tmp,title,100,-200,200);

  sprintf(tmp,"%s%s%i%s%i%s%i","fPlanex","_Sector_",sector,"_ModA_",modA,"_ModB_",modB);
  fPlanex = new TH2F(tmp,title,100,-1000,1000,100,-1000,1000);  
  sprintf(tmp,"%s%s%i%s%i%s%i","fPlaney","_Sector_",sector,"_ModA_",modA,"_ModB_",modB);
  fPlaney = new TH2F(tmp,title,100,-1000,1000,100,-1000,1000);   
  sprintf(tmp,"%s%s%i%s%i%s%i","fPlanez","_Sector_",sector,"_ModA_",modA,"_ModB_",modB);
  fPlanez = new TH2F(tmp,title,100,-1000,1000,100,-1000,1000);
 
  sprintf(tmp,"%s%s%i%s%i%s%i","fTargets","_Sector_",sector,"_ModA_",modA,"_ModB_",modB);
  fTargets = new TH1F(tmp,title,200,-1000,1000);     
  sprintf(tmp,"%s%s%i%s%i%s%i","fZVsTheta","_Sector_",sector,"_ModA_",modA,"_ModB_",modB);
  fZVsTheta = new TH2F(tmp,title,200,0,2,200,-1000,1000);  
  sprintf(tmp,"%s%s%i%s%i%s%i","fZVsRho","_Sector_",sector,"_ModA_",modA,"_ModB_",modB);
  fZVsRho = new TH2F(tmp,title,200,-100,100,200,-1000,1000);       

  sprintf(tmp,"%s%s%i%s%i%s%i","fDisZ","_Sector_",sector,"_ModA_",modA,"_ModB_",modB);
  fDisZ = new TH1F(tmp,title,200,-100,100);  
  sprintf(tmp,"%s%s%i%s%i%s%i","fDisCenter","_Sector_",sector,"_ModA_",modA,"_ModB_",modB);
  fDisCenter = new TH1F(tmp,title,200,0,1000);     
  
  sprintf(tmp,"%s%s%i%s%i%s%i","fProzY","_Sector_",sector,"_ModA_",modA,"_ModB_",modB);
  fProzY = new TH1F(tmp,title,200,-1000,1000);
  sprintf(tmp,"%s%s%i%s%i%s%i","fProzX","_Sector_",sector,"_ModA_",modA,"_ModB_",modB);
  fProzX = new TH1F(tmp,title,200,-1000,1000);
  sprintf(tmp,"%s%s%i%s%i%s%i","fProyZ","_Sector_",sector,"_ModA_",modA,"_ModB_",modB);
  fProyZ = new TH1F(tmp,title,100,-500,500);
  sprintf(tmp,"%s%s%i%s%i%s%i","fProyX","_Sector_",sector,"_ModA_",modA,"_ModB_",modB);
  fProyX = new TH1F(tmp,title,100,-500,500);
  sprintf(tmp,"%s%s%i%s%i%s%i","fProxZ","_Sector_",sector,"_ModA_",modA,"_ModB_",modB);
  fProxZ = new TH1F(tmp,title,100,-500,500);
  sprintf(tmp,"%s%s%i%s%i%s%i","fProxY","_Sector_",sector,"_ModA_",modA,"_ModB_",modB);
  fProxY = new TH1F(tmp,title,100,-200,200);

  //sprintf(tmp,"%s%s%i%s%i%s%i","fProzYAC","_Sector_",sector,"_ModA_",modA,"_ModB_",modB);
  //fProzYAC = new TH1F(tmp,title,200,-1000,1000);
  //sprintf(tmp,"%s%s%i%s%i%s%i","fProzXAC","_Sector_",sector,"_ModA_",modA,"_ModB_",modB);
  //fProzXAC = new TH1F(tmp,title,200,-1000,1000);
  //sprintf(tmp,"%s%s%i%s%i%s%i","fProyZAC","_Sector_",sector,"_ModA_",modA,"_ModB_",modB);
  //fProyZAC = new TH1F(tmp,title,100,-500,500);
  //sprintf(tmp,"%s%s%i%s%i%s%i","fProyXAC","_Sector_",sector,"_ModA_",modA,"_ModB_",modB);
  //fProyXAC = new TH1F(tmp,title,100,-500,500);
  //sprintf(tmp,"%s%s%i%s%i%s%i","fProxZAC","_Sector_",sector,"_ModA_",modA,"_ModB_",modB);
  //fProxZAC = new TH1F(tmp,title,100,-500,500);
  //sprintf(tmp,"%s%s%i%s%i%s%i","fProxYAC","_Sector_",sector,"_ModA_",modA,"_ModB_",modB);
  //fProxYAC = new TH1F(tmp,title,100,-200,200);

  fOutFile->cd(oldDirName);
}





Bool_t HMdcGeomParF :: init(void)
{
  //
  //Inits all containers
  //

  /*
  Int_t sector = fLocA[0];
  Int_t modA = fLocA[1];
  Int_t modB = fLocB[1];

  setParContainers(); 

  if(fManual == kFALSE) setGeomAuxPar();

  if(fHistograms!=kTRUE) setHistograms();

  //Filling the NTuple from the HYDRA output file!
  Char_t tmp[50];
  sprintf(tmp,"%s%s%i%s%i%s%i","All","_Sector_",sector,"_ModA_",modA,"_ModB_",modB);

  gHades->getOutputFile()->cd();
  fNtPairs = (TNtuple *)gDirectory->Get(tmp);
  */

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

  Int_t sector = fLocA[0];
  Int_t modA = fLocA[1];
  Int_t modB = fLocB[1];

  //setParContainers(); 

  //if(fManual == kFALSE) setGeomAuxPar();

  if(fEulerResult && fTransResult) {
    setEulerAngles(fEulerResult->getX(),fEulerResult->getY(),fEulerResult->getZ());
    fillRotMatrix(fEulerResult->getX(),fEulerResult->getY(),fEulerResult->getZ());
    fillTranslation(fTransResult->getX(),fTransResult->getY(),fTransResult->getZ());
  }
  else cout << "FATAL ERROR: no relative transformation found!" << endl;

  //if(fHistograms!=kTRUE) setHistograms();

  //Filling the NTuple from the HYDRA output file!
  //const char* oldDirName = gDirectory->GetPath();

  //gHades->getOutputFile()->cd();
  //Char_t tmp[255];
  //sprintf(tmp,"%s%s%i%s%i%s%i","Aligner_","S_",sector,"_ModA_",modA,"_ModB_",modB);
  //gHades->getOutputFile()->cd(tmp);
  //sprintf(tmp,"%s%s%i%s%i%s%i","All","_Sector_",sector,"_ModA_",modA,"_ModB_",modB);
  //fTree = (TTree *)gDirectory->Get(tmp);

  //gHades->getOutputFile()->cd();
  //gDirectory->cd(oldDirName);

  //Intentemoslo de  otra manera con un file indep.
    fOutFile = new TFile(fNameRoot,"UPDATE");
    Char_t tmp[255];
    //sprintf(tmp,"%s%s%i%s%i%s%i","Aligner_","S_",sector,"_ModA_",modA,"_ModB_",modB);
    //fOutFile->cd(tmp);
    sprintf(tmp,"%s%s%i%s%i%s%i","All","_Sector_",sector,"_ModA_",modA,"_ModB_",modB);
    fTree = (TTree *)fOutFile->Get(tmp);
    if(!fTree) cout << "ERROR in HMdcGeomParF::initDelayed(): No Tree found in file " <<endl;
    //fOutFile->Cd("..");

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
  cout << "WARNING: Introducing manually Geometrical Parameters" << endl; 
  cout << "without containers. Parameters should be in the macro" << endl;

}





void HMdcGeomParF :: setControlHistoOff(void)
{
  //
  // Disables control histograms output (saving memory)
  //

  fHistograms = kTRUE;  
}





void HMdcGeomParF :: setParContainers(void)
{
  //
  // Loads the parameter containers it uses later
  //

  fMdcGeomPar=(HMdcGeomPar*)gHades->getRuntimeDb()->getContainer("MdcGeomPar");
  if(!fMdcGeomPar){
    cout << "  ####### >createDetGeomPar() #########" << endl;
    fMdcGeomPar=(HMdcGeomPar*)gHades->getSetup()->createDetGeomPar("Mdc");
  }

}





void HMdcGeomParF :: setGeomAuxPar(void)
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
  //   x(B) = [R(B)]E-1 R(A) x(A) + [R(B)]E-1 [T(A)-T(B)] 
  //
  //   M = [R(B)]E-1 R(A)
  //   V = [R(B)]E-1 [T(A)-T(B)] 
  //
  // From M it is easy to get the relative Euler angles 
  // (but M is not an R Euler matrix; it is an R-1 Euler matrix!
  // This must be very important when extracting Euler angles)
  //
  Int_t sector = fLocA[0];
  Int_t moduleA = fLocA[1];
  Int_t moduleB = fLocB[1];

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

  //Printing the original transformation from the container
  cout << endl <<"Debugging output in HMdcGeomParF::setGeomAuxPar" << endl;
  cout << "Original transformation from container" << endl;
  cout << "MDC A " << endl;
  transformA.print();  
  cout << "MDC B " << endl;
  transformB.print();

  //From the moduleB rotation, get the euler angles (radians)
  Double_t euler[3];
  if(rotB(8)< 1 && rotB(8)>-1) euler[1] = acos(rotB(8));
  else  euler[1]=0;
  Double_t sinaux;
  if(euler[1] == 0.){
    //euler[0] and euler[2] are equivalent. Write all in euler[0]
    euler[0]= (TMath::Pi()/2)+ acos(rotB(0));
    euler[2]=-(TMath::Pi()/2);
  } 
  else{
    sinaux = sin(euler[1]);
    euler[0] = atan2(rotB(5)/sinaux,rotB(2)/sinaux) ;    
    euler[2] = atan2(rotB(7)/sinaux,-rotB(6)/sinaux);
  }

  //printing and testing if rigth euler angles
  cout << "Euler angles: " << euler[0] << ", " 
       << euler[1]  << ", " << euler[2] << endl;

  HGeomRotation test;  
  test.setEulerAngles(euler[0]*180./TMath::Pi(),euler[1]*180./TMath::Pi(),euler[2]*180./TMath::Pi()); 

  //printing
  cout << "Rotation from Euler angles (first attemp): " << endl;
  test.print();

  //Study and redo this part!
  /* 
  Double_t eps = 0.0001;
  if( (fabs(test(0)-relrot(0))>eps) || (fabs(test(1)-relrot(1))>eps) || 
      (fabs(test(3)-relrot(3))>eps) || (fabs(test(4)-relrot(4))>eps) ||
      ((euler[0]-(TMath::Pi()/2))<-0.2)) {
    //The last condition in the previous if is absolutely ficticious! 
    //But it is only for changing the sign of all angles 
    //It could be important in the minimization
    cout << "Bad election in first euler angle! Trying again. "<< endl;
    euler[1] = - euler[1]; 
    sinaux = sin(euler[1]);
    euler[0] = atan2(relrot(7)/sinaux,relrot(6)/sinaux);
    euler[2] = atan2(relrot(5)/sinaux,-relrot(2)/sinaux) ;
    test.setEulerAngles(euler[0]*180./TMath::Pi(),euler[1]*180./TMath::Pi(),euler[2]*180./TMath::Pi());  

    //printing
    cout << "Rotation from Euler angles (second attemp): " << endl;
    test.print();

    //testing if euler angles are rigth now
    if( (fabs(test(0)-relrot(0))>eps) || (fabs(test(1)-relrot(1))>eps) || 
	(fabs(test(3)-relrot(3))>eps) || (fabs(test(4)-relrot(4))>eps) ){
      cout << endl << "FATAL ERROR: Bad election in euler angles! "<< endl;
      cout << "Original rot matrix: "<< endl;
      relrot.print();
      cout << "From obtained euler angles: " << endl;
      test.print();
      //What to do??
    }
  }
  */

  fillRotLab(euler[0],euler[1],euler[2]);
  fillTransLab(vectorB.getX(),vectorB.getY(),vectorB.getZ());
  setEulerLabAngles(euler[0],euler[1],euler[2]);

  //printing
  cout << "HMdcGeomParF::setGeomAuxPar: from geom params: " << endl;
  cout << "Rotation (Lab): " << euler[0] << ", " << euler[1]  << ", " << euler[2] << endl;
  cout << "Translation (Lab): " << vectorB.getX() << ", " << vectorB.getY() 
       << " , " <<  vectorB.getZ() << endl;

}





void HMdcGeomParF :: setModstoAlign(Int_t sector, Int_t modA, Int_t modB)
{ 
  //
  //Sets the modules to align. 
  //

  fLocA.set(2,sector,modA); //MDC A 
  fLocB.set(2,sector,modB); //MDC B  
}





Int_t HMdcGeomParF :: execute(void)
{  
  //
  //Makes absolutely nothing
  //

  return 0;
}





Bool_t HMdcGeomParF :: finalize(void)
{    
  // 
  // Makes initialization, minimization and
  // fills the control histograms for final values
  //

  //Initialization!
  initDelayed();

  Int_t sector = fLocA[0];
  Int_t modA = fLocA[1];
  Int_t modB = fLocB[1];

  //TESTING
  cout << endl <<" Entries from HMdcAligner "<< endl;
  cout << " fEulerResult: " << fEulerResult->getX() << ", " << fEulerResult->getY() << ", " << fEulerResult->getZ() << endl;
  cout << " fTransResults: " << fTransResult->getX()  << ", " << fTransResult->getY() << ", " << fTransResult->getZ() << endl;


  //Some control histo
  if(fHistograms!=kTRUE) setHistograms();
  if(fHistograms!=kTRUE) fillAuxHisto();

  //Then minimization

  ofstream *fout=NULL;
  if (fNameAscii) fout = new ofstream(fNameAscii.Data(), ios::app);
  if (*fout){
    *fout << endl << " Laboratory transformation parameters " << endl;
    *fout << endl << "Sector: " << sector << endl;
    *fout << "Module A: " << modA << "  Module A: " << modB << endl << endl;
    *fout << "Transformation before minimization (init values): "  << endl;
    *fout << fEulerLab[0] << ", " << fEulerLab[1] << ", " << fEulerLab[2] << ", " << fTransLab(0) << ", " << fTransLab(1)<< ", " << fTransLab(2)<< endl;
    }
  
  Float_t OldEulerLab[3];
  Float_t OldTransLab[3];
  Int_t IterCounter =0;
  Float_t IterCri;

  do{
    IterCri = 0;
    for(Int_t i=0;i<3;i++){
      OldEulerLab[i] = fEulerLab[i];
      OldTransLab[i] = fTransLab(i);
    }
    minfit(0,fEulerLab[0],fEulerLab[1],fEulerLab[2],fTransLab(0),fTransLab(1),fTransLab(2));
    if (*fout){
      *fout << "Parameters after minimization "  << endl;
      *fout << fEulerLab[0] << "," << fEulerLab[1] << "," << fEulerLab[2] << "," << fTransLab(0) << "," << fTransLab(1) << "," << fTransLab(2) << endl;
    }
    for(Int_t i=0;i<3;i++){
      IterCri += (fEulerLab[i]-OldEulerLab[i])/fEulerLab[i];
      IterCri += (fTransLab(i)-OldTransLab[i])/fTransLab(i);
      }
    IterCounter++;
    if(IterCounter>10) {
      cout << "WARNING in HMdcGeomParF :: execute" << endl;
      cout << "Sector: " << sector << " ModuleA: " << modA <<  " ModuleB: " << modB << endl;
      cout << "More than 10 iterations without results!" <<endl;
      break;
    }
  }while(IterCri>fIterCriteria);
  

  //Still last parameters (third component of traslation 
  //or fTransLab(2)) is not fixed 
  findZComponent();
  //Si lo llamo dos veces tengo problemas con el histograma.Arreglar
  //findZComponent();

  //third, graphical results with the minimization results

  Float_t xa, ya, s0a, s1a, xb, yb, s0b, s1b;
  Float_t xanew, yanew, zanew;
  Float_t xalab, yalab, zalab, xblab, yblab, zblab;
  Float_t xcZ, ycZ, xcY, zcY, ycX, zcX;
  Float_t phi, theta,ztar, rtar, xtar, ytar, canaux;
  Float_t myxtar, myytar, myztar, myrho, distZ;
  Float_t part1, part2, part3, numera, denomi, discenter;
  Float_t xDir,yDir,aux;

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
    hitB = (HMdcHit*) fHits->At(0);
    hitA = (HMdcHit*) fHits->At(1);    

    xa = hitA->getX();
    ya = hitA->getY();       
    xDir = hitA->getXDir();
    yDir = hitA->getYDir(); 
    //If using HMdcHit, it is neccesary this change
    aux = sqrt(1 - xDir * xDir - yDir * yDir);
    if(aux == 0.) {
      s0a=1; 
      s1a=1; 
      cout<< "ERROR #1 in HMdcGeomParF::finalize().";
    } 
    else{
      s0a = xDir/aux;
      s1a = yDir/aux;
    }
    xb = hitB->getX();
    yb = hitB->getY();
    xDir = hitB->getXDir();
    yDir = hitB->getYDir();
    //If using HMdcHit, it is neccesary this change
    aux = sqrt(1 - xDir * xDir - yDir * yDir);
    if(aux == 0.) {
      s0b=1; 
      s1b=1;      
      cout<< "ERROR #2 in HMdcGeomParF::finalize().";
    }
    else{
      s0b = xDir/aux;
      s1b = yDir/aux;
    }

    //coord. of MDC A in MDC B system:
    // X(B) = M X(A) + V
    xanew = fRotMat(0)*xa+fRotMat(3)*ya+fTranslation(0);
    yanew = fRotMat(1)*xa+fRotMat(4)*ya+fTranslation(1);
    zanew = fRotMat(2)*xa+fRotMat(5)*ya+fTranslation(2);
    
    // point coord. in lab system
    xalab = fRotLab(0)*xanew+fRotLab(1)*yanew+fRotLab(2)*zanew+fTransLab(0);
    yalab = fRotLab(3)*xanew+fRotLab(4)*yanew+fRotLab(5)*zanew+fTransLab(1);
    zalab = fRotLab(6)*xanew+fRotLab(7)*yanew+fRotLab(8)*zanew+fTransLab(2);
    
    xblab = fRotLab(0)*xb+fRotLab(1)*yb+fTransLab(0);
    yblab = fRotLab(3)*xb+fRotLab(4)*yb+fTransLab(1);
    zblab = fRotLab(6)*xb+fRotLab(7)*yb+fTransLab(2);
    
    //Cuts with Z=0
    xcZ = xalab-(xblab-xalab)*zalab/(zblab-zalab);
    ycZ = yalab-(yblab-yalab)*zalab/(zblab-zalab);     
    fFPlanez->Fill(xcZ,ycZ);
    
    //Cuts with Y=0
    xcY = xalab -(xblab-xalab)*yalab/(yblab-yalab);
    zcY = zalab -(zblab-zalab)*yalab/(yblab-yalab);
    fFPlaney->Fill(xcY,zcY);
    
    //Cuts with X=0
    ycX = yalab -(yblab-yalab)*xalab/(xblab-xalab);
    zcX = zalab -(zblab-zalab)*xalab/(xblab-xalab);
    fFPlanex->Fill(ycX,zcX);
    
    //Projection
    fFProzY->Fill(zcY);      
    fFProzX->Fill(zcX);
    fFProyZ->Fill(ycZ);      
    fFProyX->Fill(ycX);      
    fFProxZ->Fill(xcZ);
    fFProxY->Fill(xcY);
    
    //more info
    phi = atan2(yblab-yalab,xblab-xalab);
    theta = acos((zblab-zalab)/sqrt((xblab-xalab)*(xblab-xalab)+(yblab-yalab)*(yblab-yalab)+(zblab-zalab)*(zblab-zalab)));
    //coordinates of the point where the track passes closer to beam
    ztar = -(ycZ * sin(phi) + xcZ*cos(phi))*cos(theta)/sin(theta);
    xtar = ((xblab-xalab)*(ztar-zalab)/(zblab-zalab)) + xalab;
    ytar = ((yblab-yalab)*(ztar-zalab)/(zblab-zalab)) + yalab;
    rtar = ycZ * cos(phi) - xcZ*sin(phi);
    
    fFZVsTheta->Fill(theta,ztar); //z vs. theta
    fFZVsRho->Fill(rtar,ztar); //z vs. rho
    
    fFTargets->Fill(ztar);
        
    //again, with my own calculation (not used)
    canaux = ((xalab-xblab)/(yblab-yalab)) - 
      ((yblab-yalab)/(xblab-xalab));
    myxtar = (yalab-(xalab*(yblab-yalab)/(xblab-xalab))) / canaux;
    myytar = (myxtar*(yblab-yalab)/(xblab-xalab)) + 
      (yalab-xalab*(yblab-yalab)/(xblab-xalab));
    myztar = (myxtar*(zblab-zalab)/(xblab-xalab)) + 
      (zalab-xalab*(zblab-zalab)/(xblab-xalab));
    myrho = sqrt(myxtar*myxtar + myytar*myytar);

    //distance to beam
    distZ = ( (xalab*(yblab-yalab))-(yalab*(xblab-xalab)) ) /
      sqrt( ((yblab-yalab)*(yblab-yalab)) + ((xblab-xalab)*(xblab-xalab)) );
    fFDisZ->Fill(distZ);
    
    //Distance to (0,0,0)
    part1 = ((-xalab)*(yblab-yalab)) - ((-yalab)*(xblab-xalab));
    part2 = ((-yalab)*(zblab-zalab)) - ((-zalab)*(yblab-yalab));
    part3 = ((-zalab)*(xblab-xalab)) - ((-xalab)*(zblab-zalab));
    numera = (part1*part1)+(part2*part2)+(part3*part3);
    denomi = (xblab-xalab)*(xblab-xalab)+(yblab-yalab)*(yblab-yalab)+(zblab-zalab)*(zblab-zalab); 
    discenter = sqrt(numera/denomi);
    fFDisCenter->Fill(discenter);
    
    //if(zcY>fZLoCut && zcY<fZHiCut){
      //track crosses Y=0 plane with zlolimit<z<zuplimit 
      //that is ! Taking only the primary target!!
      //fFProzYAC->Fill(zcY);
      //fFProzXAC->Fill(zcX);
      //fFProyZAC->Fill(ycZ);
      //fFProyXAC->Fill(ycX);
      //fFProxZAC->Fill(xcZ);
      //fFProxYAC->Fill(xcY);
    // }
  }  // end of for (Int_t i=0; i<nentries;i++) {
  

  //writing in own directory of standard output
  //const char* oldDirName = gDirectory->GetPath();

  //static Char_t newDirName[255];
  //sprintf(newDirName,"%s%s%i%s%i%s%i","GeomParF_","S_",sector,"_ModA_",modA,"_ModB_",modB);
  //TDirectory *newDir;

  //if (!(gHades->getOutputFile()->GetKey(newDirName))) newDir = gHades->getOutputFile()->mkdir(newDirName,newDirName);
  //else newDir = gDirectory;
  //gHades->getOutputFile()->cd(newDirName);

  //static Char_t newDirName[255];
  //sprintf(newDirName,"%s%s%i%s%i%s%i","GeomParF_","S_",sector,"_ModA_",modA,"_ModB_",modB);
  //TDirectory *newDir;
  //if (!(fOutFile->GetKey(newDirName))) fOutFile->mkdir(newDirName,newDirName);
  //else cout << "ERROR in HMdcGeomParF::finalize(): Directory already exist " << endl;
  //fOutFile->cd(newDirName);

  //  TDirectory *newDir = fOutFile->mkdir(newDirName,newDirName);
  //fOutFile->cd(newDirName);

  //static Char_t newDirName[255];
  //sprintf(newDirName,"%s%s%i%s%i%s%i","GeomParF_","S_",sector,"_ModA_",modA,"_ModB_",modB);
  //if(!(fOutFile->cd(newDirName))){
  //  fOutFile->mkdir(newDirName,newDirName);
  //  fOutFile->cd(newDirName);
  // }

  fout->close();
  delete fout;

  storeInFile();
  writeGeomParContainer();

  return kTRUE; 
}





void HMdcGeomParF :: storeInFile(void)
{  
  //
  // Stores all histos and tree in the Root file
  //
  
  Int_t sector = fLocA[0];
  Int_t modA = fLocA[1];
  Int_t modB = fLocB[1];
  
  const char* oldDirName = gDirectory->GetPath();
  static Char_t newDirName[255];
  sprintf(newDirName,"%s%s%i%s%i%s%i","GeomParF_","S_",sector,"_ModA_",modA,"_ModB_",modB);
  fOutFile->cd(newDirName);

  fFPlanex->Write();    
  fFPlaney->Write();
  fFPlanez->Write();
  fFTargets->Write();
  fFZVsTheta->Write(); 
  fFZVsRho->Write();
  fFDisZ->Write();
  fFDisCenter->Write();
  fFProzY->Write();
  fFProzX->Write(); 
  fFProyZ->Write();
  fFProyX->Write();
  fFProxZ->Write();
  fFProxY->Write();
  fPlanex->Write();    
  fPlaney->Write();
  fPlanez->Write();
  fTargets->Write();
  fZVsTheta->Write(); 
  fZVsRho->Write();
  fDisZ->Write();
  fDisCenter->Write();
  fProzY->Write();
  fProzX->Write(); 
  fProyZ->Write();
  fProyX->Write();
  fProxZ->Write();
  fProxY->Write();
  //fFProzYAC->Write();
  //fFProzXAC->Write(); 
  //fFProyZAC->Write();
  //fFProyXAC->Write();
  //fFProxZAC->Write();
  //fFProxYAC->Write();

  fOutFile->cd(oldDirName);
  fOutFile->Write();

  fOutFile->Close();
}


void HMdcGeomParF :: fillAuxHisto(void){
  //
  //Auxiliar graphics
  //

  Float_t xa, ya, s0a, s1a, xb, yb, s0b, s1b;
  Float_t xanew, yanew, zanew;
  Float_t xalab, yalab, zalab, xblab, yblab, zblab;
  Float_t xcZ, ycZ, xcY, zcY, ycX, zcX;
  Float_t phi, theta,ztar, rtar, xtar, ytar, canaux;
  Float_t myxtar, myytar, myztar, myrho, distZ;
  Float_t part1, part2, part3, numera, denomi, discenter;
  Float_t xDir,yDir,aux;

  TBranch *branch = fTree->GetBranch("hits");
  branch->SetAddress(&fHits);

  Stat_t entries = fTree->GetEntries();
  
  HMdcHit* hitA;
  HMdcHit* hitB;

  //loop on hits   
  for (Int_t i=0;i<entries;i++) {     
    fHits->Clear();
    fTree->GetEntry(i);
    //data = fNtPairs->GetArgs();

    //filling from the Tree    
    hitB = (HMdcHit*) fHits->At(0);
    hitA = (HMdcHit*) fHits->At(1);    

    //filling from the tree
    xa = hitA->getX();
    ya = hitA->getY();       
    xDir = hitA->getXDir();
    yDir = hitA->getYDir(); 
    //If using HMdcHit, it is neccesary this change
    aux = sqrt(1 - xDir * xDir - yDir * yDir);
    if(aux == 0.) {
      s0a=1; 
      s1a=1; 
      cout<< "ERROR #1 in HMdcGeomParF::fillAuxHisto().";
    } 
    else{
      s0a = xDir/aux;
      s1a = yDir/aux;
    }
    xb = hitB->getX();
    yb = hitB->getY();
    xDir = hitB->getXDir();
    yDir = hitB->getYDir();
    //If using HMdcHit, it is neccesary this change
    aux = sqrt(1 - xDir * xDir - yDir * yDir);
    if(aux == 0.) {
      s0b=1; 
      s1b=1;      
      cout<< "ERROR #2 in HMdcGeomParF::fillAuxHisto().";
    }
    else{
      s0b = xDir/aux;
      s1b = yDir/aux;
    }

    
    //coord. of MDC A in MDC B system:
    // X(B) = M X(A) + V
    xanew = fRotMat(0)*xa+fRotMat(3)*ya+fTranslation(0);
    yanew = fRotMat(1)*xa+fRotMat(4)*ya+fTranslation(1);
    zanew = fRotMat(2)*xa+fRotMat(5)*ya+fTranslation(2);
    
    // point coord. in lab system
    xalab = fRotLab(0)*xanew+fRotLab(1)*yanew+fRotLab(2)*zanew+fTransLab(0);
    yalab = fRotLab(3)*xanew+fRotLab(4)*yanew+fRotLab(5)*zanew+fTransLab(1);
    zalab = fRotLab(6)*xanew+fRotLab(7)*yanew+fRotLab(8)*zanew+fTransLab(2);
    
    xblab = fRotLab(0)*xb+fRotLab(1)*yb+fTransLab(0);
    yblab = fRotLab(3)*xb+fRotLab(4)*yb+fTransLab(1);
    zblab = fRotLab(6)*xb+fRotLab(7)*yb+fTransLab(2);
    
    //Cuts with Z=0
    xcZ = xalab-(xblab-xalab)*zalab/(zblab-zalab);
    ycZ = yalab-(yblab-yalab)*zalab/(zblab-zalab);     
    fPlanez->Fill(xcZ,ycZ);
    
    //Cuts with Y=0
    xcY = xalab -(xblab-xalab)*yalab/(yblab-yalab);
    zcY = zalab -(zblab-zalab)*yalab/(yblab-yalab);
    fPlaney->Fill(xcY,zcY);
    
    //Cuts with X=0
    ycX = yalab -(yblab-yalab)*xalab/(xblab-xalab);
    zcX = zalab -(zblab-zalab)*xalab/(xblab-xalab);
    fPlanex->Fill(ycX,zcX);
    
    //Projection
    fProzY->Fill(zcY);      
    fProzX->Fill(zcX);
    fProyZ->Fill(ycZ);      
    fProyX->Fill(ycX);      
    fProxZ->Fill(xcZ);
    fProxY->Fill(xcY);
    
    //more info
    phi = atan2(yblab-yalab,xblab-xalab);
    theta = acos((zblab-zalab)/sqrt((xblab-xalab)*(xblab-xalab)+(yblab-yalab)*(yblab-yalab)+(zblab-zalab)*(zblab-zalab)));
    //coordinates of the point where the track passes closer to beam
    ztar = -(ycZ * sin(phi) + xcZ*cos(phi))*cos(theta)/sin(theta);
    xtar = ((xblab-xalab)*(ztar-zalab)/(zblab-zalab)) + xalab;
    ytar = ((yblab-yalab)*(ztar-zalab)/(zblab-zalab)) + yalab;
    rtar = ycZ * cos(phi) - xcZ*sin(phi);
    
    fZVsTheta->Fill(theta,ztar); //z vs. theta
    fZVsRho->Fill(rtar,ztar); //z vs. rho
    
    fTargets->Fill(ztar);
        
    //again, with my own calculation (not used)
    canaux = ((xalab-xblab)/(yblab-yalab)) - 
      ((yblab-yalab)/(xblab-xalab));
    myxtar = (yalab-(xalab*(yblab-yalab)/(xblab-xalab))) / canaux;
    myytar = (myxtar*(yblab-yalab)/(xblab-xalab)) + 
      (yalab-xalab*(yblab-yalab)/(xblab-xalab));
    myztar = (myxtar*(zblab-zalab)/(xblab-xalab)) + 
      (zalab-xalab*(zblab-zalab)/(xblab-xalab));
    myrho = sqrt(myxtar*myxtar + myytar*myytar);

    //distance to beam
    distZ = ( (xalab*(yblab-yalab))-(yalab*(xblab-xalab)) ) /
      sqrt( ((yblab-yalab)*(yblab-yalab)) + ((xblab-xalab)*(xblab-xalab)) );
    fDisZ->Fill(distZ);
    
    //Distance to (0,0,0)
    part1 = ((-xalab)*(yblab-yalab)) - ((-yalab)*(xblab-xalab));
    part2 = ((-yalab)*(zblab-zalab)) - ((-zalab)*(yblab-yalab));
    part3 = ((-zalab)*(xblab-xalab)) - ((-xalab)*(zblab-zalab));
    numera = (part1*part1)+(part2*part2)+(part3*part3);
    denomi = (xblab-xalab)*(xblab-xalab)+(yblab-yalab)*(yblab-yalab)+(zblab-zalab)*(zblab-zalab); 
    discenter = sqrt(numera/denomi);
    fDisCenter->Fill(discenter);
    
    if(zcY>fZLoCut && zcY<fZHiCut){
      //track crosses Y=0 plane with zlolimit<z<zuplimit 
      //that is ! Taking only the primary target!!
      //fProzYAC->Fill(zcY);
      //fProzXAC->Fill(zcX);
      //fProyZAC->Fill(ycZ);
      //fProyXAC->Fill(ycX);
      //fProxZAC->Fill(xcZ);
      //fProxYAC->Fill(xcY);
    }
  }  // end of for (Int_t i=0; i<nentries;i++) {
  

  //writing in own directory of standard output
 //const char* oldDirName = gDirectory->GetPath();

  //static Char_t newDirName[255];
  //sprintf(newDirName,"%s%s%i%s%i%s%i","GeomParF_","S_",sector,"_ModA_",modA,"_ModB_",modB);
  //TDirectory *newDir;

  //if (!(gHades->getOutputFile()->GetKey(newDirName))) newDir = gHades->getOutputFile()->mkdir(newDirName,newDirName);
  //else newDir = gDirectory;
  //gHades->getOutputFile()->cd(newDirName);

  //fPlanex->Write();    
  //fPlaney->Write();
  //fPlanez->Write();
  //fTargets->Write();
  //fZVsTheta->Write(); 
  //fZVsRho->Write();
  //fDisZ->Write();
  //fDisCenter->Write();
  //fProzY->Write();
  //fProzX->Write(); 
  //fProyZ->Write();
  //fProyX->Write();
  //fProxZ->Write();
  //fProxY->Write();
  //fProzYAC->Write();
  //fProzXAC->Write(); 
  //fProyZAC->Write();
  //fProyXAC->Write();
  //fProxZAC->Write();
  //fProxYAC->Write();

  //newDir->cd("..");
  //newDir->cd(oldDirName);

}





void HMdcGeomParF :: writeGeomParContainer(void){ 
  //
  // Writing the new geometrical parameters from minimization results
  //
  // Getting module A parameters from module B parameters (fRotLab and fTransLab)
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

  Int_t sector = fLocA[0];
  Int_t moduleA = fLocA[1];
  Int_t moduleB = fLocB[1];

  HGeomRotation rotA;
  HGeomVector vectorA;
  rotA = fRotLab * fRotMat;
  vectorA = fRotLab * fTranslation + fTransLab;

  fMdcGeomPar->getModule(sector,moduleA)->getLabTransform().setRotMatrix(rotA);
  fMdcGeomPar->getModule(sector,moduleA)->getLabTransform().setTransVector(vectorA);
  fMdcGeomPar->getModule(sector,moduleB)->getLabTransform().setRotMatrix(fRotLab);
  fMdcGeomPar->getModule(sector,moduleB)->getLabTransform().setTransVector(fTransLab);

  //Debugging
  cout << endl << "   ########  FINAL RESULT   ########   "<< endl;
  cout << " Sector: " << sector << "  ModA: " << moduleA << "  ModB: " << moduleB << endl;
  cout << "Params for module A: " << endl; 
  rotA.print(); 
  vectorA.print();   
  cout << "Params for module B: " << endl; 
  fRotLab.print();
  fTransLab.print();

  //if(fMdcGeomPar->hasChanged()==kTRUE) cout << " Flag es kTRUE "<< endl;
  //else cout << " Flag es kFALSE "<< endl;
  //cout << "Input version[1] = " << fMdcGeomPar->getInputVersion(1)<< endl;
  //cout << "Input version[2] = " << fMdcGeomPar->getInputVersion(2)<< endl;

  fMdcGeomPar->setChanged();

  Int_t v = fMdcGeomPar->getInputVersion(1);
  if (v>0) fMdcGeomPar->setInputVersion(++v,1); 
  else {
    v=fMdcGeomPar->getInputVersion(2);
    fMdcGeomPar->setInputVersion(++v,2);     
  } 

  //if(fMdcGeomPar->hasChanged()==kTRUE) cout << " Ahora flag es kTRUE "<< endl;
  //else cout << " Ahora flag es kFALSE "<< endl;
  //cout << "Input version[1] = " << fMdcGeomPar->getInputVersion(1)<< endl;
  //cout << "Input version[2] = " << fMdcGeomPar->getInputVersion(2)<< endl;

 
}





void HMdcGeomParF :: fillRotMatrix(Float_t prim,Float_t segu, Float_t terc){ 
  // 
  // Fill a matrix using the Euler angles of the relative transformation 
  // 
  //Between two mdcs, the euler angles are 90, theta,-90 with  
  //theta = difference between polar angles of MDCs   
  //Example: A = MDC1 ; B = MDC2. Then euler angles are: 
  //     1.570796326795,0.171026,-1.570796326795 
  //     
  //Example: A = MDC2 ; B = MDC3. Then euler angles are: 
  //     1.570796326795,-0.08531169,-1.570796326795 
  // 
  /*OLD
  fRotMat[0][0] = (cos(prim) * cos(segu) * cos(terc)) - (sin(prim) * sin(terc)); 
  fRotMat[1][0] = ( - cos(prim) * cos(segu) * sin(terc)) - (sin(prim) * cos(terc)); 
  fRotMat[2][0] = (cos(prim) * sin(segu)); 
  fRotMat[0][1] = (sin(prim) * cos(segu) * cos(terc)) + (cos(prim) * sin(terc)); 
  fRotMat[1][1] = ( - sin(prim) * cos(segu) * sin(terc)) + (cos(prim) * cos(terc)); 
  fRotMat[2][1] = (sin(prim) * sin(segu)); 
  fRotMat[0][2] = ( - sin(segu) * cos(terc)); 
  fRotMat[1][2] = (sin(segu) * sin(terc)); 
  fRotMat[2][2] = (cos(segu)); 
  */  
  //Elements ordeded like in OLD 
  //
  //  rot(0) rot(3) rot(6)     fRotMat[0][0] fRotMat[0][1] fRotMat[0][2]
  //  rot(1) rot(4) rot(7)  =  fRotMat[1][0] fRotMat[1][1] fRotMat[1][2]
  //  rot(2) rot(5) rot(8)     fRotMat[2][0] fRotMat[2][1] fRotMat[2][2]
  const double rad2deg = 57.29577951308;  
  fRotMat.setEulerAngles(prim*rad2deg,segu*rad2deg,terc*rad2deg);
} 





void HMdcGeomParF :: fillTranslation(Float_t x,Float_t y, Float_t z){ 
  //
  // Translation from MDC A to MDC B
  //
 
  fTranslation.setX(x); 
  fTranslation.setY(y);
  fTranslation.setZ(z); 

}





void   HMdcGeomParF :: fillRotLab(Float_t prim,Float_t segu, Float_t terc){
  //
  // Fill a matrix using the Euler angles of the laboratory transformation
  // This matrix correspond to R
  // X(LAB) = R X(B) + T
  //
  /*OLD
  fRotLab[0][0] = (cos(prim) * cos(segu) * cos(terc)) - (sin(prim) * sin(terc));
  fRotLab[0][1] = ( - cos(prim) * cos(segu) * sin(terc)) - (sin(prim) * cos(terc));
  fRotLab[0][2] = (cos(prim) * sin(segu));
  fRotLab[1][0] = (sin(prim) * cos(segu) * cos(terc)) + (cos(prim) * sin(terc));
  fRotLab[1][1] = ( - sin(prim) * cos(segu) * sin(terc)) + (cos(prim) * cos(terc));
  fRotLab[1][2] = (sin(prim) * sin(segu));
  fRotLab[2][0] = ( - sin(segu) * cos(terc));
  fRotLab[2][1] = (sin(segu) * sin(terc));
  fRotLab[2][2] = (cos(segu));
  */
  //Elements ordeded like in OLD 
  //
  //  rot(0) rot(1) rot(2)     fRotLab[0][0] fRotLab[0][1] fRotLab[0][2]
  //  rot(3) rot(4) rot(5)  =  fRotLab[1][0] fRotLab[1][1] fRotLab[1][2]
  //  rot(6) rot(7) rot(8)     fRotLab[2][0] fRotLab[2][1] fRotLab[2][2]
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
  




void HMdcGeomParF :: setEulerAngles(Float_t f,Float_t s, Float_t t){ 
  //
  // Setting the Euler angles of the relative transformation
  //
 
  fEuler[0] = f; 
  fEuler[1] = s; 
  fEuler[2] = t; 
} 




  
void HMdcGeomParF :: setEulerLabAngles(Float_t f,Float_t s, Float_t t){ 
  //
  // Setting the Euler angles of the laboratory transformation
  //
 
  fEulerLab[0] = f; 
  fEulerLab[1] = s; 
  fEulerLab[2] = t; 
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





void HMdcGeomParF :: minfit(Int_t fix, Float_t fE, Float_t sE, Float_t tE, Float_t fT, Float_t sT, Float_t tT){ 
  //
  // Minuit menu
  // Put first argument to 1 to fix angular values 
  // Other arguments are init values for the parameters!
  //

  Double_t args[6]; 
  Int_t err = 0; 
  Double_t limit = 0;

  //This depends on MDCA and MDCB 
  Double_t start_val[]={fE, sE, tE, fT, sT, tT};
  
  Double_t step[6]; 
  step[0]=0.1; 
  step[1]=0.1; 
  step[2]=0.1; 
  step[3]=0.1; 
  step[4]=0.1; 
  step[5]=0.1; 
  
  TMinuit *minuit=new TMinuit(6); 

  //setting the object to minimize
  minuit->SetFCN(fcnGP); 
  minuit->SetObjectFit(this);

  minuit->mnparm(0,"par0",start_val[0],step[0],limit,limit,err); 
  minuit->mnparm(1,"par1",start_val[1],step[1],limit,limit,err); 
  minuit->mnparm(2,"par2",start_val[2],step[2],limit,limit,err); 
  minuit->mnparm(3,"par3",start_val[3],step[3],limit,limit,err); 
  minuit->mnparm(4,"par4",start_val[4],step[4],limit,limit,err); 
  minuit->mnparm(5,"par5",start_val[5],step[5],limit,limit,err);

  //FIXING 
  if(fix == 1) minuit->FixParameter(0);     
  if(fix == 1) minuit->FixParameter(1); 
  if(fix == 1) minuit->FixParameter(2); 
  
  //To remove all printout
  minuit->SetPrintLevel(-1);

  //args is the array of the numerical arguments of the command 
  //the third field is the number of arguments esp3cified 
  //For MIGRAD arguments are maxcalls and tolerance, both optionals 
  minuit->mnexcm("MIGRAD",args,0,err); 
  //minuit->mnexcm("SIMPLEX",args,0,err); 
  
  Double_t parresult[3];
  minuit->GetParameter(0,fEulerLab[0],fErrorLab[0]);
  minuit->GetParameter(1,fEulerLab[1],fErrorLab[1]);
  minuit->GetParameter(2,fEulerLab[2],fErrorLab[2]);
  minuit->GetParameter(3,parresult[0],fErrorLab[3]);
  minuit->GetParameter(4,parresult[1],fErrorLab[4]);
  minuit->GetParameter(5,parresult[2],fErrorLab[5]);  
  fTransLab.setX(parresult[0]);  
  fTransLab.setY(parresult[1]);
  fTransLab.setZ(parresult[2]);
  fillRotLab(fEulerLab[0],fEulerLab[1],fEulerLab[2]);

  if (err != 0) cout << endl <<"MINIMIZATION EXITED WITH ERROR " << err << endl;

}




void fcnGP(Int_t &npar, Double_t *gin, Double_t &f, Double_t *par, Int_t iflag){  
  //
  //This function contains the functional to minimize 
  //

  Double_t chisq = 0.;     
  Double_t rotlab[3][3]; 
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

  rotlab[0][0] = (cos(prim) * cos(segu) * cos(terc)) - (sin(prim) * sin(terc));
  rotlab[0][1] = ( - cos(prim) * cos(segu) * sin(terc)) - (sin(prim) * cos(terc));
  rotlab[0][2] = (cos(prim) * sin(segu));
  rotlab[1][0] = (sin(prim) * cos(segu) * cos(terc)) + (cos(prim) * sin(terc));
  rotlab[1][1] = ( - sin(prim) * cos(segu) * sin(terc)) + (cos(prim) * cos(terc));
  rotlab[1][2] = (sin(prim) * sin(segu));
  rotlab[2][0] = ( - sin(segu) * cos(terc));
  rotlab[2][1] = (sin(segu) * sin(terc));
  rotlab[2][2] = (cos(segu));

  TTree* pData; 
  HMdcGeomParF* pGeomParF; 
  TClonesArray* theHits;  
  theHits = new TClonesArray("HMdcHit",2);
  pGeomParF = (HMdcGeomParF*)(gMinuit->GetObjectFit());
  pData = pGeomParF->getTree();
  rotrel = pGeomParF->getRotMatrix();
  transrel = pGeomParF->getTranslation();

  TBranch *branch = pData->GetBranch("hits");
  branch->SetAddress(&theHits);

  Stat_t entries = pData->GetEntries();
  
  HMdcHit* hitA;
  HMdcHit* hitB;
  
  //loop on hits   
  for (Int_t i=0;i<entries;i++) {     
    theHits->Clear();
    pData->GetEntry(i);
    //data = pData->GetArgs();
    //filling from the Tree    
    hitB = (HMdcHit*) theHits->At(0);
    hitA = (HMdcHit*) theHits->At(1);    
    
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
    xanew = rotrel(0)*pxa+rotrel(3)*pya+transrel(0);
    yanew = rotrel(1)*pxa+rotrel(4)*pya+transrel(1);
    zanew = rotrel(2)*pxa+rotrel(5)*pya+transrel(2);

    // point coord. in lab system
    fxalab = rotlab[0][0]*xanew+rotlab[0][1]*yanew+rotlab[0][2]*zanew+translab[0];
    fyalab = rotlab[1][0]*xanew+rotlab[1][1]*yanew+rotlab[1][2]*zanew+translab[1];
    fzalab = rotlab[2][0]*xanew+rotlab[2][1]*yanew+rotlab[2][2]*zanew+translab[2];
    
    fxblab = rotlab[0][0]*pxb+rotlab[0][1]*pyb+translab[0];
    fyblab = rotlab[1][0]*pxb+rotlab[1][1]*pyb+translab[1];
    fzblab = rotlab[2][0]*pxb+rotlab[2][1]*pyb+translab[2];

    //probando minimizar la distancia al eje Z!
    Float_t distZ = ( (fxalab*(fyblab-fyalab))-(fyalab*(fxblab-fxalab)) ) /
      sqrt( ((fyblab-fyalab)*(fyblab-fyalab)) + ((fxblab-fxalab)*(fxblab-fxalab)) );

    if(GP_DEBUG >3){
      cout << "  ++++++++  VALUES IN fcnalign()  +++++++++ " << endl;
      cout << "HITA: " <<  pxa << ", " <<  pya << ", " <<  ps0a << ", " <<  ps1a << endl;
      cout << "HITB: " <<  pxb << ", " <<  pyb << ", " <<  ps0b << ", " <<  ps1b << endl;
      cout << "Distance: " <<  distZ  << endl;
    }

    chisq += distZ*distZ/50.;

  }
  f = chisq; 
}





void  HMdcGeomParF :: findZComponent(void)
{
  //
  //Finding the third component of the translation
  //

  Float_t xDir,yDir,aux;  
  Float_t xa, ya, s0a, s1a, xb, yb, s0b, s1b;
  Float_t xanew, yanew, zanew;
  Float_t xalab, yalab, zalab, xblab, yblab, zblab;
  Float_t xcZ, ycZ;
  Float_t phi, theta,ztar, rtar, xtar, ytar;

  Int_t sector = fLocA[0];
  Int_t modA = fLocA[1];
  Int_t modB = fLocB[1];  

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
    hitB = (HMdcHit*) fHits->At(0);
    hitA = (HMdcHit*) fHits->At(1);    

    xa = hitA->getX();
    ya = hitA->getY();       
    xDir = hitA->getXDir();
    yDir = hitA->getYDir(); 
    //If using HMdcHit, it is neccesary this change
    aux = sqrt(1 - xDir * xDir - yDir * yDir);
    if(aux == 0.) {
      s0a=1; 
      s1a=1; 
      cout<< "ERROR #1 in HMdcGeomParF::findZComponent().";
    } 
    else{
      s0a = xDir/aux;
      s1a = yDir/aux;
    }
    xb = hitB->getX();
    yb = hitB->getY();
    xDir = hitB->getXDir();
    yDir = hitB->getYDir();
    //If using HMdcHit, it is neccesary this change
    aux = sqrt(1 - xDir * xDir - yDir * yDir);
    if(aux == 0.) {
      s0b=1; 
      s1b=1;      
      cout<< "ERROR #2 in HMdcGeomParF::findZComponent().";
    }
    else{
      s0b = xDir/aux;
      s1b = yDir/aux;
    }

    //coord. of MDC A in MDC B system:
    // X(B) = M X(A) + V
    xanew = fRotMat(0)*xa+fRotMat(3)*ya+fTranslation(0);
    yanew = fRotMat(1)*xa+fRotMat(4)*ya+fTranslation(1);
    zanew = fRotMat(2)*xa+fRotMat(5)*ya+fTranslation(2);
    
    // point coord. in lab system
    xalab = fRotLab(0)*xanew+fRotLab(1)*yanew+fRotLab(2)*zanew+fTransLab(0);
    yalab = fRotLab(3)*xanew+fRotLab(4)*yanew+fRotLab(5)*zanew+fTransLab(1);
    zalab = fRotLab(6)*xanew+fRotLab(7)*yanew+fRotLab(8)*zanew+fTransLab(2);
    
    xblab = fRotLab(0)*xb+fRotLab(1)*yb+fTransLab(0);
    yblab = fRotLab(3)*xb+fRotLab(4)*yb+fTransLab(1);
    zblab = fRotLab(6)*xb+fRotLab(7)*yb+fTransLab(2);
    
    //Cuts with Z=0
    xcZ = xalab-(xblab-xalab)*zalab/(zblab-zalab);
    ycZ = yalab-(yblab-yalab)*zalab/(zblab-zalab);     

    phi = atan2(yblab-yalab,xblab-xalab);
    theta = acos((zblab-zalab)/sqrt((xblab-xalab)*(xblab-xalab)+(yblab-yalab)*(yblab-yalab)+(zblab-zalab)*(zblab-zalab)));
    //coordinates of the point where the track passes closer to beam
    ztar = -(ycZ * sin(phi) + xcZ*cos(phi))*cos(theta)/sin(theta);
    xtar = ((xblab-xalab)*(ztar-zalab)/(zblab-zalab)) + xalab;
    ytar = ((yblab-yalab)*(ztar-zalab)/(zblab-zalab)) + yalab;
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





