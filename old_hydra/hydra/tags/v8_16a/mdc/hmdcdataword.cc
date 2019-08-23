//_HADES_CLASS_DESCRIPTION 
//////////////////////////////////////////////////////////////////////////////////
//
// HMdcDataWord
//
// Raw Data object of MDC. Decoding of the data word is done here
// by masking out the single informations from the 32bit data word.
// This class is used by HMdcUnpacker to decode the streamed
// input data from hld to HMdcRaw object data level. A consistency check
// of the range of the single variables is performed here.
//////////////////////////////////////////////////////////////////////////////////
// File: $RCSfile: hmdcdataword.cc,v $
//
// Former Author: Walter Karig <W.Karig@gsi.de>
// Meanwhile Peter Zumbruch <P.Zumbruch@gsi.de>
//
// Version: $Revision: 1.22 $
// Modified by $Author: wuestenf $ on $Date: 2008-02-14 14:50:47 $

using namespace std;
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
#include <iostream> 
#include <iomanip>
#include <fstream>
#include "TROOT.h"

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
  // decoding of the 32 bit data word. version sets the
  // different decode version. A consistency check of the data
  // word is performed optionally (consistencyCheck=kTRUE).

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
      if(!noComment) Error("decode","decoding version %i not a valid number",version);
      return kFALSE;
    }
  
  if (consistencyCheck) return checkConsistency(version, data);
  return kTRUE;
}
// --------------------------------------------------------------------------------
 
UInt_t HMdcDataWord::getCodedDataWord( UInt_t version )
{
  // recoding of the 32 bit data word. version sets the
  // different decode version.

  UInt_t code = 0;

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
      
      code |= ((sam+1)   & m5 ) << 26;
      code |= ( samP     & m1 ) << 25;
      code |= ( roc      & m3 ) << 22;
      code |= ( rocP     & m1 ) << 21;
      code |= ( mboNb    & m2 ) << 19;
      code |= ((tdcNb+1) & m4 ) << 15;
      code |= ( chan     & m3 ) << 12;
      code |= ( hit      & m1 ) << 11;
      code |= ( time     & m11);

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
      
      code |= ( sam      & m5 ) << 26;  // difference to version 0
      code |= ( samP     & m1 ) << 25;
      code |= ( roc      & m3 ) << 22;
      code |= ( rocP     & m1 ) << 21;
      code |= ( mboNb    & m2 ) << 19;
      code |= ((tdcNb+1) & m4 ) << 15;
      code |= ( chan     & m3 ) << 12;
      code |= ( hit      & m1 ) << 11;
      code |= ( time     & m11);


      break;
    default:
      if(!noComment) Error("getCodedDataWord","decoding version %i not a valid number",version);
      return 0;
    }
  return code;
}

// --------------------------------------------------------------------------------
 
void HMdcDataWord::dump(UInt_t version)
{ 
    // dumps the event sequence number to the screen.

  Info(" dump()","----------------------------------------");
  if (gHades) Info(" dump()","--- EvtSeqNr: %i\n", ((HEventHeader*)((HEvent*)(gHades->getCurrentEvent())->getHeader()))->getEventSeqNumber());
  print(version);
  Info(" dump()","----------------------------------------");
}

// --------------------------------------------------------------------------------
 
