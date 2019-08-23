using namespace std;
# include <math.h>
# include <stdlib.h>
# include <iostream>
# include <iomanip> 
# include <fstream> 
# include <TMath.h>
# include <TROOT.h>
# include "hmdcmetahit.h"
# include "hades.h"
# include "hmdchit.h"
# include "mdcsdef.h"

ClassImp(HMdcMetaHit)

//*-- AUTHOR : Hector Alvarez-Pol  
//*-- Date: 05/2001
//*-- Last Update: 28/08/01
//*-- Copyright: GENP (Univ. Santiago de Compostela)
  
//_HADES_CLASS_DESCRIPTION 
///////////////////////////////////////////////////////////////////////////////
//
// HMdcMetaHit   (ongoing work)
//
// Pseudo Hit for META alignment 
//
///////////////////////////////////////////////////////////////////////////////


HMdcMetaHit::HMdcMetaHit(void){  
  //
  // Default constructor
  //
  fNum.setX(-1);
  fNum.setY(2);
  fNum.setZ(0);
}





HMdcMetaHit::HMdcMetaHit(Int_t nMDCs){  
  //
  // Default constructor
  //
  fNum.setX(-1);
  fNum.setY(nMDCs);
  fNum.setZ(0);
}





HMdcMetaHit::HMdcMetaHit(const HMdcMetaHit &other){  
  //
  // Copy constructor
  //

  fHitA = other.fHitA;    
  fHitB = other.fHitB; 
  fHitC = other.fHitC;
  fHitD = other.fHitD;
  fLocalHitA = other.fLocalHitA; 
  fLocalHitB = other.fLocalHitB;
  fNum = other.fNum;
}





HMdcMetaHit::HMdcMetaHit(Int_t nDetec, HGeomVector* hitMA,
			 Int_t nMDCs, HMdcHit* hitA,
			 HMdcHit* hitB, HMdcHit* hitC,
			 HMdcHit* hitD){
  //
  // Constructor
  //
  
  fNum.setY(nMDCs);
  fHitA  = *hitA;
  fHitB  = *hitB;
  if(nMDCs>2) fHitC  = *hitC;
  if(nMDCs>3) fHitD  = *hitD;
  fLocalHitA = *hitMA;
  fNum.setX(nDetec);
  fNum.setZ(1);
}





HMdcMetaHit::HMdcMetaHit(Int_t nDetec, HGeomVector* hitMA, HGeomVector* hitMB, 
			 Int_t nMDCs, HMdcHit* hitA,
			 HMdcHit* hitB, HMdcHit* hitC,
			 HMdcHit* hitD){
  //
  // Constructor
  //
  
  fNum.setY(nMDCs);
  fHitA  = *hitA;
  fHitB  = *hitB;
  if(nMDCs>2) fHitC  = *hitC;
  if(nMDCs>3) fHitD  = *hitD;
  fLocalHitA = *hitMA;
  fLocalHitB = *hitMB;
  fNum.setX(nDetec);
  fNum.setZ(2);
}




HMdcMetaHit::~HMdcMetaHit(void){
    // Destructor
 
}





void HMdcMetaHit::setMdcHitA(HMdcHit* hitA){
  // 
  // Fills MDC
  //
  fHitA = *hitA;
}
void HMdcMetaHit::setMdcHitB(HMdcHit* hitB){
  // 
  // Fills MDC
  //
  fHitB = *hitB;
}
void HMdcMetaHit::setMdcHitC(HMdcHit* hitC){
  // 
  // Fills MDC
  //
  fHitC = *hitC;
}
void HMdcMetaHit::setMdcHitD(HMdcHit* hitD){
  // 
  // Fills MDC
  //
  fHitD = *hitD;
}





void HMdcMetaHit::print(void){
  //
  // prints the info
  //

  cout << "******************************************************" << endl;
  cout << "Number of MDCs: "<< fNum.getY() << endl;
  cout << "Number of META Hits: "<< fNum.getZ() << endl;
  fHitA.print();
  fHitB.print();  
  if(fNum.getY()>2)fHitC.print();
  if(fNum.getY()>3)fHitD.print();
  if(fNum.getZ()>0) fLocalHitA.print();
  if(fNum.getZ()==2) fLocalHitB.print();
  cout << "In detector "<< fNum.getX() << endl; 
  cout << "KEY: 0..7->TOF modules, 8->SHOWER, >8 add the detectors" << endl; 
  cout << "*******************************************************" << endl;
}
