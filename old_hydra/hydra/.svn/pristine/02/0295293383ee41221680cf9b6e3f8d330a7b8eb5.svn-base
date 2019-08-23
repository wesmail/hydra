//*-- AUTHOR : I. Froehlich
//*-- Modified : 21/04/2005 by I. Froehlich

#ifndef HHYP_PP_PIP_PIM_PROJECTOR_H
#define HHYP_PP_PIP_PIM_PROJECTOR_H

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

class HHypPPPipPimProjector:public HHypBaseAlgorithm
{

  // ***************************************************************************
private:

  TNtuple        *miss;      // where we store the values
  TNtuple        *header;    // where we store header info like downscaling...
  Int_t           simuflag;  // if simulation
  HCategory      *simCat;
  HCategory      *CatPartSim;

protected:

public:
     
  HHypPPPipPimProjector(char *name_i = NULL, Option_t par[] = NULL);
  
  virtual ~ HHypPPPipPimProjector(void);

  Bool_t          execute();
  Bool_t          init();
  Bool_t          reinit();
  Bool_t          finalize();

  ClassDef(HHypPPPipPimProjector, 0)    //Projects events to Ntuple
};

//============================================================================

#endif
