#include "muEmulation.h"
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
#include "hmatrixcatiter.h"
#include "triggerinfodef.h"
#include "muEmulation.h"
#include "htriggerdetector.h"
#include "hlocation.h"
#include "htriggerparmulep.h"
#include "htriggerparmommap.h"

#include <iostream.h>         

void HMUEmulation::initParContainer(HSpectrometer *spec, HRuntimeDb *rtdb) {
  // Adds to the runtime database "rtdb" the containers needed by the MU Emulation
  //spec is used to get information of the spectrometer setup.
  fTrigPar=(HTriggerParMuLep *)rtdb->getContainer("TriggerParMuLep");
  if (fTrigPar) {
    cout << "Trigger Parameters MU Lepton Emulation initialized" << endl;
  } else {
    cout << "******  Trigger Parameter MU Lepton Emulation Initialization failed!! *********" << endl;
  }

  fTrigParMom=(HTriggerParMomMap *)rtdb->getContainer("TriggerParMomMap");
  if (fTrigParMom) {
    cout << "Trigger Parameters MU Momentum Mapping initialized" << endl;
  } else {
    cout << "******  Trigger Parameter MU Momentum Mapping Initialization failed!! *********" << endl;
  }
}

HMUEmulation::HMUEmulation() { 
  fTrigPar = NULL;
  fTrigParMom = NULL;
  pEmuLeptons=NULL;
  nEmuLeptons=0;
}
//============================================================================

//----------------------------------------------------------------------------
HMUEmulation::HMUEmulation(Text_t *name,Text_t *title)
               : HReconstructor(name, title) {
  fTrigPar = NULL;
  fTrigParMom = NULL;
  pEmuLeptons=NULL;
  nEmuLeptons=0;
}
//============================================================================

//----------------------------------------------------------------------------
HMUEmulation::~HMUEmulation() { 
  // set everything to 0
  // especially the iterators
  fTrigPar = NULL;
  fTrigParMom = NULL;
  pEmuLeptons=NULL;
  nEmuLeptons=0;
  
  if (iterHMatchURich) delete iterHMatchURich;
  if (iterHMatchUTof) delete iterHMatchUTof;
  if (iterHMatchUShower) delete iterHMatchUShower;
}

//============================================================================

//----------------------------------------------------------------------------
Bool_t HMUEmulation::init(){
  HSpectrometer *spec = gHades->getSetup();
  HRuntimeDb *rtdb = gHades->getRuntimeDb();
  HTriggerDetector * pTrigDet = (HTriggerDetector*)gHades->getSetup()->getDetector("Trigger");
  
  // initialize parameters
  if (pTrigDet)
    initParContainer(spec,rtdb);
  
  // allocate input categories
  ptlHMatchURich=gHades->getCurrentEvent()->getCategory(catMatchURich);
  if (!ptlHMatchURich) return kFALSE;
  else gHades->getCurrentEvent()
	 ->addCategory(catMatchURich, ptlHMatchURich, "Rich");  
  iterHMatchURich = (HIterator*)ptlHMatchURich->MakeIterator("native");

  
  ptlHMatchUTof=gHades->getCurrentEvent()->getCategory(catMatchUTof);
  if (!ptlHMatchUTof) return kFALSE;
  else gHades->getCurrentEvent()
	 ->addCategory(catMatchUTof, ptlHMatchUTof, "Tof");  
  iterHMatchUTof = (HIterator*)ptlHMatchUTof->MakeIterator("native");

  
  ptlHMatchUShower=gHades->getCurrentEvent()->getCategory(catMatchUShower);
  if (!ptlHMatchUShower) return kFALSE;
  else gHades->getCurrentEvent()
	 ->addCategory(catMatchUShower, ptlHMatchUShower, "Shower");
  iterHMatchUShower = (HIterator*)ptlHMatchUShower->MakeIterator("native");

  
  // allocate output categories
  ptlHMUEMULeptons=gHades->getCurrentEvent()->getCategory(catMUEMULeptons);
  if(!ptlHMUEMULeptons){
    ptlHMUEMULeptons=pTrigDet->buildCategory(catMUEMULeptons);
    if (!ptlHMUEMULeptons){ 
      return kFALSE;
    }
    else gHades->getCurrentEvent()
	   ->addCategory(catMUEMULeptons, ptlHMUEMULeptons, "EmuLeptons");
  }  
  
  if (pEmuLeptons) delete [] pEmuLeptons;
  pEmuLeptons =new HMUEMULeptons[1000];  //extra-size array
  
  return kTRUE;
}

//============================================================================

