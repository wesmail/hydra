/**
 * @file   hdstembedding.cc
 * @author Simon Lang
 * @date   Fri May  5 16:07:11 2006
 * 
 * @brief  This Class provides Functions for a DST Production from 
 *         real Data with an admixture of simulated Tracks
 * 
 * $Id: hdstembedding.cc,v 1.4 2009-09-09 14:13:37 jurkovic Exp $
 *
 */

// Root headers
#include "TObjString.h"
#include "TSystem.h"
#include "TUrl.h"

// Hydra headers
#include "hades.h"
#include "hruntimedb.h"
#include "hdstembedding.h"
#include "hldfilesource.h"
#include "hrootsource.h"
#include "htool.h"
#include "hdstutilities.h"

#include "hrichunpackercal99.h"
#include "hmdcunpacker.h"
#include "htofunpacker.h"
#include "htofinounpacker.h"
#include "hshowerunpacker.h"
#include "hstartunpacker.h"
#include "hmatchuunpacker.h"
#include "hwallunpacker.h"
#include "hlatchunpacker.h"
#include "htboxunpacker.h"

#include "hrichevtfilter.h"
#include "hpidtrackfiller.h"
#include "hqamaker.h"
#include "hrktrackBF.h"
#include "hpidpdfmaker.h"
#include "hmdcsetup.h"
#include "hmdcbitflipcor.h"
#include "hkicktrackbaseF.h"

#include "htaskset.h"
#include "hstarttaskset.h"
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
ClassImp(HDstEmbedding)



HDstEmbedding::HDstEmbedding() : HDstProduction()
{
   // Default Constructor: default unpacker IDs set

   // these are the default unpacker subevent ID's (derived from Sep05)
   for(UInt_t i = 0; i < sizeof(richUnpackers) / sizeof(Int_t); i++)
   {
      richUnpackers[i] = 100 + i;
   }
   for(UInt_t i = 0; i < sizeof(mdcUnpackers) / sizeof(Int_t); i++)
   {
      mdcUnpackers[i] = 201 + i;
   }
   for(UInt_t i = 0; i < sizeof(showerUnpackers) / sizeof(Int_t); i++)
   {
      showerUnpackers[i] = 300 + i;
   }
   tofUnpackers[0]    = 416;
   tofinoUnpackers[0] = 416;
   startUnpackers[0]  = 416;
   latchUnpackers[0]  = 416;
   tboxUnpackers[0]   = 416;
}



