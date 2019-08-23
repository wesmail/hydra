using namespace std;
#include "hades.h"
#include "hdetector.h"
#include "hspectrometer.h"
#include "hmdctaskset.h"
#include "htask.h"
#include "htaskset.h"
#include "hdebug.h"
#include <TROOT.h>
#include <TClass.h>
#include "haddef.h"
#include "hmdccalibrater1.h"
#include "hmdccalibrater2.h"
#include "hmdccalibrater2sim.h"
#include "hmdcdigitizer.h"
#include "hmdctrackdset.h"
#include "hmdchitf.h"
#include "hreconstructor.h"
#include "hmdcsegmentf.h"
#include "hmdcsetup.h"
#include "hruntimedb.h"
#include "hrun.h"
#include "hdatasource.h"
#include <TBrowser.h>
#include "hlocation.h"
#include "hmessagemgr.h"
#include <stdlib.h>

//#include <iostream> 
//#include <iomanip>

//*-- Author : Dan Magestro
//*--modified : Jochen Markert

///////////////////////////////////////////////////////////////////////////////
// HMdcTaskSet
//
// This HTaskSet contains the tasks for the Mdc detector
//
// HMdcTaskSet::make("selectionstring","optionstring");
//
// SELECTION : if selectionstring="rtdb" HMdcTaskSet is initialized
//             via RuntimeDb. If this flag is set, all input from the
//             optionstring will be ignored.
// ----------------------------------------------------------------
//
//
// OPTIONS :                                                     
// ----------------------------------------------------------------
//     All settings are initialized to default values. The options set in the
//     optionstring will override the defaults for the specified parameters.
//     If selectionstring is set to "rtdb", all values in optionstring will be
//     ignored.
//
//     real  (default),simulation
//     or merge                       -- real data, simulation data or embedding
//     dubna (default) or santiago    -- selects hit finder
//
//     Maximum data level
//         raw                        -- only unpacker
//         cal1                       -- up to calibrater1
//         cal2                       -- up to calibrater2
//         hit              (default) -- up to hit finder
//         fit                        -- up to track fitter
//
//     HMdcCalibrater1:
//         NoStartAndNoCal            -- do not use start,
//                                       do not use calibration
//         NoStartAndCal              -- do not use start,
//                                       use calibration
//         StartAndCal      (default) -- use start,
//                                       use calibration
//         NoTimeCuts                 -- do not use time cuts
//         TimeCuts         (default) -- use time cuts
//
//     DUBNA TRACKER:
//         MagnetOn         (default) -- Magnetic field
//         MagnetOff                  -- Magnetic field off
//         CombinedClusters (default) -- Cluster search in
//                                       combined chambers
//         ChamberClusters            -- Cluster search in
//                                       individual chambers
//         MixedClusters              -- Cluster search in
//                                       combined chambers and after it
//                                       in individual chambers
//
// Note: Thanks to TString, options can be separated and written any way
//       you want, such as
//
//             HMdcTaskSet::make("","MagnetOFF,NOstartandNOcal")
//             HMdcTaskSet::make("","ChamberClusters & STARTandCAL")
//
// For testing a pointer for the MdcDigitizer can be retrieved to set the options from the macro:
// mdcdigi=(HMdcDigitizer*)mdcTasks->HMdcTaskSet::getDigitizer();
//
// In the same way a pointer to MdcCalibrater1() can be retrieved:
// calibrater1=(HMdcCalibrater1*)mdcTasks->HMdcTaskSet::getCalibrater1();
//
// Full control over all parameters can be gained by retrieving
// the pointer to the internal setup object(example):
//
// create taskset        HMdcTaskSet *mymdctasks   = new HMdcTaskSet();
//                       Int_t testLayers[6][4]={{5,6,6,6},{6,6,6,6},{6,6,6,6},{6,6,6,6},{6,6,6,6},{6,6,6,6}};
//                       Int_t testLevel[4]={10,50,10,30};
//
// get pointer to setup  HMdcSetup* mysetup=mymdctasks->getMdcSetup();
// set values            mysetup->getMdcTrackFinderSet()->setNLayers(&testLayers[0][0]);
//  ...                   mysetup->getMdcTrackFinderSet()->setNLevel (&testLevel[0]);
//  ...                   mysetup->getMdc12FitSet()      ->setMdc12FitSet(1,1,1,kTRUE,kFALSE);
//  ...
// return pointer to
// Taskset                       HTaskSet *mdcTasks    = mymdctasks->make("","");
// For the setup objects of the other tasks it works in the same way, the description can be
// found in HMdcSetup.
//
// BEGIN_HTML<b><font color="maroon">For more information, see <a target="_top" href="http://webdb.gsi.de:8010/pls/hades_webdbs/hanal.hal_posting_query.show_posting_text?p_id=436">this analysis logbook entry<a>.</font></b> END_HTML
//
///////////////////////////////////////////////////////////////////////////////


