// File: hrichhitfit.cc
//
// Author: Thomas Eberl <Thomas.Eberl@physik.tu-muenchen.de>
// Last update by Thomas Eberl: 01/07/16 14:08:27
//
#include "hrichhitfit.h"

ClassImp(HRichHitFit)


HRichHitFit::HRichHitFit() {
    fFitRadius=0.;
    fFitCenterX=0.;
    fFitCenterY=0.;
    fFitVar=0.;
    fFitCenterTheta=0.;
    fFitCenterPhi=0.;
    nSector = 0;
    nNbCoords = 0;
}

HRichHitFit::~HRichHitFit() {}

HRichHitFit::HRichHitFit(const HRichHitFit& source) {
    fFitRadius=source.fFitRadius;
    fFitCenterX=source.fFitCenterX;
    fFitCenterY=source.fFitCenterY;
    fFitVar=source.fFitVar;
    fFitCenterTheta=source.fFitCenterTheta;
    fFitCenterPhi=source.fFitCenterPhi;
    nSector = source.nSector;
    nNbCoords = source.nNbCoords;
}

HRichHitFit& HRichHitFit::operator=(const HRichHitFit& source) {
 if (this != &source) {
     fFitRadius=source.fFitRadius;
     fFitCenterX=source.fFitCenterX;
     fFitCenterY=source.fFitCenterY;
     fFitVar=source.fFitVar;
     fFitCenterTheta=source.fFitCenterTheta;
     fFitCenterPhi=source.fFitCenterPhi;
     nSector = source.nSector;
     nNbCoords = source.nNbCoords;
 }

return *this;
}


void HRichHitFit::Reset() {
    fFitRadius=0.;
    fFitCenterX=0.;
    fFitCenterY=0.;
    fFitVar=0.;
    fFitCenterTheta=0.;
    fFitCenterPhi=0.;
    nSector = 0;
    nNbCoords = 0;
}






