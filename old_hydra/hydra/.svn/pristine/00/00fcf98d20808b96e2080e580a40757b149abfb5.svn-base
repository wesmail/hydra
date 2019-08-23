/***************************************************************************
 *   Author :                                                              *
 *             II Physikalisches Institut                                  *
 *             Universitaet Giessen                                        *
 *            								   *
 *   Project:  HYDRA Event Hypothesys  (HADES)                             *
 *   Description:   						           *
 *            								   *
 *   Created  :   2005-02-08 Ingo                                          *
 *   Modified :                                                            *
 *            								   *
 ***************************************************************************/
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

class HHypPPPipPimProjector : public HHypBaseAlgorithm
{

  // ***************************************************************************
 private:

  TNtuple * miss;
  Int_t simuflag;
  HCategory * simCat;
  HCategory * CatPartSim;

protected:

  

public:

  HHypPPPipPimProjector(char *name_i=NULL); 
  
  virtual ~HHypPPPipPimProjector(void);

  Bool_t execute();
  Bool_t init();
  Bool_t reinit();
  Bool_t finalize();
  
  ClassDef(HHypPPPipPimProjector,0)
};

//============================================================================

#endif  

