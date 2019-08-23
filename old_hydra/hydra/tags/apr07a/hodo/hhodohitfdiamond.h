#ifndef HHODOHITFDIAMOND_H
#define HHODOHITFDIAMOND_H

#include "hreconstructor.h"
#include "hlocation.h"
#include "hdetector.h"

class HCategory;
class HIterator;
class HSpectrometer;
class HHodoRefHitPar;

#define	HODOHITF_DIAMOND_MAXCHN	128

class HHodoHitFDiamond : public HReconstructor
{
protected:
  HCategory *pCalCat;         //! pointer to the cal data
  HCategory *pHitCat;         //! pointer to the hit data
  HIterator *pIterCal;        //! iterator over cal data.
  HLocation loc;              //! location for new hit object
  Int_t usemod;               //! only this hodo module is used
  HHodoRefHitPar *refhit;     //! Hit Window ...

  Bool_t hit[HODOHITF_DIAMOND_MAXCHN];
  Float_t time[HODOHITF_DIAMOND_MAXCHN];

  Int_t hitStrip;
  Float_t hitTime;

  void prepare_hitfinder(void);
  Bool_t find_hit(void);
  
public:
  HHodoHitFDiamond(void);
  HHodoHitFDiamond(Text_t* name,Text_t* title);
  ~HHodoHitFDiamond(void);
  void set_mod(Int_t m){usemod=m;};
  Bool_t init(void);
  Int_t execute(void);
  Bool_t finalize(void);
  void clear();

  ClassDef(HHodoHitFDiamond,0) // Hodo Hit finder for Diamond detector
};

#endif /* !HHODOHITFDIAMOND_H */

