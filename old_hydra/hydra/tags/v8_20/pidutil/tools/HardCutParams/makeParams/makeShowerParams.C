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

//#define MOM_ALG 0   // kick plane
//#define MOM_ALG 1 // spline
#define MOM_ALG 2 // runge kuta

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
//TString inFile = "/d/hades10/data/jacek/aug04/dst/urqmdflat_c1c/merged_hardcuts_histo.root";
TString inFile = "/d/hades10/data/jacek/aug04/dst/urqmdflat_c1c/merged_hardcuts_histo_05102006.root";

// momentum steps 
//float MOM_STEP[]={0.,150.,300.,450.,600.,750.,900.,1050.,1200.,1300.,1400.,1500.,1600.,1700.,1800.,1850.,1900.,1950.,2000.}; 
float MOM_STEP[]={50.,100.,200.,300.,400.,500.,600.,700.,800.,900.,1000.,1100.,1200.,1300.,1400.,1500.,1600.,1700.,1800.,1900.,2000.}; 
const int MOM_SLICE = (sizeof(MOM_STEP)/sizeof(float)-1); 

// CL levels  
Float_t  CL_PROBF10 = 0.8; // 80% electrons survive
Float_t  CL_PROBF20 = 0.8; //  80% electrons survive
Float_t  CL_PROBMAXF = 0.8;  // 80% electrons survive
Float_t  CL_PROBSUM0 = 0.95; // 95% electrons survive
Float_t  CL_PROBSUMDIFF = 0.8;  // 80% electrons survive

// --- not need to be changed-------------------------------------

char  name[256];
float min=0,max=0;

TFile *f = new TFile(inFile.Data());
f->cd();

// output parameter file
sprintf(name,"showerparams_F10_%4.2f_F20_%4.2f_maxF_%4.2f_sum0%4.2f_sumdiff_%4.2f_%s.root",CL_PROBF10,CL_PROBF20,CL_PROBMAXF,CL_PROBSUM0,CL_PROBSUMDIFF,context.Data());
TString outFile = name;

TH2F *pShowF10VsMom[6]; // momentum algorithm nb./sector nb.
TH2F *pShowF20VsMom[6];
TH2F *pShowMaxFVsMom[6];
TH2F *pShowSum0VsMom[6];
TH2F *pShowSumDiffVsMom[6];

TH2F *pShowF10VsMomTot=NULL; // momentum algorithm nb./sector nb.
TH2F *pShowF20VsMomTot=NULL;
TH2F *pShowMaxFVsMomTot=NULL;
TH2F *pShowSum0VsMomTot=NULL;
TH2F *pShowSumDiffVsMomTot=NULL;

TH1F *pF10[MOM_SLICE];  // F10 multiplication factor for all pids 
TH1F *pF20[MOM_SLICE];  // F20 multiplication factor for all pids
TH1F *pMaxF[MOM_SLICE]; // MaxF = max (F10,F20) for all pids 
TH1F *pSum0[MOM_SLICE]; // pSum0 sum of charge in pre-shower for all pids 
TH1F *pSumDiff[MOM_SLICE]; // Sum1+Sum2-Sum0 for all pids 

TH1F *pThrF10;  // Threshold on F10 multiplication factor for all pids 
TH1F *pThrF20;  // Threshold on F20 multiplication factor for all pids
TH1F *pThrMaxF; // Threshold on MaxF = max (F10,F20) for all pids 
TH1F *pThrSum0; // Upper limit on pSum0 sum of charge in Pre-shower for all pids 
TH1F *pThrSumDiff; // Threshold on Sum1+Sum2-Sum0 for all pids 

