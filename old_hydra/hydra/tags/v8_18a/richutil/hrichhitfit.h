// $Id: hrichhitfit.h,v 1.4 2002-09-25 16:19:58 eberl Exp $
// Last update by Thomas Eberl: 02/09/25 18:11:34
//
#ifndef HRICHHITFIT_H
#define HRICHHITFIT_H

#include "TObject.h"
#include "TArrayI.h"
#include "hcategory.h"

class HRichHitFit: public TObject {

private:
    Float_t fFitRadius;
    Float_t fFitCenterX;
    Float_t fFitCenterY;
    Float_t fFitVar;
    Float_t fFitCenterTheta;
    Float_t fFitCenterPhi;
    Int_t nSector;
    Int_t nNbCoords;
    Int_t hitindex;
public:

 HRichHitFit();
 virtual ~HRichHitFit();

 HRichHitFit(const HRichHitFit& source);
 HRichHitFit& operator=(const HRichHitFit& source);

 void Reset();

 Int_t getSector(void) {return nSector;}
 void setSector(Int_t s) {nSector = s;}
    Int_t getNbCoords(void){return nNbCoords;};
    void setNbCoords(Int_t n){nNbCoords=n;};
    Float_t getFitRadius(){return fFitRadius;};
    Float_t getFitCenterX(){return fFitCenterX;};
    Float_t getFitCenterY(){return fFitCenterY;};
    Float_t getFitVar(){return fFitVar;};
    Float_t getFitCenterTheta(){return fFitCenterTheta;};
    Float_t getFitCenterPhi(){return fFitCenterPhi;};
    Int_t   getHitIndex(){return hitindex;};
    void setFitRadius(Double_t rad){ fFitRadius= (Float_t) rad;};
    void setFitCenterX(Double_t centx){ fFitCenterX=(Float_t) centx;};
    void setFitCenterY(Double_t centy){ fFitCenterY=(Float_t) centy;};
    void setFitVar(Double_t var){ fFitVar=(Float_t) var;};
    void setFitCenterTheta(Double_t centtheta){ fFitCenterTheta=(Float_t) centtheta;};
    void setFitCenterPhi(Double_t centphi){ fFitCenterPhi=(Float_t) centphi;};
    void setHitIndex(Int_t ind){hitindex = ind;};
    void dumpToStdout();
 ClassDef(HRichHitFit,2)
};


#endif // HRICHHIT_H


