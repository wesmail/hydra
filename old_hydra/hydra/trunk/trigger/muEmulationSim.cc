//*-- Author: Alberica Toia
//*-- Modified: Joern Wuestenfeld 05/13/2005
//*-- Version: $Id: muEmulationSim.cc,v 1.10 2009-07-15 11:34:06 halo Exp $

#include "muEmulationSim.h"
#include "hades.h"
#include "hcategory.h"
#include "hiterator.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "hevent.h"
#include "heventheader.h"
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
#include "htriggerparshowermap.h"
#include "hmessagemgr.h"

#include "TArrayI.h"
#include "TArrayF.h"
#include "TObjArray.h"

//
// HMuEmulationSim
//
// Matching unit emulation code.
// This class is to be used for simulation data.
// It uses for the SHOWER hit the real geometrical phi coordinate.
// Not the one that the matching unit gets via a lookup table! (BUG?)
//

HMUEmulationSim::HMUEmulationSim()
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
  nEmuLeptons=0;
  pRich= NULL;
  pTof= NULL;
  pShower= NULL;
  pLeptons= NULL;
  iterRich=NULL;
  iterTof= NULL;
  iterShower=NULL;
  reset();
}

//----------------------------------------------------------------------------
HMUEmulationSim::HMUEmulationSim(const Text_t *name,const Text_t *title)
              : HMUEmulation(name, title)
{
  // Default constructor
  //
  // Input parameters:
  //   Text_t *name
  //      name of the object
  //   Text_t *title
  //      title of the object
  //
  // Output parameters:
  //   none
  //
  // Return code:
  //   none
  nEmuLeptons=0;
  pRich= NULL;
  pTof= NULL;
  pShower= NULL;
  pLeptons= NULL;
  iterRich=NULL;
  iterTof= NULL;
  iterShower=NULL;
  reset();
}

