// @(#)$Id: hpidparticlefiller.cc,v 1.13 2003-06-05 14:41:05 jaskula Exp $
//*-- Author : Marcin Jaskula 30/10/2002
//  Modified : Marcin Jaskula 25/02/2003
//             filling artificial particle when no particle from the track
//             created.
//  Modified : Marcin Jaskula 02/03/2003
//             get rid of HPidParticleFillerSim
//  Modified : Marcin Jaskula 23/05/2003
//             new organization of HPidCandidate

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// HPidParticleFiller                                                         //
//                                                                            //
// Filler of HPidParticle category from the data in HPidCandidate.            //
// The class checks if the HPidCandidate fulfills the condition, which may    //
// be definied in two ways:                                                   //
//                                                                            //
// - the user sets their own function Long_t(*p)(HPidCandidate *, Int_t,      //
//                                              Float_t *pfTestVal)           //
//   where the 2nd arg. is the index of the particle in HPidCandidate         //
//   and by the 3rd arg. the function returns value for HPidParticle::fTestVal//
//                                                                            //
//   It can be set by two mehods:                                             //
//   - setCheckFunCmp(Long_t(*p)(const HPidCandidate *, Int_t, Float_t *pfTV))//
//     for compiled code                                                      //
//   - setCheckFun(void *pFun) - for CINT (in macros)                         //
//                                                                            //
// - an simple condition is checked for given parameters setCondition()       //
//     - Algorithm - id of the algorithm                                      //
//     - Threshold - threshold value                                          //
//     - Operator  - one of { ==, <=, <, >, >= } defining the condition:      //
//                   Val([Algorithm]) [Operator] [Threshold]                  //
//     - CheckMax  - if set then additional check is made if the value for    //
//                   the particle is the greatest/lowerest from the whole set.//
//     - InclFakes - if fakes should be included into the condition checking  //
//                                                                            //
//     some wrappers are allowed: setPdfCondition() and setCLCondition()      //
//                                                                            //
// If special Algorithm is selected (0 - Bayes, 1 - merged PDFs) for the      //
// calculations of these values the reconstructor uses algorithms selected    //
// by addAlgorithm()/removeAlgorithm() methods. If no algorithm is chosen     //
// the reconstructor uses all PDFs available in the input HPidCandidate.      //
//                                                                            //
// The output is filled from the data taken from HPidCandidate and HKickTrack.//
//                                                                            //
// Static method of the reconstructor checkParticlePar() can be used          //
// for the condition checking in the user function.                           //
//                                                                            //
// For the TLorentzVector in HPidParticle an experimental or ideal mass       //
// may be taken into the accout. MASSEXP in parameters given in the           //
// constructors on in setParameters() method turn of exp. mass.               //
//                                                                            //
// The class name of the output category is defined by the reconstructor's    //
// constructor parameter. The class must be or extend HPidParticle class.     //
// If the output class is or extends HPidParticleSim the simulation parameters//
// are filled as well. Then bFillKine and iProcessMask parameters are used.   //
//                                                                            //
// If the output category extends HPidHitData the parameters of this class    //
// are filled as well.                                                        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include "hcategory.h"
#include "hiterator.h"

#include "hpidparticlefiller.h"

#include "hpidcandidate.h"
#include "hpidtrackcand.h"
#include "hkicktrack.h"

#include "hades.h"
#include "hevent.h"

#include "hlinearcategory.h"
#include "kickdef.h"
#include "piddef.h"

#include "hpidphysicsconstants.h"
#include "hpidtrackfiller.h"

#include <TMethodCall.h>
#include <Api.h>
#include <TROOT.h>
#include <TArrayF.h>

#include "hpidparticlesim.h"
#include "hpidhitdata.h"
#include "hpidgeanttrackset.h"

// -----------------------------------------------------------------------------

ClassImp(HPidParticleFiller)

// -----------------------------------------------------------------------------

const Char_t* HPidPartFiller__asOperators[] =
    { "==", "<", "<=", ">", ">=" };

HPidParticleFiller *HPidParticleFiller::pCurPartFiller = NULL;

// -----------------------------------------------------------------------------

#define SAFE_DELETE(A) { if(A) { delete (A); A = NULL; }}

