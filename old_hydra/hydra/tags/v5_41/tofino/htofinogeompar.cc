//*-- AUTHOR : Ilse Koenig
//*-- Created : 05/12/2000

//////////////////////////////////////////////////////////////////////////////
// HTofinoGeomPar
//
// Container class for the basic TOFINO geometry parameters
//
//////////////////////////////////////////////////////////////////////////////

#include "htofinogeompar.h"
#include "hades.h"
#include "hruntimedb.h"
#include "hpario.h"
#include "hdetpario.h"
#include <iostream.h>

ClassImp(HTofinoGeomPar)

HTofinoGeomPar::HTofinoGeomPar(HGeomShapes* s) : HDetGeomPar("Tofino",s) {
  // constructor sets the name of the container
  fName="TofinoGeomPar";
  fTitle="Tofino geometry parameters";
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
