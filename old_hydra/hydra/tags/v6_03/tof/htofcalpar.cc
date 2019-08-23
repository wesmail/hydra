//*-- Author : Ilse Koenig
//*-- Modified: 10/03/2000 by Ilse Koenig
//*-- Modified: 27/11/2000 by R. Holzmann

///////////////////////////////////////////////////////////////////////////////
// HTofCalPar
//
// Calibration parameters for TOF
//
///////////////////////////////////////////////////////////////////////////////

#include "htofcalpar.h"
#include "hades.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "htofdetector.h"
#include "hpario.h"
#include "hdetpario.h"
#include <iostream.h>

ClassImp(HTofCalPar)
ClassImp(HTofCalParSec)
ClassImp(HTofCalParMod)
ClassImp(HTofCalParCell)

void HTofCalParCell::setDefaults(void) {
  // sets the default values
  leftK=0.065;     // in ns/ch
  rightK=0.065;
  vGroup=160.0;    // in mm/ns
  posK=0.0;
  timK=0.0;
  pedestalL=0.0;
  pedestalR=0.0;
  attLen=0.0;
  gainAsym=0.0;
  edepK=1.0;
}

void HTofCalParCell::Streamer(TBuffer &R__b)
{
  if (R__b.IsReading()) {
    Version_t R__v = R__b.ReadVersion();
    TObject::Streamer(R__b);
    R__b >> leftK;
    R__b >> rightK;
    R__b >> vGroup;
    R__b >> posK;
    R__b >> timK;
    R__b >> pedestalL;
    R__b >> pedestalR;
    R__b >> attLen;
    R__b >> gainAsym;
    if(R__v <= 1) edepK = 0.;
    else R__b >> edepK;
  } else {
    R__b.WriteVersion(HTofCalParCell::IsA());
    TObject::Streamer(R__b);
    R__b << leftK;
    R__b << rightK;
    R__b << vGroup;
    R__b << posK;
    R__b << timK;
    R__b << pedestalL;
    R__b << pedestalR;
    R__b << attLen;
    R__b << gainAsym;
    R__b << edepK;
  }
}

HTofCalParMod::HTofCalParMod(Int_t s, Int_t m) {
  // constructor creates an array of pointers of type HTofCalParCell
  HDetector* det=gHades->getSetup()->getDetector("Tof");
  Int_t n=det->getMaxComponents();
  array = new TObjArray(n);
  if (det->getModule(s,m)) {
    array = new TObjArray(n);
    for(Int_t i=0 ; i<n ; i++) (*array)[i] = new HTofCalParCell();
  } else array = new TObjArray(0);
}

HTofCalParMod::~HTofCalParMod(void) {
  // destructor
  array->Delete();
  delete array;
}


HTofCalParSec::HTofCalParSec(Int_t s, Int_t n) {
  // constructor creates an array of pointers of type HTofCalParCell
  array = new TObjArray(n);
  for(Int_t i=0 ; i<n ; i++)
    (*array)[i] = new HTofCalParMod(s,i);
}

HTofCalParSec::~HTofCalParSec(void) {
  // destructor
  array->Delete();
  delete array;
}


HTofCalPar::HTofCalPar(const char* name,const char* title,
                       const char* context)
           : HParSet(name,title,context) {
  // constructor creates an array of pointers of type HTofCalParSec
  // The container name is set to "TofCalPar"
  strcpy(detName,"Tof");
  array = new TObjArray(6);
  for (Int_t i=0; i<6; i++)
      (*array)[i] = new HTofCalParSec(i,22);
}


HTofCalPar::~HTofCalPar(void) {
  // destructor
  array->Delete();
  delete array;
}
 
Bool_t HTofCalPar::init(HParIo* inp,Int_t* set) {
  // intitializes the container from an input
  HDetParIo* input=inp->getDetParIo("HTofParIo");
  if (input) return (input->init(this,set));
  return kFALSE;
}
 
