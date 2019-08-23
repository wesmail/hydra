// File: hrichparrootfileio.cc
// ***************************************************************************

//*-- Author : Witold Przygoda (przygoda@psja1.if.uj.edu.pl)
//*-- Modified : 1999/12/04 by Witold Przygoda (przygoda@psja1.if.uj.edu.pl)


#include <iostream.h>
#include "hrichparrootfileio.h"
#include "hparrootfileio.h"
#include "heventfile.h"
#include "hades.h"
#include "hspectrometer.h"
#include "hruntimedb.h"
#include "hrichgeometrypar.h"
#include "hrichdigitisationpar.h"
#include "hrichanalysispar.h"
#include "hrichcalpar.h"

// ***************************************************************************
//////////////////////////////////////////////////////////////////////////////
//
//  HRichParRootFileIo
//
//  I/O for root files.
//
//////////////////////////////////////////////////////////////////////////////
// ***************************************************************************


ClassImp(HRichParRootFileIo)

//----------------------------------------------------------------------------
HRichParRootFileIo::HRichParRootFileIo(HParRootFile* f) : HDetParRootFileIo(f) {
  // constructor sets the name of the detector I/O "HRichParIo"
  fName="HRichParIo";
  initModules=new TArrayI(24);
}
//============================================================================

//----------------------------------------------------------------------------
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
//============================================================================

//----------------------------------------------------------------------------
Bool_t HRichParRootFileIo::init(HParSet* pPar,Int_t* set) {
  // initializes a container called by name, but only the modules
  // defined in the array 'set'
  // calls the special read function for this container
  // If it is called the first time it reads the setup found in the file

  if (!isActiv) readModules("Rich");
  const Text_t* name = ((TNamed*)pPar)->GetName();
  printf("rich par file io init - %s\n", name);
  if (pFile) {
    if (strcmp(name,"RichGeometryParameters")==0) return read((HRichGeometryPar*)pPar,set);
    if (strcmp(name,"RichDigitisationParameters")==0) return read((HRichDigitisationPar*)pPar,set);
    if (strcmp(name,"RichAnalysisParameters")==0) return read((HRichAnalysisPar*)pPar,set);
    if (strcmp(name,"RichCalPar")==0) return read((HRichCalPar*)pPar,set);
  }
  cerr<<"initialization of "<<name<<" not possible from ROOT file!"<<endl;
  return kFALSE;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HRichParRootFileIo::readObject(HRichParSet *pPar) {
  const Text_t* name = ((TNamed*)pPar)->GetName();
  TKey *key = (TKey*)gDirectory->GetListOfKeys()->FindObject(name);
  if (key) pPar->Read(name);

  pPar->setStatic();
  return (key != NULL);
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HRichParRootFileIo::read(HRichGeometryPar* pPar,Int_t* set) {
  if (!readObject(pPar)) return kFALSE;
  return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HRichParRootFileIo::read(HRichDigitisationPar* pPar,Int_t* set) {
  if (!readObject(pPar)) return kFALSE;
  return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HRichParRootFileIo::read(HRichAnalysisPar* pPar,Int_t* set) {
  if (!readObject(pPar)) return kFALSE;
  return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HRichParRootFileIo::read(HRichCalPar* pPar,Int_t* set) {
  if (!readObject(pPar)) return kFALSE;
  return kTRUE;
}
//============================================================================

