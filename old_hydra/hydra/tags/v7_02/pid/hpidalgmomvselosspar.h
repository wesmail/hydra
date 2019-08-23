
#ifndef HPIDALGMOMVSELOSSPAR_H
#define HPIDALGMOMVSELOSSPAR_H

#include "hpidhistogramscont.h"

// -----------------------------------------------------------------------------

#define PIDALGMOMVSELOSSPAR_NAME    "PidAlgMomVsElossPar"
#define PIDALGMOMVSELOSSPAR_TITLE   "Parameters for HPidAlgMomVsEloss"
#define PIDALGMOMVSELOSSPAR_CONTEXT "HPidAlgMomVsElossContext"

// -----------------------------------------------------------------------------

class HPidAlgMomVsElossPar : public HPidHistogramsCont
{
public:

                HPidAlgMomVsElossPar(
                        const char *name    = PIDALGMOMVSELOSSPAR_NAME,
                        const char *title   = PIDALGMOMVSELOSSPAR_TITLE,
                        const char *context = PIDALGMOMVSELOSSPAR_CONTEXT);
    virtual    ~HPidAlgMomVsElossPar(void) {}

    // -------------------------------------------------------------------------

    Float_t     getParameterValue(Int_t iPartId, Int_t iParam,
                                    Float_t fMom, Float_t fTheta) const
                        { return getValue(iPartId, iParam, fMom, fTheta); }

    // -------------------------------------------------------------------------

    ClassDef(HPidAlgMomVsElossPar, 1) // Params for HPidReconstructor
};

// -----------------------------------------------------------------------------

#endif  /* !HPIDALGMOMVSELOSSPAR_H */
