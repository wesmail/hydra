// @(#)$Id: hpidhistogramscont.cc,v 1.7 2004-07-08 13:45:51 christ Exp $
//*-- Author : Marcin Jaskula 10/10/2002
//  Modified : Marcin Jaskula 25/02/2003
//             Now particles with negative ids are supported.
// Modified  : Marcin Jaskula 02/06/2003
//           - Draw() method added
// Modified  : Marcin Jaskula & Tomasz Wojcik 26/06/2002
//             Oracle interface added

////////////////////////////////////////////////////////////////////////////////
//
// HPidHistogramsCont
//
// Container for define set of histograms with interface for easy access
// of values from them.
// The histograms are organized in an array with access:
// (pid, index)
// The user (class) defines maximum number of histograms for one id (max index).
// The container is stored using normal Pid containers scheme.
//
// Technical warning: copy functions not implemented !!!
//
////////////////////////////////////////////////////////////////////////////////

#include "hpidhistogramscont.h"
#include "hparamlist.h"

#include <TH1.h>
#include <TClass.h>
#include <TVirtualPad.h>
#include <TROOT.h>

// -----------------------------------------------------------------------------

ClassImp(HPidHistogramsCont)

// -----------------------------------------------------------------------------

#define DEF_PID_ID -1

// -----------------------------------------------------------------------------

HPidHistogramsCont::HPidHistogramsCont(void)
                            : HPidParCont("HPidHistogramsCont",
                                    "HPidHistogramsCont", "HPidHistogramsCont")
{
// empty constructor for reading from Stream

    setDefault();
}

// -----------------------------------------------------------------------------

HPidHistogramsCont::HPidHistogramsCont(const char *name, const char *title,
                                const char *context,
                                Int_t iHistSet)
                                : HPidParCont(name, title, context)
{
// Constructor.
// Makes the array for particle ids and historgams
// The iNParticles and iHistsPerPart must be > 0
// If the parameters are wrong, or there is no memory an error is printed
// and the bOk flag is set off.

    setDefault();

    if(iHistSet < 1)
    {
        bOk = kFALSE;
        Error("HPidHistogramsCont", "Wrong size: %d\n", iHistSet);

        return;
    }

    iHistsPerPart = iHistSet;
    resize(0);
}

// -----------------------------------------------------------------------------

void HPidHistogramsCont::setDefault(void)
{
// function called in constructors for clearnig memory befor it is used

    strcpy(detName, "Pid");

    iNParticles   = 0;
    iHistsPerPart = 0;
    aHistograms.SetOwner();

    bWarnings     = kTRUE;
    bOk           = kTRUE;

    resize(0);
}

// -----------------------------------------------------------------------------

HPidHistogramsCont::~HPidHistogramsCont(void)
{
// Remove all histograms and delete arrays

    clear();
    resize(0);
}

// -----------------------------------------------------------------------------

Bool_t HPidHistogramsCont::resize(Int_t iNPSet)
{
// Change the size of the arrays and copy old contains if exists
// The histograms, which will not be included into new array are NOT DELETED !!!

    if( ! bOk)
    {
        Error("addHistogram", "The class is not initialized !!!");
        return kFALSE;
    }

    aParticles.Set(iNPSet);
    aHistograms.Expand(iNPSet * iHistsPerPart);

    iNParticles = iNPSet;

    return kTRUE;
}

// -----------------------------------------------------------------------------

void HPidHistogramsCont::clear(void)
{
// Delete the histograms and set all particle ids to default values (-1)
// The arrays of particle ids and histograms are not removed

    aParticles.Reset(DEF_PID_ID);
    aHistograms.Clear();
}

// -----------------------------------------------------------------------------

void HPidHistogramsCont::detachHistograms(void)
{
// Deteach histograms from the file by calling hist->SetDirectory(NULL)

Int_t i, c;
TH1  *pH;

    c = TMath::Min(getNHistograms(), aHistograms.GetLast() + 1);
    for(i = 0; i < c; i++)
    {
        if((pH = (TH1 *) aHistograms.At(i)) != NULL)
            pH->SetDirectory(NULL);
    }
}

// -----------------------------------------------------------------------------

void HPidHistogramsCont::putParams(HParamList* l)
{
// Put params into HParamList

    // check the configuration
    if( ! bOk)
    {
        Error("putParams", "The class is not initialized !!!");
        return;
    }

    HPidParCont::putParams(l);

    // write the whole object
    l->addBinary("hists", this);
}

// -----------------------------------------------------------------------------

Bool_t HPidHistogramsCont::getParams(HParamList* l)
{
// Get params from HParamList

    clear();
    resize(0);
    bOk = kTRUE;

    if(HPidParCont::getParams(l) != kTRUE)
        return kFALSE;

    if( ! l->fillBinary("hists", this))
    {
        Error("getParams", "Cannot get the class");
        return kFALSE;
    }

    return kTRUE;
}

// -----------------------------------------------------------------------------

