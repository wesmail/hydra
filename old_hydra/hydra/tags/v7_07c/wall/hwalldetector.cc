//*-- AUTHOR : Ilse Koenig
//*-- Created : 17/01/2005

/////////////////////////////////////////////////////////////
//
//  HWallDetector
//
//  The Forward Wall detector class
//
/////////////////////////////////////////////////////////////

using namespace std;
#include "hwalldetector.h"
#include "walldef.h"
#include "hcategory.h"
#include "hlinearcategory.h"
#include "hmatrixcategory.h"
//#include "hwallcalibrater.h"
//#include "hwallhitfinder.h"
#include "hwallcalpar.h" //FK//???????
#include "hwallhitf.h"//FK//
#include "hades.h"
#include "hpario.h"
#include "hparrootfileio.h"
#include "hparasciifileio.h"
#include "hdetpario.h"
#include "hwallparrootfileio.h"
#include "hwallparasciifileio.h"
#include "TClass.h"
#include "TArrayI.h"
#include <iostream> 
#include <iomanip>

ClassImp(HWallDetector) // Forward Wall detector class

HWallDetector::HWallDetector(void) : HDetector("Wall","The Forward Wall detector") {
  // constructor
  maxSectors=-1;
  maxComponents=384;
  modules=new TArrayI(1);
}

HWallDetector::~HWallDetector(void) {
}

HCategory* HWallDetector::buildLinearCategory(Text_t *classname,Float_t fillRate) {
  // builds a linear category for the Forward Wall
  HLinearCategory* category=new HLinearCategory(classname,maxComponents*fillRate);
  return category;
}

HCategory* HWallDetector::buildMatrixCategory(Text_t *classname,Float_t fillRate) {
  // builds a matrix category for the Forward Wall
  Int_t sizes[1];
  sizes[0]=maxComponents;
  HMatrixCategory* category=new HMatrixCategory(classname,1,sizes,fillRate);
  return category;
}

HCategory* HWallDetector::buildCategory(Cat_t cat) {
  // builds the categories for the Forward Wall
  switch (cat) {
  case catWallRaw : return buildMatrixCategory("HWallRaw",0.5);
    //  case catWallCal : return buildLinearCategory("HWallCal",0.5);
  case catWallHit : return buildMatrixCategory("HWallHit",0.5);
    //  case catWallGeantRaw : return buildLinearCategory("HGeantWall",0.5);
  default : return NULL;
  }
}

void HWallDetector::activateParIo(HParIo* io) {
  // activates the input/output class for the parameters
  // needed by the Forward Wall
  if (strcmp(io->IsA()->GetName(),"HParOraIo")==0) {
    io->setDetParIo("HWallParIo");
    return;
  }
  if (strcmp(io->IsA()->GetName(),"HParRootFileIo")==0) {
    HWallParRootFileIo* p=new HWallParRootFileIo(((HParRootFileIo*)io)->getParRootFile());
    io->setDetParIo(p);
  }
  if (strcmp(io->IsA()->GetName(),"HParAsciiFileIo")==0) {
    HWallParAsciiFileIo* p=new HWallParAsciiFileIo(((HParAsciiFileIo*)io)->getFile());
    io->setDetParIo(p);
  }
}

Bool_t HWallDetector::write(HParIo* output) {
  // writes the Forward Wall setup to the output
  HDetParIo* out=output->getDetParIo("HWallParIo");
  if (out) return out->write(this);
  return kFALSE;
}
