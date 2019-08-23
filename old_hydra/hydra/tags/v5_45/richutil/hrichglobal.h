// File: hrichglobal.h
//
// Author: Soenke Schroeder <soenke.schroeder@physik.tu-muenchen.de>
// Last update by Soenke Schroeder: 00/08/09 11:14:31
//
#ifndef HRICHGLOBAL_H
#define HRICHGLOBAL_H
#pragma interface

#include <TObject.h>
#include <iostream.h>
#include <TMath.h>

class HRichGlobal : public TObject {

public:
    HRichGlobal() {}
    ~HRichGlobal() {}

    Int_t clear();

    Int_t getSector() {return fSector;}
    Int_t getEventNr() {return fEventNr;}
    Int_t getGlobalEventNr() {return fGlobalEventNr;}
    Float_t getGlobalPhi1() {return fGlobalPhi1;}
    Float_t getGlobalPhi2() {return fGlobalPhi2;}
    Float_t getGlobalTheta1() {return fGlobalTheta1;}
    Float_t getGlobalTheta2() {return fGlobalTheta2;}
    Int_t getGlobalN1() {return fGlobalN1;}
    Int_t getGlobalN2() {return fGlobalN2;}

    void setSector(Int_t Sector) {fSector = Sector;}
    void setEventNr(Int_t EventNr) {fEventNr = EventNr;}
    void setGlobalEventNr(Int_t GlobalEventNr) {fGlobalEventNr = GlobalEventNr;}
    void setGlobalPhi1(Float_t GlobalPhi1) {fGlobalPhi1 = GlobalPhi1;}
    void setGlobalPhi2(Float_t GlobalPhi2) {fGlobalPhi2 = GlobalPhi2;}
    void setGlobalTheta1(Float_t GlobalTheta1) {fGlobalTheta1 = GlobalTheta1;}
    void setGlobalTheta2(Float_t GlobalTheta2) {fGlobalTheta2 = GlobalTheta2;}
    void setGlobalN1(Int_t GlobalN1) {fGlobalN1 = GlobalN1;}
    void setGlobalN2(Int_t GlobalN2) {fGlobalN2 = GlobalN2;}

private:
    Int_t fSector;
    Int_t fEventNr;
    Int_t fGlobalEventNr;
    Float_t fGlobalPhi1;
    Float_t fGlobalPhi2;
    Float_t fGlobalTheta1;
    Float_t fGlobalTheta2;
    Int_t fGlobalN1;
    Int_t fGlobalN2;

    ClassDef(HRichGlobal,1) // RICH simulated cal data object
	};

#endif /* !HRICHGLOBAL_H */
