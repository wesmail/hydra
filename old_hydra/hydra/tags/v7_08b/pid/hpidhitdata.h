// @(#)$Id: hpidhitdata.h,v 1.12 2005-08-29 15:39:29 spataro Exp $
//*-- Author : Marcin Jaskula 02/03/2003
//  Modified : Marcin Jaskula 14/03/2003
//              tof and mdc variables added
//  Modified : Marcin Jaskula 16/04/2003
//              flags added
//  Refactored : Tassilo Christ 16/10/2003
//           Compatibility with new tracking and hit matching scheme
//           requires redesign. Intended to hold all information from
//           hit level required to do PID. Should most of the users 
//           allow to throw away hit containers.
//  Modified : Stefano Spataro 26/08/2005
//              TOF rod/TOFINO cell included (new classdef)

#ifndef HPidHitData_h
#define HPidHitData_h

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// HPidHitData                                                                //
//                                                                            //
// Class for storing all interesting data from hit objects matched in         //
// HMetaMatch. Base class of HPidTrackCandidate.                              //
// This class should fulfill 80% of all analysis needs with the datamembers   //
// which are copied from the hit-container level. One can thus throw away     //
// the hit-containers and reduce the DST-size significantly.                  //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------

#include <TObject.h>
#include <TError.h>
#include "piddef.h"
// -----------------------------------------------------------------------------

class HMetaMatch;

// -----------------------------------------------------------------------------

class HPidHitData: public TObject
{
 public:
  
  HPidHitData(void)         { reset();  }

  virtual        ~HPidHitData(void)         { }

  void            reset(void);
  void            clearFlags(void)                { ResetBit(kALLDETS); }
  UInt_t          getFlags(void) { return ((TestBits(kALLDETS) & kALLDETS)); }
  
  
  virtual void    Clear(Option_t*)          { reset();  }
  
  void            fillFromMetaMatch(HMetaMatch *pMatch, Int_t iMatchIndex);
  void            fillFromHitData(HPidHitData *pSource);
  
  // -------------------------------------------------------------------------
  
  virtual void    Print(Option_t*) const    { print();  }
  void            print(void) const;
  
  // -------------------------------------------------------------------------

  //we use the bits 14 to 19 of the bit-field in TObject to store our detector information (cf. TObject documentation)
  enum
    {
      kRICH        = 0x01<<14,
      kINNERMDC    = 0x02<<14,
      kOUTERMDC    = 0x04<<14,
      kTOF         = 0x08<<14,
      kSHOWER      = 0x10<<14,
      kRICHIPU     = 0x20<<14,
      kALLDETS     = kRICH | kINNERMDC | kOUTERMDC | kTOF | kSHOWER | kRICHIPU
    };

  // -------------------------------------------------------------------------  

  
  void            setFlagRICH(Bool_t b = kTRUE)   { 
    if(b) SetBit(kRICH);  
    else ResetBit(kRICH);
  }
  Bool_t          getFlagRICH(void) { return TestBits(kRICH);     }
  

  
  void            setFlagInnerMDC(Bool_t b = kTRUE)    { 
    if(b) SetBit(kINNERMDC);  
    else ResetBit(kINNERMDC);
  }
  Bool_t          getFlagInnerMDC(void) { return TestBits(kINNERMDC);      }

  
  
  void            setFlagOuterMDC(Bool_t b = kTRUE)    { 
    if(b) SetBit(kOUTERMDC);  
    else ResetBit(kOUTERMDC);
  }
  Bool_t          getFlagOuterMDC(void) { return TestBits(kOUTERMDC);      }
  

  
  void            setFlagTOF(Bool_t b = kTRUE)    { 
    if(b) SetBit(kTOF);  
    else ResetBit(kTOF);
  }
  Bool_t          getFlagTOF(void) { return TestBits(kTOF);      }

  
  
  void            setFlagSHOWER(Bool_t b = kTRUE) { 
    if(b) SetBit(kSHOWER);  
    else ResetBit(kSHOWER);
  }
  Bool_t          getFlagSHOWER(void) { return TestBits(kSHOWER);   }

  
  
  void            setFlagRICHIPU(Bool_t b = kTRUE) { 
    if(b) SetBit(kRICHIPU);  
    else ResetBit(kRICHIPU);
  }
  Bool_t          getFlagRICHIPU(void) { return TestBits(kRICHIPU);   }
  
  // -------------------------------------------------------------------------

  Float_t getDeltaThetaMDCRICH();
  Float_t getDeltaPhiMDCRICH();
  // -------------------------------------------------------------------------
    void setSector(Int_t nSec){nSector=nSec;}
    Int_t getSector(){return nSector;}
    Float_t getTof(){return (iSystem==0)?fShowerTimeOfFlight:fTOFTimeOfFlight;}
  // -------------------------------------------------------------------------
  
  // all data members represent hit properties and are public at present

  Int_t nSector;
  
  // RICH stuff
  Int_t   nRingPadNr;
  Float_t fRingCentroid;
  Float_t fRichTheta;
  Float_t fRichPhi;
  Int_t   nRingPatMat;
  Int_t   nRingHouTra;
  Int_t   nRingAmplitude;
  Int_t   nRingLocalMax4;
  // -------------------------------------------------------------------------
  
  // MDC stuff inner segment
  Float_t fDistanceToVertex[nMomAlgs];
  Float_t fTrackR;
  Float_t fTrackZ;

  Int_t   iMdcClsSize0;
  Int_t   iMdcNWires0;
  Int_t   iMdcLevelCls0;
  Int_t   iMdcClsSize1;
  Int_t   iMdcNWires1;
  Int_t   iMdcLevelCls1;
  Float_t fInnerMdcChiSquare;
  Float_t fMdcRCoord; //Closest approach to beam axis
  Float_t fMdcZCoord; //z coordinate of this closest approach
  Float_t fMdcTheta;
  Float_t fMdcPhi;
  // -------------------------------------------------------------------------
  
  // MDC stuff outer segment
  Int_t   iMdcClsSize2;
  Int_t   iMdcNWires2;
  Int_t   iMdcLevelCls2;
  Int_t   iMdcClsSize3;
  Int_t   iMdcNWires3;
  Int_t   iMdcLevelCls3;
  Float_t fOuterMdcChiSquare;
  // -------------------------------------------------------------------------
  
  
  // RICH IPU ring observables
  Int_t iIPURingQuality;
  Int_t iIPUVetoQuality;
  // -------------------------------------------------------------------------
  
  //SHOWER/TOF observables
  Float_t fShowerSum[3];
  Int_t   nShowerClS[3];
  Float_t fShowerTimeOfFlight;
  Float_t fTOFTimeOfFlight;
  Int_t iTofinoMult;
  Int_t nTofClsSize;
  Int_t nMetaCell;
  // -------------------------------------------------------------------------
  
  
  //List of indices of hits in the detector - hit categories
  Short_t iIndRICH;
  Short_t iIndRICHIPU;
  Short_t iIndInnerSeg;
  Short_t iIndOuterSeg;
  Short_t iIndTOF;
  Short_t iIndShower;

  Short_t iSystem;

  //Index of Match-object in the MetaMatch-category
  Short_t iIndMatch;
  // -------------------------------------------------------------------------

  Bool_t  hasRingCorrelation[nMomAlgs]; //The answer might be different for each momentum method!
  
  ClassDef(HPidHitData, 3)  // Store data for HPidParticle from Hits
    };
    
#endif /* HPidHitData_h */
