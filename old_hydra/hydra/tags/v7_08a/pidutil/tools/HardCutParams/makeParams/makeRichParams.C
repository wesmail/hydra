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

void makeRichParams()
{
TString context = "sim";
//TString context = "exp";

HPidHistogramsCont *p = new HPidHistogramsCont(context.Data(),"Rich parameters: PM,RC,NP,AC","",1);

Int_t aPart[] = {2,3}; // pid of particles 
char name[256];

TH1F *ph[6][2]; // 6 - sectors, 2 - pids
Float_t fParams[6][4] = { {200, 2.8, 5, 4}, //pat matrix theshold,ring centroid,number of pads per ring,average charge
	                      {200, 2.8, 5, 4},
	                      {200, 2.8, 5, 4},
	                      {200, 2.8, 5, 4},
	                      {200, 2.8, 5, 4},
	                      {200, 2.8, 5, 4} };

// create and fill parameter histograms

for(Int_t iSec=0; iSec<6; iSec++) // loop over sectors 
{ 
  for(Int_t iPart=0; iPart<2; iPart++) // loop over particles 
  { 
	 sprintf(name,"ph_%d_%d",iSec,iPart );
     ph[iSec][iPart] = new TH1F(name,name,4,0,4);

		ph[iSec][iPart]->SetBinContent(1,fParams[iSec][0]); // pat matrix theshold
		ph[iSec][iPart]->SetBinContent(2,fParams[iSec][1]); // ring centroid
		ph[iSec][iPart]->SetBinContent(3,fParams[iSec][2]);   // number of pads per ring
		ph[iSec][iPart]->SetBinContent(4,fParams[iSec][3]);	  // average charge
	
	 p->addHistogram(aPart[iPart],0, iSec, ph[iSec][iPart]);
  }
}

sprintf(name,"richparams_PM%d_RC%3.1f_NP%d_AC%d_%s.root",200,2.8,5,4,context.Data());
TFile *pOut = new TFile(name, "recreate");
p->Write();

delete pOut;
}
