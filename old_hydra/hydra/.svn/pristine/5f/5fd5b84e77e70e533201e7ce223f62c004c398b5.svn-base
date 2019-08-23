#ifndef HMDCHISTBOOKER_H
#define HMDCHISTBOOKER_H

#include "hparcond.h"

class TList;
class TString;

class HMdcHistFiller;
class HMdcDetector;
class HParamList;
class HRuntimeDb;

/** Mdc Histogram Booker.
 * The container keeps the list of histograms (pool)
 * and the list of activehistograms and the HMdcHistFiller,
 * which is responsible of booking, creating and filling
 * of the histograms. All actions on the histograms
 * have to through HMdcHistBooker and HMdcHistFiller.
*/
class HMdcHistBooker : public HParCond {

protected:
    HMdcDetector* fMdc; //!
    static TList* mdcHistpool;    //!< pointer to histogram pool list
    static TList* mdcActiveHists; //!< pointer to histogram active list
    static HMdcHistBooker* fMdcHistBooker;  //!< pointer to self
    static HMdcHistFiller* mdcfiller;       //!< pointer to internal HMdcHistFiller
public:
    HMdcHistBooker(const Char_t* name   ="MdcHistBooker",
		   const Char_t* title  ="parameters for Mdc hists",
		   const Char_t* context="MdcHistBookerProduction");
    ~HMdcHistBooker();
    /// get pointer to container
    static HMdcHistBooker* getExObject();
    /// get pointer to container, if
    /// the container does not exist it will be created
    static HMdcHistBooker* getObject();
    /// set the paraeter Io to Go4ParIo
    void   setGo4ParIo(HRuntimeDb*);
    /// init the container through rtdb
    Bool_t init(HParIo*, Int_t*);
    /// init the conatiner (no rtdb)
    Bool_t initStandAlone();
    /// write the container
    Int_t  write(HParIo*);
    /// get the pointer to the list of active hists
    TList* getListOfActiveHists(){return mdcActiveHists;}
    /// get the pointer to the hist pool list
    TList* getListOfHists()      {return mdcHistpool;}
    /// reset all hists in active list
    void   resetActiveList(Int_t level,Int_t count);
    /// put the parameter to the list of HParCond
    void   putParams(HParamList*);
    /// get the parameters from the list of HParCond
    Bool_t getParams(HParamList*);
    /// reset the container
    void   clear();
    /// print the parameter to the screen
    void   printParam(void);
    /// print the status of the container to the screen
    void   printStatus(void);
    /// get the pointer to the internal HMdcHistFiller
    HMdcHistFiller* getMdcFiller(){return mdcfiller;}
    ClassDef(HMdcHistBooker,1) // Container for the MDC Hists
};

#endif  /*!HMDCHISTBOOKER_H*/

