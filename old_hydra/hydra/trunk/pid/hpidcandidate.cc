// @(#)$Id: hpidcandidate.cc,v 1.16 2009-07-15 11:40:20 halo Exp $
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
// Redesigned by Tassilo Christ to reduce number of classes to one by managing 
// size of values array dynamically (HCategory has now the possibility to set
// a flag "hasDynamicObjects" which allows to stream objects of a size which
// isn't known at compile-time.
//

//_HADES_CLASS_DESCRIPTION
////////////////////////////////////////////////////////////////////////////////
//
// HPidCandidate
//
//
// The container consists of all values returend by PidAlgorithms
// for a specific HPidTrackCand
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
// Ids of used algorithms are stored in aAlogrithms vector constisting of
// values from EnumPidAlgorithm_t (piddef.h). Ids larger then 100 mean
// values for CL for algorihm (id - 100)
// Ids of particles are stored in aParticles vector (from Geant).
//
//
// The values returned from the algorithms are stored in 2D array
// aValues[PID_ALG][PARTICLE] - where indices corresponds to values in
// aAlogrithms and aParticles vectors.
//
////////////////////////////////////////////////////////////////////////////////

#pragma implementation

#include "hpidcandidate.h"
#include "hpidtrackcand.h"
#include "hpidfl.h"
#include "hades.h"
#include "hevent.h"
#include "hlinearcategory.h"
#include <iostream>

#include "TError.h"
#include "TROOT.h"
using namespace std;
// -----------------------------------------------------------------------------

ClassImp(HPidCandidate)

// -----------------------------------------------------------------------------

HPidCandidate::HPidCandidate(void)
{
  //default constructor to satisfy root!
}

HPidCandidate::HPidCandidate(Short_t numpart, Int_t numalgs, Int_t candIndex, Int_t momAlgIndex)
{
  NUM_PARTICLES=numpart;
  NUM_ALGORITHMS=numalgs;
  NUM_VALUES=numpart*numalgs;

  //////////////////////////////////////////////////////////////////////////////////////////////////////////
  //
  // Here the "dynamic" part of memory allocation occurs. At compile time the number of algorithms and
  // particles isn't known - so we need to build the array at runtime. 
  // As we overload the Clear-Function (see below) and set "DynamicObjects" to kTRUE the HLinearCategory 
  // does the cleanup for us in HLinearCategory::Clear(Option_t). When the flag is set kTRUE it calls the
  // Clear Function of HPidCandidate which frees the memory again. 
  // (See implementation of HLinearCategory::Clear(Otion_t)
  //
  //////////////////////////////////////////////////////////////////////////////////////////////////////////

  aAlgorithms.Set(NUM_ALGORITHMS);
  aParticles.Set(NUM_PARTICLES);
  aValues.Set(NUM_PARTICLES*NUM_ALGORITHMS);

  iTrackCandIndex = candIndex;
  nMomAlgIndex = momAlgIndex;
  flags = 0;
  Reset();
}

// -----------------------------------------------------------------------------

HPidCandidate::~HPidCandidate(void)
{
// Default destructor. Do not do anything
}


// -----------------------------------------------------------------------------
// functions to access the bit flags copied from HPidTrackCand
void   HPidCandidate::setFlagBit   (Int_t bit)
{
    // set given bit in flag (0-32)
    if(bit >= 0 && bit < 32 ) flags |=  ( 0x01 << bit );
    else {
	Error("HPidCandidate::setFlagBit(Int_t)","Bit number out of range : %i!",bit);
    }
}

void   HPidCandidate::unsetFlagBit (Int_t bit)
{
    // unset given bit in flag (0-32)
    if(bit >= 0 && bit < 32 ) flags &= ~( 0x01 << bit );
    else {
	Error("HPidCandidate::unsetFlagBit(Int_t)","Bit number out of range : %i!",bit);
    }
}

