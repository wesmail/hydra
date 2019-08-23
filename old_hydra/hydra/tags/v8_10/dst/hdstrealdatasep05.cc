/**
 * @file   hdstrealdata.cc
 * @author Simon Lang
 * @date   Fri May  5 16:15:16 2006
 * 
 * @brief  This Class provides Functions for a DST Production from real Data
 *         of Sep05 Beamtime
 * 
 * $Id: hdstrealdatasep05.cc,v 1.6 2006-12-17 21:15:51 slang Exp $
 *
 */

// Root headers
#include "TString.h"
#include "TSystem.h"

// Hydra headers
#include "hades.h"
#include "hdstrealdatasep05.h"
#include "hkicktaskset.h"
#include "hkicktrackbaseF.h"
#include "hmdcbitflipcor.h"
#include "hmdcsetup.h"
#include "hmdctaskset.h"
#include "hpairfiller.h"
#include "hpairfilter.h"
#include "hpidalgstandcuts.h"
#include "hpidparticlefiller.h"
#include "hpidpdfmaker.h"
#include "hpidreconstructor.h"
#include "hpidskiptrigger.h"
#include "hpidtrackfiller.h"
#include "hqamaker.h"
#include "hrichIPUtaskset.h"
#include "hrichtaskset.h"
#include "hrktrackBF.h"
#include "hshowertaskset.h"
#include "hsplinetaskset.h"
#include "hstarttaskset.h"
#include "htaskset.h"
#include "htofinotaskset.h"
#include "htoftaskset.h"
#include "htriggertaskset.h"

using namespace std;

///////////////////////////////////////////////////////////////////////////////
//
//  Functions for a DST Production from real Data.
//
//_____________________________________________________________________________
ClassImp(HDstRealDataSep05)


void HDstRealDataSep05::setupTaskListGen0()
{
   HTriggerTaskSet* trigger_task_set  = new HTriggerTaskSet();
   HStartTaskSet*   start_task_set    = new HStartTaskSet();
   HTofTaskSet*     tof_task_set      = new HTofTaskSet();
   HTofinoTaskSet*  tofino_task_set   = new HTofinoTaskSet();
   HRichTaskSet*    rich_task_set     = new HRichTaskSet();
   HRichIPUTaskSet* rich_ipu_task_set = new HRichIPUTaskSet();
   HShowerTaskSet*  shower_task_set   = new HShowerTaskSet();
   HSplineTaskSet*  spline_task_set   = new HSplineTaskSet( "", "" );
   HKickTaskSet*    kick_task_set     = new HKickTaskSet();

   HTaskSet* mdc_bit_flip_correction_task =
      new HTaskSet( "mdc bitflipcor", "mdc bitflipcor" );
   mdc_bit_flip_correction_task->connect(
      new HMdcBitFlipCor( "mdcbitflipcor", "mdcbitflipcor" ) );  

   // override database settings of wome MDC parameter for test purposes
   Int_t mdc_level[4]    = { 15, 50, 10, 30 };
   Int_t mdc_layer[6][4] = { { 4, 5, 6, 6 }, 
			     { 4, 5, 6, 6 }, 
			     { 4, 5, 6, 6 },
			     { 4, 5, 6, 6 }, 
			     { 4, 5, 6, 6 }, 
			     { 4, 5, 6, 6 } };
   HMdcTaskSet* mdc_task_set = new HMdcTaskSet();
   HMdcSetup*   mdc_setup    = mdc_task_set->getMdcSetup();
   mdc_setup->getMdcTrackFinderSet()->setNLayers( &mdc_layer[0][0] );
   mdc_setup->getMdc12FitSet()->setMdc12FitSet( 1, 1, 0, kTRUE, kFALSE );
   mdc_setup->getMdcTrackFinderSet()->setNLevel( mdc_level );

   HTaskSet* kick_track_task_set =
      new HTaskSet( "kicktracktask", "kicktracktask" );
   kick_track_task_set->connect(
      new HKickTrackBaseF( "kicktrackbase", "kicktrackbase" ) );

   // High resolution Runge Kutta tracking:
   //     Initial momentum is taken from:
   //     0 - self guess (use this, if neither kick plane nor spline tracking
   //         available
   //     1 - from kick plane tracking
   //     2 - from spline tracking (default)
   HTaskSet* runge_kutta_task_set = new HTaskSet( "rkTrack", "rkTrack" );
   runge_kutta_task_set->connect( new HRKTrackBF( "RK-tracking" ) );
 
   HQAMaker* qa_report_task = new HQAMaker( "qa.maker", "qa.maker" );
   qa_report_task->setOutputDir( const_cast<char*>(dstCfg.qaOutputDir.Data()));
   qa_report_task->setSamplingRate( 1 );
   qa_report_task->setIntervalSize( dstCfg.qaEventInterval );

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

   HTaskSet* tasks = gHades->getTaskSet( "real" );

   tasks->add( start_task_set->make( "", "controlhists" ) );
   tasks->add( trigger_task_set->make( "real" ) );
   tasks->add( tof_task_set->make( "real", "clust" ) );
   tasks->add( tofino_task_set->make() );
   tasks->add( rich_task_set->make( "real", "" ) );
   tasks->add( rich_ipu_task_set->make( "real" ) );
   tasks->add( shower_task_set->make() );
   tasks->add( mdc_bit_flip_correction_task );
   tasks->add( mdc_task_set->make( "", "real, fit, magneton, "
				   "CombinedClusters, timecuts, startandcal"));
   tasks->add( spline_task_set->make( "", "spline" ) );
   tasks->add( kick_task_set->make("", "reslowb&old, tofclust") );
   tasks->add( kick_track_task_set );
   tasks->add( runge_kutta_task_set );
   tasks->add( new HPidTrackFiller( "SKIPNOLEPTON,NOCHI2SEG1" ) );
   tasks->add( qa_report_task );
   if (pdf_maker_task)
   {
      tasks->add( pdf_maker_task );
   }
   
   // each of the tasks should track its execution time
   tasks->isTimed( kTRUE );
}



