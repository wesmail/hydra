#include "TObject.h"
#include "TSystem.h"
#include "TThread.h"
#include "TCut.h"
#include "TGo4Hades.h"

#include "hparoraio.h"
#include "htaskset.h"

#include "hgo4onlineconfig.h"
#include "hhadespar.h"
#include "hprocessstat.h"

#include "htriggerhistbooker.h"
#include "htriggerhistfiller.h"
#include "htriggergo4mon.h"

#include "hstarthistbooker.h"
#include "hstarthistfiller.h"
#include "hstartgo4mon.h"

#include "hrichhistbooker.h"
#include "hrichhistfiller.h"
#include "hrichgo4mon.h"

#include "hmdchistbooker.h"
#include "hmdchistfiller.h"
#include "hmdcgo4mon.h"
#include "hmdccalibrationgo4mon.h"

#include "htofhistbooker.h"
#include "htofhistfiller.h"
#include "htofgo4mon.h"

#include "htofinohistbooker.h"
#include "htofinohistfiller.h"
#include "htofinogo4mon.h"

#include "hshowerhistbooker.h"
#include "hshowerhistfiller.h"
#include "hshowergo4mon.h"

#include "htriggerdetector.h"
#include "hmatchuunpacker.h"
#include "htboxdetector.h"
#include "hrecevent.h"
#include "hpartialevent.h"
#include "hmudata.h"

#include "hstartdetector.h"
#include "hstartunpacker.h"
#include "hstartcalibrater.h"
#include "hstarthitf.h"

#include "hrichdetector.h"
#include "hrichunpackercal99.h"
#include "hrichunpackerraw99.h"
#include "hrichcalibrater.h"
#include "hrichanalysis.h"
#include "hrichparticleidentifier.h"
#include "hrichpattern.h"
#include "hrichfancy.h"

#include "hmdcdetector.h"
#include "hmdcsetup.h"
#include "hmdctaskset.h"
#include "htask.h"
#include "hmdcunpacker.h"
#include "hmdccalibrater1.h"
#include "hmdcdef.h"

#include "htofdetector.h"
#include "htofunpacker.h"
#include "htofhitf.h"
#include "htofclusterf.h"

#include "htofinodetector.h"
#include "htofinounpacker.h"
#include "htofinocalibrater.h"
#include "hshowertofinocorrelator.h"

#include "hshowerdetector.h"
#include "hshowerunpacker.h"
#include "hshowerunpackparam.h"
#include "hshowercalibrater.h"
#include "hshowerhitfinder.h"
#include "hshowergeometry.h"
#include "hshowercalpar.h"
#include "hshowerhitfpar.h"
#include "hshowerunpacker.h"

#include "hmonhist.h"
#include "htrendhist.h"
#include "hhistarray.h"
#include "TList.h"

#include "hldsockremotesource.h"
#include "hldfilesource.h"

#include "hparrootfileio.h"
#include "hparasciifileio.h"
#include "hadascii.h"
#include "hmessagemgr.h"
#include "hevent.h"
#include "hcategory.h"

#include <stdlib.h>
#include <unistd.h> 

ClassImp(TGo4Hades)

Bool_t TGo4Hades::kINIT=kFALSE;
Int_t  TGo4Hades::fCount =0;
Int_t  TGo4Hades::nLoop  =100;

HGo4OnlineParam*  TGo4Hades::onlineparam=0;
HGo4OnlineConfig* TGo4Hades::onlineconfig=0;

HHadesPar* TGo4Hades::hadespar=0;

HTriggerHistBooker* TGo4Hades::Triggerhistbooker=0;
TList* TGo4Hades::TriggeractiveHists=0;

HStartHistBooker* TGo4Hades::Starthistbooker=0;
TList* TGo4Hades::StartactiveHists=0;

HRichHistBooker* TGo4Hades::Richhistbooker=0;
TList* TGo4Hades::RichactiveHists=0;
HRichPattern* TGo4Hades::richpattern=0;
HRichFancy* TGo4Hades::richfancy=0;

HMdcHistBooker* TGo4Hades::Mdchistbooker=0;
TList* TGo4Hades::MdcactiveHists=0;

