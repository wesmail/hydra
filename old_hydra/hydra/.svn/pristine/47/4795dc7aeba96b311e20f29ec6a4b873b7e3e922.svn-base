/////////////////////////////////////////////////////////////////////////
// MU (HMULeptons) - MU Emu 1-1correlation
// based on RICH (HMatchURich) and Meta (HMatchUTof, HMtchUShower) IPUs
// lepton trigger emulated
// now also momentum determination included
//
// load the library  > .L mu-muEmu_momcorr.cc+
// call the function > mu-muEmu_corr()
/////////////////////////////////////////////////////////////////////////

#ifndef __CINT__
#include <iostream.h>
#include <unistd.h>
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TCanvas.h"

#include "hmatchurich.h"
#include "hmatchushower.h"
#include "hmatchutof.h"
#include "hmuleptons.h"
#include "hmuEmuleptons.h"

#include "hlinearcategory.h"
#include "htree.h"
#include "hrecevent.h"
#include "heventheader.h"
#include "hevent.h"
#include "haddef.h"
#include "hgeantdef.h"
#include "hcategory.h"
#include "hdetector.h"
#include "hgeomvector.h"
#include "hades.h"
#endif

void process_filename(TString &inputFile, TString &outputFile, TString suffix, TString outputpath="")
{
  TString file="", inputpath="";

  if (!inputFile.EndsWith(".root") && !inputFile.EndsWith(".hld")){
    inputFile+=".root";
  }
  cout << "File input: " << inputFile << endl;

  // now, Do bad thing with string
  Char_t buffer[1000], *c;
  strncpy(buffer,(const Char_t *)inputFile,999);
  c=strrchr(buffer,'/');
  if(c){
    *c=0;
    file=c+1;
    inputpath=buffer;
  }else{
    file=inputFile;
    inputpath="";
  }

  strncpy(buffer,(const Char_t *)file,999);
  c=strrchr(buffer,'.');

  if(c){
    *c=0;
    file=buffer;
  }

  if(outputpath=="") outputpath=inputpath;
  if(!outputpath.EndsWith("/")) outputpath+="/";

  outputFile  = outputpath+file+suffix+".root";

  cout << "File output: " << outputFile << endl;
}


