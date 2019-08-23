//*-- Author: Alberica Toia
//*-- Modified: Joern Wuestenfeld 05/13/2005
//*-- Version: $Id: muEmulation.cc,v 1.20 2005-09-01 10:59:12 wuestenf Exp $

#include "muEmulation.h"
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
#include "hmessagemgr.h"
#include "hmuEmuleptons.h"
#include "heventheader.h"
#include "htriggersetup.h"

#include "TArrayI.h"
#include "TArrayF.h"
#include "TObjArray.h"

//
// HMuEmulation
//
// Matching unit emulation code.
// This class is to be used for real data that contains the matching unit subevent.
// To get a positiv trigger, all of the conditions on muRichHits, muMetaHits, muLeptons
// muDileptons and muRichTotalHits have to be fullfilled.
//

Bool_t HMUEmulation::initParContainer(HSpectrometer *spec, HRuntimeDb *rtdb)
{
  // Adds to the runtime database "rtdb" the containers needed by the MU Emulation
  //
  // Input parameters:
  //   HSpectrometer *spec
  //       To get information of the spectrometer setup.
  //   HRuntimeDb *rtdb
  //      Poineter to runtime database
  //
  // Output parameters:
  //   none
  //
  // Return code:
  //   kTRUE if all parameter containers exist in runtime database,
  //   kFALSE otherwise.
  fTrigPar=(HTriggerParMuLep *)rtdb->getContainer("TriggerParMuLep");
  if (fTrigPar)
    {
      INFO_msg(10,HMessageMgr::DET_TRIGGER,"Trigger parameters MU lepton emulation initialized");
    }
  else
    {
      INFO_msg(10,HMessageMgr::DET_TRIGGER,"Trigger parameter MU lepton emulation initialization failed!!");
      return kFALSE;
  }

  fTrigParMom=(HTriggerParMomMap *)rtdb->getContainer("TriggerParMomMap");
  if (fTrigParMom)
    {
      INFO_msg(10,HMessageMgr::DET_TRIGGER,"Trigger parameters MU momentum mapping initialized");
    }
  else
    {
      INFO_msg(10,HMessageMgr::DET_TRIGGER,"Trigger mparameter MU momentum mapping initialization failed!!");
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

HMUEmulation::HMUEmulation(void)
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
  fTrigPar = NULL;
  fTrigParMom = NULL;
  fTriggerSetup = NULL;
  pEmuLeptons=NULL;
  nEmuLeptons=0;
  pRich= NULL;
  pTof= NULL;
  pShower= NULL;
  pLeptons= NULL;
  iterRich=NULL;
  iterTof= NULL;
  iterShower=NULL;
  pEmuLeptons = new TObjArray(2);  //extra-size array
  for(Int_t sec=0;sec<6;sec++)
    {
      thetaRich[sec] = new TArrayF();
      phiRich[sec] = new TArrayF();
      thetaShower[sec] = new TArrayF();
      phiShower[sec] = new TArrayF();
      thetaTof[sec] = new TArrayF();
      phiTof[sec] = new TArrayF();
      flagTof[sec] = new TArrayI();
    }
  reset();
}
//============================================================================

//----------------------------------------------------------------------------
HMUEmulation::HMUEmulation(Text_t *name,Text_t *title)
              : HReconstructor(name, title)
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
  fTrigPar = NULL;
  fTrigParMom = NULL;
  fTriggerSetup = NULL;
  pEmuLeptons=NULL;
  nEmuLeptons=0;
  pRich= NULL;
  pTof= NULL;
  pShower= NULL;
  pLeptons= NULL;
  iterRich=NULL;
  iterTof= NULL;
  iterShower=NULL;
  pEmuLeptons = new TObjArray(2);  //extra-size array
  for(Int_t sec=0;sec<6;sec++)
    {
      thetaRich[sec] = new TArrayF();
      phiRich[sec] = new TArrayF();
      thetaShower[sec] = new TArrayF();
      phiShower[sec] = new TArrayF();
      thetaTof[sec] = new TArrayF();
      phiTof[sec] = new TArrayF();
      flagTof[sec] = new TArrayI();
    }
  reset();
}
//============================================================================

//----------------------------------------------------------------------------
HMUEmulation::~HMUEmulation(void)
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

