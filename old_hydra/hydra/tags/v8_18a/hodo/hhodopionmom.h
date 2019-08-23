
//*-- Created 30.3.2006 by B. Spruck

#ifndef HHODOPIONMOM_H
#define HHODOPIONMOM_H

#include "hreconstructor.h"
#include "hlocation.h"

class HCategory;
class HIterator;

class HHodoPionMom : public HReconstructor {
protected:
  HCategory *hitHodoCat;      //! pointer to the hit data
  HIterator *iter;            //! iterator on hodo hit data.
  Int_t usemod1;               //! only this hodo module is used
  Int_t usemod2;               //! only this hodo module is used
  Float_t mean_max, mean_min; //! min/max for mean (=momentum)
  Float_t diff_max, diff_min; //! min/max for difference(=emittance)
 public:
  HHodoPionMom(void);
  HHodoPionMom(Text_t* name,Text_t* title);
  ~HHodoPionMom(void);
  void set_mods(Int_t m,Int_t n){usemod1=m;usemod2=n;};
  void set_range(Float_t mwmax, Float_t mwmin, Float_t dmax, Float_t dmin){
    mean_max=mwmax; mean_min=mwmin; diff_max=dmax; diff_min=dmin;
  }; // setting of mean and difference
  Bool_t init(void);
  Bool_t finalize(void) { return kTRUE; }
  Int_t execute(void);
public:
  ClassDef(HHodoPionMom,0) // Hodo PionMom: Copy Hodo Hit data to Start Hit data
};

#endif /* !HHODOPIONMOM_H */
















