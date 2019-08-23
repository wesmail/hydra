//*-- AUTHOR : B. Spruck
//*-- Modified : 10/nov/2006 by B. Spruck

#ifndef HHYP_X_KINE_H
#define HHYP_X_KINE_H

#include <stdlib.h>
#include <iostream>

#include <TObject.h>
#include <TArrayI.h>
#include "hhypbasealgorithm.h"
#include "hhyplistfiller.h"
#include "hbasetrack.h"
#include "hypinfodef.h"

#include "hlagrangesolmitzfit.h"
#include <TNtuple.h>

// ***************************************************************************

class HHypXKine:public HHypBaseAlgorithm
{

  // ***************************************************************************
private:
	TNtuple       * qa;// QA ntuple (pulls, chi2, ..)
	TNtuple       *kinetest;
	
	bool use_err_fixed;// use fixed lin error per sector
	
	HLagrangeSolmitzFit *fitter;

	Double_t getMomErr(Int_t sec,Double_t P,Int_t pid);

public:

protected:

public:

  HHypXKine(char *name_i = NULL, Option_t par[] = NULL);

  ~ HHypXKine(void);

  Bool_t          execute();
  Bool_t          init();
  Bool_t          reinit();
  Bool_t          finalize();

  ClassDef(HHypXKine, 0) //Kinematic refit front-end alg
};

//============================================================================

#endif
