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
#include "hmdctrackfinder.h"
#include "hmdcclustertohit.h"
#include "hmdchitf.h"
#include "hmdc12fit.h"
#include "hreconstructor.h"
#include "hmdcsegmentf.h"
#include <TBrowser.h>
#include "hlocation.h"
#include <stdlib.h>

#include <iostream.h>

//*-- Author : Dan Magestro
//*--modified : Jochen Markert

///////////////////////////////////////////////////////////////////////////////
// HMdcTaskSet
//
//  This HTaskSet contains the tasks for the Mdc detector
//
// OPTIONS :                                                     
// ----------------------------------------------------------------
//     real  (default) or simulation  -- real data or simulation data
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
// The DUBNA track fitter can be configured via:
// mdcTasks->HMdcTaskSet::setDubnaFitter(kFALSE,kFALSE,1);  // (NTuple,print,version)
//
// BEGIN_HTML<b><font color="maroon">For more information, see <a target="_top" href="http://webdb.gsi.de:8010/pls/hades_webdbs/hanal.hal_posting_query.show_posting_text?p_id=436">this analysis logbook entry<a>.</font></b> END_HTML
//
///////////////////////////////////////////////////////////////////////////////


// set counters to 0
Int_t HMdcTaskSet::ilevel=0;
Int_t HMdcTaskSet::irealorsimulation=0;
Int_t HMdcTaskSet::icalibrater=0;
Int_t HMdcTaskSet::itimecuts=0;
Int_t HMdcTaskSet::itracker=0;
Int_t HMdcTaskSet::imagnet=0;
Int_t HMdcTaskSet::itrackermode=0;

// define default values of all arguments
Int_t HMdcTaskSet::analysisLevel=3;
Int_t HMdcTaskSet::cal1ModeFlag=2;
Int_t HMdcTaskSet::cal1TimeCutFlag=1;;

Int_t HMdcTaskSet::tracker=1;
Int_t HMdcTaskSet::typeClustFinder=0;
Bool_t HMdcTaskSet::isCoilOff=kFALSE;
Bool_t HMdcTaskSet::fitNTuple=kFALSE;
Bool_t HMdcTaskSet::fitPrint=kFALSE;
Int_t HMdcTaskSet::fitVersion=1;
Bool_t HMdcTaskSet::isSimulation=kFALSE;

HMdcDigitizer* HMdcTaskSet::mdcdigi=0;

HMdcTaskSet::HMdcTaskSet(Text_t name[],Text_t title[]) : HTaskSet(name,title) {
  // Constructor
    }

HMdcTaskSet::HMdcTaskSet(void) : HTaskSet() {
  // Default constructor
    }

HMdcTaskSet::~HMdcTaskSet(void) {
  // Destructor.
}

