//*-- Author : Dan Magestro
//*-- Created: 06/09/01
//*-- Last modified: 07/11/01

// HCutParticle Class Header

#ifndef HCUTPARTICLE_H
#define HCUTPARTICLE_H

#include "hcut.h"

class HCategory;
class HIterator;
class HParticle;

class HCutParticle : public HCut {

public:
  HCutParticle(void);
  HCutParticle(const Text_t *name, const Text_t *title);
  ~HCutParticle() {}

  void printCutList();
  void reset();
  void setStandardCuts();

  Int_t check(HParticle *part);
  TObjArray *getAcceptedParticles(TObjArray *ParticleList);

  void setCharge(Int_t n) { kFlagCharge = kTRUE; fCharge = n; }
  void setP(Double_t x, Double_t y) { kFlagP = kTRUE;
          fMinP = x; fMaxP = y; }
  void setPt(Double_t x, Double_t y) { kFlagPt = kTRUE;
          fMinPt = x; fMaxPt = y; }
  void setTheta(Double_t x, Double_t y) { kFlagTheta = kTRUE;
          fMinTheta = x; fMaxTheta = y; }
  void setPhi(Double_t x, Double_t y) { kFlagPhi = kTRUE;
          fMinPhi = x; fMaxPhi = y; }
  void setRapidity(Double_t x, Double_t y) { kFlagRapidity = kTRUE;
          fMinRapidity = x; fMaxRapidity = y; }

  void setPCutOff() { kFlagP = kFALSE; }
  void setPtCutOff() { kFlagPt = kFALSE; }

private:
  HCategory* fPartCat;          //! Pointer to Particle data category
  HIterator* iterPart;        //! Iterator over Particle category

protected:
  Bool_t fVerbose;       //! Print cut particle statements to screen

  TObjArray *fAcceptedParticles; //! List of accepted particles after cuts

  Bool_t kFlagCharge; // flag of cut
  Int_t fnCharge; // number of not accepted tracks due to this cut
  Int_t fCharge; // -1 for neg particles +1 pos particles

  Bool_t kFlagP; // flag of cut
  Int_t fnP; // number of not accepted tracks due to this cut
  Double_t fMinP; // Minumum p
  Double_t fMaxP; // Maximum p

  Bool_t kFlagPt; // flag of cut
  Int_t  fnPt; // number of not accepted tracks due to this cut
  Double_t fMinPt; // Minumum pt
  Double_t fMaxPt; // Maximum pt

  Bool_t kFlagRapidity;  // flag of cut
  Int_t  fnRapidity;     // number of not accepted tracks due to this cut
  Double_t fMinRapidity; // Minumum rapidity
  Double_t fMaxRapidity; // Maximum rapidity

  Bool_t kFlagTheta; // flag of cut
  Int_t  fnTheta; // number of not accepted tracks due to this cut
  Double_t fMinTheta; // Minumum theta
  Double_t fMaxTheta; // Maximum theta

  Bool_t kFlagPhi; // flag of cut
  Int_t  fnPhi; // number of not accepted tracks due to this cut
  Double_t fMinPhi; // Minumum phi
  Double_t fMaxPhi; // Maximum phi

ClassDef(HCutParticle,1) // Single particle cuts

};
#endif
