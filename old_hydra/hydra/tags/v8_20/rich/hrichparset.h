// File: hrichparset.h
// ****************************************************************************
//
// HRichParSet
//
// Author: Witold Przygoda (przygoda@psja1.if.uj.edu.pl)
// Modified: 1999/12/04 by Witold Przygoda (przygoda@psja1.if.uj.edu.pl)
// ----------------------------------------------------------------------------
//
// ****************************************************************************


#ifndef HRICHPARSET_H
#define HRICHPARSET_H

#include "hparset.h"
#include "hparhadasciifileio.h"

class HParIo;

// ****************************************************************************

class HRichParSet : public HParSet {

// ****************************************************************************

public:

  HRichParSet(const Char_t* name="",const Char_t* title="",const Char_t* context="");
  virtual ~HRichParSet() {}

  virtual Bool_t init(HParIo* inp, Int_t* set);  
  virtual Int_t write(HParIo*);

  ClassDef(HRichParSet,1) // Base class for all parameter containers

protected:

  HAscii *m_pReadParam;  //!
};

//============================================================================


#endif  // HRICHPARSET_H

