// $Id: readPairs.cc,v 1.8 2006-08-16 15:42:18 halo Exp $
// Author: Thomas.Eberl@ph.tum.de, last modified : 2006-06-29 15:51:11
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
#include "pairs.h"

using namespace std;

void usage(const char *arg) {
	cerr << "usage: " << arg << " outfile [options] infile [infile ...]"
		<< endl;
	cerr << "  - outfile infile [...]" << endl;
	cerr << "  - outfile is the root file to contain the histograms"
		<< endl;
	cerr << "  - options is a comma separated list of options" << endl;
	cerr << "    (only for simulation up to now)" << endl;
#ifdef SIMULATION
	cerr << "    possible values are:" << endl;
	cerr << "    - pluto" << endl;
	cerr << "    - urqmd" << endl;
	cerr << "    - exp" << endl;
#endif // SIMULATION
	cout << "  - infile [infile ...] are the root files that contain"
		<< endl << "    the PairFilter output" << endl;
	exit(1);
}

int main(int argc, char **argv)
{
	const char *myname = argv[0];
	// need ouput, [options], input [...]
	argv++; argc--;

	// first argument: output file
	if (!argc) usage(myname);
	TString outFileName(argv[0]);
	TFile* outFile = new TFile(outFileName.Data(),"RECREATE");
	cout<<"Name of output file: "<<outFileName.Data()<<endl;
	argv++; argc--;

#ifdef SIMULATION
	// second argument: option string for pair class
	if (!argc) usage(myname);
	TString opt(argv[0]);
	cout<<"Event generator options: "<<opt.Data()<<endl;
	argv++; argc--;
#else // ! SIMULATION
	TString opt("");
#endif // SIMULATION

	// other arguments:  input files to chain
	if (!argc) usage(myname);
	TChain* tup = new TChain("PairsMixer");
	for (Int_t i=0;i<argc;i++)
	{
		if (argv[i])
		{
			tup->Add(argv[i]);
			cout<<argv[i]<<" added to TChain."<<endl;
		}
	}
	cout<<"Number of files provided : "<<argc<<endl;
#if 0
	tup->Print();
#endif // 0
	outFile->cd();

#ifdef SIMULATION
	pairsS c(tup, opt); // access class for dilepton ntuple
#else // ! SIMULATION
	pairsE c(tup, opt); // access class for dilepton ntuple
#endif // ! SIMULATION

#ifdef SIMULATION
	//c.SetEvents(141653794.); // change here to have normalisation
#else // ! SIMULATION
	c.SetEvents(72587201. * 9. * 0.92 * 1.16); // change here to have normalisation
#endif // ! SIMULATION
	c.Loop(0); // "event loop" of access class, "0" means all events in file

	TObjArray* harr = c.getHistograms();
	cout<<harr->GetEntries()<<" control histograms created"<<endl;
	outFile->cd();
	for (Int_t i=0; i<harr->GetEntries(); i++)
	{
		((TH1*)(*harr)[i] ) -> Write();
#ifndef NDEBUG
		cout<<"saving control histogram "
			<<((TH1*)(*harr)[i] )->GetName()
			<<" to file "<<outFile->GetName()<<endl;
#endif // ! NDEBUG
	}

	return 0;
}
#endif