Float_t  PROBF10 = 1.0-CL_PROBF10; 
Float_t  PROBF20 = 1.0-CL_PROBF20;
Float_t  PROBMAXF = 1.0-CL_PROBMAXF;
Float_t  PROBSUM0 = CL_PROBSUM0;
Float_t  PROBSUMDIFF = 1.0-CL_PROBSUMDIFF;

     cout << "get histograms from the file"  << endl;
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

			sprintf(name,"pShowSumDiffVsMom_alg%d_sec%d",MOM_ALG,iSec);
			pShowSumDiffVsMom[iSec] = (TH2F*)f->Get(name);
	}

    // create sum histograms 

	sprintf(name,"pShowF10VsMomTot");
	pShowF10VsMomTot = (TH2F*)pShowF10VsMom[0]->Clone(); 
	pShowF10VsMomTot->SetName(name);

	sprintf(name,"pShowF20VsMomTot");
	pShowF20VsMomTot = (TH2F*)pShowF20VsMom[0]->Clone(); 
	pShowF20VsMomTot->SetName(name);

	sprintf(name,"pShowMaxFVsMomTot");
	pShowMaxFVsMomTot = (TH2F*)pShowMaxFVsMom[0]->Clone(); 
	pShowMaxFVsMomTot->SetName(name);

	sprintf(name,"pShowSum0VsMomTot");
	pShowSum0VsMomTot = (TH2F*)pShowSum0VsMom[0]->Clone(); 
	pShowSum0VsMomTot->SetName(name);

	sprintf(name,"pShowSumDiffVsMomTot");
	pShowSumDiffVsMomTot = (TH2F*)pShowSumDiffVsMom[0]->Clone(); 
	pShowSumDiffVsMomTot->SetName(name);

	for(Int_t iSec = 1; iSec < 6; iSec++) // loop has to start from 1
	{

			pShowF10VsMomTot->Add(pShowF10VsMom[iSec]);
			pShowF20VsMomTot->Add(pShowF20VsMom[iSec]);
			pShowMaxFVsMomTot->Add(pShowMaxFVsMom[iSec]);
			pShowSum0VsMomTot->Add(pShowSum0VsMom[iSec]);
			pShowSumDiffVsMomTot->Add(pShowSumDiffVsMom[iSec]);
	}

     cout << "create threshold histograms"  << endl;
			// create threshold histograms
			
			sprintf(name,"pThrF10");
			pThrF10= new TH1F(name,name,MOM_SLICE,MOM_STEP);

			sprintf(name,"pThrF20");
			pThrF20= new TH1F(name,name,MOM_SLICE,MOM_STEP);

			sprintf(name,"pThrMaxF");
			pThrMaxF= new TH1F(name,name,MOM_SLICE,MOM_STEP);

			sprintf(name,"pThrSum0");
			pThrSum0= new TH1F(name,name,MOM_SLICE,MOM_STEP);

			sprintf(name,"pThrSumDiff");
			pThrSumDiff= new TH1F(name,name,MOM_SLICE,MOM_STEP);

// ------------------------------------------

     cout << "projecting histograms"  << endl;
		for(Int_t iSlice = 0; iSlice < MOM_SLICE; iSlice++) // momentum slices
		{
			min = (int)MOM_STEP[iSlice];   // lower limit of the momentum
			max = (int)MOM_STEP[iSlice+1]; // upper limit of the momentum

			//printf("min=%4.2f,max=%4.2f\n",min,max);

			sprintf(name,"pF10_slice%d_py",iSlice);
			pShowF10VsMomTot->ProjectionY(name,
							  pShowF10VsMomTot->GetXaxis()->FindBin(min),
							  pShowF10VsMomTot->GetXaxis()->FindBin(max));
			pF10[iSlice]=(TH1F*)gROOT->FindObject(name);

			sprintf(name,"pF20_slice%d_py",iSlice);
			pShowF20VsMomTot->ProjectionY(name,
							  pShowF20VsMomTot->GetXaxis()->FindBin(min),
							  pShowF20VsMomTot->GetXaxis()->FindBin(max));
			pF20[iSlice]=(TH1F*)gROOT->FindObject(name);

			sprintf(name,"pMaxF_slice%d_py",iSlice);
			pShowMaxFVsMomTot->ProjectionY(name,
							  pShowMaxFVsMomTot->GetXaxis()->FindBin(min),
							  pShowMaxFVsMomTot->GetXaxis()->FindBin(max));
			pMaxF[iSlice]=(TH1F*)gROOT->FindObject(name);

			sprintf(name,"pSum0_slice%d_py",iSlice);
			pShowSum0VsMomTot->ProjectionY(name,
							  pShowSum0VsMomTot->GetXaxis()->FindBin(min),
							  pShowSum0VsMomTot->GetXaxis()->FindBin(max));
			pSum0[iSlice]=(TH1F*)gROOT->FindObject(name);

			sprintf(name,"pSumDiff_slice%d_py",iSlice);
			pShowSumDiffVsMomTot->ProjectionY(name,
							  pShowSumDiffVsMomTot->GetXaxis()->FindBin(min),
							  pShowSumDiffVsMomTot->GetXaxis()->FindBin(max));
			pSumDiff[iSlice]=(TH1F*)gROOT->FindObject(name);
		}
	
// ------------------------------------------

   TCanvas *can = new TCanvas("can","can");
   can->Divide(3,2);
   for(Int_t iSlice=0; iSlice<6; iSlice++)
   {
     can->cd(iSlice+1);
     pSumDiff[iSlice]->Draw();
   }

