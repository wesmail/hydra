//*-- Author: Alberica Toia
//*-- Modified: Joern Wuestenfeld 03/31/2005
//*-- Version: $Id: htriggertaskset.cc,v 1.10 2006-08-12 13:05:57 halo Exp $

#include "htriggertaskset.h"
#include "muEmulation.h"
#include "muEmulationExp.h"
#include "muEmulationSim.h"
#include "muDilepEmulation.h"
#include "muDilepEmulationExp.h"
#include "muDilepEmulationSim.h"
#include "hmuemueventfilter.h"
#include "TObjString.h"

//_HADES_CLASS_DESCRIPTION 
///////////////////////////////////////////////////////////////////////////////
// HTriggerTaskSet
//
// This HTaskSet contains the tasks for the Trigger Emulation
//
// BEGIN_HTML<b><font color="maroon">For more information, see <a target="_top" href="http://webdb.gsi.de:8010/pls/hades_webdbs/hanal.hal_posting_query.show_posting_text?p_id=436">this analysis logbook entry<a>.</font></b> END_HTML
//
///////////////////////////////////////////////////////////////////////////////

HTriggerTaskSet::HTriggerTaskSet(Text_t name[],Text_t title[]) : HTaskSet(name,title) {
  // Constructor, calls constructor of base class.
  //
  // Input parameters:
  //	Text_t name[]
  //		Name of the object
  //
  //	Text_t title[]
  //		Title of the object
  //
  // Output parameters:
  //	none
  //
  // Return code:
  //	none
  isReal = kFALSE;
  isExp = kFALSE;
  isSimulation = kFALSE;
  doFilter = kFALSE;
}


HTriggerTaskSet::HTriggerTaskSet(void) : HTaskSet() {
  // Default constructor calls constructor of base class.
  //
  // Input parameters:
  //	none
  //
  // Output parameters:
  //	none
  //
  // Return code:
  //	none
  isReal = kFALSE;
  isExp = kFALSE;
  isSimulation = kFALSE;
  doFilter = kFALSE;
}


HTriggerTaskSet::~HTriggerTaskSet(void) {
  // Destructor.
  //
  // Input parameters:
  //	none
  //
  // Output parameters:
  //	none
  //
  // Return code:
  //	none
}


HTask* HTriggerTaskSet::make(const char *select, Option_t *option) {
  // Returns a pointer to the Trigger taskset as specified by 'select'.
  // If select is empty, NO task will added to the list!
  //
  // Input parameters:
  //  const char* select -- default == ""
  //    Selects set of tasks to be created. Possible values are: real || exp || (simulation && filter)
  //    'real' ist for real dat with MU inforamtion,
  //    'exp' is for real data witout MU information and
  //    'simulation' is for simulated data.
  //    'filter' skips all negativly triggered events from the MU emulation
  //    (in combination with simulation option only!)
  //
  //  Option_t* option -- default = ""
  //    Selects sub option for taskset. Not used in this fuction!
  //
  // output parameters:
  //  none
  //
  // Return code:
  //  Returns a pointer to a Taskset, that contains the list of tasks as specifyed
  //  by the input parameters.

  HTaskSet *tasks = new HTaskSet("Trigger","List of Trigger tasks");

  parseArguments(select);


  if(isReal) {
    // Real datta, MU information available
    tasks->add(new HMUEmulation("trigger.emu", "trigger emulation"));
    tasks->add(new HMUDilepEmulation("trig.dilep.emu", "trigger dilepton emulation"));
  }
  if(isExp) {
    // Real data, MU information not available
    tasks->add(new HMUEmulationExp("trigger.emu", "trigger emulation exp"));
    tasks->add(new HMUDilepEmulationExp("trig.dilep.emu", "trigger dilepton emulation exp"));
  }
  if(isSimulation) {
    // Simulation data
   tasks->add(new HMUEmulationSim("trigger.emu", "trigger emulation sim"));
    //tasks->add(new HMUDilepEmulationSim("trigger.dilep.emu", "trigger dilepton emulation sim"));
    if(doFilter)
    {
      tasks->add(new HMUEmuEventFilter("mu.evtFilter","mu.evtFilter"));
    }
  }
  return tasks;
}

void HTriggerTaskSet::parseArguments(TString s1)
{
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
      if(!(buffer=strtok(NULL,",")))
      {
        break;
      }
      stemp=new TObjString(buffer);
      myarguments.Add(stemp);

      count++;
    }
    TIterator* myiter=myarguments.MakeIterator();
    // iterate over the lis of arguments and compare them
    // to known key words.
    while ((stemp=(TObjString*)myiter->Next())!= 0)
    {
      argument=stemp->GetString();
      if(argument.CompareTo("real")==0)
      {
        isReal = kTRUE;
      }
      else
      {
        if(argument.CompareTo("exp")==0)
        {
          isExp = kTRUE;
        }
        else
        {
          if(argument.CompareTo("simulation")==0)
          {
            isSimulation = kTRUE;
          }
          else
          {
            if(argument.CompareTo("filter")==0)
            {
              doFilter = kTRUE;
            }
          }
        }
      }
    }
  }
}


ClassImp(HTriggerTaskSet)

