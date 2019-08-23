#ifndef HSTARTHISTBOOKER_H
#define HSTARTHISTBOOKER_H

#include "hparcond.h"

class TList;
class TString;

class HStartHistFiller;
class HStartDetector;
class HParamList;
class HRuntimeDb;

class HStartHistBooker : public HParCond {

protected:
    HStartDetector* fStart; //!
    static TList* startHistpool;    //!
    static TList* startActiveHists; //!
    static HStartHistBooker* fStartHistBooker;  //!
    static HStartHistFiller* startfiller;       //!
public:
    HStartHistBooker(const char* name   ="StartHistBooker",
		     const char* title  ="parameters for Start hists",
		     const char* context="StartHistBookerProduction");
    ~HStartHistBooker();
    static HStartHistBooker* getExObject();
    static HStartHistBooker* getObject();
    void   setGo4ParIo(HRuntimeDb*);
    Bool_t init(HParIo*, Int_t*);
    Bool_t initStandAlone();
    Int_t  write(HParIo*);
    TList* getListOfActiveHists(){return startActiveHists;}
    TList* getListOfHists()      {return startHistpool;}
    void   resetActiveList(Int_t level,Int_t count);
    void   putParams(HParamList*);
    Bool_t getParams(HParamList*);
    void   clear();
    void   printParam(void);
    void   printStatus(void);
    HStartHistFiller* getStartFiller(){return startfiller;}
    ClassDef(HStartHistBooker,1) // Container for the Start Hists
};

#endif  /*!HSTARTHISTBOOKER_H*/

