#ifndef HMATCHUSHOWER_H
#define HMATCHUSHOWER_H

#include "hdataobject.h"

class HMatchUShower : public HDataObject {
protected:
  Int_t fIPC;
  Int_t fColumn;
  Int_t fRow; 
  Int_t fRowPattern;
public:
  HMatchUShower(void);
  ~HMatchUShower(void);
  Int_t getIPC(void) { return fIPC; }
  Int_t getColumn(void) { return fColumn; }
  Int_t getRowPattern(void) { return fRowPattern; }
  Int_t getRow(void) { return fRow;}
  void setIPC(Int_t ipc) { fIPC=ipc; }
  void setColumn(Int_t col) { fColumn=col; }
  void setRowPattern(Int_t rowPat) { fRowPattern=rowPat; }
   void setRow(Int_t row) { fRow=row; }
  ClassDef(HMatchUShower,0)
};

#endif
