//*-- AUTHOR : B. Spruck
//*-- CREATED : 30.11.2006

#ifndef HHYP_ALL_PROJECTOR_H
#define HHYP_ALL_PROJECTOR_H

#include <TObject.h>
#include <TArrayI.h>
#include <TNtuple.h>
#include <TFile.h>
#include <TVector3.h>
#include "hhypbasealgorithm.h"
#include <piddef.h>

// ***************************************************************************

class HHypAllProjector:public HHypBaseAlgorithm
{

  // ***************************************************************************
private:

  TNtuple   *qa;         //! where we store the values
  TVector3  momentum[HYP_MAX_PARTICLES];   //! momentum for HYP_MAX_PARTICLES (->set in filler)
  Int_t     pidofPart[HYP_MAX_PARTICLES];  //! Hyp-pids of HYP_MAX_PARTICLES (-> set in filler)

protected:

public:
     
  HHypAllProjector(char *name_i = NULL, Option_t par[] = NULL);
  
  virtual ~ HHypAllProjector(void);

  Bool_t          execute();
  Bool_t          init();
  Bool_t          reinit();
  Bool_t          finalize();

  ClassDef(HHypAllProjector, 0)    //Projects events to Ntuple
};

//============================================================================

#endif
