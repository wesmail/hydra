#ifndef HTBOXSUM_H
#define HTBOXSUM_H

#include "hreconstructor.h"
#include "hlocation.h"
#include "hstartdef.h"
#include "tofinodef.h"
 
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
















