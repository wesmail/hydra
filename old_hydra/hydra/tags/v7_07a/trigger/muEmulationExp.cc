//*-- Author: Alberica Toia
//*-- Modified: Joern Wuestenfeld 05/13/2005
//*--Version: $Id: muEmulationExp.cc,v 1.6 2005-08-25 14:36:03 wuestenf Exp $

#include "muEmulationExp.h"
#include "hdebug.h"
#include "hades.h"
#include "hcategory.h"
#include "hiterator.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "hevent.h"
#include "hrichhitIPU.h"
#include "htofhit.h"
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
#include "hmessagemgr.h"
#include "htriggerparshowermap.h"

#include "TArrayI.h"
#include "TArrayF.h"
#include "TObjArray.h"

//
// HMuEmulationExp
//
// Matching unit emulation code.
// This class is to be used for real data that does not contains the matching unit
// subevent.
//

HMUEmulationExp::HMUEmulationExp(void)
  : HMUEmulation()
{
  // Default constructor
  //
  // Input parameters:
  //   none
  //
  // Output parameters:
  //   none
  //
  // Return code:
  //   none
  pEmuLeptons=NULL;
  nEmuLeptons=0;
  pRich= NULL;
  pTof= NULL;
  pShower= NULL;
  pLeptons= NULL;
  iterRich=NULL;
  iterTof= NULL;
  iterShower=NULL;
  fRichIPUParThr=NULL;
}

//----------------------------------------------------------------------------
HMUEmulationExp::HMUEmulationExp(Text_t *name,Text_t *title)
              : HMUEmulation(name, title)
{
  // Constructor
  //
  // Iput parameters:
  //   Text_t *name
  //     Name of the object
  //   Text_t *title
  //     Title of the object
  //
  // Output parameters:
  //   none
  //
  // Return code:
  //   none
  pEmuLeptons=NULL;
  nEmuLeptons=0;
  pRich= NULL;
  pTof= NULL;
  pShower= NULL;
  pLeptons= NULL;
  iterRich=NULL;
  iterTof= NULL;
  iterShower=NULL;
  fRichIPUParThr=NULL;
  reset();
}

//----------------------------------------------------------------------------
HMUEmulationExp::~HMUEmulationExp(void)
{
  // Destructor delete the iterators
  //
  // Input parameters:
  //   none
  //
  // Output parameters:
  //   none
  //
  // Return code:
  //   none
  if(pEmuLeptons)
    {
      delete pEmuLeptons;
      pEmuLeptons=NULL;
    }
  nEmuLeptons=0;

  if (iterRich)
    {
      delete iterRich;
      iterRich= NULL;
    }
  if (iterTof)
    {
      delete iterTof;
      iterTof= NULL;
    }
  if (iterShower)
    {
      delete iterShower;
      iterShower= NULL;
    }
  for(Int_t sec=0;sec<6;sec++)
    {
      if(thetaRich[sec])
        {
          delete thetaRich[sec];
          thetaRich[sec] = NULL;
        }
      if(phiRich[sec])
        {
          delete phiRich[sec];
          phiRich[sec] = NULL;
        }
      if(thetaShower[sec])
        {
          delete thetaShower[sec];
          thetaShower[sec] = NULL;
        }
      if(phiShower[sec])
        {
          delete phiShower[sec];
          phiShower[sec] = NULL;
        }
      if(thetaTof[sec])
        {
          delete thetaTof[sec];
          thetaTof[sec] = NULL;
        }
      if(phiTof[sec])
        {
          delete phiTof[sec];
          phiTof[sec] = NULL;
        }
      if(flagTof[sec])
        {
          delete flagTof[sec];
          flagTof[sec] = NULL;
        }
    }
}

//============================================================================

