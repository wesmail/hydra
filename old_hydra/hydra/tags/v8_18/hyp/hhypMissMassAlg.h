//*-- AUTHOR : B. Spruck
//*-- Modified : June 2008

#ifndef HHYP_MISS_MASS_ALG_H
#define HHYP_MISS_MASS_ALG_H

#include <TObject.h>
#include <TArrayI.h>
#include <TH1I.h>
#include "hhypbasealgorithm.h"

// ***************************************************************************

class HHypMissMassAlg:public HHypBaseAlgorithm
{

  // ***************************************************************************
private:

  Float_t          lower_cut, upper_cut; // cuts values from pParams
  TString          lower_cut_name, upper_cut_name; //name of cuts
	TH1I						* histBefore, * histAfter;

protected:

public:

  HHypMissMassAlg(Char_t *name_i = NULL, Option_t par[] = NULL);

  virtual ~ HHypMissMassAlg(void);

  Bool_t          execute();
  Bool_t          init();
  Bool_t          reinit();
  Bool_t          finalize();

  ClassDef(HHypMissMassAlg, 0)       // exclusive dalitz decay in pp reactions
};

//============================================================================

#endif
