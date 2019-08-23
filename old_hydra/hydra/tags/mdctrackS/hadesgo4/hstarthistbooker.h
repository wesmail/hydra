#ifndef HSTARTHISTBOOKER_H
#define HSTARTHISTBOOKER_H

#include "hparcond.h"

class TList;
class TString;

class HStartHistFiller;
class HStartDetector;
class HParamList;
class HRuntimeDb;

/** Start Histogram Booker.
 * The container keeps the list of histograms (pool)
 * and the list of activehistograms and the HStartHistFiller,
 * which is responsible of booking, creating and filling
 * of the histograms. All actions on the histograms
 * have to through HStartHistBooker and HStartHistFiller.
*/

class HStartHistBooker : public HParCond {

protected:
    HStartDetector* fStart; //!
    static TList* startHistpool;    //!< list of poool histograms
    static TList* startActiveHists; //!< list of active histograms
    static HStartHistBooker* fStartHistBooker;  //!< pointer to self
    static HStartHistFiller* startfiller;       //!< pointer to internal HStartHistFiller
public:
    HStartHistBooker(const char* name   ="StartHistBooker",
		     const char* title  ="parameters for Start hists",
		     const char* context="StartHistBookerProduction");
    ~HStartHistBooker();
    /// get pointer to container
    static HStartHistBooker* getExObject();
    /// get pointer to container , if
    /// it not exists, it will be craeted
    static HStartHistBooker* getObject();
    /// set parameter Io to Go4Io
    void   setGo4ParIo(HRuntimeDb*);
    /// init the contaier using rtdb
    Bool_t init(HParIo*, Int_t*);
    /// init the container standalone (no rtdb)
    Bool_t initStandAlone();
    /// write the container
    Int_t  write(HParIo*);
    /// get list of active histograms
    TList* getListOfActiveHists(){return startActiveHists;}
    /// get list of pool histograms
    TList* getListOfHists()      {return startHistpool;}
    /// reset histograms on active list
    void   resetActiveList(Int_t level,Int_t count);
    /// put parameter to the list of HParCond
    void   putParams(HParamList*);
    /// get parameter from the list of HParCond
    Bool_t getParams(HParamList*);
    /// reset the container
    void   clear();
    /// print the parameter to the screen
    void   printParam(void);
    /// print nthe status of the container to the screen
    void   printStatus(void);
    /// get the pointer to the internal HStartHistFiller
    HStartHistFiller* getStartFiller(){return startfiller;}
    ClassDef(HStartHistBooker,1) // Container for the Start Hists
};

#endif  /*!HSTARTHISTBOOKER_H*/

