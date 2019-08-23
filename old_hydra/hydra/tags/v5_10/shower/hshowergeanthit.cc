#include "TObject.h"
#include "hshowergeanthit.h"
#include <iostream.h>

//////////////////////////////////////////////////////
// HShowerGeantHit
//
//   This class contains one hit in one cell of the rich.
//
//////////////////////////////////////////////////////

ClassImp(HShowerGeantHit)

HShowerGeantHit::HShowerGeantHit() {
}

HShowerGeantHit::~HShowerGeantHit() {
}

void HShowerGeantHit::Clear() {
   fBetaHit = 0.0;
   fMomHit = 0.0;
   fELosHit = 0.0;
   nDetHit = 0;
   fPhiHit = 0.0;
   fThetaHit = 0.0;
   fXHit = 0.0;
   fYHit = 0.0;

   nTrack = -1;
}

void HShowerGeantHit::getXY(Float_t* pfX, Float_t* pfY) {
   *pfX = fXHit;
   *pfY = fYHit;
}

void HShowerGeantHit::getAngles(Float_t* pfTheta, Float_t* pfPhi) {
   *pfTheta = fThetaHit;
   *pfPhi = fPhiHit;
}

Float_t HShowerGeantHit::getEnerLos() {
   return fELosHit;
}

Float_t HShowerGeantHit::getBeta() {
   return fBetaHit;
}

Float_t HShowerGeantHit::getMom() {
   return fMomHit;
}

Int_t HShowerGeantHit::getDetector() {
   return nDetHit;
}

Int_t HShowerGeantHit::getSector() {
   return nSectHit;
}

Int_t HShowerGeantHit::getTrack() {
   return nTrack;
}

void HShowerGeantHit::setXY(Float_t fX, Float_t fY) {
   fXHit = fX;
   fYHit= fY;
}
   
void HShowerGeantHit::setAngles(Float_t fTheta, Float_t fPhi) {
   fThetaHit= fTheta;
   fPhiHit = fPhi;
}

void HShowerGeantHit::setEnerLos(Float_t fEner) {
   fELosHit = fEner;
}

void HShowerGeantHit::setBeta(Float_t fBeta) {
   fBetaHit = fBeta;
}

void HShowerGeantHit::setMom(Float_t fMom) {
   fMomHit = fMom;
}

void HShowerGeantHit::setDetector(Int_t nDet) {
   nDetHit = nDet;
}

void HShowerGeantHit::setSector(Int_t nSect) {
   nSectHit = nSect;
}

void HShowerGeantHit::setTrack(Int_t nTrk) {
   nTrack = nTrk;
}

ostream& operator<< (ostream& output, HShowerGeantHit& raw) {
//        output<<"pad charge:"<<raw.charge;
        return output;
}    
