#pragma implementation

#include "hshowerdetector.h"
#include "hades.h"
#include "hruntimedb.h"
#include "hpario.h"
#include "hparrootfileio.h"
#include "hdetpario.h"
#include "hparasciifileio.h"
#include "hparhadasciifileio.h"
#include "hcategory.h"
#include "hmatrixcategorysplit.h"
#include "hmatrixcategory.h"
#include "hlinearcategory.h"
#include "showerdef.h"
#include "hshowerparset.h"
#include "hshowercalibrater.h"
#include "hshowerhit.h"
#include "hshowerhitfinder.h"
#include "hshowerparhadasciifileio.h"
#include "hshowerparasciifileio.h"
#include "hshowerparrootfileio.h"
#include "hshowergeometry.h"
#include "htaskset.h"
#include "TClass.h"
#include <iostream.h>

HShowerDetector::HShowerDetector(void) : HDetector("Shower","The SHOWER detector") {
  maxModules=3;
  maxRows=32;
  maxColumns=32;
  modules=new TArrayI(getMaxSectors()*maxModules);

  //  createTaskSet("ShowerTaskSet", "Shower list of tasks");
}

HShowerDetector::~HShowerDetector(void) {
}

Bool_t HShowerDetector::init(Text_t*) {
  HRuntimeDb* rtdb=gHades->getRuntimeDb();
  if (!rtdb) return kFALSE;

  HShowerGeometry *pGeom = (HShowerGeometry*)rtdb->
                                               getContainer("ShowerGeometry");
  if (!pGeom) {
       pGeom = new HShowerGeometry;
       rtdb->addContainer(pGeom);
  }
  setGeometry(pGeom);
  if (!pGeom) return kFALSE;

  maxRows = pGeom->getRows();  
  maxColumns = pGeom->getColumns();  
  
  return kTRUE;
}

////////////////////////////////////////////////////
// initialization according to Ilse's scheme

void HShowerDetector::activateParIo(HParIo* io) {
  // activates the input/output class for the parameters
  // needed by the Shower
  if (strcmp(io->IsA()->GetName(),"HOraIo")==0) {
    io->setDetParIo("HShowerParIo");
    return;
  }

  if (strcmp(io->IsA()->GetName(),"HParRootFileIo")==0) {
    HShowerParRootFileIo* p=new HShowerParRootFileIo(((HParRootFileIo*)io)
                                                        ->getParRootFile());
    io->setDetParIo(p);
  }

  if (strcmp(io->IsA()->GetName(),"HParAsciiFileIo")==0) {
    HShowerParAsciiFileIo* p=new HShowerParAsciiFileIo(((HParAsciiFileIo*)io)->getFile());
    io->setDetParIo(p);
  }

  if (strcmp(io->IsA()->GetName(),"HParHadAsciiFileIo")==0) {
    HShowerParHadAsciiFileIo* p=new HShowerParHadAsciiFileIo(((HParHadAsciiFileIo*)io)->getHadAscii());
    io->setDetParIo(p);
  }

}

Bool_t HShowerDetector::write(HParIo* output) {
  // writes the Shower setup to output
  HDetParIo* out=output->getDetParIo("HShowerParIo");
  if (out) return out->write(this);
  return kFALSE;
}
//////////////////////////////////////////////////////////

/*
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
*/

HTask *HShowerDetector::buildTask(Text_t *name,Option_t *opt="") {
 //name bedzie pozniej nazwa klasy tworzacej liste zadan dla operacji
 //opisanych przez ta nazwe


 if (name && strcmp(name, "default")==0)  name = NULL;
  
 HTaskSet *pDetTasks = new HTaskSet();

  if (!name) //default list of tasks
  { 
      HShowerCalibrater *cal = new HShowerCalibrater("ShowerCal","shower.cal");
      HShowerHitFinder *hit=new HShowerHitFinder("ShowerHitFinder","Shower hit finder");

      pDetTasks->connect(cal);
      pDetTasks->connect(hit, "ShowerCal");
      pDetTasks->connect((HTask*)NULL, "ShowerHitFinder");

      return pDetTasks;
  }

return NULL;
}

HCategory *HShowerDetector::buildLinearCat(Text_t *classname) {
  HLinearCategory *category=NULL;

  Int_t size = 0.1*32*32*3*getShowerSectors();     // allocate 10% of maximum
  category=new HLinearCategory(classname, size);
  return category;
}


HCategory *HShowerDetector::buildMatrixCat(Text_t *classname,Float_t fillRate, Int_t nLevels, Int_t* sizes) {
  return new HMatrixCategory(classname,nLevels,sizes,fillRate);
}
 
HCategory *HShowerDetector::buildMatrixCat(Text_t *classname,Float_t fillRate, Int_t nLevels) {
  Int_t maxSector=0,maxModule=0,mod=0;
  Int_t sec=0;
  Int_t i,sizes[4];
  HMatrixCategory *category=NULL;

  for (i=0;i<modules->fN;i++) {
    mod=modules->At(i)-1;
    if (mod>-1) {
      mod = i;
      sec=mod/maxModules;
      mod=mod%maxModules;
      maxSector=((sec)>maxSector)?(sec):maxSector;
      maxModule=((mod)>maxModule)?(mod):maxModule;
    }
  }
  maxSector++; maxModule++;
  sizes[0]=maxSector; sizes[1]=maxModule;
  sizes[2] = sizes[3] = 32;

  category=(HMatrixCategory*)buildMatrixCat(classname,fillRate, nLevels, sizes);
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
      mod = i;
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
      mod = i;
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
      mod = i;
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
      mod = i;
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
  int wireSizes[] = {6,600};
  
  switch (cat) {
//  case catShowerRaw : return buildMatrixCat("HShowerRaw",1, 4);
//  case catShowerCal : return buildMatrixCat("HShowerCal",1, 4);
  case catShowerRaw : return buildLinearCat("HShowerRaw");
  case catShowerCal : return buildLinearCat("HShowerCal");
  case catShowerRawMatr : return buildMatrixCat("HShowerRawMatr",1, 4);

  case catShowerHit : return buildLinearCat("HShowerHit");
  case catShowerPID : return buildLinearCat("HShowerPID");
  case catShowerTrack : return buildLinearCat("HShowerTrack");
  case catShowerGeantRaw : return buildMatrixCat("HShowerGeantHit", 0.2, 2, wireSizes);
 
  case catShowerGeantWire : return buildLinearCat("HShowerGeantWire");
  case catShowerHitHdr : return buildMatrixCat("HShowerHitHeader", 1, 2);
  case catShowerHitTrack : return buildLinearCat("HShowerHitTrack");
  case catShowerPIDTrack : return buildLinearCat("HShowerPIDTrack");

  default : return NULL;
  }
}

ClassImp(HShowerDetector)
