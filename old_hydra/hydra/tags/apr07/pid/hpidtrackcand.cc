// @(#)$Id: hpidtrackcand.cc,v 1.12 2006-11-24 16:00:29 christ Exp $
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
//_HADES_CLASS_DESCRIPTION 
////////////////////////////////////////////////////////////////////////////////

//_HADES_CLASS_DESCRIPTION
//
// HPidTrackCand
//
// In this container matched hits from all detectors are stored.
// The class does not define the algorithm how the matching is done.
// This is defined in HMetaMatch (coarse matching) and HPidTrackFiller (refined 
// matching taking into account momentum values.
//
// The structure of this class allows user to store information about
// the correlated hits as well as information about all hits in neighborhood
// of hits which contribute to the track candidate. The hits in
// the neighborhood are dedicated for identification close pairs
// as it was discussed at the PID subgroup meeting.
// ATTENTION: THE HANDLING OF CLOSE BY HITS IS NOT YET IMPKLEMENTED 
// THE CLASS ONLY STORES THE DISTANCE OF THE CLOSEST HADRON AND LEPTON CANDIDATES!
// 
////////////////////////////////////////////////////////////////////////////////

#pragma implementation

#include "hpidtrackcand.h"
#include <iostream>
#include "hmetamatch.h"
#include "hbasetrack.h"
#include "hpidfl.h"

#include "hades.h"
#include "hevent.h"
#include "hlinearcategory.h"

#include "hmdctrackgdef.h"
#include "kickdef.h"

#include <TError.h>
#include <TTree.h>
#include "hpidhitdata.h"
#include "hbasetrack.h"

// -----------------------------------------------------------------------------

ClassImp(HPidTrackCand)
  
  // -----------------------------------------------------------------------------
  
HPidTrackCand::HPidTrackCand(void)
{
  // Default HPidTrackCand ctor.
  
  Clear();
}

//Constructor
HPidTrackCand::HPidTrackCand(HMetaMatch* pMetaMatch, Int_t iMatchIndex)
{
  Clear();
  if(pMetaMatch->getSector()>5 || pMetaMatch->getSector()<0)
    {
      Error("HPidTrackCand::HPidTrackCand(HMetaMatch* p,Int_t i)","Nonsense sector in constructor of PidTrackCand");
    }
  itsHitData.fillFromMetaMatch(pMetaMatch, iMatchIndex);
  itsTrackData.fillFromMetaMatch(pMetaMatch);
}


// -----------------------------------------------------------------------------
// Clear all members - Overwrite TObject::Clear(Option *)
void HPidTrackCand::Clear(Option_t * popt)
{
  itsTrackData.reset();
  itsHitData.reset();
}

// -----------------------------------------------------------------------------

void HPidTrackCand::print(void) const
{
  itsHitData.print();
  itsTrackData.print();
}

// -----------------------------------------------------------------------------
// global function for making the category
HCategory* HPidTrackCand::buildPidTrackCandCategory(void)
{
  HCategory  *pCat;
  static char sCatName[] = "HPidTrackCand";
  HEvent     *pEvent;
  
  if((gHades == NULL) || ((pEvent = gHades->getCurrentEvent()) == NULL))
    {
      ::Error("HPidTrackCand::buildPidTrackCategory",
	      "Cannot access current event");
      
      return NULL;
    }
  
  if((pCat = pEvent->getCategory(catPidTrackCand)) != NULL)
    {
      //This category has objects of always the same type but sometimes different size!
      //This is indicated by the "dynamic objects flag"
      ((HLinearCategory*)pCat)->setDynamicObjects(kTRUE);
      return pCat;
    }
  
  if((pCat = new HLinearCategory(sCatName, 1000)) == NULL)
    {
      ::Error("HPidTrackCand::buildPidTrackCategory",
	      "Cannot create new category");
      
      return NULL;
    }
  ((HLinearCategory*)pCat)->setDynamicObjects(kTRUE);
  pEvent->addCategory(catPidTrackCand, pCat, "Pid");
  
  return pCat;
}

// -----------------------------------------------------------------------------


