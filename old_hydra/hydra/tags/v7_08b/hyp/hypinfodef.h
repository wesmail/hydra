#ifndef HYPINFODEF_H
#define HYPINFODEF_H

#include "haddef.h"

/*
const Cat_t     catHypList = PID_OFFSET + 10;
const Cat_t     catHypComb = PID_OFFSET + 11;
const Cat_t     catHypKine = PID_OFFSET + 12;
*/
const Cat_t     catHypList = HYP_OFFSET + 10;
const Cat_t     catHypComb = HYP_OFFSET + 11;
const Cat_t     catHypKine = HYP_OFFSET + 12;

#define UNDEFINED_IDX      -2
#define ERROR_IDX          -3
#define EMPTY_IDX          -1

#define CURRENT_EVENT      -1
#define FROM_TREE          -2

/*******user defined values *********/

#define MAX_USER_VALUES    100

#define DELTATOF_DTOF      100
#define DELTATOF_SIGMA     101
#define DELTATOF_OFFSET    102

#define KINEFIT_CHI2      110
#define KINEFIT_CHI24     111

#define FILLER_VALID_PIDTRACKS  200

/*******cut values (par container) *********/

#define MAX_PARAM_VALUES    100

#define PI0_LOWER_CUT       10000
#define PI0_UPPER_CUT       10001

#define PP_PIP_PIM_FILLER_ALGCUT  20000

#endif /* !HYPINFODEF_H */
