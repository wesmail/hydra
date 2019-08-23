//*-- AUTHOR : Ilse Koenig
//*-- Modified : 14/12/2001 by Ilse Koenig
//*-- Created  : 05/12/2000

//_HADES_CLASS_DESCRIPTION 
//////////////////////////////////////////////////////////////////////////////
// HTofinoGeomPar
//
// Container class for the basic TOFINO geometry parameters
//
//////////////////////////////////////////////////////////////////////////////
using namespace std;
#include "htofinogeompar.h"
#include "hades.h"
#include "hruntimedb.h"
#include "hpario.h"
#include "hdetpario.h"
#include <iostream> 
#include <iomanip>

ClassImp(HTofinoGeomPar)

HTofinoGeomPar::HTofinoGeomPar(const char* name,const char* title,
                               const char* context)
               : HDetGeomPar(name,title,context,"Tofino") {
  // constructor calling the base class constructor with the detector name
}

Bool_t HTofinoGeomPar::init(HParIo* inp,Int_t* set) {
  // intitializes the container from an input
  HDetParIo* input=inp->getDetParIo("HTofinoParIo");
  if (input) return (input->init(this,set));
  return kFALSE;
}

Int_t HTofinoGeomPar::write(HParIo* output) {
  // writes the container to an output
  HDetParIo* out=output->getDetParIo("HTofinoParIo");
  if (out) return out->write(this);
  return -1;
}
