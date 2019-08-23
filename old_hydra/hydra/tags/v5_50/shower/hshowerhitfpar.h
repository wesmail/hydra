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
  HShowerHitFPar(const char* name="ShowerHitFinderParams",
                 const char* title="Parameter for Shower hit finder",
                 const char* context="");
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

  void setModules(Int_t nMod){nModules = nMod;}
  void setSectors(Int_t nSect){nSectors = nSect;}
  void setThreshold(Int_t t){nThreshold = t;}

  void setGainPost1(Float_t gain){m_fGainPost1 = gain;}
  void setThresholdPost1(Float_t threshold){m_fThresholdPost1 = threshold;}
  void setGainPost2(Float_t gain){m_fGainPost2 = gain;}
  void setThresholdPost2(Float_t threshold){m_fThresholdPost2 = threshold;}

  ClassDef(HShowerHitFPar,1) //ROOT extension
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
};

#endif
