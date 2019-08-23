/**
 * @file   hdstsimulationsep05.cc
 * @author Simon Lang
 * @date   Fri May  5 16:07:11 2006
 * 
 * @brief  This Class provides Functions for a DST Production from 
 *         simulated Data
 * 
 * $Id: hdstsimulationsep05.cc,v 1.4 2007-11-14 13:41:36 slang Exp $
 *
 */

// Root headers
#include "TSystem.h"

// Hydra headers
#include "hades.h"
#include "hruntimedb.h"
#include "hdstsimulationsep05.h"
#include "hdstutilities.h"

#include "hrichevtfilter.h"
#include "hmdclookuptb.h"
#include "hmdc34clfinder.h"
#include "hrktrackBF.h"
#include "hqamaker.h"
#include "hpidtrackfiller.h"
#include "hpidpdfmaker.h"
#include "hpidevtinfofiller.h"

#include "muEmulationExp.h"
#include "muEmulationSim.h"
#include "hrichIPUtaskset.h"
#include "hrichIPUparthresholds.h"
#include "hrichIPUparlocmax.h"

#include "htaskset.h"
#include "htoftaskset.h"
#include "htofinotaskset.h"
#include "hrichtaskset.h"
#include "hrichIPUtaskset.h"
#include "hmdctaskset.h"
#include "hshowertaskset.h"
#include "htriggertaskset.h"
#include "hsplinetaskset.h"
#include "hkicktaskset.h"

using namespace std;



///_HADES_CLASS_DESCRIPTION 
////////////////////////////////////////////////////////////////////////////////
//
//  Functions for a DST Production from simulated Data of Beamtime Sep05.
//
//
////////////////////////////////////////////////////////////////////////////////
ClassImp(HDstSimulationSep05)



void HDstSimulationSep05::setupTaskListGen0()
{
   HTofTaskSet*     tof_task_set        = new HTofTaskSet();
   HTofinoTaskSet*  tofino_task_set     = new HTofinoTaskSet();
   HRichTaskSet*    rich_task_set       = new HRichTaskSet();
   HRichIPUTaskSet* rich_ipu_task_set   = new HRichIPUTaskSet();
   HMdcTaskSet*     mdc_task_set        = new HMdcTaskSet();
   HShowerTaskSet*  shower_task_set     = new HShowerTaskSet();
   HTriggerTaskSet* trigger_task_set    = new HTriggerTaskSet();
   HSplineTaskSet*  spline_task_set     = new HSplineTaskSet( "", "" );
   HKickTaskSet*    kick_track_task_set = new HKickTaskSet();

   // high resolution Runge-Kutta tracking
   HTaskSet* runge_kutta_task_set = new HTaskSet( "rkTrack", "rkTrack" );
   runge_kutta_task_set->connect( new HRKTrackBF( "RK-tracking" ) );

   // creation of quality assurance .pdf report files
   HQAMaker* qa_report_task = new HQAMaker( "qa.maker", "qa.maker" );
   qa_report_task->setOutputDir( const_cast<char*>(dstCfg.qaOutputDir.Data()));
   qa_report_task->setSamplingRate( 1 );
   qa_report_task->setIntervalSize( dstCfg.qaEventInterval );

   // Event Filter: Simulate 1st level trigger, using multiplicity of
   // TOF and TOFino hits (unusable for Pluto)
   HTaskSet* event_filter_task_set = NULL;
   if (dstCfg.dstId.Contains( "URQMD", TString::kIgnoreCase ))
   {
      event_filter_task_set =
	 new HTaskSet( "EvtFilter", "List of Evt Filter Tasks" );
      event_filter_task_set->connect(
	 new HRichEvtFilter( "Tof+Tofino_Mult","Tof+Tofino_Mult","tofmult",18));
   }

   // create control ntuple
   HPidPdfMaker* pdf_maker_task = NULL;
   if (dstCfg.createControlNTuple)
   {
      TString ntuple_filename = 
	 dstCfg.qaOutputDir.IsNull()  ?  dstCfg.outputDir : dstCfg.qaOutputDir;

      ntuple_filename += gSystem->BaseName(
	 const_cast<char*>(dstCfg.inputUrl.Data()) );
      ntuple_filename.ReplaceAll( ".hld", 4, "_ntuple.root", 12 );

      pdf_maker_task = new HPidPdfMaker( ntuple_filename.Data(), kFALSE );
   }

   ////// now link together the task lists

   HTaskSet* tasks = gHades->getTaskSet( "simulation" );

   tasks->add( tof_task_set->make( "simulation" ) );
   tasks->add( tofino_task_set->make( "", "simulation" ) );
   if (dstCfg.dstId.Contains( "URQMD", TString::kIgnoreCase ))
   {
      tasks->add( event_filter_task_set );
   }
   tasks->add( rich_task_set->make( "simulation", "noiseon" ));
   tasks->add( rich_ipu_task_set->make( "simulation" ) );
   tasks->add( mdc_task_set->make( "rtdb", "" ) );
   tasks->add( shower_task_set->make(
			  "", "simulation,leprecognition" ) );
   tasks->add( trigger_task_set->make( "simulation" ) );
   tasks->add( spline_task_set->make( "", "spline" ) );
   tasks->add( kick_track_task_set->make(
			  "", "reslowb&old,tofclust,simulation" ) );
   tasks->add( runge_kutta_task_set );
   tasks->add( new HPidTrackFiller( "makesimcategory,NOCHI2SEG1" ) );
   tasks->add( qa_report_task );
   if (pdf_maker_task)
   {
      tasks->add( pdf_maker_task );
   }

   tasks->isTimed( kTRUE );
}



