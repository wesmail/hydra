//*-- AUTHOR : M. Destefanis
//*-- Modified : 26/08/2005 by M. Destefanis

#ifndef HHYP_P_KP_LAMBDA_MISS0_PROJECTOR_H
#define HHYP_P_KP_LAMBDA_MISS0_PROJECTOR_H

#include "TObject.h"
#include "TArrayI.h"
#include "TNtuple.h"
#include "TFile.h"
#include "hhypbasealgorithm.h"
#include <hcategory.h>
#include <hpidtrackcandsim.h>
#include <hpidgeanttrackset.h>
#include <hgeantkine.h>
#include <piddef.h>


// ***************************************************************************

class HHypPKpLambdaMiss0Projector : public HHypBaseAlgorithm
{

  // ***************************************************************************
 private:

  TNtuple * miss;
  Int_t simuflag;
  HCategory * simCat;
  HCategory * CatTrackCandSim;

protected:

  

public:

  HHypPKpLambdaMiss0Projector(Char_t *name_i=NULL, Option_t par[]=NULL); 
  
  virtual ~HHypPKpLambdaMiss0Projector(void);

  Bool_t execute();
  Bool_t init();
  Bool_t reinit();
  Bool_t finalize();
  
  ClassDef(HHypPKpLambdaMiss0Projector,0) //Projects events to Ntuple
};

//============================================================================

#endif  

