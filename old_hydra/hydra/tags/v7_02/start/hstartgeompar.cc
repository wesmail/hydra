//*-- AUTHOR : Ilse Koenig
//*-- modified: 18/12/2001 by Ilse Koenig
//*-- Created : 02/10/2000

//////////////////////////////////////////////////////////////////////////////
// HStartGeomPar
//
// Container class for the basic START geometry parameters
//
//////////////////////////////////////////////////////////////////////////////
using namespace std;
#include "hstartgeompar.h"
#include "hpario.h"
#include "hdetpario.h"
#include <iostream> 
#include <iomanip>

ClassImp(HStartGeomPar)

HStartGeomPar::HStartGeomPar(const char* name,const char* title,
                               const char* context)
              : HDetGeomPar(name,title,context,"Start") {
  // constructor calling the base class constructor with the detector name
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
