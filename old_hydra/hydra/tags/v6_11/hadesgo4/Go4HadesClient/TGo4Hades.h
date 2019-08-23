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

class HGo4OnlineConfig;
class HGo4OnlineParam;

class HStartHistBooker;
class HRichHistBooker;
class HMdcHistBooker;
class HTofHistBooker;
class HTofinoHistBooker;
class HShowerHistBooker;

class MyParameter : public TGo4Parameter
{ 
 public:
  MyParameter (); 
  MyParameter( Text_t* text); 
  ~MyParameter( ); 
  
 Bool_t UpdateFrom(TGo4Parameter * para); 
 
 public: 
 Int_t test1;   
 TObjString test2; 
 
  ClassDef(MyParameter,1)
};

class TGo4Hades: public TGo4Analysis {

public:
  TGo4Hades();
  ~TGo4Hades();
 // TNamed * GetObject(const Text_t * name){ return NULL;}
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
  TString* RootParFile; //!
  TString* eventHost;
  HldFileSource *source; //!
  HldSockRemoteSource *sourceR; //!
  HSpectrometer* spec; //!
  HRuntimeDb* rtdb;//! 
  Int_t refrunID; //!
  Int_t nOracle; //
  Bool_t kEvtRemote;	
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

  static HGo4OnlineParam*  onlineparam;
  static HGo4OnlineConfig* onlineconfig;

  static TList* StartactiveHists;
  static HStartHistBooker* Starthistbooker;

  static TList* RichactiveHists;
  static HRichHistBooker* Richhistbooker;

  static TList* MdcactiveHists;
  static HMdcHistBooker* Mdchistbooker;

  static TList* TofactiveHists;
  static HTofHistBooker* Tofhistbooker;

  static TList* TofinoactiveHists;
  static HTofinoHistBooker* Tofinohistbooker;

  static TList* ShoweractiveHists;
  static HShowerHistBooker* Showerhistbooker;
  //-------------------------------

  ClassDef(TGo4Hades,1) //Control class for Hades reconstruction
};	

#endif
