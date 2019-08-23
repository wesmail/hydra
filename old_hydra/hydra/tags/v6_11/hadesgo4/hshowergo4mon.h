#ifndef HSHOWERGO4MON_H
#define HSHOWERGO4MON_H

//#include "hldfilesource.h" //to get file name
#include "hreconstructor.h"
#include "TString.h"

class HCategory;
class HIterator;
class HShowerHistBooker;

class HShowerGo4Mon : public HReconstructor {
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
  static HShowerHistBooker* showerHistbooker;//!
  void   initVariables();
  Bool_t getCategories();
  void   setParContainers(void);
public:
  HShowerGo4Mon(void);
  HShowerGo4Mon(Text_t* name,Text_t* title);
  ~HShowerGo4Mon(void);
  void   printStatus();
  Bool_t init(void);
  Bool_t reinit(void);
  Int_t  execute();
  Bool_t  finalize();
  ClassDef(HShowerGo4Mon,0) //
};

#endif /* !HSHOWERGO4MON_H */
