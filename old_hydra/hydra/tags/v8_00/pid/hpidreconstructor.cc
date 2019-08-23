// @(#)$Id: hpidreconstructor.cc,v 1.16 2006-08-16 16:13:35 halo Exp $
//*-- Author : Marcin Jaskula 27/07/2002
//  Modified : Marcin Jaskula 06/09/2002
//  Modified : Marcin Jaskula 18/11/2002
//  Modified : Marcin Jaskula 18/03/2003
//           - get rid of storing the merged values in the HPidCandidates
//             and the interface for it

//_HADES_CLASS_DESCRIPTION
////////////////////////////////////////////////////////////////////////////////
//
// HPidReconstructor
//
// A set of all PID algorithms
// HPidAlgorithms may be connected to the reconstructor by addAlgorithm()
// methods.
// init(), reinit(), execute() and finalize() methods of algorithms are called
// in the methods of the reconstructor with the same names.
//
// The configuration of the reconstructor may be done by options given
// in the constructor or by setParameters() method. Possible options:
// PDF    - calculate PDF
// CL     - calculate CL
// DEBUGX - debug info, if X set then it defines the debug info level
//
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
#include "hpidtrackfiller.h"
#include "hpidtrackdata.h"
#include "hpidhitdata.h"
#include "hpidfl.h"
#include "hpidphysicsconstants.h"

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
  // For par[] options see setParameters() method desription.
  setDefault();
  setParameters(par);
}

// -----------------------------------------------------------------------------

HPidReconstructor::HPidReconstructor(Text_t name[], Text_t title[],
                                    Option_t par[])
    : HReconstructor(name, title)
{
  // Constructor with names
  // For par[] options see setParameters() method desription.
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
}

// -----------------------------------------------------------------------------

void HPidReconstructor::setDefault(void)
{
  // Called in constructors to set default valuest to the members
  iSelectedMomAlg = -1;
  pInputCat = NULL;
  pitInput  = NULL;
  pOutCat   = NULL;
  bInDelete = kFALSE;
  
  for(Int_t i = 0; i < kMaxParticles; i++)
    aParticles[i] = nPID_DUMMY;
  
  pAlgorithms = new TList();
  pitList = pAlgorithms->MakeIterator();
  
  iParticles = 0;
  iAlgorithms = 0;
  iDebug = 0;
  iVectPerAlg = 0;

  pParams = NULL;
  
  bInitOk = kFALSE;
  
  bCalcPDF   = kTRUE;
  bCalcCL    = kTRUE;
  bCalcRelints = kTRUE;
}

// -----------------------------------------------------------------------------

void HPidReconstructor::setParameters(Option_t par[])
{
// Set parameters by names. Options (may be separated by comma or blank chars):
// PDF    - calculate PDF
// CL     - calculate CL
// RELINTS     - calculate RELINTS (Not useful for hard cut or banana cut pid)
// DEBUGX - debug info, if X set then it define the debug info level

TString s = par;
char   *p;

    s.ToUpper();

    bCalcPDF   = (strstr(s.Data(), "PDF") != NULL);
    bCalcCL    = (strstr(s.Data(), "CL") != NULL);
    bCalcRelints = (strstr(s.Data(), "RELINTS") != NULL);

    iDebug = 0;
    if((p = strstr(s.Data(), "DEBUG")) != NULL)
    {
        if(sscanf(p + strlen("DEBUG"), "%d", &iDebug) <= 0)
            iDebug = 1;
    }
    
    if(strstr(s.Data(),"ALG_KICK")!=NULL) iSelectedMomAlg=ALG_KICK;
    if(strstr(s.Data(),"ALG_KICK123")!=NULL) iSelectedMomAlg=ALG_KICK123;
    if(strstr(s.Data(),"ALG_SPLINE")!=NULL) iSelectedMomAlg=ALG_SPLINE;
    if(strstr(s.Data(),"ALG_REFT")!=NULL) iSelectedMomAlg=ALG_REFT;
    if(strstr(s.Data(),"ALG_RUNGEKUTTA")!=NULL) iSelectedMomAlg=ALG_RUNGEKUTTA;
}

