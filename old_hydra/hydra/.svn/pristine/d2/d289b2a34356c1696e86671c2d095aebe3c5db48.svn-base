// $Id: readPairs.cc,v 1.5 2005-11-18 13:55:02 eberl Exp $
// Author: Thomas.Eberl@ph.tum.de, last modified : 2005-05-26 03:42:56
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
#include "pairsS.h"

using namespace std;

int main(int argc, char **argv)
{
    Int_t nevt = 10000000;

    cout<<"Number of files provided : "<<argc-1<<endl;
    TString ouFileName = TString(argv[1]);
    TFile* outFile = new TFile(ouFileName.Data(),"RECREATE");
    cout << " output file " << ouFileName.Data() << endl;

    TString eventGenerator = TString(argv[2]);

    TChain* tup = new TChain("PairsFilter");

    for (Int_t i=3;i<argc;i++)
    {
	if (argv[i])
	{ 
	    tup->Add(argv[i]);
	    cout<<argv[i]<<" added to TChain."<<endl;
	}
    }
    tup->Print();
    outFile->cd();

    pairsS c(tup,"pluto"); // access class for dilepton ntuple
    c.Loop(); // "event loop" of access class

    TObjArray* harr = c.getHistograms();
    cout<<harr->GetEntries()<<" control histograms created"<<endl;

    outFile->cd();
    cout << "Save " << harr->GetEntries() << " histograms" << endl;
    for (Int_t i=0; i<harr->GetEntries(); i++)
     {
        cout << i+1 << endl;
 	((TH1*)(*harr)[i] ) -> Write();
	cout<<"saving control histogram "
	    <<((TH1*)(*harr)[i] )->GetName()
	    <<" to file "<<outFile->GetName()<<endl;
     }


    return 0;
}
#endif
