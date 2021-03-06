// @(#)$Id: pidFun.C,v 1.1 2002-11-21 16:11:39 jaskula Exp $
// The macro to make HPidParticle from HPidCandidate with user defined function
// funFiller() defined in funFiller.cc file
// Usage:
/*
.incl $HADDIR_PID
.incl $HADDIR_BASE/util
.incl $HADDIR_BASE/eventhandling
.incl $HADDIR_BASE/datastruct
.L funFiller.cc++
.L pidFun.C
ana()
*/

Bool_t ana(Int_t nEvents = 0,Int_t startEvt = 0)
{
//-----------------------------------------------------------------

TString inDir   = "/s/jaskula/";
TString inFile  = "./candidates.root";

TString outFile = "./particles.root";

//-----------------------------------------------------------------

  gROOT->Reset();
  TStopwatch timer;
  Int_t evN=0;

  if(gHades == NULL)
      new Hades;

  printf("Setting configuration...\n");

//---------------  Set HPidParticleFiller (connected later to taks list)

HPidParticleFiller *pPartFiller = new HPidParticleFiller("MASSEXP");

  pPartFiller->setCheckFun(funFiller);

  pPartFiller->print();

//---------------  Set batch (needed for TCanvas's) ------------------

  gROOT->SetBatch();

//------------ Do not write these categories to the Tree -------------

Cat_t notPersistentCat[] = {
       catMdcRaw, catShowerRaw, catStartRaw, catTofRaw, catTofinoRaw
       , catMdcCal1,catRichCal,catMdcCal2
       , catShowerCal
       , catMdcHit
       , catShowerHit, catShowerPID, catRichPID, catStartHit
       , catTofinoCal
       , catKickTrack, catKickMdcTrk,catPidTrackCand,catPidCandidate
};

// ------------ Set input data file: ------------------ --------------

  HRootSource *source=new HRootSource;
  source->setDirectory(inDir.Data());
  source->addFile(inFile.Data());
  gHades->setDataSource(source);

// ----------Add detectors to the setup: NO NEED TO CHANGE -----------

// no detectors needed on this level

//------------------------ Add PID Tasks -----------------------------

  gHades->getTaskSet("real")->add(pPartFiller);

//------------------------ Initialization ----------------------------

  printf("gHades->init()\n");

  gHades->makeCounter(1000);
  if(!gHades->init())
     printf("Error\n");

//----------------- Set not persistent categories --------------------

  HEvent *event = gHades->getCurrentEvent();
  for(Int_t i=0;i<sizeof(notPersistentCat)/sizeof(Cat_t);i++)
  {
  HCategory *hCat = event->getCategory(notPersistentCat[i]);

     if(hCat != NULL)
       hCat->setPersistency(kFALSE);
  }

//-------------------------- Set output ------------------------------

  gHades->setOutputFile(outFile.Data(),"RECREATE","Test",2);
  gHades->makeTree();

//--------------------------------------------------------------------

  printf("Processing events...\n");
  timer.Reset();
  timer.Start();
  if (nEvents<1) {
    evN=gHades->eventLoop();
  } else {
    evN=gHades->eventLoop(nEvents,startEvt);
  }

  gHades->getTaskSet("real")->printTimer();

  delete gHades;
  timer.Stop();

  printf("------------------------------------------------------\n");
  printf("Events processed: %i\n",evN);
  printf("Real time: %f\n",timer.RealTime());
  printf("Cpu time: %f\n",timer.CpuTime());
  if (evN) printf("Performance: %f s/ev\n",timer.CpuTime()/evN);

  return kTRUE;
}

