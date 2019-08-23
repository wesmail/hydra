#ifndef HTRIGGERGO4MON_H
#define HTRIGGERGO4MON_H

//#include "hldfilesource.h" //to get file name
#include "hreconstructor.h"
#include "TString.h"

class HCategory;
class HIterator;
class HTriggerHistBooker;

/** Trigger onlien Monitoring task.
 * The task gets the pointer to the categories on loops
 * over them calling the fill function of
 * HTriggerHistFiler.
 */

class HTriggerGo4Mon : public HReconstructor {
protected:

  //########################### COMMON VARIABLES ##########################################

  // pointer to categories and iterators
  HCategory* richCat;       //!< pointer to the rich category
  HCategory* tofCat;        //!< pointer to the tof category
  HCategory* showerCat;     //!< pointer to the shower category
  HCategory* matchuCat;     //!< pointer to the matchu category (muleptons)

  HIterator* iterrich;      //!< iterator
  HIterator* itertof;       //!< iterator
  HIterator* itershower;    //!< iterator
  HIterator* itermatchu;    //!< iterator

  // pointer to "this"
  static HTriggerGo4Mon* pointMe;   //!< pointer to 'this'
  // flag to determine wether to take just downscaled events or not
  Int_t useDownscaled;             //!< use, all events if 0, only downscaled events if 1, and triggered events if 2

  // pointer to used parameter containers
  static HTriggerHistBooker* triggerHistbooker;//!< pointer to HTriggerHistBooker
  /// init the variable of the task
  void   initVariables();
  /// get the pointers to the categories and create the iterators
  Bool_t getCategories();
  /// get the pointers to the needed containers
  void   setParContainers(void);
public:
  HTriggerGo4Mon(void);
  HTriggerGo4Mon(Text_t* name,Text_t* title);
  ~HTriggerGo4Mon(void);
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
  /// returns pointer to "this"
  static HTriggerGo4Mon* getExObject();
  /// sets the downscaled flag
  Int_t getDownscaled()       { return useDownscaled; }
  /// gets the downscaled flag
  void setDownscaled(Int_t b) { useDownscaled = b;    }

  ClassDef(HTriggerGo4Mon,0) // fills different hists for cal1 data
};

#endif /* !HTRIGGERGO4MON_H */
