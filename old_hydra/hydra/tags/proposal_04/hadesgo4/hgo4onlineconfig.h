#ifndef HGO4ONLINECONFIG_H
#define HGO4ONLINECONFIG_H

#include "hparcond.h"
#include "TArrayI.h"

class TList;
class TString;

class HMdcDetector;
class HParamList;
class HRuntimeDb;
class HGo4OnlineParam;

/**Configuration object for TGo4Hades.
*/
class HGo4OnlineParam : public TNamed {
protected:
    Int_t nLoop;           ///< number events in Hades event loop
    Int_t sleep;           ///< number of miliseconds to sleep
    Int_t maxrate;         ///< maximum eventrate
    Int_t triggerrefresh;  ///< refreshrate of trigger
    Int_t startrefresh;    ///< refreshrate of start
    Int_t richrefresh;     ///< refreshrate of rich
    Int_t richODrefresh;   ///< Oneline Display refreshrate of rich
    Int_t mdcrefresh;      ///< refreshrate of mdc
    Int_t tofrefresh;      ///< refreshrate of tof
    Int_t tofinorefresh;   ///< refreshrate of tofino
    Int_t showerrefresh;   ///< refreshrate of shower
    Int_t physicsrefresh;  ///< refreshrate of physics
    Int_t trigger;         ///< switch trigger on/off
    Int_t start;           ///< switch start on/off
    Int_t startRaw;        ///< switch start raw on/off
    Int_t startCal;        ///< switch start cal on/off
    Int_t startHit;        ///< switch start hit on/off
    TArrayI startdetsetup; ///< switch start detector setup
    Int_t rich;            ///< switch rich on/off
    Int_t richRaw;         ///< switch rich raw on/off
    Int_t richCal;         ///< switch rich cal on/off
    Int_t richHit;         ///< switch rich hit on/off
    Int_t mdc;             ///< switch mdc on/off
    Int_t mdcLevel;        ///< switch mdc analysis level
    TArrayI mdcdetsetup;   ///< switch mdc detector setup
    Int_t tof;             ///< switch tof on/off
    Int_t tofRaw;          ///< switch tof raw on/off
    Int_t tofCal;          ///< switch tof cal on/off
    Int_t tofHit;          ///< switch tof hit on/off
    Int_t tofino;          ///< switch tofino on/off
    Int_t tofinoRaw;       ///< switch tofino raw on/off
    Int_t tofinoCal;       ///< switch tofino cal on/off
    Int_t showertofino;    ///< switch showertofino cal on/off
    Int_t shower;          ///< switch shower on/off
    Int_t showerRaw;       ///< switch shower raw on/off
    Int_t showerCal;       ///< switch shower cal on/off
    Int_t showerHit;       ///< switch shower hit on/off
    Int_t physics;         ///< switch physics on/off
public:
    HGo4OnlineParam(const char* name   ="",
		    const char* title  ="");
    ~HGo4OnlineParam();
    void   setTriggerRefresh(Int_t i){triggerrefresh=i;}
    Int_t  getTriggerRefresh()       {return triggerrefresh;}
    void   setStartRefresh(Int_t i)  {startrefresh=i;}
    Int_t  getStartRefresh()         {return startrefresh;}
    void   setRichRefresh(Int_t i)   {richrefresh=i;}
    Int_t  getRichRefresh()          {return richrefresh;}
    void   setRichODRefresh(Int_t i) {richODrefresh=i;}
    Int_t  getRichODRefresh()        {return richODrefresh;}
    void   setMdcRefresh(Int_t i)    {mdcrefresh=i;}
    Int_t  getMdcRefresh()           {return mdcrefresh;}
    void   setTofRefresh(Int_t i)    {tofrefresh=i;}
    Int_t  getTofRefresh()           {return tofrefresh;}
    void   setTofinoRefresh(Int_t i) {tofinorefresh=i;}
    Int_t  getTofinoRefresh()        {return tofinorefresh;}
    void   setShowerRefresh(Int_t i) {showerrefresh=i;}
    Int_t  getShowerRefresh()        {return showerrefresh;}
    void   setPhysicsRefresh(Int_t i){physicsrefresh=i;}
    Int_t  getPhysicsRefresh()       {return physicsrefresh;}
    void   setNLoop(Int_t i)       {nLoop=i;}
    Int_t  getNLoop()              {return nLoop;}
    void   setSleep(Int_t i)       {sleep=i;}
    Int_t  getSleep()              {return sleep;}
    void   setMaxRate(Int_t i)     {maxrate=i;}
    Int_t  getMaxRate()            {return maxrate;}
    void   setTrigger(Int_t i)     {trigger=i;}
    Int_t  getTrigger()            {return trigger;}
    void   setStart(Int_t i)       {start=i;}
    Int_t  getStart()              {return start;}
    void   setStartRaw(Int_t i)    {startRaw=i;}
    Int_t  getStartRaw()           {return startRaw;}
    void   setStartCal(Int_t i)    {startCal=i;}
    Int_t  getStartCal()           {return startCal;}
    void   setStartHit(Int_t i)    {startHit=i;}
    Int_t  getStartHit()           {return startHit;}
    TArrayI* getStartDetSetup()    {return &startdetsetup;}
    /// copy startdetsetup to the HStartDetector
    void   setupStartDetector();
    // compare actual HStartDetector setup to parameter settings
    Bool_t setupStartDetectorHasChanged();
    void   setRich(Int_t i)        {rich=i;}
    Int_t  getRich()               {return rich;}
    void   setRichRaw(Int_t i)     {richRaw=i;}
    Int_t  getRichRaw()            {return richRaw;}
    void   setRichCal(Int_t i)     {richCal=i;}
    Int_t  getRichCal()            {return richCal;}
    void   setRichHit(Int_t i)     {richHit=i;}
    Int_t  getRichHit()            {return richHit;}
    void   setMdc(Int_t i)         {mdc=i;}
    Int_t  getMdc()                {return mdc;}
    void   setMdcLevel(Int_t i)    {mdcLevel=i;}
    Int_t  getMdcLevel()           {return mdcLevel;}
    void   setMdcDetSetup(TArrayI& array) {mdcdetsetup.Copy(array);}
    TArrayI* getMdcDetSetup()      {return &mdcdetsetup;}
    /// copy mdcdetsetup to the HMdcDetector
    void   setupMdcDetector();
    // compare actual HMdcDetector setup to parameter settings
    Bool_t setupMdcDetectorHasChanged();
    void   setTof(Int_t i)         {tof=i;}
    Int_t  getTof()                {return tof;}
    void   setTofRaw(Int_t i)      {tofRaw=i;}
    Int_t  getTofRaw()             {return tofRaw;}
    void   setTofCal(Int_t i)      {tofCal=i;}
    Int_t  getTofCal()             {return tofCal;}
    void   setTofHit(Int_t i)      {tofHit=i;}
    Int_t  getTofHit()             {return tofHit;}
    void   setTofino(Int_t i)      {tofino=i;}
    Int_t  getTofino()             {return tofino;}
    void   setTofinoRaw(Int_t i)   {tofinoRaw=i;}
    Int_t  getTofinoRaw()          {return tofinoRaw;}
    void   setTofinoCal(Int_t i)   {tofinoCal=i;}
    Int_t  getTofinoCal()          {return tofinoCal;}
    void   setShowerTofino(Int_t i){showertofino=i;}
    Int_t  getShowerTofino()       {return showertofino;}
    void   setShower(Int_t i)      {shower=i;}
    Int_t  getShower()             {return shower;}
    void   setShowerRaw(Int_t i)   {showerRaw=i;}
    Int_t  getShowerRaw()          {return showerRaw;}
    void   setShowerCal(Int_t i)   {showerCal=i;}
    Int_t  getShowerCal()          {return showerCal;}
    void   setShowerHit(Int_t i)   {showerHit=i;}
    Int_t  getShowerHit()          {return showerHit;}
    void   setPhysics(Int_t i)     {physics=i;}
    Int_t  getPhysics()            {return physics;}

