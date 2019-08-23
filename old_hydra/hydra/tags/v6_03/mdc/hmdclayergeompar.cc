//*-- AUTHOR : Alexander Nekhaev
//*-- Modified : 17/01/2002 by I. Koenig

////////////////////////////////////////////////////////////////
// HMdcLayerGeomPar
//
// Container class for the Mdc specific geometry parameters of
// the layers
// The parameter container is used by several tasks: digitizer,
// hit finder, event display.
// This class replaces the former class HMdcDigitGeomPar.
//
////////////////////////////////////////////////////////////////

#include "hmdclayergeompar.h"
#include "hades.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "hmdcdetector.h"
#include "hpario.h"
#include "hdetpario.h"
#include "hmdcgeomstruct.h"
#include <iostream.h>

ClassImp(HMdcLayerGeomParLay)
ClassImp(HMdcLayerGeomParMod)
ClassImp(HMdcLayerGeomParSec)
ClassImp(HMdcLayerGeomPar)

void HMdcLayerGeomParLay::fill(HMdcLayerGeomParLay& r)
{
  pitch=r.getPitch();
  catDist=r.getCatDist();
  wireOrient=r.getWireOrient();
  centralWireNr=r.getCentralWireNr();
}


HMdcLayerGeomParMod::HMdcLayerGeomParMod() {
  // constructor creates an array of 6 pointers of type HMdcLayerGeomParLay
  array = new TObjArray(6);
  for (Int_t i = 0; i < 6; i++)
      array->AddAt( new HMdcLayerGeomParLay(),i);
}

HMdcLayerGeomParMod::~HMdcLayerGeomParMod() {
  // destructor deletes the pointer array
  array->Delete();
  delete array;
}

Int_t HMdcLayerGeomParMod::getSize() {
  // return the size of the pointer array 
  return array->GetEntries();
}

HMdcLayerGeomParSec::HMdcLayerGeomParSec() {
  // constructor creates an array of 4 pointers of type HMdcLayerGeomParMod
  array = new TObjArray(4);
  for (Int_t i = 0; i < 4; i++)
      array->AddAt( new HMdcLayerGeomParMod,i);
}

HMdcLayerGeomParSec::~HMdcLayerGeomParSec() {
  // destructor deletes the pointer array
  array->Delete();
  delete array;
}


Int_t HMdcLayerGeomParSec::getSize() {
  // return the size of the pointer array 
  return array->GetEntries();
}

HMdcLayerGeomPar::HMdcLayerGeomPar(const char* name,const char* title,
                                   const char* context)
                 : HParSet(name,title,context) {
  // constructor creates an array of 6 pointers of type
  // HMdcLayerGeomParSec
  // The number of wires is NOT initialized via the init()
  // function but copied from the parameter container "MdcGeomStruct"
  // which must be initialized before calling the constructor
  strcpy(detName,"Mdc");
  array = new TObjArray(6);
  for (Int_t i = 0; i < 6; i++)
    array->AddAt(new HMdcLayerGeomParSec,i);
  HMdcGeomStruct* pMdc=
       (HMdcGeomStruct*)gHades->getRuntimeDb()->getContainer("MdcGeomStruct");
  if (pMdc) {
    for(Int_t i=0;i<6;i++) {
      for(Int_t j=0;j<4;j++) {
        for(Int_t k=0;k<6;k++)
          (*this)[i][j][k].setNumWires(((*pMdc)[i][j][k]));
      }
    }
  } else {
    cerr<<"numWires in container MdcLayerGeomPar not initialized"<<endl;
    cerr<<"container MdcGeomStruct not found"<<endl;
  }
  ntuple=0;
}

HMdcLayerGeomPar::~HMdcLayerGeomPar() {
  // destructor deletes the pointer array
  array->Delete();
  delete array;
  if (ntuple) delete ntuple;
}

Int_t HMdcLayerGeomPar::getSize() {
  // return the size of the pointer array
  return array->GetEntries();
}

Bool_t HMdcLayerGeomPar::init(HParIo* inp,Int_t* set) {
  // initializes the container from an input
  HDetParIo* input=inp->getDetParIo("HMdcParIo");
  Bool_t rc=kFALSE;
  if (input) {
    rc=input->init(this,set);
    if (rc) status=kTRUE;
  }
  return rc;
}