// -----------------------------------------------------------------------------

HPidParticleFiller::HPidParticleFiller(Option_t par[], Text_t cl[])
                    : HReconstructor("PidPartFiller",
                                     "Filler of HPidParticle category")
{
// Default constructor.
// par parameter is used in setParameters() method

    setDefault();
    setParameters(par);

    sOutClass = cl;

    pCurPartFiller = this;
}

// -----------------------------------------------------------------------------

HPidParticleFiller::HPidParticleFiller(Text_t name[], Text_t title[],
                                        Option_t par[], Text_t cl[])
                    : HReconstructor(name, title)
{
// Constructor with names
// par parameter is used in setParameters() method

    setDefault();
    setParameters(par);

    sOutClass = cl;

    pCurPartFiller = this;
}

// -----------------------------------------------------------------------------

HPidParticleFiller::~HPidParticleFiller(void)
{
// Destructor

    SAFE_DELETE(pitInput);

    if(pCurPartFiller == this)
        pCurPartFiller = NULL;
}

// -----------------------------------------------------------------------------

void HPidParticleFiller::setDefault(void)
{
// Called in constructors to set default valuest to the members

    pInputCat = NULL;
    pitInput  = NULL;
    pOutCat   = NULL;

    pTrackCandCat = NULL;
    pKickTrackCat = NULL;

    pCheckFun     = NULL;
    pMethodCall   = NULL;

    iAlgorithm    = -1;
    aAlgorithms.Set(0);
    fThreshold    = 0.0f;
    eOperator     = kEqual;
    bCheckMax     = kFALSE;
    bIncludeFakes = kTRUE;
    bUseMassExp   = kFALSE;

    iDebug = 0;

    bMakeArtPart = kFALSE;

    pOutClass = NULL;
    bFillKine = kTRUE;
    iProcessMask = 0;

    bIsOutSim = kFALSE;
    bIsOutHit = kFALSE;
}

// -----------------------------------------------------------------------------

Bool_t HPidParticleFiller::init(void)
{
// Build all categories

    if(iDebug)
        Warning("init", "begin");

    // output category
    if(buildOutputCategory() == kFALSE)
        return kFALSE;

    // HPidTrackCand category
    if((pTrackCandCat = HPidTrackCand::buildPidTrackCategory()) == NULL)
    {
        Error("init", "Cannot build HPidTrackCand category");
        return kFALSE;
    }

    // HPidCandidate category and iterator
    if((pInputCat = gHades->getCurrentEvent()->getCategory(catPidCandidate))
                == NULL)
    {
        Error("init", "No category catPidCandidate");
        return kFALSE;
    }

    if((pitInput = (HIterator *)pInputCat->MakeIterator()) == NULL)
    {
        Error("init", "Cannot make an iterator for HPidCandidate category");

        return kFALSE;
    }

    // HKickTrack category
    if(pKickTrackCat == NULL)
    {
        if((pKickTrackCat = gHades->getCurrentEvent()
                                ->getCategory(catKickTrack)) == NULL)
        {
            Error("init", "No category catKickTrack");
            return kFALSE;
        }
    }

    if(iDebug)
        Warning("init", "end");


    return kTRUE;
}

// -----------------------------------------------------------------------------

Bool_t HPidParticleFiller::reinit(void)
{
// Check the reconstructor parameters

    if(pOutClass == NULL)
    {
        Error("reinit", "No output class: \"%s\"", sOutClass.Data());
        return kFALSE;
    }

    if((pCheckFun == NULL) && (iAlgorithm < 0))
    {
        Error("reinit", "No condition defined");
        return kFALSE;
    }

    return kTRUE;
}

// -----------------------------------------------------------------------------

