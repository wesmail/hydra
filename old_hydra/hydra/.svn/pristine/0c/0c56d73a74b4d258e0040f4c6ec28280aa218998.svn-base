

//*-- Author : Tassilo Christ

//_HADES_CLASS_DESCRIPTION
///////////////////////////////////////////////////////////////////////////////
// HPidTaskSet
//
// This HTaskSet contains the tasks for Pid. It configures them according to the
// option strings in the "make" function
//
///////////////////////////////////////////////////////////////////////////////

using namespace std;
#include "hpidtaskset.h"
#include "htask.h"
#include "htaskset.h"
#include "hdebug.h"
#include <TROOT.h>
#include <TClass.h>
#include "haddef.h"

#include "hades.h"
#include "hdatasource.h"
#include "hpidreconstructor.h"
#include "hpidtrackfiller.h"
#include "hpidparticlefiller.h"
#include "hpidpdfmaker.h"
#include "hpidperformacetest.h"
#include "hpidskiptrigger.h"
#include "hpidalgstandcuts.h"
#include "hpidalghardcuts.h"
#include "hpidalgringproperties.h"
#include "hpidalgshower.h"
#include "hpidalgmomvsbeta.h"

#include <TBrowser.h>
#include <TString.h>
#include <TObjString.h>
#include <iostream> 
#include <iomanip>

HPidTaskSet::HPidTaskSet(Text_t name[],Text_t title[]) : HTaskSet(name,title) {
  // Constructor
}


HPidTaskSet::HPidTaskSet(void) : HTaskSet() {
  // Default constructor
}


HPidTaskSet::~HPidTaskSet(void) {
  // Destructor.
}

HTask* HPidTaskSet::make(const Char_t *dataType, 
			 const Char_t* keyword, 
			 const Char_t* inputfile)
{
  TString simulation="simulation";
  TString real ="real";
  
  TString eventType=dataType;
  eventType.ToLower();

  TString keyString=keyword;
  keyString.ToUpper();

  if(keyString=="BEAMTIMETASKS")
    {
      return makeBeamtimeTasks(dataType, inputfile);
    }
  if(keyString=="PDFTASKS")
    {

      return makePDFCreationTasks(dataType, inputfile);
    }
  ::Error("HPidTaskSet::make()","Keyword %s is not defined",keyword);
  return NULL;
}


