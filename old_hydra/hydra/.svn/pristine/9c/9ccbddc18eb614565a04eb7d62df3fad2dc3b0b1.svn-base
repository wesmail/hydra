/**
 * @file   hdstrealdata.cc
 * @author Simon Lang
 * @date   Fri May  5 16:15:16 2006
 *
 * @modified Tatyana Galatyuk
 * @date     17-Jul-2007
 *
 * @brief  This Class provides Functions for a DST Production from real Data
 *         of Apr06 Beamtime
 *
 */

// Root headers
#include "TString.h"
#include "TSystem.h"

// Hydra headers
#include "hades.h"
#include "hdstrealdataapr06.h"
#include "hkicktaskset.h"
#include "hkicktrackbaseF.h"
#include "hmdcbitflipcor.h"
#include "hmdcsetup.h"
#include "hmdclookuptb.h"
#include "hmdc34clfinder.h"
#include "hmdctaskset.h"
#include "hpairfiller.h"
#include "hpairfilter.h"
#include "hpidalgstandcuts.h"
#include "hpidalgmomvsbeta.h"
#include "hpidparticlefiller.h"
#include "hpidtrackcleaner.h"
#include "hpidpdfmaker.h"
#include "hpidreconstructor.h"
#include "hpidskiptrigger.h"
#include "hpidtrackfiller.h"
#include "hpidtofrec.h"
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
ClassImp(HDstRealDataApr06)


void HDstRealDataApr06::setupTaskListGen1()
{
    HTriggerTaskSet* trigger_task_set  = new HTriggerTaskSet();
    HTofTaskSet*     tof_task_set      = new HTofTaskSet();
    HTofinoTaskSet*  tofino_task_set   = new HTofinoTaskSet();
    HMdcTaskSet*     mdc_task_set      = new HMdcTaskSet();
    HRichTaskSet*    rich_task_set     = new HRichTaskSet();
    HRichIPUTaskSet* rich_ipu_task_set = new HRichIPUTaskSet();
    HShowerTaskSet*  shower_task_set   = new HShowerTaskSet();
    HSplineTaskSet*  spline_task_set   = new HSplineTaskSet( "", "" );

   HTaskSet* mdc_bit_flip_correction_task = new HTaskSet("mdc bitflipcor",
							 "mdc bitflipcor" );
   mdc_bit_flip_correction_task->connect(
      new HMdcBitFlipCor( "mdcbitflipcor", "mdcbitflipcor" ) );  


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

   //-------------TOF reconstruction for PP data--------------------------
   // new version of the TofReconstructor, since v7_07
   HPidTofRec *pidtofrec = new HPidTofRec("pidtofreconstructor","pidtofreconstructor","nopidparam");
   pidtofrec->setQualityRichParams(150.,5.,5.,5.);

   HPidTrackCleaner *pidtrackcleaner = new HPidTrackCleaner();
   
   HQAMaker* qa_report_task = new HQAMaker( "qa.maker", "qa.maker" );
   qa_report_task->setOutputDir( const_cast<char*>(dstCfg.qaOutputDir.Data()));
   qa_report_task->setSamplingRate( 1 );
   qa_report_task->setIntervalSize( dstCfg.qaEventInterval );

   ////// now link together the task lists

   // New version of cluster finder
   HMdcLookUpTb::setUseFloatLevel();
   HMdc34ClFinder::setUseFloatLevel();

   HTaskSet* tasks = gHades->getTaskSet( "real" );

   tasks->add( trigger_task_set->make( "real" ) );
   tasks->add( tof_task_set->make( "real" ) );
   tasks->add( tofino_task_set->make() );
   tasks->add( rich_task_set->make( "real", "" ) );
   tasks->add( rich_ipu_task_set->make( "real" ) );
   tasks->add( shower_task_set->make( "", "" ) );
   tasks->add( mdc_bit_flip_correction_task );
   tasks->add( mdc_task_set->make( "rtdb", ""));
   tasks->add( spline_task_set->make("","spline,reslowb,&old,tofclust,runge") );
   tasks->add( new HPidTrackFiller( "NOCHI2SEG0,NOCHI2SEG1" ) );
   tasks->add( pidtofrec );
   tasks->add( pidtrackcleaner);

   tasks->add( qa_report_task );
   if (pdf_maker_task)
   {
      tasks->add( pdf_maker_task );
   }

   // each of the tasks should track its execution time
   tasks->isTimed( kTRUE );
}
