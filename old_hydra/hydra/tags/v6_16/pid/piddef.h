// @(#)$Id: piddef.h,v 1.10 2004-04-30 15:08:08 christ Exp $
#ifndef __PidDEF_H
#define __PidDEF_H

#include "haddef.h"

const Cat_t catPidTrackCand  = PID_OFFSET + 1;
const Cat_t catPidCandidate  = PID_OFFSET + 2;
const Cat_t catPidPart       = PID_OFFSET + 3;
const Cat_t catPidDilepton   = PID_OFFSET + 4;

enum EnumPidAlgorithm_t
{
    algRelInt = -2,     // 1 - rel int. of the particles
    algNotSet = -1,
    algMerged,          // 0 - not filled any more (calculated in flight)
    algBayes,           // 1 - not filled any more (calculated in flight)
    algRich = 2,        // 2
    algTof,             // 3
    algShowerTofino,    // 4
    algMomVsBeta,       // 5
    algClosePair,       // 6
    algStandCuts,       // 7
    algMomVsEloss,       // 8
    algShower          // 9

};

#define LAST_COMMON_ALG     1
#define CL_ALOGRITHM_OFFSET 100

#define nPID_DUMMY 0
#define nUNKNOWN  -1.0f // return value for algorithms which they cannot
                        // calculate: e.g. shower Alg for hit in TOF

#define kIsSim      (1 << 14)// Flag for sim instance for objects
#define kIsFiltered (1 << 15)// Flag for filtered instance for objects
#define kIsBad      (1 << 16)// Flag for bad instance for objects

//Number of bins we use to build the different normalization factors for the
//PDF normalization;
#define nMomentumBins 10
			  


#endif
