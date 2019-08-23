# include <math.h>
# include <stdlib.h>
# include <iomanip.h>
# include <fstream.h>
# include <TMath.h>
# include <TROOT.h>
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
  
///////////////////////////////////////////////////////////////////////////////
//
// HMdcAligner   (ongoing work)
//
// Performs the MDC software alignment (first part).
//
// Based on hits in a pair of MDCs, gets the relative
// geometrical parameters (three euler angles and translation 
// vector). 
// After initizalition (where euler angles can be obtained 
// from standard parameter containers) the execute() function
// makes histograms for the initial parameters and fill the
// tree with accepted pairs (one hit and only one in the
// selected window). Then, a set of parameters which minimizes
// a set of (gaussian?) distributions are obtained using TMinuit. 
// This distributions are the differences in X, Y and the slope
// projections in both MDCs (after track propagation and 
// transformations).
//
//////////////////////////////////////////////////////////////////////////
  
 Int_t HMdcAligner::fRecCount=0;
 TFile* HMdcAligner::fOutRoot=0;
 Int_t A_DEBUG=0;

HMdcAligner :: HMdcAligner(void) : HReconstructor()
{  
  //
  // Default constructor. 
  //

  fLocA.setNIndex(2);
  fLocB.setNIndex(2);
  fHitCat = NULL;  
  fAuto =kFALSE;  
  fManual =kFALSE;  
  fHistograms =kFALSE;
  fDiscart = 0;
  fHitsMdcA = 0;   
  fHitsFoundInWindow =0;
  fCount =0;
  fXArea = 100;  
  fYArea = 100;
  fHits = new TClonesArray("HMdcHit",2);  
  fOutRoot = NULL;
  fLocA.set(2,0,0); //MDC A  dummy
  fLocB.set(2,0,1); //MDC B  dummy
  initMinimization();
}
 




HMdcAligner :: HMdcAligner(Int_t sector, Int_t modA, Int_t modB) : HReconstructor()
{  
  //
  // Constructor including module election. Alignment procedure 
  // proyects hits of modA in modB coordinate system and compares
  //

  fLocA.setNIndex(2);
  fLocB.setNIndex(2);
  fHitCat = NULL;  
  fAuto =kFALSE;  
  fManual =kFALSE;
  fHistograms =kFALSE;
  fDiscart = 0;
  fHitsMdcA = 0;   
  fHitsFoundInWindow =0;
  fCount =0;  
  fXArea = 100;  
  fYArea = 100;
  fHits = new TClonesArray("HMdcHit",2);  
  fOutRoot = NULL; 
  fLocA.set(2,sector,modA); //MDC A 
  fLocB.set(2,sector,modB); //MDC B   
  initMinimization();
}





HMdcAligner :: HMdcAligner(Text_t* name, Text_t* title, Int_t sector, Int_t modA, Int_t modB) : HReconstructor(name, title)
{  
  //
  // Constructor including module election (and name and title, which 
  // seems to be very important). Alignment procedure 
  // proyects hits of modA in modB coordinate system and compares
  //

  fLocA.setNIndex(2);
  fLocB.setNIndex(2);
  fHitCat = NULL; 
  fAuto =kFALSE;  
  fManual =kFALSE;
  fHistograms =kFALSE;
  fDiscart = 0;
  fHitsMdcA = 0;   
  fHitsFoundInWindow =0;
  fCount =0;  
  fXArea = 100;  
  fYArea = 100;
  fHits=new TClonesArray("HMdcHit",2);  
  fOutRoot = NULL; 
  fLocA.set(2,sector,modA); //MDC A 
  fLocB.set(2,sector,modB); //MDC B   
  initMinimization();
}





HMdcAligner :: ~HMdcAligner(void)
{    
  //
  // Destructor.
  //
}





void HMdcAligner :: initMinimization(void){
  fIterCriteria = 0.1;   
  fWeights = new Float_t[4];
  fSteps = new Float_t[6];
  fLimits = new Float_t[6];
  setWeights(400,100,0.1,0.1);
  setSteps(0.1,0.1,0.1,0.1,0.1,0.1);
  setLimits(0.,0.,0.,0.,0.,0.);
}





