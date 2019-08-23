// File: hrichparhadasciifileio.h
// ****************************************************************************
//
// HRichParHadAsciiFileIo
//
// Author: Witold Przygoda (przygoda@psja1.if.uj.edu.pl)
// Modified: 1999/12/04 by Witold Przygoda (przygoda@psja1.if.uj.edu.pl)
// ----------------------------------------------------------------------------
//
// ****************************************************************************


#ifndef HRICHPARHADASCIIFILEIO_H
#define HRICHPARHADASCIIFILEIO_H

#include <fstream.h>

#include "hdetpario.h"
#include "hdetparhadasciifileio.h"
#include "TObject.h"
#include "TFile.h"
#include "TArrayI.h"

class HRichGeometryPar;
class HRichCalPar;
class HRichAnalysisPar;
class HRichDigitisationPar;


// ****************************************************************************

class HRichParHadAsciiFileIo : public HDetParHadAsciiFileIo {

// ****************************************************************************

public:

  HRichParHadAsciiFileIo(HParHadAsciiFileIo* f);
  ~HRichParHadAsciiFileIo() {}

  // calls special read-function for each container type
  Bool_t init(HParSet*,Int_t*);
  Bool_t read(HRichGeometryPar* pPar,Int_t* set);
  Bool_t read(HRichCalPar* pPar,Int_t* set);
  Bool_t read(HRichAnalysisPar* pPar,Int_t* set);
  Bool_t read(HRichDigitisationPar* pPar,Int_t* set);

  // calls special update-function for each container type
  Int_t write(HParSet*);

  Bool_t writeFile(HRichGeometryPar* pPar);
  Bool_t writeFile(HRichCalPar* pPar);
  Bool_t writeFile(HRichAnalysisPar* pPar);
  Bool_t writeFile(HRichDigitisationPar* pPar);
  // writes the setup information to file
  Bool_t write(HDetector*);

  ClassDef(HRichParHadAsciiFileIo,0) 
};

//============================================================================


#endif  // HRICHPARHADASCIIFILEIO_H