HTofHistBooker* TGo4Hades::Tofhistbooker=0;
TList* TGo4Hades::TofactiveHists=0;

HTofinoHistBooker* TGo4Hades::Tofinohistbooker=0;
TList* TGo4Hades::TofinoactiveHists=0;

HShowerHistBooker* TGo4Hades::Showerhistbooker=0;
TList* TGo4Hades::ShoweractiveHists=0;

TGo4Hades::TGo4Hades(){
  // put default values
  fxTreeSize=500000; // 500kbytes
  fxRefreshNb=100; // refresh after 100 evt
  fxReset=0; 
  fxDump=kFALSE;
  timer=new TStopwatch();

  fbInit =kFALSE;
  //########################### GLOBAL ################################################
  hadespar=new HHadesPar("HadesParameter","HadesParameter");
  onlineconfig= new HGo4OnlineConfig("Go4OnlineConfig","Go4OnlineConfig","Go4OnlineConfigProduction");

  hadesconfig=new TGo4HadesConfigParameter("Hadesconfig","Hadesconfig");
  onlineparam=hadesconfig->GetPar();
  AddParameter(hadesconfig);

  reset=new TGo4ResetParameter("HistReset","HistReset");
  reset->SetInit();
  AddParameter(reset);

  process=new HProcessStat("process","process");

  //########################### TRIGGER ###############################################
  Triggerhistbooker  =new HTriggerHistBooker("TriggerHistBooker","TriggerHistBooker","TriggerHistBookerProduction");
  TriggeractiveHists =Triggerhistbooker->getListOfActiveHists();
  triggerParameter=new TGo4TriggerParameter("TriggerParam","TriggerParam");
  triggerParameter->SetInit();
  AddParameter(triggerParameter);

  //########################### START #################################################
  Starthistbooker  =new HStartHistBooker("StartHistBooker","StartHistBooker","StartHistBookerProduction");
  StartactiveHists =Starthistbooker->getListOfActiveHists();

  //########################### RICH ##################################################
  Richhistbooker   =new HRichHistBooker("RichHistBooker","RichHistBooker","RichHistBookerProduction");
  RichactiveHists  =Richhistbooker->getListOfActiveHists();

  //########################### MDC ###################################################
  Mdchistbooker    =new HMdcHistBooker("MdcHistBooker","MdcHistBooker","MdcHistBookerProduction");
  MdcactiveHists   =Mdchistbooker->getListOfActiveHists();
  mdcActive        =new TGo4MdcActiveParameter("MdcActiveHists","MdcActiveHists");
  AddParameter(mdcActive);

  //########################### TOF ###################################################
  Tofhistbooker    =new HTofHistBooker("TofHistBooker","TofHistBooker","TofHistBookerProduction");
  TofactiveHists   =Tofhistbooker->getListOfActiveHists();

  //########################### TOFINO ################################################
  Tofinohistbooker    =new HTofinoHistBooker("TofinoHistBooker","TofinoHistBooker","TofinoHistBookerProduction");
  TofinoactiveHists   =Tofinohistbooker->getListOfActiveHists();

  //########################### SHOWER ################################################
  Showerhistbooker =new HShowerHistBooker("ShowerHistBooker","ShowerHistBooker","ShowerHistBookerProduction");
  ShoweractiveHists=Showerhistbooker->getListOfActiveHists();
}
Bool_t TGo4Hades::InitEventClasses(){

  TGo4Analysis::InitEventClasses();
  SetDynListInterval(100);         

  //read some parameters
  if (!readAscii((Char_t*)ConfigInput.Data())) return kFALSE;
  
  myHades = new Hades();
  myHades->setQuietMode(2);
  if( kEvtRemote ==0 ){
    source = new HldFileSource;
    source->setDirectory((Text_t*) inDir->Data());
    source->addFile((Text_t*) inFile->Data(),refrunID);
    myHades->setDataSource(source); 
  } else {
    sourceR = new HldSockRemoteSource((Text_t*)eventHost->Data());
    sourceR->setRefId(refrunID);
    myHades->setDataSource(sourceR);
  }


  Int_t startUnpackers[1]  = { 412 };
  Int_t richUnpackers[12]   = { 100,101,102,103,104,105,
                              106,107,108,109,110,111 };

  Int_t mdcUnpackers[24]    = { 201,202,203,204,205,206,
                                207,208,209,210,211,212,
				213,214,215,216,217,218,
                                219,220,221,222,223,224};
  HMdcUnpacker* pUnpackers[24];

  Int_t tofUnpackers[3]    = { 411,412,413 };
  Int_t tofinoUnpackers[1] = { 412 };
  Int_t showerUnpackers[12] = { 300,301,302,303,304,305,
                                306,307,308,309,310,311 };

  spec = myHades->getSetup();
  rtdb = myHades->getRuntimeDb();

  InitDetectorSetup();
  initParams();

  onlineconfig->initStandAlone();
  if(!hadesconfig->HasChanged())
  {
      onlineparam->copySet(onlineconfig->getGo4OnlineParam());

      HMdcSetup* myset=hadesconfig->GetMdcSet();
      Int_t Layers[6][4]={{6,6,6,6},{6,6,6,6},{6,6,6,6},{6,6,6,6},{6,6,6,6},{6,6,6,6}};
      Int_t Level[4]={10,50,10,30};
      myset->getMdcCalibrater1Set() ->setMdcCalibrater1Set(1,1);
      myset->getMdcTrackFinderSet() ->setMdcTrackFinderSet(kFALSE,1,&Layers[0][0],&Level[0]);
      myset->getMdcClusterToHitSet()->setMdcClusterToHitSet(1,3);
      myset->getMdc12FitSet()       ->setMdc12FitSet(1,1,1,kFALSE,kFALSE);
      myset->getMdcCommonSet()      ->setMdcCommonSet(0,3,1);
      myset->getMdcDigitizerSet()->setTdcModeDigi      (2);
      myset->getMdcDigitizerSet()->setNtupleDigi       (kFALSE);
      myset->getMdcDigitizerSet()->setUseTofDigi       (kTRUE);
      myset->getMdcDigitizerSet()->setUseErrorDigi     (kTRUE);
      myset->getMdcDigitizerSet()->setUseWireOffsetDigi(kTRUE);
      myset->getMdcDigitizerSet()->setEmbeddingModeDigi(1);
      myset->getMdcDigitizerSet()->setNoiseModeDigi    (1);
      myset->getMdcDigitizerSet()->setNoiseOffDigi     (0);
      myset->getMdcDigitizerSet()->setNoiseLevelDigi   (0,0,0,0);
      myset->getMdcDigitizerSet()->setNoiseRangeDigi   (0,0,0,0,0,0,0,0);
      myset->getMdcDigitizerSet()->setOffsetsOffDigi   (0);
      myset->getMdcDigitizerSet()->setOffsetsDigi      (0,0,0,0);
      myset->getMdcDigitizerSet()->setCellEffOffDigi   (1);
      myset->getMdcDigitizerSet()->setCellEffDigi      (88,88,88,88);

      hadesconfig->GetMdcSet()->getMdcCommonSet()->setAnalysisLevel(onlineparam->getMdcLevel());
  }
  hadesconfig->Print();


  if( kEvtRemote ==1 ) source=(HldFileSource*)sourceR;

  if(onlineparam->getTrigger()==1)
  {
       HMatchUUnpacker *unp = new HMatchUUnpacker( 1, kTRUE, 0 );
       unp    -> setRichDebugMode( kFALSE );
       unp    -> setDebugMode( kFALSE );
       unp    -> setShowerDebugMode( kFALSE );
       source -> addUnpacker( unp );
  }
  if(onlineparam->getStart()==1)
  {
      for(UInt_t i=0; i<sizeof(startUnpackers)/sizeof(Int_t); i++){
	  source->addUnpacker( new HStartUnpacker(startUnpackers[i]));}
  }
  if(onlineparam->getRich()==1)
  {
      for(UInt_t i=0; i<sizeof(richUnpackers)/sizeof(Int_t); i++){
	  if(onlineparam->getRichRaw()==1)source->addUnpacker( new HRichUnpackerRaw99(richUnpackers[i]) );
      }
  }
  if(onlineparam->getMdc()==1)
  {
      for(UInt_t i=0; i<sizeof(mdcUnpackers)/sizeof(Int_t); i++){
	  pUnpackers[i]=new HMdcUnpacker(mdcUnpackers[i],kTRUE,kFALSE);
	  pUnpackers[i]->setDecodeVersion(1);
          pUnpackers[i]->setQuietMode();
	  source->addUnpacker(pUnpackers[i]);}
  }
  if(onlineparam->getTof()==1)
  {
      for(UInt_t i=0; i<sizeof(tofUnpackers)/sizeof(Int_t); i++){
	  source->addUnpacker( new HTofUnpacker(tofUnpackers[i]) );}
  }
  if(onlineparam->getTofino()==1)
  {
      for(UInt_t i=0; i<sizeof(tofinoUnpackers)/sizeof(Int_t); i++){
	  source->addUnpacker( new HTofinoUnpacker(tofinoUnpackers[i]) );}
  }
  if(onlineparam->getShower()==1)
  {
      for(UInt_t i=0; i<sizeof(showerUnpackers)/sizeof(Int_t); i++){
	  source->addUnpacker( new HShowerUnpacker(showerUnpackers[i]) );}
  }


  HTaskSet* triggerTasks= new HTaskSet("Trigger","List of Trigger tasks");
  HTaskSet* startTasks  = new HTaskSet("Start","List of Start tasks");
  HTaskSet* richTasks   = new HTaskSet("Rich","List of Rich tasks");
  HTaskSet* mdcTasks    = new HTaskSet("Mdc", "List of mdc tasks");
  HTaskSet* tofTasks    = new HTaskSet("Tof","List of Tof tasks");
  HTaskSet* tofinoTasks = new HTaskSet("Tofino","List of Tofino tasks");
  HTaskSet* showerTasks = new HTaskSet("Shower", "List of Shower tasks");

  HTaskSet* mdcCalTasks = new HTaskSet("Mdc", "List of mdc cal tasks");


  if(onlineparam->getTrigger()==1)
  {
      //--------------------Trigger-------------------------------------
      triggerTasks->add(new HTriggerGo4Mon("trigger.mon","trigger.mon"));
      Triggerhistbooker->initStandAlone();
  }
 
  if(onlineparam->getStart()==1)
  {
      //--------------------START-------------------------------------
      if(onlineparam->getStartCal()==1)startTasks->add( new HStartCalibrater("start.cal","Start cal") );
      startTasks->add( new HStartCalibrater("start.cal","Start cal") );
      startTasks->add( new HStartHitF("start.hitf","Start hitf") );

  if(onlineparam->getStartHit()==1)startTasks->add( new HStartHitF("start.hitf","Start hitf") );
      startTasks->add(new HStartGo4Mon("start.mon","start.mon"));
      Starthistbooker->initStandAlone();
  }
  if(onlineparam->getRich()==1)
  {
      //--------------------Rich---------------------------------------
      if(onlineparam->getRichCal()==1)richTasks->add(new HRichCalibrater("rich.cal","rich.cal"));
      if(onlineparam->getRichHit()==1)richTasks->add(new HRichAnalysis("rich.ana","Ring finder",kFALSE));
      if(onlineparam->getRichHit()==1)richTasks->add(new HRichParticleIdentifier("rich.id","Rich PID"));
      richTasks->add(new HRichGo4Mon("rich.mon","rich.mon"));
      Richhistbooker->initStandAlone();
  }
  if(onlineparam->getMdc()==1)
  {
      //--------------------MDC---------------------------------------

      HMdcTaskSet *mymdctasks=new HMdcTaskSet();
      mymdctasks->getMdcSetup()->copySet(hadesconfig->GetMdcSet());
      mdcTasks    = (HTaskSet*)(mymdctasks->make("",""));//read cal parameters from file

      mdcTasks->add(new HMdcGo4Mon("mdc.mon","mdc.mon"));
      Mdchistbooker->initStandAlone();
      mdcActive->SetInit();
      mdcCalTasks->add(new HMdcCalibrationGo4Mon("mdccal.mon","mdccal.mon"));
  }
  if(onlineparam->getTof()==1)
  {
      //--------------------TOF----------------------------------------
      if(onlineparam->getTofHit()==1)
      {
	  tofTasks->add( new HTofHitF("tof.hitf","tof.hitf") );
	  tofTasks->add( new HTofClusterF("tof.clusf","tof.clusf") );
      }
      tofTasks->add(new HTofGo4Mon("tof.mon","tof.mon"));
      Tofhistbooker->initStandAlone();
  }
  if(onlineparam->getTofino()==1)
  {
      //--------------------Tofino------------------------------------
      if(onlineparam->getTofinoCal()==1)tofinoTasks->add( new HTofinoCalibrater("tofino.cal","tofino") );
      if(onlineparam->getShowerTofino()==1)tofinoTasks->add( new HShowerTofinoCorrelator("showtofino.cor","tofinoshower") );
      tofinoTasks->add(new HTofinoGo4Mon("tofino.mon","tofino.mon"));
      Tofinohistbooker->initStandAlone();
  }

  if(onlineparam->getShower()==1)
  {
      //--------------------Shower------------------------------------
      if(onlineparam->getShowerCal()==1)showerTasks->add(new HShowerCalibrater("sho.cal","sho.cal"));
      if(onlineparam->getShowerHit()==1)showerTasks->add(new HShowerHitFinder("sho.hitf","sho.hitf"));
      showerTasks->add(new HShowerGo4Mon("shower.mon","shower.mon"));
      Showerhistbooker->initStandAlone();
  }

  HTaskSet *masterTaskSet = gHades->getTaskSet("real");
  if(onlineparam->getTrigger()==1)masterTaskSet->add(triggerTasks);
  if(onlineparam->getStart()==1)  masterTaskSet->add(startTasks);
  if(onlineparam->getRich()==1)   masterTaskSet->add(richTasks);
  if(onlineparam->getMdc()==1)    masterTaskSet->add(mdcTasks);
  if(onlineparam->getTof()==1)    masterTaskSet->add(tofTasks);
  if(onlineparam->getShower()==1) masterTaskSet->add(showerTasks);
  if(onlineparam->getTofino()==1) masterTaskSet->add(tofinoTasks);

  HTaskSet *calibrationTaskSet = gHades->getTaskSet("calibration");
  if(onlineparam->getMdc()==1)   calibrationTaskSet->add(mdcCalTasks);

  if(!myHades->init()) {
    printf("\nError in Hades::init() \n"); 
    exit(2);
  } 

  //no output now
  //	myHades->setOutputFile((Text_t*) outDir->Data(),"RECREATE","Test",2);

  //myHades->makeTree();
  //AddTree( (TTree*) myHades->getTree() );
  //myHades->getTree()->SetDirectory(gROOT);
  //gROOT->cd();
  fbInitIsDone=kTRUE;
  return kTRUE;  
}
Int_t TGo4Hades::UserPreLoop(){
    return 0;
}	

