// @(#)$Id: hpidcandidate.cc,v 1.5 2002-12-03 14:30:38 jaskula Exp $
//*-- Author : Marcin Jaskula 27/07/2002
// Modified  : Marcin Jaskula 06/09/2002
// Modified  : Marcin Jaskula 16/09/2002
//           - now this is an abstract class
// Modified  : Marcin Jaskula 31/10/2002
//           - buildPidCandidateCategory moved from global to static function
// Modified  : Marcin Jaskula 01/12/2002
//           - new method getPidCandidate()
//           - getCandidate() rewritten

////////////////////////////////////////////////////////////////////////////////
//
// HPidCandidate
//
// This is the base class for all HPidCandidat classes.
// To make a specified class for iAlg algorithms and iPart particle spices
// change directory to bin subdir and run shell script:
// makeCandidate [iAlg] [iPart]
// It makes source for class HPidCandidate_[iAlg]_[iPart]
// Do the list of actions defined in the info from the script, compile it and
// start Your script once again.
//
// The container consists of all values returend by PidAlgorithms
// for HPidTrackCand with id = nCandidate.
// The idea of this container was to make it as flexible as possible and do not
// fix the number and type of algorithms and reconstructed partices for which
// the values are stored. The organization of the data in the container looks
// like:
//
//    ...... pAlogrithms ......
// .  -------------------------
// .  |                       |
// p  |                       |
// P  |                       |
// a  |                       |
// r  |                       |
// t  |        pValues        |
// i  |                       |
// c  |                       |
// l  |                       |
// e  |                       |
// s  |                       |
// .  |                       |
// .  |                       |
// .  -------------------------
//
// Pid
// Obj ------------------------
// id
//
// Ids of used algorithms are stored in pAlogrithms vector constisting of
// values from EnumPidAlgorithm_t (piddef.h). Ids larger then 100 mean
// values for CL for algorihm (id - 100)
// Ids of particles are stored in pParticles vector (from Geant).
//
// The values returned from the algorithms are stored in 2D array
// pValues[PID_ALG][PARTICLE] - where indecs corresponds to values in
// pAlogrithms and pParticles vectors.
//
// If an algorithm returns an Id of some object specific for the alogrithm, this
// value is stored in pPidObjId vector in a place correspondig to the Id of
// the algorithm in pAlogrithms vector.
//
// The methods from the class give interface to access all data in an easy way.
//
////////////////////////////////////////////////////////////////////////////////

#pragma implementation

#include "hpidcandidate.h"
#include "hpidtrackcand.h"

#include "hades.h"
#include "hevent.h"
#include "hlinearcategory.h"

#include <TError.h>
#include <TROOT.h>

// -----------------------------------------------------------------------------

ClassImp(HPidCandidate)

// -----------------------------------------------------------------------------

HPidCandidate::HPidCandidate(void)
{
// Default constructor

    nCandidate = -1;
}

// -----------------------------------------------------------------------------

HPidCandidate::~HPidCandidate(void)
{
// Default destructor. Do not do nothing.
}

// -----------------------------------------------------------------------------

void HPidCandidate::Clear(Option_t *)
{
// Clears the arrays setting them to the default values

Int_t i, iMax;

Int_t *pAlgs      = getAlgorithms();
Short_t            *pPidObjIds = getPidObjIds();
Short_t            *pParticles = getParticles();
Float_t            *pVals      = getValues();

    nCandidate = -1;

    if(((iMax = getNAlgorithms()) > 0)
            && ((pAlgs != NULL) || (pPidObjIds != NULL)))
    {
        for(i = 0; i < iMax; i++)
        {
            if(pAlgs != NULL)
                pAlgs[i] = algNotSet;

            if(pPidObjIds != NULL)
                pPidObjIds[i]  = -1;
        }
    }

    if(((iMax = getNParticles()) > 0) && (pParticles != NULL))
    {
        for(i = 0; i < iMax; i++)
            pParticles[i] = 0;
    }

    if(pVals != NULL)
    {
        iMax = getNParticles() * getNAlgorithms();
        for(i = 0; i < iMax; i++)
            pVals[i] = nUNKNOWN;
    }
}

// -----------------------------------------------------------------------------

