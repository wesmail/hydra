#ifndef TGO4HADES_H
#define TGO4HADES_H

// root specs
#include "TMutex.h"
#include "TString.h"
#include "TObjString.h"


// go4 specs
#include "Go4LockGuard/TGo4LockGuard.h"
#include "Go4CommandsBase/TGo4CommandReceiver.h"
#include "Go4CommandsBase/TGo4CommandInvoker.h"
#include "Go4StatusBase/TGo4ObjectStatus.h"
#include "Go4Event/TGo4EventDataIndex.h"
#include "Go4ConditionsBase/TGo4Condition.h"
#include "Go4DynamicList/TGo4DynamicList.h"
#include "Go4DynamicList/TGo4DynamicListStatus.h"
#include "Go4DynamicList/TGo4EntryAction.h"
#include "Go4EventServer/TGo4MainTree.h"
#include "Go4Analysis/TGo4Analysis.h"

#include "TGo4HadesConfigParameter.h"
#include "TGo4ResetParameter.h"
#include "TGo4MdcActiveParameter.h"
#include "TGo4PhysicsActiveParameter.h"
#include "TGo4TriggerParameter.h"

//hydra specs
#include "hades.h"
#include "htree.h"
#include "hldfilesource.h"
#include "hspectrometer.h"
#include "hruntimedb.h"

// root specs
#include "TH1.h"
#include "TH2.h"
#include "TList.h"


class  HldSockRemoteSource;
class  HldGrepFileSource;
class  HldSource;
class  HldFileSource;
class  HOraInfo;

class HGo4OnlineConfig;
class HGo4OnlineParam;

class HTriggerHistBooker;
class HStartHistBooker;
class HHodoHistBooker;
class HRichHistBooker;
class HMdcHistBooker;
class HTofHistBooker;
class HTofinoHistBooker;
class HShowerHistBooker;
class HPhysicsHistBooker;
class TStopwatch;
class HRichPattern;
class HRichFancy;
class HHadesPar;
class HProcessStat;

class TGo4Hades: public TGo4Analysis {

public:
  TGo4Hades();
  ~TGo4Hades();
 // TNamed * GetObject(const Text_t * name){ return NULL;}
  void  SetConfigFile(TString param)
  {
      if(param.CompareTo("")==0)
      {
	  Error("TGo4Hades:SetConfigFile()","NO CONFIG INPUT!");
	  exit(1);
      }
      else{ ConfigInput=param; }
  }
  Int_t UserPostLoop();
  Int_t UserPreLoop();
  Bool_t InitEventClasses();
  Int_t InitDetectorSetup();
  Int_t initParams();
  Int_t UserEventFunc(); 
	//void CloseAnalysis();   
  
private:
  Bool_t readAscii(Char_t* fileName);
  TString* inFile; //!
  TString* inDir; //!
  TString* outDir; //!
  TString* ParFile; //!
  TString* ParFile2; //!
  TString* FirstInput;  //!
  TString* SecondInput; //!
  TString* Beamtime; //!
  TString  ConfigInput; //!
  TString* eventHost;
  HldSource *source; //!
  HldFileSource       *sourceFile; //!
  HldGrepFileSource   *sourceGrep; //!
  HldSockRemoteSource *sourceR; //!
  HSpectrometer* spec; //!
  HRuntimeDb* rtdb;//! 
  Int_t refrunID; //!
  Int_t nOracle; //
  Int_t kEvtRemote;
  static Bool_t kINIT;//!
  static Int_t fCount; //! 
  static Int_t nLoop;
  Hades* myHades;//!
  //parameter
  Int_t fxTreeSize;
  Int_t fxRefreshNb; 
  Int_t fxReset;
  Bool_t fxDump;
  Bool_t fbInit;

  static HHadesPar* hadespar;
  static HGo4OnlineParam*  onlineparam;
  static HGo4OnlineConfig* onlineconfig;

  static TList* TriggeractiveHists;
  static HTriggerHistBooker* Triggerhistbooker;

  static TList* StartactiveHists;
  static HStartHistBooker* Starthistbooker;

  static TList* HodoactiveHists;
  static HHodoHistBooker* Hodohistbooker;

  static TList* RichactiveHists;
  static HRichHistBooker* Richhistbooker;
  static HRichPattern* richpattern;
  static HRichFancy* richfancy;

  static TList* MdcactiveHists;
  static HMdcHistBooker* Mdchistbooker;

  static TList* TofactiveHists;
  static HTofHistBooker* Tofhistbooker;

  static TList* TofinoactiveHists;
  static HTofinoHistBooker* Tofinohistbooker;

  static TList* ShoweractiveHists;
  static HShowerHistBooker* Showerhistbooker;

  static TList* PhysicsactiveHists;
  static HPhysicsHistBooker* Physicshistbooker;
  //-------------------------------
  TGo4HadesConfigParameter* hadesconfig;
  TGo4ResetParameter* reset;
  TGo4MdcActiveParameter* mdcActive;
  TGo4PhysicsActiveParameter* physicsActive;
  TGo4TriggerParameter* triggerParameter;
  HProcessStat* process; //!
  TStopwatch* timer; //!
  ClassDef(TGo4Hades,1) //Control class for Hades reconstruction
};	

#endif
