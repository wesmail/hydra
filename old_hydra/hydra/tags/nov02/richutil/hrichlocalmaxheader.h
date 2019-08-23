// File: hrichlocalmaxheader.h
//
// Author: Laura Fabbietti <L.Fabbietti@physik.tu-muenchen.de>
// Last update by Laura Fabbietti: 02/04/25 17:22:51
//
#ifndef HRICHLOCALMAXHEADER_H
#define HRICHLOCALMAXHEADER_H
#include <TObject.h>
#include <TArrayI.h>

class HRichLocalMaxHeader: public TObject
{
public:
    HRichLocalMaxHeader();
    virtual ~ HRichLocalMaxHeader(){;}
    void setEventFlag(Int_t flag){iEventFlag =flag;}
    void setMeanThetaLoc1(Float_t theta){fMeanThetaLoc1 = theta;}
    void setMeanThetaLoc2(Float_t theta){fMeanThetaLoc2 = theta;}
    void setMeanThetaPad1(Float_t theta){fMeanThetaPad1 = theta;}
    void setMeanThetaPad2(Float_t theta){fMeanThetaPad2 = theta;}
    void setSecMult(Float_t  sec[6]);
    void setMeanChargeSec(Float_t mC[6]);
    void setEvtNum(Int_t evt) {evtNum = evt;}

    Int_t getEventFlag(){return iEventFlag;}
    Float_t getMeanThetaPad1(){return fMeanThetaPad1;}
    Float_t getMeanThetaPad2(){return fMeanThetaPad2;}
    Float_t getMeanThetaLoc1(){return fMeanThetaLoc1;}
    Float_t getMeanThetaLoc2(){return fMeanThetaLoc2;}
    void getSecMult(Float_t mult[6]);   
    void getMeanChargeSec(Float_t mC[6]);
    Int_t getEventNum(){return evtNum;}

public:
    Int_t evtNum;
    Int_t iEventFlag;
    Float_t fMeanThetaPad1;
    Float_t fMeanThetaPad2;
    Float_t fMeanThetaLoc1;
    Float_t fMeanThetaLoc2;
    Float_t multSec0,multSec1,multSec2,multSec3,multSec4,multSec5;
    Float_t meanChargeSec0,meanChargeSec1,meanChargeSec2,meanChargeSec3,meanChargeSec4,meanChargeSec5;
 ClassDef(HRichLocalMaxHeader, 1)
};
#endif /* !HRICHLOCALMAXHEADER_H */
