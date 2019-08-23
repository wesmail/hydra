// @(#)$Id: hpidparticlefiller.h,v 1.10 2005-01-11 11:06:46 halo Exp $
//*-- Author : Marcin Jaskula 30/10/2002
//  Modified : Marcin Jaskula 01/12/2002
//             filling artificial particle when no particle from the track
//             created.

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
#include "TArrayI.h"
#include "TArrayF.h"

// -----------------------------------------------------------------------------

class HCategory;
class HIterator;
class HPidCandidate;
class HPidParticle;
class HPidTrackCand;
class HPidTrackData;
class TMethodCall;
class HPidParticleSim;

// -----------------------------------------------------------------------------

class HPidParticleFiller : public HReconstructor
{
public:
                        HPidParticleFiller(Option_t par[]="");
                        HPidParticleFiller(Text_t name[], Text_t title[],
					   Option_t par[] = "");


                       ~HPidParticleFiller(void);

    // -------------------------------------------------------------------------

    virtual Int_t       execute(void);
    virtual Bool_t      reinit(void);
    virtual Bool_t      init(void);
    virtual Bool_t      finalize(void)                  { return kTRUE;        }

    // -------------------------------------------------------------------------
    /*
    void                setCheckFunCmp(Long_t(*p)
                                (const HPidCandidate *, Int_t, Float_t *))
                                                        { pCheckFun = p;       }
    void                setCheckFun(void *pFun);

    static Long_t       interactiveFun(const HPidCandidate *pCand, Int_t iIdx,
                                Float_t *pfTestVal = NULL);
    */
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
    /*
    void                setThreshold(Float_t f)         { fThreshold = f;      }
    Float_t             getThreshold(void) const        { return fThreshold;   }
    
    void                setCheckMax(Bool_t b)           { bCheckMax = b;       }
    Bool_t              getCheckMax(void) const         { return bCheckMax;    }
    void                setIncludeFakes(Bool_t b)       { bIncludeFakes = b;   }
    Bool_t              getIncludeFakes(void) const     { return bIncludeFakes;}

    void                setUseMassExp(Bool_t b)         { bUseMassExp = b;     }
    Bool_t              getUseMassExp(void) const       { return bUseMassExp;  }

    void                setMakeArtPart(Bool_t b)        { bMakeArtPart = b;    }
    Bool_t              getMakeArtPart(void) const      { return bMakeArtPart; }

    Bool_t              getFillKine(void) const         { return bFillKine;    }
    void                setFillKine(Bool_t b = kTRUE)   { bFillKine = b;       }
    */
    UInt_t              getProcessMask(void) const      { return iProcessMask; }
    void                setProcessMask(UInt_t i)        { iProcessMask = i;    }


    // -------------------------------------------------------------------------

    virtual void        setParameters(Option_t par[]);

    Int_t               getDebug(void) const            { return iDebug;       }
    void                setDebug(Int_t i)               { iDebug = i;          }

    // -------------------------------------------------------------------------

    void                addAlgorithm(Int_t iAId);
    void                removeAlgorithm(Int_t iAId);

    // -------------------------------------------------------------------------

    virtual void        Print(Option_t* option = "") const     { print();     }
    virtual void        print(void) const;

    // -------------------------------------------------------------------------

    virtual Bool_t          buildOutputCategory(Bool_t makeSimCategory = kFALSE);

    virtual HPidParticle*   getNextSlot(void);

    virtual Int_t           checkCandidate(HPidCandidate *pCand);

    virtual HPidParticle*   makeParticle(const HPidCandidate *pCand,Float_t fTestValue = -1.0f);

    // -------------------------------------------------------------------------


    // -------------------------------------------------------------------------

protected:

    HCategory          *pInputCat;      // category of HPidCandidate
    HIterator          *pitInput;       // iterator on the pInputCat category

    HCategory          *pOutCat;        // category of HPidParticle

    HCategory          *pTrackCandCat;  // category of HPidTrackCand
    HCategory          *pKickTrackCat;  // category of HKickTrack

    // -------------------------------------------------------------------------

    //Long_t            (*pCheckFun)(const HPidCandidate *, Int_t, Float_t *);
    TMethodCall        *pMethodCall;

    // -------------------------------------------------------------------------

    Int_t               iAlgorithm;     // algoritm used in checking
    TArrayI             aAlgorithms;    // list of algorithms for Bayes and Merg
    //Float_t             fThreshold;     // threshold for checking
    //EnumOperators_t     eOperator;      // checking operator
    Bool_t              bCheckMax;      // the val. must be max (see descr.)
    Bool_t              bIncludeFakes;  // should fakes be included into calc.
    Bool_t              bMakeSimCategory; // should the sim or real category be made
    Bool_t              bUseMassExp;    // if massExp should fill TLorentzVector

    //Bool_t              bMakeArtPart;   // create artificial particles

    Bool_t              bFillKine;      // fill kine in common tracks
    UInt_t              iProcessMask;   // mask CommonTrack::iProcess

    TString             sOutClass;      // name of the output class
    //TClass             *pOutClass;      // pointe of the OutClass::Class()

    TArrayF             aCandValues;    // array for the candidate values
    Int_t               iMomAlg;       // algorithm used for momentum reconstruction of this particle
    // -------------------------------------------------------------------------

    Bool_t              bIsOutSim;
    Bool_t              bIsOutHit;

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
