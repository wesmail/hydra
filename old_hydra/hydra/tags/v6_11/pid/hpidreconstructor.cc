// @(#)$Id: hpidreconstructor.cc,v 1.1 2002-10-30 10:55:33 halo Exp $
//*-- Author : Marcin Jaskula 27/07/2002
//  Modified : Marcin Jaskula 06/09/2002

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// HPidReconstructor                                                          //
//                                                                            //
// A set of all PID algorithms                                                //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include "hcategory.h"
#include "hiterator.h"

#include "hpidreconstructor.h"
#include "hpidalgorithm.h"

#include "hpidreconstructorpar.h"
#include "hruntimedb.h"

#include "hpidtrackcand.h"
#include "hades.h"

#include "hkicktrack.h"

#include <TROOT.h>
#include <TClass.h>

// -----------------------------------------------------------------------------

ClassImp(HPidReconstructor)

// -----------------------------------------------------------------------------

#define SAFE_DELETE(A) { if(A) { delete (A); A = NULL; }}

// -----------------------------------------------------------------------------

HPidReconstructor::HPidReconstructor(Option_t par[])
    : HReconstructor("PidReconstructor", "Set of all PID algorithms")
{
// Default constructor.

    setDefault();
    setParameters(par);
}

// -----------------------------------------------------------------------------

HPidReconstructor::HPidReconstructor(Text_t name[], Text_t title[],
                                    Option_t par[])
    : HReconstructor(name, title)
{
// Constructor with names

    setDefault();
    setParameters(par);
}

// -----------------------------------------------------------------------------

HPidReconstructor::~HPidReconstructor(void)
{
// Delete all alocated objects and ALL ALGORITHMS

    bInDelete = kTRUE;

    SAFE_DELETE(pitInput);
    SAFE_DELETE(pitList);

    if(pAlgorithms != NULL)
    {
        pAlgorithms->SetOwner();
        delete pAlgorithms;
    }

    bInDelete = kFALSE;

    if(aMergedAlgs != NULL)
    {
        delete [] aMergedAlgs;
        aMergedAlgs = NULL;
    }
}

// -----------------------------------------------------------------------------

void HPidReconstructor::setDefault(void)
{
// Called in constructors to set default valuest to the members

    pInputCat = NULL;
    pitInput  = NULL;
    pOutCat   = NULL;
    bInDelete = kFALSE;
    pClass    = NULL;

    for(Int_t i = 0; i < kMaxParticles; i++)
        aParticles[i] = nPID_DUMMY;

    pAlgorithms = new TList();
    pitList = pAlgorithms->MakeIterator();

    iParticles = 0;

    pParams = NULL;

    bInitOk = kFALSE;

    aMergedAlgs = NULL;
    bMerge = kTRUE;
    bPDF   = kTRUE;
    bCL    = kTRUE;
}

// -----------------------------------------------------------------------------

void HPidReconstructor::setParameters(Option_t par[])
{
// Set parameters by names. Options (may be separated by comma or blank chars):
// PDF    - calculate PDF
// CL     - calculate CL
// MERGE  - merge the output PDF
// DEBUGX - debug info, if X set then it define the debug info level

TString s = par;
char   *p;

    s.ToUpper();

    bPDF   = (strstr(s.Data(), "PDF") != NULL);
    bCL    = (strstr(s.Data(), "CL") != NULL);
    bMerge = (strstr(s.Data(), "MERGE") != NULL);

    iDebug = 0;
    if((p = strstr(s.Data(), "DEBUG")) != NULL)
    {
        if(sscanf(p + strlen("DEBUG"), "%d", &iDebug) <= 0)
            iDebug = 0;
    }

    if((bMerge) && ( ! bPDF))
        Error("setParameters", "MERGE requested while PDF not set");
}

// -----------------------------------------------------------------------------