// set counters to 0
Int_t  HMdcTaskSet::ilevel           =0;
Int_t  HMdcTaskSet::irealorsimulation=0;
Int_t  HMdcTaskSet::icalibrater      =0;
Int_t  HMdcTaskSet::itimecuts        =0;
Int_t  HMdcTaskSet::itracker         =0;
Int_t  HMdcTaskSet::imagnet          =0;
Int_t  HMdcTaskSet::itrackermode     =0;

// define default values of all arguments
Int_t  HMdcTaskSet::analysisLevel    =3;
Int_t  HMdcTaskSet::cal1ModeFlag     =2;
Int_t  HMdcTaskSet::cal1TimeCutFlag  =1;;

Int_t  HMdcTaskSet::tracker          =1;
Int_t  HMdcTaskSet::typeClustFinder  =0;
Bool_t HMdcTaskSet::isCoilOff        =kFALSE;
Int_t  HMdcTaskSet::nLevel[4]        ={10,50,10,30};
Int_t  HMdcTaskSet::nLayers[6][4]    ={{6,6,6,6},{6,6,6,6},{6,6,6,6},{6,6,6,6},{6,6,6,6},{6,6,6,6}};
Int_t  HMdcTaskSet::mdcForSeg1       =1;
Int_t  HMdcTaskSet::mdcForSeg2       =3;
Bool_t HMdcTaskSet::fitNTuple        =kFALSE;
Bool_t HMdcTaskSet::fitPrint         =kFALSE;
Int_t  HMdcTaskSet::fitAuthor        =1;
Int_t  HMdcTaskSet::fitVersion       =1;
Int_t  HMdcTaskSet::fitIndep         =0;
Int_t HMdcTaskSet::isSimulation      =0;

HMdcDigitizer*         HMdcTaskSet::mdcdigi             =0;
HMdcCalibrater1*       HMdcTaskSet::calibrater1         =0;

Int_t                  HMdcTaskSet::rtdbinput           =0;
Bool_t                 HMdcTaskSet::isCreated           =kFALSE;

HMdcSetup*             HMdcTaskSet::mdcsetup            =0;
HMdcSetup*             HMdcTaskSet::mdcsetuplocal       =0;
HMdcCalibrater1Setup*  HMdcTaskSet::calibrater1setlocal =0;
HMdcDigitizerSetup*    HMdcTaskSet::digisetlocal        =0;
HMdcTrackFinderSetup*  HMdcTaskSet::trackfindersetlocal =0;
HMdcClusterToHitSetup* HMdcTaskSet::clustertohitsetlocal=0;
HMdc12FitSetup*        HMdcTaskSet::fittersetlocal      =0;
HMdcCommonSetup*       HMdcTaskSet::commonsetlocal      =0;