Int_t HTofCalPar::write(HParIo* output) {
  // writes the container to an output
  HDetParIo* out=output->getDetParIo("HTofParIo");
  if (out) return out->write(this);
  return -1;
}

void HTofCalPar::clear() {
  // sets the default values
  for(Int_t i0=0;i0<getSize();i0++) {
    HTofCalParSec& sec=(*this)[i0];
    Int_t n1=sec.getSize();
    for(Int_t i1=0;i1<n1;i1++) {
      HTofCalParMod& mod=sec[i1];
      Int_t n2=mod.getSize();
      for(Int_t i2=0;i2<n2;i2++) mod[i2].setDefaults();
    }
  }
  status=kFALSE;
  resetInputVersions();
}

void HTofCalPar::readline(const char *buf, Int_t *set) {
  // decodes one line read from ascii file I/O
  Int_t s,m,c;
  Float_t lk,rk,vg,pk,tk,pl,pr,al,ga,ek;
  sscanf(buf,"%i%i%i%f%f%f%f%f%f%f%f%f%f",&s,&m,&c,&lk,&rk,&vg,&pk,&tk,&pl,&pr,&al,&ga,&ek);
  HTofCalParCell& cell=(*this)[s][m][c];
  cell.setLeftK(lk);
  cell.setRightK(rk);
  cell.setVGroup(vg);
  cell.setPosK(pk); 
  cell.setTimK(tk);
  cell.setPedestalL(pl);
  cell.setPedestalR(pr);
  cell.setAttLen(al);
  cell.setGainAsym(ga);
  cell.setEdepK(ek);
  set[(s*22+m)]=999;
}

void HTofCalPar::putAsciiHeader(TString& header) {
  // puts the ascii header to the string used in HTofParAsciiFileIo
  header=
    "# Calibration parameters for the TOF detector\n"
    "# Format:\n"
    "# sector  module  cell  leftK  rightK  vGroup  posK  timK\n"
    "#     pedestalL  pedestalR   attLen   gainAsym  edepK\n";
}

Bool_t HTofCalPar::writeline(char *buf,Int_t s, Int_t m, Int_t c) {
  // writes one line to the buffer used by ascii file I/O
  HTofCalParCell& cell=(*this)[s][m][c];
  sprintf(buf,"%i%4i%3i%9.5f%9.5f%8.3f%9.3f%9.3f%9.3f%9.3f%9.3f%9.3f%9.5f\n",
              s,m,c,cell.getLeftK(),cell.getRightK(),cell.getVGroup(),
              cell.getPosK(),cell.getTimK(),cell.getPedestalL(),cell.getPedestalR(),
              cell.getAttLen(),cell.getGainAsym(),cell.getEdepK());
  return kTRUE;
}

void HTofCalPar::printPause(void) {
  // pauses printing
  printf("Press any key to continue");
  getchar();
  printf("\n\n");
}

void HTofCalPar::printParam(void) {
  // prints the parameters
  printf("\n Calibration parameters for the TOF detector\n");
  printf(" Format:\n");
  printf(" sector  module  cell  leftK  rightK  vGroup  posK  timK\n");
  printf("     pedestalL  pedestalR   attLen   gainAsym edepK\n");
  printf("==============================================================\n");
  Text_t buf[155];
  Int_t line=0;
  Int_t n0 = getSize();
  for(Int_t i0=0; i0<n0; i0++) {
    Int_t n1 = (*this)[i0].getSize();
    for(Int_t i1=0; i1<n1; i1++) {
      Int_t n2 = (*this)[i0][i1].getSize();
      for(Int_t i2=0; i2<n2; i2++) {
	line++;
	//	if (line%24 == 0) printPause();
        if (writeline(buf, i0, i1, i2)) printf(buf,strlen(buf));
      }
    }
  }
  printf("==============================================================\n");
}