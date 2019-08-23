using namespace std;
#include "hkicktaskset.h"
#include "htask.h"
#include "htaskset.h"
#include "hdebug.h"
#include <TROOT.h>
#include <TClass.h>
#include "haddef.h"
#include "hkicktrackf.h"
#include "hkicktrackfsim.h"
#include "hrttrackfindsim.h"
#include "hrttrackfind.h"
#include "hrtmetamatch.h"
#include <TBrowser.h>

#include <iostream> 
#include <iomanip>

//*-- Author : Dan Magestro
//*-- Modified: 19/11/2001  by D. Magestro

///////////////////////////////////////////////////////////////////////////////
// HKickTaskSet
//
// This HTaskSet contains the tasks for the Kick detector
//
// Options: 
//   lowres - Low resolution tracking (HKickTrackF)
//                   tofclust - to use ToF hits clustering
//                              (default is non-clustering mode)
//
//   hires  - High-resolution tracking with MDC3 + Meta
//   reft   - Reference trajectories algorithm with MDC34 + Meta
//
//   debug  - Debug mode for HKickMdcTrkF
//   storeall - Stores all track candidates regardless of quality 
//              (only hires and reft)
//
// Note: lowres and hires CAN be used at the same time
// Note: Options can be separated any way you want
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

  } 
  if (opt.Contains("hires")) {
     HRtTrackFind::EMode trackingMode = HRtTrackFind::Mdc3Pos;
     HRtTrackFind *trackf = 0;
     if (opt.Contains("simulation")) 
       trackf = new HRtTrackFindSim("kick.reft","kick.reft");
     else 
       trackf = new HRtTrackFind("kick.reft","kick.reft");
     trackf->setMode(trackingMode);
     trackf->setFitting(kFALSE);
     tasks->add(trackf);
     if (opt.Contains("storeall")) trackf->ignoreCuts(kTRUE);

     HRtMetaMatch *mm = new HRtMetaMatch("kick.match","kick.match");
     if (opt.Contains("storeall")) mm->ignoreCuts(kTRUE);
     if (opt.Contains("debug")) mm->setDebug();
     tasks->add(mm);
  } else if (opt.Contains("reft")) {
     HRtTrackFind::EMode trackingMode = HRtTrackFind::Mdc34;
     HRtTrackFind *trackf = 0;
     if (opt.Contains("simulation")) 
       trackf = new HRtTrackFindSim("kick.reft","kick.reft");
     else 
       trackf = new HRtTrackFind("kick.reft","kick.reft");
     trackf->setMode(trackingMode);
     trackf->setFitting(kTRUE);
     tasks->add(trackf);
     if (opt.Contains("storeall")) trackf->ignoreCuts(kTRUE);

     HRtMetaMatch *mm = new HRtMetaMatch("kick.match","kick.match");
     if (opt.Contains("storeall")) mm->ignoreCuts(kTRUE);
     if (opt.Contains("debug")) mm->setDebug();
     tasks->add(mm);
  }

  return tasks;
}

ClassImp(HKickTaskSet)