Int_t HPidHistogramsCont::findParticleIndex(Int_t iPart) const
{
// Return the index of particle id iPart in the array.
// If the partcle id is not set -1 is returned

    for(Int_t i = 0; i < iNParticles; i++)
    {
        if(aParticles.At(i) == iPart)
            return i;
    }

    return -1;
}

// -----------------------------------------------------------------------------

Bool_t HPidHistogramsCont::addHistogram(Int_t iPartId, Int_t iOffset,
                            const TH1 *pHist)
{
// A !!! COPY !!! of histogram pHist is stored in the position for proper
// iPartId and iOffset (both >= 0). If the position of iOffset is out of bounds
// or iPartId is not on the array yet and there is no space in the array for the
// new entry the error is reported and kFALSE returned.
// If on the position there is already a histogram it is deleted and warning
// is printed

    // check the configuration
    if( ! bOk)
    {
        Error("addHistogram", "The class is not initialized !!!");
        return kFALSE;
    }

    if(pHist == NULL)
    {
        removeHistogram(iPartId, iOffset);
        return kTRUE;
    }

    if(pHist->IsA()->InheritsFrom("TH1") == kFALSE)
    {
        Error("addHistogram", "%p doesn't inherit from TH1", pHist);
        return kFALSE;
    }

    if((iOffset < 0) || (iOffset >= iHistsPerPart))
    {
        Error("addHistogram", "iOffset = %d not in the range [0, %d)",
                        iOffset, iHistsPerPart);
        return kFALSE;
    }

Int_t i, c;

    // find the place to store the histogram
    if((i = findParticleIndex(iPartId)) < 0)
    {
        // create space for new histogram
        if(resize(iNParticles + 1) == kFALSE)
            return kFALSE;

        i = iNParticles - 1;

        // create new particle's entry
        aParticles[i] = iPartId;
        c = getIndex(i, iOffset);
    }
    else
    {
    TH1 *pOld;

        // remove old histogram
        c = getIndex(i, iOffset);
        if((pOld = (TH1 *)aHistograms.RemoveAt(c)) != NULL)
        {
            if(bWarnings)
            {
                Warning("addHistogram", "Removing old histogram for %d %d",
                            iPartId, iOffset);
            }

            delete pOld;
        }
    }

    // put Clone of the histogram
TH1 *pClone;

    if((pClone = (TH1 *) pHist->Clone()) == NULL)
    {
        Error("addHistogram", "Cannot clone the histogram");
        return kFALSE;
    }

    pClone->SetDirectory(NULL);
    aHistograms.AddAt(pClone, c);

    return kTRUE;
}

// -----------------------------------------------------------------------------

void HPidHistogramsCont::removeHistogram(Int_t iPartId, Int_t iOffset)
{
// Removes histogram for the given particle id and offset

    // check the configuration
    if( ! bOk)
    {
        Error("removeHistogram", "The class is not initialized !!!");
        return;
    }

    if((iOffset < 0) || (iOffset >= iHistsPerPart))
    {
        Error("removeHistogram", "iOffset = %d not in the range [0, %d)",
                        iPartId, iHistsPerPart);
        return;
    }

Int_t i, c, k;
TH1  *pOld;

    // check if particle ids exists
    if((i = findParticleIndex(iPartId)) < 0)
        return;

    // check if the address is int the TObjArray bounds
    if((c = getIndex(i, iOffset)) > aHistograms.GetLast())
        return;

    // remove the histogram
    if((pOld = (TH1 *)aHistograms.RemoveAt(c)) != NULL)
        delete pOld;

    // checks if there is any histogram for the id
    for(c = 0; c < iHistsPerPart; c++)
    {
        if(aHistograms[getIndex(i, c)] != NULL)
            return;
    }

    // no histograms, so remove the entry for the id and move array contents
    for(c = i; c < iNParticles - 1; c++)
    {
        aParticles[c] = aParticles[c + 1];
        for(k = 0; k < iHistsPerPart; k++)
            aHistograms[getIndex(c, k)] = aHistograms[getIndex(c + 1, k)];
    }

    // clear the last elements in the arrays
    aParticles[c] = DEF_PID_ID;
    for(k = 0; k < iHistsPerPart; k++)
        aHistograms.RemoveAt(getIndex(c, k));

    resize(iNParticles - 1);
}

// -----------------------------------------------------------------------------

TH1* HPidHistogramsCont::getHistogram(Int_t iPartId, Int_t iOffset) const
{
// Return a pointer to the histogram for given iPartId and iOffset
// In case of out of bounds parameters the error is reported and NULL returned

Int_t i;

    // check the configuration
    if( ! bOk)
    {
        Error("getHistogram", "The class is not initialized !!!");
        return NULL;
    }

    if((iOffset < 0) || (iOffset >= iHistsPerPart))
    {
        Error("getHistogram", "iOffset = %d not in the range [0, %d)",
                        iOffset, iHistsPerPart);
        return NULL;
    }

    if((i = findParticleIndex(iPartId)) < 0)
    {
        if(bWarnings)
            Warning("getHistogram", "No particle id: %d\n", iPartId);

        return NULL;
    }

    if((i = getIndex(i, iOffset)) > aHistograms.GetLast())
        return NULL;

    return (TH1 *) aHistograms.At(i);
}