Int_t HMdcAligner :: getRelParams(HGeomVector*& ang,HGeomVector*& tra)
{    
  //
  // Serves pointers to the relative transformation parameters
  //

  ang = &fEuler;
  tra = &fTranslation;
  if(A_DEBUG>0){
    cout << " HMdcAligner :: getRelParams(). Filling " << endl;
    cout << "ang: " << ang << endl;
    cout << "tra: " << tra << endl;
  }
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





void HMdcAligner :: setHistograms(void)
{
  //
  // Inits histograms
  //

  Int_t sector = fLocA[0];
  Int_t modA = fLocA[1];
  Int_t modB = fLocB[1];
  
  fRecCount++; 
  if(!fOutRoot) fOutRoot = new TFile(fNameRoot,"UPDATE");

  Char_t title[50], tmp[50];
  sprintf(title,"%s%i%s%i%s%i","Sector_",sector,"_ModA_",modA,"_ModB_",modB);

  sprintf(tmp,"%s%s%i%s%i%s%i","All","_Sector_",sector,"_ModA_",modA,"_ModB_",modB);
  fAlignAll = new TTree(tmp,title);
  fAlignAll->Branch("hits",&fHits,64000);

  const char* oldDirName = gDirectory->GetPath();
  static Char_t newDirName[255];
  sprintf(newDirName,"%s%s%i%s%i%s%i","Aligner_","S_",sector,"_ModA_",modA,"_ModB_",modB);
  fOutRoot->mkdir(newDirName,newDirName);
  fOutRoot->cd(newDirName);

  sprintf(tmp,"%s%s%i%s%i%s%i","fResX","_Sector_",sector,"_ModA_",modA,"_ModB_",modB);
  fResX = new TH1F(tmp,title,2000,-1000,1000);  
  sprintf(tmp,"%s%s%i%s%i%s%i","fResY","_Sector_",sector,"_ModA_",modA,"_ModB_",modB);
  fResY = new TH1F(tmp,title,2000,-1000,1000);   
  sprintf(tmp,"%s%s%i%s%i%s%i","fResS0","_Sector_",sector,"_ModA_",modA,"_ModB_",modB);
  fResS0 = new TH1F(tmp,title,200,-1,1); 
  sprintf(tmp,"%s%s%i%s%i%s%i","fResS1","_Sector_",sector,"_ModA_",modA,"_ModB_",modB);
  fResS1 = new TH1F(tmp,title,200,-1,1);     
  sprintf(tmp,"%s%s%i%s%i%s%i","fAccResX","_Sector_",sector,"_ModA_",modA,"_ModB_",modB);
  fAccResX = new TH1F(tmp,title,2000,-100,100);  
  sprintf(tmp,"%s%s%i%s%i%s%i","fAccResY","_Sector_",sector,"_ModA_",modA,"_ModB_",modB);
  fAccResY = new TH1F(tmp,title,2000,-100,100);       
  sprintf(tmp,"%s%s%i%s%i%s%i","fAccResS0","_Sector_",sector,"_ModA_",modA,"_ModB_",modB);
  fAccResS0 = new TH1F(tmp,title,2000,-1,1);  
  sprintf(tmp,"%s%s%i%s%i%s%i","fAccResS1","_Sector_",sector,"_ModA_",modA,"_ModB_",modB);
  fAccResS1 = new TH1F(tmp,title,2000,-1,1);  

  //Histograms with final parameters!!
  sprintf(tmp,"%s%s%i%s%i%s%i","fFAccResX","_Sector_",sector,"_ModA_",modA,"_ModB_",modB);
  fFAccResX = new TH1F(tmp,title,2000,-100,100);  
  sprintf(tmp,"%s%s%i%s%i%s%i","fFAccResY","_Sector_",sector,"_ModA_",modA,"_ModB_",modB);
  fFAccResY = new TH1F(tmp,title,2000,-100,100);       
  sprintf(tmp,"%s%s%i%s%i%s%i","fFAccResS0","_Sector_",sector,"_ModA_",modA,"_ModB_",modB);
  fFAccResS0 = new TH1F(tmp,title,2000,-1,1);  
  sprintf(tmp,"%s%s%i%s%i%s%i","fFAccResS1","_Sector_",sector,"_ModA_",modA,"_ModB_",modB);
  fFAccResS1 = new TH1F(tmp,title,2000,-1,1);  

  fOutRoot->cd(oldDirName);
}





/*
void HMdcAligner :: fitHistograms(void)
{
//
//Fits to a gaussian the four relevant histograms
//and obtains the fit parameters for further data selection
//

TF1 *f1 = new TF1("f1","gaus",-100,100);

fFAccResX->Fit("f1","RQN");
//Fit parameters
Float_t fitPar0     = f1->GetParameter(0);  // constant
Float_t fitPar1     = f1->GetParameter(1);  // mean
Float_t fitPar2     = f1->GetParameter(2);  // sigma
// Cambiar el dos por un parametros ajustable por funcion publica
fXArea = 2*fitPar2;

fFAccResY->Fit("f1","RQN");
fitPar0     = f1->GetParameter(0);  // constant
fitPar1     = f1->GetParameter(1);  // mean
fitPar2     = f1->GetParameter(2);  // sigma
fYArea = 2*fitPar2;

//fFAccResS0->Fit();
//fFAccResS1->Fit();
}
*/





void HMdcAligner :: setTree(void)
{
  //
  // Inits TNtuple
  //

  Int_t sector = fLocA[0];
  Int_t modA = fLocA[1];
  Int_t modB = fLocB[1];
  
  fRecCount++; 
  if(!fOutRoot) fOutRoot = new TFile(fNameRoot,"RECREATE");

  Char_t title[50], tmp[50];
  sprintf(title,"%s%i%s%i%s%i","Sector_",sector,"_ModA_",modA,"_ModB_",modB);
  
  sprintf(tmp,"%s%s%i%s%i%s%i","All","_Sector_",sector,"_ModA_",modA,"_ModB_",modB);
  fAlignAll = new TTree(tmp,title);
  fAlignAll->Branch("hits",&fHits);
}





Bool_t HMdcAligner :: init(void)
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

  setParContainers(); 

  /*
  if(fAuto == kFALSE && fManual == kFALSE) setGeomAuxPar();
  else if(fAuto == kTRUE && fManual == kFALSE) setGeomAuxParSim();
  */
  if(fHistograms!=kTRUE) setHistograms();
  else setTree();
 
  return kTRUE;
}





