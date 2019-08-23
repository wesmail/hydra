// @(#)$Id: hpidcandidate.h,v 1.8 2006-08-16 16:13:35 halo Exp $
//*-- Author : Marcin Jaskula 27/07/2002
// Modified  : Marcin Jaskula 06/09/2002
// Modified  : Marcin Jaskula 16/09/2002
//           - now this is an abstract class
// Modified  : Marcin Jaskula 31/10/2002
//           - buildPidCandidateCategory moved from global to static function
// Modified  : Marcin Jaskula 01/12/2002
//           - new method getPidCandidate()
// Modified  : Marcin Jaskula 18/04/2003
//           - functions for calculation of the merged PDF and Bayes added

#ifndef HPIDCANDIDATE_H
#define HPIDCANDIDATE_H

/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
// HPidCandidate                                                               //
//                                                                             //
// Set of all values returned by active PID algorithms                         //
// This class stores PDF values, CL values and relative intensities of the     //
// particle species activated by the user. There is one coloumn per algorithm  //
// that holds the PDF values and one row that holds the CL values (unless      //
// switched off in the parameters of hpidreconstructor. There is one addi-     //
// tional coloumn holding the relative intensities of the particles which are  //
// required to compute posterior probabilities according to Bayes theorem.     //
// This can also be switched off when doing PID via "Hardcut" or "BananaCut"   //
// method.                                                                     //
// The number of elements in each coloumn is the number of particles activated //
// by the user in the configuration of hpidreconstructor. (The releveant data  //
// member in the hpidreconstructor is "aParticles").                           //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#include <TObject.h>
#include "piddef.h"

#include <TArrayF.h>
#include <TArrayI.h>
#include <TArrayS.h>

// -----------------------------------------------------------------------------

class HCategory;
class HPidTrackCand;

// -----------------------------------------------------------------------------

class HPidCandidate : public TObject
{
 public:
                        HPidCandidate(void);
                        HPidCandidate(Short_t numpart, Int_t numalgs, Int_t candIndex, Int_t momAlgIndex);
                       ~HPidCandidate(void);

    // -------------------------------------------------------------------------

    //Overwrites TObject::Clear()		       
    virtual void        Clear(Option_t * /* option */ = "");

    //Initialize arrays 
    void                Reset(void);

    virtual void        Print(Option_t* option = "") const     { print();     }
    Int_t               print(void) const;

    // -------------------------------------------------------------------------

    //getters and setters won't be documented unless required
    UInt_t              getNParticles(void) const { return NUM_PARTICLES;}
    UInt_t              getNAlgorithms(void) const { return NUM_ALGORITHMS;}
    UInt_t              getNValues(void) const { return NUM_VALUES;}

    // -------------------------------------------------------------------------

    void                setTrackCandIndex(Short_t nId) { iTrackCandIndex = nId;}
    Short_t             getTrackCandIndex(void) const  { return iTrackCandIndex;}

    //Return the HPidTrackCandidate object that was the origin of the HPidCandidate
    HPidTrackCand*      getTrackCandidate(HCategory *pCat = NULL) const;


    // -------------------------------------------------------------------------
    //Set algorithm Id eAlg at uiPos. Arrays bounds are checked
    void                setAlgorithmIdByIndex(UInt_t uiPosIndex, Short_t iAlgId);

    //Set full vector of algorithm IDs
    void                setAlgorithmIds(Short_t aiAlgIds[]);
    
    //Get algorithm Id from the array of algorithms at position uiPos.
    Int_t               getAlgorithmIdByIndex(UInt_t uiPosIndex) const;

    //Complementary to above function
    Int_t               getAlgorithmIndexById(Int_t eAlgId) const;

    //Same functions as for algorithm ID
    void                setParticleIdByIndex(UInt_t uiPosIndex, Short_t nPartIndex);
    void                setParticleIds(Short_t anPart[], Int_t nrOfParticles );
    Short_t             getParticleIdByIndex(UInt_t uiPosIndex) const;
    Int_t               getParticleIndexById(Short_t nPartId) const;


