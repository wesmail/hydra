#ifndef HMDCGO4MON_H
#define HMDCGO4MON_H

//#include "hldfilesource.h" //to get file name
#include "hreconstructor.h"
#include "TString.h"

class HCategory;
class HIterator;
class HMdcHistBooker;

class HMdcGo4Mon : public HReconstructor {
protected:

  //########################### COMMON VARIABLES ##########################################

  // pointer to categories and iterators
  HCategory* calCat;        //! pointer to the cal data
  HCategory* rawCat;        //! pointer to the raw data
  HCategory* rawCatHeader;  //! pointer to the raw data header

  HIterator* itercal1;      //! iterator on cal1 data.
  HIterator* iterraw;       //! iterator on raw data.
  HIterator* iterrawHeader; //! iterator on raw data header.

  // pointer to used parameter containers
  static HMdcHistBooker* Histbooker;//!
  void   initVariables();
  Bool_t getCategories();
  void   setParContainers(void);
public:
  HMdcGo4Mon(void);
  HMdcGo4Mon(Text_t* name,Text_t* title);
  ~HMdcGo4Mon(void);
  void   printStatus();
  Bool_t init(void);
  Bool_t reinit(void);
  Int_t  execute();
  Bool_t  finalize();
  ClassDef(HMdcGo4Mon,0) // fills different hists for cal1 data
};

#endif /* !HMDCGO4MON_H */
