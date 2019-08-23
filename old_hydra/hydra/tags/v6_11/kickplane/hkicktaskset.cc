#include "hkicktaskset.h"
#include "htask.h"
#include "htaskset.h"
#include "hdebug.h"
#include <TROOT.h>
#include <TClass.h>
#include "haddef.h"
#include "hkicktrackf.h"
#include "hkicktrackfsim.h"
#include "hrttrackfind.h"
#include "hrtmetamatch.h"
#include <TBrowser.h>

#include <iostream.h>

//*-- Author : Dan Magestro
//*-- Modified: 19/11/2001  by D. Magestro

///////////////////////////////////////////////////////////////////////////////
// HKickTaskSet
//
// This HTaskSet contains the tasks for the Kick detector
//
// Options: lowres - Low resolution tracking (HKickTrackF)
//
//                   tofclust - to use ToF hits clustering
//                              (default is non-clustering mode)
//
//           OR
//          hires  - High-resolution tracking (HKickMdcTrkF,MKickMatch)
//
//          debug  - Debug mode for HKickMdcTrkF
//
// Note: Thanks to TString, options can be separated and written any way
//       you want, such as
//
//             HKickTaskSet::make("","lowres + debug")
//
// BEGIN_HTML<b><font color="maroon">For more information, see <a target="_top" href="http://webdb.gsi.de:8010/pls/hades_webdbs/hanal.hal_posting_query.show_posting_text?p_id=436">this analysis logbook entry<a>.</font></b> END_HTML
//
///////////////////////////////////////////////////////////////////////////////

HKickTaskSet::HKickTaskSet(Text_t name[],Text_t title[]) : HTaskSet(name,title) {
  // Constructor
}


HKickTaskSet::HKickTaskSet(void) : HTaskSet() {
  // Default constructor
}


HKickTaskSet::~HKickTaskSet(void) {
  // Destructor.
}


HTask* HKickTaskSet::make(const char *select, Option_t *option) {
  // Returns a pointer to the Kick task or taskset specified by 'select'

  HTaskSet *tasks = new HTaskSet("Kick","List of Kick tasks");

  TString sel = select;
  sel.ToLower();
  TString opt = option;
  opt.ToLower();

  if( opt.Contains("lowres") ) {
    HKickTrackF *kicktrackf = 0;
    if (opt.Contains("simulation")) {
      kicktrackf = new HKickTrackFSim("kick.trackf","kick.trackf");
    } else {
      kicktrackf = new HKickTrackF("kick.trackf","kick.trackf");
    }
    if (opt.Contains("debug")) kicktrackf->setDebug(kTRUE);
    if( opt.Contains("tofclust")) {
      kicktrackf->setTofClusterMode(kTRUE);
    }
    if(opt.Contains("controlhists")){
      kicktrackf->enableControlHistograms();
    }
    tasks->add(kicktrackf);
    return tasks;

  } else {
     HRtTrackFind *kickmdctrkf = new HRtTrackFind("kick.reft","kick.reft");
     tasks->add(kickmdctrkf);
     tasks->add( new HRtMetaMatch("kick.match","kick.match") );
  }

  return tasks;
}

ClassImp(HKickTaskSet)
