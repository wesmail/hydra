//*-- Author: Alberica Toia
//*-- Modified: Joern Wuestenfeld 04/01/2005
//*-- Version: $Id: hmatchushower.h,v 1.11 2006-02-16 08:48:35 wuestenf Exp $

#ifndef HMATCHUSHOWER_H
#define HMATCHUSHOWER_H

#include <TObject.h>

class HMatchUShower : public TObject {
protected:
  Int_t fBuildId;     // shower IPC module
  Int_t fColumn;      // x coordinate of hit in detector coordinate system
  Int_t fRow;         // y coordinate of hit in detector coordinate system
  Int_t fRowPattern;  // pattern which encodes the y information of several hits
  Int_t fSector;      // sector in standard notation (0 - 5)
  Int_t fRowOffset;   // row offset
  Float_t fTheta;     // theta coordinate in lab coordinate system - [0..45] degree
  Float_t fPhi;       // phi coordinate in lab coordinate system - [0..360] degree

public:
  HMatchUShower(void);
  ~HMatchUShower(void);
  Int_t getBuildId(void) { return fBuildId; };
  Int_t getColumn(void) { return fColumn; };
  Int_t getRowPattern(void) { return fRowPattern; };
  Int_t getRow(void) { return fRow;};
  Int_t getRowOffset(void) { return fRowOffset;};
  Int_t getSector(void) { return fSector;};
  Float_t getTheta(void) { return fTheta;};
  Float_t getPhi(void) { return fPhi;};
  void setBuildID(Int_t buildId) { fBuildId=buildId; };
  void setColumn(Int_t col) { fColumn=col; };
  void setRowPattern(Int_t rowPat) { fRowPattern=rowPat; };
  void setRow(Int_t row) { fRow=row; };
  void setRowOffset(Int_t row) { fRowOffset=row; };
  void setSector(Int_t i){ fSector=i;};
  void setTheta(Float_t th){ fTheta=th;};
  void setPhi(Float_t ph){ fPhi=ph;};


  ClassDef(HMatchUShower,1)   // Datacontainer holding all Matching unit information from SHOWER
};

#endif
