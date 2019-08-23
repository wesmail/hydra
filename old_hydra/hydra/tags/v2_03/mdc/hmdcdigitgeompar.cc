//*-- AUTHOR : Alexander Nekhaev
//*-- Modified : 01/02/99

////////////////////////////////////////////////////////////////
// HMdcDigitGeomPar
//
// Container class for Mdc digitisation geometrical parameters
//
////////////////////////////////////////////////////////////////

#include "hmdcdigitgeompar.h"
#include "hades.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "hmdcdetector.h"
#include "hpario.h"
#include "hdetpario.h"
#include "hmdcgeomstruct.h"
#include <iostream.h>

ClassImp(HMdcDigitGeomParLayer)
ClassImp(HMdcDigitGeomParMod)
ClassImp(HMdcDigitGeomParSec)
ClassImp(HMdcDigitGeomPar)


HMdcDigitGeomParMod::HMdcDigitGeomParMod() {
  array = new TObjArray(6);
  for (Int_t i = 0; i < 6; i++)
      (*array)[i] = new HMdcDigitGeomParLayer();
}


HMdcDigitGeomParMod::~HMdcDigitGeomParMod() {
  array->Delete();
}

Int_t HMdcDigitGeomParMod::getSize() {
  return array->GetEntries(); // return the size of the pointer array
}

HMdcDigitGeomParSec::HMdcDigitGeomParSec() {
  // constructor creates an array of pointers of type
  // HMdcDigitGeomParMod
  array = new TObjArray(4);
  for (Int_t i = 0; i < 4; i++)
      (*array)[i] = new HMdcDigitGeomParMod;
}

HMdcDigitGeomParSec::~HMdcDigitGeomParSec() {
  array->Delete();
}

Int_t HMdcDigitGeomParSec::getSize() {
  return array->GetEntries(); // return the size of the pointer array
}

HMdcDigitGeomPar::HMdcDigitGeomPar() : HParSet() {
  // constructor creates an array of pointers of type
  // HMdcDigitGeomParSec
  // The container name is set to "MdcDigitGeomPar"
  fName="MdcDigitGeomPar";
  fTitle="digitisation parameters for Mdc";
  strcpy(detName,"Mdc");
  array = new TObjArray(6);
  for (Int_t i = 0; i < 6; i++)
      (*array)[i] = new HMdcDigitGeomParSec;
  HMdcGeomStruct* pMdc=
       (HMdcGeomStruct*)gHades->getRuntimeDb()->getContainer("MdcGeomStruct");
  if (pMdc) {
    for(Int_t i=0;i<6;i++) {
      for(Int_t j=0;j<4;j++) {
        for(Int_t k=0;k<6;k++)
          (*this)[i][j][k].setNumWires(((*pMdc)[i][j][k]));
      }
    }
  } else {
    cerr<<"numWires in HMdcDigitGeomParMod not initialized"<<endl;
    cerr<<"container MdcGeomStruct not found"<<endl;
  }
   ntuple=0;
}

HMdcDigitGeomPar::~HMdcDigitGeomPar() {
  // destructor
  array->Delete();
//  delete breakPoints;
  delete ntuple;
}

Int_t HMdcDigitGeomPar::getSize() {
  // return the size of the pointer array
  return array->GetEntries();
}

Bool_t HMdcDigitGeomPar::init(HParIo* inp,Int_t* set) {
  // intitializes the container from an input
  HDetParIo* input=inp->getDetParIo("HMdcParIo");
  if (input) return (input->init(this,set));
  return kFALSE;
}

Int_t HMdcDigitGeomPar::write(HParIo* output) {
  // writes the container to an output
  HDetParIo* out=output->getDetParIo("HMdcParIo");
  if (out) return out->write(this);
  return -1;
}

void HMdcDigitGeomPar::clear() {
  // clears the container
  for(Int_t s=0;s<6;s++) {
    HMdcDigitGeomParSec& sec=(*this)[s];
    for(Int_t m=0;m<4;m++) {
      HMdcDigitGeomParMod& mod=sec[m];
      Int_t nl=mod.getSize();
      for(Int_t l=0;l<nl;l++) {
        HMdcDigitGeomParLayer& lay=mod[l];
	lay.clear();
      }
    }
  }
  status=kFALSE;
  resetInputVersions();
}

TNtuple* HMdcDigitGeomPar::getNtuple() {
  // fills the digitisation parameters into an TNtuple for drawing
  if (ntuple) ntuple->Reset();
  else ntuple=new TNtuple("MdcDigitGeomParNtuple","Ntuple of Mdc digitisation parameters","s:m:l:pt:nw:cd:wor:wof");
  Int_t nw;
  Float_t pt, cd, wor, wof;
  for(Int_t s=0;s<6;s++) {
    HMdcDigitGeomParSec& sec=(*this)[s];
    for(Int_t m=0;m<4;m++) {
      HMdcDigitGeomParMod& mod=sec[m];
      Int_t nl=mod.getSize();
      for(Int_t l=0;l<nl;l++) {
        HMdcDigitGeomParLayer& lay=mod[l];
        pt=lay.getPitch();       
        nw=lay.getNumWires();       
        cd=lay.getCatDist();       
        wor=lay.getWireOrient();       
        wof=lay.getCentralWireNr();       
	ntuple->Fill(s,m,l,pt,nw,cd,wor,wof);
      }
    }
  }
  return ntuple;
}




