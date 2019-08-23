#include "htofdetector.h"
#include "hcategory.h"
#include "hmatrixcategorysplit.h"
#include "hmatrixcategory.h"
#include "tofdef.h"
#include "htofcalibrater.h"
#include "htofhitf.h"
#include "htaskset.h"
#include "hades.h"
#include "hruntimedb.h"
#include "hpario.h"
#include "hparrootfileio.h"
#include "hdetpario.h"
#include "htofparrootfileio.h"
#include "hparasciifileio.h"
#include "htofparasciifileio.h"
#include "htofgeompar.h"
#include <iostream.h>
#include "TClass.h"

HTofDetector::HTofDetector(void) : HDetector("Tof","The TOF detector") {
  maxModules=22;
  maxComponents=8;
  modules=new TArrayI(getMaxSectors()*maxModules);
}


HTofDetector::~HTofDetector(void) {
}


HTask *HTofDetector::buildTask(Text_t *name,Option_t *opt="") {
  HTask *r=NULL;
  /*  if (strcmp(name,"digitize")==0) {
    r=new TofDigitizer("tof.digitizer","tof.digitizer");
  } else */
  if (strcmp(name,"calibrate")==0) {
    r=new HTofCalibrater("tof.calibrater","calibrater");
    /*if (strcmp(opt,"digi")==0) {
      HTaskSet *ts=new HTaskSet("tof.calibrater","calibrater");
      ts->connect(new HTofDigitizer("tof.digitizer","digitizer"));
      ts->connect(r,"tof.digitizer");
      ts->connect(NULL,r);
      r=ts;
    }*/
  } else if (strcmp(name,"hitF")==0) {
    r=new HTofHitF("tof.hitF","Hit finder");
    if (strcmp(opt,"raw")==0) {
      HTaskSet *ts=new HTaskSet("tof.hitF","Hit finder");
      ts->connect(new HTofCalibrater("tof.calibrater","tof.calibrater"));
      if (!ts->connect(r,"tof.calibrater")) 
	Error("buildTask","connection failed");
      ts->connect(NULL,r);
      r=ts;
    } /*else if (strcmp(opt,"digi")==0) {
      HTaskSet *ts=new HTaskSet("tof.hitF","Hit finder");
      ts->connect(new HTofDigitizer("tof.digitizer","digitizer"));
      ts->connect(new HTofCalibrater("tof.calibrater","calibrater"),
		  "tof.digitizer");
      ts->connect(r,"tof.calibrater");
      ts->connect(NULL,r);
      r=ts;
    }*/
  }
  return r;
} 

HCategory *HTofDetector::buildMatrixCategory(Text_t *classname,Float_t fillRate) {
  Int_t maxSector=0, mod=0;
  Int_t sec=0;
  Int_t i, sizes[3];
  HMatrixCategory *category=NULL;

  for (i=0;i<modules->fN;i++) {
    mod=modules->At(i)-1;
    if (mod > -1) {
      sec=i/maxModules;
      maxSector=((sec)>maxSector)?(sec):maxSector;
    }
  }
  maxSector++;
  sizes[0]=maxSector;
  sizes[1]=maxModules;
  sizes[2]=maxComponents;
  category=new HMatrixCategory(classname,3,sizes,fillRate);
  return category;
}

HCategory *HTofDetector::buildCategorySplit(Text_t *className,Float_t fillRate){
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
      mods[sec*maxModule+mod]=maxComponents;
    }
  }
  sizes[0]=maxSector; sizes[1]=maxModule;
  category = new HMatrixCategorySplit(className,2,sizes,branchPattern);
  category->setup(mods,fillRate);

  delete[] mods;
  return category;
}

HCategory *HTofDetector::buildCategory(Cat_t cat) {
  switch (cat) {
  case catTofRaw : return buildMatrixCategory("HTofRaw",0.5);
  case catTofCal : return buildCategorySplit("HTofCal",0.5);
  case catTofHit : return buildMatrixCategory("HTofHit",0.5);
  case catTofGeantRaw : return buildMatrixCategory("HGeantTof",0.1);
  default : return NULL;
  }
}

void HTofDetector::activateParIo(HParIo* io) {
  // activates the input/output class for the parameters
  // needed by the Tof
  if (strcmp(io->IsA()->GetName(),"HParOraIo")==0) {
    io->setDetParIo("HTofParIo");
    return;
  }
  if (strcmp(io->IsA()->GetName(),"HParRootFileIo")==0) {
    HTofParRootFileIo* p=new HTofParRootFileIo(((HParRootFileIo*)io)->getParRootFile());
    io->setDetParIo(p);
  }
  if (strcmp(io->IsA()->GetName(),"HParAsciiFileIo")==0) {
    HTofParAsciiFileIo* p=new HTofParAsciiFileIo(((HParAsciiFileIo*)io)->getFile());
    io->setDetParIo(p);
  }
}

Bool_t HTofDetector::write(HParIo* output) {
  // writes the Tof setup to output
  HDetParIo* out=output->getDetParIo("HTofParIo");
  if (out) return out->write(this);
  return kFALSE;
}

HDetGeomPar* HTofDetector::createDetGeomPar(HGeomShapes* shapes) {
  // creates the geometry container "TofGeomPar"
  HTofGeomPar* c=
      (HTofGeomPar*)gHades->getRuntimeDb()->getContainer("TofGeomPar");
  if (!c) {
    c=new HTofGeomPar(shapes);
    gHades->getRuntimeDb()->addContainer(c);
  }
  return c;
}

ClassImp(HTofDetector) // TOF detector class
