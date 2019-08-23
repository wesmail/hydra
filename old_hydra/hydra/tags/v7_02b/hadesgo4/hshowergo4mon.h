#ifndef HSHOWERGO4MON_H
#define HSHOWERGO4MON_H

//#include "hldfilesource.h" //to get file name
#include "hreconstructor.h"
#include "TString.h"

class HCategory;
class HIterator;
class HShowerHistBooker;

/** Shower online Monitoring task.
 * The task gets the pointer to the Shower categories
 * and loops over them calling the fill functions of
 * HShowerHistFiller.
 */

class HShowerGo4Mon : public HReconstructor {
protected:

  //########################### COMMON VARIABLES ##########################################

  // pointer to categories and iterators
  HCategory* calCat;        //!< pointer to the cal data
  HCategory* rawCat;        //!< pointer to the raw data
  HCategory* hitCat;        //!< pointer to the hit data header

  HIterator* itercal;       //!< iterator on cal data.
  HIterator* iterraw;       //!< iterator on raw data.
  HIterator* iterhit;       //!< iterator on hit data

  // pointer to used parameter containers
  static HShowerHistBooker* showerHistbooker;//!< pointer to HShowerHistBooker
  /// init the variables of the task
  void   initVariables();
  /// get the pointer to the categories and create the iterators
  Bool_t getCategories();
  /// get the pointer to the needed containers
  void   setParContainers(void);
public:
  HShowerGo4Mon(void);
  HShowerGo4Mon(Text_t* name,Text_t* title);
  ~HShowerGo4Mon(void);
  /// print the status of the task to the screen
  void   printStatus();
  /// init the task
  Bool_t init(void);
  /// reinit the task
  Bool_t reinit(void);
  /// execute function called in the event loop
  Int_t  execute();
  /// finalize function of the task
  Bool_t  finalize();
  ClassDef(HShowerGo4Mon,0) //
};

#endif /* !HSHOWERGO4MON_H */
