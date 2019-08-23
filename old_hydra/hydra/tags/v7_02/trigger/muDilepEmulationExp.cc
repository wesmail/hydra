#include "muDilepEmulationExp.h"
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
#include "hmuEmuleptonsExp.h"
#include "hmuleptons.h"
#include "hmatrixcatiter.h"
#include "triggerinfodef.h"
#include "htriggerdetector.h"
#include "hlocation.h"
#include "hgeomvector.h"
#include "htriggerparmudilep.h"

#include <iostream.h>         

//----------------------------------------------------------------------------
HMUDilepEmulationExp::HMUDilepEmulationExp() { 
    nLep=0;
    for (int h =0; h<600;h++) {
	thetaRich[h]=0;
	phiRich[h]=0;
	mom[h]=0;
	flag[h]=0;
     }

}
//============================================================================

//----------------------------------------------------------------------------
HMUDilepEmulationExp::HMUDilepEmulationExp(Text_t *name,Text_t *title)
               : HMUDilepEmulation(name, title) {
    nLep=0;
    for (int h =0; h<600;h++) {
	thetaRich[h]=0;
	phiRich[h]=0;
	mom[h]=0;
	flag[h]=0;
     }

}
//============================================================================

//----------------------------------------------------------------------------
HMUDilepEmulationExp::~HMUDilepEmulationExp() { 
    nLep=0;
    for (int h =0; h<600;h++) {
	thetaRich[h]=0;
	phiRich[h]=0;
	mom[h]=0;
	flag[h]=0;
     }

}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HMUDilepEmulationExp::init(){
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
  cout << "emuleptons input initialized" << endl;

  iterHMULeptons = (HIterator*)ptlHMULeptons->MakeIterator("native");

// allocate output categories
  ptlHMUEMUDiLeptons=gHades->getCurrentEvent()->getCategory(catMUEMUDiLeptons);
  if(!ptlHMUEMUDiLeptons){
    ptlHMUEMUDiLeptons=pTrigDet->buildCategory(catMUEMUDiLeptons);
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
Int_t HMUDilepEmulationExp::execute() {
    nLep=0;
    for (int h =0; h<600;h++) {
	thetaRich[h]=0;
	phiRich[h]=0;
	mom[h]=0;
	flag[h]=0;
    }
    prepareInput();
    HMUDilepEmulation::searchDilep();
    if (nEmuDiLeptons) HMUDilepEmulation::updateEmuDiLeptons();

   return 0;
}
//============================================================================

//----------------------------------------------------------------------------
void HMUDilepEmulationExp::prepareInput() {
    HMUEMULeptonsExp   *pLeptons =NULL;
    iterHMULeptons->Reset();
    while (( pLeptons  = (HMUEMULeptonsExp *)iterHMULeptons->Next()) != 0) {  
	if (pLeptons->getMom()>0) {
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
Bool_t HMUDilepEmulationExp::finalize(){  
 return kTRUE;
}
//============================================================================

ClassImp(HMUDilepEmulationExp)











