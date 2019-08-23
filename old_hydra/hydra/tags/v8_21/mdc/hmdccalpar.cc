//*-- AUTHOR : Ilse Koenig
//*-- Modified : 17/01/2002 by I. Koenig

//_HADES_CLASS_DESCRIPTION 
/////////////////////////////////////////////////////////////
// HMdcCalPar
//
// Container class for Mdc calibration parameters
// (accessed by addresses: sector module layer cell)
//
/////////////////////////////////////////////////////////////
using namespace std;
#include "hmdccalpar.h"
#include "hades.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "hmdcdetector.h"
#include "hpario.h"
#include "hdetpario.h"
#include "hmdcgeomstruct.h"
#include "hmdclookupraw.h"
#include "hmessagemgr.h"
//#include <iostream> 
//#include <iomanip>

ClassImp(HMdcCalParCell)
ClassImp(HMdcCalParLayer)
ClassImp(HMdcCalParMod)
ClassImp(HMdcCalParSec)
ClassImp(HMdcCalPar)

HMdcCalParLayer::HMdcCalParLayer(Int_t n) {
  // constructor creates a TClonesArray of type
  // HMdcCalParCell
  array = new TObjArray(n);
  for (Int_t i = 0; i < n; ++i)
       array->AddAt(new HMdcCalParCell(),i);
}

HMdcCalParLayer::~HMdcCalParLayer() {
  // destructor
  array->Delete();
  delete array;
}

Int_t HMdcCalParLayer::getSize() {
  // return the size of the pointer array
  return array->GetEntries();
}

HMdcCalParMod::HMdcCalParMod(Int_t sec, Int_t mod, Int_t n) {
  // constructor creates an array of pointers of type
  // HMdcCalParLayer
  array = new TObjArray(n);
  HMdcGeomStruct* pMdc=
       (HMdcGeomStruct*)gHades->getRuntimeDb()->getContainer("MdcGeomStruct");
  if (pMdc) {
    HMdcGeomSecStru& pS=(*pMdc)[sec];
    HMdcGeomModStru& pM=pS[mod];
    for (Int_t i = 0; i < n; i++) {
      array->AddAt(new HMdcCalParLayer(pM[i]),i);
    }
  }
  else cerr<<"HMdcCalParMod not created, container MdcGeomStruct not found"<<endl;
}

HMdcCalParMod::~HMdcCalParMod() {
  // destructor
  array->Delete();
  delete array;
}

Int_t HMdcCalParMod::getSize() {
  // return the size of the pointer array
  return array->GetEntries();
}

HMdcCalParSec::HMdcCalParSec(Int_t sec, Int_t n) {
  // constructor creates an array of pointers of type
  // HMdcCalParMod
  sector=sec;
  array = new TObjArray(n);
  for (Int_t i = 0; i < n; i++)
      array->AddAt(new HMdcCalParMod(sector,i,6),i);
}

HMdcCalParSec::~HMdcCalParSec() {
  // destructor
  array->Delete();
  delete array;
}

Int_t HMdcCalParSec::getSize() {
  // return the size of the pointer array
  return array->GetEntries();
}

HMdcCalPar::HMdcCalPar(const Char_t* name,const Char_t* title,
                       const Char_t* context)
           : HParSet(name,title,context) {
  // constructor creates an array of pointers of type HMdcCalParSec
  strcpy(detName,"Mdc");
  array = new TObjArray(6);
  for (Int_t i = 0; i < 6; i++)
      array->AddAt(new HMdcCalParSec(i, 4),i);
  ntuple=0;
  HRuntimeDb* rtdb=gHades->getRuntimeDb();
  HMdcCalParRaw *calparraw=0;
  calparraw=(HMdcCalParRaw*)rtdb->getContainer("MdcCalParRaw");
  HMdcLookupRaw *lookup=0;
  lookup=(HMdcLookupRaw*)rtdb->getContainer("MdcLookupRaw");
}

HMdcCalPar::~HMdcCalPar() {
  // destructor
  array->Delete();
  delete array;
  delete ntuple;
}

Int_t HMdcCalPar::getSize() {
  // return the size of the pointer array
  return array->GetEntries();
}

Bool_t HMdcCalPar::init(HParIo* inp,Int_t* set) {
  // intitializes the container by coping the data from the parameter container
  // MdcCalParRaw, uses the lookup table MdcLookupRaw for address mapping 
  HRuntimeDb* rtdb=gHades->getRuntimeDb();
  HMdcLookupRaw *lookup=
    (HMdcLookupRaw*)rtdb->getContainer("MdcLookupRaw");
  if (!lookup) return kFALSE;
  HMdcCalParRaw *pRaw=
    (HMdcCalParRaw*)rtdb->getContainer("MdcCalParRaw");
  if (!pRaw) return kFALSE;
  if (!pRaw->hasChanged() && !lookup->hasChanged()) return kTRUE;
  clear();
  for(Int_t s=0; s<6; s++) {
    for(Int_t mo=0; mo<4; mo++) {
      HMdcCalParMod& mod=(*this)[s][mo];
      Int_t nLay=mod.getSize();
      for(Int_t l=0; l<nLay; l++) {
        for(Int_t c=0; c<mod[l].getSize(); c++) {
          HMdcLookupCell &cell=(*lookup)[s][mo][l][c];
          Int_t m=cell.getNMoth();
          Int_t t=cell.getNChan();
          if (m>=0 && t>=0) {
            HMdcCalParCell& wire=mod[l][c];
            HMdcCalParTdc& tdc=(*pRaw)[s][mo][m][t];
            wire.fill(tdc.getSlope(),tdc.getOffset(),
                      tdc.getSlopeErr(),tdc.getOffsetErr(),
                      tdc.getSlopeMethod(),tdc.getOffsetMethod());
          }
        }
      }
    }
  }
  changed=kTRUE;
  Int_t v1=pRaw->getInputVersion(1);
  Int_t v2=pRaw->getInputVersion(2);
  versions[1]=v1;
  versions[2]=v2;
  return kTRUE;
}

