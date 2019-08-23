//*-- AUTHOR : Ilse Koenig
//*-- modified : 19/10/2005 by Ilse Koenig

using namespace std;
#include "hparamlist.h"
#include "TClass.h"
#include "TStreamerInfo.h"
#include <iostream>
#include <stdlib.h>

//_HADES_CLASS_DESCRIPTION 
///////////////////////////////////////////////////////////////////////////////
//
//  HParamObj
//
//  Class for single parameters of basic types or small arrays and used as
//  list elements in HParamList::paramList.
//  The values are stored as strings. Arrays are concatinated to a string as a
//  comma-separated list.
//
///////////////////////////////////////////////////////////////////////////////


ClassImp(HParamObj)
 
HParamObj::HParamObj(const Text_t* name,const Text_t* value,
                     const char type,const Int_t numParam) {
  // (Default) Constructor
  SetName(name);
  paramValue=value;
  paramType=type;
  nParam=numParam;
}

void HParamObj::print() {
  // prints the name and the value
  std::cout<<GetName()<<": "<<paramValue<<std::endl;
}

///////////////////////////////////////////////////////////////////////////////
//
//  HParamBinObj
//
//  Class for parameters of large arrays of basic types and used as list
//  elements in HParamList::paramBinList.
//  The values are stored as binary.
//  Arrays of UChar_t, Short_t, Int_t, Float_t, Double_t are supported.
//  The arguments in the overloaded constructors and fill-functions are
//      the name of the parameter
//      the pointer to the array
//      the length of the array.      
//
///////////////////////////////////////////////////////////////////////////////

ClassImp(HParamBinObj)
 
HParamBinObj::HParamBinObj() {
  // Default constructor with type "u" for "unknown"
  paramType="u";
  basicType=kFALSE;
  classVersion=-1;
  arraySize=0;
  paramValue=0;
  streamerInfo=0;
  streamerInfoSize=0;
}

HParamBinObj::HParamBinObj(HParamBinObj& o) {
  // Copy constructor
  SetName(o.GetName());
  paramType=o.getParamType();
  if (o.isBasicType()) basicType=kTRUE;
  else basicType=kFALSE;
  classVersion=o.getClassVersion();
  arraySize=o.getLength();
  paramValue=new UChar_t[arraySize];
  memcpy(paramValue,o.getParamValue(),arraySize);
  streamerInfoSize=o.getStreamerInfoSize();
  if (streamerInfoSize>0) {
    memcpy(streamerInfo,o.getStreamerInfo(),streamerInfoSize);
  }
}

HParamBinObj::HParamBinObj(const Text_t* name,const UChar_t* value,const Int_t length) {
  // Constructor for an array of UChar_t with arraylength lenght 
  SetName(name);
  setParamType("cBin");
  arraySize=length;
  paramValue=new UChar_t[arraySize];
  memcpy(paramValue,value,arraySize);
}

HParamBinObj::HParamBinObj(const Text_t* name,const Short_t* value,const Int_t nValues) {
  // Constructor for an array of nValues elements of type Short_t
  SetName(name);
  setParamType("hBin");
  arraySize=nValues*sizeof(Short_t);
  paramValue=new UChar_t[arraySize];
  memcpy(paramValue,value,arraySize);
}

HParamBinObj::HParamBinObj(const Text_t* name,const Int_t* value,const Int_t nValues) {
  // Constructor for an array of nValues elements of type Int_t
  SetName(name);
  setParamType("iBin");
  arraySize=nValues*sizeof(Int_t);
  paramValue=new UChar_t[arraySize];
  memcpy(paramValue,value,arraySize);
}

HParamBinObj::HParamBinObj(const Text_t* name,const Float_t* value,const Int_t nValues) {
  // Constructor for an array of nValues elements of type Float_t
  SetName(name);
  setParamType("fBin");
  arraySize=nValues*sizeof(Float_t);
  paramValue=new UChar_t[arraySize];
  memcpy(paramValue,value,arraySize);
}

