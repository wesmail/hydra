// @(#)$Id: hpidcandidate.cc,v 1.6 2003-06-05 14:40:59 jaskula Exp $
//*-- Author : Marcin Jaskula 27/07/2002
// Modified  : Marcin Jaskula 06/09/2002
// Modified  : Marcin Jaskula 16/09/2002
//           - now this is an abstract class
// Modified  : Marcin Jaskula 31/10/2002
//           - buildPidCandidateCategory moved from global to static function
// Modified  : Marcin Jaskula 01/12/2002
//           - new method getPidCandidate()
//           - getCandidate() rewritten
// Modified  : Marcin Jaskula 18/04/2003
//           - functions for calculation of the merged PDF and Bayes added

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

// -----------------------------------------------------------------------------

Int_t HPidCandidate::calcBayesVect(Float_t fOut[],
                                const Int_t aAlgs[], Int_t iAlgs) const
{
// Calculate Bayes values for the all particles and the algorithms
// with ids from array aAlgs of size iAlgs. If iAlgs==0 all available algorithms
// are used - in this case aAlgs may be NULL. The output values are stored
// in fOut array, which size should be suitable to contain all values.
// fOut must not be NULL. The function returns the number of filled elements
// in fOut array

    if(fOut == NULL)
    {
        Error("calcBayesVect", "fOut == NULL");
        return 0;
    }

Int_t iRelInt;

    if((iRelInt = getAlgorithmIndex(algRelInt)) < 0)
    {
        Error("calcBayesVect", "no algRelInt in the container");
        return 0;
    }

Float_t *fRelInt = getValuesVector(iRelInt);
Int_t    i;
Int_t    iMax = getNParticles();

    if((i = calcMergedVect(fOut, aAlgs, iAlgs)) <= 0)
        return i;

Float_t fSum = 0.0f;

    // calc products
    for(i = 0; i < iMax; i++)
    {
        if(fOut[i] < 0.0)
            continue;

        if(fRelInt[i] < 0.0)
            fRelInt[i] = 0.0;

        fOut[i] *= fRelInt[i];

        fSum += fOut[i];
    }

    // renormalize
    if((fSum > 0.0f) && (fSum != 1.0f))
    {
        fSum = 1.0f / fSum;

        for(i = 0; i < iMax; i++)
        {
            if(fOut[i] <= 0.0f)
                continue;

            fOut[i] *= fSum;
        }
    }

    return iMax;
}

// -----------------------------------------------------------------------------

#define FILL_ARRAY() \
    if(iAlg0 > 0) aAlg[i++] = iAlg0; \
    if(iAlg1 > 0) aAlg[i++] = iAlg1; \
    if(iAlg2 > 0) aAlg[i++] = iAlg2; \
    if(iAlg3 > 0) aAlg[i++] = iAlg3; \
    if(iAlg4 > 0) aAlg[i++] = iAlg4; \
    if(iAlg5 > 0) aAlg[i++] = iAlg5; \
    if(iAlg6 > 0) aAlg[i++] = iAlg6; \
    if(iAlg7 > 0) aAlg[i++] = iAlg7; \
    if(iAlg8 > 0) aAlg[i++] = iAlg8; \
    if(iAlg9 > 0) aAlg[i++] = iAlg9

// -----------------------------------------------------------------------------

Int_t HPidCandidate::calcBayes(Float_t fOut[], Int_t iAlg0,
                            Int_t iAlg1, Int_t iAlg2, Int_t iAlg3,
                            Int_t iAlg4, Int_t iAlg5, Int_t iAlg6,
                            Int_t iAlg7, Int_t iAlg8, Int_t iAlg9) const
{
// Calculate Bayes values for the all particles and the algorithms
// given as the arguments. If all iAlg? == 0 all available algorithms
// are used. For more info see the descr. of the previous method, which
// is called internally.

Int_t aAlg[10];
Int_t i = 0;

    FILL_ARRAY();

    return calcBayesVect(fOut, aAlg, i);
}

// -----------------------------------------------------------------------------

Float_t HPidCandidate::getBayesVect(Short_t nType,
                                const Int_t aAlgs[], Int_t iAlgs) const
{
// Return the Bayes value for the give particle species nType.
// For commends to the eAlgs and iAlgs see calcBayesVect descr.
// In case of any error -1 is returned

Int_t iIdx;

    if((iIdx = getParticleIndex(nType)) < 0)
    {
        Error("getBayesVect", "Particle %d not in the container", nType);
        return -1.0f;
    }

Float_t *fArr;

    if((fArr = new Float_t[getNParticles()]) == NULL)
    {
        Error("getBayesVect", "Cannot create temp array");
        return -1.0f;
    }

Float_t fVal;

    if(calcBayesVect(fArr, aAlgs, iAlgs) <= 0)
        fVal = -1.0f;
    else
        fVal = fArr[iIdx];

    delete [] fArr;

    return fVal;
}

// -----------------------------------------------------------------------------