void HMUEmulation::reset(void)
{
  // Reset all datamembers that are used on a per event basis.
  //
  // Input parameters:
  //   none
  //
  // Output parameters:
  //   none
  //
  // Return code
  //   none
  for (Int_t s=0; s<6; s++)
    {
      nRichHits[s]=0;
      nTofHits[s]=0;
      nShowerHits[s]=0;
      thetaRich[s]->Reset();
      phiRich[s]->Reset();
      thetaShower[s]->Reset();
      phiShower[s]->Reset();
      thetaTof[s]->Reset();
      phiTof[s]->Reset();
      flagTof[s]->Reset();
    }
  nMetaHits=0;
  nRichTotal=0;
  triggerDecision = 0;
}

Bool_t HMUEmulation::init(void)
{
  // Initialize the task. Create all parameter containers and categories.
  //
  // Input parameters:
  //   none
  //
  // Output parameters:
  //   none
  //
  // Return code:
  //   none
  SEPERATOR_msg("*",60);
  INFO_msg(10,HMessageMgr::DET_TRIGGER,"INIT of HMUEmulation()");
  HSpectrometer *spec = gHades->getSetup();
  HRuntimeDb *rtdb = gHades->getRuntimeDb();
  HTriggerDetector * pTrigDet = (HTriggerDetector*)spec->getDetector("Trigger");
  HEvent *event = gHades->getCurrentEvent();
  if(!event)
  {
    INFO_msg(10,HMessageMgr::DET_TRIGGER,"Event structure not initialized!");
    return kFALSE;
  }

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
  if((pRich=event->getCategory(catMatchURich))==NULL) return kFALSE;
  iterRich = (HIterator*)pRich->MakeIterator("native");

  if((pTof=event->getCategory(catMatchUTof))==NULL) return kFALSE;
  iterTof = (HIterator*)pTof->MakeIterator("native");

  if((pShower=event->getCategory(catMatchUShower))==NULL) return kFALSE;
  iterShower = (HIterator*)pShower->MakeIterator("native");

  // allocate output categories
  pLeptons=event->getCategory(catMUEMULeptons);
  if(!pLeptons && pTrigDet)
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
  SEPERATOR_msg("*",60);
  return kTRUE;
}

//============================================================================

Bool_t HMUEmulation::reinit(void)
{
//   muDileptons = fTriggerSetup->getMuDileptons();
//   muLeptons = fTriggerSetup->getMuLeptons();
//   muMaxLeptons = fTriggerSetup->getMuMaxLeptons();
//   muRichHits = fTriggerSetup->getMuRichHits();
//   muRichTotalHits = fTriggerSetup->getRichTotalHits();
//   muMetaHits = fTriggerSetup->getMuMetaHits();
  return kTRUE;
}

//----------------------------------------------------------------------------
Int_t HMUEmulation::roundAngle(Float_t fAngle, Int_t iRange, Float_t round)
{
  // The Matching Unit uses an internal mapping made of integers in the range (0,255),
  // so the calculated angle has to be rounded to this mapping.
  //
  // Input parameters:
  //   Float_t fAngle
  //     Value of angle to be rounded
  //   Int_t iRange
  //     Range to which the value will be rounded.
  //   Float_t round
  //     Rounding factor, default 0.5
  //
  // Output parameters:
  //   none
  //
  // Return code:
  //   Rounded value.

  Int_t iAngle =0;
  iAngle = (Int_t)(fAngle *255./iRange +round);
  if (iAngle > 255) iAngle =255;
  if (iAngle < 0) iAngle =0;

  return iAngle;
}
//============================================================================

//----------------------------------------------------------------------------
Int_t HMUEmulation::execute(void)
{
  // Execute the task.
  // Read the RICH, TOF and SHOWER hit information. If more than 5 rings in RICH
  // found, the trigger decission is made positiv, and no leptons are searched!
  // If there are more than 1 hit in META, the search for leptons is done.
  // After lepton search, a check is done with a cut on the lepton  number to
  // be more than 7. If so, then the trigger decision is set to 2.
  //
  // Input parameters:
  //   none
  //
  // Output parameters:
  //   none
  //
  // Return code:
  //  Allways 0!
  
  reset();
  prepareInput();
  // WARNING: the cut on 5 hits is hardcoded here!!
  // if(nRichTotal > muRichTotalHits)
  if(nRichTotal >= 6)
  {
    triggerDecision = 1;
  }
  else
  {
  // WARNING: the cut on 1 META hit is hardcoded here!
  //if(nMetaHits > muMetahits)
    if(nMetaHits > 0)
    {
      matching();
      if (nEmuLeptons > 0)
      {
	    //gHades->getMsg()->info(10,HMessageMgr::DET_TRIGGER,GetName(),"Number of Leptons %d",nEmuLeptons);
        updateEmuLeptons();
        pEmuLeptons->Delete();
        pEmuLeptons->Expand(2);
        if(!triggerDecision)
        {
          triggerDecision = 1;
        }
      }
    }
  }
  pEventHeader->setMuEmuTriggerDecision(triggerDecision);
  return 0;
}
//============================================================================

