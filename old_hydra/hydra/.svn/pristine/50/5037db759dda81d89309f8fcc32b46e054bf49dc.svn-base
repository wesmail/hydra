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
//#include "hmdctrackgtofclF.h"

using namespace std;

Bool_t HSplineTaskSet::isSpline=kFALSE;
Bool_t HSplineTaskSet::isMetaMatching=kFALSE;
//Bool_t HSplineTaskSet::isSimulation=kFALSE;
Bool_t HSplineTaskSet::isClustering=kFALSE;

HMetaMatchF*    HSplineTaskSet::pMetaMatchF=0;
HSplineTrackF*  HSplineTaskSet::pSplineTrackF=0;
HSplineTofClF*  HSplineTaskSet::pSplineTofClF=0;
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
      if(isSpline)
        {
	  pSplineTrackF=new HSplineTrackF("SplineTrack","SplineTrack");
          tasks->add(pSplineTrackF);
	}
   	
      if(isClustering)
      {
      pSplineTofClF=new HSplineTofClF("tofClustering","tofCLustering");
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
					  	    
      
  }
}

  ClassImp(HSplineTaskSet)
  
  
  
