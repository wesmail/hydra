#ifndef HLOCATION_H
#define HLOCATION_H

#include <TObject.h>
#include <TArrayI.h>
#include "haddef.h"

class HLocation : public TObject {
protected:
  TArrayI fIndexes; //High indexes indicating the location
  Int_t fOffset; //Less significant index of the location
public:
  HLocation(void);
  HLocation(HLocation &aLoc);
  ~HLocation(void);
  void set(Int_t nIndex,...);
  void setNIndex(Int_t nIdx);
  void setOffset(Int_t aOfs);
  Int_t getIndex(Int_t aIdx);
  Int_t getUncheckedIndex(Int_t aIdx) {return fIndexes.fArray[aIdx];}
  void setIndex(Int_t aIdx,Int_t aValue);
  Int_t getLinearIndex(TArrayI *sizes);
  Int_t getNIndex(void); 
  Int_t getOffset(void);
  void incIndex(Int_t nIndex);
  void operator+=(Int_t shift);
  int operator++(void);
  void operator-=(Int_t shift);
  void operator--(void);
  Int_t &operator[](Int_t aIdx) {return fIndexes.fArray[aIdx];}
  HLocation &operator=(HLocation &loc);
  void readIndexes(HLocation &loc);
  void Dump(void);
  ClassDef(HLocation,1) //Indicator for a place in the event
};

#endif /* !HLOCATION_H */