//----------------------------------------------------------------------------
Bool_t HMUEmulationExp::initParContainer(HSpectrometer *spec, HRuntimeDb *rtdb)
{
  // Adds to the runtime database "rtdb" the containers needed by the RICH IPU
  // spec is used to get information of the spectrometer setup.
  //
  // Input parameters:
  //   HSpectrometer *spec
  //     Pointer to the spectrometer setup
  //   HRuntimeDb *rtdb
  //     Pointer to the HADES runtime database
  //
  // Output parameters:
  //   none
  //
  // Return code:
  //   none
  if (!fRichIPUParThr)
    {
      fRichIPUParThr=(HRichIPUParThresholds *)rtdb->getContainer("RichIPUParThresholds");
      if (fRichIPUParThr)
        {
          INFO_msg(10,HMessageMgr::DET_TRIGGER,"RichIPU Parameters Thresholds initialized");
        }
      else
        {
          ERROR_msg(HMessageMgr::DET_TRIGGER,"RichIPU Parameters Thresholds Initialization failed!");
          return kFALSE;
        }
    }

  fRichIPUParLmax=(HRichIPUParLocMax *)rtdb->getContainer("RichIPUParLocMax");
  if (fRichIPUParLmax)
    {
      INFO_msg(10,HMessageMgr::DET_TRIGGER,"RichIPU Parameters LocMax initialized");
    }
  else
    {
      ERROR_msg(HMessageMgr::DET_TRIGGER,"RichIPU Parameters LocMax Initialization failed!");
      return kFALSE;
    }

  fTrigPar=(HTriggerParMuLep *)rtdb->getContainer("TriggerParMuLep");
  if (fTrigPar)
    {
      INFO_msg(10,HMessageMgr::DET_TRIGGER,"Trigger Parameters MU Lepton Emulation initialized");
    }
  else
    {
      ERROR_msg(HMessageMgr::DET_TRIGGER,"Trigger Parameter MU Lepton Emulation Initialization failed!");
      return kFALSE;
  }

  fTrigParMom=(HTriggerParMomMap *)rtdb->getContainer("TriggerParMomMap");
  if (fTrigParMom)
    {
      INFO_msg(10,HMessageMgr::DET_TRIGGER,"Trigger Parameters MU Momentum Mapping initialized");
    }
  else
    {
      ERROR_msg(HMessageMgr::DET_TRIGGER,"Trigger Parameter MU Momentum Mapping Initialization failed!");
      return kFALSE;
    }
    fShowerMap = (HTriggerParShowerMap *)rtdb->getContainer("TriggerParShowerMap");
    if(fShowerMap)
    {
      INFO_msg(10,HMessageMgr::DET_TRIGGER,"Trigger Parameters Shower Mapping initialized");
    }
    else
    {
      ERROR_msg(HMessageMgr::DET_TRIGGER,"Trigger Parameter SHOWER Mapping Initialization failed!");
      return kFALSE;
    }
//   fTriggerSetup = (HTriggerSetup *)rtdb->getContainer("TriggerSetup");
//   if(fTriggerSetup)
//   {
//     INFO_msg(10,HMessageMgr::DET_TRIGGER,"Trigger setup parameters initialized");
//   }
//   else
//   {
//     INFO_msg(10,HMessageMgr::DET_TRIGGER,"Trigger setup parameters initialsisation failed");
//   }

  return kTRUE;
}
//============================================================================


//----------------------------------------------------------------------------
Bool_t HMUEmulationExp::init(void)
{
  // Initialization of taskset
  //
  // Input parameters:
  //   none
  //
  // Output parameters:
  //   none
  //
  // Return code:
  //   kTRUE in case of success otherwise kFALSE.
  SEPERATOR_msg("*",60);
  INFO_msg(10,HMessageMgr::DET_TRIGGER,"INIT of HMUEmulationExp");
  HSpectrometer *spec = gHades->getSetup();
  HRuntimeDb *rtdb = gHades->getRuntimeDb();
  HTriggerDetector * pTrigDet = (HTriggerDetector*)gHades->getSetup()->getDetector("Trigger");
  HEvent *event = gHades->getCurrentEvent();
  
  pEventHeader = event->getHeader();
  
  // initialize parameters
  if (pTrigDet)
    {
      if(!initParContainer(spec,rtdb))
        {
          return kFALSE;
        }
    }
  // allocate input categories
  if((pRich=event->getCategory(catHardRichHit))==NULL) return kFALSE;
  iterRich = (HIterator*)pRich->MakeIterator("native");

  if((pTof=event->getCategory(catTofHit))==NULL) return kFALSE;
  iterTof = (HIterator*)pTof->MakeIterator("native");

  if((pShower=event->getCategory(catShowerHit))==NULL) return kFALSE;
  iterShower = (HIterator*)pShower->MakeIterator("native");

  // allocate output categories
  pLeptons=event->getCategory(catMUEMULeptons);
  if(!pLeptons)
    {
      pLeptons=pTrigDet->buildCategory(catMUEMULeptons);
      if (!pLeptons)
        {
          return kFALSE;
        }
      else
        {
          event->addCategory(catMUEMULeptons, pLeptons, "EmuLeptons");
        }
    }

  pEmuLeptons =new TObjArray(2);  //extra-size array
  SEPERATOR_msg("*",60);
  return kTRUE;
}

