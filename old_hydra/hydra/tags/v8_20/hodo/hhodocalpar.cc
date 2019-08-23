//*-- AUTHOR Bjoern Spruck
//*-- created : 24.03.06

//_HADES_CLASS_DESCRIPTION
/////////////////////////////////////////////////////////////
//
// HHodoCalPar
//
// Container class for HODO calibration parameters
//
/////////////////////////////////////////////////////////////
using namespace std;
#include "hhodocalpar.h"
#include "hades.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "hhododetector.h"
#include "hpario.h"
#include "hdetpario.h"
#include <iostream>
#include <iomanip>

ClassImp(HHodoCalPar)
ClassImp(HHodoCalParMod)
ClassImp(HHodoCalParChan)

HHodoCalParMod::HHodoCalParMod(Int_t n)
{
  // constructor creates an array of pointers of type
  // HHodoCalParChan
  array = new TObjArray(n);
  for(Int_t i=0 ; i<n ; i++)
    array->AddAt( new HHodoCalParChan(),i);
}

HHodoCalParMod::~HHodoCalParMod()
{
  // destructor
  array->Delete();
  delete array;
}

HHodoCalPar::HHodoCalPar(const Char_t* name,const Char_t* title,
                           const Char_t* context)
             : HParSet(name,title,context)
{
  // constructor creates an array of pointers of type HHodoCalParMod
  strcpy(detName,"Hodo");
  HHodoDetector* det = (HHodoDetector*)gHades->getSetup()->getDetector("Hodo");
  Int_t nMod=det->getMaxModInSetup();
  Int_t nComp=det->getMaxComponents();
  array = new TObjArray(nMod);
  for (Int_t i = 0; i < nMod; i++) {
    if (det->getModule(-1,i)>0) array->AddAt( new HHodoCalParMod(nComp),i);
    else array->AddAt( new HHodoCalParMod(0),i);
  }
}

HHodoCalPar::~HHodoCalPar()
{
  // destructor
  array->Delete();
  delete array;
}

Bool_t HHodoCalPar::init(HParIo* inp,Int_t* set)
{
  // intitializes the container from an input
  HDetParIo* input=inp->getDetParIo("HHodoParIo");
  if (input) return (input->init(this,set));
  return kFALSE;
}

Int_t HHodoCalPar::write(HParIo* output)
{
  // writes the container to an output
  HDetParIo* out=output->getDetParIo("HHodoParIo");
  if (out) return out->write(this);
  return -1;
}

void HHodoCalPar::clear()
{
  // clears the container
  for(Int_t i=0;i<getSize();i++) {
    HHodoCalParMod& mod=(*this)[i];
    for(Int_t j=0;j<mod.getSize();j++)
          mod[j].clear();
  }
  status=kFALSE;
  resetInputVersions();
}

void HHodoCalPar::printParam()
{
  // prints the calibration parameters
  printf("Calibration parameters for the HODO detector\n");
  printf("module  strip  TDC.slope  TDC.offset ADC.slope  ADC.offset\n");
  for(Int_t i=0;i<getSize();i++) {
    HHodoCalParMod& mod=(*this)[i];
    for(Int_t j=0;j<mod.getSize();j++) {
      HHodoCalParChan& chan=mod[j];
      printf("%4i%4i%10.5f%12.3f%10.5f%12.3f\n",i,j,chan.getTDCSlope(),chan.getTDCOffset(),chan.getADCSlope(),chan.getADCOffset());
    }
  }
}

void HHodoCalPar::readline(const Char_t *buf, Int_t *set)
{
  // decodes one line read from ascii file I/O
  Int_t mod, strip;
  Float_t f[4]={-1,-1,-1,-1}; // Tslope, Toffset, Aslope, Aoffset;
  sscanf(buf,"%i%i%f%f%f%f", &mod, &strip, &f[0],&f[1],&f[2],&f[3]);
  if (!set[mod]) return;
  (*this)[mod][strip].fill(f);
  set[mod]=999;
}

void HHodoCalPar::putAsciiHeader(TString& header)
{
  // puts the ascii header to the string used in HHodoParAsciiFileIo
  header=
    "# Calibration parameters for the HODO detector\n"
    "# Format:\n"
    "# module   strip   TDC.slope  TDC.offset ADC.slope  ADC.offset\n";
}

Bool_t HHodoCalPar::writeline(Char_t *buf, Int_t mod, Int_t strip)
{
  // writes one line to the buffer used by ascii file I/O
  HHodoCalParChan& chan=(*this)[mod][strip];
  sprintf(buf,"%4i%4i%10.5f%12.3f%10.5f%12.3f\n",
              mod, strip, chan.getTDCSlope(),chan.getTDCOffset(),chan.getADCSlope(),chan.getADCOffset());
  return kTRUE;
}







