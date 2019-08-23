#ifndef HKICKPIONIMASS_H
#define HKICKPIONIMASS_H

#include "hreconstructor.h"
#include "TLorentzVector.h"
#include "TObject.h"
#include "TClonesArray.h"
#include "TNtuple.h"
#include "TCutG.h"
#include "hcategory.h"
#include "hiterator.h"
#include "hspecgeompar.h"
#include "hkicktrack.h"
#include "hrtmetatrack.h"

class HKickIMassParticle : public TObject {
  public:
    HKickIMassParticle(void) {}
    ~HKickIMassParticle(void) {}
    TLorentzVector &P(void) { return fMomentum; }
    void setSystem(Int_t i) { fSystem = i; }
    Int_t getSystem(void) { return fSystem; }
  protected:
    TLorentzVector fMomentum;
    Int_t fSystem;
  public:
    ClassDef(HKickIMassParticle,1) 
};

class HKickPionIMass : public HReconstructor {
public:
  enum EMode {kLowRes,kHiRes};
  HKickPionIMass(const Text_t name[],const Text_t title[]);
  ~HKickPionIMass(void);
  void setMode(EMode m) { fMode = m; }
  Int_t execute(void);
  Bool_t init(void);
  Bool_t reinit(void);
  Bool_t finalize(void);
protected:
  Bool_t isProton(HKickTrack *track);
  Bool_t isPion(HKickTrack *track);
  Bool_t isProton(HRtMetaTrack *track);
  Bool_t isPion(HRtMetaTrack *track);
  Bool_t isProton(Float_t pz, Float_t beta, Int_t sys);
  Bool_t isPion(Float_t pz, Float_t beta, Int_t sys);
  void readKickTracks(void);
  void readMetaTracks(void);
  void convert(HKickTrack *t, HKickIMassParticle *p);
  void convert(HRtMetaTrack *t, HKickIMassParticle *p);
  void clearArrays(void);
  void fillPair(Int_t type, HKickIMassParticle *p1, HKickIMassParticle *p2);
  
  HCategory *fInput; //!
  HIterator *fInputIter; //!
  TNtuple *fOutput; //!
  HSpecGeomPar *fGeomPar; //!
  TClonesArray *fPiMinus; //!
  TClonesArray *fPiPlus; //!
  TClonesArray *fProton; //!
  TCutG *fCutPiPlusSho; //!
  TCutG *fCutPiMinusSho; //!
  TCutG *fCutProtonSho; //!
  TCutG *fCutPiPlusTof; //!
  TCutG *fCutPiMinusTof; //!
  TCutG *fCutProtonTof; //!
  EMode fMode;
public:
  ClassDef(HKickPionIMass,1)
};

#endif
