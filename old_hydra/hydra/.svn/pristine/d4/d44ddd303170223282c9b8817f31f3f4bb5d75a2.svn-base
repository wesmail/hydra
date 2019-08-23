#ifndef HMDCDEDX_H
#define HMDCDEDX_H
#include "TArrayF.h"
#include "TH2.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "hparcond.h"
#include "hlocation.h"

class HParamList;
class HLocation;
class HCategory;
class HMdcSeg;
class HMdcSizesCells;
class HGeantKine;

class HMdcDeDx : public HParCond {
protected:
     HMdcSizesCells *sizescells;   //! pointer to HMdcSizesCells container

     Float_t shift[6][4][18][100]; // sector,module,angle,distance
     Float_t shiftsigma[6][4][18][100]; // sector,module,angle,distance
     Double_t ref_dedx;            // reference value for electrons (for scaling)
     Double_t shift_dedx[6][4];    // shift theoretical dedx (in betagamma axis)
     Double_t scale_dedx[6][4];    // scale theoretical dedx (in dedx axis)
     Double_t hefr;                // fraction of helium of the gas mix
     HCategory* catcal     ;       //! pointer to mdc cal1
     HCategory* cathit     ;       //! pointer to mdc hit
     HCategory* catclusinf;        //! pointer to mdc clusinf
     HCategory* catclusfit;        //! pointer to mdc clusfit
     HCategory* catwirefit;        //! pointer to mdc wirefit
     HLocation loccal;             //! location object of cal1
     Int_t minimumWires;           // minimum required wires
     Float_t window;               // window for truncating arround mean (units of sigma)
     Bool_t isInitialized;         // flag after init
     Int_t method;                 //! method switch for filling input
     Int_t module;                 //! method switch for filling input for module 1/2 of segment
     Float_t measurements[50];     //! array of measurements
     Bool_t  useShifts;            //! use/don't use normaization table
     Int_t ctskipmod0;             //! counter for wires skipped with t2<=-998 in mod0 of seg
     Int_t ctskipmod1;             //! counter for wires skipped with t2<=-998 in mod1 of seg
     void     sort(Int_t,Float_t*);
     UChar_t  fillingInput(HMdcSeg* seg,Float_t* );
     UChar_t  fillingInputClusFit(HMdcSeg* seg,Float_t* );
     Float_t  calcMean(Float_t*,UChar_t);
     Float_t  calcSigma(Float_t*,UChar_t,Float_t);
     UChar_t  select(Float_t*,Float_t,Float_t,UChar_t);
     void     calcSegPoints(HMdcSeg *,Double_t&,Double_t&,Double_t&,Double_t&,Double_t&,Double_t&);
     void     initArray(){for(UInt_t i=0;i<sizeof(measurements)/sizeof(Float_t);i++){measurements[i]=-99.;}}

public:
    HMdcDeDx(const Char_t* name ="MdcDeDx",
             const Char_t* title  ="Mdc lookup table for wire stat",
             const Char_t* context="MdcDeDxProduction");
    ~HMdcDeDx();

    Bool_t   init(HParIo*, Int_t*);
    Bool_t   initContainer();
    Int_t    write(HParIo*);
    void     putParams(HParamList*);
    Bool_t   getParams(HParamList*);
    void     clear();
    void     printParam(TString opt="all");
    Float_t  getShiftSegment     (Int_t,Int_t,Float_t,Float_t);
    Float_t  getShiftSigmaSegment(Int_t,Int_t,Float_t,Float_t);
    Float_t  getShift            (Int_t,Int_t,Int_t,Int_t);
    void     setShift            (Int_t,Int_t,Int_t,Int_t,Float_t);
    Float_t  getShiftSigma       (Int_t,Int_t,Int_t,Int_t);
    void     setShiftSigma       (Int_t,Int_t,Int_t,Int_t,Float_t);
    Float_t  getWindow()                    {return window;}
    void     setWindow(Float_t win)         {window=win;}
    Int_t    getMinimumWires()              {return minimumWires;}
    void     setMinimumWires(Int_t minwires){minimumWires=minwires;}
    void     setUseShifts(Bool_t ok)        {useShifts=ok;}
    Float_t* getArray (Int_t& size)         {size=sizeof(measurements)/sizeof(Float_t);return &measurements[0];}
    TCanvas* plotShift(Int_t,Int_t);
    Float_t  calcDeDx(HMdcSeg*,Float_t*,Float_t*,UChar_t*,Float_t*,UChar_t*,Int_t vers=0,Int_t mod=2 );
    //------------energy loss-----------------------
    void     setRefEnergyLoss(Double_t r)   {ref_dedx=r;}
    Double_t getRefEnergyLoss()             {return ref_dedx;}
    void     setShiftEnergyLoss(Int_t s,Int_t m,Double_t sh){shift_dedx[s][m]=sh;}
    Double_t getShiftEnergyLoss(Int_t s,Int_t m)            {return shift_dedx[s][m];}
    void     setScaleEnergyLoss(Int_t s,Int_t m,Double_t sc){scale_dedx[s][m]=sc;}
    Double_t getScaleEnergyLoss(Int_t s,Int_t m)            {return scale_dedx[s][m];}
    void     setHeFraction(Double_t fr)     {hefr=fr;}
    Double_t getHeFraction()                {return hefr;}
    static Double_t  energyLoss     (Int_t id,Double_t p,Double_t hefr=0.6);
    static TGraph*   energyLossGraph(Int_t id,Double_t hefr=0.6,TString opt="p",Bool_t exchange=kFALSE,Int_t markerstyle=8,Int_t markercolor=2,Float_t markersize=0.7);
    Double_t         scaledTimeAboveThreshold(HGeantKine* kine,Double_t p,Int_t s,Int_t m,Float_t alpha,Float_t mindist);
    ClassDef(HMdcDeDx,1) //  MDC lookup table (dedx normalization parameters)
};
#endif  /*!HMDCDEDX_H*/
