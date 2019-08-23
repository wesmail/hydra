// File: hrichparrootfileio.h
// ****************************************************************************
//
// HRichParRootFileIo
//
// Author: Witold Przygoda (przygoda@psja1.if.uj.edu.pl)
// Modified: 1999/12/04 by Witold Przygoda (przygoda@psja1.if.uj.edu.pl)
// ----------------------------------------------------------------------------
//
// ****************************************************************************


#ifndef HRICHPARROOTFILEIO_H
#define HRICHPARROOTFILEIO_H

#include "hdetparrootfileio.h"
#include <TFile.h>
#include <TArrayI.h>
#include <TKey.h>
class HParRootFile;
class HParSet;
class HRichParSet;
class HRichGeometryPar;
class HRichCalPar;
class HRichMappingPar;
// ****************************************************************************

class HRichParRootFileIo : public HDetParRootFileIo {

// ****************************************************************************

public:

  HRichParRootFileIo(HParRootFile* f);
  ~HRichParRootFileIo();
  Bool_t init(HParSet*,Int_t*);
  Bool_t readObject(HRichParSet* pPar);

  Bool_t read(HRichGeometryPar* pPar,Int_t* set);
  Bool_t read(HRichCalPar* pPar,Int_t* set);
  Bool_t read(HRichMappingPar* pPar,Int_t* set);

  ClassDef(HRichParRootFileIo,0) // Class for Rich parameter I/O from ROOT file
};

//============================================================================


#endif  // HRICHPARROOTFILEIO_H