Bool_t HMdcAligner :: reinit(void)
{
  //
  // Call the functions returning auxiliar geometrical parameters
  //
  
  if(fAuto == kFALSE && fManual == kFALSE) setGeomAuxPar();
  else if(fAuto == kTRUE && fManual == kFALSE) setGeomAuxParSim();
  
  return kTRUE;
}





void HMdcAligner :: setGeomParAutoOn(void)
{
  //
  //Turn on the automatic geometrical parameter input
  //

  fAuto =kTRUE;
}





void HMdcAligner :: setGeomParManOn(void)
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





void HMdcAligner :: setControlHistoOff(void)
{  
  //
  // Disables control histograms output (saving memory)
  //

  fHistograms = kTRUE;  
}





void HMdcAligner :: setParContainers(void)
{
  //
  // Loads the parameter containers it uses later
  //

  fMdcGeomPar=(HMdcGeomPar*)gHades->getRuntimeDb()->getContainer("MdcGeomPar");
  if(!fMdcGeomPar){
    fMdcGeomPar=(HMdcGeomPar*)gHades->getSetup()->createDetGeomPar("Mdc");
  }

}





void HMdcAligner :: setGeomAuxPar(void)
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
  cout << endl <<"Debugging output in HMdcAligner::setGeomAuxPar" << endl;
  cout << "Original transformation from container" << endl;
  cout << "MDC A (Module " << moduleA << ")" << endl;
  transformA.print();  
  cout << "MDC B (Module " << moduleB << ")" << endl;
  transformB.print();

  //From the previous transformation, get the relative transformation 
  //   M = [R(B)]E-1 R(A)
  //   V = [R(B)]E-1 [T(A)-T(B)] 

  HGeomRotation rotBinv = rotB.inverse();
  HGeomRotation relrot = rotBinv * rotA;
  HGeomVector relvector =  rotBinv * (vectorA - vectorB);
  
  //Printing relative transformation
  cout << "Relative transformation: " << endl;
  relrot.print();
  relvector.print();

  //From the relative rotation, get the euler angles (radians)
  //ATT! It is an R-1 Euler matrix! 
  Double_t euler[3];
  //Checking if relrot(8) is in the acos() domain
  if(relrot(8)< 1 && relrot(8)>-1) euler[1] = acos(relrot(8));
  else euler[1]=0;
  Double_t sinaux;
  if(euler[1] == 0.){
    //euler[0] and euler[2] are equivalent. Write all in euler[0]
    euler[0]= (TMath::Pi()/2)+acos(relrot(0));
    euler[2]=-(TMath::Pi()/2);
  } 
  else{
    sinaux = sin(euler[1]);
    euler[0] = atan2(relrot(7)/sinaux,relrot(6)/sinaux);
    euler[2] = atan2(relrot(5)/sinaux,-relrot(2)/sinaux) ;
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

  fillRotMatrix(euler[0],euler[1],euler[2]);
  fillTranslation(relvector.getX(),relvector.getY(),relvector.getZ());
  setEulerAngles(euler[0],euler[1],euler[2]);

  //printing
  cout << "HMdcAligner::setGeomAuxPar: from geom params: " << endl;
  cout << "Rotation: " << euler[0] << ", " << euler[1]  << ", " << euler[2] << endl;
  cout << "Translation: " << relvector.getX() << ", " << relvector.getY() 
       << " , " <<  relvector.getZ() << endl;

}





