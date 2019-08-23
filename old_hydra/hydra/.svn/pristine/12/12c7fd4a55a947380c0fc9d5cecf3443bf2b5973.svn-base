#ifndef HTOFGO4MON_H
#define HTOFGO4MON_H

//#include "hldfilesource.h" //to get file name
#include "hreconstructor.h"
#include "TString.h"

class HCategory;
class HIterator;
class HTofHistBooker;

class HTofGo4Mon : public HReconstructor {
protected:

  //########################### COMMON VARIABLES ##########################################

  // pointer to categories and iterators
  HCategory* calCat;        //! pointer to the cal data
  HCategory* rawCat;        //! pointer to the raw data
  HCategory* hitCat;        //! pointer to the hit data header

  HIterator* itercal;       //! iterator on cal data.
  HIterator* iterraw;       //! iterator on raw data.
  HIterator* iterhit;       //! iterator on hit data

  // pointer to used parameter containers
  static HTofHistBooker* tofHistbooker;//!
  void   initVariables();
  Bool_t getCategories();
  void   setParContainers(void);
public:
  HTofGo4Mon(void);
  HTofGo4Mon(Text_t* name,Text_t* title);
  ~HTofGo4Mon(void);
  void   printStatus();
  Bool_t init(void);
  Bool_t reinit(void);
  Int_t  execute();
  Bool_t  finalize();
  ClassDef(HTofGo4Mon,0) // fills different hists for cal1 data
};

#endif /* !HTOFGO4MON_H */
