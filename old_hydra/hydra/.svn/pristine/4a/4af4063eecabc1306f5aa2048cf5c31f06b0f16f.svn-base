// $Id: hpidevtinfofiller.cc,v 1.6 2009-07-15 11:40:21 halo Exp $
//
//_HADES_CLASS_DESCRIPTION 
/////////////////////////////////////////////////////////////////////////////
// HPidEvtInfoFiller                                                      
//
// Reconstructor calculates detector hit multiplicity, track (hpidtrackcand) 
// multiplicity and raction plane informations for each event. 
// At the moment Forward Wall and Reaction Plane information is not calculated.
// 
// These variables are stored in the HPidEvtInfo output category.
// 
// Author: Jacek Otwinowski 18/06/2007
// Revised by Simon Lang, 22/06/2007
//
////////////////////////////////////////////////////////////////////////////

#include "TString.h"
#include "TIterator.h"
#include "TArrayI.h"

#include "hpidevtinfo.h"
#include "hpidevtinfofiller.h"

#include "richdef.h"
#include "piddef.h"
#include "walldef.h"
#include "tofdef.h"
#include "tofinodef.h"
#include "showertofinodef.h"
#include "hmdcdef.h"
#include "hmdctrackddef.h"
#include "hgeantdef.h"

#include "hades.h"
#include "hcategory.h"
#include "hevent.h"
#include "hlocation.h"
#include "hlinearcategory.h"

#include "hshowerhittoftrack.h"
#include "hrichdirclus.h"
#include "hrichhit.h"
#include "hmdcclus.h"
#include "hmdcseg.h"
#include "hpidtrackcand.h"
#include "htofhit.h"
#include "htofinocal.h"
#include "hwallhit.h"


ClassImp(HPidEvtInfoFiller)
  
//-----------------------------------------------------------------------------
HPidEvtInfoFiller::HPidEvtInfoFiller() : HReconstructor() 
{
   clearMembers();
}


//-----------------------------------------------------------------------------
HPidEvtInfoFiller::HPidEvtInfoFiller(const Text_t* name,const Text_t* title)
   : HReconstructor(name, title) 
{
   clearMembers();
}

HPidEvtInfoFiller::~HPidEvtInfoFiller()
{
   delete pWallHitIter;
   delete pRichHitIter;
   delete pRichClusIter;
   delete pMdcClusIter;
   delete pMdcSegIter;
   delete pTofHitIter;
   delete pTofinoCalIter;
   delete pShowerHitTofIter;
   delete pPidTrackCandIter;

   clearMembers();
}
// ---------------------------------------------------------------------------
void HPidEvtInfoFiller::clearMembers()
{ 
   zeroLoc.set( 0 );
   bAnalyseSimData = kFALSE;

   pWallHitIter      = 0;
   pRichHitIter      = 0;   
   pRichClusIter     = 0;   
   pMdcClusIter      = 0;   
   pMdcSegIter       = 0;   
   pTofHitIter       = 0;   
   pTofinoCalIter    = 0;
   pShowerHitTofIter = 0;   
   pPidTrackCandIter = 0;
	

   pPidEvtInfoCat = 0;	
}


// ---------------------------------------------------------------------------
Bool_t HPidEvtInfoFiller::finalize(void)
{

   return kTRUE;
}


