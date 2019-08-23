#ifndef HTOFINOHISTBOOKER_H
#define HTOFINOHISTBOOKER_H

#include "hparcond.h"

class TList;
class TString;

class HTofinoHistFiller;
class HTofinoDetector;
class HParamList;
class HRuntimeDb;

class HTofinoHistBooker : public HParCond {

protected:
    HTofinoDetector* fTofino; //!
    static TList* tofHistpool;    //!
    static TList* tofActiveHists; //!
    static HTofinoHistBooker* fTofinoHistBooker;  //!
    static HTofinoHistFiller* toffiller;       //!
public:
    HTofinoHistBooker(const char* name   ="TofinoHistBooker",
		      const char* title  ="parameters for Tofino hists",
		      const char* context="TofinoHistBookerProduction");
    ~HTofinoHistBooker();
    static HTofinoHistBooker* getExObject();
    static HTofinoHistBooker* getObject();
    void   setGo4ParIo(HRuntimeDb*);
    Bool_t init(HParIo*, Int_t*);
    Bool_t initStandAlone();
    Int_t  write(HParIo*);
    TList* getListOfActiveHists(){return tofActiveHists;}
    TList* getListOfHists()      {return tofHistpool;}
    void   resetActiveList(Int_t level,Int_t count);
    void   putParams(HParamList*);
    Bool_t getParams(HParamList*);
    void   clear();
    Bool_t check();
    void   printParam(void);
    void   printStatus(void);
    HTofinoHistFiller* getTofinoFiller(){return toffiller;}
    ClassDef(HTofinoHistBooker,1) // Container for the Tofino Hists
};

#endif  /*!HTOFINOHISTBOOKER_H*/

