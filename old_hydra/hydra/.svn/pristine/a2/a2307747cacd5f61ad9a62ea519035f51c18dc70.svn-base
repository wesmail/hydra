#ifndef HTOFINOGO4MON_H
#define HTOFINOGO4MON_H

//#include "hldfilesource.h" //to get file name
#include "hreconstructor.h"
#include "TString.h"

class HCategory;
class HIterator;
class HTofinoHistBooker;

/** Tofino online Monitoring task.
 * The tasks get the pointers to the categories and
 * loops over them calling the fill function
 * of HTofinoHistFiller.
 */
class HTofinoGo4Mon : public HReconstructor {
protected:

  //########################### COMMON VARIABLES ##########################################

  // pointer to categories and iterators
  HCategory* calCat;        //!< pointer to the cal data
  HCategory* rawCat;        //!< pointer to the raw data
  HCategory* showCat;        //!< pointer to the HShowerHitTof  data

  HIterator* itercal;       //!< iterator on cal data.
  HIterator* iterraw;       //!< iterator on raw data.
  HIterator* itershow;       //!< iterator on HShowerHitTof data.

  // pointer to used parameter containers
  static HTofinoHistBooker* tofinoHistbooker;//!< pointer to HTofinoHistBooker
  /// init the variables of the task
  void   initVariables();
  /// get the pointers of the categories and create the iterators
  Bool_t getCategories();
  /// get the pointers to the needed containers
  void   setParContainers(void);
public:
  HTofinoGo4Mon(void);
  HTofinoGo4Mon(const Text_t* name,const Text_t* title);
  ~HTofinoGo4Mon(void);
  /// print the status of the task to the screen
  void   printStatus();
  /// init the task
  Bool_t init(void);
  /// reinit the task
  Bool_t reinit(void);
  /// execute function of the task
  Int_t  execute();
  /// finalize function of the task
  Bool_t  finalize();
  ClassDef(HTofinoGo4Mon,0) // fills different hists for cal1 data
};

#endif /* !HTOFINOGO4MON_H */
