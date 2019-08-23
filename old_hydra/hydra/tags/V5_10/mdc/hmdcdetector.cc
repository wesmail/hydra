//*-- AUTHOR : Ilse Koenig
//*-- Modified : 21/02/2000 by R. Holzmann
//*-- Modified : 27/05/99   by I. Koenig

/////////////////////////////////////////////////////////////
//HMdcDetector
//
//  class for detector Mdc
//  stores the actual setup in a linear array of 24 modules;
//
/////////////////////////////////////////////////////////////

#include <stdio.h>
#include "hmdcdetector.h"
#include "hmdcdef.h"
#include "hades.h"
#include "hruntimedb.h"
#include "hpario.h"
#include "hparrootfileio.h"
#include "hdetpario.h"
#include "hmdcparrootfileio.h"
#include "hparasciifileio.h"
#include "hmdcparasciifileio.h"
#include "hmdcrawstruct.h"
#include "hmdcgeomstruct.h"
#include "hmdcgeompar.h"
#include "hcategory.h"
#include "hlinearcategory.h"
#include "hmatrixcategory.h"
#include "hmatrixcategorysplit.h"
#include "TClass.h"

ClassImp(HMdcDetector)

HMdcDetector::HMdcDetector() {
  // constructor
  fName="Mdc";
  maxModules=4;
  maxComponents=6;
  modules= new TArrayI(24);
}


HMdcDetector::~HMdcDetector() {
  // destructor
  delete modules;
  modules=0;
}


Bool_t HMdcDetector::init(void) {
  HRuntimeDb* rtdb=gHades->getRuntimeDb();
  HParSet* pg=rtdb->getContainer("MdcGeomStruct");
  if (!pg) {
    pg=new HMdcGeomStruct();
    if (!pg->init()) return kFALSE;
    rtdb->addContainer(pg);
  }
  return kTRUE;
}


void HMdcDetector::activateParIo(HParIo* io) {
  // activates the input/output class for the parameters
  // needed by the Mdc
  if (strcmp(io->IsA()->GetName(),"HParOraIo")==0) {
    io->setDetParIo("HMdcParIo");
    return;
  }
  if (strcmp(io->IsA()->GetName(),"HParRootFileIo")==0) {
    HMdcParRootFileIo* p=new HMdcParRootFileIo(((HParRootFileIo*)io)->getParRootFile());
    io->setDetParIo(p);
  }
  if (strcmp(io->IsA()->GetName(),"HParAsciiFileIo")==0) {
    HMdcParAsciiFileIo* p=new HMdcParAsciiFileIo(((HParAsciiFileIo*)io)->getFile());
    io->setDetParIo(p);
  }
}


Bool_t HMdcDetector::write(HParIo* output) {
  // writes the Mdc setup to output
  HDetParIo* out=output->getDetParIo("HMdcParIo");
  if (out) return out->write(this);
  return kFALSE;
}


HCategory* HMdcDetector::buildLinearCategory(Text_t* className) {
  if (strcmp(className,"HMdcHit")==0 ||
      strcmp(className,"HMdcHitSim")==0 ||
      strcmp(className,"HMdcXY")==0 ) {
    Int_t size = 6*4*10;   // 6 sectors, 4 modules, 10 hits/module
    HLinearCategory* category = new HLinearCategory(className,size);
    return category;
  } else {
    printf("buildLinearCategory for %s not implemented\n",className);
    return 0;
  }
}


HCategory* HMdcDetector::buildMatrixCategory(Text_t* className,Float_t fillRate) {
  TArrayI* ind=0;
  if (strcmp(className,"HMdcRaw")==0) {
    ind=new TArrayI(4);
    HMdcRawStruct* p=
        (HMdcRawStruct*)gHades->getRuntimeDb()->getContainer("MdcRawStruct");
    if (!p) return 0;
    p->getMaxIndices(ind);
  } else {
    if (strcmp(className,"HMdcCal1")==0
          || strcmp(className,"HMdcCal2")==0
          || strcmp(className,"HMdcGeantCell")==0
          || strcmp(className,"HMdcCal1Sim")==0
          || strcmp(className,"HMdcCal2Sim")==0) {
      ind=new TArrayI(4);
      HMdcGeomStruct* p=
          (HMdcGeomStruct*)gHades->getRuntimeDb()->getContainer("MdcGeomStruct");
      if (!p) return 0;
      p->getMaxIndices(ind);
    } else {
      printf("buildMatrixCategory for %s not implemented\n",className);
      return 0;
    }
  }
  Int_t nSizes=ind->GetSize();
  Int_t* sizes=new Int_t[nSizes];
  for (Int_t i=0;i<nSizes;i++) sizes[i]=ind->At(i)+1;
  HMatrixCategory* category = new HMatrixCategory(className,nSizes,sizes,fillRate);
  delete ind;
  delete [] sizes;
  return category;
}

