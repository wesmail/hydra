//*-- AUTHOR : Ilse Koenig
//*-- Modified : 01/02/99

/////////////////////////////////////////////////////////////
//HMdcLookupRaw
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
#include "heventfile.h"
#include "TClass.h"
#include <iostream.h>

ClassImp(HMdcLookupCell)
ClassImp(HMdcLookupLayer)
ClassImp(HMdcLookupRMod)
ClassImp(HMdcLookupRSec)
ClassImp(HMdcLookupRaw)

HMdcLookupLayer::HMdcLookupLayer(Int_t n=0) {
  // constructor creates an array of pointers of type
  // HMdcLookupCell (array of all cells in a layer)
  array = new TObjArray(n);
  for (int i = 0; i < n; i++) {
    (*array)[i] = new HMdcLookupCell();
  }
}

HMdcLookupLayer::~HMdcLookupLayer() {
  // destructor
  array->Delete();
}

HMdcLookupRMod::HMdcLookupRMod(Int_t sec=0, Int_t mod=0, Int_t n=6) {
  // constructor creates an array of pointers of type
  // HMdcLookupLayer
  array = new TObjArray(n);
  HMdcGeomStruct* pMdc=
       (HMdcGeomStruct*)gHades->getRuntimeDb()->getContainer("MdcGeomStruct");
  if (pMdc) {
    HMdcGeomSecStru& pS=(*pMdc)[sec];
    HMdcGeomModStru& pM=pS[mod];
    for (Int_t i = 0; i < n; i++) {
      (*array)[i] = new HMdcLookupLayer(pM[i]);
    }
  }
  else cerr<<"HMdcLookupRaw not created, container MdcGeomStruct not found"<<endl;
}

HMdcLookupRMod::~HMdcLookupRMod() {
  // destructor
  array->Delete();
}

HMdcLookupRSec::HMdcLookupRSec(Int_t sec=0,Int_t n=4) {
  // constructor creates an array of pointers of type
  // HMdcLookupRMod
  sector=sec;
  array = new TObjArray(n);
  for(Int_t i=0;i<n;i++) (*array)[i]= new HMdcLookupRMod(sector,i,6);
}

HMdcLookupRSec::~HMdcLookupRSec() {
  // destructor
  array->Delete();
}

HMdcLookupRaw::HMdcLookupRaw(Int_t n=6) : HParSet() {
  // constructor creates an array of pointers of type
  // HMdcLookupRSec
  // sets the name of the container to "MdcLookupRaw"
  // creates the container "MdcLookupGeom" if not existing and adds the
  // pointer in the runtime database
  fName="MdcLookupRaw";
  fTitle="Mdc lookup table from cal1 to raw";
  strcpy(detName,"Mdc");
  array = new TObjArray(n);
  for(Int_t i=0;i<n;i++) (*array)[i]=new HMdcLookupRSec(i,4);
  HMdcLookupGeom* p=
      (HMdcLookupGeom*)gHades->getRuntimeDb()->getContainer("MdcLookupGeom");
  if (!p) {
    p=new HMdcLookupGeom;
    gHades->getRuntimeDb()->addContainer(p);
  }
}

HMdcLookupRaw::~HMdcLookupRaw() {
  // destructor
  array->Delete();
}

Bool_t HMdcLookupRaw::init(HParIo*,Int_t*) {
  // intitializes the container from MdcLookupGeom container
  HMdcLookupGeom* p=
      (HMdcLookupGeom*)gHades->getRuntimeDb()->getContainer("MdcLookupGeom");
  if (!p) return kFALSE;
  Int_t v1=p->getInputVersion(1);
  Int_t v2=p->getInputVersion(2);
  if (v1==versions[1] && v2==versions[2]) return kTRUE;
  // needs reinitialization
  Int_t lay,cell;
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
          if (lay>=0) {
            cell=chan.getNCell();
            modR[lay][cell].setNMoth(l);
            modR[lay][cell].setNChan(c);
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
  if (strcmp(output->IsA()->GetName(),"HParAsciFileIo")==0) {
    HDetParIo* out=output->getDetParIo("HMdcParIo");
    if (out) return out->write(this);
  }
  changed=kFALSE;
  return 0;
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
