// @(#)$Id: hpidtrackcand.h,v 1.7 2005-01-11 11:06:47 halo Exp $
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
//             Inherits now only from HPidHitData              

#ifndef HPIDTRACKCAND_H
#define HPIDTRACKCAND_H

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// HPidTrackCand                                                        //
//                                                                      //
// A container of track candidate.                                      //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "TObject.h"
#include "piddef.h"
#include "hpidhitdata.h"
#include "hpidtrackdata.h"

// -----------------------------------------------------------------------------

class HCategory;

// -----------------------------------------------------------------------------
class HPidTrackCand : public TObject
{
 public:
  
  HPidTrackCand(void);
  HPidTrackCand(HMetaMatch* pMetaMatch, Int_t iMatchIndex);
  
  HPidHitData itsHitData;        // The hit data object which holds all PID-relevant information for this class
  HPidTrackData itsTrackData;    // The track data object which holds all PID-relevant information for this class

  // -------------------------------------------------------------------------
  
  virtual void Clear(Option_t * /*option*/ = "");

  // -------------------------------------------------------------------------
  
  void            print(void) const;
  void            Print(Option_t *o = "") const { print();                   }
  
  Bool_t          isBad(void) const         { return TestBit(kIsBad);        }
  void            setBad(Bool_t b = kTRUE)  { SetBit(kIsBad, b);             }
  
  Bool_t          isFiltered(void) const       { return TestBit(kIsFiltered);}
  void            setFiltered(Bool_t b = kTRUE){ SetBit(kIsFiltered, b);     }

  HPidTrackData*  getTrackData(void) {return &itsTrackData;}
  HPidHitData*    getHitData(void) {return &itsHitData;}
  
  // -------------------------------------------------------------------------
  
  static HCategory*     buildPidTrackCandCategory(void);
  // -------------------------------------------------------------------------
  
  ClassDef(HPidTrackCand, 1)  // Track candidate for PID
};
    
#endif //HPIDTRACKCAND_H
