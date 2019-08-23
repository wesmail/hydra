//*-- AUTHOR :  Pablo Cabanelas
//*-- Created : 10/10/2005

//_HADES_CLASS_DESCRIPTION 
/////////////////////////////////////////////////////////////
//
//  HRpcDetector
//
//  The Rpc Time-of-Flight detector class
//
/////////////////////////////////////////////////////////////

using namespace std;
#include "hrpcdetector.h"
#include "rpcdef.h"
#include "hcategory.h"
#include "hlinearcategory.h"
#include "hmatrixcategory.h"
//#include "hrpccalibrater.h"
//#include "hrpchitfinder.h"
#include "hades.h"
#include "hpario.h"
#include "hparrootfileio.h"
#include "hparasciifileio.h"
#include "hdetpario.h"
#include "hrpcparrootfileio.h"
#include "hrpcparasciifileio.h"
#include "TClass.h"
#include "TArrayI.h"
#include <iostream> 
#include <iomanip>

ClassImp(HRpcDetector) // Rpc Time-of-Flight detector class

HRpcDetector::HRpcDetector(void) : HDetector("Rpc","The Rpc detector") {
  // constructor
  maxSectors=1;
  maxModules=1;
  maxComponents=180;  // FIXME: Nov05 test
  modules=new TArrayI(1*1*180);
}

HRpcDetector::~HRpcDetector(void) {
}

HCategory* HRpcDetector::buildLinearCategory(Text_t *classname,Float_t fillRate) {
  // builds a linear category for the Rpc
  HLinearCategory* category=new HLinearCategory(classname,(Int_t)(maxComponents*fillRate));
  return category;
}

HCategory *HRpcDetector::buildMatrixCategory(Text_t *classname,Float_t fillRate) {
  // builds a matrix category for the Rpc
  Int_t sizes[1];
  sizes[0]=maxComponents;
  HMatrixCategory* category=new HMatrixCategory(classname,1,sizes,fillRate);
  return category;
}

HCategory *HRpcDetector::buildCategory(Cat_t cat) {
  // builds the categories for the Rpc
  switch (cat) {
  case catRpcRaw : return buildMatrixCategory("HRpcRaw",0.5);
    //  case catRpcCal : return buildLinearCategory("HRpcCal",0.5);
    //  case catRpcHit : return buildMatrixCategory("HRpcHit",0.5);
    //  case catRpcGeantRaw : return buildLinearCategory("HGeantRpc",0.5);
  default : return NULL;
  }
}

void HRpcDetector::activateParIo(HParIo* io) {
  // activates the input/output class for the parameters
  // needed by the Rpc detector
  if (strcmp(io->IsA()->GetName(),"HParOraIo")==0) {
    io->setDetParIo("HRpcParIo");
    return;
  }
  if (strcmp(io->IsA()->GetName(),"HParRootFileIo")==0) {
    HRpcParRootFileIo* p=new HRpcParRootFileIo(((HParRootFileIo*)io)->getParRootFile());
    io->setDetParIo(p);
  }
  if (strcmp(io->IsA()->GetName(),"HParAsciiFileIo")==0) {
    HRpcParAsciiFileIo* p=new HRpcParAsciiFileIo(((HParAsciiFileIo*)io)->getFile());
    io->setDetParIo(p);
  }
}

Bool_t HRpcDetector::write(HParIo* output) {
  // writes the Rpc detector setup to the output
  HDetParIo* out=output->getDetParIo("HRpcParIo");
  if (out) return out->write(this);
  return kFALSE;
}