HTask* HMdcTaskSet::make(const char *select, Option_t *option) {
    // Returns a pointer to the Mdc task or taskset specified by 'select'
    // The task is created according to the specified options in Option_t *option.
    // if no option is specified default values will be used.

  HTaskSet *tasks = new HTaskSet("Mdc","List of Mdc tasks");

  TString sel = select;
  sel.ToLower();

  TString opt=option;

  parseArguments(opt);
  printStatus();
  if(!isSimulation)
  {
  if(analysisLevel>0)tasks->add( new HMdcCalibrater1("mdc.cal1","Cal1",cal1ModeFlag,cal1TimeCutFlag) );
  if(analysisLevel>1)tasks->add( new HMdcCalibrater2("mdc.cal2","Cal2") );
  }
  else if (isSimulation)
  {
      mdcdigi=(HMdcDigitizer*)new HMdcDigitizer("mdc.digi","Mdc digitizer");
      mdcdigi->setTdcMode(2);
      mdcdigi->setNTuple(kFALSE);
      mdcdigi->setTofUse(kTRUE);
      mdcdigi->setOffsets(1.5,2.5,4.5,5.5);
      mdcdigi->setEffLevel(88.,88.,88.,88.);

      if(analysisLevel>=1 &&analysisLevel>0)tasks->add(mdcdigi);
      if(analysisLevel>=2 &&analysisLevel>0)tasks->add( new HMdcCalibrater2Sim("mdc.cal2","Cal2") );
  }

  if(tracker==1 && analysisLevel>=3 ) {

     if(analysisLevel>=3 )tasks->add( new HMdcTrackFinder("mdc.hitf","mdc.hitf",isCoilOff,typeClustFinder) );
     if(analysisLevel==3 )tasks->add( new HMdcClusterToHit("mdc.c2h","mdc.c2h") );
     if(analysisLevel==4 )tasks->add( new HMdc12Fit("mdc.12fit","mdc.12fit",fitNTuple,fitPrint,fitVersion) );
  }
  else if(tracker==2 && analysisLevel>=3) {

     HLocation aLoc;
     aLoc.set(2,0,0);
     HMdcHitF *finder=0;
     Text_t buffer[255];

     // Get modules from "Mdc" detector, needed to make tasks for
     // individual chamber hit finders
     Int_t *mods = gHades->getSetup()->getDetector("Mdc")->getModules();
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
    TString sraw="raw";
    TString scal1="cal1";
    TString scal2="cal2";
    TString shit="hit";
    TString sfit="fit";

    // switch between real data or simulation
    TString ssimulation="simulation";
    TString sreal="real";

    //  Calibrater1 options
    TString snostartandnocal="nostartandnocal";
    TString snostartandcal="nostartandcal";
    TString sstartandcal="startandcal";
    TString stimecuts="timecuts";
    TString snotimecuts="notimecuts";

    // switch trackers
    TString sdubna="dubna";
    TString ssantiago="santiago";

    // Dubna tracker options
    TString smagnetoff="magnetoff";
    TString smagneton="magneton";

    TString scombinedclusters="combinedclusters";
    TString schamberclusters="chamberclusters";
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
		    Error("HMdcTaskSet:","\n overlap with other argument for analysis level:RAW!");
		    cout<<"INPUT OPTIONSTRING: "<<s2<<endl;
		    printOptions();
		    exit(1);
		}
		analysisLevel=0;
	    }
	    else if(argument.CompareTo(scal1)==0)
	    {
		ilevel++;
		if(ilevel>1)
		{
		    Error("HMdcTaskSet:","\n overlap with other argument for analysis level:CAL1!");
		    cout<<"INPUT OPTIONSTRING: "<<s2<<endl;
		    printOptions();
		    exit(1);
		}
		analysisLevel=1;
	    }
	    else if(argument.CompareTo(scal2)==0)
	    {
		ilevel++;
		if(ilevel>1)
		{
		    Error("HMdcTaskSet:","\n overlap with other argument for analysis level:CAL2!");
		    cout<<"INPUT OPTIONSTRING: "<<s2<<endl;
		    printOptions();
		    exit(1);
		}
		analysisLevel=2;
	    }
	    else if(argument.CompareTo(shit)==0)
	    {
		ilevel++;
		if(ilevel>1)
		{
		    Error("HMdcTaskSet:","\n overlap with other argument for analysis level:HIT!");
		    cout<<"INPUT OPTIONSTRING: "<<s2<<endl;
		    printOptions();
		    exit(1);
		}
		analysisLevel=3;
	    }
	    else if(argument.CompareTo(sfit)==0)
	    {
		ilevel++;
		if(ilevel>1)
		{
		    Error("HMdcTaskSet:","\n overlap with other argument for analysis level:FIT!");
		    cout<<"INPUT OPTIONSTRING: "<<s2<<endl;
		    printOptions();
		    exit(1);
		}
		analysisLevel=4;
	    }
	    else if(argument.CompareTo(ssimulation)==0)
	    {
		irealorsimulation++;
		if(irealorsimulation>1)
		{
		    Error("HMdcTaskSet:","\n overlap with other argument for data type!:SIMULATION");
		    cout<<"INPUT OPTIONSTRING: "<<s2<<endl;
		    printOptions();
		    exit(1);
		}
		isSimulation=kTRUE;
	    }
	    else if(argument.CompareTo(sreal)==0)
	    {
		irealorsimulation++;
		if(irealorsimulation>1)
		{
		    Error("HMdcTaskSet:","\n overlap with other argument for data type:REAL!");
		    cout<<"INPUT OPTIONSTRING: "<<s2<<endl;
		    printOptions();
		    exit(1);
		}
		isSimulation=kFALSE;
	    }
	    else if(argument.CompareTo(snostartandnocal)==0)
	    {
		icalibrater++;
		if(icalibrater>1)
		{
		    Error("HMdcTaskSet:","\n overlap with other argument for calibrater option:NoStartAndNoCal!");
		    cout<<"INPUT OPTIONSTRING: "<<s2<<endl;
		    printOptions();
		    exit(1);
		}
		cal1ModeFlag=3;
	    }
	    else if(argument.CompareTo(snostartandcal)==0)
	    {
		icalibrater++;
		if(icalibrater>1)
		{
		    Error("HMdcTaskSet:","\n overlap with other argument for calibrater option:NoStartAndCal!");
		    cout<<"INPUT OPTIONSTRING: "<<s2<<endl;
		    printOptions();
		    exit(1);
		}
		cal1ModeFlag=1;
	    }
	    else if(argument.CompareTo(sstartandcal)==0)
	    {
		icalibrater++;
		if(icalibrater>1)
		{
		    Error("HMdcTaskSet:","\n overlap with other argument for calibrater option:StartAndCal!");
		    cout<<"INPUT OPTIONSTRING: "<<s2<<endl;
		    printOptions();
		    exit(1);
		}
		cal1ModeFlag=2;
	    }
	    else if(argument.CompareTo(snotimecuts)==0)
	    {
		itimecuts++;
		if(itimecuts>1)
		{
		    Error("HMdcTaskSet:","\n overlap with other argument for time cuts:NoTimeCuts!");
		    cout<<"INPUT OPTIONSTRING: "<<s2<<endl;
		    printOptions();
		    exit(1);
		}
		cal1TimeCutFlag=0;
	    }
	    else if(argument.CompareTo(stimecuts)==0)
	    {
		itimecuts++;
		if(itimecuts>1)
		{
		    Error("HMdcTaskSet:","\n overlap with other argument for time cuts:TimeCuts!");
		    cout<<"INPUT OPTIONSTRING: "<<s2<<endl;
		    printOptions();
		    exit(1);
		}
		cal1TimeCutFlag=1;
	    }
	    else if(argument.CompareTo(sdubna)==0)
	    {
		itracker++;
		if(itracker>1)
		{
		    Error("HMdcTaskSet:","\n overlap with other argument for tracker type:DUBNA!");
		    cout<<"INPUT OPTIONSTRING: "<<s2<<endl;
		    printOptions();
		    exit(1);
		}
		tracker=1;
	    }
	    else if(argument.CompareTo(ssantiago)==0)
	    {
		itracker++;
		if(itracker>1)
		{
		    Error("HMdcTaskSet:","\n overlap with other argument for tracker type:SANTIAGO!");
		    cout<<"INPUT OPTIONSTRING: "<<s2<<endl;
		    printOptions();
		    exit(1);
		}
		tracker=2;
	    }
	    else if(argument.CompareTo(smagnetoff)==0)
	    {
		imagnet++;
		if(imagnet>1)
		{
		    Error("HMdcTaskSet:","\n overlap with other argument for magnet status:MagnetOff!");
		    cout<<"INPUT OPTIONSTRING: "<<s2<<endl;
		    printOptions();
		    exit(1);
		}
		isCoilOff=kTRUE;
	    }
	    else if(argument.CompareTo(smagneton)==0)
	    {
		imagnet++;
		if(imagnet>1)
		{
		    Error("HMdcTaskSet:","\n overlap with other argument for magnet status:MagnetOn!");
		    cout<<"INPUT OPTIONSTRING: "<<s2<<endl;
		    printOptions();
		    exit(1);
		}
		isCoilOff=kFALSE;
	    }
	    else if(argument.CompareTo(scombinedclusters)==0)
	    {
		itrackermode++;
		if(itrackermode>1)
		{
		    Error("HMdcTaskSet:","\n overlap with other argument for tracker mode:CombinedChamberClusters!");
		    cout<<"INPUT OPTIONSTRING: "<<s2<<endl;
		    printOptions();
		    exit(1);
		}
		typeClustFinder=0;
	    }
	    else if(argument.CompareTo(schamberclusters)==0)
	    {
		itrackermode++;
		if(itrackermode>1)
		{
		    Error("HMdcTaskSet:","\n overlap with other argument for tracker mode:ChamberClusters!");
		    cout<<"INPUT OPTIONSTRING: "<<s2<<endl;
		    printOptions();
		    exit(1);
		}
		typeClustFinder=1;
	    }
	    else
	    {
		Error("HMdcTaskSet:","\n unknown argument %s !",argument.Data());
		cout<<"INPUT OPTIONSTRING: "<<s2<<endl;
		printOptions();
		exit(1);
	    }

	}
    }
}
void HMdcTaskSet::printOptions()
{
    // Prints the options of HMdcTaskSet

    printf("*********************** HMdcTaskSet *******************************\n");
    printf("* OPTIONS :                                                       *\n");
    printf("* ----------------------------------------------------------------*\n");
    printf("* real  (default) or simulation  -- real data or simulation data  *\n");
    printf("* dubna (default) or santiago    -- selects hit finder            *\n");
    printf("*                                                                 *\n");
    printf("* Maximum data level                                              *\n");
    printf("*     raw                        -- only unpacker                 *\n");
    printf("*     cal1                       -- up to calibrater1             *\n");
    printf("*     cal2                       -- up to calibrater2             *\n");
    printf("*     hit              (default) -- up to hit finder              *\n");
    printf("*     fit                        -- up to track fitter            *\n");
    printf("*                                                                 *\n");
    printf("* HMdcCalibrater1:                                                *\n");
    printf("*     NoStartAndNoCal            -- do not use start,             *\n");
    printf("*                                   do not use calibration        *\n");
    printf("*     NoStartAndCal              -- do not use start,             *\n");
    printf("*                                   use calibration               *\n");
    printf("*     StartAndCal      (default) -- use start,                    *\n");
    printf("*                                   use calibration               *\n");
    printf("*     NoTimeCuts                 -- do not use time cuts          *\n");
    printf("*     TimeCuts         (default) -- use time cuts                 *\n");
    printf("*                                                                 *\n");
    printf("* DUBNA TRACKER:                                                  *\n");
    printf("*     MagnetOn         (default) -- Magnetic field                *\n");
    printf("*     MagnetOff                  -- Magnetic field off            *\n");
    printf("*     CombinedClusters (default) -- Cluster search in             *\n");
    printf("*                                   combined chambers             *\n");
    printf("*     ChamberClusters            -- Cluster search in             *\n");
    printf("*                                   individual chambers           *\n");
    printf("*******************************************************************\n");
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
void HMdcTaskSet::setCalibrater1(Int_t mode, Int_t cut)
{
    // Sets the options of the HMdcCalibrater1
    setCal1Mode(mode);
    setCal1TimeCuts(cut);
}
void HMdcTaskSet::setDubnaFitter(Bool_t ntuple,Bool_t print,Int_t vers)
{
    // Set the options of the Dubna fitter:
    // fill NTuple,print debug information, 0 for old or 1 for new version

    fitNTuple=ntuple;
    fitPrint=print;
    if(vers==0 || vers==1)
    {
	fitVersion=vers;
    }
    else
    {
	Error("HMdcTaskSet:setDubnaFitter(Bool_t,Bool_t,Int_t):"
	      ,"\n unknown argument :use \n 0 for old version \n 1 for new version");
        exit(1);
    }
}
void HMdcTaskSet::setRealOrSim(Option_t *optrealorsim="")
{
    // Sets option for real data or simulation: real,simulation
    TString optRealOrSim=optrealorsim;

    optRealOrSim.ToLower();
    if(optRealOrSim.CompareTo("real")==0)
    {
	isSimulation=kFALSE;
    }
    else if(optRealOrSim.CompareTo("simulation")==0)
    {
	isSimulation=kTRUE;
    }
    else
    {
	Error("HMdcTaskSet:setRealOrSim():"
	      ,"\n unknown argument :use real or simulation");
	printOptions();
        exit(1);
    }
}
void HMdcTaskSet::setAnalysisLevel(Option_t *optanalysislevel="")
{
    // Sets maximum analyse level:raw,cal1,cal2,hit or fit
    TString optAnalysisLevel=optanalysislevel;

    optAnalysisLevel.ToLower();

    if(optAnalysisLevel.CompareTo("raw")==0)
    {
	analysisLevel=0;
    }
    else if(optAnalysisLevel.CompareTo("cal1")==0)
    {
	analysisLevel=1;
    }
    else if(optAnalysisLevel.CompareTo("cal2")==0)
    {
	analysisLevel=2;
    }
    else if(optAnalysisLevel.CompareTo("hit")==0)
    {
	analysisLevel=3;
    }
    else if(optAnalysisLevel.CompareTo("fit")==0)
    {
	analysisLevel=4;
    }
    else
    {
	Error("HMdcTaskSet:setAnalysisLevel(Int_t):"
	      ,"\n unknown argument: use raw,cal1,cal2,hit or fit");
	printOptions();
        exit(1);
    }
}
void HMdcTaskSet::setCal1TimeCuts(Int_t cut)
{
    // Sets the for the time cuts in HMdcCalibrater1:
    // 0 for no time cuts, 1 for time cuts
    if(cut==0 || cut==1)
    {
	cal1TimeCutFlag=cut;
    }
    else
    {
	Error("HMdcTaskSet:setCal1TimeCuts(Int_t):"
	      ,"\n unknown argument: use \n 0 for no time cuts \n 1 for time cuts");
	exit(1);
    }
}
void HMdcTaskSet::setCal1Mode(Int_t mode)
{
    // Sets the option for the the mode of HMdcCalibrater1:
    // 1 for nostartandcal, 2 for startandcal, 3 for nostartandnocal
    if(mode==1 || mode==2 || mode==3)
    {
        cal1ModeFlag=mode;
    }
    else
    {
	Error("HMdcTaskSet:setCal1Mode(Int_t):"
	      ,"\n unknown argument: use \n 1 for nostartandcal,\n 2 for startandcal,\n 3 for nostartandnocal,\n ");
	exit(1);
    }
}
void HMdcTaskSet::setDubnaHitFinderMode(Int_t type,Bool_t field )
{
    // Sets option for the mode of the Dubna hitfinder and
    // magnet on and off configuration :
    // 0 for combined chamber,1 for single chamber,
    // kTRUE=magnet off,kTRUE magnet on
    if(type==0 || type==1)
    {
	typeClustFinder=type;
    }
    else
    {
	Error("HMdcTaskSet:setDubnaHitFinderMode(Int_t,Bool_t):"
	      ,"\n unknown argument: use \n 0 for combined chamber \n 1 for single chamber");
        exit(1);
    }
    isCoilOff = field;
}
void HMdcTaskSet::selectTracker(Option_t *opttracker)
{
    // Select Tracker type: dubna, santiago
    TString  optTracker=opttracker;
    optTracker.ToLower();
    if(optTracker.CompareTo("dubna")==0)
    {
	tracker=1;
    }
    else if (optTracker.CompareTo("santiago")==0)
    {
	tracker=2;
    }
    else
    {
	Error("HMdcTaskSet:selectTracker():"
	      ,"\n unknown argument: use dubna or santiago");
	printOptions();
	exit(1);
    }
}
void HMdcTaskSet::printStatus()
{
    // Prints the status of the configuration of
    // HMdcTaskSet.

    TString myisrealorsim;
    if(isSimulation==kFALSE)
    {
        myisrealorsim="REAL";
    }
    else if(isSimulation==kTRUE)
    {
	myisrealorsim="SIMULATION";
    }

    TString mylevel;
    if(analysisLevel==0)
    {
	mylevel="RAW";
    }
    else if(analysisLevel==1)
    {
	mylevel="CAL1";
    }
    else if(analysisLevel==2)
    {
	mylevel="CAL2";
      }
    else if(analysisLevel==3)
    {
	mylevel="HIT";
    }
    else if(analysisLevel==4)
    {
	mylevel="FIT";
    }

    TString mycal1timecut;
    if(cal1TimeCutFlag==0)
    {
	mycal1timecut="NoTimeCuts";
    }
    else if(cal1TimeCutFlag==1)
    {
	mycal1timecut="TimeCuts";
    }

    TString mycal1mode;
    if(cal1ModeFlag==1)
    {
	mycal1mode="NoStartAndCal";
    }
    else if(cal1ModeFlag==2)
    {
	mycal1mode="StartAndCal";
    }
    else if(cal1ModeFlag==3)
    {
	mycal1mode="NoStartAndNoCal";
    }

    TString mytracker;
    if(tracker==1)
    {
	mytracker="DUBNA";
    }
    else if(tracker==2)
    {
	mytracker="SANTIAGO";
    }

    TString myfield;
    if(isCoilOff==kFALSE)
    {
	myfield="MagnetOn";
    }
    else if(isCoilOff==kTRUE)
    {
	myfield="MagnetOff";
    }

    TString myclusterfinder;
    if(typeClustFinder==0)
    {
	myclusterfinder="COMBINED CHAMBER CLUSTER";
    }
    else if(typeClustFinder==1)
    {
	myclusterfinder="SINGLE CHAMBER CLUSTER";
    }

    TString myfittuple;
    if(fitNTuple==kFALSE)
    {
	myfittuple="kFALSE";
    }
    else if(fitNTuple==kTRUE)
    {
	myfittuple="kTRUE";
    }
    TString myfitprint;
    if(fitPrint==kFALSE)
    {
	myfitprint="kFALSE";
    }
    else if(fitPrint==kTRUE)
    {
	myfitprint="kTRUE";
    }
    printOptions();
    printf("* ACTUAL CONFIGURATION :                                          *\n");
    printf("* ----------------------------------------------------------------*\n");
    printf("*     Task list is set to %s DATA \n",myisrealorsim.Data());
    printf("*     Task list defined up to %s  \n",mylevel.Data());
    printf("*     HMdcCalibrater1 is set to %s \n",mycal1mode.Data());
    printf("*     HMdcCalibrater1 is set to %s \n",mycal1timecut.Data());
    printf("*     %s tracker is selected      \n",mytracker.Data());
    if(tracker==1)
    {
	printf("*     hit finder is set to %s mode.      \n",myclusterfinder.Data());
	printf("*     hit finder is set to %s mode.      \n",myfield.Data());
        printf("*     FITTER is set to (%s ,%s ,%i) mode.\n",myfittuple.Data(),myfitprint.Data(),fitVersion);


    }
    printf("*******************************************************************\n");

}


ClassImp(HMdcTaskSet)
