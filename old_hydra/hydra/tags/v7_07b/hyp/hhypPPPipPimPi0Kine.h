//*-- AUTHOR : I. Froehlich
//*-- Modified : 04/07/2005 by I. Froehlich

#ifndef HHYP_PP_PIP_PIM_PI0_KINE_H
#define HHYP_PP_PIP_PIM_PI0_KINE_H

#include <TObject.h>
#include <TArrayI.h>
#include "hhypbasealgorithm.h"
#include "hhyplistfiller.h"

// ***************************************************************************

class HHypPPPipPimPi0Kine:public HHypBaseAlgorithm
{

  // ***************************************************************************
private:

protected:

public:

  HHypPPPipPimPi0Kine(char *name_i = NULL, Option_t par[] = NULL);

  virtual ~ HHypPPPipPimPi0Kine(void);

  Bool_t          execute();
  Bool_t          init();
  Bool_t          reinit();
  Bool_t          finalize();

  ClassDef(HHypPPPipPimPi0Kine, 0) //Kinematic refit front-end alg
};

//============================================================================

#endif
