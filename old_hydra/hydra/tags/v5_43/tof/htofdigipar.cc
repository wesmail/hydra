//*-- Author : D.Vasiliev
//*-- Modified: 12/09/2001 by D.Zovinec
//*-- Modified: 02/07/2001 by D.Zovinec
//*-- Modified: 30/11/2000 by R.Holzmann

#include "hades.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "htofdetector.h"
#include "hpario.h"
#include "hdetpario.h"
#include "htofdigipar.h"
#include <iostream.h>

ClassImp(HTofDigiPar)
ClassImp(HTofDigiParSec)
ClassImp(HTofDigiParMod)
ClassImp(HTofDigiParCell)

void HTofDigiParCell::setDefaults(Int_t m) {
  // Sets the default values.
  if(m==0) halfLen = 1182.5;
  if(m==1) halfLen = 1132.5;
  if(m==2) halfLen = 1067.5;
  if(m==3) halfLen = 985.0;
  if(m==4) halfLen = 970.0;
  if(m==5) halfLen = 897.5;
  if(m==6) halfLen = 812.5;
  if(m==7) halfLen = 737.5;
  if(m<=3) angleRef = 47.0;
  if(m>3 && m<=7) angleRef = 38.0;
  if(m<=3) attenLen = 2000.0;
  if(m>3 && m<=7) attenLen = 1700.0;
  if(m<=3) groupVel = 164.0;
  if(m>3 && m<=7) groupVel = 162.0;
  slopeLeftTDC = 0.065;
  slopeRightTDC = 0.065;
  if(m<=7){
    thrLeftCFD = 200;
    thrRightCFD = 200;
    thrLeftADC = 30;
    thrRightADC = 30;
  } else {
    thrLeftCFD = 0;
    thrRightCFD = 0;
    thrLeftADC = 0;
    thrRightADC = 0;
  }
}

HTofDigiParMod::HTofDigiParMod(Int_t s, Int_t m) {
  // Constructor creates an array of pointers of type HTofDigiParCell.
  HDetector* det=gHades->getSetup()->getDetector("Tof");
  Int_t n=det->getMaxComponents();
  array = new TObjArray(n);
  if (det->getModule(s,m)) {
    array = new TObjArray(n);
    for(Int_t i=0 ; i<n ; i++) (*array)[i] = new HTofDigiParCell();
  } else array = new TObjArray(0);
}

HTofDigiParMod::~HTofDigiParMod(void) {
  // Destructor.
  array->Delete();
  delete array;
}


HTofDigiParSec::HTofDigiParSec(Int_t s, Int_t n) {
  // Constructor creates an array of pointers of type HTofDigiParCell.
  array = new TObjArray(n);
  for(Int_t i=0 ; i<n ; i++)
    (*array)[i] = new HTofDigiParMod(s,i);
}

HTofDigiParSec::~HTofDigiParSec(void) {
  // Destructor.
  array->Delete();
  delete array;
}


HTofDigiPar::HTofDigiPar(void) : HParSet() {
  // Constructor creates an array of pointers of type HTofDigiParSec.
  // The container name is set to "TofDigiPar".
  fName="TofDigiPar";
  fTitle="TOF digitization parameters";
  strcpy(detName,"Tof");
  array = new TObjArray(6);
  for (Int_t i=0; i<6; i++)
      (*array)[i] = new HTofDigiParSec(i,22);
}


HTofDigiPar::~HTofDigiPar(void) {
  // Destructor.
  array->Delete();
  delete array;
}

Bool_t HTofDigiPar::init(HParIo* inp,Int_t* set) {
  // Intitializes the container from an input.
  // If input not available, default initialization.
  HDetParIo* input=inp->getDetParIo("HTofParIo");
  if ((input)&&(input->init(this,set))) return kTRUE;
  else {
    printf("TofDigiPar: Parameter container not found. ");
    printf("Default initialization.\n");
    clear();
    return kTRUE;
  }
}

Int_t HTofDigiPar::write(HParIo* output) {
  // Writes the container to an output.
  HDetParIo* out=output->getDetParIo("HTofParIo");
  if (out) return out->write(this);
  return -1;
}

void HTofDigiPar::clear() {
  // Calls method setDefaults(Int_t m) for all HTofDigiParCell objects
  // in the detector.
  for(Int_t i0=0;i0<getSize();i0++) {
    HTofDigiParSec& sec=(*this)[i0];
    Int_t n1=sec.getSize();
    for(Int_t i1=0;i1<n1;i1++) {
      HTofDigiParMod& mod=sec[i1];
      Int_t n2=mod.getSize();
      for(Int_t i2=0;i2<n2;i2++) mod[i2].setDefaults(i1);
    }
  }
  status=kFALSE;
  resetInputVersions();
}

void HTofDigiPar::readline(const char *buf, Int_t *set) {
  // Decodes one line read from ascii file I/O.
  Int_t s,m,c,lcfd,rcfd,ladc,radc;
  Float_t hl,ar,al,vg,ltdc,rtdc;
  sscanf(buf,"%d%d%d%f%f%f%f%f%f%d%d%d%d",&s,&m,&c,&hl,&ar,&al,&vg,&ltdc,&rtdc,&lcfd,&rcfd,&ladc,&radc);
  HTofDigiParCell& cell=(*this)[s][m][c];
  cell.setHalfLen(hl);
  cell.setAngleRef(ar);
  cell.setAttenLen(al);
  cell.setGroupVel(vg);
  cell.setLeftTDCSlope(ltdc);
  cell.setRightTDCSlope(rtdc);
  cell.setLeftCFDThreshold(lcfd);
  cell.setRightCFDThreshold(rcfd);
  cell.setLeftADCThreshold(ladc);
  cell.setRightADCThreshold(radc);
  set[(s*22+m)]=999;
}

void HTofDigiPar::putAsciiHeader(TString& header) {
  // Puts the ascii header to the string used in HTofParAsciiFileIo.
  header=
    "# Digitization parameters for the TOF detector\n"
    "# Format:\n"
    "# sector module cell HalfLen AngleRef AttenLen GroupVel\n"
    "# lTDCslope rTDCslope lCFDthr rCFDthr lADCthr rADCthr\n";
}

Bool_t HTofDigiPar::writeline(char *buf,Int_t s, Int_t m, Int_t c) {
  // Writes one line to the buffer used by ascii file I/O.
  HTofDigiParCell& cell=(*this)[s][m][c];
  sprintf(buf,"%2i%2i%2i %8.2f %6.2f %7.1f %7.2f %7.4f %7.4f %3i %3i %3i %3i\n",
              s,m,c,cell.getHalfLen(),cell.getAngleRef(),
              cell.getAttenLen(),cell.getGroupVel(),
              cell.getLeftTDCSlope(),cell.getRightTDCSlope(),
              cell.getLeftCFDThreshold(),cell.getRightCFDThreshold(),
              cell.getLeftADCThreshold(),cell.getRightADCThreshold());
  return kTRUE;
}

void HTofDigiPar::printPause(void) {
  // Pauses printing.
  printf("Press any key to continue");
  getchar();
  printf("\n\n");
}

void HTofDigiPar::printParam(void) {
  // Prints the parameters.
  printf("\n Digitization parameters for the TOF detector\n");
  printf(" Format:\n");
  printf(" sector module cell HalfLen AngleRef AttenLen GroupVel\n");
  printf(" lTDCslope rTDCslope lCFDthr rCFDthr lADCthr rADCthr\n");
  printf("==============================================\n");
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
