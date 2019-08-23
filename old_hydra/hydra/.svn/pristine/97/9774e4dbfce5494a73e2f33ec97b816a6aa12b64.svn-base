//*-- AUTHOR : I. Froehlich
//*-- Modified : 04/07/2005 by I. Froehlich
//*-- Modified : 15/nov/2005 by B. Spruck

#ifndef HHYP_PP_PIP_PIM_PI0_KINE_H
#define HHYP_PP_PIP_PIM_PI0_KINE_H

#include <TObject.h>
#include <TArrayI.h>
#include "hhypbasealgorithm.h"
#include "hhyplistfiller.h"

#include "hhypKineFitPPPipPimPi0.h"
#include <TNtuple.h>

#define	HHYPPIPPIM_MINUIT_SIZE	16
// ***************************************************************************

class HHypPPPipPimPi0Kine:public HHypBaseAlgorithm
{

  // ***************************************************************************
private:
	TNtuple       * qa;// QA ntuple (pulls, chi2, ..)
	TNtuple       *kinetest;

	Bool_t        use_err_fixed;
	
	Float_t input[HHYPPIPPIM_MINUIT_SIZE];
	Double_t output[HHYPPIPPIM_MINUIT_SIZE];
	Double_t output2[HHYPPIPPIM_MINUIT_SIZE];
	Float_t errInput[HHYPPIPPIM_MINUIT_SIZE];
	Double_t errOutput[HHYPPIPPIM_MINUIT_SIZE];
	Float_t derOut[HHYPPIPPIM_MINUIT_SIZE];
	Double_t gInput[HHYPPIPPIM_MINUIT_SIZE];

	HHypKineFitPPPipPimPi0 *fit;

	Double_t getMomErr(Int_t sec,Double_t P,Int_t pid);
	Bool_t DoTheRefit(TVector3 *momentum,Int_t idx_p1,Int_t idx_p2,Int_t idx_pim,Int_t idx_pip,Float_t &chi2,Float_t &chi24,Float_t *pulls);

public:

protected:

public:

  HHypPPPipPimPi0Kine(char *name_i = NULL, Option_t par[] = NULL);

  ~ HHypPPPipPimPi0Kine(void);

  Bool_t          execute();
  Bool_t          init();
  Bool_t          reinit();
  Bool_t          finalize();

  ClassDef(HHypPPPipPimPi0Kine, 0) //Kinematic refit front-end alg
};

//============================================================================

#endif
