//*-- AUTHOR :  Pablo Cabanelas
//*-- Created : 10/10/2005
//*-- Modified: 11/09/2006

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
#include "hrpccal.h"
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
  maxSectors=6;
  maxModules=6;
  maxComponents=40;  
  modules=new TArrayI(maxSectors*maxModules*maxComponents);
}

HRpcDetector::~HRpcDetector(void) {
}

HCategory* HRpcDetector::buildLinearCategory(Text_t *classname,Float_t fillRate) {
  // builds a linear category for the Rpc
  HLinearCategory* category=new HLinearCategory(classname,(Int_t)(maxSectors*maxModules*maxComponents*fillRate));
  return category;
}

HCategory *HRpcDetector::buildMatrixCategory(Text_t *classname,Float_t fillRate) {
  // builds a matrix category for the Rpc (3 levels)
  Int_t* sizes=new Int_t[3];
  sizes[0]=maxSectors;
  sizes[1]=maxModules;
  sizes[2]=maxComponents;  
  HMatrixCategory* category=new HMatrixCategory(classname,3,sizes,fillRate);   
  delete [] sizes;
  return category;
}

HCategory *HRpcDetector::buildCategory(Cat_t cat) {
  // builds the categories for the Rpc
  switch (cat) {
  case catRpcRaw : return buildMatrixCategory("HRpcRaw",0.5);
  case catRpcCal : return buildMatrixCategory("HRpcCal",0.5);
    //  case catRpcHit : return buildMatrixCategory("HRpcHit",0.5);
  case catRpcGeantRaw : return buildMatrixCategory("HGeantRpc",0.5);
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
