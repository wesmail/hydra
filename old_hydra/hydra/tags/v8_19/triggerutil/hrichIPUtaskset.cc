//*-- Author: Alberica Toia
//*-- Modified: Joern Wuestenfeld 05/13/2005

using namespace std;
#include "hrichIPUtaskset.h"
#include "hrichanalysisIPU.h"
#include "hrichanalysisIPUSim.h"
//#include "hrichdigitizer.h"  // not needed at the moment! Uncommented lines in make function.

//_HADES_CLASS_DESCRIPTION 
///////////////////////////////////////////////////////////////////////////////
//
// HRichIPUTaskSet
//
// This HTaskSet contains the tasks for the RichIPU
//
// BEGIN_HTML<b><font color="maroon">For more information, see <a target="_top" href="http://webdb.gsi.de:8010/pls/hades_webdbs/hanal.hal_posting_query.show_posting_text?p_id=436">this analysis logbook entry<a>.</font></b> END_HTML
//
///////////////////////////////////////////////////////////////////////////////

HRichIPUTaskSet::HRichIPUTaskSet(Text_t name[],Text_t title[])
  : HTaskSet(name,title)
{
  // Constructor
  //
  // Input parameters:
  //   Text_t name[]
  //      Name of the Task
  //   Text_t title[]
  //      Title / desciption of the task
  //
  // Output parameters:
  //    none
  //
  // Return code:
  //    none
}


HRichIPUTaskSet::HRichIPUTaskSet(void)
  : HTaskSet()
{
  // Default constructor
  //
  // Input parameters:
  //   none
  //
  // Output parameters:
  //    none
  //
  // Return code:
  //    none
}


HRichIPUTaskSet::~HRichIPUTaskSet(void) {
  // Destructor.
  //
  // Input parameters:
  //    none
  //
  // Output parameters:
  //    none
  //
  // Return code:
  //    none
}


HTask* HRichIPUTaskSet::make(const Char_t *select, Option_t *option)
{
  // Returns a pointer to the RichIPU task or taskset specified by 'select'
  //  Tasks that are added depend on the 'selet' parameter.
  //  If selet is set to 'simulation' then the HRichAnalysisIPUSim task will be added to the list.
  //  If select is set to 'real' then the HRichAnalysisIPU task will be added to the the list.
  //  If 'select' is empty (""), no task will be added to the list!
  //
  // Input parameters:
  //    const Char_t *select
  //       Select which configuration to run
  //       Possible values: simulation || real
  //    Option_t *option
  //        Not used!
  //
  // Output parameters:
  //    none
  //
  // Return code:
  //    Returns a pointer to a Set of Tasks that will be run by this taskset.

  HTaskSet *tasks = new HTaskSet("RichIPU","List of Rich IPU tasks");
  TString simulation="simulation";
  TString real ="real";

  TString sel = select;
  sel.ToLower();
  if(sel.CompareTo(simulation)==0){
    //tasks->add(new HRichDigitizer("rich.digi","Rich digitizer"));
    tasks->add(new HRichAnalysisIPUSim("richIPU.ana","IPU Ring finder"));
  }
  if( sel.CompareTo(real)==0) {
    tasks->add(new HRichAnalysisIPU("richIPU.ana","IPU Ring finder"));
    //tasks->add(new HRichIPURemake("richIPU.remake","IPU Remake"));
  } 
  return tasks;
}

ClassImp(HRichIPUTaskSet)
