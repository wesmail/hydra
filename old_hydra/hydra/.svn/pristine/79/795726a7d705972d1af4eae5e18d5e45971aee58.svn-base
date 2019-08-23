//*-- AUTHOR : Ilse Koenig
//*-- Last modified : 13/07/2007 by Ilse Koenig

using namespace std;
#include "hparamlist.h"
#include "TClass.h"
#include "TStreamerInfo.h"
#include <iostream>
#include <stdlib.h>

//_HADES_CLASS_DESCRIPTION 
//////////////////////////////////////////////////////////////////////////////////////
//
//  HParamObj
//
//  Class for single parameters of basic types or small arrays converted to a string
//  and used as list elements in HParamList::paramList.
//
//  The total string length must not exceed 2000 characters.
//
//  ----------------------------------------------------------------------------------
//
//  HParamBinObj
//
//  Class for parameters stored in binary format in Oracle and used as list
//  elements in HParamList::paramBinList.
//
//  The overloaded constructors and fill-functions  accept arrays of type
//    UChar_t, Short_t, Int_t, Float_t, Double_t.
//  The arguments are
//      the name of the parameter
//      the pointer to the array
//      the length of the array.
//  The data are automatically converted to an UChar_t array. 
//  For classes (converted to binary format in calling function) also the class
//  version must be stored, for ROOT classes also the TStreamerInfo.      
//
//  ----------------------------------------------------------------------------------
//
//  HParamList
//
//  Class for the generic Oracle and ASCII interface for parameter containers
//  derived from HParCond
//
//  The class contains two lists:
//     paramList     stores objects of type HParamObj
//     paramBinList  stores objects of type HParamBinObj
//  The list objects store the name, the value, the parameter type and some
//  additional information depending for the object type.  
//    
//  All add/addBinary functions add an initialized parameter to the list for
//  writing. The functions create a new list element and copy the data into
//  this object.
//  Non-binary data (add functions):
//    1. accepted basic types: Int_t, Uint_t, Long_t, Float_t, Double_t
//       The data are automatically converted to a string.
//    2. accepted ROOT arrays: TArrayI, TArrayF, TArrayD, TArrayC
//       The array elements are converted to strings and concatenated to a
//       comma-separated list.
//    3. accepted string: Text_t*
//       This can be any text, for example also numbers in hexadecimal or
//       scientific format. The number of characters must be specified (default 1). 
//    The total string length must not exceed 2000 characters.
//    Float_t and Double_t variables are converted with a default precision
//    of 3 digits.
//  Binary data (add functions):
//    1. Arrays of type UChar_t, Short_t, Int_t, Float_t, Double_t
//       The arrays containing numbers are converted to an UChar_t array (with full
//       precision).
//    2. Classes derived from TObject
//       The persistent data elements are streamed into an UChar_t array using the
//       class streamer. For ROOT classes, for example histograms, the ROOT streamer
//       info is stored in an additional binary array. 
//       
//  All fill/fillBinary functions convert the data in the list element back
//  to the type of the parameter and copies them into the data element in the
//  initialization process.
//  Non-binary data (fill functions):
//    The functions return kFALSE, if the parameter is not in the list.
//  Binary data (fillBinary functions):
//    1. Arrays:
//      a) If the array size is specified (return code Bool_t), the functions return
//         kFALSE, if the number of data elements in the list objects is not the same.
//      b) If the array size is not specified (return code Int_t), the array is
//         recreated with the size of the number of data elements in the list object.
//         The functions return the number of data elements or 0, if the parameter
//         was not found.
//    2. Classes:  
//      The class version is checked and a warning printed, if it is not identical
//      with the current version (typically class version of list object higher than
//      version in the actual parameter container). The class streamer takes care
//      for backward compatibility.
//      The function returns the number of bytes in the list object or 0, if the
//      parameter was not found in the list. 
//
//////////////////////////////////////////////////////////////////////////////////////


ClassImp(HParamObj)
ClassImp(HParamBinObj)
ClassImp(HParamList)
 
HParamObj::HParamObj(const Text_t* name,const Text_t* value,
                     const Char_t type,const Int_t numParam) {
  // (Default) Constructor
  // Parameters: name     = name of the parameter
  //             value    = value of the parameter converted to a string
  //             numParam = number of data in an array
  SetName(name);
  paramValue=value;
  paramType=type;
  nParam=numParam;
}

