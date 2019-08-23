#pragma interface
#ifndef HINDEXTABLE_H
#define HINDEXTABLE_H

#include <TObject.h>
#include <TArrayI.h>
#include "hlocation.h"
#include <TBuffer.h>

class HPairListI {
 private:
  Int_t fCapacity;
  Int_t fN;
  UShort_t *fArray[2];
 public:
  HPairListI(void) {fCapacity=0; fN=0; fArray[0]=fArray[1]=0;}
  ~HPairListI(void) { if (fCapacity>0) {delete[] fArray[0]; delete[] fArray[1];} }
  void setCapacity(Int_t n);
  void add(UShort_t i1,UShort_t i2) { 
    #if DEBUG_LEVEL==1
    if (fN<fCapacity) {
    #endif
    fArray[0][fN]=i1; fArray[1][fN]=i2;
    fN++;
    #if DEBUG_LEVEL==1
    }
    #endif
  }
  void remove(Int_t idx);
  Int_t getN(void) {return fN;}
  UShort_t &getIndex1(Int_t idx) {return fArray[0][idx]; } 
  UShort_t &getIndex2(Int_t idx) {return fArray[1][idx]; }
  void clear(void) {fN=0;} //The default value of a pair is not granted to be 0
  void Streamer(TBuffer& b);
};

class HIndexTable : public TObject {
protected:
  TArrayI fSizes; //Sizes of the dimensions of the index matrix
  HPairListI fCompactTable; //Table of linear addres -- index pairs
  TArrayI fIndexArray; //!Index matrix
  Int_t fCurrentPos; //!Current position in the table for iterations.
public:
  HIndexTable(void);
  ~HIndexTable(void);
  void setDimensions(Int_t nDim,Int_t *sizes);
  Int_t getIndex(HLocation &aLoc) { return (fIndexArray[aLoc.getLinearIndex(&fSizes)]); }
  void setIndex(HLocation &aLoc,Int_t idx) { 
    int la=aLoc.getLinearIndex(&fSizes);
    fCompactTable.add(la,idx);
    fIndexArray[la]=idx; 
  }
  TArrayI *getDimensions(void) {return &fSizes;}
  const Int_t getIndex(Int_t linAddr) {return fIndexArray[linAddr];}
  void setIndex(Int_t linAddr,Int_t idx) { 
    fIndexArray[linAddr]=idx; 
    fCompactTable.add(linAddr,idx); 
  }
  Int_t getEntries(void) { return fIndexArray.fN; }
  Bool_t checkLocation(HLocation &aLoc);
  Int_t gotoLocation(HLocation &aLoc);
  Int_t gotoBegin(void);
  Int_t next(void);
  void Clear(void);
  ClassDef(HIndexTable,1) //Utility class handling an index table.
};

#endif /* !HINDEXTABLE_H */
