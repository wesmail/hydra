using namespace std;
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

#include <iostream>          
#include <iomanip>

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
  pRich= NULL;
  pTof= NULL;
  pShower= NULL;
  pLeptons= NULL;
  iterRich=NULL;
  iterTof= NULL;
  iterShower=NULL;

  for (int s=0; s<6; s++) {
     nRichHits[s]=0;
     nTofHits[s]=0;
     nShowerHits[s]=0;
     for (int h =0; h<600;h++) {
       thetaRich[s][h]=0;
       phiRich[s][h]=0;
       thetaShower[s][h]=0;
       phiShower[s][h]=0;
       thetaTof[s][h]=0;
       phiTof[s][h]=0;
       flagTof[s][h]=0;
     }
  }
}
//============================================================================

//----------------------------------------------------------------------------
HMUEmulation::HMUEmulation(Text_t *name,Text_t *title)
               : HReconstructor(name, title) {

  fTrigPar = NULL;
  fTrigParMom = NULL;
  pEmuLeptons=NULL;
  nEmuLeptons=0;
  pRich= NULL;
  pTof= NULL;
  pShower= NULL;
  pLeptons= NULL;
  iterRich=NULL;
  iterTof= NULL;
  iterShower=NULL;
  for (int s=0; s<6; s++) {
     nRichHits[s]=0;
     nTofHits[s]=0;
     nShowerHits[s]=0;
     for (int h =0; h<600;h++) {
       thetaRich[s][h]=0;
       phiRich[s][h]=0;
       thetaShower[s][h]=0;
       phiShower[s][h]=0;
       thetaTof[s][h]=0;
       phiTof[s][h]=0;
       flagTof[s][h]=0;
     }
  }
}
//============================================================================

//----------------------------------------------------------------------------
HMUEmulation::~HMUEmulation() { 
  // set everything to 0
  // especially the iterators

  pEmuLeptons=NULL;
  nEmuLeptons=0;

  if (pRich) delete pRich;
  pRich= NULL;
  if (pTof) delete pTof;
  pTof= NULL;
  if (pShower) delete pShower;
  pShower= NULL;
  if (pLeptons) delete pLeptons; 
  pLeptons= NULL;
  
  if (iterRich) delete iterRich;
  iterRich=NULL;
  if (iterTof) delete iterTof;
  iterTof= NULL;
  if (iterShower) delete iterShower;
  iterShower=NULL;

  for (int s=0; s<6; s++) {
     nRichHits[s]=0;
     nTofHits[s]=0;
     nShowerHits[s]=0;
     for (int h =0; h<600;h++) {
       thetaRich[s][h]=0;
       phiRich[s][h]=0;
       thetaShower[s][h]=0;
       phiShower[s][h]=0;
       thetaTof[s][h]=0;
       phiTof[s][h]=0;
       flagTof[s][h]=0;
     }
  }
}

//============================================================================

