#ifndef HMDCCAL2PARSIM_H
#define HMDCCAL2PARSIM_H

#include "TObject.h"
#include "TObjArray.h"
#include "hparset.h"
#include <TF1.h>

class HMdcCal2ParAngleSim : public TObject {
protected:
    Double_t par0;   // par0 of fitfunction for one angle step of xt relation
    Double_t par1;   // par1 of fitfunction for one angle step of xt relation
    Double_t par2;   // par2 of fitfunction for one angle step of xt relation
    Double_t par3;   // par3 of fitfunction for one angle step of xt relation
    Double_t par4;   // par4 of fitfunction for one angle step of xt relation
    Double_t par5;   // par5 of fitfunction for one angle step of xt relation
    Double_t par6;   // par6 of fitfunction for one angle step of xt relation
    Double_t par7;   // par7 of fitfunction for one angle step of xt relation
    Double_t par8;   // par8 of fitfunction for one angle step of xt relation
    Double_t par9;   // par8 of fitfunction for one angle step of xt relation
    Double_t par10;   // par8 of fitfunction for one angle step of xt relation


public:
    HMdcCal2ParAngleSim() : par0(0.F),par1(0.F),par2(0.F),par3(0.F),
                         par4(0.F),par5(0.F),par6(0.F),par7(0.F),
                         par8(0.F),par9(0.F),par10(0.F) {;}
    ~HMdcCal2ParAngleSim() {;}
    Float_t getPar0() {return par0;}
    Float_t getPar1() {return par1;}
    Float_t getPar2() {return par2;}
    Float_t getPar3() {return par3;}
    Float_t getPar4() {return par4;}
    Float_t getPar5() {return par5;}
    Float_t getPar6() {return par6;}
    Float_t getPar7() {return par7;}
    Float_t getPar8() {return par8;}
    Float_t getPar9() {return par9;}
    Float_t getPar10() {return par10;}


    void fill(Double_t p0,Double_t p1,Double_t p2,Double_t p3,Double_t p4,Double_t p5,Double_t p6,Double_t p7,Double_t p8,Double_t p9,Double_t p10)
    {
      par0=p0;
      par1=p1;
      par2=p2;
      par3=p3;
      par4=p4;
      par5=p5;
      par6=p6;
      par7=p7;
      par8=p8;
      par9=p9;
      par10=p10;


    }
    void fill(HMdcCal2ParAngleSim&);
    void setPar0(Double_t p0)   {par0=p0;}
    void setPar1(Double_t p1)   {par1=p1;}
    void setPar2(Double_t p2)   {par2=p2;}
    void setPar3(Double_t p3)   {par3=p3;}
    void setPar4(Double_t p4)   {par4=p4;}
    void setPar5(Double_t p5)   {par5=p5;}
    void setPar6(Double_t p6)   {par6=p6;}
    void setPar7(Double_t p7)   {par7=p7;}
    void setPar8(Double_t p8)   {par8=p8;}
    void setPar9(Double_t p9)   {par9=p9;}
    void setPar10(Double_t p10) {par10=p10;}

    void clear() {
      par0  =0.F;
      par1  =0.F;
      par2  =0.F;
      par3  =0.F;
      par4  =0.F;
      par5  =0.F;
      par6  =0.F;
      par7  =0.F;
      par8  =0.F;
      par9  =0.F;
      par10 =0.F;


    }
    ClassDef(HMdcCal2ParAngleSim,1) // Angle level of the MDC calibration parameters
};

class HMdcCal2ParModSim : public TObject {
protected:
    TObjArray *array;   // array of pointers of type HMdcCal2ParAngleSim
public:
    HMdcCal2ParModSim(Int_t sec = 0, Int_t mod = 0 , Int_t angle = 18);
    ~HMdcCal2ParModSim();
    HMdcCal2ParAngleSim& operator[](Int_t i) {
      return *static_cast<HMdcCal2ParAngleSim*>((*array)[i]);
    }
    Int_t getSize() {return array->GetEntries();}
    ClassDef(HMdcCal2ParModSim,1) // Module level of the MDC calibration parameters
};


class HMdcCal2ParSecSim : public TObject {
protected:
    TObjArray* array;   // array of pointers of type HMdcCal2ParModSim
public:
    HMdcCal2ParSecSim(Int_t sec = 0, Int_t mod = 4);
    ~HMdcCal2ParSecSim();
    HMdcCal2ParModSim& operator[](Int_t i) {
      return *static_cast<HMdcCal2ParModSim*>((*array)[i]);
    }
    Int_t getSize() {return array->GetEntries();}
    ClassDef(HMdcCal2ParSecSim,1) // Sector level of the MDC calibration parameters
};


class HMdcCal2ParSim : public HParSet {
protected:
    TObjArray* array;      // array of pointers of type HMdcCal2ParSecSim
    TF1* fdistanceToTime;
    TF1* fdistanceToTimeLin;
    Int_t sector;
    Int_t module;
    Float_t angleDeg;
    Float_t time;
    Int_t angleStep;
    Float_t distance;
public:
    HMdcCal2ParSim(Int_t n=6);
    ~HMdcCal2ParSim();
    HMdcCal2ParSecSim& operator[](Int_t i) {
        return *static_cast<HMdcCal2ParSecSim*>((*array)[i]);
    }
    Int_t getSize() {return array->GetEntries();}
    Bool_t init(HParIo*, Int_t*);
    Int_t write(HParIo*);
    void readline(const char*, Int_t*);
    void putAsciiHeader(TString&);
    Bool_t writeline(char*, Int_t, Int_t, Int_t);
    Float_t calcTime(Int_t ,Int_t ,Float_t,Float_t);
    void clear();
    ClassDef(HMdcCal2ParSim,1) // Container for the MDC calibration parameters
};

#endif  /*!HMDCCAL2PARSIM_H*/