void HDstSimulationSep05::setupTaskListGen1()
{
   HTofTaskSet*     tof_task_set        = new HTofTaskSet();
   HTofinoTaskSet*  tofino_task_set     = new HTofinoTaskSet();
   HRichTaskSet*    rich_task_set       = new HRichTaskSet();
   HRichIPUTaskSet* rich_ipu_task_set   = new HRichIPUTaskSet();
   HMdcTaskSet*     mdc_task_set        = new HMdcTaskSet();
   HShowerTaskSet*  shower_task_set     = new HShowerTaskSet();
   HSplineTaskSet*  spline_task_set     = new HSplineTaskSet( "", "" );
   HKickTaskSet*    kick_track_task_set = new HKickTaskSet();

   // high resolution Runge-Kutta tracking
   HTaskSet* runge_kutta_task_set = new HTaskSet( "rkTrack", "rkTrack" );
   runge_kutta_task_set->connect( new HRKTrackBF( "RK-tracking" ) );

   // creation of quality assurance .pdf report files
   HQAMaker* qa_report_task = new HQAMaker( "qa.maker", "qa.maker" );
   qa_report_task->setOutputDir( const_cast<char*>(dstCfg.qaOutputDir.Data()));
   qa_report_task->setSamplingRate( 1 );
   qa_report_task->setIntervalSize( dstCfg.qaEventInterval );

   // Event Filter: Simulate 1st level trigger, using multiplicity of
   // TOF and TOFino hits (unusable for Pluto)
   HTaskSet* event_filter_task_set = NULL;
   if (dstCfg.dstId.Contains( "URQMD", TString::kIgnoreCase ))
   {
      event_filter_task_set =
	 new HTaskSet( "EvtFilter", "List of Evt Filter Tasks" );
      event_filter_task_set->connect(
	 new HRichEvtFilter( "Tof+Tofino_Mult","Tof+Tofino_Mult","tofmult",18));
   }

   // create control ntuple
   HPidPdfMaker* pdf_maker_task = NULL;
   if (dstCfg.createControlNTuple)
   {
      TString ntuple_filename = 
	 dstCfg.qaOutputDir.IsNull()  ?  dstCfg.outputDir : dstCfg.qaOutputDir;

      ntuple_filename += gSystem->BaseName(
	 const_cast<char*>(dstCfg.inputUrl.Data()) );
      ntuple_filename.ReplaceAll( ".hld", 4, "_ntuple.root", 12 );

      pdf_maker_task = new HPidPdfMaker( ntuple_filename.Data(), kFALSE );
   }

   // TODO: Trigger IPU simulation
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

   ////// now link together the task lists

   HTaskSet* tasks = gHades->getTaskSet( "simulation" );

   tasks->add( tof_task_set->make( "simulation" ) );
   tasks->add( tofino_task_set->make( "", "simulation" ) );
   if (dstCfg.dstId.Contains( "URQMD", TString::kIgnoreCase ))
   {
      tasks->add( event_filter_task_set );
   }
   tasks->add( rich_task_set->make( "simulation", "noiseon" ));
   tasks->add( rich_ipu_task_set->make( "simulation" ) );
   tasks->add( mdc_task_set->make( "rtdb", "" ) );
   tasks->add( shower_task_set->make(
			  "", "simulation,leprecognition" ) );
   tasks->add( trigger_task_set->make( "simulation" ) );
   tasks->add( spline_task_set->make( "", "spline" ) );
   tasks->add( kick_track_task_set->make(
			  "", "reslowb&old,tofclust,simulation" ) );
   tasks->add( runge_kutta_task_set );
   tasks->add( new HPidTrackFiller( "makesimcategory,NOCHI2SEG1" ) );
   tasks->add( qa_report_task );
   if (pdf_maker_task)
   {
      tasks->add( pdf_maker_task );
   }

   tasks->isTimed( kTRUE );
}



