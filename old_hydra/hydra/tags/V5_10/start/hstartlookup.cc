//*-- AUTHOR Ilse Koenig
//*-- modified : 06/03/2000
/////////////////////////////////////////////////////////////
// HStartLookup
//
// Container class for mapping Tdc/TdcChannel to Module/Strip
//   neeeded by the START unpacker
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
  // constructor creates an array of pointers of type
  // HStartLookupChan
  array = new TObjArray(n);
  for(Int_t i=0 ; i<n ; i++)
    (*array)[i] = new HStartLookupChan();
}

HStartLookupTdc::~HStartLookupTdc() {
  // destructor
  array->Delete();
  delete array;
}

HStartLookup::HStartLookup(Int_t nTdc, Int_t nChan) : HParSet() {
  // constructor creates an array of pointers of type HStartLookupTdc
  // The container name is set to "StartLookup"
  fName="StartLookup";
  fTitle="Lookup table for Start unpacker";
  strcpy(detName,"Start");
  array = new TObjArray(nTdc);
  for (Int_t i = 0; i < nTdc; i++) {
    (*array)[i] = new HStartLookupTdc(nChan);
  }
}

HStartLookup::~HStartLookup() {
  // destructor
  array->Delete();
  delete array;
}

Bool_t HStartLookup::init(HParIo* inp,Int_t* set) {
  // intitializes the container from an input
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
  for(Int_t i=0;i<getSize();i++) {
    HStartLookupTdc& tdc=(*this)[i];
    for(Int_t j=0;j<tdc.getSize();j++)
          tdc[j].clear();
  }
  status=kFALSE;
  resetInputVersions();
}

void HStartLookup::printParam() {
  // prints the calibration parameters
  printf("Lookup table for the START unpacker\n");
  printf("tdc  channel  module  strip\n");
  for(Int_t i=0;i<getSize();i++) {
    HStartLookupTdc& tdc=(*this)[i];
    for(Int_t j=0;j<tdc.getSize();j++) {
      HStartLookupChan& chan=tdc[j];
      printf("%4i%4i%4i%4i\n",i,j,chan.getModule(),chan.getStrip());
    }
  }
}

void HStartLookup::readline(const char *buf, Int_t *set) {
  // decodes one line read from ascii file I/O
  Int_t tdc, chan, mod, strip;
  sscanf(buf,"%i%i%i%i", &tdc, &chan, &mod, &strip);
  if (tdc>=getSize()) return;
  (*this)[tdc][chan].fill(mod,strip);
  set[mod]=999;
}

void HStartLookup::putAsciiHeader(TString& header) {
  // puts the ascii header to the string used in HStartParAsciiFileIo
  header=
    "# Lookup table for the START unpacker\n"
    "# Format:\n"
    "# tdc  channel  module  strip\n";
}

Bool_t HStartLookup::writeline(char *buf, Int_t tdc, Int_t chan) {
  // writes one line to the buffer used by ascii file I/O
  HStartLookupChan& c=(*this)[tdc][chan];
  sprintf(buf,"%4i%4i%4i%4i\n",
               tdc, chan, c.getModule(),c.getStrip());
  return kTRUE;
}
  






