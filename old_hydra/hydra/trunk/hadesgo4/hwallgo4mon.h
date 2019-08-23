#ifndef HWALLGO4MON_H
#define HWALLGO4MON_H

//#include "hldfilesource.h" //to get file name
#include "hreconstructor.h"
#include "TString.h"

class HCategory;
class HIterator;
class HWallHistBooker;

/** Wall online Monitoring task.
 * The tasks get the pointers to the categories and
 * loops over them calling the fill function
 * of HWallHistFiller.
 */
class HWallGo4Mon : public HReconstructor {
protected:

  //########################### COMMON VARIABLES ##########################################

  // pointer to categories and iterators
  HCategory* rawCat;        //!< pointer to the raw data
  HCategory* hitCat;        //!< pointer to the hit data

  HIterator* iterraw;       //!< iterator on raw data.
  HIterator* iterhit;       //!< iterator on hit data.

  // pointer to used parameter containers
  static HWallHistBooker* wallHistbooker;//!< pointer to HWallHistBooker
  /// init the variables of the task
  void   initVariables();
  /// get the pointers of the categories and create the iterators
  Bool_t getCategories();
  /// get the pointers to the needed containers
  void   setParContainers(void);
public:
  HWallGo4Mon(void);
  HWallGo4Mon(const Text_t* name,const Text_t* title);
  ~HWallGo4Mon(void);
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
  ClassDef(HWallGo4Mon,0) // fills different hists for cal1 data
};

#endif /* !HWALLGO4MON_H */