void HParamObj::print() {
  // prints the name and the value
  std::cout<<GetName()<<": "<<paramValue<<std::endl;
}

//-----------------------------------------------------------------------------------
 
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
  // Constructor for an array of type UChar_t with array length "lenght"
  // name = name of the parameter 
  SetName(name);
  setParamType("cBin");
  arraySize=length;
  paramValue=new UChar_t[arraySize];
  memcpy(paramValue,value,arraySize);
}

HParamBinObj::HParamBinObj(const Text_t* name,const Short_t* value,const Int_t nValues) {
  // Constructor for an array with nValues elements of type Short_t
  SetName(name);
  setParamType("hBin");
  arraySize=nValues*sizeof(Short_t);
  paramValue=new UChar_t[arraySize];
  memcpy(paramValue,value,arraySize);
}

HParamBinObj::HParamBinObj(const Text_t* name,const Int_t* value,const Int_t nValues) {
  // Constructor for an array with nValues elements of type Int_t
  SetName(name);
  setParamType("iBin");
  arraySize=nValues*sizeof(Int_t);
  paramValue=new UChar_t[arraySize];
  memcpy(paramValue,value,arraySize);
}

HParamBinObj::HParamBinObj(const Text_t* name,const Float_t* value,const Int_t nValues) {
  // Constructor for an array with nValues elements of type Float_t
  SetName(name);
  setParamType("fBin");
  arraySize=nValues*sizeof(Float_t);
  paramValue=new UChar_t[arraySize];
  memcpy(paramValue,value,arraySize);
}

HParamBinObj::HParamBinObj(const Text_t* name,const Double_t* value,const Int_t nValues) {
  // Constructor for an array with nValues elements of type Double_t
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
  // Sets the streamer info of ROOT classes (the array is not copied!)
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
  // Sets the name and value (UChar_t array of size length) of an existing object 
  SetName(name);
  paramType="cBin";
  basicType=kTRUE;
  setLength(length);
  memcpy(paramValue,value,arraySize);
}

void HParamBinObj::fill(const Text_t* name,Short_t* value,const Int_t nValues) {
  // Sets the name and value (Short_t array of size nValues) of an existing object 
  SetName(name);
  paramType="hBin";
  basicType=kTRUE;
  classVersion=-1;
  setLength(nValues*sizeof(Short_t));
  memcpy(paramValue,value,arraySize);
}

void HParamBinObj::fill(const Text_t* name,Int_t* value,const Int_t nValues) {
  // Sets the name and value (Intt_t array of size nValues) of an existing object 
  SetName(name);
  paramType="iBin";
  basicType=kTRUE;
  classVersion=-1;
  setLength(nValues*sizeof(Int_t));
  memcpy(paramValue,value,arraySize);
}

void HParamBinObj::fill(const Text_t* name,Float_t* value,const Int_t nValues) {
  // Sets the name and value (Float_t array of size nValues) of an existing object 
  SetName(name);
  paramType="fBin";
  basicType=kTRUE;
  classVersion=-1;
  setLength(nValues*sizeof(Float_t));
  memcpy(paramValue,value,arraySize);
}

void HParamBinObj::fill(const Text_t* name,Double_t* value,const Int_t nValues) {
  // Sets the name and value (Double_t array of size nValues) of an existing object 
  SetName(name);
  paramType="dBin";
  basicType=kTRUE;
  classVersion=-1;
  setLength(nValues*sizeof(Double_t));
  memcpy(paramValue,value,arraySize);
}

Int_t HParamBinObj::getNumParams() {
  // Returns the number of values in the array, respectively 1 for classes
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
  // Prints the name and type of the parameters, respectively class name and version.
  // Prints also the numbers for an array of type Short_t, Int_t, Float_t, Double_t.
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

//-----------------------------------------------------------------------------------

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
                     const Char_t type,const Int_t n) {
  // Adds a string parameter to the list
  // name  = name of the parameter
  // value = string value
  // type  = 's' (default)
  // n     = number of characters (default s)
  paramList->Add(new HParamObj(name,value,type,n));
}

