/**
 * @file   hdstsimulationsep05.cc
 * @author Simon Lang
 * @date   Fri May  5 16:07:11 2006
 * 
 * @brief  This Class provides Functions for a DST Production from 
 *         simulated Data
 * 
 * $Id: hdstsimulation.cc,v 1.1.1.1 2006-09-08 07:39:37 halo Exp $
 *
 */

// Root headers
#include "TSystem.h"
#include "TUrl.h"

// Hydra headers
#include "hades.h"
#include "hruntimedb.h"
#include "hdstsimulation.h"
#include "hrootsource.h"

#include "htaskset.h"
#include "htoftaskset.h"
#include "htofinotaskset.h"
#include "hrichtaskset.h"
#include "hrichIPUtaskset.h"
#include "hrichIPUparthresholds.h"
#include "hrichIPUparlocmax.h"
#include "hmdctaskset.h"
#include "hshowertaskset.h"
#include "htriggertaskset.h"
#include "muEmulationExp.h"
#include "muEmulationSim.h"
#include "hsplinetaskset.h"
#include "hkicktaskset.h"
#include "hrichevtfilter.h"
#include "hpidtrackfiller.h"
#include "hqamaker.h"
#include "hrktrackBF.h"

using namespace std;

///////////////////////////////////////////////////////////////////////////////
//
//  Functions for a DST Production from simulated Data.
//
//
//_____________________________________________________________________________

// this macro is used to implement Root's class features (e.g. the streamer)
ClassImp(HDstSimulationSep05)



void HDstSimulationSep05::setupInput()
{
   // Setup data IO files. This function determines and opens the input
   // and output files of the analysed data. All necessary information are
   // provided by dstCfg structure; derived information are stored in dstCfg,
   // too.

   HRootSource* source = new HRootSource;
   TUrl         url( dstCfg.inputUrl.Data(), kTRUE );
   
   if (strcmp( url.GetProtocol(), "file" ) == 0)
   {
      source->replaceHeaderVersion( 0, kTRUE );
      source->setGlobalRefId( dstCfg.referenceRunId );

      // check given file input
      if (gSystem->AccessPathName( url.GetFile() ))
      {
	 cerr << "==> ERROR: Input file '" << url.GetFile()
	      << "' does not exist!" << endl;
	 terminateProduction( 2 );
      }
      source->setDirectory(
	 const_cast<Text_t*>(gSystem->DirName( url.GetFile() )) );
      if (!source->addFile(
	     const_cast<Text_t*>(gSystem->BaseName( url.GetFile() )) ))
      {
	 cerr << "==> ERROR: Could not add data source file: " << url.GetUrl()
	      << endl;
	 terminateProduction( 2 );
      }
   }
   else
   {
      cerr << "==> ERROR: Invalid input URL: " << url.GetUrl() << endl;
      terminateProduction( 2 );
   }

   gHades->setDataSource( source );
}



void HDstSimulationSep05::setupTaskListGen0()
{
   // get master task set
   masterTaskSet = gHades->getTaskSet( "simulation" );

   // each of the tasks should track its execution time
   masterTaskSet->isTimed( kTRUE );

   // TOF
   HTofTaskSet* tof_task_set = new HTofTaskSet();
   masterTaskSet->add( tof_task_set->make( "simulation" ) );

   // TOFino
   HTofinoTaskSet* tofino_task_set = new HTofinoTaskSet();
   masterTaskSet->add( tofino_task_set->make( "", "simulation" ) );

   // Event Filter: Simulate 1st level trigger, using multiplicity of
   // TOF and TOFino hits
   HTaskSet* event_filter_task_set =
      new HTaskSet( "EvtFilter", "List of Evt Filter Tasks" );
   event_filter_task_set->connect(
      new HRichEvtFilter( "Tof+Tofino_Mult", "Tof+Tofino_Mult", "tofmult", 18));
   masterTaskSet->add( event_filter_task_set );

   // RICH
   HRichTaskSet* rich_task_set = new HRichTaskSet();
   masterTaskSet->add( rich_task_set->make( "simulation", "noiseon" ));

   // RICH IPU
   HRichIPUTaskSet* rich_ipu_task_set = new HRichIPUTaskSet();
   masterTaskSet->add( rich_ipu_task_set->make( "simulation" ) );

   // MDC
   HMdcTaskSet* mdc_task_set = new HMdcTaskSet();
   masterTaskSet->add( mdc_task_set->make( "rtdb", "" ) );

   // Shower
   HShowerTaskSet* shower_task_set = new HShowerTaskSet();
   masterTaskSet->add( shower_task_set->make(
			    "", "simulation,leprecognition" ) );

   // Trigger
   HTriggerTaskSet* trigger_task_set = new HTriggerTaskSet();
   masterTaskSet->add( trigger_task_set->make( "simulation" ) );

   // medium resolution spline tracking
   HSplineTaskSet* spline_task_set = new HSplineTaskSet( "", "" );
   masterTaskSet->add( spline_task_set->make( "", "spline" ) );

   // low resolution kick plane tracking
   HKickTaskSet* kick_track_task_set = new HKickTaskSet();
   masterTaskSet->add( kick_track_task_set->make(
			    "", "reslowb&old,tofclust,simulation" ) );

   // high resolution Runge-Kutta tracking
   HTaskSet* runge_kutta_task_set = new HTaskSet( "rkTrack", "rkTrack" );
   runge_kutta_task_set->connect( new HRKTrackBF( "RK-tracking" ) );
   masterTaskSet->add( runge_kutta_task_set );

   // PID track candidate filler
   masterTaskSet->add( new HPidTrackFiller( "makesimcategory,NOCHI2SEG1" ) );

   // creation of quality assurance .pdf report files
   HQAMaker* qa_report_task = new HQAMaker( "qa.maker", "qa.maker" );
   qa_report_task->setOutputDir( const_cast<char*>(dstCfg.qaOutputDir.Data()));
   qa_report_task->setSamplingRate( 1 );
   qa_report_task->setIntervalSize( dstCfg.qaEventInterval );
   masterTaskSet->add( qa_report_task );
}



