
#ifndef HPIDELOSSMPVSHIFTPAR_H
#define HPIDELOSSMPVSHIFTPAR_H

#include "hpidhistogramscont.h"

// -----------------------------------------------------------------------------

#define PIDELOSSMPVSHIFTPAR_NAME    "PidElossMPVShiftPar"
#define PIDELOSSMPVSHIFTPAR_TITLE   "Parameters HPidElossMPVShiftPar"
#define PIDELOSSMPVSHIFTPAR_CONTEXT "HPidElossMPVShiftContext"

// -----------------------------------------------------------------------------

class HPidElossMPVShiftPar : public HPidHistogramsCont
{
public:

                HPidElossMPVShiftPar(
                        const char *name    = PIDELOSSMPVSHIFTPAR_NAME,
                        const char *title   = PIDELOSSMPVSHIFTPAR_TITLE,
                        const char *context = PIDELOSSMPVSHIFTPAR_CONTEXT);
    virtual    ~HPidElossMPVShiftPar(void) {}

    // -------------------------------------------------------------------------

    Float_t     getParameterValue(Int_t iPartId, Int_t iParam,
                                    Float_t fMom, Float_t fTheta) const
                        { return getValue(iPartId, iParam, fMom, fTheta); }

    // -------------------------------------------------------------------------

    ClassDef(HPidElossMPVShiftPar, 1) // Params for HPidAlgMomVSEloss reconstructor
};

// -----------------------------------------------------------------------------

#endif  /* !HPIDELOSSMPVSHIFTPAR_H */
