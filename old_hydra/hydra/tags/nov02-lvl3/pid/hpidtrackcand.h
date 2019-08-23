// @(#)$Id: hpidtrackcand.h,v 1.5 2003-03-04 17:21:47 jaskula Exp $
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
class HKickTrackSim;
class HTofHitSim;
class HShowerHitTofTrack;
class HMdcSegSim;
class HRichHitSim;
class HPidGeantTrackSet;

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

    void            print(void) const;
    void            Print(Option_t *o = "") const { print();                   }

    UShort_t        getInnerMdcNumber(void) const { return unInnerMdcNumber;   }

    HKickTrack*     getKickTrack(HCategory *pCat = NULL) const;
    HMdcSeg*        getMdcSeg(Int_t iPos = 0, HCategory *pCat = NULL) const;
    HRichHit*       getRichHit(HCategory *pCat = NULL) const;
    TObject*        getMetaHit(HCategory *pShwCat = NULL,
                            HCategory *pTofCat = NULL) const;
    HTofHit*        getTofHit(HCategory *pCat = NULL) const;
    HShowerHitTof*  getShowerHitTof(HCategory *pCat = NULL) const;

    // -------------------------------------------------------------------------

    Bool_t          isSim(void) const         { return TestBit(kIsSim);        }
    void            setSim(Bool_t b = kTRUE)  { SetBit(kIsSim, b);             }

    Bool_t          isBad(void) const         { return TestBit(kIsBad);        }
    void            setBad(Bool_t b = kTRUE)  { SetBit(kIsBad, b);             }

    Bool_t          isFiltered(void) const       { return TestBit(kIsFiltered);}
    void            setFiltered(Bool_t b = kTRUE){ SetBit(kIsFiltered, b);     }

    // -------------------------------------------------------------------------

    HKickTrackSim*  getKickTrackSim(HCategory *pCat = NULL) const;
    HMdcSegSim*     getMdcSegSim(Int_t iPos = 0, HCategory *pCat = NULL) const;
    HRichHitSim*    getRichHitSim(HCategory *pCat = NULL) const;
    TObject*        getMetaHitSim(HCategory *pShwCat = NULL,
                            HCategory *pTofCat = NULL) const
                            { return getMetaHit(pShwCat, pTofCat);             }
    HTofHitSim*     getTofHitSim(HCategory *pCat = NULL) const;
    HShowerHitTofTrack*
                    getShowerHitTofTrack(HCategory *pCat = NULL) const;

    // -------------------------------------------------------------------------

    static HKickTrack*      getKickTrack(Int_t iId, HCategory *pCat = NULL);
    static HMdcSeg*         getMdcSeg(Int_t iSector, Int_t iId,
                                    HCategory *pCat = NULL);
    static HRichHit*        getRichHit(Int_t iId, HCategory *pCat = NULL);
    static HTofHit*         getTofHit(Int_t iId, HCategory *pCat = NULL);
    static HShowerHitTof*   getShowerHitTof(Int_t iId, HCategory *pCat = NULL);

    static HKickTrackSim*   getKickTrackSim(Int_t iId, HCategory *pCat = NULL);
    static HMdcSegSim*      getMdcSegSim(Int_t iSector, Int_t iId,
                                    HCategory *pCat = NULL);
    static HRichHitSim*     getRichHitSim(Int_t iId, HCategory *pCat = NULL);
    static HTofHitSim*      getTofHitSim(Int_t iId, HCategory *pCat = NULL);
    static HShowerHitTofTrack*  getShowerHitTofTrack(Int_t iId,
                                    HCategory *pCat = NULL);

    // -------------------------------------------------------------------------

    static HCategory*     buildPidTrackCategory(void);
    static HPidTrackCand* getTrackCand(Int_t iIdx, HCategory *pCat = NULL);

    // -------------------------------------------------------------------------

    static HCategory*     getCategory(Cat_t cat, Bool_t bWarning = kTRUE);

    // -------------------------------------------------------------------------

    const HPidGeantTrackSet* getGeantTrackSet(void) const;

    // -------------------------------------------------------------------------

    ClassDef(HPidTrackCand, 1)  // Track candidate for PID
};

#endif //HPIDTRACKCAND_H
