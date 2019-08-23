// @(#)$Id: hpidtrackcand.h,v 1.1 2002-10-30 10:55:33 halo Exp $
//*-- Author : Marcin Jaskula 28/06/2002
// Modified  : Marcin Jaskula 16/09/2002
//             - removed: HArrayS, OuterHits
//             - Rings array -> one Ring Id

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

// -----------------------------------------------------------------------------

class HKickTrack;
class HTofHit;
class HShowerHitTof;
class HMdcSeg;
class HRichHit;
class HCategory;

// -----------------------------------------------------------------------------

#define MAX_MDC_SEG 10

// -----------------------------------------------------------------------------

class HPidTrackCand : public TObject
{
public:

                HPidTrackCand(void);

    Short_t     nSector;            // Sector where track was found

    Short_t     nKickTrackId;       // Id of the track from the Kick Plane: Index in the category
    Short_t     nKickTrackCount;    // How many times the track was used in the event

    Short_t     nRingId;            // Id of the ring
    Short_t     nRingCount;         // How many times the ring was used

    Short_t     aInnerMdcIds[MAX_MDC_SEG];      // Array of MDC segments
    Short_t     aInnerMdcCounts[MAX_MDC_SEG];   // How many times the segments were used
    UShort_t    unInnerMdcNumber;   // number of MDC segments in the set

    Short_t     nSystem;            // META system (0-PRESHOWER, 1-TOF)
    Short_t     nMetaId;            // Id of the hit in META
    Short_t     nMetaCount;         // How many times the META hit was used

    // -------------------------------------------------------------------------

    virtual void Clear(Option_t * /*option*/ = "");

    // -------------------------------------------------------------------------

    void        setSector(Short_t n)          { nSector = n;                   }
    Short_t     getSector(void) const         { return nSector;                }

    void        setKickTrackId(Short_t n)     { nKickTrackId = n;              }
    Short_t     getKickTrackId(void) const    { return nKickTrackId;           }

    void        setKickTrackCount(Short_t n)  { nKickTrackCount = n;           }
    Short_t     getKickTrackCount(void) const { return nKickTrackCount;        }
    void        addKickTrackCount(void);

    void        setRingId(Short_t n)          { nRingId = n;                   }
    Short_t     getRingId(void) const         { return nRingId;                }

    void        setRingCount(Short_t n)       { nRingCount = n;                }
    Short_t     getRingCount(void) const      { return nRingCount;             }
    void        addRingCount(void);

    void        addInnerMdcId(Short_t n);
    Short_t     getInnerMdcId(UInt_t i) const
                    { return (i >= unInnerMdcNumber) ? -1 : aInnerMdcIds[i];   }

    void        setInnerMdcCount(UInt_t iP, Short_t n)
                                              { aInnerMdcCounts[iP] = n;       }
    Short_t     getInnerMdcCount(UInt_t iP) const
                                              { return aInnerMdcCounts[iP]; }
    void        addInnerMdcCount(Int_t iP);
    void        addInnerMdcCountForId(Short_t nId);

    void        setMetaId(Short_t nSys, Short_t nId)
                                              { nSystem = nSys; nMetaId = nId; }
    Short_t     getSystem(void) const         { return nSystem;                }
    Short_t     getMetaId(void) const         { return nMetaId;                }

    void        setMetaCount(Short_t n)       { nMetaCount = n;                }
    Short_t     getMetaCount(void) const      { return nMetaCount;             }
    void        addMetaCount(void);

    // -------------------------------------------------------------------------

    Int_t       print(void) const;

    UShort_t    getInnerMdcNumber(void) const { return unInnerMdcNumber;       }

    HKickTrack*     getKickTrack(HCategory *pCat = NULL) const;
    HMdcSeg*        getMdcSeg(Int_t iPos = 0, HCategory *pCat = NULL) const;
    HRichHit*       getRichHit(HCategory *pCat = NULL) const;
    TObject*        getMetaHit(HCategory *pShwCat = NULL,
                            HCategory *pTofCat = NULL) const;
    HTofHit*        getTofHit(HCategory *pCat = NULL) const;
    HShowerHitTof*  getShowerHitTof(HCategory *pCat = NULL) const;

    // -------------------------------------------------------------------------

protected:
    HCategory*      getCategory(Cat_t cat) const;

    // -------------------------------------------------------------------------

    ClassDef(HPidTrackCand, 1)  // Track candidate for PID
};

// -----------------------------------------------------------------------------

HCategory* buildPidTrackCategory(void); // global function for making the cat.

#endif //HPIDTRACKCAND_H