Int_t  numberNonFoundLeptons(TString inputFile, Int_t nEvents=0, Bool_t verbose=false,Bool_t debug=false)
{
//  gROOT->Reset();

  TString outFile="";
  process_filename(inputFile,outFile,"_NrNotFound");

//  TFile *hfile = new TFile((Text_t *)outFile.Data(),"RECREATE","Test",2);

  TFile *f = new TFile(inputFile);
 if(!f) {cout << "File not opened! " <<inputFile << endl; exit(1);}

  HTree *T = (HTree*)gDirectory->Get("T");
  if(!T) {cout << "No tree in file!" << endl; exit(1);}

  HEventHeader* fEventHead = (HEventHeader*)f->Get("EventHeader");
  TBranch* evt = T->GetBranch("EventHeader.");
  if(evt) evt->SetAddress(&fEventHead);

  f->cd("dirTrigger");

//   HMULeptons* ptlHMULeptons = new HMULeptons();
//   HLinearCategory* catlHMULeptons = (HLinearCategory*) gDirectory->Get("HMULeptons");
//   TBranch* brlHMULeptons = T->GetBranch("HMULeptons");
//   if(brlHMULeptons) brlHMULeptons->SetAddress(&catlHMULeptons);
//   TIterator *iterHMULeptons = catlHMULeptons->MakeIterator();

  HMUEMULeptons* ptlHMUEMULeptons = new HMUEMULeptons();
  HLinearCategory* catlHMUEMULeptons = (HLinearCategory*) gDirectory->Get("HMUEMULeptons");
  TBranch* brlHMUEMULeptons = T->GetBranch("HMUEMULeptons.");
  if(brlHMUEMULeptons) brlHMUEMULeptons->SetAddress(&catlHMUEMULeptons);
  TIterator *iterHMUEMULeptons = catlHMUEMULeptons->MakeIterator();

  f->cd();

  // ******* preparations for event loop ******
  Stat_t ientries;  // how many events
  ientries = T->GetEntries();  // how many events
  if (nEvents>0 && nEvents<ientries)  ientries = nEvents;
  cout << ientries << " Events to process" << endl;

  Int_t counterNotFoundLeptons = 0;
  Int_t counterEmuLeptons = 0;

  //---------- Eventloop ----------------
  for (Int_t i=0; i<ientries; i++) {
    evt->GetEntry(i);
    if (i % 10000 == 0)
      cout << "Event " << i << " seq num " <<(fEventHead)->getEventSeqNumber() << endl;
    if (debug ) {
	cout<<"Entry = "<<i<<"\tDownscaled flag  = "<<(fEventHead)->getDownscalingFlag();
    	cout<<"\tTrigger Decision = "<<(fEventHead)->getTriggerDecision()<<endl;
    }
    if ((fEventHead)->getDownscalingFlag() == 1){ // we belive 1 == downscaled
      if ( (fEventHead)->getTriggerDecision() == 0) { //not triggered
	brlHMUEMULeptons->GetEntry(i);
	iterHMUEMULeptons->Reset();
	counterEmuLeptons = 0;
	while ((ptlHMUEMULeptons = (HMUEMULeptons *)iterHMUEMULeptons->Next()) != 0) {  // get allemu leptons
		counterEmuLeptons++;
		cout << "Lepton not found in event nr: " << i << " Lepton nr: " << counterEmuLeptons << endl;
	}

	if ( counterEmuLeptons != 0)  {
		counterNotFoundLeptons++;
	}
      }
    }

    // 	brlHMULeptons->GetEntry(i);
    //       iterHMULeptons->Reset();
    //       iterHMUEMULeptons->Reset();

    //   catlHMULeptons->Clear();
    catlHMUEMULeptons->Clear();
  } // eventloop
  cout<<"######################" << endl;
  cout<<"counterNotFoundLeptons = "<<counterNotFoundLeptons<<endl;
  cout<<"if this is nor 0, we have a serious problem!" << endl;
  cout<<"######################" << endl;
  return counterNotFoundLeptons;
}

 
//   hfile->cd();

//   hnci->Write();
//   hnci_2->Write();
//   hnca->Write();
//   hnca_2->Write();
//   hmuhits->Write();
//   hshcx->Write();
//   hshcy->Write();
//   hshcp->Write();
//   diff->Write();

//   hfile->Close();

#ifndef __CINT
Int_t main(Int_t argc, Char_t** argv)
{
        TROOT app("numberNonFoundLeptons","compiled numberNonFoundLeptons macro");
        gROOT->SetBatch(kTRUE);

 Bool_t verbose=false, debug=false;
 Int_t cur_arg=1;
	while(cur_arg<argc){
		if(argv[cur_arg][0]!='-') break;
		for(Int_t i=1; 1; i++){
			if(argv[cur_arg][i]==0) break;
			switch(argv[cur_arg][i]){
				case 'v':
					verbose=true;
					cout << "verbosity is ON"<<endl;
					break;
				case 'd':
					debug=true;
					verbose=true;
					cout << "debug is ON"<<endl;
					cout << "verbosity is ON"<<endl;
					break;
				default:
					cout << "unknown argument \""<<argv[cur_arg][i]<<"\""<<endl;
		        	        exit(1);
			}
		}
		cur_arg++;
	}
	switch( argc-cur_arg)
        {
		case 1:
			numberNonFoundLeptons(argv[cur_arg],0,verbose,debug);
			exit(0);
		case 2:
			numberNonFoundLeptons(argv[cur_arg],atoi(argv[cur_arg+1]),verbose,debug);
			exit(0);
		default:
	                cout << argv[0] << " [-vd] pathfilename [number of events]" << endl;
	                cout <<  "-v  be verbose (no effect right now)" << endl;
	                cout <<  "-d  debug mode (include -v)" << endl;
        	        exit(1);
        }
}
#endif
