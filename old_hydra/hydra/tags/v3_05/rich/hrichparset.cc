
//HRichParSet
//
//  Base class for rich parameter containers
//
/////////////////////////////////////////////////////////////

#include "hrichparset.h"
#include "hdetector.h"
#include "hpario.h"
#include "hdetpario.h"
#include <iostream.h>

ClassImp(HRichParSet)

HRichParSet::HRichParSet() {
  strcpy(detName,"Rich");
}

Bool_t HRichParSet::init(HParIo* inp,Int_t* set) {
  printf("init of HRichParSet\n");
  // intitializes the container from an input
  HDetParIo* input=inp->getDetParIo("HRichParIo");
  if (input) return (input->init(this,set));
  return kFALSE;
}

Int_t HRichParSet::write(HParIo* output) {
  printf("write of HRichParSet\n");
  HDetParIo* out=output->getDetParIo("HRichParIo");
  if (out) return out->write(this);
  return kFALSE;
}

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

