using namespace std;
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

#include <iostream>          
#include <iomanip>


void HMUDilepEmulation::initParContainer(HSpectrometer *spec, HRuntimeDb *rtdb) {
  // Adds to the runtime database "rtdb" the containers needed by the MU Emulation
  //spec is used to get information of the spectrometer setup.
  fTrigParDil=(HTriggerParMuDilep *)rtdb->getContainer("TriggerParMuDilep");
  if (fTrigParDil) {
    cout << "Trigger Parameters MU Dilepton Emulation initialized" << endl;
  } else {
    cout << "******  Trigger Parameter MU Dilepton Emulation Initialization failed!! *********" << endl;
  }
}
//============================================================================

//----------------------------------------------------------------------------
HMUDilepEmulation::HMUDilepEmulation() { 
  
  fTrigParDil = NULL;
  pEmuDiLeptons=NULL;
  nEmuDiLeptons=0;
  ptlHMULeptons=NULL;
  ptlHMUEMUDiLeptons=NULL;
  iterHMULeptons=NULL;
  iterHMULeptons2=NULL;
  
}
//============================================================================

//----------------------------------------------------------------------------
HMUDilepEmulation::HMUDilepEmulation(Text_t *name,Text_t *title)
               : HReconstructor(name, title) {
	
  fTrigParDil = NULL;
  pEmuDiLeptons=NULL;
  nEmuDiLeptons=0;
  ptlHMULeptons=NULL;
  ptlHMUEMUDiLeptons=NULL;
  iterHMULeptons=NULL;
  iterHMULeptons2=NULL;
  
}
//============================================================================

//----------------------------------------------------------------------------
HMUDilepEmulation::~HMUDilepEmulation() { 
  // set everything to 0
  // especially the iterators
  

  pEmuDiLeptons=NULL;
  nEmuDiLeptons=0;

  if (ptlHMULeptons) delete ptlHMULeptons;
  ptlHMULeptons=NULL;
  if (ptlHMUEMUDiLeptons) delete ptlHMUEMUDiLeptons;
  ptlHMUEMUDiLeptons=NULL;

  if (iterHMULeptons) delete iterHMULeptons;
  iterHMULeptons=NULL;
  if (iterHMULeptons2) delete iterHMULeptons2;
  iterHMULeptons2=NULL;
  
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

   //if (pEmuDiLeptons) delete [] pEmuDiLeptons;
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

  rth1 = rph1 = p1 = eleflag1=0;
  rth2 = rph2 = p2 = eleflag2=0;
  i1=i2=0;
  pp1=pp2=0;

  nEmuDiLeptons=0;
 
    
    HMULeptons   *pLeptons =NULL;
    iterHMULeptons->Reset();
    while (( pLeptons  = (HMULeptons *)iterHMULeptons->Next()) != 0) {  
      
      rth1= pLeptons->getThetaRich();
      rph1= pLeptons->getPhiRich();
      p1 = (pLeptons->getMom())/255*1000;
      eleflag1= pLeptons->getFlag();  
      i1++;
//        cout <<"1: " << pLeptons->getThetaRich() << "  " << pLeptons->getFlag()
//  	   << "  " << (pLeptons->getMom())/255*1000 << endl;
      
      i2=0;    
      HMULeptons   *pLeptons2 =NULL;
      iterHMULeptons2->Reset();
      while (( pLeptons2  = (HMULeptons *)iterHMULeptons2->Next()) != pLeptons ) {  
	rth2= pLeptons2->getThetaRich();
	rph2= pLeptons2->getPhiRich();
	p2  = (pLeptons2->getMom())/255*1000;
	eleflag2= pLeptons2->getFlag();  
	i2++;
//  	cout <<"2: " << pLeptons2->getThetaRich() << "  " << pLeptons2->getFlag()
//  	     << "  " << (pLeptons2->getMom())/255*1000 << endl;
	
	
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
  

  if ( nEmuDiLeptons < fTrigParDil->getMUcutoff() ) // MU cutoff
    updateEmuDiLeptons();
  
  return kTRUE;  

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