Bool_t HPidReconstructor::init(void)
{
// Initialize the reconstructora and call init of all algorithms

HPidAlgorithm *pAlg;
Bool_t         bReturn = kTRUE;
char           sClassName[100];
Int_t          i;

    bInitOk = kFALSE;

    if(( ! bPDF) && ( ! bCL))
    {
        Error("init", "No output defined");
        return kFALSE;
    }

    if((bMerge) && ( ! bPDF))
    {
        Error("init", "MERGE requested while PDF not set");
        return kFALSE;
    }

    // parameters
    if((pParams = (HPidReconstructorPar *)gHades->getRuntimeDb()
                        ->getContainer(PIDRECONSTRUCTORPAR_NAME)) == NULL)
    {
        Error("init", "Cannot get parameters: %s", PIDRECONSTRUCTORPAR_NAME);
        return kFALSE;
    }

    // input category
    if((pInputCat = buildPidTrackCategory()) == NULL)
    {
        Error("init", "Cannot build HPidTrackCand Category");
        return kFALSE;
    }

    if((pitInput = (HIterator *)pInputCat->MakeIterator()) == NULL)
    {
        Error("init", "Cannot make an iterator for HPidTrackCand Category");
        return kFALSE;
    }

    // output category
    if(pAlgorithms->GetSize() < 1)
    {
        Error("init", "At least one algorithm must be set");
        return kFALSE;
    }

    if(iParticles < 1)
    {
        Error("init", "At least one particle id must be set");
        return kFALSE;
    }

    i = pAlgorithms->GetSize();
    iVectPerAlg = 1;
    if((bPDF) && (bCL))
    {
        i *= 2;
        iVectPerAlg = 2;
    }

    if(bMerge)
        i += NUM_MERGING_VECT;

    sprintf(sClassName, "HPidCandidate_%d_%d", i, iParticles);

    if((pClass = gROOT->GetClass(sClassName)) == NULL)
    {
        Error("init", "Class %s not defined\n"
                "See documentation for class HPidCandidate", sClassName);

        return kFALSE;
    }

    if((pOutCat = buildPidCandidateCategory(sClassName)) == NULL)
    {
        Error("init", "Cannot build HPidCandidate category");
        return kFALSE;
    }

    if(pAlgorithms->GetSize() > 0)
    {
        if((aMergedAlgs = new Bool_t[pAlgorithms->GetSize()]) == NULL)
        {
            Error("init", "Cannot create array for algorithm's flag");
            return kFALSE;
        }

        memset(aMergedAlgs, 0, pAlgorithms->GetSize() * sizeof(Bool_t));
    }

    pitList->Reset();
    i = 0;
    while((pAlg = (HPidAlgorithm *)pitList->Next()) != NULL)
    {
        bReturn &= pAlg->init();
        aMergedAlgs[i++] = pAlg->getMerge();
    }

    bInitOk = bReturn;

    return bReturn;
}

// -----------------------------------------------------------------------------

Bool_t HPidReconstructor::reinit(void)
{
// Reinitialize the reconstructora and call reinit of all algorithms

HPidAlgorithm *pAlg;
Bool_t         bReturn = bInitOk;

    if(bInitOk == kFALSE)
        Warning("reinit", "HPidTrackFiller::init() didn't succeed");

    pitList->Reset();
    while((pAlg = (HPidAlgorithm *)pitList->Next()) != NULL)
        bReturn &= pAlg->reinit();

    if(bInitOk)
        bInitOk = bReturn;

    return bReturn;
}

// -----------------------------------------------------------------------------

Bool_t HPidReconstructor::finalize(void)
{
// Finalize the reconstructor and all algorithms

HPidAlgorithm *pAlg;
Bool_t         bReturn = kTRUE;

    pitList->Reset();
    while((pAlg = (HPidAlgorithm *)pitList->Next()) != NULL)
        bReturn &= pAlg->finalize();

    return bReturn;
}

// -----------------------------------------------------------------------------

