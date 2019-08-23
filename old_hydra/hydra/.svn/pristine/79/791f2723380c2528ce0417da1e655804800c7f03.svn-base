// @(#)$Id: hpidparticlefiller.cc,v 1.18 2005-06-01 15:05:47 christ Exp $
//*-- Author : Marcin Jaskula 30/10/2002
//  Modified : Marcin Jaskula 25/02/2003
//             filling artificial particle when no particle from the track
//             created.
//  Modified : Marcin Jaskula 02/03/2003
//             get rid of HPidParticleFillerSim
//  Modified : Marcin Jaskula 23/05/2003
//             new organization of HPidCandidate
//  Modified : Marcin Jaskula 24/06/2003
//             HPidParticle::fillMultiUsed used

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

using namespace std;
#include <iostream>
#include "hcategory.h"
#include "hiterator.h"

#include "hpidparticlefiller.h"

#include "hpidcandidate.h"
#include "hpidtrackcand.h"
#include "hpidtrackdata.h"

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
/*
const Char_t* HPidPartFiller__asOperators[] =
    { "==", "<", "<=", ">", ">=" };
*/

HPidParticleFiller *HPidParticleFiller::pCurPartFiller = NULL;

// -----------------------------------------------------------------------------

#define SAFE_DELETE(A) { if(A) { delete (A); A = NULL; }}

// -----------------------------------------------------------------------------

HPidParticleFiller::HPidParticleFiller(Option_t par[])
                    : HReconstructor("PidPartFiller",
                                     "Filler of HPidParticle category")
{
// Default constructor.
// par parameter is used in setParameters() method

    setDefault();
    setParameters(par);
    pCurPartFiller = this;
}

// -----------------------------------------------------------------------------

HPidParticleFiller::HPidParticleFiller(Text_t name[], Text_t title[],
                                        Option_t par[])
                    : HReconstructor(name, title)
{
// Constructor with names
// par parameter is used in setParameters() method

    setDefault();
    setParameters(par);
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
  
  iMomAlg = -1;
  
  pInputCat = NULL;
  pitInput  = NULL;
  pOutCat   = NULL;

  sOutClass = "";
  pTrackCandCat = NULL;
  pKickTrackCat = NULL;
  
  //pCheckFun     = NULL;
  pMethodCall   = NULL;
  
  iAlgorithm    = algBayes;
  aAlgorithms.Set(0);
  //fThreshold    = 0.0f;
  //eOperator     = kEqual;
  //bCheckMax     = kFALSE;
  bIncludeFakes = kFALSE;
  //bUseMassExp   = kTRUE;
  
  iDebug = 0;
  
  //bMakeArtPart = kFALSE;  
  //pOutClass = NULL;
  //bFillKine = kTRUE;
  iProcessMask = 0;
  
  bMakeSimCategory = kFALSE;
  //bIsOutSim = kFALSE;
  //bIsOutHit = kFALSE;
}

// -----------------------------------------------------------------------------

Bool_t HPidParticleFiller::init(void)
{
// Build all categories

    if(iDebug)
        Warning("init", "begin");

    // output category
    if(buildOutputCategory(bMakeSimCategory) == kFALSE)
        return kFALSE;

    // HPidTrackCand category
    if((pTrackCandCat = HPidTrackCand::buildPidTrackCandCategory()) == NULL)
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

    if(iDebug)
        Warning("init", "end");


    return kTRUE;
}

// -----------------------------------------------------------------------------

Bool_t HPidParticleFiller::reinit(void)
{
// Check the reconstructor parameters

  /*if(pOutClass == NULL)
    {
        Error("reinit", "No output class: \"%s\"", sOutClass.Data());
        return kFALSE;
    }
 
    if((pCheckFun == NULL) && (iAlgorithm < 0))
    {
        Error("reinit", "No condition defined");
        return kFALSE;
    }
  */
    return kTRUE;
}

// -----------------------------------------------------------------------------

