/**
 * @file   hdstrealdata.cc
 * @author Simon Lang
 * @date   Fri May  5 16:15:16 2006
 * 
 * @brief  This Class provides Functions for a DST Production from real Data
 * 
 * $Id: hdstrealdata.cc,v 1.1.1.1 2006-09-08 07:39:37 halo Exp $
 *
 */

// Root headers
#include "TSystem.h"
#include "TUrl.h"

// Hydra headers
#include "hades.h"
#include "hdstrealdata.h"
#include "hldfilesource.h"
#include "hqamaker.h"
#include "hmdcsetup.h"
#include "hmdcbitflipcor.h"
#include "hkicktrackbaseF.h"
#include "hrktrackBF.h"
#include "hpidtrackfiller.h"
#include "hpidpdfmaker.h"

#include "hrichunpackerraw99.h"
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

#ifdef RFIO_SUPPORT
#  include "TRFIOFile.h"
#  include "hldrfiofilesource.h"
#endif

using namespace std;

///////////////////////////////////////////////////////////////////////////////
//
//  Functions for a DST Production from real Data.
//
//_____________________________________________________________________________

// this macro is used to implement Root's class features (e.g. the streamer)
ClassImp(HDstRealDataSep05)

static const Int_t RICH_UNPACKERS_SEP05[] = {
   100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111 };

static const Int_t MDC_UNPACKERS_SEP05[] = {
   201, 202, 203, 204, 205, 206,
   207, 208, 209, 210, 211, 212,
   213, 214, 215, 216, 217, 218,
   219, 220, 221, 222, 223, 224
};

static const Int_t SHOWER_UNPACKERS_SEP05[] = {
   300, 301, 302, 303, 304, 305, 306, 307, 308, 309, 310, 311 };

static const Int_t TOF_UNPACKERS_SEP05[]    = { 416 };
static const Int_t TOFINO_UNPACKERS_SEP05[] = { 416 };
static const Int_t START_UNPACKERS_SEP05[]  = { 416 };
static const Int_t LATCH_UNPACKERS_SEP05[]  = { 416 };
static const Int_t TBOX_UNPACKERS_SEP05[]   = { 416 };