HTask* HPidTaskSet::makeTasks(const char *dataType, 
			      Option_t *tasklist, 
			      Option_t *opt_skiptrigger, 
			      Option_t *opt_trackfiller,
			      Option_t *opt_reconstructor, 
			      Option_t *opt_algorithms,
			      Option_t *opt_particlefiller)
{
  HTaskSet *tasks = new HTaskSet("Pid","List of Pid tasks");
  
  TString simulation="simulation";
  TString real ="real";
  
  //Sim or Exp data
  TString eventType=dataType;
  eventType.ToLower();

  //What tasks do you want to execute?
  TString sTaskList=tasklist;
  sTaskList.ToUpper();


  //skip-trigger-task configuration
  TString sOptSkipTrigger=opt_skiptrigger;
  sOptSkipTrigger.ToUpper();
  if(sOptSkipTrigger=="D")//If the user wants the defaults we make his life easier
    {
      sOptSkipTrigger=SKIPDEFAULTS;
    }

  //trackfiller task configuration
  TString sOptTrackFiller=opt_trackfiller;
  sOptTrackFiller.ToUpper();
  if(sOptTrackFiller=="D")
    {
      sOptTrackFiller=TRACKFILLERDEFAULTS;
    }

  //reconstructor task configuration
  TString sOptReconstructor=opt_reconstructor;
  sOptReconstructor.ToUpper();
  if(sOptReconstructor=="D")
    {
      sOptReconstructor=RECONSTRUCTORDEFAULTS;
    }

  //algorithm selection for reconstructor 
  TString sOptAlgorithms=opt_algorithms;
  sOptAlgorithms.ToUpper();
  if(sOptAlgorithms=="D")
    {
      sOptAlgorithms=ALGORITHMDEFAULTS;
    }
  
  //particlefiller task configuration
  TString sOptParticleFiller=opt_particlefiller;
  sOptParticleFiller.ToUpper();
  if(sOptParticleFiller=="D")
    {
      sOptParticleFiller=PARTICLEFILLERDEFAULTS;
    }

  if(eventType.Contains(simulation))
    {
      //append flag to the sensitive tasks
      sOptTrackFiller.Append(",MAKESIMCATEGORY");
      sOptParticleFiller.Append(",MAKESIMCATEGORY");
    }

  Bool_t bSkipTrigger         = (strstr(sTaskList.Data(),"SKIPTRIGGER") != NULL);
  Bool_t bMakeTrackCandidates = (strstr(sTaskList.Data(),"TRACKCANDIDATES") != NULL);
  Bool_t bMakeCandidates      = (strstr(sTaskList.Data(),"CANDIDATES") != NULL);
  Bool_t bMakeParticles       = (strstr(sTaskList.Data(),"PARTICLES") != NULL);
  
  //Algorithm selection for PID
  Bool_t bUseStandardCuts     = (strstr(sOptAlgorithms.Data(),"STANDARDCUTS") != NULL);
  Bool_t bUseBananaCuts       = (strstr(sOptAlgorithms.Data(),"BANANACUTS") != NULL);
  Bool_t bUseBayesianCuts     = (strstr(sOptAlgorithms.Data(),"BAYES") != NULL);


#warning This will not create a proper filename due to a missing implementation in HDataSource!
  TString SourceFileName = gHades->getDataSource()->getCurrentFileName();

  TString StdCutOutputFile = "OutputStdCuts.root";
  StdCutOutputFile.Append(SourceFileName);

  TString BananaCutOutputFile = "OutputBananaCuts.root";
  BananaCutOutputFile.Append(SourceFileName);

  if(bSkipTrigger)
    {
      tasks->add(new HPidSkipTrigger("skiptrigger","skiptrigger",sOptSkipTrigger.Data()));
      cout << "Building Skiptrigger with option: " << sOptSkipTrigger.Data() << endl;
    }
  if(bMakeTrackCandidates)
    {
      tasks->add(new HPidTrackFiller("pidtrackfiller","pidtrackfiller",sOptTrackFiller.Data()));
      cout << "Building PidTrackFiller with option: " <<sOptTrackFiller.Data() << endl;
    }

  if(bMakeCandidates)
    {
      HPidReconstructor* pPidRec = new HPidReconstructor("pidreconstructor","pidreconstructor",sOptReconstructor.Data());
      cout << "Building PidReconstructor with option: " <<sOptReconstructor.Data() << endl;

      if(bUseStandardCuts)
	{
	  pPidRec->addAlgorithm(new HPidAlgStandCuts(1.0, StdCutOutputFile.Data()));
	  cout << "Added alg std cuts with weight 1" << endl;
	  cout << "Output will be written to: " << StdCutOutputFile.Data() << endl;
	}
      if(bUseBananaCuts)
	{
	  pPidRec->addAlgorithm(new HPidAlgHardCuts(1.0, BananaCutOutputFile.Data()));
	  cout << "Added alg std cuts with weight 1" << endl;
	  cout << "Output will be written to: " << BananaCutOutputFile.Data() << endl;
	}
      if(bUseBayesianCuts)
	{
	  pPidRec->addAlgorithm(new HPidAlgMomVsBeta(1.0));
	  pPidRec->addAlgorithm(new HPidAlgRingProperties(1.0));
	  pPidRec->addAlgorithm(new HPidAlgShower(1.0));
	  cout << "Added all bayesian algorithms with weights 1" << endl;
	}
      tasks->add(pPidRec);
      Short_t particles[7]={-1,-2,2,3,8,9,14};
      pPidRec->setParticleIds(particles,7);
    }


  if(bMakeParticles)
    {
      tasks->add(new HPidParticleFiller("pidparticlefiller","pidparticlefiller",sOptParticleFiller.Data()));
      cout << "Building PidParticleFiller with option: " <<sOptParticleFiller.Data() << endl;
    }
  return tasks;
}




HTask* HPidTaskSet::makeBeamtimeTasks(const Char_t* dataType,const Char_t* outputfile)
{
  TString simulation="simulation";
  TString real ="real";
  
  TString eventType=dataType;
  eventType.ToLower();
  
  HTaskSet *tasks = new HTaskSet("Pid","List of Pid tasks");
  /*
  TString outputfile("pdf-data-");
  outputfile.Append(inputfile);
  outputfile.Append(".root");
  */
  if(eventType.Contains(simulation))
    {
      tasks->add(new HPidTrackFiller("pidtrackfiller","pidtrackfiller","NOCHI2SEG1,SKIPNOLEPTON,MAKESIMCATEGORY"));
      tasks->add(new HPidPdfMaker(outputfile,kTRUE));
    }
  else
    {
      tasks->add(new HPidTrackFiller("pidtrackfiller","pidtrackfiller","NOCHI2SEG1,SKIPNOLEPTON"));
      tasks->add(new HPidPdfMaker(outputfile,kFALSE));
    }
  return tasks;
}


HTask* HPidTaskSet::makePDFCreationTasks(const Char_t* dataType,const Char_t* outputfile)
{
  TString simulation="simulation";
  TString real ="real";

  TString eventType=dataType;
  eventType.ToLower();
  
  HTaskSet *tasks = new HTaskSet("Pid","List of Pid tasks");

  if(eventType.Contains(simulation))
    {
      tasks->add(new HPidTrackFiller("pidtrackfiller","pidtrackfiller","NOCHI2SEG1,MAKESIMCATEGORY"));
      tasks->add(new HPidPdfMaker(outputfile,kTRUE));
    }
  else
    {
      tasks->add(new HPidTrackFiller("pidtrackfiller","pidtrackfiller","NOCHI2SEG1"));
      tasks->add(new HPidPdfMaker(outputfile,kFALSE));
    }
  return tasks;
}


