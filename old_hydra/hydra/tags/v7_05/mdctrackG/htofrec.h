#ifndef  HTOFREC_H
#define  HTOFREC_H

#include "hreconstructor.h"
#include "hlocation.h"
#include "hevent.h"
#include "hrecevent.h"
#include "hcategory.h"
#include "hmatrixcategory.h"
#include "hlinearcategory.h"
#include "hmdcseg.h"
#include <TFile.h>
#include <TNtuple.h>

class HIterator;
class HCategory;

class HTofRec : public HReconstructor {

 protected:

  Bool_t fRichMode;            // flag for RICH mode (lepton ID)
  Bool_t fPlusMode;            // flag for PLUS mode (not used)
  Bool_t fMinusMode;           // flag for MINUS mode (pi- ID)
  Bool_t fDebugMode;           // flag for debug mode 
  Bool_t fillNTuple;           // flag to fill debuf ntuples
  Bool_t fKickHires;           // KickHires mode on 
  Bool_t fKickLowres;          // KickLowres mode on (not used)
  Bool_t fKickLowresB;         // KickLowresB mode on 
  Bool_t fSpline;              // Spline mode on
  Bool_t fRK;                  // RungeKutta mode on
  Bool_t fChiCutOn;            // Chi2> -1 cut
  Bool_t fSkip;                // Skip event if start is not reconstructed
  Bool_t isSimulation;         // Simulation mode ON 
  Bool_t isCommon;             // Common mode ON (time reconstructed from spline)

  HCategory* fSplineCat;       // Spline category
  HCategory* fKickLowCat;      // Kick lowres category
  HCategory* fKickLowBCat;     // kick LowresB category
  HCategory* fKickHiCat;       // Kick hires category
  HCategory* fRichCat;         // Rich category
  HCategory* fTofCat;          // Tof category
  HCategory* fTofinoCat;       // Tofino category
  HCategory* fMetaMatchCat;    // MetaMatch category
  HCategory* fMdcSegCat;       // HMdcSeg category
  HCategory* fMdcTrkCandCat;   // HMdcTrkCand category
  HCategory* fRKCat;           // RungeKutta category
  
  HIterator* iterMetaMatch;    // iterator on HMetaMatch
  HIterator* iterRich;         // iterator on HRichHit    
  
  Double_t C;                  // light speed
  Float_t rich_params[5];      // Rich correlation parameters (theta0, dtheta, phi0, dphi, lambda2)
  Float_t rich_qparams[4];     // Rich quality parameters (RingPatMat, RingPadNr, Centroid, RingAmplitude)
  Int_t spline_sec_on[6];      // Spline sectors used for tof reconstruction
  Float_t spline_sec_field[6]; // Spline sectors used for tof reconstruction
  Int_t kick123_sec_on[6];     // Hires kicktrack sectors used for tof reconstruction
  Int_t kick_sec_on[6];        // Lowres kicktrack sectors used for tof reconstruction 
  Int_t rk_sec_on[6];          // Runge Kutta kicktrack sectors used for tof reconstruction
  Int_t mdc_sec[6];            // Numbers of MDC modules for each sectors;
  Int_t common_flag;           // flag for common reconstruction
  
  TNtuple* before;
  TNtuple* after;
  TNtuple* richcorr;         // Ntuple for rich correlation
  
  TFile* r;
    
  void track_recalc(Float_t *start, Int_t *rec_ind, Int_t *flag); 
  Bool_t isRichCorr(HMdcSeg *mdc);
  
 public:    
  HTofRec(void);
  HTofRec(Text_t name[],Text_t title[],const char *select="", const char *track="");
  ~HTofRec(void);
  Bool_t init(void);
  Bool_t finalize(void);
  Int_t execute(void);
  
  void setRichMode(Bool_t r)         {fRichMode=r;}
  void setMinusMode(Bool_t m)        {fMinusMode=m;}
  void setPlusMode(Bool_t p)         {fMinusMode=p;}
  void setDebug(Bool_t d)            {fDebugMode=d;}
  void setFillNTuple(Bool_t fill)    {fillNTuple=fill;}
  void setRichParams(Float_t theta0, Float_t dtheta, Float_t phi0, Float_t dphi, Float_t lambda2) {
    // Rich algular correlation parameters {theta0, dtheta, phi0, dphi, maximum chi2}
    rich_params[0] = theta0;
    rich_params[1] = dtheta;
    rich_params[2] = phi0;
    rich_params[3] = dphi;
    rich_params[4] = lambda2;
  }
  void setQualityRichParams(Float_t RingPatMat, Float_t RingPadNr, Float_t Centroid, Float_t RingAmplitude) {
    // Rich quality parameters: {RingPatMat,RingPadNr,Centroid,RingAmplitude}
    rich_qparams[0] = RingPatMat;
    rich_qparams[1] = RingPadNr;
    rich_qparams[2] = Centroid;
    rich_qparams[3] = RingAmplitude;
  }
  void setSplineSec(Int_t spline_sec[6]) {
    // Flag used to switch on/off some sectors for tof reconstruction,
    // Default {1,1,1,1,1,1}.
    for (Int_t ii=0; ii<6; ii++) spline_sec_on[ii] = spline_sec[ii];    
  }
  void setSplineField(Float_t spline_sec[6]) {
    // Correction multiplier to spline momentum
    // Default {1,1,1,1,1,1}.
    for (Int_t ii=0; ii<6; ii++) spline_sec_field[ii] = spline_sec[ii];    
  }
  void setKickHiSec(Int_t kick123_sec[6]) {
    // Flag used to switch on/off some sectors for tof reconstruction,
    // Default {1,1,1,1,1,1}.
    for (Int_t ii=0; ii<6; ii++) kick123_sec_on[ii] = kick123_sec[ii];    
  }
  void setKickLowSec(Int_t kick_sec[6]) {
    // Flag used to switch on/off some sectors for tof reconstruction,
    // Default {1,1,1,1,1,1}.
    for (Int_t ii=0; ii<6; ii++) kick_sec_on[ii] = kick_sec[ii];    
  } 
  void setRKSec(Int_t rk_sec[6]) {
    // Flag used to switch on/off some sectors for tof reconstruction,
    // Default {1,1,1,1,1,1}.
    for (Int_t ii=0; ii<6; ii++) rk_sec_on[ii] = rk_sec[ii];    
  }

  
  
  ClassDef(HTofRec,0)
    };
#endif /* !HTOFREC_H */







