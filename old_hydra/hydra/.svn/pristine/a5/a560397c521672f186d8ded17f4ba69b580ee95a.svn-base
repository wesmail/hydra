//*-- AUTHOR : Anar
//*-- HypCopy : B. Spruck
//*-- Modified : 26/10/2005 by B. Spruck
//*-- Modified : 10/01/2006 by T. Perez     PipPimGamma Channel and Non-used functions commented out.
//_HADES_CLASS_DESCRIPTION 
////////////////////////////////////////////////////////////////////////
//
// HHypKineFitEta
//
// if you need any of the removed functions, retrieve a version earlier than 
// 01.02.2006... Tiago .
////////////////////////////////////////////////////////////////////////


#include "hhypKineFitEta.h"
#include "hhypKineRefitUtils.h"
#include "hgeomvector.h"

using namespace std;
ClassImp(HHypKineFitEta);

// needed for minuit called function -> not object oriented???
HHypKineRefitUtils kU(2200.);

// PipPimPi0
void fcnEtaPi0(Int_t &npar, Double_t *gin, Double_t &f, Double_t *par, Int_t iflag)
{
	HHypKineFitEta *myObject=(HHypKineFitEta*)(gMinuit->GetObjectFit());
	Float_t F;
	Float_t fInput[12];
	Float_t fErrInput[12];
	Float_t DER[12];
	Float_t errF;
	
	myObject->GetInput(fInput,fErrInput);
	Float_t funct = 0;
		
	for(Int_t i=0; i<12; i++)
	{
		if(i==3 || i==6 || i==9 || i==0){
			funct+=(1/fInput[i]-1/par[i])*(1/fInput[i]-1/par[i])/fErrInput[i]/fErrInput[i]*par[i]*par[i]*par[i]*par[i];
		}else{
			funct+=(fInput[i]-par[i])*(fInput[i]-par[i])/fErrInput[i]/fErrInput[i];
		}
	}
	
	F=kU.missing4(par);
	kU.CalcDerNum(DER,par);
	errF=0;
	for(Int_t i=0; i<12; i++)
	{
		errF+=DER[i]*DER[i]*fErrInput[i]*fErrInput[i];
	}
	
	// The missing mass is HardCoded Here 135. <== Pi0
	funct+=10.*(F-135.*135.)*(F-135.*135.)/errF;
	f=funct;
	
}

// PipPimGamma
void fcnEtaPipPimGamma(Int_t &npar, Double_t *gin, Double_t &f, Double_t *par, Int_t iflag)
{
	HHypKineFitEta *myObject=(HHypKineFitEta*)(gMinuit->GetObjectFit());
	Float_t F;
	Float_t fInput[12];
	Float_t fErrInput[12];
	Float_t DER[12];
	Float_t errF;
	
	myObject->GetInput(fInput,fErrInput);
	Float_t funct = 0;
	
	for(Int_t i=0; i<12; i++)
	{
		if(i==3 || i==6 || i==9 || i==0){
			funct+=(1/fInput[i]-1/par[i])*(1/fInput[i]-1/par[i])/fErrInput[i]/fErrInput[i]*par[i]*par[i]*par[i]*par[i];
		}else{
			funct+=(fInput[i]-par[i])*(fInput[i]-par[i])/fErrInput[i]/fErrInput[i];
		}
	}
	
	F=kU.missing4(par);
	kU.CalcDerNumPipPimGamma(DER,par);
	errF=0;
	for(Int_t i=0; i<12; i++)
	{
		errF+=DER[i]*DER[i]*fErrInput[i]*fErrInput[i];
	}
	
	// The missing mass is HardCoded Here 0. <== Missing Gamma
	// Copied from the previous...
	// the 10.* is comming from ?? try with other parameters.
	// 1.- 10. BAD,  2.- 70. bad 3.- 35.
	funct+=1000.*F*F/errF;
	f=funct;
	
}