Bool_t HPidCandidate::isFlagBit    (Int_t bit)
{
    // check given bit in flag (0-32)
    // return kTRUE if bit is set
    if (bit >= 0 && bit < 32 )  return (flags >> bit ) & 0x01;
    else {
	Error("HPidCandidate::isFlagBit(Int_t)","Bit number out of range : %i!",bit);
        return kFALSE;
    }
}

Bool_t HPidCandidate::isFlagOR(Int_t num,...)
{
    // check given number of bits in argument in flag (0-32)
    // return kTRUE if any bit is set
    va_list ap;
    va_start(ap,num);

    for(Int_t i=0;i<num;i++){

	Int_t bit=va_arg(ap,Int_t);
	if(bit < 0 || bit > 31)
	{
	    Error("HPidCandidate::isFlagOR()","Bit number out of range : i=%i ,%i!",i,bit);
	    va_end(ap);
	    return kFALSE;
	}
	if(isFlagBit(bit)) {
	    va_end(ap);
	    return kTRUE;
	}
    }
    va_end(ap);
    return kFALSE;
}
Bool_t HPidCandidate::isFlagAND(Int_t num, ...)
{
    // check given number of bits in argument in flag (0-32)
    // return kTRUE if all bits are set
    va_list ap;
    va_start(ap,num);

    for(Int_t i=0;i<num;i++){
        Int_t bit=va_arg(ap,Int_t);
	if(bit < 0 || bit > 31)
	{
	    Error("HPidCandidate::isFlagAND()","Bit number out of range : i=%i ,%i!",i,bit);
	    va_end(ap);
	    return kFALSE;
	}
	if(!isFlagBit(bit)) {
	    va_end(ap);
	    return kFALSE;
	}
    }
    va_end(ap);
    return kTRUE;
}

void HPidCandidate::printFlags(TString comment)
{
    // print the flag field in binary representation
    // Comment will be printed at the end of line

    TString out="";
    for(Int_t i=32;i>0;i--){
	if(i%4==0) out+=" ";
	out+= ( (flags>>(i-1)) & 0x1 );
    }
    cout<<"bin "<<out.Data()<<" "<<comment.Data()<<endl;
}
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
//This function frees the memory occupied by objects of type HPidCandidate in
//the TClonesArray.
void HPidCandidate::Clear(Option_t* opt)
{
  aAlgorithms.Set(0);
  aParticles.Set(0);
  aValues.Set(0);
}

// Clears the arrays setting them to the default values
void HPidCandidate::Reset(void)
{
  Int_t i, iMax;
  if((iMax = getNAlgorithms()) > 0 && aAlgorithms.GetSize()== iMax)
    {
      for(i = 0; i < iMax; i++)
	{
	  aAlgorithms[i] = algNotSet;
	}
    }
  else Error("HPidCandidate::Reset","There was a mismatch in the number of algorithms and the size of the array");


  if(((iMax = getNParticles()) > 0) && (aParticles.GetSize() ==iMax))
    {
      for(i = 0; i < iMax; i++)
	aParticles[i] = -99;
    }
  else Error("HPidCandidate::Reset","There was a mismatch in the number of particles and the size of the array");


  iMax = getNParticles() * getNAlgorithms();
  if(iMax == aValues.GetSize())
    {    for(i = 0; i < iMax; i++)
      {
	aValues[i] = nUNKNOWN;
      }
    }
  else Error("HPidCandidate::Reset","There was a mismatch in the number of values and the size of the array");  
}

// -----------------------------------------------------------------------------

