//*-- AUTHOR : Tiago Perez
//*-- Modified : 05/01/2005 by Tiago Perez
//*-- Modified : 04/07/2005 by I. Froehlich
//*-- Modified : 20/01/2006 by T. Perez
#ifndef HHYP_PP_PIP_PIM_GAMMA_H
#define HHYP_PP_PIP_PIM_GAMMA_H

#include <TObject.h>
#include <TArrayI.h>
#include "hhypbasealgorithm.h"
#include "hhyplistfiller.h"

// ***************************************************************************

class HHypPPPipPimGammaAlg: public HHypBaseAlgorithm
{

  // ***************************************************************************
private:
  
  //  Float_t          lower_pi_cut, upper_pi_cut; //GAMMA cuts values from pParams
  
protected:

public:

  HHypPPPipPimGammaAlg(char *name_i = NULL, Option_t par[] = NULL);

  virtual         ~HHypPPPipPimGammaAlg(void);

  Bool_t          execute();
  Bool_t          init();
  Bool_t          reinit();
  Bool_t          finalize();

  ClassDef(HHypPPPipPimGammaAlg, 0)  // Cutting on miss4==Gamma
};

//============================================================================

#endif
