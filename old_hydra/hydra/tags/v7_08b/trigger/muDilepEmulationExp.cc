//*-- Author: Alberica Toia
//*-- Modified: Joern Wuestenfeld 05/10/2005
//*-- Version: $Id: muDilepEmulationExp.cc,v 1.6 2005-10-12 08:49:27 kempter Exp $

#include "muDilepEmulationExp.h"
#include "hdebug.h"
#include "hades.h"
#include "hcategory.h"
#include "hiterator.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "hevent.h"
#include "heventheader.h"
#include "hmatchurich.h"
#include "hmatchutof.h"
#include "hmatchushower.h"
#include "hmuEmuleptonsExp.h"
#include "hmuleptons.h"
#include "hmatrixcatiter.h"
#include "triggerinfodef.h"
#include "htriggerdetector.h"
#include "hlocation.h"
#include "hgeomvector.h"
#include "htriggerparmudilep.h"
#include "hmessagemgr.h"

//
// HMuDilepEmulationExp
//
// Matching unit emulation code.
// This class is to be used for real data that does not contain the matching unit
// subevent.
//


//----------------------------------------------------------------------------
HMUDilepEmulationExp::HMUDilepEmulationExp(void)
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
HMUDilepEmulationExp::HMUDilepEmulationExp(Text_t *name,Text_t *title)
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
HMUDilepEmulationExp::~HMUDilepEmulationExp(void)
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
Bool_t HMUDilepEmulationExp::init()
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
  INFO_msg(10,HMessageMgr::DET_TRIGGER,"emuleptons input initialized");

// allocate output categories
  ptlHMUEMUDiLeptons=gHades->getCurrentEvent()->getCategory(catMUEMUDiLeptons);
  if(!ptlHMUEMUDiLeptons)
    {
      ptlHMUEMUDiLeptons=pTrigDet->buildCategory(catMUEMUDiLeptons);
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
void HMUDilepEmulationExp::prepareInput()
{
  // Retrieve information from categories, and store them in internal variables.
  //
  // Input parameters:
  //   none
  //
  // Output parameters:
  //   none
  //
  // Return code:
  //   none
  HMUEMULeptonsExp   *pLeptons =NULL;
  iterHMULeptons->Reset();
  while (( pLeptons  = (HMUEMULeptonsExp *)iterHMULeptons->Next()) != 0)
  {
      if(nLep>599)
      {
	  Error("prepareInput()","number of leptons >= size of array in event %i sequence number %i !",
		gHades->getEventCounter(),
		gHades->getCurrentEvent()->getHeader()->getEventSeqNumber());
	  break;
      }

      if (pLeptons->getMom()>0)
      {
	  thetaRich[nLep]=pLeptons->getThetaRich();
	  phiRich[nLep]=pLeptons->getPhiRich();
	  mom[nLep]=pLeptons->getMom();
	  flag[nLep]=pLeptons->getFlag();
	  nLep++;
      }
  }
}
//============================================================================
//----------------------------------------------------------------------------
Bool_t HMUDilepEmulationExp::finalize()
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

ClassImp(HMUDilepEmulationExp)