//----------------------------------------------------------------------------
Bool_t HMUEmulation::init(){
  cout << "-------------- INIT of HMUEmulation() ------------------- " << endl;
  HSpectrometer *spec = gHades->getSetup();
  HRuntimeDb *rtdb = gHades->getRuntimeDb();
  HTriggerDetector * pTrigDet = (HTriggerDetector*)gHades->getSetup()->getDetector("Trigger");

  // initialize parameters
  if (pTrigDet)
    initParContainer(spec,rtdb);
  
  // allocate input categories
  pRich=gHades->getCurrentEvent()->getCategory(catMatchURich);
  if (!pRich) return kFALSE;
  else gHades->getCurrentEvent()
	 ->addCategory(catMatchURich, pRich, "Rich");  
  iterRich = (HIterator*)pRich->MakeIterator("native");

  
  pTof=gHades->getCurrentEvent()->getCategory(catMatchUTof);
  if (!pTof) return kFALSE;
  else gHades->getCurrentEvent()
	 ->addCategory(catMatchUTof, pTof, "Tof");  
  iterTof = (HIterator*)pTof->MakeIterator("native");

  
  pShower=gHades->getCurrentEvent()->getCategory(catMatchUShower);
  if (!pShower) return kFALSE;
  else gHades->getCurrentEvent()
	 ->addCategory(catMatchUShower, pShower, "Shower");
  iterShower = (HIterator*)pShower->MakeIterator("native");

  
  // allocate output categories
  pLeptons=gHades->getCurrentEvent()->getCategory(catMUEMULeptons);
  if(!pLeptons){
    pLeptons=pTrigDet->buildCategory(catMUEMULeptons);
    if (!pLeptons){ 
      return kFALSE;
    }
    else gHades->getCurrentEvent()
	   ->addCategory(catMUEMULeptons, pLeptons, "EmuLeptons");
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

  for (int s=0; s<6; s++) {
     nRichHits[s]=0;
     nTofHits[s]=0;
     nShowerHits[s]=0;
     for (int h =0; h<600;h++) {
       thetaRich[s][h]=0;
       phiRich[s][h]=0;
       thetaShower[s][h]=0;
       phiShower[s][h]=0;
       thetaTof[s][h]=0;
       phiTof[s][h]=0;
       flagTof[s][h]=0;
     }
  }
  prepareInput();
  matching();
  if (nEmuLeptons) updateEmuLeptons();
  return 0;
}
//============================================================================

//----------------------------------------------------------------------------
Int_t HMUEmulation::matching() { 
  Float_t fphiReg_Rich;
  Int_t   iphiReg_Rich;
  Float_t fRich_phi,fTof_phi,fShower_phi;
  Float_t fRich_theta,fTof_theta,fShower_theta;
  Int_t   iRich_phi,iTof_phi,iShower_phi;
  Int_t   iRich_theta,iTof_theta,iShower_theta;
  fphiReg_Rich = fRich_phi = fTof_phi = fShower_phi=0;
  iphiReg_Rich = iRich_phi = iTof_phi = iShower_phi=0;
  iRich_theta = iTof_theta = iShower_theta=0;
  
  Float_t a,b,c;
  Float_t fmom;
  Long_t imom;
  ULong_t mom;
  a=b=c=mom=0;
  Float_t low_cut, high_cut;
  Float_t field_current = fTrigPar->getMUfield();
  low_cut = 20.0 * field_current / 3465.0; 
  high_cut = 1000.0 * field_current / 3465.0; 
  
  nEmuLeptons=0;
  
  for (Int_t sec =0; sec <6; sec++) { 

    for (Int_t actRich=0; actRich < nRichHits[sec]; actRich++) { // rich loop    

      fRich_theta = thetaRich[sec][actRich];    
      fRich_phi = phiRich[sec][actRich];    
      //cout << "RICH " << fRich_theta << " " << fRich_phi << endl;  
    
      if (fRich_theta>0.1) { // valid range of mapped rings
	
	iRich_theta = roundAngle(fRich_theta,90);    
	iRich_phi = roundAngle(fRich_phi,60);    
	
	fphiReg_Rich = fabs(fRich_phi-30);
	iphiReg_Rich = roundAngle(fphiReg_Rich,60,0.);
	// !!!!!! problem is the round: +0. or +0.5???
	// for the matching window +0. is ok, but for the momentum +0.5 seems to be better...
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!#
	
	for (Int_t actTof=0; actTof < nTofHits[sec]; actTof++) { // tof loop    

	  if (flagTof[sec][actTof]==1){ // lepton-like tof
	    
	    fTof_theta = thetaTof[sec][actTof];    
	    fTof_phi = phiTof[sec][actTof];    
	    //cout << "TOF " << fTof_theta << " " << fTof_phi << endl;
	    
	    iTof_theta = roundAngle(fTof_theta,90);    
	    iTof_phi = roundAngle(fTof_phi,60);    
	    
	    if (  (  int(fabs((Float_t) (iRich_phi - iTof_phi)))   <
                     ( fTrigPar->getMUphiOffset() + int(iphiReg_Rich/fTrigPar->getMUphiSlope()) )  )
		  && ( int(fabs((Float_t)(iRich_theta - iTof_theta))) < fTrigPar->getMUtheta() )  ) {
	      //cout << "MATCH!" << endl;
	      //emuleptons
	      HMUEMULeptons & lepton = pEmuLeptons[nEmuLeptons]; 
	      
	      lepton.iDetBit     = 0;
	      lepton.fThetaRich  = fRich_theta;
	      lepton.fPhiRich    = fRich_phi;
	      lepton.fThetaMeta  = fTof_theta;
	      lepton.fPhiMeta    = fTof_phi;
	      lepton.iSector     = sec;
	      
	      // re-binning of variables according to the momentum table
	      a=(float(iRich_theta)/255.)*(fTrigParMom->getTheta_bins());
	      b=(float(fabs((Float_t)(iRich_theta-iTof_theta)))/255.)*(fTrigParMom->getDeltatheta_bins());
	      c=(float(iphiReg_Rich)/128)*(fTrigParMom->getPhi_bins());
	      
//   	      cout << "fRich_theta " << fRich_theta << " iRich_theta " << iRich_theta 
//   		   << " fRich_phi " << fRich_phi << " iRich_phi " << iRich_phi << endl; 
//   	      cout << "fTof_theta " << fTof_theta << " iTof_theta " << iTof_theta 
//   		   << " fTof_phi " << fTof_phi << " iTof_phi " << iTof_phi << endl; 
//   	      cout << "phi region " << fphiReg_Rich << " " << iphiReg_Rich << endl;	      
//  	      cout << "abc " << (int)a << " " << (int)b << " "<< (int)c ;

	      fmom        = fTrigParMom->getMom((int)a,(int)b,(int)c);
	      fmom = fmom * field_current / 3465.0;
	      if (fmom > 0.1) { 	 
		if (fmom < low_cut)  imom = 1; 	 
	        else if (fmom > high_cut)  imom = 255; 	 
		else {
		  imom = int(255 * fmom / (high_cut) + 0.5);
		  if (imom > 255)  imom = 255; 	 
	        }
		mom        = imom;
	      } else {
		mom        = 0;
	      }
	      lepton.fMom  = mom;
	      //cout << " " << mom << endl;
	      if ((iRich_theta - iTof_theta) < 0) 
		lepton.iFlag = 1;  //electrons;
	      else  if ((iRich_theta - iTof_theta) > 0) 
		lepton.iFlag = 0;  //positrons;
	      
	      nEmuLeptons++;	      
	    }
	  }	  
	}
	
	for (Int_t actShower=0; actShower < nShowerHits[sec]; actShower++) { // shower loop          

	  fShower_theta = thetaShower[sec][actShower];    
	  fShower_phi = phiShower[sec][actShower];    
	  
	  iShower_theta = roundAngle(fShower_theta,90);    
	  iShower_phi = roundAngle(fShower_phi,60);    
	  
          if (  (  int(fabs((Float_t)(iRich_phi - iShower_phi)))   <
		   ( fTrigPar->getMUphiOffset() + int(iphiReg_Rich/fTrigPar->getMUphiSlope()) )  )
		&& ( int(fabs((Float_t)(iRich_theta - iShower_theta))) < fTrigPar->getMUtheta() )  ) {  

	    //emuleptons 
	    HMUEMULeptons & lepton = pEmuLeptons[nEmuLeptons]; 	
	    
	    lepton.iDetBit     = 1;
	    lepton.fThetaRich  = fRich_theta;
	    lepton.fPhiRich    = fRich_phi;
	    lepton.fThetaMeta  = fShower_theta;
	    lepton.fPhiMeta    = fShower_phi;
	    lepton.iSector     = sec;
	    
	    a=(float(iRich_theta)/255.)*(fTrigParMom->getTheta_bins());
	    b=(float(fabs((Float_t)(iRich_theta-iShower_theta)))/255.)*(fTrigParMom->getDeltatheta_bins());
	    c=(float(iphiReg_Rich)/128)*(fTrigParMom->getPhi_bins());
 
//  	    cout << "fRich_theta " << fRich_theta << " iRich_theta " << iRich_theta 
//   		 << " fRich_phi " << fRich_phi << " iRich_phi " << iRich_phi << endl; 
//   	    cout << "fShower_theta " << fShower_theta << " iShower_theta " << iShower_theta 
//   		 << " fShower_phi " << fShower_phi << " iShower_phi " << iShower_phi << endl; 
//   	    cout << "phi region " << fphiReg_Rich << " " << iphiReg_Rich << endl;	      
//  	    cout << "abc " << (int)a << " " << (int)b << " "<< (int)c;
  
 	    
	    fmom        = fTrigParMom->getMom((int)a,(int)b,(int)c);
	    fmom = fmom * field_current / 3465.0;
	    if (fmom > 0.1) {
	      if (fmom < low_cut)  imom = 1; 	 
	      else if (fmom > high_cut)  imom = 255; 	 
	      else {
		imom = 255 * fmom / (high_cut) + 0.5;
		if (imom > 255)  imom = 255; 	 
	      }
	      mom        = imom;
	    } else {
	      mom        = 0;
	    }
	    lepton.fMom  = mom;
	    //cout << " " << mom << endl;
	    if ((iRich_theta - iShower_theta) < 0) 
	      lepton.iFlag = 1;    //electrons;
	    else  if ((iRich_theta - iShower_theta) > 0) 
	      lepton.iFlag = 0;  //positrons;
	    
	    nEmuLeptons++;
	    
	  }	  
	}    
      }
    }
  }
  return nEmuLeptons;  
}
//============================================================================
//----------------------------------------------------------------------------
void HMUEmulation::updateEmuLeptons() {
  
  HMUEMULeptons *emuLepton=NULL; 
      
  for (int i = 0; i < nEmuLeptons; i++) {
    loc.set(1, 0);    
    emuLepton=(HMUEMULeptons *)pLeptons->getNewSlot(loc);
    
    if (emuLepton!=NULL) {
      emuLepton=new(emuLepton) HMUEMULeptons;      
      *emuLepton = pEmuLeptons[i];
    } 
  }
}      
//============================================================================

//----------------------------------------------------------------------------
void HMUEmulation::prepareInput() {
  Int_t sector;
  HMatchURich   *pMURich=NULL;
  HMatchUTof    *pMUTof=NULL;
  HMatchUShower *pMUShower=NULL;

  // ---- RICH ---- //
  iterRich->Reset(); 
  while ((pMURich = (HMatchURich *)iterRich->Next()) != 0) {     
      sector = pMURich->getSector();
      phiRich[sector][nRichHits[sector]] = pMURich->getPhi();
      if (sector<5)
 	phiRich[sector][nRichHits[sector]] -= 60*(sector+1);
      thetaRich[sector][nRichHits[sector]] = pMURich->getTheta();
      nRichHits[sector]++;
  }
  // ---- TOF ---- //
  iterTof->Reset(); 
  while ((pMUTof = (HMatchUTof *)iterTof->Next()) != 0) {     
    sector = pMUTof->getSector();
    phiTof[sector][nTofHits[sector]] = pMUTof->getPhi();
    if (sector<5)
      phiTof[sector][nTofHits[sector]] -= 60*(sector+1);
    thetaTof[sector][nTofHits[sector]] = pMUTof->getTheta();
    flagTof[sector][nTofHits[sector]] = pMUTof->getPID();
    nTofHits[sector]++;
  }
  // ---- SHOWER ---- //
  iterShower->Reset(); 
  while ((pMUShower = (HMatchUShower *)iterShower->Next()) != 0) {     
      sector = pMUShower->getSector();
      phiShower[sector][nShowerHits[sector]] = pMUShower->getPhi();
      if (sector<5)
	phiShower[sector][nShowerHits[sector]] -= 60*(sector+1);
      thetaShower[sector][nShowerHits[sector]] = pMUShower->getTheta();
      nShowerHits[sector]++;
  }

}      
//============================================================================

//----------------------------------------------------------------------------
Bool_t HMUEmulation::finalize(){
 return kTRUE;
}
//============================================================================

ClassImp(HMUEmulation)





  





