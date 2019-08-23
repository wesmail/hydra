#ifndef HShowerHitFPar_H
#define HShowerHitFPar_H
#pragma interface

#include "hparset.h"
#include "hlocation.h"
#include "hshowerpad.h"
#include "hshowerwire.h"
#include "hshowerframe.h"

class HParHadAsciiFileIo;

class HShowerHitFPar : public HShowerParSet {
public:
  HShowerHitFPar(const Char_t* name="ShowerHitFinderParams",
		         const Char_t* title="Parameter for Shower hit finder",
				 const Char_t* context="ShowerStandardHitFinder");
  ~HShowerHitFPar();

  virtual Bool_t initAscii(HParHadAsciiFileIo* pHadAsciiFile);
  virtual Bool_t writeAscii(HParHadAsciiFileIo* pHadAsciiFile);
  virtual void clear();

  Int_t setColBord(Int_t nSect, Int_t nMod, Int_t nLowerCol, Int_t nUpperCol);
  Int_t setRowBord(Int_t nSect, Int_t nMod, Int_t nLowerRow, Int_t nUpperRow);
  Int_t getColBord(Int_t nSect, Int_t nMod, Int_t* nLowerCol, Int_t* nUpperCol);
  Int_t getRowBord(Int_t nSect, Int_t nMod, Int_t* nLowerRow, Int_t* nUpperRow);

  Int_t getModules(){return nModules;}
  Int_t getSectors(){return nSectors;}
  Int_t getThreshold(){return nThreshold;}

  Float_t getGainPost1(){return m_fGainPost1;}
  Float_t getThresholdPost1(){return m_fThresholdPost1;}
  Float_t getGainPost2(){return m_fGainPost2;}
  Float_t getThresholdPost2(){return m_fThresholdPost2;}

  Float_t *getFirstFitParam(){return m_pPar1;}
  Float_t *getSecondFitParam(){return m_pPar2;}
  Float_t  getMinTof() {return m_fMinTof;}
  Float_t  getMaxTof() {return m_fMaxTof;}
  Float_t  getScaleFactor() {return m_fScaleFactor;}
  Int_t    getParamTableSize() {return m_nTableSize;}   

  void setModules(Int_t nMod){nModules = nMod;}
  void setSectors(Int_t nSect){nSectors = nSect;}
  void setThreshold(Int_t t){nThreshold = t;}

  void setGainPost1(Float_t gain){m_fGainPost1 = gain;}
  void setThresholdPost1(Float_t threshold){m_fThresholdPost1 = threshold;}
  void setGainPost2(Float_t gain){m_fGainPost2 = gain;}
  void setThresholdPost2(Float_t threshold){m_fThresholdPost2 = threshold;}

  void setParamTableSize(Int_t iSize) {m_nTableSize = iSize;}   
  void setFirstFitParam(Float_t *par1 ) {m_pPar1 = par1;}
  void setSecondFitParam(Float_t *par2) {m_pPar2 = par2;}
  void setScaleFactor(Float_t fFactor) {m_fScaleFactor = fFactor;}
  
  void setMinTof(Float_t fMinTof) {m_fMinTof = fMinTof;}
  void setMaxTof(Float_t fMaxTof) {m_fMaxTof = fMaxTof;}
  

  ClassDef(HShowerHitFPar,2) //ROOT extension
public:
  void defaultInit();
  Int_t getIndex(Int_t nSect, Int_t nMod);

  Int_t nSectors;  //number of sectors
  Int_t nModules;  //number of modules
  Int_t *pRowBorder; //It defines active area for local maxima searching
  Int_t *pColBorder; //It defines active area for local maxima searching

  Int_t nThreshold; //Local maxima searching threshold

  Float_t m_fGainPost1;  //Gain parameter in shower criterium for post1
  Float_t m_fThresholdPost1; //Threshold parameter in shower criterium for post1

  Float_t m_fGainPost2;  //Gain parameter in shower criterium for post2
  Float_t m_fThresholdPost2; //Threshold parameter in shower criterium for post2

  
  Float_t m_fMinTof;
  Float_t m_fMaxTof;
  Int_t m_nTableSize;
  Float_t *m_pPar1;
  Float_t *m_pPar2;
  Float_t m_fScaleFactor;

};

#endif
