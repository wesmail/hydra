//*-- AUTHOR Ilse Koenig
//*-- Created : 10/05/2005

//_HADES_CLASS_DESCRIPTION
//////////////////////////////////////////////////////////////////////
//
//  HHodoLookup
//
//  Parameter container for mapping Crate/Slot/Channel to Module/Fiber
//  needed by the unpacker for the Pion Hodoscopes
//  It was used for hhodounpacker, which is not used anymore.
//  useful only for old pion test beam data with v1190 data
//
//////////////////////////////////////////////////////////////////////

using namespace std;
#include "hhodolookup.h"
#include "hpario.h"
#include "hdetpario.h"
#include <iostream>
#include <iomanip>

ClassImp(HHodoLookupChan)
ClassImp(HHodoLookupSlot)
ClassImp(HHodoLookupCrate)
ClassImp(HHodoLookup)

HHodoLookupSlot::HHodoLookupSlot(Int_t nChan) {
  // constructor creates an array of nChan pointers of type HHodoLookupChan
  array = new TObjArray(nChan);
  for(Int_t i=0;i<nChan;i++) array->AddAt(new HHodoLookupChan(),i);
  nChannels=128;
  maxChannel=nChan-1;
  clear();
}

HHodoLookupSlot::~HHodoLookupSlot(void) {
  // destructor
  array->Delete();
  delete array;
}

void HHodoLookupSlot::clear(void) {
  // clears the array
  for(Int_t i=0;i<=maxChannel;i++) (*this)[i].clear();
}

void HHodoLookupSlot::fill(Int_t channel, Int_t module, Int_t fiber) {
  // Fills the HLookupChan objects with index channel
  // The array is automatically expanded to 128 channels.
  if (channel>maxChannel) {
    array->Expand(nChannels);
    for(Int_t i=maxChannel+1;i<nChannels;i++) array->AddAt(new HHodoLookupChan(),i);
    maxChannel=nChannels-1;
  }
  (*this)[channel].fill(module,fiber);
}

HHodoLookupCrate::HHodoLookupCrate(Int_t numSlots) {
  // constructor creates an array of pointers of type HHodoLookupSlot
  nSlots=21;
  maxSlot=numSlots-1;
  array = new TObjArray(numSlots);
  for(Int_t i=0;i<numSlots;i++) array->AddAt(new HHodoLookupSlot(),i);
}

HHodoLookupCrate::~HHodoLookupCrate(void) {
  // destructor
  array->Delete();
  delete array;
}

void HHodoLookupCrate::fill(Int_t slot, Int_t channel, Int_t module, Int_t fiber) {
  // Expands the array, if necessary, creates the objects of type HHodoLookupSlot,
  // and then calls the fill function of the objects with index slot.
  if (slot>maxSlot) {
    array->Expand(slot+1);
    for(Int_t i=maxSlot+1;i<=slot;i++) array->AddAt(new HHodoLookupSlot(),i);
    maxSlot=slot;
  }
  (*this)[slot].fill(channel,module,fiber);
}

HHodoLookup::HHodoLookup(const Char_t* name,const Char_t* title,
                       const Char_t* context,
                       Int_t nCrates, Int_t nSlots)
           : HParSet(name,title,context) {
  // The constructor creates an array of pointers of type HHodoLookupCrate of size nSlots
  // The container name is set to "HodoLookup".
  strcpy(detName,"Hodo");
  maxCrate=nCrates-1;
  array = new TObjArray(nCrates);
  for (Int_t i=0;i<nCrates;i++) array->AddAt(new HHodoLookupCrate(nSlots),i);
}

HHodoLookup::~HHodoLookup(void) {
  // destructor
  array->Delete();
  delete array;
}

Bool_t HHodoLookup::init(HParIo* inp,Int_t* set) {
  // initializes the container from an input
  HDetParIo* input=inp->getDetParIo("HHodoParIo");
  if (input) return (input->init(this,set));
  return kFALSE;
}

Int_t HHodoLookup::write(HParIo* output) {
  // writes the container to an output
  HDetParIo* out=output->getDetParIo("HHodoParIo");
  if (out) return out->write(this);
  return -1;
}

void HHodoLookup::clear(void) {
  // clears the container
  for(Int_t c=0;c<=maxCrate;c++) {
    for(Int_t s=0;s<(*this)[c].getSize();s++) (*this)[c][s].clear();
  }
  status=kFALSE;
  resetInputVersions();
}

void HHodoLookup::printParam(void) {
  // prints the lookup table
  printf("Lookup table for the Forward Hodo unpacker\n");
  printf("crate  slot  channel  module  fiber\n");
  for(Int_t c=0;c<getSize();c++) {
    for(Int_t s=0;s<(*this)[c].getSize();s++) {
      HHodoLookupSlot& slot=(*this)[c][s];
      for(Int_t r=0;r<slot.getSize();r++) {
        HHodoLookupChan& chan=slot[r];
        Int_t fiber=chan.getFiber();
        if (fiber>=0) printf("%4i%4i%4i%4i%5i\n",
            c,s,r,chan.getModule(),fiber);
      }
    }
  }
}

void HHodoLookup::fill(Int_t crate, Int_t slot,
                       Int_t channel, Int_t module, Int_t fiber) {
  // Expands the array, if necessary, creates the objects of type HHodoLookupCrate,
  // and then calls the fill function of the objects with index slot.
  if (crate>maxCrate) {
    array->Expand(crate+1);
    for(Int_t i=maxCrate+1;i<=crate;i++) array->AddAt(new HHodoLookupCrate(),i);
    maxCrate=crate;
  }
  (*this)[crate].fill(slot,channel,module,fiber);
}

void HHodoLookup::readline(const Char_t *buf, Int_t *set) {
  // decodes one line read from ASCII file I/O
  Int_t crate, slot, chan, module, fiber;
  sscanf(buf,"%i%i%i%i%i",&crate,&slot,&chan,&module,&fiber);
  fill(crate,slot,chan,module,fiber);
  if (set[module]) set[module]=999;
}

void HHodoLookup::putAsciiHeader(TString& header) {
  // puts the ascii header to the string used in HHodoParAsciiFileIo
  header=
    "# Lookup table for the Forward Hodo unpacker\n"
    "# Format:\n"
    "# crate  slot  channel  module  fiber\n";
}

Bool_t HHodoLookup::writeline(Char_t *buf, Int_t crate, Int_t slot, Int_t chan) {
  // writes one line to the buffer used by ASCII file I/O
  HHodoLookupChan& c=(*this)[crate][slot][chan];
  Int_t fiber=c.getFiber();
  if (fiber>=0) {
    sprintf(buf,"%4i%4i%4i%4i%5i\n",
	    crate,slot,chan,c.getModule(),fiber);
    return kTRUE;
  }
  return kFALSE;
}
