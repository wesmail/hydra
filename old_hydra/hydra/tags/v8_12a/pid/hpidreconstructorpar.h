// @(#)$Id: hpidreconstructorpar.h,v 1.5 2006-08-16 16:13:35 halo Exp $
//*-- Author : Marcin Jaskula 10/10/2002

#ifndef HPIDRECONSTRUCTORPAR_H
#define HPIDRECONSTRUCTORPAR_H

////////////////////////////////////////////////////////////////////////////////
//
// HPidReconstructorPar: 
// Stores histograms with relative intensities of particles as a function of 
// momentum and polar angle. These "RELINTS" are the prior probabilities that
// are to be taken into account when computing posterior probabilites with bayes
// theorem
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


    //Get the relaitve intensity for a certain species, system, sector and region of phase space (momentum, polar angle)
    Float_t     getIntensity(Int_t iPartId, Int_t iSystem, Int_t iSector,
			     Float_t fMom, Float_t fTheta) const
      { return getValue(iPartId, iSystem, iSector, fMom, fTheta); }

    // -------------------------------------------------------------------------

    ClassDef(HPidReconstructorPar, 1) // Params for HPidReconstructor
};

// -----------------------------------------------------------------------------

#endif  /* !HPIDRECONSTRUCTORPAR_H */