// -----------------------------------------------------------------------------

Stat_t HPidHistogramsCont::getValue(Int_t iPartId, Int_t iOffset,
                            Float_t fX, Float_t fY, Float_t fZ) const
{
// Returns value from the histogram corresponding to given parameters.
// If the address of the histogram is out of bounds or histogram
// is not set in the array a warning is reported and -1.0f returned.
// Otherwise the returned value is calculated in a way:
// pHist->GetBinContent(pHist->FindBin((fX, fY, fZ))
// If (fX, fY, fZ) is out of range of the histogram, then the value from
// the "closest" bin is returned

TH1 *pH;

    if((pH = (TH1 *) getHistogram(iPartId, iOffset)) == NULL)
    {
        if(bWarnings)
            Warning("getValue", "No histogram for: %d %d", iPartId, iOffset);

        return -1.0f;
    }

TAxis *pA;
Int_t  iBinX, iBinY, iBinZ;

    pA = pH->GetXaxis();
    if((iBinX = pA->FindBin(fX)) <= 0)
        iBinX = 1;
    else
    {
        if(iBinX > pA->GetNbins())
            iBinX = pA->GetNbins();
    }

    pA = pH->GetYaxis();
    if((iBinY = pA->FindBin(fY)) <= 0)
        iBinY = 1;
    else
    {
        if(iBinY > pA->GetNbins())
            iBinY = pA->GetNbins();
    }

    pA = pH->GetZaxis();
    if((iBinZ = pA->FindBin(fZ)) <= 0)
        iBinZ = 1;
    else
    {
        if(iBinZ > pA->GetNbins())
            iBinZ = pA->GetNbins();
    }

    return pH->GetBinContent(pH->GetBin(iBinX, iBinY, iBinZ));
}

// -----------------------------------------------------------------------------

void HPidHistogramsCont::print(void)
{
// Print information about the histograms in the container

    HParSet::print();

    if( ! bOk)
    {
        Error("print", "The class is not initialized !!!");
        return;
    }

    printf("Particles   : %3d\nHistPerPart : %3d\n",
                iNParticles, iHistsPerPart);

Int_t i, c, iMax, k;
TH1 *pH;

    printf("PartId ");
    for(c = 0; c < iHistsPerPart; c++)
        printf("%15d ", c);

    printf("\n\n");

    iMax = aHistograms.GetLast();

    for(i = 0; i < iNParticles; i++)
    {
        printf("%6d ", aParticles[i]);

        for(c = 0; c < iHistsPerPart; c++)
        {
            k = getIndex(i, c);

            printf("%15s ",
                    ((k > iMax) || (pH = (TH1 *)aHistograms.At(k)) == NULL)
                    ? "-----" : pH->GetName());
        }

        printf("\n");
    }
}

// -----------------------------------------------------------------------------

void HPidHistogramsCont::Draw(const Option_t *pO)
{
    if( ! gPad)
    {
        (gROOT->GetMakeDefCanvas())();
    }

TVirtualPad *pPad = gPad;

    gPad->Clear();

    gPad->Divide(iHistsPerPart, iNParticles, 0, 0, 10);

Int_t iR, iC, iI;
TH1  *h;
char  s[100];

    for(iR = 0; iR < iNParticles; iR++)
    {
        for(iC = 0; iC < iHistsPerPart; iC++)
        {
            pPad->cd(1 + iR * iHistsPerPart + iC);
            if((iI = getIndex(iR, iC)) > aHistograms.GetLast())
                continue;

            if((h = (TH1 *) aHistograms.At(iI)) != NULL)
            {
                sprintf(s, "%s ", (h->GetDimension() == 2) ? "colz" : "");

                if(pO != NULL)
                    sprintf(s + strlen(s), "%s", pO);

                h->Draw(s);
            }
        }
    }
}

// -----------------------------------------------------------------------------

void HPidHistogramsCont::Streamer(TBuffer &R__b)
{
// Stream an object of class HPidHistogramsCont.
// Default Streamer created by Cint + detachHistograms() after reading

UInt_t R__s, R__c;

    if(R__b.IsReading())
    {
    Version_t R__v = R__b.ReadVersion(&R__s, &R__c);

        if (R__v) { }

        HPidParCont::Streamer(R__b);
        R__b >> iNParticles;
        R__b >> iHistsPerPart;
        aParticles.Streamer(R__b);
        aHistograms.Streamer(R__b);
        R__b >> bWarnings;
        R__b.CheckByteCount(R__s, R__c, HPidHistogramsCont::IsA());

        detachHistograms();

        bOk = kTRUE;
    }
    else
    {
        R__c = R__b.WriteVersion(HPidHistogramsCont::IsA(), kTRUE);
        HPidParCont::Streamer(R__b);
        R__b << iNParticles;
        R__b << iHistsPerPart;
        aParticles.Streamer(R__b);
        aHistograms.Streamer(R__b);
        R__b << bWarnings;
        R__b.SetByteCount(R__c, kTRUE);
    }
}

