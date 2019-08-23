//*-- AUTHOR : B. Spruck
//*-- Modified : 7 May 2008 B. Spruck
//*-- Modified : 14 July 2008 B. Spruck

#ifndef HHYP_ALL_FILLER_INCLUSIVE_H
#define HHYP_ALL_FILLER_INCLUSIVE_H

#include "TObject.h"
#include "TArrayI.h"
#include "hhypbasealgorithm.h"
#include "hhyplistfillerinclusive.h"

#define HHypAllFillerInclusiveAlg_ANZ_FILLER	20

class  HHypAllFillerInclusiveAlg:public HHypBaseAlgorithm
{

private:
  HHypListFillerInclusive *filler[HHypAllFillerInclusiveAlg_ANZ_FILLER];

	Bool_t InitFillers(Int_t &fnr,Int_t particles_anz,Int_t *particles,Int_t missing_anz,Int_t *missing);

protected:

public:

  HHypAllFillerInclusiveAlg(Char_t *name_i = NULL, Option_t par[] = NULL);

  virtual ~ HHypAllFillerInclusiveAlg(void);

  Bool_t          execute();
  Bool_t          init();
  Bool_t          reinit();
  Bool_t          finalize();

  ClassDef(HHypAllFillerInclusiveAlg, 0)  // The ALL Filler
};

//============================================================================

#endif
