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
#include "projectPidHisto.h"

using namespace std;

int anaProjectPidHisto(TString inFileName,TString outFileName,Int_t nEvents = 0)
{
    TChain* tup = new TChain("pTrackCandidate");
	if (tup && inFileName.Length()>0)
	{ 
	    tup->Add(inFileName.Data());
	    cout<<inFileName.Data()<<" input file added to TChain."<<endl;
	}
    tup->Print();

	cout << " output file " << outFileName.Data() << endl;
	TFile* outFile = new TFile(outFileName.Data(),"RECREATE");

    projectPidHisto c(tup,outFile); // access class for dilepton ntuple
    c.Loop(nEvents); // "event loop" of access class

	outFile->Write();

    return 1;
}

#ifndef __CINT__
int main(int argc, char **argv)
{
  TROOT AnalysisDST("HADES","Analysis of Events");

  switch(argc)
  {
    case 3:
      return anaProjectPidHisto(TString(argv[1]),TString(argv[2]));
      break;
    case 4:
      return anaProjectPidHisto(TString(argv[1]),TString(argv[2]),atoi(argv[3]));
      break;
    default:
    break;
   }
  return 0;
}
#endif



#endif
