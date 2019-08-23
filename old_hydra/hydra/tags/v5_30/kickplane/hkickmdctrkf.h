#ifndef HKICKMDCTRKF_H
#define HKICKMDCTRKF_H

#include <TObject.h>
#include <TNtuple.h>
#include "hkicktask.h"
#include "hlocation.h"
#include "hkicktrackf.h"
#include "hvertlsmfit.h"

class HCategory;
class HIterator;
class HMdcGeomPar;
class HMdcDetector;
class HSpecGeomPar;
class HEvent;
class HSpectrometer;
class HRuntimeDb;
class HMdcSeg;
class HGeomVector;
class HKickPlane;
class HKickCandidate;
class HKickImpact;
class HMdcHit;

class HKickMdcTrkF : public HKickTask {
public:
  HKickMdcTrkF(Text_t name[],Text_t title[]);
  ~HKickMdcTrkF(void);
  Int_t execute(void);
  Bool_t init(void);
  Bool_t finalize(void);
  void setConservativeMode(Bool_t m) { fConservativeMode=m; }

protected:
  inline Bool_t passesCuts(HKickCandidate &c);
  Bool_t isUnique(Int_t i,Int_t j);
  virtual void fillData(HKickCandidate &, HKickImpact *);
  void evaluateCandidates(void);
  void assignUniqueCombinations(void);
  void assignRemainingCombinations(void);
  virtual void fillControl(HKickCandidate &, HKickImpact *);
  virtual void evaluate(HKickImpact *imp, HMdcHit *hit, HKickCandidate &out);


  HMdcDetector *fMdc;
  HLocation fLaLocation;      //! Lever arm location.
  HLocation fSectorLoc; //! Sector location
  HMdcGeomPar *fMdcGeometry; //! MDC's geometry
  // HSpecGeomPar *fSpecGeometry;
  HCategory *fMdcInput;       //! 
  HCategory *fMdcOutput;      //!
  HCategory *fMdcHitInput;    //!
  HIterator *fMdcInputIter;   //!
  HIterator *fMdcHitInputIter;   //!
  HSpecGeomPar *fSpecGeomPar; //!
  TClonesArray *fImpacts;     //!List of impacts in kick plane.
  Int_t fNImpacts;            //Nb of valid impacts
  TObjArray fOuterHitArray;
  HKickPlane *fKickPlane;
  Float_t fMaxXPull; 
  Float_t fMaxD1[2];
  Float_t fMaxKickDist[2];
  Float_t fMinMomentum;
  Bool_t fConservativeMode;
  HKickCandidateMatrix fCandidates;
  HVertLsmFit fVertexFitter;

  Bool_t initMdc(HEvent *event,HRuntimeDb *rtdb,HSpectrometer *spec);
  virtual void readHits(HLocation &loc);
  void transform(HMdcSeg *hit, HGeomVector &r,HGeomVector &alpha);
  void transform(HMdcHit *hit, HGeomVector &r,HGeomVector &alpha);
  void readOuterHits(HLocation &levArmLoc);
public:
  ClassDef(HKickMdcTrkF,0) //Class for tracking in the mag. field
};


Bool_t HKickMdcTrkF::passesCuts(HKickCandidate &c) {
  //Checks if a given candidate passes all cuts to be considered a good candidate

  return ( !c.isAssigned() && 
	   ( (c.getDist() < fMaxD1[0] && c.getKickDist() < fMaxKickDist[0]) ||
	     (c.getDist() < fMaxD1[1] && c.getKickDist() < fMaxKickDist[1]) ) 
	   && c.getXPull() < fMaxXPull);
  //--------------------------------------------------------//
/*   Float_t aPhi=TMath::Abs(c.getXPull()); */
/*   return ( !c.isAssigned() &&  */
/* 	   ( aPhi < fMaxXPull ) && */
/* 	   (c.getMomentum() > fMinMomentum) ); */
}

#endif