void HDstRealDataSep05::setupTaskListPostGen0()
{
   Short_t             nParticles[2]       = { 2, 3 };
   HTaskSet*           tasks               = gHades->getTaskSet( "real" );
   HTaskSet*           pair_analysis_tasks = NULL;
   HPidReconstructor*  pid_rec             = NULL;
   HPidParticleFiller* part_filler         = NULL;
   HPairFiller*        pair_filler         = NULL;
   HPairFilter*        pair_filter         = NULL;
   TString             pid_ntuple;
   TString             pair_ntuple;

   // derive output and NTuple file name from input file name
   pid_ntuple = gSystem->BaseName( dstCfg.inputUrl.Data() );
   pid_ntuple.ReplaceAll( ".root", 5, "_lep_nt.root", 12 );
   pid_ntuple.Prepend( "/" );
   pid_ntuple.Prepend( dstCfg.outputDir );

   pair_ntuple = gSystem->BaseName( dstCfg.inputUrl.Data() );
   pair_ntuple.ReplaceAll( ".root", 5, "_pair_nt.root", 13 );
   pair_ntuple.Prepend( "/" );
   pair_ntuple.Prepend( dstCfg.outputDir );

   if (dstCfg.dstId.Contains( "LVL2", TString::kIgnoreCase ))
   {
      HPidSkipTrigger *pSkipLVL1 = new HPidSkipTrigger(
	 "skip trigger", "skip trigger" );
      tasks->add( pSkipLVL1 );
   }

   if (dstCfg.dstId.Contains( "KICK", TString::kIgnoreCase ))
   {
      pid_rec     = new HPidReconstructor( "pdf,CL,merge,ALG_KICK" );
      part_filler = new HPidParticleFiller( "KICK" );
   }

   if (dstCfg.dstId.Contains( "SPLINE", TString::kIgnoreCase ))
   {
      pid_rec     = new HPidReconstructor ( "pdf,CL,merge,ALG_SPLINE" );
      part_filler = new HPidParticleFiller( "SPLINE" );
   }

   if (dstCfg.dstId.Contains( "RUNGEKUTTA", TString::kIgnoreCase ) ||
       dstCfg.dstId.Contains( "RK",         TString::kIgnoreCase ))
   {
      pid_rec     = new HPidReconstructor ( "pdf,CL,merge,ALG_RUNGEKUTTA" );
      part_filler = new HPidParticleFiller( "RUNGEKUTTA" );
   }

   if (dstCfg.dstId.Contains( "STANDARDCUTS",  TString::kIgnoreCase ) ||
       dstCfg.dstId.Contains( "STANDARD CUTS", TString::kIgnoreCase ) ||
       dstCfg.dstId.Contains( "STANDCUTS",     TString::kIgnoreCase ))
   {
      if (pid_rec)
      {
	 pid_rec->addAlgorithm(new HPidAlgStandCuts( pid_ntuple ));
      }
      if (part_filler)
      {
	 part_filler->setAlgorithm( 7 ); // only standard cuts
      }
   }

   pid_rec->setParticleIds( nParticles, sizeof(nParticles) / sizeof(Short_t) );
   part_filler->print();

   if (dstCfg.dstId.Contains( "PAIR", TString::kIgnoreCase ))
   {
      pair_analysis_tasks = new HTaskSet( "Pair Analysis", "Pair Analysis" );
      pair_filler         = new HPairFiller( "HPairFiller", "HPairFiller" );
      pair_filter         = new HPairFilter( "HPairFilter", "HPairFilter" );

      pair_filter->setQAFileName( (Char_t *)pair_ntuple.Data() );
      pair_analysis_tasks->add( pair_filler );
      pair_analysis_tasks->add( pair_filter );
   }

   if (pid_rec && part_filler)
   {
      tasks->add( pid_rec     );
      tasks->add( part_filler );
   }

   if (pair_filler)
   {
      tasks->add( pair_analysis_tasks );
   }

   tasks->isTimed( kTRUE );
   tasks->print();
}



