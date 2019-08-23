#ifndef HRTM3METAMATCHSIM_H
#define HRTM3METAMATCHSIM_H

#include "hrtmetasegsim.h"
#include "hrtm3metamatch.h"
#include "TNtuple.h"

class HRtM3MetaMatchSim : public HRtM3MetaMatch {
 public:
  HRtM3MetaMatchSim(const Text_t name[],const Text_t title[]) : HRtM3MetaMatch(name,title) {fControl = 0;}
  ~HRtM3MetaMatchSim(void) {}
  Bool_t init(void);
 protected:
  HRtMetaSeg *fillData(HMdcHit *hit, HGeomVector &pos, HGeomVector &alpha,
		      HRtMetaPoint *point);
  HCategory *makeOutputCategory(HEvent *ev);
  HRtMetaSeg *makeSegment(TObject *s);
  void fillControl(HMdcHit *hit, HRtMetaPoint &point,
		   Float_t dx,Float_t dy,Int_t &nValid);
  HCategory *fCatKine;
  TNtuple *fControl;
 public:
  ClassDef(HRtM3MetaMatchSim,1)
};

#endif
