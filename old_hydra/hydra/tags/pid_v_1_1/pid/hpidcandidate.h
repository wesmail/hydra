// @(#)$Id: hpidcandidate.h,v 1.4 2002-12-03 14:30:38 jaskula Exp $
//*-- Author : Marcin Jaskula 27/07/2002
// Modified  : Marcin Jaskula 06/09/2002
// Modified  : Marcin Jaskula 16/09/2002
//           - now this is an abstract class
// Modified  : Marcin Jaskula 31/10/2002
//           - buildPidCandidateCategory moved from global to static function
// Modified  : Marcin Jaskula 01/12/2002
//           - new method getPidCandidate()

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
    virtual void        Print(void)                 { print();                 }
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

    Float_t*            getValuesVector(UInt_t uiPartIdx) const;
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
