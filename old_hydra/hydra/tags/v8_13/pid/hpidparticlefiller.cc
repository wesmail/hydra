// @(#)$Id: hpidparticlefiller.cc,v 1.24 2006-12-08 14:52:27 christ Exp $
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

//_HADES_CLASS_DESCRIPTION
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// HPidParticleFiller                                                         //
//                                                                            //
// Filler of HPidParticle category from the data in HPidCandidate.            //
// This reconstructor loops over all HPidCandidates and builds HPidParticles  //
// from them. The procedure is as follows:                                    //
// 1. Pick a HPidCandidate                                                    //
// 2. Get merged PDF or full Bayesian probability from the Candidate          // 
// 


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

HPidParticleFiller *HPidParticleFiller::pCurPartFiller = NULL;

// -----------------------------------------------------------------------------

#define SAFE_DELETE(A) { if(A) { delete (A); A = NULL; }}

// -----------------------------------------------------------------------------

HPidParticleFiller::HPidParticleFiller(Option_t par[])
                    : HReconstructor("PidPartFiller",
                                     "Filler of HPidParticle category")
{
  // Default constructor.
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
    setDefault();
    setParameters(par);
    pCurPartFiller = this;
}

// -----------------------------------------------------------------------------

HPidParticleFiller::~HPidParticleFiller(void)
{
  // Destructor
  
  SAFE_DELETE(pitInput);
  aCandValues.Set(0);
  aAlgorithms.Set(0);

  if(pCurPartFiller == this)
    pCurPartFiller = NULL;
}

// -----------------------------------------------------------------------------

void HPidParticleFiller::setDefault(void) //OK
{
// Called in constructors to set default valuest to the members
  

  pInputCat = NULL;
  pitInput  = NULL;
  pOutCat   = NULL;

  
  iAlgorithm    = algBayes;
  bIncludeFakes = kFALSE;
  bMakeSimCategory = kFALSE;
  bUseMassExp=kTRUE;


  aAlgorithms.Set(0);
  aCandValues.Set(0);


  iMomAlg = -1;
  iDebug = 0;
}

// -----------------------------------------------------------------------------

Bool_t HPidParticleFiller::init(void)//OK
{
// Build all categories

    if(iDebug)
        Warning("init", "begin");

    // output category
    if(buildOutputCategory(bMakeSimCategory) == kFALSE)
        return kFALSE;

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

Bool_t HPidParticleFiller::reinit(void) //OK
{
// Check the reconstructor parameters
  
  if((iAlgorithm < 0))
    {
      Error("reinit", "No algorithm defined");
      return kFALSE;
    }
  
    return kTRUE;
}

// -----------------------------------------------------------------------------

Bool_t HPidParticleFiller::buildOutputCategory(Bool_t makeSimCategory) //OK
{
// Check the output class and build output category for it.


  HEvent *pEvent;

  if((gHades == NULL) || ((pEvent = gHades->getCurrentEvent()) == NULL))
    {
      Error("buildOutputCategory", "Cannot access current event");
      return kFALSE;
    }

  if(bMakeSimCategory)
    {
      pOutCat = HPidParticleSim::buildPidParticleSimCategory();
    }
  else
    {
      pOutCat = HPidParticle::buildPidParticleCategory();
    }

  if(pOutCat == NULL)
    {
      Error("buildOutputCategory", "Cannot create new category");
      return kFALSE;
    }

  ((HLinearCategory*)pOutCat)->setDynamicObjects(kTRUE);
  pEvent->addCategory(catPidPart, pOutCat, "Pid");
  
  
  return kTRUE;
}

// -----------------------------------------------------------------------------

void HPidParticleFiller::setParameters(Option_t par[]) //OK
{
// Set parameters by names. Options (may be separated by comma or blank chars):
// DEBUGX  - debug info, if X set then it define the debug info level
// MASSEXP - use experimental mass to fill TLorentzVector
// MAKESIMCATEGORY - fill simulation objects (HPidParticleSim)
// INCLUDEFAKES - consider "fake" as a species as which a particle can be identified
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
  
  bUseMassExp  = (strstr(s.Data(), "MASSEXP") != NULL);
  bIncludeFakes  = (strstr(s.Data(), "INCLUDEFAKES") != NULL);
  bMakeSimCategory = (strstr(s.Data(), "MAKESIMCATEGORY") != NULL);
  //iAlgorithm = strstr(s.Data(), "ALGLIKELIHOOD") ? algLikelihood : algBayes;
  if(s.Contains("ALGLIKELIHOOD")) iAlgorithm=algLikelihood;
  if(s.Contains("ALGBAYES")) iAlgorithm=algBayes;
  if(s.Contains("ALGRICH")) iAlgorithm=algRich;
  if(s.Contains("ALGTOF")) iAlgorithm=algTof;
  if(s.Contains("ALGSHOWER")) iAlgorithm=algShower;
  if(s.Contains("ALGMOMVSBETA")) iAlgorithm=algMomVsBeta;

  if(s.Contains("ALGSTANDCUTS")) iAlgorithm=algStandCuts;
  if(s.Contains("ALGMOMVSELOSS")) iAlgorithm=algMomVsEloss;
  if(s.Contains("ALGHARDCUTS")) iAlgorithm=algHardCuts;
  if(s.Contains("ALGMDCELOSS")) iAlgorithm=algMdcEloss;


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
      exit(-1);
    }
  if(nMomAlgsDefined==0)
    {
      cout << "No momentum algorithm selected for HPidParticleFiller - switching to default: kickplane momentum reconstruction!" << endl;
      iMomAlg=ALG_KICK;
    }
}

