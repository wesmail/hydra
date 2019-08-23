// @(#)$Id: hpidparticlefiller.cc,v 1.3 2002-11-04 23:21:50 jaskula Exp $
//*-- Author : Marcin Jaskula 30/10/2002

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// HPidParticleFiller                                                         //
//                                                                            //
// Filler of HPidParticle category from the data in HPidCandidate.            //
// The class checks if the HPidCandidate fulfills the condition, which may    //
// be definied in two ways:                                                   //
// - the user sets their own function Bool_t(*p)(HPidCandidate *, Int_t)      //
// - an simple condition is checked for given parameters                      //
//     - Algorithm - id of the algorithm                                      //
//     - Threshold - threshold value                                          //
//     - Operator  - one of { ==, <=, <, >, >= } defining the condition:      //
//                   Val([Algorithm]) [Operator] [Threshold]                  //
//     - CheckMax  - if set then additional check is made if the value for    //
//                   the particle is the greatest/lowerest from the whole set.//
//                                                                            //
// The output is filled from the data taken from HPidCandidate and HKickTrack.//
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include "hcategory.h"
#include "hiterator.h"

#include "hpidparticlefiller.h"

#include "hpidcandidate.h"
#include "hpidtrackcand.h"
#include "hkicktrack.h"
#include "hpidparticle.h"

#include "hades.h"
#include "hevent.h"

#include "hlinearcategory.h"
#include "kickdef.h"
#include "piddef.h"

#include "hpidphysicsconstants.h"
#include "hpidtrackfiller.h"

// -----------------------------------------------------------------------------

ClassImp(HPidParticleFiller)

// -----------------------------------------------------------------------------

const Char_t* HPidPartFiller__asOperators[] =
    { "==", "<", "<=", ">", ">=" };

// -----------------------------------------------------------------------------

#define SAFE_DELETE(A) { if(A) { delete (A); A = NULL; }}

// -----------------------------------------------------------------------------

HPidParticleFiller::HPidParticleFiller(Option_t par[])
    : HReconstructor("PidPartFiller", "Filler of HPidParticle category")
{
// Default constructor.

    setDefault();
    setParameters(par);
}

// -----------------------------------------------------------------------------

HPidParticleFiller::HPidParticleFiller(Text_t name[], Text_t title[],
                                        Option_t par[])
    : HReconstructor(name, title)
{
// Constructor with names

    setDefault();
    setParameters(par);
}

// -----------------------------------------------------------------------------

HPidParticleFiller::~HPidParticleFiller(void)
{
// Destructor

    SAFE_DELETE(pitInput);
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

    pCheckFun = NULL;

    iAlgorithm    = -1;
    fThreshold    = 0.0f;
    eOperator     = kEqual;
    bCheckMax     = kFALSE;
    bIncludeFakes = kTRUE;

    iDebug = 0;
}

// -----------------------------------------------------------------------------

Bool_t HPidParticleFiller::init(void)
{
// Build all categories

    // output category
    if((pOutCat = HPidParticle::buildPidParticleCategory()) == NULL)
    {
        Error("init", "Cannot build HPidParticle category");
        return kFALSE;
    }

    // HPidTrackCand category
    if((pTrackCandCat = HPidTrackCand::buildPidTrackCategory()) == NULL)
    {
        Error("init", "Cannot build HPidTrackCand category");
        return kFALSE;
    }

    return kTRUE;
}

// -----------------------------------------------------------------------------

void HPidParticleFiller::setParameters(Option_t par[])
{
// Set parameters by names. Options (may be separated by comma or blank chars):
// DEBUGX - debug info, if X set then it define the debug info level

TString s = par;
char   *p;

    s.ToUpper();

    iDebug = 0;
    if((p = strstr(s.Data(), "DEBUG")) != NULL)
    {
        if(sscanf(p + strlen("DEBUG"), "%d", &iDebug) <= 0)
            iDebug = 1;
    }
}

// -----------------------------------------------------------------------------

Bool_t HPidParticleFiller::reinit(void)
{
// Empty method

    return kTRUE;
}

// -----------------------------------------------------------------------------

Bool_t HPidParticleFiller::finalize(void)
{
// Empty method

    return kTRUE;
}

