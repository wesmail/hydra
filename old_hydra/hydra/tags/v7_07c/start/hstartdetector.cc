//*-- AUTHOR : Rainer Schicker
//*-- modified : 20/07/2003 by T.Wojcik
//*-- modified : 18/12/2001 by I.Koenig
//*-- modified : 02/03/2000 by I.Koenig

//////////////////////////////////////////////////////////////////
//  HStartDetector
//
//  class for detector Start
//  stores the setup in a linear array of 2 modules;
//  first module is start detector, second module is veto detector 
//////////////////////////////////////////////////////////////////

#include <stdio.h>
#include "hstartdetector.h"
#include "hades.h"
#include "hpario.h"
#include "hdetpario.h"
#include "hparasciifileio.h"
#include "hparrootfileio.h"
#include "hstartparasciifileio.h"
#include "hstartparrootfileio.h"
#include "hcategory.h"
#include "hmatrixcategory.h"
#include "hlinearcategory.h"
#include "TClass.h"

#define  START_MAX_MODULES   6 
#define  START_MAX_COMPONENTS 32
#define  START_MAX_SECTORS    -1

ClassImp(HStartDetector)

HStartDetector::HStartDetector() {
  // constructor
  fName="Start";
  maxSectors = START_MAX_SECTORS;
  maxModules = START_MAX_MODULES;
  maxComponents= START_MAX_COMPONENTS;
  modules= new TArrayI(maxModules);
	for(Int_t i=0;i<maxModules;i++) {
 		modules->AddAt(0,i);
}

}


HStartDetector::~HStartDetector() {
  // destructor
  if (modules) delete modules;
  modules=0;
}


Bool_t HStartDetector::init(void) {
  // nothing to be done
  return kTRUE;
}


void HStartDetector::activateParIo(HParIo* io) {
  // activates the input/output class for the parameters
  // needed by the Start
  if (strcmp(io->IsA()->GetName(),"HParOraIo")==0) {
    io->setDetParIo("HStartParIo");
    return;
  }
  if (strcmp(io->IsA()->GetName(),"HParRootFileIo")==0) {
    HStartParRootFileIo* p=new HStartParRootFileIo(((HParRootFileIo*)io)->getParRootFile());
    io->setDetParIo(p);
  }
  if (strcmp(io->IsA()->GetName(),"HParAsciiFileIo")==0) {
    HStartParAsciiFileIo* p=new HStartParAsciiFileIo(((HParAsciiFileIo*)io)->getFile());
    io->setDetParIo(p);
  }
}


Bool_t HStartDetector::write(HParIo* output) {
  // writes the Start setup to output
  HDetParIo* out=output->getDetParIo("HStartParIo");
  if (out) return out->write(this);
  return kFALSE;
}


HCategory* HStartDetector::buildMatrixCategory(Text_t* className,Float_t fillRate) {
  Int_t maxMod=getMaxModInSetup();
  if (maxMod==0) return 0;
  Int_t* sizes=new Int_t[2]; // 2 levels 
  sizes[0]=maxMod;  
  sizes[1]=maxComponents;
  HMatrixCategory* category = new HMatrixCategory(className,2,sizes,fillRate);
  delete [] sizes;
  return category;
}


HCategory* HStartDetector::buildLinearCategory(Text_t* className) {
  Int_t size=getMaxModInSetup()*maxComponents;
  if (size) {
    HLinearCategory* category = new HLinearCategory(className,size);
    return category;
  }
  return 0;
}


HCategory* HStartDetector::buildCategory(Cat_t cat) {
  // builds the default categories (here LinearCategories)
  switch (cat) {
    case catStartRaw : return buildMatrixCategory("HStartRaw");
    case catStartCal : return buildMatrixCategory("HStartCal");
    default : return NULL;
  }
}


Int_t HStartDetector::getMaxModInSetup(void) {
  Int_t maxMod=-1; 
  for(Int_t i=0;i<maxModules;i++) {
    if (modules->At(i)) maxMod=(i>maxMod)?i:maxMod;
  }
  maxMod++;
  return maxMod;
}  