//This option returns a traskset that expects PidTrackCandidates and build PidParticles from them
HTask* HPidTaskSet::makeBayesianIdTasks(const Char_t* dataType,const Char_t* outputfile,const Char_t* trackingmethod)
{
  TString simulation="simulation";
  TString real ="real";
  
  TString eventType=dataType;
  eventType.ToLower();

  TString reconstructorOpts("PDF,CL,RELINTS,");
  reconstructorOpts.Append(trackingmethod);
  HPidReconstructor *pReconstructor =new HPidReconstructor(reconstructorOpts.Data());
  pReconstructor->addAlgorithm(new HPidAlgRingProperties());
  pReconstructor->addAlgorithm(new HPidAlgShower());
  pReconstructor->addAlgorithm(new HPidAlgMomVsBeta());

  Short_t particles[7]={-1,-2,2,3,8,9,14};
  pReconstructor->setParticleIds(particles,7);
  
  
  TString particlefillerOpts(trackingmethod);
  if(eventType.Contains(simulation))
    {
      particlefillerOpts.Append(",MAKESIMCATEGORY");
    }

  HPidParticleFiller* pParticleFiller = new HPidParticleFiller(particlefillerOpts.Data());
  HPidPerformaceTest* pPerformaceTest = new HPidPerformaceTest(outputfile);
  HTaskSet *tasks = new HTaskSet("Pid","List of Pid tasks");
  tasks->add(pReconstructor);
  tasks->add(pParticleFiller);
  tasks->add(pPerformaceTest);
  return tasks;
}


HTask* HPidTaskSet::makeBayesianIdTasksFromScratch(const Char_t* dataType,const Char_t* outputfile,const Char_t* trackingmethod)
{
  TString simulation="simulation";
  TString real ="real";
  
  TString eventType=dataType;
  eventType.ToLower();

  
  TString reconstructorOpts("PDF,CL,RELINTS,");
  reconstructorOpts.Append(trackingmethod);
  HPidReconstructor *pReconstructor =new HPidReconstructor(reconstructorOpts.Data());
  pReconstructor->addAlgorithm(new HPidAlgRingProperties());
  pReconstructor->addAlgorithm(new HPidAlgShower());
  pReconstructor->addAlgorithm(new HPidAlgMomVsBeta());

  Short_t particles[7]={-1,-2,2,3,8,9,14};
  pReconstructor->setParticleIds(particles,7);
  
  
  TString particlefillerOpts(trackingmethod);
  if(eventType.Contains(simulation))
    {
      particlefillerOpts.Append(",MAKESIMCATEGORY");
    }

  HPidParticleFiller* pParticleFiller = new HPidParticleFiller(particlefillerOpts.Data());
  HPidPerformaceTest* pPerformaceTest = new HPidPerformaceTest(outputfile);
  HTaskSet *tasks = new HTaskSet("Pid","List of Pid tasks");

  if(eventType.Contains(simulation))
    {
      tasks->add(new HPidTrackFiller("pidtrackfiller","pidtrackfiller","NOCHI2SEG1,SKIPNOLEPTON,MAKESIMCATEGORY"));
    }
  else
    {
      tasks->add(new HPidTrackFiller("pidtrackfiller","pidtrackfiller","NOCHI2SEG1,SKIPNOLEPTON"));
    }


  tasks->add(pReconstructor);
  tasks->add(pParticleFiller);
  tasks->add(pPerformaceTest);
  return tasks;
}



//This option returns a traskset that expects PidTrackCandidates and build PidParticles from them
HTask* HPidTaskSet::makeHardcutIdTasks(const Char_t* dataType,const Char_t* outputfile,const Char_t* trackingmethod,const Char_t *targetfile)
{
  TString simulation="simulation";
  TString real ="real";
  
  TString eventType=dataType;
  eventType.ToLower();

  TString reconstructorOpts("PDF,CL,RELINTS,");
  reconstructorOpts.Append(trackingmethod);
  HPidReconstructor *pReconstructor =new HPidReconstructor(reconstructorOpts.Data());
  pReconstructor->addAlgorithm(new HPidAlgStandCuts(targetfile));


  Short_t particles[4]={-1,-2,2,3};
  pReconstructor->setParticleIds(particles,5);
  
  
  TString particlefillerOpts(trackingmethod);
  if(eventType.Contains(simulation))
    {
      particlefillerOpts.Append(",MAKESIMCATEGORY");
    }

  HPidParticleFiller* pParticleFiller = new HPidParticleFiller(particlefillerOpts.Data());
  HPidPerformaceTest* pPerformaceTest = new HPidPerformaceTest(outputfile);
  HTaskSet *tasks = new HTaskSet("Pid","List of Pid tasks");
  tasks->add(pReconstructor);
  tasks->add(pParticleFiller);
  tasks->add(pPerformaceTest);
  return tasks;
}











ClassImp(HPidTaskSet)


