#include "hkickmassfit.h"
#include <TMath.h>
#include <iostream.h>
#include "hkicktrack.h"

Double_t HKickMassFit::fMassTable[] = {139.57, 938.27, -1.};
Char_t HKickMassFit::fPidTable[] = {HKickTrack::piPlusId,HKickTrack::piMinusId,
				   HKickTrack::protonId, HKickTrack::antiProtonId};

HKickMassFit::HKickMassFit(void) {
  fMomentum = -1;
  fMomentumErr = -1;
  fTolerance = 3.;
}

HKickMassFit::~HKickMassFit(void) {

}

void HKickMassFit::fitMomentum(Double_t p0, Double_t errP0, 
			       Double_t beta, Double_t errBeta,
			       Int_t polarity) {
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
  Double_t *pMass = &fMassTable[0];
  Double_t pull=0.;
  Double_t r=0;
  int count = 0,i=0;

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
