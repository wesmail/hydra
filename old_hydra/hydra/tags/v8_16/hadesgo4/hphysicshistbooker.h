#ifndef HPHYSICSHISTBOOKER_H
#define HPHYSICSHISTBOOKER_H

#include "hparcond.h"

class TList;
class TString;

class HPhysicsHistFiller;
class HParamList;
class HRuntimeDb;

/** Physic Histogram Booker.
 * The container keeps the list of histograms (pool)
 * and the list of activehistograms and the HPhysicHistFiller,
 * which is responsible of booking, creating and filling
 * of the histograms. All actions on the histograms
 * have to through HPhysicHistBooker and HPhysicHistFiller.
*/

class HPhysicsHistBooker : public HParCond {

protected:
    //HStartDetector* fStart; //!
    static TList* physicsHistpool;    //!< list of poool histograms
    static TList* physicsActiveHists; //!< list of active histograms
    static HPhysicsHistBooker* fPhysicsHistBooker;  //!< pointer to self
    static HPhysicsHistFiller* physicsfiller;       //!< pointer to internal HPhysicsHistFiller
public:
    HPhysicsHistBooker(const char* name  ="PhysicsHistBooker",
		       const char* title  ="parameters for Physics hists",
		       const char* context="PhysicsHistBookerProduction");
    ~HPhysicsHistBooker();
    /// get pointer to container
    static HPhysicsHistBooker* getExObject();
    /// get pointer to container , if
    /// it not exists, it will be craeted
    static HPhysicsHistBooker* getObject();
    /// set parameter Io to Go4Io
    void   setGo4ParIo(HRuntimeDb*);
    /// init the contaier using rtdb
    Bool_t init(HParIo*, Int_t*);
    /// init the container standalone (no rtdb)
    Bool_t initStandAlone();
    /// write the container
    Int_t  write(HParIo*);
    /// get list of active histograms
    TList* getListOfActiveHists(){return physicsActiveHists;}
    /// get list of pool histograms
    TList* getListOfHists()      {return physicsHistpool;}
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
    /// get the pointer to the internal HPhysicsHistFiller
    HPhysicsHistFiller* getPhysicsFiller(){return physicsfiller;}
    ClassDef(HPhysicsHistBooker,1) // Container for the Physics Hists
};

#endif  /*!HPHYSICSHISTBOOKER_H*/

