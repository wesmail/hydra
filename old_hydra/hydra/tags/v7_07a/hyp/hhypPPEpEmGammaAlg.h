//*-- AUTHOR : B. Spruck
//*-- Modified : 20/4/2005

#ifndef HHYP_PP_EP_EM_GAMMA_H
#define HHYP_PP_EP_EM_GAMMA_H

#include <TObject.h>
#include <TArrayI.h>
#include "hhypbasealgorithm.h"
#include "hhyplistfiller.h"

// ***************************************************************************

class HHypPPEpEmGammaAlg:public HHypBaseAlgorithm
{

  // ***************************************************************************
private:

protected:

public:

  HHypPPEpEmGammaAlg(char *name_i = NULL, Option_t par[] = NULL);

  virtual ~ HHypPPEpEmGammaAlg(void);

  Bool_t          execute();
  Bool_t          init();
  Bool_t          reinit();
  Bool_t          finalize();

  ClassDef(HHypPPEpEmGammaAlg, 0)       // exclusive dalitz decay in pp reactions
};

//============================================================================

#endif
