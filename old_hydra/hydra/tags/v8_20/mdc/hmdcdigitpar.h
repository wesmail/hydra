#ifndef HMDCDIGITPAR_H
#define HMDCDIGITPAR_H
#include "TArrayF.h"
#include "hparcond.h"

class HParamList;

class HMdcDigitPar : public HParCond {
protected:
    TArrayF layEff;      // efficiency per module and layer
    Float_t signalspeed; // speed of signal on the wire (ns/mm)
public:
    HMdcDigitPar(const Char_t* name ="MdcDigitPar",
	      const Char_t* title  ="digitisation parameters for Mdc",
	      const Char_t* context="MdcDigitParProduction");
    ~HMdcDigitPar();
    Float_t getLayerEfficiency(Int_t s,Int_t m,Int_t l)             {return layEff[(s*24)+(m*6)+l];}
    void    setLayerEfficiency(Int_t s,Int_t m,Int_t l,Float_t eff) {layEff[(s*24)+(m*6)+l]=eff;}
    Float_t getSignalSpeed()                                        {return signalspeed;}
    void    setSignalSpeed(Float_t speed)                           {signalspeed=speed;}
    Bool_t  init(HParIo*, Int_t*);
    Int_t   write(HParIo*);
    void    putParams(HParamList*);
    Bool_t  getParams(HParamList*);
    void    clear();
    void    printParam(void);
    ClassDef(HMdcDigitPar,1) // Container for the MDC Digitizer parameters
};
#endif  /*!HMDCDIGITPAR_H*/
