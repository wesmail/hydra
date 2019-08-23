//*-- AUTHOR : Ilse Koenig
//*-- Modified : 25/05/99

/////////////////////////////////////////////////////////////
//HMdcLookupGeom
//
//  Container class for lookup parameters to map the hardware
//  tree of the Mdc (sectors, modules, motherboards, TDC,
//  TDC channel) to the geometrie tree (sectors, modules,
//  layer, cell).
//  This information is needed for data mapping from raw
//  level to cal1 level
//
/////////////////////////////////////////////////////////////

#include "hmdclookupgeom.h"
#include "hades.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "hmdcdetector.h"
#include "hmdcrawstruct.h"
#include "hpario.h"
#include "hdetpario.h"
#include "heventfile.h"
#include <iostream.h>

ClassImp(HMdcLookupChan)
ClassImp(HMdcLookupMoth)
ClassImp(HMdcLookupGMod)
ClassImp(HMdcLookupGSec)
ClassImp(HMdcLookupGeom)

HMdcLookupMoth::HMdcLookupMoth(Int_t n, Text_t* name) {
  // constructor creates an array of pointers of type
  // HMdcLookupChan (array of all Tdc channels on a
  // motherboard)
  SetName(name);
  array = new TObjArray(n);
  for (int i = 0; i < n; i++) {
    (*array)[i] = new HMdcLookupChan();
  }
}

HMdcLookupMoth::~HMdcLookupMoth() {
  // destructor
  array->Delete();
  delete array;
}

HMdcLookupGMod::HMdcLookupGMod(Int_t sec=0, Int_t mod=0, Int_t n=16) {
  // constructor creates an array of pointers of type
  // HMdcLookupMoth
  array = new TObjArray(n);
  HMdcRawStruct* pMdc=
       (HMdcRawStruct*)gHades->getRuntimeDb()->getContainer("MdcRawStruct");
  if (pMdc) {
    HMdcRawSecStru& pS=(*pMdc)[sec];
    HMdcRawModStru& pM=pS[mod];
    for (Int_t i = 0; i < n; i++) {
      (*array)[i] = new HMdcLookupMoth(pM[i].getNTdcs(),(char*)pM[i].GetName());
    }
  }
  else cerr<<"HMdcLookupGeom not created, container MdcRawStruct not found"<<endl;
}

HMdcLookupGMod::~HMdcLookupGMod() {
  // destructor
  array->Delete();
  delete array;
}

HMdcLookupGSec::HMdcLookupGSec(Int_t sec=0,Int_t n=4) {
  // constructor creates an array of pointers of type
  // HMdcLookupGMod
  sector=sec;
  array = new TObjArray(n);
  for(Int_t i=0;i<n;i++) (*array)[i]= new HMdcLookupGMod(sector,i,16);
}

HMdcLookupGSec::~HMdcLookupGSec() {
  // destructor
  array->Delete();
  delete array;
}

HMdcLookupGeom::HMdcLookupGeom(Int_t n=6) : HParSet() {
  // constructor creates an array of pointers of type
  // HMdcLookupGSec
  // sets the name of the container to "MdcLookupGeom"
  fName="MdcLookupGeom";
  fTitle="Mdc lookup table from raw to cal1";
  strcpy(detName,"Mdc");
  array = new TObjArray(n);
  for(Int_t i=0;i<n;i++) (*array)[i]=new HMdcLookupGSec(i,4);
}

HMdcLookupGeom::~HMdcLookupGeom() {
  // destructor
  array->Delete();
  delete array;
}

Bool_t HMdcLookupGeom::init(HParIo* inp,Int_t* set) {
  // intitializes the container from an input
  HDetParIo* input=inp->getDetParIo("HMdcParIo");
  if (input) return (input->init(this,set));
  return kFALSE;
}

Int_t HMdcLookupGeom::write(HParIo* output) {
  // writes the container to an output
  HDetParIo* out=output->getDetParIo("HMdcParIo");
  if (out) return out->write(this);
  return -1;
}

void HMdcLookupGeom::clear() {
  // clears the container
  for(Int_t s=0;s<6;s++) {
    HMdcLookupGSec& sec=(*this)[s];
    for(Int_t m=0;m<4;m++) {
      HMdcLookupGMod& mod=sec[m];
      Int_t nl=mod.getSize();
      for(Int_t l=0;l<nl;l++) {
        HMdcLookupMoth& moth=mod[l];
        Int_t nm=moth.getSize();
        for(Int_t c=0;c<nm;c++) {
          HMdcLookupChan& chan=moth[c];
          chan.clear();
        }
      }
    }
  }
  status=kFALSE;
  resetInputVersions();
}


