#ifndef HKICKCANDIDATEEVALUATOR_H
#define HKICKCANDIDATEEVALUATOR_H

#include <TObject.h>
#include "hgeomvector.h"
#include "hkickplane.h"
#include "hkickcandidate.h"
#include "hkickmassfit.h"

class HTofGeomPar;
class HSpecGeomPar;
class HKickImpact;
class HTofHit;
class HShowerHit;
class HMdcGeomPar;
class HShowerGeometry;

class HKickCandidateEvaluator : public TObject {
protected:
  HKickMassFit fMassFitter;
  Float_t fTofXResolution; //Position resolution of TOF in X
  Double_t fMass,fMassErr;
  Double_t fGhi1,fGhi2,fPhi1,fPhi2; //Deflection angles
  Double_t fP; //Fitted momentum
  Double_t fSinGhi,fSinPhi,fPredictedSinPhi; 
  HGeomVector fFarPos,fFarDir,fFarPosErr;
  Double_t fFarPlaneSlope; //Slope of TOF rod or Shower pad
  Double_t fFarPlaneOffset; //Offset of TOF rod or Shower pad
  HKickPlane *fKickPlane; //! Not owner
  HShowerGeometry *fShoGeometry; //!Shower geometry. Not owner
  HMdcGeomPar *fMdcGeometry; //!Mdc geometry. Not owner
  HTofGeomPar *fTofGeometry; //!TOF geometry. Not owner
  HSpecGeomPar *fSpecGeometry; //!TOF geometry. Not owner
  Double_t fDistanceToFarHit; //dist. from kick impact to far hit.
  Bool_t fUseSimTof; //FIXME: SIM: Use HTofHitSim

  void evaluate(HKickImpact *imp,HKickCandidate &out);
  void transform(HTofHit *data,HGeomVector &r);
  void transform(HShowerHit *data);
public:
  HKickCandidateEvaluator(HKickPlane *k)   { fKickPlane=k; fUseSimTof=kFALSE;}
  ~HKickCandidateEvaluator(void) {}
  void setTofGeometry(HTofGeomPar *tof)    { fTofGeometry=tof; }
  void setSpecGeometry(HSpecGeomPar *spec) { fSpecGeometry=spec; }
  void setMdcGeometry(HMdcGeomPar *mdc)    { fMdcGeometry = mdc; }
  void setShowerGeometry(HShowerGeometry *sho) { fShoGeometry = sho; }
  void setSimulatedTof(void)                   { fUseSimTof=kTRUE; }
  void setTofXResolution(Float_t f)            { fTofXResolution = f;}
  Bool_t isTofSimulated(void)                  { return fUseSimTof; }
  virtual void evaluate(HKickImpact *imp,HTofHit *hit,
			HKickCandidate &out);
  virtual void evaluate(HKickImpact *imp,HShowerHit *hit,HKickCandidate &out);
 
  ClassDef(HKickCandidateEvaluator,0) //Evaluator of track candidates
};

#endif