Int_t TGo4Hades::UserPostLoop(){
  // delete the whole stuff every new start

    if (myHades){
    //RemoveTree( (TTree*) myHades->getTree() );
    delete myHades; myHades=0;
  }
  CloseAnalysis();
  return 0;
}

TGo4Hades::~TGo4Hades(){
  if (myHades) {delete myHades; myHades=0;} 
}	

Int_t TGo4Hades::InitDetectorSetup(){
  
  // Detectors setup configuration 
    Int_t richMods[1]   = {1};

    Int_t mdcMods[6][4] = {{1,1,1,1},
                          {1,1,0,0},
                          {1,1,1,0},
                          {1,1,1,1},
                          {1,1,0,0},
                          {1,1,1,0} };      

    Int_t tofMods[22] = {1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

    Int_t showerMods[3]   = {1,2,3};
    Int_t tofinoMods[6][1]= { {1},{1},{1},{1},{1},{1} };
    Int_t startMods[3]    = {1,1,1};
    Int_t trigMods[1]     = {1};

    HSpectrometer* spec = gHades->getSetup();

    spec->addDetector(new HTriggerDetector);
    spec->addDetector(new HStartDetector);
    spec->addDetector(new HRichDetector);
    spec->addDetector(new HMdcDetector);
    spec->addDetector(new HTofDetector);
    spec->addDetector(new HTofinoDetector);
    spec->addDetector(new HShowerDetector);
    spec->addDetector(new HTBoxDetector);


    spec->getDetector("Start")->setModules(-1,startMods);
    spec->getDetector("TBox") ->setModules( 0, trigMods);

    for (Int_t is=0; is<6; is++) {
	spec->getDetector("Rich")->setModules(is,richMods);
	spec->getDetector("Mdc")->setModules(is,mdcMods[is]);
	spec->getDetector("Tof")->setModules(is,tofMods);
	spec->getDetector("Tofino")->setModules(is,tofinoMods[is]);
	spec->getDetector("Shower")->setModules(is,showerMods);
    }

  return 0;
}

Int_t TGo4Hades::initParams(){

    FirstInput ->ReplaceAll(" ","");
    SecondInput->ReplaceAll(" ","");
    FirstInput ->ToLower();
    SecondInput->ToLower();

    if(FirstInput->CompareTo("ascii")==0)
    {
	HParAsciiFileIo* input1= new HParAsciiFileIo();
	input1->open((Text_t*) ParFile->Data());
	rtdb->setFirstInput(input1);
    }
    else if(FirstInput->CompareTo("root")==0)
    {
	HParRootFileIo *input1=new HParRootFileIo;
	input1->open((Text_t*) ParFile->Data(),"READ");
	rtdb->setFirstInput(input1);
    }
    else if(FirstInput->CompareTo("ora")==0)
    {
	HParOraIo *input1 =new HParOraIo();
	input1->open();
	rtdb->setFirstInput(input1);
    }
    else {Warning("TGo4Hades::initParams()","FirstInput not set!");}

    if(SecondInput->CompareTo("ascii")==0)
    {
	HParAsciiFileIo* input2= new HParAsciiFileIo();
	input2->open((Text_t*) ParFile2->Data());
	rtdb->setSecondInput(input2);
    }
    else if(SecondInput->CompareTo("root")==0)
    {
	HParRootFileIo *input2=new HParRootFileIo;
	input2->open((Text_t*) ParFile2->Data(),"READ");
	rtdb->setSecondInput(input2);
    }
    else if(SecondInput->CompareTo("ora")==0)
    {
	HParOraIo *input2 =new HParOraIo();
	input2->open();
	rtdb->setSecondInput(input2);
    }
    else {Warning("TGo4Hades::initParams()","SecondInput not set!");}

    kINIT=kTRUE;
    return 0;

}
Int_t TGo4Hades::UserEventFunc(){

    if (! gHades ) InitEventClasses();

    nLoop=onlineparam->getNLoop();
    timer->Start();
    Int_t stat= gHades->eventLoop(nLoop);
    timer->Stop();
    Double_t realtime=timer->RealTime();
    timer->Reset();

    if(nLoop!=0)
    {
	// not finished.....
	//process->getCurrentPid();
	//process->update();
        //process->print();
	hadespar->setEvtRate((((Double_t)nLoop)/realtime));
	SendObjectToGUI((TNamed*)hadespar);
	if((((Float_t)nLoop)/realtime) >onlineparam->getMaxRate())
	{
            cout<<"TO FAST!!!! Go4 coffee for "<<onlineparam->getSleep()<<" ms ! "<<nLoop/realtime<<" Events/sec."<<endl;
	    gSystem->Sleep(onlineparam->getSleep());
	}

    }
    if (stat <= 0 ){
	CloseAnalysis();
    }

    // control the Tree buffering size
    //if ( (gHades->getTree()->GetTotBytes() > fxTreeSize ) && (fxDump == kFALSE) ) {
    //    gHades->getTree()->Reset();
    //}

    //---------------------------SENDING TO GUI-----------------------------

    //---------------------------TRIGGER--------------------------------------
    if(onlineparam->getTrigger()==1)
    {
	if(((fCount*nLoop)%onlineparam->getTriggerRefresh()) == 0 )
	{
	    Triggerhistbooker->getTriggerFiller()->fillTrend();
	    Triggerhistbooker->writeHists("active");
	    TIterator* myiter=TriggeractiveHists->MakeIterator();
	    HMonHistAddon* h=0;
	    TString buffer;
	    while ((h=(HMonHistAddon*)myiter->Next())!= 0)
	    {
		if(h->getSend()==1)
		{
		    SendObjectToGUI((TNamed*) h);
		}
	    }
	    delete myiter;
	    Triggerhistbooker->resetActiveList(1,fCount*nLoop);
	}
    }

    //---------------------------START--------------------------------------
    if(onlineparam->getStart()==1)
    {
	if(((fCount*nLoop)%onlineparam->getStartRefresh()) == 0 )
	{
	    Starthistbooker->getStartFiller()->fillTrend();
	    TIterator* myiter=StartactiveHists->MakeIterator();
	    HMonHistAddon* h=0;
	    TString buffer;
	    while ((h=(HMonHistAddon*)myiter->Next())!= 0)
	    {
		if(h->getSend()==1)SendObjectToGUI((TNamed*) h);
	    }
	    delete myiter;
	    Starthistbooker->resetActiveList(1,fCount*nLoop);
	}
    }

    //---------------------------RICH---------------------------------------
    if(onlineparam->getRich()==1)
    {
	if(((fCount*nLoop)%onlineparam->getRichRefresh()) == 0 )
	{
	    TIterator* myiter=RichactiveHists->MakeIterator();
	    HMonHistAddon* h=0;
	    TString buffer;
	    while ((h=(HMonHistAddon*)myiter->Next())!= 0)
	    {
		if(h->getSend()==1)SendObjectToGUI((TNamed*) h);
	    }
	    delete myiter;
	    Richhistbooker->resetActiveList(1,fCount*nLoop);
	    // richfancy=(Richhistbooker->getRichFiller())->getRichFancy();
	    // 	    if(richfancy){
	    // 	      if(richfancy->getKnown()==kFALSE){
	    // 		//We check whether the GUI allready knows this object - it has to be sent only once!
	    // 		cout << "Sending object richfancy with name " << richfancy->GetName() << endl;
	    // 		//richfancy->Dump();
	    // 		SendObjectToGUI((TNamed*) richfancy);
	    // 		richfancy->setKnown(kTRUE);
	    // 	      }
	    // 	      richpattern=(Richhistbooker->getRichFiller())->getRichPattern();
	    // 	      cout << "Sending object richpattern with name:" <<  richpattern->GetName()<< endl;
	    // 	      //richpattern->Dump();
	    // 	      SendObjectToGUI((TNamed*) richpattern);
	    // 	      cout << "done" << endl;
	    //	}
	}
    }
    //--------------------------MDC-------------------------------------------
    if(onlineparam->getMdc()==1)
    {
	if(((fCount*nLoop)%onlineparam->getMdcRefresh()) == 0 )
	{
	    Mdchistbooker->getMdcFiller()->fillTrend();
	    Mdchistbooker->getMdcFiller()->fillCutStat();
	    Mdchistbooker->getMdcFiller()->fillMdcConfig(fCount*nLoop);

	    SendObjectToGUI((TNamed*)Mdchistbooker->getMdcFiller()->getMdcLookup());
 
	    TIterator* myiter=MdcactiveHists->MakeIterator();
	    HMonHistAddon* h=0;
	    TString buffer;
	    while ((h=(HMonHistAddon*)myiter->Next())!= 0)
	    {
		if(h->getSend()==1)SendObjectToGUI((TNamed*) h);
	    }
	    SendObjectToGUI((TNamed*)Mdchistbooker->getMdcFiller()->getMdcConfig());
	    Mdchistbooker->resetActiveList(2,fCount*nLoop);
	    delete myiter;
	}
    }
    //--------------------------TOF-------------------------------------------
    if(onlineparam->getTof()==1)
    {
	if(((fCount*nLoop)%onlineparam->getTofRefresh()) == 0 )
	{
	    TIterator* myiter=TofactiveHists->MakeIterator();
	    HMonHistAddon* h=0;
	    TString buffer;
	    while ((h=(HMonHistAddon*)myiter->Next())!= 0)
	    {
		if(h->getSend()==1)SendObjectToGUI((TNamed*) h);
	    }
	    Tofhistbooker->resetActiveList(1,fCount*nLoop);
	    delete myiter;
	}
    }
    //--------------------------TOFINO-------------------------------------------
    if(onlineparam->getTofino()==1)
    {
	if(((fCount*nLoop)%onlineparam->getTofinoRefresh()) == 0 )
	{
	    TIterator* myiter=TofinoactiveHists->MakeIterator();
	    HMonHistAddon* h=0;
	    TString buffer;
	    while ((h=(HMonHistAddon*)myiter->Next())!= 0)
	    {
		if(h->getSend()==1)SendObjectToGUI((TNamed*) h);
	    }
	    Tofinohistbooker->resetActiveList(1,fCount*nLoop);
	    delete myiter;
	}
    }
    //--------------------------SHOWER-------------------------------------------
    if(onlineparam->getShower()==1)
    {
	if(((fCount*nLoop)%onlineparam->getShowerRefresh()) == 0 )
	{
	    Showerhistbooker->getShowerFiller()->fillTrend((fCount*nLoop)%1000);
	    Showerhistbooker->getShowerFiller()->fillShowerConfig(fCount*nLoop);

	    TIterator* myiter=ShoweractiveHists->MakeIterator();
	    HMonHistAddon* h=0;
	    TString buffer;
	    while ((h=(HMonHistAddon*)myiter->Next())!= 0)
	    {
		if(h->getSend()==1)SendObjectToGUI((TNamed*) h);
	    }
	    Showerhistbooker->resetActiveList(1,fCount*nLoop);
	    delete myiter;

	    SendObjectToGUI((TNamed*)Showerhistbooker->getShowerFiller()->getShowerConfig());
	    Showerhistbooker->getShowerFiller()->resetHits();
	}
    }
    //-----------------------------------------------------------------------

    ++fCount;
    return stat;
}
Bool_t TGo4Hades::readAscii(Char_t *fileName){

    Bool_t status = kTRUE;
    try {
	cout<<"############### HADES ONLINE CONFIG ############################"<<endl;
        cout<<"Reading from "<<ConfigInput.Data()<<endl;

	HAsciiKey paramData;
	paramData.OpenFile(fileName,"read");
	paramData.SetActiveSection("Parameters for analysis");

	FirstInput  = new TString(paramData.ReadString("FirstInput"));
	cout<<"FirstInput      = "<<(Text_t*)FirstInput->Data()<<endl;
	SecondInput = new TString(paramData.ReadString("SecondInput"));
	cout<<"SecondInput     = "<<(Text_t*)SecondInput->Data()<<endl;

	ParFile = new TString(paramData.ReadString("ParametersFileName"));
	cout<<"Parameterfile 1 = "<<(Text_t*)ParFile->Data()<<endl;
	ParFile2 = new TString(paramData.ReadString("ParametersFileName2"));
	cout<<"Parameterfile 2 = "<<(Text_t*)ParFile2->Data()<<endl;

	kEvtRemote = paramData.ReadInt("RemoteEventServer");
	cout<<"kEvtRemote      = "<<kEvtRemote<<endl;
	if ( kEvtRemote == 0){
	    inDir  = new TString(paramData.ReadString("HLD dir"));
	    inFile = new TString(paramData.ReadString("HLD file"));
	    cout<<"HLD dir         = "<<(Text_t*)inDir ->Data()<<endl;
	    cout<<"HLD file        = "<<(Text_t*)inFile->Data()<<endl;
	}
	else {
	    eventHost = new TString(paramData.ReadString("RemoteEventServerAddress"));
	    cout<<"eventHost       = "<<(Text_t*)eventHost->Data()<<endl;
	}
	refrunID = paramData.ReadInt("Reference runID");
	cout<<"Reference runID = "<<refrunID<<endl;
     
	outDir = new TString(paramData.ReadString("Output File"));
	cout<<"Output File     ="<<(Text_t*)outDir->Data()<<endl;
	cout<<"################################################################"<<endl;
	paramData.CloseFile();
    }
    catch (Bool_t ret) {
	status = ret;
    }
    return status;
}