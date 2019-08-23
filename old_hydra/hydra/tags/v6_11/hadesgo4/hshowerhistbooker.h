#ifndef HSHOWERHISTBOOKER_H
#define HSHOWERHISTBOOKER_H

#include "hparcond.h"

class TList;
class TString;

class HShowerHistFiller;
class HShowerDetector;
class HParamList;
class HRuntimeDb;

class HShowerHistBooker : public HParCond {

protected:
    HShowerDetector* fShower; //!
    static TList* showerHistpool;    //!
    static TList* showerActiveHists; //!
    static HShowerHistBooker* fShowerHistBooker;  //!
    static HShowerHistFiller* showerfiller;       //!
public:
    HShowerHistBooker(const char* name   ="ShowerHistBooker",
		      const char* title  ="parameters for Shower hists",
		      const char* context="ShowerHistBookerProduction");
    ~HShowerHistBooker();
    static HShowerHistBooker* getExObject();
    static HShowerHistBooker* getObject();
    void   setGo4ParIo(HRuntimeDb*);
    Bool_t init(HParIo*, Int_t*);
    Bool_t initStandAlone();
    Int_t  write(HParIo*);
    TList* getListOfActiveHists(){return showerActiveHists;}
    TList* getListOfHists()      {return showerHistpool;}
    void   resetActiveList(Int_t level,Int_t count);
    void   putParams(HParamList*);
    Bool_t getParams(HParamList*);
    void   clear();
    Bool_t check();
    void   printParam(void);
    void   printStatus(void);
    HShowerHistFiller* getShowerFiller(){return showerfiller;}
    ClassDef(HShowerHistBooker,1) // Container for the Shower Hists
};

#endif  /*!HSHOWERHISTBOOKER_H*/

