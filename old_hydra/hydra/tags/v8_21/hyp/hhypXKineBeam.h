//*-- AUTHOR : B. Spruck
//*-- Modified : 10/nov/2006 by B. Spruck

#ifndef HHYP_X_KINE_BEAM_H
#define HHYP_X_KINE_BEAM_H

#include <stdlib.h>
#include <iostream>

#include "TObject.h"
#include "TArrayI.h"
#include "hhypbasealgorithm.h"
#include "hhyplistfiller.h"
#include "hbasetrack.h"
#include "hypinfodef.h"

#include "hlagrangesolmitzbeamfit.h"
#include "TNtuple.h"
#include "TH1I.h"
#include "hhypMomError.h"

// ***************************************************************************

class HHypXKineBeam:public HHypBaseAlgorithm
{

  // ***************************************************************************
private:
	TNtuple       * qa;// QA ntuple (pulls, chi2, ..)
	TNtuple       *kinetest;

 	HLagrangeSolmitzBeamFit *fitter;

	Double_t getMomError(Double_t theta, Double_t phi, Double_t mass, Double_t p,Int_t sec,Bool_t simu);

	Double_t FakErrMomExp, FakErrThetaExp, FakErrPhiExp;
	Double_t FakErrMomSim, FakErrThetaSim, FakErrPhiSim;

	TH1I *hPullsMomEp, *hPullsThetaEp, *hPullsPhiEp, *hPullsMomEm, *hPullsThetaEm,
			 *hPullsPhiEm, *hPullsMomPip, *hPullsThetaPip, *hPullsPhiPip, *hPullsMomPim,
			 *hPullsThetaPim, *hPullsPhiPim, *hPullsMomP, *hPullsThetaP, *hPullsPhiP,
			 *hProb;

public:

protected:

public:

  HHypXKineBeam(Char_t *name_i = NULL, Option_t par[] = NULL);

  ~ HHypXKineBeam(void);

  Bool_t          execute();
  Bool_t          init();
  Bool_t          reinit();
  Bool_t          finalize();

  ClassDef(HHypXKineBeam, 0) //Kinematic refit front-end alg
};

//============================================================================

#endif
