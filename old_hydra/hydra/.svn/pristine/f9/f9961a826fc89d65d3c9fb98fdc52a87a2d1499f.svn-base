#ifndef HMATCHUSHOWER_H
#define HMATCHUSHOWER_H

#include <TObject.h>

class HMatchUShower : public TObject {
protected:
  Int_t fBuildId;
  Int_t fColumn;
  Int_t fRow; 
  Int_t fRowPattern;
  Int_t fSector;
  Int_t fRowOffset; 
  Float_t fTheta;
  Float_t fPhi;

public:
  HMatchUShower(void);
  ~HMatchUShower(void);
  Int_t getBuildId(void) { return fBuildId; }
  Int_t getColumn(void) { return fColumn; }
  Int_t getRowPattern(void) { return fRowPattern; }
  Int_t getRow(void) { return fRow;}
  Int_t getRowOffset(void) { return fRowOffset;}
  Int_t getSector(void) { return fSector;}
  Float_t getTheta(void) { return fTheta;}
  Float_t getPhi(void) { return fPhi;}
  void setBuildID(Int_t buildId) { fBuildId=buildId; }
  void setColumn(Int_t col) { fColumn=col; }
  void setRowPattern(Int_t rowPat) { fRowPattern=rowPat; }
  void setRow(Int_t row) { fRow=row; }
  void setRowOffset(Int_t row) { fRowOffset=row; }
  void setSector(Int_t i){ fSector=i;}
  void setTheta(Float_t th){ fTheta=th;}
  void setPhi(Float_t ph){ fPhi=ph;}
 
  ClassDef(HMatchUShower,1)
};

#endif
