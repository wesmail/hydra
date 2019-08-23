//*-- AUTHOR : Ilse Koenig
//*-- modified : 18/09/2001 by Ilse Koenig

///////////////////////////////////////////////////////////////
//
//  HParamObj
//
///////////////////////////////////////////////////////////////

#include <iostream.h>
#include "hparamlist.h"

ClassImp(HParamObj)
 
HParamObj::HParamObj(const Text_t* name,const Text_t* value,
                     const char type,const Int_t numParam) {
  SetName(name);
  paramValue=value;
  paramType=type;
  nParam=numParam;
}

void HParamObj::print() {
  cout<<GetName()<<": "<<paramValue<<endl;
}

///////////////////////////////////////////////////////////////
//
//  HParamList
//
///////////////////////////////////////////////////////////////

ClassImp(HParamList)

HParamList::HParamList() {
  paramList=new TList;
}

HParamList::~HParamList() {
  if (paramList) {
    paramList->Delete();
    delete paramList;
    paramList=0;
  }
}

void HParamList::add(HParamObj& p) {
  paramList->Add(new HParamObj(p.GetName(),p.getParamValue(),
                               p.getParamType(),p.getNumParams()));
}

void HParamList::add(const Text_t* name,const Text_t* value,
                     const char type,const Int_t n) {
  paramList->Add(new HParamObj(name,value,type,n));
}

void HParamList::add(const Text_t* name,const Int_t value) {
  const char f[]="%i";
  Text_t buf[100];
  sprintf(buf,f,value);
  paramList->Add(new HParamObj(name,buf,'i',1));
}

void HParamList::add(const Text_t* name,const Long_t value) {
  const char f[]="%ld";
  Text_t buf[100];
  sprintf(buf,f,value);
  paramList->Add(new HParamObj(name,buf,'l',1));
}

void HParamList::add(const Text_t* name,const Float_t value,const Int_t precision) {
  Text_t f[100];
  sprintf(f,"%s%i%s","%.",precision,"f");
  Text_t buf[100];
  sprintf(buf,f,value);
  paramList->Add(new HParamObj(name,buf,'f',1));
}

void HParamList::add(const Text_t* name,const Double_t value,const Int_t precision) {
  Text_t f[100];
  sprintf(f,"%s%i%s","%.",precision,"lf");
  Text_t buf[100];
  sprintf(buf,f,value);
  paramList->Add(new HParamObj(name,buf,'d',1));
}

void HParamList::add(const Text_t* name,TArrayI& value) {
  Int_t n=value.GetSize();
  const char f[]="%i";
  Text_t buf[100];
  TString s;
  for(Int_t i=0;i<n;i++) {
    if (i>0) s.Append(",");
    sprintf(buf,f,value.At(i));
    s.Append(buf);
  }
  paramList->Add(new HParamObj(name,s,'i',n));
}

void HParamList::add(const Text_t* name,TArrayF& value,const Int_t precision){
  Int_t n=value.GetSize();
  Text_t f[100];
  sprintf(f,"%s%i%s","%.",precision,"f");
  Text_t buf[100];
  TString s;
  for(Int_t i=0;i<n;i++) {
    if (i>0) s.Append(",");
    sprintf(buf,f,value.At(i));
    s.Append(buf);
  }
  paramList->Add(new HParamObj(name,s,'f',n));
}

void HParamList::add(const Text_t* name,TArrayD& value,const Int_t precision){
  Int_t n=value.GetSize();
  Text_t f[100];
  sprintf(f,"%s%i%s","%.",precision,"lf");
  Text_t buf[100];
  TString s;
  for(Int_t i=0;i<n;i++) {
    if (i>0) s.Append(",");
    sprintf(buf,f,value.At(i));
    s.Append(buf);
  }
  paramList->Add(new HParamObj(name,s,'d',n));
}

void HParamList::print() {
  TIter next(paramList);
  HParamObj* o;
  while ((o=(HParamObj*)next())!=0) o->print();
}

Bool_t HParamList::fill(const Text_t* name,Text_t* value,const Int_t length) {
  HParamObj* o=(HParamObj*)paramList->FindObject(name);
  if (value==0) return kFALSE;
  if (o!=0 && o->getParamType()=='s') {
    const char* v=o->getParamValue();
    Int_t l=strlen(v);
    if (l<length) {
      strcpy(value,o->getParamValue());
      return kTRUE;
    } else Error("HParamList::fill(const Text_t*,Text_t*)",
                 "char array too small");
  }
  Error("HParamList::fill \nNot found: %s",name);
  return kFALSE;
}

