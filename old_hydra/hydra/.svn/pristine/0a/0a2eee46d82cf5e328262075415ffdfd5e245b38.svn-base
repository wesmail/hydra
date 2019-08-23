// File: hphotoncluster.h
//
// Author: Laura Fabbietti <L.Fabbietti@physik.tu-muenchen.de>
// Last update by Laura Fabbietti: 02/08/09 17:34:27
//
#ifndef HPHOTONCLUSTER_H
#define HPHOTONCLUSTER_H
#pragma interface
using namespace std;
#include "TList.h"
#include "TSeqCollection.h"
#include "TObject.h"
#include <iostream> 
#include <iomanip>
#include "TVector.h"
#include "hrichpad.h"
#include "hrichcal.h"
#include "hrichgeometrypar.h"

class HPhotonCluster : public TObject {

public:
    HPhotonCluster();
    ~HPhotonCluster();
    void clear();

    void setiPadNr(Int_t nr){iPadNr = nr;}
    void setfPhiMaxThetaMax(HRichGeometryPar* pGeomPar);
    void setiClass(Int_t cl){iClass = cl;}
    void setiXDimension(Int_t xD){iXDimension = xD;}
    void setiYDimension(Int_t yD){iYDimension = yD;}
    void setfTotalCharge(Float_t tC){fTotalCharge = tC;}
    void setSector(Int_t iSec){iSector=iSec;}
    void addToTheList(HRichCal* pPad){fPads->Add(pPad);}
    void sortListByCharge(void){fPads->Sort();}
    void calculatemaxmin(void);
    

    Int_t getiPadNr(void){return iPadNr;}
    Float_t getfThetaMax(void){return fThetaMax;}
    Float_t getfPhiMax(void){return fPhiMax;}
    Int_t getiClass(void){return iClass;}
    Int_t getiXDimension(void){return iXDimension;}
    Int_t getiYDimension(void){return iYDimension;}
    Float_t getfTotalCharge(void){return fTotalCharge;}
    Int_t getSector(void){return iSector;}
    HRichCal * getPadInList(Int_t i){return (HRichCal*)fPads->At(i);}
    Bool_t isInPadList(Int_t col, Int_t row);   
private:
    Int_t iPadNr;
    Float_t fThetaMax, fPhiMax;
    Int_t iClass;
    Int_t iXDimension;
    Int_t  iYDimension;
    Float_t fTotalCharge;
    Int_t iSector;
    TList *fPads;//!
  
 ClassDef(HPhotonCluster,1)
    
};
#endif /* !HPHOTONCLUSTER_H */
