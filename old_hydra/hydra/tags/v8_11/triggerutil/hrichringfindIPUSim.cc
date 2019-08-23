//*-- Author: Alberica Toia
//*-- Modified: Joern Wuestenfeld 05/17/2005
//*-- Version: $Id: hrichringfindIPUSim.cc,v 1.6 2005-10-10 11:46:23 kempter Exp $

#include "hrichringfindIPUSim.h"
#include "hrichanalysisIPUSim.h"
#include "hrichanalysisIPU.h"
#include "hrichhitIPUSim.h"
#include "hmessagemgr.h"

//******************************************************************************
//
// HRichRingFindIPU
//
// Rich ring finder part of the IPU emulation for simulated data.
//
//******************************************************************************

ClassImp(HRichRingFindIPUSim)

//----------------------------------------------------------------------------
HRichRingFindIPUSim::HRichRingFindIPUSim(void)
  : HRichRingFindIPU()
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
  //  none
  //pRings = new TObjArray();
  //pRings->SetOwner(kFALSE);
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HRichRingFindIPUSim::init(HRichAnalysisIPUSim* showMe)
{
  // Initialization function.
  //
  // Input parameters:
  //   HRichAnalysisIPUSim* showMe
  //     Pointer to IPU analysis class.
  //
  // Output parameters:
  //   none
  //
  // Return code:
  //  Returns kTRUE allways.
  HRichRingFindIPU::init(showMe);
  return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
HRichRingFindIPUSim::~HRichRingFindIPUSim(void)
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
  //  none
  if (pRings)
    {
      delete pRings;
      pRings = NULL;
    }
}
//============================================================================

//----------------------------------------------------------------------------
HRichRingFindIPUSim::HRichRingFindIPUSim(const HRichRingFindIPUSim& source)
{
  // Copy constructor
  //
  // Input parameters:
  //   const HRichRingFindIPUSim& source
  //     Source object from which to copy, unused!
  //
  // Output parameters:
  //   none
  //
  // Return code:
  //  none
  ERROR_msg(HMessageMgr::DET_TRIGGER,"HRichRingFindIPUSim object can not be initialized with values of another object!");
  ERROR_msg(HMessageMgr::DET_TRIGGER,"Default constructor will be called.");
  HRichRingFindIPUSim();
}
//============================================================================

//----------------------------------------------------------------------------
HRichRingFindIPUSim& HRichRingFindIPUSim::operator=(const HRichRingFindIPUSim& source)
{
  // Operator =
  // Does not work, creates ne objec.
  //
  // Input parameters:
  //   const HRichRingFindIPUSim& source
  //     Source object from which to copy, unused!
  //
  // Output parameters:
  //   none
  //
  // Return code:
  //  Pointer to newly created object, or this object.
  if (this != &source)
    {
      ERROR_msg(HMessageMgr::DET_TRIGGER,"HRichRingFindIPUSim object can not be assigned!");
      ERROR_msg(HMessageMgr::DET_TRIGGER,"Default constructor will be called.");
      return *(new HRichRingFindIPUSim());
    }
  return *this;
}
//============================================================================

//----------------------------------------------------------------------------
Int_t HRichRingFindIPUSim::Execute(HRichAnalysisIPUSim *giveMe)
{
  // Execute the job.
  //
  // Input parameters:
  //   const HRichRingFindIPUSim& source
  //     ???
  //
  // Output parameters:
  //   none
  //
  // Return code:
  //  Number of rings.
    iRingNr = HRichRingFindIPU::Execute();
//   for(Int_t i=0; i<iRingNr;++i)
// 		{
// 			pRings->AddLast(((HRichHitIPUSim*)pRings->At(i)));
// 		}
//   for(Int_t i=0;i<pRings->GetEntries();i++)
//   {
//     gHades->getMsg()->info(10,HMessageMgr::DET_TRIGGER,GetName()," after %d %d",((HRichHitIPU*)pRings->At(i))->getX(), ((HRichHitIPU*)pRings->At(i))->getY());
//   }

  if(iRingNr>0)
    {
      LookForTrack(giveMe);
    }
  return (iRingNr);
}
//============================================================================


//----------------------------------------------------------------------------

