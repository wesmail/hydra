#ifndef HRICHHISTBOOKER_H
#define HRICHHISTBOOKER_H

#include "hparcond.h"

class TList;
class TString;

class HRichHistFiller;
class HRichDetector;
class HParamList;
class HRuntimeDb;

class HRichHistBooker : public HParCond {

protected:
    HRichDetector* fRich; //!
    static TList* richHistpool;    //!
    static TList* richActiveHists; //!
    static HRichHistBooker* fRichHistBooker;  //!
    static HRichHistFiller* richfiller;       //!
public:
    HRichHistBooker(const char* name   ="RichHistBooker",
		    const char* title  ="parameters for Rich hists",
		    const char* context="RichHistBookerProduction");
    ~HRichHistBooker();
    static HRichHistBooker* getExObject();
    static HRichHistBooker* getObject();
    void   setGo4ParIo(HRuntimeDb*);
    Bool_t init(HParIo*, Int_t*);
    Bool_t initStandAlone();
    Int_t  write(HParIo*);
    TList* getListOfActiveHists(){return richActiveHists;}
    TList* getListOfHists()      {return richHistpool;}
    void   resetActiveList(Int_t level,Int_t count);
    void   putParams(HParamList*);
    Bool_t getParams(HParamList*);
    void   clear();
    void   printParam(void);
    void   printStatus(void);
    HRichHistFiller* getRichFiller(){return richfiller;}
    ClassDef(HRichHistBooker,1) // Container for the Rich Hists
};

#endif  /*!HRICHHISTBOOKER_H*/