// -----------------------------------------------------------------------------
// Initialize the reconstructor and call init of all algorithms
Bool_t HPidReconstructor::init(void)
{
  HPidAlgorithm *pAlg;
  Bool_t         bReturn = kTRUE;
  Int_t          i;
  
  bInitOk = kFALSE;
  
  if(( ! bCalcPDF) && ( ! bCalcCL))
    {
      Error("init", "No output defined");
      return kFALSE;
    }
  
  // parameters of hpidreconstructor task - this should only be done if relative intensities are required
  if(bCalcRelints)
    {
      if((pParams = (HPidReconstructorPar *)gHades->getRuntimeDb()
	  ->getContainer(PIDRECONSTRUCTORPAR_NAME)) == NULL)
	{
	  Error("init", "Cannot get parameters: %s", PIDRECONSTRUCTORPAR_NAME);
	  return kFALSE;
	}
    }

  // input category
  if((pInputCat = HPidTrackCand::buildPidTrackCandCategory()) == NULL)
    {
      Error("init", "Cannot build HPidTrackCand Category");
      return kFALSE;
    }
  
  if((pitInput = (HIterator *)pInputCat->MakeIterator()) == NULL)
    {
      Error("init", "Cannot make an iterator for HPidTrackCand Category");
      return kFALSE;
    }
  
  // algorithms
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

  if((bCalcPDF) && (bCalcCL))
    {
      i *= 2;
      iVectPerAlg = 2;
    }

  i++; 
  //for the relative intensities - this is always incremented, 
  //even if we do not compute relative intensities because we
  //provide a default intensity of 1.0;
  
  
  iAlgorithms = i;
  
  if((pOutCat = HPidCandidate::buildPidCandidateCategory()) == NULL)
    {
      Error("init", "Cannot build HPidCandidate category");
      return kFALSE;
    }
  
  pitList->Reset();
  while((pAlg = (HPidAlgorithm *)pitList->Next()) != NULL)
    bReturn &= pAlg->init();
  
  bInitOk = bReturn;
  print();
  return bReturn;
}

// -----------------------------------------------------------------------------