Int_t HMdcLayerGeomPar::write(HParIo* output) {
  // writes the container to an output
  HDetParIo* out=output->getDetParIo("HMdcParIo");
  if (out) return out->write(this);
  return -1;
}

void HMdcLayerGeomPar::readline(const char *buf, Int_t *set) {
  // decodes one line read from ascii file I/O
    Int_t sector, module, layer, pos;
    Float_t pit, cd, wor, cwn;
    sscanf(buf,"%i%i%i%f%f%f%f",&sector,&module,&layer,&pit,&cd,&wor,&cwn);
    pos=sector*4+module;
    if (!set[pos]) return;
    HMdcLayerGeomParLay& l=(*this)[sector][module][layer];
    l.fill(pit,cd,wor,cwn);
    set[pos]=999;
    return;
}

void HMdcLayerGeomPar::putAsciiHeader(TString& header) {
  // puts the ascii header to the string used in HMdcParAsciiFileIo
  header=
    "# Geometry parameters for digitization of the MDC\n"
    "# Format:\n"
    "# sector   module   layer\n"
    "#       pitch\n"
    "#       distace of cathode planes\n"
    "#       wire orientation\n"
    "#       number of central wire\n";
}

Bool_t HMdcLayerGeomPar::writeline(char *buf, Int_t sec, Int_t mod, Int_t lay) {
  // writes one line to the buffer used by ascii file I/O
    HMdcLayerGeomParLay &l = (*this)[sec][mod][lay];
    if (l.getNumWires()<=0) return kFALSE;
    sprintf(buf,"%1i %1i %1i %8.4f %8.4f %8.2f %8.4f\n",
            sec, mod, lay,
            l.getPitch(), l.getCatDist(), l.getWireOrient(),
            l.getCentralWireNr());
    return kTRUE;
}

void HMdcLayerGeomPar::clear() {
  // clears the container
  for(Int_t s=0;s<6;s++) {
    HMdcLayerGeomParSec& sec=(*this)[s];
    for(Int_t m=0;m<4;m++) {
      HMdcLayerGeomParMod& mod=sec[m];
      for(Int_t l=0;l<mod.getSize();l++) {
        HMdcLayerGeomParLay& lay=mod[l];
	lay.clear();
      }
    }
  }
  status=kFALSE;
  resetInputVersions();
}

TNtuple* HMdcLayerGeomPar::getNtuple() {
  // fills the parameters into an TNtuple for drawing
  if (ntuple) ntuple->Reset();
  else ntuple=new TNtuple("MdcLayerGeomParNtuple","Ntuple of Mdc layer geometry",
                          "s:m:l:pt:nw:cd:wor:wof");
  Int_t nw;
  Float_t pt, cd, wor, wof;
  for(Int_t s=0;s<6;s++) {
    HMdcLayerGeomParSec& sec=(*this)[s];
    for(Int_t m=0;m<4;m++) {
      HMdcLayerGeomParMod& mod=sec[m];
      for(Int_t l=0;l<mod.getSize();l++) {
        HMdcLayerGeomParLay& lay=mod[l];
        pt=lay.getPitch();
        nw=lay.getNumWires();
        cd=lay.getCatDist();
        wor=lay.getWireOrient();
        wof=lay.getCentralWireNr();
        ntuple->Fill(s,m,l,pt,nw,cd,wor,wof);
      }
    }
  }
  return ntuple;
}

void HMdcLayerGeomPar::printParam() {
  for(Int_t s=0;s<6;s++) {
    HMdcLayerGeomParSec& sec=(*this)[s];
    for(Int_t m=0;m<4;m++) {
      HMdcLayerGeomParMod& mod=sec[m];
      for(Int_t l=0;l<mod.getSize();l++) {
        HMdcLayerGeomParLay& lay=mod[l];
        Int_t nw=lay.getNumWires();
        if (nw)
          printf("%1i %1i %1i %8.4f %5i %8.4f %8.2f %8.4f\n",
                 s, m, l, lay.getPitch(), nw, lay.getCatDist(), lay.getWireOrient(),
                 lay.getCentralWireNr());
      }
    }
  }
  printf("------------------------------------------------------------\n\n");
}
