#ifndef HMDCGEOMSTRUCT_H
#define HMDCGEOMSTRUCT_H

#include "TObject.h"
#include "TObjArray.h"
#include "TArrayI.h"
#include "hparset.h"

class HParIo;

class HMdcGeomModStru: public TObject {
protected:
  Int_t nCells[6];  // number of cells in the layers
public:
  HMdcGeomModStru() {}
  ~HMdcGeomModStru() {}
  Int_t operator [](Int_t i) {return nCells[i];}
  void fill(Int_t i, Int_t n) {if (i<6) nCells[i]=n;}
  void clear() {for(Int_t i=0;i<6;i++) nCells[i] = 0;}
  ClassDef(HMdcGeomModStru,1) // Layer level of the MdcGeomStruct container
};


class HMdcGeomSecStru: public TObject {
protected:
  TObjArray* array;  // pointer array containing HMdcGeomModStru objects
public:
  HMdcGeomSecStru(Int_t n=4);
  ~HMdcGeomSecStru();
  HMdcGeomModStru& operator[](Int_t i) {
    return *static_cast<HMdcGeomModStru*>((*array)[i]);
  }
  ClassDef(HMdcGeomSecStru,1) // Sector level of the MdcGeomStruct container
};


class HMdcGeomStruct : public HParSet {
protected:
  TObjArray* array;  // pointer array containing HMdcGeomSecStru objects
public:
  HMdcGeomStruct(Int_t n=6);
  ~HMdcGeomStruct();
  HMdcGeomSecStru& operator[](Int_t i) {
    return *static_cast<HMdcGeomSecStru*>((*array)[i]);
  }
  Bool_t init(HParIo* input,Int_t* set);
  Int_t write(HParIo* output);
  void clear();
  void printParam();
  void getMaxIndices(TArrayI*);
  ClassDef(HMdcGeomStruct,1) // MdcGeomStruct container for number of wires per layer
};

#endif /* !HMDCGEOMSTRUCT_H */



