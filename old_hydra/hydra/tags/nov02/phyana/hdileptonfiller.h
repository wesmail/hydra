//*-- AUTHOR : D.Magestro
//*-- Created : 18/06/2001
#ifndef HDILEPTONFILLER_H
#define HDILEPTONFILLER_H

#include "hreconstructor.h"

class HCategory;
class HParticle;
class HPhysicsEvent;

class HDileptonFiller : public HReconstructor {

private:
  HCategory* fPartCat;          //! Pointer to Particle data category
  HCategory* fDileptCat;        //! Pointer to Dilepton data category
  HPhysicsEvent *phyEvent;      //! Pointer to physics event object
  Bool_t kSkip;                 //! Option to skip events without certain requirements

public:
  HDileptonFiller(void);
  HDileptonFiller(Text_t* name,Text_t* title,Bool_t skip=kFALSE);
  ~HDileptonFiller(void) {;}

  Bool_t init(void);
  Bool_t finalize(void) {return kTRUE;}
  void makeDilepton(HParticle *p1, HParticle *p2);
  Int_t  execute(void);

public:
  ClassDef(HDileptonFiller,1) // Filler for HDilepton container
};

#endif