Bool_t HPidParticleFiller::buildOutputCategory(void)
{
// Check the output class and build output category for it.

    if((pOutClass = gROOT->GetClass(sOutClass)) == NULL)
    {
        Error("buildOutputCategory", "Class \"%s\" not defined",
                sOutClass.Data());

        return kFALSE;
    }

    if( ! pOutClass->InheritsFrom(HPidParticle::Class()))
    {
        Error("buildOutputCategory",
                "Class \"%s\" doesn't inherits from HPidParticle",
                sOutClass.Data());

        pOutClass = NULL;

        return kFALSE;
    }

    // check the output
    bIsOutSim = pOutClass->InheritsFrom(HPidParticleSim::Class());

HPidParticle *pTest;

    if((pTest = (HPidParticle *) pOutClass->New()) == NULL)
    {
        Error("buildOutputCategory", "Cannot create object of class: \"%s\"",
                sOutClass.Data());

        pOutClass = NULL;

        return kFALSE;
    }

    bIsOutHit = (pTest->getHPidHitData() != NULL);

    delete pTest;

HEvent *pEvent;

    if((gHades == NULL) || ((pEvent = gHades->getCurrentEvent()) == NULL))
    {
        Error("buildOutputCategory", "Cannot access current event");
        pOutClass = NULL;

        return kFALSE;
    }

    if((pOutCat = pEvent->getCategory(catPidPart)) != NULL)
    {
        if(strcmp(pOutCat->getClassName(), sOutClass.Data()) != 0)
        {
            Error("buildOutputCategory", "Category with id %d "
                        "already allocated for class %s not %s",
                        catPidPart, pOutCat->getClassName(), sOutClass.Data());

            pOutClass = NULL;

            return kFALSE;
        }
    }
    else
    {
        if((pOutCat = new HLinearCategory((Text_t *) sOutClass.Data(), 1000))
                == NULL)
        {
            Error("buildOutputCategory", "Cannot create new category");
            pOutClass = NULL;

            return kFALSE;
        }

        pEvent->addCategory(catPidPart, pOutCat, "Pid");
    }

    return kTRUE;
}

// -----------------------------------------------------------------------------

void HPidParticleFiller::setParameters(Option_t par[])
{
// Set parameters by names. Options (may be separated by comma or blank chars):
// DEBUGX  - debug info, if X set then it define the debug info level
// MASSEXP - use experimental mass to fill TLorentzVector
// MAKEART - create artificial particle when no "real" particle from
//           HPidCandidate may be created
// NOKINE  - do not fill kine data in HPidParticleSim

TString s = par;
char   *p;

    s.ToUpper();

    iDebug = 0;
    if((p = strstr(s.Data(), "DEBUG")) != NULL)
    {
        if(sscanf(p + strlen("DEBUG"), "%d", &iDebug) <= 0)
            iDebug = 1;
    }

    bUseMassExp  = (strstr(s.Data(), "MASSEXP") != NULL);
    bMakeArtPart = (strstr(s.Data(), "MAKEART") != NULL);
    bFillKine    = (strstr(s.Data(), "NOKINE") == NULL);
}

// -----------------------------------------------------------------------------

void HPidParticleFiller::addAlgorithm(Int_t iAId)
{
    aAlgorithms.Set(aAlgorithms.GetSize() + 1);
    aAlgorithms[aAlgorithms.GetSize() - 1] = iAId;
}

// -----------------------------------------------------------------------------

void HPidParticleFiller::removeAlgorithm(Int_t iAId)
{
Int_t iIn, iOut;

    for(iIn = 0, iOut = 0; iIn < aAlgorithms.GetSize(); iIn++)
    {
        if(aAlgorithms[iIn] != iAId)
        {
            if(iOut != iIn)
                aAlgorithms[iOut] = aAlgorithms[iIn];

            iOut++;
        }
    }

    aAlgorithms.Set(iOut);
}

// -----------------------------------------------------------------------------

Int_t HPidParticleFiller::execute(void)
{
// Make the particle identification

    if(pOutClass == NULL)
    {
        Error("execute", "No output class");
        return -3;
    }

    if((pCheckFun == NULL) && (iAlgorithm < 0))
    {
        Error("execute", "No condition defined");
        return -2;
    }

HPidCandidate *pCand;
Int_t          i;
Int_t          iSum = 0;

    // clear the data from the old file
    pOutCat->Clear();

    pitInput->Reset();
    while((pCand = (HPidCandidate *)pitInput->Next()) != NULL)
    {
        i = checkCandidate(pCand);

        if((i <= 0) && (bMakeArtPart))
            makeArtificialParticle(pCand);

        iSum += i;

        if(iDebug)
            printf("%d ", i);
    }

    if(iDebug)
        printf("(%d)\n", iSum);

    return 0;
}

