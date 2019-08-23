//*-- AUTHOR : B. Spruck
//*-- Modified : 20/4/2005

#ifndef HHYP_PPX_H
#define HHYP_PPX_H

#include <TObject.h>
#include <TArrayI.h>
#include "hhypbasealgorithm.h"
#include "hhyplistfiller.h"

class HHypPPXAlg:public HHypBaseAlgorithm
{

private:
  HHypListFiller * filler;

protected:

public:

  HHypPPXAlg(char *name_i = NULL, Option_t par[] = NULL);

  virtual ~ HHypPPXAlg(void);
  
  Bool_t          execute();
  Bool_t          init();
  Bool_t          reinit();
  Bool_t          finalize();

  ClassDef(HHypPPXAlg, 0)  // filling pp candidates
};

//============================================================================

#endif
