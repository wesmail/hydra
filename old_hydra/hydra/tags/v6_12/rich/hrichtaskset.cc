using namespace std;
#include "hrichtaskset.h"
#include "htask.h"
#include "htaskset.h"
#include "hdebug.h"
#include <TROOT.h>
#include <TClass.h>
#include "haddef.h"
#include "hrichanalysis.h"
#include "hrichanalysissim.h"
#include "hrichdigitizer.h"
#include "hrichparticleidentifier.h"
#include <TBrowser.h>
#include <TString.h>
#include <TObjString.h>
#include <iostream> 
#include <iomanip>

//*-- Author : Dan Magestro
//*-- Modified: 19/11/2001  by D. Magestro

///////////////////////////////////////////////////////////////////////////////
// HRichTaskSet
//
// This HTaskSet contains the tasks for the Rich detector
//
// BEGIN_HTML<b><font color="maroon">For more information, see <a target="_top" href="http://webdb.gsi.de:8010/pls/hades_webdbs/hanal.hal_posting_query.show_posting_text?p_id=436">this analysis logbook entry<a>.</font></b> END_HTML
//
///////////////////////////////////////////////////////////////////////////////


Bool_t HRichTaskSet::inoiseOn=kFALSE;
Bool_t HRichTaskSet::ilowField=kFALSE;
HRichTaskSet::HRichTaskSet(Text_t name[],Text_t title[]) : HTaskSet(name,title) {
  // Constructor
}


HRichTaskSet::HRichTaskSet(void) : HTaskSet() {
  // Default constructor
}


HRichTaskSet::~HRichTaskSet(void) {
  // Destructor.
}


HTask* HRichTaskSet::make(const char *select, Option_t *option) {
  // Returns a pointer to the Rich task or taskset specified by 'select'

  HTaskSet *tasks = new HTaskSet("Rich","List of Rich tasks");
  TString simulation="simulation";
  TString real ="real";

  TString opt=option;
  parseArguments(opt);
  TString sel = select;
  sel.ToLower();
  if(sel.CompareTo(simulation)==0){
      tasks->add(new HRichDigitizer("rich.digi","Rich digitizer",inoiseOn));
      tasks->add(new HRichAnalysisSim("rich.ana","Ring finder",ilowField));
  }
  if( sel.CompareTo(real)==0) {
    tasks->add(new HRichAnalysis("rich.ana","Ring finder",ilowField));
    tasks->add(new HRichParticleIdentifier("rich.id","Rich PID"));
  }
  return tasks;
}
void HRichTaskSet::parseArguments(TString s1)
{
  TString lowField="lowfield";
  TString noiseOn="noiseon";

  TString s2=s1;
  s1.ToLower();
  cout<<s1.Data()<<endl;
  s1.ReplaceAll(" ","");
  cout<<s1.Data()<<endl;
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
	if(argument.CompareTo(lowField)==0){
	  ilowField = kTRUE;
	}
	else if(argument.CompareTo(noiseOn)==0){
	  inoiseOn  = kTRUE;
	}
      }
      
    }
}
ClassImp(HRichTaskSet)