Int_t HPidReconstructor::execute(void)
{
// Call execute of all algorithms and all HPidTrackAlgorithms
// Calculate the merged output vector as well

HPidAlgorithm *pAlg;
HPidTrackCand *pTrack;
HPidCandidate *pCand;
Int_t          iAlgs;
Int_t          iObjIdx;
Int_t          iTrack = 0;
Float_t       *pPDF;
Float_t       *pCL;

    if(bInitOk == kFALSE)
    {
        Error("execute", "Class not initialized");
        return -1;
    }

    if(pitInput == NULL)
    {
        Error("execute", "pitInput == NULL");
        return 0;
    }

    pitInput->Reset();
    while((pTrack = (HPidTrackCand *)pitInput->Next()) != NULL)
    {
        if(iDebug)
            printf(".");

        if((pCand = getNextSlot()) == NULL)
            return 0;

        pCand->setCandidateId(iTrack++);

        pitList->Reset();
        iAlgs = 0;

        while((pAlg = (HPidAlgorithm *)pitList->Next()) != NULL)
        {
            pPDF = pCL = NULL;
            iObjIdx = iAlgs;

            if(bPDF)
            {
                pPDF = pCand->getValuesVector(iAlgs);
                pCand->setAlgorithm(iAlgs, pAlg->getAlgId());
                iAlgs++;
            }

            if(bCL)
            {
                pCL = pCand->getValuesVector(iAlgs);
                pCand->setAlgorithm(iAlgs,
                            pAlg->getAlgId() + CL_ALGORITHM_OFFSET);

                iAlgs++;
            }

            if(iDebug > 1)
                printf("Alg: %d %p %p\n", iAlgs, pPDF, pCL);

            pAlg->execute(pTrack, pPDF, pCL, pCand->getPidObjIds()[iObjIdx]);
        }

        if(bMerge)
            merge(pTrack, pCand);
    }

    if(iDebug)
        printf("\n");

    return 0;
}

// -----------------------------------------------------------------------------

HPidCandidate* HPidReconstructor::getNextSlot(void)
{
// Get next slot from the output category

HPidCandidate    *pOut = NULL;
static HLocation  locDummy;

    if(pOutCat == NULL)
    {
        Error("getNextSlot", "Output category not set: use init/reinit");
        return NULL;
    }

    if((pOut = (HPidCandidate *) pOutCat->getNewSlot(locDummy)) == NULL)
    {
        Error("getNextSlot", "No new slot");
        return NULL;
    }

    pOut = (HPidCandidate *) pClass->New(pOut);

    pOut->setCandidateId(-1);
    pOut->setParticles(aParticles);

    return pOut;
}

// -----------------------------------------------------------------------------

void HPidReconstructor::merge(HPidTrackCand *pTrack, HPidCandidate *pCand)
{
// Merge values for all particles and set output vectors to the end of output
// in the HPidCandidate. Algorithms from range [0, iMaxAlg) which have
// flag getMerge() on are included !!!

Int_t   iPart;
Int_t   iAlg;
Float_t fVal, fTheta, fMom, fSum, f;
Int_t   iMax = pAlgorithms->GetSize() * iVectPerAlg;
Float_t afIntens[kMaxParticles];
HKickTrack *pKT;
Int_t   iSys;
static Float_t R2D = 180.0 / TMath::Pi();

    pCand->setAlgorithm(iMax, algMerged);

    // merge data
    for(iPart = 0;
            (iPart < kMaxParticles) && (aParticles[iPart] != nPID_DUMMY);
            iPart++)
    {
        fVal = 1.0f;

        for(iAlg = 0; iAlg < iMax; iAlg++)
        {
            if(aMergedAlgs[iAlg] == kFALSE)
                continue;

            // throw out artifical values (the algorithm doesn't know)
            if((f = pCand->getValue_Idx(iVectPerAlg * iAlg, iPart)) < 0.0)
                continue;

            if((fVal *= f) == 0.0f)
                break;
        }

        pCand->setValue_Idx(iMax, iPart, fVal);
    }

    pCand->setAlgorithm(iMax + 1, algWithIntensites);

    // it is not the full track
    if(pTrack->getKickTrackId() < 0)
        return;

    // calculate Bayes results
    memset(afIntens, 0, sizeof(afIntens));
    if((pKT = pTrack->getKickTrack()) == NULL)
    {
        Error("merge", "Cannot get HKickTrack for HPidTrackCand");
        return;
    }

    fMom   = pKT->getP();
    fTheta = R2D * pKT->getTheta();
    iSys   = pKT->getSystem();

    // calculate parameters for all particles
    fSum = 0.0f;
    for(iPart = 0; (iPart < kMaxParticles) && (aParticles[iPart] > 0); iPart++)
    {
        if((fVal = pParams->getIntensity(aParticles[iPart],
                            iSys, fMom, fTheta)) <= 0.0)
        {
            continue;
        }

        afIntens[iPart] = fVal * pCand->getValue_Idx(iMax, iPart);

        fSum += afIntens[iPart];
    }

    // no sum calculated
    if(fSum <= 0.0f)
        return;

    for(iPart = 0; (iPart < kMaxParticles) && (aParticles[iPart] > 0); iPart++)
        pCand->setValue_Idx(iMax + 1, iPart, afIntens[iPart] / fSum);
}

