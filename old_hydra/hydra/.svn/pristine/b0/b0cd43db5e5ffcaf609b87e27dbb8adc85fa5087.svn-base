// @(#)$Id: hshowercheckIPU.cc,v 1.5 2008-09-18 13:03:30 halo Exp $
//*-- Author : Jerzy Pietraszko & Marcin Jaskula 02/11/2002

//_HADES_CLASS_DESCRIPTION 
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// HShowerCheckIPU                                                            //
//                                                                            //
// Compare the results of the software IPU emulation with the data from MU    //
// The class makes some debug histograms and save them in finalize() method.  //
// The location of the output is set cY setOutputFile() method.               //
// If pName is empty then the histograms are written to gFile                 //
// If it is `!' then the histograms are not written anywhere                  //
// otherwise there are stored with the RECREATED file named pName             //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include "hcategory.h"
#include "hiterator.h"

#include "hshowercheckIPU.h"

#include "hades.h"
#include "hevent.h"

#include "hshowerhitIPU.h"
#include "hmatchushower.h"

#include "showerdef.h"
#include "triggerinfodef.h"

#include "hqamaker.h"
#include "hqahistograms.h"

#include "TH2.h"
#include "TList.h"

// -----------------------------------------------------------------------------

ClassImp(HShowerCheckIPU)

// -----------------------------------------------------------------------------

#define SAFE_DELETE(A) { if(A) { delete (A); A = NULL; }}

// -----------------------------------------------------------------------------

HShowerCheckIPU::HShowerCheckIPU(Int_t iRatio, const Char_t *pOut)
    : HReconstructor("ShowerCheckIPU",
                    "Check shower IPU with the software data")
{
// Default constructor.

    setDefault(iRatio, pOut);
}

// -----------------------------------------------------------------------------

HShowerCheckIPU::HShowerCheckIPU(Text_t name[], Text_t title[],
                                    Int_t iRatio, const Char_t *pOut)
    : HReconstructor(name, title)
{
// Constructor with names

    setDefault(iRatio, pOut);
}

// -----------------------------------------------------------------------------

HShowerCheckIPU::~HShowerCheckIPU(void)
{
// Delete all alocated objects

    SAFE_DELETE(pitAna);
    SAFE_DELETE(pitIPU);

    // histograms are removed by root
}

// -----------------------------------------------------------------------------

void HShowerCheckIPU::setDefault(Int_t iRatio, const Char_t *pOut)
{
// Called in constructors to set default valuest to the members

    pitAna = NULL;
    pitIPU = NULL;

    memset(asAna, 0, sizeof(asAna));
    memset(asIPU, 0, sizeof(asIPU));
    memset(ahOut, 0, sizeof(ahOut));

    iInfoRatio  = iRatio;
    iVerbose    = 1;
    sOutputFile = pOut;

    iEvents     = 0;
    iErrors     = 0;

    iHistEvents = 10000;

    bInitOk     = kFALSE;

    pQA         = NULL;
}

// -----------------------------------------------------------------------------

Bool_t HShowerCheckIPU::init(void)
{
// Get the iterators and create the histogrmas

    bInitOk = kFALSE;

HCategory *pCat;

    if((gHades == NULL) || (gHades->getCurrentEvent() == NULL))
    {
        Error("init", "Cannot get current event");
        return kFALSE;
    }

    // input categories
    if((pCat = gHades->getCurrentEvent()->getCategory(catShowerHitIPU)) == NULL)
    {
        Error("init", "Cannot get catShowerHitIPU category");
        return kFALSE;
    }

    if((pitAna = (HIterator *)pCat->MakeIterator()) == NULL)
    {
        Error("init", "Cannot make an iterator for catShowerHitIPU category");
        return kFALSE;
    }

    if((pCat = gHades->getCurrentEvent()->getCategory(catMatchUShower)) == NULL)
    {
        Error("init", "Cannot get catMatchUShower category");
        return kFALSE;
    }

    if((pitIPU = (HIterator *)pCat->MakeIterator()) == NULL)
    {
        Error("init", "Cannot make an iterator for catMatchUShower category");
        return kFALSE;
    }

    // histograms
Char_t *sNames[] = { "hShowerIPUCorrXX", "hShowerIPUCorrYY", "hShowerIPUCorrPos",
                    "hShowerIPUNoCorrAna", "hShowerIPUNoCorrIPU",
                    "hShowerIPUNoCorrXY",
                    "hShowerIPUDoubleAna", "hShowerIPUDoubleIPU",
                    "hShowerIPU_IPUPerEvent", "hShowerIPU_AnaPerEvent",
                    "hShowerIPUErrorsPerEvent"};

    for(Int_t i = 0; i < kHistMax; i++)
    {
        if(i >= kHistIPUInEvent)
        {
            if(iHistEvents < 1)
                iHistEvents = 10000;

            if((ahOut[i] = new TH1F(sNames[i], sNames[i], iHistEvents,
                            1, iHistEvents)) == NULL)
            {
                Error("init", "Cannot create histogram %d %s", i, sNames[i]);
                return kFALSE;
            }

            ahOut[i]->SetLineColor(i + 1 - kHistIPUInEvent);

            continue;
        }

        if(i == kHistNoCorrXY)
        {
            if((ahOut[i] = new TH2F(sNames[i], "No corr: Pos_Ana-Pos_IPU",
                            63, -31.5, 31.5, 63, -31.5, 31.5)) == NULL)
            {
                Error("init", "Cannot create histogram %d %s", i, sNames[i]);
                return kFALSE;
            }

            continue;
        }

        if((ahOut[i] = new TH2F(sNames[i], sNames[i],
                            (i <= kHistCorrY) ? 33 : 32,
                            (i <= kHistCorrY) ? -1 : 0, 32,
                            (i <= kHistCorrY) ? 33 : 32,
                            (i <= kHistCorrY) ? -1 : 0, 32)) == NULL)
        {
            Error("init", "Cannot create histogram %d %s", i, sNames[i]);
            return kFALSE;
        }
    }

    bInitOk = kTRUE;

    return kTRUE;
}

