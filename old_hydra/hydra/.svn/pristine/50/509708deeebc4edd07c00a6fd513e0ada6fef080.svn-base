

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
#include "TROOT.h"
#include "TClass.h"
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
#include "hpidalgmdceloss.h"
#include "hpidefficiencycalculation.h"

#include "TBrowser.h"
#include "TString.h"
#include "TObjString.h"
#include <iostream> 
#include <iomanip>

HPidTaskSet::HPidTaskSet(const Text_t name[],const Text_t title[]) : HTaskSet(name,title) {
  // Constructor
}


HPidTaskSet::HPidTaskSet(void) : HTaskSet() {
  // Default constructor
}


HPidTaskSet::~HPidTaskSet(void) {
  // Destructor.
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

  Short_t particles[7]={2,3,8,9,14};
  pReconstructor->setParticleIds(particles,7);
  
  
  TString particlefillerOpts(trackingmethod);
  if(eventType.Contains(simulation))
    {
      particlefillerOpts.Append(",MAKESIMCATEGORY");
    }
  else
    {
      //pReconstructor->addAlgorithm(new HPidAlgMdcEloss());
    }
  HPidParticleFiller* pParticleFiller = new HPidParticleFiller(particlefillerOpts.Data());
  HPidPerformaceTest* pPerformaceTest = new HPidPerformaceTest(outputfile);
  HTaskSet *tasks = new HTaskSet("Pid","List of Pid tasks");
  tasks->add(pReconstructor);
  tasks->add(pParticleFiller);
  //tasks->add(pPerformaceTest);
  return tasks;
}


//This option returns a traskset that expects PidTrackCandidates and build PidParticles from them
//further a bias on the hadron yields can be applied
HTask* HPidTaskSet::makePerformanceTestTasks(const Char_t* dataType,const Char_t* outputfile,const Char_t* trackingmethod, Float_t HadronBias)
{
  TString simulation="simulation";
  TString real ="real";
  
  TString eventType=dataType;
  eventType.ToLower();

  TString reconstructorOpts("PDF,CL,RELINTS,");
  reconstructorOpts.Append(trackingmethod);
  HPidReconstructor *pReconstructor =new HPidReconstructor(reconstructorOpts.Data(),HadronBias);  
  pReconstructor->addAlgorithm(new HPidAlgRingProperties());
  pReconstructor->addAlgorithm(new HPidAlgShower());
  pReconstructor->addAlgorithm(new HPidAlgMomVsBeta());

  Short_t particles[7]={2,3,8,9,14};
  pReconstructor->setParticleIds(particles,7);
  
  
  TString particlefillerOpts(trackingmethod);
  if(eventType.Contains(simulation))
    {
      particlefillerOpts.Append(",MAKESIMCATEGORY");
    }
  else
    {
      //pReconstructor->addAlgorithm(new HPidAlgMdcEloss());
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

  Short_t particles[7]={2,3,8,9,14};
  pReconstructor->setParticleIds(particles,7);
  
  
  TString particlefillerOpts(trackingmethod);
  if(eventType.Contains(simulation))
    {
      particlefillerOpts.Append(",MAKESIMCATEGORY");
    }
  else
    {
      //pReconstructor->addAlgorithm(new HPidAlgMdcEloss());
    }
  HPidParticleFiller* pParticleFiller = new HPidParticleFiller(particlefillerOpts.Data());
  //HPidPerformaceTest* pPerformaceTest = new HPidPerformaceTest(outputfile);
  HTaskSet *tasks = new HTaskSet("Pid","List of Pid tasks");

  if(eventType.Contains(simulation))
    {
      tasks->add(new HPidTrackFiller("pidtrackfiller","pidtrackfiller","NOCHI2SEG1,MAKESIMCATEGORY"));
    }
  else
    {
      tasks->add(new HPidTrackFiller("pidtrackfiller","pidtrackfiller","NOCHI2SEG1"));
    }


  tasks->add(pReconstructor);
  tasks->add(pParticleFiller);
  //tasks->add(pPerformaceTest);
  return tasks;
}


//This option returns a traskset that expects PidTrackCandidates and build PidParticles from them
HTask* HPidTaskSet::makeHardcutIdTasksFromScratch(const Char_t* dataType,const Char_t* outputfile,const Char_t* trackingmethod,const Char_t *targetfile,Option_t* opt_standcutsalgorithms)
{
  TString simulation="simulation";
  TString real ="real";
  
  TString eventType=dataType;
  eventType.ToLower();

  TString reconstructorOpts("PDF,CL,RELINTS,");
  reconstructorOpts.Append(trackingmethod);
  HPidReconstructor *pReconstructor =new HPidReconstructor(reconstructorOpts.Data());
  pReconstructor->addAlgorithm(new HPidAlgStandCuts(1.0,targetfile,opt_standcutsalgorithms));


  Short_t particles[4]={-1,-2,2,3};
  pReconstructor->setParticleIds(particles,4);
  
  
  TString particlefillerOpts(trackingmethod);

  //Better for hard cut analysis!
  particlefillerOpts.Append(",ALGLIKELIHOOD");

  if(eventType.Contains(simulation))
    {
      particlefillerOpts.Append(",MAKESIMCATEGORY");
    }

  HPidParticleFiller* pParticleFiller = new HPidParticleFiller(particlefillerOpts.Data());
  //HPidPerformaceTest* pPerformaceTest = new HPidPerformaceTest(outputfile);
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
  //tasks->add(pPerformaceTest);
  return tasks;
}



//This option returns a traskset that expects PidTrackCandidates and build PidParticles from them
HTask* HPidTaskSet::makeHardcutIdTasks(const Char_t* dataType,const Char_t* outputfile,const Char_t* trackingmethod,const Char_t *targetfile,Option_t* opt_standcutsalgorithms)
{
  TString simulation="simulation";
  TString real ="real";
  
  TString eventType=dataType;
  eventType.ToLower();

  TString reconstructorOpts("PDF,CL,RELINTS,");
  reconstructorOpts.Append(trackingmethod);
  HPidReconstructor *pReconstructor =new HPidReconstructor(reconstructorOpts.Data());
  pReconstructor->addAlgorithm(new HPidAlgStandCuts(1.0,targetfile,opt_standcutsalgorithms));


  Short_t particles[4]={-1,-2,2,3};
  pReconstructor->setParticleIds(particles,4);
  
  
  TString particlefillerOpts(trackingmethod);
  particlefillerOpts.Append(",ALGLIKELIHOOD");
  if(eventType.Contains(simulation))
    {
      particlefillerOpts.Append(",MAKESIMCATEGORY");
    }

  HPidParticleFiller* pParticleFiller = new HPidParticleFiller(particlefillerOpts.Data());
  //HPidPerformaceTest* pPerformaceTest = new HPidPerformaceTest(outputfile);
  HTaskSet *tasks = new HTaskSet("Pid","List of Pid tasks");
  tasks->add(pReconstructor);
  tasks->add(pParticleFiller);
  //tasks->add(pPerformaceTest);
  return tasks;
}











ClassImp(HPidTaskSet)