//----------------------------------------------------------------------------
Int_t HMUEmulation::matching(void)
{
  // Match the infomation from RICH TOF and SHOWER, to emulate the matching unit.
  //
  // Input Parameters:
  //   none
  //
  // Output parameters:
  //   none
  //
  // Return code:
  //   Returns number of found leptons.
  Float_t fphiReg_Rich;
  Int_t   iphiReg_Rich;
  Float_t fRich_phi,fTof_phi,fShower_phi;
  Float_t fRich_theta,fTof_theta,fShower_theta;
  Int_t   iRich_phi,iTof_phi,iShower_phi;
  Int_t   iRich_theta,iTof_theta,iShower_theta;
  fphiReg_Rich = fRich_phi = fTof_phi = fShower_phi=0;
  iphiReg_Rich = iRich_phi = iTof_phi = iShower_phi=0;
  iRich_theta = iTof_theta = iShower_theta=0;
  fRich_theta = fTof_theta = fShower_theta = 0.;

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

  for (Int_t sec =0; sec <6; sec++)
    { // loop over sectors
      for (Int_t actRich=0; actRich < nRichHits[sec]; actRich++)
        { // rich loop

          fRich_theta = thetaRich[sec]->At(actRich);
          fRich_phi = phiRich[sec]->At(actRich);

          if (fRich_theta>0.1)
            { // valid range of mapped rings

              iRich_theta = roundAngle(fRich_theta,90);
              iRich_phi = roundAngle(fRich_phi,60);

              fphiReg_Rich = fabs(fRich_phi-30);
              iphiReg_Rich = roundAngle(fphiReg_Rich,60,0.);
              //iphiReg_Rich = roundAngle(fRich_phi,60,0.);
              // !!!!!! problem is the round: +0. or +0.5???
              // for the matching window +0. is ok, but for the momentum +0.5 seems to be better...
              // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!#

              for (Int_t actTof=0; actTof < nTofHits[sec]; actTof++)
                { // tof loop

                  if (flagTof[sec]->At(actTof)==1)
                    { // lepton-like tof

                      fTof_theta = thetaTof[sec]->At(actTof);
                      fTof_phi = phiTof[sec]->At(actTof);

                      iTof_theta = roundAngle(fTof_theta,90);
                      iTof_phi = roundAngle(fTof_phi,60);

                      if ((Int_t(fabs((Float_t) (iRich_phi - iTof_phi))) <
                          (fTrigPar->getMUphiOffset() + int(iphiReg_Rich/fTrigPar->getMUphiSlope())))
                          && ( Int_t(fabs((Float_t)(iRich_theta - iTof_theta))) < fTrigPar->getMUtheta()))
                        {
                          //emuleptons
                          HMUEMULeptons *lepton = new HMUEMULeptons();;

                          lepton->setDetBit(0);
                          lepton->setThetaRich(fRich_theta);
                          lepton->setPhiRich(fRich_phi);
                          lepton->setThetaMeta(fTof_theta);
                          lepton->setPhiMeta(fTof_phi);
                          lepton->setSector(sec);

                          // re-binning of variables according to the momentum table
                          a=((Float_t)(iRich_theta)/255.)*(fTrigParMom->getTheta_bins());
                          b=((Float_t)(fabs((Float_t)(iRich_theta-iTof_theta)))/255.)*(fTrigParMom->getDeltatheta_bins());
                          c=((Float_t)(iphiReg_Rich)/128)*(fTrigParMom->getPhi_bins());

                          fmom = fTrigParMom->getMom((Int_t)a,(Int_t)b,(Int_t)c);
                          fmom = fmom * field_current / 3465.0;
                          if (fmom > 0.1)
                            { 	 
                              if (fmom < low_cut) 
                                {
                                  imom = 1;
                                } 	 
                              else
                                {
                                  if (fmom > high_cut) 
                                    {
                                      imom = 255;
                                    }
                                  else
                                    {
                                      imom = (Long_t)(255 * fmom / (high_cut) + 0.5);
                                      if (imom > 255) 
                                        {
                                          imom = 255; 	 
                                        }
                                      mom = imom;
                                    }
                                }
                            }
                          else
                            {
                              mom = 0;
                            }
                          lepton->setMomentum(mom);
                          if ((iRich_theta - iTof_theta) < 0)
                            {
                              lepton->setEFlag(1);  //electrons;
                            }
                          else
                            {
//                            if ((iRich_theta - iTof_theta) > 0)
//                              {
                                  lepton->setEFlag(0);  //positrons;
//                              }
                            }
                          pEmuLeptons->Add(lepton);
                          nEmuLeptons++;
                          // WARNING number of max leptons hardcoded
                          // if(nEmuLeptons > muMaxLeptons)
                          if(nEmuLeptons > 6)
                          {
                            return nEmuLeptons;
                          }
                        }// if ((Int_t(fabs((Float_t) (iRich_phi - iTof_phi))) ...
                    }// if (flagTof[sec][actTof]==1)
                }//for (Int_t actTof=0; actTof < nTofHits[sec]; actTof++)

              for (Int_t actShower=0; actShower < nShowerHits[sec]; actShower++)
                { // shower loop

                  fShower_theta = thetaShower[sec]->At(actShower);
                  fShower_phi = phiShower[sec]->At(actShower);

                  iShower_theta = roundAngle(fShower_theta,90);
                  iShower_phi = roundAngle(fShower_phi,60);

                  if ((Int_t(fabs((Float_t)(iRich_phi - iShower_phi)))   <
                      (fTrigPar->getMUphiOffset() + Int_t(iphiReg_Rich/fTrigPar->getMUphiSlope())))
                      && (Int_t(fabs((Float_t)(iRich_theta - iShower_theta))) < fTrigPar->getMUtheta()))
                    {

                      //emuleptons
                      HMUEMULeptons *lepton = new HMUEMULeptons();

                      lepton->setDetBit(1);
                      lepton->setThetaRich(fRich_theta);
                      lepton->setPhiRich(fRich_phi);
                      lepton->setThetaMeta(fShower_theta);
                      lepton->setPhiMeta(fShower_phi);
                      lepton->setSector(sec);

                      a=((Float_t)(iRich_theta)/255.)*(fTrigParMom->getTheta_bins());
                      b=((Float_t)(fabs((Float_t)(iRich_theta-iShower_theta)))/255.)*(fTrigParMom->getDeltatheta_bins());
                      c=((Float_t)(iphiReg_Rich)/128)*(fTrigParMom->getPhi_bins());

                      fmom = fTrigParMom->getMom((Int_t)a,(Int_t)b,(Int_t)c);
                      fmom = fmom * field_current / 3465.0;
                      if (fmom > 0.1)
                        {
                          if (fmom < low_cut) 
                            {
                              imom = 1;
                            }
                          else
                            {
                              if (fmom > high_cut) 
                                {
                                  imom = 255;
                                }
                              else
                                {
                                  imom = (Long_t)(255 * fmom / (high_cut) + 0.5);
                                  if (imom > 255)
                                    {
                                      imom = 255;
                                    } 	 
                                }
                              mom = imom;
                            }
                        }
                      else
                        {
                          mom = 0;
                        }
                      lepton->setMomentum(mom);
                      if ((iRich_theta - iShower_theta) < 0)
                        { 
                          lepton->setEFlag(1);    //electrons;
                        }
                      else
                        {
//                        if ((iRich_theta - iShower_theta) > 0)
//                        {
                            lepton->setEFlag(0);  //positrons;
//                        }
                        }
                      pEmuLeptons->Add(lepton);
                      nEmuLeptons++;
                      // WARNING number of max leptons hardcoded
                      // if(nEmuLeptons > muMaxLeptons)
                      if(nEmuLeptons > 6)
                      {
                        return nEmuLeptons;
                      }
                   }// if ((Int_t(fabs((Float_t)(iRich_phi - iShower_phi)))...
                }// for (Int_t actShower=0; actShower < nShowerHits[sec]; actShower++)
            }// if (fRich_theta>0.1)
        } // for (Int_t actRich=0; actRich < nRichHits[sec]; actRich++)
    }//for (Int_t sec =0; sec <6; sec++)
  return nEmuLeptons;
}
//============================================================================
//----------------------------------------------------------------------------
void HMUEmulation::updateEmuLeptons(void)
{
  // Store the found leptons in the category.
  //
  // Input parameters:
  //   none
  //
  // Output parameters:
  //   none
  //
  // Return code:
  //   none
  HMUEMULeptons *emuLepton=NULL;

  for (Int_t i = 0; i < nEmuLeptons; i++)
    {
      loc.set(1, 0);
      emuLepton=(HMUEMULeptons *)pLeptons->getNewSlot(loc);

      if (emuLepton!=NULL)
        {
          emuLepton=new(emuLepton) HMUEMULeptons;
          *emuLepton = *(HMUEMULeptons*)(pEmuLeptons->At(i));
        }
    }
}
//============================================================================

