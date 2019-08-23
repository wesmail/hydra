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

#include <iostream.h>         

HMUDilepEmulation::HMUDilepEmulation() { 
 pEmuDiLeptons=NULL;
 nEmuDiLeptons=0;
}
//============================================================================

//----------------------------------------------------------------------------

HMUDilepEmulation::HMUDilepEmulation(Text_t *name,Text_t *title)
               : HReconstructor(name, title) {
 pEmuDiLeptons=NULL;
 nEmuDiLeptons=0;
}
//============================================================================

//----------------------------------------------------------------------------

HMUDilepEmulation::~HMUDilepEmulation() { 
 pEmuDiLeptons=NULL;
 nEmuDiLeptons=0;

  if (iterHMULeptons) delete iterHMULeptons;
  if (iterHMULeptons2) delete iterHMULeptons2;
  
  // set everything to 0
  // especially the iterators
}

//============================================================================

//----------------------------------------------------------------------------

Bool_t HMUDilepEmulation::init(){
   HTriggerDetector * pTrigDet = (HTriggerDetector*)gHades->getSetup()->getDetector("Trigger");

    // allocate input categories

  ptlHMULeptons=gHades->getCurrentEvent()->getCategory(catMatchULeptons);
  //cout << "input category exists" << endl;
  if (!ptlHMULeptons) return kFALSE;
  else gHades->getCurrentEvent()
	 ->addCategory(catMatchULeptons, ptlHMULeptons, "Leptons");

  iterHMULeptons = (HIterator*)ptlHMULeptons->MakeIterator("native");
  iterHMULeptons2 = (HIterator*)ptlHMULeptons->MakeIterator("native");


// allocate output categories
  ptlHMUEMUDiLeptons=gHades->getCurrentEvent()->getCategory(catMUEMUDiLeptons);
  //cout << "input category exists" << endl;
   if(!ptlHMUEMUDiLeptons){
     //cout << " Build Category " << endl;

     ptlHMUEMUDiLeptons=pTrigDet->buildCategory(catMUEMUDiLeptons);
     if (!ptlHMUEMUDiLeptons){ 
       //cout << "output category does not exist!!! " << endl;     
       return kFALSE;
     }
     else gHades->getCurrentEvent()
	    ->addCategory(catMUEMUDiLeptons, ptlHMUEMUDiLeptons, "EmuDiLeptons");
   }  

   if (pEmuDiLeptons) delete [] pEmuDiLeptons;
   pEmuDiLeptons =new HMUEMUDiLeptons[1000]; //5000 non vuol dire un cazzo
   //showMe->pEmuDiLeptons = pEmuDiLeptons; 


   return kTRUE;
}

//============================================================================

//----------------------------------------------------------------------------

Int_t HMUDilepEmulation::execute() {
  //  cout << "start the execute " << endl;
  Float_t rth1, rph1, p1, eleflag1;
  Float_t rth2, rph2, p2, eleflag2;
  Int_t i1,i2;
  HGeomVector pp1,pp2;
  
  i1=i2=0;
  nEmuDiLeptons=0;
  
  if ( nEmuDiLeptons < 16 ) { // MU cutoff
    
    HMULeptons   *ptlHMULeptons;
    iterHMULeptons->Reset();
    while (( ptlHMULeptons  = (HMULeptons *)iterHMULeptons->Next()) != 0) {  
      
      rth1= ptlHMULeptons->getTheta();
      rph1= ptlHMULeptons->getPhi();
      p1 = (ptlHMULeptons->getMom())/255*1000;
      eleflag1= ptlHMULeptons->getFlag();  
      i1++;
      //      cout <<"1: " << ptlHMULeptons->getTheta() << "  " << ptlHMULeptons->getFlag()
      // 	  << "  " << (ptlHMULeptons->getMom())/255*1000 << endl;
      
      i2=0;    
      HMULeptons   *ptlHMULeptons2;
      iterHMULeptons2->Reset();
      while (( ptlHMULeptons2  = (HMULeptons *)iterHMULeptons2->Next()) != ptlHMULeptons ) {  
	rth2= ptlHMULeptons2->getTheta();
	rph2= ptlHMULeptons2->getPhi();
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
	  
	  //       cout << dilepton.opangle << "   " << dilepton.fMass << endl;
	  
	  nEmuDiLeptons++;
	  
	}
	
      }
      
    }
    
  }

  //cout << "number of leptons " << nEmuLeptons << endl;
  if (nEmuDiLeptons) updateEmuDiLeptons();
  
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
      //cout << " Set emuDiLepton: " << pEmuDiLeptons[i].imass << endl;
    } else {
      //cout << "emuDiLepton = NULL" << endl;
    }
  }
}      
//============================================================================
//----------------------------------------------------------------------------

Bool_t HMUDilepEmulation::finalize(){

 return kTRUE;
}

//============================================================================

//----------------------------------------------------------------------------

ClassImp(HMUDilepEmulation)