void HParamList::add(const Text_t* name,const Int_t value) {
  // Adds a parameter of type Int_t to the list
  const Char_t f[]="%i";
  Text_t buf[100];
  sprintf(buf,f,value);
  paramList->Add(new HParamObj(name,buf,'i',1));
}

void HParamList::add(const Text_t* name,const UInt_t value) {
  // Adds a parameter of type UInt_t to the list
  const Char_t f[]="%u";
  Text_t buf[100];
  sprintf(buf,f,value);
  paramList->Add(new HParamObj(name,buf,'u',1));
}

void HParamList::add(const Text_t* name,const Long_t value) {
  // Adds a parameter of type Long_t to the list
  const Char_t f[]="%ld";
  Text_t buf[100];
  sprintf(buf,f,value);
  paramList->Add(new HParamObj(name,buf,'l',1));
}

void HParamList::add(const Text_t* name,const Float_t value,const Int_t precision) {
  // Adds a parameter of type Float_t to the list
  // (default precision 3 in string conversion)
  Text_t f[100];
  sprintf(f,"%s%i%s","%.",precision,"f");
  Text_t buf[100];
  sprintf(buf,f,value);
  paramList->Add(new HParamObj(name,buf,'f',1));
}

void HParamList::add(const Text_t* name,const Double_t value,const Int_t precision) {
  // Adds a parameter of type Double_t to the list
  // (default precision 3 in string conversion)
  Text_t f[100];
  sprintf(f,"%s%i%s","%.",precision,"lf");
  Text_t buf[100];
  sprintf(buf,f,value);
  paramList->Add(new HParamObj(name,buf,'d',1));
}

void HParamList::add(const Text_t* name,TArrayI& value) {
  // Adds a parameter of type TArrayI to the list
  Int_t n=value.GetSize();
  const Char_t f[]="%i";
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
  // Adds a parameter of type TArrayC  to the list
  Int_t n=value.GetSize();
  const Char_t f[]="%i";
  Text_t buf[100];
  TString s;
  for(Int_t i=0;i<n;i++) {
    if (i>0) s.Append(",");
    sprintf(buf,f,int(value.At(i)));
    s.Append(buf);
  }
  paramList->Add(new HParamObj(name,s,'i',n));
}

