//*-- AUTHOR : B. Spruck
//*-- Modified : 7 May 2008 B. Spruck

#ifndef HHYP_ALL_FILLER_H
#define HHYP_ALL_FILLER_H

#include <TObject.h>
#include <TArrayI.h>
#include "hhypbasealgorithm.h"
#include "hhyplistfiller.h"

#define HHypAllFillerAlg_ANZ_FILLER	20
class  HHypAllFillerAlg:public HHypBaseAlgorithm
{

private:
  HHypListFiller *filler[HHypAllFillerAlg_ANZ_FILLER];

	Bool_t InitFillers(Int_t &fnr,Int_t particles_anz,Int_t *particles,Int_t missing_anz,Int_t *missing);

protected:

public:

  HHypAllFillerAlg(Char_t *name_i = NULL, Option_t par[] = NULL);

  virtual ~ HHypAllFillerAlg(void);

  Bool_t          execute();
  Bool_t          init();
  Bool_t          reinit();
  Bool_t          finalize();

  ClassDef(HHypAllFillerAlg, 0)  // The ALL Filler
};

//============================================================================

#endif
