//*-- AUTHOR : Ilse Koenig
//*-- created : 02/03/2000
//*-- modified : 15/11/2000 by R. Holzmann
//*-- modified : 19/05/2000 by Leszek Kidon
 
//////////////////////////////////////////////////////////////////
//  HTofinoDetector
//
//  class for detector Tofino
//////////////////////////////////////////////////////////////////

#include <stdio.h>
#include "htofinodetector.h"
#include "tofinodef.h"
#include "hades.h"
#include "hruntimedb.h"
#include "hpario.h"
#include "hdetpario.h"
#include "hparasciifileio.h"
#include "hparrootfileio.h"
#include "htofinoparhadasciifileio.h"
#include "htofinoparrootfileio.h"
#include "htofinoparasciifileio.h"
#include "htofinogeompar.h"
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
  if (strcmp(io->IsA()->GetName(),"HParOraIo")==0) {
    io->setDetParIo("HTofinoParIo");
    return;
  }
  if (strcmp(io->IsA()->GetName(),"HParRootFileIo")==0) {
    HTofinoParRootFileIo* p=new HTofinoParRootFileIo(((HParRootFileIo*)io)
                                                          ->getParRootFile());
    io->setDetParIo(p);
  }
  if (strcmp(io->IsA()->GetName(),"HParHadAsciiFileIo")==0) {
    HTofinoParHadAsciiFileIo* p=
       new HTofinoParHadAsciiFileIo(((HParHadAsciiFileIo*)io)->getHadAscii());
    io->setDetParIo(p);
  }
  if (strcmp(io->IsA()->GetName(),"HParAsciiFileIo")==0) {
    HTofinoParAsciiFileIo* p=new HTofinoParAsciiFileIo(((HParAsciiFileIo*)io)
                                                          ->getFile());
    io->setDetParIo(p);
  }
}


Bool_t HTofinoDetector::write(HParIo* output) {
  // writes the Tofino setup to output
  HDetParIo* out=output->getDetParIo("HTofinoParIo");
  if (out) return out->write(this);
  return kFALSE;
}


HCategory* HTofinoDetector::buildLinearCategory(Text_t* className) {
  HLinearCategory* category = new HLinearCategory(className,24); // 6x1x4 cells
  return category;
}


HCategory* HTofinoDetector::buildMatrixCategory(Text_t* className, Float_t fill) {
  Int_t ind[3] = {6,1,4};
  HMatrixCategory* category = new HMatrixCategory(className,3,ind,fill);
  return category;
}


HCategory* HTofinoDetector::buildCategory(Cat_t cat) {
  // builds the default categories
  switch (cat) {
    case catTofinoRaw : return buildMatrixCategory("HTofinoRaw",1.0);
    case catTofinoCal : return buildMatrixCategory("HTofinoCal",1.0);
    default : return NULL;
  }
}
 
HDetGeomPar* HTofinoDetector::createDetGeomPar(HGeomShapes* shapes) {
  // creates the geometry container "TofinoGeomPar"
  HTofinoGeomPar* c=
      (HTofinoGeomPar*)gHades->getRuntimeDb()->getContainer("TofinoGeomPar");
  if (!c) {
    c=new HTofinoGeomPar(shapes);
    gHades->getRuntimeDb()->addContainer(c);
  }
  return c;
}