// -----------------------------------------------------------------------------

Bool_t HShowerCheckIPU::finalize(void)
{
// Writes the output

TFile  *pFile    = NULL;
TFile  *pTmpFile = gFile;
Bool_t  bReturn;
Bool_t  bClose   = kFALSE;

    if( ! bInitOk)
    {
        Error("reinit", "Class not initialized");
        return kFALSE;
    }

    //    if(pQA != NULL)
    //        putHistograms(pQA->getHists()->getHistList());

    if(sOutputFile.CompareTo("!") == 0)
        return kTRUE;

    if((sOutputFile.IsNull()) || (sOutputFile.Length() <= 0))
        pFile = gHades->getOutputFile();
    else
    {
        pFile = new TFile(sOutputFile.Data(), "RECREATE");
        bClose = kTRUE;
    }

    pFile->cd();

    bReturn = writeHistograms();

    if(pFile != pTmpFile)
    {
        pTmpFile->cd();

        if(bClose)
        {
            pFile->Close();
            delete pFile;
        }
    }

    writeInfo();

    return bReturn;
}

// -----------------------------------------------------------------------------

Int_t HShowerCheckIPU::execute(void)
{
// Make the loop over all entries and check the matching

    if( ! bInitOk)
    {
        Error("execute", "Class not initialized");
        return -1;
    }

    iEvents++;
    iAnaNum = 0;
    iIPUNum = 0;

HShowerHitIPU *pAna;
HMatchUShower *pIPU;
Int_t          iInError = iErrors;

    pitAna->Reset();
    while((pAna = (HShowerHitIPU *)pitAna->Next()) != NULL)
    {
        if(iAnaNum >= kMaxAna)
        {
            Error("execute", "Event: %6d: Too many HShowerHitIPU (> %d)",
                        iEvents, kMaxAna);
            break;
        }

        asAna[iAnaNum++].set(pAna->getSector(), pAna->getCol(), pAna->getRow());
    }

    pitIPU->Reset();
    while((pIPU = (HMatchUShower *)pitIPU->Next()) != NULL)
    {
        if(iIPUNum >= kMaxIPU)
        {
            Error("execute", "Event: %6d: Too many HMatchUShower (> %d)",
                        iEvents, kMaxIPU);
            break;
        }

        asIPU[iIPUNum++].set(pIPU->getSector(),
                        pIPU->getColumn(), pIPU->getRow());
    }

    checkDoubles();
    matchHits();

    if((iVerbose > 1) && (iInError != iErrors))
        dumpEvent();

    ahOut[kHistIPUInEvent]->Fill(iEvents, iIPUNum);
    ahOut[kHistAnaInEvent]->Fill(iEvents, iAnaNum);
    ahOut[kHistErrorInEvent]->Fill(iEvents, iErrors - iInError);

    if((iInfoRatio > 0) && ((iEvents % iInfoRatio) == 0))
        writeInfo();

    return 0;
}

// -----------------------------------------------------------------------------

void HShowerCheckIPU::checkDoubles(void)
{
// Check any doubles in the hits

Int_t iType;

    for(iType = 0; iType <= 1; iType++)
    {
        iErrors += checkDoubles((iType == 0) ? asAna : asIPU,
                                (iType == 0) ? iAnaNum : iIPUNum,
                                iType);
    }
}

// -----------------------------------------------------------------------------

Int_t HShowerCheckIPU::checkDoubles(HShowerHitIPUAddres *pArr,
                            Int_t iMax, Int_t iType)
{
// Check an array of HShowerHitIPUAddres for doubles

static const Char_t   *sNames[] = { "HShowerHitIPU", "HMatchUShower" };
HShowerHitIPUAddres *pCur;
Int_t                i, k;
Int_t                iErr = 0;

    for(i = 0; i < iMax - 1; i++, pArr++)
    {
        if(pArr->isMasked())
            continue;

        pCur = pArr;
        for(k = i + 1; k < iMax; k++)
        {
            pCur++;
            if( ! (*pArr == *pCur))
                continue;

            if(iVerbose)
            {
                Error("checkDoubles", "\nEvent: %6d: Double hit: %s (%s) "
                        " %d <-> %d / %d",
                        iEvents, sNames[iType], pArr->toString(),
                        i, k, iMax);
            }

            iErr++;
            ahOut[kHistDoubleAna + iType]->Fill(pArr->cX, pArr->cY);

            pCur->mask();
        }
    }

    return iErr;
}

