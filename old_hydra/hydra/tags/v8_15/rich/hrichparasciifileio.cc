// File: hrichparasciifileio.cc
// ***************************************************************************

//*-- Author : Witold Przygoda (przygoda@psja1.if.uj.edu.pl)
//    last changed by Thomas Eberl Fri Aug 17 14:22:41 CEST 2001
//*-- Modified : 1999/12/04 by Witold Przygoda (przygoda@psja1.if.uj.edu.pl)

using namespace std;

#include "hades.h"
#include "hparset.h"
#include "hrichdetector.h"
#include "hrichparasciifileio.h"
#include "hspectrometer.h"

#include <iomanip>
#include <iostream> 


// ***************************************************************************
//_HADES_CLASS_DESCRIPTION 
//////////////////////////////////////////////////////////////////////////////
//
//  HRichParAsciiFileIo
//
//  For plain i/o ascii. Used only partially in RICH.
//
//////////////////////////////////////////////////////////////////////////////
// ***************************************************************************


ClassImp(HRichParAsciiFileIo)

//----------------------------------------------------------------------------
HRichParAsciiFileIo::HRichParAsciiFileIo(fstream* f) 
                             : HDetParAsciiFileIo(f) {
   fName = "HRichParIo";
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t
HRichParAsciiFileIo::init(HParSet* pPar, Int_t* set) {
   // calls the appropriate read function for the container

   const  Text_t* name = ((TNamed*)pPar)->GetName();

   if ( NULL != pFile ) 
   {
      if ( 0 == strncmp(name, "RichAnalysisParameters",
                        strlen("RichAnalysisParameters")) )
      {
         return HDetParAsciiFileIo::readCond((HParCond*)pPar);
      }
      if ( 0 == strncmp(name, "RichCorrelatorParameters",
                        strlen("RichCorrelatorParameters")) )
      {
         return HDetParAsciiFileIo::readCond((HParCond*)pPar);
      }
      if ( 0 == strncmp(name, "RichDigitisationParameters",
                        strlen("RichDigitisationParameters")) )
      {
         return HDetParAsciiFileIo::readCond((HParCond*)pPar);
      }
   }

   Error("init", "Initialization of %s not possible from ASCII file!", name);
   return kFALSE;
}
//============================================================================

//----------------------------------------------------------------------------
Int_t
HRichParAsciiFileIo::write(HParSet* pPar) {
   // calls the appropriate write function for the container

   const  Text_t* name = ((TNamed*)pPar)->GetName();
   if ( NULL != pFile )
   {
      if ( 0 == strncmp(name, "RichAnalysisParameters",
                        strlen("RichAnalysisParameter")) )
      {
         return HDetParAsciiFileIo::writeCond((HParCond*)pPar);
      }
      if ( 0 == strncmp(name, "RichCorrelatorParameters",
                        strlen("RichCorrelatorParameters")) )
      {
         return HDetParAsciiFileIo::writeCond((HParCond*)pPar);
      }
      if ( 0 == strncmp(name, "RichDigitisationParameters",
                        strlen("RichDigitisationParameters")) )
      {
         return HDetParAsciiFileIo::writeCond((HParCond*)pPar);
      }
   }

   Error("write", "%s could not be written to ASCII file", name);
   return kFALSE;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t
HRichParAsciiFileIo::write(HDetector* p) {

   return kTRUE;  // not implemented!

}
//============================================================================

