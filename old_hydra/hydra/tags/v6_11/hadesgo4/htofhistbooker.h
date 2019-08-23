#ifndef HTOFHISTBOOKER_H
#define HTOFHISTBOOKER_H

#include "hparcond.h"

class TList;
class TString;

class HTofHistFiller;
class HTofDetector;
class HParamList;
class HRuntimeDb;

class HTofHistBooker : public HParCond {

protected:
    HTofDetector* fTof; //!
    static TList* tofHistpool;    //!
    static TList* tofActiveHists; //!
    static HTofHistBooker* fTofHistBooker;  //!
    static HTofHistFiller* toffiller;       //!
public:
    HTofHistBooker(const char* name   ="TofHistBooker",
		   const char* title  ="parameters for Tof hists",
		   const char* context="TofHistBookerProduction");
    ~HTofHistBooker();
    static HTofHistBooker* getExObject();
    static HTofHistBooker* getObject();
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
    HTofHistFiller* getTofFiller(){return toffiller;}
    ClassDef(HTofHistBooker,1) // Container for the Tof Hists
};

#endif  /*!HTOFHISTBOOKER_H*/