void HDstRealDataSep05::setupTaskListGen1()
{
   HTriggerTaskSet* trigger_task_set  = new HTriggerTaskSet();
   HStartTaskSet*   start_task_set    = new HStartTaskSet();
   HTofTaskSet*     tof_task_set      = new HTofTaskSet();
   HTofinoTaskSet*  tofino_task_set   = new HTofinoTaskSet();
   HRichTaskSet*    rich_task_set     = new HRichTaskSet();
   HRichIPUTaskSet* rich_ipu_task_set = new HRichIPUTaskSet();
   HShowerTaskSet*  shower_task_set   = new HShowerTaskSet();
   HSplineTaskSet*  spline_task_set   = new HSplineTaskSet( "", "" );
   HKickTaskSet*    kick_task_set     = new HKickTaskSet();
   HMdcTaskSet*     mdc_task_set      = new HMdcTaskSet();

   HTaskSet* mdc_bit_flip_correction_task =
      new HTaskSet( "mdc bitflipcor", "mdc bitflipcor" );
   mdc_bit_flip_correction_task->connect(
      new HMdcBitFlipCor( "mdcbitflipcor", "mdcbitflipcor" ) );  

   HTaskSet* kick_track_task_set =
      new HTaskSet( "kicktracktask", "kicktracktask" );
   kick_track_task_set->connect(
      new HKickTrackBaseF( "kicktrackbase", "kicktrackbase" ) );

   HTaskSet* runge_kutta_task_set = new HTaskSet( "rkTrack", "rkTrack" );
   runge_kutta_task_set->connect( new HRKTrackBF( "RK-tracking" ) );
 
   HQAMaker* qa_report_task = new HQAMaker( "qa.maker", "qa.maker" );
   qa_report_task->setOutputDir( const_cast<char*>(dstCfg.qaOutputDir.Data()));
   qa_report_task->setSamplingRate( 1 );
   qa_report_task->setIntervalSize( dstCfg.qaEventInterval );

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

   HTaskSet* tasks = gHades->getTaskSet( "real" );

   tasks->add( start_task_set->make( "", "" ) );
   tasks->add( trigger_task_set->make( "real" ) );
   tasks->add( tof_task_set->make( "real", "clust" ) );
   tasks->add( tofino_task_set->make() );
   tasks->add( rich_task_set->make( "real", "" ) );
   tasks->add( rich_ipu_task_set->make( "real" ) );
   tasks->add( shower_task_set->make() );
   tasks->add( mdc_bit_flip_correction_task );
   tasks->add( mdc_task_set->make( "rtdb" ) );
   tasks->add( spline_task_set->make( "", "spline" ) );
   tasks->add( kick_task_set->make("", "reslowb&old, tofclust") );
   tasks->add( kick_track_task_set );
   tasks->add( runge_kutta_task_set );
   tasks->add( new HPidTrackFiller( "NOCHI2SEG1,NOCHI2SEG2" ) );
   tasks->add( qa_report_task );
   if (pdf_maker_task)
   {
      tasks->add( pdf_maker_task );
   }
   
   // each of the tasks should track its execution time
   tasks->isTimed( kTRUE );
}
