//*-- AUTHOR : Ilse Koenig
//*-- Modified : 25/08/2009 by I. Koenig

//_HADES_CLASS_DESCRIPTION 
/////////////////////////////////////////////////////////////
//  HMdcLookupRaw
//
//  Container class for lookup parameters to map the
//  geometrical tree of the Mdc (sectors, modules, layers,
//  cells) to the hardware tree of the Mdc (sectors, modules,
//  motherboards, TDC channels).
//  This information is needed for data mapping from cal1
//  level to raw level.
//  This container has no read functions in the I/O classes,
//  but is initialized from the lookup table HMdcLookupGeom,
//  which contains the same information but accesssible with
//  hardware addresses. This garanties the consistency when
//  both containers are needed.
//  A write function is only privided for an Ascii I/O.
//
/////////////////////////////////////////////////////////////

#include "hmdclookupraw.h"
#include "hmdclookupgeom.h"
#include "hades.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "hmdcdetector.h"
#include "hmdcgeomstruct.h"
#include "hpario.h"
#include "hdetpario.h"
#include "hmessagemgr.h"
#include "TClass.h"

ClassImp(HMdcLookupCell)
ClassImp(HMdcLookupLayer)
ClassImp(HMdcLookupRMod)
ClassImp(HMdcLookupRSec)
ClassImp(HMdcLookupRaw)

void HMdcLookupCell::Streamer(TBuffer &R__b) {
  // Stream an object of class HMdcLookupCell.
  UInt_t R__s, R__c;
  if (R__b.IsReading()) {
    Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
    TObject::Streamer(R__b);
    R__b >> nMoth;
    R__b >> nChan;
    if (R__v>1) {
      R__b >> readoutSide;
    } else {
      readoutSide='0';
    }
    R__b.CheckByteCount(R__s, R__c, HMdcLookupCell::IsA());
  } else {
    R__c = R__b.WriteVersion(HMdcLookupCell::IsA(), kTRUE);
    TObject::Streamer(R__b);
    R__b << nMoth;
    R__b << nChan;
    R__b << readoutSide;
    R__b.SetByteCount(R__c, kTRUE);
  }
}

HMdcLookupLayer::HMdcLookupLayer(Int_t n) {
  // constructor creates an array of pointers of type
  // HMdcLookupCell (array of all cells in a layer)
  array = new TObjArray(n);
//printf("results :n:%i \n",n);
  for (Int_t i = 0; i < n; i++) {
    array->AddAt(new HMdcLookupCell(),i);
  }
}

HMdcLookupLayer::~HMdcLookupLayer() {
  // destructor
  array->Delete();
  delete array;
}

HMdcLookupRMod::HMdcLookupRMod(Int_t sec, Int_t mod, Int_t n) {
  // constructor creates an array of pointers of type
  // HMdcLookupLayer
  array = new TObjArray(n);
  HMdcGeomStruct* pMdc=
       (HMdcGeomStruct*)gHades->getRuntimeDb()->getContainer("MdcGeomStruct");
  if (pMdc) {
    HMdcGeomSecStru& pS= pMdc->operator[](sec);
    HMdcGeomModStru& pM=pS.operator[](mod);
    for (Int_t i = 0; i < n; i++) {
      array->AddAt(new HMdcLookupLayer(pM[i]),i);
    }
  }
  else ERROR_msg(HMessageMgr::DET_MDC,"HMdcLookupRaw not created, container MdcGeomStruct not found");
}

HMdcLookupRMod::~HMdcLookupRMod() {
  // destructor
  array->Delete();
  delete array;
}

HMdcLookupRSec::HMdcLookupRSec(Int_t sec,Int_t n) {
  // constructor creates an array of pointers of type
  // HMdcLookupRMod
  sector=sec;
  array = new TObjArray(n);
  for(Int_t i=0;i<n;i++) array->AddAt(new HMdcLookupRMod(sector,i,6),i);
}

HMdcLookupRSec::~HMdcLookupRSec() {
  // destructor
  array->Delete();
  delete array;
}

