#ifndef HRTSEGMENTREF_H
#define HRTSEGMENTREF_H

#include <TObject.h>
#include "hgeomvector.h"
#include "hmdcseg.h"
#include "hmdchit.h"
#include "hrtmetaseg.h"

class HRtSegmentRef : public TObject {
public:
  HRtSegmentRef(void) { fTimesUsed=0; }
  ~HRtSegmentRef(void);
  HGeomVector &getR(void) { return fR; }
  HGeomVector &getAlpha(void) { return fAlpha; }
  TObject *getSegment(void) { return fSeg; }
  void setSegment(HMdcSeg *seg) { fSeg = seg; }
  void setSegment(HMdcHit *hit) { fSeg = hit; }
  void setSegment(HRtMetaSeg *hit) { fSeg = hit; }
  Int_t getUsedTimes(void) { return fTimesUsed; }
protected:
  HGeomVector fR, fAlpha; //These two vectors define a straight line
  TObject *fSeg;          //Original segment
  Int_t fTimesUsed;       //How many times this Ref was used
public:
  ClassDef(HRtSegmentRef,1)
};

#endif
