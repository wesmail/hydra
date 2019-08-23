// File: hmdcraw.h
//
// Author: Walter Karig <W.Karig@gsi.de>
// Last update: 98/12/09 17:31:52
//

#ifndef HMDCRAW_H
#define HMDCRAW_H

#include "hdataobject.h"

class HMdcRaw : public HDataObject {
public:
    HMdcRaw() { clear(); }
    ~HMdcRaw(){;}
    void clear();

    void setSector(const int s) { sector = s; }
    void setModule(const int m) { module = m; }
    void setMbo(const int m) { mbo = m; }
    void setTdc(const int t) { tdc = t; }
    void setTime(const int);

    int getSector() const { return sector; }
    int getModule() const { return module; }
    int getMbo() const { return mbo; }
    int getTdc() const { return tdc; }
    int getNrOfHits() const { return nrOfHits; }
    int getTime(int hit = 0) const;

private:
    Short_t sector;
    Short_t module;
    Short_t mbo;
    Short_t tdc;
    Int_t nrOfHits;
    Short_t time1;
    Short_t time2;
    Short_t time3;
    Short_t time4;
    Short_t time5;
    Short_t time6;

    ClassDef(HMdcRaw, 1) //Raw hit on a MDC (test class)
};

#endif /* !HMDCRAW_H */
