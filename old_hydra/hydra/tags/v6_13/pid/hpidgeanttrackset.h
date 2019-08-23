// @(#)$Id: hpidgeanttrackset.h,v 1.3 2003-03-04 17:14:27 jaskula Exp $
//*-- Author : Marcin Jaskula 01/12/2002
//  Modified : Marcin Jaskula 02/03/2003
//             get rid of TObject

#ifndef HPidGeantTrackSet_h
#define HPidGeantTrackSet_h

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// HPidGeantTrackSet                                                          //
//                                                                            //
// Geant tracks matched in HPidTrackCand                                      //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------

#include <TObject.h>

// -----------------------------------------------------------------------------

class HGeantKine;
class HCategory;
class HPidTrackCand;
class HPidParticleSim;
class HPidCommonTrack;

// -----------------------------------------------------------------------------

class HPidGeantTrackSet
{
public:

    enum
    {
        kNoTrack       =  0,    // no track
        kNotFilled     = -1,    // this member was not filled
        kNoRefObj      = -2,    // no availabe input object with tracks
        kOutOfBound    = -3,    // error value when data out of bounds requested

        kMaxRichTracks = 3,
        kMaxMdcTracks  = 5,
        kMaxMetaTracks = 2,

        kMaxCommonTracks = 100
    };

    // -------------------------------------------------------------------------

                HPidGeantTrackSet(void)             { reset();  }
    virtual    ~HPidGeantTrackSet(void)             { }

    void        reset(void);

    // -------------------------------------------------------------------------

    Short_t     getRichTrack(UInt_t iIdx) const;
    void        setRichTrack(UInt_t iIdx, Short_t nTrack);

    Short_t     getMdcTrack(UInt_t iIdx) const;
    void        setMdcTrack(UInt_t iIdx, Short_t nTrack);

    Short_t     getMetaTrack(UInt_t iIdx) const;
    void        setMetaTrack(UInt_t iIdx, Short_t nTrack);

    // -------------------------------------------------------------------------

    Int_t       getNRichTracks(void) const;
    Int_t       getNMdcTracks(void) const;
    Int_t       getNMetaTracks(void) const;

    // -------------------------------------------------------------------------

    Short_t     getCommonTrack(Bool_t bWithRich = kTRUE) const;

    UInt_t      fillCommonTracks(HPidParticleSim *pPart,
                        Bool_t bFillKine = kTRUE, UInt_t iMask = 0) const;
    UInt_t      fillCommonTracks(Char_t acDets[], Short_t anTracks[],
                        UInt_t iMax) const;

    // -------------------------------------------------------------------------

    void        Clear(Option_t *opt = "")          { reset();  }
    void        Print(Option_t *opt = "") const    { print();  }

    void        print(void) const;

    // -------------------------------------------------------------------------

    static HGeantKine*  getGeantKine(Short_t nTrack, HCategory *pCat = NULL);

    // -------------------------------------------------------------------------

protected:

    Short_t     anRichTracks[kMaxRichTracks];
    Short_t     anMdcTracks[kMaxMdcTracks];
    Short_t     anMetaTracks[kMaxMetaTracks];

    // -------------------------------------------------------------------------

    ClassDef(HPidGeantTrackSet, 2)  // Geant tracks matched in HPidTrackCand
};

#endif /* HPidGeantTrackSet_h */