Int_t HPidCandidate::print(void) const
{
  UInt_t              iAlg;
  UInt_t              iPart;
  Bool_t              arraysOK=kTRUE;
  printf(" P \\ A |");
  
  if(aAlgorithms.GetSize() != (Int_t)getNAlgorithms())
    {
      arraysOK=kFALSE;
      printf("Mismatch in size of algorithms array");
    }
  
  if(aParticles.GetSize() != (Int_t)getNParticles())
    {
      arraysOK=kFALSE;
      printf("Mismatch in size of particles array");
    }
  
  if((aValues.GetSize() != (Int_t)getNValues()))
    {
      arraysOK=kFALSE;
      printf("Mismatch in size of values array");
    }
  
  if(arraysOK)
    {
      for(iAlg = 0; iAlg < getNAlgorithms(); iAlg++)
	printf("  %6d |", aAlgorithms[iAlg]);
      
      printf("\n-------+");
      
      for(iAlg = 0; iAlg < getNAlgorithms(); iAlg++)
	printf("---------+");
      
      printf("\n");
      
      for(iPart = 0; iPart < getNParticles(); iPart++)
        {
	  printf("%6d |", aParticles[iPart]);
	  for(iAlg = 0; iAlg < getNAlgorithms(); iAlg++)
	    printf("%8.4f |", aValues[getValuePositionByIndex(iAlg, iPart)]);
	  
	  printf("\n");
        }
    }
  
  printf("\n");
  
  return 0;   // allow to use in T.Draw(...)
}

// -----------------------------------------------------------------------------
//Return the HPidTrackCandidate object that was the origin of the HPidCandidate
HPidTrackCand* HPidCandidate::getTrackCandidate(HCategory *pCat) const
{
  // Returns HPidTrackCand object corresponding to iTrackCandIndex (if it exists)
  // Works when pCat is set or gHades->getCurrentEvent() is accessible
  
  if(iTrackCandIndex < 0)
    return NULL;
  
  return HPidFL::getTrackCand(iTrackCandIndex, pCat);
}


// -----------------------------------------------------------------------------
// Set algorithm Id eAlg at uiPos. Arrays bounds are checked
void HPidCandidate::setAlgorithmIdByIndex(UInt_t uiPos, Short_t eAlg)
{
  if((aAlgorithms.GetSize()<(Int_t)NUM_ALGORITHMS) || (uiPos >=NUM_ALGORITHMS))
    {
      Error("setAlgorithmIdByIndex", "out of bound: [0, %d)", NUM_ALGORITHMS);
        return;
    }

    aAlgorithms[uiPos] = eAlg;
}

// -----------------------------------------------------------------------------

// Set whole vector of algoritms. The input array must have getNAlgorithms() members
void HPidCandidate::setAlgorithmIds(Short_t aeAlgs[])
{
  if(sizeof(aeAlgs)/sizeof(Short_t)!=NUM_ALGORITHMS)
    {
      ::Error("HPidCandidate::setAlgorithms","Number of algorithms not matching selection!");
      return;
    }

  if(aAlgorithms.GetSize() != (Int_t)NUM_ALGORITHMS)
    {
        Error("setAlgorithms", "Mismatch in size of algorithms array");
        return;
    }

  aAlgorithms.Set(NUM_ALGORITHMS,aeAlgs);
}

// -----------------------------------------------------------------------------
// Get algorithm Id from the array of algorithms at position uiPos.
// algNotSet is return if uiPos is out of bounds.
Int_t HPidCandidate::getAlgorithmIdByIndex(UInt_t uiPos) const
{

  if((aAlgorithms.GetSize()<=(Int_t)uiPos) || (uiPos >= NUM_ALGORITHMS))
    {
        Error("getAlgorithmIdByIndex", "out of bound: [0, %d)", NUM_ALGORITHMS);
        return algNotSet;
    }

  return aAlgorithms[uiPos];
}

// -----------------------------------------------------------------------------

Int_t HPidCandidate::getAlgorithmIndexById(Int_t eAlg) const
{
// Find the position of the eAlg algorithm in the array of algorithms.
// Returns -1 if algorithm not set.
// Existing of the proper algorithms array is checked

Int_t               i, iMax;

 
    iMax = getNAlgorithms();
    for(i = 0; i < iMax; i++)
    {
        if(aAlgorithms[i] == eAlg)
            return i;
    }

    return -1;
}

// -----------------------------------------------------------------------------