    //Same functions as for algorithm ID
    void                setValueById(Int_t eAlgId, Short_t nPartId, Float_t fVal);
    void                setValueByIndex(UInt_t uiAlgIndex, UInt_t uiPartIndex, Float_t fVal);
    Float_t             getValueById(Int_t eAlgId, Short_t nPartId) const;
    Float_t             getValueByIndex(UInt_t uiAlgIndex, UInt_t uiPartIndex) const;

    // -------------------------------------------------------------------------

    Float_t*            getValuesVectorByIndex(UInt_t uiAlgIndex);
    Int_t               getValuePositionByIndex(UInt_t uiAlgIndex, UInt_t uiPartIndex) const;
    //Float_t*            getValuesVectorById(UInt_t uiAlgID) const;
    Int_t               getValuePositionById(UInt_t uiAlgID, UInt_t uiPartID) const;

    
    const TArrayS*      getAlgorithmIds(void) {return &aAlgorithms;}
    const TArrayS*      getParticleIds(void) {return &aParticles;}
    const TArrayF*      getValues(void) {return &aValues;}
    

    Int_t               getMomAlg(void) const { return nMomAlgIndex; }

    // -------------------------------------------------------------------------

    static HCategory*   buildPidCandidateCategory();

    // -------------------------------------------------------------------------
    //Calculate Bayes values for the all particles and the algorithms specified
    Int_t       calcBayesVector(Float_t fOut[],
				const Int_t aAlgs[]=0, Int_t iAlgs=0);

    //Calculate Bayes values for the all particles and the algorithms selected by hand
    Int_t       calcBayesVectorFromAlgSelection(Float_t fOut[], Int_t iAlg0 = 0,
						Int_t iAlg1 = 0, Int_t iAlg2 = 0, Int_t iAlg3 = 0,
						Int_t iAlg4 = 0, Int_t iAlg5 = 0, Int_t iAlg6 = 0,
						Int_t iAlg7 = 0, Int_t iAlg8 = 0, Int_t iAlg9 = 0);

    //compute probability for one specific particle ID using the array of algorithms aAlgs
    Float_t     getBayesValue(Short_t nType, const Int_t aAlgs[], Int_t iAlgs);

    //compute probability for one specific particle ID using the listed algorithms
    Float_t     getBayesValueFromAlgSelection(Short_t nType, Int_t iAlg0 = 0,
					      Int_t iAlg1 = 0, Int_t iAlg2 = 0, Int_t iAlg3 = 0,
					      Int_t iAlg4 = 0, Int_t iAlg5 = 0, Int_t iAlg6 = 0,
					      Int_t iAlg7 = 0, Int_t iAlg8 = 0, Int_t iAlg9 = 0);

    // -------------------------------------------------------------------------
    //Same functions as above but without usage of prior probabilities (relative intensities)
    Int_t       calcMergedPDFVector(Float_t fOut[], const Int_t aAlgs[], Int_t iAlgs) const;

    Int_t       calcMergedPDFVectorFromAlgSelection(Float_t fOut[], Int_t iAlg0 = 0,
                    Int_t iAlg1 = 0, Int_t iAlg2 = 0, Int_t iAlg3 = 0,
                    Int_t iAlg4 = 0, Int_t iAlg5 = 0, Int_t iAlg6 = 0,
                    Int_t iAlg7 = 0, Int_t iAlg8 = 0, Int_t iAlg9 = 0) const;

    Float_t     getMergedPDFValue(Short_t nPID,  const Int_t aAlgs[], Int_t iAlgs) const;

