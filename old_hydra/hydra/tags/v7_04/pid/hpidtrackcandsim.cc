// @(#)$Id: hpidtrackcandsim.cc,v 1.1 2005-01-11 11:06:47 halo Exp $
//*-- Author : Marcin Jaskula 28/06/2002
// Modified  : Marcin Jaskula 16/09/2002
//             - removed: HArrayS, OuterHits
//             - Rings array -> one Ring Id
// Modified  : Marcin Jaskula 31/10/2002
//           - buildPidTrackCategory moved from global to static function
// Modified  : Marcin Jaskula 10/11/2002
//           - get*Sim methods added
// Modified  : Marcin Jaskula 25/02/2003
//           - static functions to retrive the Hits, Rings and Tracks
//            Definition of getGeantTrackSet() changed !
// Restructured : Tassilo Christ 11.11.2003 for compliance with new pid/tracking scheme
//                Matching functionality is now performed by HMetaMatch class.
//                Category retrieval is now done ion HPidFL::getHitObject
//                Get/Set functions removed as all data members are public
//                this might change later on
//                The real data and the sim data class will be different in the future!
//                Thus the isSim bit has been removed
////////////////////////////////////////////////////////////////////////////////
//
// HPidTrackCand
//
// In this container matched hits from all detectors are stored.
// The class do not define the algorithm how the mathcing is done.
// It should be defined in a correlator which fills the container.
//
// The structure of this class allow user to store information about
// the correlated hits as well as information about all hits in neighborhood
// to hits which construct the root of the track candidate. The hits in
// the neighborhood are dedicated for identification close pairs
// as it was discussed at the PID subgroup meeting.
//
// The structure of the container is similar to the HKickTrack category.
// There is information about the sector and META hit like in the HKickTrack.
// The nKickTrackId keeps id of the kick track in the category. To get the
// HKickTrack from this information a call similar for getting HMdcSeg must
// be done:
// HLocation loc;
// loc.set(3,0,0,0);
// loc[0] = nSector;
// loc[1] = 0;
// loc[2] = nKickTrackId;
// category->getObject(loc);
//
// For Mdc's segs array of ids is dedicated. The idea was
// that the first id in the arrays ([0]) is used for building the track
// candidate. The rest of the ids in the arrays are used only as information
// for the close pairs and similar tasks.
//
// Methods getXXX are dedicate to make access to hits/rings/tracks in
// an easy way. They work only in gHades loop.
//
// Marker Sim (setSim(), getSim()) is chosen when the ids in the category
// stores ids for simulation objects. A proper getXXXSim should be used
// in this case.
//
// For every id of hit/ring/track a counter for this object is dedicated.
// They should store information in how many HPidTrackCand the hit/ring/track
// was used and make this information available for cleaning the tracks
// candidates.
// This is arbitrary if the track fillers (correlators) fill these members.
//
////////////////////////////////////////////////////////////////////////////////

#pragma implementation

#include "hpidtrackcandsim.h"
#include "hpidtrackcand.h"
#include "hmetamatch.h"
#include "hpidfl.h"

#include "hades.h"
#include "hevent.h"
#include "hlinearcategory.h"

#include "hmdctrackgdef.h"
#include "kickdef.h"

#include <TError.h>
#include <TTree.h>

#include "hbasetrack.h"

// -----------------------------------------------------------------------------

ClassImp(HPidTrackCandSim)
  
  // -----------------------------------------------------------------------------
  
HPidTrackCandSim::HPidTrackCandSim(void)
  : HPidTrackCand()
{
  
}

void HPidTrackCandSim::Clear(Option_t *popt)
{
  //cout << "Clearing in track cand sim" << endl;
  // Clear all members
  // Overwrite TObject::Clear(Option *)
  HPidTrackCand::Clear();
  itsGeantTrackSet.Clear();
}

HPidTrackCandSim::HPidTrackCandSim(HMetaMatch* pMetaMatch, Int_t iMatchIndex)
  : HPidTrackCand(pMetaMatch, iMatchIndex)
{
  //HitData and momentum reconstruction information is filled in the mother class constructor
  //Fill the trackset information here!
  itsGeantTrackSet.fillFromMetaMatch(pMetaMatch);
}

// -----------------------------------------------------------------------------

HCategory* HPidTrackCandSim::buildPidTrackCandCategory(void)
{
  // global function for making the category - identical to mother class, except the category name!
  
  HCategory  *pCat;
  static char sCatName[] = "HPidTrackCandSim";
  HEvent     *pEvent;
  
  if((gHades == NULL) || ((pEvent = gHades->getCurrentEvent()) == NULL))
    {
      ::Error("HPidTrackCandSim::buildPidTrackCategory",
	      "Cannot access current event");
      
      return NULL;
    }
  
  if((pCat = pEvent->getCategory(catPidTrackCand)) != NULL)
    {
      return pCat;
      ((HLinearCategory*)pCat)->setDynamicObjects(kTRUE);
    }
  
  if((pCat = new HLinearCategory(sCatName, 1000)) == NULL)
    {
      ::Error("HPidTrackCandSim::buildPidTrackCategory",
	      "Cannot create new category");
      
      return NULL;
    }
  ((HLinearCategory*)pCat)->setDynamicObjects(kTRUE);
  pEvent->addCategory(catPidTrackCand, pCat, "Pid");
  
  return pCat;
}

// -----------------------------------------------------------------------------