// EpEmGamma
void fcnEtaElec(Int_t &npar, Double_t *gin, Double_t &f, Double_t *par, Int_t iflag)
{
	HHypKineFitEta *myObject=(HHypKineFitEta*)(gMinuit->GetObjectFit());
	Float_t F;
	Float_t fInput[12];
	Float_t fErrInput[12];
	Float_t DER[12];
	Float_t errF;
	
	myObject->GetInput(fInput,fErrInput);
	Float_t funct = 0;
	
	for(Int_t i=0; i<12; i++)
	{
		if(i==3 || i==6 || i==9 || i==0){
			funct+=(1/fInput[i]-1/par[i])*(1/fInput[i]-1/par[i])/fErrInput[i]/fErrInput[i]*par[i]*par[i]*par[i]*par[i];
		}else{
			funct+=(fInput[i]-par[i])*(fInput[i]-par[i])/fErrInput[i]/fErrInput[i];
		}
	}
  
	F=kU.missing4Elec(par);
	kU.CalcDerNumElec(DER,par);
	errF=0;
	for(Int_t i=0; i<12; i++)
	{
		errF+=DER[i]*DER[i]*fErrInput[i]*fErrInput[i];
	}
	funct+=70.*F*F/errF;
	f=funct;
}

HHypKineFitEta::HHypKineFitEta()
{
	kUU=new HHypKineRefitUtils(2200.);   
	input=new Float_t [12];
	errInput=new Float_t [12];
	output=new Double_t [12];
	errOutput=new Double_t [12];
}

HHypKineFitEta::HHypKineFitEta(Float_t* in,Float_t* errIn)
{
	kUU=new HHypKineRefitUtils(2200.);   
	input=new Float_t [12];
	errInput=new Float_t [12];
	output=new Double_t [12];
	errOutput=new Double_t [12];
	
	for(Int_t i=0; i<12; i++)
	{
		input[i]=in[i];
		errInput[i]=errIn[i];
		output[i]=-1000.;
		errOutput[i]=-1000.;
	}
}

HHypKineFitEta::~HHypKineFitEta()
{
	if(kUU) delete kUU;
	// if(gMinuit) delete gMinuit; 
	if (input) delete [] input;
	if (output) delete [] output;
	if (errInput) delete [] errInput;
	if (errOutput) delete [] errOutput;
}

void HHypKineFitEta::ResetOutput(void)
{
	for(Int_t i=0; i<12; i++)
	{
		output[i]=-1000.;
		errOutput[i]=-1000.;
	}
}

void HHypKineFitEta::SetInput(Float_t* in,Float_t* errIn)
{
	for(Int_t i=0; i<12; i++)
	{
		input[i]=in[i];
		errInput[i]=errIn[i];
	}
}

void HHypKineFitEta::SetGInput(Double_t* in)
{
	for(Int_t i=0; i<12; i++)
	{
		gInput[i]=in[i];
	}
}

 
void HHypKineFitEta::SetOutput(Double_t* out,Double_t* errOut)
{
	for(Int_t i=0; i<12; i++)
	{
		output[i]=out[i];
		errOutput[i]=errOut[i];
	}
}

void HHypKineFitEta::GetInput(Float_t* in,Float_t* errIn)
{
	for(Int_t i=0; i<12; i++)
	{
		in[i]=input[i];
		errIn[i]=errInput[i];
	}
}
 
void HHypKineFitEta::GetOutput(Double_t* out,Double_t* errOut)
{
	for(Int_t i=0; i<12; i++)
	{
		out[i]=output[i];
		errOut[i]=errOutput[i];
	}
}

