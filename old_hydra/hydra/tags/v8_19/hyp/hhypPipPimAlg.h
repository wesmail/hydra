//*-- Modified : 18 Aug 2008  M. Palka

#ifndef HHYP_PipPim_H
#define HHYP_PipPim_H

#include "TObject.h"
#include "TArrayI.h"
#include "hhypbasealgorithm.h"
#include "hhyplistfiller.h"
#include "hhyplistfillerinclusive.h"

class HHypPipPimAlg:public HHypBaseAlgorithm
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

  HHypPipPimAlg(Char_t *name_i = NULL, Option_t par[] = NULL);

  virtual ~ HHypPipPimAlg(void);
  
  Bool_t          execute();
  Bool_t          init();
  Bool_t          reinit();
  Bool_t          finalize();

  ClassDef(HHypPipPimAlg, 0)  // filling pp candidates
};

//============================================================================

#endif