// ------------------------------------------
     cout << "calculate thresholds"  << endl;
// calculate thresholds

float thr;
float mom;	

		for(Int_t iSlice = 0; iSlice < MOM_SLICE; iSlice++) // momentum slices
		{
			mom=(MOM_STEP[iSlice]+MOM_STEP[iSlice+1])/2.;

	        printf("mom%6.3f \n",mom);

			thr = getThr(pF10[iSlice],PROBF10);
			pThrF10->Fill(mom,thr);

        	printf("thrF10=%6.3f\n",thr);

			thr = getThr(pF20[iSlice],PROBF20);
			pThrF20->Fill(mom,thr);

        	printf("thrF20=%6.3f\n",thr);

			thr = getThr(pMaxF[iSlice],PROBMAXF);
			pThrMaxF->Fill(mom,thr);

        	printf("thrMaxF=%6.3f\n",thr);

			thr = getThr(pSum0[iSlice],PROBSUM0);
			pThrSum0->Fill(mom,thr);

        	printf("thrSum0=%6.3f\n",thr);

			thr = getThr(pSumDiff[iSlice],PROBSUMDIFF);
			pThrSumDiff->Fill(mom,thr);

        	printf("thrSumDiff=%6.3f\n",thr);
			
			printf("--------------------------- \n");
		}
		
// -----------------------------------------
// fit 3rd order polynomials (threshold parametrization)

TH1F *pThrF10Param[6];
TH1F *pThrF20Param[6];
TH1F *pThrMaxFParam[6];
TH1F *pThrSum0Param[6];
TH1F *pThrSumDiffParam[6];

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

		sprintf(name,"pThrSumDiffParam_sec%d",iSec);
        pThrSumDiffParam[iSec] = new TH1F(name,name,4,0,4 );

	// fit and fill parametrization 
	pThrF10->Fit("pol3","","",200,1600);
	for(int i =0; i <4; i++) pThrF10Param[iSec]->SetBinContent(i+1,((TF1 *)gROOT->FindObject("pol3"))->GetParameter(i));

	pThrF20->Fit("pol3","","",400,1600);
	for(int i =0; i <4; i++) pThrF20Param[iSec]->SetBinContent(i+1,((TF1 *)gROOT->FindObject("pol3"))->GetParameter(i));

	pThrMaxF->Fit("pol3","","",150,1800);
	for(int i =0; i <4; i++) pThrMaxFParam[iSec]->SetBinContent(i+1,((TF1 *)gROOT->FindObject("pol3"))->GetParameter(i));

	pThrSum0->Fit("pol2","","",300,1800);
        for(int i =0; i <3; i++) pThrSum0Param[iSec]->SetBinContent(i+1,((TF1 *)gROOT->FindObject("pol2"))->GetParameter(i));

	pThrSumDiff->Fit("pol3","","",350,2000);
	for(int i =0; i <4; i++) pThrSumDiffParam[iSec]->SetBinContent(i+1,((TF1 *)gROOT->FindObject("pol3"))->GetParameter(i));

	}

// ------------------------------------------

   TCanvas *can1 = new TCanvas("can1","can1");
   can1->Divide(3,2);

   can1->cd(1);
   pThrF10->Draw();
   can1->cd(2);
   pThrF20->Draw();
   can1->cd(3);
   pThrMaxF->Draw();
   can1->cd(4);
   pThrSum0->Draw();
   can1->cd(5);
   pThrSumDiff->Draw();

//----------------------------

HPidHistogramsCont *p = new HPidHistogramsCont(context.Data(),
                        "Shower Parameters F10,f20,MaxF,Sum0,SumDiff", "", 5);
Int_t aPart[] = {2,3}; // pid of particles i am intrested

 for(Int_t iSec = 0; iSec < 6; iSec++) // sectors
 {
	for(Int_t iPol = 0; iPol < 2; iPol++) // number of pids
	{
		p->addHistogram(aPart[iPol], 0, iSec, pThrF10Param[iSec]); 
		p->addHistogram(aPart[iPol], 1, iSec, pThrF20Param[iSec]);
		p->addHistogram(aPart[iPol], 2, iSec, pThrMaxFParam[iSec]); 
		p->addHistogram(aPart[iPol], 3, iSec, pThrSum0Param[iSec]);
		p->addHistogram(aPart[iPol], 4, iSec, pThrSumDiffParam[iSec]);
	}
 }

	TFile *pOld = gFile;
    TFile *pOut = new TFile(outFile.Data(), "recreate");
    p->Write();

    delete pOut;
    pOld->cd();
}
