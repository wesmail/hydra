//*-- Author : Dan Magestro
//*-- Modified: 19/11/2001  by Dan Magestro
//*-- Modified: 14/02/2002  by Jacek Otwinowski 
//*-- Modified: 19/04/2005  by Jacek Otwinowski 
//
//_HADES_CLASS_DESCRIPTION 
///////////////////////////////////////////////////////////////////////////////
//
// HShowerTofinoTaskSet
//
// This HTaskSet contains the tasks for the Shower/Tofino detector.
// It is not used at the moment
//
///////////////////////////////////////////////////////////////////////////////

using namespace std;
#include "hshowertofinotaskset.h"
#include "htask.h"
#include "htaskset.h"
#include "hdebug.h"
#include <TROOT.h>
#include <TClass.h>
#include "haddef.h"
#include "hshowertofpidfinder.h"
#include "hshowertofpidtrackmatcher.h"
#include <TBrowser.h>
#include <TString.h>
#include <TObjString.h>
#include <iostream> 
#include <iomanip>
#include <stdlib.h>

int HShowerTofinoTaskSet::ianalysisSimOrReal=0;
int HShowerTofinoTaskSet::ianalysisLevel=0;
int HShowerTofinoTaskSet::imagnet=0;
int HShowerTofinoTaskSet::analysisSimOrReal=0;
int HShowerTofinoTaskSet::analysisLevel=1;
Bool_t HShowerTofinoTaskSet::isCoilOff=kFALSE;
Bool_t HShowerTofinoTaskSet::isSimulation=kFALSE;

HShowerTofinoTaskSet::HShowerTofinoTaskSet(Text_t name[],Text_t title[]) : HTaskSet(name,title) {
  // Constructor
  /* 
  ianalysisSimOrReal=0;
  ianalysisLevel=0;
  imagnet=0;

  analysisSimOrReal=0;
  analysisLevel=1;
  isCoilOff=kFALSE;
  isSimulation=kFALSE;
  */
}


HShowerTofinoTaskSet::HShowerTofinoTaskSet(void) : HTaskSet() {
  // Default constructor
  /* 
  ianalysisSimOrReal=0;
  ianalysisLevel=0;
  imagnet=0;

  analysisSimOrReal=0;
  analysisLevel=1;
  isCoilOff=kFALSE;
  isSimulation=kFALSE;
  */
}


HShowerTofinoTaskSet::~HShowerTofinoTaskSet(void) {
  // Destructor.
}


HTask* HShowerTofinoTaskSet::make(const char *select, Option_t *option) {
  // Returns a pointer to the Shower task or taskset specified by 'select'

  HTaskSet *tasks = new HTaskSet("ShowerTofino","List of Showertofino tasks");

  TString sel = select;
  sel.ToLower() ;

  TString opt = option;
	  
  passArguments(opt);
  if(isCoilOff == kFALSE) 
  { // this algorithm work only with magnetic filed switched on
	  
  	if(isSimulation) 
	{
  		if(analysisLevel == 1) {
			tasks->add( new HShowerTofPIDFinder("shotof.pid","shotof.pid") );
			tasks->add( 
				new HShowerTofPIDTrackMatcher("shotof.trmatch","shotof.trmatch") );
		}
  	}
  	else if(!isSimulation) 
    {
  		if(analysisLevel == 1)  
		{ 
			tasks->add( new HShowerTofPIDFinder("shotof.pid","shotof.pid") );
		}
  	}
  } 
  else 
  {	  
	cerr <<"\n HShowerTofinoTaskSet::make(): THIS TASK DOES NOT WORK WITHOUT MAGNETIC FIELD"<< endl;	    
	printOptions();
	exit(1);
  }	  
  return tasks;
}

void HShowerTofinoTaskSet::setAnalysisSimOrReal(Option_t* optrealorsim="") {
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
    	cerr<<"HShowerTofinoTaskSet:setRealOrSim() unknown argument :use real or simulation" << endl;
        exit(1);
    }
}