//----------------------------------------------------------------------------
void HMUEmulation::prepareInput(void)
{
  // Retrieve information from categories, and store them in internal variables.
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
  Float_t phi;
  HMatchURich   *pMURich=NULL;
  HMatchUTof    *pMUTof=NULL;
  HMatchUShower *pMUShower=NULL;

  // ---- RICH ---- //
  iterRich->Reset();
  size = pRich->getEntries();
  for(Int_t sec=0;sec<6;sec++)
    {
      thetaRich[sec]->Set(size);
      phiRich[sec]->Set(size);
    }
  while ((pMURich = (HMatchURich *)iterRich->Next()) != 0)
    {
      sector = pMURich->getSector();
      if(nRichHits[sector] < 600)
        {
          phi = pMURich->getPhi();
          if (sector<5)
            {
              phi -= 60*(sector+1);
            }
          phiRich[sector]->AddAt(phi,nRichHits[sector]);
          thetaRich[sector]->AddAt(pMURich->getTheta(),nRichHits[sector]);
          nRichHits[sector]++;
          nRichTotal++;
        }
      else
        {
          ERROR_msg(HMessageMgr::DET_TRIGGER,"Maximum number of hits in RICH exceeded(600), skipping rest of event!");
          break;
        }
    }
  // ---- TOF ---- //
  iterTof->Reset();
  size = pTof->getEntries();
  for(Int_t sec=0;sec<6;sec++)
    {
      thetaTof[sec]->Set(size);
      phiTof[sec]->Set(size);
      flagTof[sec]->Set(size);
    }
  while ((pMUTof = (HMatchUTof *)iterTof->Next()) != 0)
    {
      sector = pMUTof->getSector();
      if(nTofHits[sector] < 600)
        {
/*          if (sector>5)
            {
              continue;
            }*/
          phi = pMUTof->getPhi();
          if (sector<5)
            {
              phi -= 60*(sector+1);
            }
          phiTof[sector]->AddAt(phi,nTofHits[sector]);
          thetaTof[sector]->AddAt(pMUTof->getTheta(),nTofHits[sector]);
          flagTof[sector]->AddAt(pMUTof->getPID(),nTofHits[sector]);
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
  iterShower->Reset();
  size = pShower->getEntries();
  for(Int_t sec=0;sec<6;sec++)
    {
      thetaShower[sec]->Set(size);
      phiShower[sec]->Set(size);
    }
  while ((pMUShower = (HMatchUShower *)iterShower->Next()) != 0)
    {
      sector = pMUShower->getSector();
      if(nShowerHits[sector] < 600)
        {
          phi = pMUShower->getPhi();
          if (sector<5)
            {
              phi -= 60*(sector+1);
            }
          phiShower[sector]->AddAt(phi,nShowerHits[sector]);
          thetaShower[sector]->AddAt(pMUShower->getTheta(),nShowerHits[sector]);
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
Bool_t HMUEmulation::finalize(void)
{
  // Finalize the job of the taskset, nothing to be done here!
  //
  // Input parameters:
  //   none
  //
  // Output parameters:
  //   none
  //
  // Return code:
  //   Allways kTRUE!
return kTRUE;
}
//============================================================================

ClassImp(HMUEmulation)
