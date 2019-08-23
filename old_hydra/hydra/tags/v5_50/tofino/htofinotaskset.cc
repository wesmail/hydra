#include "htofinotaskset.h"
#include "htask.h"
#include "htaskset.h"
#include "hdebug.h"
#include <TROOT.h>
#include <TClass.h>
#include "haddef.h"
#include "htofinocalibrater.h"
#include <TBrowser.h>

#include <iostream.h>

//*-- Author : Dan Magestro
//*-- Modified: 19/11/2001  by D. Magestro

///////////////////////////////////////////////////////////////////////////////
// HTofinoTaskSet
//
// This HTaskSet contains the tasks for the Tofino detector
//
// BEGIN_HTML<b><font color="maroon">For more information, see <a target="_top" href="http://webdb.gsi.de:8010/pls/hades_webdbs/hanal.hal_posting_query.show_posting_text?p_id=436">this analysis logbook entry<a>.</font></b> END_HTML
//
///////////////////////////////////////////////////////////////////////////////

HTofinoTaskSet::HTofinoTaskSet(Text_t name[],Text_t title[]) : HTaskSet(name,title) {
  // Constructor
}


HTofinoTaskSet::HTofinoTaskSet(void) : HTaskSet() {
  // Default constructor
}


HTofinoTaskSet::~HTofinoTaskSet(void) {
  // Destructor.
}


HTask* HTofinoTaskSet::make(const char *select, Option_t *option) {
  // Returns a pointer to the Tofino task or taskset specified by 'select'

  HTaskSet *tasks = new HTaskSet("Tofino","List of Tofino tasks");

  TString sel = select;

  if( sel.IsNull() ) {
    return new HTofinoCalibrater("tofino.cal","tofino");
    // tasks->add( new HTofinoHitF("tofino.hitf","tofino.hitf") );
  }
  return tasks;
}

ClassImp(HTofinoTaskSet)
