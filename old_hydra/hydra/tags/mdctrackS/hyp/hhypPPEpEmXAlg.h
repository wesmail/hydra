//*-- AUTHOR : B. Spruck
//*-- Modified : 20/4/2005

#ifndef HHYP_PP_EP_EM_X_H
#define HHYP_PP_EP_EM_X_H

#include <TObject.h>
#include <TArrayI.h>
#include "hhypbasealgorithm.h"
#include "hhyplistfiller.h"

class  HHypPPEpEmXAlg:public HHypBaseAlgorithm
{

private:
  HHypListFiller * filler;

protected:

public:

  HHypPPEpEmXAlg(char *name_i = NULL, Option_t par[] = NULL);

  virtual ~ HHypPPEpEmXAlg(void);

  Bool_t          execute();
  Bool_t          init();
  Bool_t          reinit();
  Bool_t          finalize();

  ClassDef(HHypPPEpEmXAlg, 0)  // The PP Ep Em Filler
};

//============================================================================

#endif
