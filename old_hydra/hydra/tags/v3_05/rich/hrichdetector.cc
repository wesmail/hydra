#pragma implementation
#include "hades.h"
#include "hruntimedb.h"
#include "hpario.h"
#include "hparrootfileio.h"
#include "hdetpario.h"
#include "hparasciifileio.h"
#include "hparhadasciifileio.h"
#include "hrichparset.h"
#include "hrichparhadasciifileio.h"
#include "hrichparasciifileio.h"
#include "hrichparrootfileio.h"
#include "hrichgeometry.h"
#include "hevent.h"
#include "hrichdetector.h"
#include "hcategory.h"
#include "hmatrixcategorysplit.h"
#include "hmatrixcategory.h"
#include "hlinearcategory.h"
#include "richdef.h"
#include "hrichcalibrater.h"
#include "hrichhit.h"
#include "hrichhitfinder.h"
#include "hrichanalysis.h"
#include "htaskset.h"
#include "TClass.h"
#include <iostream.h>

HRichDetector::HRichDetector(void) : HDetector("Rich","The RICH detector") {

  maxModules = 1;
  maxColumns = 92;
  maxRows = 90;
  
  modules = new TArrayI(getMaxSectors());

//  createTaskSet("RichTaskSet", "Rich list of tasks");
}

HRichDetector::~HRichDetector(void) {
}


HTask *HRichDetector::buildTask(Text_t *name, Option_t *opt="") {
  if (name && strcmp(name, "default")==0)
       name = NULL;

   HTaskSet *pDetTasks = new HTaskSet();

  if (!name) //default list of tasks
  {
      HRichCalibrater *cal = new HRichCalibrater("RichCal","rich.cal");
      HRichAnalysis *hit=new HRichAnalysis("RichAnalysis","Rich hit finder");

      pDetTasks->connect(cal);
      pDetTasks->connect(hit, "RichCal");
      pDetTasks->connect((HTask*)NULL, "RichAnalysis");
 
      return pDetTasks;
  }

  return HDetector::buildTask(name, opt);
}


HCategory *HRichDetector::buildLinearCat(Text_t *classname) {
  HLinearCategory *category=NULL;

  category=new HLinearCategory(classname);
  return category;
}


HCategory *HRichDetector::buildMatrixCat(Text_t *classname,Float_t fillRate) {
  Int_t maxSector=0,mod=0;
  Int_t sec=0;
  Int_t i,sizes[3];
  HMatrixCategory *category=NULL;

  for (i=0;i<modules->fN;i++) {
    mod = modules->At(i)-1;
    if (mod > -1) {
      sec = mod;
      maxSector = ((sec)>maxSector)?(sec):maxSector;
    }
  }
  maxSector++; 
  sizes[0]=maxSector; 
  sizes[1] = sizes[2] = 64;

  printf("max sectors = %d\n", maxSector);
  category=new HMatrixCategory(classname,3,sizes,fillRate);
  return category;
}

int HRichDetector::getSectors() {
  int sect;
  getMaxValues(&sect);

  return sect;
}


void HRichDetector::getMaxValues(int* pMaxSector) {
  Int_t mod,sec=0;
  Int_t i;
  Int_t maxSector=0;

  for (i=0;i<modules->fN;i++) {
    mod=modules->At(i)-1;
    if (mod>-1) {
      sec=mod;
      maxSector=((sec)>maxSector)?(sec):maxSector;
    }
  }
  *pMaxSector = ++maxSector; 
}

HCategory *HRichDetector::buildCategorySplit(Text_t *className,Float_t fillRate){
  Text_t branchPattern[]="S%0%";
  Int_t size;
  Int_t *mods=NULL;
  Int_t maxSector=0;
  Int_t mod,sec=0;
  Int_t i;
  HMatrixCategorySplit *category=NULL;

  for (i=0;i<modules->fN;i++) {
    mod=modules->At(i)-1;
    if (mod>-1) {
      sec=mod;
      maxSector=((sec)>maxSector)?(sec):maxSector;
    }
  }
  maxSector++; 
  mods=new Int_t[maxSector];
  for (i=0;i<maxSector;i++) mods[i]=-1;
  for (i=0;i<modules->fN;i++) {
    mod=modules->At(i)-1;
    if (mod>-1) {
      sec=mod;
      mods[sec] = mod*maxRows*maxColumns;
    }
  }
  size=maxSector;
  category = new HMatrixCategorySplit(className,1,&size,branchPattern);
  category->setup(mods,fillRate);

  delete[] mods;
  return category;
}




HCategory *HRichDetector::buildCategory(Cat_t cat) {
 switch (cat) {
  case catRichRaw : return buildMatrixCat("HRichRaw",1);
  case catRichCal : return buildMatrixCat("HRichCal",1);
  case catRichHit : return buildLinearCat("HRichHit");
  case catRichGeantDirHit : return buildLinearCat("HRichGeantDirHit");
  case catRichGeantCherHit : return buildLinearCat("HRichGeantCherHit");
  case catRichGeantWire : return buildLinearCat("HRichGeantWire");
  case catRichHitHdr : return buildLinearCat("HRichHitHeader");
  default : return NULL;
 }
}


////////////////////////////////////////////////////
// initialization according to Ilse's scheme

void HRichDetector::activateParIo(HParIo* io) {
  // activates the input/output class for the parameters
  // needed by the Rich
  if (strcmp(io->IsA()->GetName(),"HOraIo")==0) {
    io->setDetParIo("HRichParIo");
    return;
  }

  if (strcmp(io->IsA()->GetName(),"HParRootFileIo")==0) {
    HRichParRootFileIo* p=new HRichParRootFileIo(((HParRootFileIo*)io)
                                                        ->getParRootFile());
    io->setDetParIo(p);
  }

  if (strcmp(io->IsA()->GetName(),"HParAsciiFileIo")==0) {
    HRichParAsciiFileIo* p=new HRichParAsciiFileIo(((HParAsciiFileIo*)io)->getFile());
    io->setDetParIo(p);
  }

  if (strcmp(io->IsA()->GetName(),"HParHadAsciiFileIo")==0) {
    HRichParHadAsciiFileIo* p=new HRichParHadAsciiFileIo(((HParHadAsciiFileIo*)io)->getHadAscii());
    io->setDetParIo(p);
  }

}

Bool_t HRichDetector::write(HParIo* output) {
  // writes the Rich setup to output
  HDetParIo* out=output->getDetParIo("HRichParIo");
  if (out) return out->write(this);
  return kFALSE;
}
//////////////////////////////////////////////////////////



ClassImp(HRichDetector)
