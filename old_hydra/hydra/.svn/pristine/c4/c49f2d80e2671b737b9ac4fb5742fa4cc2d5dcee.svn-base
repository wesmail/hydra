// File: hrichparset.cc
// ***************************************************************************

//*-- Author : Witold Przygoda (przygoda@psja1.if.uj.edu.pl)
//*-- Modified : 1999/12/04 by Witold Przygoda (przygoda@psja1.if.uj.edu.pl)
using namespace std;
#include <iostream> 
#include <iomanip>
#include "hrichparset.h"
#include "hdetector.h"
#include "hpario.h"
#include "hdetpario.h"

// ***************************************************************************
//_HADES_CLASS_DESCRIPTION 
//////////////////////////////////////////////////////////////////////////////
//
//  HRichParSet
//
//  Set of RICH containers.
//
//////////////////////////////////////////////////////////////////////////////
// ***************************************************************************


ClassImp(HRichParSet)

//----------------------------------------------------------------------------
HRichParSet::HRichParSet(const Char_t* name,const Char_t* title,
                         const Char_t* context)
            : HParSet(name,title,context) {
  strcpy(detName,"Rich");
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HRichParSet::init(HParIo* inp,Int_t* set) {
  // intitializes the container from an input
  HDetParIo* input=inp->getDetParIo("HRichParIo");
  if (input) return (input->init(this,set));
  return kFALSE;
}
//============================================================================

//----------------------------------------------------------------------------
Int_t HRichParSet::write(HParIo* output) {
  HDetParIo* out=output->getDetParIo("HRichParIo");
  if (out) return out->write(this);
  return kFALSE;
}
//============================================================================

/*
//______________________________________________________________________________
void HRichParSet::Streamer(TBuffer &R__b)
{
   // Stream an object of class HRichParSet.

   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(); if (R__v) { }
      HParSet::Streamer(R__b);
      m_pReadParam = NULL;
//      R__b >> m_pReadParam;
   } else {
      R__b.WriteVersion(HRichParSet::IsA());
      HParSet::Streamer(R__b);
//      R__b << m_pReadParam;
   }
}
//============================================================================
*/
