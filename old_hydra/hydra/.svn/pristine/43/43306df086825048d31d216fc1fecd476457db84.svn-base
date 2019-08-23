// File: hrichparhadasciifileio.cc
// ***************************************************************************

//*-- Author : Witold Przygoda (przygoda@psja1.if.uj.edu.pl)
//*-- Modified : 2002/23/01 by Ilse Koenig
//    last changed by Thomas Eberl Fri Aug 17 14:22:41 CEST 2001
//*-- Modified : 2000/02/11 by Witold Przygoda (przygoda@psja1.if.uj.edu.pl)


#include <iostream.h>
#include "hrichparhadasciifileio.h"
#include "hades.h"
#include "hspectrometer.h"
#include "hrichdetector.h"
#include "hrichgeometrypar.h"
#include "hrichthresholdpar.h"
#include "hrichcalpar.h"
#include "hrichanalysispar.h"
#include "hrichdigitisationpar.h"
#include "hrichmappingpar.h"
#include "hrichcorrelatorpar.h"
// ***************************************************************************
//////////////////////////////////////////////////////////////////////////////
//
//  HRichParHadAsciiFileIo
//
//  I/O in had-ascii.
//
//////////////////////////////////////////////////////////////////////////////
// ***************************************************************************


ClassImp(HRichParHadAsciiFileIo)

//----------------------------------------------------------------------------
HRichParHadAsciiFileIo::HRichParHadAsciiFileIo(HParHadAsciiFileIo* f) 
                            : HDetParHadAsciiFileIo(f) {
  fName="HRichParIo";
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HRichParHadAsciiFileIo::init(HParSet* pPar,Int_t* set) {
  // initializes a container called by name, but only the modules
  // defined in the array 'set'
  // calls the special read function for this container
  // If it is called the first time it reads the setup found in the file

 using namespace HadAsciiAuxiliaryFunctions;
  const Text_t* name = ((TNamed*)pPar)->GetName();
  // printf("rich par file io init - %s\n", name);
  if (pFile) {
    if (strcmp(name,"RichGeometryParameters")==0) 
                                return read((HRichGeometryPar*)pPar,set);
    if (strcmp(name,"RichCalPar")==0) 
                                return read((HRichCalPar*)pPar,set);
    if (strcmp(name,"RichThresholdPar")==0) 
                                return read((HRichThresholdPar*)pPar,set);
    if (strncmp(name,"RichAnalysisParameters",strlen("RichAnalysisParameters"))==0)
                                return read((HRichAnalysisPar*)pPar,set);
    if (strncmp(name,"RichDigitisationParameters",strlen("RichDigitisationParameters"))==0)
                                return read((HRichDigitisationPar*)pPar,set);
    if (strcmp(name,"RichMappingParameters")==0)
                                return read((HRichMappingPar*)pPar,set);
    if (strncmp(name,"RichCorrelatorParameters",strlen("RichCorrelatorParameters"))==0)
                                return read((HRichCorrelatorPar*)pPar,set);
  }
  ErrorMsg(2,"HRichParHadAsciiFileIo::init",
   3,"Initialisation of \'",name,"\' container from ASCII file not possible. 
Container of this name is not known.");
  return kFALSE;
}
//============================================================================

//----------------------------------------------------------------------------
Int_t HRichParHadAsciiFileIo::write(HParSet* pPar) {
 using namespace HadAsciiAuxiliaryFunctions;
  const  Text_t* name = ((TNamed*)pPar)->GetName();
  if (pFile) {

    if (strcmp(name,"RichGeometryParameters")==0) 
                                return writeFile((HRichGeometryPar*)pPar);
    if (strcmp(name,"RichCalPar")==0) 
                                return writeFile((HRichCalPar*)pPar);
    if (strcmp(name,"RichThresholdPar")==0) 
                                return writeFile((HRichThresholdPar*)pPar);
    if (strncmp(name,"RichAnalysisParameters",strlen("RichAnalysisParameters"))==0)
                                return writeFile((HRichAnalysisPar*)pPar);
    if (strncmp(name,"RichDigitisationParameters",strlen("RichDigitisationParameters"))==0)
                                return writeFile((HRichDigitisationPar*)pPar);
    if (strcmp(name,"RichMappingParameters")==0)
                                return writeFile((HRichMappingPar*)pPar);
    if (strncmp(name,"RichCorrelatorParameters",strlen("RichCorrelatorParameters"))==0)
                                return writeFile((HRichCorrelatorPar*)pPar);
  }

  ErrorMsg(2,"HRichParHadAsciiFileIo::write",
   3,"Writing of \'",name,"\' container to ASCII file not possible. 
Container of this name is not known.");
  return kFALSE;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HRichParHadAsciiFileIo::write(HDetector* p) {
  return kTRUE;  // not implemented!
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HRichParHadAsciiFileIo::read(HRichGeometryPar* pPar,Int_t* set) {
    Bool_t status = pPar->initAscii(pFile);
    pPar->setChanged();
    return status;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HRichParHadAsciiFileIo::read(HRichThresholdPar* pPar,Int_t* set) {
    Bool_t status = pPar->initAscii(pFile);
    pPar->setChanged();
    return status;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HRichParHadAsciiFileIo::read(HRichCalPar* pPar,Int_t* set) {
    Bool_t status = pPar->initAscii(pFile);
    pPar->setChanged();
    return status;
}
//============================================================================


//----------------------------------------------------------------------------
Bool_t HRichParHadAsciiFileIo::read(HRichAnalysisPar* pPar, Int_t* set) {
    Bool_t status = pPar->initAscii(pFile);
    pPar->setChanged();
    return status;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HRichParHadAsciiFileIo::read(HRichDigitisationPar* pPar, Int_t* set) {
    Bool_t status = pPar->initAscii(pFile);
    pPar->setChanged();
    return status;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HRichParHadAsciiFileIo::read(HRichMappingPar* pPar, Int_t* set) {
    Bool_t status = pPar->initAscii(pFile);
    pPar->setChanged();
    return status;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HRichParHadAsciiFileIo::read(HRichCorrelatorPar* pPar, Int_t* set) {
    Bool_t status = pPar->initAscii(pFile);
    pPar->setChanged();
    return status;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HRichParHadAsciiFileIo::writeFile(HRichGeometryPar* pPar) {
 return pPar->writeAscii(pFile);
} 
//============================================================================

//----------------------------------------------------------------------------
Bool_t HRichParHadAsciiFileIo::writeFile(HRichCalPar* pPar) {
 return pPar->writeAscii(pFile);
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HRichParHadAsciiFileIo::writeFile(HRichThresholdPar* pPar) {
 return pPar->writeAscii(pFile);
}
//============================================================================


//----------------------------------------------------------------------------
Bool_t HRichParHadAsciiFileIo::writeFile(HRichAnalysisPar* pPar) {
 return pPar->writeAscii(pFile);
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HRichParHadAsciiFileIo::writeFile(HRichDigitisationPar *pPar) {
 return pPar->writeAscii(pFile);
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HRichParHadAsciiFileIo::writeFile(HRichMappingPar *pPar) {
 return pPar->writeAscii(pFile);
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HRichParHadAsciiFileIo::writeFile(HRichCorrelatorPar *pPar) {
 return pPar->writeAscii(pFile);
}
//============================================================================


