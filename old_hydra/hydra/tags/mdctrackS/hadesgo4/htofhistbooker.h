#ifndef HTOFHISTBOOKER_H
#define HTOFHISTBOOKER_H

#include "hparcond.h"

class TList;
class TString;

class HTofHistFiller;
class HTofDetector;
class HParamList;
class HRuntimeDb;

/** Toff Histogram Booker.
 * The container keeps the list of histograms (pool)
 * and the list of active histograms and the HTofHistFiller,
 * which is responsible of booking, creating and filling
 * of the histograms. All actions on the histograms
 * have to through HTofHistBooker and HTofHistFiller.
*/

class HTofHistBooker : public HParCond {

protected:
    HTofDetector* fTof; //!
    static TList* tofHistpool;    //!< list of pool histograms
    static TList* tofActiveHists; //!< list of active histograms
    static HTofHistBooker* fTofHistBooker;  //!< pointer to self
    static HTofHistFiller* toffiller;       //!< pointer to internal TofHistFiller
public:
    HTofHistBooker(const char* name   ="TofHistBooker",
		   const char* title  ="parameters for Tof hists",
		   const char* context="TofHistBookerProduction");
    ~HTofHistBooker();
    /// get pointer to container
    static HTofHistBooker* getExObject();
    /// get pointer to conainer, if
    /// it does not exist , it will be created
    static HTofHistBooker* getObject();
    /// set parameter Io to Go4Io
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
    /// reset histogram on active list
    void   resetActiveList(Int_t level,Int_t count);
    /// put parameter to list of HParCond
    void   putParams(HParamList*);
    /// get parameter from list of HParCond
    Bool_t getParams(HParamList*);
    /// reset the container
    void   clear();
    Bool_t check();
    /// print parameter to the screen
    void   printParam(void);
    /// print status of the conatiner to the screen
    void   printStatus(void);
    /// get pointer to internal HTofHistFiller
    HTofHistFiller* getTofFiller(){return toffiller;}
    ClassDef(HTofHistBooker,1) // Container for the Tof Hists
};

#endif  /*!HTOFHISTBOOKER_H*/

