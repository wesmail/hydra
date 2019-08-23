//*-- AUTHOR : Ilse Koenig
//*-- Modified : 25/05/99

/////////////////////////////////////////////////////////////
//HMdcGeomStruct
//
// Container class for parameters decribing the geometrical
// tree of an Mdc (sectors,modules,layers with the number of
// cells in each layer.
// This information is needed to create the data tree and to
// create the parameter containers for the Mdc. 
//
/////////////////////////////////////////////////////////////

#include "hmdcgeomstruct.h"
#include "hades.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "hmdcdetector.h"
#include "hpario.h"
#include "hdetpario.h"
#include "heventfile.h"
#include <iostream.h>

ClassImp(HMdcGeomModStru)
ClassImp(HMdcGeomSecStru)
ClassImp(HMdcGeomStruct)

HMdcGeomSecStru::HMdcGeomSecStru(Int_t n=4) {
  // constructor creates an array of pointers of type
  // HMdcGeomModStru each containing an Int_t array[6]
  // to hold the number of the cells in each layer
  array = new TObjArray(n);
  for(Int_t i=0;i<n;i++) (*array)[i]= new HMdcGeomModStru();
}

HMdcGeomSecStru::~HMdcGeomSecStru() {
  // destructor
  array->Delete();
  delete array;
}

HMdcGeomStruct::HMdcGeomStruct(Int_t n=6) : HParSet() {
  // constructor creates an array of pointers of type
  // HMdcGeomSecStru
  // sets the name of the container to "MdcGeomStruct"
  fName="MdcGeomStruct";
  fTitle="Mdc parameters for geometry structure";
  strcpy(detName,"Mdc");
  status=kTRUE;
//  breakPoints=0;
  array = new TObjArray(n);
  for(Int_t i=0;i<n;i++) (*array)[i]=new HMdcGeomSecStru(4);
}

HMdcGeomStruct::~HMdcGeomStruct() {
  // destructor
  array->Delete();
  delete array;
//  delete breakPoints;
}

Bool_t HMdcGeomStruct::init(HParIo* inp,Int_t* set) {
  // intitializes the container from an input
  HDetParIo* input=inp->getDetParIo("HMdcParIo");
  if (input) return (input->init(this,set));
  return kFALSE;
}

Int_t HMdcGeomStruct::write(HParIo* output) {
  // writes the container to an output
  HDetParIo* out=output->getDetParIo("HMdcParIo");
  if (out) return out->write(this);
  return -1;
}

void HMdcGeomStruct::clear() {
  // clears the container
  for(Int_t i=0;i<6;i++) {
    HMdcGeomSecStru& sec=(*this)[i];
    for(Int_t j=0;j<4;j++) {
      HMdcGeomModStru& mod=sec[j];
      mod.clear();
    }
  }
  status=kFALSE;
  resetInputVersions();
}

void HMdcGeomStruct::printParam() {
  // prints the number of the cells
  cout<<"Number of Mdc cells:"<<'\n';
  for(Int_t i=0;i<6;i++) {
    HMdcGeomSecStru& sec=(*this)[i];
    cout<<"sector: "<<i+1<<'\n';
    for(Int_t j=0;j<4;j++) {
      HMdcGeomModStru& mod=sec[j];
      for(Int_t k=0;k<6;k++) { cout<<" "<<mod[k]; }
      cout<<'\n';
    }
    cout<<'\n';
  }
  cout<<'\n';
}


void HMdcGeomStruct::getMaxIndices(TArrayI* ind) {
  // return the maximum indices of the sectors, modules, layers,
  // cells in the actual setup
  Int_t  maxSec=-1, maxMod=-1, maxLay=-1,maxCell=-1;
  for(Int_t i=0;i<6;i++) {
    HMdcGeomSecStru& sec=(*this)[i];
    for(Int_t j=0;j<4;j++) {
      HMdcGeomModStru& mod=sec[j];
      for(Int_t k=0;k<6;k++) {
        Int_t n=mod[k]-1;  // index is number of cells -1
        if (n>=0) {
          maxSec=(i>maxSec)?i:maxSec;
          maxMod=(j>maxMod)?j:maxMod;
          maxLay=(k>maxLay)?k:maxLay;
          maxCell=(n>maxCell)?n:maxCell;
        }
      }
    }
  }
  ind->AddAt(maxSec,0);
  ind->AddAt(maxMod,1);
  ind->AddAt(maxLay,2);
  ind->AddAt(maxCell,3);
}
