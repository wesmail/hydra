
#ifndef HKICKCANDIDATEEVALUATOR_H
#define HKICKCANDIDATEEVALUATOR_H

#include <TObject.h>
#include "hgeomvector.h"
#include "hkickplane2.h"
#include "hkickmatchpar.h"
#include "hkickcandidate.h"
#include "hkickmassfit.h"

class HTofGeomPar;
class HSpecGeomPar;
class HKickImpact;
class HTofHit;
class HTofCluster;
class HShowerHit;
class HMdcGeomPar;
class HShowerGeometry;
class HTofinoGeomPar;

class HKickCandidateEvaluator : public TObject {
protected:
  HKickMassFit fMassFitter;
  Float_t fTofXResolution; //Position resolution of TOF in X
  Float_t fMass,fMassErr;
  Float_t fGhi1,fGhi2,fPhi1,fPhi2; //Deflection angles
  Float_t fP; //Fitted momentum
  Float_t fSinGhi,fSinPhi,fPredictedSinPhi; 
  HGeomVector fFarPos,fFarDir,fFarPosErr;
  Double_t fFarPlaneSlope; //Slope of TOF rod or Shower pad
  Double_t fFarPlaneOffset; //Offset of TOF rod or Shower pad
  HKickPlane2 *fKickPlane; //! Not owner
  HKickMatchPar *fMatchPar; //! Not owner
  HShowerGeometry *fShoGeometry; //!Shower geometry. Not owner
  HMdcGeomPar *fMdcGeometry; //!Mdc geometry. Not owner
  HTofGeomPar *fTofGeometry; //!TOF geometry. Not owner
  HTofinoGeomPar *fTofinoGeometry; //! Tofino geometry
  HSpecGeomPar *fSpecGeometry; //!TOF geometry. Not owner
  Double_t fDistanceToFarHit; //dist. from kick impact to far hit.
  HGeomVector fMetaNormVec;  // vector perpendicular to the META wall
  Bool_t fUseSimTof; //FIXME: SIM: Use HTofHitSim

  void evaluate(HKickImpact *imp,HKickCandidate &out);
  void transform(HTofHit *data,HGeomVector &r);
  void transform(HTofCluster *data,HGeomVector &r);
  void transform(HShowerHit *data);
public:
  HKickCandidateEvaluator(HKickPlane2 *k,HKickMatchPar *p)   
    { fKickPlane=k; fUseSimTof=kFALSE; fMatchPar= p;}
  ~HKickCandidateEvaluator(void) {}
  void setTofGeometry(HTofGeomPar *tof)    { fTofGeometry=tof; }
  void setKickPlane(HKickPlane2 *k, HKickMatchPar *p)    { //FIXME 
	  fKickPlane = k;
	  fMatchPar = p;
  }
  void setTofinoGeometry(HTofinoGeomPar *tofino) { fTofinoGeometry = tofino; }
  void setSpecGeometry(HSpecGeomPar *spec) { fSpecGeometry=spec; }
  void setMdcGeometry(HMdcGeomPar *mdc)    { fMdcGeometry = mdc; }
  void setShowerGeometry(HShowerGeometry *sho) { fShoGeometry = sho; }
  void setSimulatedTof(void)                   { fUseSimTof=kTRUE; }
  void setTofXResolution(Float_t f)            { fTofXResolution = f;}
  Bool_t isTofSimulated(void)                  { return fUseSimTof; }
  virtual void evaluate(HKickImpact *imp,HTofHit *hit,
			HKickCandidate &out);
  virtual void evaluate(HKickImpact *imp,HTofCluster *cluster,
			HKickCandidate &out);
  virtual void evaluate(HKickImpact *imp,HShowerHit *hit,HKickCandidate &out);
 
  ClassDef(HKickCandidateEvaluator,0) //Evaluator of track candidates
};

#endif
