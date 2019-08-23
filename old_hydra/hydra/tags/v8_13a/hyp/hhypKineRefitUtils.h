//*-- AUTHOR : Anar
//*-- HypCopy : B. Spruck
//*-- Modified : 26/10/2005 by B. Spruck
//*-- Modified : 26/01/2006 by T. Perez

#ifndef HHYP_KINE_REFIT_UTILS_H
#define HHYP_KINE_REFIT_UTILS_H

#include <TObject.h>
#include <TArrayI.h>

#include "hgeomtransform.h"
//#include "hhypreconstructor.h"
#include "hhypbasealgorithm.h"

// ***************************************************************************

class HHypKineRefitUtils:public TNamed
{
public:
	Float_t Etot[4]; //Etotal after first step;
	Float_t eBeam; 
	Float_t mProton;
	Float_t mElec;
	Float_t mPion;
	Float_t ELab;
	Float_t ECM;
	Float_t GammaF;
	Float_t BetaF;
	Float_t pInput;
	Float_t AllE,AllX,AllZ,AllY;
	Double_t output[12];
	//Float_t input[12];
	// Float_t errInput[12];
	//Double_t outputNew[12];
	

  // ***************************************************************************
public:
	HHypKineRefitUtils();
	HHypKineRefitUtils(Float_t);
	~HHypKineRefitUtils(){;};
	
	void init(Float_t);
	Float_t calcFromKin(Float_t);
	Float_t missing4(HGeomVector &,HGeomVector &,HGeomVector &,HGeomVector &);
	Float_t missing4Elec(HGeomVector &,HGeomVector &,HGeomVector &,HGeomVector &);
	
	Float_t missing4(Float_t *);
	Float_t missing4Elec(Float_t *);
	
	Float_t missing4(Double_t *);
	Float_t missing4Elec(Double_t *);
	
	Float_t missing2(HGeomVector &, HGeomVector &);
	
	Float_t missing2Elec(HGeomVector &, HGeomVector &); 
	Float_t scalar(HGeomVector &, HGeomVector &);
/* 	void calcFConserv(Double_t *,Float_t *, Float_t *,Float_t *); */
	void calcFConservElec(Double_t *,Float_t *, Float_t *,Float_t *);
	//	void CalcDer(Float_t *,Double_t *);
	void CalcDerNum(Float_t *,Double_t *);
	void CalcDerNumPipPimGamma(Float_t *,Double_t *);
	void CalcDerNumElec(Float_t *,Double_t *);
/* 	Float_t derP(Int_t,Int_t); */
/* 	Float_t derTet(Int_t,Int_t); */
/* 	Float_t derPhi(Int_t,Int_t); */
/* 	Float_t derE(Int_t,Int_t); */
/* 	Float_t derE(Int_t); */
	Float_t secondIter(Float_t *,Double_t *,Float_t *,Double_t *,Float_t *);
	Float_t secondIterPipPimGamma(Float_t *,Double_t *,Float_t *,Double_t *,Float_t *);
	Float_t secondIterElec(Float_t *,Double_t *,Float_t *,Double_t *,Float_t *);
   
 private: 
	TLorentzVector hypBeam;

	ClassDef(HHypKineRefitUtils, 0) //Kinematic refit
};

//============================================================================

#endif
