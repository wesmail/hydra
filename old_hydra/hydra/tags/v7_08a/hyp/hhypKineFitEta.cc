//*-- AUTHOR : Anar
//*-- HypCopy : B. Spruck
//*-- Modified : 26/10/2005 by B. Spruck

#include "hhypKineFitEta.h"
#include "hhypKineRefitUtils.h"
#include "hgeomvector.h"

using namespace std;
ClassImp(HHypKineFitEta);

// needed for minuit called function -> not object oriented???
HHypKineRefitUtils kU(2200.);

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
	
	
	for(int i=0; i<12; i++)
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
	
	//bu adron kanali uchun
	funct+=10.*(F-135.*135.)*(F-135.*135.)/errF;
	f=funct;
	
}

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
	
	for(int i=0; i<12; i++)
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

void fcnEta0(Int_t &npar, Double_t *gin, Double_t &f, Double_t *par, Int_t iflag)
{
	HHypKineFitEta *myObject=(HHypKineFitEta*)(gMinuit->GetObjectFit());
	Float_t F[4],errF[4];
	//F -miissing mass
	Float_t fInput[12];
	Float_t fErrInput[12];
	
	
	myObject->GetInput(fInput,fErrInput);
	HGeomVector data[4];
	Float_t funct = 0;
	
	//par[0] par[1] par[2] par[3] par[4] par[5] par[6]
	data[0].setXYZ(par[0]*sin(par[1])*cos(par[2]),par[0]*sin(par[1])*sin(par[2]),par[0]*cos(par[1]));
	data[1].setXYZ(par[3]*sin(par[4])*cos(par[5]),par[3]*sin(par[4])*sin(par[5]),par[3]*cos(par[4]));
	data[2].setXYZ(par[6]*sin(par[7])*cos(par[8]),par[6]*sin(par[7])*sin(par[8]),par[6]*cos(par[7]));
	data[3].setXYZ(par[9]*sin(par[10])*cos(par[11]),par[9]*sin(par[10])*sin(par[11]),par[9]*cos(par[10]));
	
	// F=kU.missing4(data[0],data[1],data[2],data[3]);
	kU.calcFConserv(par,fErrInput,F,errF);
	
	for(Int_t i=0; i<4; i++){
		funct+=1000000000.*F[i]*F[i];///errF[i];
	}
	f = funct;
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

void HHypKineFitEta::minuitFitSeek(Int_t select)
{  
	Int_t ierflg = 0;
	
	if(!gMinuit) gMinuit=new TMinuit(12);
	
	gMinuit->SetFCN(fcnEtaPi0);
	gMinuit->SetObjectFit(this);
		
	Double_t arglist[12];	
	Double_t vstart[13],low[13],up[13];
	Double_t step[13];
	
	for(Int_t i=0; i<12; i++)
	{
		vstart[i]=input[i];//-errInput[i];
		step[i]=errInput[i];///10.;///2.;
		low[i]=input[i]-5.*errInput[i];
		up[i]=input[i]+5.*errInput[i];
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
	
	arglist[0] = 1;	
	gMinuit->mnexcm("SET ERR", arglist ,1,ierflg);
	
	arglist[0] = 20;
	gMinuit->mnexcm("SEEK",arglist,1,ierflg);
	
	arglist[1] = 1;
	arglist[0] = 500;
	
	for (Int_t ii=0; ii<12; ii++)
	{
		gMinuit->GetParameter(ii,output[ii],errOutput[ii]);
	}
}

void HHypKineFitEta::minuitFit(Int_t select)
{
	Int_t ierflg = 0;
	
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
	
	for (Int_t ii=0; ii<12; ii++)
	{
		gMinuit->GetParameter(ii,output[ii],errOutput[ii]);	
	}
}


void HHypKineFitEta::minuitFitElec(Int_t select)
{
	Int_t ierflg = 0;
	
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
	  
	for (Int_t ii=0; ii<12; ii++)
	{
		gMinuit->GetParameter(ii,output[ii],errOutput[ii]);	
	}
}


void HHypKineFitEta::minuitFit2()
{
	Int_t ierflg = 0;
	
	if(gMinuit) delete gMinuit;
	gMinuit=new TMinuit(12);
	
	gMinuit->SetFCN(fcnEtaPi0);
	gMinuit->SetObjectFit(this);
	
	Double_t arglist[12];
	Double_t vstart[13],low[13],up[13];
	Double_t step[13];
	for(Int_t i=0; i<12; i++)
	{
		vstart[i]=gInput[i];
		step[i]=errInput[i]/10.;///2.;
		low[i]=input[i]-10.*errInput[i];
		up[i]=input[i]+10.*errInput[i];
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
	
	arglist[1] = 0.000001;
		
	arglist[0] = 1000000000;
	gMinuit->mnexcm("MIGRAD",arglist,1,ierflg);
	gMinuit->mnexcm("MINIMIZE",arglist,3,ierflg);
	Double_t amin,edm,errdef;
	Int_t nvpar,nparx,icstat;
	for (Int_t ii=0; ii<12; ii++)
	{
		gMinuit->GetParameter(ii,output[ii],errOutput[ii]);
		
	}
}

void HHypKineFitEta::minuitFit0(Int_t select)
{
	Int_t ierflg = 0;
	
	if(gMinuit) delete gMinuit;
	gMinuit=new TMinuit(12);
	gMinuit->SetObjectFit(this);
	
	gMinuit->SetFCN(fcnEta0);
	
	Double_t arglist[2];//={0,0,0,0,0,0,0,0,0,0,0,0};
	Double_t vstart[13];
	Double_t step[13];
 	for(Int_t i=0; i<12; i++)
	{
		vstart[i]=input[i]-input[i]*5./100.;
		step[i]=0.01;
	}
  
	gMinuit->mnparm(0, "P1", vstart[0],1., 0.,0.,ierflg);
	gMinuit->mnparm(1, "Theta1",vstart[1], step[1],0.,0,ierflg);
	gMinuit->mnparm(2, "Phi1", vstart[2], step[2],0.,0.,ierflg);
	gMinuit->mnparm(3, "P2", vstart[3], 1., 0,0,ierflg);
	gMinuit->mnparm(4, "Theta2",vstart[4], step[4], 0.,0,ierflg);
	gMinuit->mnparm(5, "Phi2", vstart[5], step[5], 0,0,ierflg);
	gMinuit->mnparm(6, "P3", vstart[6], 1., 0,0,ierflg);
	gMinuit->mnparm(7, "Theta3",vstart[7], step[7], 0,0,ierflg);
	gMinuit->mnparm(8, "Phi3", vstart[8], step[5], 0.,0.,ierflg);
	gMinuit->mnparm(9, "P4", vstart[9], 1., 0,0,ierflg);
	gMinuit->mnparm(10, "Theta4", vstart[10], step[7], 0.,0,ierflg);
	gMinuit->mnparm(11, "Phi4", vstart[11], step[7], 0.,0.,ierflg);
		
	gMinuit->SetPrintLevel(-1);
	
	arglist[0] =0;
	gMinuit->mnexcm("SET PRINT", arglist,1,ierflg);
	
	arglist[0] = 1000000;
	arglist[1] = 10.;
	gMinuit->mnexcm("MIGRAD",arglist,1,ierflg);

	for (Int_t ii=0; ii<12; ii++)
	{
		gMinuit->GetParameter(ii,output[ii],errOutput[ii]);
	}
}

void HHypKineFitEta::miniChi2()
{
	//burada output1 ve output2, kvadrat tenliyin iki helli vardir;
	//nParam, parametrlerin sayi:
	Float_t foutput[12],dOutput[12]={1.,0.0001,0.0001,1.,0.0001,0.0001,1.,0.0001,0.0001,1.,0.0001,0.0001};
	Float_t Toutput[12];  
	Float_t cHi1,cHi2,cHi3,temp;
	
	for(Int_t i=0; i<12; i++) foutput[i]=input[i]+5.*input[i]/100.;
	
	for(Int_t i=0; i<12; i++)
	{
		cHi2=calcChi(foutput);
		
		foutput[i]+=dOutput[i];
		
		cHi3=calcChi(foutput);
		
		if(cHi3>=cHi2) 
		{
			dOutput[i]*=-1.;
			temp=cHi2;
			cHi2=cHi3;
			cHi3=temp;
			
		}
		
		while(cHi3<=cHi2)
		{
			cHi1=cHi2;
			cHi2=cHi3;
			foutput[i]+=dOutput[i];
			cHi3=calcChi(foutput);
			//  cout<<cHi2<<"   "<<cHi3<<"   "<<dOutput[i]<<endl;
			//  cout<<i<<"  while"<<endl;	
		}
		Float_t a3=foutput[i];
		Float_t a2=foutput[i]-dOutput[i];
		Float_t a1=foutput[i]-2*dOutput[i];
		Float_t CHI[3]={cHi1,cHi2,cHi3};
		Float_t A[3]={a1,a2,a3};
		
		//cout<<a1<<"    a1    "<<a2<<"   a2 "<<a3<<endl; 
		findMinimum(A,CHI,output[i]);   
	}
}


void HHypKineFitEta::miniChi2For4()
{
	//burada output1 ve output2, kvadrat tenliyin iki helli vardir;
	//nParam, parametrlerin sayi:
	Float_t foutput[12],dOutput[12]={1.,0.0001,0.0001,1.,0.0001,0.0001,1.,0.0001,0.0001,1.,0.0001,0.0001};
	Float_t Toutput[12];  
	Float_t cHi1,cHi2,cHi3,temp;
	
	for(Int_t i=0; i<12; i++) foutput[i]=input[i]+5.*input[i]/100.;
	
	for(Int_t i=0; i<12; i++)
	{
		cHi2=calcChiFor4(foutput);
		foutput[i]+=dOutput[i];
		cHi3=calcChiFor4(foutput);
		if(cHi3>=cHi2) 
		{
			dOutput[i]*=-1.;
			temp=cHi2;
			cHi2=cHi3;
			cHi3=temp;
			
		}
		
		while(cHi3<=cHi2)
		{
			cHi1=cHi2;
			cHi2=cHi3;
			foutput[i]+=dOutput[i];
			cHi3=calcChiFor4(foutput);
			//  cout<<cHi2<<"   "<<cHi3<<"   "<<dOutput[i]<<endl;
			//  cout<<i<<"  while"<<endl;	
		}
		
		
		Float_t a3=foutput[i];
		Float_t a2=foutput[i]-dOutput[i];
		Float_t a1=foutput[i]-2*dOutput[i];
		Float_t CHI[3]={cHi1,cHi2,cHi3};
		Float_t A[3]={a1,a2,a3};
		
		//      cout<<a1<<"    a1    "<<a2<<"   a2 "<<a3<<endl; 
		findMinimum(A,CHI,output[i]);   
	}
}

Float_t HHypKineFitEta::calcChi(Float_t *foutput)
{
	Float_t cHi=0;
	HGeomVector data[4];
	for(Int_t i=0; i<12; i++)
	{
		cHi+=(input[i]-foutput[i])*(input[i]-foutput[i])/errInput[i]/errInput[i];
	}
	
	for(Int_t i=0; i<4; i++)
	{
		data[i].setXYZ(foutput[3*i]*sin(foutput[3*i+1])*cos(foutput[3*i+2]),foutput[3*i]*sin(foutput[3*i+1])*sin(foutput[3*i+2]),foutput[3*i]*cos(foutput[3*i+1]));
	}
	//Float_t F=kUU->missing4(data[0],data[1],da);
	
	Float_t F=kUU->missing4(foutput);
	
	cHi+=1000000000.*(F-135*135)*(F-135*135); 
	return cHi;
}

Float_t HHypKineFitEta::calcChiFor4(Float_t *foutput)
{
	Float_t cHi=0;
	HGeomVector data[4];
	Float_t F[4],errF[4];
	for(Int_t i=0; i<12; i++)
	{
		cHi+=(input[i]-foutput[i])*(input[i]-foutput[i])/errInput[i]/errInput[i];
	}
	
	for(Int_t i=0; i<4; i++)
	{
		data[i].setXYZ(foutput[3*i]*sin(foutput[3*i+1])*cos(foutput[3*i+2]),foutput[3*i]*sin(foutput[3*i+1])*sin(foutput[3*i+2]),foutput[3*i]*cos(foutput[3*i+1]));
	}
	//  kUU->calcFConserv(input,errInput,F,errF);
	
	//  Float_t F=kUU->missing4(foutput);
	
	for(Int_t i=0; i<4; i++)
	cHi+=1000000000.*F[i]*F[i]/errF[i]; 
	return cHi;
}

void HHypKineFitEta::findMinimum(Float_t *a,Float_t *chi,Double_t &foutput)
{
	Float_t AA,BB,CC;
	
	Float_t delta[3][3]={{a[0]*a[0],a[0],1},
						{a[1]*a[1],a[1],1},
						{a[2]*a[2],a[2],1}};
	
	Float_t deltaA[3][3]={{chi[0],a[0],1},
				{chi[1],a[1],1},
				{chi[2],a[2],1}};
	
	Float_t deltaB[3][3]={{a[0]*a[0],chi[0],1},
						{a[1]*a[1],chi[1],1},
						{a[2]*a[2],chi[2],1}};
	
	Float_t deltaC[3][3]={{a[0]*a[0],a[0],chi[0]},
						{a[1]*a[1],a[1],chi[1]},
				{a[2]*a[2],a[2],chi[2]}};
	
	
	// cout<<"D E TE R M I N A N T"<<endl;
	if(det3(delta)==0)
	{
		foutput=-100000;
	}else{ 
		AA=det3(deltaA)/det3(delta);
		BB=det3(deltaB)/det3(delta);
		CC=det3(deltaC)/det3(delta);
		foutput=-BB/2/AA;
	}
}

Float_t HHypKineFitEta::det3(Float_t a[3][3])
{
   //calculates determinant of size 3
   return (a[0][0]*a[1][1]*a[2][2]+a[1][0]*a[2][1]*a[0][2]+
	      a[0][1]*a[1][2]*a[2][0]-a[0][2]*a[1][1]*a[2][0]-
	      a[2][1]*a[1][2]*a[0][0]-a[1][0]*a[0][1]*a[2][2]);
}
