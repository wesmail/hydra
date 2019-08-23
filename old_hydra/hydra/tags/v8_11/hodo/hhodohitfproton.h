#ifndef HHODOHITFPROTON_H
#define HHODOHITFPROTON_H

#include "hreconstructor.h"
#include "hlocation.h"
#include "htofinocalpar.h"
#include "hdetector.h"

class HCategory;
class HIterator;
class HSpectrometer;

#define	HODOHITF_PROTON_MAXCHN	32

class HHodoHitFProton : public HReconstructor
{
protected:
  HCategory *pCalCat;          // pointer to the cal data
  HCategory *pHitCat;          // pointer to the hit data
  HIterator *pIterCal;            // iterator over cal data.
  HLocation loc;              // location for new hit object

  Bool_t hit[HODOHITF_PROTON_MAXCHN];
  Float_t time[HODOHITF_PROTON_MAXCHN];

  Int_t hitStrip;
  Float_t hitTime;

  void prepare_hitfinder(void);
  Int_t find_hit(void);
  
public: 
  HHodoHitFProton(void);
  HHodoHitFProton(Text_t* name,Text_t* title);
  ~HHodoHitFProton(void);
  Bool_t init(void);
  Int_t execute(void);
  Bool_t finalize(void);
  void clear();

  ClassDef(HHodoHitFProton,0) // Hit finder for START detector
};

#endif /* !HHODOHITFPROTON_H */

