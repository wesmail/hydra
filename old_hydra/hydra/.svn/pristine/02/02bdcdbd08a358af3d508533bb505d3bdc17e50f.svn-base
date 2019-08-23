//*-- AUTHOR : Ilse Koenig
//*-- Modified : 11/03/99

/////////////////////////////////////////////////////////////
//HMdcMboReadout
//
// Container class for the readout addresses of all MDC
// motherboards
// This information is used by the packer raw->hld
//
/////////////////////////////////////////////////////////////

#include "hmdcmboreadout.h"
#include "hmdcrawstruct.h"
#include "hades.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "hmdcdetector.h"
#include "hpario.h"
#include "hdetpario.h"
#include "heventfile.h"
#include <iostream.h>

ClassImp(HMdcMboReadoutMoth)
ClassImp(HMdcMboReadoutMod)
ClassImp(HMdcMboReadoutSec)
ClassImp(HMdcMboReadout)

HMdcMboReadoutMod::HMdcMboReadoutMod(Int_t n=16) {
  // constructor creates an array of pointers of type
  // HMdcMboReadoutMoth
  array = new TObjArray(n);
  for(Int_t i=0;i<n;i++) (*array)[i]= new HMdcMboReadoutMoth();
}

HMdcMboReadoutMod::~HMdcMboReadoutMod() {
  // destructor
  array->Delete();
}

HMdcMboReadoutSec::HMdcMboReadoutSec(Int_t n=4) {
  // constructor creates an array of pointers of type
  // HMdcMboReadoutMod
  array = new TObjArray(n);
  for(Int_t i=0;i<n;i++) (*array)[i]= new HMdcMboReadoutMod(16);
}

HMdcMboReadoutSec::~HMdcMboReadoutSec() {
  // destructor
  array->Delete();
}

HMdcMboReadout::HMdcMboReadout(Int_t n=6) : HParSet() {
  // constructor creates an array of pointers of type
  // HMdcMboReadoutSec
  // sets the name of the container to "MdcMboReadout"
  fName="MdcMboReadout";
  fTitle="Mdc Mbo readout addresses";
  strcpy(detName,"Mdc");
  array = new TObjArray(n);
  for(Int_t i=0;i<n;i++) (*array)[i]=new HMdcMboReadoutSec(4);
  HMdcRawStruct* pMdc=
       (HMdcRawStruct*)gHades->getRuntimeDb()->getContainer("MdcRawStruct");
  if (pMdc) {
    for(Int_t i=0;i<6;i++) {
      for(Int_t j=0;j<4;j++) {
        for(Int_t k=0;k<16;k++)
          (*this)[i][j][k].SetName((char*)((*pMdc)[i][j][k].GetName()));
      }
    }
  } else {
    cerr<<"MBO names in HMdcMboReadout not initialized"<<endl;
    cerr<<"container MdcRawStruct not found"<<endl;
  }
}

HMdcMboReadout::~HMdcMboReadout() {
  // destructor
  array->Delete();
}

Bool_t HMdcMboReadout::init(HParIo* inp,Int_t* set) {
  // intitializes the container from an input
  HDetParIo* input=inp->getDetParIo("HMdcParIo");
  if (input) return (input->init(this,set));
  return kFALSE;
}

Int_t HMdcMboReadout::write(HParIo* output) {
  // writes the container to an output
  HDetParIo* out=output->getDetParIo("HMdcParIo");
  if (out) return out->write(this);
  return -1;
}

void HMdcMboReadout::clear() {
  // clears the container
  for(Int_t i=0;i<this->getSize();i++) {
    HMdcMboReadoutSec& sec=(*this)[i];
    for(Int_t j=0;j<sec.getSize();j++) {
      HMdcMboReadoutMod& mod=sec[j];
      for(Int_t k=0;k<mod.getSize();k++) {
        HMdcMboReadoutMoth& moth=mod[k];
        moth.clear();
      }
    }
  }
  status=kFALSE;
  resetInputVersions();
}

void HMdcMboReadout::printParam() {
  // prints the names (characterizes the position on the chamber) of the
  // motherboards and the readout addresses;
  cout<<"------------------------------------------------------------"<<'\n';
  cout<<'\n'<<"Position identifier of motherboards"<<'\n'
      <<"BUS"<<"  BUS position"<<"  ROC"<<"  SAM position"<<"  SAM"
      <<'\n'<<'\n';
  HMdcDetector* set=(HMdcDetector*)gHades->getSetup()->getDetector("Mdc");
  if (set) {
    Text_t buf[155];
    for(Int_t i=0;i<this->getSize();i++) {
      HMdcMboReadoutSec& sec=(*this)[i];
      for(Int_t j=0;j<sec.getSize();j++) {
        HMdcMboReadoutMod& mod=sec[j];
        if (set->getModule(i,j)) {
	  cout<<"sector: "<<i<<"  module: "<<j<<'\n';
          for(Int_t k=0;k<mod.getSize();k++) {
            HMdcMboReadoutMoth& moth=mod[k];
            Int_t bus=moth.getBus();
            if (bus!=-1) {
              sprintf(buf,"%3i%3i%4i  %s%6i%3i%3i%4i%3i\n",i,j,k,
                moth.GetName(),bus,moth.getBusPos(),moth.getRoc(),
                moth.getSamPos(),moth.getSam());
              cout<<buf;
            }
          }  
          cout<<'\n';
        }
      }
    }
  }
  cout<<"------------------------------------------------------------"<<'\n';
}
