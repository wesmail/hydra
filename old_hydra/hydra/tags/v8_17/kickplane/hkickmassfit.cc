using namespace std;
#include "hkickmassfit.h"
#include <TMath.h>
#include <iostream> 
#include <iomanip>
#include "hkicktrack.h"

//*-- Author : M. Sanchez
//*-- Modified : M. Sanchez (14.12.2000)

//_HADES_CLASS_DESCRIPTION 
/////////////////////////////////////////////////////////////
// HKickMassFit
//
//   This class makes both particle identification and a inverse
//kinematics momentum fit.
//
//   There is a table of masses for known particles defined in the class. 
//When a user calls fitMomentum giving a mass and a velocity, that mass is
//compared to each of the masses in the table.
//
//   If the mass given by the user is compatible with one and only one of the
//masses in the table then the corresponding Id is assigned to the particle,
//as a second step the momentum of this particlei, determined from its real mass
//(the one in the table) and the given velocity, is calculated.
//
//   To check if a mass is compatible with one of those in the table the
//following criterium is used
//
//    (mass - mass_table) / error_in_mass  <  tolerance
//
//   Where tolerance is a parameter of the class with the default value of 3.
//
//   The table of masses consists of two static arrays: fMassTable and 
//fPidTable. The first table is an array of known masses plus one last element
//which always equals -1 and signals the end of the table.
//
//   Each of the entries in the fMassTable array corresponds to two entries in
//fPidTable which are the two types of particles with that mass: first is
//positive charged and second is negative charged.
////////////////////////////////////////////////////////////////////

Double_t HKickMassFit::fMassTable[] = {139.57, 938.27, 1876.54, -1.};
Char_t HKickMassFit::fPidTable[] =
                 {HKickTrack::piPlusId,HKickTrack::piMinusId,
		  HKickTrack::protonId,HKickTrack::antiProtonId,100,101};

HKickMassFit::HKickMassFit(void) {
  // Constructor
  fMomentum = -1;
  fMomentumErr = -1;
  fTolerance = 3.;
}

HKickMassFit::~HKickMassFit(void) {
  //Obvious

}

void HKickMassFit::fitMomentum(Double_t p0, Double_t errP0, 
			       Double_t beta, Double_t errBeta,
			       Int_t polarity) {
  // This is the core method: a initial momentum is provided together with
  // a velocity and the corresponding errors. Also a polarity (sign of charge)
  // is given as an argument.
  //
  //  The method will identify the particle and calculate the corresponding
  //PID and a new value for the momentum. Those results can be retrieved 
  //using other class methods later
  fPolarity = polarity;
  if (beta>0. && beta <= 1.) {
    Double_t mass = p0 * TMath::Sqrt(1 - beta * beta) / beta;
    Double_t b2 = beta * beta;
    Double_t g2 = 1 - beta * beta;
    Double_t errMass = TMath::Sqrt (( 
			(1 - b2) * errP0 * errP0 +
			errBeta * errBeta * p0 * p0 / (b2 * (1-b2))  
			) / b2 );
    mass = snap(mass,errMass);
    if (mass>0.) {
      fMomentum = mass * beta / TMath::Sqrt( 1 - beta * beta);
      //Correction for low momenta.
       if (fMomentum < 600. && mass==938.27)
	fMomentum = fMomentum/(1-1.6505 * TMath::Exp(-8.6369e-3 * fMomentum));
      fMomentumErr = (( (g2 - beta/2.) * (g2 - beta/2.) ) / (g2 * g2 * g2)) *
	mass * mass * errBeta * errBeta;  
      fMass = mass;
    } else 
      fMomentum = -1.;
  } else fMomentum = -1;
}

Double_t HKickMassFit::snap(Double_t mass, Double_t errMass) {
  //Looks which mass in the mass table is compatible with the given one
  // withing the errors.

  Double_t *pMass = &fMassTable[0];
  Double_t pull=0.;
  Double_t r=0;
  Int_t count = 0,i=0;

  while ( *pMass > 0. ) { // Iterate on mass table
    pull = ( (mass - *pMass) / errMass );
 
    if ( TMath::Abs(pull) < getTolerance() ) { // Masses are compatible
      r = *pMass;
      count++; 
      fPid = fPidTable[(i*2 + ((fPolarity==1)?0:1))];
    }
    pMass++; i++;
  }
  if (count > 1) {
    r = -1;
    fPid = HKickTrack::unknownId;
  }
  //  if (count>1) Warning("snap","conflict for %f +- %f",mass,errMass);
  //if (count == 0)  Warning("snap","Not assigned for %f +- %f",mass,errMass);
  //if (count == 1) Warning("smap","+++++++++++++++++++++++++++++++");

  return r;
}

ClassImp(HKickMassFit)
