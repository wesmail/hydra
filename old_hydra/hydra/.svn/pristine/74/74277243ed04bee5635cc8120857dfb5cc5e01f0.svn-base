// File: hrichsectorpar.h
// ****************************************************************************
//
// HRichSectorPar
//
// Author: Witold Przygoda (przygoda@psja1.if.uj.edu.pl)
// Modified: 1999/12/04 by Witold Przygoda (przygoda@psja1.if.uj.edu.pl)
// ----------------------------------------------------------------------------
//
// ****************************************************************************


#ifndef HRICHSECTORPAR_H
#define HRICHSECTORPAR_H


#include "TObject.h"
#include "TObjArray.h"
#include "hrichparset.h"

// ****************************************************************************

class HRichSectorPar: public HRichParSet {

// ****************************************************************************

  public:

    HRichSectorPar();
    virtual ~HRichSectorPar();

    void reset();

    void setSectorId(Int_t id) { fSectorId = id; }
    Int_t getSectorId() { return fSectorId; }

    Float_t getPhi() { return fRotationAngle; }
    void setPhi(const Float_t ang) { fRotationAngle = ang; }


public:

  Float_t fRotationAngle; // in degrees 
  Int_t fSectorId;


 ClassDef(HRichSectorPar, 1)
};

//============================================================================


#endif // HRICHSECTORPAR_H
