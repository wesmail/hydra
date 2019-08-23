//*-- Author: Alberica Toia
//*-- Modified: Joern Wuestenfeld 05/13/2005
//*-- Version: $Id: hrichanalysisIPUSim.cc,v 1.8 2005-10-10 11:30:03 kempter Exp $

#include "hrichanalysisIPUSim.h"
#include "hades.h"
#include "hcategory.h"
#include "hlinearcategory.h"
#include "hiterator.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "hevent.h"
#include "hrichcalsim.h"
#include "hrichhitIPUSim.h"
#include "hmatrixcatiter.h"
#include "hrichringfindIPUSim.h"
#include "hrichgeometrypar.h"
#include "hrichpad.h"
#include "hrichcalsim.h"
#include "richutildef.h"
#include "hlocation.h"
#include "hrichdetector.h"
#include "htriggerdetector.h"
#include "triggerinfodef.h"
#include "hrichtrack.h"
#include "hmessagemgr.h"

//---------------------------------------------------------------------------- 
HRichAnalysisIPUSim::HRichAnalysisIPUSim()
  : HRichAnalysisIPU()
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
  pHardRingFindSim = new HRichRingFindIPUSim();
  pRichCalSim =  NULL;
}
//============================================================================

//----------------------------------------------------------------------------

HRichAnalysisIPUSim::HRichAnalysisIPUSim(Text_t *name,Text_t *title)
  :HRichAnalysisIPU(name, title)
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
  pHardRingFindSim = new HRichRingFindIPUSim();
  pRichCalSim =  NULL;
}
//============================================================================

//---------------------------------------------------------------------------- 

HRichAnalysisIPUSim::~HRichAnalysisIPUSim(void)
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
  if (pHardRingFindSim) delete pHardRingFindSim;
  pHardRingFindSim = NULL;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HRichAnalysisIPUSim::init(void)
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
  //   none
  HRichDetector * pRichDet = (HRichDetector*)gHades->getSetup()->getDetector("Rich");
  HTriggerDetector * pTriggerDet = (HTriggerDetector*)gHades->getSetup()->getDetector("Trigger");

  //allocate input categories
  m_pRawCat=gHades->getCurrentEvent()->getCategory(catRichCal);
  if (!m_pRawCat)
    {
      ERROR_msg(HMessageMgr::DET_TRIGGER,"Can not find HRichCal category!");
      return kFALSE;
    }
  fIter = (HIterator*)m_pRawCat->MakeIterator("native");


  //allocate output categories
  m_pHardHitCat=gHades->getCurrentEvent()->getCategory(catHardRichHit);
  if (!m_pHardHitCat)
    {
      m_pHardHitCat=pTriggerDet->buildLinearCat("HRichHitIPUSim");
      if (!m_pHardHitCat)
        {
          ERROR_msg(HMessageMgr::DET_TRIGGER,"Can not create HRichHitIPUSim category!");
          return kFALSE;
        }
      else
        {
          gHades->getCurrentEvent()->addCategory(catHardRichHit, m_pHardHitCat, "Trigger");
        }
    }

  m_pTrackCat = gHades->getCurrentEvent()->getCategory(catRichTrack);
  if (!m_pTrackCat)
    {
      m_pTrackCat = pRichDet->buildCategory(catRichTrack);
      if (!m_pTrackCat)
        {
            ERROR_msg(HMessageMgr::DET_TRIGGER,"Can not create HRichTrack category!");
          return kFALSE;
        }
      else
        {
          gHades->getCurrentEvent()->addCategory(catRichTrack,m_pTrackCat , "Rich");
        }
    }

  HRuntimeDb* rtdb=gHades->getRuntimeDb();
  if(rtdb)
    {
      HRichGeometryPar *pGeomPar = (HRichGeometryPar*)rtdb->getContainer("RichGeometryParameters");
      if (pGeomPar == NULL)
        {
          ERROR_msg(HMessageMgr::DET_TRIGGER,"Can not find RichGeometryParameters container!");
          return kFALSE;
        }
      setGeomPar(pGeomPar);
    }

  pHardRingFindSim->init(this);
 
  return kTRUE;
}
//============================================================================