// -----------------------------------------------------------------------------

void HShowerCheckIPU::matchHits(void)
{
// Check matches

Int_t iA, iI;
Int_t iNotMatched_A = 0;
Int_t iNotMatched_I = 0;

    for(iA = 0; iA < iAnaNum; iA++)
    {
        // was Double_t         if(asAna[iA].isMasked())
            continue;

        // check matching
        for(iI = 0; iI < iIPUNum; iI++)
        {
            // was used
            if(asIPU[iI].isMasked())
                continue;

            if( ! (asAna[iA] == asIPU[iI]))
                continue;

            // matched
            ahOut[kHistCorrX]->Fill(asAna[iA].cX, asAna[iA].cX);
            ahOut[kHistCorrY]->Fill(asAna[iA].cY, asAna[iA].cY);
            ahOut[kHistCorr]->Fill(asAna[iA].cX, asAna[iA].cY);
            asAna[iA].mask();
            asIPU[iI].mask();

            break;
        }

        if(iI < iIPUNum)
            continue;

        // no match
        if(iVerbose)
        {
            Error("matchHits", "\nEvent: %4d: No matched hit "
                        "HShowerHitIPU (%s)  %d / %d",
                        iEvents, asAna[iA].toString(), iA, iAnaNum);
        }

        iErrors++;

        ahOut[kHistNoCorrAna]->Fill(asAna[iA].cX, asAna[iA].cY);

        iNotMatched_A++;
    }

    // find all not matched HMatchUShowers
    for(iI = 0; iI < iIPUNum; iI++)
    {
        if(asIPU[iI].isMasked())
            continue;

        // no match
        if(iVerbose)
        {
            Error("matchHits", "\nEvent: %4d: No matched hit "
                        "HMatchUShowers (%s)  %d / %d",
                        iEvents, asIPU[iI].toString(), iI, iIPUNum);
        }

        iErrors++;

        ahOut[kHistNoCorrIPU]->Fill(asIPU[iI].cX, asIPU[iI].cY);

        iNotMatched_I++;
    }

    if((iNotMatched_A == 0) || (iNotMatched_I == 0))
        return;

    // fill difference of all not matched
    for(iA = 0; iA < iAnaNum; iA++)
    {
        // was Double_t         if(asAna[iA].isMasked())
            continue;

        // check matching
        for(iI = 0; iI < iIPUNum; iI++)
        {
            // was used
            if(asIPU[iI].isMasked())
                continue;

            ahOut[kHistNoCorrXY]->Fill(asAna[iA].cX - asIPU[iI].cX,
                                asAna[iA].cY - asIPU[iI].cY);
        }
    }
}

// -----------------------------------------------------------------------------

Bool_t HShowerCheckIPU::putHistograms(TList *pList)
{
Char_t s[100];

    if(pList == NULL)
    {
        Error("putHistograms", "List == NULL");
        return kFALSE;
    }

    for(Int_t i = kHistIPUInEvent; i < kHistMax; i++)
    {
printf("Makeing %d\n", i);
	sprintf(s, "trigger--%s", (i == kHistIPUInEvent) ? "" : "");

        if(ahOut[i] != NULL)
            pList->Add(ahOut[i], s);
    }

printf("Bye bye\n");
    return kTRUE;
}

// -----------------------------------------------------------------------------

Bool_t HShowerCheckIPU::writeHistograms(void)
{
// Writes histogram to the gFile

    if(gFile == NULL)
    {
        Error("writeHistograms", "gFile == NULL");
        return kFALSE;
    }

    if(( ! gFile->IsOpen()) || ( ! gFile->IsWritable()))
    {
        Error("writeHistograms", "File %s not open for writing",
                    gFile->GetName());

        return kFALSE;
    }

    for(Int_t i = 0; i < kHistMax; i++)
    {
        if(ahOut[i] != NULL)
            ahOut[i]->Write();
    }

    return kTRUE;
}

// -----------------------------------------------------------------------------

void HShowerCheckIPU::dumpEvent(void) const
{
// Dump all hits to the stdout

    printf("Event: %d\n", iEvents);
    for(Int_t i = 0; i < ((iAnaNum > iIPUNum) ? iAnaNum : iIPUNum); i++)
    {
        printf("%5d  A : %-30s ", i, (i < iAnaNum) ? asAna[i].toString() : "");
        printf("I : %-30s\n", (i < iIPUNum) ? asIPU[i].toString() : "");
    }
}

// -----------------------------------------------------------------------------

void HShowerCheckIPU::writeInfo(void) const
{
// Write info about the status

    printf("HShowerCheckIPU: Events: %6d  Errors: %6d\n", iEvents, iErrors);
}
