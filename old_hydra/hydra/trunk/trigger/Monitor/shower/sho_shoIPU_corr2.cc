
#ifndef __CINT__
#include <stdio.h>
#include <iostream.h>
#include <unistd.h>
#include <fstream.h>
#include <iomanip.h>
#include <stdlib.h>

#include "TROOT.h"
#include "TStopwatch.h"
#include "TString.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TCanvas.h"
#include "hlinearcategory.h"
#include "hmatrixcategory.h"
#include "htree.h"
#include "hrecevent.h"
#include "hevent.h"
#include "haddef.h"
#include "hgeantdef.h"
#include "hcategory.h"
#include "hdetector.h"
#include "haddef.h"

#include "hshowercalibraterIPU.h"
#include "hshowercheckIPU.h"
#include "hshowerhitfinderIPU.h"
#endif

#ifndef __CINT__

#include "TNtuple.h"
#include "TTree.h"
#include "TUnixSystem.h"

#include "hades.h"
#include "haddef.h"
#include "heventheader.h"
#include "hiterator.h"
#include "hmatrixcategory.h"
#include "hrecevent.h"
#include "htree.h"

#include "hrichdetector.h"
#include "hmdcdetector.h"
#include "htofdetector.h"
#include "htofrec.h"
#include "hpidtofrec.h"
#include "htofinodetector.h"
#include "hshowerdetector.h"
#include "hstartdetector.h"
#include "htboxdetector.h"
#include "htboxunpacker.h"
#include "htriggerdetector.h"
#include "htriggertaskset.h"

#include "hrichtaskset.h"
#include "hrichIPUtaskset.h"
#include "hrichchernovringfitter.h"

#include "hkicktaskset.h"
#include "hmdctaskset.h"
#include "hmdctrackdset.h"
#include "hshowertaskset.h"
#include "hshowertofinotaskset.h"
#include "hshowerhittof.h"

#include "hstarttaskset.h"
#include "htoftaskset.h"
#include "htofinotaskset.h"

#include "hrichunpackerraw99.h"
#include "hrichunpackercal99.h"
#include "hmdcunpacker.h"
#include "htofunpacker.h"
#include "htofinounpacker.h"
#include "hshowerunpacker.h"
#include "hstartunpacker.h"
#include "hlatchunpacker.h"
#include "hmatchuunpacker.h"

#include "hspectrometer.h"

#include "showerdef.h"
#include "hstartdef.h"
#include "tofinodef.h"
#include "tofdef.h"
#include "richdef.h"
#include "hmdcdef.h"
#include "hmdctrackddef.h"
#include "triggerinfodef.h"

#include "hldfilesource.h"

#include "hparoraio.h"
#include "hparrootfileio.h"
#include "hparasciifileio.h"
#include "hruntimedb.h"
#include "htofraw.h"
#include "htofhit.h"
#include "htofhitsim.h"
#include "htofinocal.h"
#include "htofinocalsim.h"
#include "hkicktrack.h"
#include "hkicktrackB.h"
#include "hrtmdctrk.h"
#include "hmetamatch.h"
#include "hmdctrackgdef.h"
#include "showertofinodef.h"
#include "kickdef.h"

#include "muEmulation.h"
#include "muEmulationExp.h"
#include "muEmulationSim.h"
#include "muDilepEmulation.h"
#include "muDilepEmulationExp.h"
#include "muDilepEmulationSim.h"
#include "hmuemueventfilter.h"

#endif