Int_t HMdcCalPar::write(HParIo* output) {
    // writes the container to an output
    HDetParIo* out=output->getDetParIo("HMdcParIo");
    if (out) return out->write(this);
    return -1;
}

void HMdcCalPar::clear() {
  // clears the container
  for(Int_t s=0;s<6;s++) {
    HMdcCalParSec& sec=(*this)[s];
    for(Int_t m=0;m<4;m++) {
      HMdcCalParMod& mod=sec[m];
      Int_t nl=mod.getSize();
      for(Int_t l=0;l<nl;l++) {
        HMdcCalParLayer& lay=mod[l];
        Int_t nc=lay.getSize();
        for(Int_t n=0;n<nc;n++) {
          HMdcCalParCell& cell=lay[n];
          cell.clear();
        }
      }
    }
  }
  status=kFALSE;
  resetInputVersions();
}

TNtuple* HMdcCalPar::getNtuple() {
  // fills the calibration parameters into an TNtuple for drawing
  if (ntuple) ntuple->Reset();
  else ntuple=new TNtuple("MdcCalParNtuple","Ntuple of Mdc calibration parameters",
                          "s:m:l:c:slo:off");
  Float_t slo, off;
  for(Int_t s=0;s<6;s++) {
    HMdcCalParSec& sec=(*this)[s];
    for(Int_t m=0;m<4;m++) {
      HMdcCalParMod& mod=sec[m];
      Int_t nl=mod.getSize();
      for(Int_t l=0;l<nl;l++) {
        HMdcCalParLayer& lay=mod[l];
        Int_t nc=lay.getSize();
        for(Int_t n=0;n<nc;n++) {
          HMdcCalParCell& cell=lay[n];
          slo=cell.getSlope();
          off=cell.getOffset();
          ntuple->Fill(s,m,l,n,slo,off);
        }
      }
    }
  }
  return ntuple;
}

Bool_t HMdcCalPar::fillMdcCalParRaw(HMdcCalParRaw& rPar) {
  // copies the calibration parameters to the container MdcCalParRaw
  HRuntimeDb* rtdb=gHades->getRuntimeDb();
  rPar.clear();
  HMdcLookupRaw *lookup=
    (HMdcLookupRaw*)rtdb->getContainer("MdcLookupRaw");
  if (!lookup) {
    ERROR_msg(HMessageMgr::DET_MDC,"lookup table MdcLookupRaw not found");
    return kFALSE;
  }
  for(Int_t s=0; s<6; s++) {
    for(Int_t mo=0; mo<4; mo++) {
      HMdcCalParMod& mod=(*this)[s][mo];
      Int_t nLay=mod.getSize();
      for(Int_t l=0; l<nLay; l++) {
        for(Int_t c=0; c<mod[l].getSize(); c++) {
          HMdcLookupCell &cell=(*lookup)[s][mo][l][c];
          Int_t m=cell.getNMoth();
          Int_t t=cell.getNChan();
          if (m>=0 && t>=0) {
            HMdcCalParCell& wire=mod[l][c];
            HMdcCalParTdc& tdc=rPar[s][mo][m][t];
            tdc.setOffset(wire.getOffset());
            tdc.fill(wire.getSlope(),wire.getOffset(),
                     wire.getSlopeErr(),wire.getOffsetErr(),
                     wire.getSlopeMethod(),wire.getOffsetMethod());
          }
        }
      }
    }
  }
  return kTRUE;
}

void HMdcCalPar::putAsciiHeader(TString& header) {
  // puts the ascii header to the string used in HMdcParAsciiFileIo
  header=
    "# Calibration parameters of the MDC\n"
    "# Format:\n"
    "# sec  mod  mbo  tdc  slope  offset  slopeErr  offsetErr  slopeMethod  offsetMethod\n";
}

Bool_t HMdcCalPar::writeline(Char_t *buf, Int_t sec, Int_t mod, Int_t lay, Int_t cell) {
  // writes one line to the buffer used by ascii file I/O
  HMdcCalParCell &chan=(*this)[sec][mod][lay][cell];
  sprintf(buf,"%1i %1i %1i %3i %10.5f %10.2f  %12.5f %12.2f %1i %1i\n",
          sec, mod, lay, cell,
          chan.getSlope(), chan.getOffset(),chan.getSlopeErr(), chan.getOffsetErr(),
          chan.getSlopeMethod(), chan.getOffsetMethod());
  return kTRUE;
}
