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
HMUDilepEmulation::HMUDilepEmulation(Text_t *name,Text_t *title)
               : HReconstructor(name, title) {
	
  fTrigParDil = NULL;
  pEmuDiLeptons=NULL;
  nEmuDiLeptons=0;
  ptlHMULeptons=NULL;
  ptlHMUEMUDiLeptons=NULL;
  iterHMULeptons=NULL;
  iterHMULeptons2=NULL;
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
   pEmuDiLeptons =new HMUEMUDiLeptons[100]; //extra-size array

   return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
Int_t HMUDilepEmulation::execute() {
    nLep=0;
    for (int h =0; h<600;h++) {
	thetaRich[h]=0;
	phiRich[h]=0;
	mom[h]=0;
	flag[h]=0;
     }

    prepareInput();
    searchDilep();
    if (nEmuDiLeptons) updateEmuDiLeptons();
    return 0;

}
//============================================================================
 
//----------------------------------------------------------------------------
void HMUDilepEmulation::prepareInput() {
    HMULeptons   *pLeptons =NULL;
    iterHMULeptons->Reset();
    while (( pLeptons  = (HMULeptons *)iterHMULeptons->Next()) != 0) {  
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
Int_t HMUDilepEmulation::searchDilep() {
    Int_t i1,i2;
    HGeomVector pp1,pp2;
    pp1=pp2=0;
    
    nEmuDiLeptons=0;
    
    if ( nEmuDiLeptons < fTrigParDil->getMUcutoff() ) { // MU cutoff

	i1=0;
	for (Int_t act_Lep =0; act_Lep < nLep; act_Lep++) {
	    i1++;
	    
	    i2=0;    
	    for (Int_t act_Lep2 =act_Lep; act_Lep2 < nLep; act_Lep2++) {
		i2++;	

		if ( nEmuDiLeptons < fTrigParDil->getMUcutoff() ) { // MU cutoff
		    
		    if (flag[act_Lep] != flag[act_Lep2]) { // unlike sign pairs
			
			pp1.setX( sin(thetaRich[act_Lep]* TMath::Pi()/180)*cos(phiRich[act_Lep]* TMath::Pi()/180));
			pp1.setY( sin(thetaRich[act_Lep]* TMath::Pi()/180)*sin(phiRich[act_Lep]* TMath::Pi()/180));
			pp1.setZ( cos(thetaRich[act_Lep]* TMath::Pi()/180) );
			
			pp2.setX( sin(thetaRich[act_Lep2]* TMath::Pi()/180)*cos(phiRich[act_Lep2]* TMath::Pi()/180) );
			pp2.setY( sin(thetaRich[act_Lep2]* TMath::Pi()/180)*sin(phiRich[act_Lep2]* TMath::Pi()/180) );
			pp2.setZ( cos(thetaRich[act_Lep2]* TMath::Pi()/180) );
			
			Float_t cOpening = pp1.scalarProduct(pp2);
			Float_t openAngle=acos(cOpening);
			
			//emudileptons
			HMUEMUDiLeptons & dilepton = pEmuDiLeptons[nEmuDiLeptons]; 
			
			dilepton.opangle = openAngle * 180/TMath::Pi();
			dilepton.fMass = 2 * mom[act_Lep] * mom[act_Lep2] * (1-cOpening) ; // michael formula (mass squared)
			dilepton.iN1 = i1;
			dilepton.iN2 = i2;
			
			nEmuDiLeptons++;
			
		    }
		}	
	    }      
	}    	
    }
    return nEmuDiLeptons;  
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