void HMdcAligner :: setGeomAuxParSim(Float_t x=0,Float_t y=0, Float_t z=0, Float_t prim=0,Float_t segu=0, Float_t terc=0)
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

  cout << "WARNING: Introducing automatically Geometrical" << endl; 
  cout << "Parameters without containers" << endl;

  //  Int_t sector = fLocA[0];
  Int_t moduleA = fLocA[1];
  Int_t moduleB = fLocB[1];

  if(x!=0 || y!=0 || z!=0 || prim!=0 || segu!=0 || terc!=0){
    fillRotMatrix(prim,segu,terc);
    fillTranslation(x,y,z);  
    setEulerAngles(prim,segu,terc);
  }
  else{
    if(moduleA ==0  && moduleB ==1 ){
      fillRotMatrix(1.570796326795,0.171026,-1.570796326795);
      fillTranslation(0,-95.790,-169.571);  
      setEulerAngles(1.570796326795,0.171026,-1.570796326795);
    }
    if(moduleA ==1  && moduleB ==2 ){
      fillRotMatrix(1.570796326795,-0.08531169,-1.570796326795);
      fillTranslation(0,61.707,-655.130);    
      setEulerAngles(1.570796326795,-0.08531169,-1.570796326795);
    }
    if(moduleA ==2  && moduleB ==3 ){
      fillRotMatrix(1.570796326795,0,-1.570796326795);
      fillTranslation(0,0,-299.994);   
      setEulerAngles(1.570796326795,0,-1.570796326795);
    }  
    if(moduleA ==0  && moduleB ==2 ){
      fillRotMatrix(1.570796326795,0.08571377,-1.570796326795);
      fillTranslation(0,-48.183,-815.923);    
      setEulerAngles(1.570796326795,0.08571377,-1.570796326795);
    }  
    if(moduleA ==0  && moduleB ==3 ){
      fillRotMatrix(1.570796326795,0.08571377,-1.570796326795);
      fillTranslation(0,-48.183,-1115.917);    
      setEulerAngles(1.570796326795,0.08571377,-1.570796326795);
    }  
    //  if(moduleA ==1  && moduleB ==3 ){
    //    fillRotMatrix(1.570796326795,-0.08531169,-1.570796326795);
    //    fillTranslation(,,);      
    //    setEulerAngles(1.570796326795,-0.08531169,-1.570796326795);
    //  }
  }
}





void HMdcAligner :: setModstoAlign(Int_t sector, Int_t modA, Int_t modB)
{ 
  //
  //Sets the modules to align. Alignment procedure proyects hits 
  //of modA in modB coordinate system and compares
  //

  fLocA.set(2,sector,modA); //MDC A 
  fLocB.set(2,sector,modB); //MDC B 
  
}





