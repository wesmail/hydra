//*-- Author: Alberica Toia
//*-- Modified: Joern Wuestenfeld 07/14/2005
//*-- Version: $Id: htriggerdetector.cc,v 1.14 2009-07-15 11:34:06 halo Exp $

#pragma implementation
#include "TClass.h"
#include "hades.h"
#include "hpario.h"
#include "hparrootfileio.h"
#include "hdetpario.h"
#include "hparasciifileio.h"
#include "hparhadasciifileio.h"
#include "hcategory.h"
#include "hmatrixcategorysplit.h"
#include "hmatrixcategory.h"
#include "hlinearcategory.h"
#include "htaskset.h"
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

HTriggerDetector::HTriggerDetector(void) : HDetector("Trigger","The TRIGGER detector")
{
  // Default constructor
  //
  // Input parameters:
  //  none
  //
  // Output parameters:
  //  none
  //
  // Return code:
  //  none
}

HTriggerDetector::~HTriggerDetector(void)
{
  // Default destructor
  //
  // Input parameters:
  //  none
  //
  // Output parameters:
  //  none
  //
  // Return code:
  //  none
}


HTask *HTriggerDetector::buildTask(const Text_t *name,const Option_t *opt)
{
  // Build the default list of tasks foir this detector.
  // Depreceated, use taskset instead!
  //
  // Input parameters:
  //  Text_t *name
  //    Name of taskset
  //  Option_t *opt
  //    select between options of taskset
  //
  // Output parameters:
  //  none
  //
  // Return code:
  //  Pointer to created taskset
  if (name && strcmp(name, "default")==0)
  {
      HTaskSet *pDetTasks = new HTaskSet();

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


HCategory *HTriggerDetector::buildLinearCat(const Text_t *classname)
{
  // Build a HLinearCategory used for this detector.
  //
  // Input parameters:
  //  Text_t *classname
  //    Name of the category
  //
  // Output parameters:
  //  none
  //
  // Return code:
  // Pointer to created category.
  HLinearCategory *category=NULL;

  category = new HLinearCategory(classname);
  return category;
}


HCategory *HTriggerDetector::buildMatrixCat(const Text_t *classname,Float_t fillRate)
{
  // Build a HMatrixCategory used for this detector.
  //
  // Input parameters:
  //  Text_t *classname
  //    Name of the category
  //  Float_t fillRate
  //    Rate with which to create ne objects in the category
  //
  // Output parameters:
  //  none
  //
  // Return code:
  // Pointer to created category.
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


HCategory* HTriggerDetector::buildSimMatrixCat(const Text_t *classname,Float_t fillRate,Int_t size)
{
  // Build a HMatrixCategory used for simulation data this detector.
  //
  // Input parameters:
  //  Text_t *classname
  //    Name of the category
  //  Float_t fillRate
  //    Rate with which to create ne objects in the category
  //  Int_t size
  //    Size of the category
  //
  // Output parameters:
  //  none
  //
  // Return code:
  // Pointer to created category.

  HMatrixCategory *category = NULL;
  Int_t ini[2];
  ini[0] = 6;
  ini[1] = size;
  category = new HMatrixCategory(classname, 2, ini, fillRate);
  return category;
}



HCategory *HTriggerDetector::buildCategory(Cat_t cat)
{
  // Generic function to build the detector categories.
  //
  // Input Parameters:
  //  cat_t cat
  //    number of category to be build.
  //
  // Output parameters:
  //  none
  //
  // Return code:
  //  Pointer to created category.
    switch (cat) {
    case catMUEMULeptons: return buildLinearCat("HMUEMULeptons");
    case catMUEMUDiLeptons: return buildLinearCat("HMUEMUDiLeptons");
    case catHardRichHit: return buildLinearCat("HRichHitIPU");
    case catShowerCalIPU : return buildLinearCat("HShowerCalIPU");
    case catShowerHitIPU : return buildLinearCat("HShowerHitIPU");
    default: return NULL;
  }
}

void HTriggerDetector::activateParIo(HParIo* io)
{
  // Activates the input/output class for the parameters
  // needed by the Trigger
  //
  // Input parameters:
  //  HParIo *io
  //    Pointer to parameter IO class
  //
  // Output parameters:
  //  none
  //
  // Return code:
  //  none
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

Bool_t HTriggerDetector::write(HParIo* output)
{
  // Writes the Trigger setup to output
  //
  // Input parameters:
  //  HParIo *io
  //    Pointer to parameter IO class to be used for writing.
  //
  // Output parameters:
  //  none
  //
  // Return code:
  //  If parameter IO for this detector exists, return result of writing parameters,
  //  otherwise kFALSE.
  HDetParIo* out=output->getDetParIo("HTriggerParIo");
  if (out) return out->write(this);
  return kFALSE;
}


ClassImp(HTriggerDetector)
