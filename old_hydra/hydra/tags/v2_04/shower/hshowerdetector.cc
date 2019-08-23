#pragma implementation

#include "hshowerdetector.h"
#include "hcategory.h"
#include "hmatrixcategorysplit.h"
#include "hmatrixcategory.h"
#include "hlinearcategory.h"
#include "showerdef.h"
#include "hshowercalibrater.h"
//#include "hshowerdigitizer.h"
#include "hshowerhit.h"
#include "hshowerhitfinder.h"
#include "htaskset.h"
#include <iostream.h>

HShowerDetector::HShowerDetector(void) : HDetector("Shower","The SHOWER detector") {
  maxModules=3;
  maxRows=32;
  maxColumns=32;
  modules=new TArrayI(getMaxSectors()*maxModules);
}

HShowerDetector::~HShowerDetector(void) {
}

HTask *HShowerDetector::buildTask(Text_t *name,Option_t *opt="") {
  HTask *r=NULL;
  if (strcmp(name,"calibrate")==0) {
    r=new HShowerCalibrater("shower.calibrater","calibrater");
  } else if (strcmp(name,"hitF")==0) {
    r=new HShowerHitFinder("shower.hitF","Hit finder");
    if (strcmp(opt,"raw")==0) {
      HTaskSet *ts=new HTaskSet("shower.hitF","Hit finder");
      ts->connect(new HShowerCalibrater("shower.calibrater","shower.calibrater"));
      if (!ts->connect(r,"shower.calibrater")) 
	Error("buildTask","connection failed");
      ts->connect(NULL,r);
      r=ts;
    }
  }
  return r;
} 

HCategory *HShowerDetector::buildLinearCat(Text_t *classname) {
  HLinearCategory *category=NULL;

  category=new HLinearCategory(classname);
  return category;
}


HCategory *HShowerDetector::buildMatrixCat(Text_t *classname,Float_t fillRate) {
  Int_t maxSector=0,maxModule=0,mod=0;
  Int_t sec=0;
  Int_t i,sizes[4];
  HMatrixCategory *category=NULL;

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
  sizes[0]=maxSector; sizes[1]=maxModule;
  sizes[2] = sizes[3] = 32;

  category=new HMatrixCategory(classname,4,sizes,fillRate);
  return category;
}

int HShowerDetector::getShowerSectors() {
  int sect, mod;
  getMaxValues(&sect, &mod);

  return sect;
}

int HShowerDetector::getShowerModules() {
  int sect, mod;
  getMaxValues(&sect, &mod);

  return mod;
}

void HShowerDetector::getMaxValues(int* pMaxSector, int *pMaxModule) {
  Int_t mod,sec=0;
  Int_t i;
  Int_t maxSector=0;
  Int_t maxModule=0;

  for (i=0;i<modules->fN;i++) {
    mod=modules->At(i)-1;
    if (mod>-1) {
      sec=mod/maxModules;
      mod=mod%maxModules;
      maxSector=((sec)>maxSector)?(sec):maxSector;
      maxModule=((mod)>maxModule)?(mod):maxModule;
    }
  }
  *pMaxSector = ++maxSector; 
  *pMaxModule = ++maxModule;
}

HCategory *HShowerDetector::buildCategorySplitS(Text_t *className,Float_t fillRate){
  Text_t branchPattern[]="S%0%";
  Int_t size;
  Int_t *mods=NULL;
  Int_t maxSector=0;
  Int_t maxModule=0;
  Int_t mod,sec=0;
  Int_t i;
  HMatrixCategorySplit *category=NULL;

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
  mods=new Int_t[maxSector];
  for (i=0;i<maxSector;i++) mods[i]=-1;
  for (i=0;i<modules->fN;i++) {
    mod=modules->At(i)-1;
    if (mod>-1) {
      sec=mod/maxModules;
      mod=mod%maxModules;
      mods[sec] = mod*maxRows*maxColumns;
    }
  }
  size=maxSector;
  category = new HMatrixCategorySplit(className,1,&size,branchPattern);
  category->setup(mods,fillRate);

  delete[] mods;
  return category;
}



HCategory *HShowerDetector::buildCategorySplitSM(Text_t *className,Float_t fillRate){
  Text_t branchPattern[]="S%0%_M%1%";
  Int_t sizes[2];
  Int_t *mods=NULL;
  Int_t maxSector=0,maxModule=0,mod=0;
  Int_t sec=0;
  Int_t i;
  HMatrixCategorySplit *category=NULL;

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

  mods=new Int_t[maxSector*maxModule];
  for (i=0;i<maxSector*maxModule;i++) mods[i]=-1;
  for (i=0;i<modules->fN;i++) {
    mod=modules->At(i)-1;
    if (mod>-1) {
      sec=mod/maxModules;
      mod=mod%maxModules;
      mods[sec*maxModule + mod] = maxRows*maxColumns;
    }
  }
  sizes[0]=maxSector; sizes[1]=maxModule;
  category = new HMatrixCategorySplit(className,2,sizes,branchPattern);
  category->setup(mods,fillRate);

  delete[] mods;
  return category;
}

HCategory *HShowerDetector::buildCategorySplitSMR(Text_t *className,Float_t fillRate){
  Text_t branchPattern[]="S%0%_M%1%_R%2%";
  Int_t sizes[3];
  Int_t *mods=NULL;
  Int_t maxSector=0,maxModule=0,mod=0;
  Int_t sec=0;
  Int_t i;
  HMatrixCategorySplit *category=NULL;

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

  mods=new Int_t[maxSector*maxModule*maxRows];
  for (i=0;i<maxSector*maxModule*maxRows;i++) mods[i]=-1;
  for (i=0;i<modules->fN;i++) {
    mod=modules->At(i)-1;
    if (mod>-1) {
      sec=mod/maxModules;
      mod=mod%maxModules;
      for (int j = 0; j < maxRows; j++)
        mods[(sec*maxModule + mod)*maxRows + j] = maxColumns;
    }
  }
  sizes[0]=maxSector; sizes[1]=maxModule; sizes[2]=maxRows;
  category = new HMatrixCategorySplit(className,3,sizes,branchPattern);
  category->setup(mods,fillRate);

  delete[] mods;
  return category;
}

HCategory *HShowerDetector::buildCategory(Cat_t cat) {
  switch (cat) {
  case catShowerRaw : return buildMatrixCat("HShowerRaw",1);
  case catShowerCal : return buildMatrixCat("HShowerCal",1);
  case catShowerHit : return buildLinearCat("HShowerHit");
  case catShowerGeantHit : return buildLinearCat("HShowerGeantHit");
  case catShowerGeantWire : return buildLinearCat("HShowerGeantWire");
  case catShowerHitHdr : return buildMatrixCat("HShowerHitHeader", 1.0);
  default : return NULL;
  }
}

ClassImp(HShowerDetector)