HCategory* HMdcDetector::buildCategorySplit(Text_t* className,Float_t fillRate) {
  TArrayI* ind=0;
  Int_t* mods=0;
  Int_t nSizes=0;
  Int_t* sizes=0;
  Text_t branchPattern[20];
  if (strcmp(className,"HMdcRaw")==0) {
    strcpy(branchPattern,"S%0%_M%1%_B%2%");
    ind=new TArrayI(4);
    HMdcRawStruct* p=
        (HMdcRawStruct*)gHades->getRuntimeDb()->getContainer("MdcRawStruct");
    if (!p) return 0;
    p->getMaxIndices(ind);
    nSizes=3;
    sizes=new Int_t[nSizes];
    sizes[0]=ind->At(0)+1;
    sizes[1]=ind->At(1)+1;
    sizes[2]=ind->At(2)+1;
    mods=new Int_t[(sizes[0]*sizes[1]*sizes[2])];
    for(Int_t i=0;i<sizes[0];i++) {
      for(Int_t j=0;j<sizes[1];j++) {
        HMdcRawModStru& r=(*p)[i][j];
        for (Int_t k=0;k<sizes[2];k++)
            mods[i*sizes[1]*sizes[2]+j*sizes[2]+k]=r[k].getNTdcs();
      }
    }
  } else {
    if (strcmp(className,"HMdcCal1")==0
          || strcmp(className,"HMdcCal2")==0
          || strcmp(className,"HMdcGeantCell")==0
          || strcmp(className,"HMdcCal1Sim")==0
          || strcmp(className,"HMdcCal2Sim")==0) {
      strcpy(branchPattern,"S%0%_M%1%_L%2%");
      ind=new TArrayI(4);
      HMdcGeomStruct* p=
          (HMdcGeomStruct*)gHades->getRuntimeDb()->getContainer("MdcGeomStruct");
      if (!p) return 0;
      p->getMaxIndices(ind);
      nSizes=3;
      sizes=new Int_t[nSizes];
      sizes[0]=ind->At(0)+1;
      sizes[1]=ind->At(1)+1;
      sizes[2]=ind->At(2)+1;
      mods=new Int_t[(sizes[0]*sizes[1]*sizes[2])];
      for(Int_t i=0;i<sizes[0];i++) {
        for(Int_t j=0;j<sizes[1];j++) {
          HMdcGeomModStru& r=(*p)[i][j];
          for (Int_t k=0;k<sizes[2];k++)
              mods[i*sizes[1]*sizes[2]+j*sizes[2]+k]=r[k];
        }
      }
    } else {
      printf("buildMatrixCategorySplit for %s not implemented\n",className);
      return 0;
    }
  }
  HMatrixCategorySplit* category=
      new HMatrixCategorySplit(className,nSizes,sizes,branchPattern);
  category->setup(mods,fillRate);
  delete ind;
  delete [] mods;
  delete [] sizes;
  return category;
}


HCategory* HMdcDetector::buildCategory(Cat_t cat) {
  // builds the default categories (here MatrixCategories)
  switch (cat) {
    case catMdcRaw : return buildMatrixCategory("HMdcRaw",0.5);
    case catMdcCal1 : return buildMatrixCategory("HMdcCal1",0.5);
    case catMdcCal2 : return buildMatrixCategory("HMdcCal2",0.5);
    case catMdcGeantCell : return buildMatrixCategory("HMdcGeantCell",0.5);
//    case catMdcHit : return buildLinearCategory("HMdcHit");
    case catMdcHit : return buildLinearCategory("HMdcXY");   // temporary
    default : return NULL;
  }
}


HDetGeomPar* HMdcDetector::createDetGeomPar(HGeomShapes* shapes) {
  // creates the geometry container "MdcGeomPar"
  HMdcGeomPar* c=
      (HMdcGeomPar*)gHades->getRuntimeDb()->getContainer("MdcGeomPar");
  if (!c) {
    c=new HMdcGeomPar(shapes);
    gHades->getRuntimeDb()->addContainer(c);
  }
  return c;
}





