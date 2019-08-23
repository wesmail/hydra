//*-- AUTHOR : Ilse Koenig
//*-- created : 02/03/2000

//////////////////////////////////////////////////////////////////
//  HTofinoDetector
//
//  class for detector Tofino
//////////////////////////////////////////////////////////////////

#include <stdio.h>
#include "htofinodetector.h"
#include "hades.h"
#include "hruntimedb.h"
#include "hpario.h"
#include "hdetpario.h"
#include "hparasciifileio.h"
#include "hparrootfileio.h"
//#include "htofinoparasciifileio.h"
//#include "htofinoparrootfileio.h"
#include "hcategory.h"
#include "hmatrixcategory.h"
#include "hlinearcategory.h"
#include "TClass.h"
#include "TArrayI.h"

ClassImp(HTofinoDetector)

HTofinoDetector::HTofinoDetector() {
  // constructor
  fName="Tofino";
  maxSectors=6;
  maxModules=1;
  maxComponents=4;
  modules= new TArrayI(maxModules*maxSectors);
}


HTofinoDetector::~HTofinoDetector() {
  // destructor
  if (modules) delete modules;
  modules=0;
}


Bool_t HTofinoDetector::init(void) {
  // nothing to be done
  return kTRUE;
}


void HTofinoDetector::activateParIo(HParIo* io) {
  // activates the input/output class for the parameters
  // needed by the Tofino
/*  not yet implemented
  if (strcmp(io->IsA()->GetName(),"HParOraIo")==0) {
    io->setDetParIo("HTofinoParIo");
    return;
  }
  if (strcmp(io->IsA()->GetName(),"HParRootFileIo")==0) {
    HTofinoParRootFileIo* p=new HTofinoParRootFileIo(((HParRootFileIo*)io)
                                                          ->getParRootFile());
    io->setDetParIo(p);
  }
  if (strcmp(io->IsA()->GetName(),"HParAsciiFileIo")==0) {
    HTofinoParAsciiFileIo* p=new HTofinoParAsciiFileIo(((HParAsciiFileIo*)io)
                                                          ->getFile());
    io->setDetParIo(p);
  }
*/
}


Bool_t HTofinoDetector::write(HParIo* output) {
  // writes the Tofino setup to output
  HDetParIo* out=output->getDetParIo("HTofinoParIo");
  if (out) return out->write(this);
  return kFALSE;
}


HCategory* HTofinoDetector::buildLinearCategory(Text_t* className) {
  HLinearCategory* category = new HLinearCategory(className,1);
  return category;
}


HCategory* HTofinoDetector::buildMatrixCategory(Text_t* className) {
  Int_t ind[3] = {6,1,4};
  HMatrixCategory* category = new HMatrixCategory(className,3,ind,1.0);
  return category;
}


HCategory* HTofinoDetector::buildCategory(Cat_t cat) {
  // builds the default categories (here HLinearCategories)
  switch (cat) {
    case catTofinoRaw : return buildLinearCategory("HTofinoRaw");
//    case catTofinoRaw : return buildMatrixCategory("HTofinoRaw");
    default : return NULL;
  }
}
 
