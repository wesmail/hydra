#include "hevent.h"
#include "heventheader.h"
#include "hmudata.h"
#include <iostream>
///////////////////////////////////////////////////////////////////////////
//HMUData
// 
///////////////////////////////////////////////////////////////////////////

#define VEC(x,shift)      ( x << shift )
#define BIT(shift)        ( 1 << shift )
                                               


HMUData::HMUData(){
version= 0;
triggerCode= 0;
nHitsRich= 0;
nHitsShower= 0;
nHitsTof=  0;
secHitPatternRich= 0;
secHitPatternShower= 0;
secHitPatternTof= 0;
nLeptons=0;
nDiLeptons=0;


}


void HMUData::clear(){
   version= 0;
   triggerCode= 0;
   nHitsRich= 0;
   nHitsShower= 0;
   nHitsTof=  0;
   secHitPatternRich= 0;
   secHitPatternShower= 0;
   secHitPatternTof= 0;
   nLeptons=0;
}



Int_t HMUData::decode (UInt_t *ptr, Int_t maxLength){
  Int_t count=0,r=0;
  UInt_t contents[12];

  UInt_t downscaling=0;
  UInt_t downscalingFlag=0;
  UInt_t triggerDecision=0;
eventhead = gHades->getCurrentEvent()->getHeader();

  for(int i=0;i<10;i++) contents[i]=0;

  while ( (count < maxLength) && (count<10) ) {
      contents[count] = *ptr;
      count++;
      ptr++;
  } 

  if (fTriggerCodeFirstFormat==1) {
    //   triggerCode= contents[0];
    version = contents[0];
    downscaling = (contents[1] & VEC(0xfff,0) );
    downscalingFlag = (contents[1] & BIT(12) );
    triggerDecision = ( (contents[1] & VEC(0x6000,0)>>13) );
    nHitsRich = contents[2];
    nHitsShower = contents[3];
    nHitsTof = contents[4];
    secHitPatternRich=contents[5];
    secHitPatternShower=contents[6];
    secHitPatternTof=contents[7];
    nLeptons=contents[8];
    contents[9]=0;

    eventhead->setDownscaling(downscaling);
    eventhead->setDownscalingFlag(downscalingFlag);
    eventhead->setTriggerDecision(triggerDecision);

  }
  else { // old mode
    triggerCode= contents[0];
    version = contents[1];
    downscaling = contents[2];
    nHitsRich = contents[3];
    nHitsShower = contents[4];
    nHitsTof = contents[5];
    secHitPatternRich=contents[6];
    secHitPatternShower=contents[7];
    secHitPatternTof=contents[8];
    nLeptons=contents[9];
  }

  
 if ( count > maxLength) {
                        Error("MU Data decode","size inconsistency"); 
                        r=-1;
                        clear();  
			return r; 
  }

 //  printf("MU-Data trigger:%i version:%x downscaling:%i
 //           nHitsRich:%i nHitsShower:%i nHitsTof:%i 
 //           secHitPatternRich:%i  \n
 //          secHitPatternShower:%i  secHitPatternTof:%i 
 //          nLeptons:%i   
 //        \n",  
 //	triggerCode,  version,  downscaling,
 //        nHitsRich, nHitsShower,nHitsTof, secHitPatternRich, 
 //        secHitPatternShower, secHitPatternTof,
 //        nLeptons 
 //         ); 

  r = count;
  return r;  
}

ClassImp(HMUData)





