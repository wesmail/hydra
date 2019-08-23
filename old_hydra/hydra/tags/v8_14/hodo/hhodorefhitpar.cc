//*-- AUTHOR Bjoern Spruck
//*-- created : 19.04.06

//_HADES_CLASS_DESCRIPTION 
/////////////////////////////////////////////////////////////
// HHodoRefHitPar
//
// Container class for HODO RefHit parameters
//
/////////////////////////////////////////////////////////////
using namespace std;
#include "hhodorefhitpar.h"
#include "hades.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "hhododetector.h"
#include "hpario.h"
#include "hdetpario.h"
#include <iostream>
#include <iomanip>

ClassImp(HHodoRefHitPar)
ClassImp(HHodoRefHitParMod)

HHodoRefHitPar::HHodoRefHitPar(const char* name,const char* title,
                           const char* context)
             : HParSet(name,title,context)
{
  // constructor creates an array of pointers of type HHodoRefHitParMod
  strcpy(detName,"Hodo");
  HHodoDetector* det = (HHodoDetector*)gHades->getSetup()->getDetector("Hodo");
  Int_t nMod=det->getMaxModInSetup();
  array = new TObjArray(nMod);
  for (Int_t i = 0; i < nMod; i++) {
    array->AddAt( new HHodoRefHitParMod,i);
  }
}

HHodoRefHitPar::~HHodoRefHitPar()
{
  // destructor
  array->Delete();
  delete array;
}

Bool_t HHodoRefHitPar::init(HParIo* inp,Int_t* set)
{
  // intitializes the container from an input
  HDetParIo* input=inp->getDetParIo("HHodoParIo");
  if (input) return (input->init(this,set));
  return kFALSE;
}

Int_t HHodoRefHitPar::write(HParIo* output)
{
  // writes the container to an output
  HDetParIo* out=output->getDetParIo("HHodoParIo");
  if (out) return out->write(this);
  return -1;
}

void HHodoRefHitPar::clear()
{
  // clears the container
  for(Int_t i=0;i<getSize();i++) {
    (*this)[i].clear();
  }
  status=kFALSE;
  resetInputVersions();
}

void HHodoRefHitPar::printParam()
{
  // prints the calibration parameters
  printf("Reference Trigger and Hit parameters for the HODO detector\n");
  printf("module  trigref.low trigref.high hit.low hit.high\n");
  for(Int_t i=0;i<getSize();i++) {
    HHodoRefHitParMod& mod=(*this)[i];
    printf("%4i%8.0f%8.0f%9.1f%9.1f\n",i,mod.getTrigRefLow(),mod.getTrigRefHigh(),mod.getHitLow(),mod.getHitHigh());
  }
}

void HHodoRefHitPar::readline(const char *buf, Int_t *set)
{
  // decodes one line read from ascii file I/O
  Int_t mod;
  Float_t f[4]={-1,-1,-1,-1}; // Tlog, Thigh, Hlow, Hhigh;
  sscanf(buf,"%i%f%f%f%f", &mod,&f[0],&f[1],&f[2],&f[3]);
  if (!set[mod]) return;
  (*this)[mod].fill(f);
  set[mod]=999;
}

void HHodoRefHitPar::putAsciiHeader(TString& header)
{
  // puts the ascii header to the string used in HHodoParAsciiFileIo
  header=
    "# Reference Trigger and Hit parameters for the HODO detector\n"
    "# Format:\n"
    "# module   trigref.low   trigref.high   hit.low   hit.high\n";
}

Bool_t HHodoRefHitPar::writeline(char *buf, Int_t m)
{
  // writes one line to the buffer used by ascii file I/O
  HHodoRefHitParMod& mod=(*this)[m];
  sprintf(buf,"%4i%8.0f%8.0f%9.1f%9.1f\n",
              m, mod.getTrigRefLow(),mod.getTrigRefHigh(),mod.getHitLow(),mod.getHitHigh());
  return kTRUE;
}