void HDstEmbedding::setupInput()
{
   // Setup first and second data input file for track embedding: This function
   // determines and opens one or more .hld and .root input files of real data
   // and HGeant output data and adds those files as data sources. All
   // necessary information are provided by dstCfg structure.
   // This function supports only local file locations for simulation data.

   HRootSource* root_source = NULL;
   HldSource*   hld_source  = NULL;
   TObjArray*   files       = NULL;
   TIterator*   file        = NULL;
   TObjString*  filename    = NULL;
   TObjArray*   local_files = NULL; 
   TIterator*   local_file  = NULL;
   TString      protocol;
   TString      directory;
   
   // switch analysis to embedding mode
   gHades->setEmbeddingMode( 1 );

   if (dstCfg.expId.IsNull())
   {
      Error( "setupInput", "Experiment ID not set!" );
      terminate( 2 );
   }


   ////// add HLD data source


   files = dstCfg.inputUrl.Tokenize( " \t" );
   file  = files->MakeIterator();
   file->Reset();
   while ((filename = (TObjString*)(file->Next())))
   {
      TUrl url( filename->GetString().Data(), kTRUE );

      if (protocol.IsNull())
      {
	 protocol = url.GetProtocol();
      }
      else if (protocol.CompareTo( url.GetProtocol() ) != 0)
      {
	 Error( "setupInput", "Only one type of input protocol allowed!" );
	 terminate( 2 );
      }
	 
      if (protocol.CompareTo( "file" ) == 0)
      {
	 if (!hld_source)
	 {
	    hld_source = new HldFileSource;
	    ((HldFileSource*)hld_source)->setDirectory(
	       const_cast<Text_t*>(gSystem->DirName( url.GetFile() )) );
	    directory = gSystem->DirName( url.GetFile() );
	 }
	 
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

	    if (dstCfg.referenceRunId)
	    {
	       ((HldFileSource*)hld_source)->addFile(
		  const_cast<Text_t*>(gSystem->BaseName(
					 filename->GetString().Data() )),
		  dstCfg.referenceRunId );
	    }
	    else
	    {
	       ((HldFileSource*)hld_source)->addFile(
	          const_cast<Text_t*>(gSystem->BaseName(
					 filename->GetString().Data() )) );
	    }
	 }
	 delete local_file;
	 delete local_files;
      }
#ifdef RFIO_SUPPORT
      else if (protocol.CompareTo( "rfio" ) == 0)
      {
	 if (!hld_source)
	 {
	    TString location;
	    hld_source = new HldRFIOFileSource;

	    // extract robot host and archive name
	    ((HldRFIOFileSource*)hld_source)->setRobot( url.GetHost() );
	    location = url.GetFile();
	    location.Remove( TString::kLeading, '/' );
	    location.Resize( location.Index( '/' ) );
	    ((HldRFIOFileSource*)hld_source)->setArchive( location.Data() );

	    // remove archive name and extract directory and file name
	    location = url.GetFile();
	    location.Remove( TString::kLeading, '/' );
	    location.Replace( 0, location.Index( '/' ) + 1, "", 0 );
	    ((HldRFIOFileSource*)hld_source)->setDirectory(
	       const_cast<Text_t*>(gSystem->DirName( location.Data() )) );
	 }

	 ((HldRFIOFileSource*)hld_source)->addFile(
	    const_cast<Text_t*>(gSystem->BaseName( url.GetFile() )) );
      }
#endif // RFIO_SUPPORT
      else
      {
	 Error( "setupInput", "Invalid input URL: %s", url.GetUrl() );
	 terminate( 2 );
      }
   }
   delete file;


   if (dstCfg.richIsUsed)
   {
      for(UInt_t i = 0; i < sizeof(richUnpackers) / sizeof(Int_t); i++)
      {
        // HRichUnpackerCal99(ID,writeRaw)
        // writeRaw = kFALSE  --> write only HRichCal category
        // writeRaw = kTRUE   --> write HRichRaw and HRichCal category
         hld_source->addUnpacker( new HRichUnpackerCal99( richUnpackers[i] , kTRUE ) );
      }
   }


   if  (dstCfg.mdcIsUsed)
   {
      for(UInt_t i = 0; i < sizeof(mdcUnpackers) / sizeof(Int_t); i++)
      {
	 HMdcUnpacker* mdc_unpacker =
	    new HMdcUnpacker( mdcUnpackers[i], kFALSE );
	 mdc_unpacker->enableExcludeBlackList( dstCfg.expId.Data() );
	 mdc_unpacker->enableRecoverBlackList( dstCfg.expId.Data() );
	 mdc_unpacker->setDecodeVersion( 1 );
	 mdc_unpacker->setQuietMode();
	 hld_source->addUnpacker( mdc_unpacker );
      }
   }

   if (dstCfg.showerIsUsed)
   {
      for(UInt_t i = 0; i < sizeof(showerUnpackers) / sizeof(Int_t); i++)
      {
	 hld_source->addUnpacker( new HShowerUnpacker( showerUnpackers[i] ) );
      }
   }

   if (dstCfg.tofIsUsed)
   {
      for(UInt_t i = 0; i < sizeof(tofUnpackers) / sizeof(Int_t); i++)
      {
	 hld_source->addUnpacker( new HTofUnpacker( tofUnpackers[i] ) );
      }
   }

   if (dstCfg.tofinoIsUsed)
   {
      for(UInt_t i = 0; i < sizeof(tofinoUnpackers) / sizeof(Int_t); i++)
      {
	 hld_source->addUnpacker( new HTofinoUnpacker( tofinoUnpackers[i] ) );
      }
   }

   if (dstCfg.startIsUsed)
   {
      for(UInt_t i = 0; i < sizeof(startUnpackers) / sizeof(Int_t); i++)
      {
	 hld_source->addUnpacker( new HStartUnpacker( startUnpackers[i] ) );
      }
   }

   if (dstCfg.triggerIsUsed)
   {
      for(UInt_t i = 0; i < sizeof(latchUnpackers) / sizeof(Int_t); i++)
      {
	 hld_source->addUnpacker( new HLatchUnpacker( latchUnpackers[i] ) );
      }
      for(UInt_t i = 0; i < sizeof(tboxUnpackers) / sizeof(Int_t); i++)
      {
	 hld_source->addUnpacker( new HTBoxUnpacker( tboxUnpackers[i] ) );
      }
      hld_source->addUnpacker( new HMatchUUnpacker( 512, 0, kTRUE ) );
   }

   gHades->setDataSource( hld_source );


   ////// add Root data source


   files = dstCfg.embeddingInput.Tokenize( " \t" );
   file  = files->MakeIterator();
   file->Reset();
   while ((filename = (TObjString*)file->Next()))
   {
      TUrl url( filename->GetString().Data(), kTRUE );

      if (!root_source)
      {
	 root_source = new HRootSource( kTRUE, kTRUE );
	 root_source->setDirectory(
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

	    if (!root_source->addFile(
		   const_cast<Text_t*>(gSystem->BaseName(
					  filename->GetString().Data() )) ))
	    {
	       Error( "setupInput", Form( "Adding input file '%s' failed!",
					  filename->GetString().Data() ) );
	       terminate( 2 );
	    }
	 }
	 delete local_file;
	 delete local_files;
      }
      else
      {
	 Error( "setupInput", "Input URL must refer to a local file!" );
	 terminate( 2 );
      }
   }
   delete file;

   gHades->setSecondDataSource( root_source );
}



