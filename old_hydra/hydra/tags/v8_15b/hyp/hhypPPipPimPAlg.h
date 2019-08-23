//*-- AUTHOR : B. Spruck
//*-- Modified : 28 Feb 2007

#ifndef HHYP_PPIPPIM_P_H
#define HHYP_PPIPPIM_P_H

#include <TObject.h>
#include <TArrayI.h>
#include "hhypbasealgorithm.h"
#include "hhyplistfiller.h"
#include "hhyplistfillerinclusive.h"

class  HHypPPipPimPAlg:public HHypBaseAlgorithm
{

private:
	bool use_3_1, use_3_2;

  HHypListFiller * filler3;// 3

  HHypListFiller * filler3p1;// 3 + 1 pos fake
  HHypListFiller * filler3m1;// 3 + 1 neg fake
  
  HHypListFiller * filler3p2;// 3 + 2 pos fake
  HHypListFiller * filler3p1m1;// 3 + 1 pos + 1 neg fake
  HHypListFiller * filler3m2;// 3 + 2 neg fake

protected:

public:

  HHypPPipPimPAlg(char *name_i = NULL, Option_t par[] = NULL);

  virtual ~ HHypPPipPimPAlg(void);

  Bool_t          execute();
  Bool_t          init();
  Bool_t          reinit();
  Bool_t          finalize();

  ClassDef(HHypPPipPimPAlg, 0)  // The P Pi+ Pi- missing P Filler
};

//============================================================================

#endif