Bool_t HPidReconstructor::reinit(void)
{
// Reinitialize the reconstructor and call reinit of all algorithms

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
// Finalize the reconstructor and all algorithms
Bool_t HPidReconstructor::finalize(void)
{
  HPidAlgorithm *pAlg;
  Bool_t         bReturn = kTRUE;
  
  pitList->Reset();
  while((pAlg = (HPidAlgorithm *)pitList->Next()) != NULL)
    bReturn &= pAlg->finalize();
  
  return bReturn;
}


// -----------------------------------------------------------------------------
// Call execute of all algorithms and store result in a new HPidCandidate object
Int_t HPidReconstructor::execute(void)
{
  HPidAlgorithm *pAlg;
  HPidTrackCand *pTrack;
  HPidCandidate *pCand;

  Int_t          iAlgs;
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
  
  // clear the data from the old file
  pOutCat->Clear();
  
  //loop over all HPidTrackCandidates
  pitInput->Reset();
  while((pTrack = (HPidTrackCand *)pitInput->Next()) != NULL)
    {
      
      //Skip all particle candidates for which the desired momentum algorithm was NOT successful!
      if(pTrack->getTrackData()->bIsAccepted[iSelectedMomAlg]==kFALSE) continue;

      //Skip all particle candidates for which the desired momentum algorithm had NO matching with a meta hit
      if(pTrack->getHitData()->getMetaTrackCorrelation(iSelectedMomAlg)==kFALSE) continue;

      //Skip all particle candidates without hit in META
      if(pTrack->getHitData()->iSystem==-1) continue;
      
      
      if(iDebug)
	printf(".");
      
      if((pCand = getNextSlot()) == NULL)
	{
	  Error("HPidReconstructor::execute()","No new slot in catPidCandidate available");
	  return 0;
	}

      
      pCand->setTrackCandIndex(pInputCat->getIndex(pTrack));
      
      pitList->Reset();
      iAlgs = 0;

      //loop over all algorithms in the list and ask them for a statement about the current HPidTrackCand
      while((pAlg = (HPidAlgorithm *)pitList->Next()) != NULL)
        {
	  pPDF = pCL = NULL;
	  
	  //We need the PDF values for the observables of this HPidTrackCand object
	  if(bCalcPDF)
            {
	      //Get the storage pointer within the HPidCandidate object where to put the PDF values for the algorithm
	      pPDF = pCand->getValuesVectorByIndex(iAlgs);

	      //Tell the HPidCandidate that at position "iAlgs" the algorithm "pAlg->getAlgId()" has stored its PDF result
	      pCand->setAlgorithmIdByIndex(iAlgs, pAlg->getAlgId());

	      iAlgs++;
            }
	  
	  if(bCalcCL)
            {
	      //Get the storage pointer within the HPidCandidate object where to put the CL values for the algorithm
	      pCL = pCand->getValuesVectorByIndex(iAlgs);

	      //Tell the HPidCandidate that at position "iAlgs" the algorithm "pAlg->getAlgId()" has stored its CL result
	      pCand->setAlgorithmIdByIndex(iAlgs, pAlg->getAlgId() + CL_ALOGRITHM_OFFSET);
	      
	      iAlgs++;
            }
	  
	  if(iDebug > 1)
	    printf("Alg: %d %p %p\n", iAlgs, pPDF, pCL);
	  
	  //fix me
	  //Now the HPidCandidate has told the current algorithm where to place its results
	  //We can now execute the algorithm
	  Short_t dummy;
	  pAlg->execute(pTrack, pPDF, pCL, dummy);
	  if(pPDF != NULL)
	    normalize(pPDF, iParticles);
        }
      
      //The last results we store are the relative intensities for all active particle species
      calculateRelInts(pTrack, pCand);
      
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
  
  pOut = (HPidCandidate *) new (pOut) HPidCandidate(iParticles,iAlgorithms,0,iSelectedMomAlg);
  
  pOut->setTrackCandIndex(-1);
  pOut->setParticleIds( aParticles, iParticles );
  
  return pOut;
}

// -----------------------------------------------------------------------------
//Computes relative intensities of all species for a given kinematical region
//Input object is pTrack, output object is pCand
void HPidReconstructor::calculateRelInts(HPidTrackCand *pTrack,
					 HPidCandidate *pCand)
{
  Int_t          iPart;
  Float_t        fTheta, fMom;

  //We store the relative intensities in the LAST coloumn of the pCand object
  //All others are filled with PDF values and CL values from the algorithms!
  Int_t          iMax = pCand->getNAlgorithms() - 1;
  Int_t          iSys, iSect;

  //The LAST algorithm is the relint algorithm
  pCand->setAlgorithmIdByIndex(iMax, algRelInt);


  //Get relevant data structures from input object
  HPidTrackData* pTrackData = pTrack->getTrackData();
  HPidHitData* pHitData = pTrack->getHitData();

  

  //pick and check momentum from track data because our relative intensities depend on momentum
  if(!pTrackData->bIsAccepted[iSelectedMomAlg]) 
    {
      Error("HPidReconstructor::calculateRelints()","The current track is not accepted. Tis should be intercepted");
      exit(-1);
    }

  fMom = pTrackData->fMomenta[iSelectedMomAlg];
  if(fMom<0)
    {
      Error("HPidReconstructor::calculateRelints()","The current track has negative momentum");
      exit(-1);
    }
  

  fTheta = pHitData->fMdcTheta;
  iSys = pHitData->iSystem;
  iSect = pHitData->nSector; 

  //get a place to store the relative intensities -> by convention this is the LAST vector in the array
  Float_t *fRelInt = pCand->getValuesVectorByIndex(iMax);
  

  // calculate relative intensity parameters for all particles
  if(bCalcRelints)
    {
      for(iPart = 0;
	  (iPart < kMaxParticles) && (aParticles[iPart] != nPID_DUMMY);
	  iPart++)
	{
	  fRelInt[iPart] = pParams->getIntensity(aParticles[iPart],
						 iSys, iSect, fMom, fTheta); /*ADDON*/
	  
	  //This is for experts only. Users beware to temper with this!!
	  //This is provided SOLELY for testing the robustness of PID with respect to
	  //arbitrary variations in the relative intensities of leptons/hadrons
	  //DO NOT CHANGE THE DEFAULTS PERMANENTLY IN CVS!! THIS WILL BREAK THE ANALYSIS
	  Float_t scale_protons = 1.0;
	  Float_t scale_piplus = 1.0;
	  Float_t scale_piminus = 1.0;
	  
	  Float_t scale_electrons = 1.0;
	  Float_t scale_positrons = 1.0;
	  Float_t scale_fakeminus = 1.0;
	  Float_t scale_fakeplus = 1.0;
	  Float_t scale_default = 1.0;
	  //scale here: 
	  switch (aParticles[iPart]){
	  case 14:
	    fRelInt[iPart]*=scale_protons;
	    break;
	  case 8:
	    fRelInt[iPart]*=scale_piplus;
	    break;
	  case 9:
	    fRelInt[iPart]*=scale_piminus;
	    break;
	  case 3:
	    fRelInt[iPart]*=scale_electrons;
	    break;
	  case 2:
	    fRelInt[iPart]*=scale_positrons;
	    break;
	  case -1:
	    fRelInt[iPart]*=scale_fakeminus;
	    break;
	  case -2:
	    fRelInt[iPart]*=scale_fakeplus;
	    break;
	  default:
	    fRelInt[iPart]*=scale_default;
	    break;
	  }
	}
      //end expert section!!
    }
  else
    {
      for(iPart = 0;
	  (iPart < kMaxParticles) && (aParticles[iPart] != nPID_DUMMY);
	  iPart++)
	{
	  //If the user disabled computation of relative intensities we store a default value which
	  //sets the weight of all species equal.
	  fRelInt[iPart] = 1.0;
	}
    }
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

// Returns particle type with index iPos in array of active particles
Short_t HPidReconstructor::getParticleId(Int_t iPos) const
{


  if((iPos < 0) || (iPos > iParticles)) //should be >= ??
    {
      Error("getParticleId", "%d out of bounds", iPos);
      return -1;
    }
  
  return aParticles[iPos];
}

// -----------------------------------------------------------------------------

// Returns position of particle nType in aParticles vector or -1 if not set
Int_t HPidReconstructor::getParticleIndex(Short_t nType) const
{

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
// Add particle nType to the array of the particles the user is interested in.
// Returns the position of the particle in the array or
// -1 - no place in the array
// -2 - the particle is already added to the array

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
// copy ids of particles form the input array to aParticles
  if(iSize <= 0)
    {
      Error("setParticleIds", "iSize <= 0");
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
  
  fprintf( stderr, "iParticles = %i, kMaxParticles = %i, IDs : ", iParticles, kMaxParticles );
  for( Int_t loop = 0; loop < iParticles; loop++ ) fprintf( stderr, "%i   ", aParticles[loop] );
  fprintf(stderr, "\n" );
    
}

// -----------------------------------------------------------------------------

void HPidReconstructor::setDefaultParticleIds(void)
{
// Sets default set of the particles:
// 2(e+), 3(e-), 8(pi+), 9(pi-), 14(p), 45(d), 11(K+), -2 and -1 (fakes)

Short_t aPartIds[] =
{
    2,  // e+
    3,  // e-
    8,  // pi+
    9,  // pi-
   14,  // p
   45,  // d
   11,  // K+
   HPidPhysicsConstants::fakeNeg(),  // fake negative
   HPidPhysicsConstants::fakePos()   // fake positive
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

    printf("\nOutputs      : %s %s\n", (bCalcPDF) ? "PDF" : "",
                                        (bCalcCL) ? "CL" :  "");
    printf("\nRelints: %s \n",    (bCalcRelints) ? "yes" : "no");

    printf("\nAlgorithms:\n");

    pitList->Reset();
    while((pAlg = (HPidAlgorithm *)pitList->Next()) != NULL)
    {
        printf("* %d %s, weight: %f\n", pAlg->getAlgId(),
                    pAlg->getName().Data(), pAlg->getWeight());
        pAlg->print();
    }
}

// -----------------------------------------------------------------------------
// Static function to normalize the propability vector computed by the algorithm
// Only values >= 0.0 are taken into the accout
void HPidReconstructor::normalize(Float_t af[], UInt_t iSize)
{
  Float_t fSum = 0.0;
  UInt_t  i;
  
  for(i = 0; i < iSize; i++)
    {
      if(af[i] >= 0.0f)
	fSum += af[i];
    }
  
  if((fSum > 0.0f) && (fSum != 1.0f))
    {
      fSum = 1.0f / fSum;
      
      for(i = 0; i < iSize; i++)
        {
	  if(af[i] >= 0.0f)
	    af[i] *= fSum;
        }
    }
}
