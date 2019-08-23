//*-- Author : Dan Magestro
//*-- Created: 06/09/01
//*-- Last modified: 07/11/01

// HCutHadronId Class Header

#ifndef HCUTHADRONID_H
#define HCUTHADRONID_H

#include "hcut.h"
#include <TCutG.h>
class HKickTrack;

class HCutHadronId : public HCut {

public:
  HCutHadronId();
  HCutHadronId(const Text_t *name, const Text_t *title);
  ~HCutHadronId() {}

  void printCutList();
  void reset();
  void setStandardCuts();

  Int_t getPid(HKickTrack *track);
  Int_t check(HKickTrack *track);

  void Draw();
  void setCutWindow(Int_t pid, TCutG *cut);
  void setCutWindow(Char_t *pid, TCutG *cut);

  void setP(Double_t x, Double_t y) { kFlagP = kTRUE;
          fMinP = x; fMaxP = y; }

  void setTofinoTof(Double_t x, Double_t y) { kFlagTofinoTof = kTRUE;
          fMinTofinoTof = x; fMaxTofinoTof = y; }

  void setTofTof(Double_t x, Double_t y) { kFlagTofTof = kTRUE;
          fMinTofTof = x; fMaxTofTof = y; }

protected:
  TCutG *piPlusCut;            // Pi+ cut window
  TCutG *piMinusCut;           // Pi- cut window
  TCutG *pionCut;              // PION cut window
  TCutG *kPlusCut;             // K+ cut window
  TCutG *kMinusCut;            // K- cut window
  TCutG *kaonCut;              // KAON cut window
  TCutG *protonCut;            // proton cut window
  TCutG *deuteronCut;          // deuteron cut window

  Bool_t kFlagP;  // flag of cut
  Int_t fnP;      // number of not accepted tracks due to this cut
  Double_t fMinP; // Minumum p
  Double_t fMaxP; // Maximum px

  Bool_t kFlagTofinoTof;  // Tofino time cut flag
  Int_t fnTofinoTof;      // Number of cut particles
  Double_t fMinTofinoTof; // Min Tofino time-of-flight (ns)
  Double_t fMaxTofinoTof; // Max Tofino time-of-flight (ns)

  Bool_t kFlagTofTof;  // Tof time cut flag
  Int_t fnTofTof;      // Number of cut particles
  Double_t fMinTofTof; // Max Tof time-of-flight (ns)
  Double_t fMaxTofTof; // Max Tof time-of-flight (ns)

ClassDef(HCutHadronId,1) // Cuts to identify hadrons (with 2-d cuts)

};

#endif