Int_t HHypKineFitEta::minuitFit(Int_t select)
{
	Int_t result, ierflg = 0;
	
	if(!gMinuit) gMinuit=new TMinuit(12);
	
	gMinuit->SetFCN(fcnEtaPi0);
	gMinuit->SetObjectFit(this);
	
	Double_t arglist[12];
	Double_t vstart[13],low[13],up[13];
	Double_t step[13];
	
	for(Int_t i=0; i<12; i++)
	{
		vstart[i]=input[i];//+5*errInput[i];
		step[i]=errInput[i]*2.;///10.;///2.;
		low[i]=input[i]-25.*errInput[i];
		up[i]=input[i]+25.*errInput[i];
	}
	
	gMinuit->mnparm(0, "P1", vstart[0],step[0],low[0],up[0],ierflg);
	gMinuit->mnparm(1, "Theta1",vstart[1], step[1],low[1],up[1],ierflg);
	gMinuit->mnparm(2, "Phi1", vstart[2], step[2],low[2],up[2],ierflg);
	gMinuit->mnparm(3, "P2", vstart[3], step[3], low[3],up[3],ierflg);
	gMinuit->mnparm(4, "Theta2",vstart[4], step[4],low[4],up[4],ierflg);
	gMinuit->mnparm(5, "Phi2", vstart[5], step[5], low[5],up[5],ierflg);
	gMinuit->mnparm(6, "P3", vstart[6], step[6], low[6],up[6],ierflg);
	gMinuit->mnparm(7, "Theta3",vstart[7], step[7],low[7],up[7],ierflg);
	gMinuit->mnparm(8, "Phi3", vstart[8], step[8], low[8],up[8],ierflg);
	gMinuit->mnparm(9, "P4", vstart[9], step[9], low[9],up[9],ierflg);
	gMinuit->mnparm(10, "Theta4", vstart[10], step[10],low[10],up[10],ierflg);
	gMinuit->mnparm(11, "Phi4", vstart[11], step[11], low[11],up[11],ierflg);

	gMinuit->SetPrintLevel(-1);
	
	arglist[0] =0;
	gMinuit->mnexcm("SET NOW", arglist,1,ierflg);
	gMinuit->SetErrorDef(0.5);
	
	arglist[0]=2;
	gMinuit->mnexcm("SET STR", arglist,1,ierflg);
	
	arglist[0] = 1;
	gMinuit->mnexcm("SET ERR", arglist ,1,ierflg);
	
	arglist[1] = 0.1;
	arglist[0] = 500;
	gMinuit->mnexcm("SIMPLEX",arglist,1,ierflg);

	result=ierflg;	
	for (Int_t ii=0; ii<12; ii++)
	{
		gMinuit->GetParameter(ii,output[ii],errOutput[ii]);	
	}
	return(result);
}


Int_t HHypKineFitEta::minuitFitPipPimGamma(Int_t select)
{	
	Int_t result, ierflg = 0;
	if(!gMinuit) gMinuit=new TMinuit(12);
	
	gMinuit->SetFCN(fcnEtaPipPimGamma);
	gMinuit->SetObjectFit(this);
	
	Double_t arglist[12];
	Double_t vstart[13],low[13],up[13];
	Double_t step[13];
	
	for(Int_t i=0; i<12; i++)
	{
		vstart[i]=input[i];//+5*errInput[i];
		step[i]=errInput[i]*2.;///10.;///2.;
		low[i]=input[i]-25.*errInput[i];
		up[i]=input[i]+25.*errInput[i];
	}
	
	gMinuit->mnparm(0, "P1", vstart[0],step[0],low[0],up[0],ierflg);
	gMinuit->mnparm(1, "Theta1",vstart[1], step[1],low[1],up[1],ierflg);
	gMinuit->mnparm(2, "Phi1", vstart[2], step[2],low[2],up[2],ierflg);
	gMinuit->mnparm(3, "P2", vstart[3], step[3], low[3],up[3],ierflg);
	gMinuit->mnparm(4, "Theta2",vstart[4], step[4],low[4],up[4],ierflg);
	gMinuit->mnparm(5, "Phi2", vstart[5], step[5], low[5],up[5],ierflg);
	gMinuit->mnparm(6, "P3", vstart[6], step[6], low[6],up[6],ierflg);
	gMinuit->mnparm(7, "Theta3",vstart[7], step[7],low[7],up[7],ierflg);
	gMinuit->mnparm(8, "Phi3", vstart[8], step[8], low[8],up[8],ierflg);
	gMinuit->mnparm(9, "P4", vstart[9], step[9], low[9],up[9],ierflg);
	gMinuit->mnparm(10, "Theta4", vstart[10], step[10],low[10],up[10],ierflg);
	gMinuit->mnparm(11, "Phi4", vstart[11], step[11], low[11],up[11],ierflg);

	gMinuit->SetPrintLevel(-1);
	
	arglist[0] =0;
	gMinuit->mnexcm("SET NOW", arglist,1,ierflg);
	gMinuit->SetErrorDef(0.5);
	
	arglist[0]=2;
	gMinuit->mnexcm("SET STR", arglist,1,ierflg);
	
	arglist[0] = 1;
	gMinuit->mnexcm("SET ERR", arglist ,1,ierflg);
	
	arglist[1] = 0.1;
	arglist[0] = 500;
	gMinuit->mnexcm("SIMPLEX",arglist,1,ierflg);
	result=ierflg;
	
	for (Int_t ii=0; ii<12; ii++)
	{
		gMinuit->GetParameter(ii,output[ii],errOutput[ii]);	
	}
	return(result);
}


