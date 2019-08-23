using namespace std;
#include "htofinoparset.h"
#include "hdetector.h"
#include "hpario.h"
#include "hdetpario.h"
#include <iostream> 
#include <iomanip>

ClassImp(HTofinoParSet)
/////////////////////////////////////////////////////////////
//HTofinoParSet
//
//  Base class for all tofino parameter containers
//
/////////////////////////////////////////////////////////////


HTofinoParSet::HTofinoParSet(const char* name,const char* title,
                             const char* context)
              : HParSet(name,title,context) {
  //initialize HParSet for tofino detector
  strcpy(detName,"Tofino");
}

Bool_t HTofinoParSet::init(HParIo* inp,Int_t* set) {
  // intitializes the container from an input
  // set additional information for tofino detector

  //printf("xxxxxxxxxxxxx\n");
  HDetParIo* input=inp->getDetParIo("HTofinoParIo");
  if (input) return (input->init(this,set));
  return kFALSE;
}

Int_t HTofinoParSet::write(HParIo* output) {
  // writes the tofino container to the output defined in the runtime
  // database

  //printf("-----------");
  HDetParIo* out=output->getDetParIo("HTofinoParIo");
  if (out) return out->write(this);
  return -1;
}