// -----------------------------------------------------------------------------

HPidParticle* HPidParticleFiller::getNextSlot(void)
{
// Get next slot from the output category

HPidParticle  *pOut = NULL;
static HLocation  locDummy;

    if((pOutCat == NULL) || (pOutClass == NULL))
    {
        Error("getNextSlot", "Output category not set: use init/reinit");
        return NULL;
    }

    if((pOut = (HPidParticle *) pOutCat->getNewSlot(locDummy)) == NULL)
    {
        Error("getNextSlot", "No new slot");
        return NULL;
    }

    pOut = (HPidParticle *) pOutClass->New(pOut);

    return pOut;
}

// -----------------------------------------------------------------------------

Int_t HPidParticleFiller::checkCandidate(const HPidCandidate *pCand)
{
// Check if pCand has a good quality and fills the HPidParticle for it.
// The HKickTrack correlated with the HPidCandidate must be available.
// The method returns number of HPidParticle created for the pCand.

Int_t          iReturn = 0;
HPidTrackCand *pTrackCand;
HKickTrack    *pKickTrack;
UInt_t         iPart;
Int_t          i;
Float_t        fTestVal;

    // get the HPidTrackCand
    if(pCand->getCandidateId() < 0)
        return 0;

    if((pTrackCand = pCand->getCandidate(pTrackCandCat)) == NULL)
    {
        Error("checkCandidate", "Cannot get HPidTrackCand");
        return 0;
    }

    // get HKickTrack
    if(pTrackCand->getKickTrackId() < 0)
        return 0;

    if((pKickTrack = pTrackCand->getKickTrack(pKickTrackCat)) == NULL)
    {
        Error("checkCandidate", "Cannot get HKickTrack");
        return 0;
    }

    // for the user function (condition)
    if(pCheckFun != NULL)
    {
        for(iPart = 0; iPart < pCand->getNParticles(); iPart++)
        {
            if(pCheckFun(pCand, iPart, &fTestVal))
            {
                if(makeParticle(pCand, pKickTrack, iPart, fTestVal) != NULL)
                    iReturn++;
            }
        }

        return iReturn;
    }

    // "normal" condition

    // prepare the output array
    if(aCandValues.GetSize() < (Int_t) pCand->getNParticles())
        aCandValues.Set(pCand->getNParticles());

    // copy the propper vector of the parameters
    switch(iAlgorithm)
    {
        case algMerged:
            pCand->calcMergedVect(aCandValues.GetArray(),
                        aAlgorithms.GetArray(), aAlgorithms.GetSize());
            break;

        case algBayes:
            pCand->calcBayesVect(aCandValues.GetArray(),
                        aAlgorithms.GetArray(), aAlgorithms.GetSize());
            break;

        default:
            if((i = pCand->getAlgorithmIndex(iAlgorithm)) < 0)
            {
                ::Error("HPidParticleFiller::checkCandidate",
                            "No algorithm %d in the HPidCandidate", iAlgorithm);

                return kFALSE;
            }

            aCandValues.Set(pCand->getNParticles(), pCand->getValuesVector(i));

            break;
    }

    // when (bCheckMax && ! bIncludeFakes) set the values for fakes to -1
    // (artif. value)
    if((bCheckMax) && ( ! bIncludeFakes))
    {
        for(i = 0; i < (Int_t) pCand->getNParticles(); i++)
        {
            if(HPidPhysicsConstants::isFake(pCand->getParticle(i)))
                aCandValues[i] = -1.0f;
        }
    }

    // find next propper particle for the output
    for(iPart = 0; iPart < pCand->getNParticles(); iPart++)
    {
        if(checkParticlePar(aCandValues.GetArray(), aCandValues.GetSize(),
                    iPart, fThreshold, eOperator, bCheckMax))
        {
            if(makeParticle(pCand, pKickTrack, iPart, aCandValues[iPart]))
                iReturn++;
        }
    }

    return iReturn;
}

// -----------------------------------------------------------------------------

