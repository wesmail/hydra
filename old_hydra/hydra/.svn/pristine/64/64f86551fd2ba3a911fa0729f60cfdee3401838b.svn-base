#ifndef HSTARTRAW_H
#define HSTARTRAW_H

#include "TObject.h"

#define TYPE_DIAMOND   -1
#define TYPE_DIAMOND_NEW 0
#define TYPE_LEFT      1
#define TYPE_RIGHT     2
#define TYPE_MEANTIMER 3

class HStartRaw : public TObject {
protected:
    Short_t time;  // tdc time - diamond detectors
    Short_t timeR;  // tdc right-side time - hodoscope
    Short_t timeL;  // tdc left-side time  - hodoscope
    Short_t timeM;  // meantimer time      - hardware meantimer

    Int_t module;  // module number
    Int_t strip;   // strip number
    Int_t nType;    // signal(readout scheme) type (see defines above)
public:
    HStartRaw(void) { clear(); }
    ~HStartRaw(void) {;}
    void setTime(const Short_t t) { time=t; }
    void setTimeR(const Short_t t) { timeR=t; }
    void setTimeL(const Short_t t) { timeL=t; }
    void setTimeM(const Short_t t) { timeM=t; }
    void setTime(const Int_t t) { time=(Short_t)t; }
    void setModule(const Int_t n) { module=n; }
    void setStrip(const Int_t n)  { strip=n; }
    void setType(const  Int_t n)  { nType=n; }
    void setAddress(const Int_t m,const Int_t s) {
      module=m;
      strip=s;
    }
    void fill(const Short_t t, const Int_t m,const Int_t s) {
      time=t;
      module=m;
      strip=s;
    }
    void fill(const Short_t t, const Int_t m,const Int_t s, Int_t n) {
        module=m;
        strip=s;
        nType = n;
        switch (nType){
            case TYPE_RIGHT:
                timeR=t;
                break;
            case TYPE_LEFT:
                timeL=t;
                break;
            case TYPE_MEANTIMER:
                timeM=t;
                break;
            default:
                break;
        }
    }

    Short_t getTime(void) const { return time; }
    Short_t getTimeR(void) const { return timeR; }
    Short_t getTimeL(void) const { return timeL; }
    Short_t getTimeM(void) const { return timeM; }
    Int_t getModule(void) const { return module; }
    Int_t getStrip(void) const { return strip; }
    Int_t  getType(void) const { return nType;}
    void getAddress(Int_t& m,Int_t& s) {
      m=module;
      s=strip;
    }
    void clear(void) {
      time=-1;
      timeR=-1;
      timeL=-1;
      timeM=-1;
      module=-1;
      strip=-1;
      nType=-1;
    }

    ClassDef(HStartRaw,2)  // START detector raw data
};

#endif /* ! HSTARTRAW_H */










