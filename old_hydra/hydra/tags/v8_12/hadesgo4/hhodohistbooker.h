#ifndef HHODOHISTBOOKER_H
#define HHODOHISTBOOKER_H

#include "hparcond.h"

class TList;
class TString;

class HHodoHistFiller;
class HHodoDetector;
class HParamList;
class HRuntimeDb;

/** Hodo Histogram Booker.
 * The container keeps the list of histograms (pool)
 * and the list of activehistograms and the HHodoHistFiller,
 * which is responsible of booking, creating and filling
 * of the histograms. All actions on the histograms
 * have to through HHodoHistBooker and HHodoHistFiller.
*/

class HHodoHistBooker : public HParCond {

protected:
    HHodoDetector* fHodo; //!
    static TList* hodoHistpool;    //!< list of pool histograms
    static TList* hodoActiveHists; //!< list of active histograms
    static HHodoHistBooker* fHodoHistBooker;  //!< pointer to self
    static HHodoHistFiller* hodofiller;       //!< pointer to internal HHodoHistFiller
public:
    HHodoHistBooker(const char* name   ="HodoHistBooker",
		      const char* title  ="parameters for Hodo hists",
		      const char* context="HodoHistBookerProduction");
    ~HHodoHistBooker();
    /// get pointer to container
    static HHodoHistBooker* getExObject();
    /// get pointer to container, if
    /// it does not exist, it will be created
    static HHodoHistBooker* getObject();
    /// set the parameter Io to Go4Io
    void   setGo4ParIo(HRuntimeDb*);
    /// init the container using rtdb
    Bool_t init(HParIo*, Int_t*);
    /// init the container standalone (no rtdb)
    Bool_t initStandAlone();
    /// write the container
    Int_t  write(HParIo*);
    /// get list of active histograms
    TList* getListOfActiveHists(){return hodoActiveHists;}
    /// get list of pool histograms
    TList* getListOfHists()      {return hodoHistpool;}
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
    /// get pointer t the internal HHodoHistFiller
    HHodoHistFiller* getHodoFiller(){return hodofiller;}
    ClassDef(HHodoHistBooker,1) // Container for the Hodo Hists
};

#endif  /*!HHODOHISTBOOKER_H*/

