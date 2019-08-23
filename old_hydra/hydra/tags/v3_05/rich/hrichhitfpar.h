#ifndef HRICHHITFPAR_H
#define HRICHHITFPAR_H
#pragma interface

#include "hparset.h"
#include "hlocation.h"
#include "hrichpad.h"
#include "hrichwire.h"
#include "hrichframe.h"

class HParHadAsciiFileIo;

class HRichHitFPar : public HRichParSet {

public:

  HRichHitFPar();
  ~HRichHitFPar();

  virtual Bool_t initAscii(HParHadAsciiFileIo* pHadAsciiFile);
  virtual void clear();

  Int_t setColBord(Int_t nSect, Int_t nLowerCol, Int_t nUpperCol);
  Int_t setRowBord(Int_t nSect, Int_t nLowerRow, Int_t nUpperRow);
  Int_t getColBord(Int_t nSect, Int_t* nLowerCol, Int_t* nUpperCol);
  Int_t getRowBord(Int_t nSect, Int_t* nLowerRow, Int_t* nUpperRow);

  Int_t getSectors() { return nSectors; }
  Int_t getThreshold() { return nThreshold; }

  void setSectors(Int_t nSect) { nSectors = nSect; }
  void setThreshold(Int_t t) { nThreshold = t; }

  ClassDef(HRichHitFPar,1) //ROOT extension

public:

  void defaultInit();
  Int_t getIndex(Int_t nSect);

  Int_t nSectors;
  Int_t *pRowBorder;
  Int_t *pColBorder;

  Int_t nThreshold;
};

#endif // HRICHHITFPAR_H