void HDstSimulationSep05::setupTaskListGen1()
{
   // get master task set
   masterTaskSet = gHades->getTaskSet( "simulation" );

   // each of the tasks should track its execution time
   masterTaskSet->isTimed( kTRUE );

   // TOF
   HTofTaskSet* tof_task_set = new HTofTaskSet();
   masterTaskSet->add( tof_task_set->make( "simulation" ) );

   // TOFino
   HTofinoTaskSet* tofino_task_set = new HTofinoTaskSet();
   masterTaskSet->add( tofino_task_set->make( "", "simulation" ) );

   // Event Filter: Simulate 1st level trigger, using multiplicity of
   // TOF and TOFino hits
   HTaskSet* event_filter_task_set =
      new HTaskSet( "EvtFilter", "List of Evt Filter Tasks" );
   event_filter_task_set->connect(
      new HRichEvtFilter( "Tof+Tofino_Mult", "Tof+Tofino_Mult", "tofmult", 4));
   masterTaskSet->add( event_filter_task_set );

   // RICH
   HRichTaskSet* rich_task_set = new HRichTaskSet();
   masterTaskSet->add( rich_task_set->make( "simulation", "noiseon" ));

   // RICH IPU
   HRichIPUTaskSet* rich_ipu_task_set = new HRichIPUTaskSet();
   masterTaskSet->add( rich_ipu_task_set->make( "simulation" ) );

   // MDC
   HMdcTaskSet* mdc_task_set = new HMdcTaskSet();
   masterTaskSet->add( mdc_task_set->make( "rtdb", "" ) );

   // Shower
   HShowerTaskSet* shower_task_set = new HShowerTaskSet();
   masterTaskSet->add( shower_task_set->make(
			    "", "simulation,leprecognition" ) );

   // TODO: Trigger
   HRichIPUParThresholds* rich_ipu_thresholds =
      new HRichIPUParThresholds( "RichIPUParThresholds_RICHIPUEmulation", "",
				 "RICHIPUEmulation" );
   gHades->getRuntimeDb()->addContainer( rich_ipu_thresholds );

   HRichIPUParLocMax* rich_ipu_location_max =
      new HRichIPUParLocMax( "RichIPUParLocMax_RICHIPUEmulation", "",
			     "RICHIPUEmulation" );
   gHades->getRuntimeDb()->addContainer( rich_ipu_location_max );

   HTriggerTaskSet* trigger_task_set = new HTriggerTaskSet();
   HTask*           trigger_tasks    = trigger_task_set->make( "simulation" );
   dynamic_cast<HMUEmulationSim*>(trigger_tasks->getTask( "trigger.emu" ))
      ->setParThresholds( rich_ipu_thresholds );
   dynamic_cast<HMUEmulationExp*>(trigger_tasks->getTask( "trigger.emu" ))
      ->setParLocMax( rich_ipu_location_max );

   masterTaskSet->add( trigger_task_set->make( "simulation" ) );

   // medium resolution spline tracking
   HSplineTaskSet* spline_task_set = new HSplineTaskSet( "", "" );
   masterTaskSet->add( spline_task_set->make( "", "spline" ) );

   // low resolution kick plane tracking
   HKickTaskSet* kick_track_task_set = new HKickTaskSet();
   masterTaskSet->add( kick_track_task_set->make(
			    "", "reslowb&old,tofclust,simulation" ) );

   // high resolution Runge-Kutta tracking
   HTaskSet* runge_kutta_task_set = new HTaskSet( "rkTrack", "rkTrack" );
   runge_kutta_task_set->connect( new HRKTrackBF( "RK-tracking" ) );
   masterTaskSet->add( runge_kutta_task_set );

   // PID track candidate filler
   masterTaskSet->add( new HPidTrackFiller( "makesimcategory,NOCHI2SEG1" ) );

   // creation of quality assurance .pdf report files
   HQAMaker* qa_report_task = new HQAMaker( "qa.maker", "qa.maker" );
   qa_report_task->setOutputDir( const_cast<char*>(dstCfg.qaOutputDir.Data()));
   qa_report_task->setSamplingRate( 1 );
   qa_report_task->setIntervalSize( 5000 );
   masterTaskSet->add( qa_report_task );
}

