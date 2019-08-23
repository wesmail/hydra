//*-- AUTHOR : B. Spruck
//*-- Modified : 20/4/2005

#ifndef HHYP_PP_EP_EM_PROJECTOR_H
#define HHYP_PP_EP_EM_PROJECTOR_H

#include <TObject.h>
#include <TArrayI.h>
#include <TNtuple.h>
#include <TFile.h>
#include "hhypbasealgorithm.h"
#include <hcategory.h>
#include <hpidparticlesim.h>
#include <hpidgeanttrackset.h>
#include <hgeantkine.h>
#include <piddef.h>

// ***************************************************************************

class HHypPPEpEmProjector:public HHypBaseAlgorithm
{

  // ***************************************************************************
private:

  TNtuple        *miss;
  Int_t           simuflag;
  HCategory      *simCat;
  HCategory      *CatPartSim;

protected:

public:

     
  HHypPPEpEmProjector(char *name_i = NULL, Option_t par[] = NULL);
  
  virtual ~ HHypPPEpEmProjector(void);

  Bool_t          execute();
  Bool_t          init();
  Bool_t          reinit();
  Bool_t          finalize();

  ClassDef(HHypPPEpEmProjector, 0)      // ntuple creation for PPEmEp
};

//============================================================================

#endif
