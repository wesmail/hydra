#ifndef HKICKTRACKF_H
#define HKICKTRACKF_H

#include "hreconstructor.h"
#include "hgeomvector.h"
#include "hkickplane2.h"
#include "hkickmatchpar.h"
#include "htofhit.h"
#include "htofcluster.h"
#include "hshowerhit.h"
#include "hkickcandidate.h"
#include "hkickcandidateevaluator.h"
#include "hkickimpact.h"
#include "hkicktask.h"
#include "hkickcandidatematrix.h"
#include "TObject.h"
#include "TFile.h"
#include "TNtuple.h"

class HCategory;
class HMdcSeg;
class HMdcGeomPar;
class HTofGeomPar;
class HTofClusterFPar;
class HTofinoGeomPar;
class HSpecGeomPar;
class HIterator;
class HKickTrackFPar;
class HRuntimeDb;
class HShowerGeometry;
class HKickTrack;
class HEvent;
class TH1F;

class HKickTrackF : public HKickTask {
private:
  static const Int_t kInactiveSector;  //Sector is not active
  static const Int_t kLowResSector;    //Sector has not outer chambers
protected:
  Cat_t fShowerCatId;
  HLocation fSectorLoc; //!
  HKickPlane2 *fKickPlane; //The kick plane
  HKickMatchPar *fMatchPar;
  HKickPlane2 *fKickPlaneList[2];
  HKickMatchPar *fMatchParList[2];
  HKickTrackFPar *fKickPar; //Finder parameters
  TClonesArray *fImpacts; //!List of impacts in kick plane.
  Int_t fNImpacts; //Nb of valid impacts
  HCategory *fInput; //!Input category
  HCategory *fTofHits; //!TOF hits
  HCategory *fTofClusters; //!TOF clusters
  HCategory *fShowerHits; //!
  HCategory *fCandidateOut; //! Output category
  //  HTofHit *fTofArray[22*8]; //! array of tof hits;
  //HShowerHit *fShowerArray[32*32]; //FIXME:
  TObjArray fOuterHitArray;
  Int_t fNTofHits, fNShowerHits,fNTotalHits;
  HIterator *fIter; //Iterator in fInput
  HIterator *fTofIter; //Iterator in fInput
  HIterator *fTofCIter; //Iterator in fInput
  HIterator *fShowerIter; //!
  Double_t fMinMomentum; 
  Double_t fMaxXPull; //Max pull in x
  HMdcGeomPar *fMdcGeometry; //!Geometry transformation
  HTofGeomPar *fTofGeometry; //!TOF geometry
  HTofClusterFPar *fTofClFPar;
  HTofinoGeomPar *fTofinoGeometry; //! Tofino Geometry
  HSpecGeomPar *fSpecGeometry;
  HShowerGeometry *fShowerGeometry;
  HKickCandidateMatrix fCandidates;
  HKickCandidateEvaluator *fEvaluator;
  Bool_t fHasTof, fHasShower;
  Bool_t fConservativeMode; 
  Bool_t fTofClusterMode;
  TArrayI fSectors; //Sectors to be analized.
  Float_t fTimeOffset;
  Bool_t fDebug;
  Bool_t fControlHistograms;  // flag for control histograms
  TNtuple *fRecStat;  //! Reconstruction statistics
  Float_t fTargetFlag; //Flag indicating which target was used.
  Bool_t fUsingVertex; //Wheter to use or not to use vertex algorithm
  Bool_t fMultipleTarget;

  TH1F *histCorELoss;
  TH1F *histCorELossCut;
  TH1F *histDiffMom;
  TH1F *histDiffMomCut;

  virtual void readHits(HLocation &loc);
  virtual void readMetaHits(HLocation &loc);
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
  void selectTofClusters(Int_t i);
  void checkTofClusterParticipants(Int_t i,Int_t j,Int_t sz);
  void checkMETAoverlap(Int_t i,Int_t j);
  virtual HKickTrack *makeTrack(void);
  virtual HCategory *makeOutputCategory(HEvent *event);
public:
  //HKickTrackF(void);
  HKickTrackF(const Text_t name[]="kick.trackf",const Text_t title[]="Kick track finder");
  ~HKickTrackF(void);
  Int_t execute(void);
  Bool_t init(void);
  Bool_t reinit(void);
  Bool_t finalize(void);

  Bool_t hasTof(void) { return fHasTof; }
  Bool_t hasShower(void) { return fHasShower; }
  void setConservativeMode(Bool_t m) { fConservativeMode=m; }
  Bool_t getConservativeMode(void) { return fConservativeMode; }
  void setTofClusterMode(Bool_t m) { fTofClusterMode = m; }
  Bool_t getTofClusterMode(void) { return fTofClusterMode; }
  void setDebug(Bool_t flag=kTRUE) { fDebug = flag; }
  void enableControlHistograms() { fControlHistograms = kTRUE; }
  Bool_t usingVertexAlgorithm(void) { return fUsingVertex; }
  void setVertexAlgorithm(Bool_t f=kTRUE) { fUsingVertex=f; }
  Bool_t hasMultipleTarget(void) { return fMultipleTarget; }
  void setMultipleTarget(Bool_t f=kTRUE) { fMultipleTarget=f; }
  ClassDef(HKickTrackF,0)
};

#endif





