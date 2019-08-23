/////////////////////////////////////////////////////////////
//HRichParRootFileIo
/////////////////////////////////////////////////////////////

#include "hrichparrootfileio.h"
#include "hparrootfileio.h"
#include "heventfile.h"
#include "hades.h"
#include "hspectrometer.h"
#include "hruntimedb.h"
#include "hrichgeometry.h"
#include "hrichdigidetpar.h"
#include "hrichhitfpar.h"
#include "hrichcalpar.h"

#include <iostream.h>

ClassImp(HRichParRootFileIo)

HRichParRootFileIo::HRichParRootFileIo(HParRootFile* f) : HDetParRootFileIo(f) {
  // constructor sets the name of the detector I/O "HRichParIo"
  fName="HRichParIo";
  initModules=new TArrayI(24);
}


HRichParRootFileIo::~HRichParRootFileIo() {
  // destructor
  if (modulesFound) {
    delete modulesFound;
    modulesFound=0;
  }
  if (initModules) {
    delete initModules;
    initModules=0;
  }
}


Bool_t HRichParRootFileIo::init(HParSet* pPar,Int_t* set) {
  // initializes a container called by name, but only the modules
  // defined in the array 'set'
  // calls the special read function for this container
  // If it is called the first time it reads the setup found in the file

  if (!isActiv) readModules("Rich");
  const Text_t* name = ((TNamed*)pPar)->GetName();
  printf("rich par file io init - %s\n", name);
  if (pFile) {
    if (strcmp(name,"RichGeometry")==0) return read((HRichGeometry*)pPar,set);
    if (strcmp(name,"RichDigiPar")==0) return read((HRichDigiDetPar*)pPar,set);
    if (strcmp(name,"RichHitFinderParams")==0) return read((HRichHitFPar*)pPar,set);
    if (strcmp(name,"RichCalPar")==0) return read((HRichCalPar*)pPar,set);
  }
  cerr<<"initialization of "<<name<<" not possible from ROOT file!"<<endl;
  return kFALSE;
}

Bool_t HRichParRootFileIo::readObject(HRichParSet *pPar) {
  const Text_t* name = ((TNamed*)pPar)->GetName();
  TKey *key = (TKey*)gDirectory->GetListOfKeys()->FindObject(name);
  if (key) pPar->Read(name);

  return (key != NULL);
}

Bool_t HRichParRootFileIo::read(HRichGeometry* pPar,Int_t* set) {
  readObject(pPar);
  
  return kTRUE;
}

Bool_t HRichParRootFileIo::read(HRichDigiDetPar* pPar,Int_t* set) {
  readObject(pPar);
  
  return kTRUE;
}

Bool_t HRichParRootFileIo::read(HRichHitFPar* pPar,Int_t* set) {
  if (!readObject(pPar))
     return kFALSE;

  return kTRUE;
}

Bool_t HRichParRootFileIo::read(HRichCalPar* pPar,Int_t* set) {
  if (!readObject(pPar))
     return kFALSE;

  printf("Rich CalPar initialized from ROOT\n"); 
  return kTRUE;
}


