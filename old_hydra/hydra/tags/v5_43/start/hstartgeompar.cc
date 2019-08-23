//*-- AUTHOR : Ilse Koenig
//*-- Createded : 02/10/2000

//////////////////////////////////////////////////////////////////////////////
// HStartGeomPar
//
// Container class for the basic START geometry parameters
//
//////////////////////////////////////////////////////////////////////////////

#include "hstartgeompar.h"
#include "hades.h"
#include "hruntimedb.h"
#include "hpario.h"
#include "hdetpario.h"
#include <iostream.h>

ClassImp(HStartGeomPar)

HStartGeomPar::HStartGeomPar(HGeomShapes* s) : HDetGeomPar("Start",s) {
  // constructor sets the name of the container
  fName="StartGeomPar";
  fTitle="Start geometry parameters";
}

Bool_t HStartGeomPar::init(HParIo* inp,Int_t* set) {
  // intitializes the container from an input
  HDetParIo* input=inp->getDetParIo("HStartParIo");
  if (input) return (input->init(this,set));
  return kFALSE;
}

Int_t HStartGeomPar::write(HParIo* output) {
  // writes the container to an output
  HDetParIo* out=output->getDetParIo("HStartParIo");
  if (out) return out->write(this);
  return -1;
}
