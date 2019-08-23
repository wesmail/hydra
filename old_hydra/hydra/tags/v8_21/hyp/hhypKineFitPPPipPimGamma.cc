//*-- AUTHOR : Anar
//*-- HypCopy : B. Spruck
//*-- Rewritten: 12/06/2006 by B. Spruck

//_HADES_CLASS_DESCRIPTION 
////////////////////////////////////////////////////////////////////////
//
// HHypKineFitPPPipPimGamma
//
////////////////////////////////////////////////////////////////////////


#include "hhypKineFitPPPipPimGamma.h"
#include "hgeomvector.h"

using namespace std;
ClassImp(HHypKineFitPPPipPimGamma);

// First the Function which are neede for the minuit stuff...

// PipPimGamma
void fcnPipPimGamma(Int_t &npar, Double_t *gin, Double_t &f, Double_t *par, Int_t iflag)
{
	HHypKineFitPPPipPimGamma *myObject=(HHypKineFitPPPipPimGamma*)(gMinuit->GetObjectFit());
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
	
	F=myObject->missing4(par);
	myObject->CalcDerNum(DER,par);
	errF=0;
	for(Int_t i=0; i<12; i++)
	{
		errF+=DER[i]*DER[i]*fErrInput[i]*fErrInput[i];
	}
	
	// The missing mass is HardCoded Here 0. <== Missing Gamma
	// Copied from the previous...
	// the 10.* is comming from ?? try with other parameters.
	// 1.- 10. BAD,  2.- 70. bad 3.- 35.
	funct+=100.*F*F/errF;
	f=funct;
}

Int_t HHypKineFitPPPipPimGamma::minuitFit(void)
{//PipPimGamma
	Int_t result, ierflg = 0;
	
	if(!gMinuit) gMinuit=new TMinuit(13);// 12 PLUS 1, or memory corruption, Internal error in Minuit?
	
	gMinuit->SetFCN(fcnPipPimGamma);
	gMinuit->SetObjectFit(this);
	
	Double_t arglist[12];
	Double_t vstart[13],low[13],up[13];
	Double_t step[13];
	
	for(Int_t i=0; i<12; i++)
	{
		vstart[i]=fitInput[i];
		step[i]=errInput[i]*2.;
		low[i]=fitInput[i]-25.*errInput[i];
		up[i]=fitInput[i]+25.*errInput[i];
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
		gMinuit->GetParameter(ii,fitOutput[ii],errOutput[ii]);
	}
	return(result);
}

void HHypKineFitPPPipPimGamma::CalcDerNum(Float_t *DER11,Double_t *Output)
{// PipPimGamma
	Double_t out[12][12];
	Double_t out1[12][12];
	for(Int_t i=0; i<12; i++)
	{
		output[i]=Output[i];  
	}
	for(Int_t j=0; j<12; j++)
	{
		for(Int_t i=0; i<12; i++)
		{
			out[j][i]=output[i];
			out1[j][i]=output[i];
		}
	}
	for(Int_t i=0; i<12; i++)
	{
		out[i][i]+=0.0001;
		out1[i][i]-=0.0001;
		if(i==3 || i==6 || i==9 || i==0)
		{
			out[i][i]+=0.1;
			out1[i][i]-=0.1;
		}
		// CHANNEL eta -> pi+ pi- gamma. i.e. missing gamma= 0
		DER11[i]=(missing4(out[i])-missing4(out1[i]))/(out[i][i]-out1[i][i])/2.;
	}
} // END CalcDerNumPipPimGamma

