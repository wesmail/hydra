# ifndef HMDCSEGMENTF_H
# define HMDCSEGMENTF_H

# include "TH1.h"
# include "TH2.h"
# include "hlocation.h"
# include "hmdcleverarmgeometry.h"
# include "hmdcmodulegeometry.h"
# include "hreconstructor.h"
# include "hmdcsegmentfpar.h"

class HCategory;
class HMdcHit;
class HMdcSeg;

class HMdcSegmentF : public HReconstructor{

private:
  HCategory* fHitCat;                        // !category of hits
  HCategory* fSegCat;                        // !category of segments
  HMdcLeverArmGeometry* geoLever;            // !geometry of lever arm
  HMdcModuleGeometry* geoModules;                  // !geometry of modules
  HLocation fLoc;                            // location of the reconstructor

  HMdcSegmentFPar *parContainer;            // !parameter container of this
                                             // reconstructor
  Int_t fEventId;
   
  TH1F *hDiffX, *hDiffY, *hDiffSlopeX, *hDiffSlopeY;  // some histograms
  TH1F *hDiffXRec, *hDiffYRec, *hDiffSlopeXRec, *hDiffSlopeYRec;

  TH2F *hDiffYDiffX, *hDiffXSlopeDiffX, *hDiffYSlopeDiffX;
  TH2F *hDiffXSlopeDiffY, *hDiffYSlopeDiffY, *hDiffXSlopeDiffYSlope;

  TH1F *hProbEllipse;             // probability
  TH2F *hDiffYDiffXOverErr, *hDiffXSlopeDiffXOverErr, *hDiffYSlopeDiffXOverErr;
  TH2F *hDiffXSlopeDiffYOverErr, *hDiffYSlopeDiffYOverErr, *hDiffXSlopeDiffYSlopeOverErr;

  TH1F* hFitDiffX, *hFitDiffY, *hFitDiffXSlope, *hFitDiffYSlope, *hFitChiX, *hFitChiY, *hFitChi;


  void setParContainers(void);                // sets parameter containers
  void bookHisto(void);
  void fit(HMdcHit* hit1, Float_t z1, HMdcHit* hit2, Float_t z2, HMdcSeg* segment);

  void fitProjection(Float_t z1, Float_t x1, Float_t errorX1, Float_t slope1, Float_t errSlope1, Int_t n1, Float_t z2, Float_t x2, Float_t errorX2, Float_t slope2, Float_t errSlope2, Int_t n2, Float_t& a, Float_t& b, Float_t& errora, Float_t& errorb, Float_t& chi1);


public:
  HMdcSegmentF(void);
  HMdcSegmentF(Text_t* name, Text_t* title);
  ~HMdcSegmentF(void);

  void setLoc(HLocation& location);

  //void findParameters(void); //Borrado para entrega oficial
  Int_t execute(void);
  Bool_t init(void);
  Bool_t finalize(void){return kTRUE;}


ClassDef(HMdcSegmentF,1)

};
# endif              /* !HMDCSEGMENTF_H */

