// @(#)$Id: hpidparticlesim.h,v 1.10 2006-08-16 16:13:35 halo Exp $
//*-- Author : Marcin Jaskula 11/11/2002
//  Modified : Marcin Jaskula 01/12/2002
//             new method getGeantTrackSet()
//  Modified : Marcin Jaskula 26/02/2003
//             nGeantId & nGeantTrack -> cCommonTrack structure
//  Modified : Marcin Jaskula 29/05/2003
//             getTrackLorentzVector added

#ifndef HPIDPARTICLESIM_H
#define HPIDPARTICLESIM_H

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// HPidParticleSim                                                            //
// extends HPidParticle with a pointer to HPidGeantTrackSet retrieved from    //
// the HPidTrackCand instance that this objects was made from. For a detailed //
// description see HPidParticle source code.                                  //
//                                                                            //
// Identified particle for SIM data                                           //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include "hpidparticle.h"

#include "hpidgeanttrackset.h"

#include "hpidtrackcandsim.h"
// -----------------------------------------------------------------------------

class HGeantKine;
class HPidGeantTrackSet;

// -----------------------------------------------------------------------------

class HPidParticleSim: public HPidParticle
{
public:
  //Constructor only calls base-class constructor
  HPidParticleSim(HPidCandidate* pCandidate, Float_t* assignedWeights,
	       Bool_t useMassIdeal=kTRUE,  Int_t userDefSpecies=-99, Float_t userWeight=0.0);

  HPidParticleSim(void) {};



  ~HPidParticleSim(void) {}
  
  // -------------------------------------------------------------------------
  //Override TObject::Clear() This is required to clear the dynamic data fields in the base class
  void    Clear(Option_t *opt = "");
  void    print(void) const;


  //The information about the hits and geant particles that contributed to this object is stored in
  //HPidGeantTrackSet. This is not an additional datamember but it is retrieved dynmaically from
  //HPidTrackCand.
  HPidTrackCandSim*  getTrackCandSim(void) const;
  HPidTrackCand*     getTrackCand(void) const {return getTrackCandSim();}
  const HPidGeantTrackSet* getGeantTrackSet(void) const {return &itsGeantTrackSet;}
  //const HPidGeantTrackSet& getGeantTrackSet(void) const {return  itsGeantTrackSet;}

  //Function to distinguish between sim/exp category
  static HCategory*  buildPidParticleSimCategory(void);  


  //Functions calling the geant-trackset functions for accessing simulation information more conveniently
  Int_t               getValidCommonTracks(void) const {return getGeantTrackSet()->getNCorrelatedTrackIds();}
  inline Char_t       getGeantTrackDetectors(UInt_t iPos = 0) const {return getGeantTrackSet()->getCorrelationFlag(iPos);}
  inline UInt_t       getGeantTrackProcess(UInt_t iPos = 0) const {return getGeantTrackSet()->getGeantProcessID(iPos);}
  inline Short_t      getGeantTrackID(UInt_t iPos = 0) const {return getGeantTrackSet()->getGeantTrackID(iPos);}
  inline Short_t      getGeantPID(UInt_t iPos = 0) const {return getGeantTrackSet()->getGeantPID(iPos);}

  
  inline Double_t     getTrackMomX(UInt_t iPos = 0) const {return getGeantTrackSet()->getGeantMomX(iPos);}
  inline Double_t     getTrackMomY(UInt_t iPos = 0) const {return getGeantTrackSet()->getGeantMomY(iPos);}
  inline Double_t     getTrackMomZ(UInt_t iPos = 0) const {return getGeantTrackSet()->getGeantMomZ(iPos);}
  // -------------------------------------------------------------------------
  
  
  inline const TVector3*  getGeantMomentum(UInt_t iPos = 0) const; //OK
  inline const TLorentzVector*  getGeantLorentzVector(UInt_t iPos = 0) const; //OK

  HPidGeantTrackSet itsGeantTrackSet;


protected:
  //This class has no variables going beyond HPidParticle. It just provided more accessors to retrieve
  //sim information more conveniently.
  
    // -------------------------------------------------------------------------

    ClassDef(HPidParticleSim, 3)   // Identified particle for sim data
};

// -----------------------------------------------------------------------------

inline const TVector3* HPidParticleSim::getGeantMomentum(UInt_t iPos) const
{
  //return (getGeantTrackSet()->getCommonTrack(iPos)->getMomentumVector());
  return (getGeantTrackSet()->buildGeantMomentumVector(iPos));
}

// -----------------------------------------------------------------------------

inline const TLorentzVector* HPidParticleSim::getGeantLorentzVector(UInt_t iPos) const
{
  return (getGeantTrackSet()->buildGeantLorentzVector(iPos));

}


#endif /* HPIDPARTICLESIM_H */
