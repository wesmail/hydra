#ifndef HMATCHURICH_H
#define HMATCHURICH_H

#include "hdataobject.h"

class HMatchURich : public TObject {
protected:
  Int_t fFifoNb;
  Int_t fColumnPattern;
  Int_t fSegmentId;
  Int_t fRowNb;
  Int_t fColumn;
  Int_t fRow;
public:
  HMatchURich(void);
  ~HMatchURich(void);
  Int_t getFifo(void) { return fFifoNb; }
  Int_t getColumnPattern(void) { return fColumnPattern; }
  Int_t getSegmentId(void) { return fSegmentId; }
  Int_t getRowNb(void) {return fRowNb; }
  Int_t getRow(void) {return fRow; }
  Int_t getColumn(void) { return fColumn; }
  void setFifo(Int_t fifo) { fFifoNb=fifo; }
  void setColumnPattern(Int_t colPat) { fColumnPattern=colPat; }
  void setSegmentId(Int_t segId) { fSegmentId=segId; }
  void setRowNb(Int_t row) { fRowNb=row; }
  void setColumn(Int_t col) { fColumn=col; }
  void setRow(Int_t row) { fRow=row; }
  ClassDef(HMatchURich,0)
};

#endif
