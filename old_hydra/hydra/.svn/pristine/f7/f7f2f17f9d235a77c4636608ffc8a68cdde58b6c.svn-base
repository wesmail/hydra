#include "hrichtaskset.h"
#include "htask.h"
#include "htaskset.h"
#include "hdebug.h"
#include <TROOT.h>
#include <TClass.h>
#include "haddef.h"
#include "hrichanalysis.h"
#include "hrichparticleidentifier.h"
#include <TBrowser.h>

#include <iostream.h>

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

  TString sel = select;

  if( sel.IsNull() ) {
    tasks->add(new HRichAnalysis("rich.ana","Ring finder"));
    tasks->add(new HRichParticleIdentifier("rich.id","Rich PID"));
  }
  return tasks;
}

ClassImp(HRichTaskSet)
