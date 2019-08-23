// @(#)$Id: hpidreconstructor.h,v 1.9 2006-08-16 16:13:35 halo Exp $
//*-- Author : Marcin Jaskula 27/07/2002
//  Modified : Marcin Jaskula 18/03/2003
//           - get rid of storing the merged values in the HPidCandidates
//             and the interface for it

#ifndef HPIDRECONSTRUCTOR_H
#define HPIDRECONSTRUCTOR_H

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// HPidReconstructor: This is the Hydra task executing a list of algorithms   //
// for PID that the user chooses to append to the reconstructor               //
// Controls and manages execution and ionitialization of algorithms           //
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

//Default creation options of the task
#define DEFAULT_OPTIONS  "PDF,CL,ALG_KICK,RELINTS"

// -----------------------------------------------------------------------------

class HPidReconstructor : public HReconstructor
{
public:
                        HPidReconstructor(Option_t par[] = DEFAULT_OPTIONS);
                        HPidReconstructor(Text_t name[], Text_t title[],
                                        Option_t par[] = DEFAULT_OPTIONS);

                       ~HPidReconstructor(void);

    // -------------------------------------------------------------------------

    //All reconstructors must have this		       
    virtual Int_t       execute(void);
    virtual Bool_t      init(void);
    virtual Bool_t      reinit(void);
    virtual Bool_t      finalize(void);



    //manage list of algorithms
    // -------------------------------------------------------------------------
    Bool_t              addAlgorithm(HPidAlgorithm *pAlg);
    //get a specific algorithm by ID or by name
    HPidAlgorithm*      getAlgorithm(const TString &sName) const;
    HPidAlgorithm*      getAlgorithm(EnumPidAlgorithm_t eId) const;
    Bool_t              removeAlgorithm(HPidAlgorithm *pAlg);
    Bool_t              removeAlgorithm(const TString &sName);
    Bool_t              removeAlgorithm(EnumPidAlgorithm_t eId);
    //return number of appended algorithms
    Int_t               algorithmsNumber(void) const { return pAlgorithms->GetSize();       }


    //manage list of particles considered
    // -------------------------------------------------------------------------
    //Returns particle type with index iPos in array of active particles
    Short_t             getParticleId(Int_t iPos) const;

    //Returns position of particle nType in aParticles vector or -1 if not set
    Int_t               getParticleIndex(Short_t nType) const;

    //Append a particle species top the list of possible particle types
    Int_t               addParticleId(Short_t nType);
    
    //Set a the pid at a specific position in the array
    void                setParticleId(Int_t iPos, Short_t nType);

    //Set the whole list of relevant species
    void                setParticleIds(Short_t aIds[], Int_t iSize);
    
    //Set default list of species (positron,electron,pi-lus,pi-minus,proton,deuteron,k+,fake-negative,fake-positive
    void                setDefaultParticleIds(void);

    Int_t               particlesNumber(void) const     { return iParticles;   }


    //configure and check behaviour of reconstructor
    // -------------------------------------------------------------------------
    void                setParameters(Option_t par[]);

    Bool_t              getCalcPDF(void) const              { return bCalcPDF;         }
    void                setCalcPDF(Bool_t b = kTRUE)        { bCalcPDF = b;            }

    Bool_t              getCalcRelints(void) const              { return bCalcRelints;         }
    void                setCalcRelints(Bool_t b = kTRUE)        { bCalcRelints = b;            }

    Bool_t              getCalcCL(void) const               { return bCalcCL;          }
    void                setCalcCL(Bool_t b = kTRUE)         { bCalcCL = b;             }

    Int_t               getDebug(void) const            { return iDebug;       }
    void                setDebug(Int_t i)               { iDebug = i;          }

    // -------------------------------------------------------------------------

    virtual void        Print(Option_t* option = "") const     { print();     }
    virtual void        print(void) const;

    // -------------------------------------------------------------------------

    static void         normalize(Float_t af[], UInt_t iSize);

    // -------------------------------------------------------------------------

protected:


    Bool_t              bCalcPDF;       // calculate PDF enabled
    Bool_t              bCalcCL;        // calculate CL enabled
    Bool_t              bCalcRelints;   // calculate relative intensities enabled


    static const Int_t  kMaxParticles = 100; //maximum number of different particle species


    TList              *pAlgorithms;    // list of all algorithms
    TIterator          *pitList;        // iterator over the list of the algs

    Bool_t              bInDelete;      // flag used during removing to indicate state of reconstructor instance

    HCategory          *pInputCat;      // category HPidTrackCand (input)
    HIterator          *pitInput;       // iterator over pInputCat category

    HCategory          *pOutCat;        // category HPidCandidate (output)

    Short_t             aParticles[kMaxParticles];  // ids of particles
    Int_t               iParticles;     // number of active particles
    Int_t               iAlgorithms;    // number of active coloumns needed by the algorithms 
                                        // (depends on whether or not cl-compuatition is enabled or not!)

    Bool_t              bInitOk;        // set kTRUE if last init was ok

    HPidReconstructorPar *pParams;      // pointer to container with parameters (relative intensities)

    Int_t               iDebug;         // level of debug informations

    Int_t               iVectPerAlg;    // number of vectors for one algorithm. This can be 1 or 2 
                                        // (depends on whether or not cl-compuatition is enabled or not!)

    // -------------------------------------------------------------------------

    void                setDefault(void);

    HPidCandidate*      getNextSlot(void);

    virtual void        calculateRelInts(HPidTrackCand *pTrack, HPidCandidate *pCand);

    // -------------------------------------------------------------------------

 public:
    // ID of the momentum algorithm selected by the user to perform PID (for possible selections see piddef.h
    Int_t              iSelectedMomAlg; 

    ClassDef(HPidReconstructor, 0)  // Set of all PID algorithms
};

// -----------------------------------------------------------------------------

#undef DEFAULT_OPTIONS

// -----------------------------------------------------------------------------

#endif //HPIDRECONSTRUCTOR_H
