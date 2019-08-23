#ifndef __HADDEF_H
#define __HADDEF_H

#ifndef DEBUG_LEVEL
#define DEBUG_LEVEL 0
#endif

typedef Short_t Cat_t;
typedef Short_t IdOfs_t;
typedef UInt_t Loc_t;
 
const Cat_t catTrack=127;
const Cat_t catInvalid=-1;

const Cat_t catMdcRaw=0;
const Cat_t catMdcHit=1;

const Cat_t catRichRaw=32;
const Cat_t catRichHit=33;

const Cat_t catShowerRaw=64;
const Cat_t catShowerHit=65;

const Cat_t catTofRaw=96;
const Cat_t catTofHit=97;

const Cat_t catStartRaw=128;
const Cat_t catStartCal=129;
const Cat_t catStartHit=130;
const Cat_t catTBox=131;
const Cat_t catTBoxChan=132;
const Cat_t catTBoxChanSum=133;

const Cat_t catTofinoRaw=160;

const Cat_t catCaloRaw=224;

const Cat_t catGeantHeader=192;
const Cat_t catGeantKine=193;
const Cat_t catMdcGeantRaw=194;
const Cat_t catRichGeantRaw=195;
const Cat_t catShowerGeantRaw=198;    // RICH needs 3 categories, skip 2 !
const Cat_t catTofGeantRaw=199;
const Cat_t catStartGeantRaw=200;
const Cat_t catCaloGeantRaw=201;

const Cat_t catMdc=0;   // partial events
const Cat_t catRich=32;
const Cat_t catShower=64;
const Cat_t catTof=96;
const Cat_t catStart=128;
const Cat_t catTofino=160;
const Cat_t catSimul=192;
const Cat_t catCalo=224;
const Cat_t catMatchU=256;

const Int_t kBitCategorySize=5;
const Int_t kCategorySize=32;

const Int_t rlRaw=0;
const Int_t rlHit=-1;
const Int_t rlUndefined=-2;

const Int_t ctUnknown=-1;
const Int_t ctMatrix=0;
const Int_t ctSplit=1;
const Int_t ctLinear=2;

const Int_t kSimulationEvent=0;
const Int_t kRealEvent=1;
const Int_t kCalibrationEvent=4;
const UInt_t kStartEvent=65538;

#endif





