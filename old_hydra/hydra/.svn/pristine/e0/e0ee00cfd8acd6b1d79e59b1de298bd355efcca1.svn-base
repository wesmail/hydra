// @(#)$Id: hpidparticle.h,v 1.13 2005-04-27 16:49:31 wojcik Exp $
//*-- Author : Marcin Jaskula 30/11/2002
//  Modified : Marcin Jaskula 11/11/2002
//             fExpMass added
//  Modified : Marcin Jaskula 01/12/2002
//             new methods getIndex(), getPidParticle()
//  Modified : Marcin Jaskula 25/02/2003
//             new methods getTrackCand() and getKickTrack()
//  Modified : Marcin Jaskula ???
//             fPull, fTestVal, fMomRescal variables added
//  Modified : Marcin Jaskula 24/06/2003
//             *MultiUsed flags and interface added

#ifndef HPIDPARTICLE_H
#define HPIDPARTICLE_H

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// HPidParticle                                                               //
//                                                                            //
// Identified particle                                                        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include <TLorentzVector.h>
#include <TMath.h>
#include "hpidphysicsconstants.h"
#include "piddef.h"
#include "hpidtrackdata.h"
#include "hpidhitdata.h"
#include "hpidtrackcand.h"
#include "TArrayS.h"
#include "TArrayF.h"
// -----------------------------------------------------------------------------

class HPidCandidate;
class HCategory;

class HPidTrackData;

// -----------------------------------------------------------------------------

class HPidParticle: public TLorentzVector
{
public:

  //This is currently the ONLY constructor
  HPidParticle(HPidCandidate* pCandidate, Float_t* assignedWeights,
	       Bool_t useMassIdeal=kTRUE,  Int_t userDefSpecies=-99, Float_t userDefWeight=0.0);
  
  HPidParticle(void) {}; //Default constructor to satisfy ROOT
  
  ~HPidParticle(void) {}

    // -------------------------------------------------------------------------

    enum {
        kMassExpInTLV = -1,
        kMassExpNoPhy = -2
    };

    // -------------------------------------------------------------------------

    void            setPidCandidateIndex(Short_t n)    { nPidCandidateIndex = n;     }
    Short_t         getPidCandidateIndex(void) const   { return nPidCandidateIndex ; }

    HPidCandidate*  getPidCandidate(HCategory *pCat = NULL) const;


    Float_t         getSystem(void) const           { return getHitData()->iSystem; }
    Float_t         getR(void) const                { return getTrackData()->fTrackR[getMomAlg()]; }
    Float_t         getZ(void) const                { return getTrackData()->fTrackZ[getMomAlg()]; }
    Float_t         getMassExp(void) const          { return (Float_t) M();   }
    Float_t         getBetaExp(void) const          { return getTrackData()->fCorrectedBeta[getMomAlg()]; }

    void            setMomAlg(Int_t i)              { momAlgIndex = i;     }
    Int_t           getMomAlg(void) const           { return momAlgIndex;  }
    Int_t           getCharge(void) const
                                { return HPidPhysicsConstants::charge(nAssignedPID); }

    Short_t         getPid(Short_t n) const         { return possibleSpecies[n]; } //Get the n-th species with non-zero probability
    Float_t         getWeight(Short_t n) const { return assignedWeights[n]; }   //Get the n-th probability-value

    Float_t         getWeightForPID(Short_t pid) const;      //Get the probability that this particle is from species <pid>

    Int_t           getMostProbablePID(void) const;          //Find the species which has the maximum probability and return it to the user
    
    Int_t           getPid(void) const              {return nAssignedPID;} // returns the PID assigned automatically or 
                                                                           // by external function. This is for the default user
                                                                           // which doesn't care for several possibilities

    void            setPid(Short_t npid)            {nAssignedPID=npid;} 

    Int_t           getSpeciesIndex(Short_t pid) const ; //Returns the index for particle <pid> in the TArrayS of possible Species

    void            setTestVal(Float_t f)           { fTestVal = f;          }
    Float_t         getTestVal(void) const          { return fTestVal;       }

    void            setWeight(Float_t f)            { fWeight = f;           }
    Float_t         getWeight(void) const           { return fWeight;        }

    void            setPidParams(Float_t fTestVal, Int_t userDefSpecies=-99,
				 Float_t userWeight=1.0);

    // -------------------------------------------------------------------------

    // function for masking the 'proabilistic nature' of the particle - returning the ideal value for the assigned species    
    Float_t         getMassIdeal(void) const    {return HPidPhysicsConstants::mass(getPid());}
    

    // -------------------------------------------------------------------------

    void            setMomRescal(Float_t f)         { fMomRescal = f;        }
    Float_t         getMomRescal(void) const        { return fMomRescal;     }

    TLorentzVector  getRescaledVector(void) const;

    // -------------------------------------------------------------------------

    virtual void    Clear(Option_t *opt = "");
    virtual void    Print(Option_t* option = "") const     { print();      }
    virtual void    print(void) const;

    // -------------------------------------------------------------------------

    inline Double_t thetaDeg(void) const;
    inline Double_t phiDeg(void) const;
    inline Short_t  sector(void) const;

    // -------------------------------------------------------------------------

    Int_t           getIndex(HCategory *pCat = NULL);

    // -------------------------------------------------------------------------

    static HCategory*    buildPidParticleCategory(void);

    // -------------------------------------------------------------------------

    enum
    {
        kTrackCandOffset = -1000
    };

    HPidTrackCand*  getTrackCand(void) const;


    // -------------------------------------------------------------------------

    HPidHitData*    getHitData(void)   const { return &(getTrackCand()->itsHitData);}
    HPidTrackData*  getTrackData(void) const { return &(getTrackCand()->itsTrackData);}


    // -------------------------------------------------------------------------

protected:

    void            setDefault(void);

    // -------------------------------------------------------------------------

    Bool_t      kUsesIdealMass; //The particle stores the ideal mass value of its assigned PID to build the Lorentz vector

    Int_t       nPossibleSpecies; //number of possible species (dimension of the vectors possibleSpecies and assignedWeights
    
    Short_t     momAlgIndex;    // Index indicating which algorithm for momentum computation has been chosen    

    Short_t     nPidCandidateIndex;  // index of the HPidCandidate

    TArrayS    possibleSpecies; // size: nPossibleSpecies
                                // A set of particle species for which this particle has NON-Zero probability 

    TArrayF    assignedWeights; // size: nPossibleSpecies
                                // A vector holding the weights (most of times the bayesian probabilities) for each species 
                                // in the possibleSpecies vector

    Int_t       nAssignedPID;   // A pid assigned to the particle either by an external algorithm or by choosing the
                                // maximum value of the weight vector
    
    Float_t     fTestVal;       // test value checked during creation

    Float_t     fWeight;        // statistical weight of the particle (probability of the externally assigned species - if any)

    Float_t     fMomRescal;     // rescaling factor for the momentum

    // -------------------------------------------------------------------------

    ClassDef(HPidParticle, 1)   // Identified particle
};



// -----------------------------------------------------------------------------


inline Double_t HPidParticle::thetaDeg(void) const
{
    return TMath::RadToDeg() * Theta();
}

// -----------------------------------------------------------------------------

inline Double_t HPidParticle::phiDeg(void) const
{
Double_t dPhi;

    if((dPhi = TMath::RadToDeg() * Phi()) < 0.0)
        dPhi += 360.0;

    return dPhi;
}

// -----------------------------------------------------------------------------

inline Short_t HPidParticle::sector(void) const
{
  return getHitData()->nSector;
}

// -----------------------------------------------------------------------------

#endif /* HPIDPARTICLE_H */
