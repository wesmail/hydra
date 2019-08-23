
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
   downscaling= 0;
   downscalingFlag = 0;
   triggerDecision = 0;
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
   downscaling= 0;
   downscalingFlag = 0;
   triggerDecision = 0;
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

  for(int i=0;i<10;i++) contents[i]=0;

  // cout << "MU max length  " << maxLength << endl;  

  while ( (count < maxLength) && (count<10) ) {
      contents[count] = *ptr;
      count++;
      ptr++;
  } 

  // for(int i=count;i<10;i++) contents[i]=0;

  // printf(" triggerCodeFirstFormat %i \n",fTriggerCodeFirstFormat);
  if (fTriggerCodeFirstFormat==1) {
    //   triggerCode= contents[0];
    version = contents[0];
    downscaling = (contents[1] & VEC(0xfff,0) );
    downscalingFlag = (contents[1] & BIT(13) );
    triggerDecision = (contents[1] & BIT(14) );
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
//     printf("- MU data block %i-%i  \n",i,contents[i]);

  
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

/*num_of_hits_in_SHOWER
3    num_of_hits_in_TOF
 
4    sector_hitpattern_RICH     bit-hitpattern, where every bit (0-5) describes
                                at least one hit in this sector (0-5)
5    sector_hitpattern_SHOWER
6    sector_hitpattern_TOF
 
7    num_of_leptons             number of leptons found in whole event
## for(i=0; i<num_of_leptons; i++) {
8    lepton (MSW)                       describes found lepton, theta, phi
                       
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




