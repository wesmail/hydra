
//HShowerParSet
//
//  Base class for shower parameter containers
//
/////////////////////////////////////////////////////////////

#include "hshowerparset.h"
#include "hdetector.h"
#include "hpario.h"
#include "hdetpario.h"
#include <iostream.h>

ClassImp(HShowerParSet)

HShowerParSet::HShowerParSet() {
  strcpy(detName,"Shower");
}

Bool_t HShowerParSet::init(HParIo* inp,Int_t* set) {
  printf("xxxxxxxxxxxxx\n");
  // intitializes the container from an input
  HDetParIo* input=inp->getDetParIo("HShowerParIo");
  if (input) return (input->init(this,set));
  return kFALSE;
}

Int_t HShowerParSet::write(HParIo* output) {
  printf("-----------");
  HDetParIo* out=output->getDetParIo("HShowerParIo");
  if (out) return out->write(this);
  return kFALSE;
}

//______________________________________________________________________________
void HShowerParSet::Streamer(TBuffer &R__b)
{
   // Stream an object of class HShowerParSet.

   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(); if (R__v) { }
      HParSet::Streamer(R__b);
      m_pReadParam = NULL;
//      R__b >> m_pReadParam;
   } else {
      R__b.WriteVersion(HShowerParSet::IsA());
      HParSet::Streamer(R__b);
//      R__b << m_pReadParam;
   }
}

