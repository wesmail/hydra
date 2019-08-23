//*-- Author : Dan Magestro
//*-- Created: 06/09/01
//*-- Last modified: 07/11/01

// HCutTrack Class Header

#ifndef HCUTTRACK_H
#define HCUTTRACK_H

#include "hcut.h"

class HCategory;
class HIterator;
class HKickTrack;

class HCutTrack : public HCut {

public:
  HCutTrack();
  HCutTrack(const Text_t *name, const Text_t *title);
  ~HCutTrack() {}

  void printCutList();
  void printCutStatistics();
  void reset();
  void setStandardCuts();

  Int_t check(HKickTrack *track);

  void setP(Double_t x, Double_t y) { kFlagP = kTRUE;
          fMinP = x; fMaxP = y; }

  void setTofinoTof(Double_t x, Double_t y) { kFlagTofinoTof = kTRUE;
          fMinTofinoTof = x; fMaxTofinoTof = y; }

  void setTofTof(Double_t x, Double_t y) { kFlagTofTof = kTRUE;
          fMinTofTof = x; fMaxTofTof = y; }

  void isIdentifiedHadron(void) { kFlagIdentifiedHadron = kTRUE;}

private:
  HCategory* fTrackCat;        //! Pointer to HKickTrack data category (not used yet)
  HIterator* iterTrack;        //! Iterator over HKickTrack category (not used yet)

protected:
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

  Bool_t kFlagIdentifiedHadron;  //  time cut flag
  Int_t fnIdentifiedHadron;      // Number of cut particles

ClassDef(HCutTrack,1)  // Cuts on tracks and hits which make up track

};

#endif
