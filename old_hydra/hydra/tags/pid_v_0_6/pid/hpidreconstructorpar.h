// @(#)$Id: hpidreconstructorpar.h,v 1.1 2002-10-30 10:55:33 halo Exp $
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
#define PIDRECONSTRUCTORPAR_CONTEXT "PidTrackFillerParContext"

// -----------------------------------------------------------------------------

class HPidReconstructorPar : public HPidHistogramsCont
{
public:

                HPidReconstructorPar(
                        const char *name    = PIDRECONSTRUCTORPAR_NAME,
                        const char *title   = PIDRECONSTRUCTORPAR_TITLE,
                        const char *context = PIDRECONSTRUCTORPAR_CONTEXT);
    virtual    ~HPidReconstructorPar(void) {}

    // -------------------------------------------------------------------------

    Float_t     getIntensity(Int_t iPartId, Int_t iSystem,
                                    Float_t fMom, Float_t fTheta) const
                        { return getValue(iPartId, iSystem, fMom, fTheta); }

    // -------------------------------------------------------------------------

    ClassDef(HPidReconstructorPar, 1) // Params for HPidReconstructor
};

// -----------------------------------------------------------------------------

#endif  /* !HPIDRECONSTRUCTORPAR_H */
