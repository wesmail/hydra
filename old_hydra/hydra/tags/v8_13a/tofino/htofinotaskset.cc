using namespace std;
#include "htofinotaskset.h"
#include "htask.h"
#include "htaskset.h"
#include "hdebug.h"
#include <TROOT.h>
#include <TClass.h>
#include "haddef.h"
#include "hades.h"
#include "htofinocalibrater.h"
#include "htofinodigitizer.h"
#include <TBrowser.h>

#include <iostream> 
#include <iomanip>

//*-- Author : Dan Magestro
//*-- Modified: 19/11/2001  by D. Magestro

//_HADES_CLASS_DESCRIPTION 
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
  sel.ToLower();
  TString opt = option;
  opt.ToLower();

  if(sel.Contains("simulation")&&gHades->getEmbeddingMode()!=0)
  {
      Error("HTofinoTaskSet::make()","embeddingMode > 0 in simulation NOT ALLOWED!");
      exit(1);
  }

  if( opt.Contains("simulation") ) {
    tasks->add( new HTofinoDigitizer("tofi.digi","tofi.digi"));
  }
  else 
  {
    tasks->add( new HTofinoCalibrater("tofino.cal","tofino") );

    if(gHades->getEmbeddingMode()!=0){
	tasks->add( new HTofinoDigitizer("tofi.digi","tofi.digi"));
    }
  }
  return tasks;
}

ClassImp(HTofinoTaskSet)