HMdcTaskSet::HMdcTaskSet(Text_t name[],Text_t title[]) : HTaskSet(name,title) {
    // Constructor
    setDefaults();
    isCreated=kTRUE;
}
HMdcTaskSet::HMdcTaskSet(void) : HTaskSet() {
    // Default constructor
    setDefaults();
    isCreated=kTRUE;
}
HMdcTaskSet::~HMdcTaskSet(void) {
    // Destructor.
}
void HMdcTaskSet::copySetup()
{
    mdcsetuplocal=new HMdcSetup(*mdcsetup);
}
void HMdcTaskSet::getSets()
{
    digisetlocal        =mdcsetuplocal->getMdcDigitizerSet();
    calibrater1setlocal =mdcsetuplocal->getMdcCalibrater1Set();
    trackfindersetlocal =mdcsetuplocal->getMdcTrackFinderSet();
    clustertohitsetlocal=mdcsetuplocal->getMdcClusterToHitSet();
    fittersetlocal      =mdcsetuplocal->getMdc12FitSet();
    commonsetlocal      =mdcsetuplocal->getMdcCommonSet();
}
void HMdcTaskSet::setDefaults()
{
    // define default values of all arguments

    mdcsetuplocal=new HMdcSetup("setuplocal","setuplocal");
    getSets();
    commonsetlocal      ->setMdcCommonSet      (isSimulation,analysisLevel,tracker);
    calibrater1setlocal ->setMdcCalibrater1Set (cal1ModeFlag,cal1TimeCutFlag);
    trackfindersetlocal ->setMdcTrackFinderSet (isCoilOff,typeClustFinder,&nLayers[0][0],&nLevel[0]);
    clustertohitsetlocal->setMdcClusterToHitSet(mdcForSeg1,mdcForSeg2);
    fittersetlocal      ->setMdc12FitSet       (fitAuthor,fitVersion,fitIndep,fitNTuple,fitPrint);

    digisetlocal->setTdcModeDigi      (2);
    digisetlocal->setNtupleDigi       (kFALSE);
    digisetlocal->setUseTofDigi       (kTRUE);
    digisetlocal->setUseErrorDigi     (kTRUE);
    digisetlocal->setUseWireOffsetDigi(kTRUE);
    digisetlocal->setUseWireStatDigi  (kFALSE);
    digisetlocal->setEmbeddingModeDigi(1);
    digisetlocal->setNoiseModeDigi    (1);
    digisetlocal->setNoiseOffDigi     (0);
    digisetlocal->setNoiseLevelDigi   (0,0,0,0);
    digisetlocal->setNoiseRangeDigi   (0,0,0,0,0,0,0,0);
    digisetlocal->setOffsetsOffDigi   (0);
    digisetlocal->setOffsetsDigi      (0,0,0,0);
    digisetlocal->setCellEffOffDigi   (1);
    digisetlocal->setCellEffDigi      (88,88,88,88);

    if(!mdcsetuplocal->check()) exit(1);
}
void HMdcTaskSet::getSetupContainer()
{
    // gets the pointer to setup container,
    // init the container with the actual runId (or RefId),
    // copies the setup object to the local setup object and
    // retrieves the pointer to the sub setup objects.

    HRuntimeDb *rtdb=gHades->getRuntimeDb();

    // get runid from data source/compare if a refId has been
    // used
    HDataSource* source=gHades->getDataSource();
    Int_t refId=source->getCurrentRefId();
    Int_t runId=source->getCurrentRunId();
    Int_t myId=-99;

    if(runId==refId || refId<0)
    {
	myId=runId;
    }
    else
    {
	myId=refId;
    }

    // get pointer to HMdcSetup
    mdcsetup=(HMdcSetup*)rtdb->getContainer("MdcSetup");
    if(!mdcsetup)
    {
	Error("HMdcTaskSet:make()","RETRIEVED 0 POINTER FOR HMDCSETUP!");
	exit(1);
    }
    else
    {   // init HMdcSetup for the runId if the pointer is not 0
	// this is needed because the container should be available
	// before the TaskSet is created
	rtdbinput=1;
	rtdb->initContainers(myId);
	mdcsetup->setStatic();
	if(!mdcsetup->check())
	{  // check if init was correct

	    rtdbinput=0;
	    mdcsetup->printParam();
	    Error("HMdcTaskSet:getSetupContainer()","HMDCSETUP NOT INITIALIZED CORRECTLY!");
	    exit(1);
	}

	if(rtdbinput==1)
	{
            copySetup();
            getSets();
	}
    }
}
HTask* HMdcTaskSet::make(const char *select, Option_t *option) {
    // Returns a pointer to the Mdc task.
    // The task is created according to the specified options in Option_t *option
    // and select. If no option is specified default values will be used. If select
    // specifies rtdb the parameters will be used from runtimedb container HMdcSetup
    // and the parameters in option will be ignored.
    if(!isCreated) {setDefaults();}

  HTaskSet *tasks = new HTaskSet("Mdc","List of Mdc tasks");
  TString sel = select;
  sel.ToLower();
  TString opt=option;
  if((sel.CompareTo("rtdb"))==0)
  {
      getSetupContainer(); // gets setupcontainer, copy, get sets
  }
  else
  {   // if no rtdb input was specified
      parseArguments(opt);
  }

  //create the taskset

  //######################### REAL/ EMBEDDING ###################################

  if(commonsetlocal->getIsSimulation()==0 || commonsetlocal->getIsSimulation()==2)
  {  // real data or merge
      if(commonsetlocal->getAnalysisLevel()>0)
      {   // cal1 and higher
	  calibrater1=new HMdcCalibrater1("mdc.cal1","Cal1");
          calibrater1setlocal ->setupMdcCalibrater1(calibrater1);
	  tasks->add(calibrater1);
      }
      if(commonsetlocal->getIsSimulation()==0 && commonsetlocal->getAnalysisLevel()>1)
      {   // cal2 and higher , real data only
	  tasks->add( new HMdcCalibrater2("mdc.cal2","Cal2") );
      } 
  }  
  if (commonsetlocal->getIsSimulation()==1 || commonsetlocal->getIsSimulation()==2)
  {
      //######################### SIMULATION / EMBEDDING ###################################

      if(commonsetlocal->getAnalysisLevel()>=1 && commonsetlocal->getAnalysisLevel()>0)
      {   // cal1sim and higher
	  mdcdigi=(HMdcDigitizer*)new HMdcDigitizer("mdc.digi","Mdc digitizer");
	  digisetlocal->setupMdcDigitizer(mdcdigi);
	  tasks->add(mdcdigi);
      }
      if(commonsetlocal->getAnalysisLevel()>=2 && commonsetlocal->getAnalysisLevel()>0)
      {   // cal2sim and higher
	  tasks->add( new HMdcCalibrater2Sim("mdc.cal2","Cal2"));
      }
  }
  //######################### DUBNA TRACKER ###################################

  if(commonsetlocal->getTracker()==1 && commonsetlocal->getAnalysisLevel()>=3 )
  {   // dubna tracker / hit and higher
      if(commonsetlocal->getAnalysisLevel()>=3 )
      {   // trackfinder and higher
	  trackfindersetlocal->setupMdcTrackFinder();
	  tasks->add( HMdcTrackDSet::hMdcTrackFinder("mdc.hitf","mdc.hitf") );
      }
      if(commonsetlocal->getAnalysisLevel()==3 )
      {   // trackfinder
          clustertohitsetlocal->setupMdcClusterToHit();
	  tasks->add( HMdcTrackDSet::hMdcClusterToHit("mdc.c2h","mdc.c2h") );
      }
      if(commonsetlocal->getAnalysisLevel()==4 )
      {   // fitter
	  fittersetlocal->setupMdc12Fit();
	  tasks->add( HMdcTrackDSet::hMdc12Fit("mdc.12fit","mdc.12fit") );
      }
  }
  //######################### SANTIAGO TRACKER ###################################

  else if(commonsetlocal->getTracker()==2 && commonsetlocal->getAnalysisLevel()>=3)
  {  // santiago tracker

     HLocation aLoc;
     aLoc.set(2,0,0);
     HMdcHitF *finder=0;
     Text_t buffer[255];

     // Get modules from "Mdc" detector, needed to make tasks for
     // individual chamber hit finders
     Int_t *mods =((HDetector*)(((HSpectrometer*)(gHades->getSetup()))->getDetector("Mdc")))->getModules();
     Int_t mdcMods[6][4];
     for (Int_t sector = 0; sector<6; sector++) {
     	for (Int_t i=0;i<3;i++) mdcMods[sector][i] = mods[sector*4+i];
     }

     for (Int_t sector = 0; sector<6; sector++) {
     	aLoc[0]=sector;

     	// Add hit finders for sector
     	for (Int_t i=0;i<3;i++) {

     	   if (mdcMods[sector][i] != 0) {
     	      sprintf(buffer,"%s%i.%i","mdc.hitf",sector,i);
     	      finder=new HMdcHitF(buffer,"Mdc Hit finder");
     	      aLoc[1]=i;
     	      finder->setLoc(aLoc);
     	      tasks->add(finder);
     	   }
     	}
     }

     tasks->add( new HMdcSegmentF("mdc.h2s","mdc.h2s") );
  }
  // print the setup status to the screen
  printStatus();

  return tasks;
}
void HMdcTaskSet::parseArguments(TString s1)
{
    // The option string is searched for known arguments and
    // the internal variables of the task are defined according
    // to the arguments. Not set options will be defined by the
    // default values.
    // If the string is empty default values will be used.
    // If unknown arguments are found error messages will be displayed
    // and the program will be exited.
    // If two arguments for the same type of option are found an error
    // message will show up and the program is exited too.

    // switch maximum data level
    TString sraw             ="raw";
    TString scal1            ="cal1";
    TString scal2            ="cal2";
    TString shit             ="hit";
    TString sfit             ="fit";

    // switch between real data or simulation
    TString ssimulation      ="simulation";
    TString sreal            ="real";
    TString smerge           ="merge";

    //  Calibrater1 options
    TString snostartandnocal ="nostartandnocal";
    TString snostartandcal   ="nostartandcal";
    TString sstartandcal     ="startandcal";
    TString stimecuts        ="timecuts";
    TString snotimecuts      ="notimecuts";

    // switch trackers
    TString sdubna           ="dubna";
    TString ssantiago        ="santiago";

    // Dubna tracker options
    TString smagnetoff       ="magnetoff";
    TString smagneton        ="magneton";

    TString scombinedclusters="combinedclusters";
    TString schamberclusters ="chamberclusters";
    TString smixedclusters   ="mixedclusters";
    TString s2=s1;
    s1.ToLower();
    s1.ReplaceAll(" ","");
    Ssiz_t len=s1.Length();
    if(len!=0)
    {
	char* mystring=(char*)s1.Data();

	Char_t* buffer;
	TList myarguments;
	TObjString *stemp;
	TString argument;
	Int_t count=0;

	while(1) // find all token in option string and put them to a list
	{
	    if(count==0)
	    {
		buffer=strtok(mystring,",");
		stemp=new TObjString(buffer);
		myarguments.Add(stemp);
	    }

	    if(!(buffer=strtok(NULL,",")))break;
	    stemp=new TObjString(buffer);
	    myarguments.Add(stemp);

	    count++;
	}
	ilevel=0;
        irealorsimulation=0;
	icalibrater=0;
	icalibrater=0;
        itimecuts=0;
	itracker=0;
        imagnet=0;
	itrackermode=0;

	TIterator* myiter=myarguments.MakeIterator();

	// iterate over the lis of arguments and compare the
        // to known key words.
	while ((stemp=(TObjString*)myiter->Next())!= 0) {
	    argument=stemp->GetString();
	    if(argument.CompareTo(sraw)==0)
	    {
		ilevel++;
		if(ilevel>1)
		{
		    ERROR_msg(DET_MDC,"overlap with other argument for analysis level:RAW!");
		    gHades->getMsg()->error(10,DET_MDC,GetName(),"INPUT OPTIONSTRING: ",s2.Data());
		    printOptions();
		    exit(1);
		}
                commonsetlocal->setAnalysisLevel(0);
	    }
	    else if(argument.CompareTo(scal1)==0)
	    {
		ilevel++;
		if(ilevel>1)
		{
		    ERROR_msg(DET_MDC,"overlap with other argument for analysis level:CAL1!");
		    gHades->getMsg()->error(10,DET_MDC,GetName(),"INPUT OPTIONSTRING: %s",s2.Data());
		    printOptions();
		    exit(1);
		}
                commonsetlocal->setAnalysisLevel(1);
	    }
	    else if(argument.CompareTo(scal2)==0)
	    {
		ilevel++;
		if(ilevel>1)
		{
		    ERROR_msg(DET_MDC,"overlap with other argument for analysis level:CAL2!");
		    gHades->getMsg()->error(10,DET_MDC,GetName(),"INPUT OPTIONSTRING: %s",s2.Data());
		    printOptions();
		    exit(1);
		}
                commonsetlocal->setAnalysisLevel(2);
	    }
	    else if(argument.CompareTo(shit)==0)
	    {
		ilevel++;
		if(ilevel>1)
		{
		    ERROR_msg(DET_MDC,"overlap with other argument for analysis level:HIT!");
		    gHades->getMsg()->error(10,DET_MDC,GetName(),"INPUT OPTIONSTRING: %s",s2.Data());
		    printOptions();
		    exit(1);
		}
                commonsetlocal->setAnalysisLevel(3);
	    }
	    else if(argument.CompareTo(sfit)==0)
	    {
		ilevel++;
		if(ilevel>1)
		{
		    ERROR_msg(DET_MDC,"overlap with other argument for analysis level:FIT!");
		    gHades->getMsg()->error(10,DET_MDC,GetName(),"INPUT OPTIONSTRING: %s",s2.Data());
		    printOptions();
		    exit(1);
		}
                commonsetlocal->setAnalysisLevel(4);
	    }
	    else if(argument.CompareTo(ssimulation)==0)
	    {
		irealorsimulation++;
		if(irealorsimulation>1)
		{
		    ERROR_msg(DET_MDC,"overlap with other argument for data type!:SIMULATION");
		    gHades->getMsg()->error(10,DET_MDC,GetName(),"INPUT OPTIONSTRING: %s",s2.Data());
		    printOptions();
		    exit(1);
		}
                commonsetlocal->setIsSimulation(1);
	    }
	    else if(argument.CompareTo(sreal)==0)
	    {
		irealorsimulation++;
		if(irealorsimulation>1)
		{
		    ERROR_msg(DET_MDC,"overlap with other argument for data type:REAL!");
		    gHades->getMsg()->error(10,DET_MDC,GetName(),"INPUT OPTIONSTRING: ",s2.Data());
		    printOptions();
		    exit(1);
		}
		commonsetlocal->setIsSimulation(0);
	    }
	    else if(argument.CompareTo(smerge)==0)
	    {
		irealorsimulation++;
		if(irealorsimulation>1)
		{
		    ERROR_msg(DET_MDC,"overlap with other argument for data type:REAL!");
		    gHades->getMsg()->error(10,DET_MDC,GetName(),"INPUT OPTIONSTRING: ",s2.Data());
		    printOptions();
		    exit(1);
		}
		commonsetlocal->setIsSimulation(2);
	    }
	    else if(argument.CompareTo(snostartandnocal)==0)
	    {
		icalibrater++;
		if(icalibrater>1)
		{
		    ERROR_msg(DET_MDC,"overlap with other argument for calibrater option:NoStartAndNoCal!");
		    gHades->getMsg()->error(10,DET_MDC,GetName(),"INPUT OPTIONSTRING: %s",s2.Data());
		    printOptions();
		    exit(1);
		}
                calibrater1setlocal->setModeFlagCal1(3);
	    }
	    else if(argument.CompareTo(snostartandcal)==0)
	    {
		icalibrater++;
		if(icalibrater>1)
		{
		    ERROR_msg(DET_MDC,"overlap with other argument for calibrater option:NoStartAndCal!");
		    gHades->getMsg()->error(10,DET_MDC,GetName(),"INPUT OPTIONSTRING: %s",s2.Data());
		    printOptions();
		    exit(1);
		}
		calibrater1setlocal->setModeFlagCal1(1);
	    }
	    else if(argument.CompareTo(sstartandcal)==0)
	    {
		icalibrater++;
		if(icalibrater>1)
		{
		    ERROR_msg(DET_MDC,"overlap with other argument for calibrater option:StartAndCal!");
		    gHades->getMsg()->error(10,DET_MDC,GetName(),"INPUT OPTIONSTRING: %s",s2.Data());
		    printOptions();
		    exit(1);
		}
		calibrater1setlocal->setModeFlagCal1(2);
	    }
	    else if(argument.CompareTo(snotimecuts)==0)
	    {
		itimecuts++;
		if(itimecuts>1)
		{
		    ERROR_msg(DET_MDC,"overlap with other argument for time cuts:NoTimeCuts!");
		    gHades->getMsg()->error(10,DET_MDC,GetName(),"INPUT OPTIONSTRING: %s",s2.Data());
		    printOptions();
		    exit(1);
		}
		calibrater1setlocal->setTimeCutFlagCal1(0);
	    }
	    else if(argument.CompareTo(stimecuts)==0)
	    {
		itimecuts++;
		if(itimecuts>1)
		{
		    ERROR_msg(DET_MDC,"overlap with other argument for time cuts:TimeCuts!");
		    gHades->getMsg()->error(10,DET_MDC,GetName(),"INPUT OPTIONSTRING: %s",s2.Data());
		    printOptions();
		    exit(1);
		}
		calibrater1setlocal->setTimeCutFlagCal1(1);
	    }
	    else if(argument.CompareTo(sdubna)==0)
	    {
		itracker++;
		if(itracker>1)
		{
		    ERROR_msg(DET_MDC,"overlap with other argument for tracker type:DUBNA!");
		    gHades->getMsg()->error(10,DET_MDC,GetName(),"INPUT OPTIONSTRING: %s",s2.Data());
		    printOptions();
		    exit(1);
		}
		commonsetlocal->setTracker(1);
	    }
	    else if(argument.CompareTo(ssantiago)==0)
	    {
		itracker++;
		if(itracker>1)
		{
		    ERROR_msg(DET_MDC,"overlap with other argument for tracker type:SANTIAGO!");
		    gHades->getMsg()->error(10,DET_MDC,GetName(),"INPUT OPTIONSTRING: %s",s2.Data());
		    printOptions();
		    exit(1);
		}
		commonsetlocal->setTracker(2);
	    }
	    else if(argument.CompareTo(smagnetoff)==0)
	    {
		imagnet++;
		if(imagnet>1)
		{
		    ERROR_msg(DET_MDC,"overlap with other argument for magnet status:MagnetOff!");
		    gHades->getMsg()->error(10,DET_MDC,GetName(),"INPUT OPTIONSTRING: %s",s2.Data());
		    printOptions();
		    exit(1);
		}
		trackfindersetlocal->setIsCoilOff(kTRUE);
	    }
	    else if(argument.CompareTo(smagneton)==0)
	    {
		imagnet++;
		if(imagnet>1)
		{
		    ERROR_msg(DET_MDC,"overlap with other argument for magnet status:MagnetOn!");
		    gHades->getMsg()->error(10,DET_MDC,GetName(),"INPUT OPTIONSTRING: %s",s2.Data());
		    printOptions();
		    exit(1);
		}
		trackfindersetlocal->setIsCoilOff(kFALSE);
	    }
	    else if(argument.CompareTo(scombinedclusters)==0)
	    {
		itrackermode++;
		if(itrackermode>1)
		{
		    ERROR_msg(DET_MDC,"overlap with other argument for tracker mode:CombinedChamberClusters!");
		    gHades->getMsg()->error(10,DET_MDC,GetName(),"INPUT OPTIONSTRING: %s",s2.Data());
		    printOptions();
		    exit(1);
		}
		trackfindersetlocal->setTypeClustFinder(0);
	    }
	    else if(argument.CompareTo(schamberclusters)==0)
	    {
		itrackermode++;
		if(itrackermode>1)
		{
		    ERROR_msg(DET_MDC,"overlap with other argument for tracker mode:ChamberClusters!");
		    gHades->getMsg()->error(10,DET_MDC,GetName(),"INPUT OPTIONSTRING: %s",s2.Data());
		    printOptions();
		    exit(1);
		}
		trackfindersetlocal->setTypeClustFinder(1);
	    }
            else if(argument.CompareTo(smixedclusters)==0)
	    {
		itrackermode++;
		if(itrackermode>1)
		{
		    ERROR_msg(DET_MDC,"overlap with other argument for tracker mode:MixedClusters!");
		    gHades->getMsg()->error(10,DET_MDC,GetName(),"INPUT OPTIONSTRING: %s",s2.Data());
		    printOptions();
		    exit(1);
		}
		trackfindersetlocal->setTypeClustFinder(2);
	    }
	    else
	    {
		gHades->getMsg()->error(10,DET_MDC,GetName(),"unknown argument %s !",argument.Data());
		gHades->getMsg()->error(10,DET_MDC,GetName(),"INPUT OPTIONSTRING: %s",s2.Data());
		printOptions();
		exit(1);
	    }
	}
    }
}
void HMdcTaskSet::printOptions()
{
    // Prints the options of HMdcTaskSet
  SEPERATOR_msg("#",60);
  INFO_msg(10,DET_MDC,"SELECTION : rtdb               -- init from HMdcSetup");
  INFO_msg(10,DET_MDC,"            no selection       -- init from OptionString");
  SEPERATOR_msg("-",60);
  INFO_msg(10,DET_MDC," OPTIONS   :");
  SEPERATOR_msg("-",60);
  INFO_msg(10,DET_MDC,"real  (default) or simulation  -- real data or simulation data");
  INFO_msg(10,DET_MDC,"dubna (default) or santiago    -- selects hit finder");
  SEPERATOR_msg("-",60);
  INFO_msg(10,DET_MDC,"Maximum data level");
  INFO_msg(10,DET_MDC,"     raw                        -- only unpacker");
  INFO_msg(10,DET_MDC,"     cal1                       -- up to calibrater1");
  INFO_msg(10,DET_MDC,"     cal2                       -- up to calibrater2");
  INFO_msg(10,DET_MDC,"     hit              (default) -- up to hit finder");
  INFO_msg(10,DET_MDC,"     fit                        -- up to track fitter");
  INFO_msg(10,DET_MDC,"HMdcCalibrater1:");
  INFO_msg(10,DET_MDC,"     NoStartAndNoCal            -- do not use start,");
  INFO_msg(10,DET_MDC,"                                   do not use calibration");
  INFO_msg(10,DET_MDC,"     NoStartAndCal              -- do not use start,");
  INFO_msg(10,DET_MDC,"                                   use calibration");
  INFO_msg(10,DET_MDC,"     StartAndCal      (default) -- use start,");
  INFO_msg(10,DET_MDC,"                                   use calibration");
  INFO_msg(10,DET_MDC,"     NoTimeCuts                 -- do not use time cuts");
  INFO_msg(10,DET_MDC,"     TimeCuts         (default) -- use time cuts");
  INFO_msg(10,DET_MDC," DUBNA TRACKER:");
  INFO_msg(10,DET_MDC,"      MagnetOn         (default) -- Magnetic field");
  INFO_msg(10,DET_MDC,"      MagnetOff                  -- Magnetic field off");
  INFO_msg(10,DET_MDC,"     CombinedClusters (default) -- Cluster search in");
  INFO_msg(10,DET_MDC,"                                   combined chambers");
  INFO_msg(10,DET_MDC,"     ChamberClusters            -- Cluster search in");
  INFO_msg(10,DET_MDC,"                                   individual chambers");
  INFO_msg(10,DET_MDC,"     MixedClusters              -- Cluster search in");
  INFO_msg(10,DET_MDC,"                                   comb.+indiv. chambers");
  SEPERATOR_msg("#",60);
}
HMdcDigitizer* HMdcTaskSet::getDigitizer()
{
    // Returns a pointer to the HMdcDigitizer.
    // If the pointer is 0 an error message will be displayed
    // and the program will be exited.

    if(mdcdigi!=0)
    {
	return mdcdigi;
    }
    else
    {
     	Error("HMdcTaskSet:getDigitzer():"
	      ,"\n POINTER TO MDCDIGITIZER IS ZERO! \n DON'T CALL THIS FUNCTION WHEN RUNNING WITH REAL DATA!");
        exit(1);
    }
}
HMdcCalibrater1* HMdcTaskSet::getCalibrater1()
{
    // Returns a pointer to the HMdcCalibrater1.
    // If the pointer is 0 an error message will be displayed
    // and the program will be exited.

    if(calibrater1!=0)
    {
	return calibrater1;
    }
    else
    {
     	Error("HMdcTaskSet:getCalibrater1():"
	      ,"\n POINTER TO MDCCALIBRATER1 IS ZERO! \n DON'T CALL THIS FUNCTION WHEN RUNNING WITH SIM DATA!");
        exit(1);
    }
}
HMdcSetup* HMdcTaskSet::getMdcSetup()
{
    if(mdcsetuplocal!=0)
    {
	return mdcsetuplocal;
    }
    else
    {
	Error("HMdcTaskSet:getMdcSetup():"
	      ,"\n POINTER TO MDCSETUP IS ZERO!");
	exit(1);
    }
}
void HMdcTaskSet::printStatus()
{
    // Prints the status of the configuration of
    // HMdcTaskSet.

    printOptions();
    SEPERATOR_msg("#",60);
    INFO_msg(10,DET_MDC,"ACTUAL CONFIGURATION :");
    commonsetlocal->printParam();
    if(calibrater1)calibrater1->printStatus();
    if(mdcdigi)    mdcdigi    ->printStatus();
    HMdcTrackDSet::printStatus();//pointer checked inside function
    SEPERATOR_msg("#",60);
}


ClassImp(HMdcTaskSet)
