//*-- AUTHOR : B. Spruck
//*-- Modified : 20 Sep 2006

#ifndef HHYP_CORRECT_ELOSS_H
#define HHYP_CORRECT_ELOSS_H

#include "TObject.h"
#include "TArrayI.h"
#include "TNtuple.h"
#include "hhypbasealgorithm.h"
// from MdcG:
#include "henergylosscorrpar.h"

// ***************************************************************************

class HHypCorrectEloss:public HHypBaseAlgorithm
{

  // ***************************************************************************
private:
	TNtuple       * qa;// QA ntuple
	HEnergyLossCorrPar *enLossCorr;
protected:

public:

  HHypCorrectEloss(Char_t *name_i = NULL, Option_t par[] = NULL);

  virtual ~ HHypCorrectEloss(void);

  Bool_t          execute();
  Bool_t          init();
  Bool_t          reinit();
  Bool_t          finalize();

  ClassDef(HHypCorrectEloss, 0)// Removes all but one Combination based on Kine Chi2/Chi24
};

//============================================================================

#endif
