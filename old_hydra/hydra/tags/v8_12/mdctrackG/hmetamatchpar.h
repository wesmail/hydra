#ifndef HMETAMATCHPAR_H
#define HMETAMATCHPAR_H

#include "hparcond.h"
#include "TNamed.h"
#include "TArrayF.h"

class HMetaMatchPar : public HParCond {
protected:
  TArrayF richThetaMinCut;       // lower limit of delta theta cut in RICH MDC matching
  TArrayF richThetaMaxCut;       // upper limit of delta theta cut in RICH MDC matching
  TArrayF richSigmaPhi;          // sigma phi of RICH MDC matching
  TArrayF richSigmaPhiOffset;    // phi-offset for sigma in RICH MDC matching
  TArrayF richQualityCut;        // cut for quality RICH MDC matching
  
  TArrayF richIPUThetaMinCut;    // lower limit of delta theta cut in RICHIPU MDC matching
  TArrayF richIPUThetaMaxCut;    // upper limit of delta theta cut in RICHIPU MDC matching
  TArrayF richIPUSigmaPhi;       // sigma phi of RICHIPU MDC matching
  TArrayF richIPUSigmaPhiOffset; // phi-offset for sigma in RICHIPU MDC matching
  TArrayF richIPUQualityCut;     // cut for quality RICHIPU MDC matching
  
  TArrayF showerSigmaXMdc;       // sigma of MDC segment cross point of SHOWER
  TArrayF showerSigmaYMdc;       // sigma of MDC segment cross point of SHOWER
  TArrayF showerSigmaXOffset;    // x-offset for sigma in MDC SHOWER matching
  TArrayF showerSigmaYOffset;    // y-offset for sigma in MDC SHOWER matching
  TArrayF showerQualityCut;      // cut for quality of MDC SHOWER matching
  
  TArrayF tofSigmaX;             // sigma (x-dir) of MDC TOF matching
  TArrayF tofSigmaY;             // sigma (y-dir) of MDC TOF matching
  TArrayF tofSigmaXOffset;       // x-offset for sigma in MDC TOF matching
  TArrayF tofSigmaYOffset;       // y-offset for sigma in MDC TOF matching
  TArrayF tofQualityCut;         // cut for quality of MDC TOF matching
  
public:
  HMetaMatchPar(const char* name="MetaMatchPar",
                const char* title="Meta match parameters",
                const char* context="MetaMatchProductionCuts");
  ~HMetaMatchPar(void) {}
  Float_t getRichThetaMinCut(Int_t s)       {return richThetaMinCut[s];}
  Float_t getRichThetaMaxCut(Int_t s)       {return richThetaMaxCut[s];}
  Float_t getRichSigmaPhi(Int_t s)          {return richSigmaPhi[s];}
  Float_t getRichSigmaPhiOffset(Int_t s)    {return richSigmaPhiOffset[s];}
  Float_t getRichQualityCut(Int_t s)        {return richQualityCut[s];}
  
  Float_t getRichIPUThetaMinCut(Int_t s)    {return richIPUThetaMinCut[s];}
  Float_t getRichIPUThetaMaxCut(Int_t s)    {return richIPUThetaMaxCut[s];}
  Float_t getRichIPUSigmaPhi(Int_t s)       {return richIPUSigmaPhi[s];}
  Float_t getRichIPUSigmaPhiOffset(Int_t s) {return richIPUSigmaPhiOffset[s];}
  Float_t getRichIPUQualityCut(Int_t s)     {return richIPUQualityCut[s];}
  
  Float_t getShowerSigmaXMdc(Int_t s)       {return showerSigmaXMdc[s];}
  Float_t getShowerSigmaYMdc(Int_t s)       {return showerSigmaYMdc[s];}
  Float_t getShowerSigmaXOffset(Int_t s)    {return showerSigmaXOffset[s];}
  Float_t getShowerSigmaYOffset(Int_t s)    {return showerSigmaYOffset[s];}
  Float_t getShowerQualityCut(Int_t s)      {return showerQualityCut[s];}
  
  Float_t getTofSigmaX(Int_t s)             {return tofSigmaX[s];}
  Float_t getTofSigmaY(Int_t s)             {return tofSigmaY[s];}
  Float_t getTofSigmaXOffset(Int_t s)       {return tofSigmaXOffset[s];}
  Float_t getTofSigmaYOffset(Int_t s)       {return tofSigmaYOffset[s];}
  Float_t getTofQualityCut(Int_t s)         {return tofQualityCut[s];}
  
  void setRichThetaMinCut(Float_t v,Int_t s)    {richThetaMinCut.AddAt(v,s);}
  void setRichThetaMaxCut(Float_t v,Int_t s)    {richThetaMaxCut.AddAt(v,s);}
  void setRichSigmaPhi(Float_t v,Int_t s)       {richSigmaPhi.AddAt(v,s);}
  void setRichSigmaPhiOffset(Float_t v,Int_t s) {richSigmaPhiOffset.AddAt(v,s);}
  void setRichQualityCut(Float_t v,Int_t s)     {richQualityCut.AddAt(v,s);}
  
  void setRichIPUThetaMinCut(Float_t v,Int_t s)    {richIPUThetaMinCut.AddAt(v,s);}
  void setRichIPUThetaMaxCut(Float_t v,Int_t s)    {richIPUThetaMaxCut.AddAt(v,s);}
  void setRichIPUSigmaPhi(Float_t v,Int_t s)       {richIPUSigmaPhi.AddAt(v,s);}
  void setRichIPUSigmaPhiOffset(Float_t v,Int_t s) {richIPUSigmaPhiOffset.AddAt(v,s);}
  void setRichIPUQualityCut(Float_t v,Int_t s)     {richIPUQualityCut.AddAt(v,s);}
  
  void setShowerSigmaXMdc(Float_t v,Int_t s)    {showerSigmaXMdc.AddAt(v,s);}
  void setShowerSigmaYMdc(Float_t v,Int_t s)    {showerSigmaYMdc.AddAt(v,s);}
  void setShowerSigmaXOffset(Float_t v,Int_t s) {showerSigmaXOffset.AddAt(v,s);}
  void setShowerSigmaYOffset(Float_t v,Int_t s) {showerSigmaYOffset.AddAt(v,s);}
  void setShowerQualityCut(Float_t v,Int_t s)   {showerQualityCut.AddAt(v,s);}
  
  
  void setTofSigmaX(Float_t v,Int_t s)          {tofSigmaX.AddAt(v,s);}
  void setTofSigmaY(Float_t v,Int_t s)          {tofSigmaY.AddAt(v,s);}
  void setTofSigmaXOffset(Float_t v,Int_t s)    {tofSigmaXOffset.AddAt(v,s);}
  void setTofSigmaYOffset(Float_t v,Int_t s)    {tofSigmaYOffset.AddAt(v,s);}
  void setTofQualityCut(Float_t v,Int_t s)      {tofQualityCut.AddAt(v,s);}
  
  Bool_t init(HParIo*,Int_t* set=0);
  Int_t write(HParIo*);
  void clear(void);
  void putParams(HParamList*);
  Bool_t getParams(HParamList*);
  ClassDef(HMetaMatchPar,1) // parameter container for HMetaMatchF
};

#endif /* !HMETAMATCHPAR_H */
