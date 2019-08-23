#ifndef HPARAMLIST_H
#define HPARAMLIST_H

#include "TNamed.h"
#include "TString.h"
#include "TArrayI.h"
#include "TArrayC.h"
#include "TArrayF.h"
#include "TArrayD.h"
#include "TList.h"

class HParamObj : public TNamed {
protected:
  TString paramValue;  // Parameter value(s) a string
  char paramType;      // Basic type of the parameter
  Int_t nParam;        // Number of parameters
public:
  HParamObj() {nParam=0;}
  HParamObj(const Text_t*,const Text_t*,const char,const Int_t);
  ~HParamObj() {}
  void setParamValue(const Text_t* v) { paramValue=v; }
  void setParamType(const char t) { paramType=t; }
  void setNumParams(const char n) { nParam=n; }
  const char* getParamValue() { return paramValue.Data(); }
  const char getParamType() { return paramType; }
  Int_t getNumParams() { return nParam; }
  void print();  
  ClassDef(HParamObj,0) // Class for parameter object (name + string-value)
};

class HParamBinObj : public TNamed {
protected:
  UChar_t* paramValue;  // Pointer to binary array
  Int_t arraySize;      // Size of binary array
  TString paramType;    // Type of original parameter array or class name
  Bool_t basicType;     // kTRUE for basic type parameter arrays, kFALSE for classes
  Int_t classVersion;   // Code version of classes stored as binary
public:
  HParamBinObj();
  HParamBinObj(HParamBinObj&);
  HParamBinObj(const Text_t*,const UChar_t*,const Int_t);
  HParamBinObj(const Text_t*,const Short_t*,const Int_t);
  HParamBinObj(const Text_t*,const Int_t*,const Int_t);
  HParamBinObj(const Text_t*,const Float_t*,const Int_t);
  HParamBinObj(const Text_t*,const Double_t*,const Int_t);
  ~HParamBinObj();
  void fill(const Text_t*,UChar_t*,const Int_t);
  void fill(const Text_t*,Short_t*,const Int_t);
  void fill(const Text_t*,Int_t*,const Int_t);
  void fill(const Text_t*,Float_t*,const Int_t);
  void fill(const Text_t*,Double_t*,const Int_t);
  void setParamValue(UChar_t*,const Int_t);
  void setParamType(const Text_t* t);
  void setClassVersion(const Int_t v) { classVersion=v; }
  void setLength(Int_t l);
  UChar_t* getParamValue() { return paramValue; }
  const char* getParamType() { return paramType.Data(); }
  Bool_t isBasicType() { return basicType; }
  Int_t getClassVersion() { return classVersion; }
  Int_t getLength() { return arraySize; }
  Int_t getNumParams();
  void print();  
  ClassDef(HParamBinObj,0) // Class for binary parameter object (name + binary array)
};

class HParamList : public TObject {
protected:
  TList* paramList;      // List for parameters stored as string
  TList* paramBinList;   // List for parameters stored as binary
public:
  HParamList();
  ~HParamList();
  void add(HParamObj&);
  void addBinary(HParamBinObj&);
  void add(const Text_t*,const Text_t*,const char type='s',
           const Int_t n=1);
  void add(const Text_t*,const Int_t);
  void add(const Text_t*,const UInt_t);
  void add(const Text_t*,const Long_t);
  void add(const Text_t*,const Float_t,const Int_t precision=3);
  void add(const Text_t*,const Double_t,const Int_t precision=3);
  void add(const Text_t*,TArrayI&);
  void add(const Text_t*,TArrayC&);
  void add(const Text_t*,TArrayF&,const Int_t precision=3);
  void add(const Text_t*,TArrayD&,const Int_t precision=3);
  void addBinary(const Text_t*,const UChar_t*,const Int_t);
  void addBinary(const Text_t*,const Short_t*,const Int_t);
  void addBinary(const Text_t*,const Int_t*,const Int_t);
  void addBinary(const Text_t*,const Float_t*,const Int_t);
  void addBinary(const Text_t*,const Double_t*,const Int_t);
  void addBinary(const Text_t*,TObject*);
  Bool_t fill(const Text_t*,Text_t*,const Int_t);
  Bool_t fill(const Text_t*,Int_t*);  
  Bool_t fill(const Text_t*,UInt_t*);  
  Bool_t fill(const Text_t*,Long_t*);  
  Bool_t fill(const Text_t*,Float_t*);  
  Bool_t fill(const Text_t*,Double_t*);  
  Bool_t fill(const Text_t*,TArrayI*);  
  Bool_t fill(const Text_t*,TArrayC*);  
  Bool_t fill(const Text_t*,TArrayF*);  
  Bool_t fill(const Text_t*,TArrayD*);  
  Bool_t fillBinary(const Text_t*,UChar_t*,const Int_t);
  Bool_t fillBinary(const Text_t*,Short_t*,const Int_t);
  Bool_t fillBinary(const Text_t*,Int_t*,const Int_t);
  Bool_t fillBinary(const Text_t*,Float_t*,const Int_t);
  Bool_t fillBinary(const Text_t*,Double_t*,const Int_t);
  Int_t fillBinary(const Text_t*,UChar_t*);
  Int_t fillBinary(const Text_t*,Short_t*);
  Int_t fillBinary(const Text_t*,Int_t*);
  Int_t fillBinary(const Text_t*,Float_t*);
  Int_t fillBinary(const Text_t*,Double_t*);
  Int_t fillBinary(const Text_t*,TObject*);
  void print();
  HParamObj* find(const Text_t* name) {
    return (HParamObj*)paramList->FindObject(name);
  }
  HParamBinObj* findBinary(const Text_t* name) {
    return (HParamBinObj*)paramBinList->FindObject(name);
  }
  TList* getList() { return paramList; }
  TList* getBinaryList() { return paramBinList; }
  ClassDef(HParamList,0) // Class for lists of parameters (of type HParamObj and HParamBinObj)
};

#endif  /* !HPARAMLIST_H */
 
