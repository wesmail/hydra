//*-- AUTHOR : Alexander Nekhaev
//*-- Modified : 11/02/2000 by I. Koenig

////////////////////////////////////////////////////////////////
// HMdcDigitPar
//
// Container class for Mdc digitisation geometrical parameters
//
////////////////////////////////////////////////////////////////

#include "hmdcdigitpar.h"
#include "hades.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "hmdcdetector.h"
#include "hpario.h"
#include "hdetpario.h"
#include <iostream.h>

ClassImp(HMdcDigitParLayer)
ClassImp(HMdcDigitParMod)
ClassImp(HMdcDigitParSec)
ClassImp(HMdcDigitPar)

void HMdcDigitParLayer::fill(HMdcDigitParLayer& r) {
  driftVelocity=r.getDriftVelocity();
  mdcEfficiency=r.getMdcEfficiency();
  spacePar1=r.getSpacePar1();
  spacePar2=r.getSpacePar2();
  spacePar3=r.getSpacePar3();
  spacePar4=r.getSpacePar4();
  spacePar5=r.getSpacePar5();
}

HMdcDigitParMod::HMdcDigitParMod() {
  array = new TObjArray(6);
  for (Int_t i = 0; i < 6; i++) {
    (*array)[i] = new HMdcDigitParLayer;
  }
}


HMdcDigitParMod::~HMdcDigitParMod() {
  array->Delete();
  delete array;
}

Int_t HMdcDigitParMod::getSize() {
  return array->GetEntries(); // return the size of the pointer array
}

HMdcDigitParSec::HMdcDigitParSec() {
  // constructor creates an array of pointers of type
  // HMdcDigitParMod
  array = new TObjArray(4);
  for (Int_t i = 0; i < 4; i++)
      (*array)[i] = new HMdcDigitParMod;
}

HMdcDigitParSec::~HMdcDigitParSec() {
  array->Delete();
  delete array;
}

Int_t HMdcDigitParSec::getSize() {
  return array->GetEntries(); // return the size of the pointer array
}

HMdcDigitPar::HMdcDigitPar() : HParSet() {
  // constructor creates an array of pointers of type
  // HMdcDigitParSec
  // The container name is set to "MdcDigitPar"
  fName="MdcDigitPar";
  fTitle="digitisation parameters for Mdc";
  strcpy(detName,"Mdc");
  array = new TObjArray(6);
  for (Int_t i = 0; i < 6; i++)
      (*array)[i] = new HMdcDigitParSec;
  ntuple=0;
}

HMdcDigitPar::~HMdcDigitPar() {
  // destructor
  array->Delete();
  delete array;
  delete ntuple;
}

Int_t HMdcDigitPar::getSize() {
  // return the size of the pointer array
  return array->GetEntries();
}

Bool_t HMdcDigitPar::init(HParIo* inp,Int_t* set) {
  // intitializes the container from an input
  HDetParIo* input=inp->getDetParIo("HMdcParIo");
  if (input) return (input->init(this,set));
  return kFALSE;
}

Int_t HMdcDigitPar::write(HParIo* output) {
  // writes the container to an output
  HDetParIo* out=output->getDetParIo("HMdcParIo");
  if (out) return out->write(this);
  return -1;
}

void HMdcDigitPar::readline(const char *buf, Int_t *set) {
  // decodes one line read from ascii file I/O
    Int_t sec, mod, lay, pos;
    Float_t driftVel, mdcEf, sR1, sR2, sR3, sR4, sR5;
    sscanf(buf,"%i%i%i%f%f%f%f%f%f%f",
           &sec, &mod, &lay, &driftVel, &mdcEf,
           &sR1, &sR2, &sR3, &sR4, &sR5);
    pos=sec*4+mod;
    if (!set[pos]) return;
    HMdcDigitParLayer& l=(*this)[sec][mod][lay];
    l.fill(driftVel, mdcEf, sR1, sR2, sR3, sR4, sR5);
    set[pos]=999;
    return;
}

void HMdcDigitPar::putAsciiHeader(TString& header) {
  // puts the ascii header to the string used in HMdcParAsciiFileIo
  header=
    "# Digitization parameters of the MDC\n"
    "# Format:\n"
    "# sector  module  layer  drift velocity  efficiency  5 fit parameters\n";
}

Bool_t HMdcDigitPar::writeline(char *buf, Int_t sec, Int_t mod, Int_t lay) {
  // writes one line to the buffer used by ascii file I/O
    HMdcDigitParLayer &l=(*this)[sec][mod][lay];
    Float_t vel=l.getDriftVelocity();
    if (vel<=0.0F) return kFALSE;
    sprintf(buf, "%1i %1i %1i %6.2f %8.4f %8.3f %8.3f %8.3f %8.3f %8.3f\n",
            sec, mod, lay, vel,
            l.getMdcEfficiency(), l.getSpacePar1(),
            l.getSpacePar2(),     l.getSpacePar3(),
            l.getSpacePar4(),     l.getSpacePar5());
    return kTRUE;
}

void HMdcDigitPar::clear() {
  // clears the container
  for(Int_t s=0;s<6;s++) {
    HMdcDigitParSec& sec=(*this)[s];
    for(Int_t m=0;m<4;m++) {
      HMdcDigitParMod& mod=sec[m];
      Int_t nl=mod.getSize();
      for(Int_t l=0;l<nl;l++) {
        HMdcDigitParLayer& lay=mod[l];
	lay.clear();
      }
    }
  }
  status=kFALSE;
  resetInputVersions();
}

TNtuple* HMdcDigitPar::getNtuple() {
  // fills the digitisation parameters into an TNtuple for drawing
  if (ntuple) ntuple->Reset();
  else ntuple=new TNtuple("MdcDigitParNtuple","Ntuple of Mdc digitisation parameters","s:m:l:dv:me:sp0:sp1:sp2");
  Float_t dv, me, sp1, sp2, sp3, sp4, sp5;
  for(Int_t s=0;s<6;s++) {
    HMdcDigitParSec& sec=(*this)[s];
    for(Int_t m=0;m<4;m++) {
      HMdcDigitParMod& mod=sec[m];
      Int_t nl=mod.getSize();
      for(Int_t l=0;l<nl;l++) {
        HMdcDigitParLayer& lay=mod[l];
        dv=lay.getDriftVelocity();       
        me=lay.getMdcEfficiency();       
        sp1=lay.getSpacePar1();       
        sp2=lay.getSpacePar2();       
        sp3=lay.getSpacePar3();
        sp4=lay.getSpacePar4();
        sp5=lay.getSpacePar5();
	ntuple->Fill(s,m,l,dv,me,sp1,sp2,sp3,sp4,sp5);
      }
    }
  }
  return ntuple;
}
void HMdcDigitPar::printParam() {
  printf("---------------------HMdcDigitPar--------------------------\n\n");

    for(Int_t s=0;s<6;s++) {
    HMdcDigitParSec& sec=(*this)[s];
    for(Int_t m=0;m<4;m++) {
      HMdcDigitParMod& mod=sec[m];
      for(Int_t l=0;l<mod.getSize();l++) {
        HMdcDigitParLayer& lay=mod[l];
	printf("%1i %1i %1i %3.2f %1.2f %3.3f %3.3f %3.3f %3.3f %3.3f\n",
		 s, m, l, lay.getDriftVelocity(), lay.getMdcEfficiency(),
		 lay.getSpacePar1(),
                 lay.getSpacePar2(),
                 lay.getSpacePar3(),
		 lay.getSpacePar4(),
                 lay.getSpacePar5() );
      }
    }
  }
  printf("------------------------------------------------------------\n\n");
}


