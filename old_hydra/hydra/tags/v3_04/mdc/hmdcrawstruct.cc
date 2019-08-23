//*-- AUTHOR : Ilse Koenig
//*-- Modified : 06/09/99 by Ilse Koenig

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

HMdcRawModStru::HMdcRawModStru(Int_t n) {
  // constructor creates an array of pointers of type
  // HMdcRawMothStru
  array = new TObjArray(n);
  for(Int_t i=0;i<n;i++) (*array)[i]= new HMdcRawMothStru();
}


HMdcRawModStru::~HMdcRawModStru() {
  // destructor
  array->Delete();
  delete array;
}


Int_t HMdcRawModStru::getMboIndex(const Text_t* mbo) {
  // returns the index of the MBO given by its logical position
  for(Int_t i=0;i<getNMotherboards();i++) {
    const Text_t* name=array->At(i)->GetName();
    if (strcmp(name,mbo)==0) return i;
  }
  return -1;
}


HMdcRawSecStru::HMdcRawSecStru(Int_t n) {
  // constructor creates an array of pointers of type
  // HMdcRawModStru
  array = new TObjArray(n);
  for(Int_t i=0;i<n;i++) (*array)[i]= new HMdcRawModStru(16);
}


HMdcRawSecStru::~HMdcRawSecStru() {
  // destructor
  array->Delete();
  delete array;
}


HMdcRawStruct::HMdcRawStruct(Int_t n) : HParSet() {
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
  delete array;
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


void HMdcRawStruct::printParam() {
  // prints the names (characterizes the position on the chamber) of the
  // motherboards and the number of TDCs on each of them;
  cout<<"------------------------------------------------------------"<<'\n';
  cout<<'\n'<<"Position identifier of motherboards"<<'\n'<<"number of TDCs"
      <<'\n'<<'\n';
  HMdcDetector* set=(HMdcDetector*)gHades->getSetup()->getDetector("Mdc");
  if (set) {
    for(Int_t i=0;i<6;i++) {
      HMdcRawSecStru& sec=(*this)[i];
      for(Int_t j=0;j<4;j++) {
        HMdcRawModStru& mod=sec[j];
        if (set->getModule(i,j)) {
	  cout<<"sector: "<<i<<"  module: "<<j<<'\n';
          for(Int_t k=0;k<16;k++) {
            cout<<mod[k].GetName()<<" ";
          }
          cout<<'\n';
          for(Int_t k=0;k<16;k++) {
            Int_t nt=mod[k].getNTdcs();
            if (nt<10) cout<<"  "<<nt<<" ";
            else cout<<" "<<nt<<" ";
          }
          cout<<'\n'<<'\n';
        }
      }
    }
    cout<<"------------------------------------------------------------"<<'\n';
  }
}
          

void HMdcRawStruct::getMaxIndices(TArrayI* ind) {
  // returns the maximum indices of the sectors, modules, motherborads,
  // tdc channels in the actual setup
  Int_t maxSec=-1, maxMod=-1, maxMbo=-1, maxTdc=-1;
  for(Int_t i=0;i<6;i++) {
    HMdcRawSecStru& sec=(*this)[i];
    for(Int_t j=0;j<4;j++) {
      HMdcRawModStru& mod=sec[j];
      for(Int_t k=0;k<16;k++) {
        Int_t nt=mod[k].getNTdcs()-1; // index is number of tds -1
        if (nt>=0) {
          maxSec=(i>maxSec)?i:maxSec;
          maxMod=(j>maxMod)?j:maxMod;
          maxMbo=(k>maxMbo)?k:maxMbo;
          maxTdc=(nt>maxTdc)?nt:maxTdc;
        }
      }
    }
  }
  ind->AddAt(maxSec,0);
  ind->AddAt(maxMod,1);
  ind->AddAt(maxMbo,2);
  ind->AddAt(maxTdc,3);
}





