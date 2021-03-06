#ifndef __HADDEF_H
#define __HADDEF_H

#ifndef DEBUG_LEVEL
#define DEBUG_LEVEL 0
#endif

typedef Short_t Cat_t;
typedef Short_t IdOfs_t;
typedef UInt_t Loc_t;


// Hydra Global Variables
const Int_t kBitCategorySize=5;
const Int_t kCategorySize=32;

const Int_t rlRaw=0;
const Int_t rlHit=-1;
const Int_t rlUndefined=-2;

const Int_t ctUnknown=-1;
const Int_t ctMatrix=0;
const Int_t ctSplit=1;
const Int_t ctLinear=2;

const UInt_t kStartEvent=0xd;
const UInt_t kLastEvent=0xe;

//Task control variable
const Int_t kSkipEvent=-9;


// Main Partial Event  identifiers

const Int_t INCREMENT=32;
const Int_t STARTVALUE=0; 
const Int_t MDC_OFFSET=STARTVALUE;                    // 0
const Int_t RICH_OFFSET= STARTVALUE + 1*INCREMENT;    // 32
const Int_t SHOWER_OFFSET= STARTVALUE + 2*INCREMENT;  // 64
const Int_t TOF_OFFSET= STARTVALUE + 3*INCREMENT;     // 96
const Int_t START_OFFSET= STARTVALUE + 4*INCREMENT;   // 128
const Int_t TOFINO_OFFSET= STARTVALUE + 5*INCREMENT;  // 160
const Int_t SIMUL_OFFSET= STARTVALUE + 6*INCREMENT;   // 192
const Int_t CALO_OFFSET= STARTVALUE + 7*INCREMENT;    // 224
const Int_t MATCHU_OFFSET= STARTVALUE + 8*INCREMENT;  // 256
const Int_t TRACK_OFFSET= STARTVALUE + 9*INCREMENT;   // 288
const Int_t PHYANA_OFFSET= STARTVALUE + 10*INCREMENT; // 320
const Int_t PID_OFFSET = STARTVALUE + 11*INCREMENT;   // 352

//define offset for the partial events
const Cat_t catMdc=     MDC_OFFSET;   
const Cat_t catRich=    RICH_OFFSET;
const Cat_t catShower=  SHOWER_OFFSET;
const Cat_t catTof=     TOF_OFFSET;
const Cat_t catStart=   START_OFFSET;
const Cat_t catTofino=  TOFINO_OFFSET;
const Cat_t catSimul=   SIMUL_OFFSET;
const Cat_t catCalo=    CALO_OFFSET;
const Cat_t catMatchU=  MATCHU_OFFSET;
const Cat_t catTracks=  TRACK_OFFSET;
const Cat_t catPhyAna=  PHYANA_OFFSET;

const Cat_t catTrack=127;
const Cat_t catInvalid=-1;

#endif