Int_t HMdcAligner :: execute(void)
{    
  // 
  // Iteration in the hit category. Fills histograms
  // and TTree and calculates relevant quantities
  //

  HMdcHit* pHitA;       //  HMdcHitAux* pHitA;  
  HMdcHit* pHitB;       //  HMdcHitAux* pHitB; 

  Float_t xDir, yDir, aux;       
  Float_t xsearch, ysearch, zsearch; 
  Float_t s0search,s1search,den;   
  Float_t origValues[4];   
  Float_t xa, xb, ya, yb, s0a, s1a, s0b, s1b;    // hit datamembers
  Float_t xlolimit,xuplimit,ylolimit,yuplimit;   // window limits
 
  Bool_t used = kFALSE; 
  Bool_t invalid = kFALSE; 

  fIter1->Reset(); 
  fIter1->gotoLocation(fLocA); 
  while ( (pHitA =(HMdcHit*)fIter1->Next()) != 0 ){ 

    fHitsMdcA++;
    used = kFALSE;                   //reinit control flag
    invalid = kFALSE;                //reinit control flag
    xa = pHitA->getX();              //getting the hit info in MDC A     
    ya = pHitA->getY();
    xDir = pHitA->getXDir(); 
    yDir = pHitA->getYDir();

    //If using HMdcHit, it is neccesary this change
    aux = sqrt(1 - xDir * xDir - yDir * yDir);
    if(aux == 0.) {s0a=1; s1a=1;} //non-sense values
      else{
	s0a = xDir/aux;
	s1a = yDir/aux;
      }

    //calculation of cross point in MDC B    
    zsearch = -(fRotMat(2)*xa + fRotMat(5)*ya + fTranslation(2)) / 
      (fRotMat(8) + fRotMat(2)*s0a + fRotMat(5)*s1a); 
    
    xsearch = xa*fRotMat(0) + ya*fRotMat(3) + fTranslation(0) + 
      zsearch*(s0a*fRotMat(0) + s1a*fRotMat(3) + fRotMat(6));
    
    ysearch = xa*fRotMat(1) + ya*fRotMat(4) + fTranslation(1) + 
      zsearch*(s0a*fRotMat(1) + s1a*fRotMat(4) + fRotMat(7));

    //For getting the histograms in slopes and also for new cuts!!
    den = s0a*fRotMat(2) + s1a*fRotMat(5) + fRotMat(8);
    s0search = (s0a*fRotMat(0) + s1a*fRotMat(3) + fRotMat(6)) / den;
    s1search = (s0a*fRotMat(1) + s1a*fRotMat(4) + fRotMat(7)) / den;
    
    //preparing the cuts  <=========== MODIFY THIS CONDITION
    xlolimit = xsearch - fXArea;
    xuplimit = xsearch + fXArea;
    ylolimit = ysearch - fYArea;
    yuplimit = ysearch + fYArea;
    
    //Iteration on the second MDC (MDC B) for each hit in the first (MDC A)
    fIter2->Reset(); 
    fIter2->gotoLocation(fLocB); 
    while ( (pHitB =(HMdcHit*)fIter2->Next()) != 0 ){ 	
      
      xb = pHitB->getX();       //getting the hit info in MDC B
      yb = pHitB->getY();
      xDir = pHitB->getXDir();
      yDir = pHitB->getYDir();
      //if using hit!
      aux = sqrt(1 - xDir * xDir - yDir * yDir);    
      if(aux == 0.){ s0b=1; s1b=1;} //non-sense values
      else{
	s0b = xDir/aux;
	s1b = yDir/aux;
      }
      
      fResX->Fill(xb-xsearch);
      fResY->Fill(yb-ysearch);
      fResS0->Fill(s0b-s0search);
      fResS1->Fill(s1b-s1search);

      //let's use this condition  <=========== MODIFY THIS CONDITION

      if( (xb>xlolimit) && (xb<xuplimit) && (yb>ylolimit) && (yb<yuplimit)) {
	fHitsFoundInWindow++;
	
	if(used == kFALSE){       
	  
	  // fHits->Clear();
	  new((*fHits)[0])HMdcHit(*pHitB);
	  new((*fHits)[1])HMdcHit(*pHitA);	  

	  origValues[0]= xb-xsearch;
	  origValues[1]= yb-ysearch;  
	  origValues[2]= s0b-s0search;
	  origValues[3]= s1b-s1search;

	  fCount ++;
	  used = kTRUE;	
	  
	}
	else{	      
	  //the hit has already another partner!! Discart this HIT A
	  if(invalid == kFALSE){
	    fCount--;  
	    invalid = kTRUE;
	    fDiscart++;	
	  }   //  end of if(invalid == kFALSE){
	}   //  end of   if(used == kFALSE){  } else{ 
      }	  //  end of if( (xb>xlolimit) && (xb<xuplimit) ...
    }   //  end of while ( (pHitB =(HMdcHit*)iter2->Next()) != 0 ){
    
    if(used == kTRUE && invalid != kTRUE){
      fAlignAll->Fill();
      fHits->Clear();
      fAccResX->Fill(origValues[0]);
      fAccResY->Fill(origValues[1]);
      fAccResS0->Fill(origValues[2]);
      fAccResS1->Fill(origValues[3]);      
      if(fCount%100 ==0) cout << "."<< flush;
    } 
    
  }   //  end of while ( (pHitA =(HMdcHit*)iter->Next()) != 0 ){ 

  return 0; 
}





Bool_t HMdcAligner :: finalize(void)
{   
  // 
  // Statistical information and Minimization procedure
  //

  Int_t sector = fLocA[0];
  Int_t modA = fLocA[1];
  Int_t modB = fLocB[1];

  ofstream *fout=NULL;
  if (fNameAscii) fout = new ofstream(fNameAscii.Data(), ios::app);
  if (*fout){
    *fout << endl << "Sector: " << sector << endl;
    *fout << "Module A: " << modA << "  Module A: " << modB << endl << endl; 
    *fout << "Number of events: "  << fNEntries << endl;  
    *fout << "Hits in MDC A: "  << fHitsMdcA << endl;
    *fout << "Window (mm): " << fXArea <<"," << fYArea << endl;
    *fout << "Hits found in MDC B window: " << fHitsFoundInWindow << endl;
    *fout << "Valid hits for alignment: " << fCount << endl;
    *fout << "Discarted events (more than one option): " << fDiscart << endl;
   }
  //cout << "Number of events: "  << fNEntries << endl;    
  //cout << "Hits in MDC A: "  << fHitsMdcA << endl;
  //cout << "Window (mm): " << fXArea <<"," << fYArea << endl;
  //cout << "Hits found in MDC B window: " << fHitsFoundInWindow << endl;
  //cout << "Valid hits for alignment: " << fCount << endl;
  //cout << "Discarted hits (more than one option): " << fDiscart << endl;
  


  if (*fout){
    *fout << "Transformation before minimization (init values): "  << endl;
    *fout << fEuler(0) << ", " << fEuler(1) << ", " << fEuler(2) << ", " 
	  << fTranslation(0) << ", " << fTranslation(1)<< ", " 
	  << fTranslation(2)<< endl;
   }

  Float_t OldEuler[3];
  Float_t OldTranslation[3];
  Int_t IterCounter =0;
  Float_t IterCri;

  do{
    IterCri = 0;
    for(Int_t i=0;i<3;i++){
      OldEuler[i] = fEuler(i);
      OldTranslation[i] = fTranslation(i);
    }
    
    minfit(0,fEuler(0),fEuler(1),fEuler(2),fTranslation(0),fTranslation(1),fTranslation(2));
    if (*fout){
      *fout << "Parameters after minimization "  << endl;
      *fout << fEuler(0) << "+-" << fError[0] << ", " << fEuler(1) << "+-" << fError[1] << ", " 
	    << fEuler(2) << "+-" << fError[2] << ", " << fTranslation(0) << "+-" 
	    << fError[3] << ", " << fTranslation(1) << "+-" << fError[4] << ", " 
	    << fTranslation(2) << "+-" << fError[5] << endl;
    }
    for(Int_t i=0;i<3;i++){
      IterCri += (fEuler(i)-OldEuler[i])/fEuler(i);
      IterCri += (fTranslation(i)-OldTranslation[i])/fTranslation(i);
      }
    IterCounter++;
    if(IterCounter>10) {
      cout << "WARNING in HMdcAligner :: finalize" << endl;
      cout << "Sector: " << sector << " ModuleA: " << modA <<  " ModuleB: " << modB << endl;
      cout << "More than 10 iterations without results!" <<endl;
      break;
    }
  }while(IterCri>fIterCriteria);

  //Fill the rotation matrix with the new obtained values 
  fillRotMatrix(fEuler(0),fEuler(1),fEuler(2));

  //Recalculate all histograms for the new parameters!!!
  fillHistograms();
  storeInFile();

  // close ascii file
  fout->close();
  delete fout;
  
  return kTRUE; 
}





