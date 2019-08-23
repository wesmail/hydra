#ifndef HYPINFODEF_H
#define HYPINFODEF_H

#include "haddef.h"

const Cat_t     catHypList = HYP_OFFSET + 10;
const Cat_t     catHypComb = HYP_OFFSET + 11;
const Cat_t     catHypKine = HYP_OFFSET + 12;

#define UNDEFINED_IDX      -2
#define ERROR_IDX          -3
#define EMPTY_IDX          -1

#define CURRENT_EVENT      -1
#define FROM_TREE          -2

/*******user defined values *********/

#define MAX_USER_VALUES    3000

#define DELTATOF_DTOF      100
#define DELTATOF_SIGMA     101
#define DELTATOF_OFFSET    102
#define DELTATOF_CHI2      103
#define DELTATOF_DTOF2     104

// ANARS Pre-Fit
#define KINEFIT_PRECHI2        110
// Final Kine Chi2
#define KINEFIT_CHI2           111
// Channel codes which were fittet
#define KINEFIT_CHANNEL_LO     112
#define KINEFIT_CHANNEL_HI     113
// probability, more usefull than chi2
#define KINEFIT_PROB           114

// Number of valid PID tracks which were used by filler (incl "fakes")
#define FILLER_VALID_PIDTRACKS  200
// The missing particle which was set by filler (for fitter)
#define FILLER_MISSING_PID      201

#define FW_THETA      300
#define FW_PHI        301
#define FW_TOF        302

/*******cut values (par container) *********/

#define MAX_PARAM_VALUES    100

#define PI0_LOWER_CUT       10000
#define PI0_UPPER_CUT       10001

#define PP_PIP_PIM_FILLER_ALGCUT  20000

// definition for the FILLER
// ... is also used by other algos for defining (max) array sizes
#define HYP_MAX_PARTICLES         10

#endif /* !HYPINFODEF_H */
