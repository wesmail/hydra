using namespace std;
#include "hshowerparset.h"
#include "hdetector.h"
#include "hpario.h"
#include "hdetpario.h"
#include <iostream> 
#include <iomanip>

ClassImp(HShowerParSet)
//_HADES_CLASS_DESCRIPTION 
/////////////////////////////////////////////////////////////
//HShowerParSet
//
//  Base class for all shower parameter containers
//
/////////////////////////////////////////////////////////////


HShowerParSet::HShowerParSet(const char* name,const char* title,
                             const char* context)
        : HParSet(name,title,context) {
//initializate HParSet for shower detector
  strcpy(detName,"Shower");
}

Bool_t HShowerParSet::init(HParIo* inp,Int_t* set) {
  // intitializes the container from an input
  // set additional information for shower detector

  //printf("xxxxxxxxxxxxx\n");
  HDetParIo* input=inp->getDetParIo("HShowerParIo");
  if (input) return (input->init(this,set));
  return kFALSE;
}

Int_t HShowerParSet::write(HParIo* output) {
  // writes the shower container to the output defined in the runtime
  // database

  //printf("-----------");
  HDetParIo* out=output->getDetParIo("HShowerParIo");
  if (out) return out->write(this);
  return kFALSE;
}

//______________________________________________________________________________
/*
void HShowerParSet::Streamer(TBuffer &R__b)
{
   // Stream an object of class HShowerParSet.

   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(); if (R__v) { }
      HParSet::Streamer(R__b);
   } else {
      R__b.WriteVersion(HShowerParSet::IsA());
      HParSet::Streamer(R__b);
   }
}
*/
