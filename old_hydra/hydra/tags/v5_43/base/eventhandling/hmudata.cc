#include "hmudata.h"

///////////////////////////////////////////////////////////////////////////
//HMUData
// 
///////////////////////////////////////////////////////////////////////////




HMUData::HMUData(){
}


void HMUData::clear(){
   version=
   triggerCode=
   downscaling=
   nHitsRich=
   nHitsShower=
   nHitsTof= 
   secHitPatternRich=
   secHitPatternShower=
   secHitPatternTof=
   nLeptons=9999;
}



Int_t HMUData::decode (UInt_t *ptr, Int_t maxLength){
  Int_t count=0,r=0;
  int limit = maxLength;
  UInt_t contents[maxLength];
  for(int i=0;i<maxLength;i++) contents[i]=0;

  while ( (count < maxLength) && ( count < 10)  ) {
      contents[count] = *ptr;
      count++;ptr++;
  } 


  for(int i=count;i<10;i++) contents[i]=0;

  // printf(" triggerCodeFirstFormat %i \n",fTriggerCodeFirstFormat);
  if (fTriggerCodeFirstFormat==1) {
    //   triggerCode= contents[0];
    version = contents[0];
    downscaling = contents[1];
    nHitsRich = contents[2];
    nHitsShower = contents[3];
    nHitsTof = contents[4];
    secHitPatternRich=contents[5];
    secHitPatternShower=contents[6];
    secHitPatternTof=contents[7];
    nLeptons=contents[8];
    contents[9]=0;
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

  // for(int i=0;i<10;i++)
  //   printf("- MU data block %i-%i  \n",i,contents[i]);

  
 if ( count > maxLength) {
                        Error("MU Data decode","size inconsistency"); 
                        r=-1;
                        clear();  
			return r; 
  }

//  printf("MU-Data trigger:%i version:%x downscaling:%i
//           nHitsRich:%i nHitsShower:%i nHitsTof:%i 
//           secHitPatternRich:%i  \n
//           secHitPatternShower:%i  secHitPatternTof:%i 
//           nLeptons:%i   
//         \n",  
// 	triggerCode,  version,  downscaling,
//         nHitsRich, nHitsShower,nHitsTof, secHitPatternRich, 
//         secHitPatternShower, secHitPatternTof,
//         nLeptons 
//          ); 

  r = count;
  return r;  
}

/*
void HMUData::Streamer(TBuffer &R__b) {
   // Stream an object of class HEventHeader.

   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(); if (R__v) { }
      TObject::Streamer(R__b);
      R__b >> version;      
      R__b >> nHitsRich;   
      R__b >> nHitsShower; 

   } else {
      R__b.WriteVersion(HMUData::IsA());
      TObject::Streamer(R__b);
      R__b << version;      
      R__b << nHitsRich;   
      R__b << nHitsShower; 

   }
}
*/

ClassImp(HMUData)

