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
  ptlHMatchURich= NULL;
  ptlHMatchUTof= NULL;
  ptlHMatchUShower= NULL;
  ptlHMUEMULeptons= NULL;
  iterHMatchURich=NULL;
  iterHMatchUTof= NULL;
  iterHMatchUShower=NULL;

}
//============================================================================

//----------------------------------------------------------------------------
HMUEmulation::HMUEmulation(Text_t *name,Text_t *title)
               : HReconstructor(name, title) {

  fTrigPar = NULL;
  fTrigParMom = NULL;
  pEmuLeptons=NULL;
  nEmuLeptons=0;
  ptlHMatchURich= NULL;
  ptlHMatchUTof= NULL;
  ptlHMatchUShower= NULL;
  ptlHMUEMULeptons= NULL;
  iterHMatchURich=NULL;
  iterHMatchUTof= NULL;
  iterHMatchUShower=NULL;

}
//============================================================================

//----------------------------------------------------------------------------
HMUEmulation::~HMUEmulation() { 
  // set everything to 0
  // especially the iterators

  pEmuLeptons=NULL;
  nEmuLeptons=0;

  if (ptlHMatchURich) delete ptlHMatchURich;
  ptlHMatchURich= NULL;
  if (ptlHMatchUTof) delete ptlHMatchUTof;
  ptlHMatchUTof= NULL;
  if (ptlHMatchUShower) delete ptlHMatchUShower;
  ptlHMatchUShower= NULL;
  if (ptlHMUEMULeptons) delete ptlHMUEMULeptons; 
  ptlHMUEMULeptons= NULL;
  
  if (iterHMatchURich) delete iterHMatchURich;
  iterHMatchURich=NULL;
  if (iterHMatchUTof) delete iterHMatchUTof;
  iterHMatchUTof= NULL;
  if (iterHMatchUShower) delete iterHMatchUShower;
  iterHMatchUShower=NULL;

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
  
  pEmuLeptons =new HMUEMULeptons[1000];  //extra-size array
  
  return kTRUE;
}

//============================================================================

//----------------------------------------------------------------------------
Int_t HMUEmulation::roundAngle(Float_t fAngle, Int_t iRange, Float_t round=0.5) {
// the Matching Unit uses an internal mapping made of integers in the range (0,255)

  Int_t iAngle =0;
  iAngle = int(fAngle *255./iRange +round); 
  if (iAngle > 255) iAngle =255;
  if (iAngle < 0) iAngle =0;

  return iAngle;
}
//============================================================================

