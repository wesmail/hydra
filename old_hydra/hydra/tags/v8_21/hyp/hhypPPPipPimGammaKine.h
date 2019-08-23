//*-- AUTHOR : I. Froehlich
//*-- Modified : 04/07/2005 by I. Froehlich
//*-- Modified : 15/nov/2005 by B. Spruck

#ifndef HHYP_PP_PIP_PIM_GAMMA_KINE_H
#define HHYP_PP_PIP_PIM_GAMMA_KINE_H

#include "hhypbasealgorithm.h"
#include "hhyplistfiller.h"

#include "hhypKineFitPPPipPimGamma.h"
#include "TNtuple.h"
#include "hhypMomError.h"

#define	HHYPPIPPIM_MINUIT_SIZE	16
// ***************************************************************************

class HHypPPPipPimGammaKine:public HHypBaseAlgorithm
{

  // ***************************************************************************
private:
	TNtuple       * qa;// QA ntuple (pulls, chi2, ..)
	TNtuple       *kinetest;
	
	Float_t input[HHYPPIPPIM_MINUIT_SIZE];
	Double_t output[HHYPPIPPIM_MINUIT_SIZE];
	Double_t output2[HHYPPIPPIM_MINUIT_SIZE];
	Float_t errInput[HHYPPIPPIM_MINUIT_SIZE];
	Double_t errOutput[HHYPPIPPIM_MINUIT_SIZE];
	Float_t derOut[HHYPPIPPIM_MINUIT_SIZE];
	Double_t gInput[HHYPPIPPIM_MINUIT_SIZE];

	HHypKineFitPPPipPimGamma *fit;

	Double_t getMomError(Double_t theta, Double_t phi, Double_t mass, Double_t p,Int_t sec,Bool_t simu);
	Bool_t DoTheRefit(TVector3 *momentum,Int_t idx_p1,Int_t idx_p2,Int_t idx_pim,Int_t idx_pip,Float_t &chi2,Float_t &chi24,Float_t *pulls);

public:

protected:

public:

  HHypPPPipPimGammaKine(Char_t *name_i = NULL, Option_t par[] = NULL);

  ~ HHypPPPipPimGammaKine(void);

  Bool_t          execute();
  Bool_t          init();
  Bool_t          reinit();
  Bool_t          finalize();

  ClassDef(HHypPPPipPimGammaKine, 0) //Kinematic refit front-end alg
};

//============================================================================

#endif
