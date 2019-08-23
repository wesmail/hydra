#ifndef HWALLHISTBOOKER_H
#define HWALLHISTBOOKER_H

#include "hparcond.h"

class TList;
class TString;

class HWallHistFiller;
class HWallDetector;
class HParamList;
class HRuntimeDb;

/** Wall Histogram Booker.
 * The container keeps the list of histograms (pool)
 * and the list of activehistograms and the HWallHistFiller,
 * which is responsible of booking, creating and filling
 * of the histograms. All actions on the histograms
 * have to through HWallHistBooker and HWallHistFiller.
*/

class HWallHistBooker : public HParCond {

protected:
    HWallDetector* fWall; //!
    static TList* wallHistpool;    //!< list of pool histograms
    static TList* wallActiveHists; //!< list of active histograms
    static HWallHistBooker* fWallHistBooker;  //!< pointer to self
    static HWallHistFiller* wallfiller;       //!< pointer to internal HWallHistFiller
public:
    HWallHistBooker(const char* name   ="WallHistBooker",
		      const char* title  ="parameters for Wall hists",
		      const char* context="WallHistBookerProduction");
    ~HWallHistBooker();
    /// get pointer to container
    static HWallHistBooker* getExObject();
    /// get pointer to container, if
    /// it does not exist, it will be created
    static HWallHistBooker* getObject();
    /// set the parameter Io to Go4Io
    void   setGo4ParIo(HRuntimeDb*);
    /// init the container using rtdb
    Bool_t init(HParIo*, Int_t*);
    /// init the container standalone (no rtdb)
    Bool_t initStandAlone();
    /// write the container
    Int_t  write(HParIo*);
    /// get list of active histograms
    TList* getListOfActiveHists(){return wallActiveHists;}
    /// get list of pool histograms
    TList* getListOfHists()      {return wallHistpool;}
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
    /// get pointer t the internal HWallHistFiller
    HWallHistFiller* getWallFiller(){return wallfiller;}
    ClassDef(HWallHistBooker,1) // Container for the Wall Hists
};

#endif  /*!HWALLHISTBOOKER_H*/

