#pragma implementation
using namespace std;
#include "hades.h"
#include "hruntimedb.h"
#include "hpario.h"
#include "hparrootfileio.h"
#include "hdetpario.h"
#include "hparasciifileio.h"
#include "hparhadasciifileio.h"
#include "hevent.h"
#include "hcategory.h"
#include "hmatrixcategorysplit.h"
#include "hmatrixcategory.h"
#include "hlinearcategory.h"
#include "htaskset.h"
#include "TClass.h"
#include <iostream> 
#include <iomanip>
#include "htriggerdetector.h"
#include "triggerinfodef.h"
#include "muEmulation.h"
#include "muEmulationSim.h"
#include "muEmulationExp.h"
#include "muDilepEmulation.h"
#include "muDilepEmulationSim.h"
#include "muDilepEmulationExp.h"
#include "htriggerparrootfileio.h"
#include "htriggerparasciifileio.h"

HTriggerDetector::HTriggerDetector(void) : HDetector("Trigger","The TRIGGER detector") {

}

HTriggerDetector::~HTriggerDetector(void) {
}


HTask *HTriggerDetector::buildTask(Text_t *name, Option_t *opt) {
  if (name && strcmp(name, "default")==0)
    name = NULL;

  HTaskSet *pDetTasks = new HTaskSet();

  if (!name) {//default list of tasks

//     HMUEmulation *lep_emu = new HMUEmulation("trigger.emu", "trigger.emu");
//     HMUDilepEmulation *dilep_emu = new HMUDilepEmulation("trigger.dilep.emu", "trigger.dilep.emu");
//     HMUEmulationExp *lep_emu_exp = new HMUEmulationExp("trigger.full.emu", "trigger.full.emu");
    //HMUDilepEmulationExp *dilep_emu_exp = new HMUDilepEmulationExp("trigger.dilep.emu.exp", "trigger.dilep.emu.exp");

    //pDetTasks->connect(lep_emu);
    //pDetTasks->connect(dilep_emu,"trigger.emu");
    //pDetTasks->connect(lep_emu_exp,"trigger.dilep.emu");
//     pDetTasks->connect(lep_emu_exp);
//     pDetTasks->connect((HTask*)NULL,"trigger.full.emu");
    
    return pDetTasks;
  }

  return HDetector::buildTask(name, opt);
}


HCategory *HTriggerDetector::buildLinearCat(Text_t *classname) {
  HLinearCategory *category=NULL;

  category = new HLinearCategory(classname);
  return category;
}


HCategory *HTriggerDetector::buildMatrixCat(Text_t *classname,Float_t fillRate) {
  Int_t maxSector=0;
  Int_t i,sizes[3];
  HMatrixCategory *category=NULL;

  for (i = 0; i < 6; i++) if (getModule(i,0) > 0) maxSector++; 
  sizes[0] = maxSector; 
  sizes[1] = sizes[2] = 96;

  //printf("**************** Max sectors = %d\n", maxSector);
  category = new HMatrixCategory(classname,3,sizes,fillRate);
  return category;
}


HCategory* HTriggerDetector::buildSimMatrixCat(Text_t *classname,Float_t fillRate,Int_t size) {
 
 HMatrixCategory *category = NULL;
 Int_t ini[2];
 ini[0] = 6;
 ini[1] = size;
 category = new HMatrixCategory(classname, 2, ini, fillRate);
 return category;
}



HCategory *HTriggerDetector::buildCategory(Cat_t cat) {
    switch (cat) {
    case catMUEMULeptons: return buildLinearCat("HMUEMULeptons");
    case catMUEMUDiLeptons: return buildLinearCat("HMUEMUDiLeptons");
    case catHardRichHit: return buildLinearCat("HRichHitIPU");
    case catShowerCalIPU : return buildLinearCat("HShowerCalIPU");
    case catShowerHitIPU : return buildLinearCat("HShowerHitIPU");
    default: return NULL;
  }
}

void HTriggerDetector::activateParIo(HParIo* io) {
  // activates the input/output class for the parameters
  // needed by the Trigger
  if (strcmp(io->IsA()->GetName(),"HParOraIo")==0) {
    io->setDetParIo("HTriggerParIo");
    return;
  }
  if (strcmp(io->IsA()->GetName(),"HParRootFileIo")==0) {
    HTriggerParRootFileIo* p=new HTriggerParRootFileIo(((HParRootFileIo*)io)->getParRootFile());
    io->setDetParIo(p);
  }
  if (strcmp(io->IsA()->GetName(),"HParAsciiFileIo")==0) {
    HTriggerParAsciiFileIo* p=new HTriggerParAsciiFileIo(((HParAsciiFileIo*)io)->getFile());
    io->setDetParIo(p);
  }
}

Bool_t HTriggerDetector::write(HParIo* output) {
  // writes the Trigger setup to output
  HDetParIo* out=output->getDetParIo("HTriggerParIo");
  if (out) return out->write(this);
  return kFALSE;
}


ClassImp(HTriggerDetector)