void HMdcAligner :: fillHistograms (){ 
  //
  // Performs the graphical output from obtained parameters
  //   

  HMdcHit* hitA;      
  HMdcHit* hitB;      

  Float_t xsearch, ysearch, zsearch; 
  Float_t s0search,s1search,den; 
  // candidates to fill the TNtuple
  Float_t xa, xb, ya, yb, s0a, s1a, s0b, s1b;    // Hits datamembers
  Float_t xDir,yDir,aux;

  Stat_t entries = fAlignAll->GetEntries();
  
  for (Int_t i=0;i<entries;i++) { 
    fAlignAll->GetEntry(i);
    //    data = fAlignAll->GetArgs();

    hitA = (HMdcHit*) fHits->At(1);
    hitB = (HMdcHit*) fHits->At(0);
    xa = hitA->getX();
    ya = hitA->getY();       
    xDir = hitA->getXDir();
    yDir = hitA->getYDir(); 
    //If using HMdcHit, it is neccesary this change
    aux = sqrt(1 - xDir * xDir - yDir * yDir);
    if(aux == 0.) {
      s0a=1; 
      s1a=1; 
      cout<< "ERROR #1 in HMdcAligner::fcnalign().";
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
      cout<< "ERROR #2 in HMdcAligner::fcnalign().";
    }
    else{
      s0b = xDir/aux;
      s1b = yDir/aux;
    }

    //calculation of cross point in MDC B    
    zsearch = -(fRotMat(2)*xa + fRotMat(5)*ya + fTranslation(2)) / 
      (fRotMat(8) + fRotMat(2)*s0a + fRotMat(5)*s1a); 
    
    xsearch = xa*fRotMat(0) + ya*fRotMat(3) + fTranslation(0) + 
      zsearch*(s0a*fRotMat(0) + s1a*fRotMat(3) + fRotMat(6));
    
    ysearch = xa*fRotMat(1) + ya*fRotMat(4) + fTranslation(1) + 
      zsearch*(s0a*fRotMat(1) + s1a*fRotMat(4) + fRotMat(7));

    //For getting the histograms in slopes and also for new cuts!!
    den = s0a*fRotMat(2) + s1a*fRotMat(5) + fRotMat(8);
    s0search = (s0a*fRotMat(0) + s1a*fRotMat(3) + fRotMat(6)) / den;
    s1search = (s0a*fRotMat(1) + s1a*fRotMat(4) + fRotMat(7)) / den;

    fFAccResX->Fill(xb-xsearch);
    fFAccResY->Fill(yb-ysearch);
    fFAccResS0->Fill(s0b-s0search);
    fFAccResS1->Fill(s1b-s1search);  
    
  } // end of for (Int_t i=0; i<nentries;i++) {
}