    /// Calculate max level of trigger
    Int_t  getTriggerMaxLevel() {return trigger;}
    /// set max level of trigger
    void   setTriggerMaxLevel(Int_t i){trigger=i;}
    /// Calculate max level of start
    Int_t  getStartMaxLevel()
    {
	if(startHit==1)     return 3;
	else if(startCal==1)return 2;
	else if(startRaw==1)return 1;
	else                return 0;
    }
    /// set max level of start
    void   setStartMaxLevel(Int_t i)
    {
	if(i==3)     {startHit=startCal=startRaw=1;}
	else if(i==2){startHit=0;startCal=startRaw=1;}
	else if(i==1){startHit=startCal=0;startRaw=1;}
	else if(i==0){startHit=startCal=startRaw=start=0;}
    }
    /// Calculate max level of rich
    Int_t  getRichMaxLevel()
    {
	if(richHit==1)     return 3;
	else if(richCal==1)return 2;
	else if(richRaw==1)return 1;
	else               return 0;
    }
    /// Calculate max level of rich
    void   setRichMaxLevel(Int_t i)
    {
	if(i==3)     {richHit=richCal=richRaw=1;}
	else if(i==2){richHit=0;richCal=richRaw=1;}
	else if(i==1){richHit=richCal=0;richRaw=1;}
	else if(i==0){richHit=richCal=richRaw=rich=0;}
    }
    /// Calculate max level of mdc
    Int_t  getMdcMaxLevel() {return mdcLevel;}
    /// set max level of mdc
    void   setMdcMaxLevel(Int_t i) {mdcLevel=i;}
    /// Calculate max level of tof
    Int_t  getTofMaxLevel()
    {
	if(tofHit==1)return 3;
	if(tofCal==1)return 2;
	if(tofRaw==1)return 1;
	else         return 0;
    }
   /// set max level of tof
    void   setTofMaxLevel(Int_t i)
    {
	if(i==3)     {tofHit=tofCal=tofRaw=1;}
	else if(i==2){tofHit=0;tofCal=tofRaw=1;}
	else if(i==1){tofHit=tofCal=0;tofRaw=1;}
	else if(i==0){tofHit=tofCal=tofRaw=tof=0;}
    }
    /// Calculate max level of tofino
    Int_t  getTofinoMaxLevel()
    {
	if(tofinoCal==1)return 2;
	if(tofinoRaw==1)return 1;
	else            return 0;
    }
    /// set max level of tofino
    void   setTofinoMaxLevel(Int_t i)
    {
	if(i==2)     {tofinoCal=tofinoRaw=1;}
	else if(i==1){tofinoCal=0;tofinoRaw=1;}
	else if(i==0){tofinoCal=tofinoRaw=tofino=0;}
    }
    /// Calculate max level of showertofino
    Int_t  getShowerTofinoMaxLevel() {return showertofino;}
    /// set max level of showertofino
    void   setTofinoShowerMaxLevel(Int_t i){showertofino=i;}
    /// set max level of shower
    void   setShowerMaxLevel(Int_t i)
    {
	if(i==3)     {showerHit=showerCal=showerRaw=1;}
	else if(i==2){showerHit=0;showerCal=showerRaw=1;}
	else if(i==1){showerHit=showerCal=0;showerRaw=1;}
	else if(i==0){showerHit=showerCal=showerRaw=shower=0;}
    }
    /// Calculate max level of shower
    Int_t  getShowerMaxLevel()
    {
	if(showerHit==1)return 3;
	if(showerCal==1)return 2;
	if(showerRaw==1)return 1;
	else            return 0;
    }
    /// put parameters to the List of HParCond
    void   putParams(HParamList*);
    /// get parameters from the List of HParCond
    Bool_t getParams(HParamList*);
    /// reset the container
    void   clear();
    /// print the parameters of the container to the screen
    void   printParam(void);
    /// print the status of the container to the screen
    void   printStatus(void);
    /// copy all data members from an other object
    void   copySet(HGo4OnlineParam*);
    /// compare with other object,
    /// returns kTRUE if equal and does not contain default init values
    Bool_t isEqual(HGo4OnlineParam*,Int_t det=0);
    ClassDef(HGo4OnlineParam,1) // Container object for configuration parameters
};

