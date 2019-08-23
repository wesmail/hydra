// @(#)$Id: hpidreconstructorpar.h,v 1.4 2005-01-26 11:17:55 wojcik Exp $
//*-- Author : Marcin Jaskula 10/10/2002

#ifndef HPIDRECONSTRUCTORPAR_H
#define HPIDRECONSTRUCTORPAR_H

////////////////////////////////////////////////////////////////////////////////
//
// HPidReconstructorPar
//
////////////////////////////////////////////////////////////////////////////////

#include "hpidhistogramscont.h"

// -----------------------------------------------------------------------------

#define PIDRECONSTRUCTORPAR_NAME    "PidReconstructorPar"
#define PIDRECONSTRUCTORPAR_TITLE   "Parameters for HPidReconstructor"
#define PIDRECONSTRUCTORPAR_CONTEXT "PidReconstructorParContext"

// -----------------------------------------------------------------------------

class HPidReconstructorPar : public HPidHistogramsCont
{
public:

                HPidReconstructorPar(
                        const char *name    = PIDRECONSTRUCTORPAR_NAME,
                        const char *title   = PIDRECONSTRUCTORPAR_TITLE,
                        const char *context = PIDRECONSTRUCTORPAR_CONTEXT);
    virtual    ~HPidReconstructorPar(void) {;}

    // -------------------------------------------------------------------------

/*ADDON*/

    Float_t     getIntensity(Int_t iPartId, Int_t iSystem, Int_t iSector,
                                    Float_t fMom, Float_t fTheta) const
                        { return getValue(iPartId, iSystem, iSector, fMom, fTheta); }

    // -------------------------------------------------------------------------

    ClassDef(HPidReconstructorPar, 1) // Params for HPidReconstructor
};

// -----------------------------------------------------------------------------

#endif  /* !HPIDRECONSTRUCTORPAR_H */
