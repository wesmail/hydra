#include "muEmulationSim.h"
#include "hdebug.h"
#include "hades.h"
#include "hcategory.h"
#include "hiterator.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "hevent.h"
#include "hrichhitIPUSim.h"
#include "htofhitsim.h"
#include "hshowerhit.h"
#include "hmatrixcatiter.h"
#include "triggerinfodef.h"
#include "htriggerdetector.h"
#include "tofdef.h"
#include "showerdef.h"
#include "hlocation.h"
#include "htriggerparmulep.h"
#include "htriggerparmommap.h"
#include "hrichIPUparthresholds.h"
#include "hrichIPUparlocmax.h"
#include <iostream.h>         

HMUEmulationSim::HMUEmulationSim() { 

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

//----------------------------------------------------------------------------
HMUEmulationSim::HMUEmulationSim(Text_t *name,Text_t *title)
               : HMUEmulation(name, title) {

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

//----------------------------------------------------------------------------
HMUEmulationSim::~HMUEmulationSim() { 
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
void HMUEmulationSim::initParContainer(HSpectrometer *spec, HRuntimeDb *rtdb) {
  // Adds to the runtime database "rtdb" the containers needed by the RICH IPU
  //spec is used to get information of the spectrometer setup.

  if (!fRichIPUParThr) {
    fRichIPUParThr=(HRichIPUParThresholds *)rtdb->getContainer("RichIPUParThresholds");
    if (fRichIPUParThr) {
      cout << "RichIPU Parameters Thresholds initialized" << endl;
    } else {
      cout << "******  RichIPU Parameters Thresholds Initialization failed!! *********" << endl;
    }
  }  

  fRichIPUParLmax=(HRichIPUParLocMax *)rtdb->getContainer("RichIPUParLocMax");
  if (fRichIPUParLmax) {
    cout << "RichIPU Parameters LocMax initialized" << endl;
  } else {
    cout << "******  RichIPU Parameters LocMax Initialization failed!! *********" << endl;
  }

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
//============================================================================


//----------------------------------------------------------------------------
Bool_t HMUEmulationSim::init(){
  cout << "-------------- INIT of HMUEmulationSim() ------------------- " << endl;
  HSpectrometer *spec = gHades->getSetup();
  HRuntimeDb *rtdb = gHades->getRuntimeDb();
  HTriggerDetector * pTrigDet = (HTriggerDetector*)gHades->getSetup()->getDetector("Trigger");
  
  // initialize parameters
  if (pTrigDet)
    initParContainer(spec,rtdb);
  
  // allocate input categories
  pRich=gHades->getCurrentEvent()->getCategory(catHardRichHit);
  if (!pRich) {
    return kFALSE;
  }
  else gHades->getCurrentEvent()
	 ->addCategory(catHardRichHit, pRich, "Rich");  
  iterRich = (HIterator*)pRich->MakeIterator("native");
  
  pTof=gHades->getCurrentEvent()->getCategory(catTofHit);
  if (!pTof) return kFALSE;
  else gHades->getCurrentEvent()
	 ->addCategory(catTofHit, pTof, "Tof");  
  iterTof = (HIterator*)pTof->MakeIterator("native");
  
  pShower=gHades->getCurrentEvent()->getCategory(catShowerHit);
  if (!pShower) return kFALSE;
  else gHades->getCurrentEvent()
	 ->addCategory(catShowerHit, pShower, "Shower");
  iterShower = (HIterator*)pShower->MakeIterator("native");

  // allocate output categories
  pLeptons=gHades->getCurrentEvent()->getCategory(catMUEMULeptons);
  if(!pLeptons){
    pLeptons=pTrigDet->buildLinearCat("HMUEMULeptonsSim");
    if (!pLeptons) return kFALSE;
    else gHades->getCurrentEvent()
	   ->addCategory(catMUEMULeptons, pLeptons, "EmuLeptons");
  }  

  pEmuLeptons =new HMUEMULeptons[1000];  //extra-size array

  return kTRUE;
}

//============================================================================

//----------------------------------------------------------------------------
Int_t HMUEmulationSim::execute() { 
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
 HMUEmulation::matching();
 if (nEmuLeptons) HMUEmulation::updateEmuLeptons();
 return 0;  
}
//============================================================================

//----------------------------------------------------------------------------
void HMUEmulationSim::prepareInput() {
  Int_t sector;
  HRichHitIPUSim   *pMURich=NULL;
  HTofHitSim       *pMUTof=NULL;
  HShowerHit       *pMUShower=NULL;

  // ---- RICH ---- //
  Int_t yRichHitIPU, xRichHitIPU;
  Int_t RingsumPattern[6][96][96];
  for (Int_t s=0;s<6;s++) 
    for (Int_t r=0;r<96;r++)
      for(Int_t c=0;c<96;c++)
	RingsumPattern[s][r][c]= 0;
  
  iterRich->Reset();
  while ((pMURich = (HRichHitIPUSim *)iterRich->Next()) != 0) {
    sector = pMURich->getSector();

    if( (pMURich->getQuality()    > fRichIPUParThr->getThresholds(0,sector)) &&
	(pMURich->getVetoQuality()< -fRichIPUParThr->getThresholds(1,sector))) {
      yRichHitIPU = pMURich->getY();
      xRichHitIPU = pMURich->getX();
      RingsumPattern[sector][xRichHitIPU][yRichHitIPU]=pMURich->getQuality();
    }
  }

  iterRich->Reset();
  while ((pMURich = (HRichHitIPUSim *)iterRich->Next()) != 0 ) {
    Int_t fulfill=0;
    sector = pMURich->getSector();
    yRichHitIPU = pMURich->getY();
    xRichHitIPU = pMURich->getX();
    if (RingsumPattern[sector][xRichHitIPU][yRichHitIPU] ){
      Int_t CurrentSum = pMURich->getQuality();

      if  (fRichIPUParLmax->getLocMaxFlag()==1) {
	if ( (fRichIPUParLmax->getLocMax()==4)||(fRichIPUParLmax->getLocMax()==8) )  {
	  if ( CurrentSum  >= RingsumPattern[sector][xRichHitIPU+1][yRichHitIPU] &&
	       CurrentSum  >= RingsumPattern[sector][xRichHitIPU][yRichHitIPU+1] &&
	       CurrentSum  >  RingsumPattern[sector][xRichHitIPU-1][yRichHitIPU] &&
	       CurrentSum  >  RingsumPattern[sector][xRichHitIPU][yRichHitIPU-1] ) {
	    
	    if (fRichIPUParLmax->getLocMax()==8) {
	      if ( CurrentSum  >= RingsumPattern[sector][xRichHitIPU+1][yRichHitIPU+1] &&
		   CurrentSum  >= RingsumPattern[sector][xRichHitIPU-1][yRichHitIPU+1] &&
		   CurrentSum  >  RingsumPattern[sector][xRichHitIPU-1][yRichHitIPU-1] &&
		   CurrentSum  >  RingsumPattern[sector][xRichHitIPU+1][yRichHitIPU-1] ) {
		fulfill=1;
	      }
	    } else {
	      fulfill=1;	    
	    }	      
	  }
	} else {
	  cout << " wrong parameter value!!!" << endl;
	}
      } else {
	fulfill=1;
      }

      if (fulfill) {
	thetaRich[sector][nRichHits[sector]] = pMURich->getTheta();
	phiRich[sector][nRichHits[sector]] = pMURich->getPhi();
	nRichHits[sector]++;  
      }    
    }
  }   

  // ---- TOF ---- //
  Float_t phi_tof, theta_tof;
  iterTof->Reset(); 
  while ((pMUTof = (HTofHitSim *)iterTof->Next()) != 0) {     
    sector = pMUTof->getSector();
    pMUTof->getPhi(phi_tof);
    pMUTof->getTheta(theta_tof);
    phiTof[sector][nTofHits[sector]] = phi_tof;
    thetaTof[sector][nTofHits[sector]] = theta_tof;

    if (pMUTof->getTof() < fTrigPar->getMUtof())
      flagTof[sector][nTofHits[sector]] = 1;
    nTofHits[sector]++;
  }

  // ---- SHOWER ---- //
  Float_t phi_shower, theta_shower, r_shower;
  iterShower->Reset(); 
  while ((pMUShower = (HShowerHit *)iterShower->Next()) != 0) {     
      sector = pMUShower->getSector();
      pMUShower->getSphereCoord(&r_shower,&phi_shower,&theta_shower);
      phiShower[sector][nShowerHits[sector]] = phi_shower;
      thetaShower[sector][nShowerHits[sector]] = theta_shower;
      nShowerHits[sector]++;
  }

}      
//============================================================================

//----------------------------------------------------------------------------
Bool_t HMUEmulationSim::finalize(){
 return kTRUE;
}
//============================================================================

ClassImp(HMUEmulationSim)





  