HParamBinObj::HParamBinObj(const Text_t* name,const Double_t* value,const Int_t nValues) {
  // Constructor for an array of nValues elements of type Double_t
  SetName(name);
  setParamType("dBin");
  arraySize=nValues*sizeof(Double_t);
  paramValue=new UChar_t[arraySize];
  memcpy(paramValue,value,arraySize);
}

HParamBinObj::~HParamBinObj() {
  // Destructor
  if (paramValue) {
    delete [] paramValue;
    paramValue=0;
  }
  if (streamerInfo) {
    delete [] streamerInfo;
    streamerInfo=0;
  }
}

void HParamBinObj::setParamType(const Text_t* t) {
  // Sets the parameter type. Accepted type names are:
  //     cBin for UChar_t
  //     hBin for Short_t
  //     iBin for Int_t
  //     fBin for Float_t
  //     dBin for Double_t
  //     class name
  if (strcmp(t,"cBin")==0 || strcmp(t,"hBin")==0 || strcmp(t,"iBin")==0
         || strcmp(t,"fBin")==0 || strcmp(t,"dBin")==0) {
    basicType=kTRUE;
    classVersion=-1;
    streamerInfo=0;
    streamerInfoSize=0;
  }
  else basicType=kFALSE;
  paramType=t;
}

void HParamBinObj::setParamValue(UChar_t* value,const Int_t length) {
  // Sets the parameter value (the array is not copied!)
  if (paramValue) delete [] paramValue;
  arraySize=length;
  paramValue=value;
}

void HParamBinObj::setLength(Int_t l) {
  // Sets the length of the binary array
  if (paramValue && arraySize!=l) delete [] paramValue;
  arraySize=l;
  if (l>0) paramValue=new UChar_t[arraySize];
  else paramValue=0;
}

void HParamBinObj::setStreamerInfo(UChar_t* array,const Int_t length) {
  // Sets the parameter value (the array is not copied!)
  if (streamerInfo) delete [] streamerInfo;
  streamerInfoSize=length;
  streamerInfo=array;
}

void HParamBinObj::setStreamerInfoSize(Int_t l) {
  // Sets the length of the streamer info
  if (streamerInfo && streamerInfoSize!=l) delete [] streamerInfo;
  streamerInfoSize=l;
  if (l>0) streamerInfo=new UChar_t[streamerInfoSize];
  else streamerInfo=0;
}

void HParamBinObj::fill(const Text_t* name,UChar_t* value,const Int_t length) {
  // Sets the name and value (UChar_t array) of an existing object 
  SetName(name);
  paramType="cBin";
  basicType=kTRUE;
  setLength(length);
  memcpy(paramValue,value,arraySize);
}

void HParamBinObj::fill(const Text_t* name,Short_t* value,const Int_t nValues)   // Sets the name and value (Short_t array) of an existing object 
{
  SetName(name);
  paramType="hBin";
  basicType=kTRUE;
  classVersion=-1;
  setLength(nValues*sizeof(Short_t));
  memcpy(paramValue,value,arraySize);
}

void HParamBinObj::fill(const Text_t* name,Int_t* value,const Int_t nValues) {
  // Sets the name and value (Intt_t array) of an existing object 
  SetName(name);
  paramType="iBin";
  basicType=kTRUE;
  classVersion=-1;
  setLength(nValues*sizeof(Int_t));
  memcpy(paramValue,value,arraySize);
}

void HParamBinObj::fill(const Text_t* name,Float_t* value,const Int_t nValues)   // Sets the name and value (Float_t array) of an existing object 
{
  SetName(name);
  paramType="fBin";
  basicType=kTRUE;
  classVersion=-1;
  setLength(nValues*sizeof(Float_t));
  memcpy(paramValue,value,arraySize);
}

void HParamBinObj::fill(const Text_t* name,Double_t* value,const Int_t nValues)  // Sets the name and value (Double_t array) of an existing object 
 {
  SetName(name);
  paramType="dBin";
  basicType=kTRUE;
  classVersion=-1;
  setLength(nValues*sizeof(Double_t));
  memcpy(paramValue,value,arraySize);
}