void HDstRealDataSep05::setupInput()
{
   // Setup data IO files. This function determines and opens the input
   // and output files of the analysed data. All necessary information are
   // provided by dstCfg structure; derived information are stored in dstCfg,
   // too.

   HldSource* source = NULL;
   TUrl       url( dstCfg.inputUrl.Data(), kTRUE );
   
   if (dstCfg.expId.IsNull())
   {
      cerr << "==> ERROR: Experiment ID not set!" << endl;
      terminateProduction( 2 );
   }

   if (strcmp( url.GetProtocol(), "file" ) == 0)
   {
      // check given file input
      if (gSystem->AccessPathName( url.GetFile() ))
      {
	 cerr << "==> ERROR: Input file '" << url.GetFile()
	      << "' does not exist!" << endl;
	 terminateProduction( 2 );
      }

      source = new HldFileSource;
      ((HldFileSource*)source)->setDirectory(
	 const_cast<Text_t*>(gSystem->DirName( url.GetFile() )) );
      ((HldFileSource*)source)->addFile(
	 const_cast<Text_t*>(gSystem->BaseName( url.GetFile() )) );
   }
#ifdef RFIO_SUPPORT
   else if (strcmp( url.GetProtocol(), "rfio" ) == 0)
   {
      TString location;

      source = new HldRFIOFileSource;

      // extract robot host and archive name
      ((HldRFIOFileSource*)source)->setRobot( url.GetHost() );
      location = url.GetFile();
      location.Remove( TString::kLeading, '/' );
      location.Resize( location.Index( '/' ) );
      ((HldRFIOFileSource*)source)->setArchive( location.Data() );

      // remove archive name and extract directory and file name
      location = url.GetFile();
      location.Remove( TString::kLeading, '/' );
      location.Replace( 0, location.Index( '/' ) + 1, "", 0 );
      ((HldRFIOFileSource*)source)->setDirectory(
	 const_cast<Text_t*>(gSystem->DirName( location.Data() )) );
      ((HldRFIOFileSource*)source)->addFile(
	 const_cast<Text_t*>(gSystem->BaseName( location.Data() )) );
   }
#endif // RFIO_SUPPORT
   else
   {
      cerr << "==> ERROR: Invalid input URL: " << url.GetUrl() << endl;
      terminateProduction( 2 );
   }

   for(UInt_t i = 0; i < sizeof(RICH_UNPACKERS_SEP05) / sizeof(Int_t); i++)
   {
      source->addUnpacker( new HRichUnpackerCal99( RICH_UNPACKERS_SEP05[i] ) );
      source->addUnpacker( new HRichUnpackerRaw99( RICH_UNPACKERS_SEP05[i] ) );
   }

   for(UInt_t i = 0; i < sizeof(MDC_UNPACKERS_SEP05) / sizeof(Int_t); i++)
   {
      HMdcUnpacker* mdc_unpacker =
         new HMdcUnpacker( MDC_UNPACKERS_SEP05[i], kFALSE );
      mdc_unpacker->enableExcludeBlackList( dstCfg.expId.Data() );
      mdc_unpacker->enableRecoverBlackList( dstCfg.expId.Data() );
      mdc_unpacker->setDecodeVersion( 1 );
      mdc_unpacker->setQuietMode();
      source->addUnpacker( mdc_unpacker );
   }

   for(UInt_t i = 0; i < sizeof(SHOWER_UNPACKERS_SEP05) / sizeof(Int_t); i++)
   {
      source->addUnpacker( new HShowerUnpacker( SHOWER_UNPACKERS_SEP05[i] ) );
   }

   for(UInt_t i = 0; i < sizeof(TOF_UNPACKERS_SEP05) / sizeof(Int_t); i++)
   {
      source->addUnpacker( new HTofUnpacker( TOF_UNPACKERS_SEP05[i] ) );
   }

   for(UInt_t i = 0; i < sizeof(TOFINO_UNPACKERS_SEP05) / sizeof(Int_t); i++)
   {
      source->addUnpacker( new HTofinoUnpacker( TOFINO_UNPACKERS_SEP05[i] ) );
   }

   for(UInt_t i = 0; i < sizeof(START_UNPACKERS_SEP05) / sizeof(Int_t); i++)
   {
      source->addUnpacker( new HStartUnpacker( START_UNPACKERS_SEP05[i] ) );
   }

   for(UInt_t i = 0; i < sizeof(LATCH_UNPACKERS_SEP05) / sizeof(Int_t); i++)
   {
      source->addUnpacker( new HLatchUnpacker( LATCH_UNPACKERS_SEP05[i] ) );
   }

   for(UInt_t i = 0; i < sizeof(TBOX_UNPACKERS_SEP05) / sizeof(Int_t); i++)
   {
      source->addUnpacker( new HTBoxUnpacker( TBOX_UNPACKERS_SEP05[i] ) );
   }

   source->addUnpacker( new HMatchUUnpacker( 512, 0, kTRUE ) );

   gHades->setDataSource( source );
}



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

   HMdcTaskSet* mdc_task_set = new HMdcTaskSet();
   HMdcSetup*   mdc_setup    = mdc_task_set->getMdcSetup();
   mdc_setup->getMdcTrackFinderSet()->setNLayers( &dstCfg.mdcTestLayer[0][0] );
   mdc_setup->getMdc12FitSet()->setMdc12FitSet( 1, 1, 0, kTRUE, kFALSE );
   mdc_setup->getMdcTrackFinderSet()->setNLevel( dstCfg.mdcTestLevel );

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

   HTaskSet* task_on_all_events = gHades->getTaskSet( "all" );
   task_on_all_events->add( start_task_set->make( "", "controlhists" ) );
   
   masterTaskSet = gHades->getTaskSet( "real" );

   masterTaskSet->add( trigger_task_set->make( "real" ) );
   masterTaskSet->add( tof_task_set->make( "real", "clust" ) );
   masterTaskSet->add( tofino_task_set->make() );
   masterTaskSet->add( rich_task_set->make("real", "") );
   masterTaskSet->add( rich_ipu_task_set->make( "real" ) );
   masterTaskSet->add( shower_task_set->make() );
   masterTaskSet->add( mdc_bit_flip_correction_task );
   masterTaskSet->add( mdc_task_set->make(
     "", "real, fit, magneton, CombinedClusters, timecuts, nostartandcal" ) );
   masterTaskSet->add( spline_task_set->make( "", "spline" ) );
   masterTaskSet->add( kick_task_set->make("", "reslowb&old, tofclust") );
   masterTaskSet->add( kick_track_task_set );
   masterTaskSet->add( runge_kutta_task_set );
   masterTaskSet->add( new HPidTrackFiller( "NOCHI2SEG0,NOCHI2SEG1" ) );
   masterTaskSet->add( qa_report_task );
   
   // each of the tasks should track its execution time
   masterTaskSet->isTimed( kTRUE );
}



void HDstRealDataSep05::setupTaskListGen1()
{
   // not yet implemented
}
