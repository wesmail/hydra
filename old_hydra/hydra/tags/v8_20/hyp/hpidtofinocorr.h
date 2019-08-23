//*-- Author : Stefano Spataro (spataro@lns.infn.it)
using namespace std;

#ifndef  HPIDTOFINOCORR_H
#define  HPIDTOFINOCORR_H

#include "hreconstructor.h"
#include "hcategory.h"
#include "hiterator.h"
#include "piddef.h"
#include "TString.h"
#include "TNtuple.h"

class HIterator;
class HCategory;

class HPidTofinoCorr : public HReconstructor {

 protected:


  HCategory* pTrackCand;           // HPidTrackCand category
  HIterator* iterTrackCand;        // iterator on HPidTrackCand
  
  
 public:    
  HPidTofinoCorr(void);
  ~HPidTofinoCorr(void);
  Bool_t init(void);
  Int_t execute(void);
  Bool_t finalize(void);
  ClassDef(HPidTofinoCorr,0)
    };
#endif /* !HPIDTOFINOCORR_H */







