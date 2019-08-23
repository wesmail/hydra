using namespace std;
#include "htoftaskset.h"
#include "hdebug.h"
#include "haddef.h"
#include "htofdigitizer.h"
#include "htofhitf.h"
#include "htofhitfsim.h"
#include "htofclusterf.h"
#include <iostream> 
#include <iomanip>

//*-- Author : Dan Magestro
//*-- Modified: 04/12/2001  by R. Holzmann
//*-- Modified: 19/11/2001  by D. Magestro

//*-- Modified: 30/06/2004  by J. Otwinowski
// TofCluster category is filled by default

//*-- Modified: 06/07/2004  by J. Otwinowski
//	Come back to the optional TofCluster (better for cosmics) 

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
  TString simulation="simulation";
  TString real ="real";
  TString nocluster ="noclust";

  TString sel = select;
  TString opt = option;
  sel.ToLower();

  if(sel.CompareTo(simulation)==0) 
  {
  	if(opt.CompareTo(nocluster)==0) 
	{
    	  tasks->add( new HTofDigitizer("tof.digi","tof.digi") );
    	  tasks->add( new HTofHitFSim("tof.hitfs","tof.hitfs") );
	}
	else
	{
    	  tasks->add( new HTofDigitizer("tof.digi","tof.digi") );
    	  tasks->add( new HTofHitFSim("tof.hitfs","tof.hitfs") );
      	  tasks->add( new HTofClusterF("tof.clusf","tof.clusf") );
	}
  }
  if(sel.CompareTo(real)==0) 
  {
  	if(opt.CompareTo(nocluster)==0) 
	{
    	  tasks->add( new HTofHitF("tof.hitf","tof.hitf") );
	}
	else 
	{
    	  tasks->add( new HTofHitF("tof.hitf","tof.hitf") );
    	  tasks->add( new HTofClusterF("tof.clusf","tof.clusf") );
	}
  }
  return tasks;
}

ClassImp(HTofTaskSet)










