//*-- AUTHOR : Alexander Nekhaev
//*-- Modified : 01/02/99

////////////////////////////////////////////////////////////////
// HMdcDigitPar
//
// Container class for Mdc digitisation geometrical parameters
//
////////////////////////////////////////////////////////////////

#include "hmdcdigitpar.h"
#include "hades.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "hmdcdetector.h"
#include "hpario.h"
#include "hdetpario.h"
//#include "hmdcgeomstruct.h"
#include <iostream.h>

ClassImp(HMdcDigitParLayer)
ClassImp(HMdcDigitParMod)
ClassImp(HMdcDigitParSec)
ClassImp(HMdcDigitPar)


HMdcDigitParMod::HMdcDigitParMod() {
  array = new TObjArray(6);
  for (Int_t i = 0; i < 6; i++) {
    (*array)[i] = new HMdcDigitParLayer;
  }
}


HMdcDigitParMod::~HMdcDigitParMod() {
  array->Delete();
}

Int_t HMdcDigitParMod::getSize() {
  return array->GetEntries(); // return the size of the pointer array
}

HMdcDigitParSec::HMdcDigitParSec() {
  // constructor creates an array of pointers of type
  // HMdcDigitParMod
  array = new TObjArray(4);
  for (Int_t i = 0; i < 4; i++)
      (*array)[i] = new HMdcDigitParMod;
}

HMdcDigitParSec::~HMdcDigitParSec() {
  array->Delete();
}

Int_t HMdcDigitParSec::getSize() {
  return array->GetEntries(); // return the size of the pointer array
}

HMdcDigitPar::HMdcDigitPar() : HParSet() {
  // constructor creates an array of pointers of type
  // HMdcDigitParSec
  // The container name is set to "MdcDigitPar"
  fName="MdcDigitPar";
  fTitle="digitisation parameters for Mdc";
  strcpy(detName,"Mdc");
  array = new TObjArray(6);
  for (Int_t i = 0; i < 6; i++)
      (*array)[i] = new HMdcDigitParSec;
  ntuple=0;
}

HMdcDigitPar::~HMdcDigitPar() {
  // destructor
  array->Delete();
//  delete breakPoints;
  delete ntuple;
}

Int_t HMdcDigitPar::getSize() {
  // return the size of the pointer array
  return array->GetEntries();
}

Bool_t HMdcDigitPar::init(HParIo* inp,Int_t* set) {
  // intitializes the container from an input
  HDetParIo* input=inp->getDetParIo("HMdcParIo");
  if (input) return (input->init(this,set));
  return kFALSE;
}

Int_t HMdcDigitPar::write(HParIo* output) {
  // writes the container to an output
  HDetParIo* out=output->getDetParIo("HMdcParIo");
  if (out) return out->write(this);
  return -1;
}

void HMdcDigitPar::clear() {
  // clears the container
  for(Int_t s=0;s<6;s++) {
    HMdcDigitParSec& sec=(*this)[s];
    for(Int_t m=0;m<4;m++) {
      HMdcDigitParMod& mod=sec[m];
      Int_t nl=mod.getSize();
      for(Int_t l=0;l<nl;l++) {
        HMdcDigitParLayer& lay=mod[l];
	lay.clear();
      }
    }
  }
  status=kFALSE;
  resetInputVersions();
}

TNtuple* HMdcDigitPar::getNtuple() {
  // fills the digitisation parameters into an TNtuple for drawing
  if (ntuple) ntuple->Reset();
  else ntuple=new TNtuple("MdcDigitParNtuple","Ntuple of Mdc digitisation parameters","s:m:l:dv:me:sp0:sp1:sp2");
  Float_t dv, me, sp0, sp1, sp2;
  for(Int_t s=0;s<6;s++) {
    HMdcDigitParSec& sec=(*this)[s];
    for(Int_t m=0;m<4;m++) {
      HMdcDigitParMod& mod=sec[m];
      Int_t nl=mod.getSize();
      for(Int_t l=0;l<nl;l++) {
        HMdcDigitParLayer& lay=mod[l];
        dv=lay.getDriftVelocity();       
        me=lay.getMdcEfficiency();       
        sp0=lay.getSpacePar0();       
        sp1=lay.getSpacePar1();       
        sp2=lay.getSpacePar2();       
	ntuple->Fill(s,m,l,dv,me,sp0,sp1,sp2);
      }
    }
  }
  return ntuple;
}




