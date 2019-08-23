//*-- AUTHOR : Ilse Koenig
//*-- Modified : 17/01/2002 by I. Koenig

//////////////////////////////////////////////////////////////////////////////
// HMdcGeomPar
//
// Container class for the basic MDC geometry parameters
//
//////////////////////////////////////////////////////////////////////////////

#include "hmdcgeompar.h"
#include "hades.h"
#include "hruntimedb.h"
#include "hpario.h"
#include "hdetpario.h"
#include <iostream.h>

ClassImp(HMdcGeomPar)

HMdcGeomPar::HMdcGeomPar(const char* name,const char* title,
                               const char* context)
            : HDetGeomPar(name,title,context,"Mdc") {
  // constructor calling the base class constructor with the detector name
}

Bool_t HMdcGeomPar::init(HParIo* inp,Int_t* set) {
  // intitializes the container from an input
  HDetParIo* input=inp->getDetParIo("HMdcParIo");
  if (input) return (input->init(this,set));
  return kFALSE;
}

Int_t HMdcGeomPar::write(HParIo* output) {
  // writes the container to an output
  HDetParIo* out=output->getDetParIo("HMdcParIo");
  if (out) return out->write(this);
  return -1;
}