Bool_t HPidParticleFiller::buildOutputCategory(Bool_t makeSimCategory)
{
// Check the output class and build output category for it.


  HEvent *pEvent;

  if((gHades == NULL) || ((pEvent = gHades->getCurrentEvent()) == NULL))
    {
      Error("buildOutputCategory", "Cannot access current event");
      return kFALSE;
    }
  /*
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
  {*/
  if(bMakeSimCategory)
    pOutCat = HPidParticleSim::buildPidParticleSimCategory();
  else
    pOutCat = HPidParticle::buildPidParticleCategory();
  
  if(pOutCat == NULL)
    {
      Error("buildOutputCategory", "Cannot create new category");
      //pOutClass = NULL;
      
      return kFALSE;
    }
  ((HLinearCategory*)pOutCat)->setDynamicObjects(kTRUE);
  pEvent->addCategory(catPidPart, pOutCat, "Pid");
  
  
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
// SPLINE/KICK/KICK123

TString s = par;
char   *p;

    s.ToUpper();

    iDebug = 0;
    if((p = strstr(s.Data(), "DEBUG")) != NULL)
    {
        if(sscanf(p + strlen("DEBUG"), "%d", &iDebug) <= 0)
            iDebug = 1;
    }

    //bUseMassExp  = (strstr(s.Data(), "MASSEXP") != NULL);
    //bMakeArtPart = (strstr(s.Data(), "MAKEART") != NULL);
    bIncludeFakes  = (strstr(s.Data(), "INCLUDEFAKES") != NULL);
    bMakeSimCategory = (strstr(s.Data(), "MAKESIMCATEGORY") != NULL);
    if(bMakeSimCategory)
      {
	sOutClass = "HPidParticleSim";
      }
    else
      {
	sOutClass = "HPidParticle";
      }
    Int_t nMomAlgsDefined=0;
    
    if(s.Contains("SPLINE"))
      {
	iMomAlg=ALG_SPLINE;
	nMomAlgsDefined++;
      }
    if(s.Contains("RUNGEKUTTA"))
      {
	iMomAlg=ALG_RUNGEKUTTA;
	nMomAlgsDefined++;
      }
    if(s.Contains("KICK"))
      {
	iMomAlg=ALG_KICK;
	nMomAlgsDefined++;
      }
    if(s.Contains("KICK123"))
      {
	iMomAlg=ALG_KICK123;
	nMomAlgsDefined++;
      }
    
    if(nMomAlgsDefined>1)
      {
	Error("HPidParticleFiller::setParameters()","Too many momentum algorithms selected");
	//return 0;
      }
    if(nMomAlgsDefined==0)
      {
	cout << "No momentum algorithm selected for HPidParticleFiller - default is spline momentum reconstruction!" << endl;
	iMomAlg=ALG_SPLINE;
      }
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
/*
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
*/
HPidCandidate *pCand;
Int_t          i;
Int_t          iSum = 0;

    // clear the data from the old file
    pOutCat->Clear();

    pitInput->Reset();
    while((pCand = (HPidCandidate *)pitInput->Next()) != NULL)
      {
        i = checkCandidate(pCand);
	/*
        if((i <= 0) && (bMakeArtPart))
            makeArtificialParticle(pCand);

        iSum += 1;
	*/
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
//HKickTrack    *pKickTrack;
UInt_t         iPart;
Int_t          i;
Float_t        fTestVal;

    // get the HPidTrackCand
    if(pCand->getTrackCandIndex() < 0)
        return 0;

    if((pTrackCand = pCand->getTrackCandidate(pTrackCandCat)) == NULL)
    {
        Error("checkCandidate", "Cannot get HPidTrackCand");
        return 0;
    }
    /*
    // get HKickTrack
    if(pTrackCand->getKickTrackId() < 0)
        return 0;

    if((pKickTrack = pTrackCand->getKickTrack(pKickTrackCat)) == NULL)
    {
        Error("checkCandidate", "Cannot get HKickTrack");
        return 0;
    }
    */
    // for the user function (condition) (this I must reimplement when I have understood it)
    /*    if(pCheckFun != NULL)
    {
      //Loops over all possible species in the PidCandidate and makes an individual decision for each one
      for(iPart = 0; iPart < pCand->getNParticles(); iPart++)
	{ //Here the user defined checks take place 
	  if(pCheckFun(pCand, iPart, &fTestVal))
	    {
	      //If a hypothesis is tested OK then for this hypothesis a particle is made - weights are not set anywhere
	      if(makeParticle(pCand, pKickTrack, iPart, fTestVal) != NULL)
		iReturn++;
            }
        }
      
      return iReturn;
    }
    */
    // "normal" condition
    // Here the same happens for the automated decision according to Bayes theorem
    // prepare the output array

    if(aCandValues.GetSize() < (Int_t) pCand->getNParticles())
      aCandValues.Set(pCand->getNParticles());
    
    // copy the propper vector of the parameters
    switch(iAlgorithm)
    {
    case algLikelihood:
      pCand->calcMergedPDFVector(aCandValues.GetArray(),
				 aAlgorithms.GetArray(), aAlgorithms.GetSize());
      break;
      
    case algBayes:
      pCand->calcBayesVector(aCandValues.GetArray(),
			     aAlgorithms.GetArray(), aAlgorithms.GetSize());
      break;
      
    default:
      if((i = pCand->getAlgorithmIndexById(iAlgorithm)) < 0)
	{
	  ::Error("HPidParticleFiller::checkCandidate",
		  "No algorithm %d in the HPidCandidate", iAlgorithm);
	  
	  return kFALSE;
	}
      
      aCandValues.Set(pCand->getNParticles(), pCand->getValuesVectorByIndex(i));
      
      break;
    }

    if(! bIncludeFakes ){
        Int_t nPart =  (Int_t) pCand->getNParticles();
        for(i = 0; i < nPart; i++){
            if(HPidPhysicsConstants::isFake(pCand->getParticleIdByIndex(i))){
                aCandValues[i] = -1.0f;
            }
        }   
        Float_t fSum=0.0;
        //Renormalize prob. values
        for(i = 0; i< nPart; i++){
            if(!HPidPhysicsConstants::isFake(pCand->getParticleIdByIndex(i))){
                fSum += aCandValues[i];
            } 
        }
        if(fSum>0.0){
            for(i = 0; i< nPart; i++){
                aCandValues[i] /= fSum;             
            }
        }

    }   
    /*
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
    */
    HPidParticle* pParticle = getNextSlot();
    if(bMakeSimCategory)
      {
	pParticle = new (pParticle) HPidParticleSim(pCand, aCandValues.GetArray());
      }
    else
      pParticle = new (pParticle) HPidParticle(pCand, aCandValues.GetArray());
    iReturn++;

    return iReturn;
}

// -----------------------------------------------------------------------------

HPidParticle* HPidParticleFiller::makeParticle(const HPidCandidate *pCand, Float_t fTestValue)
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
  Int_t MomAlgIndex = pCand->getMomAlg();
  HPidTrackData* pTrackData = pCand->getTrackCandidate()->getTrackData();
  Float_t fBeta = pTrackData->fCorrectedBeta[MomAlgIndex];

    if(fBeta > 0.0f)
    {
        if((fMassExp = 1.0f / (fBeta * fBeta) - 1.0f) < 0.0f)
            fMassExp = - TMath::Sqrt(-fMassExp);
        else
            fMassExp = TMath::Sqrt(fMassExp);

        fMassExp *= pTrackData->fMomenta[MomAlgIndex];
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

    // kinematic vector
    Double_t dM;
    Short_t  nPid;
    /* 
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
    */
    /*computed automatically in constructor now
    pParticle->SetXYZM(dP * dSin * TMath::Cos(dPhi),
                        dP * dSin * TMath::Sin(dPhi),
                        dP * TMath::Cos(dTheta),
                        dM);
    */
    // PID parameters
      // done internally in constructor
    //pParticle->setPidParams(nPid, fTestValue);
    /*
    // experimental parameters -- obsolete: can be retrieved from hitdata and trackdata
    pParticle->setExpParams(pKickTrack->getSystem(), pKickTrack->getBeta(),
                pKickTrack->getR(), pKickTrack->getZ(), pKickTrack->getPull());
    */
    // filling additional data

    /*
      HPidTrackCand *pTrackCand = NULL;

    if((pTrackCand = pCand->getCandidate()) == NULL)
    {
        Error("makeParticle", "Cannot get HPidTrackCand");
    }
    else
    {
        // filling the MultiUsed flags
        pParticle->fillMultiUsed(pTrackCand);

        if((bIsOutSim == kTRUE) || (bIsOutHit == kTRUE))
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
    */
    return pParticle;
}

// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
/*
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
*/
// -----------------------------------------------------------------------------
/*
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

    //pCheckFun = interactiveFun;
}
*/
// -----------------------------------------------------------------------------

void HPidParticleFiller::print(void) const
{
// Print the list of all algorithms

  printf("Output class         : %s   ", sOutClass.Data());
    /*
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
    */
    //printf("Fill kine            : %s\n", (bFillKine) ? "ON" : "OFF");
    printf("Process Mask         : 0x%08X\n", iProcessMask);
}
