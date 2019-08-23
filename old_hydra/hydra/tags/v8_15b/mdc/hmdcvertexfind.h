#ifndef HMDCVERTEXFIND_H
#define HMDCVERTEXFIND_H

#include "hreconstructor.h"
#include "hgeomvector.h"
#include "hgeomvertexfit.h"
#include "hspecgeompar.h"
#include "TIterator.h"
#include "TNtuple.h"
#include "TFile.h"
#include "TClonesArray.h"

class HCategory;
class HMdcGeomPar;
class HIterator;
class HMdcSeg;

class HMdcVertexFind : public HReconstructor {
public:
  enum EInputMode {kSegments,kHits};
  HMdcVertexFind(Text_t name[] = "",Text_t title[] = "",EInputMode m = kSegments,Bool_t tukey = kFALSE);
  virtual ~HMdcVertexFind(void);
  Bool_t init(void);
  Bool_t finalize(void);
  Int_t  execute(void);
  void        useTukeyWeights        (Bool_t flag) { fUsingTukey    = flag; }
  void        setDebugMode           (Bool_t flag) { fDebugMode     = flag; }
  void        setTukeyConstant       (Double_t d)  { fTukeyConst    = d; }
  void        setMaxIterations       (Int_t max)   { fMaxIterations = max; }
  void        setEpsilon             (Double_t ep) { fEpsilon       = ep; }
  EInputMode  getInputMode           (void) { return fInputMode; }
  Double_t    getTukeyConstant       (void) { return fTukeyConst; }
  Int_t       getMaxIterations       (void) { return fMaxIterations; }
  Double_t    getEpsilon             (void) { return fEpsilon; }
  static void setRejectEmbeddedTracks(Bool_t reject = kTRUE){rejectEmbeddedTracks = reject;}
  static void setUseEventSeqNumber   (Bool_t use    = kTRUE){useEventSeqNumber    = use;   }
  static void setSkipNoVertex        (Bool_t skip   = kTRUE){doSkipNoVertex       = skip;  }
protected:
  void initVars(void);
  Bool_t pointsToTarget(const HGeomVector &r,HGeomVector &alpha,Int_t sector,Int_t module);
  Bool_t readSegments  (HGeomVector &estimate);
  Bool_t readHits      (HGeomVector &estimate);
  void transform       (HMdcSeg *data, HGeomVector &r, HGeomVector &alpha);
  Bool_t hasConverged  (HGeomVector &v,HGeomVector &oldV);

  HGeomVertexFit fFitter;              //!
  TNtuple        *fControl;            //!
  TClonesArray   fPos;                 //! r for each track
  TClonesArray   fAlpha;               //! alpha vector for each track
  HCategory      *fInput;              //! Geant MDC data
  HMdcGeomPar    *fGeometry;           //! Geometry transformation
  HSpecGeomPar   *fSpecGeometry;       //! Spectrometer's geometry
  HIterator      *fIter;               //!
  Double_t       fTukeyConst;          // Value used as Tukey constant
  Int_t          fMaxIterations;       // Maximum number of iterations
  Bool_t         fUsingTukey;          // Flag turning on and off Tukey weights
  Bool_t         fDebugMode;           // Debug mode on/off
  Double_t       fEpsilon;             // Used for convergence criterium
  EInputMode     fInputMode;           // Read segments or hits?
  Bool_t         isEmbedding;          //! kTRUE if is in embedding mode
  static Bool_t  rejectEmbeddedTracks; //! = kTRUE (default) reject embedded tracks from vertex calculation
  static Bool_t  useEventSeqNumber;    //! use the eventSeq Number decide of event skipping in embedding mode
  static Bool_t  doSkipNoVertex;       //! skip events if no vertex is calculated
  TIterator*     geantKineIter;        //! Iterator over GeantKine category

public:
 
  ClassDef(HMdcVertexFind,1)
};

#endif
