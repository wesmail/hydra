// @(#)$Id: hpidparticle.h,v 1.18 2006-11-24 16:00:42 christ Exp $
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
// An instance of HPidParticle stores the probabilities of assignment to one  //
// or more species. A HPidParticle represents the PIDs best guess about the   //
// species as which a track should be treated!                                //
//                                                                            //
// A HPidParticle has also a so called "assigned PID". This value exists in   //
// order to allow the particle to be treated as an unambiguous lepton,proton  //
// or pion in the analysis if the user doesn´t care for the probabilistic     //
// nature of the decision. At creation the assigned PID can be determined     //
// automatically from the _most_likely_pid_ in the probability-list or it can //
// be set by hand from outside (user decision). The latter is implemented     //
// at present but there exists no reconstructor using the feature at present. //
// Thus the assigned PID is always determined from the most likely one when   //
// constructor is called. In the future there might be reason to implement the//
// possibility to call the reconstructor HPidParticleFiller with a poiner to  //
// an external function that overides the automatic assignment.               //
//                                                                            // 
// If the user decides to override the default assignment he/she has to       //
// provide a weight which is to be assigned to the particle also. The weight  //
// can then be used later to determine the reliability of the PID assignment  //
// by another user. At present the default weight of assignment is the        //
// probability that was associated to the "assigned PID" - i.e. the biggest   //
// probability in the probability vector.                                     //
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

  //This constructor is used by HPidParticleFiller to build an HPidParticle once all pid methods
  //have given their statement about the possible species this track should be assigned to
  HPidParticle(HPidCandidate* pCandidate, Float_t* assignedWeights,
	       Bool_t useMassIdeal=kTRUE,  Int_t userDefSpecies=-99, Float_t userDefWeight=0.0);
  
  HPidParticle(void) {}; //Default constructor to satisfy ROOT
  HPidParticle(const HPidParticle& source); //copy constructor
  
  ~HPidParticle(void) {}

    // -------------------------------------------------------------------------

    enum {
        kMassExpInTLV = -1,
        kMassExpNoPhy = -2
    };

    // -------------------------------------------------------------------------

    //The index to an instance of the PID input class
    void            setPidCandidateIndex(Short_t n)    { nPidCandidateIndex = n;     }
    Short_t         getPidCandidateIndex(void) const   { return nPidCandidateIndex ; }

    //The instance itself
    HPidCandidate*  getPidCandidate(HCategory *pCat = NULL) const;

    //getters and setters will not be documented unless required
    Float_t         getSystem(void) const           { return getHitData()->iSystem; }
    Float_t         getR(void) const                { return getTrackData()->fTrackR[getMomAlg()]; }
    Float_t         getZ(void) const                { return getTrackData()->fTrackZ[getMomAlg()]; }
    Float_t         getMassExp(void) const          { return (Float_t) M();   }
    Float_t         getBetaExp(void) const          { return getTrackData()->fCorrectedBeta[getMomAlg()]; }

    void            setMomAlg(Int_t i)              { momAlgIndex = i;     }
    Int_t           getMomAlg(void) const           { return momAlgIndex;  }
    Int_t           getCharge(void) const
                                { return HPidPhysicsConstants::charge(nAssignedPID); }

    //Get the n-th species which was considered in the analysis (the probability might zero!)
    Short_t         getPid(Short_t n) const         { return possibleSpecies[n]; } 

    //Get the n-th probability-value
    Float_t         getWeight(Short_t n) const { return assignedWeights[n]; }   

    //Get the probability that this particle is from species <pid>
    Float_t         getWeightForPID(Short_t pid) const;      

    //Find the species which has the _maximum_probability_ and return it to the user
    Int_t           getMostProbablePID(void) const;          
    

    Int_t           getPid(void) const              {return nAssignedPID;} // returns the PID assigned automatically or 
                                                                           // by external function. This is for the default user
                                                                           // which doesn't care for several possibilities

    // set the assigned pid by external decision mechanism
    void            setPid(Short_t npid)            {nAssignedPID=npid;}   

    //Returns the index for particle <pid> in the TArrayS of possible Species
    Int_t           getSpeciesIndex(Short_t pid) const ; 

    
    void            setTestVal(Float_t f)           { fTestVal = f;          }
    Float_t         getTestVal(void) const          { return fTestVal;       }
    
    //void            setWeight(Float_t f)            { fWeight = f;           }
    Float_t         getWeight(void) const           { return fWeight;        }

    //This function decides whether the PID results are used to build the particle or whether they are overridden
    //by user supplied values. (See documentation in cc file)
    void            setPidParams(Float_t fTestVal, Int_t userDefSpecies=-99,
				 Float_t userWeight=1.0);

    // -------------------------------------------------------------------------

    // function for masking the 'proabilistic nature' of the particle - returning the ideal mass value for the assigned species    
    Float_t         getMassIdeal(void) const    {return HPidPhysicsConstants::mass(getPid());}
    


    // -------------------------------------------------------------------------
    //The momentum of the particle might be rescaled in order to fulfill the relativistic
    //energy momentum relation after assignment of an ideal mass.
    void            setMomRescal(Float_t f)         { fMomRescal = f;        }
    Float_t         getMomRescal(void) const        { return fMomRescal;     }

    TLorentzVector  getRescaledVector(void) const;

    // -------------------------------------------------------------------------

    //Overrides TObject::Clear() and frees dynamic memory
    virtual void    Clear(Option_t *opt = "");
    virtual void    Print(Option_t* option = "") const     { print();      }
    virtual void    print(void) const;

    // -------------------------------------------------------------------------

    inline Double_t thetaDeg(void) const; //returns the polar angle of the track  -  Attention: This depends on the chosen
                                          //tracking method!!
    inline Double_t phiDeg(void) const;   //returns the azimuthal angle of the track  -  Attention: This depends on the chosen
                                          //tracking method!!
    inline Short_t  sector(void) const;

    // -------------------------------------------------------------------------
    // Return index of the HPidParticle in the pCat or catPidPart if pCat == NULL
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

    const HPidHitData*    getHitData(void)   const { return &itsHitData;} //OK
    const HPidTrackData*  getTrackData(void) const { return &itsTrackData;} //OK


    // -------------------------------------------------------------------------
    Int_t getNPossibleSpecies(void){return nPossibleSpecies;};