//============================================================================
//----------------------------------------------------------------------------
void HMUEmulationExp::prepareInput(void)
{
  // Collect all Information from the categories.
  // Called for each event.
  //
  // Input parameters:
  //   none
  //
  // Output parameters:
  //   none
  //
  // Return code:
  //   none
  Int_t sector;
  Int_t size;
  HRichHitIPU   *pMURich=NULL;
  HTofHit       *pMUTof=NULL;
  HShowerHit    *pMUShower=NULL;

  // ---- RICH ---- //
  iterRich->Reset();
  size = pRich->getEntries();
  for(Int_t sec=0;sec<6;sec++)
    {
      thetaRich[sec]->Set(size);
      phiRich[sec]->Set(size);
      thetaTof[sec]->Set(size);
      phiTof[sec]->Set(size);
    }
  while ((pMURich = (HRichHitIPU *)iterRich->Next()) != 0)
    {
      sector = pMURich->getSector();
      if(nRichHits[sector] < 600)
        {
          thetaRich[sector]->AddAt(pMURich->getTheta(),nRichHits[sector]);
          phiRich[sector]->AddAt(pMURich->getPhi(),nRichHits[sector]);
          nRichHits[sector]++;
        }
      else
        {
          ERROR_msg(HMessageMgr::DET_TRIGGER,"Maximum number of hits in RICH exceeded(600), skipping rest of event!");
          break;
        }
    }    

  // ---- TOF ---- //
  Float_t phi_tof, theta_tof;
  iterTof->Reset(); 
  iterTof->Reset();
  size = pTof->getEntries();
  for(Int_t sec=0;sec<6;sec++)
  {
    thetaTof[sec]->Set(size);
    phiTof[sec]->Set(size);
    flagTof[sec]->Set(size);
  }
  while ((pMUTof = (HTofHit *)iterTof->Next()) != 0)
    {     
      sector = pMUTof->getSector();
      if(nTofHits[sector] < 600)
        {
          pMUTof->getPhi(phi_tof);
          pMUTof->getTheta(theta_tof);
          phiTof[sector]->AddAt(phi_tof,nTofHits[sector]);
          thetaTof[sector]->AddAt(theta_tof,nTofHits[sector]);

          if (pMUTof->getTof() < fTrigPar->getMUtof())
            {
              flagTof[sector]->AddAt(1,nTofHits[sector]);
            }
          nTofHits[sector]++;
        }
      else
        {
          ERROR_msg(HMessageMgr::DET_TRIGGER,"Maximum number of hits in Tof exceeded(600), skipping rest of event!");
          break;
        }
    }

  // ---- SHOWER ---- //
  Float_t phi_shower, theta_shower/*, r_shower*/;
  iterShower->Reset(); 
  size = pShower->getEntries();
  for(Int_t sec=0;sec<6;sec++)
    {
      thetaShower[sec]->Set(size);
      phiShower[sec]->Set(size);
    }
  while ((pMUShower = (HShowerHit *)iterShower->Next()) != 0)
    {
      sector = pMUShower->getSector();
      if(nShowerHits[sector] < 600)
        {
          //pMUShower->getSphereCoord(&r_shower,&phi_shower,&theta_shower);
          phi_shower = fShowerMap->getShowerPhi(pMUShower->getCol(),pMUShower->getRow());
          theta_shower = fShowerMap->getShowerTheta(pMUShower->getCol(),pMUShower->getRow());
          phiShower[sector]->AddAt(phi_shower,nShowerHits[sector]);
          thetaShower[sector]->AddAt(theta_shower,nShowerHits[sector]);
          nShowerHits[sector]++;
        }
      else
        {
          ERROR_msg(HMessageMgr::DET_TRIGGER,"Maximum number of hits in SHOWER exceeded(600), skipping rest of event!");
          break;
        }
    }
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HMUEmulationExp::finalize(void)
{
  // Called at the end of the eventloop to finalize the task.
  // Nothing done here!
  //
  // Input parameters:
  //   none
  // 
  // Output parameters:
  //   none
  //
  // Return code:
  //   Allways returns kTRUE;
return kTRUE;
}
//============================================================================

ClassImp(HMUEmulationExp)
