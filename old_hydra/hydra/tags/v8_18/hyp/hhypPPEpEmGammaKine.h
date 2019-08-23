//*-- AUTHOR : B. Spruck
//*-- Modified : 26/10/2005 by B. Spruck
//*-- Modified : 15/nov/2005 by B. Spruck

#ifndef HHYP_PP_EP_EM_GAMMA_KINE_H
#define HHYP_PP_EP_EM_GAMMA_KINE_H

#include "hhypbasealgorithm.h"
#include "hhyplistfiller.h"

#include "hhypKineFitPPEpEmGamma.h"
#include <TNtuple.h>
#include "hhypMomError.h"

// ***************************************************************************

#define	HHYPEPEM_MINUIT_SIZE	16
// 12 should be enough already... stay on the save side

class HHypPPEpEmGammaKine:public HHypBaseAlgorithm
{

  // ***************************************************************************
private:
	TNtuple       * qa;// QA ntuple (pulls, chi2, ..)
	TNtuple       *kinetest;

	Float_t input[HHYPEPEM_MINUIT_SIZE];
	Double_t output[HHYPEPEM_MINUIT_SIZE];
	Double_t output2[HHYPEPEM_MINUIT_SIZE];
	Float_t errInput[HHYPEPEM_MINUIT_SIZE];
	Double_t errOutput[HHYPEPEM_MINUIT_SIZE];
	Float_t derOut[HHYPEPEM_MINUIT_SIZE];
	Double_t gInput[HHYPEPEM_MINUIT_SIZE];

	HHypKineFitPPEpEmGamma *fit;

	Double_t getMomError(Double_t theta, Double_t phi, Double_t mass, Double_t p,Int_t sec,Bool_t simu);
	Bool_t DoTheRefit(TVector3 *momentum,Int_t idx_p1,Int_t idx_p2,Int_t idx_em,Int_t idx_ep,Float_t &chi2,Float_t &chi24,Float_t *pulls);

public:

	HHypPPEpEmGammaKine(Char_t *name_i = NULL, Option_t par[] = NULL);
	
	// virtual 
	~ HHypPPEpEmGammaKine(void);
	
	Bool_t          execute();
	Bool_t          init();
	Bool_t          reinit();
	Bool_t          finalize();
	
	ClassDef(HHypPPEpEmGammaKine, 0) //Kinematic refit front-end alg
};

//============================================================================

#endif