void HDstEmbedding::setupTaskList()
{
   // The default task list for DST production from real data where simulated
   // tracks are embedded into the data stream.
   // All actions and definitions for a specific beam time must
   // go to a derived class, where other task list can be
   // implemented.

   HTriggerTaskSet* trigger_task_set  = new HTriggerTaskSet();
   HStartTaskSet*   start_task_set    = new HStartTaskSet();
   HTofTaskSet*     tof_task_set      = new HTofTaskSet();
   HTofinoTaskSet*  tofino_task_set   = new HTofinoTaskSet();
   HRichTaskSet*    rich_task_set     = new HRichTaskSet();
   HRichIPUTaskSet* rich_ipu_task_set = new HRichIPUTaskSet();
   HMdcTaskSet*     mdc_task_set      = new HMdcTaskSet();
   HShowerTaskSet*  shower_task_set   = new HShowerTaskSet();
   HSplineTaskSet*  spline_task_set   = new HSplineTaskSet( "", "" );

   HTaskSet* mdc_bit_flip_correction_task =
      new HTaskSet( "mdc bitflipcor", "mdc bitflipcor" );
   mdc_bit_flip_correction_task->connect(
      new HMdcBitFlipCor( "mdcbitflipcor", "mdcbitflipcor" ) );  

   // this includes the following tasks:
   // Kick Tracking, Kick Track Clustering,
   // Spline Tracking, Spline Clustering,
   // Runge-Kutta Tracking, MetaMatch Procedure
   HTask* tracking_tasks = spline_task_set->make(
      "", "spline,reslowb,&old,tofclust,runge" );
 
   HQAMaker* qa_report_task = new HQAMaker( "qa.maker", "qa.maker" );
   qa_report_task->setOutputDir( const_cast<char*>(dstCfg.qaOutputDir.Data()));
   qa_report_task->setSamplingRate( 1 );
   qa_report_task->setIntervalSize( dstCfg.qaEventInterval );

   HPidPdfMaker* pdf_maker_task = NULL;
   if (dstCfg.createControlNTuple)
      pdf_maker_task = new HPidPdfMaker(
	 HDstUtilities::GetFileStem(
	    dstCfg.qaOutputDir.IsNull()  ?
	    dstCfg.outputDir  :  dstCfg.qaOutputDir, dstCfg.inputUrl )
	 + "_ntuple.root", kFALSE );


   ////// now link together the task lists

   HTaskSet* task_on_all_events = gHades->getTaskSet( "all" );
   task_on_all_events->add( start_task_set->make( "", "controlhists" ) );
   
   HTaskSet* tasks = gHades->getTaskSet( "real" );

   tasks->add( trigger_task_set->make( "real" ) );
   tasks->add( tof_task_set->make( "real", "clust" ) );
   tasks->add( tofino_task_set->make() );
   tasks->add( rich_task_set->make("real", "") );
   tasks->add( rich_ipu_task_set->make( "real" ) );
   tasks->add( shower_task_set->make("","leprecognition,lowshowerefficiency"));
   tasks->add( mdc_bit_flip_correction_task );
   tasks->add( mdc_task_set->make( "rtdb" ) );
   tasks->add( tracking_tasks );
   tasks->add( new HPidTrackFiller("makesimcategory,NOCHI2SEG0,NOCHI2SEG1") );
   tasks->add( qa_report_task );
   if (pdf_maker_task)
      tasks->add( pdf_maker_task );
   
   tasks->isTimed( kTRUE );
}
