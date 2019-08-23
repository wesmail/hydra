#ifndef HRTSAVGOLPAR_H
#define HRTSAVGOLPAR_H

#include "hparcond.h"

class HParIo;
class HParamList;

class HRtSavGolPar : public HParCond {
  public:
    HRtSavGolPar(const char *name="RtSavGolPar",
	const char *title="Savitzky-Golay parameters",
	const char *context="RtProduction");
    virtual ~HRtSavGolPar(void) { }
    Bool_t init(HParIo *inp, Int_t *set);
    Int_t write(HParIo *io);
    void putParams(HParamList*);
    Bool_t getParams(HParamList*);
    Int_t getNLeft(void) { return fNLeft; }
    Int_t getNRight(void) { return fNRight; }
    Int_t getM(void) { return fM; }
    void setParams(Int_t nl,Int_t nr,Int_t m) {fNLeft=nl; fNRight=nr; fM=m;}
    void readFrom(const HRtSavGolPar &o);
  protected:
    Int_t fNLeft, fNRight;
    Int_t fM;
  public:
    ClassDef(HRtSavGolPar,1)
};

#endif