Bool_t HMdcDataWord::subHeader( const UInt_t* data, UInt_t version, Bool_t consistencyCheck)
{
    // decodes the subheader information of the mdc data word
    //version sets the different decode version. A consistency check of the data
    // word is performed optionally (consistencyCheck=kTRUE).

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
      //                               bit 0: event truncated or wrong cal size
      //                               bit 1: fatal error: overwritten event
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

// --------------------------------------------------------------------------------
 
Bool_t HMdcDataWord::checkConsistency(UInt_t version, UInt_t data)
{
  // checks for the different decoding version whether the decoded value are consistent.
  // I.e. that
  //             sam in [ 0,   31]
  //    sam position in [ 0,    1]
  //             roc in [ 1,    5]
  //    roc position in [ 0,    1]
  //           mboNb in [ 1,    3]
  //           tdcNb in [ 0,   11]
  //        channels in [ 0,    7]
  // hits (module 2) in [ 0,    1]
  //            time in [ 0,0x7ff] (only here hex value!)
  //
  // NOTE: boundaries are the same for version 0 and 1, all other are not supported
  //
  // return values:
  //  kTRUE all checks passed
  //  kFALSE else

  if (version==1) version--; // checks for version 1 same as version 0
  // checks for the consistency of the data word
  switch(version)
    {
    case 0:
      {
	//             sam in [ 0,   31]
	//    sam position in [ 0,    1]
	//             roc in [ 1,    5]
	//    roc position in [ 0,    1]
	//           mboNb in [ 1,    3]
	//           tdcNb in [ 0,   11]
	//        channels in [ 0,    7]
	// hits (module 2) in [ 0,    1]
	//            time in [ 0,0x7ff]
	
	Int_t samMin   = 0;
	Int_t samMax   = 31;
	Int_t samPMin  = 0;
	Int_t samPMax  = 1;
	Int_t rocMin   = 1;
	Int_t rocMax   = 5;
	Int_t rocPMin  = 0;
	Int_t rocPMax  = 1;
	Int_t mboNbMin = 1;
	Int_t mboNbMax = 3;
	Int_t tdcNbMin = 1;
	Int_t tdcNbMax = 12;
	tdcNbMin --; // adaption to 
	tdcNbMax --; // some communication problems
	Int_t chanMin  = 0;
	Int_t chanMax  = 7;
	Int_t hitMin   = 0;
	Int_t hitMax   = 1;
	Int_t timeMin  = 0;
	Int_t timeMax  = 0x7ff;
	
	if (sam < samMin || sam > samMax ) 
	  {
	    if(!noComment)Error("checkConsistency()","%s = %i out of range [%i,%i] while decoding: 0x%08x", "SAM number", sam, samMin, samMax, data );
	    return kFALSE; 
	  }
	if (samP < samPMin || samP > samPMax ) 
	  {
	    if(!noComment)Error("checkConsistency()","%s = %i out of range [%i,%i] while decoding: 0x%08x", "SAM Position", samP, samPMin, samPMax, data  );
	    return kFALSE;
	  }
	if (roc < rocMin || roc > rocMax ) 
	  {
	    if(!noComment)Error("checkConsistency()","%s = %i out of range [%i,%i] while decoding: 0x%08x", "ROC Number", roc, rocMin, rocMax, data  );
	    return kFALSE; 
	  }
	if (rocP < rocPMin || rocP > rocPMax ) 
	  {
	    if(!noComment)Error("checkConsistency()","%s = %i out of range [%i,%i] while decoding: 0x%08x", "LVL1 Slot", rocP, rocPMin, rocPMax, data  );
	    return kFALSE; 
	  }
	if (mboNb < mboNbMin || mboNb > mboNbMax ) 
	  {
	    if(!noComment)Error("checkConsistency()","%s = %i out of range [%i,%i] while decoding: 0x%08x", "MBO Position", mboNb, mboNbMin, mboNbMax, data  );
	    return kFALSE; 
	  }
	if (tdcNb < tdcNbMin || tdcNb > tdcNbMax ) 
	  {
	    if(!noComment)Error("checkConsistency()","%s = %i out of range [%i,%i] while decoding: 0x%08x", "TDC Number", tdcNb, tdcNbMin, tdcNbMax, data  );
	    return kFALSE; 
	  }
	if (chan < chanMin || chan > chanMax ) 
	  {
	    if(!noComment)Error("checkConsistency()","%s = %i out of range [%i,%i] while decoding: 0x%08x", "Channel Number", chan, chanMin, chanMax, data  );
	    return kFALSE; 
	  }
	if (hit < hitMin || hit > hitMax ) 
	  {
	    if(!noComment)Error("checkConsistency()","%s = %i out of range [%i,%i] while decoding: 0x%08x", "Hit Number (modulo 2)", hit, hitMin, hitMax, data  );
	    return kFALSE; 
	  }
	if (time < timeMin || time > timeMax ) 
	  {
	    if(!noComment)Error("checkConsistency()","%s = %i out of range [%i,%x] while decoding: 0x%08x", "Time", time, timeMin, timeMax, data  );
	    return kFALSE; 
	  }
      }
      break;
    default:
      if(!noComment)Error("checkConsistency()","decoding version %i not valid",version);
      return kFALSE;
    }
  return kTRUE;
};

// --------------------------------------------------------------------------------
 
Bool_t HMdcDataWord::checkConsistencyRocHeader(UInt_t version)
{
  // boundary checks for the consistency of the roc header
  // I.e. that
  //  event number in [ 0x0,0xffff]
  // subevent size in [ 0x0,0xffff]
  //  trigger type in [ 0x1,0xf]
  //    roc number in [ 0x1,0x5]
  //     errorFlag in [ 0x0,0xf]
  //
  // NOTE: boundaries are the same for version 0 and 1, all other are not supported
  //
  // return values:
  //  kTRUE all checks passed
  //  kFALSE else

	if (version==1) version--; // checks for version 1 same as version 0
	// checks for the consistency of the header
  switch(version)
    {
    case 0:
      {
	//  event number in [ 0x0,0xffff]
	// subevent size in [ 0x0,0xffff]
	//  trigger type in [ 0x1,0xf]
	//    roc number in [ 0x1,0x5]
	//     errorFlag in [ 0x0,0xf]
      
	Int_t evNbMin       = 0;
	Int_t evNbMax       = 0xffff;
	Int_t subevtSizeMin = 0;
	Int_t subevtSizeMax = 0xffff;
	Int_t trigTypeMin   = 0;
	Int_t trigTypeMax   = 0xf;
	Int_t rocNbMin      = 1;
	Int_t rocNbMax      = 0x5;
	Int_t errorFlagMin  = 0;
	Int_t errorFlagMax  = 0xf;
      
	if (evNb < evNbMin || evNb > evNbMax ) 
	  {
	    if(!noComment)Error("checkConsistencyRocHeader()","%s = %i out of range [%i,%x]", "Event Number number", evNb, evNbMin, evNbMax );
	    return kFALSE; 
	  }
	if (subevtSize < subevtSizeMin || subevtSize > subevtSizeMax ) 
	  {
	    if(!noComment)Error("checkConsistencyRocHeader()","%s = %i out of range [%i,%x]", "Subevent size number", subevtSize, subevtSizeMin, subevtSizeMax );
	    return kFALSE; 
	  }
	if (trigType < trigTypeMin || trigType > trigTypeMax ) 
	  {
	    if(!noComment)Error("checkConsistencyRocHeader()","%s = %i out of range [%i,%x]", "Trigger Type", trigType, trigTypeMin, trigTypeMax );
	    return kFALSE; 
	  }
	if (rocNb < rocNbMin || rocNb > rocNbMax ) 
	  {
	    if(!noComment)Error("checkConsistencyRocHeader()","%s = %i out of range [%i,%i]", "ROC Number", rocNb, rocNbMin, rocNbMax );
	    return kFALSE; 
	  }
	if (errorFlag < errorFlagMin || errorFlag > errorFlagMax ) 
	  {
	    if(!noComment)Error("checkConsistencyRocHeader()","%s = %i out of range [%i,%x]", "Error Flag", errorFlag, errorFlagMin, errorFlagMax );
	    return kFALSE; 
	  }
      }
      break;
    default:
      if(!noComment)Error("checkConsistency()","decoding version %i not valid",version);
      return kFALSE;
    }
  return kTRUE;
};

// --------------------------------------------------------------------------------
 
void HMdcDataWord::print(UInt_t version)
{
  switch(version)
    {
    case 0:
      printf("\t%s: ",ClassName());
      printf("\n\t>>>>> DataWord   : ");    
      printf("%s: %2i | ","sam" ,sam);    
      printf("%s: %2i | ","samP",samP);   
      printf("%s: %2i | ","roc" ,roc);    
      printf("%s: %2i | ","rocP",rocP);   
      printf("%s: %2i | ","mboNb" ,mboNb);    
      printf("%s: %2i | ","tdcNb" ,tdcNb);    
      printf("%s: %2i | ","chan",chan);   
      printf("%s: %2i | ","hit" ,hit);    
      printf("%s: %4i   ","time",time);   
      printf("\n\t  >>> ROC-Header : ");    
      printf("%s: %4i | ","event number" ,evNb);      
      printf("%s: %2i | ","roc number"   ,rocNb);     
      printf("%s: %2i | ","trigger type" ,trigType);  
      printf("%s: %2i | ","error flag"   ,errorFlag); 
      printf("%s: %4i   ","subevent size",subevtSize);
      printf("\n\t  >>> RAW-Address: ");    
      printf("%s: %2i | ","sector" ,sector);
      printf("%s: %2i | ","module" ,module);   
      printf("%s: %2i | ","mbo"    ,mbo);
      printf("%s: %2i | ","tdc"    ,tdc);
      printf("%s: %2i   ","entry# ",entry);
      printf("\n");
      break;
    case 1:
      printf("\t%s: ",ClassName());
      printf("\n\t>>>>> DataWord   : ");    
      printf("%s: %2i | ","virtualSamNr" ,getVirtualSamNr());    
      printf("%s: %2i | ","sam" ,sam);    
      printf("%s: %2i | ","samP",samP);   
      printf("%s: %2i | ","roc" ,roc);    
      printf("%s: %2i | ","rocP",rocP);   
      printf("%s: %2i | ","mboNb" ,mboNb);    
      printf("%s: %2i | ","tdcNb" ,tdcNb);    
      printf("%s: %2i | ","chan",chan);   
      printf("%s: %2i | ","hit" ,hit);    
      printf("%s: %4i   ","time",time);   
      printf("\n\t  >>> ROC-Header : ");    
      printf("%s: %4i | ","event number" ,evNb);      
      printf("%s: %2i | ","roc number"   ,rocNb);     
      printf("%s: %2i | ","trigger type" ,trigType);  
      printf("%s: %2i | ","error flag"   ,errorFlag); 
      printf("%s: %4i   ","subevent size",subevtSize);
      printf("\n\t  >>> RAW-Address: ");    
      printf("%s: %2i | ","sector" ,sector);
      printf("%s: %2i | ","module" ,module);   
      printf("%s: %2i | ","mbo"    ,mbo);
      printf("%s: %2i | ","tdc"    ,tdc);
      printf("%s: %2i   ","entry# ",entry);
      printf("\n");
      break;
    default:
      Error("print()","decoding version %i not valid",version);
    }

}

