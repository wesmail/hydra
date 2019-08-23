//*-- AUTHOR : Anar
//*-- HypCopy : B. Spruck
//*-- Modified : 26/10/2005 by B. Spruck
//*-- Modified : 10/01/2006 by T. Perez     PipPimGamma Channel and Non-used functions commented out.

#ifndef HHYP_KINE_FIT_H
#define HHYP_KINE_FIT_H

#include "TMinuit.h"
#include "TObject.h"

#include "hgeomtransform.h"

class HHypKineFit:public TObject
{
protected:
	Float_t *fitInput, *errInput;
	Double_t *fitOutput,*errOutput;
	Float_t chi2;
//	Int_t N;

//	Float_t Etot[4]; //Etotal after first step;
	Float_t eBeam; 
	Float_t mProton;
	Float_t m2Proton;
	Float_t m2Elec;
	Float_t m2Pion0;
	Float_t m2PionC;
	Float_t ELab;
	Float_t pInput;
/*	Float_t ECM;
	Float_t GammaF;
	Float_t BetaF;
	Float_t AllE,AllX,AllZ,AllY;*/
	Double_t output[12];

public:
	HHypKineFit();
	HHypKineFit(Float_t *,Float_t *);
	~HHypKineFit();

	void init(Float_t eB);
	
	void ResetOutput(void);
	void SetInput(Float_t *, Float_t *);
	void GetInput(Float_t *,Float_t *);
	
	void SetOutput(Double_t *,Double_t *);
	void GetOutput(Double_t *,Double_t *);

	Float_t getM2Pion0(void){return(m2Pion0);};

	Float_t SetChi2() {return chi2;}
	void GetChi2(Float_t _chi2) {chi2=_chi2;}
	Float_t scalar(HGeomVector &data1, HGeomVector &data2);

	ClassDef(HHypKineFit,0);
};

#endif