void HRichRingFindIPUSim::LookForTrack(HRichAnalysisIPUSim *showMe)
{
  // This method stores the track numbers for each ring and it stores maximally 3
  // tracks  for each ring. At this stage the initial and final track positions in
  // the catRichTrack array are stored.
  //
  // Input parameters:
  //   HRichAnalysisIPUSim *showMe
  //      ???
  //
  // Output patrameters:
  //   none
  //
  // Return code:
  //   none
  Int_t n,j,k,z;
#warning "ringHalfSize hardcoded to 8!"
  Int_t ringHalfSize = 8;

//   gHades->getMsg()->info(10,HMessageMgr::DET_TRIGGER,GetName(),"0x%x %d %d",pRings, iRingNr, pRings->GetEntries());

  for (n = 0; n < iRingNr; n++)
    {
	Int_t trackCount = 0;
	HRichHitIPUSimCont* ringCont=(HRichHitIPUSimCont*)pRings->At(n);
	HRichHitIPUSim*     ring=ringCont->getData();

	Int_t iNowX = ring->getX();
	Int_t iNowY = ring->getY();
	/*      gHades->getMsg()->info(10,HMessageMgr::DET_TRIGGER,GetName(),"x %d y %d",iNowX, iNowY);*/
	for (Int_t row = iNowY-ringHalfSize; (row <=iNowY+ringHalfSize) && (trackCount < 255); row++)
	{
	    for (Int_t col = iNowX-ringHalfSize; (col <= iNowX+ringHalfSize) && (trackCount < 255); col++)
	    {
		//     for(Int_t lnr=0; lnr < label_size; lnr++)  {
		//       Int_t row = iNowX + getLabelPattern(lnr,0);
		//       Int_t col = iNowY + getLabelPattern(lnr,1); // this would be the IPU mask

		if ((row>=0) && (row<=95) && (col>=0) && (col<=95))
		{ // boundaries of the pad plane
		    if (pads[Sector][row][col])
		    { // pad fired
			/*                      if(n<iRingNr-1)
			 cout << ((HRichHitIPUSim*)pRings->At(n+1))->getX() << endl;
			 gHades->getMsg()->info(10,HMessageMgr::DET_TRIGGER,GetName()," tracks: %d %d %d",showMe->getPadsNTrack1(col, row, Sector),showMe->getPadsNTrack2(),trackCount);
			 */
			ringCont->setRingPadNTrack1(trackCount,showMe->getPadsNTrack1(col, row, Sector));
			ringCont->setRingPadNTrack2(trackCount,showMe->getPadsNTrack2());

			trackCount++;
		    }
		}
	    }
	}
	ringCont->setRingPadNTrack1(trackCount,-1); // signals end of list
	ringCont->setRingPadNTrack2(trackCount,-1); // signals end of list
	j = k = z = 0;
	while((ringCont->getRingPadNTrack1(j)>=0) && (k < 255))
        {
	    ringCont->setRingTrack(k,showMe->getTrack(ringCont->getRingPadNTrack1(j)));
	    ringCont->setRingFlag(k,showMe->getFlag(ringCont->getRingPadNTrack1(j)));
	    k++;

	    z = ringCont->getRingPadNTrack2(j) - ringCont->getRingPadNTrack1(j);
	    // if z > 0 then it means that there is more then 1 track for the analyzed Ring
	    // In this case i loop over the other tracks and store them

	    while ((z>0) && (k < 255))
	    {
		ringCont->setRingTrack(k,showMe->getTrack(ringCont->getRingPadNTrack1(j)+z));
		ringCont->setRingFlag(k,showMe->getFlag(ringCont->getRingPadNTrack1(j)+z));

		k++;
		z--;
	    }
	    j++;
	}
	ringCont->setRingTrack(k,0); // signals end of list
    }
  /*    gHades->getMsg()->info(10,HMessageMgr::DET_TRIGGER,GetName(),"end of LookForTracks()");*/
}
//============================================================================
void HRichRingFindIPUSim::fillHit(Int_t row, Int_t column, Int_t quality, Int_t vetoQuality, Int_t nPads)
{
  HRichHitIPUSimCont  *ringCont = new HRichHitIPUSimCont;

  if(ringCont == NULL)
  {
    ERROR_msg(HMessageMgr::DET_TRIGGER,"No entry at the position!");
    return;
  }
  if ( iRingNr < 4999 )
  {
    iRingNr++;
  }
  else
  {
    INFO_msg(10,HMessageMgr::DET_TRIGGER,"iRingNr reached MAXIMUM!!!");
  }

  HRichHitIPUSim*     ring=ringCont->getData();

  // now i store the ring info
  ring->setX(column);
  ring->setY(row);
  ring->setQuality(quality);
  ring->setVetoQuality(vetoQuality);
  ring->setNPads(nPads);
  pRings->Add(ringCont);
}
