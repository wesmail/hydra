#ifndef HKICKRESOLUTION_H
#define HKICKRESOLUTION_H

#include "hreconstructor.h"
#include "hgeomvector.h"
#include "hgeomvertexfit.h"
#include "hkickplane.h"
#include "hkicktask.h"
#include "TNtuple.h"
#include "TFile.h"
#include "hkickplane2.h"

class HCategory;
class HMdcGeomPar;
class HIterator;
class HGeantMdc;
class HTofGeomPar;
class HGeantTof;


class HKickResolution : public HKickTask {
protected:
  Int_t fEvCounter;
  void initVars(void);
  TNtuple *fData,*fControl;     //Ntupla con resultados.

  HGeomVertexFit fFitter;
  HKickPlane *fKickPlane; //Kick plane.
  HCategory *fInput; // Geant MDC data
  HCategory *fTofInput;
  HCategory *fKine;  // Kine Data from Geant
  HMdcGeomPar *fGeometry; //Geometry transformation
  HTofGeomPar *fTofGeometry; //TOF geometry
  HIterator *fIter;
  HIterator *fTofIter;
  void transform(HGeantMdc *data,HGeomVector &r,HGeomVector &alpha);
  void transform(HGeantTof *data,HGeomVector &r,HGeomVector &err,
		 Double_t &k,Double_t &offset); // zRod = k*yRod + offset.
  Bool_t fUsingTofResol;
  Bool_t fUsingMdcResol;
public:
  HKickResolution(void);
  HKickResolution(Text_t name[],Text_t title[]);
  ~HKickResolution(void);
  Bool_t init(void);
  Bool_t finalize(void);
  //Int_t execute(void);
  void setUsingTofResolution(Bool_t f) { fUsingTofResol = f; }
  Bool_t usingTofResolution(void) { return fUsingTofResol; }
  void setUsingMdcResolution(Bool_t f) { fUsingMdcResol = f; }
  Bool_t usingMdcResolution(void) { return fUsingMdcResol; }
  ClassDef(HKickResolution,0)
};

class HKickResolutionTof : public HKickResolution {
protected:
  HKickPlane2 *fKickPlane; //Kick plane.
public:
  HKickResolutionTof(Text_t name[],Text_t title[]) : HKickResolution(name,title) {}
  ~HKickResolutionTof(void) {}
  Int_t execute(void);
  Bool_t init(void);
  ClassDef(HKickResolutionTof,0)
};


class HKickResolutionMdc : public HKickResolution {
protected:
  HKickPlane *fKickPlane; //Kick plane.
public:
  HKickResolutionMdc(Text_t name[],Text_t title[]) : HKickResolution(name,title) {}
  ~HKickResolutionMdc(void) {}
  Int_t execute(void);
  Bool_t init(void);
  ClassDef(HKickResolutionMdc,0)
};

class HKickResolutionMdc2 : public HKickResolution {
protected:
  enum EMode {kSlopeMode,kKickPosMode,kMdcTofMode,kMdc34Mode} fMode;
  HKickPlane2 *fKickPlane; //Kick plane.
  Bool_t fUseKickPosition; //Use kickplane position to build outer segment
public:
  HKickResolutionMdc2(Text_t name[],Text_t title[]) : HKickResolution(name,title) {fMode=kSlopeMode;}
  ~HKickResolutionMdc2(void) {}
  Int_t execute(void);
  Bool_t init(void);
  void useKickPosition(void) { fMode = kKickPosMode; }
  void useMdcTof(void) { fMode = kMdcTofMode; }
  void useMdc34(void) { fMode = kMdc34Mode; }
  ClassDef(HKickResolutionMdc2,0)
};

#endif
