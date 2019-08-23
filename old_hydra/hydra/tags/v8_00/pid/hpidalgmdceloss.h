// @(#)$Id: hpidalgmdceloss.h,v 1.1 2006-08-16 16:12:53 halo Exp $
//*-- Author : Tassilo Christ 23/04/2002

#ifndef HPIDALGMDCELOSS_H
#define HPIDALGMDCELOSS_H

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// HPidAlgMdceloss                                                            //
//                                                                            //
// Pid method using MDC Eloss observables                                     //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include "hpidalgorithm.h"


// -----------------------------------------------------------------------------

class HPidAlgMdcElossPar;
class TVector;
// -----------------------------------------------------------------------------

class HPidAlgMdcEloss : public HPidAlgorithm
{
private:

  HPidAlgMdcElossPar *pParams;
  

 public:
  
  HPidAlgMdcEloss(void);
  HPidAlgMdcEloss(Float_t fWeight);
  ~HPidAlgMdcEloss(void) {}
  
  // -------------------------------------------------------------------------
  
  
  //All reconstructors must overload these functions
  virtual Bool_t      init(void);
  virtual Bool_t      reinit(void);
  virtual Bool_t      finalize(void);

  //compute and store the PDF values given by the RICH histograms
  virtual Bool_t      calculatePDF(HPidTrackCand *pTrack,
				   Float_t afReturn[], Short_t &nCatIndex);
  
  virtual void        print(void) const;

  // -------------------------------------------------------------------------
  
    ClassDef(HPidAlgMdcEloss, 0)  // PID Algorithm for Lepton ID using RICH observables
};

// -----------------------------------------------------------------------------

#endif //HPIDALGMDCELOSS_H