//----------------------------------------------------------------------------
HMUEmulationSim::~HMUEmulationSim()
{
  // Destructor, deletes all internaly created objects.
  //
  // Input parameters:
  //   none
  //
  // Output parameters:
  //   none
  //
  // Return code:
  //    none
  if(pEmuLeptons)
    {
      delete pEmuLeptons;
      pEmuLeptons=NULL;
    }
  nEmuLeptons=0;

  pRich= NULL;
  pTof= NULL;
  pShower= NULL;
  pLeptons= NULL;
  if (iterRich)
    {
      delete iterRich;
      iterRich=NULL;
    }
  if (iterTof)
    {
      delete iterTof;
      iterTof= NULL;
    }
  if (iterShower)
    {
      delete iterShower;
      iterShower=NULL;
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
Bool_t HMUEmulationSim::initParContainer(HSpectrometer *spec, HRuntimeDb *rtdb)
{
  // Adds to the runtime database "rtdb" the containers needed by the RICH IPU
  //spec is used to get information of the spectrometer setup.
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
  // Return code
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
Bool_t HMUEmulationSim::init(void)
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
  if((pRich=event->getCategory(catHardRichHit))==NULL)
  {
    ERROR_msg(HMessageMgr::DET_TRIGGER,"Category HRichHit not found!");
    return kFALSE;
  }
  iterRich = (HIterator*)pRich->MakeIterator("native");

  if((pTof=event->getCategory(catTofHit))==NULL)
  {
    ERROR_msg(HMessageMgr::DET_TRIGGER,"Category HTofHit not found!");
    return kFALSE;
    }
  iterTof = (HIterator*)pTof->MakeIterator("native");

  if((pShower=event->getCategory(catShowerHit))==NULL)
  {
    ERROR_msg(HMessageMgr::DET_TRIGGER,"Category HShowerHit not found!");
    return kFALSE;
  }
  iterShower = (HIterator*)pShower->MakeIterator("native");

  // allocate output categories
  pLeptons=event->getCategory(catMUEMULeptons);
  if(!pLeptons)
    {
      pLeptons=pTrigDet->buildLinearCat("HMUEMULeptonsSim");
      if (!pLeptons)
        {
          return kFALSE;
        }
      else
        {
          event->addCategory(catMUEMULeptons, pLeptons, "EmuLeptons");
        }
    }

  SEPERATOR_msg("*",60);
  return kTRUE;
}

//============================================================================

//----------------------------------------------------------------------------
void HMUEmulationSim::prepareInput(void)
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
#warning "version hardcoded!"
  Int_t version=1;
  Int_t sector;
  Int_t size;
  HRichHitIPUSim   *pMURich=NULL;
  HTofHitSim       *pMUTof=NULL;
  HShowerHit       *pMUShower=NULL;

  // ---- RICH ---- //
  Int_t yRichHitIPU, xRichHitIPU;
  Int_t RingsumPattern[6][96][96];
  for (Int_t s=0;s<6;s++)
    {
      for (Int_t r=0;r<96;r++)
        {
          for(Int_t c=0;c<96;c++)
            {
              RingsumPattern[s][r][c]= 0;
            }
        }
    }

  iterRich->Reset();
  while ((pMURich = (HRichHitIPUSim *)iterRich->Next()) != 0)
    {
      sector = pMURich->getSector();
      yRichHitIPU = pMURich->getY();
      xRichHitIPU = pMURich->getX();

      if((sector<0||sector>5)||
	 (yRichHitIPU<0||yRichHitIPU>95)||
         (xRichHitIPU<0||xRichHitIPU>95)
	)
      {
	  Error("prepareInput()","rich Address out of Range in event %i sequence number %i !",
		gHades->getEventCounter(),
		gHades->getCurrentEvent()->getHeader()->getEventSeqNumber());
	  continue;
      }

      if (version==1)
        {
          RingsumPattern[sector][xRichHitIPU][yRichHitIPU]=pMURich->getQuality();
        }
      else
        {
          if (version==2)
            {
              if( (pMURich->getQuality() > fRichIPUParThr->getThresholds(0,sector)) &&
                  (pMURich->getVetoQuality()< -fRichIPUParThr->getThresholds(1,sector)))
                {
                  RingsumPattern[sector][xRichHitIPU][yRichHitIPU]=pMURich->getQuality();
                }
            }
        }
    }

  iterRich->Reset();
  size = pRich->getEntries();
  for(Int_t sec=0;sec<6;sec++)
    {
      thetaRich[sec]->Set(size);
      phiRich[sec]->Set(size);
    }
  while ((pMURich = (HRichHitIPUSim *)iterRich->Next()) != 0 )
    {
      Int_t fulfill=0;
      sector = pMURich->getSector();
      if(nRichHits[sector]>=size)
      {
	  Error("prepareInput()","rich nHits >= size of array in event %i sequence number %i !",
		gHades->getEventCounter(),
		gHades->getCurrentEvent()->getHeader()->getEventSeqNumber());
	  break;
      }
      if(nRichHits[sector] < 600)
        {
          yRichHitIPU = pMURich->getY();
          xRichHitIPU = pMURich->getX();


	  if((xRichHitIPU<=0||yRichHitIPU<=0) || (xRichHitIPU>94||yRichHitIPU>94))
	  {
	      Error("prepareInput()","rich -1 or >95 array index in event %i sequence number %i !",
		    gHades->getEventCounter(),
		    gHades->getCurrentEvent()->getHeader()->getEventSeqNumber());
	      continue;
	  }

	  if( ( (version==1) && (pMURich->getQuality() > fRichIPUParThr->getThresholds(0,sector)) &&
                (pMURich->getVetoQuality() < (-1 * fRichIPUParThr->getThresholds(1,sector))) ) ||
              ( (version==2) && (RingsumPattern[sector][xRichHitIPU][yRichHitIPU] )) )
            {
              Int_t CurrentSum = pMURich->getQuality();

              if  (fRichIPUParLmax->getLocMaxFlag()==1)
                {
                  if ( (fRichIPUParLmax->getLocMax()==4)||(fRichIPUParLmax->getLocMax()==8) )
                    {
		       if ( CurrentSum  >= RingsumPattern[sector][xRichHitIPU+1][yRichHitIPU] &&
                          CurrentSum  >= RingsumPattern[sector][xRichHitIPU][yRichHitIPU+1] &&
                          CurrentSum  >  RingsumPattern[sector][xRichHitIPU-1][yRichHitIPU] &&
                          CurrentSum  >  RingsumPattern[sector][xRichHitIPU][yRichHitIPU-1] )
                        {

                          if (fRichIPUParLmax->getLocMax()==8)
                            {
                              if ( CurrentSum  >= RingsumPattern[sector][xRichHitIPU+1][yRichHitIPU+1] &&
                                  CurrentSum  >= RingsumPattern[sector][xRichHitIPU-1][yRichHitIPU+1] &&
                                  CurrentSum  >  RingsumPattern[sector][xRichHitIPU-1][yRichHitIPU-1] &&
                                  CurrentSum  >  RingsumPattern[sector][xRichHitIPU+1][yRichHitIPU-1] )
                                {
                                  fulfill=1;
                                }
                            }
                          else
                            {
                              fulfill=1;
                            }
                        }
                    }
                  else
                    {
                      INFO_msg(10,HMessageMgr::DET_TRIGGER," wrong parameter value!!!");
                    }
                }
              else
                {
                  fulfill=1;
                }
              if (fulfill)
                {

		  thetaRich[sector]->AddAt(pMURich->getTheta(),nRichHits[sector]);
                  phiRich[sector]->AddAt(pMURich->getPhi(),nRichHits[sector]);
                  nRichHits[sector]++;
                  nRichTotal++;
                }
            }
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
  size = pTof->getEntries();
  for(Int_t sec=0;sec<6;sec++)
    {
      thetaTof[sec]->Set(size);
      phiTof[sec]->Set(size);
      flagTof[sec]->Set(size);
    }
  while ((pMUTof = (HTofHitSim *)iterTof->Next()) != 0)
    {
      sector = pMUTof->getSector();
      if(nTofHits[sector]>=size)
      {
	  Error("prepareInput()","tof nHits >= size of array in event %i sequence number %i !",
		gHades->getEventCounter(),
		gHades->getCurrentEvent()->getHeader()->getEventSeqNumber());
	  break;
      }
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
	  else
	  {
              flagTof[sector]->AddAt(0,nTofHits[sector]);
	  }
          nTofHits[sector]++;
          nMetaHits++;
        }
      else
        {
          ERROR_msg(HMessageMgr::DET_TRIGGER,"Maximum number of hits in TOF exceeded(600), skipping rest of event!");
          break;
        }
    }

  // ---- SHOWER ---- //
  Float_t phi_shower, theta_shower; //, r_shower;
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
      if(nShowerHits[sector]>=size)
      {
	  Error("prepareInput()","shower nHits >= size of array in event %i sequence number %i !",
		gHades->getEventCounter(),
		gHades->getCurrentEvent()->getHeader()->getEventSeqNumber());
	  break;
      }
      if(nShowerHits[sector] < 600)
        {
          // Real phi coordinate of hit used. Not exactly what the matching unit does!
          //pMUShower->getSphereCoord(&r_shower,&phi_shower,&theta_shower);
          phi_shower = fShowerMap->getShowerPhi(pMUShower->getCol(),pMUShower->getRow());
          theta_shower = fShowerMap->getShowerTheta(pMUShower->getCol(),pMUShower->getRow());
          if (sector<5)
          {
            phi_shower += 60*(sector+1);
          }
          phiShower[sector]->AddAt(phi_shower,nShowerHits[sector]);
          thetaShower[sector]->AddAt(theta_shower,nShowerHits[sector]);
          nShowerHits[sector]++;
          nMetaHits++;
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
Bool_t HMUEmulationSim::finalize(void)
{
  return kTRUE;
}
//============================================================================

ClassImp(HMUEmulationSim)
