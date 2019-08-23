#include "TObject.h"
#include "hrichgeantdirhit.h"
#include <iostream.h>

//////////////////////////////////////////////////////
// HRichGeantDirHit
//
//   This class contains one hit in one cell of the rich.
//
//////////////////////////////////////////////////////

ClassImp(HRichGeantDirHit)

HRichGeantDirHit::HRichGeantDirHit() {
}

HRichGeantDirHit::~HRichGeantDirHit() {
}

void HRichGeantDirHit::Clear() {
   fXHit = 0.0;
   fYHit = 0.0;
   fZHit = 0.0;
   nPartHit = 0;
   fTrkLenHit = 0.0;
   fMomHit = 0.0;
   fThetaHit = 0.0;
   fPhiHit = 0.0;
   fELosHit = 0.0;
   fTLenHit = 0.0;
   nMirHit = 0;
   nTrkHit = 0;
   nParentHit = 0;
   nProcesHit = 0;
}

void HRichGeantDirHit::getXYZ(Float_t* pfX, Float_t* pfY, Float_t* pfZ) {
   *pfX = fXHit;
   *pfY = fYHit;
   *pfZ = fZHit;
}

void HRichGeantDirHit::getAngles(Float_t* pfTheta, Float_t* pfPhi) {
   *pfTheta = fThetaHit;
   *pfPhi = fPhiHit;
}

Float_t HRichGeantDirHit::getEnerLos() {
   return fELosHit;
}

Float_t HRichGeantDirHit::getTLen() {
   return fTLenHit;
}

Int_t HRichGeantDirHit::IsMirror() {
   return nMirHit;
}

void HRichGeantDirHit::setXYZ(Float_t fX, Float_t fY, Float_t fZ) {
   fXHit = fX;
   fYHit= fY;
   fZHit = fZ;
}
   
void HRichGeantDirHit::setAngles(Float_t fTheta, Float_t fPhi) {
   fThetaHit= fTheta;
   fPhiHit = fPhi;
}

void HRichGeantDirHit::setEnerLos(Float_t fEner) {
   fELosHit = fEner;
}

void HRichGeantDirHit::setTLen(Float_t fTLen) {
   fTLenHit = fTLen;
}

void HRichGeantDirHit::setMirror(Int_t nMir) {
   nMirHit = nMir;
}

ostream& operator<< (ostream& output, HRichGeantDirHit& raw) {
//        output<<"pad charge:"<<raw.charge;
        return output;
}    