Int_t HParamBinObj::getNumParams() {
  // returns the number of values in the array, respectively 1 for classes
  Int_t n=0;
  if (basicType) {
    switch(paramType[0]) {
      case 'c': n=arraySize;                  break;
      case 'h': n=arraySize/sizeof(Short_t);  break;
      case 'i': n=arraySize/sizeof(Int_t);    break;
      case 'f': n=arraySize/sizeof(Float_t);  break;
      case 'd': n=arraySize/sizeof(Double_t); break;
      default: Error("HParamBinObj::getNumParams()","Unknown Type %s",paramType.Data());
    }
  } else n=1;  // always 1 for classes
  return n;
}

void HParamBinObj::print() {
  // Prints the name and type of the parameters, respectively class name and version
  // Prints also the numbers for an array of Short_t, Int_t, Float_t, Double_t.
  printf("%s:  ",GetName());
  if (!basicType) {
    printf("\n  Class Type:    %s\n  Class Version: %i\n",
           paramType.Data(),classVersion);
  } else {
    Int_t l=0, i=0, k=0;
    TString format;
    switch(paramType[0]) {
      case 'c':
        printf("Binary UChar_t Array\n");
        break;
      case 'h':
        printf("Binary Short_t Array\n");
        l=arraySize/sizeof(Short_t);
        while (k<l) {
          if (i==10) {
            printf("\n");
            i=0;
          }
          printf("%i ",((Short_t*)paramValue)[k]);
          i++;
          k++;  
        }
        printf("\n");
        break;
      case 'i':
        printf("Binary Int_t Array\n");
        l=arraySize/sizeof(Int_t);
        while (k<l) {
          if (i==10) {
            printf("\n");
            i=0;
          }
          printf("%i ",((Int_t*)paramValue)[k]);
          i++;
          k++;  
        }
        printf("\n");
        break;
      case 'f':
        printf("Binary Float_t Array\n");
        l=arraySize/sizeof(Float_t);
        while (k<l) {
          if (i==10) {
            printf("\n");
            i=0;
          }
          printf("%g ",((Float_t*)paramValue)[k]);
          i++;
          k++;  
        }
        printf("\n");
        break;
      case 'd':
        printf("Binary Double_t Array\n");
        l=arraySize/sizeof(Double_t);
        while (k<l) {
          if (i==10) {
            printf("\n");
            i=0;
          }
          printf("%g ",((Double_t*)paramValue)[k]);
          i++;
          k++;  
        }
        printf("\n");
        break;
      default:
        Error("HParamBinObj::getNumParams()","Unknown Type %s",paramType.Data());
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
//
//  HParamList
//
//  Class for generic Oracle and ASCII interface for parameter containers
//  derived from HParCond
//
//  All add/addBin functions add an initialized parameter to the list for
//  writing, all fill-functions copies the value from the list to the
//  parameter in the initialization process.
//
///////////////////////////////////////////////////////////////////////////////

ClassImp(HParamList)

HParamList::HParamList() {
  // Constructor
  paramList=new TList;
  paramBinList=new TList;
}

HParamList::~HParamList() {
  // Destructor
  if (paramList) {
    paramList->Delete();
    delete paramList;
    paramList=0;
  }
  if (paramBinList) {
    paramBinList->Delete();
    delete paramBinList;
    paramBinList=0;
  }
}

void HParamList::add(HParamObj& p) {
  // Adds a HParamObj object to the list
  paramList->Add(new HParamObj(p.GetName(),p.getParamValue(),
                               p.getParamType(),p.getNumParams()));
}

void HParamList::addBinary(HParamBinObj& p) {
  // Adds a HBinParamObj object to the list
  paramBinList->Add(new HParamBinObj(p));
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

void HParamList::add(const Text_t* name,const UInt_t value) {
  const char f[]="%u";
  Text_t buf[100];
  sprintf(buf,f,value);
  paramList->Add(new HParamObj(name,buf,'u',1));
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

void HParamList::add(const Text_t* name,TArrayC& value) {
  Int_t n=value.GetSize();
  const char f[]="%i";
  Text_t buf[100];
  TString s;
  for(Int_t i=0;i<n;i++) {
    if (i>0) s.Append(",");
    sprintf(buf,f,int(value.At(i)));
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

void HParamList::addBinary(const Text_t* name,const UChar_t* values,const Int_t nValues){
  paramBinList->Add(new HParamBinObj(name,values,nValues));
}

void HParamList::addBinary(const Text_t* name,const Short_t* values,const Int_t nValues){
  paramBinList->Add(new HParamBinObj(name,values,nValues));
}

void HParamList::addBinary(const Text_t* name,const Int_t* values,const Int_t nValues){
  paramBinList->Add(new HParamBinObj(name,values,nValues));
}

void HParamList::addBinary(const Text_t* name,const Float_t* values,const Int_t nValues){
  paramBinList->Add(new HParamBinObj(name,values,nValues));
}

void HParamList::addBinary(const Text_t* name,const Double_t* values,const Int_t nValues){
  paramBinList->Add(new HParamBinObj(name,values,nValues));
}

void HParamList::addBinary(const Text_t* name,TObject* obj) {
  if (!obj) return;
  HParamBinObj* o=new HParamBinObj;
  o->SetName(name);
  o->setParamType(obj->IsA()->GetName());
  o->setClassVersion(obj->IsA()->GetClassVersion());
  TFile* filesave=gFile;
  HParamTFile* paramFile=new HParamTFile();
  gFile=paramFile;
  const Int_t bufsize=10000;
  TBuffer* buffer=new TBuffer(TBuffer::kWrite,bufsize);
  buffer->SetParent(paramFile);
  buffer->MapObject(obj);
  obj->Streamer(*buffer);
  Int_t len=buffer->Length();
  char* buf=new char[len];
  memcpy(buf,buffer->Buffer(),len);
  o->setParamValue((UChar_t*)buf,len);
  TArrayC* fClassIndex=paramFile->GetClassIndex();
  if (fClassIndex&&fClassIndex->fArray[0] != 0) {
    TIter next(gROOT->GetListOfStreamerInfo());
    TStreamerInfo *info;
    TList list;
    while ((info=(TStreamerInfo*)next())) {
      Int_t uid=info->GetNumber();
      if (fClassIndex->fArray[uid]) list.Add(info);
    }
    if (list.GetSize()>0) {
      list.Sort();
      fClassIndex->fArray[0]=2; //to prevent adding classes in TStreamerInfo::TagFile
      TBuffer* infoBuffer=new TBuffer(TBuffer::kWrite,bufsize);
      infoBuffer->MapObject(&list);
      list.Streamer(*infoBuffer);
      Int_t infolen=infoBuffer->Length();
      char* infobuf=new char[infolen];
      memcpy(infobuf,infoBuffer->Buffer(),infolen);
      o->setStreamerInfo((UChar_t*)infobuf,infolen);
      delete infoBuffer;
    } else {
      o->setStreamerInfo(0,0);
    }
  }
  fClassIndex->fArray[0]=0;
  delete paramFile;
  paramBinList->Add(o);
  delete buffer;
  gFile=filesave;
}

void HParamList::print() {
  TIter next(paramList);
  HParamObj* o;
  while ((o=(HParamObj*)next())!=0) o->print();
  TIter nextBin(paramBinList);
  HParamBinObj* b;
  while ((b=(HParamBinObj*)nextBin())!=0) b->print();
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

Bool_t HParamList::fill(const Text_t* name,UInt_t* value) {
  HParamObj* o=(HParamObj*)paramList->FindObject(name);
  if (value==0) return kFALSE;
  if (o!=0 && o->getParamType()=='u' && o->getNumParams()==1) {
    sscanf(o->getParamValue(),"%u",value);
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

Bool_t HParamList::fill(const Text_t* name,TArrayC* value) {
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
    value->AddAt(char(v),0);
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

Bool_t HParamList::fillBinary(const Text_t* name,UChar_t* values,const Int_t nValues) {
  // Fills the array (no resize).
  if (values==0) return kFALSE;
  HParamBinObj* o=(HParamBinObj*)paramBinList->FindObject(name);
  if (o!=0 && strcmp(o->getParamType(),"cBin")==0) {
    Int_t n=o->getLength();  
    if (n==nValues) {
      memcpy(values,o->getParamValue(),n);
      return kTRUE;
    } else {
      Error("HParamList::fillBinary \nDifferent array sizes for parameter ",name);
      return kFALSE;
    }
  }
  Error("HParamList::fill \nNot found: ",name);
  return kFALSE;
}

Bool_t HParamList::fillBinary(const Text_t* name,Short_t* values,const Int_t nValues) {
  // Fills the array (no resize).
  if (values==0) return kFALSE;
  HParamBinObj* o=(HParamBinObj*)paramBinList->FindObject(name);
  if (o!=0 && strcmp(o->getParamType(),"hBin")==0) {
    Int_t l=o->getLength();
    Int_t n=l/sizeof(Short_t);  
    if (n==nValues) {
      memcpy(values,o->getParamValue(),l);
      return kTRUE;
    } else {
      Error("HParamList::fillBinary \nDifferent array sizes for parameter ",name);
      return kFALSE;
    }
  }
  Error("HParamList::fill \nNot found: ",name);
  return kFALSE;
}

Bool_t HParamList::fillBinary(const Text_t* name,Int_t* values,const Int_t nValues) {
  // Fills the array (no resize).
  if (values==0) return kFALSE;
  HParamBinObj* o=(HParamBinObj*)paramBinList->FindObject(name);
  if (o!=0 && strcmp(o->getParamType(),"iBin")==0) {
    Int_t l=o->getLength();
    Int_t n=l/sizeof(Int_t);  
    if (n==nValues) {
      memcpy(values,o->getParamValue(),l);
      return kTRUE;
    } else {
      Error("HParamList::fillBinary \nDifferent array sizes for parameter ",name);
      return kFALSE;
    }
  }
  Error("HParamList::fill \nNot found: ",name);
  return kFALSE;
}

Bool_t HParamList::fillBinary(const Text_t* name,Float_t* values,const Int_t nValues) {
  // Fills the array (no resize).
  if (values==0) return kFALSE;
  HParamBinObj* o=(HParamBinObj*)paramBinList->FindObject(name);
  if (o!=0 && strcmp(o->getParamType(),"fBin")==0) {
    Int_t l=o->getLength();
    Int_t n=l/sizeof(Float_t);  
    if (n==nValues) {
      memcpy(values,o->getParamValue(),l);
      return kTRUE;
    } else {
      Error("HParamList::fillBinary \nDifferent array sizes for parameter ",name);
      return kFALSE;
    }
  }
  Error("HParamList::fill \nNot found: ",name);
  return kFALSE;
}

Bool_t HParamList::fillBinary(const Text_t* name,Double_t* values,const Int_t nValues) {
  // Fills the array (no resize).
  if (values==0) return kFALSE;
  HParamBinObj* o=(HParamBinObj*)paramBinList->FindObject(name);
  if (o!=0 && strcmp(o->getParamType(),"dBin")==0) {
    Int_t l=o->getLength();
    Int_t n=l/sizeof(Double_t);  
    if (n==nValues) {
      memcpy(values,o->getParamValue(),l);
      return kTRUE;
    } else {
      Error("HParamList::fillBinary \nDifferent array sizes for parameter ",name);
      return kFALSE;
    }
  }
  Error("HParamList::fill \nNot found: ",name);
  return kFALSE;
}

Int_t HParamList::fillBinary(const Text_t* name,UChar_t* values) {
  // Recreates the array and returns the number of array elements
  HParamBinObj* o=(HParamBinObj*)paramBinList->FindObject(name);
  if (o!=0 && strcmp(o->getParamType(),"cBin")==0) {
    Int_t l=o->getLength();
    if (values) delete values;
    values=new UChar_t[l];
    memcpy(values,o->getParamValue(),l);
    return l;
  }
  Error("HParamList::fill \nNot found: ",name);
  return 0;
}

Int_t HParamList::fillBinary(const Text_t* name,Short_t* values) {
  // Recreates the array and returns the number of array elements
  HParamBinObj* o=(HParamBinObj*)paramBinList->FindObject(name);
  if (o!=0 && strcmp(o->getParamType(),"hBin")==0) {
    Int_t l=o->getLength();
    Int_t n=l/sizeof(Short_t);  
    if (values) delete values;
    values=new Short_t[n];
    memcpy(values,o->getParamValue(),l);
    return n;
  }
  Error("HParamList::fill \nNot found: ",name);
  return 0;
}

Int_t HParamList::fillBinary(const Text_t* name,Int_t* values) {
  // Recreates the array and returns the number of array elements
  HParamBinObj* o=(HParamBinObj*)paramBinList->FindObject(name);
  if (o!=0 && strcmp(o->getParamType(),"iBin")==0) {
    Int_t l=o->getLength();
    Int_t n=l/sizeof(Int_t);  
    if (values) delete values;
    values=new Int_t[n];
    memcpy(values,o->getParamValue(),l);
    return n;
  }
  Error("HParamList::fill \nNot found: ",name);
  return 0;
}

Int_t HParamList::fillBinary(const Text_t* name,Float_t* values) {
  // Recreates the array and returns the number of array elements
  HParamBinObj* o=(HParamBinObj*)paramBinList->FindObject(name);
  if (o!=0 && strcmp(o->getParamType(),"fBin")==0) {
    Int_t l=o->getLength();
    Int_t n=l/sizeof(Float_t);  
    if (values) delete values;
    values=new Float_t[n];
    memcpy(values,o->getParamValue(),l);
    return n;
  }
  Error("HParamList::fill \nNot found: ",name);
  return 0;
}

Int_t HParamList::fillBinary(const Text_t* name,Double_t* values) {
  // Recreates the array and returns the number of array elements
  HParamBinObj* o=(HParamBinObj*)paramBinList->FindObject(name);
  if (o!=0 && strcmp(o->getParamType(),"dBin")==0) {
    Int_t l=o->getLength();
    Int_t n=l/sizeof(Double_t);  
    if (values) delete values;
    values=new Double_t[n];
    memcpy(values,o->getParamValue(),l);
    return n;
  }
  Error("HParamList::fill \nNot found: ",name);
  return 0;
}

Int_t HParamList::fillBinary(const Text_t* name,TObject* obj) {
  // Fills the object obj (must exist!) via the Streamer and returns the class version
  // Prints a warning if the class version read differs from the actual class version
  if (!obj) return 0;
  HParamBinObj* o=(HParamBinObj*)paramBinList->FindObject(name);
  if (o!=0 && strcmp(o->getParamType(),obj->IsA()->GetName())==0) {
    if (o->getClassVersion()!=obj->IsA()->GetClassVersion())
      Warning("HParamList::fillBinary",
              "\n       Read Class Version = %i does not match actual version = %i",
              o->getClassVersion(),obj->IsA()->GetClassVersion());
    TFile* filesave=gFile;
    gFile=0;
    TBuffer* buf=0;
    Int_t len=o->getStreamerInfoSize();
    if (len>0&&o->getStreamerInfo()!=0) {
      buf=new TBuffer(TBuffer::kRead,len);
      memcpy(buf->Buffer(),(char*)o->getStreamerInfo(),len);
      buf->SetBufferOffset(0);
      TList list;
      buf->MapObject(&list);
      list.Streamer(*buf);
      delete buf;
      TStreamerInfo *info;
      TIter next(&list);
      while ((info = (TStreamerInfo*)next())) {
        if (info->IsA() != TStreamerInfo::Class()) {
	  Warning("HParamList::fill","not a TStreamerInfo object");
          continue;
        }
        info->BuildCheck();
      }
      list.Clear();  //this will delete all TStreamerInfo objects with kCanDelete 
    }
    len=o->getLength();
    buf=new TBuffer(TBuffer::kRead,len);
    memcpy(buf->Buffer(),(char*)o->getParamValue(),len);
    buf->SetBufferOffset(0);
    buf->MapObject(obj);
    obj->Streamer(*buf);
    delete buf;
    gFile=filesave;
    return len;
  }
  Error("HParamList::fill \nNot found: ",name);
  return 0;
}
