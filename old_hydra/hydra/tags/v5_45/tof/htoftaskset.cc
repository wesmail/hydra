#include "htoftaskset.h"
#include "hdebug.h"
#include "haddef.h"
#include "htofhitf.h"
#include "htofclusterf.h"
#include <iostream.h>

//*-- Author : Dan Magestro
//*-- Modified: 04/12/2001  by R. Holzmann
//*-- Modified: 19/11/2001  by D. Magestro

///////////////////////////////////////////////////////////////////////////////
// HTofTaskSet
//
// This HTaskSet contains the tasks for the Tof detector
//
// BEGIN_HTML<b><font color="maroon">For more information, see <a target="_top" href="http://webdb.gsi.de:8010/pls/hades_webdbs/hanal.hal_posting_query.show_posting_text?p_id=436">this analysis logbook entry<a>.</font></b> END_HTML
//
///////////////////////////////////////////////////////////////////////////////

HTofTaskSet::HTofTaskSet(Text_t name[],Text_t title[]) : HTaskSet(name,title) {
  // Constructor
}


HTofTaskSet::HTofTaskSet(void) : HTaskSet() {
  // Default constructor
}


HTofTaskSet::~HTofTaskSet(void) {
  // Destructor.
}


HTask* HTofTaskSet::make(const char *select, Option_t *option) {
  // Returns a pointer to the Tof task or taskset specified by 'select'

  HTaskSet *tasks = new HTaskSet("Tof","List of Tof tasks");

  TString sel = select;

  if( sel.IsNull() ) {
    tasks->add( new HTofHitF("tof.hitf","tof.hitf") );
    tasks->add( new HTofClusterF("tof.clusf","tof.clusf") );
  }
  return tasks;
}

ClassImp(HTofTaskSet)










