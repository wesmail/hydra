#ifndef HTOFINOHISTBOOKER_H
#define HTOFINOHISTBOOKER_H

#include "hparcond.h"

class TList;
class TString;

class HTofinoHistFiller;
class HTofinoDetector;
class HParamList;
class HRuntimeDb;

/** Tofino Histogram Booker.
 * The container keeps the list of histograms (pool)
 * and the list of activehistograms and the HTofinoHistFiller,
 * which is responsible of booking, creating and filling
 * of the histograms. All actions on the histograms
 * have to through HTofinoHistBooker and HTofinoHistFiller.
*/

class HTofinoHistBooker : public HParCond {

protected:
    HTofinoDetector* fTofino; //!
    static TList* tofHistpool;    //!< list of pool histograms
    static TList* tofActiveHists; //!< list of active histograms
    static HTofinoHistBooker* fTofinoHistBooker;  //!< pointer to self
    static HTofinoHistFiller* toffiller;       //!< pointer to internal HTofinoHistFiller
public:
    HTofinoHistBooker(const char* name   ="TofinoHistBooker",
		      const char* title  ="parameters for Tofino hists",
		      const char* context="TofinoHistBookerProduction");
    ~HTofinoHistBooker();
    /// get pointer to container
    static HTofinoHistBooker* getExObject();
    /// get pointer to container, if
    /// it does not exist, it will be created
    static HTofinoHistBooker* getObject();
    /// set the parameter Io to Go4Io
    void   setGo4ParIo(HRuntimeDb*);
    /// init the container using rtdb
    Bool_t init(HParIo*, Int_t*);
    /// init the container standalone (no rtdb)
    Bool_t initStandAlone();
    /// write the container
    Int_t  write(HParIo*);
    /// get list of active histograms
    TList* getListOfActiveHists(){return tofActiveHists;}
    /// get list of pool histograms
    TList* getListOfHists()      {return tofHistpool;}
    /// reset histograms of active list
    void   resetActiveList(Int_t level,Int_t count);
    /// put parameter to lsit of HParCond
    void   putParams(HParamList*);
    /// ge parameter from list of HParCond
    Bool_t getParams(HParamList*);
    /// reset the container
    void   clear();
    Bool_t check();
    /// print parameter to the screen
    void   printParam(void);
    /// print status of the container to the screen
    void   printStatus(void);
    /// get pointer t the internal HTofinoHistFiller
    HTofinoHistFiller* getTofinoFiller(){return toffiller;}
    ClassDef(HTofinoHistBooker,1) // Container for the Tofino Hists
};

#endif  /*!HTOFINOHISTBOOKER_H*/

