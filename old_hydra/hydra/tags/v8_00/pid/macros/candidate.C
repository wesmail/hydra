// @(#)$Id: candidate.C,v 1.2 2003-03-05 14:36:57 jaskula Exp $
// The macro to make PID up to HPidCandidate level for the real data
// Usage:
// .L candidate.C
// ana()

Bool_t ana(Int_t nEvents = 0,Int_t startEvt = 0)
{
//---- parameters -------------------------------------------------

TString inDir   = "/d/hades03/data/nov01/dst/gen4/";
TString inFile  = "./xx01342021209_dst.root";

TString outFile = "/s/jaskula/candidates.root";

//-----------------------------------------------------------------

  gROOT->Reset();

  if(gHades == NULL)
      new Hades;

  printf("Setting configuration...\n");

//---- PID reconstructors -----------------------------------------

HPidTrackFiller *pPidTrackFiller = new HPidTrackFiller();

HPidReconstructor *pPidRec = new HPidReconstructor("pdf,CL,merge,debug0");

    pPidRec->setDefaultParticleIds();
    pPidRec->addAlgorithm(new HPidAlgMomVsBeta());

//-----------------------------------------------------------------

  TStopwatch timer;
  Int_t evN=0;

//---------------  Set batch (needed for TCanvas's)----------------

  gROOT->SetBatch();

//--------- Do not write these categories to the Tree -------------

Cat_t notPersistentCat[] = {
       catMdcRaw, catShowerRaw, catStartRaw, catTofRaw, catTofinoRaw
       , catMdcCal1,catRichCal,catMdcCal2
       , catShowerCal
       , catMdcHit
       , catShowerHit, catShowerPID, catRichPID, catStartHit
       , catTofinoCal
       , catKickMdcTrk
       , catShowerHitTof, catMdcSeg, catRichRaw
       , catShowerTofPID, catStartCal, catTofCluster, catTofHit
       , catRichHitHdr, catShowerHitHdr
};

// ------------ Set input data file: ------------------------------

  HRootSource *source=new HRootSource;
  source->setDirectory(inDir.Data());
  source->addFile(inFile.Data());
  source->setRefId(source->getCurrentRunId(), 1);
  gHades->setDataSource(source);

// ----------Add detectors to the setup: NO NEED TO CHANGE -----------

// no detectors needed on this level

//---------------  PID tasks

  gHades->getTaskSet("real")->add(pPidTrackFiller);
  gHades->getTaskSet("real")->add(pPidRec);

//------------------------ Initialization ----------------------------

  HRuntimeDb* rtdb=gHades->getRuntimeDb();

  HParRootFileIo* input1=new HParRootFileIo;
  input1->open("pidparams.root");
  rtdb->setFirstInput(input1);


  gHades->makeCounter(1000);
  if(!gHades->init())
  {
     printf("Error\n");
     return -1;
  }

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

