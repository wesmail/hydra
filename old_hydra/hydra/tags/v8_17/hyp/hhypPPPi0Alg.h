//*-- AUTHOR : Marcin Wisniowski
//*-- Modified : 31/8/2005

#ifndef HHYP_PP_PI0_H
#define HHYP_PP_PI0_H

#include <TObject.h>
#include <TArrayI.h>
#include "hhypbasealgorithm.h"
#include "hhyplistfiller.h"

class HHypPPPi0Alg:public HHypBaseAlgorithm
{

private:

protected:

public:

  HHypPPPi0Alg(Char_t *name_i = NULL, Option_t par[] = NULL);

  virtual ~ HHypPPPi0Alg(void);

  Bool_t          execute();
  Bool_t          init();
  Bool_t          reinit();
  Bool_t          finalize();

  ClassDef(HHypPPPi0Alg, 0) //  P P Pi0 selection
};

//============================================================================

#endif
