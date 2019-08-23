#include "muDilepEmulation.h"
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


void HMUDilepEmulation::initParContainer(HSpectrometer *spec, HRuntimeDb *rtdb) {
  // Adds to the runtime database "rtdb" the containers needed by the MU Emulation
  //spec is used to get information of the spectrometer setup.
  fTrigPar=(HTriggerParMuDilep *)rtdb->getContainer("TriggerParMuDilep");
  if (fTrigPar) {
    cout << "Trigger Parameters MU Dilepton Emulation initialized" << endl;
  } else {
    cout << "******  Trigger Parameter MU Dilepton Emulation Initialization failed!! *********" << endl;
  }
}
//============================================================================

//----------------------------------------------------------------------------
HMUDilepEmulation::HMUDilepEmulation() { 
  fTrigPar = NULL;
  pEmuDiLeptons=NULL;
  nEmuDiLeptons=0;
}
//============================================================================

//----------------------------------------------------------------------------
HMUDilepEmulation::HMUDilepEmulation(Text_t *name,Text_t *title)
               : HReconstructor(name, title) {
  fTrigPar = NULL;
  pEmuDiLeptons=NULL;
  nEmuDiLeptons=0;
}
//============================================================================

//----------------------------------------------------------------------------
HMUDilepEmulation::~HMUDilepEmulation() { 
  // set everything to 0
  // especially the iterators
  fTrigPar = NULL;
  pEmuDiLeptons=NULL;
  nEmuDiLeptons=0;

  if (iterHMULeptons) delete iterHMULeptons;
  if (iterHMULeptons2) delete iterHMULeptons2;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HMUDilepEmulation::init(){
  HSpectrometer *spec = gHades->getSetup();
  HRuntimeDb *rtdb = gHades->getRuntimeDb();
  HTriggerDetector * pTrigDet = (HTriggerDetector*)gHades->getSetup()->getDetector("Trigger");
  
  // initialize parameters
  if (pTrigDet)
    initParContainer(spec,rtdb);  

    // allocate input categories
  ptlHMULeptons=gHades->getCurrentEvent()->getCategory(catMatchULeptons);
  if (!ptlHMULeptons) return kFALSE;
  else gHades->getCurrentEvent()
	 ->addCategory(catMatchULeptons, ptlHMULeptons, "Leptons");

  iterHMULeptons = (HIterator*)ptlHMULeptons->MakeIterator("native");
  iterHMULeptons2 = (HIterator*)ptlHMULeptons->MakeIterator("native");


// allocate output categories
  ptlHMUEMUDiLeptons=gHades->getCurrentEvent()->getCategory(catMUEMUDiLeptons);
   if(!ptlHMUEMUDiLeptons){
     ptlHMUEMUDiLeptons=pTrigDet->buildCategory(catMUEMUDiLeptons);
     if (!ptlHMUEMUDiLeptons){ 
       return kFALSE;
     }
     else gHades->getCurrentEvent()
	    ->addCategory(catMUEMUDiLeptons, ptlHMUEMUDiLeptons, "EmuDiLeptons");
   }  

   if (pEmuDiLeptons) delete [] pEmuDiLeptons;
   pEmuDiLeptons =new HMUEMUDiLeptons[1000]; //extra-size array

   return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
Int_t HMUDilepEmulation::execute() {

  Float_t rth1, rph1, p1, eleflag1;
  Float_t rth2, rph2, p2, eleflag2;
  Int_t i1,i2;
  HGeomVector pp1,pp2;

  i1=i2=0;
  nEmuDiLeptons=0;
  
  if ( nEmuDiLeptons < fTrigPar->getMUcutoff() ) { // MU cutoff
    
    HMULeptons   *ptlHMULeptons;
    iterHMULeptons->Reset();
    while (( ptlHMULeptons  = (HMULeptons *)iterHMULeptons->Next()) != 0) {  
      
      rth1= ptlHMULeptons->getThetaRich();
      rph1= ptlHMULeptons->getPhiRich();
      p1 = (ptlHMULeptons->getMom())/255*1000;
      eleflag1= ptlHMULeptons->getFlag();  
      i1++;
      //      cout <<"1: " << ptlHMULeptons->getTheta() << "  " << ptlHMULeptons->getFlag()
      // 	  << "  " << (ptlHMULeptons->getMom())/255*1000 << endl;
      
      i2=0;    
      HMULeptons   *ptlHMULeptons2;
      iterHMULeptons2->Reset();
      while (( ptlHMULeptons2  = (HMULeptons *)iterHMULeptons2->Next()) != ptlHMULeptons ) {  
	rth2= ptlHMULeptons2->getThetaRich();
	rph2= ptlHMULeptons2->getPhiRich();
	p2  = (ptlHMULeptons2->getMom())/255*1000;
	eleflag2= ptlHMULeptons2->getFlag();  
	i2++;
	//     cout <<"2: " << ptlHMULeptons2->getTheta() << "  " << ptlHMULeptons2->getFlag()
	// 	 << "  " << (ptlHMULeptons2->getMom())/255*1000 << endl;
	
	
	if (eleflag1 != eleflag2) { // unlike sign pairs
	  
	  pp1.setX( sin(rth1* TMath::Pi()/180)*cos(rph1* TMath::Pi()/180));
	  pp1.setY( sin(rth1* TMath::Pi()/180)*sin(rph1* TMath::Pi()/180));
	  pp1.setZ( cos(rth1* TMath::Pi()/180) );
	  
	  pp2.setX( sin(rth2* TMath::Pi()/180)*cos(rph2* TMath::Pi()/180) );
	  pp2.setY( sin(rth2* TMath::Pi()/180)*sin(rph2* TMath::Pi()/180) );
	  pp2.setZ( cos(rth2* TMath::Pi()/180) );
	  
	  Float_t cOpening = pp1.scalarProduct(pp2);
	  Float_t openAngle=acos(cOpening);
	  
	  //emudileptons
	  HMUEMUDiLeptons & dilepton = pEmuDiLeptons[nEmuDiLeptons]; 

	  
	  dilepton.opangle = openAngle * 180/TMath::Pi();
	  dilepton.fMass = 2 * p1 * p2 * (1-cOpening) ; // michael formula (mass squared)
	  dilepton.iN1 = i1;
	  dilepton.iN2 = i2;
	  
	  nEmuDiLeptons++;	  
	}	
      }      
    }    
  }

  //cout << "number of leptons " << nEmuLeptons << endl;
  if (nEmuDiLeptons) updateEmuDiLeptons();
  
  return 0;  
}
//============================================================================
//----------------------------------------------------------------------------
void HMUDilepEmulation::updateEmuDiLeptons() {

  HMUEMUDiLeptons *emuDiLepton=NULL; 
    
  for (int i = 0; i < nEmuDiLeptons; i++) {
    loc.set(1, 0);
    emuDiLepton=(HMUEMUDiLeptons *)ptlHMUEMUDiLeptons->getNewSlot(loc);
    
    if (emuDiLepton!=NULL) {
      emuDiLepton=new(emuDiLepton) HMUEMUDiLeptons;      
      *emuDiLepton = pEmuDiLeptons[i];
    } 
  }
}      
//============================================================================
//----------------------------------------------------------------------------
Bool_t HMUDilepEmulation::finalize(){  
 return kTRUE;
}
//============================================================================

ClassImp(HMUDilepEmulation)











