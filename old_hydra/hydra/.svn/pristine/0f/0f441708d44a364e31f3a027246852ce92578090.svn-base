//*-- Author  : D. Gonzalez-Diaz
//*-- Created : 08/06/2006
//*-- Modified: 12/09/2006 by P.Cabanelas

//_HADES_CLASS_DESCRIPTION
/////////////////////////////////////////////////////////////////////////////
//
//  HRpcDigiPar
//  Container for the RPC detector digitization parameters
//
/////////////////////////////////////////////////////////////////////////////

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
ClassImp(HRpcDigiParCol)
ClassImp(HRpcDigiParCell)
  
void HRpcDigiParCell::setDefaults(Int_t f) {
  vprop   = 0.0;
  S_x     = 0.0;
  S_time  = 0.0;
  V_th    = 0.0;
  eff     = 0.0;
}

void HRpcDigiParCell::clear() {
  vprop   = -999;
  S_x     = -999;
  S_time  = -999;
  V_th    = -999;
  eff     = -999;
}

HRpcDigiParCol::HRpcDigiParCol(Int_t s, Int_t co) {
  // Constructor creates an array of pointers of type HRpcDigiParCell.

  HRpcDetector* det=(HRpcDetector*)(gHades->getSetup()->getDetector("Rpc"));

  Int_t ce = det->getMaxCells();

  //  if (det->getModule(s,m)) {
  array = new TObjArray(ce);
  for(Int_t i=0 ; i<ce ; i++) array->AddAt(new HRpcDigiParCell(),i);
  // } else array = new TObjArray(0);

}

HRpcDigiParCol::~HRpcDigiParCol(void) {
  // Destructor.
  if(array)
    {
      array->Delete();
      delete array;
    }
}

HRpcDigiParSec::HRpcDigiParSec(Int_t s, Int_t co) {
  // Constructor creates an array of pointers of type HRpcDigiParCell.
 
  array = new TObjArray(co);
  for(Int_t i=0 ; i<co ; i++)
    array->AddAt(new HRpcDigiParCol(s,i),i);
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

  HRpcDetector* det=(HRpcDetector*)(gHades->getSetup()->getDetector("Rpc"));
  
  Int_t s  = det->getMaxSectors();
  Int_t co = det->getMaxColumns();

  array = new TObjArray(s);

  for (Int_t i=0; i<s; i++)
    array->AddAt(new HRpcDigiParSec(i,co),i);

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
      HRpcDigiParCol& mod=sec[i1];
      Int_t n2=mod.getSize();
      for(Int_t i2=0;i2<n2;i2++) mod[i2].setDefaults(i1);
    }
  }
  status=kFALSE;
  resetInputVersions();
}

void HRpcDigiPar::readline(const char *buf, Int_t *set) {
  // Decodes one line read from ascii file I/O.
  Int_t s, co, ce;
  Float_t vprop, S_x, S_time, V_th, eff;

  sscanf(buf,"%d%d%d%f%f%f%f%f", &s, &co, &ce, &vprop, &S_x, &S_time, &V_th, &eff);
  HRpcDigiParCell& cell=(*this)[s][co][ce];

  cell.setVprop(vprop);
  cell.setSigmaX(S_x);
  cell.setSigmaT(S_time);
  cell.setVthres(V_th);
  cell.setEff(eff);
  set[s]=999;           //the index runs from 0 to 6
}

void HRpcDigiPar::putAsciiHeader(TString& header) {
  // Puts the ascii header to the string used in HRpcParAsciiFileIo.
  header=
    "####################################################################################"
    "# Digitization parameters for the RPC detector cells"
    "# Format:"
    "#"
    "# Sector Column Cell     v[mm/ns]  s_x[mm]  s_t[ps] Vth[mV] eff"
    "####################################################################################";
}

Bool_t HRpcDigiPar::writeline(char *buf, Int_t s, Int_t co, Int_t ce) {
  // Writes one line to the buffer used by ascii file I/O.
  HRpcDigiParCell& cell=(*this)[s][co][ce];
  sprintf(buf,"%2i%2i%2i%4f%4f%4f%4f%1.3f\n",
	  s, co, ce, cell.getVprop(),
	  cell.getSigmaX(), cell.getSigmaT(), cell.getVthres(), cell.getEff());
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

  printf("\n Digitization parameters for the RPC detector cells\n");
  printf(" Format:\n");
  printf(" Sector Column Cell     v[mm/ns]  s_x[mm]  s_t[ps] Vth[mV] eff\n");
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
