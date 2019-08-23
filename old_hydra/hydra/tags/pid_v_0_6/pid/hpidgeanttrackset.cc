// @(#)$Id: hpidgeanttrackset.cc,v 1.1 2002-12-03 14:25:04 jaskula Exp $
//*-- Author : Marcin Jaskula 11/11/2002

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// HPidGeantTrackSet                                                          //
//                                                                            //
// Geant tracks matched in HPidTrackCand                                      //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include "hpidgeanttrackset.h"
#include "hlinearcategory.h"
#include "hpidtrackcand.h"

#include <TError.h>

#include "hgeantdef.h"

// -----------------------------------------------------------------------------

ClassImp(HPidGeantTrackSet)

// -----------------------------------------------------------------------------

void HPidGeantTrackSet::reset(void)
{
// Set all memebers to 0

    memset(anRichTracks, 0, sizeof(anRichTracks));
    memset(anMdcTracks,  0, sizeof(anMdcTracks));
    memset(anMetaTracks, 0, sizeof(anMetaTracks));
}

// -----------------------------------------------------------------------------

Short_t HPidGeantTrackSet::getRichTrack(UInt_t iIdx) const
{
// Get track from RICH

    return (iIdx >= kMaxRichTracks) ? kOutOfBound : anRichTracks[iIdx];
}

// -----------------------------------------------------------------------------

void HPidGeantTrackSet::setRichTrack(UInt_t iIdx, Short_t nTrack)
{
// Set track from RICH

    if(iIdx >= kMaxRichTracks)
    {
        ::Error("HPidGeantTrackSet::setRichTrack", "Out of bounds %d / %d",
                    iIdx, kMaxRichTracks);

        return;
    }

    anRichTracks[iIdx] = nTrack;
}

// -----------------------------------------------------------------------------

Short_t HPidGeantTrackSet::getMdcTrack(UInt_t iIdx) const
{
// Get track from Mdc

    return (iIdx >= kMaxMdcTracks) ? kOutOfBound : anMdcTracks[iIdx];
}

// -----------------------------------------------------------------------------

void HPidGeantTrackSet::setMdcTrack(UInt_t iIdx, Short_t nTrack)
{
// Set track from Mdc

    if(iIdx >= kMaxMdcTracks)
    {
        ::Error("HPidGeantTrackSet::setMdcTrack", "Out of bounds %d / %d",
                    iIdx, kMaxMdcTracks);

        return;
    }

    anMdcTracks[iIdx] = nTrack;
}

// -----------------------------------------------------------------------------

Short_t HPidGeantTrackSet::getMetaTrack(UInt_t iIdx) const
{
// Get track from Meta

    return (iIdx >= kMaxMetaTracks) ? kOutOfBound : anMetaTracks[iIdx];
}

// -----------------------------------------------------------------------------

void HPidGeantTrackSet::setMetaTrack(UInt_t iIdx, Short_t nTrack)
{
// Set track from Meta

    if(iIdx >= kMaxMetaTracks)
    {
        ::Error("HPidGeantTrackSet::setMetaTrack", "Out of bounds %d / %d",
                    iIdx, kMaxMetaTracks);

        return;
    }

    anMetaTracks[iIdx] = nTrack;
}

// -----------------------------------------------------------------------------

Int_t HPidGeantTrackSet::getNRichTracks(void) const
{
// get number of tracks from RICH. This method loops over all tracks in the
// array and check if the stored track is > 0

    for(Int_t i = 0; i < kMaxRichTracks; i++)
    {
        if(anRichTracks[i] <= 0)
            return i;
    }

    return kMaxRichTracks;
}

// -----------------------------------------------------------------------------

Int_t HPidGeantTrackSet::getNMdcTracks(void) const
{
// get number of tracks from Mdc. This method loops over all tracks in the
// array and check if the stored track is > 0

    for(Int_t i = 0; i < kMaxMdcTracks; i++)
    {
        if(anMdcTracks[i] <= 0)
            return i;
    }

    return kMaxMdcTracks;
}

// -----------------------------------------------------------------------------

Int_t HPidGeantTrackSet::getNMetaTracks(void) const
{
// get number of tracks from Meta. This method loops over all tracks in the
// array and check if the stored track is > 0

    for(Int_t i = 0; i < kMaxMetaTracks; i++)
    {
        if(anMetaTracks[i] <= 0)
            return i;
    }

    return kMaxMetaTracks;
}

// -----------------------------------------------------------------------------

HGeantKine* HPidGeantTrackSet::getGeantKine(Short_t nTrack, HCategory *pCat)
{
// Return HGeantKine object corresponding to nTrack from pCat.
// If the pCat is not set (NULL) catGeantKine category from gHades is used.

    if(nTrack <= 0)
        return NULL;

    if((pCat == NULL) && ((pCat = HPidTrackCand::getCategory(
                            catGeantKine, kFALSE)) == NULL))
    {
        ::Error("HPidGeantTrackSet::getGeantKine", "No catGeantKine category");

        return NULL;
    }

    return (HGeantKine *) pCat->getObject(nTrack - 1);
}

// -----------------------------------------------------------------------------

Short_t HPidGeantTrackSet::getCommonTrack(Bool_t bWithRich) const
{
// Finds the first common track id, checked in data from Mdc and Meta, and
// from Rich if bWithRich is kTRUE.
// Returns the track id or -1 if no common track found.

Int_t   i, k, r;
Short_t nTrack;
Bool_t  bOk = kTRUE;

    for(i = 0; (i < kMaxMdcTracks) && ((nTrack = anMdcTracks[i]) > 0); i++)
    {
        for(k = 0; (k < kMaxMetaTracks) && (anMetaTracks[k] > 0); k++)
        {
            if(anMetaTracks[k] != nTrack)
                continue;

            if(bWithRich)
            {
                bOk = kFALSE;
                for(r = 0; (r < kMaxRichTracks) && (anRichTracks[r] > 0); r++)
                {
                    if(anRichTracks[r] == nTrack)
                    {
                        bOk = kTRUE;
                        break;
                    }
                }

                if(bOk == kFALSE)
                    continue;
            }

            return nTrack;
        }
    }

    return -1;
}

// -----------------------------------------------------------------------------

void HPidGeantTrackSet::print(void) const
{
// Print all the members

Int_t i;

    printf("HPidGeantTrackSet tracks:\nRICH (%d) :", getNRichTracks());
    for(i = 0; i < kMaxRichTracks; i++)
        printf(" %3d", anRichTracks[i]);

    printf("\nMDC  (%d) :", getNMdcTracks());
    for(i = 0; i < kMaxMdcTracks; i++)
        printf(" %3d", anMdcTracks[i]);

    printf("\nMeta (%d) :", getNMetaTracks());
    for(i = 0; i < kMaxMetaTracks; i++)
        printf(" %3d", anMetaTracks[i]);

    printf("\nCommon   : (R) %d  (NR) %d\n",
                    getCommonTrack(kTRUE), getCommonTrack(kFALSE));
}