void HPidCandidate::setParticleIdByIndex(UInt_t uiPos, Short_t nPart)
{
// Sets particle id nPart in the array of particle ids at position uiPos. Array bounds are checked.

    if((aParticles.GetSize() <= (Int_t)uiPos) || (uiPos >= getNParticles()))
    {
        Error("setParticleIdByIndex", "out of bound: [0, %d)", getNParticles());
        return;
    }

    aParticles[uiPos] = nPart;
}

// -----------------------------------------------------------------------------
// Set whole vector of particle Ids. The input array must have getNParticles() members
void HPidCandidate::setParticleIds(Short_t anPart[], Int_t nrOfParticles)
{
  
  if( nrOfParticles != (Int_t)NUM_PARTICLES )
    {
      ::Error("HPidCandidate::setParticleIds","Number of particles does not match");
      return;
    }
  
  if(aParticles.GetSize() != nrOfParticles)
    {
      Error("setParticleIds", "Mismatch in size of particles array");
      return;
    }
  aParticles.Set(NUM_PARTICLES,anPart);
  
}

// -----------------------------------------------------------------------------

Short_t HPidCandidate::getParticleIdByIndex(UInt_t uiPos) const
{
// Gets particle id nPart at position uiPos in the array of particle ids used.
// Array bounds are checked and -10 returned in case of out of bounds.

  if((aParticles.GetSize()<=(Int_t)uiPos) || (uiPos >= getNParticles()))
    {
        Error("getParticleIdByIndex", "out of bound: [0, %d)", getNParticles());
        return -10;
    }

    return aParticles[uiPos];
}

// -----------------------------------------------------------------------------

Int_t HPidCandidate::getParticleIndexById(Short_t nPartId) const
{
// Find the position of the particle with PID nPartId in the array of involved species.
// Returns -1 if particle id not set.
// Existing of a propper particlesId array is checked.

Int_t    i, iMax;


 iMax = getNParticles();
 for(i = 0; i < iMax; i++)
   {
     if(aParticles[i] == nPartId)
       return i;
   }
 
 return -1;
}

// -----------------------------------------------------------------------------


void HPidCandidate::setValueById(Int_t eAlg, Short_t nPartId,
                        Float_t fVal)
{
// Set value in the array of pdf-values for the specified algorithm and particle Id


Int_t    iPart, iAlg;

    if(aValues.GetSize()!=(Int_t)(getNParticles())*(Int_t)(getNAlgorithms()))
    {
        Error("setValueById", "Mismatch in size of values array");
        return;
    }

    if(((iAlg = getAlgorithmIndexById(eAlg)) < 0)
            || ((iPart = getParticleIndexById(nPartId)) < 0))
    {
        return;
    }

    aValues[getValuePositionByIndex(iAlg, iPart)] = fVal;
}

// -----------------------------------------------------------------------------

void HPidCandidate::setValueByIndex(UInt_t uiAlgIdx, UInt_t uiPartIdx,
                        Float_t fVal)
{
// Set value fVal for known position in the values array

Int_t    i;

    if((i = getValuePositionByIndex(uiAlgIdx, uiPartIdx)) < 0)
        Error("setValueByIndex", "Out of bounds");
    else
      aValues[i] = fVal;
}

// -----------------------------------------------------------------------------

Float_t HPidCandidate::getValueById(Int_t eAlg, Short_t nPartId) const
{
// Returns value computed by algorithm eAlg for the particle nPartId
// or 0.0 if algorithm or particle not in the arrays
#warning This implementation has to be extended to strict checking
Int_t    iPart, iAlg;

    if(((iAlg = getAlgorithmIndexById(eAlg)) < 0)
            || ((iPart = getParticleIndexById(nPartId)) < 0))
    {
      return 0.0f;
    }

    return aValues[getValuePositionByIndex(iAlg, iPart)];
}

// -----------------------------------------------------------------------------

