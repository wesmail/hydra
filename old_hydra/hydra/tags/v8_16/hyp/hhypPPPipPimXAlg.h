//*-- AUTHOR : Tiago Perez
//*-- Modified : 05/01/2005 by Tiago Perez
//*-- Modified : 04/07/2005 by I. Froehlich
//*-- Modified : 22/Dec/2005 by B. Spruck

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
  HHypListFiller * filler4;// 4 Particles
  HHypListFiller * filler4p1;// 4 + 1 pos fake
  HHypListFiller * filler4m1;// 4 + 1 neg fake
  
  HHypListFiller * filler4p2;// 4 + 2 pos fake
  HHypListFiller * filler4p1m1;// 4 + 1 pos + 1 neg fake
  HHypListFiller * filler4m2;// 4 + 2 neg fake

  Bool_t          use_4_1;
  Bool_t          use_4_2;

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
