#ifndef HShowerDigiDetPar_H
#define HShowerDigiDetPar_H
#pragma interface

#include "hparset.h"
#include "hshowerparset.h"
#include "hlocation.h"
#include "hshowerpad.h"
#include "hshowerwire.h"
#include "hshowerframe.h"

class HParHadAsciiFileIo;

class HShowerDigiDetPar : public HShowerParSet {
public:
  HShowerDigiDetPar();
  ~HShowerDigiDetPar();

  virtual Bool_t initAscii(HParHadAsciiFileIo* pHadAsciiFile);
  virtual Bool_t writeAscii(HParHadAsciiFileIo* pHadAsciiFile);
  virtual void clear();

  Float_t getThickDet(){return fThickDet;} 
  Float_t getFwhmResponse(){return fFwhmResponse;} 
  Float_t getFactorInd(){return fFactorInd;}
  Float_t* getChargeMatrix(){return chargeMatrix;} 
  Int_t getPadRange(){return nPadRange;}
  Int_t getMatrixRange(){return nMatrixRange;}
  Float_t getBoxSize(){return fBoxSize;}
  Float_t getPlaneDist(){return fPlaneDist;}
  Int_t getNumElec(){return nNumElec;}
  Float_t getGainFact(){return fGainFact;}
  Float_t getTrackThreshold(){return fTrackThreshold;}

  void setThickDet(Float_t fThick){fThickDet = fThick;}
  void setFwhmResponse(Float_t fFwhm){fFwhmResponse = fFwhm;}
  void setFactorInd(Float_t fFact){fFactorInd = fFact;}
  void setPadRange(Int_t nRange){nPadRange = nRange;}
  void setMatrixRange(Int_t nRange){nMatrixRange = nRange;}
  void setBoxSize(Float_t fSize){fBoxSize = fSize;}
  void setPlaneDist(Float_t fDist){fPlaneDist = fDist;}
  void setNumElec(Int_t nElec){nNumElec = nElec;}
  void setGainFact(Float_t fGainF){fGainFact = fGainF;}
  void setTrackThreshold(Float_t fThreshold){fTrackThreshold = fThreshold;}

  float calcCharge(float charge, float dist, float Xd, float Yd, 
                                                float Xu , float Yu);
  void initChargeMatrix(); 

  ClassDef(HShowerDigiDetPar,1) //ROOT extension
public:
  float fThickDet;              //thickness of gas chamber
  float fFwhmResponse;          //fwhm of charge distribution
  float fFactorInd;		//charge factor induced on cathod plane
  float *chargeMatrix;          //charge density matrix
  Int_t nMatrixRange;           //size of charge density matrix
  Float_t fPlaneDist;           //plane cathod to sense wires plane distance
  Float_t fBoxSize;             //size of element of charge density matrix
  Int_t nPadRange;              //range of integration area
  Int_t nNumElec;               //number of primary electrons
  Float_t fGainFact;            //gain factor for the Pre-Shower chamber 
  Float_t fTrackThreshold;      //charge threshold for track category updating   
};

#endif
