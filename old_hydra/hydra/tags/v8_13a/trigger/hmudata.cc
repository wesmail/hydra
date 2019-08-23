//*-- Author: Alberica Toia
//*-- Modified: Joern Wuestenfeld 07/14/2005
//*-- Version: $Id: hmudata.cc,v 1.14 2006-08-12 13:05:56 halo Exp $

#include "hevent.h"
#include "heventheader.h"
#include "hmudata.h"
#include "Rtypes.h"
#include "hades.h"
//_HADES_CLASS_DESCRIPTION 
///////////////////////////////////////////////////////////////////////////
//
//HMUData
//
// Datacontainer holding data from MatchingUnit subevent.
//
// The access to the variables is done via the following public functions:
// To get the content of a variable:
//  UInt_t getMULength()
//		Get the length of the MU subevent.
//  Bool_t isTriggerCodeFirstFormat(void)
//		Get the format of the MU subevent. 0 means old format, 1 means new format.
//  Bool_t getTriggerCodeFirstFormat()
//		Get the format of the MU subevent. 0 means old format, 1 means new format.
//  UInt_t getVersion()
//		Get the version.
//  UInt_t getTriggerCode()
//		Get the trigger code.
//  UInt_t getNHitsRich()
//		Get the number of hits in RICH.
//  UInt_t getNHitsShower()
//		Get the number of hits in SHOWER.
//  UInt_t getNHitsTof()
//		Get the number of hits in TOF.
//  UInt_t getSecHitPatternRich()
//		Get the pattern of RICH sectors that had hits.
//  UInt_t getSecHitPatternShower()
//		Get the pattern of SHOWER sectors that had hits.
//  UInt_t getSecHitPatternTof()
//		Get the pattern of TOF sectors that had hits.
//  UInt_t getNLeptons()
//		Get the number of found leptons.
//  UInt_t getNDiLeptons()
//		get the number of found Dileptons.
//
// To set a variable:
//  void setMULength(Int_t mul)
//		Set the length of the MU subevent
//  void setTriggerCodeFirstFormat(Bool_t tcff)
//		Set the format of the MU subevent. FALSE means old format, TRUE means new format.
//  void setVersion(Int_t v)
//		Set the version.
//  void setTriggerCode(Int_t tc)
//		Set the trigger code.
//  void setNHitsRich(Int_t hr)
//		Set the number of hits found in RICH.
//  void setNHitsShower(Int_t hs)
//		Set the number of hits found in SHOWER.
//  void setNHitsTof(Int_t ht)
//		Set the number of hits found in TOF.
//  void setSecHitPatternRich(Int_t pr)
//		Set the bit pattern of RICH sectors that had hits.
//  void setSecHitPatternShower(Int_t pr)
//		Set the bit pattern of SHOWER sectors that had hits.
//  void setSecHitPatternTof(Int_t pr)
//		Set the bit pattern of TOF sectors that had hits.
//  void setNLeptons(Int_t nl)
//		Set the number of found leptons.
//  void setNDiLeptons(Int_t nl)
//		Set the number of found dileptons.
// If you use these functions, make sure your input parameter is in the rigth
// coordinate system and units! No checking or transformations are done.
//
///////////////////////////////////////////////////////////////////////////

#define VEC(x,shift)      ( x << shift )

HMUData::HMUData(void)
{
// Default constructor. Sets all variables to zero except fTriggerCodeFirstFormat,
// which is set to 1, to define new format of MU data.
//
// Input parameters:
//	none
//
// Output parameters:
//	none
//
// Return code:
//	none
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


void HMUData::clear(void)
{
// Clears dataobject, by setting all variables to zero, except fTriggerCodeFirstFormat,
// which is left untouched.
//
// Input parameters:
//	none
//
// Output parameters:
//	none
//
// return code:
// none
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



Int_t HMUData::decode (UInt_t *ptr, Int_t maxLength)
{
// Decodes dataword from MU subevent.
//
//Input parameters:
//	UInt_t* ptr
//		Pointer to MU data.
//
//	Int_t maxLength
//	Maximum number of datawords to be decoded.
//
// Output parameters:
//	none
//
// Return code:
//	Number of decoded datawords.
  Int_t count=0,r=0;
  UInt_t contents[12];

  UInt_t downscaling=0;
  UInt_t downscalingFlag=0;
  UInt_t triggerDecision=0;
  
  eventhead = gHades->getCurrentEvent()->getHeader();


  for(Int_t i=0;i<10;i++)
  {
    contents[i]=0;
  }

  count = 0;
  while( (count<maxLength) && (count<10) )
  {
      contents[count] = *ptr;
      count++;
      ptr++;
  }

  if (fTriggerCodeFirstFormat==1)
  {
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
  else
  { // old mode
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

		eventhead->setDownscaling(downscaling);
  }


 if ( count > maxLength)
  {
    Error("MU Data decode","size inconsistency");
    r=-1;
    clear();
    return r;
  }

//   printf("MU-Data trigger:%i version:%x downscaling:%i
//            nHitsRich:%i nHitsShower:%i nHitsTof:%i
//            secHitPatternRich:%i  \n
//           secHitPatternShower:%i  secHitPatternTof:%i
//           nLeptons:%i
//         \n",
//  	triggerCode,  version,  downscaling,
//         nHitsRich, nHitsShower,nHitsTof, secHitPatternRich,
//         secHitPatternShower, secHitPatternTof,
//         nLeptons
//          );

  r = count;
  return r;
}

void HMUData::Streamer(TBuffer &R__b)
{
   // Stream an object of class HMUData.
   // Individual streamer needed, because variable eventhead was excluded from streaming for
   // Classversions > 1.
   //
   // Input parameters:
   //	TBuffer& R__b
   //		Buffer object to read data from.
   //
   // Output parameters:
   //	none
   //
   // Return code:
   //	none

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





