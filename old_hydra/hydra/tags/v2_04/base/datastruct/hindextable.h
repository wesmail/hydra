#pragma interface
#ifndef HINDEXTABLE_H
#define HINDEXTABLE_H

#include <TObject.h>
#include <TArrayI.h>

class HLocation;

class HIndexTable : public TObject {
protected:
  TArrayI fSizes; //Sizes of the dimensions of the index matrix
  TArrayI fIndexArray; //Index matrix
  Int_t fCurrentPos; //!Current position in the table for iterations.
public:
  HIndexTable(void);
  ~HIndexTable(void);
  void setDimensions(Int_t nDim,Int_t *sizes);
  Int_t getIndex(HLocation &aLoc);
  void setIndex(HLocation &aLoc,Int_t idx);
  TArrayI *getDimensions(void);
  const Int_t getIndex(Int_t linAddr);
  void setIndex(Int_t linAddr,Int_t idx);
  Int_t getEntries(void);
  Bool_t checkLocation(HLocation &aLoc);
  Int_t gotoLocation(HLocation &aLoc);
  Int_t gotoBegin(void);
  Int_t next(void);
  void Clear(void);
  ClassDef(HIndexTable,1) //Utility class handling an index table.
};

#endif /* !HINDEXTABLE_H */
