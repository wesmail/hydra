//*-- Author: Alberica Toia
//*-- Modified: Joern Wuestenfeld 05/13/2005
//*-- Version: $Id: hrichanalysisIPU.cc,v 1.12 2005-10-10 11:26:10 kempter Exp $

#include "hrichanalysisIPU.h"
#include "hades.h"
#include "hcategory.h"
#include "hiterator.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "hevent.h"
#include "richutildef.h"
#include "hrichdetector.h"
#include "hrichraw.h"
#include "hrichhitIPU.h"
#include "hrichhitIPUSim.h"
#include "hrichgeometrypar.h"
#include "hrichringfindIPU.h"
#include "triggerinfodef.h"
#include "htriggerdetector.h"
#include "hlocation.h"
#include "hmessagemgr.h"

//----------------------------------------------------------------------------

HRichAnalysisIPU::HRichAnalysisIPU(void)
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
  fIter = NULL;

  pRings = NULL;
  iRingNr = 0;
  iRingNrTot = 0;

  iEmulationContext = 0;

  pHardRingFind = new HRichRingFindIPU();
}
//============================================================================

//----------------------------------------------------------------------------

HRichAnalysisIPU::HRichAnalysisIPU(Text_t *name,Text_t *title)
              : HReconstructor(name, title)
{
  // Default constructor
  //
  // Input parameters:
  //   Text_t *name
  //     Name of the object
  //   Text_t *title
  //     Title /desciption of the object
  //
  // Output parameters:
  //   none
  //
  // Return code:
  //   none
  fIter = NULL;

  pRings = NULL;
  iRingNr = 0;
  iRingNrTot = 0;

  iEmulationContext = 0;

  pHardRingFind = new HRichRingFindIPU();
}
//============================================================================

//----------------------------------------------------------------------------

HRichAnalysisIPU::~HRichAnalysisIPU(void)
{
  // Destructor
  //
  // Input parameters:
  //   none
  //
  // Output parameters:
  //   none
  //
  // Return code:
  //   none
  fIter = NULL;
  if (pHardRingFind)
    {
      delete pHardRingFind;
      pHardRingFind = NULL;
    }

  if (fIter)
    {
      delete fIter;
      fIter = NULL;
    }
}
//============================================================================

//----------------------------------------------------------------------------
HRichAnalysisIPU::HRichAnalysisIPU(const HRichAnalysisIPU& source)
{
  // Copy constructor
  //   Can not be used due to technical reasons! Will exit program!
  //
  // Input parameters:
  //   const HRichAnalysisIPU &source
  //
  // Output parameters:
  //   none
  //
  // Return code:
  //   none
  ERROR_msg(HMessageMgr::DET_TRIGGER,"HRichAnalysisIPU object can not be initialized with values of another object!");
  exit(-1);
}
//============================================================================

