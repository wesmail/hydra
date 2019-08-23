#pragma implementation
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
#include <iostream.h>
#include "htriggerdetector.h"
#include "triggerinfodef.h"
#include "muEmulation.h"

HTriggerDetector::HTriggerDetector(void) : HDetector("Trigger","The TRIGGER detector") {

}

HTriggerDetector::~HTriggerDetector(void) {
}


HTask *HTriggerDetector::buildTask(Text_t *name, Option_t *opt="") {
  if (name && strcmp(name, "default")==0)
       name = NULL;

  HTaskSet *pDetTasks = new HTaskSet();

  if (!name) {//default list of tasks

    HMUEmulation *lep_emu = new HMUEmulation("trigger.emu", "trigger emulation");
    pDetTasks->connect(lep_emu);
    pDetTasks->connect((HTask*)NULL,"trigger.emu");
    
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
// sizes[1] = sizes[2] = 64;

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

   default: return NULL;
 }
}




ClassImp(HTriggerDetector)




