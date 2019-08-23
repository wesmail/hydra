//-----------------------------------------------------------------------------
// Example of the executable program which projects ntupleproduced by 
// projectNtuple program to histograms. It uses "projectPidHisto" class. 
// As the input it requries the *.root file with TNtuple called "Ntuple" 
// keeping all variables needed to create PidStandatdCuts parameters. 
//
// The program must be compiled and linked with ROOT libraries. 
//
// To run the program use:
// ./anaProjectPidHisto -i [input_file.root] -o [output_file.root] -e [events] -s [startevent]
// or
// ./anaProjectPidHisto -i [input_file.root] -o [output_file.root]
//
// All options are descibed by using help:
// ./anaProjectPidHisto -h
//
//*--Author: Jacek Otwinowski 06/02/2007
// ----------------------------------------------------------------------------

#include <iostream> 
#include <stdlib.h>
#include <getopt.h>
/////////////////////
#include <TNtuple.h>
#include <TFile.h>
#include <TH1.h>
#include <TH2.h>
#include <TH3.h>
#include <TObjArray.h>
#include <TError.h>
/////////////////////
#include "projectPidHisto.h"

using namespace std;

//-----------------------------------------------------------------------------
// The anaProjectPidHisto() function is called later in the main()
//-----------------------------------------------------------------------------
bool anaProjectPidHisto(TString inFileName,TString outFileName,Int_t nEvents = 0, Int_t nStart=0)
{
	TFile* outFile = 0;
    TChain* tup = new TChain("Ntuple");

	if (tup && inFileName.Length()>0)
	{ 
	    tup->Add(inFileName.Data());
	    cout<<inFileName.Data()<<" input file added to TChain."<<endl;
	} else {
	    ::Error("anaProjectPidHisto","No input file");
    	return kFALSE;
	}
    tup->Print();

	if(outFileName.Length()>0) {
		cout << " output file " << outFileName.Data() << endl;
		outFile = new TFile(outFileName.Data(),"RECREATE");
	} else {
	    ::Error("anaProjectPidHisto","No output file");
    	return kFALSE;
	}

    projectPidHisto c(tup,outFile); // access class for dilepton ntuple
    c.Loop(nEvents,nStart); // "event loop" of access class

	outFile->Write();

    return kTRUE;
}

//-----------------------------------------------------------------------------
// The main() function
//-----------------------------------------------------------------------------
int main(int argc, char **argv)
{
  Int_t option;
  Int_t events=0, startevent=0;
  Bool_t bError;
  TString inFile;
  TString outFile;

  // load arguments 
  opterr = 0;
  bError = kFALSE;

	while((option = getopt(argc, argv, "i:o:e:s:h:?")) != -1)
	{
		switch(option)
		{
			case 'i':
			inFile=TString(optarg);
			break;
			case 'o':
			outFile=TString(optarg);
			break;
			case 'e':
			events=atoi(optarg);
			break;
			case 's':
			startevent=atoi(optarg);
			break;
			case 'h':
			case '?':
			bError=kTRUE;
			break;
			default:
			Error("main load arguments", "Unknown option %c\n", optopt);
			bError=kTRUE;
			break;
		}
	}
	
	// print help
	if(bError == kTRUE)
	{
		printf("Use:\n\n%s -i file -o file [-e events] [-s startevent]\n\n"
		"\t-i file - input file (*.root) \n "
		"\t-o file - output file (*.root)\n"
		"\t-e events - number of events to process\n"
		"\t-s startevent - start event\n\n",argv[0]);
		return -1;
	}

	// run analysis
	
	if( anaProjectPidHisto(inFile,outFile,events,startevent) == kFALSE ) 
	{
		return -2;
	}

return 0;
}
