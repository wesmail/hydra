#ifndef HShowerDigiDetPar_H
#define HShowerDigiDetPar_H
#pragma interface

#include "hparset.h"
#include "hlocation.h"
#include "hadioascii.h"
#include "hshowerpad.h"
#include "hshowerwire.h"
#include "hshowerframe.h"

class HShowerDigiDetPar : public HParSet {
public:
  HShowerDigiDetPar();
  ~HShowerDigiDetPar();

  void setHadIoAscii(HadIoAscii *pReadParam);
  HadIoAscii* getHadIoAscii();

  virtual Bool_t init();
  virtual void clear();

  HShowerFrame* getFrame(Int_t n){return &frames[n];} 
//  Int_t getNFrames(){return frames.getNFrames();}
  HShowerWireTab* getWireTab(Int_t nMod){return &wires[nMod];}
  HShowerPadTab* getPadParam(Int_t nMod){return &pads[nMod];}
  Float_t getThickDet(){return fThickDet;} 
  Float_t getFwhmResponse(){return fFwhmResponse;} 
  Float_t getFactorInd(){return fFactorInd;}
  Float_t* getChargeMatrix(){return chargeMatrix;} 

  Int_t getPadRange(){return nPadRange;}
  Int_t getMatrixRange(){return nMatrixRange;}
  Float_t getBoxSize(){return fBoxSize;}
  Float_t getPlaneDist(){return fPlaneDist;}
  Int_t getModules(){return nModules;}
  Int_t getNumElec(){return nNumElec;}

  void setPadRange(Int_t nRange){nPadRange = nRange;}
  void setMatrixRange(Int_t nRange){nMatrixRange = nRange;}
  void setBoxSize(Float_t fSize){fBoxSize = fSize;}
  void setPlaneDist(Float_t fDist){fPlaneDist = fDist;}
  void setModules(Int_t nMod){nModules = nMod;}
  void setThickDet(Float_t fThick){fThickDet = fThick;}
  void setFwhmResponse(Float_t fFwhm){fFwhmResponse = fFwhm;}
  void setNumElec(Int_t nElec){nNumElec = nElec;}

  float calcCharge(float charge, float dist, float Xd, float Yd, 
                                                float Xu , float Yu);
  void initChargeMatrix(); 

  ClassDef(HShowerDigiDetPar,1) //ROOT extension
public:
  int detNr;
  float fThickDet;
  float fFwhmResponse;
  float fFactorInd;
  float *chargeMatrix;
  Int_t nMatrixRange;
  Float_t fPlaneDist;
  Float_t fBoxSize;
  Int_t nPadRange;
  Int_t nNumElec;
  
  Int_t nModules;

  HShowerFrame frames[3];
  HShowerWireTab wires[3];//max. 3 modules
  HShowerPadTab pads[3];

  HadIoAscii *m_pReadParam;
};

#endif
