//*-- AUTHOR : Tiago Perez
//*-- Modified : 05/01/2005 by Tiago Perez
//*-- Modified : 04/07/2005 by I. Froehlich

#ifndef HHYP_PP_PIP_PIM_PI0_H
#define HHYP_PP_PIP_PIM_PI0_H

#include <TObject.h>
#include <TArrayI.h>
#include "hhypbasealgorithm.h"
#include "hhyplistfiller.h"

// ***************************************************************************

class HHypPPPipPimPi0Alg: public HHypBaseAlgorithm
{

  // ***************************************************************************
private:
  
  Float_t          lower_pi_cut, upper_pi_cut; //PI0 cuts values from pParams
  
protected:

public:

  HHypPPPipPimPi0Alg(char *name_i = NULL, Option_t par[] = NULL);

  virtual         ~HHypPPPipPimPi0Alg(void);

  Bool_t          execute();
  Bool_t          init();
  Bool_t          reinit();
  Bool_t          finalize();

  ClassDef(HHypPPPipPimPi0Alg, 0)  // Cutting on miss4==Pi0
};

//============================================================================

#endif
