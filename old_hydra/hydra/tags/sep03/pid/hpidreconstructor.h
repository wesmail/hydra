// @(#)$Id: hpidreconstructor.h,v 1.4 2003-06-05 14:41:06 jaskula Exp $
//*-- Author : Marcin Jaskula 27/07/2002
//  Modified : Marcin Jaskula 18/03/2003
//           - get rid of storing the merged values in the HPidCandidates
//             and the interface for it

#ifndef HPIDRECONSTRUCTOR_H
#define HPIDRECONSTRUCTOR_H

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// HPidReconstructor                                                          //
//                                                                            //
// Set of all PID algorithms                                                  //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include "hreconstructor.h"
#include "piddef.h"

#include "hpidcandidate.h"

#include <TList.h>

// -----------------------------------------------------------------------------

class HPidAlgorithm;
class HCategory;
class HIterator;
class HPidCandidate;
class HPidTrackCand;
class HPidReconstructorPar;

// -----------------------------------------------------------------------------

#define DEFAULT_OPTIONS  "PDF,MERGE,CL"

// -----------------------------------------------------------------------------

class HPidReconstructor : public HReconstructor
{
public:
                        HPidReconstructor(Option_t par[] = DEFAULT_OPTIONS);
                        HPidReconstructor(Text_t name[], Text_t title[],
                                        Option_t par[] = DEFAULT_OPTIONS);

                       ~HPidReconstructor(void);

    // -------------------------------------------------------------------------

    virtual Int_t       execute(void);
    virtual Bool_t      init(void);
    virtual Bool_t      reinit(void);
    virtual Bool_t      finalize(void);

    // -------------------------------------------------------------------------

    Bool_t              addAlgorithm(HPidAlgorithm *pAlg);

    HPidAlgorithm*      getAlgorithm(const TString &sName) const;
    HPidAlgorithm*      getAlgorithm(EnumPidAlgorithm_t eId) const;

    Bool_t              removeAlgorithm(HPidAlgorithm *pAlg);
    Bool_t              removeAlgorithm(const TString &sName);
    Bool_t              removeAlgorithm(EnumPidAlgorithm_t eId);

    Int_t               algorithmsNumber(void) const
                                        { return pAlgorithms->GetSize();       }

    // -------------------------------------------------------------------------

    Short_t             getParticleId(Int_t iPos) const;
    Int_t               getParticleIndex(Short_t nType) const;
    Int_t               addParticleId(Short_t nType);
    void                setParticleId(Int_t iPos, Short_t nType);

    void                setParticleIds(Short_t aIds[], Int_t iSize);
    void                setDefaultParticleIds(void);

    Int_t               particlesNumber(void) const     { return iParticles;   }

    // -------------------------------------------------------------------------

    void                setParameters(Option_t par[]);

    Bool_t              getPDF(void) const              { return bPDF;         }
    void                setPDF(Bool_t b = kTRUE)        { bPDF = b;            }
    Bool_t              getCL(void) const               { return bCL;          }
    void                setCL(Bool_t b = kTRUE)         { bCL = b;             }
    Int_t               getDebug(void) const            { return iDebug;       }
    void                setDebug(Int_t i)               { iDebug = i;          }

    // -------------------------------------------------------------------------

    virtual void        Print(Option_t* option) const   { print();             }
    virtual void        print(void) const;

    // -------------------------------------------------------------------------

    static void         normalize(Float_t af[], UInt_t iSize);

    // -------------------------------------------------------------------------

protected:

    static const Int_t  kMaxParticles = 100;

    TList              *pAlgorithms;    // list of all algorithms
    TIterator          *pitList;        // iterator over the list of the algs

    Bool_t              bInDelete;      // flag used during removing

    HCategory          *pInputCat;      // category of HPidTrackCand
    HIterator          *pitInput;       // iterator on the pInputCat category

    HCategory          *pOutCat;        // category of HPidCandidate

    Short_t             aParticles[kMaxParticles];  // ids of particles
    Int_t               iParticles;     // number of active particles

    TClass             *pClass;         // pointer to TClass for the output

    Bool_t              bInitOk;        // set kTRUE if last init was ok

    HPidReconstructorPar *pParams;      // pointer to container with params

    Bool_t              bPDF;           // calculate PDF
    Bool_t              bCL;            // calculate CL
    Int_t               iDebug;         // level of debug informations

    Int_t               iVectPerAlg;    // number of vectors for one alg.

    // -------------------------------------------------------------------------

    void                setDefault(void);

    HPidCandidate*      getNextSlot(void);
    virtual void        calculateRelInts(const HPidTrackCand *pTrack,
                                        HPidCandidate *pCand);

    // -------------------------------------------------------------------------

    ClassDef(HPidReconstructor, 0)  // Set of all PID algorithms
};

// -----------------------------------------------------------------------------

#undef DEFAULT_OPTIONS

// -----------------------------------------------------------------------------

#endif //HPIDRECONSTRUCTOR_H
