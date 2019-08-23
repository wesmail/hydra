//*-- Author : Jochen Markert

#ifndef  HPIDMDCSTART_H
#define  HPIDMDCSTART_H

#include "hreconstructor.h"

class HIterator;
class HCategory;
class HMatrixCategory;
class HLinearCategory;
class HPidAlgStandCutsPar;

class HPidMdcStart : public HReconstructor {

 protected:

  HLinearCategory* pStartHitCat;    // pointer to start hit category
  HCategory* pShowerHitTofCat;      // pointer to showerhittof category
  HMatrixCategory* pTofClusterCat;  // pointer to tof cluster  category
  HMatrixCategory* pTofHitCat;      // pointer to tof hit  category
  HIterator* iterShowerHitTofCat;   // pointer to showerhittof category iterator
  HIterator* iterTofClusterCat;     // pointer to tof cluster  category iterator
  HIterator* iterTofHitCat;         // pointer to tof hit  category iterator
  Bool_t isEmbedding;               // ktrue = embedding mode
  Bool_t print;                     // ktrue = print debug info
  Float_t timeVal;                  // reference val for fastest particle
  void initParameters();
public:
  HPidMdcStart(void);
  HPidMdcStart(Text_t name[],Text_t title[]);
  ~HPidMdcStart(void);
  Bool_t init(void);
  Bool_t finalize(void);
  Int_t  execute(void);
  void   setPrint(Bool_t doprint){ print   = doprint; }
  void   setTimeVal(Float_t val) { timeVal = val;}
  ClassDef( HPidMdcStart,0)
};
#endif /* !HPIDMDCSTART_H */







