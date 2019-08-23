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

#include <iostream.h>         

HMUEmulation::HMUEmulation() { 
 pEmuLeptons=NULL;
 nEmuLeptons=0;
}
//============================================================================

//----------------------------------------------------------------------------

HMUEmulation::HMUEmulation(Text_t *name,Text_t *title)
               : HReconstructor(name, title) {
 pEmuLeptons=NULL;
 nEmuLeptons=0;
}
//============================================================================

//----------------------------------------------------------------------------

HMUEmulation::~HMUEmulation() { 
 pEmuLeptons=NULL;
 nEmuLeptons=0;

  if (iterHMatchURich) delete iterHMatchURich;
  if (iterHMatchUTof) delete iterHMatchUTof;
  if (iterHMatchUShower) delete iterHMatchUShower;
  
  // set everything to 0
  // especially the iterators
}

//============================================================================

//----------------------------------------------------------------------------

Bool_t HMUEmulation::init(){
  //  HRichDetector * pRichDet = (HRichDetector*)gHades->getSetup()->getDetector("Rich");
   HTriggerDetector * pTrigDet = (HTriggerDetector*)gHades->getSetup()->getDetector("Trigger");
    // allocate input categories

  ptlHMatchURich=gHades->getCurrentEvent()->getCategory(catMatchURich);
  //cout << "input category exists" << endl;
  if (!ptlHMatchURich) return kFALSE;
  else gHades->getCurrentEvent()
	 ->addCategory(catMatchURich, ptlHMatchURich, "Rich");

  iterHMatchURich = (HIterator*)ptlHMatchURich->MakeIterator("native");

  ptlHMatchUTof=gHades->getCurrentEvent()->getCategory(catMatchUTof);
  //cout << "input category exists" << endl;
  if (!ptlHMatchUTof) return kFALSE;
  else gHades->getCurrentEvent()
	 ->addCategory(catMatchUTof, ptlHMatchUTof, "Tof");
  
  iterHMatchUTof = (HIterator*)ptlHMatchUTof->MakeIterator("native");

  ptlHMatchUShower=gHades->getCurrentEvent()->getCategory(catMatchUShower);
  //cout << "input category exists" << endl;
  if (!ptlHMatchUShower) return kFALSE;
  else gHades->getCurrentEvent()
	 ->addCategory(catMatchUShower, ptlHMatchUShower, "Shower");
  
  iterHMatchUShower = (HIterator*)ptlHMatchUShower->MakeIterator("native");

// allocate output categories
  ptlHMUEMULeptons=gHades->getCurrentEvent()->getCategory(catMUEMULeptons);
  //cout << "input category exists" << endl;
   if(!ptlHMUEMULeptons){
     //cout << " Build Category " << endl;

     ptlHMUEMULeptons=pTrigDet->buildCategory(catMUEMULeptons);
     if (!ptlHMUEMULeptons){ 
       //cout << "output category does not exist!!! " << endl;     
       return kFALSE;
     }
     else gHades->getCurrentEvent()
	    ->addCategory(catMUEMULeptons, ptlHMUEMULeptons, "EmuLeptons");
   }  

   if (pEmuLeptons) delete [] pEmuLeptons;
   pEmuLeptons =new HMUEMULeptons[1000]; //5000 non vuol dire un cazzo
   //showMe->pEmuLeptons = pEmuLeptons; 

fMUMomMap = new HMUMomMap();
   
   return kTRUE;
}

//============================================================================

//----------------------------------------------------------------------------

