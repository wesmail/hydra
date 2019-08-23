#include "TObject.h"
#include "TThread.h"
#include "TCut.h"
#include "TGo4Hades.h"

#include "hparoraio.h"
#include "htaskset.h"

#include "hgo4onlineconfig.h"

#include "hstarthistbooker.h"
#include "hstarthistfiller.h"
#include "hstartgo4mon.h"

#include "hrichhistbooker.h"
#include "hrichhistfiller.h"
#include "hrichgo4mon.h"

#include "hmdchistbooker.h"
#include "hmdchistfiller.h"
#include "hmdcgo4mon.h"

#include "htofhistbooker.h"
#include "htofhistfiller.h"
#include "htofgo4mon.h"

#include "htofinohistbooker.h"
#include "htofinohistfiller.h"
#include "htofinogo4mon.h"

#include "hshowerhistbooker.h"
#include "hshowerhistfiller.h"
#include "hshowergo4mon.h"

#include "hstartdetector.h"
#include "hstartunpacker.h"
#include "hstartcalibrater.h"
#include "hstarthitf.h"

#include "hrichdetector.h"
#include "hrichunpackercal99.h"
#include "hrichunpackerraw99.h"
#include "hrichanalysis.h"
#include "hrichparticleidentifier.h"

#include "hmdcdetector.h"
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

ClassImp(TGo4Hades) 

Bool_t TGo4Hades::kINIT=kFALSE;
Int_t  TGo4Hades::fCount =0;
Int_t  TGo4Hades::nLoop  =100;

HGo4OnlineParam*  TGo4Hades::onlineparam=0;
HGo4OnlineConfig* TGo4Hades::onlineconfig=0;

HStartHistBooker* TGo4Hades::Starthistbooker=0;
TList* TGo4Hades::StartactiveHists=0;

HRichHistBooker* TGo4Hades::Richhistbooker=0;
TList* TGo4Hades::RichactiveHists=0;

HMdcHistBooker* TGo4Hades::Mdchistbooker=0;
TList* TGo4Hades::MdcactiveHists=0;

HTofHistBooker* TGo4Hades::Tofhistbooker=0;
TList* TGo4Hades::TofactiveHists=0;

HTofinoHistBooker* TGo4Hades::Tofinohistbooker=0;
TList* TGo4Hades::TofinoactiveHists=0;

HShowerHistBooker* TGo4Hades::Showerhistbooker=0;
TList* TGo4Hades::ShoweractiveHists=0;

MyParameter::MyParameter(){}
MyParameter::MyParameter( Text_t* text) : TGo4Parameter( text){} 
MyParameter::~MyParameter(){}

Bool_t MyParameter::UpdateFrom(TGo4Parameter* para){
  if ( para->InheritsFrom("MyParameter" )) {
    MyParameter *src = dynamic_cast<MyParameter*> (para); 
    test1=src->test1; 
    test2=src->test2;
  }
      return kTRUE;
}

