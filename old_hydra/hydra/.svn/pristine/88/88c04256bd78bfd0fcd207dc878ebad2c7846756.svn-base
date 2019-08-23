using namespace std;
#include "htriggertaskset.h"
#include "htask.h"
#include "htaskset.h"
#include "hdebug.h"
#include <TROOT.h>
#include <TClass.h>
#include "haddef.h"
#include "muEmulation.h"
#include "muDilepEmulation.h"
#include <TBrowser.h>

#include <iostream> 
#include <iomanip>


///////////////////////////////////////////////////////////////////////////////
// HTriggerTaskSet
//
// This HTaskSet contains the tasks for the Trigger Emulation
//
// BEGIN_HTML<b><font color="maroon">For more information, see <a target="_top" href="http://webdb.gsi.de:8010/pls/hades_webdbs/hanal.hal_posting_query.show_posting_text?p_id=436">this analysis logbook entry<a>.</font></b> END_HTML
//
///////////////////////////////////////////////////////////////////////////////

HTriggerTaskSet::HTriggerTaskSet(Text_t name[],Text_t title[]) : HTaskSet(name,title) {
  // Constructor
}


HTriggerTaskSet::HTriggerTaskSet(void) : HTaskSet() {
  // Default constructor
}


HTriggerTaskSet::~HTriggerTaskSet(void) {
  // Destructor.
}


HTask* HTriggerTaskSet::make(const char *select, Option_t *option) {
  // Returns a pointer to the Rich task or taskset specified by 'select'

  HTaskSet *tasks = new HTaskSet("Trigger","List of Trigger tasks");
  TString real ="real";

  TString sel = select;
  sel.ToLower();


  if( sel.CompareTo(real)==0) {
    tasks->add(new HMUEmulation("trigger.emu", "trigger emulation"));
    tasks->add(new HMUDilepEmulation("trigger.dilep.emu", "trigger dilepton emulation"));
  }
  return tasks;


}

ClassImp(HTriggerTaskSet)

