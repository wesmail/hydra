#ifndef HDILEPTONFILLER_H
#define HDILEPTONFILLER_H

#include "hreconstructor.h"
#include "iostream.h"
#include "hlocation.h"

class HIterator;
class HCategory;

class HDileptonFiller : public HReconstructor {

private:
  HCategory* fPartCat;          //! Pointer to Particle data category
  HCategory* fDileptCat;     //! Pointer to Dilepton data category
  HIterator* iterPart;        //! Iterator over Particle category
  Bool_t kSkip;                 //! Option to skip events without certain requirements

public:
  HDileptonFiller(void);
  HDileptonFiller(Text_t* name,Text_t* title,Bool_t skip=kFALSE);
  ~HDileptonFiller(void) {;}

  Bool_t init(void);
  Bool_t finalize(void) {return kTRUE;}
  Int_t  execute(void);


public:
  ClassDef(HDileptonFiller,1) // Filler for Dilepton container
};

#endif
