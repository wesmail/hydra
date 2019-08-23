//*-- Author  : D. Gonzalez-Diaz
//*-- Created : 12/06/2006
//*-- Modified: 12/09/2006 by P.Cabanelas
//*-- Modified: 12/08/2007 by D.Gonzalez-Diaz

using namespace std;
#include "hades.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "hrpcdetector.h"
#include "hpario.h"
#include "hdetpario.h"
#include "hrpcgeomcellpar.h"
#include <iostream> 
#include <iomanip>

ClassImp(HRpcGeomCellPar)
ClassImp(HRpcGeomCellParSec)
ClassImp(HRpcGeomCellParCol)
ClassImp(HRpcGeomCellParCell)
  
void HRpcGeomCellParCell::setDefaults(Int_t f) {
  Length  = 0.0;
  X_left  = 0.0;
  Y_left  = 0.0;
  width   = 0.0;
  widthT  = 0.0;
  thetaL  = 0.0;
  thetaR  = 0.0;
}

void HRpcGeomCellParCell::clear() {
  Length  = 0.0;
  X_left  = 0.0;
  Y_left  = 0.0;
  width   = 0.0;
  widthT  = 0.0;
  thetaL  = 0.0;
  thetaR  = 0.0;
}

HRpcGeomCellParCol::HRpcGeomCellParCol(Int_t s, Int_t co) {
  // Constructor creates an array of pointers of type HRpcGeomCellParCell.

  HRpcDetector* det=(HRpcDetector*)(gHades->getSetup()->getDetector("Rpc"));

  Int_t ce = det->getMaxCells();

  //  if (det->getModule(s,m)) {
  array = new TObjArray(ce);
  for(Int_t i=0 ; i<ce ; i++) array->AddAt(new HRpcGeomCellParCell(),i);
  //} else array = new TObjArray(0);
}

HRpcGeomCellParCol::~HRpcGeomCellParCol(void) {
  // Destructor.
  if(array)
    {
      array->Delete();
      delete array;
    }
}

HRpcGeomCellParSec::HRpcGeomCellParSec(Int_t s, Int_t co) {
  // Constructor creates an array of pointers of type HRpcGeomCellParCol

  array = new TObjArray(co);
  for(Int_t i=0 ; i<co ; i++)
    array->AddAt(new HRpcGeomCellParCol(s,i),i);
}

HRpcGeomCellParSec::~HRpcGeomCellParSec(void) {
  // Destructor.
  array->Delete();
  delete array;
}

HRpcGeomCellPar::HRpcGeomCellPar(const char* name,const char* title,
			 const char* context)
  : HParSet(name,title,context) {
  // Constructor creates an array of pointers of type HRpcGeomCellParSec.
  // The container name is set to "RpcGeomCellPar".

  strcpy(detName,"Rpc");

  HRpcDetector* det=(HRpcDetector*)(gHades->getSetup()->getDetector("Rpc"));
  
  Int_t s  = det->getMaxSectors();
  Int_t co = det->getMaxColumns();

  array = new TObjArray(s);

  for (Int_t i=0; i<s; i++)
    array->AddAt(new HRpcGeomCellParSec(i,co),i);
}

HRpcGeomCellPar::~HRpcGeomCellPar(void) {
  // Destructor.
  array->Delete();
  delete array;
}

Bool_t HRpcGeomCellPar::init(HParIo* inp,Int_t* set) {
  // Intitializes the container from an input.
  // If input not available, default initialization.
  HDetParIo* input=inp->getDetParIo("HRpcParIo");
  if (input) return (input->init(this,set));
  return kFALSE;
}

Int_t HRpcGeomCellPar::write(HParIo* output) {
  // Writes the container to an output.
  HDetParIo* out=output->getDetParIo("HRpcParIo");
  if (out) return out->write(this);
  return -1;
}

void HRpcGeomCellPar::clear() {
  // Calls method setDefaults(Int_t m) for all HRpcGeomCellParCell objects
  // in the detector.
  for(Int_t i0=0;i0<getSize();i0++) {
    HRpcGeomCellParSec& sec=(*this)[i0];
    Int_t n1=sec.getSize();
    for(Int_t i1=0;i1<n1;i1++) {
      HRpcGeomCellParCol& col=sec[i1];
      Int_t n2=col.getSize();
      for(Int_t i2=0;i2<n2;i2++) col[i2].setDefaults(i1);
    }
  }
  status=kFALSE;
  resetInputVersions();
}

void HRpcGeomCellPar::readline(const char *buf, Int_t *set) {
  // Decodes one line read from ascii file I/O.
  Int_t s,co,ce;
  Float_t D, Xm, Ym, L, Lt, thetaL, thetaR;
  sscanf(buf,"%d%d%d%f%f%f%f%f%f%f", &s, &co, &ce, &Ym, &Xm, &D, &L, &Lt, &thetaL, &thetaR);

  HRpcGeomCellParCell& cell=(*this)[s][co][ce];
  cell.setY(Ym);
  cell.setX(Xm);
  cell.setLength(D);
  cell.setWidth(L);
  cell.setWidthT(Lt);
  cell.setThetaL(thetaL);
  cell.setThetaR(thetaR);
  set[s]=999;           
}

void HRpcGeomCellPar::putAsciiHeader(TString& header) {
  // Puts the ascii header to the string used in HRpcParAsciiFileIo.
  header=
    "#####################################################################################"
    "# Geometry parameters for the RPC detector cells\n"
    "# Format:\n"
    "# \n"
    "# Sector Column Cell Ybox[mm] Xbox[mm] Dinf[mm] Lcell[mm] Ltube[mm] theta_L theta_R\n"
    "#####################################################################################";
}

Bool_t HRpcGeomCellPar::writeline(char *buf, Int_t s, Int_t co, Int_t ce) {
  // Writes one line to the buffer used by ascii file I/O.
  HRpcGeomCellParCell& cell=(*this)[s][co][ce];
  sprintf(buf,"%2i%2i%2i%4.3f%4.3f%4.3f%4.3f%4.3f%4.3f%4.3f\n",
	   s, co, ce, cell.getY(),
	   cell.getX(), cell.getLength(), cell.getWidth(), cell.getWidthT(), cell.getThetaL(),
	   cell.getThetaR());
  return kTRUE;
}

void HRpcGeomCellPar::printPause(void) {
  // Pauses printing.
  printf("Press any key to continue");
  getchar();
  printf("\n\n");
}

void HRpcGeomCellPar::printParam(void) {
  // Prints the parameters.
  printf("\n Geometry parameters for the RPC detector cells\n");
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