Float_t HPidCandidate::getValueByIndex(UInt_t uiAlgIdx, UInt_t uiPartIdx) const
{
// Returns pdf-value for a specific particle and algorithm index in the values array or 0.0 if out of bound
#warning This implementation has to be extended to strict checking
  Int_t    i = getValuePositionByIndex(uiAlgIdx, uiPartIdx);
  return (i < 0 ? -1.0f : aValues[i]);
}


// -----------------------------------------------------------------------------

HCategory* HPidCandidate::buildPidCandidateCategory()
{
// Static function for making the category for the specified class name.
  
  HCategory  *pCat;
  static const Char_t* sCatName="HPidCandidate";
  HEvent     *pEvent;

    if((gHades == NULL) || ((pEvent = gHades->getCurrentEvent()) == NULL))
    {
        ::Error("buildPidCandidateCategory", "Cannot access current event");
        return NULL;
    }

    if((pCat = pEvent->getCategory(catPidCandidate)) != NULL)
      {
	((HLinearCategory*)pCat)->setDynamicObjects(kTRUE);
        return pCat;
      }

    if((pCat = new HLinearCategory(sCatName, 1000)) == NULL)
    {
        ::Error("HPidCandidate::buildPidCandidateCategory",
                    "Cannot create new category");

        return NULL;
    }
    ((HLinearCategory*)pCat)->setDynamicObjects(kTRUE);
    //This category stores a variable amount of data. Within one DST the size of the
    //array is fixed (nAlgs times nParticles) but it is NOT known at compiletime!
    //Therefor we declare this category to be dynamic in size. This implies that the
    //Clear() function of the category is called with option "C" which calls the Clear()
    //function of all HPidCandidates in the TClonesArray
    
    pEvent->addCategory(catPidCandidate, pCat, "Pid");

    return pCat;

}


// -----------------------------------------------------------------------------