void HDstSimulationSep05::setupTaskListGen2()
{
   // Task list for simulated DST production 'gen2' - based on Hydra 8.13

   HRichTaskSet*    rich_task_set     = new HRichTaskSet();
   HRichIPUTaskSet* rich_ipu_task_set = new HRichIPUTaskSet();
   HMdcTaskSet*     mdc_task_set      = new HMdcTaskSet();
   HTofTaskSet*     tof_task_set      = new HTofTaskSet();
   HTofinoTaskSet*  tofino_task_set   = new HTofinoTaskSet();
   HShowerTaskSet*  shower_task_set   = new HShowerTaskSet();
   HSplineTaskSet*  spline_task_set   = new HSplineTaskSet( "", "" );

   // use new MDC cluster finder with level adaption
   HMdcLookUpTb::setUseFloatLevel();        // inner MDCs
   HMdc34ClFinder::setUseFloatLevel();      // outer MDCs

   // this includes the following tasks:
   // Kick Tracking, Kick Track Clustering,
   // Spline Tracking, Spline Clustering,
   // Runge-Kutta Tracking, MetaMatch Procedure
   HTask* tracking_tasks = spline_task_set->make(
      "", "spline,reslowb,&old,tofclust,runge,simulation" );
   
   HQAMaker* qa_report_task = new HQAMaker( "qa.maker", "qa.maker" );
   qa_report_task->setOutputDir( const_cast<char*>(dstCfg.qaOutputDir.Data()));
   qa_report_task->setSamplingRate( 1 );
   qa_report_task->setIntervalSize( dstCfg.qaEventInterval );

   HPidPdfMaker* pdf_maker_task = NULL;
   if (dstCfg.createControlNTuple)
      pdf_maker_task = new HPidPdfMaker(
	 HDstUtilities::GetFileStem( dstCfg.outputDir, dstCfg.inputUrl )
	 + "_ntuple.root", kFALSE );

   // Event Filter: Simulate 1st level trigger, using multiplicity of
   // TOF and TOFino hits (unusable for Pluto)
   HTaskSet* event_filter_task_set = NULL;
   if (dstCfg.dstId.Contains( "URQMD", TString::kIgnoreCase ))
   {
      event_filter_task_set =
	 new HTaskSet( "EvtFilter", "List of Evt Filter Tasks" );
      event_filter_task_set->connect(
	 new HRichEvtFilter("Tof+Tofino_Mult","Tof+Tofino_Mult","tofmult",18));
   }

   ////// now link together the task lists

   HTaskSet* tasks = gHades->getTaskSet( "simulation" );
   
   tasks->add( tof_task_set->make( "simulation" ) );
   tasks->add( tofino_task_set->make( "", "simulation" ) );
   if (dstCfg.dstId.Contains( "URQMD", TString::kIgnoreCase ))
      tasks->add( event_filter_task_set );
   tasks->add( rich_task_set->make( "simulation", "noiseon" ));
   tasks->add( rich_ipu_task_set->make( "simulation" ) );
   tasks->add( shower_task_set->make(
      "", "simulation,leprecognition,lowshowerefficiency" ) );
   tasks->add( mdc_task_set->make( "rtdb" ) );
   tasks->add( tracking_tasks );
   tasks->add( new HPidTrackFiller( "makesimcategory,NOCHI2SEG1" ) );
   tasks->add( new HPidEvtInfoFiller ); 
   tasks->add( qa_report_task );
   if (pdf_maker_task)
      tasks->add( pdf_maker_task );

   // each of the tasks should track its execution time
   tasks->isTimed( kTRUE );
}
