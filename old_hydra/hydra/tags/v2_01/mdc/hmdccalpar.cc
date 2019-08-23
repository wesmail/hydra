//*-- AUTHOR : Ilse Koenig
//*-- Modified : 02/11/98

/////////////////////////////////////////////////////////////
//HMdcCalPar
//
// Container class for Mdc calibration parameters
//
/////////////////////////////////////////////////////////////

#include "hmdccalpar.h"
#include "hades.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "hmdcdetector.h"
#include "hpario.h"
#include "hdetpario.h"
#include "hmdcgeomstruct.h"
#include <iostream.h>

ClassImp(HMdcCalParCell)
ClassImp(HMdcCalParLayer)
ClassImp(HMdcCalParMod)
ClassImp(HMdcCalParSec)
ClassImp(HMdcCalPar)

HMdcCalParLayer::HMdcCalParLayer(Int_t n=0) {
  // constructor creates a TClonesArray of type
  // HMdcCalParCell
  array = new TClonesArray("HMdcCalParCell", n);
  for (Int_t i = 0; i < n; ++i)
       new((*array)[i]) HMdcCalParCell;
}

HMdcCalParLayer::~HMdcCalParLayer() {
  // destructor
  array->Delete();
}

Int_t HMdcCalParLayer::getSize() {
  // return the size of the pointer array
  return array->GetLast()+1;
}

HMdcCalParMod::HMdcCalParMod(Int_t sec=0, Int_t mod=0, Int_t n = 6) {
  // constructor creates an array of pointers of type
  // HMdcCalParLayer
  array = new TObjArray(n);
  HMdcGeomStruct* pMdc=
       (HMdcGeomStruct*)gHades->getRuntimeDb()->getContainer("MdcGeomStruct");
  if (pMdc) {
    HMdcGeomSecStru& pS=(*pMdc)[sec];
    HMdcGeomModStru& pM=pS[mod];
    for (Int_t i = 0; i < n; i++) {
      (*array)[i] = new HMdcCalParLayer(pM[i]);
    }
  }
  else cerr<<"HMdcCalParMod not created, container MdcGeomStruct not found"<<endl;
}

HMdcCalParMod::~HMdcCalParMod() {
  // destructor
  array->Delete();
}

Int_t HMdcCalParMod::getSize() {
  // return the size of the pointer array
  return array->GetEntries();
}

HMdcCalParSec::HMdcCalParSec(Int_t sec=0, Int_t n = 4) {
  // constructor creates an array of pointers of type
  // HMdcCalParMod
  sector=sec;
  array = new TObjArray(n);
  for (Int_t i = 0; i < n; i++)
      (*array)[i] = new HMdcCalParMod(sector,i,6);
}

HMdcCalParSec::~HMdcCalParSec() {
  // destructor
  array->Delete();
}

Int_t HMdcCalParSec::getSize() {
  // return the size of the pointer array
  return array->GetEntries();
}

HMdcCalPar::HMdcCalPar() : HParSet() {
  // constructor creates an array of pointers of type
  // HMdcCalParSec
  // The container name is set to "MdcCalPar"
  fName="MdcCalPar";
  fTitle="calibration parameters for Mdc";
  strcpy(detName,"Mdc");
  array = new TObjArray(6);
  for (Int_t i = 0; i < 6; i++)
      (*array)[i] = new HMdcCalParSec(i, 4);
//  breakPoints=0;
  ntuple=0;
}

HMdcCalPar::~HMdcCalPar() {
  // destructor
  array->Delete();
//  delete breakPoints;
  delete ntuple;
}

Int_t HMdcCalPar::getSize() {
  // return the size of the pointer array
  return array->GetEntries();
}

Bool_t HMdcCalPar::init(HParIo* inp,Int_t* set) {
  // intitializes the container from an input
  HDetParIo* input=inp->getDetParIo("HMdcParIo");
  if (input) return (input->init(this,set));
  return kFALSE;
}

Int_t HMdcCalPar::write(HParIo* output) {
  // writes the container to an output
  HDetParIo* out=output->getDetParIo("HMdcParIo");
  if (out) return out->write(this);
  return -1;
}

void HMdcCalPar::clear() {
  // clears the container
  for(Int_t s=0;s<6;s++) {
    HMdcCalParSec& sec=(*this)[s];
    for(Int_t m=0;m<4;m++) {
      HMdcCalParMod& mod=sec[m];
      Int_t nl=mod.getSize();
      for(Int_t l=0;l<nl;l++) {
        HMdcCalParLayer& lay=mod[l];
        Int_t nc=lay.getSize();
        for(Int_t n=0;n<nc;n++) {
          HMdcCalParCell& cell=lay[n];
          cell.clear();
        }
      }
    }
  }
  status=kFALSE;
  resetInputVersions();
}

TNtuple* HMdcCalPar::getNtuple() {
  // fills the calibration parameters into an TNtuple for drawing
  if (ntuple) ntuple->Reset();
  else ntuple=new TNtuple("MdcCalParNtuple","Ntuple of Mdc calibration parameters",
                          "s:m:l:c:slo:off");
  Float_t slo, off;
  for(Int_t s=0;s<6;s++) {
    HMdcCalParSec& sec=(*this)[s];
    for(Int_t m=0;m<4;m++) {
      HMdcCalParMod& mod=sec[m];
      Int_t nl=mod.getSize();
      for(Int_t l=0;l<nl;l++) {
        HMdcCalParLayer& lay=mod[l];
        Int_t nc=lay.getSize();
        for(Int_t n=0;n<nc;n++) {
          HMdcCalParCell& cell=lay[n];
          slo=cell.getSlope();
          off=cell.getOffset();
          ntuple->Fill(s,m,l,n,slo,off);
        }
      }
    }
  }
  return ntuple;
}
