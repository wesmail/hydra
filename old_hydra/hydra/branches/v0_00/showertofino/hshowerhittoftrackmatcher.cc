#include "hshowerhittoftrackmatcher.h"

#include "hruntimedb.h"
#include "hevent.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hshowerdetector.h"
#include "htofinodetector.h"
#include "hcategory.h"
#include "hlinearcategory.h"
#include "hlocation.h"
#include "hshowerhittof.h"
#include "hshowertrack.h"
#include "hshowerhittoftrack.h"
#include "hdebug.h"
#include "hades.h"
#include "hiterator.h"
#include "showerdef.h"
#include "showertofinodef.h"

ClassImp(HShowerHitTofTrackMatcher)

///////////////////////////////////////////////////////////////////////
//HShowerHitTofTrackMatcher
//
// HShowerHitTofTrackMatcher reconstructor searches 
// data describing the same pad in HShowerHitTof and HShowerTrack categories
// and joins them to one object. 
// Results are stored in the HShowerHitTofTrack category.
//
//////////////////////////////////////////////////////////////////////

HShowerHitTofTrackMatcher::HShowerHitTofTrackMatcher(Text_t *name,Text_t *title) :
  HReconstructor(name,title)
{
   fHitIter=NULL;
   fTrackIter=NULL;
   m_zeroLoc.set(0);
}

HShowerHitTofTrackMatcher::HShowerHitTofTrackMatcher()
{
   fHitIter=NULL;
   fTrackIter=NULL;
   m_zeroLoc.set(0);
}


HShowerHitTofTrackMatcher::~HShowerHitTofTrackMatcher(void) {
     if (fHitIter) delete fHitIter;
     if (fTrackIter) delete fTrackIter;
}

Bool_t HShowerHitTofTrackMatcher::init() {
  // creates the HShowerHitTof, HShowerTrack and HShowerHitTofTrack categories 
  // and adds them to the current event,
  // creates an iterator which loops over all local maxima
  // and an iterator which loops over track number information

    printf("initialization of Shower/Tofino hit-track matcher\n");
    
    HShowerDetector *pShowerDet = (HShowerDetector*)gHades->getSetup()
                                                  ->getDetector("Shower");
    HTofinoDetector *pTofinoDet = (HTofinoDetector*)gHades->getSetup()
                                                  ->getDetector("Tofino");

    m_pTrackCat=gHades->getCurrentEvent()->getCategory(catShowerTrack);
    if (!m_pTrackCat) {
      m_pTrackCat=pShowerDet->buildCategory(catShowerTrack);

      if (!m_pTrackCat) return kFALSE;
      else gHades->getCurrentEvent()
                 ->addCategory(catShowerTrack, m_pTrackCat, "Shower");
    }

    m_pHitCat=gHades->getCurrentEvent()->getCategory(catShowerHitTof);
    if (!m_pHitCat) {
      m_pHitCat=pShowerDet->buildCategory(catShowerHitTof);

      if (!m_pHitCat) return kFALSE;
      else gHades->getCurrentEvent()
                 ->addCategory(catShowerHitTof, m_pHitCat, "Tofino");
    }

    m_pHitTrackCat = gHades->getCurrentEvent()
                           ->getCategory(catShowerHitTofTrack);
    if (!m_pHitTrackCat) {
//      m_pHitTrackCat = pTofinoDet->buildCategory(catShowerHitTofTrack);
      m_pHitTrackCat = new HLinearCategory("HShowerHitTofTrack", 1000);

      if (!m_pHitTrackCat) return kFALSE;
      else gHades->getCurrentEvent()
                 ->addCategory(catShowerHitTofTrack, m_pHitTrackCat, "Tofino");
    }

    fHitIter=(HIterator*)m_pHitCat->MakeIterator();
    fTrackIter=(HIterator*)m_pTrackCat->MakeIterator();

    return kTRUE;
}

Bool_t HShowerHitTofTrackMatcher::finalize(void) {
   return kTRUE;
}


void HShowerHitTofTrackMatcher::addHitTrack(HShowerHitTof* pHit, 
         HShowerTrack* pTrack) {
// create one object which contains data
// from HShowerHitTof and HShowerTrack objects
  
    HShowerHitTofTrack *pHitTrack;

    pHitTrack=(HShowerHitTofTrack *)m_pHitTrackCat->getNewSlot(m_zeroLoc);
    if (pHitTrack!=NULL) {
      pHitTrack=new(pHitTrack) HShowerHitTofTrack;

      *pHitTrack = *pHit;
      pHitTrack->setTrack(pTrack->getTrack());
    }
}



Int_t HShowerHitTofTrackMatcher::execute()
{
//searching all pairs in HShowerHitTof and HShowerTrack, which
//have the same address
//Because HShowerHitTof and HShowerTrack are sorted by m_nAddress
//reading data in tables is done once!

  HShowerHitTof *pHit;
  HShowerTrack *pTrack;
 
  fHitIter->Reset();
  fTrackIter->Reset();

  pTrack = (HShowerTrack *)fTrackIter->Next();
  while((pHit = (HShowerHitTof*)fHitIter->Next())) { //loop over hits
    //if tracks and hits are sorted resetting fTrackIter is not necessery

    do {
       if (pTrack && pHit->getAddress()==pTrack->getAddress()) { 
         addHitTrack(pHit, pTrack); //ok, the same track
         break; 
       }
     }
    while((pTrack = (HShowerTrack *)fTrackIter->Next())) ;

    //searching other tracks with the same address number
    while((pTrack = (HShowerTrack *)fTrackIter->Next())) {
       if (pHit->getAddress()==pTrack->getAddress()) { 
         addHitTrack(pHit, pTrack);
       } else break;
    }

  }

  return 0;
}



