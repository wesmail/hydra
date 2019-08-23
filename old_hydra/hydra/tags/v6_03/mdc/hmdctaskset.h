#ifndef HMDCTASKSET_H
#define HMDCTASKSET_H

#include "htaskset.h"
#include "hmdcdigitizer.h"

class HMdcDigitizer;
class HMdcTaskSet : public HTaskSet {

private:

static Int_t ilevel;            // counter for arguments of analysis levels
static Int_t irealorsimulation; // counter for arguments of real/simulation
static Int_t icalibrater;       // counter for arguments of calibrater mode
static Int_t itimecuts;         // counter for arguments of calibrater time cuts
static Int_t itracker;          // counter for arguments of tracker type
static Int_t imagnet;           // counter for arguments of magnet status
static Int_t itrackermode;      // counter for arguments of tracker mode (DUBNA)

static Int_t analysisLevel;   // 0=raw,1=cal1,2=cal2,3=hit,4=fit
static Int_t cal1TimeCutFlag; // 0=nocuts, 1=cuts
static Int_t cal1ModeFlag;    // 1=nostartandcal,2=startandcal,3=nostartandnocal

static Int_t tracker;         // 1=dubna,2=santiago
static Bool_t isCoilOff;      // kTRUE=magnetoff,kFALSE=magneton
static Int_t typeClustFinder; // 0=combinedchamberclusters,1=singlechamberclusters
static Bool_t fitNTuple;      // DUBNA:kTRUE=fill NTuple,kFALSE=fill nothing
static Bool_t fitPrint;       // DUBNA:kTRUE=print debug,kFALSE=print nothing
static Int_t fitVersion;      // DUBNA:1=old,2=new
static Bool_t isSimulation;   // kTRUE=sim.kFALSE=real

static HMdcDigitizer* mdcdigi;

public:
  HMdcTaskSet(void);
  HMdcTaskSet(Text_t name[],Text_t title[]);
  ~HMdcTaskSet(void);
  HTask *make(const char *select="",Option_t *option="");
  void HMdcTaskSet::parseArguments(TString);
  void setRealOrSim(Option_t*);
  void setAnalysisLevel(Option_t*);
  void setCal1TimeCuts(Int_t);
  void setCal1Mode(Int_t);
  void setCalibrater1(Int_t,Int_t);
  void selectTracker(Option_t*);
  void setDubnaHitFinderMode(Int_t,Bool_t);
  void setDubnaFitter(Bool_t,Bool_t,Int_t);
  HMdcDigitizer* getDigitizer();
  void printStatus();
  void printOptions();

  ClassDef(HMdcTaskSet,1) // Set of tasks
};

#endif /* !HMDCTASKSET_H */
