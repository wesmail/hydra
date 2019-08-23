//*-- AUTHOR : Ilse Koenig
//*-- Modified : 17/01/2002

/////////////////////////////////////////////////////////////
//HMdcEvReadout
//
//  Container for lookup table needed by the unpacker to map
//  the readout addresses to a motherboard.
//  This container has no read functions in the I/O classes,
//  but is initialized from the lookup table HMdcMboReadout,
//  which contains the same information but accesssible with
//  the sector, module and motherboard number.
//
/////////////////////////////////////////////////////////////

#include "hmdcevreadout.h"
#include "hmdcmboreadout.h"
#include "hades.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "hmdcdetector.h"
#include "hmdcgeomstruct.h"
#include "hpario.h"
#include "hdetpario.h"
#include "TClass.h"
#include <iostream.h>

ClassImp(HMdcEvReadout)
ClassImp(HMdcEvReadoutSam)
ClassImp(HMdcEvReadoutRoc)
ClassImp(HMdcEvReadoutBusPos)
ClassImp(HMdcEvReadoutBus)

HMdcEvReadoutBusPos::HMdcEvReadoutBusPos(Int_t n) {
  // constructor creates an array of pointers of type
  // HMdcEvReadoutBus
  array = new TObjArray(n);
  for (int i = 0; i < n; i++) {
    (*array)[i] = new HMdcEvReadoutBus();
  }
}

HMdcEvReadoutBusPos::~HMdcEvReadoutBusPos() {
  // destructor
  array->Delete();
  delete array;
}


HMdcEvReadoutRoc::HMdcEvReadoutRoc(Int_t n) {
  // constructor creates an array of pointers of type
  // HMdcEvReadoutBusPos
  array = new TObjArray(n);
  for (int i = 0; i < n; i++) {
    (*array)[i] = new HMdcEvReadoutBusPos();
  }
}

HMdcEvReadoutRoc::~HMdcEvReadoutRoc() {
  // destructor
  array->Delete();
  delete array;
}


HMdcEvReadoutSam::HMdcEvReadoutSam(Int_t n) {
  // constructor creates an array of pointers of type
  // HMdcEvReadoutRoc
  array = new TObjArray(n);
  for (int i = 0; i < n; i++) {
    (*array)[i] = new HMdcEvReadoutRoc();
  }
}

HMdcEvReadoutSam::~HMdcEvReadoutSam() {
  // destructor
  array->Delete();
  delete array;
}


HMdcEvReadout::HMdcEvReadout(const char* name,const char* title,
                             const char* context,Int_t n)
              : HParSet(name,title,context) {
  // constructor creates an array of pointers of type
  // HMdcEvReadoutSam (array of 36 pointers)
  // creates the container "MdcMboReadout" if not existing
  strcpy(detName,"Mdc");
  array = new TObjArray(n);
  for (int i = 0; i < n; i++) {
    (*array)[i] = new HMdcEvReadoutSam();
  }
  HMdcMboReadout* p=
      (HMdcMboReadout*)gHades->getRuntimeDb()->getContainer("MdcMboReadout");
}

HMdcEvReadout::~HMdcEvReadout() {
  // destructor
  array->Delete();
  delete array;
}


Bool_t HMdcEvReadout::init(HParIo*,Int_t*) {
  // intitializes the container from MdcMboReadout container
  HMdcMboReadout* p=
      (HMdcMboReadout*)gHades->getRuntimeDb()->getContainer("MdcMboReadout");
  if (!p) return kFALSE;
  Int_t v1=p->getInputVersion(1);
  Int_t v2=p->getInputVersion(2);
  if (v1==versions[1] && v2==versions[2]) return kTRUE;
  // needs reinitialization
  Int_t sam, samNo;
  for(Int_t s=0;s<p->getSize();s++) {
    HMdcMboReadoutSec& sector=(*p)[s];
    for(Int_t m=0;m<sector.getSize();m++) {
      HMdcMboReadoutMod& module=sector[m];
      for(Int_t l=0;l<module.getSize();l++) {
        HMdcMboReadoutMoth& moth=module[l];
        sam=moth.getSam();
        if (sam>=0) {
          samNo=sam*2+moth.getSamPos();
          (*this)[samNo][moth.getRoc()][moth.getBusPos()][moth.getBus()].fill(s,m,l);
        }
      }
    }
  }
  versions[1]=v1;
  versions[2]=v2;
  changed=kTRUE;
  return kTRUE;
}


Int_t HMdcEvReadout::write(HParIo*) {
  // no output of this container
  changed=kFALSE;
  return 0;
}


void HMdcEvReadout::clear() {
  // clears the container

  HMdcEvReadout& readout=(*this);
  for(Int_t i=0;i<readout.getSize();i++) {
      HMdcEvReadoutSam& sam=(*this)[i];
      for(Int_t j=0;j<sam.getSize();j++) {
	  HMdcEvReadoutRoc& roc=sam[j];
	  for(Int_t l=0;l<roc.getSize();l++) {
	      HMdcEvReadoutBusPos& busPos=roc[l];
	      for(Int_t m=0;m<busPos.getSize();m++) {
		  busPos[m].clear();
	      }
	  }
      }
  }
  status=kFALSE;
  resetInputVersions();
}


void HMdcEvReadout::printParam() {
  // prints the content of the container
  Text_t buf[80];
  printf("MdcEvReadout container\n");
  printf("SAM  SAM-pos  ROC  BUS-pos  BUS   Sec  Mod  Mbo\n\n");
  HMdcEvReadout& readout=(*this);
  for(Int_t i=0;i<readout.getSize();i++) {
    HMdcEvReadoutSam& sam=(*this)[i];
    for(Int_t j=0;j<sam.getSize();j++) {
      HMdcEvReadoutRoc& roc=sam[j];
      for(Int_t l=0;l<roc.getSize();l++) {
        HMdcEvReadoutBusPos& busPos=roc[l];
        for(Int_t m=0;m<busPos.getSize();m++) {
          HMdcEvReadoutBus& bus=busPos[m];
          Int_t mbo=bus.getMbo();
          if (mbo>=0) {
             sprintf(buf,"%3i%3i%3i%3i%3i%6i%3i%3i\n",
                     i/2,i%2,j,l,m,bus.getSec(),bus.getMod(),mbo);
             printf(buf);
          }
        }
      }
    }
  }
}
