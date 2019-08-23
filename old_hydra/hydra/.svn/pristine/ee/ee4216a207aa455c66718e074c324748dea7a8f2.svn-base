using namespace std;
#include "hstarttaskset.h"
#include "htask.h"
#include "htaskset.h"
#include "hdebug.h"
#include <TROOT.h>
#include <TClass.h>
#include "haddef.h"
#include "hstartcalibrater.h"
#include "hstarthitf.h"
#include "hstarthitfproton.h"
#include "htboxsum.h"
#include <TBrowser.h>

#include <iostream> 
#include <iomanip>

//*-- Author : Dan Magestro
//*-- Modified: 19/11/2001  by D. Magestro

//_HADES_CLASS_DESCRIPTION 
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


HTask* HStartTaskSet::make(const Char_t *select, Option_t *option) {
  // Returns a pointer to the Start task or taskset specified by 'select'

  HTaskSet *tasks = new HTaskSet("Start","List of Start tasks");

  TString sel = select;
  TString opt = option;
  opt.ToLower();

  if( sel.IsNull() ) {
    tasks->add( new HStartCalibrater("start.cal","Start cal") );
	if(opt.Contains("protons")){
		HStartHitFProton *hstarthitf = 
			new  HStartHitFProton("start.hitfproton","Start hitfproton"); 
    tasks->add(hstarthitf);
	}	
	else{
    	HStartHitF *hstarthitf= new HStartHitF("start.hitf","Start hitf");
    	if(opt.Contains("controlhists")) {
      	hstarthitf->enableControlHistograms();
    	}
    tasks->add(hstarthitf);
	}
    if(opt.Contains("scalers")) {
      HTBoxSum *htboxsum= new HTBoxSum("scaler.add","Scalers");
      tasks->add(htboxsum);
    }
  }
  return tasks;
}

ClassImp(HStartTaskSet)