Int_t HPidCandidate::calcBayesVector(Float_t fOut[],const Int_t aAlgs[], Int_t iAlgs)
{
// Calculate Bayes values for the all particles and the algorithms
// with ids from array aAlgs of size iAlgs. If iAlgs==0 all available algorithms
// are used - in this case aAlgs may be NULL. The output values are stored
// in fOut array, which size should be suitable to contain all values.
// fOut must not be NULL. The function returns the number of filled elements
// in fOut array

    if(fOut == NULL)
    {
        Error("calcBayesVector", "fOut == NULL");
        return 0;
    }

    Int_t iRelInt;

    if((iRelInt = getAlgorithmIndexById(algRelInt)) < 0)
    {
        Error("calcBayesVector", "no algRelInt in the container");
        return 0;
    }

    //Float_t *fRelInt = getValuesVectorByIndex(iRelInt);
    Int_t    i;
    Int_t    iMax = getNParticles();

    if((i = calcMergedPDFVector(fOut, aAlgs, iAlgs)) <= 0)
        return i;
    
    Float_t fSum = 0.0f;

    // calc products of joined PDFs with relative intensities
    for(i = 0; i < iMax; i++)
    {
        if(fOut[i] < 0.0)
            continue;
	
        if(getValueByIndex(iRelInt,i) < 0.0)
	  {
	    setValueByIndex(iRelInt,i,0.0);
	    Error("HPidCandidate::calcBayesVector()","Negatiev relint value detected and resetted!");
	  }
	fOut[i] *= getValueByIndex(iRelInt,i) ;

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

Int_t HPidCandidate::calcBayesVectorFromAlgSelection(Float_t fOut[], Int_t iAlg0,
                            Int_t iAlg1, Int_t iAlg2, Int_t iAlg3,
                            Int_t iAlg4, Int_t iAlg5, Int_t iAlg6,
                            Int_t iAlg7, Int_t iAlg8, Int_t iAlg9)
{
// Calculate Bayes values for the all particles and the algorithms
// given as the arguments. If all iAlg? == 0 all available algorithms
// are used. For more info see the descr. of the previous method, which
// is called internally.

Int_t aAlg[10];
Int_t i = 0;

    FILL_ARRAY();

    return calcBayesVector(fOut, aAlg, i);
}

// -----------------------------------------------------------------------------

Float_t HPidCandidate::getBayesValue(Short_t nPID,
				     const Int_t aAlgs[], Int_t iAlgs)
{
// Return the Bayes value for the give particle species nType.
// For commends to the eAlgs and iAlgs see calcBayesVect descr.
// In case of any error -1 is returned

Int_t iIdx;

    if((iIdx = getParticleIndexById(nPID)) < 0)
    {
        Error("getBayesVect", "Particle %d not in the container", nPID);
        return -1.0f;
    }

Float_t *fArr;

    if((fArr = new Float_t[getNParticles()]) == NULL)
    {
        Error("getBayesVect", "Cannot create temp array");
        return -1.0f;
    }

Float_t fVal;

    if(calcBayesVector(fArr, aAlgs, iAlgs) <= 0)
        fVal = -1.0f;
    else
        fVal = fArr[iIdx];

    delete [] fArr;

    return fVal;
}

// -----------------------------------------------------------------------------

Float_t HPidCandidate::getBayesValueFromAlgSelection(Short_t nPID, Int_t iAlg0,
						     Int_t iAlg1, Int_t iAlg2, Int_t iAlg3,
						     Int_t iAlg4, Int_t iAlg5, Int_t iAlg6,
						     Int_t iAlg7, Int_t iAlg8, Int_t iAlg9)
{
// Return the Bayes value for the give particle species nPID and the given
// set of algorithms. For more info see descr. of getBayesVect and calcBayesVect
// methods.

Int_t aAlg[10];
Int_t i = 0;

    FILL_ARRAY();

    return getBayesValue(nPID, aAlg, i);
}

// -----------------------------------------------------------------------------

Int_t HPidCandidate::calcMergedPDFVector(Float_t fOut[],
                                const Int_t aAlgs[], Int_t iAlgs) const
{
// Calculate merged propability density for the all particles and the algorithms
// with ids from array aAlgs of size iAlgs. If iAlgs==0 all available algorithms
// are used - in this case aAlgs may be NULL. The output values are stored
// in fOut array, which size should be suitable to contain all values.
// fOut must not be NULL. The function returns the number of filled elements
// in fOut array

    if(fOut == NULL)
    {
        Error("calcMergedPDFVector", "fOut == NULL");
        return 0;
    }

    UInt_t   iMax = getNParticles();
    UInt_t   i;
    Int_t    a, c, k;
    
    Float_t pdf_factor=0.0;
    
    // clear the output
    for(i = 0; i < iMax; i++)
      fOut[i] = -1.0f;
    
    if(iAlgs <= 0)
      {
        c = 0;
	
        // loop over all algorithms and choose the PDF vectors
        for(k = 0; k < (Int_t)getNAlgorithms(); k++)
        {
            if(((a = getAlgorithmIdByIndex(k)) <= LAST_COMMON_ALG)
                    || (a >= CL_ALOGRITHM_OFFSET))
            {
                continue;
            }
#warning This takes Long_t and can be done much more efficiently
	    //cout << "Algor ID is: " << a << endl;
	    
            // calc the products - changed by Tassilo - check this later!
	    /*
            af = getValuesVectorByIndex(k);

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
	    */
	    
	    for(i = 0; i < iMax; i++)
	      {
		pdf_factor=getValueByIndex(k,i);
		if(pdf_factor >= 0.0f) // do not consider negative probability densities!! They indicate void statements
		  {
		    if(fOut[i] < 0.0f)
		      fOut[i] = pdf_factor;
		    else
		      fOut[i] *= pdf_factor;
		  }
	      }
	    
            c++;
        }

        if(c <= 0)
        {
            Error("calcMergedPDFVector", "No PDF vector found");
            goto lab_Error;
        }
      }
    else
    {
        for(c = 0; c < iAlgs; c++)
        {
            if((k = getAlgorithmIndexById(aAlgs[c])) < 0)
            {
                Error("calcMergedPDFVector", "No values for algorithm %d", aAlgs[c]);
                goto lab_Error;
            }

            // calc the products - changed by Tassilo check this!
	    /*
            af = getValuesVectorByIndex(k);
	    
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
	    */ 
	    
            for(i = 0; i < iMax; i++)
	      {
		pdf_factor=getValueByIndex(k,i);
                if(pdf_factor >= 0.0f) // do not consider negative probability densities!! They indicate void statements
		  {
                    if(fOut[i] < 0.0f)
		      fOut[i] = pdf_factor;
                    else
		      fOut[i] *= pdf_factor;
		  }
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

Int_t HPidCandidate::calcMergedPDFVectorFromAlgSelection(Float_t fOut[], Int_t iAlg0,
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

    return calcMergedPDFVector(fOut, aAlg, i);
}

// -----------------------------------------------------------------------------

Float_t HPidCandidate::getMergedPDFValue(Short_t nPID,
					      const Int_t aAlgs[], Int_t iAlgs) const
{
// Return the merged propability value for the give particle species nPID.
// For commends to the eAlgs and iAlgs see calcMergedVect descr.
// In case of any error -1 is returned

Int_t iIdx;

    if((iIdx = getParticleIndexById(nPID)) < 0)
    {
        Error("getMergedPDFValue", "Particle %d not in the container", nPID);
        return -1.0f;
    }

Float_t *fArr;

    if((fArr = new Float_t[getNParticles()]) == NULL)
    {
        Error("getMergedPDFValue", "Cannot create temp array");
        return -1.0f;
    }

Float_t fVal;

    if(calcMergedPDFVector(fArr, aAlgs, iAlgs) <= 0)
        fVal = -1.0f;
    else
        fVal = fArr[iIdx];

    delete [] fArr;

    return fVal;
}

// -----------------------------------------------------------------------------

Float_t HPidCandidate::getMergedPDFValueFromAlgSelection(Short_t nPID, Int_t iAlg0,
                            Int_t iAlg1, Int_t iAlg2, Int_t iAlg3,
                            Int_t iAlg4, Int_t iAlg5, Int_t iAlg6,
                            Int_t iAlg7, Int_t iAlg8, Int_t iAlg9) const
{
// Return the merged propability value for the give particle species nPID.
// and the set of algorithms. For more info see descr. of getMergedPDFValue
// and calcMergedVect methods.

Int_t aAlg[10];
Int_t i = 0;

    FILL_ARRAY();
    cout << "Calling fobidden function!" <<endl ;
    return getMergedPDFValue(nPID, aAlg, i);
}


void HPidCandidate::Streamer(TBuffer &R__b)
{
   // Stream an object of class HPidCandidate.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      TObject::Streamer(R__b);
      R__b >> iTrackCandIndex;
      R__b >> NUM_ALGORITHMS;
      R__b >> NUM_PARTICLES;
      R__b >> NUM_VALUES;
      aAlgorithms.Streamer(R__b);
      aParticles.Streamer(R__b);
      aValues.Streamer(R__b);
      R__b >> nMomAlgIndex;
      if (R__v > 1) {
	  R__b >> flags;
      } else { flags = 0; }
      R__b.CheckByteCount(R__s, R__c, HPidCandidate::IsA());
   } else {
      R__c = R__b.WriteVersion(HPidCandidate::IsA(), kTRUE);
      TObject::Streamer(R__b);
      R__b << iTrackCandIndex;
      R__b << NUM_ALGORITHMS;
      R__b << NUM_PARTICLES;
      R__b << NUM_VALUES;
      aAlgorithms.Streamer(R__b);
      aParticles.Streamer(R__b);
      aValues.Streamer(R__b);
      R__b << nMomAlgIndex;
      R__b << flags;
      R__b.SetByteCount(R__c, kTRUE);
   }
}