Bool_t HPidParticleFiller::checkParticlePar(const HPidCandidate *pCand,
                                Int_t iIdx,
                                Int_t iAlg, Float_t fThr, const Char_t *pOper,
                                Bool_t bMax, Bool_t bFakesIncl)
{
// Static function for checking the candidate for a given particle

Int_t i;

    if(pCand == NULL)
    {
        ::Error("HPidParticleFiller::checkParticlePar", "pCand == NULL");
        return kFALSE;
    }

    if((i = findOperator(pOper)) < 0)
    {
        ::Error("HPidParticleFiller::checkParticlePar",
                    "Unknown operator `%s'", pOper);

        return kFALSE;
    }

    return checkParticlePar(pCand, iIdx, iAlg, fThr,
                        (EnumOperators_t) i, bMax, bFakesIncl);
}

// -----------------------------------------------------------------------------

Bool_t HPidParticleFiller::checkParticlePar(const HPidCandidate *pCand,
                        Int_t iIdx,
                        Int_t iAlg, Float_t fThr, EnumOperators_t eOper,
                        Bool_t bMax, Bool_t bFakesIncl)
{
// Check if the particle in position iIdx fulfill the condition.
// The value set by algorithm must NOT be negative.

    // skip fakes when they are not taken into the checking max values
    if((bMax) && ( ! bFakesIncl)
                && (HPidPhysicsConstants::isFake(pCand->getParticle(iIdx))))
    {
        return kFALSE;
    }

    if(iAlg < 0)
    {
    ::Error("HPidParticleFiller::checkParticle",
        "No propper algorithm set");

        return kFALSE;
    }

static TArrayF  aVars;
Int_t           i;

    // check the output array size
    if(aVars.GetSize() < (Int_t) pCand->getNParticles())
    {
        aVars.Set(pCand->getNParticles());
    }

    // copy the propper vector of the parameters
    switch(iAlg)
    {
        case algMerged:
            pCand->calcMergedVect(aVars.GetArray(), NULL, 0);
            break;

        case algBayes:
            pCand->calcBayesVect(aVars.GetArray(), NULL, 0);
            break;

        default:
            if((i = pCand->getAlgorithmIndex(iAlg)) < 0)
            {
                ::Error("HPidParticleFiller::checkParticle",
                            "No algorithm %d in the HPidCandidate", iAlg);

                return kFALSE;
            }

            aVars.Set(pCand->getNParticles(), pCand->getValuesVector(i));

            break;
    }

    // when (bMax && ! bFakesIncl) set the values for fakes to -1 (artif. value)
    if((bMax) && ( ! bFakesIncl))
    {
        for(i = 0; i < (Int_t) pCand->getNParticles(); i++)
        {
            if((i != iIdx)
                    && (HPidPhysicsConstants::isFake(pCand->getParticle(i))))
            {
                aVars[i] = -1.0f;
            }
        }

    }

    return checkParticlePar(aVars.GetArray(), aVars.GetSize(),
                            iIdx, fThr, eOper, bMax);

}

// -----------------------------------------------------------------------------

Bool_t HPidParticleFiller::checkParticlePar(const Float_t *aVals, Int_t iSize,
                                        Int_t iIdx, Float_t fThr,
                                        const Char_t *pOper, Bool_t bMax)
{
// Static function for checking the candidate for a given vector of the values

Int_t i;

    if((i = findOperator(pOper)) < 0)
    {
        ::Error("HPidParticleFiller::checkParticlePar",
                    "Unknown operator `%s'", pOper);

        return kFALSE;
    }

    return checkParticlePar(aVals, iSize, iIdx, fThr,
                        (EnumOperators_t) i, bMax);
}


// -----------------------------------------------------------------------------

