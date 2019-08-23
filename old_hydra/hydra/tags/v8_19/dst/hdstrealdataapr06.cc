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
#include "hpidevtinfofiller.h"
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
   pidtofrec->setQualityRichParams(150.,5.,5.,5.); // RingPatMat, RingPadNr, Centroid, RingverageCharge

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



void HDstRealDataApr06::setupTaskListGen2()
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
    mdc_bit_flip_correction_task->connect(new HMdcBitFlipCor("mdcbitflipcor",
							     "mdcbitflipcor"));


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
    pidtofrec->setQualityRichParams(200.,5.,5.,4.);  // RingPatMat, RingPadNr, Centroid, RingverageCharge

    HPidTrackCleaner  *pidtrackcleaner  = new HPidTrackCleaner();
    HPidEvtInfoFiller *pidevtinfofiller = new HPidEvtInfoFiller("PidEvtInfoFiller","PidEvtInfoFiller");
//    HMdcVertexWriter  *mdcvertexwriter  = new HMdcVertexWriter("MdcVertexWriter","MdcVertexWriter");  // will be done on thrack embedding level


    //------------------------Particle identification----------------------
    TString   pid_ntuple;
    TString   pair_filt_ntuple;

    // derive output and NTuple file name from input file name
    pid_ntuple = gSystem->BaseName( const_cast<char*>(dstCfg.inputUrl.Data()) );
    pid_ntuple.ReplaceAll( ".hld", 4, "_lep_nt.root", 12 );
    pid_ntuple.Prepend( "/" );
    pid_ntuple.Prepend( dstCfg.outputDir );

    pair_filt_ntuple = gSystem->BaseName( const_cast<char*>(dstCfg.inputUrl.Data()) );
    pair_filt_ntuple.ReplaceAll( ".hld", 4, "_pair_filt_nt.root", 18 );
    pair_filt_ntuple.Prepend( "/" );
    pair_filt_ntuple.Prepend( dstCfg.outputDir );

    TString opt_pid_rec = "pdf,CL,merge,ALG_RUNGEKUTTA";
    HPidReconstructor *pid_rec = new HPidReconstructor(opt_pid_rec.Data());

    Short_t nParticles[2] = {2,3};
    pid_rec->setParticleIds(nParticles,sizeof(nParticles)/sizeof(Short_t));

    TString cuts = "RICHRINGCUTS,TOFPVSBETA,TOFINOPVSBETA,SHOWERSUMDIFFVSP";
    HPidAlgStandCuts *pid_alg_stand_cuts = new HPidAlgStandCuts(pid_ntuple.Data(),cuts.Data());
    pid_rec->addAlgorithm(pid_alg_stand_cuts);

    HPidParticleFiller *part_filler=new HPidParticleFiller("RUNGEKUTTA");
    part_filler->setAlgorithm(7); // 7-check values from HPidAlgStandCuts algorithm
    part_filler->print();


    //------------------------Pair creation--------------------------------
    HPairFiller *pair_filler=new HPairFiller("HPairFiller","HPairFiller");
    HPairFilter *pair_filter=new HPairFilter("HPairFilter","HPairFilter");
    pair_filter->setQAFileName((Char_t *)pair_filt_ntuple.Data());


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
    tasks->add( pidtrackcleaner );
    tasks->add( pidevtinfofiller );
//    tasks->add( mdcvertexwriter );

    tasks->add(pid_rec);
    tasks->add(part_filler);
    tasks->add(pair_filler);
    tasks->add(pair_filter);


    tasks->add( qa_report_task );
    if (pdf_maker_task)
    {
	tasks->add( pdf_maker_task );
    }

    // each of the tasks should track its execution time
    tasks->isTimed( kTRUE );
}
