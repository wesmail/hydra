#include "hshowerhittrackmatcher.h"

#include "hruntimedb.h"
#include "hevent.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hshowerdetector.h"
#include "hcategory.h"
#include "hmatrixcatiter.h"
#include "hlocation.h"
#include "hshowerhit.h"
#include "hshowertrack.h"
#include "hshowerhittrack.h"
#include "hdebug.h"
#include "hades.h"
#include "hiterator.h"
#include "showerdef.h"


ClassImp(HShowerHitTrackMatcher)

//_HADES_CLASS_DESCRIPTION 
///////////////////////////////////////////////////////////////////////
//HShowerHitTrackMatcher
//
// HShowerHitTrackMatcher reconstructor searches 
// data describing the same pad in HShowerHit and HShowerTrack categories
// and join its to one object. 
// Results are stored in HShowerHitTrack category
//
// The Shower task is split into several tasks as shown
// in the flow diagram below.
//
//   ----------------------
//  |     HShowerUnpacker  |                                                                              //
//  |   (embedding mode)   | \                                                                            //
//  |                      |  \      ------------------                                                   //
//   ----------------------   |     |  HGeantShower    |                                                  //
//                            |      ------------------\                                                  //
//                            |                         \                                                 //
//                            |      ------------------  \------------->  ----------------------          //
//                            |     |  HGeantWire      |   <------------ |  HShowerHitDigitizer |         //
//                            |      ------------------\                  ----------------------          //
//                            |                         \                                                 //
//                 -------------     ------------------  \------------->  -----------------------         //
//             -- | HShowerRaw  |   |  HShowerRawMatr  |   <------------ |  HShowerPadDigitizer  |        //
//            |    -------------     ------------------\                 |( creates track objects|        //
//            |                                         \                |  for real tracks in   |        //
//   ----------------------          ------------------  \               |  embedding mode too)  |        //
//  |   HShowerCalibrater  |        |  HShowerTrack    |  \<------------  -----------------------         //
//  |   (embedding mode)   |         ------------------\   \                                              //
//   ----------------------                             \   \             -----------------------         //
//            |                      ------------------  \   ----------> |   HShowerCopy         |        //
//             -------------------> |  HShowerCal      |  \<------------ |(add charge of real hit|        //
//                                   ------------------\   \             | in embedding too )    |        //
//                                                      \   \             -----------------------         //
//                                   ------------------  ----\--------->  -----------------------         //
//                                  |  HShowerHitHdr   |   <--\--------- |  HShowerHitFinder     |        //
//                                   ------------------        \          -----------------------         //
//                                   ------------------         \        |                                //
//                                  |  HShowerPID      |   <-----\-------|                                //
//                                   ------------------           \      |                                //
//                                   ------------------            \     |                                //
//                                  |  HShowerHit      |   <--------\----|                                //
//                                   ------------------ \            \                                    //
//                                                       \            \                                   //
//                                   ------------------   \-------------> ------------------------        //
//                                  |  HShowerHitTrack |   <------------ | HShowerHitTrackMatcher |       //
//                                   ------------------                   ------------------------        //
//
//
//  In the case of TRACK EMBEDDING of simulated tracks into
//  experimental data the real data are written by the HShowerUnpacker into
//  HShowerRaw category. The real hits are taken into
//  account by the digitizer (adding of charges). The embedding mode is recognized
//  automatically by analyzing the
//  gHades->getEmbeddingMode() flag.
//            Mode ==0 means no embedding
//                 ==1 realistic embedding (first real or sim hit makes the game)
//                 ==2 keep GEANT tracks   (as 1, but GEANT track numbers will always
//                     win against real data. besides the tracknumber the output will
//                     be the same as in 1)
//
//
//////////////////////////////////////////////////////////////////////

HShowerHitTrackMatcher::HShowerHitTrackMatcher(Text_t *name,Text_t *title) :
  HReconstructor(name,title)
{
   fHitIter=NULL;
   fTrackIter=NULL;
   m_zeroLoc.set(0);
}

HShowerHitTrackMatcher::HShowerHitTrackMatcher()
{
   fHitIter=NULL;
   fTrackIter=NULL;
   m_zeroLoc.set(0);
}


HShowerHitTrackMatcher::~HShowerHitTrackMatcher(void) {
     if (fHitIter) delete fHitIter;
     if (fTrackIter) delete fTrackIter;
}

Bool_t HShowerHitTrackMatcher::init() {
  // creates the ShowerHit, ShowerTrack and ShowerHitTrack categories 
  // and adds them to the current event
  // creates an iterator which loops over all local maxima
  // and an iterator which loops over all track number information

    printf("initialization of shower hit-track matcher\n");
    
    HShowerDetector *pShowerDet = (HShowerDetector*)gHades->getSetup()
                                                  ->getDetector("Shower");

    m_pTrackCat=gHades->getCurrentEvent()->getCategory(catShowerTrack);
    if (!m_pTrackCat) {
      m_pTrackCat=pShowerDet->buildCategory(catShowerTrack);

      if (!m_pTrackCat) return kFALSE;
      else gHades->getCurrentEvent()
                        ->addCategory(catShowerTrack, m_pTrackCat, "Shower");
    }

    m_pHitCat=gHades->getCurrentEvent()->getCategory(catShowerHit);
    if (!m_pHitCat) {
      m_pHitCat=pShowerDet->buildCategory(catShowerHit);

      if (!m_pHitCat) return kFALSE;
      else gHades->getCurrentEvent()
                         ->addCategory(catShowerHit, m_pHitCat, "Shower");
    }

    m_pHitTrackCat=gHades->getCurrentEvent()->getCategory(catShowerHitTrack);
    if (!m_pHitTrackCat) {
      m_pHitTrackCat=pShowerDet->buildCategory(catShowerHitTrack);

      if (!m_pHitTrackCat) return kFALSE;
      else gHades->getCurrentEvent()
                        ->addCategory(catShowerHitTrack, m_pHitTrackCat, "Shower");
    }

    fHitIter=(HIterator*)m_pHitCat->MakeIterator();
    fTrackIter=(HIterator*)m_pTrackCat->MakeIterator();

    return kTRUE;
}

Bool_t HShowerHitTrackMatcher::finalize(void) {
   return kTRUE;
}


void HShowerHitTrackMatcher::addHitTrack(HShowerHit* pHit, 
         HShowerTrack* pTrack) {
//create one object, which contains data
//from HShowerHit and HShowerTrack objects
  
    HShowerHitTrack *pHitTrack;

    pHitTrack=(HShowerHitTrack *)m_pHitTrackCat->getNewSlot(m_zeroLoc);
    if (pHitTrack!=NULL) {
      pHitTrack=new(pHitTrack) HShowerHitTrack;

      *pHitTrack = *pHit;
      pHitTrack->setTrack(pTrack->getTrack());
    }
}



Int_t HShowerHitTrackMatcher::execute()
{
//searching all pairs in HShowerHit and HShowerTrack, which
//have the same address
//Because HShowerHit and HShowerTrack are sorted by m_nAddress
//reading data in tables is done once!

  HShowerHit *pHit;
  HShowerTrack *pTrack;
 
  fHitIter->Reset();
  fTrackIter->Reset();

  pTrack = (HShowerTrack *)fTrackIter->Next();
  while((pHit = (HShowerHit*)fHitIter->Next())) { //loop over hits
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



