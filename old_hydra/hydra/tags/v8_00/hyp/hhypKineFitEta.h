//*-- AUTHOR : Anar
//*-- HypCopy : B. Spruck
//*-- Modified : 26/10/2005 by B. Spruck
//*-- Modified : 10/01/2006 by T. Perez     PipPimGamma Channel and Non-used functions commented out.

#ifndef HHYP_KINE_FIT_ETA_H
#define HHYP_KINE_FIT_ETA_H

#include "TMinuit.h"
#include "TObject.h"
#include "hhypKineRefitUtils.h"

#include "hgeomtransform.h"

class HHypKineFitEta:public TObject
{
protected:
	HHypKineRefitUtils *kUU;
	Float_t *input, *errInput;
	Double_t *output,*errOutput;
	Double_t gInput[12];
	Float_t chi2;
	Int_t N;
	
public:
	HHypKineFitEta();
	HHypKineFitEta(Float_t *,Float_t *);
	virtual ~HHypKineFitEta();
	
	void ResetOutput(void);
	void SetInput(Float_t *, Float_t *);
	void GetInput(Float_t *,Float_t *);
	void SetGInput(Double_t *);
	
	void SetOutput(Double_t *,Double_t *);
	void GetOutput(Double_t *,Double_t *);
	
	Float_t SetChi2() {return chi2;}
	void GetChi2(Float_t _chi2) {chi2=_chi2;}
	

	Int_t minuitFit(Int_t select);
	Int_t minuitFitPipPimGamma(Int_t select);
	Int_t minuitFitElec(Int_t select);
	// for reference, deleted funtions are::
/* 	void minuitFitSeek(Int_t select); */
/* 	void minuitFit2(); */
/* 	void minuitFit0(Int_t select); */
/* 	void miniChi2(); */
/* 	void miniChi2For4();  */
/* 	Float_t calcChi(Float_t *); */
/* 	Float_t calcChiFor4(Float_t *);  */
/* 	void findMinimum(Float_t *,Float_t *,Double_t &); */
/* 	Float_t det3(Float_t a[3][3]); */
	ClassDef(HHypKineFitEta,0);

};

#endif

