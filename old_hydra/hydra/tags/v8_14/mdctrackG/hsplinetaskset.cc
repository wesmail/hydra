//*-- Author : A.Rustamov

//Tsk set to All momentum Reconstruction methods
//The arguments:
//metamatching is connected by default!!!!
//spline -conect spline  task
//docluser - connect Tof clustering

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
#include "hmdcdedxmaker.h"
#include "hmdcvertexfind.h"
#include "hmdchitf.h"
#include "hreconstructor.h"
#include "hmdcsegmentf.h"
#include "hmdcsetup.h"
#include "hruntimedb.h"
#include "hrun.h"
#include "hdatasource.h"
#include "hlocation.h"
#include "hmessagemgr.h"
#include "hsplinetrackF.h"
#include "hmetamatchF.h"
#include "htofcluster.h"
#include "hsplinetaskset.h"
#include "hsplinetofclF.h"
#include <TBrowser.h>
#include "hrktrackBF.h"
#include "hkicktrackfb.h"
#include "hkicktrackfbsim.h"
//#include "hmdctrackgtofclF.h"

using namespace std;

Bool_t HSplineTaskSet::isSpline=kFALSE;
Bool_t HSplineTaskSet::isMetaMatching=kFALSE;

Bool_t HSplineTaskSet::isSimulation=kFALSE;
Bool_t HSplineTaskSet::isClustering=kFALSE;

Bool_t HSplineTaskSet::isKickB=kFALSE;
Bool_t HSplineTaskSet::isAlsoOld=kFALSE;
Bool_t HSplineTaskSet::isDebug=kFALSE;
Bool_t HSplineTaskSet::isKickCluster=kFALSE;
Bool_t HSplineTaskSet::isControlHists=kFALSE;
Bool_t HSplineTaskSet::isMultipleTarget=kFALSE;
Bool_t HSplineTaskSet::isRunge=kFALSE;



HMetaMatchF*    HSplineTaskSet::pMetaMatchF=NULL;
HSplineTrackF*  HSplineTaskSet::pSplineTrackF=NULL;
HSplineTofClF*  HSplineTaskSet::pSplineTofClF=NULL;
HKickTrackFB*   HSplineTaskSet::pKickB=NULL;
HRKTrackBF*     HSplineTaskSet::pRunge=NULL;

//HTofClusterf*   HSplineTaskSet::pTofClusterF=0;

HSplineTaskSet::HSplineTaskSet(){;}
HSplineTaskSet::HSplineTaskSet(Text_t name[],Text_t title[]):HTaskSet(name,title){;}
HSplineTaskSet::~HSplineTaskSet()
{ 
 if( pMetaMatchF)
{ 
  delete pMetaMatchF;
  pMetaMatchF=0;
}
if (pSplineTrackF)
{
  delete pSplineTrackF;
  pSplineTrackF=0;
}
//if(pTofClusterF)
//{
//  delete pTofClusterF;
//  ptofClusterF=0;

//}
}

HTask* HSplineTaskSet::make(const char* select,Option_t *options)
{
  HTaskSet *tasks=new HTaskSet("Spline","Spline Task Sets");
  TString opt=options;
  parseArguments(opt);
  pMetaMatchF=new HMetaMatchF("MetaMAtching","MetaMAtching");
  tasks->add(pMetaMatchF);
  //if(isSimulation)
//    {

      if(isKickB)
      {
      if(isSimulation)
      { 
      pKickB=new HKickTrackFBSim("kick.trackfBsim","kick.trackfBsim");
      }
      else
      {
      pKickB=new HKickTrackFB("kick.trackfB","kick.trackfB");
      }
      if(isAlsoOld) pKickB->fillOldKickTrack();
      if(isDebug)   pKickB->setDebug(kTRUE);
      if(isKickCluster) pKickB->setTofClusterMode(kTRUE);
      if(isControlHists) pKickB->enableControlHistograms();
      if(isMultipleTarget)  pKickB->setMultipleTarget(kTRUE);
      else pKickB->setMultipleTarget(kFALSE); 
      
      tasks->add(pKickB);
      
      }


      if(isSpline)
        {
	  pSplineTrackF=new HSplineTrackF("SplineTrack","SplineTrack");
          tasks->add(pSplineTrackF);
	}
	
      if(isRunge)
      {
      pRunge=new HRKTrackBF("RK-tracking", 2);
      tasks->add(pRunge);
      } 	
	
   	
      if(isClustering)
      {
      pSplineTofClF=new HSplineTofClF("tofClustering","tofClustering",1);
      tasks->add(pSplineTofClF);
      }	
      
      else
      {
        pSplineTofClF=new HSplineTofClF("tofClustering","tofClustering",0);
        tasks->add(pSplineTofClF);
      }
      
      
//    }

  return tasks;
}

void HSplineTaskSet::parseArguments(TString a)
{
  char* mystring=(char*)a.Data();
  char* buffer;
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
  while ((stemp=(TObjString*)myiter->Next())!= 0) {
    argument=stemp->GetString();
    if(argument.CompareTo("spline")==0)
	    {
	    isSpline=kTRUE;
	    }
    if(argument.CompareTo("doClustering")==0)
                {
		isClustering=kTRUE;
		}
    if(argument.CompareTo("reslowb")==0)
    {
    isKickB=kTRUE;
    } 
    if(argument.CompareTo("simulation")==0)
    {
    isSimulation=kTRUE;
    } 
     		
    if(argument.CompareTo("&old")==0)
    {
    isAlsoOld=kTRUE;
    } 
    
    if(argument.CompareTo("debug")==0)
    {
    isDebug=kTRUE;
    } 
    
    if(argument.CompareTo("tofclust")==0)
    {
    isKickCluster=kTRUE;
    } 
    
    if(argument.CompareTo("controlhists")==0)
    {
    isControlHists=kTRUE;
    } 
    
    if(argument.CompareTo("multipletarget")==0)
    {
    isMultipleTarget=kTRUE;
    } 
     
    if(argument.CompareTo("runge")==0)
    {
    isRunge=kTRUE;
    } 
  }
  delete myiter;
}

  ClassImp(HSplineTaskSet)
  
  
  