/**Configuration container for TGo4Hades.
* Keeps a HGo4OnlineParam object which holds the data
*/
class HGo4OnlineConfig : public HParCond {

protected:
    HMdcDetector* fMdc; //!
    static HGo4OnlineConfig* fGo4OnlineConfig;  //! pointer to own
    HGo4OnlineParam* param; ///< pointer to HGo4OnlineParam config object

public:
    HGo4OnlineConfig(const char* name   ="Go4OnlineConfig",
		     const char* title  ="parameters for Go4 online configuration",
		     const char* context="Go4OnlineConfigProduction");
    ~HGo4OnlineConfig();
    /// get a pointer to the container.
    static HGo4OnlineConfig* getExObject();
    /// get a pointer to the container.
    /// if the container does not exists, it will be created.
    static HGo4OnlineConfig* getObject();
    /// set the parameter Io to Go4 Io
    void   setGo4ParIo(HRuntimeDb*);
    /// init the container using rtdb
    Bool_t init(HParIo*, Int_t*);
    /// init the container standalone (not in rtdb)
    Bool_t initStandAlone();
    /// write this container to parameter Io
    Int_t  write(HParIo*);
    /// put parameter to the list of HParCond
    void   putParams(HParamList*);
    /// get the parameter from the list of HParCond
    Bool_t getParams(HParamList*);
    /// rest the object
    void   clear();
    /// print the parameter to the screen
    void   printParam(void);
    /// print the status to the screen
    void   printStatus(void);
    /// get the pointer to the internal HGo4OnlineParam object
    HGo4OnlineParam* getGo4OnlineParam(){return param;}
    ClassDef(HGo4OnlineConfig,1) // Container for configuration parameters
};

#endif  /*!HGO4ONLINECONFIG_H*/

