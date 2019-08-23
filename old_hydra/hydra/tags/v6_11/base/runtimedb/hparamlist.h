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
  TString paramValue;
  char paramType;
  Int_t nParam;
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
  ClassDef(HParamObj,1) // Class for parameter object (name + string-value)
};

class HParamList : public TObject {
protected:
  TList* paramList;
public:
  HParamList();
  ~HParamList();
  void add(HParamObj&);
  void add(const Text_t* name,const Text_t* value,const char type='s',
           const Int_t n=1);
  void add(const Text_t* name,const Int_t value);
  void add(const Text_t* name,const UInt_t value);
  void add(const Text_t* name,const Long_t value);
  void add(const Text_t* name,const Float_t value,const Int_t precision=3);
  void add(const Text_t* name,const Double_t value,const Int_t precision=3);
  void add(const Text_t* name,TArrayI& value);
  void add(const Text_t* name,TArrayC& value);
  void add(const Text_t* name,TArrayF& value,const Int_t precision=3);
  void add(const Text_t* name,TArrayD& value,const Int_t precision=3);
  Bool_t fill(const Text_t* name,Text_t* value,const Int_t length);
  Bool_t fill(const Text_t* name,Int_t* value);  
  Bool_t fill(const Text_t* name,UInt_t* value);  
  Bool_t fill(const Text_t* name,Long_t* value);  
  Bool_t fill(const Text_t* name,Float_t* value);  
  Bool_t fill(const Text_t* name,Double_t* value);  
  Bool_t fill(const Text_t* name,TArrayI* value);  
  Bool_t fill(const Text_t* name,TArrayC* value);  
  Bool_t fill(const Text_t* name,TArrayF* value);  
  Bool_t fill(const Text_t* name,TArrayD* value);  
  void print();
  HParamObj* find(const Text_t* name) {
    return (HParamObj*)paramList->FindObject(name);
  }
  TList* getList() { return paramList; }
  ClassDef(HParamList,1) // Class for list of parameters (of type HParamObj)
};

#endif  /* !HPARAMLIST_H */
 
