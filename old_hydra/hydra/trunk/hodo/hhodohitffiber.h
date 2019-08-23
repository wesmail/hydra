#ifndef HHODOHITFFIBER_H
#define HHODOHITFFIBER_H

#include "hreconstructor.h"
#include "hlocation.h"
#include "hdetector.h"

class HCategory;
class HIterator;
class HSpectrometer;
class HHodoRefHitPar;

#define	HODOHITF_FIBER_MAXCHN	128

class HHodoHitFFiber : public HReconstructor
{
protected:
  HCategory *pCalCat;         //! pointer to the cal data
  HCategory *pHitCat;         //! pointer to the hit data
  HIterator *pIterCal;        //! iterator over cal data.
  HLocation loc;              //! location for new hit object
  Int_t     usemod;           //! only this hodo module is used
  HHodoRefHitPar *refhit;     //! Hit Window ...

  Bool_t hit[HODOHITF_FIBER_MAXCHN];
  Float_t time[HODOHITF_FIBER_MAXCHN];

  Int_t hitStrip;
  Float_t hitTime;

  void prepare_hitfinder(void);
  Bool_t find_hit(void);
  
public: 
  HHodoHitFFiber(void);
  HHodoHitFFiber(const Text_t* name,const Text_t* title);
  ~HHodoHitFFiber(void);
  void set_mod(Int_t m){usemod=m;};
  Bool_t init(void);
  Int_t execute(void);
  Bool_t finalize(void);
  void clear();

  ClassDef(HHodoHitFFiber,0) // Hodo Hit finder for Fiber detector
};

#endif /* !HHODOHITFFIBER_H */