void HParamList::add(const Text_t* name,TArrayF& value,const Int_t precision) {
  // Adds a parameter of type TArrayF to the list
  // (conversion to string with default precision 3)
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

void HParamList::add(const Text_t* name,TArrayD& value,const Int_t precision) {
  // Adds a parameter of type TArrayD to the list
  // (conversion to string with default precision 3)
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

void HParamList::addBinary(const Text_t* name,const UChar_t* values,const Int_t nValues) {
  // Adds a binary array of size nValues to the list
  paramBinList->Add(new HParamBinObj(name,values,nValues));
}

void HParamList::addBinary(const Text_t* name,const Short_t* values,const Int_t nValues) {
  // Adds an array of type Short_t and of size nValues as binary to the list
  paramBinList->Add(new HParamBinObj(name,values,nValues));
}

void HParamList::addBinary(const Text_t* name,const Int_t* values,const Int_t nValues) {
  // Adds an array of type Int_t and of size nValues as binary to the list
  paramBinList->Add(new HParamBinObj(name,values,nValues));
}

void HParamList::addBinary(const Text_t* name,const Float_t* values,const Int_t nValues) {
  // Adds an array of type Float_t and of size nValues as binary to the list
  paramBinList->Add(new HParamBinObj(name,values,nValues));
}

void HParamList::addBinary(const Text_t* name,const Double_t* values,const Int_t nValues) {
  // Adds an array of type Double_t and of size nValues as binary to the list
  paramBinList->Add(new HParamBinObj(name,values,nValues));
}

void HParamList::addBinary(const Text_t* name,TObject* obj) {
  // Adds a TObject to the list, sets the class version and the streamer info for
  // ROOT classes
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
  Char_t* buf=new char[len];
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
      Char_t* infobuf=new char[infolen];
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
  // Prints the parameter list including values
  TIter next(paramList);
  HParamObj* o;
  while ((o=(HParamObj*)next())!=0) o->print();
  TIter nextBin(paramBinList);
  HParamBinObj* b;
  while ((b=(HParamBinObj*)nextBin())!=0) b->print();
}

Bool_t HParamList::fill(const Text_t* name,Text_t* value,const Int_t length) {
  // Copies the data from the list object into the parameter value of type string
  HParamObj* o=(HParamObj*)paramList->FindObject(name);
  if (value==0) return kFALSE;
  if (o!=0 && o->getParamType()=='s') {
    const Char_t* v=o->getParamValue();
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
  // Copies the data from the list object into the parameter value of type Int_t
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
  // Copies the data from the list object into the parameter value of type UInt_t
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
  // Copies the data from the list object into the parameter value of type Long_t
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
  // Copies the data from the list object into the parameter value of type Float_t
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
  // Copies the data from the list object into the parameter value of type Double_t
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
  // Copies the data from the list object into the parameter value of type TArrayI
  // The array is resized, if the number of data is different.
  HParamObj* o=(HParamObj*)paramList->FindObject(name);
  if (value==0) return kFALSE;
  if (o!=0 && o->getParamType()=='i') {
    Int_t n=o->getNumParams();
    if (value->GetSize()!=n) value->Set(n);
    const Char_t d[]=",";
    Char_t* ss;
    Int_t v;
    Char_t s[4001];
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
  // Copies the data from the list object into the parameter value of type TArrayC
  // The array is resized, if the number of data is different.
  HParamObj* o=(HParamObj*)paramList->FindObject(name);
  if (value==0) return kFALSE;
  if (o!=0 && o->getParamType()=='i') {
    Int_t n=o->getNumParams();
    if (value->GetSize()!=n) value->Set(n);
    const Char_t d[]=",";
    Char_t* ss;
    Int_t v;
    Char_t s[4001];
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
  // Copies the data from the list object into the parameter value of type TArrayF
  // The array is resized, if the number of data is different.
  HParamObj* o=(HParamObj*)paramList->FindObject(name);
  if (value==0) return kFALSE;
  if (o!=0 && o->getParamType()=='f') {
    Int_t n=o->getNumParams();
    if (value->GetSize()!=n) value->Set(n);
    const Char_t d[]=",";
    Char_t* ss;
    Float_t v;
    Char_t s[4001];
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
  // Copies the data from the list object into the parameter value of type TArrayD
  // The array is resized, if the number of data is different.
  HParamObj* o=(HParamObj*)paramList->FindObject(name);
  if (value==0) return kFALSE;
  if (o!=0 && o->getParamType()=='d') {
    Int_t n=o->getNumParams();
    if (value->GetSize()!=n) value->Set(n);
    const Char_t d[]=",";
    Char_t* ss;
    Double_t v;
    Char_t s[4001];
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
  // Copies the data from the list object into the parameter array of type UChar_t.
  // The function returns an error, if the array size of the list object is not equal
  // to nValues. 
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
  // Copies the data from the list object into the parameter array of type Short_t.
  // The function returns an error, if the array size of the list object is not equal
  // to nValues. 
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
  // Copies the data from the list object into the parameter array of type Int_t.
  // The function returns an error, if the array size of the list object is not equal
  // to nValues. 
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
  // Copies the data from the list object into the parameter array of type Float_t.
  // The function returns an error, if the array size of the list object is not equal
  // to nValues. 
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
  // Copies the data from the list object into the parameter array of type Double_t.
  // The function returns an error, if the array size of the list object is not equal
  // to nValues. 
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
  // Copies the data from the list object into the parameter array of type UChar_t.
  // Resizes the array, if necessary, and returns the number of array elements.
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
  // Copies the data from the list object into the parameter array of type Short_t.
  // Resizes the array, if necessary, and returns the number of array elements
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
  // Copies the data from the list object into the parameter array of type Int_t.
  // Resizes the array, if necessary, and returns the number of array elements.
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
  // Copies the data from the list object into the parameter array of type Float_t.
  // Resizes the array, if necessary, and returns the number of array elements.
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
  // Copies the data from the list object into the parameter array of type Double_t.
  // Resizes the array, if necessary, and returns the number of array elements.
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
  // Fills the object obj (must exist!) via the Streamer and returns the class version.
  // Prints a warning if the class version in the list objects differs from the actual
  // class version.
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
      memcpy(buf->Buffer(),(Char_t*)o->getStreamerInfo(),len);
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
    memcpy(buf->Buffer(),(Char_t*)o->getParamValue(),len);
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
