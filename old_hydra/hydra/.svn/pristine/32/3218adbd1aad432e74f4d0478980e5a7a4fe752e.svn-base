//*-- Author: Alberica Toia
//*-- Modified: Joern Wuestenfeld 04/01/2005
//*-- Version: $Id: hmatchurich.h,v 1.7 2005-07-15 09:00:24 wuestenf Exp $

#ifndef HMATCHURICH_H
#define HMATCHURICH_H

#include <TObject.h>

class HMatchURich : public TObject
{
protected:
  Int_t fFifoNb;      // Fifo which has computed the ring finder
  Int_t fSegmentId;   // sector number in standard notation (0 - 5)

  // The expressions "row" and "column" below refer to the hardware
  // notation. The standard HADES "row" and "column" are the other
  // way round
  Int_t fColumnPattern; // pattern which encodes the y information of several hits
  Int_t fRowNb;         // row + offset
  Int_t fColumn;        // x coordinate of ring center in hardware notation
  Int_t fRow;           // y coordinate of ring center in hardware notation

  // The following "X", "Y", "Theta" and "Phi" are consistent with
  // the standard HADES notation.
  Float_t fTheta;       // theta coordinate in lab coordinate system
  Float_t fPhi;         // phi coordinate in lab coordinate system
  Int_t iRingX;         // x coordinate of ring center in standard detector coordinate system
  Int_t iRingY;         // y coordinate of ring center in standard detector coordinate system

public:
  HMatchURich(void);
  ~HMatchURich(void);
  Int_t getFifo(void) { return fFifoNb; };
  Int_t getColumnPattern(void) { return fColumnPattern; };
  Int_t getSegmentId(void) { return fSegmentId; };
  Int_t getSector(void) {return fSegmentId;};
  Int_t getRowNb(void) {return fRowNb; };
  Int_t getRow(void) {return fRow; };
  Int_t getColumn(void) { return fColumn; };
  Float_t getTheta(void) { return fTheta; };
  Float_t getPhi(void) { return fPhi; };
  Int_t getX(void) {return iRingX;};
  Int_t getY(void) {return iRingY;};

  void setFifo(Int_t fifo) { fFifoNb=fifo; };
  void setColumnPattern(Int_t colPat) { fColumnPattern=colPat; };
  void setSegmentId(Int_t segId) { fSegmentId=segId; };
  void setRowNb(Int_t row) { fRowNb=row; };
  void setColumn(Int_t col) { fColumn=col; };
  void setRow(Int_t row) { fRow=row; };
  void setTheta(Float_t th) { fTheta=th; };
  void setPhi(Float_t ph) { fPhi=ph; };
  void setX(Int_t px) { iRingX = px; };
  void setY(Int_t py) { iRingY = py; };

  ClassDef(HMatchURich,1)   // Data container holding all Matching unit information from RICH
};

#endif
