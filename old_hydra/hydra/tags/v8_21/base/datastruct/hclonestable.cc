#include "hclonestable.h"
#include "TArrayI.h"

//*-- Author : Manuel Sanchez
//*-- Modified : 22/09/98 by Manuel Sanchez

//_HADES_CLASS_DESCRIPTION 
////////////////////////////////////////
//HClonesTable
//
//  The HClonesTable extends the TClonesArray by allowing to have holes in
//within. However, for the empty locations only 4 bytes of memory are used
//instead of the whole size of one object.
///////////////////////////////////////

HClonesTable::HClonesTable(void) {
  //Constructor
}

HClonesTable::HClonesTable(Text_t *className,Int_t capacity,Int_t size,
			   Int_t callDtor) : 
  //Constructor. See TClonesArray.
  TClonesArray(className,size,callDtor), fTable(capacity),fNObjects(0) {
    Int_t i;
    for (i=0;i<capacity;i++) fTable[i]=-1;
}

HClonesTable::~HClonesTable(void) {
  //Destructor
  fNObjects=0;
}

void HClonesTable::Clear(Option_t *) {
  //see TCollection
  Int_t i;
  TClonesArray::Clear();
  for (i=0;i<fTable.fN;i++) fTable.fArray[i]=-1;
  fNObjects=0;
}

void HClonesTable::Delete(Option_t *) {
  //see TCollection
  Int_t i;
  TClonesArray::Delete();
  for (i=0;i<fTable.fN;i++) fTable.fArray[i]=-1;
  fNObjects=0;
}

TObject* HClonesTable::RemoveAt(Int_t idx) {
  //Remove object at position idx.
  //Anhadir index checking?
  if (fTable[idx]>-1) {
    TObject* out= TClonesArray::RemoveAt(fTable.fArray[idx]);
    fTable.fArray[idx]=-1;
    return out;  
  }
  return 0;
  //fNObjects--?. Crea un agujero (lo tapamos?)
}

TObject *HClonesTable::Remove(TObject *) {
  //Removes the object given as a parameter.
  Warning("HClonesTable::Remove","Not yet implemented");
  return NULL;
}

void HClonesTable::Sort(Int_t) {
  //see TCollection
  Warning("HClonesTable::Sort","not yet implemented");
}

TObject *&HClonesTable::operator[](Int_t idx) {
  //see TClonesArray
  Int_t n;
  if (fTable[idx]>-1) n=fTable[idx];
  else {n=fNObjects++; fTable[idx]=n;}
  return TClonesArray::operator[](n);
}

TObject *HClonesTable::At(Int_t idx) {
  //see TClonesArray
  if (idx<fTable.fN) {
    return (fTable.fArray[idx]>-1)?(TClonesArray::UncheckedAt(fTable.fArray[idx])):((TObject *)NULL);
  } else return NULL; 
}

void HClonesTable::Compress(void) {
  //see TClonesArray::Compress()
  Warning("HClonesTable::Compress","not yet implemented");
}

ClassImp(HClonesTable)


