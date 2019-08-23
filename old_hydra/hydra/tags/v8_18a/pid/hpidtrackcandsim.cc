// @(#)$Id: hpidtrackcandsim.cc,v 1.5 2008-05-09 16:15:06 halo Exp $
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
//                                                                            //
// HPidTrackCandSim                                                           //
// Extension of PidTrackCand to simulation data                               //
// New data member: HPidGeantTrackSet                                         //
//                                                                            //
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

// Clear all members - Overwrite TObject::Clear(Option *)
void HPidTrackCandSim::Clear(Option_t *popt)
{
  HPidTrackCand::Clear();
  itsGeantTrackSet.Clear();
}

//HitData and momentum reconstruction information is filled in the mother class constructor
//Fill the simulation information here!
HPidTrackCandSim::HPidTrackCandSim(HMetaMatch* pMetaMatch, Int_t iMatchIndex)
  : HPidTrackCand(pMetaMatch, iMatchIndex)
{
  itsGeantTrackSet.fillFromMetaMatch(pMetaMatch);
}

// -----------------------------------------------------------------------------
// global function for making the category - identical to mother class, except the category name!
HCategory* HPidTrackCandSim::buildPidTrackCandCategory(void)
{
  HPidGeantTrackSet::nullparent=0;

  //UsePlutoGeneratorInfo = bUsePlutoGenerator;
  HCategory  *pCat;
  static Char_t sCatName[] = "HPidTrackCandSim";
  HEvent     *pEvent;
  
  if((gHades == NULL) || ((pEvent = gHades->getCurrentEvent()) == NULL))
    {
      ::Error("HPidTrackCandSim::buildPidTrackCategory",
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
      ::Error("HPidTrackCandSim::buildPidTrackCategory",
	      "Cannot create new category");
      
      return NULL;
    }
  ((HLinearCategory*)pCat)->setDynamicObjects(kTRUE);
  pEvent->addCategory(catPidTrackCand, pCat, "Pid");
  
  return pCat;
}

// -----------------------------------------------------------------------------
