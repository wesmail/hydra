/**
 * @file   hdstembeddingsep05.cc
 * @author Simon Lang
 * @date   Wed Nov 14 14:44:11 2007
 * 
 * @brief  This Class provides Functions for a DST Production from 
 *         embedded Data
 * 
 * $Id: hdstembeddingsep05.cc,v 1.1 2007-11-14 14:13:41 slang Exp $
 *
 */


// Hydra headers
#include "hades.h"
#include "hdstembeddingsep05.h"
#include "hdstutilities.h"
#include "hmdc34clfinder.h"
#include "hmdcbitflipcor.h"
#include "hmdclookuptb.h"
#include "hmdctaskset.h"
#include "hpidevtinfofiller.h"
#include "hpidtrackfiller.h"
#include "hrichIPUtaskset.h"
#include "hrichtaskset.h"
#include "hshowertaskset.h"
#include "hsplinetaskset.h"
#include "hstarttaskset.h"
#include "htask.h"
#include "htaskset.h"
#include "htofinotaskset.h"
#include "htoftaskset.h"
#include "htriggertaskset.h"

using namespace std;


///_HADES_CLASS_DESCRIPTION 
////////////////////////////////////////////////////////////////////////////////
//
//  Functions for a DST Production from embedded Data of Beamtime Sep05.
//
////////////////////////////////////////////////////////////////////////////////
ClassImp(HDstEmbeddingSep05)


void HDstEmbeddingSep05::setupTaskListGen1()
{
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

   // use new MDC cluster finder with level adaption
   HMdcLookUpTb::setUseFloatLevel();        // inner MDCs
   HMdc34ClFinder::setUseFloatLevel();      // outer MDCs

   // this includes the following tasks:
   // Kick Tracking, Kick Track Clustering,
   // Spline Tracking, Spline Clustering,
   // Runge-Kutta Tracking, MetaMatch Procedure
   HTask* tracking_tasks = spline_task_set->make(
      "", "spline,reslowb,&old,tofclust,runge" );
 

   ////// now link together the task lists

   HTaskSet* tasks = gHades->getTaskSet( "real" );

   tasks->add( start_task_set->make( "", "controlhists" ) );
   tasks->add( trigger_task_set->make( "real" ) );
   tasks->add( tof_task_set->make( "real", "clust" ) );
   tasks->add( tofino_task_set->make() );
   tasks->add( rich_task_set->make("real", "") );
   tasks->add( rich_ipu_task_set->make( "real" ) );
   tasks->add( shower_task_set->make("","leprecognition,lowshowerefficiency"));
   tasks->add( mdc_bit_flip_correction_task );
   tasks->add( mdc_task_set->make( "rtdb" ) );
   tasks->add( tracking_tasks );
   tasks->add( new HPidTrackFiller( "makesimcategory,NOCHI2SEG1" ) );
   tasks->add( new HPidEvtInfoFiller );

   // each of the tasks should track its execution time
   tasks->isTimed( kTRUE );
}