//----------------------------------------------------------------------------
Int_t HMUEmulation::execute() {

HMatchURich   *ptlHMatchURich;
HMatchUTof    *ptlHMatchUTof;
HMatchUShower *ptlHMatchUShower;

Float_t a,b,c;

iterHMatchURich->Reset();
nEmuLeptons=0;
  
 while ((ptlHMatchURich = (HMatchURich *)iterHMatchURich->Next()) != 0) {     
   iterHMatchUTof->Reset();    
   
   while ((ptlHMatchUTof = (HMatchUTof *)iterHMatchUTof->Next()) != 0) {
     if (ptlHMatchURich->getSegmentId()==ptlHMatchUTof->getSector()) {
       if (  (fabs(ptlHMatchURich->getPhi()-ptlHMatchUTof->getPhi()) < fTrigPar->getMUphi() )
	  && (fabs(ptlHMatchURich->getTheta()-ptlHMatchUTof->getTheta()) < fTrigPar->getMUtheta() )  )  {
	 
	 if(ptlHMatchUTof->getPID() ==1){
	   
	   //emuleptons
	   HMUEMULeptons & lepton = pEmuLeptons[nEmuLeptons]; 
	   
	   lepton.iDetBit     = 0;
	   lepton.fThetaRich  = ptlHMatchURich->getTheta() ;
	   lepton.fPhiRich    = ptlHMatchURich->getPhi();
	   lepton.fThetaMeta  = ptlHMatchUTof->getTheta() ;
	   lepton.fPhiMeta    = ptlHMatchUTof->getPhi();
	   
	   // re-binning of variables according to the momentum table
	   a=(lepton.fThetaRich)/90*(fTrigParMom->getTheta_bins());
	   b=fabs(lepton.fThetaRich-lepton.fThetaMeta)/90*(fTrigParMom->getDeltatheta_bins());
	   c=fabs((lepton.fPhiMeta)-30)/30*(fTrigParMom->getPhi_bins());
	   lepton.fMom        = fTrigParMom->getMom((int)a,(int)b,(int)c);
	   
	   if ((ptlHMatchURich->getTheta()-ptlHMatchUTof->getTheta()) < 0) 
	     lepton.iFlag = 1;  //electrons;
	   else  if ((ptlHMatchURich->getTheta()-ptlHMatchUTof->getTheta()) > 0) 
	     lepton.iFlag = 0;  //positrons;
	   
	   nEmuLeptons++;
	   
	 }
       }
     }      
   }
   
   iterHMatchUShower->Reset();
   while ((ptlHMatchUShower = (HMatchUShower *)iterHMatchUShower->Next()) != 0) {
     
     if (ptlHMatchURich->getSegmentId()==ptlHMatchUShower->getSector()) { 
       if (  (fabs(ptlHMatchURich->getPhi()-ptlHMatchUShower->getPhi()) < fTrigPar->getMUphi() )
          && (fabs(ptlHMatchURich->getTheta()-ptlHMatchUShower->getTheta()) < fTrigPar->getMUtheta() )  ){
	 
	 //emuleptons 
	 HMUEMULeptons & lepton = pEmuLeptons[nEmuLeptons]; 	
	 
	 lepton.iDetBit     = 1;
	 lepton.fThetaRich  = ptlHMatchURich->getTheta() ;
	 lepton.fPhiRich    = ptlHMatchURich->getPhi();
	 lepton.fThetaMeta  = ptlHMatchUShower->getTheta() ;
	 lepton.fPhiMeta    = ptlHMatchUShower->getPhi();
	 
	 a=(lepton.fThetaRich)/90*(fTrigParMom->getTheta_bins());
	 b=fabs(lepton.fThetaRich-lepton.fThetaMeta)/90*(fTrigParMom->getDeltatheta_bins());
	 c=fabs((lepton.fPhiMeta)-30)/30*(fTrigParMom->getPhi_bins());
	 lepton.fMom        = fTrigParMom->getMom((int)a,(int)b,(int)c);
	  
	  
	 if ((ptlHMatchURich->getTheta()-ptlHMatchUShower->getTheta()) < 0) 
	   lepton.iFlag = 1;    //electrons;
	 else  if ((ptlHMatchURich->getTheta()-ptlHMatchUShower->getTheta()) > 0) 
	   lepton.iFlag = 0;  //positrons;
	 
	 nEmuLeptons++;
	 
       }
     }
   }    
 }
 //cout << "number of leptons " << nEmuLeptons << endl;
 if (nEmuLeptons) updateEmuLeptons();
 
 return 0;  
}
//============================================================================
//----------------------------------------------------------------------------
void HMUEmulation::updateEmuLeptons() {
  
  HMUEMULeptons *emuLepton=NULL; 
      
  for (int i = 0; i < nEmuLeptons; i++) {
    loc.set(1, 0);    
    emuLepton=(HMUEMULeptons *)ptlHMUEMULeptons->getNewSlot(loc);
    
    if (emuLepton!=NULL) {
      emuLepton=new(emuLepton) HMUEMULeptons;      
      *emuLepton = pEmuLeptons[i];
    } 
  }
}      
//============================================================================
//----------------------------------------------------------------------------
Bool_t HMUEmulation::finalize(){
 return kTRUE;
}
//============================================================================

ClassImp(HMUEmulation)





  





