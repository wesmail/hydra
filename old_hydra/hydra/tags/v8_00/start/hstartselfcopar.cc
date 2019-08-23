//_HADES_CLASS_DESCRIPTION 
/////////////////////////////////////////////////////////////
// HStartSelfCoPar
//
// Container class for Start selfcoincidence parameters
// Adapted from  hstartcalpar (by Rainer Schicker)
//
/////////////////////////////////////////////////////////////
using namespace std;
#include "hstartselfcopar.h"
#include "hades.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "hstartdetector.h"
#include "hpario.h"
#include "hdetpario.h"
#include <iostream> 
#include <iomanip>

ClassImp(HStartSelfCoPar)
ClassImp(HStartSelfCoParMod)
ClassImp(HStartSelfCoParChan)

HStartSelfCoParMod::HStartSelfCoParMod(Int_t n) {
  // constructor creates an array of pointers of type
  // HStartSelfCoParChan
  array = new TObjArray(n);
  for(Int_t i=0 ; i<n ; i++)
    array->AddAt( new HStartSelfCoParChan(),i);
}

HStartSelfCoParMod::~HStartSelfCoParMod() {
  // destructor
  array->Delete();
  delete array;
}

HStartSelfCoPar::HStartSelfCoPar(const char* name,const char* title,
                                 const char* context)
                : HParSet(name,title,context) {
  // constructor creates an array of pointers of type HStartSelfCoParMod
  strcpy(detName,"Start");
  HDetector* det= gHades->getSetup()->getDetector("Start");
  Int_t nMod=1;
  Int_t nComp=det->getMaxComponents(); 
  array = new TObjArray(nMod);
  for (Int_t i = 0; i < nMod; i++) {
    if (det->getModule(-1,i)>0) array->AddAt( new HStartSelfCoParMod(nComp),i);
    else array->AddAt(new HStartSelfCoParMod(0),i);
  }
}

HStartSelfCoPar::~HStartSelfCoPar() {
  // destructor
  array->Delete();
  delete array;
}

Bool_t HStartSelfCoPar::init(HParIo* inp,Int_t* set) {
  // intitializes the container from an input
  HDetParIo* input=inp->getDetParIo("HStartParIo");
  if (input) return (input->init(this,set));
  return kFALSE;
}

Int_t HStartSelfCoPar::write(HParIo* output) {
  // writes the container to an output
  HDetParIo* out=output->getDetParIo("HStartParIo");
  if (out) return out->write(this);
  return -1;
}

void HStartSelfCoPar::clear() {
  // clears the container
  for(Int_t i=0;i<getSize();i++) {
    HStartSelfCoParMod& mod=(*this)[i];
    for(Int_t j=0;j<mod.getSize();j++)
          mod[j].clear();
  }
  status=kFALSE;
  resetInputVersions();
}

void HStartSelfCoPar::printParam() {
  // prints the selfcoincidence parameters
  printf("Selfcoincidence parameters for the START detector\n");
  printf("module  strip  time  halfwidth\n");
  for(Int_t i=0;i<getSize();i++) {
    HStartSelfCoParMod& mod=(*this)[i];
    for(Int_t j=0;j<mod.getSize();j++) {
      HStartSelfCoParChan& chan=mod[j];
      printf("%4i%4i %10.5f%8.3f\n",i,j,chan.getTime(),chan.getHalfWidth()); 
    }
  }
}

void HStartSelfCoPar::readline(const char *buf, Int_t *set) {
  // decodes one line read from ascii file I/O
  Int_t mod, strip;
  Float_t time, halfwidth;
  sscanf(buf,"%i%i%f%f", &mod, &strip, &time, &halfwidth);
  if (!set[mod]) return;
  (*this)[mod][strip].fill(time,halfwidth);
  set[mod]=999;
}

void HStartSelfCoPar::putAsciiHeader(TString& header) {
  // puts the ascii header to the string used in HStartParAsciiFileIo
  header=
    "# Selfcoincidence parameters for the START detector\n"
    "# Format:\n"
    "# module   strip   time  halfwidth\n";
}

Bool_t HStartSelfCoPar::writeline(char *buf, Int_t mod, Int_t strip) {
  // writes one line to the buffer used by ascii file I/O
  HStartSelfCoParChan& chan=(*this)[mod][strip];
  sprintf(buf,"%4i%4i %10.5f%8.3f\n", 
              mod, strip, chan.getTime(),chan.getHalfWidth());
  return kTRUE;
}