// -----------------------------------------------------------------------------

Bool_t HPidReconstructor::addAlgorithm(HPidAlgorithm *pAlg)
{
// Add the algorithm to the set of algorithms.
// The id of the algorithm must be unique in the set
// The reconstructor of the algorithm is set to the current one

    if(pAlg == NULL)
    {
        Error("addAlgorithm", "NULL algorithm");
        return kFALSE;
    }

    if(getAlgorithm(pAlg->getAlgId()) != NULL)
    {
        Error("addAlgorithm", "Algorithm %d already in the set",
                        pAlg->getAlgId());
        return kFALSE;
    }

    pAlgorithms->AddLast(pAlg);

    pAlg->setReconstructor(this);

    return kTRUE;
}

// -----------------------------------------------------------------------------

HPidAlgorithm* HPidReconstructor::getAlgorithm(const TString &sName) const
{
// Find an algorithm in the set by it's name.
// Returns the first algorithm in the list with specified name (name do not
// need to be unique)

HPidAlgorithm *pAlg;

    pitList->Reset();
    while((pAlg = (HPidAlgorithm *)pitList->Next()) != NULL)
    {
        if(sName.CompareTo(pAlg->getName()) == 0)
            return pAlg;
    }

    return NULL;
}

// -----------------------------------------------------------------------------

HPidAlgorithm* HPidReconstructor::getAlgorithm(EnumPidAlgorithm_t eId) const
{
// Find an algorithm in the set by it's id.

HPidAlgorithm *pAlg;

    pitList->Reset();
    while((pAlg = (HPidAlgorithm *)pitList->Next()) != NULL)
    {
        if(eId == pAlg->getAlgId())
            return pAlg;
    }

    return NULL;
}

// -----------------------------------------------------------------------------

Bool_t HPidReconstructor::removeAlgorithm(HPidAlgorithm *pAlg)
{
// Remove the object from the list if it's in the set

    if(bInDelete)
        return kTRUE;

    if(pAlgorithms->Remove(pAlg) == NULL)
    {
        Error("removeAlgorithm", "Algorithm %p not in the set", pAlg);
        return kFALSE;
    }

    return kTRUE;
}

// -----------------------------------------------------------------------------

Bool_t HPidReconstructor::removeAlgorithm(const TString &sName)
{
// Remove the object from the list by its name.

HPidAlgorithm *pAlg;

    if(bInDelete)
        return kTRUE;

    if((pAlg = getAlgorithm(sName)) == NULL)
    {
        Error("removeAlgorithm", "Algorithm %s not in the set", sName.Data());
        return kFALSE;
    }

    return removeAlgorithm(pAlg);
}

// -----------------------------------------------------------------------------

Bool_t HPidReconstructor::removeAlgorithm(EnumPidAlgorithm_t eId)
{
// Remove the object from the list by its id.

HPidAlgorithm *pAlg;

    if(bInDelete)
        return kTRUE;

    if((pAlg = getAlgorithm(eId)) == NULL)
    {
        Error("removeAlgorithm", "Algorithm %d not in the set", eId);
        return kFALSE;
    }

    return removeAlgorithm(pAlg);
}