HMdcLookupRaw::HMdcLookupRaw(const Char_t* name,const Char_t* title,
                             const Char_t* context,Int_t n)
              : HParSet(name,title,context) {
  // constructor creates an array of pointers of type HMdcLookupRSec
  // creates the container "MdcLookupGeom"
  strcpy(detName,"Mdc");
  array = new TObjArray(n);
  for(Int_t i=0;i<n;i++) array->AddAt(new HMdcLookupRSec(i,4),i);
  HMdcLookupGeom* p=0;
  p=(HMdcLookupGeom*)gHades->getRuntimeDb()->getContainer("MdcLookupGeom");
}

HMdcLookupRaw::~HMdcLookupRaw() {
  // destructor
  array->Delete();
  delete array;
}

Bool_t HMdcLookupRaw::init(HParIo*) {
  // intitializes the container from MdcLookupGeom container
  HMdcLookupGeom* p=
     (HMdcLookupGeom*)gHades->getRuntimeDb()->findContainer("MdcLookupGeom");
  if (!p) return kFALSE;
  Int_t v1=p->getInputVersion(1);
  Int_t v2=p->getInputVersion(2);
  if (v1==versions[1] && v2==versions[2]) return kTRUE;
  // needs reinitialization
  clear();
  Int_t lay, cell;
  for(Int_t s=0;s<p->getSize();s++) {
    HMdcLookupGSec& secG=(*p)[s];
    for(Int_t m=0;m<secG.getSize();m++) {
      HMdcLookupGMod& modG=secG[m];
      HMdcLookupRMod& modR=(*this)[s][m];
      for(Int_t l=0;l<modG.getSize();l++) {
        HMdcLookupMoth& moth=modG[l];
        for(Int_t c=0;c<moth.getSize();c++) {
          HMdcLookupChan& chan=moth[c];
          lay=chan.getNLayer();
          cell=chan.getNCell();
          if (lay>=0) {
            modR[lay][cell].setNMoth(l);
            modR[lay][cell].setNChan(cell);
            modR[lay][cell].setReadoutSide(chan.getReadoutSide());
          }
        }
      }
    }
  }
  versions[1]=v1;
  versions[2]=v2;
  changed=kTRUE;                          
  return kTRUE;
}

Int_t HMdcLookupRaw::write(HParIo* output) {
  // writes the container to an output if the output is an ascii file
  if (strcmp(output->IsA()->GetName(),"HParAsciiFileIo")==0) {
    HDetParIo* out=output->getDetParIo("HMdcParIo");
    if (out) return out->write(this);
  }
  changed=kFALSE;
  return 0;
}

void HMdcLookupRaw::putAsciiHeader(TString& header) {
  // puts the ascii header to the string used in HMdcParAsciiFileIo
  header=
    "# Mdc lookup table (geometry->hardware)\n"
    "# Format:\n"
    "# sector  module  layer  cell  motherboard index  TDC channel  readout side\n";
}

Bool_t HMdcLookupRaw::writeline(Char_t *buf, Int_t sec, Int_t mod, Int_t mbo,
                                           Int_t tdc) {
  // writes one line to the buffer used by ascii file I/O
    HMdcLookupCell &cell=(*this)[sec][mod][mbo][tdc];
    Int_t nTdc=cell.getNChan();
    if (nTdc==-1) return kFALSE;
    sprintf(buf,"%1i %1i %1i %3i %2i %2i %c\n",
            sec, mod, mbo, tdc, cell.getNMoth(), nTdc, cell.getReadoutSide());
    return kTRUE;
}

void HMdcLookupRaw::clear() {
  // clears the container
   for(Int_t s=0;s<6;s++) {
    HMdcLookupRSec& sec=(*this)[s];
    for(Int_t m=0;m<4;m++) {
      HMdcLookupRMod& mod=sec[m];
      Int_t nl=mod.getSize();
      for(Int_t l=0;l<nl;l++) {
        HMdcLookupLayer& lay=mod[l];
        Int_t nc=lay.getSize();
        for(Int_t c=0;c<nc;c++) {
          HMdcLookupCell& cell=lay[c];
          cell.clear();
        }
      }
    }
  }
  status=kFALSE;
  resetInputVersions();
}