//---------------------------------------------------------------------------- 

Bool_t HRichAnalysisIPUSim::finalize(void)
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
  //   Returns allways kTRUE.
  return kTRUE;
}
//============================================================================

//---------------------------------------------------------------------------- 

Int_t HRichAnalysisIPUSim::execute(void)
{
  // Execute the job of the task
  //
  // Input parameters:
  //   none
  //
  // Output parameters:
  //   none
  //
  // Return code:
  //   Returns allways kTRUE; 

  HRichCalSim *pCalSim;

  pHardRingFindSim->ZeroArray();

  fIter->Reset();
  while((pCalSim = (HRichCalSim *)fIter->Next()))
    {
      pHardRingFindSim->FiredPad(pCalSim->getSector(),pCalSim->getRow(),pCalSim->getCol());
    }

  iRingNrTot=0;
  // ------- loop over sectors --- begin ---
  for(int i = 0; i < 6; i++)
  {
    pHardRingFindSim->SetSector(i);
    iRingNr = pHardRingFindSim->Execute(this);
    iRingNrTot+=iRingNr;
    if (iRingNr)
    {
	updateHits(i);
    }
  }
  // ------- loop over sectors --- end ---

  return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
void HRichAnalysisIPUSim::updateHits(Int_t nSec)
{
  // Store acquired information in output category.
  //
  // Input parameters:
  //   Int_t nSec
  //     Number of sector for which to write output.
  //
  // Output parameters:
  //   none
  //
  // Return code:
  //   none
  HRichHitIPUSim *hit=NULL;

  for (int i = 0; i < iRingNr; i++)
  {
      loc.set(1, nSec);

      hit=(HRichHitIPUSim *)m_pHardHitCat->getNewSlot(loc);

      if (hit!=NULL)
      {
	  HRichHitIPUSimCont* myHitCont=((HRichHitIPUSimCont*)pRings->At(i));
	  HRichHitIPUSim*     myHit    =((HRichHitIPUSimCont*)pRings->At(i))->getData();

	  myHit->setSector(nSec);

	  // mapping according to RICH
	  HRichPad * pad = ((HRichGeometryPar*)fpGeomPar)->getPadsPar()->getPad(myHit->getX(),myHit->getY());

	  if(!pad)
	  {
	      gHades->getMsg()->error(10,HMessageMgr::DET_TRIGGER,GetName(),
				      "RICH ring  %d coordinate out of sector (%d %d)!",
				      i, myHit->getX(),myHit->getY());
	      continue;
	  }

	  myHit->setPhi(pad->getPhi(nSec));
	  myHit->setTheta(pad->getTheta());

	  // here i write the track number with its weight
	  Int_t k = 0;
	  while(myHitCont->getRingTrack(k) && ( k < 256))
	  {

	      if (myHit->getTrack1()==0)
	      {
		  myHit->setTrack1(myHitCont->getRingTrack(k));
		  myHit->setFlag1(myHitCont->getRingFlag(k));
		  myHit->setWeightTrack1(myHit->getWeightTrack1()+1);
	      }
	      else
	      {
		  if(myHitCont->getRingTrack(k)== myHit->getTrack1())
		  {
		      myHit->setWeightTrack1(myHit->getWeightTrack1()+1);
		  }
		  else
		  {
		      if(myHit->getTrack2()==0)
		      {
			  myHit->setTrack2(myHitCont->getRingTrack(k));
			  myHit->setFlag2(myHitCont->getRingFlag(k));
			  myHit->setWeightTrack2(myHit->getWeightTrack2()+1);
		      }
		      else
		      {
			  if(myHitCont->getRingTrack(k)== myHit->getTrack2())
			  {
			      myHit->setWeightTrack2(myHit->getWeightTrack2()+1);
			  }
			  else
			  {
			      if (myHit->getTrack3()==0)
			      {
				  myHit->setTrack3(myHitCont->getRingTrack(k));
				  myHit->setFlag3(myHitCont->getRingFlag(k));
				  myHit->setWeightTrack3(myHit->getWeightTrack3()+1);
			      }
			      else
			      {
				  myHit->setWeightTrack3(myHit->getWeightTrack3()+1);
			      }
			  }
		      }
		  }
	      }
	      k++;
	  }//end loop on track array

	  hit=new (hit) HRichHitIPUSim(*myHit);

      }
      else
      {
	  ERROR_msg(HMessageMgr::DET_TRIGGER,"No free slot in category HRichHitIPUSim!");
      }
  }
}
//============================================================================

//----------------------------------------------------------------------------
Int_t  HRichAnalysisIPUSim::getPadsNTrack1(Int_t padx, Int_t pady,Int_t sec)
{
  // This function returns for each pad the corresponding
  // NTrack1 value (cfr. HRichCalSim, HRichDigitizer::execute).
  // All track numbers are stored during digitization  in the
  // catRichTrack Linear Category.
  // NTrack1 is the index in the Track array corresponding to the
  // first track for each pad.
  // This function is called from HRichRingFindIPUSim::LookForTrack
  //
  // Input parameters:
  //   Int_t padx
  //     x coordinate of pad
  //   Int_t pady
  //     y coordinate of pad
  //   Int_t sec
  //     sector number [0..5]
  //
  // Output parameters:
  //   none
  //
  // Return code:
  //   Index in the track array of HRichCalSim coresponding to the first track for the pad.

  HLocation loc1;
  loc1.set(3, sec, pady, padx);
  pRichCalSim=NULL;

  pRichCalSim = (HRichCalSim*)((HMatrixCategory*)getRawCat())->getObject(loc1);
  if (pRichCalSim)
    {
      return pRichCalSim->getNTrack1();
    }
return 0;
}
//============================================================================

//----------------------------------------------------------------------------
Int_t  HRichAnalysisIPUSim::getPadsNTrack2(void)
{
  // This functions returns NTrack2, which is the index in
  // the Track Array corresponding to the last track for each pad.
  // Pad must be identified by calling getPadsNTrack1(...) first.
  //
  // Input parametrs:
  //   none
  //
  // Output parameters:
  //   none
  //
  // Return code:
  //   Index in the track array of HRichCalSim coresponding to the last track for the pad.
  if (pRichCalSim)
    {
      return pRichCalSim->getNTrack2();
    }
  return 0;
}
//============================================================================

//----------------------------------------------------------------------------
Int_t HRichAnalysisIPUSim::getFlag(Int_t index)
{
  // This function returns the flag contained in the
  // catRichTrack container at the position index.
  // This flag is 0 for Cherenkov photons and 1 for IP.
  // It is called from HRichRingFindIPUSim::LookForTrack().
  //
  // Input parametrs:
  //   Int_t index
  //     Index in the HRichTrach category.
  //
  // Output parameters:
  //   none
  //
  // Return code:
  //   Flag from HRichTrack, 0 for Cherencov photons, 1 for IP.
  //   -1 if no entry for the index exists.
  HRichTrack *t = (HRichTrack*)((HLinearCategory*)m_pTrackCat)->getObject(index);
  Int_t i = -1;
  if (t)
    {
      i=t->getFlag();
    }
  return  i;
}
//============================================================================

//----------------------------------------------------------------------------
Int_t HRichAnalysisIPUSim::getTrack(Int_t index)
{
  // This function returns the track number contained in the
  // catRichTrack container at the position index.
  //
  // Input parameters:
  //   Int_t index
  //     Index into HRichTrack category
  //
  // Output parameters:
  //   none
  //
  // Return code:
  //   Track number of the track at position index
  HRichTrack *trk = ((HRichTrack*)((HLinearCategory*)m_pTrackCat)->getObject(index));
  Int_t track=-1;
  if(trk)
    {
      track=trk->getTrack();
    }
  return track;
}
//============================================================================

//----------------------------------------------------------------------------

ClassImp(HRichAnalysisIPUSim)
