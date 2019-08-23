//*-- Author : Stefano Spataro (spataro@lns.infn.it)
using namespace std;

#ifndef  HPIDTOFREC_H
#define  HPIDTOFREC_H

#include "hreconstructor.h"
#include "hcategory.h"
#include "hiterator.h"
#include "TString.h"
#include "TNtuple.h"
#include "piddef.h"

class HIterator;
class HCategory;
class HMatrixCategory;
class HLinearCategory;
class HPidAlgStandCutsPar;
class HPidTrackCand;

class HPidTofRec : public HReconstructor {

 protected:

  Bool_t bRichMode;                // flag for RICH mode (lepton ID)
  Bool_t bMinusMode;               // flag for MINUS mode (pi- ID)
  Bool_t bFillNTuple;              // flag to fill debug ntuples
  Bool_t bTrackAlg[MAX_MOM_ALGS];  // Switch ON Tracking algorythms
  Bool_t bChiCutInner;             // Chi2> -1 cut for inner MDC segment
  Bool_t bChiCutOuter;             // Chi2> -1 cut for outer MDC segment
  Bool_t bSkip;                    // Skip event if start is not reconstructed
  Bool_t bIsCommon;                // Common mode ON (time reconstructed from only one algorythm)
  Bool_t bIsFirst;                 // Is first event
  Bool_t bIsSimulation;            // Simulation flag (only for debug purpose)
  Bool_t bPidParam;                // get rich params from pid param containers
  Bool_t bSetStartHit;             // flag to set HStartHit with reconstructed TOF (default kFALSE)
  Bool_t isEmbedding;              // = kTRUE if we are running embedding
  HCategory* pTrackCand;           // HPidTrackCand category
  HIterator* iterTrackCand;        // iterator on HPidTrackCand
  HLinearCategory* pStartHitCat;   // pointer to start hit category
  HCategory* pShowerHitTofCat;     // pointer to showerhittof category
  HMatrixCategory* pTofinoCalCat;  // pointer to tofinocal  category
  HPidAlgStandCutsPar *richParam;  // new source of RICH quality parameters
                                   // iRingPatMatrThresh, fRingCentroidThresh,
                                   // iRingPadNrThres, fRingAvChargeThres
  Int_t pidalg;                    // algorithm number
  Float_t fRichQparams[4];         // Rich quality parameters (RingPatMat, RingPadNr, Centroid, RingAmplitude)
  Short_t nMdcSeg[6];              // Number of MDC modules for each sectors;
  Short_t nCommonFlag;             // flag for common reconstruction
  Short_t nStartAlg;               // flag for filling HStartHit

  TNtuple* hit;                    // Ntuple for debug
  TNtuple* richcorr;               // Ntuple for rich correlation
  TString sDir;                    // Ntuple output directory
  TString sFile;                   // Ntuple output file
  Bool_t isRealTrack(HPidTrackCand*);

 public:    
  HPidTofRec(void);
  HPidTofRec(const Text_t name[],const Text_t title[],const Char_t *select="", const Char_t *track="");
  ~HPidTofRec(void);
  Bool_t init(void);
  Bool_t reinit(void);
  Bool_t finalize(void);
  Int_t  execute(void);
  void setQualityRichParams(Float_t RingPatMat, Float_t RingPadNr, Float_t Centroid, Float_t RingverageCharge) {
    // Rich quality parameters: {RingPatMat,RingPadNr,Centroid,RingAverageCharge}
    fRichQparams[0] = RingPatMat;
    fRichQparams[1] = RingPadNr;
    fRichQparams[2] = Centroid;
    fRichQparams[3] = RingverageCharge;
  }
  void setOutputDir(const Char_t *fdir) {sDir = fdir;}
  void setOutputFile(const Char_t *ffile) {sFile = ffile;}
  
  
  ClassDef(HPidTofRec,0)
};
#endif /* !HPIDTOFREC_H */







