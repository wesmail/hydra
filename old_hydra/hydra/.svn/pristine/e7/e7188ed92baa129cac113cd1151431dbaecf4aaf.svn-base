#ifndef HMDCDEDX_H
#define HMDCDEDX_H
#include "TArrayF.h"
#include "TH2.h"
#include "TCanvas.h"
#include "hparcond.h"
#include "hlocation.h"

class HParamList;
class HLocation;
class HMatrixCategory;
class HLinearCategory;
class HMdcSeg;

class HMdcDeDx : public HParCond {
protected:
     Float_t shift[6][4][18][100]; // sector,module,angle,distance
     Float_t shiftsigma[6][4][18][100]; // sector,module,angle,distance
     HMatrixCategory* catcal     ; //!
     HMatrixCategory* cathit     ; //!
     HLinearCategory* catclusinf ; //!
     HLinearCategory* catclusfit ; //!
     HLinearCategory* catwirefit ; //!
     HLocation loccal;             //!
     Int_t minimumWires;           // minimum required wires
     Float_t window;               // window for truncating arround mean (units of sigma)
public:
    HMdcDeDx(const char* name ="MdcDeDx",
             const char* title  ="Mdc lookup table for wire stat",
             const char* context="MdcDeDxProduction");
    ~HMdcDeDx();

    Bool_t   init(HParIo*, Int_t*);
    Int_t    write(HParIo*);
    void     putParams(HParamList*);
    Bool_t   getParams(HParamList*);
    void     clear();
    void     printParam(void);
    Float_t  getShiftSegment(Int_t,Int_t,Float_t,Float_t);
    Float_t  getShiftSigmaSegment(Int_t,Int_t,Float_t,Float_t);
    Float_t  getShift(Int_t,Int_t,Int_t,Int_t);
    void     setShift(Int_t,Int_t,Int_t,Int_t,Float_t);
    Float_t  getShiftSigma(Int_t,Int_t,Int_t,Int_t);
    void     setShiftSigma(Int_t,Int_t,Int_t,Int_t,Float_t);
    Float_t  getWindow()           {return window;}
    void     setWindow(Float_t win){window=win;}
    Int_t    getMinimumWires()              {return minimumWires;}
    void     setMinimumWires(Int_t minwires){minimumWires=minwires;}
    TCanvas* plotShift(Int_t,Int_t);
    void     sort(Int_t,Float_t*);
    Float_t  calcDeDx(HMdcSeg*,Float_t*,Float_t*,Int_t*,Float_t*,Int_t*);
    Int_t    fillingInput(HMdcSeg* seg,Float_t* );
    Float_t  calcMean(Float_t*,Int_t);
    Float_t  calcSigma(Float_t*,Int_t,Float_t);
    Int_t    select(Float_t*,Float_t,Float_t,Int_t);
    ClassDef(HMdcDeDx,1) //  MDC lookup table (dedx normalization parameters)
};
#endif  /*!HMDCDEDX_H*/