Int_t HPidCandidate::print(void) const
{
// Print the contents of the container

Int_t *pAlgs;
Short_t            *pParts;
Short_t            *pObjId;
Float_t            *pVals;
UInt_t              iAlg;
UInt_t              iPart;

    printf(" P \\ A |");

    if((pAlgs = getAlgorithms()) == NULL)
        printf("No algorithms array");

    if((pParts = getParticles()) == NULL)
        printf("No particles array");

    if((pVals = getValues()) == NULL)
        printf("No values array");

    if((pAlgs != NULL) && (pParts != NULL) && (pVals != NULL))
    {
        for(iAlg = 0; iAlg < getNAlgorithms(); iAlg++)
            printf("  %6d |", pAlgs[iAlg]);

        printf("\n-------+");

        for(iAlg = 0; iAlg < getNAlgorithms(); iAlg++)
            printf("---------+");

        printf("\n");

        for(iPart = 0; iPart < getNParticles(); iPart++)
        {
            printf("%6d |", pParts[iPart]);
            for(iAlg = 0; iAlg < getNAlgorithms(); iAlg++)
                printf("%8.4f |", pVals[getValueIndex(iAlg, iPart)]);

            printf("\n");
        }
    }

    if((pObjId = getPidObjIds()) != NULL)
    {
        printf("\nPidIds |");
        for(iPart = 0; iPart < getNAlgorithms(); iPart++)
            printf("%8d |", pObjId[iPart]);

        printf("\n");
    }

    printf("\n");

    return 0;   // allow to use in T.Draw(...)
}

// -----------------------------------------------------------------------------

UInt_t HPidCandidate::getNParticles(void) const
{
// Return number of the particles. This is an abstract method and it is
// overwitten by the dauther classes.

    Error("getNParticles", "Abstract method");
    return 0;
}

// -----------------------------------------------------------------------------

UInt_t HPidCandidate::getNAlgorithms(void) const
{
// Return number of the algorithms. This is an abstract method and it is
// overwitten by the dauther classes.

    Error("getNAlgorithms", "Abstract method");
    return 0;
}

// -----------------------------------------------------------------------------

HPidTrackCand* HPidCandidate::getCandidate(HCategory *pCat) const
{
// Returns HPidTrackCand object corresponding to nCandidate (if exists)
// Works when pCat is set or gHades->getCurrentEvent() is accessible

    if(nCandidate < 0)
        return NULL;

    return HPidTrackCand::getTrackCand(nCandidate, pCat);
}

// -----------------------------------------------------------------------------

void HPidCandidate::setAlgorithm(UInt_t uiPos, Int_t eAlg)
{
// Set algorithm Id eAlg at uiPos. Arrays bounds are checked

Int_t *pAlgs = getAlgorithms();

    if((pAlgs == NULL) || (uiPos >= getNAlgorithms()))
    {
        Error("setAlgorithm", "out of bound: [0, %d)", getNAlgorithms());
        return;
    }

    pAlgs[uiPos] = eAlg;
}

// -----------------------------------------------------------------------------

void HPidCandidate::setAlgorithms(Int_t aeAlgs[])
{
// Set whole vector of algoritms.
// The input array must have getNAlgorithms() members - NOT CHECKED !!!

Int_t *pAlgs = getAlgorithms();

    if(pAlgs == NULL)
    {
        Error("setAlgorithms", "No algorithms array");
        return;
    }

    memcpy(pAlgs, aeAlgs, sizeof(Int_t) * getNAlgorithms());
}

// -----------------------------------------------------------------------------

Int_t HPidCandidate::getAlgorithm(UInt_t uiPos) const
{
// Get algorithm Id from uiPos.
// algNotSet is return if uiPos is out of bounds.

Int_t *pAlgs = getAlgorithms();

    if((pAlgs == NULL) || (uiPos >= getNAlgorithms()))
    {
        Error("getAlgorithm", "out of bound: [0, %d)", getNAlgorithms());
        return algNotSet;
    }

    return pAlgs[uiPos];
}

// -----------------------------------------------------------------------------

Int_t HPidCandidate::getAlgorithmIndex(Int_t eAlg) const
{
// Find the position of the eAlg algorithm in the array.
// Returns -1 if algorithm not set.
// Existing of the proper algorithms array is checked

Int_t               i, iMax;
Int_t *pAlgs;

    if((pAlgs = getAlgorithms()) == NULL)
    {
        Error("getAlgorithmIndex", "No algorithms array");
        return -1;
    }

    iMax = getNAlgorithms();
    for(i = 0; i < iMax; i++)
    {
        if(pAlgs[i] == eAlg)
            return i;
    }

    return -1;
}

// -----------------------------------------------------------------------------

void HPidCandidate::setParticle(UInt_t uiPos, Short_t nPart)
{
// Sets particle id nPart in position uiPos. Array bounds are checked.

Short_t *pParts = getParticles();

    if((pParts == NULL) || (uiPos >= getNParticles()))
    {
        Error("setParticle", "out of bound: [0, %d)", getNParticles());
        return;
    }

    pParts[uiPos] = nPart;
}