TGo4Hades::TGo4Hades(){
  // put default values
  fxTreeSize=500000; // 500kbytes
  fxRefreshNb=100; // refresh after 100 evt
  fxReset=0; 
  fxDump=kFALSE;


  fbInit =kFALSE;
  onlineconfig= new HGo4OnlineConfig("Go4OnlineConfig","Go4OnlineConfig","Go4OnlineConfigProduction");
  onlineparam=onlineconfig->getGo4OnlineParam();

  Starthistbooker  =new HStartHistBooker("StartHistBooker","StartHistBooker","StartHistBookerProduction");
  StartactiveHists =Starthistbooker->getListOfActiveHists();

  Richhistbooker   =new HRichHistBooker("RichHistBooker","RichHistBooker","RichHistBookerProduction");
  RichactiveHists  =Richhistbooker->getListOfActiveHists();

  Mdchistbooker    =new HMdcHistBooker("MdcHistBooker","MdcHistBooker","MdcHistBookerProduction");
  MdcactiveHists   =Mdchistbooker->getListOfActiveHists();

  Tofhistbooker    =new HTofHistBooker("TofHistBooker","TofHistBooker","TofHistBookerProduction");
  TofactiveHists   =Tofhistbooker->getListOfActiveHists();

  Tofinohistbooker    =new HTofinoHistBooker("TofinoHistBooker","TofinoHistBooker","TofinoHistBookerProduction");
  TofinoactiveHists   =Tofinohistbooker->getListOfActiveHists();

  Showerhistbooker =new HShowerHistBooker("ShowerHistBooker","ShowerHistBooker","ShowerHistBookerProduction");
  ShoweractiveHists=Showerhistbooker->getListOfActiveHists();

}
Bool_t TGo4Hades::InitEventClasses(){

  TGo4Analysis::InitEventClasses();
  SetDynListInterval(100);         

  //read some parameters
  if (!readAscii("../../analysisParams.txt")) return kFALSE;
  
  myHades = new Hades();

  //myHades->getMsg()->switchOff();
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

  Int_t mdcUnpackers[17]    = { 201,202,203,204,205,206,
                                207,208,209,210,211,212,
				213,214,215,216,217 };

  Int_t tofUnpackers[3]    = { 411,412,413 };
  Int_t tofinoUnpackers[1] = { 412 };
  Int_t showerUnpackers[12] = { 300,301,302,303,304,305,
                                306,307,308,309,310,311 };

  spec = myHades->getSetup();
  rtdb = myHades->getRuntimeDb();

  InitDetectorSetup();
  initParams();

  onlineconfig->initStandAlone();
  onlineconfig->printStatus();

  if(onlineparam->getStart()==1)
  {
      for(UInt_t i=0; i<sizeof(startUnpackers)/sizeof(Int_t); i++){
          source->addUnpacker( new HStartUnpacker(startUnpackers[i]) );}
  }

  if(onlineparam->getRich()==1)
  {
      for(UInt_t i=0; i<sizeof(richUnpackers)/sizeof(Int_t); i++){
	  if(onlineparam->getRichRaw()==1)source->addUnpacker( new HRichUnpackerRaw99(richUnpackers[i]) );
	  if(onlineparam->getRichCal()==1)source->addUnpacker( new HRichUnpackerCal99(richUnpackers[i]) );
      }
  }
  if(onlineparam->getMdc()==1)
  {
      for(UInt_t i=0; i<sizeof(mdcUnpackers)/sizeof(Int_t); i++){
	  source->addUnpacker( new HMdcUnpacker(mdcUnpackers[i],kTRUE,kFALSE) );}
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


  HTaskSet *startTasks  = new HTaskSet("Start","List of Start tasks");
  HTaskSet *richTasks   = new HTaskSet("Rich","List of Rich tasks");
  HTaskSet* mdcTasks    = new HTaskSet("Mdc", "List of mdc tasks");
  HTaskSet *tofTasks    = new HTaskSet("Tof","List of Tof tasks");
  HTaskSet *tofinoTasks = new HTaskSet("Tofino","List of Tofino tasks");
  HTaskSet* showerTasks = new HTaskSet("Shower", "List of Shower tasks");

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
      if(onlineparam->getRichHit()==1)richTasks->add(new HRichAnalysis("rich.ana","Ring finder",kFALSE));
      if(onlineparam->getRichHit()==1)richTasks->add(new HRichParticleIdentifier("rich.id","Rich PID"));
      richTasks->add(new HRichGo4Mon("rich.mon","rich.mon"));
      Richhistbooker->initStandAlone();
  }
  if(onlineparam->getMdc()==1)
  {
      //--------------------MDC---------------------------------------
      if(onlineparam->getMdcCal()==1)mdcTasks->add(new HMdcCalibrater1("mdc.cal1","mdc.cal1",1,1,0));
      mdcTasks->add(new HMdcGo4Mon("mdc.mon","mdc.mon"));
      Mdchistbooker->initStandAlone();
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
  if(onlineparam->getStart()==1) masterTaskSet->add(startTasks);
  if(onlineparam->getRich()==1)  masterTaskSet->add(richTasks);
  if(onlineparam->getMdc()==1)   masterTaskSet->add(mdcTasks);
  if(onlineparam->getTof()==1)   masterTaskSet->add(tofTasks);
  if(onlineparam->getTofino()==1)masterTaskSet->add(tofinoTasks);
  if(onlineparam->getShower()==1)masterTaskSet->add(showerTasks);

  if(!myHades->init()) {
    printf("\nError in Hades::init() \n"); 
    exit(2);
  } 

  //no output now
  //	myHades->setOutputFile((Text_t*) outDir->Data(),"RECREATE","Test",2);
  myHades->makeTree();
  
  AddTree( (TTree*) myHades->getTree() );
  myHades->getTree()->SetDirectory(gROOT);

  gROOT->cd();
  fbInitIsDone=kTRUE;
  return kTRUE;  
}
Int_t TGo4Hades::UserPreLoop(){
  return 0;
}	

Int_t TGo4Hades::UserPostLoop(){
  // delete the whole stuff every new start

    if (myHades){
    RemoveTree( (TTree*) myHades->getTree() );
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
                          {1,1,1,0},
                          {1,1,0,0},
                          {1,1,1,0} };      

    Int_t tofMods[22] = {1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

    Int_t showerMods[3]   = {1,2,3};
    Int_t tofinoMods[6][1]= { {1},{1},{1},{1},{1},{1} };
    Int_t startMods[2]    = {1,1};
    Int_t trigMods[1]     = {1};

    HSpectrometer* spec = gHades->getSetup();

    spec->addDetector(new HStartDetector);
    spec->addDetector(new HRichDetector);
    spec->addDetector(new HMdcDetector);
    spec->addDetector(new HTofDetector);
    spec->addDetector(new HTofinoDetector);
    spec->addDetector(new HShowerDetector);

    spec->getDetector("Start")->setModules(-1,startMods);


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

    if( nOracle ==0 || nOracle == 2 ){
	//HParRootFileIo *input=new HParRootFileIo;
	//input->open((Text_t*) RootParFile->Data(),"READ");

	if (nOracle ==0) {      

	    HParRootFileIo *input1=new HParRootFileIo;
	    //input1->open("/misc/kempter/Calibration/Nov2001/params_final/out/mdcNov01_All_real_geom_270902.root","READ");
            input1->open("../../allParamTest.root","READ");
            rtdb->setFirstInput(input1);

	    HParAsciiFileIo* input2= new HParAsciiFileIo();
	    input2->open("../../onlinehists.txt");
	    rtdb->setSecondInput(input2);
	}
    }
    if(nOracle == 1 ||  nOracle == 2){
	HParOraIo *input2 =new HParOraIo();
	input2->open();
	rtdb->setSecondInput(input2);
    }
    kINIT=kTRUE;
    return 0;
}
Int_t TGo4Hades::UserEventFunc(){

    if (! gHades ) InitEventClasses();

    nLoop=onlineparam->getNLoop();
    Int_t stat= gHades->eventLoop(nLoop);

    if (stat <= 0 ){
	CloseAnalysis();
    }

    // control the Tree buffering size
    if ( (gHades->getTree()->GetTotBytes() > fxTreeSize ) && (fxDump == kFALSE) ) {
	gHades->getTree()->Reset();
    }

    //---------------------------SENDING TO GUI-----------------------------

    //---------------------------START--------------------------------------
    if(onlineparam->getStart()==1)
    {
	if(((fCount*nLoop)%onlineparam->getStartRefresh()) == 0 )
	{
	    TIterator* myiter=StartactiveHists->MakeIterator();
	    HMonHistAddon* h=0;
	    TString buffer;
	    while ((h=(HMonHistAddon*)myiter->Next())!= 0)
	    {
		SendObjectToGUI((TNamed*) h);
	    }
	    delete myiter;
	    Starthistbooker->resetActiveList(2,fCount*nLoop);
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
		SendObjectToGUI((TNamed*) h);
	    }
	    delete myiter;
	    Richhistbooker->resetActiveList(2,fCount*nLoop);
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

	    TIterator* myiter=MdcactiveHists->MakeIterator();
	    HMonHistAddon* h=0;
	    TString buffer;
	    while ((h=(HMonHistAddon*)myiter->Next())!= 0)
	    {
		SendObjectToGUI((TNamed*) h);
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
		SendObjectToGUI((TNamed*) h);
	    }
	    Tofhistbooker->resetActiveList(2,fCount*nLoop);
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
		SendObjectToGUI((TNamed*) h);
	    }
	    Tofinohistbooker->resetActiveList(2,fCount*nLoop);
	    delete myiter;
	}
    }
    //--------------------------SHOWER-------------------------------------------
    if(onlineparam->getShower()==1)
    {
	if(((fCount*nLoop)%onlineparam->getShowerRefresh()) == 0 )
	{
	    TIterator* myiter=ShoweractiveHists->MakeIterator();
	    HMonHistAddon* h=0;
	    TString buffer;
	    while ((h=(HMonHistAddon*)myiter->Next())!= 0)
	    {
		SendObjectToGUI((TNamed*) h);
	    }
	    Showerhistbooker->resetActiveList(2,fCount*nLoop);
	    delete myiter;
	}
    }
    //-----------------------------------------------------------------------

    ++fCount;
    return stat;
}
Bool_t TGo4Hades::readAscii(Char_t *fileName){

    Bool_t status = kTRUE;
    try {
	HAsciiKey paramData;
	paramData.OpenFile(fileName,"read");
	paramData.SetActiveSection("Parameters for analysis");
	nOracle  = paramData.ReadInt("Oracle");
	cout<<"nOracle "<<nOracle<<endl;
	if( nOracle == 0 || nOracle == 2 ){
	    RootParFile = new TString(paramData.ReadString("ParametersFileName"));
	    cout<<(Text_t*)RootParFile->Data()<<endl;
	}
	kEvtRemote = paramData.ReadInt("RemoteEventServer");
	cout<<"kEvtRemote"<<kEvtRemote<<endl;
	if ( kEvtRemote == 0){
	    inDir  = new TString(paramData.ReadString("HLD dir"));
	    inFile = new TString(paramData.ReadString("HLD file"));
	}
	else {
	    eventHost = new TString(paramData.ReadString("RemoteEventServerAddess"));
	    cout<<"eventHost"<<(Text_t*)eventHost->Data()<<endl;
	}
	refrunID = paramData.ReadInt("Reference runID");
	outDir = new TString(paramData.ReadString("Output File"));
	paramData.CloseFile();
    }
    catch (Bool_t ret) {
	status = ret;
    }
    return status;
}