Int_t HMUEmulation::execute() {
  // cout << "start the execute " << endl;
HMatchURich   *ptlHMatchURich;
HMatchUTof    *ptlHMatchUTof;
HMatchUShower *ptlHMatchUShower;

Int_t a,b,c;
Float_t leptonPhi;

  iterHMatchURich->Reset();

  nEmuLeptons=0;
  
  while ((ptlHMatchURich = (HMatchURich *)iterHMatchURich->Next()) != 0) {     
    iterHMatchUTof->Reset();    
    //cout << "phi rich " << ptlHMatchURich->getPhi() <<  endl; 	  
 
    while ((ptlHMatchUTof = (HMatchUTof *)iterHMatchUTof->Next()) != 0) {
      if (ptlHMatchURich->getSegmentId()==ptlHMatchUTof->getSector()) {
	if (fabs(ptlHMatchURich->getPhi()-ptlHMatchUTof->getPhi()) < 15.7647058823529402){
	  if(ptlHMatchUTof->getPID() ==1){
	    //cout << "phi rich " << ptlHMatchURich->getPhi() << "  phi tof " << ptlHMatchUTof->getPhi() << endl; 	  
	    //emuleptons
	    HMUEMULeptons &lepton = pEmuLeptons[nEmuLeptons]; 
	    
	    lepton.iDetBit     = 0;
	    lepton.fThetaRich  = ptlHMatchURich->getTheta() ;
	    lepton.fPhiRich    = ptlHMatchURich->getPhi();
	    lepton.fThetaMeta  = ptlHMatchUTof->getTheta() ;
	    lepton.fPhiMeta    = ptlHMatchUTof->getPhi();
//  	    cout << "lepton  " << lepton.iDetBit << "  " << lepton.fThetaRich << "  " 
// 		 << lepton.fThetaRich-lepton.fThetaMeta  << "  " << lepton.fPhiRich << endl;
	    
	    a=(lepton.fThetaRich)/90*64;
	    b=fabs(lepton.fThetaRich-lepton.fThetaMeta)/90*64;

	    if (ptlHMatchURich->getSegmentId()<5)
	      leptonPhi = lepton.fPhiRich -(ptlHMatchURich->getSegmentId()*60);

	    c=fabs((leptonPhi)-30)/30*8;
	    //cout << a << "  " << b << "  "<< c << "   "  << mu_mom[a][b][c] << endl;
	    lepton.fMom        = fMUMomMap->getMom(a,b,c);
	    
	 	    
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
      //cout << "  phi shower " << ptlHMatchUShower->getPhi() << endl; 	  

      if (ptlHMatchURich->getSegmentId()==ptlHMatchUShower->getSector()) { 
	if (fabs(ptlHMatchURich->getPhi()-ptlHMatchUShower->getPhi()) < 15.7647058823529402){
	  //cout << "phi rich " << ptlHMatchURich->getPhi() << "  phi shower " << ptlHMatchUShower->getPhi() << endl; 	  
	  //emuleptons 
	  HMUEMULeptons & lepton = pEmuLeptons[nEmuLeptons]; 	
	  
	  lepton.iDetBit     = 1;
	  lepton.fThetaRich  = ptlHMatchURich->getTheta() ;
	  lepton.fPhiRich    = ptlHMatchURich->getPhi();
	  lepton.fThetaMeta  = ptlHMatchUShower->getTheta() ;
	  lepton.fPhiMeta    = ptlHMatchUShower->getPhi();
 	  //cout << "lepton  " << lepton.iDetBit << "  " << lepton.fThetaRich << "  " << lepton.fPhiRich << endl;

	  a=(lepton.fThetaRich)/90*64;
	  b=fabs(lepton.fThetaRich-lepton.fThetaMeta)/90*64;
	  c=fabs((lepton.fPhiMeta)-30)/30*8;
	  //cout << a << "  " << b << "  "<< c << "   "  << mu_mom[a][b][c] << endl;
	  lepton.fMom        = fMUMomMap->getMom(a,b,c);
	  //cout << lepton.iDetBit << "  " << lepton.fThetaRich << "  " << lepton.fPhiRich << endl;
	  
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

 return kTRUE;  
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
      //cout << " Set emuLepton: " << pEmuLeptons[i].fThetaRich << endl;
    } else {
      //cout << "emuLepton = NULL" << endl;
    }
  }
}      
//============================================================================
//----------------------------------------------------------------------------

Bool_t HMUEmulation::finalize(){

 return kTRUE;
}

//============================================================================

//----------------------------------------------------------------------------

ClassImp(HMUEmulation)











