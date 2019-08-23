//*-- Author : Dan Magestro
//*-- Created: 06/09/01
//*-- Last modified: 07/11/01

// HCutLeptonId Class Header

#ifndef HCUTLEPTONID_H
#define HCUTLEPTONID_H

#include "hcut.h"

class HParticle;
class HRichHit;
class HKickTrack;

class HCutLeptonId : public HCut {

public:
  HCutLeptonId();
  HCutLeptonId(const Text_t *name, const Text_t *title);
  ~HCutLeptonId() {}

  void printCutList();
  void reset();
  void setStandardCuts();

  Int_t check(HRichHit *ring, HParticle *part);

  Int_t check(HRichHit *ring, HKickTrack *track);

  void setTofinoTof(Double_t x, Double_t y) { kFlagTofinoTof = kTRUE;
          fMinTofinoTof = x; fMaxTofinoTof = y; }

  void setTofTof(Double_t x, Double_t y) { kFlagTofTof = kTRUE;
          fMinTofTof = x; fMaxTofTof = y; }

  void setThetaDiff(Double_t x, Double_t y) { kFlagThetaDiff = kTRUE;
          fMinThetaDiff = x; fMaxThetaDiff = y; }

  void setThetaDiff(Double_t x) { kFlagThetaDiff = kTRUE;
          fMinThetaDiff = -x; fMaxThetaDiff = x; }

  void setPhiDiff(Double_t x, Double_t y) { kFlagPhiDiff = kTRUE;
          fMinPhiDiff = x; fMaxPhiDiff = y; }

  void setPhiDiff(Double_t x) { kFlagPhiDiff = kTRUE;
          fMinPhiDiff = -x; fMaxPhiDiff = x; }

  void setShowerSum(Double_t x, Double_t y) { kFlagShowerSum = kTRUE;
          fShowerSum10 = x; fShowerSum20 = y; }

  void setShowerSum(Double_t x) { kFlagShowerSum = kTRUE;
          fShowerSum10 = x; fShowerSum20 = x; }

  void setRingPatMat(Double_t x, Double_t y) { kFlagRingPatMat = kTRUE;
          fMinRingPatMat = x; fMaxRingPatMat = y; }

  void setRingPatMat(Double_t x) { kFlagRingPatMat = kTRUE;
          fMinRingPatMat = x; }

  void setRingHouTra(Double_t x, Double_t y) { kFlagRingHouTra = kTRUE;
          fMinRingHouTra = x; fMaxRingHouTra = y; }

  void setRingHouTra(Double_t x) { kFlagRingHouTra = kTRUE;
          fMinRingHouTra = x; }

protected:
  Bool_t kFlagTofinoTof;  // Tofino time cut flag
  Int_t fnTofinoTof;      // Number of cut particles
  Double_t fMinTofinoTof; // Min Tofino time-of-flight (ns)
  Double_t fMaxTofinoTof; // Max Tofino time-of-flight (ns)

  Bool_t kFlagTofTof;  // Tof time cut flag
  Int_t fnTofTof;      // Number of cut particles
  Double_t fMinTofTof; // Max Tof time-of-flight (ns)
  Double_t fMaxTofTof; // Max Tof time-of-flight (ns)

  Bool_t kFlagShowerSum;  // Shower mult factor cut flag
  Int_t fnShowerSum;      // Number of cut particles
  Double_t fShowerSum10;  // Min shower multiplication factor 1/0
  Double_t fShowerSum20;  // Min shower multiplication factor 2/0

  Bool_t kFlagThetaDiff;  // Theta cut flag
  Int_t fnThetaDiff;      // Number of cut particles
  Double_t fMinThetaDiff; // Min theta difference: ring - kick
  Double_t fMaxThetaDiff; // Max theta difference: ring - kick

  Bool_t kFlagPhiDiff;  // Phi cut flag
  Int_t fnPhiDiff;      // Number of cut particles
  Double_t fMinPhiDiff; // Min phi difference: ring - track
  Double_t fMaxPhiDiff; // Max phi difference: ring - track

  Bool_t kFlagRingPatMat;  // Ring pattern matrix cut flag
  Int_t fnRingPatMat;      // Number of cut particles
  Double_t fMinRingPatMat; // Min value for pattern matrix
  Double_t fMaxRingPatMat; // Max value for pattern matrix

  Bool_t kFlagRingHouTra;  // Ring Hough trans. cut flag
  Int_t fnRingHouTra;      // Number of cut particles
  Double_t fMinRingHouTra; // Min value for Hough trans
  Double_t fMaxRingHouTra; // Max value for Hough trans

ClassDef(HCutLeptonId,1) // Single particle cuts

};

#endif
