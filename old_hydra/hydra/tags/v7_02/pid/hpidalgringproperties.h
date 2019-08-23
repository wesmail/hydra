// @(#)$Id: hpidalgringproperties.h,v 1.4 2004-04-30 15:08:08 christ Exp $
//*-- Author : Tassilo Christ 23/04/2002

#ifndef HPIDALGRINGPROPERTIES_H
#define HPIDALGRINGPROPERTIES_H

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// HPidAlgRingProperties                                                      //
//                                                                            //
// Test algorithm for PID from RICH ring observables                          //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include "hpidalgorithm.h"


// -----------------------------------------------------------------------------

class HPidAlgRingPropertiesPar;
class TVector;
// -----------------------------------------------------------------------------

class HPidAlgRingProperties : public HPidAlgorithm
{
private:

  HPidAlgRingPropertiesPar *pParams;
  Double_t RingPattern5D[5];
  Double_t RingPattern4D[4];
  Double_t TransformedRingPattern5D[5];
  Double_t TransformedRingPattern4D[4];

 public:
  
  HPidAlgRingProperties(void);
  HPidAlgRingProperties(Float_t fWeight);
  ~HPidAlgRingProperties(void) {}
  
  // -------------------------------------------------------------------------
  
  enum RingCentroid {
    CentroidDefined=0, //The ring does have a well defined centroid
    CentroidUndefined=1 //The centroid could not be computed by the ring finder
  };
 
  virtual Bool_t      correctRelativeYields(Float_t* pdfValues,Float_t TrackMomentum);

  virtual Bool_t      TransformToEigenspace5D(Double_t* OriginalData, Double_t* TransformedData);
  virtual Bool_t      TransformToEigenspace4D(Double_t* OriginalData, Double_t* TransformedData);
  

  virtual Bool_t      init(void);
  virtual Bool_t      reinit(void);
  virtual Bool_t      finalize(void);
  
  virtual Bool_t      calculatePDF(HPidTrackCand *pTrack,
				   Float_t afReturn[], Short_t &nCatIndex);
  virtual Bool_t      calculateCL(HPidTrackCand *pTrack,
				  Float_t afReturn[], Short_t &nCatIndex);
  
  virtual void        print(void) const;
  
  Int_t               getThetaBin(Float_t fKickTheta);

  // -------------------------------------------------------------------------
  
    ClassDef(HPidAlgRingProperties, 0)  // PID Algorithm for Lepton ID using RICH observables
};

// -----------------------------------------------------------------------------

#endif //HPIDALGRINGPROPERTIES_H
