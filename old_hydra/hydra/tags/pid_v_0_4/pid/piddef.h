// @(#)$Id: piddef.h,v 1.3 2002-11-04 23:21:50 jaskula Exp $
#ifndef __PidDEF_H
#define __PidDEF_H

#include "haddef.h"

const Cat_t catPidTrackCand  = PID_OFFSET + 1;
const Cat_t catPidCandidate  = PID_OFFSET + 2;
const Cat_t catPidPart       = PID_OFFSET + 3;

enum EnumPidAlgorithm_t
{
    algNotSet = -1,
    algMerged,          // 0
    algBayes,           // 1
    algRich,            // 2
    algTof,             // 3
    algShowerTofino,    // 4
    algMomVsBeta,       // 5
    algClosePair        // 6
};

#define CL_ALOGRITHM_OFFSET 100

#define nPID_DUMMY 0
#define nUNKNOWN  -1.0f // return value for algorithms which they cannot
                        // calculate: e.g. shower Alg for hit in TOF

#endif
