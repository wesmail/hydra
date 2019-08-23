// @(#)$Id: hpidparticlefiller.h,v 1.2 2002-11-20 17:26:37 jaskula Exp $
//*-- Author : Marcin Jaskula 30/10/2002

#ifndef HPIDPARTICLEFILLER_H
#define HPIDPARTICLEFILLER_H

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// HPidParticleFiller                                                         //
//                                                                            //
// Filler of HPidParticle category from the data in HPidCandidate             //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include "hreconstructor.h"

// -----------------------------------------------------------------------------

class HCategory;
class HIterator;
class HPidCandidate;
class HPidParticle;
class HPidTrackCand;
class HKickTrack;
class TMethodCall;

// -----------------------------------------------------------------------------

class HPidParticleFiller : public HReconstructor
{
public:
                        HPidParticleFiller(Option_t par[] = "");

                        HPidParticleFiller(Text_t name[], Text_t title[],
                                            Option_t par[] = "");

                       ~HPidParticleFiller(void);

    // -------------------------------------------------------------------------

    virtual Int_t       execute(void);
    virtual Bool_t      init(void);
    virtual Bool_t      finalize(void)                  { return kTRUE;        }

    // -------------------------------------------------------------------------

    void                setCheckFunCmp(Long_t(*p)(const HPidCandidate *, Int_t))
                                                        { pCheckFun = p;       }
    void                setCheckFun(void *pFun);

    static Long_t       interactiveFun(const HPidCandidate *pCand, Int_t iIdx);

    // -------------------------------------------------------------------------

    enum EnumOperators_t
    {
        kEqual = 0,     // ==
        kLower,         // <
        kLowerEqual,    // <=
        kGreater,       // >
        kGreaterEqual   // >=
    };

    // -------------------------------------------------------------------------

    void                setAlgorithm(Int_t i)           { iAlgorithm = i;      }
    Int_t               getAlgorithm(void) const        { return iAlgorithm;   }
    void                setThreshold(Float_t f)         { fThreshold = f;      }
    Float_t             getThreshold(void) const        { return fThreshold;   }
    Bool_t              setOperator(const Char_t *pOper);
    void                setOperator(EnumOperators_t e)  { eOperator = e;       }
    static Int_t        findOperator(const Char_t *pOper);
    const Char_t*       getOperator(void) const;
    void                setCheckMax(Bool_t b)           { bCheckMax = b;       }
    Bool_t              getCheckMax(void) const         { return bCheckMax;    }
    void                setIncludeFakes(Bool_t b)       { bIncludeFakes = b;   }
    Bool_t              getIncludeFakes(void) const     { return bIncludeFakes;}

    void                setUseMassExp(Bool_t b)         { bUseMassExp = b;     }
    Bool_t              getUseMassExp(void) const       { return bUseMassExp;  }

    // -------------------------------------------------------------------------

    Bool_t              setCondition(Int_t iAlg, Float_t fThr,
                                const Char_t *pOper, Bool_t bMax = kFALSE,
                                Bool_t bInclFakes = kTRUE);

    Bool_t              setCondition(Int_t iAlg, Float_t fThr,
                                EnumOperators_t eOper, Bool_t bMax = kFALSE,
                                Bool_t bInclFakes = kTRUE);

    Bool_t              setPdfCondition(Int_t iAlg, Float_t fThr,
                                Bool_t bMax = kFALSE)
                                {
                                    return setCondition(iAlg, fThr,
                                                kGreaterEqual, bMax, kTRUE);
                                }

    Bool_t              setCLCondition(Int_t iAlg, Float_t fThr,
                                Bool_t bMax = kFALSE)
                                {
                                    return setCondition(iAlg, fThr,
                                                kLowerEqual, bMax, kFALSE);
                                }

    // -------------------------------------------------------------------------

    virtual void        setParameters(Option_t par[]);

    Int_t               getDebug(void) const            { return iDebug;       }
    void                setDebug(Int_t i)               { iDebug = i;          }

    // -------------------------------------------------------------------------

    virtual void        Print(Option_t* option) const   { print();             }
    virtual void        print(void) const;

    // -------------------------------------------------------------------------

    virtual Bool_t          buildOutputCategory(void);

    virtual HPidParticle*   getNextSlot(void);

    virtual Int_t           checkCandidate(HPidCandidate *pCand);
    virtual Bool_t          checkParticle(HPidCandidate *pCand, Int_t iIdx);

    virtual HPidParticle*   makeParticle(HPidCandidate *pCand,
                                    HKickTrack *pKickTrack, Int_t iIdx);

    // -------------------------------------------------------------------------

    static Bool_t       checkParticlePar(HPidCandidate *pCand, Int_t iIdx,
                                Int_t iAlg, Float_t fThr,
                                const Char_t *pOper, Bool_t bMax = kFALSE,
                                Bool_t bFakesIncl = kTRUE);

    static Bool_t       checkParticlePar(HPidCandidate *pCand, Int_t iIdx,
                                Int_t iAlg, Float_t fThr,
                                EnumOperators_t eOper, Bool_t bMax = kFALSE,
                                Bool_t bFakesIncl = kTRUE);

    // -------------------------------------------------------------------------

protected:

    HCategory          *pInputCat;      // category of HPidCandidate
    HIterator          *pitInput;       // iterator on the pInputCat category

    HCategory          *pOutCat;        // category of HPidParticle

    HCategory          *pTrackCandCat;  // category of HPidTrackCand
    HCategory          *pKickTrackCat;  // category of HKickTrack

    // -------------------------------------------------------------------------

    Long_t            (*pCheckFun)(const HPidCandidate *, Int_t);
    TMethodCall        *pMethodCall;

    // -------------------------------------------------------------------------

    Int_t               iAlgorithm;     // algoritm used in checking
    Float_t             fThreshold;     // threshold for checking
    EnumOperators_t     eOperator;      // checking operator
    Bool_t              bCheckMax;      // the val. must be max (see descr.)
    Bool_t              bIncludeFakes;  // should fakes be included into calc.

    Bool_t              bUseMassExp;    // if massExp should fill TLorentzVector

    // -------------------------------------------------------------------------

    Int_t               iDebug;         // debug level

    // -------------------------------------------------------------------------

    void                setDefault(void);

    // -------------------------------------------------------------------------

    static HPidParticleFiller *pCurPartFiller;

    // -------------------------------------------------------------------------

    ClassDef(HPidParticleFiller, 0)     // Filler of HPidParticle
};

// -----------------------------------------------------------------------------

#endif //HPIDPARTICLEFILLER_H