// -----------------------------------------------------------------------------
//Append an algorithm to the list of algorithms which shall be used in building a merged probability statement
void HPidParticleFiller::addAlgorithm(Int_t iAId) //OK
{
    aAlgorithms.Set(aAlgorithms.GetSize() + 1);
    aAlgorithms[aAlgorithms.GetSize() - 1] = iAId;
}

// -----------------------------------------------------------------------------
//Remove an algorithm from the list of algorithms which shall be used in building a merged probability statement
void HPidParticleFiller::removeAlgorithm(Int_t iAId) //OK
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
    //Brilliant loop by magic Marcin - difficult to understand but correct
}

// -----------------------------------------------------------------------------
//All work has been relayed to the checkCandidate function
Int_t HPidParticleFiller::execute(void)//OK
{

  HPidCandidate *pCand;
  Int_t          i;
  Int_t          iSum = 0;
  
  // clear the data from the old file
  pOutCat->Clear();
  
  pitInput->Reset();
  while((pCand = (HPidCandidate *)pitInput->Next()) != NULL)
    {

      //Here is where eventually a HPidParticle is created
      i = checkCandidate(pCand);

      if(iDebug)
	printf("%d ", i);
    }
  
  if(iDebug)
    printf("(%d)\n", iSum);
  
  return 0;
}

// -----------------------------------------------------------------------------

HPidParticle* HPidParticleFiller::getNextSlot(void) //OK
{
// Get next slot from the output category


  static HLocation  locDummy;
  HPidParticle  *pOut = NULL;
  
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

Int_t HPidParticleFiller::checkCandidate(HPidCandidate *pCand) //OK
{
// Check if pCand has a good quality and fills the HPidParticle for it.
// The HKickTrack correlated with the HPidCandidate must be available.
// The method returns number of HPidParticle created for the pCand.

  Int_t          iReturn = 0;
  
  Int_t          i;
  
  
  // Here the input to the constructor of HPidParticle is prepared: The merded probability statement of all methods 
  // selected from HPidCandidate. This can be done in two ways: Merging only all PDFs ("algLilekihood") or taking
  // into account prior probabilities ("algBayes"). The user can configure the list of algorithms to be used
  // by adding and removing algorithms from the array aAlgorithms. By default the size of this array is 0 and 
  // HPidCandidate instances use ALL algorithms that have been executed successfully

 
 if(aCandValues.GetSize() < (Int_t) pCand->getNParticles())
   {
     aCandValues.Set(pCand->getNParticles());
   }
 aCandValues.Reset(-1);

 //Shall only all PDFs be combined or shall also the relative intensities be taken into account?
 switch(iAlgorithm)
   {
   case algLikelihood: //Merge PDF statements - do not care for relints
     pCand->calcMergedPDFVector(aCandValues.GetArray(),
				aAlgorithms.GetArray(), aAlgorithms.GetSize());
     break;
     
   case algBayes: //Multiply merged pdf statements by relative intensities 
     pCand->calcBayesVector(aCandValues.GetArray(),
			    aAlgorithms.GetArray(), aAlgorithms.GetSize());
     break;
     
   default: //Pick one single algorithm from the list in HPidCandidate 
     if((i = pCand->getAlgorithmIndexById(iAlgorithm)) < 0)
       {
	 ::Error("HPidParticleFiller::checkCandidate",
		 "No algorithm %d in the HPidCandidate", iAlgorithm);
	 
	 return kFALSE;
       }
     
     aCandValues.Set(pCand->getNParticles(), pCand->getValuesVectorByIndex(i));
     
     break;
   }
 
 //If fakes shall not be considered as species we artificially set their probability value to -1!
 if(! bIncludeFakes ){
   Int_t nPart =  (Int_t) pCand->getNParticles();
   for(i = 0; i < nPart; i++){
     if(HPidPhysicsConstants::isFake(pCand->getParticleIdByIndex(i))){
       aCandValues[i] = -1.0f;
     }
   }   

   //Renormalize probability values
   Float_t fSum=0.0;
   
   //Sum up all contributions except from the fakes (if excluded!)
   for(i = 0; i< nPart; i++){
     if(HPidPhysicsConstants::isFake(pCand->getParticleIdByIndex(i)) && !bIncludeFakes)
       {
	 continue;
       } 
     if(aCandValues[i]<0.0)
       {
	 continue;
       }
     fSum += aCandValues[i];
   }

   //Normalize the sum to unity
   if(fSum>0.0){
     for(i = 0; i< nPart; i++){
       aCandValues[i] /= fSum;             
     }
   }
   
 }   

 //Here we build the output object 
 HPidParticle* pParticle = getNextSlot();
 if(bMakeSimCategory)
   {
     pParticle = new (pParticle) HPidParticleSim(pCand, aCandValues.GetArray());
   }
 else
   {
     pParticle = new (pParticle) HPidParticle(pCand, aCandValues.GetArray());
   }

 iReturn++;
 
 return iReturn;
}


void HPidParticleFiller::print(void) const
{
  //Not implemented
}