void HMdcAligner :: storeInFile(void)
{  
  //
  // Stores all histos and tree in the Root file
  //

  //Esto mprovocaba los errores!!
  //TFile *fOutRoot = new TFile(fNameRoot,"RECREATE");

  Int_t sector = fLocA[0];
  Int_t modA = fLocA[1];
  Int_t modB = fLocB[1];

  fAlignAll->Write();

  const char* oldDirName = gDirectory->GetPath();
  static Char_t newDirName[255];
  sprintf(newDirName,"%s%s%i%s%i%s%i","Aligner_","S_",sector,"_ModA_",modA,"_ModB_",modB);
  fOutRoot->cd(newDirName);

  fFAccResX->Write();
  fFAccResY->Write();
  fFAccResS0->Write();
  fFAccResS1->Write(); 

  fResX->Write();
  fResY->Write();
  fResS0->Write(); 
  fResS1->Write();
  fAccResX->Write();
  fAccResY->Write();
  fAccResS0->Write();
  fAccResS1->Write(); 

  //Enter in the file the HMdcGeomRotation HMdcGeomVector
  fRotMat.Write();
  fTranslation.Write();
  
  fOutRoot->cd(oldDirName);
  fOutRoot->Write();
  
  fRecCount--;
  if(!fRecCount)fOutRoot->Close();
}