    Float_t     getMergedPDFValueFromAlgSelection(Short_t nPID, Int_t iAlg0 = 0,
                    Int_t iAlg1 = 0, Int_t iAlg2 = 0, Int_t iAlg3 = 0,
                    Int_t iAlg4 = 0, Int_t iAlg5 = 0, Int_t iAlg6 = 0,
                    Int_t iAlg7 = 0, Int_t iAlg8 = 0, Int_t iAlg9 = 0) const;

    // -------------------------------------------------------------------------

protected:

    Short_t             iTrackCandIndex; // index of the track candidate in alg. cat.

    // -------------------------------------------------------------------------
    
    UInt_t NUM_ALGORITHMS;      //Number of active alorithms
    UInt_t NUM_PARTICLES;       //Number of active partcles
    UInt_t NUM_VALUES;          //Number of values in the array of PDFs, CLs and RELINTs
    TArrayS             aAlgorithms; //Array storing the list of active algorithms
    TArrayS             aParticles;  //Array storing the list of active particless
    TArrayF             aValues;     //Array storing the list of values (PDFs and CLs for each algorithm
                                     //and the relative intensities of the particles defined in aParticles

    Int_t               nMomAlgIndex; //Index specifying which momentum reconstructor has been used

    ClassDef(HPidCandidate, 1)  // Set of all values returned by the algorithms
};

// -----------------------------------------------------------------------------

inline Int_t HPidCandidate::getValuePositionByIndex(UInt_t uiAlgIndex, UInt_t uiPartIndex) const
{
  //returns the linear index in the data array belonging to the algorithm index and the particle index
    if((uiAlgIndex >= getNAlgorithms()) || (uiPartIndex >= getNParticles()))
      {
        Error("getValuePositionByIndex", "Out of bounds (%d / %d) (%d / %d)",
	      uiAlgIndex, getNAlgorithms(), uiPartIndex, getNParticles());
        return -1;
      }
    if(uiAlgIndex* getNParticles() + uiPartIndex >= getNValues())
      {
	Error("HPidCandidate::getValuePositionByIndex","Mismatch in value array size!");
	return -1;
      }
    //cout << "returning: " << uiAlgIndex * getNParticles() + uiPartIndex << endl;
    return uiAlgIndex * getNParticles() + uiPartIndex;
}

// -----------------------------------------------------------------------------

inline Float_t* HPidCandidate::getValuesVectorByIndex(UInt_t uiAlgIndex)
{
// Get vector of values for the specific algorithm INDEX

    if(uiAlgIndex >= getNAlgorithms())
    {
        Error("getValuesVectorByIndex", "Out of bounds (%d / %d)",
                uiAlgIndex, getNAlgorithms());

        return NULL;
    }
    
    return aValues.GetArray() + uiAlgIndex * getNParticles();
}
#warning It would be nice to have this function implemented HPidCandidate.etValuesVectorById() 
/*
  inline const Float_t* HPidCandidate::getValuesVectorById(UInt_t uiAlgoID) const
  {
  Int_t algoIndex = getAlgorithmIndexById(uiAlgoID);
  if(algoIndex<0)
  {
  Error("getValuesVectorById","No index found for algorithm %d",uiAlgoID);
  return NULL;
  }
  
  // Get vector of values for the specific algorithm ID
  return aValues.GetArray() + algoIndex * getNParticles();
  }
*/

inline Int_t HPidCandidate::getValuePositionById(UInt_t uiAlgID, UInt_t uiPartID) const
{
  //returns the linear index in the data array belonging to the algorithm ID and the particle ID
  UInt_t algoIndex = getAlgorithmIndexById(uiAlgID);
  UInt_t particleIndex = getParticleIndexById(uiPartID);
  
  if(particleIndex<0 || algoIndex<0)
    {
        Error("getValuePositionById", "No algorithm found for algorithm/particle: %d/%d",uiAlgID, uiPartID);
        return -1;
    }

    return algoIndex * getNParticles() + particleIndex;
}

#endif //HPIDCANDIDATE_H