Int_t ana_shower(TString inputFile,Int_t nEvents=0, Int_t startEvt =0)
{
 // gROOT->Reset();
  Hades *myHades=NULL;
  TStopwatch timer;
  Int_t evN=0;

  myHades=new Hades;
  myHades->setTreeBufferSize(8000); //very important. Do not remove it !!!!!!!
  printf("Setting configuration...+++\n");

  //---------------  Set batch (needed for TCanvas's) ------------------
  gROOT->SetBatch();
  Bool_t ora_input=kTRUE;

   TString inFile="";
   TString inDir;
   TString outFile="";
   {
    TString file="", inputpath="", outputpath="";
    TString suffix="_dst_shoCORR";

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

    outFile  = outputpath+file+suffix+".root";

    cout << "File output: " << outFile << endl;

    inFile=file+".hld";
    inDir=inputpath;
  }
    cout << "File input: " << inDir <<"/"<< inFile << endl;

  //------------- Detector setup configuration -------------------------------
  HSpectrometer* spec = myHades->getSetup();

  HShowerDetector *shower=new HShowerDetector;
  HTriggerDetector *trig=new HTriggerDetector;
  spec->addDetector(shower);
  spec->addDetector(trig);

  Int_t showerMods[3] = {1,2,3};
  for (Int_t is=0; is<6; is++)
    {
      shower->setModules(is,showerMods);
    }

// --------------  RuntimeDb inputs -----------------------------------
  HRuntimeDb *rtdb=gHades->getRuntimeDb();
  HParOraIo *input =new HParOraIo();
  input->open();
  rtdb->setFirstInput(input);

// --------------------- set input data file  ---------------------------------
  HldFileSource *source=new HldFileSource;
  source->setDirectory((Text_t *)inDir.Data());
  source->addFile((Text_t *)inFile.Data());
  gHades->setDataSource(source);

  // ----------------  Unpackers ----------------------------------
  Int_t showerUnpackers[] = { 300,301,302,303,304,305,
                              306,307,308,309,310,311 };
  for(UInt_t i=0; i<(sizeof(showerUnpackers)/sizeof(Int_t)); i++)
    source->addUnpacker( new HShowerUnpacker(showerUnpackers[i]) );

  source->addUnpacker( new HMatchUUnpacker(1,kTRUE,0,"aug04"));

// ------------------- Task list -----------------------------------
  HTaskSet* showTasks = new HTaskSet("Shower", "List of Shower tasks");
  showTasks->add(new HShowerCalibraterIPU());
  showTasks->add(new HShowerHitFinderIPU(0x6,0x0,0x0));
  HShowerCheckIPU *pCh = new HShowerCheckIPU();
  showTasks->add(pCh);
  pCh->setVerbose(2);

  showTasks->print();

//----------------------- Task concatenation ---- The order matters!!!
   gHades->getTaskSet("real")->add(showTasks);
   gHades->getTaskSet("real")->isTimed(kTRUE);
   gHades->getTaskSet("real")->print();

//---------------- Initialization----------------------------
   myHades->makeCounter(10000);
   if(!myHades->init())
     {
       printf("\nError in Hades::init() \n");
       // rtdb->closeFirstInput();
       exit(2);
     }
   //HEvent *event = gHades->getCurrentEvent();
// ------------------ Build output tree -----------------------------

    cout<<"\n***************************************************\n";
    cout<<"***************************************************\n";
    cout<<"Output will be written to : "<< outFile.Data()<<endl;
    cout<<"***************************************************\n";
    cout<<"***************************************************\n\n";

    myHades->setOutputFile((Text_t *)outFile.Data(),"RECREATE","Test",2);
    myHades->makeTree();

  //------------------------------------------------------------------

  printf("Processing events...\n");
  timer.Reset();
  timer.Start();

  if (nEvents<1)
    {
      evN=myHades->eventLoop();
    }
  else
    {
      evN=myHades->eventLoop(nEvents,startEvt);
    }

  myHades->getTaskSet("real")->printTimer();

  delete myHades;
  timer.Stop();

  printf("------------------------------------------------------\n");
  printf("Events processed: %i\n",evN);
  printf("Real time: %f\n",timer.RealTime());
  printf("Cpu time: %f\n",timer.CpuTime());
  if (evN) printf("Performance: %f s/ev\n",timer.CpuTime()/evN);

  cout<<outFile.Data()<<endl;

  return 0;
}

#ifndef __CINT
Int_t main(Int_t argc, Char_t** argv)
{
        TROOT app("shower-showerIPU_corr","compiled shower showerIPU correlation macro");
        gROOT->SetBatch(kTRUE);

 Bool_t verbose=false;
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
				default:
					cout << "unknown argument \""<<argv[cur_arg][i]<<"\""<<endl;
		        	        exit(1);
			}
		}
		cur_arg++;
	}

	switch( argc-cur_arg)
       {
		case 2:
        		ana_shower(argv[cur_arg],0);
        		exit(0);
		case 3:
        		ana_shower(argv[cur_arg],atoi(argv[cur_arg+1]));
        		exit(0);
		default:
                 	cout << argv[0] << " [-v] pathfilename> [number of events]" << endl;
                	cout << " -v   be verbose (no effect until now)" << endl;
                	exit(1);
        }
}
#endif
