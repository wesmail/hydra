#ifndef HPIDALGSTANDCUTSPAR_H
#define HPIDALGSTANDCUTSPAR_H

#include "hpidhistogramscont.h"

// -----------------------------------------------------------------------------

#define PIDALGSTANDCUTSPAR_NAME    "PidAlgStandCutsPar"
#define PIDALGSTANDCUTSPAR_TITLE   "Parameters for HPidAlgStandCutsPar"
#define PIDALGSTANDCUTSPAR_CONTEXT "HPidAlgStandCutsParContext"

// -----------------------------------------------------------------------------

class HPidAlgStandCutsPar : public HPidHistogramsCont
{
public:

                HPidAlgStandCutsPar(
                        const char *name    = PIDALGSTANDCUTSPAR_NAME,
                        const char *title   = PIDALGSTANDCUTSPAR_TITLE,
                        const char *context = PIDALGSTANDCUTSPAR_CONTEXT);
    virtual    ~HPidAlgStandCutsPar(void) {printf("HPidAlgStandCutsPar deleted\n");}

    // -------------------------------------------------------------------------

    ClassDef(HPidAlgStandCutsPar, 1) // Params for HPidReconstructor
};

// -----------------------------------------------------------------------------

#endif  /* !HPidAlgStandCutsPar_H */
