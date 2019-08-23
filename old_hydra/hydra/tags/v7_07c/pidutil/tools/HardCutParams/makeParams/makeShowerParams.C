#include <TROOT.h>
#include <TFile.h>
#include <TH2.h>
#include <TLine.h>
#include <TF1.h>
#include <TLegend.h>
#include <TCanvas.h>
#include <TFormula.h>
#include <hpidhistogramscont.h>
#include <iostream>

#define MOM_ALG 0   // kick plane
//#define MOM_ALG 1 // spline
//#define MOM_ALG 2 // runge kuta

Float_t getThr(TH1F* ph1, Float_t fProb)
{
Float_t sumTot = ph1->Integral();
Int_t nBins = ph1->GetNbinsX();

Int_t i;
Float_t thr = 0, sum = 0;

	for(i = 0; i <= nBins; i++)
	{
	  sum += ph1->GetBinContent(i);

	  if(sum >= (fProb*sumTot))
	  {
		thr = ph1->GetBinCenter(i);
		break;
	  }
	}
return thr;
}

void makeShowerParams()
{
// sim/exp comment/uncomment
TString context="sim";
//TString context="exp";

//input histogram file
TString inFile = "../projectHistograms/testhisto.root";

// momentum steps 
float MOM_STEP[]={0.,100.,200.,300.,400.,500.}; 
const int MOM_SLICE = (sizeof(MOM_STEP)/sizeof(float)-1); 

// CL levels  
Float_t  CL_PROBF10 = 0.8; // 80% electrons survive
Float_t  CL_PROBF20 = 0.8; //  80% electrons survive
Float_t  CL_PROBMAXF = 0.8;  // 80% electrons survive
Float_t  CL_PROBSUM0 = 0.95; // 95% electrons survive

// --- not need to be changed-------------------------------------

char  name[256];
float min=0,max=0;

TFile *f = new TFile(inFile.Data());
f->cd();

// output parameter file
sprintf(name,"showerparams_maxF_%4.2fCL_sum0_%4.2fCL_%s.root",CL_PROBF10,CL_PROBSUM0,context.Data());
TString outFile = name;

TH2F *pShowF10VsMom[6]; // momentum algorithm nb./sector nb.
TH2F *pShowF20VsMom[6];
TH2F *pShowMaxFVsMom[6];
TH2F *pShowSum0VsMom[6];

TH1F *pF10[6][MOM_SLICE];  // F10 multiplication factor for all pids 
TH1F *pF20[6][MOM_SLICE];  // F20 multiplication factor for all pids
TH1F *pMaxF[6][MOM_SLICE]; // MaxF = max (F10,F20) for all pids 
TH1F *pSum0[6][MOM_SLICE]; // pSum0 sum of charge in pre-shower for all pids 

TH1F *pThrF10[6];  // Threshold on F10 multiplication factor for all pids 
TH1F *pThrF20[6];  // Threshold on F20 multiplication factor for all pids
TH1F *pThrMaxF[6]; // Threshold on MaxF = max (F10,F20) for all pids 
TH1F *pThrSum0[6]; // Upper limit on pSum0 sum of charge in Pre-shower for all pids 

Float_t  PROBF10 = 1.0-CL_PROBF10; 
Float_t  PROBF20 = 1.0-CL_PROBF20;
Float_t  PROBMAXF = 1.0-CL_PROBMAXF;
Float_t  PROBSUM0 = CL_PROBSUM0;

	 for(Int_t iSec = 0; iSec < 6; iSec++) // sectors
	 {
			// get histograms from the file
			sprintf(name,"pShowF10VsMom_alg%d_sec%d",MOM_ALG,iSec);
			pShowF10VsMom[iSec] = (TH2F*)f->Get(name);

			sprintf(name,"pShowF20VsMom_alg%d_sec%d",MOM_ALG,iSec);
			pShowF20VsMom[iSec] = (TH2F*)f->Get(name);

			sprintf(name,"pShowMaxFVsMom_alg%d_sec%d",MOM_ALG,iSec);
			pShowMaxFVsMom[iSec] = (TH2F*)f->Get(name);

			sprintf(name,"pShowSum0VsMom_alg%d_sec%d",MOM_ALG,iSec);
			pShowSum0VsMom[iSec] = (TH2F*)f->Get(name);

			// create threshold histograms
			
			sprintf(name,"pThrF10_sec%d",iSec);
			pThrF10[iSec]= new TH1F(name,name,MOM_SLICE,MOM_STEP);

			sprintf(name,"pThrF20_sec%d",iSec);
			pThrF20[iSec]= new TH1F(name,name,MOM_SLICE,MOM_STEP);

			sprintf(name,"pThrMaxF_sec%d",iSec);
			pThrMaxF[iSec]= new TH1F(name,name,MOM_SLICE,MOM_STEP);

			sprintf(name,"pThrSum0_sec%d",iSec);
			pThrSum0[iSec]= new TH1F(name,name,MOM_SLICE,MOM_STEP);
	  }
// ------------------------------------------

	 for(Int_t iSec = 0; iSec < 6; iSec++) // sectors
	 {
		for(Int_t iSlice = 0; iSlice < MOM_SLICE; iSlice++) // momentum slices
		{
			min = (int)MOM_STEP[iSlice];   // lower limit of the momentum
			max = (int)MOM_STEP[iSlice+1]; // upper limit of the momentum

			//printf("min=%4.2f,max=%4.2f\n",min,max);

			sprintf(name,"pF10_sec%d_slice%d_py",iSec,iSlice);
			pShowF10VsMom[iSec]->ProjectionY(name,
							  pShowF10VsMom[iSec]->GetXaxis()->FindBin(min),
							  pShowF10VsMom[iSec]->GetXaxis()->FindBin(max));
			pF10[iSec][iSlice]=(TH1F*)gROOT->FindObject(name);

			sprintf(name,"pF20_sec%d_slice%d_py",iSec,iSlice);
			pShowF20VsMom[iSec]->ProjectionY(name,
							  pShowF20VsMom[iSec]->GetXaxis()->FindBin(min),
							  pShowF20VsMom[iSec]->GetXaxis()->FindBin(max));
			pF20[iSec][iSlice]=(TH1F*)gROOT->FindObject(name);

			sprintf(name,"pMaxF_sec%d_slice%d_py",iSec,iSlice);
			pShowMaxFVsMom[iSec]->ProjectionY(name,
							  pShowMaxFVsMom[iSec]->GetXaxis()->FindBin(min),
							  pShowMaxFVsMom[iSec]->GetXaxis()->FindBin(max));
			pMaxF[iSec][iSlice]=(TH1F*)gROOT->FindObject(name);

			sprintf(name,"pSum0_sec%d_slice%d_py",iSec,iSlice);
			pShowSum0VsMom[iSec]->ProjectionY(name,
							  pShowSum0VsMom[iSec]->GetXaxis()->FindBin(min),
							  pShowSum0VsMom[iSec]->GetXaxis()->FindBin(max));
			pSum0[iSec][iSlice]=(TH1F*)gROOT->FindObject(name);
		}
	}
	
   TCanvas *can = new TCanvas("can","can");
   can->Divide(3,2);
   for(Int_t iSec=0; iSec<6; iSec++)
   {
     can->cd(iSec+1);
     pMaxF[iSec][4]->Draw();
   }
// ------------------------------------------
// calculate thresholds

float thr;
float mom;	

 	for(Int_t iSec = 0; iSec < 6; iSec++) // sectors
 	{
		for(Int_t iSlice = 0; iSlice < MOM_SLICE; iSlice++) // momentum slices
		{
			mom=(MOM_STEP[iSlice]+MOM_STEP[iSlice+1])/2.;

	        printf("sec%d mom%6.3f \n",iSec,mom);

			thr = getThr(pF10[iSec][iSlice],PROBF10);
			pThrF10[iSec]->Fill(mom,thr);

        	printf("thrF10=%6.3f\n",thr);

			thr = getThr(pF20[iSec][iSlice],PROBF20);
			pThrF20[iSec]->Fill(mom,thr);

        	printf("thrF20=%6.3f\n",thr);

			thr = getThr(pMaxF[iSec][iSlice],PROBMAXF);
			pThrMaxF[iSec]->Fill(mom,thr);

        	printf("thrMaxF=%6.3f\n",thr);

			thr = getThr(pSum0[iSec][iSlice],PROBSUM0);
			pThrSum0[iSec]->Fill(mom,thr);

        	printf("thrSum0=%6.3f\n",thr);

			printf("--------------------------- \n");
		}
	}
		
// ------------------------------------------
// fit 3rd order polynomials (threshold parametrization)

TH1F *pThrF10Param[6];
TH1F *pThrF20Param[6];
TH1F *pThrMaxFParam[6];
TH1F *pThrSum0Param[6];

 	for(Int_t iSec = 0; iSec < 6; iSec++) // sectors
 	{
		sprintf(name,"pThrF10Param_sec%d",iSec);
        pThrF10Param[iSec] = new TH1F(name,name,4,0,4 );


		sprintf(name,"pThrF20Param_sec%d",iSec);
        pThrF20Param[iSec] = new TH1F(name,name,4,0,4 );

		sprintf(name,"pThrMaxFParam_sec%d",iSec);
        pThrMaxFParam[iSec] = new TH1F(name,name,4,0,4 );

		sprintf(name,"pThrSum0Param_sec%d",iSec);
        pThrSum0Param[iSec] = new TH1F(name,name,3,0,3 );

	// fit and fill parametrization 
	pThrF10[iSec]->Fit("pol3","","",200,1500);
	for(int i =0; i <4; i++) pThrF10Param[iSec]->SetBinContent(i+1,((TF1 *)gROOT->FindObject("pol3"))->GetParameter(i));

	pThrF20[iSec]->Fit("pol3","","",300,1500);
	for(int i =0; i <4; i++) pThrF20Param[iSec]->SetBinContent(i+1,((TF1 *)gROOT->FindObject("pol3"))->GetParameter(i));

	pThrMaxF[iSec]->Fit("pol3","","",150,1500);
	for(int i =0; i <4; i++) pThrMaxFParam[iSec]->SetBinContent(i+1,((TF1 *)gROOT->FindObject("pol3"))->GetParameter(i));

	pThrSum0[iSec]->Fit("pol2","","",0,1600);
        for(int i =0; i <3; i++) pThrSum0Param[iSec]->SetBinContent(i+1,((TF1 *)gROOT->FindObject("pol2"))->GetParameter(i));
	}

//----------------------------

HPidHistogramsCont *p = new HPidHistogramsCont(context.Data(),
                        "Shower Parameters F10,f20,MaxF,Sum0", "", 4);
Int_t aPart[] = {2,3}; // pid of particles i am intrested

 for(Int_t iSec = 0; iSec < 6; iSec++) // sectors
 {
	for(Int_t iPol = 0; iPol < 2; iPol++) // number of pids
	{
		p->addHistogram(aPart[iPol], 0, iSec, pThrF10Param[iSec]); 
		p->addHistogram(aPart[iPol], 1, iSec, pThrF20Param[iSec]);
		p->addHistogram(aPart[iPol], 2, iSec, pThrMaxFParam[iSec]); 
		p->addHistogram(aPart[iPol], 3, iSec, pThrSum0Param[iSec]);
	}
 }

	TFile *pOld = gFile;
    TFile *pOut = new TFile(outFile.Data(), "recreate");
    p->Write();

    delete pOut;
    pOld->cd();
}
