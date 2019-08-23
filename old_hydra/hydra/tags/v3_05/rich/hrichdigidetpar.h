#ifndef HRichDigiDetPar_H
#define HRichDigiDetPar_H
#pragma interface

#include "hparset.h"
#include "hrichparset.h"
#include "hlocation.h"
#include "hrichpad.h"
#include "hrichwire.h"
#include "hrichframe.h"

class HRichDigiDetPar : public HRichParSet {
public:
  HRichDigiDetPar();
  ~HRichDigiDetPar();

  virtual Bool_t initAscii(HParHadAsciiFileIo* pHadAsciiFile);
  virtual void clear();


  HRichFrame* getFrame(Int_t n){return frames.getFrame(n);} 
  Int_t getNFrames(){return frames.getNFrames();}
  HRichWireTab* getWireTab(){return &wires;}
  HRichPadTab* getPadParam(){return &pads;}
  Float_t getNumElec(){return numElec;} 
  Float_t getFactorInd(){return fFactorInd;}
  Float_t* getChargeMatrix(){return chargeMatrix;} 

  Int_t getPadRange(){return nPadRange;}
  Int_t getMatrixRange(){return nMatrixRange;}
  Float_t getBoxSize(){return fBoxSize;}
  Float_t getPlaneDist(){return fPlaneDist;}
  void setPadRange(Int_t nRange){nPadRange = nRange;}
  void setMatrixRange(Int_t nRange){nMatrixRange = nRange;}
  void setBoxSize(Float_t fSize){fBoxSize = fSize;}
  void setPlaneDist(Float_t fDist){fPlaneDist = fDist;}

  float calcCharge(float charge, float dist, float Xd, float Yd, 
                                                float Xu , float Yu);
  void initChargeMatrix(); 

  ClassDef(HRichDigiDetPar,1) //ROOT extension
public:
  int detNr;
  float numElec;
  float fFactorInd;
  float *chargeMatrix;
  Int_t nMatrixRange;
  Float_t fPlaneDist;
  Float_t fBoxSize;
  Int_t nPadRange;

  int binNr;
  float *photlen;
  float *photeff;

  HRichFrameTab frames;
  HRichWireTab wires;
  HRichPadTab pads;

};

#endif
