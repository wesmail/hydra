// @(#)$Id: hpidalgorithm.h,v 1.4 2005-01-11 11:06:41 halo Exp $
//*-- Author : Marcin Jaskula 27/07/2002
//*-- Modified : Marcin Jaskula 11/10/2002
//               - bMerge flag added
//*-- Modified : Marcin Jaskula 29/10/2002
//               - calculatePDF and calculateCL added

#ifndef HPIDALGORITHM_H
#define HPIDALGORITHM_H

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// HPidAlgorithm                                                        //
//                                                                      //
// Abstract class for all PID algorithms.                               //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "TObject.h"
#include "TString.h"
#include "piddef.h"

// -----------------------------------------------------------------------------

class HPidReconstructor;
class HPidTrackCand;

// -----------------------------------------------------------------------------

class HPidAlgorithm : public TObject
{
public:

                        HPidAlgorithm(void);
                        HPidAlgorithm(TString sName, Int_t iAlgId);
                        HPidAlgorithm(TString sName, Int_t iAlgId,
                                        Float_t fWeight);

                       ~HPidAlgorithm(void);

    // -------------------------------------------------------------------------

    HPidReconstructor*  getReconstructor(void) const { return pRec;            }

    const TString&      getName(void) const          { return sName;           }

    Int_t               getAlgId(void) const         { return iAlgId;          }

    Float_t             getWeight(void) const        { return fWeight;         }
    void                setWeight(Float_t f)         { fWeight = f;            }

    Bool_t              getMerge(void) const         { return bMerge;          }
    void                setMerge(Bool_t b)           { bMerge = b;             }

    // -------------------------------------------------------------------------

    virtual Bool_t      init(void);
    virtual Bool_t      reinit(void);
    virtual Bool_t      finalize(void);

    virtual Bool_t      execute(HPidTrackCand *pTrack,
                                    Float_t afReturnPDF[], Float_t afReturnCL[],
                                    Short_t &nCatIndex);
    virtual Bool_t      calculatePDF(HPidTrackCand *pTrack,
                                    Float_t afReturn[], Short_t &nCatIndex);
    virtual Bool_t      calculateCL(HPidTrackCand *pTrack,
                                    Float_t afReturn[], Short_t &nCatIndex);

    // -------------------------------------------------------------------------

    virtual void        print(void) const;
    virtual void        Print(Option_t* option = "") const     { print();     }

    // -------------------------------------------------------------------------

protected:

    HPidReconstructor  *pRec;       // pointer to the reconstructor
    TString             sName;      // name of the algorithm
    Int_t               iAlgId;     // unique id of the algorithm
    Float_t             fWeight;    // weight of the values from algorithm
    Bool_t              bMerge;     // if algorithm should be used in merging

    // -------------------------------------------------------------------------

    void                setReconstructor(HPidReconstructor *pRec)
                                                    { this->pRec = pRec;       }

    friend class HPidReconstructor;

    // -------------------------------------------------------------------------

    ClassDef(HPidAlgorithm, 0)  // Abstract class for all PID algorithms
};

// -----------------------------------------------------------------------------

#endif //HPIDALGORITHM_H
