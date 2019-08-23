#include <TROOT.h>
#include <TFile.h>
#include <TH2.h>
#include <TLine.h>
#include <TF1.h>
#include <TLegend.h>
#include <TFormula.h>
#include "hpidhistogramscont.h"

#define MOM_ALG 0   // kick plane
//#define MOM_ALG 1 // spline
//#define MOM_ALG 2 // runge kuta

#define BETASYS0_MIN 0.9  // beta lower limit of the fit for sys0  
#define BETASYS0_MAX 1.1  // beta upper limit of the fit for sys0  
#define BETASYS1_MIN 0.9  // beta lower limit of the fit for sys1  
#define BETASYS1_MAX 1.1  // beta upper limit of the fit for sys1  

void makeBetaParams()
{

// --sim/exp and 
TString context = "sim";
//TString context = "exp";

// -- input histogram file
TString inFile = "../projectHistograms/testhisto.root";

// -- momentum steps
float MOM_STEP[]= {0,300,600}; 
const int MOM_SLICE = ( sizeof(MOM_STEP)/sizeof(Float_t) -1); 

// -- window width (in sigmas)
Float_t  SIGMA_FACT=3.0;

// -- not need to be changed ------------------------------------------------------
//
char  name[256];
int   min=0,max=0;

TFile *f = new TFile(inFile.Data());
f->cd();

sprintf(name,"betaparams_%3.1fsigma_%s.root",SIGMA_FACT,context.Data());
TString outFile = name;

TH2F *pMomVsBeta[6][2][2]; // sector/system/charge

TH1F *pBeta[6][2][2][MOM_SLICE]; // beta for all pids  sys0,sys1 
TH1F *pBetaMin[6][2][2]; // min beta window for all pids  sys0,sys1 
TH1F *pBetaMax[6][2][2]; // max beta window for all pids  sys0,sys1

	 for(Int_t iSec = 0; iSec < 6; iSec++) // sectors
	 {
		for(Int_t iSys = 0; iSys < 2; iSys++) // systems
		{
			for(Int_t iPol = 0; iPol < 2; iPol++) // polarization
			{
				// get histograms from the file
				sprintf(name,"pMomVsBeta_alg%d_sec%d_sys%d_pol%d",MOM_ALG,iSec,iSys,iPol);
				pMomVsBeta[iSec][iSys][iPol] = (TH2F*)f->Get(name);

				// create beta histograms for momenta slices
				for(Int_t iSlice = 0; iSlice < MOM_SLICE; iSlice++) // momentum slices
				{
					sprintf(name,"pBeta_sec%d_sys%d_pol%d_slice%d",iSec,iSys,iPol,iSlice);
					//pBeta[iSec][iSys][iPol][iSlice] = new TH1F(name,name,MOM_SLICE,0,MOM_SLICE*MOM_STEP); 
					pBeta[iSec][iSys][iPol][iSlice] = new TH1F(name,name,MOM_SLICE,MOM_STEP); 
				}

				// create beta_min and beta_max histograms
					sprintf(name,"pBetaMin_sec%d_sys%d_pol%d",iSec,iSys,iPol);
					pBetaMin[iSec][iSys][iPol] = new TH1F(name,name,MOM_SLICE,MOM_STEP);

					sprintf(name,"pBetaMax_sec%d_sys%d_pol%d",iSec,iSys,iPol);
					pBetaMax[iSec][iSys][iPol] = new TH1F(name,name,MOM_SLICE,MOM_STEP);
			}
		} 
	 }

// ------------------------------------------

	 for(Int_t iSec = 0; iSec < 6; iSec++) // sectors
	 {
		for(Int_t iSys = 0; iSys < 2; iSys++) // systems
		{
			for(Int_t iPol = 0; iPol < 2; iPol++) // polarization
			{
				for(Int_t iSlice = 0; iSlice < MOM_SLICE; iSlice++) // momentum slices
				{
					
					min = (int)MOM_STEP[iSlice]; // lower limit of the momentum
					max = (int)MOM_STEP[iSlice+1];    // upper limit of the momentum

					//printf("min = %d, max = %d\n",min,max);

					sprintf(name,"pBeta_sec%d_sys%d_pol%d_slice%d_px",iSec,iSys,iPol,iSlice);
					pMomVsBeta[iSec][iSys][iPol]->ProjectionX(name,
							          pMomVsBeta[iSec][iSys][iPol]->GetYaxis()->FindBin(min),
							          pMomVsBeta[iSec][iSys][iPol]->GetYaxis()->FindBin(max));
				    pBeta[iSec][iSys][iPol][iSlice]=(TH1F*)gROOT->FindObject(name);
				}
			}
		}
	}
	
// ------------------------------------------
Float_t minBeta[6][2][2][MOM_SLICE];
Float_t maxBeta[6][2][2][MOM_SLICE];

Float_t mom[MOM_SLICE];	

 for(Int_t iSec = 0; iSec < 6; iSec++) // sectors
 {
	for(Int_t iSys = 0; iSys < 2; iSys++) // systems
	{
		for(Int_t iPol = 0; iPol < 2; iPol++) // polarization
		{
			for(Int_t iSlice = 0; iSlice < MOM_SLICE; iSlice++) // momentum slices
			{
				mom[iSlice]=(MOM_STEP[iSlice]+MOM_STEP[iSlice+1])/2.;

				if(iSys==0) pBeta[iSec][iSys][iPol][iSlice]->Fit("gaus","","",BETASYS0_MIN,BETASYS0_MAX);
				else pBeta[iSec][iSys][iPol][iSlice]->Fit("gaus","","",BETASYS1_MIN,BETASYS1_MAX);
				
				TF1 *gaus = (TF1*)gROOT->FindObject("gaus"); 
				
				minBeta[iSec][iSys][iPol][iSlice] = gaus->GetParameter(1)-SIGMA_FACT*gaus->GetParameter(2);
        		maxBeta[iSec][iSys][iPol][iSlice] = gaus->GetParameter(1)+SIGMA_FACT*gaus->GetParameter(2);
				pBetaMin[iSec][iSys][iPol]->Fill(mom[iSlice],minBeta[iSec][iSys][iPol][iSlice]);
        		pBetaMax[iSec][iSys][iPol]->Fill(mom[iSlice],maxBeta[iSec][iSys][iPol][iSlice]);

        		printf("sec%d iSys%d iPol%d mom%6.3f \n",iSec,iSys,iPol,mom[iSlice]);
        		printf("minBeta=%6.3f\n",minBeta[iSec][iSys][iPol][iSlice]);
        		printf("maxBeta=%6.3f\n",maxBeta[iSec][iSys][iPol][iSlice]);
				printf("--------------------------- \n");
			}
		}
	}
 }
		
// ------------------------------------------
HPidHistogramsCont *p = new HPidHistogramsCont(context.Data(),
                        "Beta window vs Mom: MinSys0 MaxSys0 MinSys1 MaxSys1", "", 4);
Int_t aPart[] = {2,3}; // pid of particles i am intrested

 for(Int_t iSec = 0; iSec < 6; iSec++) // sectors
 {
	for(Int_t iPol = 0; iPol < 2; iPol++) // number of pids
	{
		p->addHistogram(aPart[iPol], 0, iSec, pBetaMin[iSec][0][iPol]); 
		p->addHistogram(aPart[iPol], 1, iSec, pBetaMax[iSec][0][iPol]);
		p->addHistogram(aPart[iPol], 2, iSec, pBetaMin[iSec][1][iPol]); 
		p->addHistogram(aPart[iPol], 3, iSec, pBetaMax[iSec][1][iPol]);
	}
 }
	TFile *pOld = gFile;
    TFile *pOut = new TFile(outFile.Data(), "recreate");
    p->Write();

    delete pOut;
    pOld->cd();
}
