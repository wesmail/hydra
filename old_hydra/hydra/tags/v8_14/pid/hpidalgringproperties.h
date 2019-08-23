// @(#)$Id: hpidalgringproperties.h,v 1.9 2006-12-01 15:42:26 halo Exp $
//*-- Author : Tassilo Christ 23/04/2002

#ifndef HPIDALGRINGPROPERTIES_H
#define HPIDALGRINGPROPERTIES_H

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// HPidAlgRingProperties                                                      //
//                                                                            //
// Pid method using RICH ring observables                                     //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include "hpidalgorithm.h"


// -----------------------------------------------------------------------------

class HPidAlgRingPropertiesPar;
// -----------------------------------------------------------------------------

class HPidAlgRingProperties : public HPidAlgorithm
{
private:

  HPidAlgRingPropertiesPar *pParams;
  
  //Obsolete Variables - not needed any more until Principal Component Analysis is revived
  Double_t RingPattern5D[5];
  Double_t RingPattern4D[4];
  Double_t TransformedRingPattern5D[5];
  Double_t TransformedRingPattern4D[4];
  //End obsolete section

 public:
  
  HPidAlgRingProperties(void);
  HPidAlgRingProperties(Float_t fWeight);
  ~HPidAlgRingProperties(void) {}
  
  // -------------------------------------------------------------------------
  
  virtual Bool_t      correctRelativeYields(Float_t* pdfValues,Float_t TrackMomentum, 
					    Float_t fKickTheta, Int_t sector);

  //Obsolete until revival of PCA
  virtual Bool_t      TransformToEigenspace5D(Double_t* OriginalData, Double_t* TransformedData,Int_t sector);
  virtual Bool_t      TransformToEigenspace4D(Double_t* OriginalData, Double_t* TransformedData, Int_t sector);
  //End obsolete section
  
  //All reconstructors must overload these functions
  virtual Bool_t      init(void);
  virtual Bool_t      reinit(void);
  virtual Bool_t      finalize(void);

  //compute and store the PDF values given by the RICH histograms
  virtual Bool_t      calculatePDF(HPidTrackCand *pTrack,
				   Float_t afReturn[], Short_t &nCatIndex);
  
  virtual void        print(void) const;

  // -------------------------------------------------------------------------
  
    ClassDef(HPidAlgRingProperties, 0)  // PID Algorithm for Lepton ID using RICH observables
};

// -----------------------------------------------------------------------------

#endif //HPIDALGRINGPROPERTIES_H
