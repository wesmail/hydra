// $Id: hrichhitfit.cc,v 1.5 2003-01-24 18:24:34 halo Exp $
// Last update by Thomas Eberl: 02/09/25 17:41:25
//
using namespace std;
#include "hrichhitfit.h"
#include <iostream> 
#include <iomanip>
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
    hitindex=-1;
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
     hitindex = source.hitindex;
 }

return *this;
}


void HRichHitFit::Reset() {
    fFitRadius=-1.;
    fFitCenterX=-1.;
    fFitCenterY=-1.;
    fFitVar=-1.;
    fFitCenterTheta=-1.;
    fFitCenterPhi=-1.;
    nSector = -1;
    nNbCoords = -1;
    hitindex = -1;
}

void HRichHitFit::dumpToStdout()
{

    cout<<"Fit> sec:"<<nSector<<" x:"<<fFitCenterX<<" y:"<<fFitCenterY
	<<" rad:"<<fFitRadius<<" theta:"<<fFitCenterTheta
	<<" phi:"<<fFitCenterPhi
	<<" var:"<<fFitVar<<" nb coords:"<<nNbCoords
	<<" ind:"<<hitindex
	<<endl;

}




