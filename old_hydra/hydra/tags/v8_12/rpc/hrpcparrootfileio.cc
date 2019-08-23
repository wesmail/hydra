//*-- AUTHOR :   Pablo Cabanelas 
//*-- Created :  17/10/2005

/////////////////////////////////////////////////////////////
//  HRpcParRootFileIo
//
//  Class for RPC parameter input/output from/into ROOT file
//
//  It is derived from the base class HDetParRootFileIo and
//  inherits from it basic functions e.g. write(...)
//
/////////////////////////////////////////////////////////////
using namespace std;
#include "hrpcparrootfileio.h"
#include "hparrootfileio.h"
#include "hades.h"
#include "hspectrometer.h"
#include "hruntimedb.h"
#include "hrpcdetector.h"
#include "hrpclookup.h"
#include <iostream> 
#include <iomanip>

ClassImp(HRpcParRootFileIo)

HRpcParRootFileIo::HRpcParRootFileIo(HParRootFile* f) : HDetParRootFileIo(f) {
  // constructor sets the name of the detector I/O "HRpcParIo"
  fName="HRpcParIo";
  //HDetector* det=gHades->getSetup()->getDetector("Rpc");
  //Int_t n=(det->getMaxSectors())*(det->getMaxModules());
  //initModules=new TArrayI(n);
}

HRpcParRootFileIo::~HRpcParRootFileIo(void) {
  // destructor
}

Bool_t HRpcParRootFileIo::init(HParSet* pPar,Int_t* set) {
  // initializes a container called by name, but only the modules
  // defined in the array 'set'
  // calls the special read function for this container
  // If it is called the first time it reads the setup found in the file
  if (!isActiv) readModules("Rpc");
  const Text_t* name=pPar->GetName();
  if (pFile) {
    if (strcmp(name,"RpcLookup")==0) return read((HRpcLookup*)pPar,set);
  }
  cerr<<"initialization of "<<name<<" not possible from ROOT file!"<<endl;
  return kFALSE;
}

Bool_t HRpcParRootFileIo::read(HRpcLookup* pPar,Int_t* set) {
  // reads and fills the container "RpcLookup" needed by the unpacker.
  // Returns kFalse, when not initialized completely from this input
  Text_t* name=(char*)pPar->GetName();
  Int_t version=findInputVersion(name);
  if (version<=0) {
    pPar->setInputVersion(-1,inputNumber);
    return kFALSE;    // not in ROOT file
  }
  if (pPar->getInputVersion(inputNumber)==version
        && pPar->getInputVersion(inputNumber)!=-1) return kTRUE;
  // needs reinitialization
  HRpcLookup* rRpc=(HRpcLookup*)findContainer(name,version);
  //initModules->Reset();
  pPar->clear();   // no initialization from 2 inputs possible!
  Int_t cell;  
  Char_t side;   
  Bool_t allFound=kFALSE;
  for(Int_t c=0;c<rRpc->getSize();c++) {
      HRpcLookupChan& chan=(*rRpc)[c];
        chan.getAddress(cell,side); 
         if (cell>0) {
           pPar->fill(c,cell,side);
           allFound=kTRUE;  
         }
      }
  if (allFound) {
    pPar->setInputVersion(version,inputNumber);
    pPar->setChanged();
    cout << "RpcLookup: module(s) initialized from Root file \n";
  }
  return allFound;
}


