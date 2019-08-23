//*-- AUTHOR : B. Spruck
//*-- Modified : 20/4/2005
//*-- Modified : 22 Dez 2005  B. Spruck
//*-- Modified : 16 Mai 2006  M. Wisniowski

#ifndef HHYP_PPIP_ALG_H
#define HHYP_PPIP_ALG_H

#include <TObject.h>
#include <TArrayI.h>
#include "hhypbasealgorithm.h"
#include "hhyplistfiller.h"
#include "hhyplistfillerinclusive.h"

class HHypPPipAlg:public HHypBaseAlgorithm
{

private:
  HHypListFillerInclusive * fillerInclusive;// 2 protons (fakes taken into account) 
  HHypListFiller * filler2;// 2 Particles
  HHypListFiller * filler2p1;// 2 + 1 pos fake
  HHypListFiller * filler2m1;// 2 + 1 neg fake
  
  HHypListFiller * filler2p2;// 2 + 2 pos fake
  HHypListFiller * filler2p1m1;// 2 + 1 pos + 1 neg fake
  HHypListFiller * filler2m2;// 2 + 2 neg fake

  Bool_t          use_InclusiveFiller;
  Bool_t          use_2_1;
  Bool_t          use_2_2;

protected:

public:

  HHypPPipAlg(char *name_i = NULL, Option_t par[] = NULL);

  virtual ~ HHypPPipAlg(void);
  
  Bool_t          execute();
  Bool_t          init();
  Bool_t          reinit();
  Bool_t          finalize();

  ClassDef(HHypPPipAlg, 0)  // filling pp candidates
};

//============================================================================

#endif
