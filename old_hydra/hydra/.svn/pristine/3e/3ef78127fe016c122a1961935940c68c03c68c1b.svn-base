//*-- AUTHOR : Ilse Koenig
//*-- Modified : 11/02/2000 by I. Koenig

/////////////////////////////////////////////////////////////
// HMdcGeomStruct
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
  array = new TObjArray(n);
  for(Int_t i=0;i<n;i++) (*array)[i]=new HMdcGeomSecStru(4);
  clear();
}

HMdcGeomStruct::~HMdcGeomStruct() {
  // destructor
  array->Delete();
  delete array;
}

Bool_t HMdcGeomStruct::init(HParIo* inp,Int_t* set) {
  // initializes the container from an input
  HDetParIo* input=inp->getDetParIo("HMdcParIo");
  Bool_t rc=kFALSE;
  if (input) {
    rc=input->init(this,set);
    if (rc) status=kTRUE;
  }
  return rc;
}

Int_t HMdcGeomStruct::write(HParIo* output) {
  // writes the container to an output
  HDetParIo* out=output->getDetParIo("HMdcParIo");
  if (out) return out->write(this);
  return -1;
}

void HMdcGeomStruct::readline(const char *buf, Int_t *set) {
  // decodes one line read from ascii file I/O
  Int_t sec, mod, lay, nwires, pos;
  sscanf(buf,"%i%i%i%i", &sec, &mod, &lay, &nwires);
  pos=sec*4+mod;
  if (!set[pos]) return;
  HMdcGeomStruct &pp = *this;
  pp[sec][mod].fill(lay, nwires);
  set[pos]=999;
}

void HMdcGeomStruct::putAsciiHeader(TString& header) {
  // puts the ascii header to the string used in HMdcParAsciiFileIo
  header=
    "# Parameters describing the geometrical sizes of the MDC\n"
    "# Format:\n"
    "# sector   module   layer   max number of wires\n";
}

Bool_t HMdcGeomStruct::writeline(char *buf, Int_t sec, Int_t mod, Int_t lay) {
  // writes one line to the buffer used by ascii file I/O
  Int_t nCells = (*this)[sec][mod][lay];
  if (nCells<=0) return kFALSE;
  sprintf(buf,"%1i %1i %1i %3i\n", sec, mod, lay, nCells);
  return kTRUE;
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
  resetInputVersions();
  status=kFALSE;
  changed=kFALSE;
}

void HMdcGeomStruct::printParam() {
  // prints the number of the cells
  cout<<"Number of Mdc cells:"<<'\n';
  for(Int_t i=0;i<6;i++) {
    for(Int_t j=0;j<4;j++) {
      for(Int_t k=0;k<6;k++) {
        Int_t nCells = (*this)[i][j][k];
        if (nCells>0) printf("%1i %1i %1i %3i\n",i,j,k,nCells);
      }
    }
  }
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





