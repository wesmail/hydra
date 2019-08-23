// File: hrichparrootfileio.cc
// ***************************************************************************

//*-- Author : Witold Przygoda (przygoda@psja1.if.uj.edu.pl)
//*-- Modified : 2002/15/01 by Ilse Koenig
//*-- Modified : 1999/12/04 by Witold Przygoda (przygoda@psja1.if.uj.edu.pl)

using namespace std;
#include <iostream> 
#include <iomanip>
#include "hrichparrootfileio.h"
#include "hparrootfileio.h"
#include "hades.h"
#include "hspectrometer.h"
#include "hruntimedb.h"
#include "hrichgeometrypar.h"
#include "hrichdigitisationpar.h"
#include "hrichanalysispar.h"
#include "hrichthresholdpar.h"
#include "hrichcalpar.h"
#include "hrichmappingpar.h"
#include "hrichcorrelatorpar.h"

// ***************************************************************************
//_HADES_CLASS_DESCRIPTION 
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
  initModules=new TArrayI(6);
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
    if (strncmp(name,"RichDigitisationParameters",strlen("RichDigitisationParameters"))==0) return HDetParRootFileIo::read(pPar);
    if (strncmp(name,"RichAnalysisParameters",strlen("RichAnalysisParameters"))==0) return HDetParRootFileIo::read(pPar);
    if (strcmp(name,"RichThresholdPar")==0) return read((HRichThresholdPar*)pPar,set);
    if (strcmp(name,"RichCalPar")==0) return read((HRichCalPar*)pPar,set);
    if (strcmp(name,"RichMappingParameters")==0) return read((HRichMappingPar*)pPar,set);
    if (strncmp(name,"RichCorrelatorParameters",strlen("RichCorrelatorParameters"))==0) return HDetParRootFileIo::read(pPar);
  }
  ErrorMsg(2,"HRichParRootFileIo::init",
   3,"Initialisation of \'",name,"\' container from ROOT file not possible. \
Container of this name is not known.");

  return kFALSE;
}
//============================================================================

//----------------------------------------------------------------------------
//  Bool_t HRichParRootFileIo::readObject(HRichParSet *pPar) {
//    without version management
//    const Text_t* name = ((TNamed*)pPar)->GetName();
//    TKey *key = (TKey*)gDirectory->GetListOfKeys()->FindObject(name);
//    if (key) pPar->Read(name);

//    pPar->setStatic();
//    pPar->setChanged();
//    return (key != NULL);
//  }
//============================================================================

//----------------------------------------------------------------------------
Bool_t HRichParRootFileIo::readObject(HRichParSet *pPar) {
    // with version management from J. Otwinowski and I. Koenig
    Text_t* name = (char*)pPar->GetName();
    Int_t version = findInputVersion(name);
    
    if (version<=0) 
    {
	pPar->setInputVersion(-1,inputNumber);
	return kFALSE;    // not in ROOT file
    }
    if (pPar->getInputVersion(inputNumber)==version
        && pPar->getInputVersion(inputNumber)!=-1) return kTRUE;
    
    TKey *key = (TKey*)gDirectory->GetKey(name,version);
    
    // need reinitalization 
    
    if(key)
    { 

	key->Read(pPar); 
	pPar->setInputVersion(version,inputNumber);
	pPar->setChanged();
	
    } else {
	
	pPar->setInputVersion(-1,inputNumber);
	//pPar->setChanged();
	
    }
    
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
Bool_t HRichParRootFileIo::read(HRichCalPar* pPar,Int_t* set) {
 using namespace HadAsciiAuxiliaryFunctions;
  if (!readObject(pPar)) return kFALSE;
  ErrorMsg(0,"HRichParRootFileIo::read",
   1,"Container \'RichCalPar\' has been read from ROOT file.");
  return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HRichParRootFileIo::read(HRichThresholdPar* pPar,Int_t* set) {
  using namespace HadAsciiAuxiliaryFunctions;
  if (!readObject(pPar)) return kFALSE;
  ErrorMsg(0,"HRichParRootFileIo::read",
	   1,"Container \'RichThresholdPar\' has been read from ROOT file.");
  return kTRUE;
}
//==

//----------------------------------------------------------------------------
Bool_t HRichParRootFileIo::read(HRichMappingPar* pPar,Int_t* set) {
 using namespace HadAsciiAuxiliaryFunctions;
  if (!readObject(pPar)) return kFALSE;
  ErrorMsg(0,"HRichParRootFileIo::read",
   1,"Container \'RichMappingPar\' has been read from ROOT file.");
  return kTRUE;
}






