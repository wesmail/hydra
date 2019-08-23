//*-- Author: Alberica Toia
//*-- Modified: Joern Wuestenfeld 05/10/2005
//*-- Version: $Id: muDilepEmulationSim.cc,v 1.5 2009-07-15 11:34:06 halo Exp $

#include "muDilepEmulationSim.h"
#include "hdebug.h"
#include "hades.h"
#include "hcategory.h"
#include "hiterator.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "hevent.h"
#include "hmatchurich.h"
#include "hmatchutof.h"
#include "hmatchushower.h"
#include "hmuleptons.h"
#include "hmatrixcatiter.h"
#include "triggerinfodef.h"
#include "htriggerdetector.h"
#include "hlocation.h"
#include "hgeomvector.h"
#include "htriggerparmudilep.h"
#include "hmessagemgr.h"

//
// HMuDilepEmulationSim
//
// Matching unit emulation code.
// This class is to be used for simulation data.
//

//----------------------------------------------------------------------------
HMUDilepEmulationSim::HMUDilepEmulationSim(void)
{
  // Default constructor
  //
  // Input parameters:
  //   none
  //
  // Output parameters:
  //   none
  //
  // Return code:
  //   none
  reset();
}
//============================================================================

//----------------------------------------------------------------------------
HMUDilepEmulationSim::HMUDilepEmulationSim(const Text_t *name,const Text_t *title)
              : HMUDilepEmulation(name, title)
{
  // Default constructor
  //
  // Input parameters:
  //   Text_t *name
  //      name of the object
  //   Text_t *title
  //      title of the object
  //
  // Output parameters:
  //   none
  //
  // Return code:
  //   none
  reset();
}
//============================================================================

//----------------------------------------------------------------------------
HMUDilepEmulationSim::~HMUDilepEmulationSim()
{
  // Destructor, deletes all internaly created objects.
  //
  // Input parameters:
  //   none
  //
  // Output parameters:
  //   none
  //
  // Return code:
  //    none
  if(pEmuDiLeptons)
    {
      delete pEmuDiLeptons;
      pEmuDiLeptons=NULL;
    }
  nEmuDiLeptons=0;

  ptlHMULeptons=NULL;
  ptlHMUEMUDiLeptons=NULL;

  if (iterHMULeptons) delete iterHMULeptons;
  iterHMULeptons=NULL;
  if (iterHMULeptons2) delete iterHMULeptons2;
  iterHMULeptons2=NULL;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HMUDilepEmulationSim::init()
{
  // Initialize the task. Create all parameter containers and categories.
  //
  // Input parameters:
  //   none
  //
  // Output parameters:
  //   none
  //
  // Return code:
  //   none
  HSpectrometer *spec = gHades->getSetup();
  HRuntimeDb *rtdb = gHades->getRuntimeDb();
  HTriggerDetector * pTrigDet = (HTriggerDetector*)gHades->getSetup()->getDetector("Trigger");

  // initialize parameters
  if (pTrigDet)
    {
      if(!initParContainer(spec,rtdb))
        {
          return kFALSE;
        }
    }
  INFO_msg(10,HMessageMgr::DET_TRIGGER,"dilep parameters initialized");

  // allocate input categories
  if((ptlHMULeptons=gHades->getCurrentEvent()->getCategory(catMUEMULeptons))==NULL) return kFALSE;
  iterHMULeptons = (HIterator*)ptlHMULeptons->MakeIterator("native");
  iterHMULeptons2 = (HIterator*)ptlHMULeptons->MakeIterator("native");
  INFO_msg(10,HMessageMgr::DET_TRIGGER,"emuleptons inbput initialized");

// allocate output categories
  ptlHMUEMUDiLeptons=gHades->getCurrentEvent()->getCategory(catMUEMUDiLeptons);
  if(!ptlHMUEMUDiLeptons)
    {
      ptlHMUEMUDiLeptons=pTrigDet->buildLinearCat("HMUEMUDiLeptonsSim");
      if (!ptlHMUEMUDiLeptons)
        {
          return kFALSE;
        }
      else
        {
          gHades->getCurrentEvent()->addCategory(catMUEMUDiLeptons, ptlHMUEMUDiLeptons, "EmuDiLeptons");
        }
    }  
  INFO_msg(10,HMessageMgr::DET_TRIGGER,"dilep output initialized");
  return kTRUE;
}
//============================================================================
//----------------------------------------------------------------------------
Bool_t HMUDilepEmulationSim::finalize(void)
{
  // Finalize the job f the taskset, nothing to be done here!
  //
  // Input parameters:
  //   none
  //
  // Output parameters:
  //   none
  //
  // Return code:
  //   Allways kTRUE!
  return kTRUE;
}
//============================================================================

ClassImp(HMUDilepEmulationSim)