Bool_t HParamList::fill(const Text_t* name,Int_t* value) {
  HParamObj* o=(HParamObj*)paramList->FindObject(name);
  if (value==0) return kFALSE;
  if (o!=0 && o->getParamType()=='i' && o->getNumParams()==1) {
    sscanf(o->getParamValue(),"%i",value);
    return kTRUE;
  }
  Error("HParamList::fill \nNot found: ",name);
  return kFALSE;
}

Bool_t HParamList::fill(const Text_t* name,Long_t* value) {
  HParamObj* o=(HParamObj*)paramList->FindObject(name);
  if (value==0) return kFALSE;
  if (o!=0 && o->getParamType()=='l' && o->getNumParams()==1) {
    sscanf(o->getParamValue(),"%ld",value);
    return kTRUE;
  }
  Error("HParamList::fill \nNot found: ",name);
  return kFALSE;
}

Bool_t HParamList::fill(const Text_t* name,Float_t* value) {
  HParamObj* o=(HParamObj*)paramList->FindObject(name);
  if (value==0) return kFALSE;
  if (o!=0 && o->getParamType()=='f' && o->getNumParams()==1) {
    sscanf(o->getParamValue(),"%f",value);
    return kTRUE;
  }
  Error("HParamList::fill \nNot found: ",name);
  return kFALSE;
}

Bool_t HParamList::fill(const Text_t* name,Double_t* value) {
  HParamObj* o=(HParamObj*)paramList->FindObject(name);
  if (value==0) return kFALSE;
  if (o!=0 && o->getParamType()=='d' && o->getNumParams()==1) {
    sscanf(o->getParamValue(),"%lf",value);
    return kTRUE;
  }
  Error("HParamList::fill \nNot found: ",name);
  return kFALSE;
}

Bool_t HParamList::fill(const Text_t* name,TArrayI* value) {
  HParamObj* o=(HParamObj*)paramList->FindObject(name);
  if (value==0) return kFALSE;
  if (o!=0 && o->getParamType()=='i') {
    Int_t n=o->getNumParams();
    if (value->GetSize()!=n) value->Set(n);
    const char d[]=",";
    char* ss;
    Int_t v;
    char s[4001];
    strcpy(s,o->getParamValue());
    ss=strtok(s,d);
    sscanf(ss,"%i",&v);
    value->AddAt(v,0);
    for(Int_t i=1;i<n;i++) {
      ss=strtok(NULL,d);
      sscanf(ss,"%i",&v);
      value->AddAt(v,i);
    }
    return kTRUE;
  }
  Error("HParamList::fill \nNot found: ",name);
  return kFALSE;
}

Bool_t HParamList::fill(const Text_t* name,TArrayF* value) {
  HParamObj* o=(HParamObj*)paramList->FindObject(name);
  if (value==0) return kFALSE;
  if (o!=0 && o->getParamType()=='f') {
    Int_t n=o->getNumParams();
    if (value->GetSize()!=n) value->Set(n);
    const char d[]=",";
    char* ss;
    Float_t v;
    char s[4001];
    strcpy(s,o->getParamValue());
    ss=strtok(s,d);
    sscanf(ss,"%f",&v);
    value->AddAt(v,0);
    for(Int_t i=1;i<n;i++) {
      ss=strtok(NULL,d);
      sscanf(ss,"%f",&v);
      value->AddAt(v,i);
    }
    return kTRUE;
  }
  Error("HParamList::fill \nNot found: ",name);
  return kFALSE;
}

Bool_t HParamList::fill(const Text_t* name,TArrayD* value) {
  HParamObj* o=(HParamObj*)paramList->FindObject(name);
  if (value==0) return kFALSE;
  if (o!=0 && o->getParamType()=='d') {
    Int_t n=o->getNumParams();
    if (value->GetSize()!=n) value->Set(n);
    const char d[]=",";
    char* ss;
    Double_t v;
    char s[4001];
    strcpy(s,o->getParamValue());
    ss=strtok(s,d);
    sscanf(ss,"%lf",&v);
    value->AddAt(v,0);
    for(Int_t i=1;i<n;i++) {
      ss=strtok(NULL,d);
      sscanf(ss,"%lf",&v);
      value->AddAt(v,i);
    }
    return kTRUE;
  }
  Error("HParamList::fill \nNot found: ",name);
  return kFALSE;
}