void HShowerTofinoTaskSet::setAnalysisLevel(Option_t* optanalysislevel="") {
// Sets maximum analysis levels leprecognition 
TString optanalLavel = optanalysislevel; 

 	optanalLavel.ToLower();
	if(optanalLavel.CompareTo("leprecognition")==0) 
    {
		analysisLevel=1;	
    }   
    else
    {
    	cerr<<"HShowerTofinoTaskSet::setAnalysisLevel(Option_t*): unknown argument use leprecognition" << endl;
		exit(1);     
	}
}
	
void HShowerTofinoTaskSet::passArguments(TString s1) {
// pass arguments to the make tasks function

TString slevel = "leprecognition" ;
TString ssimulation = "simulation" ;
TString sreal = "real" ;
TString smagneton = "magneton" ;
TString smagnetoff = "magnetoff" ;


TString s2 = s1;

s1.ToLower();
s1.ReplaceAll("","");

Ssiz_t len=s1.Length();

  if(len!=0) {
     
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
	// go over list of arguments and compare the known keys

	while ((stemp=(TObjString*)myiter->Next())!= 0) {

	    argument=stemp->GetString();

		if(argument.CompareTo(slevel) == 0) 
		{
          ianalysisLevel++; 		
		  if(ianalysisLevel>1) 
		  {
		     cerr<<"HShowerTofinoTaskSet: overlap with other argument for analysis level:LEPRECOGNITION!" << endl; 
	  
	    	cout << "input option string: " << s2  << endl;
	    	exit(1);
	 	  }

           analysisLevel=1;
		}	
       
		
        if(argument.CompareTo(ssimulation) == 0) {
	      	
          ianalysisSimOrReal++; 		
		  if(ianalysisSimOrReal>1) 
		  {
		     cerr<<"HShowerTofinoTaskSet: ovrlap with other argument for analysis level:SIMULATION!"<< endl; 
	   	 	 cout << "input option string: " << s2  << endl;
	    	 exit(1);
		  }
		  isSimulation=kTRUE;
		}

        else if(argument.CompareTo(sreal)==0) 
		{
    	    ianalysisSimOrReal++;
	    	if(ianalysisSimOrReal>1) 
			{
		      cerr<<"HShowerTofinoTaskSet: overlap with other argument for analysis level:REAL!" << endl;
			  cout << "input option string: " << s2  << endl;
			  exit(1);
            }
	     isSimulation=kFALSE;
		}

        if(argument.CompareTo(smagnetoff)==0) 
		{
           imagnet++;
           if(imagnet>1)
           {
              cerr<< "HShowerTofinoTaskSet: overlap with other argument for magnet status:MagnetOff!" << endl;
              cout<<"INPUT OPTIONSTRING: "<<s2<<endl;
              printOptions();
              exit(1);
           }
           isCoilOff=kTRUE;
        }
		 else if(argument.CompareTo(smagneton)==0)
        {
			imagnet++;
				cout<<"INPUT OPTIONSTRING: "<<s2<<endl;
			if(imagnet>1)
			{
				cerr<<"HShowerTofinoTaskSet: overlap with other argument for magnet status:MagnetOn!" << endl;
				cout<<"INPUT OPTIONSTRING: "<<s2<<endl;
				printOptions();
        	    exit(1);
        	}
        	isCoilOff=kFALSE;
        }

     }
  }
}

void HShowerTofinoTaskSet::printOptions(void) {

printf("*********************************************************** \n\n");

printf("HShowerTofPIDfinder WORKS PROPERLY ONLY FOR EVENTS WITH MAGNETIC FIELD \n");
printf("THESE ARE POSSIBLE ANALYSIS OPTIONS FOR HShowerTofPIDfinder \n\n");
printf("real - REAL DATA ANALYSIS / DEFAULT \n");
printf("simulation - SIMULATION DATA ANALYSIS \n");
printf("magnetoff - MAGNETIC FIELD SWITCH OFF \n");
printf("magneton - MAGNETIC FIELD SWITCH ON / DEFAULT \n");
printf("leprecognition - LEPTON RECOGNITION ALGORITHM / DEFAULT  \n\n");

printf("*********************************************************** \n");
}

ClassImp(HShowerTofinoTaskSet)
