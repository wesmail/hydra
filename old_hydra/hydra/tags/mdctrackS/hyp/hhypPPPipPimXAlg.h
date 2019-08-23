//*-- AUTHOR : Tiago Perez
//*-- Modified : 05/01/2005 by Tiago Perez
//*-- Modified : 04/07/2005 by I. Froehlich

#ifndef HHYP_PP_PIP_PIM_X_H
#define HHYP_PP_PIP_PIM_X_H

#include <TObject.h>
#include <TArrayI.h>
#include "hhypbasealgorithm.h"
#include "hhyplistfiller.h"

// ***************************************************************************

class HHypPPPipPimXAlg : public HHypBaseAlgorithm
{

  // ***************************************************************************
private:
  HHypListFiller * filler;

protected:

public:

  HHypPPPipPimXAlg(char *name_i = NULL, Option_t par[] = NULL);

  virtual ~ HHypPPPipPimXAlg(void);

  Bool_t          execute();
  Bool_t          init();
  Bool_t          reinit();
  Bool_t          finalize();

  ClassDef(HHypPPPipPimXAlg, 0)  // filling PP Pip Pim candidates
};

//============================================================================

#endif