//----------------------------------------------------------------------------
HRichAnalysisIPU& HRichAnalysisIPU::operator=(const HRichAnalysisIPU& source)
{
  // Operator =
  //   Can not be used! No effect!
  //
  // Input parameters:
  //   const HRichAnalysisIPU &source
  //
  // Output parameters:
  //   none
  //
  // Return code:
  //   Returns pointer to itself.
  if (this != &source)
    {
      ERROR_msg(HMessageMgr::DET_TRIGGER,"HRichAnalysisIPU object can not be assigned!");
    }
  return *this;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HRichAnalysisIPU::init(void)
{
  // Initialization function.
  //   Checks for needed categories and parameter containers.
  //
  // Input parameters:
  //   none
  //
  // Output parameters:
  //   none
  //
  // Return code:
  //   Returns kTRUE if successfull, kFALSE otherwise.
  SEPERATOR_msg("-",60);
  INFO_msg(10,HMessageMgr::DET_TRIGGER,"INIT of HRichAnalysisIPU()");
  HRuntimeDb *rtdb = gHades->getRuntimeDb();
  HRichDetector * pRichDet = (HRichDetector*)gHades->getSetup()->getDetector("Rich");
  HTriggerDetector * pTriggerDet = (HTriggerDetector*)gHades->getSetup()->getDetector("Trigger");

  if(!pRichDet)
    {
      return kFALSE;
    }
  if(!pTriggerDet)
    {
      return kFALSE;
    }

  //allocate input categories
  m_pRawCat=gHades->getCurrentEvent()->getCategory(catRichRaw);
  if (!m_pRawCat)
    {
      ERROR_msg(HMessageMgr::DET_TRIGGER,"HRichRaw category not found!");
      return kFALSE;
    }
  fIter = (HIterator*)m_pRawCat->MakeIterator("native");

  //allocate output categories
  m_pHardHitCat=gHades->getCurrentEvent()->getCategory(catHardRichHit);
  if (!m_pHardHitCat)
    {
      m_pHardHitCat=pTriggerDet->buildCategory(catHardRichHit);
      if (!m_pHardHitCat)
        {
          ERROR_msg(HMessageMgr::DET_TRIGGER,"Can not create HRichHardHit category!");
          return kFALSE;
        }
      else
        {
          gHades->getCurrentEvent()->addCategory(catHardRichHit, m_pHardHitCat, "Trigger");
        }
    }

  // check for the RICH geometry container
  if(rtdb)
    {
      HRichGeometryPar *pGeomPar = (HRichGeometryPar*)rtdb->getContainer("RichGeometryParameters");
      if (pGeomPar == NULL)
        {
          ERROR_msg(HMessageMgr::DET_TRIGGER,"RICH geometry parameters not found!");
          return kFALSE;
        }
      setGeomPar(pGeomPar);
    }

  pHardRingFind->init(this);
  if (!strcmp(pHardRingFind->checkParamContext(),"RICHIPUEmulation"))
    {
      gHades->getMsg()->info(10,HMessageMgr::DET_TRIGGER,GetName(),"%s %s",pHardRingFind->checkParamContext(),"is equal");
      iEmulationContext = 1;
    } 

  SEPERATOR_msg("-",60);
  return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------

Bool_t HRichAnalysisIPU::finalize(void)
{
  // Finalize the job of the task
  //
  // Input parameters:
  //   none
  //
  // Output parameters:
  //   none
  //
  // Return code:
  //   Always kTRUE.
  return kTRUE;
}     
//============================================================================

//---------------------------------------------------------------------------- 

Int_t HRichAnalysisIPU::execute(void)
{
  // Execute the job of the task.
  //
  // Input parameters:
  //   none
  //
  // Output parameters:
  //   none
  //
  // Return code:
  //   Always kTRUE;
  HRichRaw *pRaw; 

  pHardRingFind->ZeroArray();

  fIter->Reset();
  while((pRaw = (HRichRaw *)fIter->Next())!= NULL)
    {
      pHardRingFind->FiredPad(pRaw->getSector(),pRaw->getRow(),pRaw->getCol());
    }

  iRingNrTot=0;

  // ------- loop over sectors --- begin ---
  for(int i = 0; i < 6; i++)
    {
      pHardRingFind->SetSector(i);
      iRingNr = pHardRingFind->Execute();
      iRingNrTot+=iRingNr;  
      if (iRingNr)
        {
          if ( iEmulationContext )
	    {
              DataWordCut(i);
            } 
          updateHits(i);
        }
    }  // ------- loop over sectors --- end ---
  return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
void HRichAnalysisIPU::updateHits(Int_t nSec)
{
  // Put the information into the output category
  //
  // Input parameters:
  //   Int_t nSec
  //     number of sector for which the information is stored.
  //
  // Output parameters:
  //   none
  //
  // Return code:
  //   none
  HRichHitIPU *hit=NULL; 

  for (int i = 0; i < iRingNr; i++)
    {
      loc.set(1, nSec);

      hit=(HRichHitIPU *)m_pHardHitCat->getNewSlot(loc);

      if (hit!=NULL)
      {
          HRichHitIPU* myHit=((HRichHitIPUCont *)pRings->At(i))->getData();
	  hit=new(hit) HRichHitIPU(*myHit);
	  hit->setSector(nSec);

	  // mapping according to RICH
	  HRichPad * pad = ((HRichGeometryPar*)fpGeomPar)->getPadsPar()->
	      getPad(hit->getX(),hit->getY());

	  hit->setPhi(pad->getPhi(nSec));
	  hit->setTheta(pad->getTheta());
      }
    }
}
//============================================================================

//----------------------------------------------------------------------------
void HRichAnalysisIPU::DataWordCut(Int_t nSec)
{
  // Sort the aquired ring information
  //
  // Input parameters:
  //   Int_t nSec
  //     Number of sector to work on.
  //
  // Output parameters:
  //   none
  //
  // Return code:
  //   none


    for(Int_t jj=0; jj<iRingNr;jj++)
    {
	HRichHitIPU*  hit=((HRichHitIPUCont*)pRings->At(jj))->getData();

	hit->setY(100*hit->getY() + hit->getX());
    }

    HRichHitIPUSimCont::sortIncreasingOrder(kTRUE);
    HRichHitIPUCont::sortIncreasingOrder(kTRUE);
    Int_t size=pRings->GetLast();

    if(size>=0) { pRings->Sort(size+1);}

    Int_t currentNumberOfWords = 0;
    Int_t newX, newY, oldX, oldY;
    oldX = -111; oldY = -111;

    for (Int_t jj = 0;  jj <iRingNr; jj++)
    {
	HRichHitIPU* hit=((HRichHitIPUCont*)pRings->At(jj))->getData();

	newX = hit->getX();
	newY = (Int_t)hit->getY()/100;

	if ( !( (newY==oldY) && (newX/8==oldX/8)) )
	{
	    currentNumberOfWords++;
	}

	hit->setY(newY);
	oldX = newX; oldY = newY;

	hit->setDataWord(currentNumberOfWords);
	if (hit->getDataWord()==999)
	{
	    gHades->getMsg()->error(10,HMessageMgr::DET_TRIGGER,GetName(),"HELP !!!!! DataW %c",currentNumberOfWords);
	}
    }

}
//============================================================================
ClassImp(HRichAnalysisIPU)
