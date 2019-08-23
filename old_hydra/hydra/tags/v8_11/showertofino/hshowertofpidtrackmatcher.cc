//*-- AUTHOR : Jacek Otwinowski
//*-- Modified : 19/04/05 by Jacek Otwinowski
//
//_HADES_CLASS_DESCRIPTION 
///////////////////////////////////////////////////////////////////////
//
//HShowerTofPIDTrackMatcher
//
// HShowerTofPIDTrackMatcher reconstructor searches 
// data describing the same pad in HShowerTofPID and HShowerTrack categories
// and joins them to one object. 
// Results are stored in the HShowerTofPIDTrack category.
//
//////////////////////////////////////////////////////////////////////

#include "hshowertofpidtrackmatcher.h"
#include "hruntimedb.h"
#include "hevent.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hshowerdetector.h"
#include "htofinodetector.h"
#include "hcategory.h"
#include "hlinearcategory.h"
#include "hlocation.h"
#include "hshowertofpid.h"
#include "hshowertrack.h"
#include "hshowertofpidtrack.h"
#include "hdebug.h"
#include "hades.h"
#include "hiterator.h"
#include "showerdef.h"
#include "showertofinodef.h"

ClassImp(HShowerTofPIDTrackMatcher)

HShowerTofPIDTrackMatcher::HShowerTofPIDTrackMatcher(Text_t *name,Text_t *title) :
  HReconstructor(name,title)
{
   fHitIter=NULL;
   fTrackIter=NULL;
   m_zeroLoc.set(0);
}

HShowerTofPIDTrackMatcher::HShowerTofPIDTrackMatcher()
{
   fHitIter=NULL;
   fTrackIter=NULL;
   m_zeroLoc.set(0);
}


HShowerTofPIDTrackMatcher::~HShowerTofPIDTrackMatcher(void) {
     if (fHitIter) delete fHitIter;
     if (fTrackIter) delete fTrackIter;
}

Bool_t HShowerTofPIDTrackMatcher::init() {
  // creating the HShowerTofPID, HShowerTrack and HShowerTofPIDTrack categories 
  // and addidng them to the current event,
  // create an iterator looping over all pid local maxima
  // and an iterator looping over track number information

    printf("initialization of Shower/Tofino pid-track matcher\n");
    
    HShowerDetector *pShowerDet = (HShowerDetector*)gHades->getSetup()
                                                  ->getDetector("Shower");

    m_pTrackCat=gHades->getCurrentEvent()->getCategory(catShowerTrack);
    if (!m_pTrackCat) {
      m_pTrackCat=pShowerDet->buildCategory(catShowerTrack);

      if (!m_pTrackCat) return kFALSE;
      else gHades->getCurrentEvent()
                 ->addCategory(catShowerTrack, m_pTrackCat, "Shower");
    }

    m_pHitCat=gHades->getCurrentEvent()->getCategory(catShowerTofPID);
    if (!m_pHitCat) {
      m_pHitTrackCat = new HLinearCategory("HShowerTofPID", 1000);

      if (!m_pHitCat) return kFALSE;
      else gHades->getCurrentEvent()
                 ->addCategory(catShowerTofPID, m_pHitCat, "ShowerTofino");
    }

    m_pHitTrackCat = gHades->getCurrentEvent()
                           ->getCategory(catShowerTofPIDTrack);
    if (!m_pHitTrackCat) {
      m_pHitTrackCat = new HLinearCategory("HShowerTofPIDTrack", 1000);

      if (!m_pHitTrackCat) return kFALSE;
      else gHades->getCurrentEvent()
                 ->addCategory(catShowerTofPIDTrack, m_pHitTrackCat, "ShowerTofino");
    }

    fHitIter=(HIterator*)m_pHitCat->MakeIterator();
    fTrackIter=(HIterator*)m_pTrackCat->MakeIterator();

    return kTRUE;
}

Bool_t HShowerTofPIDTrackMatcher::finalize(void) {
   return kTRUE;
}

void HShowerTofPIDTrackMatcher::addHitTrack(HShowerTofPID* pHit, 
         HShowerTrack* pTrack) {
// create one object which contains data
// from HShowerTofPID and HShowerTrack objects
  
    HShowerTofPIDTrack *pHitTrack;

    pHitTrack=(HShowerTofPIDTrack *)m_pHitTrackCat->getNewSlot(m_zeroLoc);
    if (pHitTrack!=NULL) {
      pHitTrack=new(pHitTrack) HShowerTofPIDTrack;

      *pHitTrack = *pHit;
      pHitTrack->setTrack(pTrack->getTrack());
    }
}



Int_t HShowerTofPIDTrackMatcher::execute()
{
//searching all pairs in HShowerTofPID and HShowerTrack, which
//have the same address
//Because HShowerTofPID and HShowerTrack are sorted by m_nAddress
//reading data in tables is done once!

  HShowerTofPID *pHit;
  HShowerTrack *pTrack;
 
  fHitIter->Reset();
  fTrackIter->Reset();

  pTrack = (HShowerTrack *)fTrackIter->Next();
  while((pHit = (HShowerTofPID*)fHitIter->Next())) { //loop over hits
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
