#ifndef HTOFINOGO4MON_H
#define HTOFINOGO4MON_H

//#include "hldfilesource.h" //to get file name
#include "hreconstructor.h"
#include "TString.h"

class HCategory;
class HIterator;
class HTofinoHistBooker;

class HTofinoGo4Mon : public HReconstructor {
protected:

  //########################### COMMON VARIABLES ##########################################

  // pointer to categories and iterators
  HCategory* calCat;        //! pointer to the cal data
  HCategory* rawCat;        //! pointer to the raw data

  HIterator* itercal;       //! iterator on cal data.
  HIterator* iterraw;       //! iterator on raw data.

  // pointer to used parameter containers
  static HTofinoHistBooker* tofHistbooker;//!
  void   initVariables();
  Bool_t getCategories();
  void   setParContainers(void);
public:
  HTofinoGo4Mon(void);
  HTofinoGo4Mon(Text_t* name,Text_t* title);
  ~HTofinoGo4Mon(void);
  void   printStatus();
  Bool_t init(void);
  Bool_t reinit(void);
  Int_t  execute();
  Bool_t  finalize();
  ClassDef(HTofinoGo4Mon,0) // fills different hists for cal1 data
};

#endif /* !HTOFINOGO4MON_H */
