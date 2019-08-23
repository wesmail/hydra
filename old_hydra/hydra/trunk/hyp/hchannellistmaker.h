// Author: Alexander Belyaev
// Dubna, JINR, Laboratory of High Energy
// 25 February 2007

#ifndef HCHANNELLISTMAKER_H
#define HCHANNELLISTMAKER_H

#include "TObject.h"
#include "TH1.h"
#include "hhypbljvpidlist.h" // -> hBPidPhysicsConstants
//#include "hmdcbljvlib.h"

class HChannelListMaker : public TObject 
{
 enum {codeMul = 50};
 enum {limProng    =    10, 
       chNmbMaxTmp = 15000  // temporary of internal using channels 
                            // 12358 for n p -> with Ek (n) = 9 GeV
      };
 enum {decayNmbMax = 10, maxDecayedParticles = 30};
 enum tChannelType {charged,neutral};

 protected:
//  HMdcBljvLib bLib;
  HHypBljvPidList bLib;

 public:
            HChannelListMaker ();
   virtual ~HChannelListMaker (void);

  Bool_t    makeChannelList (Int_t maxProng, Int_t beamN, Float_t Ek, Int_t targN,
                             Float_t regDist, Bool_t allChargedParticlesAreRegistered,
                             Int_t maxChNmb, Int_t* numberOfChannels, Double_t* chCodeList, TString* chLineList,
                             Int_t* pNmb, Int_t* posPID, Int_t* nNmb, Int_t* negPID, Int_t* tNmb, Int_t* totPID);
  Double_t  getChannelCode  (Int_t prong, Int_t* digit, Int_t missaPid);
  Int_t     getMissaPid (Double_t chCode);
//   void      printInUseParticles (FILE* extOut);
//   void      typeInUseParticles ();
//-------------------------------    
 private:

  tPhysProp Phys;
 
  struct tChannel
   {tChannelType channelType;
    Int_t     prong;
    Double_t  code;
    Int_t     pID[limProng+2];
   };
  struct tChList
   {Int_t    chNmb;
    tChannel ch[chNmbMaxTmp+1];
   };
  Int_t   idx,idxNew;
  tChList chList[2]; 

  Int_t       prong,nLngth;
  Int_t       num,neutralIndex;
  Int_t       ndex[limProng+1];

  Double_t    newCodeOut,newCode;
 
  struct tDecay
   {Int_t prong;
    Int_t pID[limProng+1];
   };
  struct tDecPart
   {Int_t   pID;
    Float_t cTau;
    Int_t   decaysNmb;
    tDecay  decay[decayNmbMax+1];
   };
   
  Int_t    decPartsNmb;
  tDecPart decPart[maxDecayedParticles+1];

  Int_t    getPid (const Char_t* nomen);
  Int_t    getNextCharged (Int_t pid);
  void     addPart (Int_t partIndex, Int_t* dim, Int_t* pidList);
  Double_t sqr (Double_t value);
  void     getDecayList (); //(FILE* otp);
  Bool_t   newChannelFound (Double_t newCodeOut);
  void     sortDigit (Int_t n, Int_t* A);
  void     addNewChannel (Int_t prong, Int_t* digit, Double_t newCodeOut, 
                     tChannelType channelType, Float_t massSum, FILE* otp);
  void     copyChString (Int_t idxFrom, Int_t chNFrom, Int_t idxTo, Int_t chNTo);
  void     printChString (const Char_t* marker, Int_t idx, Int_t chN, FILE* otp);
  void     typeChString  (const Char_t* marker, Int_t idx, Int_t chN);
  void     shiftParticleToEnde (Int_t idxNew, Int_t newChN, Int_t pos);
  void     printChannelCodes (Int_t idx, Int_t chN, FILE* otp);
  void     printChannelList (Int_t idx, FILE* otp);
  void     typeChannelCodes (Int_t idx, Int_t chN);
  void     typeChannelList (Int_t idx);
  void     copyChannel (tChannel* chFrom, tChannel* chTo);
  Bool_t   validParticle (Int_t pid);

 //-------------------------------------------------------------------------------------------
   ClassDef(HChannelListMaker,0) //Digitizes MDC data.
 };

#endif