Bool_t HPidParticleFiller::checkParticlePar(const Float_t *aVals, Int_t iSize,
                                        Int_t iIdx, Float_t fThr,
                                        EnumOperators_t eOper, Bool_t bMax)
{
    if(aVals == NULL)
    {
        ::Error("HPidParticleFiller::checkParticlePar", "aVals == NULL");
        return kFALSE;
    }

    if((iIdx < 0) || (iIdx >= iSize))
    {
        ::Error("HPidParticleFiller::checkParticlePar",
                "Wrong index %d [0, %d)", iIdx, iSize);
        return kFALSE;
    }

Float_t fVal    = aVals[iIdx];
Bool_t  bReturn = kFALSE;

    // negative means - some error or algortihm cannot determine
    if(fVal < 0.0f)
        return kFALSE;

    switch(eOper)
    {
        case kEqual:
            bReturn = (fVal == fThr);
            break;

        case kLower:
            bReturn = (fVal < fThr);
            break;

        case kLowerEqual:
            bReturn = (fVal <= fThr);
            break;

        case kGreater:
            bReturn = (fVal > fThr);
            break;

        case kGreaterEqual:
            bReturn = (fVal >= fThr);
            break;

        default:
            ::Error("HPidParticleFiller::checkParticle", "Wrong operator type");
            return kFALSE;
    }

    if((bReturn == kFALSE) || (bMax == kFALSE))
        return bReturn;

Float_t f;
Int_t   i;
Bool_t  bGr;

    switch(eOper)
    {
        case kEqual:
        case kGreater:
        case kGreaterEqual:
            bGr = kTRUE;
            break;

        default:
            bGr = kFALSE;
            break;
    }

    for(i = 0; i < iSize; i++)
    {
        if((i == iIdx) || ((f = aVals[i]) < 0.0f))
            continue;

        if((bGr) ? (f > fVal) : (f < fVal))
            return kFALSE;
    }

    return kTRUE;
}

// -----------------------------------------------------------------------------

HPidParticle* HPidParticleFiller::makeArtificialParticle(
                    const HPidCandidate *pCand)
{
// Create artificial particle when no "real" particle from HPidCandidate
// may be created.

HPidTrackCand *pTrackCand;
HKickTrack    *pKickTrack;

    // get the HPidTrackCand
    if(pCand->getCandidateId() < 0)
        return NULL;

    if((pTrackCand = pCand->getCandidate(pTrackCandCat)) == NULL)
    {
        Error("makeArtificialParticle", "Cannot get HPidTrackCand");
        return 0;
    }

    if(pTrackCand->getKickTrackId() < 0)
        return NULL;

    // get HKickTrack
    if((pKickTrack = pTrackCand->getKickTrack(pKickTrackCat)) == NULL)
    {
        Error("makeArtificialParticle", "Cannot get HKickTrack");
        return 0;
    }

    return makeParticle(pCand, pKickTrack, -1);
}

// -----------------------------------------------------------------------------

