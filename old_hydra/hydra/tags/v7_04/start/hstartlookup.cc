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
using namespace std;
#include "hstartlookup.h"
#include "hpario.h"
#include "hdetpario.h"
#include <iostream>
#include <iomanip>
#include <ctype.h>
#include <stdlib.h>

ClassImp(HStartLookupChan)
ClassImp(HStartLookupTdc)
ClassImp(HStartLookup)

void HStartLookupChan::Streamer(TBuffer &R__b)
{
   // Stream an object of class HStartLookupChan.
   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      TObject::Streamer(R__b);
      if (R__v==1) {
        R__b >> module;
        R__b >> strip;
      }
      else{
        R__b >> module;
        R__b >> strip;
        R__b >> cType;
      }
      R__b.CheckByteCount(R__s, R__c, HStartLookupChan::IsA());
   } else {
      R__c = R__b.WriteVersion(HStartLookupChan::IsA(), kTRUE);
      TObject::Streamer(R__b);
      R__b << module;
      R__b << strip;
      R__b << cType;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

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
  printf("crate  slot  channel  module  strip  type\n");
  for(Int_t i=0;i<nTdc;i++) {
    HStartLookupTdc& tdc=(*this)[i];
    for(Int_t j=0;j<tdc.getSize();j++) {
      HStartLookupChan& chan=tdc[j];
      if (chan.getModule()>=0) printf("%5i%5i%9i%6i%7i%8c\n",tdc.crate,tdc.slot,
          j, chan.getModule(),chan.getStrip(),chan.getType());
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
void HStartLookup::setAddress(Int_t crate, Int_t slot,Int_t channel,
                Int_t module, Int_t strip,char cType) {
  // Fills the objects of type HStartLookupTdc.
  // Adds a new objects, if necessary (different crate and/or slot).
  for(Int_t i=0;i<nTdc;i++) {
    HStartLookupTdc& tdc=(*this)[i];
    if (tdc.crate==-1) {
      tdc.setAddress(crate,slot);
      tdc[channel].fill(module,strip,cType);
      return;
    }
    if (tdc.crate==crate && tdc.slot==slot) {
      tdc[channel].fill(module,strip,cType);
      return;
    }
  }
  array->Expand(++nTdc);
  HStartLookupTdc* tdc=new HStartLookupTdc((*this)[0].getSize());
  tdc->setAddress(crate,slot);
  ((*tdc)[channel]).fill(module,strip,cType);
  array->AddAt(tdc,nTdc-1);
}

void HStartLookup::readline(const char *buf, Int_t *set) {
  // decodes one line read from ascii file I/O
  Int_t crate, slot, chan, mod, strip;
    char cType='\0';
  sscanf(buf,"%i%i%i%i%i   %c",&crate,&slot,&chan,&mod,&strip,&cType);
    //Fill only modules which are defined in setup
    if(set[mod]){
        if(isalpha(cType)){
        cType = toupper(cType);
            switch(cType){
                case 'R':
                    setAddress(crate,slot,chan,mod,strip,cType);
                    break;
                case 'L':
                    setAddress(crate,slot,chan,mod,strip,cType);
                    break ;
                case 'M':
                    setAddress(crate,slot,chan,mod,strip,cType);
                    break;
                default:
                    Error("readline","Unknown type %c\n",cType);
                    exit(EXIT_FAILURE);
            }
        }
        else{
            setAddress(crate,slot,chan,mod,strip);
        }
        set[mod]=999;
    }
}

void HStartLookup::putAsciiHeader(TString& header) {
  // puts the ascii header to the string used in HStartParAsciiFileIo
  header=
    "# Lookup table for the START unpacker\n"
    "# type can be: r-right, l-left, m-meantimer or empty (for diamond det.) \n"
    "# Format:\n"
    "# crate  slot  channel  module  strip   type\n";
}

Bool_t HStartLookup::writeline(char *buf, Int_t tdc, Int_t chan) {
  // writes one line to the buffer used by ascii file I/O
  HStartLookupTdc& t=(*this)[tdc];
  HStartLookupChan& c=t[chan];
  if (c.getModule()>=0) {
    sprintf(buf,"%4i%4i%4i%4i%4i%4c\n",
               t.crate,t.slot,chan,c.getModule(),c.getStrip(),c.getType());
    return kTRUE;
  }
  return kFALSE;
}
