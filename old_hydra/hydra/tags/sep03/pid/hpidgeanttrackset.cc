// @(#)$Id: hpidgeanttrackset.cc,v 1.2 2003-03-04 17:14:27 jaskula Exp $
//*-- Author : Marcin Jaskula 11/11/2002
//  Modified : Marcin Jaskula 02/03/2003
//             get rid of TObject

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// HPidGeantTrackSet                                                          //
//                                                                            //
// Geant tracks matched in HPidTrackCand                                      //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include "hpidgeanttrackset.h"
#include "hpidparticlesim.h"

#include "hlinearcategory.h"
#include "hpidtrackcand.h"

#include <TError.h>
#include "hgeantdef.h"
#include "hgeantkine.h"

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
// Get the common track id from the array of HPidCommonTracks.
// When bWithRich == kTRUE the HPidCommonTracks::cDetectors must be kFull.
// When bWithRich == kFALSE this member may be kFull or kTrack.
// When no that track is found -1 is returned

Char_t  c[1];
Short_t t[1];

    if(fillCommonTracks(c, t, 1) == 0)
        return -1;

    if((c[0] == HPidCommonTrack::kTrackFull)
            || ((bWithRich == kFALSE)
                    && (c[0] == HPidCommonTrack::kTrackKickPlane)))
    {
        return t[0];
    }

    return -1;
}

// -----------------------------------------------------------------------------

UInt_t HPidGeantTrackSet::fillCommonTracks(Char_t acDets[], Short_t anTracks[],
                            UInt_t iMax) const
{
// Fill the array of the common tracks using the trackIds set in the
// an*Tracks arrays. The output is stored in the acDets and anTracks
// arrays. The method returns the number of the filled common tracks.
// Any possible combination is checked. The common tracks are stored in the
// order which depends on its "commonity": kTrackFull, kTrackKickPlane,
// kTrackTrackLet.

UInt_t  iK, iM, iR;
UInt_t  iReturn = 0;
UInt_t  iRMax = getNRichTracks();
UInt_t  iKMax = getNMdcTracks();
UInt_t  iMMax = getNMetaTracks();

    if(iMax < 1)
        return 0;

    // find kFull tracks
    for(iR = 0; iR < iRMax; iR++)
    {
        for(iK = 0; iK < iKMax; iK++)
        {
            if(anMdcTracks[iK] != anRichTracks[iR])
                continue;

            for(iM = 0; iM < iMMax; iM++)
            {
                if(anMetaTracks[iM] != anMdcTracks[iK])
                    continue;

                acDets[iReturn]   = HPidCommonTrack::kTrackFull;
                anTracks[iReturn] = anMetaTracks[iM];

                if(++iReturn >= iMax)
                    return iReturn;
            }
        }
    }

    // find kTracks
    for(iK = 0; iK < iKMax; iK++)
    {
        // check whether the track is already used
        for(iM = 0; iM < iReturn; iM++)
        {
            if(anMdcTracks[iK] == anTracks[iM])
                break;
        }
        if(iM < iReturn)
            continue;

        for(iM = 0; iM < iMMax; iM++)
        {
            if(anMetaTracks[iM] != anMdcTracks[iK])
                continue;

            acDets[iReturn]   = HPidCommonTrack::kTrackKickPlane;
            anTracks[iReturn] = anMetaTracks[iM];

            if(++iReturn >= iMax)
                return iReturn;
        }
    }

    // find kTrackLet
    for(iR = 0; iR < iRMax; iR++)
    {
        // check whether the track is already used
        for(iK = 0; iK < iReturn; iK++)
        {
            if(anRichTracks[iR] == anTracks[iK])
                break;
        }
        if(iK < iReturn)
            continue;

        for(iK = 0; iK < iKMax; iK++)
        {
            if(anMdcTracks[iK] != anRichTracks[iR])
                continue;

            acDets[iReturn]   = HPidCommonTrack::kTracklet;
            anTracks[iReturn] = anMdcTracks[iK];

            if(++iReturn >= iMax)
                return iReturn;
        }
    }

    return iReturn;
}

// -----------------------------------------------------------------------------

UInt_t HPidGeantTrackSet::fillCommonTracks(HPidParticleSim *pPart,
                    Bool_t bFillKine, UInt_t iMask) const
{
// Fills common tracks in HPidParticleSim. The max number of the filled tracks
// depends of the number of HPidCommonTracks returned by the HPidParticleSim.
// The order of the tracks is defined by findCommonTracks method.
// If bFillKine is ON the parmeters from the HGeantKine are copied.
// When iMask != 0 HPidPhysicsConstants::getProcess() is called for the
// track (only when bFillKine == kTRUE).
// This method may fill only up to 100 common tracks.

Char_t  acDets[kMaxCommonTracks];
Short_t anTracks[kMaxCommonTracks];
UInt_t  iM, iRet;

    if(pPart == NULL)
    {
        ::Error("HPidGeantTrackSet::fillCommonTracks", "pPart == NULL");
        return 0;
    }

    if((iM = pPart->getMaxCommonTracks()) > kMaxCommonTracks)
    {
        ::Error("HPidGeantTrackSet::fillCommonTracks",
                    "Can fill only up to %d common tracks", kMaxCommonTracks);

        iM = kMaxCommonTracks;
    }

    // fill temp. table
    iRet = fillCommonTracks(acDets, anTracks, iM);

    // fill the tracks in the particle
HPidCommonTrack cTrack;

    for(iM = 0; iM < iRet; iM++)
    {
        cTrack.Clear();

        cTrack.setTrackId(anTracks[iM]);
        cTrack.setDetectors(acDets[iM]);
        cTrack.fillKine(iMask);

        pPart->setTrack(&cTrack, iM);
    }

    return iRet;
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

    printf("\n");
}

// -----------------------------------------------------------------------------

void HPidGeantTrackSet::Streamer(TBuffer &R__b)
{
// Stream an object of class HPidGeantTrackSet.

UInt_t R__s, R__c;

    if (R__b.IsReading())
    {
    Version_t R__v = R__b.ReadVersion(&R__s, &R__c);

        if(R__v == 1)
        {
        TObject o;

            o.Streamer(R__b);
        }

        R__b.ReadStaticArray(anRichTracks);
        R__b.ReadStaticArray(anMdcTracks);
        R__b.ReadStaticArray(anMetaTracks);
        R__b.CheckByteCount(R__s, R__c, HPidGeantTrackSet::IsA());
    }
    else
    {
        R__c = R__b.WriteVersion(HPidGeantTrackSet::IsA(), kTRUE);
        R__b.WriteArray(anRichTracks, 3);
        R__b.WriteArray(anMdcTracks, 5);
        R__b.WriteArray(anMetaTracks, 2);
        R__b.SetByteCount(R__c, kTRUE);
    }
}
