#include "hshowertaskset.h"
#include "htask.h"
#include "htaskset.h"
#include "hdebug.h"
#include <TROOT.h>
#include <TClass.h>
#include "haddef.h"
#include "hshowercalibrater.h"
#include "hshowerhitfinder.h"
#include "hshowertofinocorrelator.h"
#include <TBrowser.h>

#include <iostream.h>

//*-- Author : Dan Magestro
//*-- Modified: 19/11/2001  by D. Magestro

///////////////////////////////////////////////////////////////////////////////
// HShowerTaskSet
//
// This HTaskSet contains the tasks for the Shower detector
//
// BEGIN_HTML<b><font color="maroon">For more information, see <a target="_top" href="http://webdb.gsi.de:8010/pls/hades_webdbs/hanal.hal_posting_query.show_posting_text?p_id=436">this analysis logbook entry<a>.</font></b> END_HTML
//
///////////////////////////////////////////////////////////////////////////////

HShowerTaskSet::HShowerTaskSet(Text_t name[],Text_t title[]) : HTaskSet(name,title) {
  // Constructor
}


HShowerTaskSet::HShowerTaskSet(void) : HTaskSet() {
  // Default constructor
}


HShowerTaskSet::~HShowerTaskSet(void) {
  // Destructor.
}


HTask* HShowerTaskSet::make(const char *select, Option_t *option) {
  // Returns a pointer to the Shower task or taskset specified by 'select'

  HTaskSet *tasks = new HTaskSet("Shower","List of Shower tasks");

  TString sel = select;

  if( sel.IsNull() ) {
    tasks->add( new HShowerCalibrater("sho.cal","sho.cal") );
    tasks->add( new HShowerHitFinder("sho.hitf","sho.hitf") );
    tasks->add( new HShowerTofinoCorrelator("sho.cor","sho.cor") );
  }
  return tasks;
}

ClassImp(HShowerTaskSet)