// -----------------------------------------------------------------------------

void HPidCandidate::setParticles(Short_t anPart[])
{
// Set whole vector of particles.
// The input array must have getNParticles() members - NOT CHECKED !!!

Short_t *pParts = getParticles();

    if(pParts == NULL)
    {
        Error("setParticles", "No particles array");
        return;
    }

    memcpy(pParts, anPart, sizeof(Short_t) * getNParticles());
}

// -----------------------------------------------------------------------------

Short_t HPidCandidate::getParticle(UInt_t uiPos) const
{
// Gets particle id nPart at position uiPos.
// Array bounds are checked and -10 returned in case of out of bounds.

Short_t *pParts = getParticles();

    if((pParts == NULL) || (uiPos >= getNParticles()))
    {
        Error("setParticle", "out of bound: [0, %d)", getNParticles());
        return -10;
    }

    return pParts[uiPos];
}

// -----------------------------------------------------------------------------

Int_t HPidCandidate::getParticleIndex(Short_t nPartId) const
{
// Find the position of the nPartId id of the particle in the array.
// Returns -1 if particle id not set.
// Existing of a propper particlesId array is checked.

Int_t    i, iMax;
Short_t *pPart;

    if((pPart = getParticles()) == NULL)
    {
        Error("getParticleIndex", "No particlesId array");
        return -1;
    }

    iMax = getNParticles();
    for(i = 0; i < iMax; i++)
    {
        if(pPart[i] == nPartId)
            return i;
    }

    return -1;
}

// -----------------------------------------------------------------------------

void HPidCandidate::setPidObjectId_Idx(UInt_t uiPos, Short_t nId)
{
// Sets PidObjectId nId in position uiPos. Array bounds are checked.

Short_t *pObjId = getPidObjIds();

    if((pObjId == NULL) || (uiPos >= getNAlgorithms()))
    {
        Error("setPidObjectId_Idx", "out of bound: [0, %d)", getNAlgorithms());
        return;
    }

    pObjId[uiPos] = nId;
}

// -----------------------------------------------------------------------------

void HPidCandidate::setPidObjectId_Alg(Int_t eAlg, Short_t n)
{
// Set PidObjectId n for the algorithm eAlg (if the algorithm is set)

Short_t *pObjId = getPidObjIds();
Int_t    i;

    if(pObjId == NULL)
    {
        Error("setPidObjectId_Alg", "No PidObjIds array");
        return;
    }

    if((i = getAlgorithmIndex(eAlg)) >= 0)
        pObjId[i] = n;
}

// -----------------------------------------------------------------------------

Short_t HPidCandidate::getPidObjectId_Idx(UInt_t uiPos) const
{
// Gets PidObjectId from position uiPos.
// Array bounds are checked and -2 returned in case of out of bounds.

Short_t *pObjId = getPidObjIds();

    if((pObjId == NULL) || (uiPos >= getNAlgorithms()))
    {
        Error("getPidObjectId_Idx", "out of bound: [0, %d)", getNAlgorithms());
        return -2;
    }

    return pObjId[uiPos];
}

// -----------------------------------------------------------------------------

Short_t HPidCandidate::getPidObjectId_Alg(Int_t eAlg) const
{
// Set PidObjectId n for the algorithm eAlg if the algorithm is set.
// Otherwise returns -2 (-1 means no PidObjectId set)

Short_t *pObjId = getPidObjIds();
Int_t    i;

    if(pObjId == NULL)
    {
        Error("setPidObjectId_Alg", "No PidObjIds array");
        return -2;
    }

    return ((i = getAlgorithmIndex(eAlg)) >= 0) ? pObjId[i] : -2;
}

// -----------------------------------------------------------------------------

void HPidCandidate::setValue(Int_t eAlg, Short_t nPartId,
                        Float_t fVal)
{
// Set value fVal for the specified algorithm and particle Id

Float_t *pValues = getValues();
Int_t    iPart, iAlg;

    if(pValues == NULL)
    {
        Error("setValue", "No values array");
        return;
    }

    if(((iAlg = getAlgorithmIndex(eAlg)) < 0)
            || ((iPart = getParticleIndex(nPartId)) < 0))
    {
        return;
    }

    pValues[getValueIndex(iAlg, iPart)] = fVal;
}

// -----------------------------------------------------------------------------

