//*-- AUTHOR : B. Spruck
//*-- Modified : 7 Nov 2005
//*-- Modified : 22 Dec 2005 B Spruck

#ifndef HHYP_PP_EP_EM_X_H
#define HHYP_PP_EP_EM_X_H

#include <TObject.h>
#include <TArrayI.h>
#include "hhypbasealgorithm.h"
#include "hhyplistfiller.h"
#include "hhyplistfillerinclusive.h"

class  HHypPPEpEmXAlg:public HHypBaseAlgorithm
{

private:
  HHypListFillerInclusive * filler;// 4 + X Particles; x=0,1,2,3...
  
  HHypListFiller * filler4;// 4 Particles
  HHypListFiller * filler4p1;// 4 + 1 pos fake
  HHypListFiller * filler4m1;// 4 + 1 neg fake
  
  HHypListFiller * filler4p2;// 4 + 2 pos fake
  HHypListFiller * filler4p1m1;// 4 + 1 pos + 1 neg fake
  HHypListFiller * filler4m2;// 4 + 2 neg fake

  Bool_t          use_InclusiveFiller;
  Bool_t          use_4_1;
  Bool_t          use_4_2;

protected:

public:

  HHypPPEpEmXAlg(char *name_i = NULL, Option_t par[] = NULL);

  virtual ~ HHypPPEpEmXAlg(void);

  Bool_t          execute();
  Bool_t          init();
  Bool_t          reinit();
  Bool_t          finalize();

  ClassDef(HHypPPEpEmXAlg, 0)  // The PP Ep Em plus X Filler
};

//============================================================================

#endif
