//*-- AUTHOR : I. Froehlich
//*-- Modified : 21/04/2005 by I. Froehlich

#ifndef HHYP_PP_KP_PIM_MISS0_PROJECTOR_H
#define HHYP_PP_KP_PIM_MISS0_PROJECTOR_H

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

class HHypPPKpPimMiss0Projector : public HHypBaseAlgorithm
{

  // ***************************************************************************
 private:

  TNtuple * miss;
  Int_t simuflag;
  HCategory * simCat;
  HCategory * CatPartSim;

protected:

  

public:

  HHypPPKpPimMiss0Projector(char *name_i=NULL, Option_t par[]=NULL); 
  
  virtual ~HHypPPKpPimMiss0Projector(void);

  Bool_t execute();
  Bool_t init();
  Bool_t reinit();
  Bool_t finalize();
  
  ClassDef(HHypPPKpPimMiss0Projector,0) //Projects events to Ntuple
};

//============================================================================

#endif  