HPidParticle* HPidParticleFiller::makeParticle(const HPidCandidate *pCand,
                        HKickTrack *pKickTrack, Int_t iIdx, Float_t fTestValue)
{
// Make a new HPidParticle from the input data and returns pointer on it.
// The momentum, betaExp, angles, system, Z and R are taken from the HKickTrack
// Mass is the ideal one which comes from the found id if bUseMassExp is kFALSE.
// Otherwise the calculated experimental mass is used. In this case if the mass
// may have negative value (in case when beta>1). When beta<0 the mass
// cannot be calculated (nonphysical beta) and the HPidParticle is not created!
// PDF value is taken from algMerged algorithm, while Inten from
// algWithIntensites. The CL is set from (algMerged + CL_ALOGRITHM_OFFSET)
// algorithm.

Float_t fMassExp;
Float_t fBeta = pKickTrack->getBeta();

    if(fBeta > 0.0f)
    {
        if((fMassExp = 1.0f / (fBeta * fBeta) - 1.0f) < 0.0f)
            fMassExp = - TMath::Sqrt(-fMassExp);
        else
            fMassExp = TMath::Sqrt(fMassExp);

        fMassExp *= pKickTrack->getP();
    }
    else
    {
        fMassExp = HPidParticle::kMassExpNoPhy;

        if(bUseMassExp)
            return NULL;
    }

HPidParticle* pParticle;

    if((pParticle = getNextSlot()) == NULL)
        return NULL;

    pParticle->setPidCandidateId(pCand->getCandidateId());

    // kinematic vector
Double_t dM;
Short_t  nPid;

    if(iIdx >= 0)
        nPid = pCand->getParticle(iIdx);
    else
    {
        nPid = (pKickTrack->getCharge() < 0)
                ? HPidPhysicsConstants::artificialNeg()
                : HPidPhysicsConstants::artificialPos(); // artificial particles
    }

Double_t dP     = pKickTrack->getP();
Double_t dTheta = pKickTrack->getTheta();
Double_t dPhi   = TMath::DegToRad()
                    * HPidTrackFiller::getMdcPhi(
                                pKickTrack->getSector(), pKickTrack->getPhi());
Double_t dSin   = TMath::Sin(dTheta);

    if(bUseMassExp)
    {
        dM = fMassExp;
        pParticle->setMassExp(HPidParticle::kMassExpInTLV);
    }
    else
    {
        pParticle->setMassExp(fMassExp);

        if((dM = HPidPhysicsConstants::mass(nPid)) < 0.0)
            dM = 0.0;
    }

    pParticle->SetXYZM(dP * dSin * TMath::Cos(dPhi),
                        dP * dSin * TMath::Sin(dPhi),
                        dP * TMath::Cos(dTheta),
                        dM);

    // PID parameters
    pParticle->setPidParams(nPid, fTestValue);

    // experimental parameters
    pParticle->setExpParams(pKickTrack->getSystem(), pKickTrack->getBeta(),
                pKickTrack->getR(), pKickTrack->getZ(), pKickTrack->getPull());

    // filling additional data

HPidTrackCand *pTrackCand = NULL;

    if((bIsOutSim == kTRUE) || (bIsOutHit == kTRUE))
    {
        if((pTrackCand = pCand->getCandidate()) == NULL)
            Error("makeParticle", "Cannot get HPidTrackCand");
        else
        {
            if(bIsOutSim == kTRUE)
                fillHPidParticleSim((HPidParticleSim *) pParticle, pTrackCand);

            if(bIsOutHit == kTRUE)
            {
            HPidHitData *pHD ;

                if((pHD = pParticle->getHPidHitData()) == NULL)
                {
                    Error("makeParticle",
                            "Cannot cast HPidParticle->HPidHitData");
                }
                else
                    pHD->fill(pTrackCand);
            }
        }
    }

    return pParticle;
}

// -----------------------------------------------------------------------------

Bool_t HPidParticleFiller::fillHPidParticleSim(HPidParticleSim *pSim,
                                    HPidTrackCand *pTrack) const
{
// Fill HPidParticleSim params from the track (HPidGeantTrackSet
// and common tracks).  bFillKine and iProcessMask are used.

const HPidGeantTrackSet *pSet;

    if((pSet = pTrack->getGeantTrackSet()) == NULL)
    {
        Error("fillHPidParticleSim", "Cannot get HPidGeantTrackSet");
        return kFALSE;
    }

    pSim->setGeantTrackSet(pSet);
    pSet->fillCommonTracks(pSim, bFillKine, iProcessMask);

    return kTRUE;
}

// -----------------------------------------------------------------------------

Bool_t HPidParticleFiller::setOperator(const Char_t *pOper)
{
// Sets the operator by the name
// The method returns kTRUE if the pOper was defined in a proper way

Int_t i;

    if((i = findOperator(pOper)) < 0)
    {
        Error("setOperator", "Wrong operator: \"%s\"", pOper);
        return kFALSE;
    }

    eOperator = (EnumOperators_t) i;

    return kTRUE;
}

// -----------------------------------------------------------------------------

Int_t HPidParticleFiller::findOperator(const Char_t *pOper)
{
// Find the operator by the name
// The method returns (Int_t)eOperaror if succeeded or -1 when the operator
// is not known

UInt_t i;

    for(i = 0;
        i < sizeof(HPidPartFiller__asOperators) / sizeof(const Char_t *);
        i++)
    {
        if(strcmp(pOper, HPidPartFiller__asOperators[i]) == 0)
            return i;
    }

    return -1;
}

// -----------------------------------------------------------------------------

const Char_t* HPidParticleFiller::getOperator(void) const
{
// Return the name of the operator

    return HPidPartFiller__asOperators[eOperator];
}

// -----------------------------------------------------------------------------

