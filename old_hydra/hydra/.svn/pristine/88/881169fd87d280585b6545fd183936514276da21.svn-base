#ifndef __CINT__
#include <stdio.h>
#include <iostream.h>
#include <fstream.h>
#include <iomanip.h>
#include <stdlib.h>

#include "TCanvas.h"
#include "TFile.h"
#include "TStyle.h"
#include "TH1.h"
#include "TH2.h"
#include "TLatex.h"
#include "TMath.h"
#include "TPad.h"
#include "TPaveText.h"
#include "TPaveLabel.h"
#include "TROOT.h"
#endif

void show_rich(TString infile)
{

	gStyle->SetPadBorderSize(0);
	gStyle->SetPadBorderMode(0);
	gStyle->SetHistFillColor(0);
	gStyle->SetPadColor(0);
	gStyle->SetFrameFillColor(0);
	gStyle->SetFillColor(0);
	gStyle->SetOptStat(10);
	gStyle->SetStatStyle(10);
 	gStyle->SetPalette(1,0);

	TCanvas *c1;
	c1=new TCanvas("rich_corr","rich_corr");
	c1->SetBorderMode(0);
	c1->SetBorderSize(0);
//	c1->SetHistFillColor(0);
//	c1->SetPadColor(0);
	c1->SetFrameFillColor(0);
	c1->SetFillColor(0);

	TFile *f = new TFile(infile);
	if (!f) { cout<<"--E-- file not found "<< infile << endl; return;}
	if (f->IsZombie()) { cout << "--E-- Error opening file: " << infile<< endl; return;}

	c1->Clear();

	TPaveLabel *ll;
	ll=new TPaveLabel(0.0,1.0,1.0,0.96,gFile->GetName());
	ll->SetBorderSize(0);
	ll->Draw();

	c1->Divide(3,2);

	TVirtualPad *p;
	p=c1->GetPad(1);
	p->SetPad(0.0,0.48,0.32,0.95);
	p->SetBorderSize(0);
	p=c1->GetPad(2);
	p->SetPad(0.33,0.48,0.65,0.95);
	p->SetBorderSize(0);
	p=c1->GetPad(3);
	p->SetPad(0.66,0.48,1.0,0.95);
	p->SetBorderSize(0);
	p=c1->GetPad(4);
	p->SetPad(0.0,0.0,0.32,0.47);
	p->SetBorderSize(0);
	p=c1->GetPad(5);
	p->SetPad(0.33,0.0,0.65,0.47);
	p->SetBorderSize(0);
	p=c1->GetPad(6);
	p->SetPad(0.66,0.0,1.0,0.47);
	p->SetBorderSize(0);

	TH2F *dummy[6];
	TString a;
 Char_t name[20];

	c1->Update();
	for(Int_t i=0; i<6; i++){
		sprintf(name,"hshcx%d",i);
		c1->cd(i+1);
		dummy[i]=(TH2F*)gROOT->FindObject(name);
		if(dummy[i]) dummy[i]->Draw("colz");
	}
	c1->cd(0);
   a="";
   a+=gFile->GetName();
   a+=".rich_x.ps";
	c1->Print(a);

	c1->Update();
	for(Int_t i=0; i<6; i++){
		sprintf(name,"hshcy%d",i);
		c1->cd(i+1);
		dummy[i]=(TH2F*)gROOT->FindObject(name);
		if(dummy[i]) dummy[i]->Draw("colz");
	}
	c1->cd(0);
   a="";
   a+=gFile->GetName();
   a+=".rich_y.ps";
	c1->Print(a);

	f->Close();
	delete f;

}

#ifndef __CINT
Int_t main(Int_t argc, Char_t** argv)
{
        TROOT app("show_rich_corr","compiled show_rich correlation macro");
        gROOT->SetBatch(kTRUE);
	switch( argc)
        {
		case 2:
        		show_rich(argv[1]);
        		exit(0);
		default:
                	cout << argv[0] << " <path/filename> " << endl;
                	exit(1);
        }
}
#endif
