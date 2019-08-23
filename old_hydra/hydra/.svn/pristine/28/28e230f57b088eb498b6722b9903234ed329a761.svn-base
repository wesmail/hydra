// File: hrichlocal.h
//
// Author: Soenke Schroeder <soenke.schroeder@physik.tu-muenchen.de>
// Last update by Soenke Schroeder: 01/05/14 15:06:07
//
#ifndef HRICHLOCAL_H
#define HRICHLOCAL_H
#pragma interface

#include <TObject.h>
#include <iostream.h>
#include <TMath.h>

class HRichLocal : public TObject {

public:
    HRichLocal() {}
    ~HRichLocal() {}

    Int_t clear();
    
    Int_t getRow() {return fRow;}
    Int_t getCol() {return fCol;}
    Int_t getSector() {return fSector;}
    Int_t getAddress() {return fAddress;}
    Int_t getEventNr() {return fEventNr;}
    Int_t getLocalEventNr() {return fLocalEventNr;}
    Float_t getLocalCharge() {return fLocalCharge;}
    Float_t getLocalTheta() {return fLocalTheta;}
    Float_t getLocalPhi() {return fLocalPhi;}
    Float_t getLocalEnergy() {return fLocalEnergy;}//from simulated photons
    Int_t getRing() {return fRing;}
    Int_t getFlagMax(){ return iFlag;}
    Int_t getPadNr(){ return iPadNr;}
    
    void setRow(Int_t Row) {fRow = Row;}
    void setCol(Int_t Col) {fCol = Col;}
    void setSector(Int_t Sector) {fSector = Sector;}
    void setAddress(Int_t Address) {fAddress = Address;}
    void setEventNr(Int_t EventNr) {fEventNr = EventNr;}
    void setLocalEventNr(Int_t LocalEventNr) {fLocalEventNr = LocalEventNr;}
    void setLocalCharge(Float_t LocalCharge) {fLocalCharge = LocalCharge;}
    void setLocalTheta(Float_t LocalTheta) {fLocalTheta = LocalTheta;}
    void setLocalPhi(Float_t LocalPhi) {fLocalPhi = LocalPhi;}
    void setLocalEnergy(Float_t LocalEnergy) {fLocalEnergy = LocalEnergy;}
    void setRing(Int_t Ring) {fRing = Ring;}
    void setPadNr(Int_t pN){iPadNr = pN;}
    void setFlagMax(Int_t fM){iFlag = fM;}
private:

    Int_t fRow;
    Int_t fCol; 
    Int_t fSector;
    Int_t fAddress;
    Int_t fEventNr;
    Int_t fLocalEventNr;
    Float_t fLocalCharge;
    Float_t fLocalPhi;
    Float_t fLocalTheta;
    Float_t fLocalEnergy;
    Int_t fRing;
    Int_t iFlag;
    Int_t iPadNr;

    ClassDef(HRichLocal,1) // RICH simulated cal data object
	};

#endif /* !HRICHLOCAL_H */

