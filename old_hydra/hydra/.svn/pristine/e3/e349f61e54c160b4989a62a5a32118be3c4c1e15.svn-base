// @(#)$Id: hpidgeanttrackset.h,v 1.1 2002-12-03 14:25:04 jaskula Exp $
//*-- Author : Marcin Jaskula 01/12/2002

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

// -----------------------------------------------------------------------------

class HPidGeantTrackSet : public TObject
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
        kMaxMetaTracks = 2
    };

    // -------------------------------------------------------------------------

                HPidGeantTrackSet(void)             { reset();  }
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

    // -------------------------------------------------------------------------

    virtual void Clear(Option_t *opt = "")          { reset();  }
    virtual void Print(Option_t *opt = "") const    { print();  }

    void         print(void) const;

    // -------------------------------------------------------------------------

    static HGeantKine*  getGeantKine(Short_t nTrack, HCategory *pCat = NULL);

    // -------------------------------------------------------------------------

protected:

    Short_t     anRichTracks[kMaxRichTracks];
    Short_t     anMdcTracks[kMaxMdcTracks];
    Short_t     anMetaTracks[kMaxMetaTracks];

    // -------------------------------------------------------------------------

    friend HPidTrackCand;

    // -------------------------------------------------------------------------

    ClassDef(HPidGeantTrackSet, 1)  // Geant tracks matched in HPidTrackCand
};

#endif /* HPidGeantTrackSet_h */
