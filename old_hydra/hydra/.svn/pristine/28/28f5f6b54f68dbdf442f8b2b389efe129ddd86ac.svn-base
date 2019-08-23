#ifndef HGO4ONLINECONFIG_H
#define HGO4ONLINECONFIG_H

#include "hparcond.h"

class TList;
class TString;

class HMdcDetector;
class HParamList;
class HRuntimeDb;
class HGo4OnlineParam;


class HGo4OnlineParam : public TNamed {
protected:
    Int_t nLoop;
    Int_t triggerrefresh;
    Int_t startrefresh;
    Int_t richrefresh;
    Int_t mdcrefresh;
    Int_t tofrefresh;
    Int_t tofinorefresh;
    Int_t showerrefresh;
    Int_t trigger;
    Int_t start;
    Int_t startRaw;
    Int_t startCal;
    Int_t startHit;
    Int_t rich;
    Int_t richRaw;
    Int_t richCal;
    Int_t richHit;
    Int_t mdc;
    Int_t mdcRaw;
    Int_t mdcCal;
    Int_t mdcHit;
    Int_t tof;
    Int_t tofRaw;
    Int_t tofCal;
    Int_t tofHit;
    Int_t tofino;
    Int_t tofinoRaw;
    Int_t tofinoCal;
    Int_t shower;
    Int_t showerRaw;
    Int_t showerCal;
    Int_t showerHit;

public:
    HGo4OnlineParam(const char* name   ="",
		   const char* title  ="");
    ~HGo4OnlineParam();
    void   setTriggerRefresh(Int_t i){triggerrefresh=i;}
    Int_t  getTriggerRefresh()       {return triggerrefresh;}
    void   setStartRefresh(Int_t i)  {startrefresh=i;}
    Int_t  getStartRefresh()         {return startrefresh;}
    void   setRichRefresh(Int_t i) {richrefresh=i;}
    Int_t  getRichRefresh()        {return richrefresh;}
    void   setMdcRefresh(Int_t i)  {mdcrefresh=i;}
    Int_t  getMdcRefresh()         {return mdcrefresh;}
    void   setTofRefresh(Int_t i)  {tofrefresh=i;}
    Int_t  getTofRefresh()         {return tofrefresh;}
    void   setTofinoRefresh(Int_t i){tofinorefresh=i;}
    Int_t  getTofinoRefresh()       {return tofinorefresh;}
    void   setShowerRefresh(Int_t i){showerrefresh=i;}
    Int_t  getShowerRefresh()       {return showerrefresh;}
    void   setNLoop(Int_t i)       {nLoop=i;}
    Int_t  getNLoop()              {return nLoop;}
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
    void   setMdcRaw(Int_t i)      {mdcRaw=i;}
    Int_t  getMdcRaw()             {return mdcRaw;}
    void   setMdcCal(Int_t i)      {mdcCal=i;}
    Int_t  getMdcCal()             {return mdcCal;}
    void   setMdcHit(Int_t i)      {mdcHit=i;}
    Int_t  getMdcHit()             {return mdcHit;}
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
    void   setShower(Int_t i)      {shower=i;}
    Int_t  getShower()             {return shower;}
    void   setShowerRaw(Int_t i)   {showerRaw=i;}
    Int_t  getShowerRaw()          {return showerRaw;}
    void   setShowerCal(Int_t i)   {showerCal=i;}
    Int_t  getShowerCal()          {return showerCal;}
    void   setShowerHit(Int_t i)   {showerHit=i;}
    Int_t  getShowerHit()          {return showerHit;}
    void   putParams(HParamList*);
    Bool_t getParams(HParamList*);
    void   clear();
    void   printParam(void);
    void   printStatus(void);
    ClassDef(HGo4OnlineParam,1) // Container object for configuration parameters
};



class HGo4OnlineConfig : public HParCond {

protected:
    HMdcDetector* fMdc; //!
    static HGo4OnlineConfig* fGo4OnlineConfig;  //!
    HGo4OnlineParam* param;

public:
    HGo4OnlineConfig(const char* name   ="Go4OnlineConfig",
		     const char* title  ="parameters for Go4 online configuration",
		     const char* context="Go4OnlineConfigProduction");
    ~HGo4OnlineConfig();
    static HGo4OnlineConfig* getExObject();
    static HGo4OnlineConfig* getObject();
    void   setGo4ParIo(HRuntimeDb*);
    Bool_t init(HParIo*, Int_t*);
    Bool_t initStandAlone();
    Int_t  write(HParIo*);
    void   putParams(HParamList*);
    Bool_t getParams(HParamList*);
    void   clear();
    void   printParam(void);
    void   printStatus(void);
    HGo4OnlineParam* getGo4OnlineParam(){return param;}
    ClassDef(HGo4OnlineConfig,1) // Container for configuration parameters
};

#endif  /*!HGO4ONLINECONFIG_H*/

