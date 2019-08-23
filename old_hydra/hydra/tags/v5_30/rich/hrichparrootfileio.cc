// File: hrichparrootfileio.cc
// ***************************************************************************

//*-- Author : Witold Przygoda (przygoda@psja1.if.uj.edu.pl)
//*-- Modified : 1999/12/04 by Witold Przygoda (przygoda@psja1.if.uj.edu.pl)


#include <iostream.h>
#include "hrichparrootfileio.h"
#include "hparrootfileio.h"
#include "hades.h"
#include "hspectrometer.h"
#include "hruntimedb.h"
#include "hrichgeometrypar.h"
#include "hrichdigitisationpar.h"
#include "hrichanalysispar.h"
#include "hrichcalpar.h"
#include "hrichmappingpar.h"

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

 using namespace HadAsciiAuxiliaryFunctions;
  if (!isActiv) readModules("Rich");
  const Text_t* name = ((TNamed*)pPar)->GetName();
  if (pFile) {
    if (strcmp(name,"RichGeometryParameters")==0) return read((HRichGeometryPar*)pPar,set);
    if (strcmp(name,"RichDigitisationParameters")==0) return read((HRichDigitisationPar*)pPar,set);
    if (strcmp(name,"RichAnalysisParameters")==0) return read((HRichAnalysisPar*)pPar,set);
    if (strcmp(name,"RichCalPar")==0) return read((HRichCalPar*)pPar,set);
    if (strcmp(name,"RichMappingParameters")==0) return read((HRichMappingPar*)pPar,set);
  }
  ErrorMsg(2,"HRichParRootFileIo::init",
   3,"Initialisation of \'",name,"\' container from ROOT file not possible.
Container of this name is not known.");

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
 using namespace HadAsciiAuxiliaryFunctions;
  if (!readObject(pPar)) return kFALSE;
  ErrorMsg(0,"HRichParRootFileIo::read",
   1,"Container \'RichGeometryParameters\' has been read from ROOT file.");
  return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HRichParRootFileIo::read(HRichDigitisationPar* pPar,Int_t* set) {
 using namespace HadAsciiAuxiliaryFunctions;
  if (!readObject(pPar)) return kFALSE;
  ErrorMsg(0,"HRichParRootFileIo::read",
   1,"Container \'RichDigitisationParameters\' has been read from ROOT file.");
  return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HRichParRootFileIo::read(HRichAnalysisPar* pPar,Int_t* set) {
 using namespace HadAsciiAuxiliaryFunctions;
  if (!readObject(pPar)) return kFALSE;
  ErrorMsg(0,"HRichParRootFileIo::read",
   1,"Container \'RichAnalysisParameters\' has been read from ROOT file.");
  return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HRichParRootFileIo::read(HRichCalPar* pPar,Int_t* set) {
 using namespace HadAsciiAuxiliaryFunctions;
  if (!readObject(pPar)) return kFALSE;
  ErrorMsg(0,"HRichParRootFileIo::read",
   1,"Container \'RichCalPar\' has been read from ROOT file.");
  return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HRichParRootFileIo::read(HRichMappingPar* pPar,Int_t* set) {
 using namespace HadAsciiAuxiliaryFunctions;
  if (!readObject(pPar)) return kFALSE;
  ErrorMsg(0,"HRichParRootFileIo::read",
   1,"Container \'RichMappingPar\' has been read from ROOT file.");
  return kTRUE;
}
//============================================================================


