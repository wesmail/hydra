#ifndef HTOFGO4MON_H
#define HTOFGO4MON_H

//#include "hldfilesource.h" //to get file name
#include "hreconstructor.h"
#include "TString.h"

class HCategory;
class HIterator;
class HTofHistBooker;

/**Tof online Monitoring task.
 * The task gets the pointers to the categories
 * and loops over them calling the fill function
 * of HTofHistFiller.
 */
class HTofGo4Mon : public HReconstructor {
protected:

  //########################### COMMON VARIABLES ##########################################

  // pointer to categories and iterators
  HCategory* rawCat;        //!< pointer to the raw data
  HCategory* hitCat;        //!< pointer to the hit data

  HCategory* tofinorawCat;  //!< pointer to the tofino raw data

  HIterator* iterraw;       //!< iterator on raw data.
  HIterator* iterhit;       //!< iterator on hit data

  HIterator* itertofinoraw; //!< iterator on tofino raw data

  // pointer to used parameter containers
  static HTofHistBooker* tofHistbooker;//!< pointer to  HTofHistBooker*
  /// init the variables of the task
  void   initVariables();
  /// get the pointers to the categories and create the iterators
  Bool_t getCategories();
  /// get the pointers to the needed containers
  void   setParContainers(void);
public:
  HTofGo4Mon(void);
  HTofGo4Mon(Text_t* name,Text_t* title);
  ~HTofGo4Mon(void);
  /// print the status of the task to he screen
  void   printStatus();
  /// init the task
  Bool_t init(void);
  /// reinit the task
  Bool_t reinit(void);
  /// execute function called in event loop
  Int_t  execute();
  /// finalize function of the task
  Bool_t  finalize();
  ClassDef(HTofGo4Mon,0) // fills different hists for cal1 data
};

#endif /* !HTOFGO4MON_H */