//----------------------------------------------------------------------------
Int_t HMUEmulation::execute() { 

  HMatchURich   *pMURich=NULL;
  HMatchUTof    *pMUTof=NULL;
  HMatchUShower *pMUShower=NULL;
  
  Float_t fphiReg_Rich;
  Int_t   iphiReg_Rich;
  Float_t fRich_phi,fTof_phi,fShower_phi;
  Int_t   iRich_phi,iTof_phi,iShower_phi;
  Int_t   iRich_theta,iTof_theta,iShower_theta;
  fphiReg_Rich = fRich_phi = fTof_phi = fShower_phi=0;
  iphiReg_Rich = iRich_phi = iTof_phi = iShower_phi=0;
  iRich_theta = iTof_theta = iShower_theta=0;

  Float_t a,b,c;
  a=b=c=0;
  
  nEmuLeptons=0;
  

  iterHMatchURich->Reset(); 
  while ((pMURich = (HMatchURich *)iterHMatchURich->Next()) != 0) {     
        
    if (pMURich->getTheta()>0.1) { // valid range of mapped rings
	
      fRich_phi = pMURich->getPhi();
      if (pMURich->getSegmentId()<5)
	fRich_phi -= 60*(pMURich->getSegmentId()+1);

      iRich_theta = roundAngle(pMURich->getTheta(),90);    
      iRich_phi = roundAngle(fRich_phi,60);    
      
      fphiReg_Rich = fabs(fRich_phi-30);
      iphiReg_Rich = roundAngle(fphiReg_Rich,60,0.);


      iterHMatchUTof->Reset();    
      while ((pMUTof = (HMatchUTof *)iterHMatchUTof->Next()) != 0) {
	
	fTof_phi = pMUTof->getPhi();
	if (pMUTof->getSector()<5)
	  fTof_phi -= 60*(pMUTof->getSector()+1);
	
	if (pMURich->getSegmentId()==pMUTof->getSector()) {
	  if(pMUTof->getPID() ==1){
	    
 	    iTof_theta = pMUTof->getiTheta();  
 	    iTof_phi = pMUTof->getiPhi();

 	    if (  (  int(fabs( iRich_phi - iTof_phi))   <  
		     ( fTrigPar->getMUphiOffset() + int(iphiReg_Rich/fTrigPar->getMUphiSlope()) )  ) 
	    	      && ( int(fabs(iRich_theta - iTof_theta)) < fTrigPar->getMUtheta() )  )  {

	      
	      //emuleptons
	      HMUEMULeptons & lepton = pEmuLeptons[nEmuLeptons]; 
	      
	      lepton.iDetBit     = 0;
	      lepton.fThetaRich  = pMURich->getTheta() ;
	      lepton.fPhiRich    = pMURich->getPhi();
	      lepton.fThetaMeta  = pMUTof->getTheta() ;
	      lepton.fPhiMeta    = pMUTof->getPhi();
	      lepton.iSector     = pMURich->getSegmentId();
	      // re-binning of variables according to the momentum table
	      a=(lepton.fThetaRich)/90*(fTrigParMom->getTheta_bins());
	      b=fabs(lepton.fThetaRich-lepton.fThetaMeta)/90*(fTrigParMom->getDeltatheta_bins());
	      c=fabs((lepton.fPhiMeta)-30)/30*(fTrigParMom->getPhi_bins());
	      lepton.fMom        = fTrigParMom->getMom((int)a,(int)b,(int)c);
	      
	      if ((pMURich->getTheta()-pMUTof->getTheta()) < 0) 
		lepton.iFlag = 1;  //electrons;
	      else  if ((pMURich->getTheta()-pMUTof->getTheta()) > 0) 
		lepton.iFlag = 0;  //positrons;
	      
	      nEmuLeptons++;
	      
	    }
	  }
	}      
      }
      
      iterHMatchUShower->Reset();
      while ((pMUShower = (HMatchUShower *)iterHMatchUShower->Next()) != 0) {
	
	fShower_phi = pMUShower->getPhi();
	if (pMUShower->getSector()<5)
	  fShower_phi -= 60*(pMUShower->getSector()+1);

	if (pMURich->getSegmentId()==pMUShower->getSector()) { 

	    iShower_theta = roundAngle(pMUShower->getTheta(),90);    
	    iShower_phi = roundAngle(fShower_phi,60);    


 	    if (  (  int(fabs( iRich_phi - iShower_phi))   <  
		     ( fTrigPar->getMUphiOffset() + int(iphiReg_Rich/fTrigPar->getMUphiSlope()) )  ) 
	    	      && ( int(fabs(iRich_theta - iShower_theta)) < fTrigPar->getMUtheta() )  )  {

	    
	    //emuleptons 
	    HMUEMULeptons & lepton = pEmuLeptons[nEmuLeptons]; 	
	    
	    lepton.iDetBit     = 1;
	    lepton.fThetaRich  = pMURich->getTheta() ;
	    lepton.fPhiRich    = pMURich->getPhi();
	    lepton.fThetaMeta  = pMUShower->getTheta() ;
	    lepton.fPhiMeta    = pMUShower->getPhi();
	    lepton.iSector     = pMURich->getSegmentId();
	    
	    a=(lepton.fThetaRich)/90*(fTrigParMom->getTheta_bins());
	    b=fabs(lepton.fThetaRich-lepton.fThetaMeta)/90*(fTrigParMom->getDeltatheta_bins());
	    c=fabs((lepton.fPhiMeta)-30)/30*(fTrigParMom->getPhi_bins());
	    lepton.fMom        = fTrigParMom->getMom((int)a,(int)b,(int)c);
	    
	    
	    if ((pMURich->getTheta()-pMUShower->getTheta()) < 0) 
	      lepton.iFlag = 1;    //electrons;
	    else  if ((pMURich->getTheta()-pMUShower->getTheta()) > 0) 
	      lepton.iFlag = 0;  //positrons;
	    
	    nEmuLeptons++;
	    
	  }
	}
      }    
    }
  }
  
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





  





