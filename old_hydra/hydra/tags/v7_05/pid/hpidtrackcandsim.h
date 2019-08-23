// @(#)$Id: hpidtrackcandsim.h,v 1.1 2005-01-11 11:06:47 halo Exp $
//*-- Author : Marcin Jaskula 28/06/2002
// Modified  : Marcin Jaskula 16/09/2002
//             - removed: HArrayS, OuterHits
//             - Rings array -> one Ring Id
// Modified  : Marcin Jaskula 31/10/2002
//           - buildPidTrackCategory moved from global to static function
// Modified  : Marcin Jaskula 10/11/2002
//            - get*Sim methods added
// Modified  : Marcin Jaskula 05/02/2003
//            - isBad() and isFiltered() added
// Modified  : Marcin Jaskula 25/02/2003
//            - static functions to retrive the Hits, Rings and Tracks
//            Definition of getGeantTrackSet() changed !
// Redesigned: Tassilo Christ for compliance with new tracking scheme 
//

#ifndef HPIDTRACKCANDSIM_H
#define HPIDTRACKCANDSIM_H

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// HPidTrackCand                                                        //
//                                                                      //
// A container of track candidate.                                      //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

//#include "TObject.h"
//#include "piddef.h"

#include "hpidgeanttrackset.h"
#include "hpidtrackcand.h"

// -----------------------------------------------------------------------------

class HCategory;


// -----------------------------------------------------------------------------
class HPidTrackCandSim : public HPidTrackCand
{
 public:
  
  HPidTrackCandSim(void);
  HPidTrackCandSim(HMetaMatch* pMetaMatch, Int_t iMatchIndex);

  void Clear(Option_t * /*option*/ = "");
  // -------------------------------------------------------------------------
  
  static HCategory*     buildPidTrackCandCategory(void);
  
  // -------------------------------------------------------------------------
 private:
  HPidGeantTrackSet        itsGeantTrackSet;

 public:

  HPidGeantTrackSet* getGeantTrackSet(void) {return &itsGeantTrackSet;}
  
  ClassDef(HPidTrackCandSim, 1)  // Track candidate sim for PID
};
    
#endif //HPIDTRACKCANDSIM_H
