// @(#)$Id: hpidalgmomvsbetapar.h,v 1.2 2005-01-11 11:06:40 halo Exp $
//*-- Author : Marcin Jaskula 14/10/2002

#ifndef HPIDALGMOMVSBETAPAR_H
#define HPIDALGMOMVSBETAPAR_H

////////////////////////////////////////////////////////////////////////////////
//
// HPidAlgMomVsBetaPar
//
////////////////////////////////////////////////////////////////////////////////

#include "hpidhistogramscont.h"

// -----------------------------------------------------------------------------

#define PIDALGMOMVSBETAPAR_NAME    "PidAlgMomVsBetaPar"
#define PIDALGMOMVSBETAPAR_TITLE   "Parameters for HPidAlgMomVsBeta"
#define PIDALGMOMVSBETAPAR_CONTEXT "HPidAlgMomVsBetaContext"

// -----------------------------------------------------------------------------

class HPidAlgMomVsBetaPar : public HPidHistogramsCont
{
public:

                HPidAlgMomVsBetaPar(
                        const char *name    = PIDALGMOMVSBETAPAR_NAME,
                        const char *title   = PIDALGMOMVSBETAPAR_TITLE,
                        const char *context = PIDALGMOMVSBETAPAR_CONTEXT);
    virtual    ~HPidAlgMomVsBetaPar(void) {printf("HPidAlgMomVsBetaPar deleted\n");}

    // -------------------------------------------------------------------------

    Float_t     getParameterValue(Int_t iPartId, Int_t iParam, Int_t iSector,
                                    Float_t fMom, Float_t fTheta) const
                        { return getValue(iPartId, iParam, iSector, fMom, fTheta); }

    // -------------------------------------------------------------------------

    ClassDef(HPidAlgMomVsBetaPar, 1) // Params for HPidReconstructor
};

// -----------------------------------------------------------------------------

#endif  /* !HPIDALGMOMVSBETAPAR_H */