// -----------------------------------------------------------------------------

Int_t HPidParticleFiller::execute(void)
{
// Make the particle identification

    if(checkInputCategories() == kFALSE)
        return -1;

    if((pCheckFun == NULL) && (iAlgorithm < 0))
    {
        Error("execute", "No condition defined");
        return -2;
    }

HPidCandidate *pCand;
Int_t          i;
Int_t          iSum = 0;

    pitInput->Reset();
    while((pCand = (HPidCandidate *)pitInput->Next()) != NULL)
    {
        i = checkCandidate(pCand);
        iSum += i;

        if(iDebug)
            printf("%d ", i);
    }

    if(iDebug)
        printf("(%d)\n", iSum);

    return 0;
}

// -----------------------------------------------------------------------------

Bool_t HPidParticleFiller::checkInputCategories(void)
{
// Builds input categories, which cannot be bulid in the init function

    // get the input category
    if(pInputCat == NULL)
    {
        if((pInputCat = gHades->getCurrentEvent()->getCategory(catPidCandidate))
                    == NULL)
        {
            Error("checkInputCategories", "No category catPidCandidate");
            return kFALSE;
        }

        if((pitInput = (HIterator *)pInputCat->MakeIterator()) == NULL)
        {
            Error("checkInputCategories",
                        "Cannot make an iterator for HPidCandidate category");

            return kFALSE;
        }
    }

    // HKickTrack category
    if(pKickTrackCat == NULL)
    {
        if((pKickTrackCat = gHades->getCurrentEvent()
                                ->getCategory(catKickTrack)) == NULL)
        {
            Error("checkInputCategories", "No category catKickTrack");
            return kFALSE;
        }
    }

    return kTRUE;
}

// -----------------------------------------------------------------------------

HPidParticle* HPidParticleFiller::getNextSlot(void)
{
// Get next slot from the output category

HPidParticle  *pOut = NULL;
static HLocation  locDummy;

    if(pOutCat == NULL)
    {
        Error("getNextSlot", "Output category not set: use init/reinit");
        return NULL;
    }

    if((pOut = (HPidParticle *) pOutCat->getNewSlot(locDummy)) == NULL)
    {
        Error("getNextSlot", "No new slot");
        return NULL;
    }

    pOut = new(pOut) HPidParticle();

    return pOut;
}

// -----------------------------------------------------------------------------

Int_t HPidParticleFiller::checkCandidate(HPidCandidate *pCand)
{
// Check if pCand has a good quality and fills the HPidParticle for it.
// The HKickTrack correlated with the HPidCandidate must be available.
// The method returns number of HPidParticle created for the pCand.

Int_t          iReturn = 0;
HPidTrackCand *pTrackCand;
HKickTrack    *pKickTrack;
UInt_t         iPart;

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

    // find next propper particle to the output
    for(iPart = 0; iPart < pCand->getNParticles(); iPart++)
    {
        if(checkParticle(pCand, iPart))
        {
            if(makeParticle(pCand, pKickTrack, iPart) == NULL)
                return iReturn;

            iReturn++;
        }
    }

    return iReturn;
}

// -----------------------------------------------------------------------------

Bool_t HPidParticleFiller::checkParticle(HPidCandidate *pCand, Int_t iIdx)
{
// Check if the particle in position iIdx fulfill the condition defined
// by the class parameters.
// If pCheckFun != NULL the function is called
// Otherwise checkParticlePar is called

    if(pCheckFun != NULL)
        return pCheckFun(pCand, iIdx);

    return checkParticlePar(pCand, iIdx, iAlgorithm, fThreshold,
                            eOperator, bCheckMax, bIncludeFakes);
}

// -----------------------------------------------------------------------------