void HMdcAligner :: fillRotMatrix(Float_t prim,Float_t segu, Float_t terc){ 
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





void HMdcAligner :: fillTranslation(Float_t x,Float_t y, Float_t z){ 
  //
  // Translation from MDC A to MDC B
  //
 
  fTranslation.setX(x); 
  fTranslation.setY(y);
  fTranslation.setZ(z); 
}





void HMdcAligner :: setEulerAngles(Float_t f,Float_t s, Float_t t){ 
  //
  // Euler angles in transformation from MDC A to MDC B
  //
 
  fEuler.setX(f); 
  fEuler.setY(s); 
  fEuler.setZ(t); 
} 





void HMdcAligner :: setSearchLimits(Float_t x, Float_t y){ 
  //
  // Limits of the square defined in the search procedure
  //
 
  fXArea = x; 
  fYArea = y; 
} 





void HMdcAligner :: setIterCriteria(Float_t cri){ 
  //
  // Set the criteria for iteration in the minimization (see finalize())
  //
 
  fIterCriteria = cri; 
}





void HMdcAligner :: setWeights(Float_t w0,Float_t w1,Float_t w2,Float_t w3){ 
  //
  // Set the weights in the fcn()
  //
 
  fWeights[0]= w0; 
  fWeights[1]= w1; 
  fWeights[2]= w2; 
  fWeights[3]= w3; 

}





void HMdcAligner :: setSteps(Float_t s0,Float_t s1,Float_t s2,Float_t s3, Float_t s4, Float_t s5){ 
  //
  // Set the steps in the minimization
  //
 
  fSteps[0]= s0; 
  fSteps[1]= s1; 
  fSteps[2]= s2; 
  fSteps[3]= s3;   
  fSteps[4]= s4; 
  fSteps[5]= s5;
  cout << "Steps in the minimization adjusted to " << s0 << ", " << s1 << ", " << s2 << ", " << s3 
       << ", " << s4 << ", " << s5 << endl;
}





void HMdcAligner :: setLimits(Float_t l0,Float_t l1,Float_t l2,Float_t l3, Float_t l4, Float_t l5){ 
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





void HMdcAligner :: minfit(Int_t fix, Float_t fE, Float_t sE, Float_t tE, Float_t fT, Float_t sT, Float_t tT){ 
  //
  // Minuit menu
  // Put first argument to 1 to fix angular values   
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
  minuit->SetFCN(fcnalign); 
  minuit->SetObjectFit(this);
  if(A_DEBUG>0){
    cout << "HMdcAligner::minfit()" <<endl;
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

  //FIXING 
  if(fix == 1) minuit->FixParameter(0);     
  if(fix == 1) minuit->FixParameter(1); 
  if(fix == 1) minuit->FixParameter(2); 
  
  //To remove all printout
  if(A_DEBUG<2){ 
    minuit->SetPrintLevel(-1);
  }

  //args is the array of the numerical arguments of the command 
  //the third field is the number of arguments esp3cified 
  //For MIGRAD arguments are maxcalls and tolerance, both optionals 
  minuit->mnexcm("MIGRAD",args,0,err); 
  //minuit->mnexcm("SIMPLEX",args,0,err); 
  
  Double_t parresult[6];
  minuit->GetParameter(0,parresult[0],fError[0]);
  minuit->GetParameter(1,parresult[1],fError[1]);
  minuit->GetParameter(2,parresult[2],fError[2]);
  minuit->GetParameter(3,parresult[3],fError[3]);
  minuit->GetParameter(4,parresult[4],fError[4]);
  minuit->GetParameter(5,parresult[5],fError[5]);

  fEuler.setX(parresult[0]); 
  fEuler.setY(parresult[1]); 
  fEuler.setZ(parresult[2]); 
  fTranslation.setX(parresult[3]);  
  fTranslation.setY(parresult[4]);
  fTranslation.setZ(parresult[5]);

  if (err != 0) cout << endl <<"MINIMIZATION EXITED WITH ERROR " << err << endl;

}




void fcnalign(Int_t &npar, Double_t *gin, Double_t &f, Double_t *par, Int_t iflag){ 

  //
  // This function contains the functional to minimize
  //

  Double_t chisq = 0.; 
  Double_t zproy, resx, resy, ress0, ress1; 
  Double_t rotmat[3][3]; 
  Double_t translation[3]; 
  Double_t prim, segu, terc; 
  Float_t pxa, pya, ps0a, ps1a, pxb, pyb, ps0b, ps1b;
  Float_t xDir,yDir,aux;

  prim = par[0]; 
  segu = par[1]; 
  terc = par[2]; 
  translation[0] = par[3]; 
  translation[1] = par[4]; 
  translation[2] = par[5]; 

  if (A_DEBUG>2){
    cout << "Parameters: " << par[0] << "," << par[1] << "," <<  par[2] 
	 << "," << par[3] << ","  << par[4] << ","  << par[5] << endl;
  }
  
  rotmat[0][0] = (cos(prim) * cos(segu) * cos(terc)) - (sin(prim) * sin(terc)); 
  rotmat[1][0] = ( - cos(prim) * cos(segu) * sin(terc)) - (sin(prim) * cos(terc)); 
  rotmat[2][0] = (cos(prim) * sin(segu)); 
  rotmat[0][1] = (sin(prim) * cos(segu) * cos(terc)) + (cos(prim) * sin(terc)); 
  rotmat[1][1] = ( - sin(prim) * cos(segu) * sin(terc)) + (cos(prim) * cos(terc)); 
  rotmat[2][1] = (sin(prim) * sin(segu)); 
  rotmat[0][2] = ( - sin(segu) * cos(terc)); 
  rotmat[1][2] = (sin(segu) * sin(terc)); 
  rotmat[2][2] = (cos(segu)); 
  
  //Data from the TTree

  TTree* pData; 
  HMdcAligner* pAlign;  
  TClonesArray* theHits;
  pAlign = (HMdcAligner*)(gMinuit->GetObjectFit());
  pData = pAlign->getTree();
  //  theHits = pAlign->getHits();
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
    //data = pData->GetArgs();
    //filling from the ntuple
    hitA = (HMdcHit*) theHits->At(1);
    hitB = (HMdcHit*) theHits->At(0);
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
    zproy = -(rotmat[2][0]*pxa + rotmat[2][1]*pya + translation[2]) / 
      (rotmat[2][2] + rotmat[2][0]*ps0a + rotmat[2][1]*ps1a);  
    
    resx = pxb - (pxa*rotmat[0][0] + pya*rotmat[0][1] + translation[0] + 
		  zproy*(ps0a*rotmat[0][0] + ps1a*rotmat[0][1] + rotmat[0][2])); 
     
    resy = pyb - (pxa*rotmat[1][0] + pya*rotmat[1][1] + translation[1] + 
		  zproy*(ps0a*rotmat[1][0] + ps1a*rotmat[1][1] + rotmat[1][2])); 
     
    ress0 = ps0b - ((rotmat[0][0]*ps0a + rotmat[0][1]*ps1a + rotmat[0][2]) / 
		    (rotmat[2][0]*ps0a + rotmat[2][1]*ps1a + rotmat[2][2])); 
     
    ress1 = ps1b - ((rotmat[1][0]*ps0a + rotmat[1][1]*ps1a + rotmat[1][2]) / 
		    (rotmat[2][0]*ps0a + rotmat[2][1]*ps1a + rotmat[2][2])); 

    //Some extra debbuging (always needed ;-) )
    if(A_DEBUG >3){
      cout << "  ++++++++  VALUES IN fcnalign()  +++++++++ " << endl;
      cout << "HITA: " <<  pxa << ", " <<  pya << ", " <<  ps0a << ", " <<  ps1a << endl;
      cout << "HITB: " <<  pxb << ", " <<  pyb << ", " <<  ps0b << ", " <<  ps1b << endl;
      cout << "Dist: " << resx << ", " << resy << ", " << ress0 << ", " << ress1 << endl;
    }
 
    //Choose the chisquare funcion 
    //chisq += resx*resx*VI00 + 2*resx*ress0*VI01 + resy*resy*VI22 
    //+ 2*resy*ress1*VI23 + ress0*ress0*VI11 + ress1*ress1*VI33; 
    //chisq += resx*resx/400. + resy*resy/213. + ress0*ress0/0.0037 + ress1*ress1/0.0027; 
    chisq += resx*resx/weights[0] + resy*resy/weights[1] 
      + ress0*ress0/weights[2] + ress1*ress1/weights[3]; 
  } 
    f = chisq; 
    if(A_DEBUG>2){
      cout  << "chisqr= " << chisq << " out of " << entries << " combinations."<< endl; 
    }
} 
 
