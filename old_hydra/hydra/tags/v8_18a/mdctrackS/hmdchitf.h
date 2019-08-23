# ifndef HMDCHITF_H
# define HMDCHITF_H

# include "TFile.h"
# include "TH1.h"
# include "TMatrix.h"
# include "hlocation.h"
# include "hreconstructor.h"


class HCategory;
class HMatrixCategory;
class HMdcCal2;
class HMdcCalLinkList;
class HMdcModuleGeometry;
class HMdcTargetGeometry;
class HMdcCalHit;
class HMdcCal3;
class HMdcHitAux;
class HMdcHitFPar;


class HMdcHitF : public HReconstructor {

protected:
  HMdcHitFPar* parContainer;    // !parameter container
  HMdcTargetGeometry* targetGeo;       // !target geometry
  HCategory* mdcCal;                // !calibrated data to be fitted
  HMdcModuleGeometry* geo;                // !geometry data
  HCategory* fCalCat;               // ! category of cal2 data
  HCategory* mdcSegments;           // !segments produced after fitting
  HCategory* fHitCat;               // ! category of reconstructed hits
  HMdcCalLinkList** fList;          //! lists of cal hits (internal use only)

  HLocation fLoc;                  // !location of the reconstructor
  Int_t fEventId;                  

  Bool_t target;                  // ! target = kFALSE==>rec. without target 
  Bool_t slopeCorrection;         // ! slopeCorrection = kTRUE==>rec. with makeSlopeCorrection

  virtual void buildList(void);            // builds the Double_t linked list
  Float_t calculatePosPartner(HMdcCal3* hit);
  Int_t calculateCell(Int_t sector, Int_t module, Int_t layer, Float_t coordinate);

  void fit(HMdcHitAux* seg);
  void makeSlopeCorrection();

  Int_t find(Int_t listPlanes[6], Int_t nWantedHits, Int_t segPos);
  void markHits(Int_t first, Int_t last);
  Bool_t checkCommon(HMdcHitAux* segment);   // check if segment shares   
                                              // maxNumberCommonHits or more
                                              // with the other segments in
                                              // the segments collection
  Bool_t firstCheck(TMatrix param);
  Bool_t checkFit(HMdcHitAux* segment);

  void fillHitCat(void);                     
  virtual Text_t *getCalHitClassName(void) { return "HMdcCal3"; }

public:
  HMdcHitF(void);
  HMdcHitF(Text_t* name, Text_t* title);
  HMdcHitF(HMatrixCategory* data, HMdcModuleGeometry* mdcGeo, HCategory* seg);
  HMdcHitF(HCategory* data, HMdcModuleGeometry* mdcGeo, HCategory *seg);

  ~HMdcHitF(void);

  void setNoTarget(void){target = kFALSE;}  
  void setSlopeCorrection(void){slopeCorrection = kTRUE;}
  void setLoc(HLocation& location);
  void setParContainer(void);
  HMdcHitFPar* getParContainer(void){return parContainer;}

  Int_t execute(void);
  Bool_t init(void);
  Bool_t finalize(void){return kTRUE;}

  void printSegments(ofstream &file);
  void printCalHitCat(ofstream &file);

  ClassDef(HMdcHitF,1)
};



# endif            /* !HMDCHITF_H */
 





 