protected:

    void            setDefault(void);

    // -------------------------------------------------------------------------

    Bool_t      kUsesIdealMass;       // kTRUE if the particle stores the ideal mass value of its assigned PID to build the Lorentz 
                                      // vector

    Int_t       nPossibleSpecies;     // number of possible species (dimension of the vectors possibleSpecies and assignedWeights
    
    Short_t     momAlgIndex;          // Index indicating which algorithm for momentum computation has been chosen    

    Short_t     nPidCandidateIndex;   // index of the HPidCandidate this particle was made from

    TArrayS    possibleSpecies;       // size: nPossibleSpecies
                                      // A set of particle species for which this particle has NON-Zero probability 

    TArrayF    assignedWeights;       // size: nPossibleSpecies
                                      // A vector holding the weights (most of times the bayesian probabilities) for each species 
                                      // in the possibleSpecies vector

    Int_t       nAssignedPID;   // A pid assigned to the particle either by an external algorithm or by choosing the
                                // maximum value of the weight vector
    
    Float_t     fTestVal;       // test value checked during creation

    Float_t     fWeight;        // statistical weight of the particle (probability of the externally assigned species - if any)

    Float_t     fMomRescal;     // rescaling factor for the momentum which might be applied to compute from the stored 
                                // momentum the "kinematically correct" one assuming correct time of flight, path length and
                                // the ideal mass. 

    HPidHitData     itsHitData;
    HPidTrackData   itsTrackData;
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
