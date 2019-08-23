#ifndef HKICKTRACKF_H
#define HKICKTRACKF_H

#include "hreconstructor.h"
#include "hgeomvector.h"
#include "hkickplane2.h"
#include "hkickmatchpar.h"
#include "htofhit.h"
#include "hshowerhit.h"
#include "hkickcandidate.h"
#include "hkickcandidateevaluator.h"
#include "hkickimpact.h"
#include "hkicktask.h"
#include "hkickcandidatematrix.h"
#include <TObject.h>
#include <TFile.h>

class HCategory;
class HMdcSeg;
class HMdcGeomPar;
class HTofGeomPar;
class HTofinoGeomPar;
class HSpecGeomPar;
class HIterator;
class HKickTrackFPar;
class HRuntimeDb;
class HShowerGeometry;
class HKickTrack;
class HEvent;

class HKickTrackF : public HKickTask {
private:
  static const Int_t kInactiveSector;  //Sector is not active
  static const Int_t kLowResSector;    //Sector has not outer chambers
protected:
  Cat_t fShowerCatId;
  HLocation fSectorLoc; //!
  HKickPlane2 *fKickPlane; //The kick plane
  HKickMatchPar *fMatchPar;
  HKickTrackFPar *fKickPar; //Finder parameters
  TClonesArray *fImpacts; //!List of impacts in kick plane.
  Int_t fNImpacts; //Nb of valid impacts
  HCategory *fInput; //!Input category
  HCategory *fTofHits; //!TOF hits
  HCategory *fShowerHits; //!
  HCategory *fCandidateOut; //! Output category
  //  HTofHit *fTofArray[22*8]; //! array of tof hits;
  //HShowerHit *fShowerArray[32*32]; //FIXME:
  TObjArray fOuterHitArray;
  Int_t fNTofHits, fNShowerHits,fNTotalHits;
  HIterator *fIter; //Iterator in fInput
  HIterator *fTofIter; //Iterator in fInput
  HIterator *fShowerIter; //!
  Double_t fMinMomentum; 
  Double_t fMaxXPull; //Max pull in x
  HMdcGeomPar *fMdcGeometry; //!Geometry transformation
  HTofGeomPar *fTofGeometry; //!TOF geometry
  HTofinoGeomPar *fTofinoGeometry; //! Tofino Geometry
  HSpecGeomPar *fSpecGeometry;
  HShowerGeometry *fShowerGeometry;
  HKickCandidateMatrix fCandidates;
  HKickCandidateEvaluator *fEvaluator;
  Bool_t fHasTof, fHasShower;
  Bool_t fConservativeMode; 
  TArrayI fSectors; //Sectors to be analized.
  Float_t fTimeOffset;

  virtual void readHits(HLocation &loc);
  void readMetaHits(HLocation &loc);
  void transform(HMdcSeg *hit,HGeomVector &r,HGeomVector &alpha);
  Bool_t isUnique(Int_t i,Int_t j);
  Int_t execute_mult(void);
  virtual void fillControl(HKickCandidate &candidate, HKickImpact *imp);
  virtual HKickTrack *fillData(HKickCandidate &candidate,HKickImpact *imp);
  virtual HKickCandidateEvaluator *makeEvaluator(void);
  void assignRemainingCombinations(void);
  void clearImpactList();
  void evaluateMetaCandidates(void);
  Bool_t passesCuts(HKickCandidate &c); 
  void assignUniqueCombinations(void);
  virtual HKickTrack *makeTrack(void);
  virtual HCategory *makeOutputCategory(HEvent *event);
public:
  HKickTrackF(void);
  HKickTrackF(Text_t name[],Text_t title[]);
  ~HKickTrackF(void);
  Int_t execute(void);
  Bool_t init(void);
  Bool_t reinit(void);
  Bool_t finalize(void);

  Bool_t hasTof(void) { return fHasTof; }
  Bool_t hasShower(void) { return fHasShower; }
  void setConservativeMode(Bool_t m) { fConservativeMode=m; }
  Bool_t getConservativeMode(void) { return fConservativeMode; }
  ClassDef(HKickTrackF,0)
};

#endif





