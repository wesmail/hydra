#ifndef HKICKTRACKF_H
#define HKICKTRACKF_H

#include "hreconstructor.h"
#include "hgeomvector.h"
#include "hkickplane.h"
#include "htofhit.h"
#include "hshowerhit.h"
#include "hkickcandidate.h"
#include "hkickcandidateevaluator.h"
#include "hkickimpact.h"
#include "hkicktask.h"
#include <TObject.h>
#include <TFile.h>

class HCategory;
class HMdcSeg;
class HMdcGeomPar;
class HTofGeomPar;
class HSpecGeomPar;
class HIterator;
class HKickTrackFPar;
class HRuntimeDb;
class HShowerGeometry;


class HKickCandidateMatrix { //FIXME: Generalize
protected:
  HKickCandidate *fData;
  Int_t fDimX,fDimY,fSize;
public:
  HKickCandidateMatrix(void) : fData(0), fDimX(0), fDimY(0), fSize(0) {}
  HKickCandidateMatrix(Int_t dx,Int_t dy) : fData(0) 
    { resize(dx,dy); }
  ~HKickCandidateMatrix();
  void resize(Int_t dx,Int_t dy);
  HKickCandidate &operator()(Int_t i,Int_t j);
  Int_t getDimX(void) const { return fDimX; }
  Int_t getDimY(void) const { return fDimY; }
  ostream &print(ostream &out);  
};

class HKickTrackF : public HKickTask {
private:
  static const Int_t kInactiveSector;  //Sector is not active
  static const Int_t kLowResSector;    //Sector has not outer chambers
  static const Int_t kMidResSector;    //Only one outer chamber is available
public:
  HLocation fSectorLoc; //!
  HKickPlane *fKickPlane; //The kick plane
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
  HSpecGeomPar *fSpecGeometry;
  HShowerGeometry *fShowerGeometry;
  HKickCandidateMatrix fCandidates;
  HKickCandidateEvaluator *fEvaluator;
  Bool_t fHasTof, fHasShower;
  Bool_t fConservativeMode; 
  TArrayI fSectors; //Sectors to be analized.

  virtual void readHits(HLocation &loc);
  void readOuterHits(HLocation &levArmLoc);
  void readMetaHits(HLocation &loc);
  void transform(HMdcSeg *hit,HGeomVector &r,HGeomVector &alpha);
  Bool_t isUnique(Int_t i,Int_t j);
  Int_t execute_mult(void);
  virtual void fillControl(HKickCandidate &candidate, HKickImpact *imp);
  virtual void fillData(HKickCandidate &candidate,HKickImpact *imp);
  virtual HKickCandidateEvaluator *makeEvaluator(void);
  void assignRemainingCombinations(void);
  void clearImpactList();
  void evaluateMetaCandidates(void);
  void evaluateOuterCandidates(void);
  Bool_t passesCuts(HKickCandidate &c); 
  void assignUniqueCombinations(void);
  void setMidResSector(Int_t s) { fSectors[s] = kMidResSector; }

public:
  HKickTrackF(Text_t name[],Text_t title[]);
  ~HKickTrackF(void);
  Int_t execute(void);
  Bool_t init(void);
  Bool_t finalize(void);

  Bool_t hasTof(void) { return fHasTof; }
  Bool_t hasShower(void) { return fHasShower; }
  void setConservativeMode(Bool_t m) { fConservativeMode=m; }
  Bool_t getConservativeMode(void) { return fConservativeMode; }
  ClassDef(HKickTrackF,0)
};

#endif





