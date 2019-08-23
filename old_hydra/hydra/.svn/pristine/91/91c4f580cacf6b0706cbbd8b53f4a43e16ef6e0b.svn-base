#include "hades.h"
#include "hdetector.h"
#include "hspectrometer.h"
#include "hmdctaskset.h"
#include "htask.h"
#include "htaskset.h"
#include "hdebug.h"
#include <TROOT.h>
#include <TClass.h>
#include "haddef.h"
#include "hmdccalibrater1.h"
#include "hmdccalibrater2.h"
#include "hmdctrackfinder.h"
#include "hmdcclustertohit.h"
#include "hmdchitf.h"
#include "hreconstructor.h"
#include "hmdcsegmentf.h"
#include <TBrowser.h>
#include "hlocation.h"

#include <iostream.h>

//*-- Author : Dan Magestro
//*-- Modified: 19/11/2001  by D. Magestro

///////////////////////////////////////////////////////////////////////////////
// HMdcTaskSet
//
//  This HTaskSet contains the tasks for the Mdc detector
//
//  Options (Defaults in CAPITAL letters)
//  -------------------------------------
//      "dubna" or "santiago" (no default) -- selects hit finder
//
//      "NOSTARTANDCAL", "StartAndCal" -- HMdcCalibrater1 mode
//       or "NoStartAndNoCal"
//
//      "MAGNETON" or "MagnetOff" -- Magnetic field (for Dubna
//                                   hit-finding mode; see HMdcTrackFinder)
//
//      "COMBINEDCLUSTERS" or -- Cluster-finding mode; 'chamber' means
//      "ChamberClusters"        clusters found for individual chambers
//                               (for Dubna hit-finder; see HMdcTrackFinder)
//
//
// Note: Thanks to TString, options can be separated and written any way
//       you want, such as
//
//             HMdcTaskSet::make("","MagnetOFF,NOstartandNOcal")
//             HMdcTaskSet::make("","ChamberClusters & STARTandCAL")
//
// BEGIN_HTML<b><font color="maroon">For more information, see <a target="_top" href="http://webdb.gsi.de:8010/pls/hades_webdbs/hanal.hal_posting_query.show_posting_text?p_id=436">this analysis logbook entry<a>.</font></b> END_HTML
//
///////////////////////////////////////////////////////////////////////////////

HMdcTaskSet::HMdcTaskSet(Text_t name[],Text_t title[]) : HTaskSet(name,title) {
  // Constructor
}


HMdcTaskSet::HMdcTaskSet(void) : HTaskSet() {
  // Default constructor
}


HMdcTaskSet::~HMdcTaskSet(void) {
  // Destructor.
}


HTask* HMdcTaskSet::make(const char *select, Option_t *option) {
  // Returns a pointer to the Mdc task or taskset specified by 'select'

  HTaskSet *tasks = new HTaskSet("Mdc","List of Mdc tasks");

  TString sel = select;
  sel.ToLower();
  TString opt = option;
  opt.ToLower();

  Int_t startCalFlag = 1;    // Default value: No Start and Cal
  Int_t timeCutFlag = 1;     // Default value: Time cuts switched on
  Bool_t isCoilOff = kFALSE; // Default value: No magnetic field
  Int_t typeClustFinder=0;   // Default value: Chamber-combined cluster finding

  // Check for start/cal option in "option"... The order here is important,
  // since 'nostartandcal' needs to be checked before 'startandcal'
  if( opt.Contains("nostartandcal") ) startCalFlag = 1;
  else if( opt.Contains("startandcal") ) startCalFlag = 2;
  else if( opt.Contains("nostartandnocal") ) startCalFlag = 3;

  // Check for NO time cut flag
  if( opt.Contains("notimecut") ) timeCutFlag = 0;

  // Check for magnetic field
  if( opt.Contains("magnetoff") ) isCoilOff = kTRUE;

  // Check for cluster-finding mode
  if( opt.Contains("chamber") ) typeClustFinder=1;

  tasks->add( new HMdcCalibrater1("mdc.cal1","Cal1",startCalFlag,timeCutFlag) );
  tasks->add( new HMdcCalibrater2("mdc.cal2","Cal2") );

  if( opt.Contains("dubna") ) {

     tasks->add( new HMdcTrackFinder("mdc.hitf","mdc.hitf",
                                              isCoilOff,typeClustFinder) );
     tasks->add( new HMdcClusterToHit("mdc.c2h","mdc.c2h") );
     //mdcTasks->connect(new HMdc12Fit("mdc.12fit","mdc.12fit"),"mdc.hitf");
  }
  else if( opt.Contains("santiago") ) {

     HLocation aLoc;
     aLoc.set(2,0,0);
     HMdcHitF *finder=0;
     Text_t buffer[255];

     // Get modules from "Mdc" detector, needed to make tasks for
     // individual chamber hit finders
     Int_t *mods = gHades->getSetup()->getDetector("Mdc")->getModules();
     Int_t mdcMods[6][4];
     for (Int_t sector = 0; sector<6; sector++) {
     	for (Int_t i=0;i<3;i++) mdcMods[sector][i] = mods[sector*4+i];
     }

     for (Int_t sector = 0; sector<6; sector++) {
     	aLoc[0]=sector;

     	// Add hit finders for sector
     	for (Int_t i=0;i<3;i++) {

     	   if (mdcMods[sector][i] != 0) {
     	      sprintf(buffer,"%s%i.%i","mdc.hitf",sector,i);
     	      finder=new HMdcHitF(buffer,"Mdc Hit finder");
     	      aLoc[1]=i;
     	      finder->setLoc(aLoc);
     	      tasks->add(finder);
     	   }
     	}
     }

     tasks->add( new HMdcSegmentF("mdc.h2s","mdc.h2s") );
  }

  return tasks;
}

ClassImp(HMdcTaskSet)
