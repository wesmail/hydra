//*-- AUTHOR Rainer Schicker
//*-- modified: 18/12/2001 by Ilse Koenig
//*-- modified: 06/03/2000 by Ilse Koenig
//*-- created : 18/02/00

/////////////////////////////////////////////////////////////
// HStartCalPar
//
// Container class for Start calibration parameters
//
/////////////////////////////////////////////////////////////
#include "hstartcalpar.h"
#include "hades.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "hstartdetector.h"
#include "hpario.h"
#include "hdetpario.h"
#include <iostream.h>

ClassImp(HStartCalPar)
ClassImp(HStartCalParMod)
ClassImp(HStartCalParChan)

HStartCalParMod::HStartCalParMod(Int_t n) {
  // constructor creates an array of pointers of type
  // HStartCalParChan
  array = new TObjArray(n);
  for(Int_t i=0 ; i<n ; i++)
    array->AddAt( new HStartCalParChan(),i);
}

HStartCalParMod::~HStartCalParMod() {
  // destructor
  array->Delete();
  delete array;
}

HStartCalPar::HStartCalPar(const char* name,const char* title,
                           const char* context)
             : HParSet(name,title,context) {
  // constructor creates an array of pointers of type HStartCalParMod
  strcpy(detName,"Start");
  HDetector* det= gHades->getSetup()->getDetector("Start");
  Int_t nMod=det->getMaxModules();
  Int_t nComp=det->getMaxComponents(); 
  array = new TObjArray(nMod);
  for (Int_t i = 0; i < nMod; i++) {
    if (det->getModule(-1,i)>0) array->AddAt( new HStartCalParMod(nComp),i);
    else array->AddAt( new HStartCalParMod(0),i);
  }
}

HStartCalPar::~HStartCalPar() {
  // destructor
  array->Delete();
  delete array;
}

Bool_t HStartCalPar::init(HParIo* inp,Int_t* set) {
  // intitializes the container from an input
  HDetParIo* input=inp->getDetParIo("HStartParIo");
  if (input) return (input->init(this,set));
  return kFALSE;
}

Int_t HStartCalPar::write(HParIo* output) {
  // writes the container to an output
  HDetParIo* out=output->getDetParIo("HStartParIo");
  if (out) return out->write(this);
  return -1;
}

void HStartCalPar::clear() {
  // clears the container
  for(Int_t i=0;i<getSize();i++) {
    HStartCalParMod& mod=(*this)[i];
    for(Int_t j=0;j<mod.getSize();j++)
          mod[j].clear();
  }
  status=kFALSE;
  resetInputVersions();
}

void HStartCalPar::printParam() {
  // prints the calibration parameters
  printf("Calibration parameters for the START detector\n");
  printf("module  strip  slope  offset\n");
  for(Int_t i=0;i<getSize();i++) {
    HStartCalParMod& mod=(*this)[i];
    for(Int_t j=0;j<mod.getSize();j++) {
      HStartCalParChan& chan=mod[j];
      printf("%4i%4i%10.5f%8.3f\n",i,j,chan.getSlope(),chan.getOffset());
    }
  }
}

void HStartCalPar::readline(const char *buf, Int_t *set) {
  // decodes one line read from ascii file I/O
  Int_t mod, strip;
  Float_t slope, offset;
  sscanf(buf,"%i%i%f%f", &mod, &strip, &slope, &offset);
  if (!set[mod]) return;
  (*this)[mod][strip].fill(slope,offset);
  set[mod]=999;
}

void HStartCalPar::putAsciiHeader(TString& header) {
  // puts the ascii header to the string used in HStartParAsciiFileIo
  header=
    "# Calibration parameters for the START detector\n"
    "# Format:\n"
    "# module   strip   slope   offset\n";
}

Bool_t HStartCalPar::writeline(char *buf, Int_t mod, Int_t strip) {
  // writes one line to the buffer used by ascii file I/O
  HStartCalParChan& chan=(*this)[mod][strip];
  sprintf(buf,"%4i%4i%10.5f%8.3f\n",
              mod, strip, chan.getSlope(),chan.getOffset());
  return kTRUE;
}
  






