#ifndef HMDCGO4MON_H
#define HMDCGO4MON_H

#include "hreconstructor.h"
#include "TString.h"

class HCategory;
class HIterator;
class HMdcHistBooker;
/** Mdc monitor task.
 * Gets the pointers to the Mdc Categories and
 * loops over them calling the fill functions
 * of HMdcHistFiller.
*/
class HMdcGo4Mon : public HReconstructor {
protected:

  //########################### COMMON VARIABLES ##########################################

  // pointer to categories and iterators
  HCategory* hitCat;        //!< pointer to the hit data
  HCategory* calCat;        //!< pointer to the cal data
  HCategory* rawCat;        //!< pointer to the raw data
  HCategory* rawCatHeader;  //!< pointer to the raw data header

  HIterator* iterhit;       //!< iterator on hit data.
  HIterator* itercal1;      //!< iterator on cal1 data.
  HIterator* iterraw;       //!< iterator on raw data.
  HIterator* iterrawHeader; //!< iterator on raw data header.

  // pointer to used parameter containers
  static HMdcHistBooker* Histbooker;//!< pointer to HMdcHistBooker
  /// init variables
  void   initVariables();
  /// get pointers to the Mdc categories.
  /// Create the iterators for existing categories
  Bool_t getCategories();
  /// get the needed parameter containers
  void   setParContainers(void);
public:
  HMdcGo4Mon(void);
  HMdcGo4Mon(Text_t* name,Text_t* title);
  ~HMdcGo4Mon(void);
  /// print the status of the monitor task to the screen
  void   printStatus();
  /// init the task
  Bool_t init(void);
  /// reinit the task
  Bool_t reinit(void);
  /// execute function called in the event loop
  Int_t  execute();
  /// finalize function of th task
  Bool_t  finalize();
  ClassDef(HMdcGo4Mon,0) // fills different hists for cal1 data
};

#endif /* !HMDCGO4MON_H */