void HHypKineFitPPPipPimGamma::secondIter(Float_t *input,Double_t *foutput,Float_t *errInput,Double_t *out,Float_t *derOut)
{// Only for PipPimGamma
	Float_t der[12];
	CalcDerNum(der,foutput);
	Float_t fT=missing4(output);
	Float_t sum=0.;
	Float_t sum1=0.;
	Float_t lambda;
	for(Int_t i=0; i<12; i++)
	{
		sum+=der[i]*(input[i]-output[i]);
		sum1+=der[i]*der[i]*errInput[i]*errInput[i];
	}
	sum+=fT;
	lambda=sum/sum1;

	for(Int_t i=0; i<12; i++)
	{
		out[i]=input[i]-der[i]*lambda*errInput[i]*errInput[i];
	}
	
	Double_t errInput2[12];
	Double_t derLambda[12];
	
	for(Int_t i=0; i<12; i++)
	{
		errInput2[i]=errInput[i]*errInput[i];
		derLambda[i]=der[i]/sum1;
	}
	for(Int_t i=0; i<12; i++)
	{
		derOut[i]=TMath::Power((1-derLambda[i]*der[i]*errInput2[i]),2)*errInput2[i];
		for(Int_t j=0; j<12; j++)
		{
			if(j==i) continue;
			derOut[i]+=TMath::Power(der[i]*derLambda[j]*errInput2[i],2)*errInput2[j];
		}
	}
}

Float_t HHypKineFitPPPipPimGamma::missing4(HGeomVector &data3,HGeomVector &data4,HGeomVector &data5,HGeomVector &data6)
{// for _PipPimGamma (this should be the same code as for PipPimPi0)
	Float_t P1=pInput;
	Float_t P2=0.;
	Float_t P1z=P1;
	Float_t E1=sqrt(P1*P1+m2Proton);
	Float_t E2=mProton;

	HGeomVector dataa1;
	dataa1.setXYZ(0.,0.,P1z);

	Float_t P3=sqrt(data3.getX()*data3.getX()+data3.getY()*data3.getY()+data3.getZ()*data3.getZ());
	Float_t P4=sqrt(data4.getX()*data4.getX()+data4.getY()*data4.getY()+data4.getZ()*data4.getZ());
	Float_t P5=sqrt(data5.getX()*data5.getX()+data5.getY()*data5.getY()+data5.getZ()*data5.getZ());
	Float_t P6=sqrt(data6.getX()*data6.getX()+data6.getY()*data6.getY()+data6.getZ()*data6.getZ());

	Float_t E3=sqrt(P3*P3+m2Proton);
	Float_t E4=sqrt(P4*P4+m2Proton);
	Float_t E5=sqrt(P5*P5+m2PionC);
	Float_t E6=sqrt(P6*P6+m2PionC);
	Float_t totalESq=(E1+E2-E3-E4-E5-E6)*(E1+E2-E3-E4-E5-E6);
	Float_t totalPSq=P1*P1+P2*P2+P3*P3+P4*P4+P5*P5+P6*P6-
		2*scalar(dataa1,data3)-2*scalar(dataa1,data4)-
		2*scalar(dataa1,data5)-2*scalar(dataa1,data6)+
		2*scalar(data3,data4)+2*scalar(data5,data6)+
		2*scalar(data3,data5)+2*scalar(data4,data5)+
		2*scalar(data3,data6)+2*scalar(data4,data6);
	return (totalESq-totalPSq);
}

Float_t HHypKineFitPPPipPimGamma::missing4(Double_t *input)
{
	HGeomVector data[4];
	for(Int_t i=0; i<4; i++)
	{
		data[i].setXYZ(input[3*i]*sin(input[3*i+1])*cos(input[3*i+2]),input[3*i]*sin(input[3*i+1])*sin(input[3*i+2]),input[3*i]*cos(input[3*i+1]));
	}

	return (missing4(data[0],data[1],data[2],data[3]));
}

Float_t HHypKineFitPPPipPimGamma::missing4(Float_t *input)
{
	HGeomVector data[4];
	for(Int_t i=0; i<4; i++)
	{
		data[i].setXYZ(input[3*i]*sin(input[3*i+1])*cos(input[3*i+2]),input[3*i]*sin(input[3*i+1])*sin(input[3*i+2]),input[3*i]*cos(input[3*i+1]));
	}

	return (missing4(data[0],data[1],data[2],data[3]));
}
