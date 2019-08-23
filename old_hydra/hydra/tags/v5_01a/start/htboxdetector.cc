//*-- AUTHOR : Rainer Schicker
//////////////////////////////////////////////////////////////////
//  HTBoxDetector
//
//  class for detector TBox
//  stores the setup in an array of 1 module;
//////////////////////////////////////////////////////////////////

#include <stdio.h>
#include "htboxdetector.h"
#include "hades.h"
#include "hruntimedb.h"
#include "hpario.h"
#include "hdetpario.h"
#include "hparasciifileio.h"
#include "hparrootfileio.h"
//#include "htboxparasciifileio.h"
//#include "htboxparrootfileio.h"
#include "hcategory.h"
#include "hmatrixcategory.h"
#include "hlinearcategory.h"
#include "TClass.h"

ClassImp(HTBoxDetector)

HTBoxDetector::HTBoxDetector() {
  // constructor
  fName="TBox";
  maxSectors=-1;
  maxModules=1;
  maxComponents=8;
  modules = new TArrayI(maxModules);
}


HTBoxDetector::HTBoxDetector(Int_t mods, Int_t chan) {
  // constructor
  fName="TBox";
  maxSectors=-1;
  maxModules=mods;
  maxComponents=chan;
  modules = new TArrayI(maxModules);
}


HTBoxDetector::~HTBoxDetector() {
  // destructor
  if (modules) delete modules;
  modules=0;
}


Bool_t HTBoxDetector::init(void) {
  // nothing to be done
  return kTRUE;
}

/*
void HTBoxDetector::activateParIo(HParIo* io) {
  // activates the input/output class for the parameters
  // needed by the TBox
  if (strcmp(io->IsA()->GetName(),"HParOraIo")==0) {
    io->setDetParIo("HTBoxParIo");
    return;
  }
  if (strcmp(io->IsA()->GetName(),"HParRootFileIo")==0) {
    HTBoxParRootFileIo* p=new HTBoxParRootFileIo(((HParRootFileIo*)io)
                                                         ->getParRootFile());
    io->setDetParIo(p);
  }
  if (strcmp(io->IsA()->GetName(),"HParAsciiFileIo")==0) {
    HTBoxParAsciiFileIo* p=new HTBoxParAsciiFileIo(((HParAsciiFileIo*)io)
                                                         ->getFile());
    io->setDetParIo(p);
  }
}


Bool_t HTBoxDetector::write(HParIo* output) {
  // writes the TBox setup to output
  HDetParIo* out=output->getDetParIo("HTBoxParIo");
  if (out) return out->write(this);
  return kFALSE;
}
*/

HCategory* HTBoxDetector::buildMatrixCategory(Text_t* className) {
  Int_t maxMod=getMaxModInSetup();
  if (maxMod==0) return 0;
  Int_t* sizes=new Int_t[2]; // 2 levels 
  sizes[0]=maxMod;  
  sizes[1]=maxComponents;
  HMatrixCategory* category = new HMatrixCategory(className,2,sizes,1.0);
  delete [] sizes;
  return category;
}


HCategory* HTBoxDetector::buildLinearCategory(Text_t* className) {
  Int_t size=getMaxModInSetup()*maxComponents;
  if (size) {
    HLinearCategory* category = new HLinearCategory(className,size);
    return category;
  }
  printf("TBox linear category could not be built\n");
  return NULL;
}


HCategory* HTBoxDetector::buildCategory(Cat_t cat) {
  // builds the default categories (here MatrixCategories)
  switch (cat) {
    case catTBoxChan : return buildLinearCategory("HTBoxChan");
//  case catTBoxChan : return buildMatrixCategory("HTBoxChan");
    default : return NULL;
  }
}

Int_t HTBoxDetector::getMaxModInSetup(void) {
  Int_t maxMod=-1; 
  for(Int_t i=0;i<maxModules;i++) {
    if (modules->At(i)) maxMod=(i>maxMod)?i:maxMod;
  }
  maxMod++;
  return maxMod;
}  