// ---------------------------------------------------------------------------
Bool_t HPidEvtInfoFiller::init(void)
{
   HCategory* cat   = 0;
   HEvent*    event = gHades->getCurrentEvent();

   // ckeck for Geant information - if present, then these are simulation data
   bAnalyseSimData = Bool_t(event->getCategory( catGeantKine ));

   // create iterators to input categories
   if ((cat = event->getCategory( catWallHit )))
      pWallHitIter = cat->MakeIterator();
   else
      Info( "init", "Category 'catWallHit' not found, "
	    "thus information not transfered HPidEvtInfo" );

   if ((cat = event->getCategory( catRichHit )))
      pRichHitIter = cat->MakeIterator();
   else
      Info( "init", "Category 'catRichHit' not found, "
	    "thus information not transfered to HPidEvtInfo" );

   if ((cat = event->getCategory( catRichDirClus )))
      pRichClusIter = cat->MakeIterator();
   else
      Info( "init", "Category 'catRichDirClus' not found, "
	    "thus information not transfered to HPidEvtInfo" );

   if ((cat = event->getCategory( catMdcClus )))
      pMdcClusIter = cat->MakeIterator();
   else
      Info( "init", "Category 'catMdcClus' not found, "
	    "thus information not transfered to HPidEvtInfo" );

   if ((cat = event->getCategory( catMdcSeg )))
      pMdcSegIter = cat->MakeIterator();
   else
      Info( "init", "Category 'catMdcSeg' not found, "
	    "thus information not transfered to HPidEvtInfo" );

   if ((cat = event->getCategory( catTofHit )))
      pTofHitIter = cat->MakeIterator();
   else
      Info( "init", "Category 'catTofHit' not found, "
	    "thus information not transfered to HPidEvtInfo" );

   if ((cat = event->getCategory( catTofinoCal )))
      pTofinoCalIter = cat->MakeIterator();
   else
      Info( "init", "Category 'catTofinoCal' not found, "
	    "thus information not transfered to HPidEvtInfo" );

   if ((cat = event->getCategory( catPidTrackCand )))
      pPidTrackCandIter = cat->MakeIterator();
   else
      Info( "init", "Category 'catPidTrackCand' not found, "
	    "thus information not transfered to HPidEvtInfo" );

   if (bAnalyseSimData)
   {
      if ((cat = event->getCategory( catShowerHitTofTrack )))
	 pShowerHitTofIter = cat->MakeIterator();
      else
	 Info( "init", "Category 'catShowerHitTofTrack' not found, "
	       "thus information not transfered to HPidEvtInfo" );
   }
   else
   {
      if ((cat = event->getCategory( catShowerHitTof )))
	 pShowerHitTofIter = cat->MakeIterator();
      else
	 Info( "init", "Category 'catShowerHitTof' not found, "
	       "thus information not transfered to HPidEvtInfo" );
   }


   // make output category
   pPidEvtInfoCat = event->getCategory( catPidEvtInfo );
   if(!pPidEvtInfoCat)
      pPidEvtInfoCat = new HLinearCategory( "HPidEvtInfo", 1 ); 
  
   if(!pPidEvtInfoCat)
   {
      Error( "init", "Cannot make catPidEvtInfo category" );
      return kFALSE;
   }
   else
   { 
      event->addCategory( catPidEvtInfo, pPidEvtInfoCat, "Pid" );
   }
  
   return kTRUE;
}


