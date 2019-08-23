// $Id: readPairsE.cc,v 1.2 2005-05-26 02:27:50 eberl Exp $
// Author: Thomas.Eberl@ph.tum.de, last modified : 2005-05-26 03:50:37
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

    TFile* outFile = new TFile("histsE.root","RECREATE");
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
    c.Loop(); // "event loop" of access class

    TObjArray* harr = c.getHistograms();
    cout<<harr->GetEntries()<<" control histograms created"<<endl;
    //    harr->Print();
    outFile->cd();
     for (Int_t i=0; i<harr->GetEntries(); i++)
     {
 	((TH1*)(*harr)[i] ) -> Write();
 	cout<<"saving control histogram "
 	    <<((TH1*)(*harr)[i] )->GetName()
 	    <<" to file "<<outFile->GetName()<<endl;
     }


    return 0;
}
#endif