Int_t HHypKineFitEta::minuitFitElec(Int_t select)
{
  Int_t result, ierflg = 0;

	
	if(!gMinuit) gMinuit=new TMinuit(12);
	
	gMinuit->SetFCN(fcnEtaElec);
	gMinuit->SetObjectFit(this);
	
	Double_t arglist[12];
	Double_t vstart[13],low[13],up[13];
	Double_t step[13];
	
	for(Int_t i=0; i<12; i++)
	{
		vstart[i]=input[i];//+5*errInput[i];
		step[i]=errInput[i]*2.;///10.;///2.;
		low[i]=input[i]-25.*errInput[i];
		up[i]=input[i]+25.*errInput[i];
	}
	
	gMinuit->mnparm(0, "P1", vstart[0],step[0],low[0],up[0],ierflg);
	gMinuit->mnparm(1, "Theta1",vstart[1], step[1],low[1],up[1],ierflg);
	gMinuit->mnparm(2, "Phi1", vstart[2], step[2],low[2],up[2],ierflg);
	gMinuit->mnparm(3, "P2", vstart[3], step[3], low[3],up[3],ierflg);
	gMinuit->mnparm(4, "Theta2",vstart[4], step[4],low[4],up[4],ierflg);
	gMinuit->mnparm(5, "Phi2", vstart[5], step[5], low[5],up[5],ierflg);
	gMinuit->mnparm(6, "P3", vstart[6], step[6], low[6],up[6],ierflg);
	gMinuit->mnparm(7, "Theta3",vstart[7], step[7],low[7],up[7],ierflg);
	gMinuit->mnparm(8, "Phi3", vstart[8], step[8], low[8],up[8],ierflg);
	gMinuit->mnparm(9, "P4", vstart[9], step[9], low[9],up[9],ierflg);
	gMinuit->mnparm(10, "Theta4", vstart[10], step[10],low[10],up[10],ierflg);
	gMinuit->mnparm(11, "Phi4", vstart[11], step[11], low[11],up[11],ierflg);
	
	gMinuit->SetPrintLevel(-1);
	
	arglist[0] =0;
	gMinuit->mnexcm("SET NOW", arglist, 1,ierflg);
	gMinuit->SetErrorDef(0.5);
	
	arglist[0]=2;
	gMinuit->mnexcm("SET STR", arglist,1,ierflg);
	
	arglist[0] = 1;
	gMinuit->mnexcm("SET ERR", arglist ,1,ierflg);
	
	arglist[1] = 0.1;
	arglist[0] = 500;
	gMinuit->mnexcm("SIMPLEX",arglist,1,ierflg);
	result=ierflg;
	  
	for (Int_t ii=0; ii<12; ii++)
	{
		gMinuit->GetParameter(ii,output[ii],errOutput[ii]);	
	}
	return(result);
}
