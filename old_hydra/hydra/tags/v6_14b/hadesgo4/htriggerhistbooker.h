#ifndef HTRIGGERHISTBOOKER_H
#define HTRIGGERHISTBOOKER_H

#include "hparcond.h"

class TList;
class TString;

class HTriggerHistFiller;
class HTriggerDetector;
class HParamList;
class HRuntimeDb;

/** Trigger Histogram Booker.
 * The container keeps the list of histograms (pool)
 * and the list of activehistograms and the HTriggerHistFiller,
 * which is responsible of booking, creating and filling
 * of the histograms. All actions on the histograms
 * have to through HTriggerHistBooker and HTriggerHistFiller.
*/

class HTriggerHistBooker : public HParCond {

protected:
    HTriggerDetector* fTrigger; //!
    static TList* triggerHistpool;    //!< list of pool histograms
    static TList* triggerActiveHists; //!< list of active histograms
    static HTriggerHistBooker* fTriggerHistBooker;  //!< pointer to self
    static HTriggerHistFiller* triggerfiller;       //!< pointer to internal HTriggerHistFiller
public:
    HTriggerHistBooker(const char* name   ="TriggerHistBooker",
		       const char* title  ="parameters for Trigger hists",
		       const char* context="TriggerHistBookerProduction");
    ~HTriggerHistBooker();
    /// get pointer to container
    static HTriggerHistBooker* getExObject();
    /// gt pointer to container, if
    /// it does not exist, it will be created
    static HTriggerHistBooker* getObject();
    /// set parameter Io to Go4Io
    void   setGo4ParIo(HRuntimeDb*);
    /// init container using rtdb
    Bool_t init(HParIo*, Int_t*);
    /// init container standalone (no rtdb)
    Bool_t initStandAlone();
    /// write container
    Int_t  write(HParIo*);
    /// get list of active histograms
    TList* getListOfActiveHists(){return triggerActiveHists;}
    /// get list of pool histograms
    TList* getListOfHists()      {return triggerHistpool;}
    /// reset histograms of active list
    void   resetActiveList(Int_t level,Int_t count);
    /// put parameter to list of HParCond
    void   putParams(HParamList*);
    /// get parameter from list of HParCond
    Bool_t getParams(HParamList*);
    /// reset the container
    void   clear();
    /// print parametr to the screen
    void   printParam(void);
    /// print status of the container to the screen
    void   printStatus(void);
    /// get pointer to internal HTriggerHistFiller
    HTriggerHistFiller* getTriggerFiller(){return triggerfiller;}
    /// write histograms to .root file
    Bool_t writeHists( const TString &option );

    ClassDef(HTriggerHistBooker,1) // Container for the Trigger Hists
};

#endif  /*!HTRIGGERHISTBOOKER_H*/

