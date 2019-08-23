#include "hstarttaskset.h"
#include "htask.h"
#include "htaskset.h"
#include "hdebug.h"
#include <TROOT.h>
#include <TClass.h>
#include "haddef.h"
#include "hstartcalibrater.h"
#include "hstarthitf.h"
#include <TBrowser.h>

#include <iostream.h>

//*-- Author : Dan Magestro
//*-- Modified: 19/11/2001  by D. Magestro

///////////////////////////////////////////////////////////////////////////////
// HStartTaskSet
//
// This HTaskSet contains the tasks for the Start detector
//
// BEGIN_HTML<b><font color="maroon">For more information, see <a target="_top" href="http://webdb.gsi.de:8010/pls/hades_webdbs/hanal.hal_posting_query.show_posting_text?p_id=436">this analysis logbook entry<a>.</font></b> END_HTML
//
///////////////////////////////////////////////////////////////////////////////

HStartTaskSet::HStartTaskSet(Text_t name[],Text_t title[]) : HTaskSet(name,title) {
  // Constructor
}


HStartTaskSet::HStartTaskSet(void) : HTaskSet() {
  // Default constructor
}


HStartTaskSet::~HStartTaskSet(void) {
  // Destructor.
}


HTask* HStartTaskSet::make(const char *select, Option_t *option) {
  // Returns a pointer to the Start task or taskset specified by 'select'

  HTaskSet *tasks = new HTaskSet("Start","List of Start tasks");

  TString sel = select;

  if( sel.IsNull() ) {
    tasks->add( new HStartCalibrater("start.cal","Start cal") );
    tasks->add( new HStartHitF("start.hitf","Start hitf") );
  }
  return tasks;
}

ClassImp(HStartTaskSet)