void HPidCandidate::setValue_Idx(UInt_t uiAlgIdx, UInt_t uiPartIdx,
                        Float_t fVal)
{
// Set value fVal for known position in the values array

Float_t *pValues = getValues();
Int_t    i;

    if(pValues == NULL)
    {
        Error("setValue", "No values array");
        return;
    }

    if((i = getValueIndex(uiAlgIdx, uiPartIdx)) < 0)
        Error("setValue_Idx", "Out of bounds");
    else
        pValues[i] = fVal;
}

// -----------------------------------------------------------------------------

Float_t HPidCandidate::getValue(Int_t eAlg, Short_t nPartId) const
{
// Returns value for the algorithm eAlg for the particle nPartId
// or 0.0 if algorithm or particle not in the arrays

Float_t *pValues = getValues();
Int_t    iPart, iAlg;

    if(pValues == NULL)
    {
        Error("setValue", "No values array");
        return 0.0f;
    }

    if(((iAlg = getAlgorithmIndex(eAlg)) < 0)
            || ((iPart = getParticleIndex(nPartId)) < 0))
    {
        return 0.0f;
    }

    return pValues[getValueIndex(iAlg, iPart)];
}

// -----------------------------------------------------------------------------

Float_t HPidCandidate::getValue_Idx(UInt_t uiAlgIdx, UInt_t uiPartIdx) const
{
// Returns value for known position in the values array or 0.0 if out of bound

Float_t *pValues = getValues();
Int_t    i;

    if(pValues == NULL)
    {
        Error("setValue", "No values array");
        return 0.0f;
    }

    return ((i = getValueIndex(uiAlgIdx, uiPartIdx)) < 0) ? 0.0f : pValues[i];
}

// -----------------------------------------------------------------------------

Int_t* HPidCandidate::getAlgorithms(void) const
{
    Error("getAlgorithms", "Abstract method");
    return NULL;
}

// -----------------------------------------------------------------------------

Short_t* HPidCandidate::getPidObjIds(void) const
{
    Error("getPidObjIds", "Abstract method");
    return NULL;
}

// -----------------------------------------------------------------------------

Short_t* HPidCandidate::getParticles(void) const
{
    Error("getParticles", "Abstract method");
    return NULL;
}

// -----------------------------------------------------------------------------

Float_t* HPidCandidate::getValues(void) const
{
    Error("getValues", "Abstract method");
    return NULL;
}

// -----------------------------------------------------------------------------

HCategory* HPidCandidate::buildPidCandidateCategory(UInt_t iAlg, UInt_t iPart)
{
// Static function for making the category for the specified number
// of algorithms and particles

char s[100];

    sprintf(s, "HPidCandidate_%d_%d", iAlg, iPart);

    return buildPidCandidateCategory(s);
}

// -----------------------------------------------------------------------------

HCategory* HPidCandidate::buildPidCandidateCategory(const char sClassName[])
{
// Static function for making the category for the specified class name.

HCategory  *pCat;
HEvent     *pEvent;

    if((gHades == NULL) || ((pEvent = gHades->getCurrentEvent()) == NULL))
    {
        ::Error("buildPidCandidateCategory", "Cannot access current event");
        return NULL;
    }

    if((pCat = pEvent->getCategory(catPidCandidate)) != NULL)
    {
        if(strcmp(pCat->getClassName(), sClassName) != 0)
        {
            ::Error("buildPidCandidateCategory", "Category with id %d "
                    "already allocated for class %s not %s",
                    catPidCandidate, pCat->getClassName(), sClassName);

            return NULL;
        }

        return pCat;
    }

    if(gROOT->GetClass(sClassName) == NULL)
    {
        ::Error("buildPidCandidateCategory", "Class %s is not defined\n"
                "See documentation for class HPidCandidate", sClassName);

        return NULL;
    }

    if((pCat = new HLinearCategory((Text_t *) sClassName, 1000)) == NULL)
    {
        ::Error("buildPidCandidateCategory", "Cannot create new category");
        return NULL;
    }

    pEvent->addCategory(catPidCandidate, pCat, "Pid");

    return pCat;
}

// -----------------------------------------------------------------------------

HPidCandidate* HPidCandidate::getPidCandidate(Int_t iIdx, HCategory *pCat)
{
// Return HPidCandidate object corresponding to iIdx from pCat.
// If the pCat is not set (NULL) catPidCandidate category from gHades is used.

    if(iIdx < 0)
        return NULL;

    if((pCat == NULL) && ((pCat = HPidTrackCand::getCategory(
                            catPidCandidate, kFALSE)) == NULL))
    {
        ::Error("HPidCandidate::getPidCandidate",
                        "No catPidCandidate category");

        return NULL;
    }

    return (HPidCandidate *) pCat->getObject(iIdx);
}
