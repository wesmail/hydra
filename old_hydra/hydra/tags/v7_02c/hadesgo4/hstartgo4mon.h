#ifndef HSTARTGO4MON_H
#define HSTARTGO4MON_H

#include "hreconstructor.h"
#include "TString.h"

class HCategory;
class HIterator;
class HStartHistBooker;

/** Start online monitoring task.
 * The task gets the pointer to the categories and
 * loops over them calling the fill functions of
 * HStartHistFiller.
*/
class HStartGo4Mon : public HReconstructor {
protected:

  //########################### COMMON VARIABLES ##########################################

  // pointer to categories and iterators
  HCategory* calCat;        //!< pointer to the cal data
  HCategory* rawCat;        //!< pointer to the raw data
  HCategory* hitCat;        //!< pointer to the hit data

  HIterator* itercal;       //!< iterator on cal data
  HIterator* iterraw;       //!< iterator on raw data
  HIterator* iterhit;       //!< iterator on hit data

  // pointer to used parameter containers
  static HStartHistBooker* startHistbooker;//!< pointer to HStartHistBooker
  /// init the variable of the task
  void   initVariables();
  /// get the pointers to the categories and create the iterators
  Bool_t getCategories();
  /// get the pointers to the needed Containers
  void   setParContainers(void);
public:
  HStartGo4Mon(void);
  HStartGo4Mon(Text_t* name,Text_t* title);
  ~HStartGo4Mon(void);
  /// prints the status of the task to the screen
  void   printStatus();
  /// init the task
  Bool_t init(void);
  /// reinit the task
  Bool_t reinit(void);
  /// execute function called in event loop
  Int_t  execute();
  /// finalize function of the task
  Bool_t  finalize();
  ClassDef(HStartGo4Mon,0) // fills different hists
};

#endif /* !HStartGO4MON_H */
