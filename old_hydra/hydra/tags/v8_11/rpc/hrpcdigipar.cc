//*-- Author  : D. Gonzalez-Diaz
//*-- Created : 08/06/2006
//*-- Modified: 12/09/2006 by P.Cabanelas

using namespace std;
#include "hades.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "hrpcdetector.h"
#include "hpario.h"
#include "hdetpario.h"
#include "hrpcdigipar.h"
#include <iostream> 
#include <iomanip>

ClassImp(HRpcDigiPar)
ClassImp(HRpcDigiParSec)
ClassImp(HRpcDigiParMod)
ClassImp(HRpcDigiParCell)
  
void HRpcDigiParCell::setDefaults(Int_t m) {
  // FIXME: Sets the default values. What is this in our case??
  Length  = 0.0;
  X_left  = 0.0;
  Y_left  = 0.0;
  width   = 0.0;
  widthT  = 0.0;
  thetaL  = 0.0;
  thetaR  = 0.0;
}

void HRpcDigiParCell::clear() {
  // FIXME: Really??
  Length  = -1;
  X_left  = -1;
  Y_left  = -1;
  width   = -1;
  widthT  = -1;
  thetaL  = -1;
  thetaR  = -1;
}

HRpcDigiParMod::HRpcDigiParMod(Int_t s, Int_t m) {
  // Constructor creates an array of pointers of type HRpcDigiParCell.

  HDetector* det=gHades->getSetup()->getDetector("Rpc");

  Int_t n=det->getMaxComponents();

  if (det->getModule(s,m)) {
    array = new TObjArray(n);
    for(Int_t i=0 ; i<n ; i++) array->AddAt(new HRpcDigiParCell(),i);
  } else array = new TObjArray(0);

}

HRpcDigiParMod::~HRpcDigiParMod(void) {
  // Destructor.
  if(array)
    {
      array->Delete();
      delete array;
    }
}

HRpcDigiParSec::HRpcDigiParSec(Int_t s, Int_t n) {
  // Constructor creates an array of pointers of type HRpcDigiParCell.
  array = new TObjArray(n);
  for(Int_t i=0 ; i<n ; i++)
    array->AddAt(new HRpcDigiParMod(s,i),i);
}

HRpcDigiParSec::~HRpcDigiParSec(void) {
  // Destructor.
  array->Delete();
  delete array;
}


HRpcDigiPar::HRpcDigiPar(const char* name,const char* title,
			 const char* context)
  : HParSet(name,title,context) {
  // Constructor creates an array of pointers of type HRpcDigiParSec.
  // The container name is set to "RpcDigiPar".
  strcpy(detName,"Rpc");
  array = new TObjArray(6);

  for (Int_t i=0; i<6; i++)
    array->AddAt(new HRpcDigiParSec(i,6),i);

}

HRpcDigiPar::~HRpcDigiPar(void) {
  // Destructor.
  array->Delete();
  delete array;
}

Bool_t HRpcDigiPar::init(HParIo* inp,Int_t* set) {
  // Intitializes the container from an input.
  // If input not available, default initialization.
  HDetParIo* input=inp->getDetParIo("HRpcParIo");
  if (input) return (input->init(this,set));
  return kFALSE;
}

Int_t HRpcDigiPar::write(HParIo* output) {
  // Writes the container to an output.
  HDetParIo* out=output->getDetParIo("HRpcParIo");
  if (out) return out->write(this);
  return -1;
}

void HRpcDigiPar::clear() {
  // Calls method setDefaults(Int_t m) for all HRpcDigiParCell objects
  // in the detector.
  for(Int_t i0=0;i0<getSize();i0++) {
    HRpcDigiParSec& sec=(*this)[i0];
    Int_t n1=sec.getSize();
    for(Int_t i1=0;i1<n1;i1++) {
      HRpcDigiParMod& mod=sec[i1];
      Int_t n2=mod.getSize();
      for(Int_t i2=0;i2<n2;i2++) mod[i2].setDefaults(i1);
    }
  }
  status=kFALSE;
  resetInputVersions();
}

void HRpcDigiPar::readline(const char *buf, Int_t *set) {
  // Decodes one line read from ascii file I/O.
  Int_t s,m,c;
  Float_t D, Xm, Ym, L, Lt, thetaL, thetaR;

  sscanf(buf,"%d%d%d%f%f%f%f%f%f%f", &s, &m, &c, &Xm, &Ym, &D, &L, &Lt, &thetaL, &thetaR);
  HRpcDigiParCell& cell=(*this)[s][m][c];
  cell.setX(Xm);
  cell.setY(Ym);
  cell.setLength(D);
  cell.setWidth(L);
  cell.setWidthT(Lt);
  cell.setThetaL(thetaL);
  cell.setThetaR(thetaR);
  set[(s*6 + m)]=999;           //the index runs from 0 to 24
}

void HRpcDigiPar::putAsciiHeader(TString& header) {
  // Puts the ascii header to the string used in HRpcParAsciiFileIo.
  header=
    "#####################################################################################"
    "# Geometry parameters for the RPC detector\n"
    "# Format:\n"
    "# \n"
    "# Sector Column Cell Ybox[mm] Xbox[mm] Dinf[mm] Lcell[mm] Ltube[mm] theta_L theta_R\n"
    "#####################################################################################";
}

Bool_t HRpcDigiPar::writeline(char *buf, Int_t s, Int_t m, Int_t c) {
  // Writes one line to the buffer used by ascii file I/O.
  HRpcDigiParCell& cell=(*this)[s][m][c];
  sprintf(buf,"%2i%2i%2i%4.3f%4.3f%4.3f%4.3f%4.3f%4.3f%4.3f\n",
	   s, m, c, cell.getX(),
	   cell.getY(), cell.getLength(), cell.getWidth(), cell.getWidthT(), cell.getThetaL(),
	   cell.getThetaR());
  return kTRUE;
}

void HRpcDigiPar::printPause(void) {
  // Pauses printing.
  printf("Press any key to continue");
  getchar();
  printf("\n\n");
}

void HRpcDigiPar::printParam(void) {
  // Prints the parameters.
  printf("\n Geometry parameters for the RPC detector\n");
  printf(" Format:\n");
  printf(" Sector Column Cell Ybox[mm] Xbox[mm] Dinf[mm] Lcell[mm] Ltube[mm] theta_L theta_R\n");
  printf("==============================================\n");
  Text_t buf[155];
  Int_t n0 = getSize();
  for(Int_t i0=0; i0<n0; i0++) {
    Int_t n1 = (*this)[i0].getSize();
    for(Int_t i1=0; i1<n1; i1++) {
      Int_t n2 = (*this)[i0][i1].getSize();
      for(Int_t i2=0; i2<n2; i2++) {
	if (writeline(buf, i0, i1, i2)) printf(buf,strlen(buf));
      }
    }
  }
  printf("==============================================================\n");
}
