//*-- AUTHOR : J. Kempter
//*-- Modified : 18/02/2000 by I. Koenig

////////////////////////////////////////////////////////////////////////////
// HMdcCalParRaw
//
// Container class for the TDC calibration parameters of the MDC
//
////////////////////////////////////////////////////////////////////////////
#include <stdlib.h>

#include "hmdccalparraw.h"
#include "hades.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "hmdcdetector.h"
#include "hpario.h"
#include "hdetpario.h"
#include "hmdcrawstruct.h"
#include <iostream.h>

ClassImp(HMdcCalParTdc)
ClassImp(HMdcCalParMbo)
ClassImp(HMdcCalParRawMod)
ClassImp(HMdcCalParRawSec)
ClassImp(HMdcCalParRaw)

void HMdcCalParTdc::fill(HMdcCalParTdc& r) {
  slope=r.getSlope();
  offset=r.getOffset();
  slopeErr=r.getSlopeErr();
  offsetErr=r.getOffsetErr();
  slopeMethod=r.getSlopeMethod();
  offsetMethod=r.getOffsetMethod();
}

HMdcCalParMbo::HMdcCalParMbo(Int_t tdc, Text_t* name) {
  // constructor takes to number of TDC channels and the name of the MBO
  SetName(name);
  array = new TObjArray(tdc);
  for (Int_t i=0; i<tdc; ++i) (*array)[i] = new HMdcCalParTdc();
}

HMdcCalParMbo::~HMdcCalParMbo() {
  // destructor
  array->Delete();
  delete array;
}

HMdcCalParRawMod::HMdcCalParRawMod(Int_t sec, Int_t mod, Int_t mbo) {
  // constructor takes the sector, module and MBO number
  array = new TObjArray(mbo);
  HMdcRawStruct* pMdc=
       (HMdcRawStruct*)gHades->getRuntimeDb()->getContainer("MdcRawStruct");
  if (pMdc) {
    HMdcRawModStru& rMod=(*pMdc)[sec][mod];
    for (Int_t i=0; i<rMod.getSize(); i++) {
      HMdcRawMothStru& rMbo=rMod[i];
      (*array)[i] = new HMdcCalParMbo(rMbo.getNTdcs(),(char*)rMbo.GetName());
    }
  }
  else cerr<<"HMdcCalParRaw not created, container MdcRawStruct not found"<<endl;
}

HMdcCalParRawMod::~HMdcCalParRawMod() {
  // destructor
  array->Delete();
  delete array;
}

HMdcCalParRawSec::HMdcCalParRawSec(Int_t sec, Int_t mod) {
  // constructor takes the sector, module number
  array = new TObjArray(mod);
  for (Int_t i=0; i<mod; i++)
        (*array)[i] = new HMdcCalParRawMod(sec,i);
}

HMdcCalParRawSec::~HMdcCalParRawSec() {
  // destructor
  array->Delete();
  delete array;
}

HMdcCalParRaw::HMdcCalParRaw(Int_t n) : HParSet() {
  // constructor set the name of the container to "MdcCalParRaw"
  fName="MdcCalParRaw";
  fTitle="raw calibration parameters for Mdc";
  strcpy(detName,"Mdc");
  array = new TObjArray(n);
  for (Int_t i=0; i<n; i++) (*array)[i] = new HMdcCalParRawSec(i);
}

HMdcCalParRaw::~HMdcCalParRaw() {
  // destructor
  array->Delete();
  delete array;
}

Bool_t HMdcCalParRaw::init(HParIo* inp,Int_t* set) {
  // intitializes the container from an input
  HDetParIo* input=inp->getDetParIo("HMdcParIo");
  if (input) return (input->init(this,set));
  return kFALSE;
}

Int_t HMdcCalParRaw::write(HParIo* output) {
  // writes the container to an output
  HDetParIo* out=output->getDetParIo("HMdcParIo");
  if (out) return out->write(this);
  return -1;
}

void HMdcCalParRaw::readline(const char* buf, Int_t* set) {
  // decodes one line read from ascii file I/O and calls HMdcCalParTdc::fill(...) 
  Int_t   sec, mod, mbo, tdc, slopeMethod,offsetMethod;
  Float_t slope, offset, slopeErr,offsetErr;
  char mboName[10];
  sscanf(buf,"%i%i%i%s%i%f%f%f%f%i%i",
         &sec, &mod, &mbo, mboName, &tdc, &slope, &offset, &slopeErr,
         &offsetErr, &slopeMethod, &offsetMethod);
  Int_t n=sec*4+mod;
  if (!set[n]) return;
  HMdcCalParMbo& rMbo=(*this)[sec][mod][mbo];
  rMbo.SetName(mboName);
  HMdcCalParTdc& t=rMbo[tdc];
  t.fill(slope,offset,slopeErr,offsetErr,slopeMethod,offsetMethod);
  set[n]=999;
}

void HMdcCalParRaw::putAsciiHeader(TString& header) {
  // puts the ascii header to the string used in HMdcParAsciiFileIo
  header=
    "# Calibration parameters of the MDC\n"
    "# Format:\n"
    "# sector  module  mbo  mboName  tdc  slope  offset  slopeErr  offsetErr  slopeMethod  offsetMethod\n";
}

Bool_t HMdcCalParRaw::writeline(char *buf, Int_t sec, Int_t mod, Int_t mbo, Int_t tdc) {
  // writes one line to the buffer used by ascii file I/O
  HMdcCalParMbo& rMbo=(*this)[sec][mod][mbo];
  HMdcCalParTdc& chan=rMbo[tdc];
  sprintf(buf,"%1i %1i %2i %s %3i % 10.5f % 10.2f % 12.5f % 12.2f %1i %1i \n",
          sec, mod, mbo, rMbo.GetName(), tdc,
          chan.getSlope(), chan.getOffset(), chan.getSlopeErr(),
          chan.getOffsetErr(),chan.getSlopeMethod(), chan.getOffsetMethod());
  return kTRUE;
}

void HMdcCalParRaw::clear() {
  // clears the container
  for(Int_t s=0;s<getSize();s++) {
    HMdcCalParRawSec& sec=(*this)[s];
    for(Int_t m=0;m<sec.getSize();m++) {
      HMdcCalParRawMod& mod=sec[m];
      for(Int_t l=0;l<mod.getSize();l++) {
        HMdcCalParMbo& mbo=mod[l];
        for(Int_t c=0;c<mbo.getSize();c++) mbo[c].clear();
      }
    }
  }
  status=kFALSE;
  resetInputVersions();
}

