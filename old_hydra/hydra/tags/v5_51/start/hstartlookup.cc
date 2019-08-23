//*-- AUTHOR Ilse Koenig
//*-- modified : 18/12/2001 by Ilse Koenig
//*-- modified : 29/11/2000 by Ilse Koenig
/////////////////////////////////////////////////////////////
// HStartLookup
//
// Container class for mapping Crate/Tdc/TdcChannel to Module/Strip
//   needed by the START unpacker
//
/////////////////////////////////////////////////////////////

#include "hstartlookup.h"
#include "hpario.h"
#include "hdetpario.h"
#include <iostream.h>

ClassImp(HStartLookupChan)
ClassImp(HStartLookupTdc)
ClassImp(HStartLookup)

HStartLookupTdc::HStartLookupTdc(Int_t n) {
  // constructor creates an array of pointers of type HStartLookupChan
  nChan=n;
  crate=-1;
  slot=-1;
  array = new TObjArray(n);
  for(Int_t i=0;i<n;i++) array->AddAt(new HStartLookupChan(),i);
}

HStartLookupTdc::~HStartLookupTdc() {
  // destructor
  array->Delete();
  delete array;
}

void HStartLookupTdc::clear() {
  // clears the data members
  crate=-1;
  slot=-1;
  for(Int_t i=0;i<nChan;i++) (*this)[i].clear();
}

HStartLookup::HStartLookup(const char* name, const char* title,
                           const char* context,
                           Int_t numTdc, Int_t numChan)
             : HParSet(name,title,context) {
  // Constructor creates an array of pointers of type HStartLookupTdc.
  strcpy(detName,"Start");
  nTdc=numTdc;
  array = new TObjArray(nTdc);
  for (Int_t i = 0; i < nTdc; i++) array->AddAt(new HStartLookupTdc(numChan),i);
}

HStartLookup::~HStartLookup() {
  // destructor
  array->Delete();
  delete array;
}

Bool_t HStartLookup::init(HParIo* inp,Int_t* set) {
  // initializes the container from an input
  HDetParIo* input=inp->getDetParIo("HStartParIo");
  if (input) return (input->init(this,set));
  return kFALSE;
}

Int_t HStartLookup::write(HParIo* output) {
  // writes the container to an output
  HDetParIo* out=output->getDetParIo("HStartParIo");
  if (out) return out->write(this);
  return -1;
}

void HStartLookup::clear() {
  // clears the container
  for(Int_t i=0;i<nTdc;i++) {
    (*this)[i].clear();
  }
  status=kFALSE;
  resetInputVersions();
}

void HStartLookup::printParam() {
  // prints the calibration parameters
  printf("Lookup table for the START unpacker\n");
  printf("crate  slot  channel  module  strip\n");
  for(Int_t i=0;i<nTdc;i++) {
    HStartLookupTdc& tdc=(*this)[i];
    for(Int_t j=0;j<tdc.getSize();j++) {
      HStartLookupChan& chan=tdc[j];
      if (chan.getModule()>=0) printf("%4i%4i%4i%4i%4i\n",tdc.crate,tdc.slot,
          j, chan.getModule(),chan.getStrip());
    }
  }
}

void HStartLookup::setAddress(Int_t crate, Int_t slot,Int_t channel,
                Int_t module, Int_t strip) {
  // Fills the objects of type HStartLookupTdc.
  // Adds a new objects, if necessary (different crate and/or slot).
  for(Int_t i=0;i<nTdc;i++) {
    HStartLookupTdc& tdc=(*this)[i];
    if (tdc.crate==-1) {
      tdc.setAddress(crate,slot);
      tdc[channel].fill(module,strip);
      return;
    }
    if (tdc.crate==crate && tdc.slot==slot) {
      tdc[channel].fill(module,strip);
      return;
    }
  }
  array->Expand(++nTdc);
  HStartLookupTdc* tdc=new HStartLookupTdc((*this)[0].getSize());
  tdc->setAddress(crate,slot);
  ((*tdc)[channel]).fill(module,strip);
  array->AddAt(tdc,nTdc-1);
}

void HStartLookup::readline(const char *buf, Int_t *set) {
  // decodes one line read from ascii file I/O
  Int_t crate, slot, chan, mod, strip;
  sscanf(buf,"%i%i%i%i%i",&crate,&slot,&chan,&mod,&strip);
  setAddress(crate,slot,chan,mod,strip);
  set[mod]=999;
}

void HStartLookup::putAsciiHeader(TString& header) {
  // puts the ascii header to the string used in HStartParAsciiFileIo
  header=
    "# Lookup table for the START unpacker\n"
    "# Format:\n"
    "# crate  slot  channel  module  strip\n";
}

Bool_t HStartLookup::writeline(char *buf, Int_t tdc, Int_t chan) {
  // writes one line to the buffer used by ascii file I/O
  HStartLookupTdc& t=(*this)[tdc];
  HStartLookupChan& c=t[chan];
  if (c.getModule()>=0) {
    sprintf(buf,"%4i%4i%4i%4i%4i\n",
               t.crate,t.slot,chan,c.getModule(),c.getStrip());
    return kTRUE;
  }
  return kFALSE;
}
