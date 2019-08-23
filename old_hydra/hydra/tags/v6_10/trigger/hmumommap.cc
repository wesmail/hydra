#pragma implementation

#include <iostream.h>
#include "hades.h"
#include "hspectrometer.h"
/*
#include "hparhadasciifileio.h"
*/
#include "hlocation.h"
#include "hspectrometer.h"
#include "hmumommap.h"

ClassImp(HMUMomMap)
  
//-----------------------------------------------------------------------------------------------  
  HMUMomMap::HMUMomMap() {
    
    // default initialization
    for (Int_t xx=0;xx<64;xx++) {
      for (Int_t yy=0;yy<64;yy++) {
	for (Int_t zz=0;zz<8;zz++) {
	  mu_mom[xx][yy][zz]=0;
	}
      }
    }
    
    Int_t Delta_th, th, ph;
    Float_t mom;
    
    Int_t k, fileMom_status;
    FILE *fpMom;    
    
    fpMom = fopen("/u/sanchez/mutable.txt","r");
    
    if(fpMom){
      
      for(k=0; ((k<32768)&& (!feof(fpMom)) ) ; k++) {
	fileMom_status = fscanf(fpMom,"%i%i%i%f",&th,&Delta_th,&ph,&mom);
	mu_mom[th][Delta_th][ph]=0;
	
	if((Delta_th>-1) && (Delta_th <64) && (th>-1) && (th<64) && (ph>-1) && (ph<8) ) {
	  mu_mom[th][Delta_th][ph] = mom;
	}
      }
      
      if(fpMom) fclose(fpMom);
    }
}
//==============================================================================================

//-----------------------------------------------------------------------------------------------  
HMUMomMap::~HMUMomMap()  {
}
//==============================================================================================