// -----------------------------------------------------------------------------

Short_t HPidReconstructor::getParticleId(Int_t iPos) const
{
// Returns particle type in iPos position

    if((iPos < 0) || (iPos > iParticles))
    {
        Error("getParticleId", "%d out of bounds", iPos);
        return -1;
    }

    return aParticles[iPos];
}

// -----------------------------------------------------------------------------

Int_t HPidReconstructor::getParticleIndex(Short_t nType) const
{
// Returns particle nType position in aParticles vector or -1 if not set

    for(Int_t i = 0; i < kMaxParticles; i++)
    {
        if(aParticles[i] == nType)
            return i;
    }

    return -1;
}

// -----------------------------------------------------------------------------

Int_t HPidReconstructor::addParticleId(Short_t nType)
{
// Add nType type to the array of the particles the user is interested in.
// Returns the position of the particle in the array or
// -1 - no place in the array
// -2 - the particle alraedy added to the array

    for(Int_t i = 0; i < kMaxParticles; i++)
    {
        if(aParticles[i] == nType)
        {
            Error("addParticleId", "Particle %d already in the array", nType);
            return -2;
        }

        if(aParticles[i] == nPID_DUMMY)
        {
            aParticles[i] = nType;

            if(iParticles <= i)
                iParticles = i + 1;

            return i;
        }
    }

    Error("addParticleId", "No place for new particle type in the array");

    return -1;
}

// -----------------------------------------------------------------------------

void HPidReconstructor::setParticleId(Int_t iPos, Short_t nType)
{
// Set the iPos element of the array of the particle types to nType.
// Bounds are checked.

    if((iPos < 0) || (iPos >= kMaxParticles))
        Error("setParticleId", "%d out of bounds", iPos);
    else
    {
        aParticles[iPos] = nType;

        if(iParticles <= iPos)
            iParticles = iPos + 1;
    }
}

// -----------------------------------------------------------------------------

void HPidReconstructor::setParticleIds(Short_t aIds[], Int_t iSize)
{
// copy ids of particles form the input array

    if(iSize <= 0)
    {
        Error("setParticleIds", "iSiza <= 0");
        return;
    }

    if(iSize > kMaxParticles)
    {
        Warning("setParticleIds", "Only %d from %d ids used", kMaxParticles,
                iSize);

        iSize = kMaxParticles;
    }

    memcpy(aParticles, aIds, iSize * sizeof(Short_t));

    iParticles = iSize;
    for(Int_t i = iParticles; i < kMaxParticles; i++)
        aParticles[i] = nPID_DUMMY;
}

// -----------------------------------------------------------------------------

void HPidReconstructor::setDefaultParticleIds(void)
{
// Sets default set of the particles:
// 2(e+), 3(e-), 8(pi+), 9(pi-), 14(p), 45(d), 500 and 501 (fakes)

Short_t aPartIds[] =
{
    2,  // e+
    3,  // e-
    8,  // pi+
    9,  // pi-
   14,  // p
   45,  // d
  500,  // fake negative
  501   // fake positive
};

    setParticleIds(aPartIds, sizeof(aPartIds) / sizeof(Short_t));
}

// -----------------------------------------------------------------------------

void HPidReconstructor::print(void) const
{
// Print the list of all algorithms

HPidAlgorithm *pAlg;
Int_t          i;

    printf("PidReconstructor:print():\nParticles: ");
    for(i = 0; (i < iParticles) && (aParticles[i] >= 0); i++)
        printf(" %d", aParticles[i]);

    printf("\nOutputs      : %s %s\n", (bPDF) ? "PDF" : "",
                                        (bCL) ? "CL" : "");
    printf("\nMerge outputs: %s\n", (bMerge) ? "YES" : "NO");
    printf("\nAlgorithms:\n");

    pitList->Reset();
    while((pAlg = (HPidAlgorithm *)pitList->Next()) != NULL)
    {
        printf("* %d %s, weight: %f\n", pAlg->getAlgId(),
                    pAlg->getName().Data(), pAlg->getWeight());
        pAlg->print();
    }
}
