// @(#)$Id: hpidtrackcand.h,v 1.8 2006-08-16 16:13:35 halo Exp $
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
//             Inherits now only from TObject
// Upgrade of documentation 05/02/2005              

#ifndef HPIDTRACKCAND_H
#define HPIDTRACKCAND_H

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// HPidTrackCand                                                        //
//                                                                      //
// A container of track candidate.                                      //
// Contains basically two other objects HPidHitData and HPidTrackData   //
// Serves as starting point for particle identification                 //
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

  //Overwrite base class function
  virtual void Clear(Option_t * /*option*/ = "");

  // -------------------------------------------------------------------------

  //reports content of this object
  void            print(void) const;
  void            Print(Option_t *o = "") const { print();                   }
  /*
  //flagged bad or not?
  Bool_t          isBad(void) const         { return TestBit(kIsBad);        }
  void            setBad(Bool_t b = kTRUE)  { SetBit(kIsBad, b);             }
  
  //filtered by whatever algorithm?
  Bool_t          isFiltered(void) const       { return TestBit(kIsFiltered);}
  void            setFiltered(Bool_t b = kTRUE){ SetBit(kIsFiltered, b);     }
  */
  HPidTrackData*  getTrackData(void) {return &itsTrackData;}
  HPidHitData*    getHitData(void) {return &itsHitData;}
  
  // -------------------------------------------------------------------------
  
  static HCategory*     buildPidTrackCandCategory(void);
  // -------------------------------------------------------------------------
  
  ClassDef(HPidTrackCand, 1)  // Track candidate for PID
};
    
#endif //HPIDTRACKCAND_H
