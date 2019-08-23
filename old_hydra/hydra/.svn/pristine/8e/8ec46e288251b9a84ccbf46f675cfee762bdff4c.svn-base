// File: hmdcdataword.cc
//
// Author: Walter Karig <W.Karig@gsi.de>
// Modified: 27/11/02 by Peter Zumbruch <P.Zumbruch@gsi.de>

#include <stdio.h>
#include "hades.h"
#include "hcategory.h"
#include "hiterator.h"
#include "hldsubevt.h"
#include "hevent.h"
#include "hmdcraw.h"
#include "hmdcrawstruct.h"
#include "hmdcraweventheader.h"
#include "hspectrometer.h"
#include "hmdcdetector.h"
#include "hruntimedb.h"
#include "heventheader.h"
#include <iostream.h>
#include <fstream>
#include <TROOT.h>

#include "hmdcdataword.h"
#include "hmdcunpacker.h"

Bool_t HMdcDataWord::noComment=kFALSE;

ClassImp(HMdcDataWord)

HMdcDataWord::HMdcDataWord()
{
    clearAll();
    noComment=HMdcUnpacker::getQuietMode();
}

// --------------------------------------------------------------------------------
 
Bool_t HMdcDataWord::decode( const UInt_t data, UInt_t version, Bool_t consistencyCheck)
{
  switch(version)
    {
    case 0:
      //***************************************************** 
      // decoding of 32 bit data word from right->left 
      //
      //  bits        sz  mask           comments 
      // ----------------------------------------------------
      //  0 - 10      11  0x7ff          time value
      //      11       1  0x1            hit #0..1
      // 12 - 14       3  0x7            Channel of TDC Chip#0..7
      // 15 - 18       4  0xf            TDC Chip#1..12
      // 19 - 20       2  0x3            MBO#1..3 
      //      21       1  0x1            LVL1 Bus Pos (0,1) 
      // 22 - 24       3  0x7            RoC#1..7 
      //      25       1  0x1            SAM# Pos  
      // 26 - 30       5  0x1f           SAM#1..31
      //      31       1   -             not used
      //*******************************************************  
      
      sam   = ((data & m31) >> 26 & m5)-1; 
      samP  =  (data & m31) >> 25 & m1;
      roc   = ((data & m31) >> 22 & m3);  
      rocP  =  (data & m31) >> 21 & m1;
      mboNb = ((data & m31) >> 19 & m2); 
      tdcNb = ((data & m31) >> 15 & m4)-1;
      chan  =  (data & m31) >> 12 & m3;
      hit   =  (data & m31) >> 11 & m1; 
      time  =  (data & m31) & m11;   
      break;
    case 1:
      //***************************************************** 
      // decoding of 32 bit data word from right->left 
      //
      //  bits        sz  mask           comments 
      // ----------------------------------------------------
      //  0 - 10      11  0x7ff          time value
      //      11       1  0x1            hit #0..1
      // 12 - 14       3  0x7            Channel of TDC Chip#0..7
      // 15 - 18       4  0xf            TDC Chip#1..12
      // 19 - 20       2  0x3            MBO#1..3 
      //      21       1  0x1            LVL1 Bus Pos (0,1) 
      // 22 - 24       3  0x7            RoC#1..7 
      //      25       1  0x1            SAM# Pos  
      // 26 - 30       5  0x1f           SAM#0..31  << difference to version 0
      //      31       1   -             not used
      //*******************************************************  
      
      sam   = ((data & m31) >> 26 & m5); // difference to version 0
      samP  =  (data & m31) >> 25 & m1;
      roc   = ((data & m31) >> 22 & m3);  
      rocP  =  (data & m31) >> 21 & m1;
      mboNb = ((data & m31) >> 19 & m2); 
      tdcNb = ((data & m31) >> 15 & m4)-1;
      chan  =  (data & m31) >> 12 & m3;
      hit   =  (data & m31) >> 11 & m1; 
      time  =  (data & m31) & m11;   
      break;
    default:
      if(!noComment)Error("decode","decoding version %i not a valid number",version);
      return kFALSE;
    }
  
  if (consistencyCheck) return checkConsistency(version, data);
  return kTRUE;
}

// --------------------------------------------------------------------------------
 
void HMdcDataWord::dump()
{ 
  Info(" dump()","----------------------------------------");
  Info(" dump()","--- EvtSeqNr: %i\n", ((HEventHeader*)((HEvent*)(gHades->getCurrentEvent())->getHeader()))->getEventSeqNumber());
  print();
  Info(" dump()","----------------------------------------");
}

// --------------------------------------------------------------------------------
 
Bool_t HMdcDataWord::subHeader( const UInt_t* data, UInt_t version, Bool_t consistencyCheck)
{
  if (version==1) version--; // same decoding as version 0;
  switch(version)
    {
    case 0:
      //************************************************************
      // decoding of additionnal Mdc sub-header() 
      // always mask with m16!!!
      // 1): ev-nb  
      // 2): subevt size 
      // 3): flag-roc#-trig
      // the upper 16 bits of each of this 3  words contains additional
      // not yet documented/implemented debug informations
      //
      // bits       sz-mask             comments
      // ---------------------------------------       
      // 0 - 8       m4                trigger type               
      // 8 - 12      m4                Roc#    
      // 12          m4                flag size: 0:normal 1:abnormal 
      //                               bit 0: event truncated
      //                               bit 2: token not returned on port 0
      //                               bit 3: token not returned on port 1
      //************************************************************ 
      
      evNb       = *data            & m16;  
      subevtSize = *(data+1)        & m16;
      trigType   = *(data+2)        & m4;
      rocNb      = *(data+2) >> 8   & m4;
      errorFlag  = *(data+2) >> 12  & m4; 
      break;
    default:
      if(!noComment)Error("subHeader","decoding version %i not a valid number",version);
      return kFALSE;
    }

  if (consistencyCheck) return checkConsistencyRocHeader(version);
  return kTRUE;
}


