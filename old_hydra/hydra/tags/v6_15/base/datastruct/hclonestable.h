#ifndef HCLONESTABLE_H
#define HCLONESTABLE_H

#include <TClonesArray.h>
#include <TArrayI.h>

class HClonesTable : public TClonesArray {
protected:
  TArrayI fTable; //Lookup table for the array
  Int_t fNObjects;
public:
  HClonesTable(void);
  HClonesTable(Text_t *className,Int_t capacity,Int_t size=1000,
	       Int_t callDtor=kFALSE);
  ~HClonesTable(void);
  void Clear(Option_t *option="");
  void Delete(Option_t *option="");
  TObject* RemoveAt(Int_t idx);
  TObject *Remove(TObject *obj);
  void Sort(Int_t upto=kMaxInt);
  TObject *&operator[](Int_t idx);
  TObject *At(Int_t idx);
  Int_t getCapacity(void) {return fTable.fN;}
  void Compress(void);
  ClassDef(HClonesTable,1) //Table of clone objects
};

#endif /* !HCLONESTABLE */
