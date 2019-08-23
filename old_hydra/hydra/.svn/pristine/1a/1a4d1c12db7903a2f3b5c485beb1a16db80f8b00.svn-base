//*-- AUTHOR : Ilse Koenig
//*-- Created : 09/05/2005
//*-- Changed : 19.7.2005 B. Spruck

/////////////////////////////////////////////////////////////
//
//  HHodoDetector
//
//  Detector class for the Pion Hodoscopes
//
/////////////////////////////////////////////////////////////

using namespace std;
#include "hhododetector.h"
#include "hododef.h"
#include "hcategory.h"
#include "hlinearcategory.h"
#include "hmatrixcategory.h"
//#include "hhodocalibrater.h"
#include "hades.h"
#include "hpario.h"
#include "hparrootfileio.h"
#include "hparasciifileio.h"
#include "hdetpario.h"
#include "hhodoparrootfileio.h"
#include "hhodoparasciifileio.h"
#include "TClass.h"
#include "TArrayI.h"
#include <iostream> 
#include <iomanip>

ClassImp(HHodoDetector)

HHodoDetector::HHodoDetector(void) : HDetector("Hodo","The Pion Hodoscopes") {
  // constructor
  // sets the name, the number of modules and the maximum number of fibers per module
  fName="Hodo";
  maxSectors=-1;
  maxModules=4;
  maxComponents=128;// 96;
  modules=new TArrayI(4);
}

HHodoDetector::~HHodoDetector(void) {
  // destructor
  delete modules;
  modules=0;
}

HCategory* HHodoDetector::buildLinearCategory(Text_t *classname,Float_t fillRate) {
  // builds a linear category for the Hodoscopes
  HLinearCategory* category=new HLinearCategory(classname,maxModules*maxComponents*fillRate);
  return category;
}

HCategory* HHodoDetector::buildMatrixCategory(Text_t *className,Float_t fillRate) {
  // builds a matrix category for the Hodoscopes (2 levels)
  Int_t maxMod=getMaxModInSetup();
  if (maxMod==0) return 0;
  Int_t* sizes=new Int_t[2];
  sizes[0]=maxMod;
  sizes[1]=maxComponents;
  HMatrixCategory* category = new HMatrixCategory(className,2,sizes,fillRate);
  delete [] sizes;
  return category;
}

HCategory* HHodoDetector::buildCategory(Cat_t cat) {
  // builds the categories for the Hodoscopes
  switch (cat) {
  case catHodoRaw : return buildMatrixCategory("HHodoRaw",0.5);
  default : return NULL;
  }
}

void HHodoDetector::activateParIo(HParIo* io) {
  // activates the input/output class for the parameters needed by the Hodoscopes
  if (strcmp(io->IsA()->GetName(),"HParOraIo")==0) {
    io->setDetParIo("HHodoParIo");
    return;
  }
  if (strcmp(io->IsA()->GetName(),"HParRootFileIo")==0) {
    HHodoParRootFileIo* p=new HHodoParRootFileIo(((HParRootFileIo*)io)->getParRootFile());
    io->setDetParIo(p);
  }
  if (strcmp(io->IsA()->GetName(),"HParAsciiFileIo")==0) {
    HHodoParAsciiFileIo* p=new HHodoParAsciiFileIo(((HParAsciiFileIo*)io)->getFile());
    io->setDetParIo(p);
  }
}

Bool_t HHodoDetector::write(HParIo* output) {
  // writes the setup of the Hodoscopes to the output
  HDetParIo* out=output->getDetParIo("HHodoParIo");
  if (out) return out->write(this);
  return kFALSE;
}

Int_t HHodoDetector::getMaxModInSetup(void) {
  Int_t maxMod=-1;
  for(Int_t i=0;i<maxModules;i++) {
    if (modules->At(i)) maxMod=(i>maxMod)?i:maxMod;
  }
  maxMod++;
  return maxMod;
}
