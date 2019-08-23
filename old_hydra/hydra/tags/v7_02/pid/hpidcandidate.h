// @(#)$Id: hpidcandidate.h,v 1.6 2003-10-23 18:54:20 jaskula Exp $
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

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// HPidCandidate                                                              //
//                                                                            //
// Set of all values returned by algorithms                                   //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include <TObject.h>
#include "piddef.h"

#include <TArrayF.h>

// -----------------------------------------------------------------------------

class HCategory;
class HPidTrackCand;

// -----------------------------------------------------------------------------

class HPidCandidate : public TObject
{
public:
                        HPidCandidate(void);
                       ~HPidCandidate(void);

    // -------------------------------------------------------------------------

    virtual void        Clear(Option_t * /* option */ = "");
    virtual void        Print(Option_t* option = "") const     { print();     }
    Int_t               print(void) const;

    // -------------------------------------------------------------------------

    virtual UInt_t      getNParticles(void) const;
    virtual UInt_t      getNAlgorithms(void) const;

    // -------------------------------------------------------------------------

    void                setCandidateId(Short_t nId) { nCandidate = nId;        }
    Short_t             getCandidateId(void) const  { return nCandidate;       }
    HPidTrackCand*      getCandidate(HCategory *pCat = NULL) const;

    HPidTrackCand*      getTrackCand(HCategory *pCat = NULL) const
                                            { return getCandidate(pCat);       }

    // -------------------------------------------------------------------------

    void                setAlgorithm(UInt_t uiPos, Int_t iAlg);
    void                setAlgorithms(Int_t aiAlg[]);
    Int_t               getAlgorithm(UInt_t uiPos) const;
    Int_t               getAlgorithmIndex(Int_t eAlg) const;

    void                setParticle(UInt_t uiPos, Short_t nPart);
    void                setParticles(Short_t anPart[]);
    Short_t             getParticle(UInt_t uiPos) const;
    Int_t               getParticleIndex(Short_t nPartId) const;

    void                setPidObjectId_Idx(UInt_t uiPos, Short_t nId);
    void                setPidObjectId_Alg(Int_t eAlg, Short_t n);
    Short_t             getPidObjectId_Idx(UInt_t uiPos) const;
    Short_t             getPidObjectId_Alg(Int_t ePid) const;

    void                setValue(Int_t eAlg, Short_t nPartId,
                                Float_t fVal);
    void                setValue_Idx(UInt_t uiAlgIdx, UInt_t uiPartIdx,
                                Float_t fVal);
    Float_t             getValue(Int_t eAlg, Short_t nType) const;
    Float_t             getValue_Idx(UInt_t uiAlgIdx, UInt_t uiPartIdx) const;

    // -------------------------------------------------------------------------

    Float_t*            getValuesVector(UInt_t uiAlg) const;
    Int_t               getValueIndex(UInt_t uiAlg, UInt_t uiPart) const;

    virtual Int_t*      getAlgorithms(void) const;
    virtual Short_t*    getPidObjIds(void) const;
    virtual Short_t*    getParticles(void) const;
    virtual Float_t*    getValues(void) const;

    // -------------------------------------------------------------------------

    static HCategory*   buildPidCandidateCategory(UInt_t iAlg, UInt_t iPart);
    static HCategory*   buildPidCandidateCategory(const char sClassName[]);

    static HPidCandidate* getPidCandidate(Int_t iIdx, HCategory *pCat = NULL);

    // -------------------------------------------------------------------------

    Int_t       calcBayesVect(Float_t fOut[],
                                    const Int_t aAlgs[], Int_t iAlgs) const;
    Int_t       calcBayes(Float_t fOut[], Int_t iAlg0 = 0,
                    Int_t iAlg1 = 0, Int_t iAlg2 = 0, Int_t iAlg3 = 0,
                    Int_t iAlg4 = 0, Int_t iAlg5 = 0, Int_t iAlg6 = 0,
                    Int_t iAlg7 = 0, Int_t iAlg8 = 0, Int_t iAlg9 = 0) const;

    Float_t     getBayesVect(Short_t nType,
                                    const Int_t aAlgs[], Int_t iAlgs) const;
    Float_t     getBayes(Short_t nType, Int_t iAlg0 = 0,
                    Int_t iAlg1 = 0, Int_t iAlg2 = 0, Int_t iAlg3 = 0,
                    Int_t iAlg4 = 0, Int_t iAlg5 = 0, Int_t iAlg6 = 0,
                    Int_t iAlg7 = 0, Int_t iAlg8 = 0, Int_t iAlg9 = 0) const;

    // -------------------------------------------------------------------------

    Int_t       calcMergedVect(Float_t fOut[],
                                    const Int_t aAlgs[], Int_t iAlgs) const;
    Int_t       calcMerged(Float_t fOut[], Int_t iAlg0 = 0,
                    Int_t iAlg1 = 0, Int_t iAlg2 = 0, Int_t iAlg3 = 0,
                    Int_t iAlg4 = 0, Int_t iAlg5 = 0, Int_t iAlg6 = 0,
                    Int_t iAlg7 = 0, Int_t iAlg8 = 0, Int_t iAlg9 = 0) const;

    Float_t     getMergedVect(Short_t nType,
                                    const Int_t aAlgs[], Int_t iAlgs) const;
    Float_t     getMerged(Short_t nType, Int_t iAlg0 = 0,
                    Int_t iAlg1 = 0, Int_t iAlg2 = 0, Int_t iAlg3 = 0,
                    Int_t iAlg4 = 0, Int_t iAlg5 = 0, Int_t iAlg6 = 0,
                    Int_t iAlg7 = 0, Int_t iAlg8 = 0, Int_t iAlg9 = 0) const;

    // -------------------------------------------------------------------------

protected:

    Short_t             nCandidate; // index of the track candidate in alg. cat.

    // -------------------------------------------------------------------------

    ClassDef(HPidCandidate, 1)  // Set of all values returned by the algorithms
};

// -----------------------------------------------------------------------------

inline Int_t HPidCandidate::getValueIndex(UInt_t uiAlg, UInt_t uiPart) const
{
    if((uiAlg >= getNAlgorithms()) || (uiPart >= getNParticles()))
    {
        Error("getValueIndex", "Out of bounds (%d / %d) (%d / %d)",
                    uiAlg, getNAlgorithms(), uiPart, getNParticles());
        return -1;
    }

    return uiAlg * getNParticles() + uiPart;
}

// -----------------------------------------------------------------------------

inline Float_t* HPidCandidate::getValuesVector(UInt_t uiAlg) const
{
// Get vector for values for the specific algorithm INDEX

    if(uiAlg >= getNAlgorithms())
    {
        Error("getValuesVector", "Out of bounds (%d / %d)",
                uiAlg, getNAlgorithms());

        return NULL;
    }

    return getValues() + uiAlg * getNParticles();
}

#endif //HPIDCANDIDATE_H
