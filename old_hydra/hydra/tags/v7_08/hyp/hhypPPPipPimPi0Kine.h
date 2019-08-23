//*-- AUTHOR : I. Froehlich
//*-- Modified : 04/07/2005 by I. Froehlich

#ifndef HHYP_PP_PIP_PIM_PI0_KINE_H
#define HHYP_PP_PIP_PIM_PI0_KINE_H

#include <TObject.h>
#include <TArrayI.h>
#include "hhypbasealgorithm.h"
#include "hhyplistfiller.h"

#include "hhypKineRefitUtils.h"
#include "hhypKineFitEta.h"

// ***************************************************************************

class HHypPPPipPimPi0Kine:public HHypBaseAlgorithm
{

  // ***************************************************************************
private:
	Float_t input[12];
	Double_t output[12];
	Double_t output2[12];
	Float_t errInput[12];
	Double_t errOutput[12];
	Float_t derOut[12];
	Double_t gInput[12];

	HHypKineRefitUtils *kk;
	HHypKineFitEta *fitEta;

	Bool_t DoTheRefit(TVector3 *momentum,Int_t idx_p1,Int_t idx_p2,Int_t idx_pim,Int_t idx_pip,Float_t &chi2,Float_t &chi24);

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
