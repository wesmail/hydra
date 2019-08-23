#ifndef HMDCHISTBOOKER_H
#define HMDCHISTBOOKER_H

#include "hparcond.h"

class TList;
class TString;

class HMdcHistFiller;
class HMdcDetector;
class HParamList;
class HRuntimeDb;

class HMdcHistBooker : public HParCond {

protected:
    HMdcDetector* fMdc; //!
    static TList* mdcHistpool;    //!
    static TList* mdcActiveHists; //!
    static HMdcHistBooker* fMdcHistBooker;  //!
    static HMdcHistFiller* mdcfiller;       //!
public:
    HMdcHistBooker(const char* name   ="MdcHistBooker",
		   const char* title  ="parameters for Mdc hists",
		   const char* context="MdcHistBookerProduction");
    ~HMdcHistBooker();
    static HMdcHistBooker* getExObject();
    static HMdcHistBooker* getObject();
    void   setGo4ParIo(HRuntimeDb*);
    Bool_t init(HParIo*, Int_t*);
    Bool_t initStandAlone();
    Int_t  write(HParIo*);
    TList* getListOfActiveHists(){return mdcActiveHists;}
    TList* getListOfHists()      {return mdcHistpool;}
    void   resetActiveList(Int_t level,Int_t count);
    void   putParams(HParamList*);
    Bool_t getParams(HParamList*);
    void   clear();
    void   printParam(void);
    void   printStatus(void);
    HMdcHistFiller* getMdcFiller(){return mdcfiller;}
    ClassDef(HMdcHistBooker,1) // Container for the MDC Hists
};

#endif  /*!HMDCHISTBOOKER_H*/

