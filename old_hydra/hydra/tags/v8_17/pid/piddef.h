// @(#)$Id: piddef.h,v 1.18 2007-06-19 16:43:50 jacek Exp $
#ifndef __PidDEF_H
#define __PidDEF_H

#include "haddef.h"

const Cat_t catPidTrackCand  = PID_OFFSET + 1;
const Cat_t catPidCandidate  = PID_OFFSET + 2;
const Cat_t catPidPart       = PID_OFFSET + 3;
const Cat_t catPidDilepton   = PID_OFFSET + 4;
const Cat_t catPidDynamic    = PID_OFFSET + 5;
const Cat_t catPidPoorCandidate  = PID_OFFSET + 6;
const Cat_t catPidEvtInfo  = PID_OFFSET + 7;


//List of all defined PID algorithms
enum EnumPidAlgorithm_t
{
    algRelInt = -2,     // 1 - rel int. of the particles
    algNotSet = -1,
    algMerged,          // 0 - not filled any more (calculated in flight)
    algBayes,           // 1 - not filled any more (calculated in flight)
    algRich = 2,        // 2
    algTof,             // 3
    algShower,          // 4
    algMomVsBeta,       // 5
    algClosePair,       // 6
    algStandCuts,       // 7
    algMomVsEloss,      // 8
    algHardCuts,        // 9
    algMdcEloss         // 10
};

#define algLikelihood algMerged

//Enumerated variables indicating which detector has seen a geant particle
//and combinations thereof (to be used as shorthand-notation)
enum enumDetBits
  {
    //individual detectors
    kTrackNotSet         = 0x00,  // This object does not contain valid data
    kSeenInRICHIPU       = 0x01,  // RICH IPU has seen this trackid
    
    kSeenInOuterMDC      = 0x02,  // Outer Mdc has seen this trackid
    kSeenInRICH          = 0x04,  // RICH has seen this trackid
    kSeenInInnerMDC      = 0x08,  // Inner Mdc has seen this trackid
    
    kSeenInTOF           = 0x10,  // TOF has seen this trackid
    kSeenInShower        = 0x20,  // Shower has seen this trackid
    kSeenInMETA          = 0x40,  // One of the Meta dets has seen this trackid
    
  };
enum combinations
  {
    // combinations
    
    kTracklet            = kSeenInRICH | kSeenInInnerMDC,      //RICH and Inner Mdc have seen this trackid
    
    kTrackKick           = kSeenInInnerMDC | kSeenInMETA,      //No outer MDC segment but KickPlane       
    kTrackSpline         = kSeenInInnerMDC | kSeenInOuterMDC,  //enough for spline computation
    kTrackFullHadron     = kTrackSpline | kSeenInMETA,          
    
    kTrackKickRICH       = kTrackKick | kSeenInRICH,            //Same as the above, but now with RICH
    kTrackSplineRICH     = kTrackSpline | kSeenInRICH,          
    kTrackFullLepton     = kTrackFullHadron | kSeenInRICH
  };

#define LAST_COMMON_ALG     1
#define CL_ALOGRITHM_OFFSET 100

#define nPID_DUMMY 0
#define nUNKNOWN  -1.0f // return value for algorithms which they cannot
                        // calculate: e.g. shower Alg for hit in TOF

//#define kIsSim      (1 << 14)// Flag for sim instance for objects
//#define kIsFiltered (1 << 15)// Flag for filtered instance for objects
//#define kIsBad      (1 << 16)// Flag for bad instance for objects



#define MAX_MOM_ALGS 10
#define MAX_SECTOR 6
#define MAX_IOSEG 2

#define ALG_KICK 0
#define ALG_KICK123 1
#define ALG_SPLINE 2
#define ALG_REFT 3
#define ALG_RUNGEKUTTA 4

#define PLUTO_COCKTAIL 1
#define URQMD_EMBEDDED 2
#define REAL_EMBEDDED 3
#define URQMD_PURE 4

#endif
