#ifndef HRTMDCTRK_H
#define HRTMDCTRK_H

#include "hmdctrk.h"
#include "hgeomvector.h"

class HRtMdcTrk : public HMdcTrk {
public:
  HRtMdcTrk(void) {}
  ~HRtMdcTrk(void) {}
  HGeomVector &getOuterPos(void) { return fOuterPos; }
  HGeomVector &getOuterDir(void) { return fOuterDir; }
  Float_t getLength(void) { return length; }
  void setOuterPos(HGeomVector &p) { fOuterPos = p; }
  void setOuterDir(HGeomVector &v) { fOuterDir = v; }
  void setLength(Float_t l) { length = l; }
  void setQuality(Float_t dk, Float_t dd) { dKick=dk; d=dd; }
  void setLeverArm(Float_t la) { leverArm = la; }
  Float_t getDKick(void) { return dKick; }
  Float_t getD(void) { return d; }
  Float_t getLeverArm(void) { return leverArm; }
protected:
  HGeomVector fOuterPos; //Measured position of outer track
  HGeomVector fOuterDir; //Measured direction of outer track
  Float_t length;        //Length up to measurement point
  Float_t dKick,d;       //Quality of candidate
  Float_t leverArm;      //Lever arm used in slope determination
public:
  ClassDef(HRtMdcTrk,1)
};

#endif
