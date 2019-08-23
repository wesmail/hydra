//*-- AUTHOR : Ilse Koenig
//*-- Modified : 02/11/98

/////////////////////////////////////////////////////////////
//HMdcRawStruct
//
// Container class for parameters decribing the hardware
// tree of an Mdc (sectors,modules,motherboards with the
// maximum number of TDCs on each motherboards.
// This information is needed to create the data tree and to
// create the lookup table for the Mdc. 
//
/////////////////////////////////////////////////////////////

#include "hmdcrawstruct.h"
#include "hades.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "hmdcdetector.h"
#include "hpario.h"
#include "hdetpario.h"
#include "heventfile.h"
#include <iostream.h>

ClassImp(HMdcRawMothStru)
ClassImp(HMdcRawModStru)
ClassImp(HMdcRawSecStru)
ClassImp(HMdcRawStruct)

HMdcRawModStru::HMdcRawModStru(Int_t n=16) {
  // constructor creates an array of pointers of type
  // HMdcRawMothStru
  array = new TObjArray(n);
  for(Int_t i=0;i<n;i++) (*array)[i]= new HMdcRawMothStru(0);
}

HMdcRawModStru::~HMdcRawModStru() {
  // destructor
  array->Delete();
}

HMdcRawSecStru::HMdcRawSecStru(Int_t n=4) {
  // constructor creates an array of pointers of type
  // HMdcRawModStru
  array = new TObjArray(n);
  for(Int_t i=0;i<n;i++) (*array)[i]= new HMdcRawModStru(16);
}

HMdcRawSecStru::~HMdcRawSecStru() {
  // destructor
  array->Delete();
}

HMdcRawStruct::HMdcRawStruct(Int_t n=6) : HParSet() {
  // constructor creates an array of pointers of type
  // HMdcRawSecStru
  // sets the name of the container to "MdcRawStruct"
  fName="MdcRawStruct";
  fTitle="Mdc parameters for hardware structure";
  strcpy(detName,"Mdc");
  status=kTRUE;
  array = new TObjArray(n);
  for(Int_t i=0;i<n;i++) (*array)[i]=new HMdcRawSecStru(4);
}

HMdcRawStruct::~HMdcRawStruct() {
  // destructor
  array->Delete();
}

Bool_t HMdcRawStruct::init(HParIo* inp,Int_t* set) {
  // intitializes the container from an input
  HDetParIo* input=inp->getDetParIo("HMdcParIo");
  if (input) return (input->init(this,set));
  return kFALSE;
}

Int_t HMdcRawStruct::write(HParIo* output) {
  // writes the container to an output
  HDetParIo* out=output->getDetParIo("HMdcParIo");
  if (out) return out->write(this);
  return -1;
}

void HMdcRawStruct::clear() {
  // clears the container
  for(Int_t i=0;i<6;i++) {
    HMdcRawSecStru& sec=(*this)[i];
    for(Int_t j=0;j<4;j++) {
      HMdcRawModStru& mod=sec[j];
      for(Int_t k=0;k<16;k++) {
        HMdcRawMothStru& moth=mod[k];
        moth.clear();
      }
    }
  }
  status=kFALSE;
  resetInputVersions();
}

void HMdcRawStruct::printNTdcs() {
  // prints the names (characterizes the position) of the
  // motherboards and the number of TDCs on each of them
  cout<<'\n'<<"Position identifier of motherboards, number of TDCs:"<<'\n';
  for(Int_t i=0;i<6;i++) {
    HMdcRawSecStru& sec=(*this)[i];
    cout<<"sector: "<<i+1<<'\n';
    for(Int_t j=0;j<4;j++) {
      HMdcRawModStru& mod=sec[j];
      for(Int_t k=0;k<16;k++) {
        cout<<mod[k].GetName()<<" ";
      }
      cout<<'\n'<<"  ";
      for(Int_t k=0;k<16;k++) {
        Int_t nt=mod[k].getNTdcs();
        if (nt<10) cout<<nt<<"     ";
        else cout<<nt<<"    ";
      }
      cout<<'\n';
    }
    cout<<'\n';
  }
  cout<<'\n';
}

