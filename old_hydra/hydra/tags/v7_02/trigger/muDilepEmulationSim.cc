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

#include <iostream.h>         

//----------------------------------------------------------------------------
HMUDilepEmulationSim::HMUDilepEmulationSim() { 
}
//============================================================================

//----------------------------------------------------------------------------
HMUDilepEmulationSim::HMUDilepEmulationSim(Text_t *name,Text_t *title)
               : HMUDilepEmulation(name, title) {
}
//============================================================================

//----------------------------------------------------------------------------
HMUDilepEmulationSim::~HMUDilepEmulationSim() { 
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HMUDilepEmulationSim::init(){
  HSpectrometer *spec = gHades->getSetup();
  HRuntimeDb *rtdb = gHades->getRuntimeDb();
  HTriggerDetector * pTrigDet = (HTriggerDetector*)gHades->getSetup()->getDetector("Trigger");
  
  // initialize parameters
  if (pTrigDet)
    initParContainer(spec,rtdb);  
  cout << "dilep parameters initialized" << endl;

  // allocate input categories
  ptlHMULeptons=gHades->getCurrentEvent()->getCategory(catMUEMULeptons);
  if (!ptlHMULeptons) return kFALSE;
  else gHades->getCurrentEvent()
	 ->addCategory(catMUEMULeptons, ptlHMULeptons, "EmuLeptons");
  cout << "emuleptons inbput initialized" << endl;

  iterHMULeptons = (HIterator*)ptlHMULeptons->MakeIterator("native");
  iterHMULeptons2 = (HIterator*)ptlHMULeptons->MakeIterator("native");


// allocate output categories
  ptlHMUEMUDiLeptons=gHades->getCurrentEvent()->getCategory(catMUEMUDiLeptons);
  if(!ptlHMUEMUDiLeptons){
    ptlHMUEMUDiLeptons=pTrigDet->buildLinearCat("HMUEMUDiLeptonsSim");
    if (!ptlHMUEMUDiLeptons) return kFALSE;
    else gHades->getCurrentEvent()
	   ->addCategory(catMUEMUDiLeptons, ptlHMUEMUDiLeptons, "EmuDiLeptons");
  }  
  cout << "dilep output initialized " << endl;

   //if (pEmuDiLeptons) delete [] pEmuDiLeptons;
   pEmuDiLeptons =new HMUEMUDiLeptons[1000]; //extra-size array

   return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
Int_t HMUDilepEmulationSim::execute() {
  HMUDilepEmulation::execute();
  return kTRUE;  

}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HMUDilepEmulationSim::finalize(){  
 return kTRUE;
}
//============================================================================

ClassImp(HMUDilepEmulationSim)











