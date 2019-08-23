//*-- Author: Alberica Toia
//*-- Modified: Joern Wuestenfeld 05/10/2005
//*-- Version: $Id: muDilepEmulation.cc,v 1.12 2005-10-12 08:49:27 kempter Exp $

#include "muDilepEmulation.h"
#include "hades.h"
#include "hcategory.h"
#include "hiterator.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "hevent.h"
#include "heventheader.h"
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
#include "hmuEmuleptons.h"
#include "hmessagemgr.h"

//
// HMuDilepEmulation
//
// Matching unit emulation code.
// This class is to be used for real data that contains the matching unit subevent.
//

Bool_t HMUDilepEmulation::initParContainer(HSpectrometer *spec, HRuntimeDb *rtdb)
{
  // Adds to the runtime database "rtdb" the containers needed by the MU Emulation
  //spec is used to get information of the spectrometer setup.
  //
  // Input parameters:
  //   HSpectrometer *spec
  //     Pointer to detector setup.
  //   HRuntimeDb *rtdb
  //     Pointer to HADES runtime database
  //
  // Output parameters:
  //   none
  //
  // Return code:
  //  kTRUE if all containers found, kFALSe otherwise.
  fTrigParDil=(HTriggerParMuDilep *)rtdb->getContainer("TriggerParMuDilep");
  if (fTrigParDil)
    {
      INFO_msg(10,HMessageMgr::DET_TRIGGER,"Trigger Parameters MU Dilepton Emulation initialized");
    }
  else
    {
      ERROR_msg(HMessageMgr::DET_TRIGGER,"Trigger Parameter MU Dilepton Emulation Initialization failed!");
      return kFALSE;
    }
  return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
HMUDilepEmulation::HMUDilepEmulation(void)
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
  fTrigParDil = NULL;
  pEmuDiLeptons=NULL;
  nEmuDiLeptons=0;
  ptlHMULeptons=NULL;
  ptlHMUEMUDiLeptons=NULL;
  iterHMULeptons=NULL;
  iterHMULeptons2=NULL;
  nLep=0;
  reset();
  pEmuDiLeptons = new TObjArray(2);  //extra-size array
}
//============================================================================

//----------------------------------------------------------------------------
HMUDilepEmulation::HMUDilepEmulation(Text_t *name,Text_t *title)
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
  fTrigParDil = NULL;
  pEmuDiLeptons=NULL;
  nEmuDiLeptons=0;
  ptlHMULeptons=NULL;
  ptlHMUEMUDiLeptons=NULL;
  iterHMULeptons=NULL;
  iterHMULeptons2=NULL;
  nLep=0;
  reset();
  pEmuDiLeptons = new TObjArray(2);  //extra-size array
}
//============================================================================

//----------------------------------------------------------------------------
HMUDilepEmulation::~HMUDilepEmulation()
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
  if(pEmuDiLeptons)
    {
      delete pEmuDiLeptons;
      pEmuDiLeptons=NULL;
    }
  nEmuDiLeptons=0;

  ptlHMULeptons=NULL;
  ptlHMUEMUDiLeptons=NULL;

  if (iterHMULeptons) delete iterHMULeptons;
  iterHMULeptons=NULL;
  if (iterHMULeptons2) delete iterHMULeptons2;
  iterHMULeptons2=NULL;

  nLep=0;
}
//============================================================================

