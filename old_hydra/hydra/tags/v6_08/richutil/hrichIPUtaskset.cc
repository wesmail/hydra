#include "hrichIPUtaskset.h"
#include "htask.h"
#include "htaskset.h"
#include "hdebug.h"
#include <TROOT.h>
#include <TClass.h>
#include "haddef.h"
#include "hrichanalysisIPU.h"
#include "hrichanalysisIPUSim.h"
#include "hrichdigitizer.h"
#include <TBrowser.h>

#include <iostream.h>


///////////////////////////////////////////////////////////////////////////////
// HRichIPUTaskSet
//
// This HTaskSet contains the tasks for the Rich detector
//
// BEGIN_HTML<b><font color="maroon">For more information, see <a target="_top" href="http://webdb.gsi.de:8010/pls/hades_webdbs/hanal.hal_posting_query.show_posting_text?p_id=436">this analysis logbook entry<a>.</font></b> END_HTML
//
///////////////////////////////////////////////////////////////////////////////

HRichIPUTaskSet::HRichIPUTaskSet(Text_t name[],Text_t title[]) : HTaskSet(name,title) {
  // Constructor
}


HRichIPUTaskSet::HRichIPUTaskSet(void) : HTaskSet() {
  // Default constructor
}


HRichIPUTaskSet::~HRichIPUTaskSet(void) {
  // Destructor.
}


HTask* HRichIPUTaskSet::make(const char *select, Option_t *option) {
  // Returns a pointer to the Rich task or taskset specified by 'select'

  HTaskSet *tasks = new HTaskSet("RichIPU","List of Rich IPU tasks");
  TString simulation="simulation";
  TString real ="real";

  TString sel = select;
  sel.ToLower();
  if(sel.CompareTo(simulation)==0){
      tasks->add(new HRichDigitizer("rich.digi","Rich digitizer"));
      tasks->add(new HRichAnalysisIPUSim("richIPU.ana","IPU Ring finder"));
  }
  if( sel.CompareTo(real)==0) {
    tasks->add(new HRichAnalysisIPU("richIPU.ana","IPU Ring finder"));
  }
  return tasks;
}

ClassImp(HRichIPUTaskSet)