// ---------------------------------------------------------------------------
Int_t HPidEvtInfoFiller::execute(void)
{
   HWallHit           *wall_hit       = 0;
   HRichHit           *rich_hit       = 0;
   HRichDirClus       *rich_clus      = 0;
   HMdcClus           *mdc_clus       = 0;
   HMdcSeg            *mdc_seg        = 0;
   HShowerHitTofTrack *shower_hit     = 0;
   HTofHit            *tof_hit        = 0;
   HTofinoCal         *tofino_hit     = 0;
   HPidTrackCand      *pid_track_cand = 0;   
   HPidEvtInfo 	      *pPidEvtInfo    = 0;

   Int_t wall_hits                                    = 0;
   Int_t rings                                        = 0;
   Int_t rich_rings[MAX_SECTOR]                       = { 0 };
   Int_t rich_corr_rings[MAX_SECTOR]                  = { 0 };
   Int_t rich_clusters[MAX_SECTOR]                    = { 0 };
   Int_t mdc_clusters[MAX_SECTOR][MAX_IOSEG]          = { { 0 }, { 0 } };
   Int_t mdc_segments_fitted[MAX_SECTOR][MAX_IOSEG]   = { { 0 }, { 0 } };
   Int_t mdc_segments_unfitted[MAX_SECTOR][MAX_IOSEG] = { { 0 }, { 0 } };
   Int_t tof_hits[MAX_SECTOR]                         = { 0 };
   Int_t tofino_hits[MAX_SECTOR]                      = { 0 };
   Int_t shower_hits[MAX_SECTOR]                      = { 0 };
   Int_t pid_track_cands[MAX_SECTOR]                  = { 0 };
   Int_t selected_pid_track_cands[MAX_SECTOR]         = { 0 };

   TArrayI  pUsedRichHit;
   Int_t    idxRich;

   // fill counters
   if (pWallHitIter)
   {
      pWallHitIter->Reset();
      while ((wall_hit = (HWallHit*)pWallHitIter->Next()))
	 ++wall_hits;
   }

   if (pRichHitIter)
   {
      pRichHitIter->Reset();
      while ((rich_hit = (HRichHit*)pRichHitIter->Next())) 
      {
         ++rings;
	 ++rich_rings[rich_hit->getSector()];
      }
   }

   if (pRichClusIter)
   {
      pRichClusIter->Reset();
      while ((rich_clus = (HRichDirClus*)pRichClusIter->Next())) 
	 ++rich_clusters[rich_clus->getSector()];
   }

   if (pMdcClusIter)
   {
      pMdcClusIter->Reset();
      while ((mdc_clus = (HMdcClus*)pMdcClusIter->Next())) 
	 ++mdc_clusters[mdc_clus->getSec()][mdc_clus->getIOSeg()];
   }

   if (pMdcSegIter)
   {
      pMdcSegIter->Reset();
      while ((mdc_seg = (HMdcSeg*)pMdcSegIter->Next()))
      {
	 if (mdc_seg->getChi2() == -1)
	    ++mdc_segments_unfitted[mdc_seg->getSec()][mdc_seg->getIOSeg()];
	 else
	    ++mdc_segments_fitted[mdc_seg->getSec()][mdc_seg->getIOSeg()];
      }
   }

   if (pTofHitIter)
   {
      pTofHitIter->Reset();
      while ((tof_hit = (HTofHit*)pTofHitIter->Next()))
	 ++tof_hits[(Int_t)tof_hit->getSector()]; 
   }
   if (pTofinoCalIter)
   {
      pTofinoCalIter->Reset();
      while ((tofino_hit = (HTofinoCal*)pTofinoCalIter->Next()))
	 ++tofino_hits[(Int_t)tofino_hit->getSector()];
   }

   if (pShowerHitTofIter)
   {
      pShowerHitTofIter->Reset();
      while ((shower_hit = (HShowerHitTofTrack*)pShowerHitTofIter->Next()))
	 ++shower_hits[(Int_t)shower_hit->getSector()]; 
   }

   pUsedRichHit.Set(rings);
   pUsedRichHit.Reset();

   if (pPidTrackCandIter)
   {
      pPidTrackCandIter->Reset();
      while ((pid_track_cand = (HPidTrackCand*)pPidTrackCandIter->Next()))
      {
	  ++pid_track_cands[pid_track_cand->getHitData()->getSector()];
	  if(pid_track_cand->isFlagBit(HPidTrackCand::kIsUsed))
          {
	      ++selected_pid_track_cands[pid_track_cand->getHitData()->getSector()];
	  }
          if ( (idxRich = pid_track_cand->getHitData()->getIndRICH()) > -1 && 
               pid_track_cand->getHitData()->getRingCorrelation(4) == 1    &&
               pUsedRichHit.At(idxRich) == 0 )
          {
             pUsedRichHit.AddAt(1, idxRich);
             ++rich_corr_rings[pid_track_cand->getHitData()->getSector()];
          }
      }
   }


   // fill output category
   if ((pPidEvtInfo = (HPidEvtInfo*)pPidEvtInfoCat->getNewSlot( zeroLoc )))
   {
      pPidEvtInfo = new(pPidEvtInfo) HPidEvtInfo;
      pPidEvtInfo->setFWallMult( wall_hits );
      pPidEvtInfo->setRichMult( rich_rings );
      pPidEvtInfo->setRichCorrMult( rich_corr_rings );
      pPidEvtInfo->setRichClusMult( rich_clusters );
      pPidEvtInfo->setTofMult( tof_hits );
      pPidEvtInfo->setTofinoMult( tofino_hits );
      pPidEvtInfo->setShowerMult( shower_hits );
      pPidEvtInfo->setPidTrackCandMult( pid_track_cands );
      pPidEvtInfo->setSelectedPidTrackCandMult( selected_pid_track_cands );
      pPidEvtInfo->setMdcClusMult( mdc_clusters );
      pPidEvtInfo->setMdcSegFittedMult( mdc_segments_fitted );
      pPidEvtInfo->setMdcSegUnfittedMult( mdc_segments_unfitted );
   }
   else
   {
      Error( "execute", "Couldn't get a new slot from 'catPidEvtInfo'!" );
      exit( 2 );
   }

   return 0;
}