//----------------------------------------------------------------------------
void HMUDilepEmulation::reset(void)
{
  // Reset all datamembers taht are used on a per event basis.
  //
  // Input parameters:
  //   none
  //
  // Output parameters:
  //   none
  //
  // Return code
  //   none
  nLep=0;
  for (int h =0; h<600;h++)
    {
      thetaRich[h]=0;
      phiRich[h]=0;
      mom[h]=0;
      flag[h]=0;
    }
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HMUDilepEmulation::init(void)
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
  HSpectrometer *spec = gHades->getSetup();
  HRuntimeDb *rtdb = gHades->getRuntimeDb();
  HTriggerDetector * pTrigDet = (HTriggerDetector*)gHades->getSetup()->getDetector("Trigger");

  // initialize parameters
  if (pTrigDet)
    {
      if(!initParContainer(spec,rtdb))
        {
          return kFALSE;
        }
    }

  // allocate input categories
  if((ptlHMULeptons=gHades->getCurrentEvent()->getCategory(catMatchULeptons))==NULL) return kFALSE;
  iterHMULeptons = (HIterator*)ptlHMULeptons->MakeIterator("native");

  // allocate output categories
  ptlHMUEMUDiLeptons=gHades->getCurrentEvent()->getCategory(catMUEMUDiLeptons);
  if(!ptlHMUEMUDiLeptons)
    {
      ptlHMUEMUDiLeptons=pTrigDet->buildCategory(catMUEMUDiLeptons);
      if (!ptlHMUEMUDiLeptons)
        { 
          return kFALSE;
        }
      else
        {
          gHades->getCurrentEvent()->addCategory(catMUEMUDiLeptons, ptlHMUEMUDiLeptons, "EmuDiLeptons");
        }
    }
  return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
Int_t HMUDilepEmulation::execute()
{
  // Execute the task.
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
  searchDilep();
  if (nEmuDiLeptons)
    {
      updateEmuDiLeptons();
    }
  pEmuDiLeptons->Delete();
  return 0;

}
//============================================================================

//----------------------------------------------------------------------------
void HMUDilepEmulation::prepareInput(void)
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
  HMULeptons *pLeptons =NULL;
  iterHMULeptons->Reset();
  while (( pLeptons  = (HMULeptons *)iterHMULeptons->Next()) != 0)
  {
      if(nLep>599)
      {
	  Error("prepareInput()","number of leptons >= size of array in event %i sequence number %i !",
		gHades->getEventCounter(),
		gHades->getCurrentEvent()->getHeader()->getEventSeqNumber());
	  break;
      }
      if (pLeptons->getMom()>0)
      {
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
Int_t HMUDilepEmulation::searchDilep(void)
{
  // Search for Dileptons.
  //
  // Input Parameters:
  //   none
  //
  // Output parameters:
  //   none
  //
  // Return code:
  //   Returns number of found leptons.
  Int_t i1,i2;
  HGeomVector pp1,pp2;
  pp1=pp2=0;

  nEmuDiLeptons=0;

  if ( nEmuDiLeptons < fTrigParDil->getMUcutoff() )
    { // MU cutoff

      i1=0;
      for (Int_t act_Lep =0; act_Lep < nLep; act_Lep++)
        {
          i1++;

          i2=0;
          for (Int_t act_Lep2 =act_Lep; act_Lep2 < nLep; act_Lep2++)
            {
              i2++;

              if ( nEmuDiLeptons < fTrigParDil->getMUcutoff() )
                { // MU cutoff

                  if (flag[act_Lep] != flag[act_Lep2])
                    { // unlike sign pairs

                      pp1.setX( sin(thetaRich[act_Lep]* TMath::Pi()/180)*cos(phiRich[act_Lep]* TMath::Pi()/180));
                      pp1.setY( sin(thetaRich[act_Lep]* TMath::Pi()/180)*sin(phiRich[act_Lep]* TMath::Pi()/180));
                      pp1.setZ( cos(thetaRich[act_Lep]* TMath::Pi()/180) );

                      pp2.setX( sin(thetaRich[act_Lep2]* TMath::Pi()/180)*cos(phiRich[act_Lep2]* TMath::Pi()/180) );
                      pp2.setY( sin(thetaRich[act_Lep2]* TMath::Pi()/180)*sin(phiRich[act_Lep2]* TMath::Pi()/180) );
                      pp2.setZ( cos(thetaRich[act_Lep2]* TMath::Pi()/180) );

                      Float_t cOpening = pp1.scalarProduct(pp2);
                      Float_t openAngle=acos(cOpening);

                      //emudileptons
                      HMUEMUDiLeptons *dilepton = new HMUEMUDiLeptons();

                      dilepton->setOpangle(openAngle * 180/TMath::Pi());
                      dilepton->setMass(2 * mom[act_Lep] * mom[act_Lep2] * (1-cOpening)); // michael formula (mass squared)
                      dilepton->setN1(i1);
                      dilepton->setN2(i2);

                      pEmuDiLeptons->Add(dilepton);
                      nEmuDiLeptons++;								
                    }//	if (flag[act_Lep] != flag[act_Lep2])
                }//if ( nEmuDiLeptons < fTrigParDil->getMUcutoff() )
            }//for (Int_t act_Lep2 =act_Lep; act_Lep2 < nLep; act_Lep2++)   
        }//for (Int_t act_Lep =0; act_Lep < nLep; act_Lep++)
    }//if ( nEmuDiLeptons < fTrigParDil->getMUcutoff() )
    return nEmuDiLeptons;  
}
//============================================================================
//----------------------------------------------------------------------------
void HMUDilepEmulation::updateEmuDiLeptons()
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
  HMUEMUDiLeptons *emuDiLepton=NULL; 

  for (int i = 0; i < nEmuDiLeptons; i++)
    {
      loc.set(1, 0);
      emuDiLepton=(HMUEMUDiLeptons *)ptlHMUEMUDiLeptons->getNewSlot(loc);

      if (emuDiLepton!=NULL)
        {
          emuDiLepton=new(emuDiLepton) HMUEMUDiLeptons(*((HMUEMUDiLeptons*)pEmuDiLeptons->At(i)));
        } 
    }
}      
//============================================================================
//----------------------------------------------------------------------------
Bool_t HMUDilepEmulation::finalize(void)
{
  // Finalize the job f the taskset, nothing to be done here!
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

ClassImp(HMUDilepEmulation)
