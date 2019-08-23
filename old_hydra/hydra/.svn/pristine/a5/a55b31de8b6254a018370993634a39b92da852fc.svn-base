// $Id: readPairsE.cc,v 1.4 2005-10-28 15:01:29 eberl Exp $
// Author: Thomas.Eberl@ph.tum.de, last modified : 2005-10-28 16:59:30
//
/*************************************************************************
   Purpose: access pair ntuple created from HPairQA

**************************************************************************/
#ifndef __CINT__
#include <iostream>
#include <stdlib.h>
/////////////////////
#include <TNtuple.h>
#include <TFile.h>
#include <TH1.h>
#include <TH2.h>
#include <TH3.h>
#include <TObjArray.h>
/////////////////////
#include "pairsE.h"

using namespace std;

int main(int argc, char **argv)
{

    cout<<"Number of files provided : "<<argc-1<<endl;
    if (argc<=1) exit(0);
    TString s(argv[1]);
    s.Remove(s.Length()-5,s.Length());
    s.Append("_histos.root");
    TFile* outFile = new TFile(s.Data(),"RECREATE");

    TChain* tup = new TChain("PairsFilter");

    for (Int_t i=1;i<argc;i++) 
    {
	if (argv[i])
	{ 
	    tup->Add(argv[i]);
	    cout<<argv[i]<<" added to TChain."<<endl;
	}
    }
    tup->Print();
    outFile->cd();
    pairsE c(tup); // access class for dilepton ntuple
    c.Loop(0); // "event loop" of access class, "0" means all events in file

    TObjArray* harr = c.getHistograms();
    cout<<harr->GetEntries()<<" control histograms created"<<endl;
    //    harr->Print();
    outFile->cd();
     for (Int_t i=0; i<harr->GetEntries(); i++)
     {
	 //	 cout << i+1 <<" ";
 	((TH1*)(*harr)[i] ) -> Write();
//  	cout<<"saving control histogram "
//  	    <<((TH1*)(*harr)[i] )->GetName()
//  	    <<" to file "<<outFile->GetName()<<endl;
     }
     //cout << endl;


    return 0;
}
#endif
