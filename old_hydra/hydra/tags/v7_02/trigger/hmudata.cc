#include "hevent.h"
#include "heventheader.h"
#include "hmudata.h"
#include "Rtypes.h"
#include <iostream>
///////////////////////////////////////////////////////////////////////////
//HMUData
// 
///////////////////////////////////////////////////////////////////////////

#define VEC(x,shift)      ( x << shift )

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
fTriggerCodeFirstFormat=1;

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


  for(Int_t i=0;i<10;i++) {
    contents[i]=0; 
  }

  count = 0;
  while( (count<maxLength) && (count<10) ) {
      contents[count] = *ptr;
      count++;
      ptr++;
  } 

  if (fTriggerCodeFirstFormat==1) {
    version = contents[0];
    downscaling = (contents[1] & (0x0fff) );
    downscalingFlag = ( contents[1] & BIT(12) ) >>12;
    triggerDecision = ( (contents[1] & VEC(0x7,13)) >>13 );
    nHitsRich = contents[2];
    nHitsShower = contents[3];
    nHitsTof = contents[4];
    secHitPatternRich=contents[5];
    secHitPatternShower=contents[6];
    secHitPatternTof=contents[7];
    nLeptons=contents[8];
    contents[9]=0;

    eventhead->setDownscaling(downscaling);
//     eventhead->setDownscalingFlag(downscalingFlag);
//     eventhead->setTriggerDecision(triggerDecision);
// downscaling flag and trigger decision are already written 
// in the eventheader by the event unpacker

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

void HMUData::Streamer(TBuffer &R__b)
{
   // Stream an object of class HMUData.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      TObject::Streamer(R__b);
      R__b >> muLength;
      R__b >> version;
      R__b >> triggerCode;
      R__b >> nHitsRich;
      R__b >> nHitsShower;
      R__b >> nHitsTof;
      R__b.ReadStaticArray(scalers);
      R__b >> secHitPatternRich;
      R__b >> secHitPatternShower;
      R__b >> secHitPatternTof;
      R__b >> nLeptons;
      R__b >> nDiLeptons;
      R__b >> fTriggerCodeFirstFormat;
      if(R__v<2)
      {
	  R__b >> eventhead;
	  eventhead=0;
      }
      R__b.CheckByteCount(R__s, R__c, HMUData::IsA());
   } else {
      R__c = R__b.WriteVersion(HMUData::IsA(), kTRUE);
      TObject::Streamer(R__b);
      R__b << muLength;
      R__b << version;
      R__b << triggerCode;
      R__b << nHitsRich;
      R__b << nHitsShower;
      R__b << nHitsTof;
      R__b.WriteArray(scalers, 20);
      R__b << secHitPatternRich;
      R__b << secHitPatternShower;
      R__b << secHitPatternTof;
      R__b << nLeptons;
      R__b << nDiLeptons;
      R__b << fTriggerCodeFirstFormat;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

ClassImp(HMUData)





