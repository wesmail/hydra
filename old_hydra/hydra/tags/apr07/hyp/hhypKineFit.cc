//*-- AUTHOR : Anar
//*-- HypCopy : B. Spruck
//*-- Modified : 26/10/2005 by B. Spruck
//*-- Modified : 10/01/2006 by T. Perez     PipPimGamma Channel and Non-used functions commented out.
//_HADES_CLASS_DESCRIPTION 
////////////////////////////////////////////////////////////////////////
//
// HHypKineFit
//
// if you need any of the removed functions, retrieve a version earlier than 
// 01.02.2006... Tiago .
////////////////////////////////////////////////////////////////////////


#include "hhypKineFit.h"
#include "hgeomvector.h"

using namespace std;
ClassImp(HHypKineFit);

HHypKineFit::HHypKineFit()
{
	init(2200.);
}

HHypKineFit::HHypKineFit(Float_t* in,Float_t* errIn)
{
	init(2200.);
	
	for(Int_t i=0; i<12; i++)
	{
		fitInput[i]=in[i];
		errInput[i]=errIn[i];
		fitOutput[i]=-1000.;
		errOutput[i]=-1000.;
	}
}

HHypKineFit::~HHypKineFit()
{
	if(gMinuit) delete gMinuit;
	if(fitInput) delete [] fitInput;
	if(fitOutput) delete [] fitOutput;
	if(errInput) delete [] errInput;
	if(errOutput) delete [] errOutput;
}

void HHypKineFit::init(Float_t eB)
{
	gMinuit=0;
	fitInput=new Float_t [12];
	errInput=new Float_t [12];
	fitOutput=new Double_t [12];
	errOutput=new Double_t [12];
  
	eBeam=eB;
	mProton=938.27200;
	Float_t mPion0=134.9766;
	Float_t mPionC=139.57018;
	Float_t mElec=0.511;
	
	m2Proton=mProton*mProton;
	m2Pion0=mPion0*mPion0;
	m2PionC=mPionC*mPionC;
	m2Elec=mElec*mElec;
	
	ELab=eBeam+mProton;
	pInput=sqrt(ELab*ELab-m2Proton);

/*	ECM=sqrt(2.0*m2Proton+2.0*ELab*mProton);
	GammaF=(ELab+mProton)/ECM;
	BetaF=pInput/(ELab+mProton);
	AllE=ELab+mProton;
	AllX=0;
	AllY=0;
	AllZ=pInput;*/
}

void HHypKineFit::ResetOutput(void)
{
	for(Int_t i=0; i<12; i++)
	{
		fitOutput[i]=-1000.;
		errOutput[i]=-1000.;
	}
}

void HHypKineFit::SetInput(Float_t* in,Float_t* errIn)
{
	for(Int_t i=0; i<12; i++)
	{
		fitInput[i]=in[i];
		errInput[i]=errIn[i];
	}
}

void HHypKineFit::SetOutput(Double_t* out,Double_t* errOut)
{
	for(Int_t i=0; i<12; i++)
	{
		fitOutput[i]=out[i];
		errOutput[i]=errOut[i];
	}
}

void HHypKineFit::GetInput(Float_t* in,Float_t* errIn)
{
	for(Int_t i=0; i<12; i++)
	{
		in[i]=fitInput[i];
		errIn[i]=errInput[i];
	}
}
 
void HHypKineFit::GetOutput(Double_t* out,Double_t* errOut)
{
	for(Int_t i=0; i<12; i++)
	{
		out[i]=fitOutput[i];
		errOut[i]=errOutput[i];
	}
}

Float_t HHypKineFit::scalar(HGeomVector &data1, HGeomVector &data2)
{
   return (data1.getX()*data2.getX()+data1.getY()*data2.getY()+data1.getZ()*data2.getZ());
}


