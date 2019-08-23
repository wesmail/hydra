#ifndef HSHOWERHISTBOOKER_H
#define HSHOWERHISTBOOKER_H

#include "hparcond.h"

class TList;
class TString;

class HShowerHistFiller;
class HShowerDetector;
class HParamList;
class HRuntimeDb;

/** Shower Histogram Booker.
 * The container keeps the list of histograms (pool)
 * and the list of activehistograms and the HShowerHistFiller,
 * which is responsible of booking, creating and filling
 * of the histograms. All actions on the histograms
 * have to through HShowerHistBooker and HShowerHistFiller.
*/

class HShowerHistBooker : public HParCond {

protected:
    HShowerDetector* fShower; //!
    static TList* showerHistpool;    //!< list of histograms in pool
    static TList* showerActiveHists; //!< list of active histograms
    static HShowerHistBooker* fShowerHistBooker;  //!< pointer to HShowerHistBooker
    static HShowerHistFiller* showerfiller;       //!< pointer to internal HShowerHistFiller
public:
    HShowerHistBooker(const Char_t* name   ="ShowerHistBooker",
		      const Char_t* title  ="parameters for Shower hists",
		      const Char_t* context="ShowerHistBookerProduction");
    ~HShowerHistBooker();
    /// get pointer to the container
    static HShowerHistBooker* getExObject();
    /// get pointer to the container,
    /// if it does not exist , it will be created
    static HShowerHistBooker* getObject();
    /// set the parameter Io to Go4Io
    void   setGo4ParIo(HRuntimeDb*);
    /// init the container using rtdb
    Bool_t init(HParIo*, Int_t*);
    /// init the container standalone (no rtdb)
    Bool_t initStandAlone();
    /// write the container
    Int_t  write(HParIo*);
    /// get list of active histograms
    TList* getListOfActiveHists(){return showerActiveHists;}
    /// get list of histograms in pool
    TList* getListOfHists()      {return showerHistpool;}
    /// reset histograms on active list
    void   resetActiveList(Int_t level,Int_t count);
    /// put parameters to the list of HParCond
    void   putParams(HParamList*);
    /// gt parameters from list of HParCond
    Bool_t getParams(HParamList*);
    /// reset the container
    void   clear();
    Bool_t check();
    /// print the paramters to the screen
    void   printParam(void);
    /// print the status to the screen
    void   printStatus(void);
    /// get the pointer to the internal HShowerHistFiller
    HShowerHistFiller* getShowerFiller(){return showerfiller;}
    ClassDef(HShowerHistBooker,1) // Container for the Shower Hists
};

#endif  /*!HSHOWERHISTBOOKER_H*/

