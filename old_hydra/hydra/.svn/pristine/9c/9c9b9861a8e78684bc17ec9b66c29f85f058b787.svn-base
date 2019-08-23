//*-- AUTHOR : Ilse Koenig
//*-- Modified : 02/11/98

/////////////////////////////////////////////////////////////
//HMdcDetector
//
//  class for detector Mdc
//  stores the actual setup in a linear array of 24 modules
//
/////////////////////////////////////////////////////////////

#include "hmdcdetector.h"
#include "hmdcdef.h"
#include "hades.h"
#include "hruntimedb.h"
#include "hpario.h"
#include "hparfileio.h"
#include "hdetpario.h"
#include "hmdcparfileio.h"
#include "hparascifileio.h"
#include "hmdcparasciio.h"
#include "hmdcrawstruct.h"
#include "hmdcgeomstruct.h"
#include "hcategory.h"
#include "hmatrixcategorysplit.h"
#include "TClass.h"

ClassImp(HMdcDetector)

HMdcDetector::HMdcDetector() {
  // constructor
  fName="Mdc";
  maxModules=4;
  maxLayers=6;
  modules= new TArrayI(24);
}

HMdcDetector::~HMdcDetector() {
  // destructor
  delete modules;
  modules=0;
}

Int_t HMdcDetector::getMaxLayers() {
  // returns the maximum number of layers (==6)
  return maxLayers;
}

Bool_t HMdcDetector::init(Text_t* level="raw") {
  HRuntimeDb* rtdb=gHades->getRuntimeDb();
  if (strcmp(level,"raw")==0) {
    HMdcRawStruct* pr=
        (HMdcRawStruct*)rtdb->getContainer("MdcRawStruct");
    if (!pr) {
      pr=new HMdcRawStruct();
      if (!pr->HParSet::init()) return kFALSE;
      rtdb->addContainer(pr);
    }
  }
  HMdcGeomStruct* pg=
      (HMdcGeomStruct*)rtdb->getContainer("MdcGeomStruct");
  if (!pg) {
    pg=new HMdcGeomStruct();
    if (!pg->HParSet::init()) return kFALSE;
    rtdb->addContainer(pg);
  }
  return kTRUE;
}

void HMdcDetector::activateParIo(HParIo* io) {
  // activates the input/output class for the parameters
  // needed by the Mdc
  if (strcmp(io->IsA()->GetName(),"HOraIo")==0) {
    io->setDetParIo("HMdcParIo");
    return;
  }
  if (strcmp(io->IsA()->GetName(),"HParFileIo")==0) {
    HMdcParFileIo* p=new HMdcParFileIo(((HParFileIo*)io)->getParRootFile());
    io->setDetParIo(p);
  }
  if (strcmp(io->IsA()->GetName(),"HParAsciFileIo")==0) {
    HMdcParAsciIo* p=new HMdcParAsciIo(((HParAsciFileIo*)io)->getFile());
    io->setDetParIo(p);
  }
}

Bool_t HMdcDetector::write(HParIo* output) {
  // writes the Mdc setup to output
  HDetParIo* out=output->getDetParIo("HMdcParIo");
  if (out) return out->write(this);
  return kFALSE;
}

HCategory* HMdcDetector::buildCategorySplit(Text_t* className,Float_t fillRate){
  Int_t sizes[3];
  Int_t* mods=0;
  Int_t i, maxSector=0, maxModule=0, maxLayer=0, mod=0, sec=0;
  Text_t branchPattern[20];
  HMatrixCategorySplit* category=0;
  for (i=0;i<modules->fN;i++) {
    mod=modules->At(i)-1;
    if (mod>-1) {
      sec=mod/maxModules;
      mod=mod%maxModules;
      maxSector=((sec)>maxSector)?(sec):maxSector;
      maxModule=((mod)>maxModule)?(mod):maxModule;
    }
  }
  maxSector++; maxModule++;
  if (strcmp(className,"HMdcCal")==0) {
    strcpy(branchPattern,"S%0%_M%1%_L%2%");
    mods=new Int_t[maxSector*maxModule*6];
    for (i=0;i<maxSector*maxModule*6;i++) mods[i]=-1;
    HMdcGeomStruct* p=
        (HMdcGeomStruct*)gHades->getRuntimeDb()->getContainer("MdcGeomStruct");
    if (!p) return 0;
    for (i=0;i<modules->fN;i++) {
      mod=modules->At(i)-1;
      if (mod>-1) {
        sec=mod/maxModules;
        mod=mod%maxModules;
        HMdcGeomModStru& r=(*p)[sec][mod];            
        for (Int_t k=0;k<6;k++) {
          mods[sec*maxModule*6+mod*6+k]=r[k];
        }
      }
    }maxLayer=6;
  } else {
    if (strcmp(className,"HMdcRaw")==0) {
      strcpy(branchPattern,"S%0%_M%1%_B%2%");
      mods=new Int_t[maxSector*maxModule*16];
      for (i=0;i<maxSector*maxModule*16;i++) mods[i]=-1;
      HMdcRawStruct* p=
          (HMdcRawStruct*)gHades->getRuntimeDb()->getContainer("MdcRawStruct");
      if (!p) return 0;
      for (i=0;i<modules->fN;i++) {
        mod=modules->At(i)-1;
        if (mod>-1) {
          sec=mod/maxModules;
          mod=mod%maxModules;
          HMdcRawModStru& r=(*p)[sec][mod];
          for (Int_t k=0;k<16;k++) {
            mods[sec*maxModule*16+mod*16+k]=r[k].getNTdcs();
          }
        }
      }
      maxLayer=16;  // maximum number of motherboards hardwired
                    // it's not possible in this framework to take the
                    // actual number which might be <16
    }
  }
  sizes[0]=maxSector;
  sizes[1]=maxModule;
  sizes[2]=maxLayer;
  category = new HMatrixCategorySplit(className,3,sizes,branchPattern);
  category->setup(mods,fillRate);
  delete[] mods;
  return category;
}

HCategory* HMdcDetector::buildCategory(Cat_t cat) {
  switch (cat) {
    case catMdcRaw : return buildCategorySplit("HMdcRaw",0.5);
    case catMdcCal : return buildCategorySplit("HMdcCal",0.5);
    default : return NULL;
  }
}