Float_t HPidCandidate::getBayes(Short_t nType, Int_t iAlg0,
                            Int_t iAlg1, Int_t iAlg2, Int_t iAlg3,
                            Int_t iAlg4, Int_t iAlg5, Int_t iAlg6,
                            Int_t iAlg7, Int_t iAlg8, Int_t iAlg9) const
{
// Return the Bayes value for the give particle species nType and the given
// set of algorithms. For more info see descr. of getBayesVect and calcBayesVect
// methods.

Int_t aAlg[10];
Int_t i = 0;

    FILL_ARRAY();

    return getBayesVect(nType, aAlg, i);
}

// -----------------------------------------------------------------------------

Int_t HPidCandidate::calcMergedVect(Float_t fOut[],
                                const Int_t aAlgs[], Int_t iAlgs) const
{
// Calculate merged propability for the all particles and the algorithms
// with ids from array aAlgs of size iAlgs. If iAlgs==0 all available algorithms
// are used - in this case aAlgs may be NULL. The output values are stored
// in fOut array, which size should be suitable to contain all values.
// fOut must not be NULL. The function returns the number of filled elements
// in fOut array

    if(fOut == NULL)
    {
        Error("calcMergedVect", "fOut == NULL");
        return 0;
    }

UInt_t   iMax = getNParticles();
UInt_t   i;
Int_t    a, c, k;
Float_t *af;

    // clear the output
    for(i = 0; i < iMax; i++)
        fOut[i] = -1.0f;

    if(iAlgs <= 0)
    {
        c = 0;

        // loop over all algorithms and choose the PDF vectors
        for(k = 0; k < (Int_t)getNAlgorithms(); k++)
        {
            if(((a = getAlgorithm(k)) <= LAST_COMMON_ALG)
                    || (a >= CL_ALOGRITHM_OFFSET))
            {
                continue;
            }

            // calc the products
            af = getValuesVector(k);

            for(i = 0; i < iMax; i++)
            {
                if(af[i] >= 0.0f)
                {
                    if(fOut[i] < 0.0f)
                        fOut[i] = af[i];
                    else
                        fOut[i] *= af[i];
                }
            }

            c++;
        }

        if(c <= 0)
        {
            Error("calcMergedVect", "No PDF vector found");
            goto lab_Error;
        }
    }
    else
    {
        for(c = 0; c < iAlgs; c++)
        {
            if((k = getAlgorithmIndex(aAlgs[c])) < 0)
            {
                Error("calcMergedVect", "No values for algorithm %d", aAlgs[c]);
                goto lab_Error;
            }

            // calc the products
            af = getValuesVector(k);

            if(af[i] < 0.0f)
                continue;

            for(i = 0; i < iMax; i++)
            {
                if(fOut[i] < 0.0f)
                    fOut[i] = af[i];
                else
                    fOut[i] *= af[i];
            }
        }
    }

    return iMax;

lab_Error:
    for(i = 0; i < iMax; i++)
        fOut[i] = -1.0f;

    return 0;
}

// -----------------------------------------------------------------------------

Int_t HPidCandidate::calcMerged(Float_t fOut[], Int_t iAlg0,
                            Int_t iAlg1, Int_t iAlg2, Int_t iAlg3,
                            Int_t iAlg4, Int_t iAlg5, Int_t iAlg6,
                            Int_t iAlg7, Int_t iAlg8, Int_t iAlg9) const
{
// Calculate merged propability for the all particles and the algorithms
// given as the arguments. If all iAlg? == 0 all available algorithms
// are used. For more info see the descr. of the previous method, which
// is called internally.

Int_t aAlg[10];
Int_t i = 0;

    FILL_ARRAY();

    return calcMergedVect(fOut, aAlg, i);
}

// -----------------------------------------------------------------------------

Float_t HPidCandidate::getMergedVect(Short_t nType,
                                const Int_t aAlgs[], Int_t iAlgs) const
{
// Return the merged propability value for the give particle species nType.
// For commends to the eAlgs and iAlgs see calcMergedVect descr.
// In case of any error -1 is returned

Int_t iIdx;

    if((iIdx = getParticleIndex(nType)) < 0)
    {
        Error("getMergedVect", "Particle %d not in the container", nType);
        return -1.0f;
    }

Float_t *fArr;

    if((fArr = new Float_t[getNParticles()]) == NULL)
    {
        Error("getMergedVect", "Cannot create temp array");
        return -1.0f;
    }

Float_t fVal;

    if(calcMergedVect(fArr, aAlgs, iAlgs) <= 0)
        fVal = -1.0f;
    else
        fVal = fArr[iIdx];

    delete [] fArr;

    return fVal;
}

// -----------------------------------------------------------------------------

Float_t HPidCandidate::getMerged(Short_t nType, Int_t iAlg0,
                            Int_t iAlg1, Int_t iAlg2, Int_t iAlg3,
                            Int_t iAlg4, Int_t iAlg5, Int_t iAlg6,
                            Int_t iAlg7, Int_t iAlg8, Int_t iAlg9) const
{
// Return the merged propability value for the give particle species nType.
// and the set of algorithms. For more info see descr. of getMergedVect
// and calcMergedVect methods.

Int_t aAlg[10];
Int_t i = 0;

    FILL_ARRAY();

    return getMergedVect(nType, aAlg, i);
}
