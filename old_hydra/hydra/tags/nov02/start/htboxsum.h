#ifndef HTBOXSUM_H
#define HTBOXSUM_H

#include "hreconstructor.h"
#include "hlocation.h"
#include "hstartdef.h"
 
class HCategory;
class HIterator;

class HTBoxSum : public HReconstructor {
protected:
  Int_t evsum;                 //! number of events summed 
  HCategory *tboxCat;          //! pointer to TBox data
  HCategory *tboxsCat;         //! pointer to TBox data summed
  TIterator *iter;             //! iterator on data
  TIterator *iters;            //! iterator on data sum
  HLocation loc;               //! location for object
  Float_t scaler1lsum[8];      //! scaler sums
  Float_t scaler2lsum[8];      //!
  Float_t scaler3lsum[8];      //!
  Float_t scaler4lsum[8];      //!
  Int_t nChan;                 //! nb. of scaler channels
  Int_t timeInSpill;           //! timeInSpill
  Bool_t spill;                //! spill on/off

public: 
  HTBoxSum(void);
  HTBoxSum(Text_t* name,Text_t* title);
  ~HTBoxSum(void);
  Bool_t init(void);
  Bool_t finalize(void);
  Int_t execute(void);
public:
  ClassDef(HTBoxSum,0) // Sums the HTBox data over many events
};

#endif /* !HTBoxSum_H */
