Bool_t HPidParticleFiller::setCondition(Int_t iAlg, Float_t fThr,
                        const Char_t *pOper, Bool_t bMax, Bool_t bInclFakes)
{
// Define condition.
// pOper - must be one of: { == < <= > >= }
// The method returns kTRUE if the pOper was defined in a proper way
// If the bMax flag == kTRUE then additional checking is done:
// for pOper == { ==, >, >= } the value must be maximum from the set
// while for the rest of the operators minimal (but not negative)
// If bInclFakes = kFALSE the values for the fakes particles are not taken
// into comparision turned on by bMax flag.

    if(setOperator(pOper) == kFALSE)
        return kFALSE;

    return setCondition(iAlg, fThr, eOperator, bMax, bInclFakes);
}

// -----------------------------------------------------------------------------

Bool_t HPidParticleFiller::setCondition(Int_t iAlg, Float_t fThr,
                        EnumOperators_t eOper, Bool_t bMax, Bool_t bInclFakes)
{
// Define condition. For the description see the method above.
// In this implementation the it returns kTRUE.

    eOperator = eOper;

    if((iAlgorithm = iAlg) < 0)
        Warning("setCondition", "Negative algorithm turns off the condition !");

    fThreshold    = fThr;
    bCheckMax     = bMax;
    bIncludeFakes = bInclFakes;

    return kTRUE;
}

// -----------------------------------------------------------------------------

Long_t HPidParticleFiller::interactiveFun(
                    const HPidCandidate *pCand, Int_t iIdx, Float_t *pfTestVal)
{
// This function is called internally to wrap pointer to functions from CINT

    if(pCurPartFiller == NULL)
    {
        ::Error("interactiveFun", "No pCurPartFiller");
        return 0;
    }

    if(pCurPartFiller->pMethodCall == NULL)
    {
        ::Error("interactiveFun", "No pMethodCall");
        return 0;
    }

Long_t aArgs[3];
Long_t lResult = 0;

    aArgs[0] = (Long_t) pCand;
    aArgs[1] = (Long_t) iIdx;
    aArgs[2] = (Long_t) pfTestVal;

    pCurPartFiller->pMethodCall->SetParamPtrs(aArgs);
    pCurPartFiller->pMethodCall->Execute(lResult);

    return lResult;
}

// -----------------------------------------------------------------------------

void HPidParticleFiller::setCheckFun(void *pFun)
{
// This function is called by CINT instead of
// setCheckFun(Int_t(*p)(const HPidCandidate *, Int_t, Float_t *))
// The idea was copied from TMinuit

    if( ! pFun)
        return;

char *pFuncName = G__p2f2funcname(pFun);

    if(pFuncName)
    {
        pMethodCall = new TMethodCall();
        pMethodCall->InitWithPrototype(pFuncName,
                        "const HPidCandidate *,Int_t&,Float_t *");
    }
    else
    {
        Error("setCheckFun", "Unknown function");
        return;
    }

    pCheckFun = interactiveFun;
}

// -----------------------------------------------------------------------------

void HPidParticleFiller::print(void) const
{
// Print the list of all algorithms

    printf("Output class         : %s   ", sOutClass.Data());

    if(pOutClass == NULL)
        printf("NOT INITIALIZED YET\n");
    else
    {
        printf("%p\n", pOutClass);
        printf("Output isSim         : %s\n", (bIsOutSim) ? "YES" : "NO");
        printf("Output isHit         : %s\n", (bIsOutHit) ? "YES" : "NO");
    }

    printf("Checking function    : %p\n", pCheckFun);
    printf("Checking condition   : Val(alg = %d) %s %f%s   FAKES %s\n"
           "Mass used            : %s\n",
                    iAlgorithm, getOperator(), fThreshold,
                    (bCheckMax) ? "    ! MAX !" : "",
                    (bIncludeFakes) ? "ON" : "OFF",
                    (bUseMassExp) ? "EXP" : "IDEAL");

    printf("Algortithms list (%d) :", aAlgorithms.GetSize());
    for(Int_t i = 0; i < aAlgorithms.GetSize(); i++)
        printf(" %d", aAlgorithms[i]);

    printf("\n");

    printf("Fill kine            : %s\n", (bFillKine) ? "ON" : "OFF");
    printf("Process Mask         : 0x%08X\n", iProcessMask);
}
