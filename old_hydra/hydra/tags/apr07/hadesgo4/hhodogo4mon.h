#ifndef HHODOGO4MON_H
#define HHODOGO4MON_H

//#include "hldfilesource.h" //to get file name
#include "hreconstructor.h"
#include "TString.h"

class HCategory;
class HIterator;
class HHodoHistBooker;

/** Hodo online Monitoring task.
 * The tasks get the pointers to the categories and
 * loops over them calling the fill function
 * of HHodoHistFiller.
 */
class HHodoGo4Mon : public HReconstructor {
protected:

  //########################### COMMON VARIABLES ##########################################

  // pointer to categories and iterators
  HCategory* rawCat;        //!< pointer to the raw data

  HIterator* iterraw;       //!< iterator on raw data.

  // pointer to used parameter containers
  static HHodoHistBooker* hodoHistbooker;//!< pointer to HHodoHistBooker
  /// init the variables of the task
  void   initVariables();
  /// get the pointers of the categories and create the iterators
  Bool_t getCategories();
  /// get the pointers to the needed containers
  void   setParContainers(void);
public:
  HHodoGo4Mon(void);
  HHodoGo4Mon(Text_t* name,Text_t* title);
  ~HHodoGo4Mon(void);
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
  ClassDef(HHodoGo4Mon,0) // fills different hists for cal1 data
};

#endif /* !HHODOGO4MON_H */
