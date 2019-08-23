/**
 * @file   hdstsimulationsep05.cc
 * @author Simon Lang
 * @date   Fri May  5 16:07:11 2006
 * 
 * @brief  This Class provides Functions for a DST Production from 
 *         simulated Data
 * 
 * $Id: hdstsimulation.cc,v 1.8 2007-02-02 13:19:30 slang Exp $
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
#include "htool.h"

#include "hrichevtfilter.h"
#include "hpidtrackfiller.h"
#include "hqamaker.h"
#include "hrktrackBF.h"
#include "hpidpdfmaker.h"

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



//_HADES_CLASS_DESCRIPTION 
////////////////////////////////////////////////////////////////////////////////
//
//  Base Class for a DST Production from simulated Data.
//
////////////////////////////////////////////////////////////////////////////////
ClassImp(HDstSimulation)



void HDstSimulation::setupInput()
{
   // Setup data input file: This function determines and opens
   // one or more .root input files of HGeant output data. All necessary
   // information are provided by dstCfg structure.
   // This function supports only local file locations.

   HRootSource* source      = NULL;
   TObjArray*   files       = NULL;
   TIterator*   file        = NULL;
   TObjString*  filename    = NULL;
   TObjArray*   local_files = NULL; 
   TIterator*   local_file  = NULL;
   TString      directory;
   
   files = dstCfg.inputUrl.Tokenize( " \t" );
   file  = files->MakeIterator();
   file->Reset();
   while ((filename = (TObjString*)file->Next()))
   {
      TUrl url( filename->GetString().Data(), kTRUE );

      if (!source)
      {
	 source = new HRootSource;
	 source->replaceHeaderVersion( 0, kTRUE );
	 source->setGlobalRefId( dstCfg.referenceRunId );
	 source->setDirectory(
	    const_cast<Text_t*>(gSystem->DirName( url.GetFile() )) );
	 directory = gSystem->DirName( url.GetFile() );
      }
      if (strcmp( url.GetProtocol(), "file" ) == 0)
      {
	 local_files = HTool::glob( url.GetFile() );
	 if (!local_files)
	 {
	    Error( "setupInput", "Input file(s) do not exist!" );
	    terminate( 2 );
	 }
	 local_file = local_files->MakeIterator();
	 while ((filename = (TObjString*)local_file->Next()))
	 {
	    if (directory.CompareTo(
		   gSystem->DirName( filename->GetString().Data() ) ) != 0)
	    {
	       Error( "setupInput", "Only one input directory allowed!" );
	       terminate( 2 );
	    }
	    
	    if (!source->addFile(
		   const_cast<Text_t*>(gSystem->BaseName(
					  filename->GetString().Data() )) ))
	    {
	       Error( "setupInput", "Adding input file failed!" );
	       terminate( 2 );
	    }
	 }
	 delete local_files;
	 delete local_file;
      }
      else
      {
	 Error( "setupInput", "Input URL must refer to a local file!" );
	 terminate( 2 );
      }
   }
   delete file;

   gHades->setDataSource( source );
}



void HDstSimulation::setupTaskList()
{
   // The default task list for DST production from simulated data.
   // All actions and definitions for a specific beam time must
   // go to a derived class, where other task list can be
   // implemented.

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
