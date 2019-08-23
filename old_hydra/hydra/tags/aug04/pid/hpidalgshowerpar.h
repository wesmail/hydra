// @(#)$Id:
//*-- Author : Marcin Jaskula 14/10/2002

#ifndef HPIDALGSHOWERPAR_H
#define HPIDALGSHOWERPAR_H

////////////////////////////////////////////////////////////////////////////////
//
// HPidAlgShowerPar
//
////////////////////////////////////////////////////////////////////////////////

#include "hpidhistogramscont.h"

// -----------------------------------------------------------------------------

#define PIDALGSHOWERPAR_NAME    "PidAlgShowerPar"
#define PIDALGSHOWERPAR_TITLE   "Parameters for HPidAlgShower"
#define PIDALGSHOWERPAR_CONTEXT "HPidAlgShowerContext"
#define PIDALGSHOWERPAR_HIST_NUM 4
#define SUM0_OFFSET 0
#define MAXF_OFFSET 1
// -----------------------------------------------------------------------------

class HPidAlgShowerPar : public HPidHistogramsCont
{
public:

                HPidAlgShowerPar(
                        const char *name    = PIDALGSHOWERPAR_NAME,
                        const char *title   = PIDALGSHOWERPAR_TITLE,
                        const char *context = PIDALGSHOWERPAR_CONTEXT);
    virtual    ~HPidAlgShowerPar(void) {}

    // -------------------------------------------------------------------------

    Float_t     getParameterValue(Int_t iPartId, Int_t iParam,
                                    Float_t fMom, Float_t fTheta) const
                        { return getValue(iPartId, iParam, fMom, fTheta); }

    // -------------------------------------------------------------------------

    ClassDef(HPidAlgShowerPar, 1) // Params for HPidReconstructor
};

// -----------------------------------------------------------------------------

#endif  /* !HPIDALGSHOWERPAR_H */