Bool_t HPidParticleFiller::checkParticlePar(HPidCandidate *pCand, Int_t iIdx,
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

Bool_t HPidParticleFiller::checkParticlePar(HPidCandidate *pCand, Int_t iIdx,
                        Int_t iAlg, Float_t fThr, EnumOperators_t eOper,
                        Bool_t bMax, Bool_t bFakesIncl)
{
// Check if the particle in position iIdx fulfill the condition.

    // skip fakes when they are not taken into the checking max values
    if((bMax) && ( ! bFakesIncl)
                && (HPidPhysicsConstants::isFake(pCand->getParticle(iIdx))))
    {
        return kFALSE;
    }

    if(iAlg < 0)
    {
        ::Error("HPidParticleFiller::checkParticle", "
                    No propper algorithm set");

        return kFALSE;
    }

Int_t iAlgIdx;

    if((iAlgIdx = pCand->getAlgorithmIndex(iAlg)) < 0)
    {
        ::Error("HPidParticleFiller::checkParticle",
                    "No algorithm %d in the HPidCandidate", iAlg);

        return kFALSE;
    }

Float_t fVal    = pCand->getValue_Idx(iAlgIdx, iIdx);
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
UInt_t  i;
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

    for(i = 0; i < pCand->getNParticles(); i++)
    {
        if((f = pCand->getValue_Idx(iAlgIdx, i)) < 0.0f)
            continue;

        if(( ! bFakesIncl)
                && (HPidPhysicsConstants::isFake(pCand->getParticle(i))))
        {
            continue;
        }

        if((bGr) ? (f > fVal) : (f < fVal))
            return kFALSE;
    }

    return kTRUE;
}

// -----------------------------------------------------------------------------

HPidParticle* HPidParticleFiller::makeParticle(HPidCandidate *pCand,
                                HKickTrack *pKickTrack, Int_t iIdx)
{
// Make a new HPidParticle from the input data and returns pointer on it.
// The momentum, betaExp, angles, system, Z and R are taken from the HKickTrack
// Mass is the ideal one which comes from the found id
// PDF value is taken from algMerged algorithm, while Inten from
// algWithIntensites. The CL is set from (algMerged + CL_ALOGRITHM_OFFSET)
// algorithm.

HPidParticle* pParticle;

    if((pParticle = getNextSlot()) == NULL)
        return NULL;

    pParticle->setPidCandidateId(pCand->getCandidateId());

    // kinematic vector
Double_t dM;
Short_t  nPid   = pCand->getParticle(iIdx);
Double_t dP     = pKickTrack->getP();
Double_t dTheta = pKickTrack->getTheta();
Double_t dPhi   = TMath::DegToRad()
                    * HPidTrackFiller::getMdcPhi(
                                pKickTrack->getSector(), pKickTrack->getPhi());
Double_t dSin   = TMath::Sin(dTheta);

    if((dM = HPidPhysicsConstants::mass(nPid)) < 0.0)
        dM = 0.0;

    pParticle->SetXYZM(dP * dSin * TMath::Cos(dPhi),
                        dP * dSin * TMath::Sin(dPhi),
                        dP * TMath::Cos(dTheta),
                        dM);

    // PID parameters
Float_t fPdf;
Float_t fIntens;
Float_t fCl;
Int_t   i;

    fPdf    = ((i = pCand->getAlgorithmIndex(algMerged)) >= 0)
                ? pCand->getValue_Idx(i, iIdx) : -1.0f;

    fIntens = ((i = pCand->getAlgorithmIndex(algBayes)) >= 0)
                ? pCand->getValue_Idx(i, iIdx) : -1.0f;

    fCl     = ((i = pCand->getAlgorithmIndex(
                        algMerged + CL_ALOGRITHM_OFFSET)) >= 0)
                ? pCand->getValue_Idx(i, iIdx) : -1.0f;

    pParticle->setPidParams(nPid, fPdf, fIntens, fCl);

    // experimental parameters
    pParticle->setExpParams(pKickTrack->getSystem(), pKickTrack->getBeta(),
                pKickTrack->getR(), pKickTrack->getZ());

    return pParticle;
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

void HPidParticleFiller::print(void) const
{
// Print the list of all algorithms

    printf("HPidParticleFiller\n");
    printf("Checking function : %p\n", pCheckFun);
    printf("Checking condition: Val(alg = %d) %s %f%s   FAKES %s\n", iAlgorithm,
                    getOperator(), fThreshold,
                    (bCheckMax) ? "    ! MAX !" : "",
                    (bIncludeFakes) ? "ON" : "OFF");
}
