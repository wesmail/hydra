// $Id: subtables.cc,v 1.2 2008-05-09 16:16:33 halo Exp $
// Author: Thomas.Eberl@ph.tum.de, last modified : 2005-11-08 14:34:39
//
/*************************************************************************
   Purpose: subtract each table from a given reference table
            write out to ROOT file
            
   Input: pad table histograms
   All coordinates are given in meters and degrees
**************************************************************************/
#ifndef __CINT__
#include <iostream>
#include <istream>
#include <fstream>
#include <stdlib.h>
#include "TMath.h"
#include "TH2.h"
#include "TH3.h"
#include "TFile.h"
#include "TObjArray.h"
#include "TString.h"
#include "TGraph.h"
#include "TCanvas.h"
using namespace std;


Int_t main(Int_t argc, Char_t **argv)
{
    TFile fin("/home/teberl/hades/richpads/output/padtables.root");
    
    Char_t* cRef = argv[1];
    TString sTableName("padtable_z_");
    sTableName+=cRef;
    TH2D* hdefault = (TH2D*) fin.Get(sTableName.Data());

    TObjArray *t = new TObjArray(300);
    for (Int_t call=0; call<=300; call++)
    {
	Double_t tz = -0.1 + 0.001 * call;
	if (tz == 0.073) continue;
	Char_t* fname = new Char_t[20];
	sprintf(fname,"padtable_z_%2.3f",tz);
	TH2D* h = (TH2D*)fin.Get(fname);
	t->Add(h);
    }

    TFile fout("difftables.root","RECREATE");

    for (Int_t table=0; table<(t->GetLast()+1); table++)
    {
	TH2D *	htmp1 = (TH2D*)(*t)[table];
	TString st(htmp1->GetName());
	st+="_d";
	htmp1->SetName(st.Data());
	htmp1->SetTitle(st.Data());


	TH2D* hd = new TH2D(htmp1->GetName(),htmp1->GetTitle(),96,0,96,96,0,96);
	for (Int_t i = 1; i < 96; i++)
	{
	    for (Int_t j = 1; j < 96; j++)
	    {
 		if ( (hdefault->GetBinContent(hdefault->GetBin(i,j))
		      - htmp1->GetBinContent(htmp1->GetBin(i,j))    
		      ) < 1.e-9 &&
		     (hdefault->GetBinContent(hdefault->GetBin(i,j)) 
		      -  htmp1->GetBinContent(htmp1->GetBin(i,j))
		      )> -1.e-9	    
		     )
		    hd->SetBinContent(hd->GetBin(i,j),-10.);
		else
		    hd->SetBinContent(hd->GetBin(i,j),
				      htmp1->GetBinContent(htmp1->GetBin(i,j)) - 
				      hdefault->GetBinContent(hdefault->GetBin(i,j))
				      );
	    }
	}
	fout.cd();
	hd->Write();
// 	TCanvas *c = new TCanvas("ccc","ccc",10,10,500,500);
// 	c->cd();
// 	htmp1->DrawCopy("colz");
// 	c->Write();
//	delete c;
	delete hd;
    }

    fout.Close();
    return 0;
}
#endif
