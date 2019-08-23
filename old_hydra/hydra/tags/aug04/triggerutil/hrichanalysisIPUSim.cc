using namespace std;
#include "hrichanalysisIPUSim.h"
#include "hdebug.h"
#include "hades.h"
#include "hcategory.h"
#include "hiterator.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "hevent.h"
#include "hrichcalsim.h"
#include "hrichhitIPUSim.h"
#include "hmatrixcatiter.h"
#include "hrichringfindIPUSim.h"
#include "richutildef.h"
#include "hmatrixcategory.h"
#include "hlinearcategory.h"
#include "hlocation.h"
#include "hrichdetector.h"
#include "htriggerdetector.h"
#include "triggerinfodef.h"
#include "hrichtrack.h"

#include <iostream>          
#include <iomanip>


//---------------------------------------------------------------------------- 
HRichAnalysisIPUSim::HRichAnalysisIPUSim() { 
  pHardRingFindSim = new HRichRingFindIPUSim;
  pRichCalSim =  NULL;
}       
//============================================================================

//---------------------------------------------------------------------------- 

HRichAnalysisIPUSim::HRichAnalysisIPUSim(Text_t *name,Text_t *title)
               :HRichAnalysisIPU(name, title) {
  pHardRingFindSim = new HRichRingFindIPUSim;
  pRichCalSim =  NULL;
}  
//============================================================================
 
//---------------------------------------------------------------------------- 

HRichAnalysisIPUSim::~HRichAnalysisIPUSim(void) {
 if (pHardRingFindSim) delete pHardRingFindSim;
 pHardRingFindSim = NULL;
}
//============================================================================
 
//----------------------------------------------------------------------------
Bool_t HRichAnalysisIPUSim::init() { 
  HRichDetector * pRichDet = (HRichDetector*)gHades->getSetup()->getDetector("Rich");
  HTriggerDetector * pTriggerDet = (HTriggerDetector*)gHades->getSetup()->getDetector("Trigger");
  
  //allocate input categories
  m_pRawCat=gHades->getCurrentEvent()->getCategory(catRichCal);
  if (!m_pRawCat) {
     m_pRawCat=pRichDet->buildMatrixCat("HRichCalSim",1);
     
    if (!m_pRawCat) return kFALSE;
    else gHades->getCurrentEvent()
	   ->addCategory(catRichCal, m_pRawCat, "Rich");
  }
  fIter = (HIterator*)m_pRawCat->MakeIterator("native");
    
  
  //allocate output categories
  m_pHardHitCat=gHades->getCurrentEvent()->getCategory(catHardRichHit);
  if (!m_pHardHitCat) {
    m_pHardHitCat=pTriggerDet->buildLinearCat("HRichHitIPUSim");
    if (!m_pHardHitCat) return kFALSE;
    else gHades->getCurrentEvent()
	   ->addCategory(catHardRichHit, m_pHardHitCat, "Trigger");
  }

  m_pTrackCat = gHades->getCurrentEvent()->getCategory(catRichTrack);  
  if (!m_pTrackCat) {
    m_pTrackCat = pRichDet->buildCategory(catRichTrack);
    if (!m_pTrackCat) return kFALSE;
    else gHades->getCurrentEvent()->addCategory(catRichTrack,m_pTrackCat , "Rich");
  }
  
  HRuntimeDb* rtdb=gHades->getRuntimeDb();
  
  if(rtdb){  
    HRichGeometryPar *pGeomPar = (HRichGeometryPar*)rtdb->getContainer("RichGeometryParameters");
    setGeomPar(pGeomPar);
    if (pGeomPar == NULL) return kFALSE;
  }

  pHardRingFindSim->init(this);
  
  return kTRUE;
}             
//============================================================================
 
//---------------------------------------------------------------------------- 

Bool_t HRichAnalysisIPUSim::finalize() {
  return kTRUE;
}     
//============================================================================

//---------------------------------------------------------------------------- 

Int_t HRichAnalysisIPUSim::execute() {  
  HRichCalSim *pCalSim; 
  
  pHardRingFindSim->ZeroArray();
  
  fIter->Reset();
  while((pCalSim = (HRichCalSim *)fIter->Next()) )   {
    pHardRingFindSim->FiredPad(pCalSim->getSector(),pCalSim->getRow(),pCalSim->getCol());
  }
  
  iRingNrTot=0;
  // ------- loop over sectors --- begin ---
  for(int i = 0; i < 6; i++) {
    pHardRingFindSim->SetSector(i);
    iRingNr = pHardRingFindSim->Execute(this);
    iRingNrTot+=iRingNr;  
    if (iRingNr) updateHits(i);
  }
  // ------- loop over sectors --- end ---
  
  return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
void HRichAnalysisIPUSim::updateHits(Int_t nSec) {

  HRichHitIPUSim *hit=NULL; 

  //  cout << " HardRich::updateHits " <<  iRingNr << endl;
  
  for (int i = 0; i < iRingNr; i++) {
      loc.set(1, nSec);

      hit=(HRichHitIPUSim *)m_pHardHitCat->getNewSlot(loc);

      if (hit!=NULL) {
        hit=new(hit) HRichHitIPUSim;

	HRichHitIPUSim & ring = pRings[i];
 
        *hit = ring;
	hit->setSector(nSec);

      // mapping according to RICH 
        HRichPad * pad = ((HRichGeometryPar*)fpGeomPar)->getPadsPar()->
                         getPad(ring.iRingX,ring.iRingY);

        hit->fPhi = pad->getPhi(nSec);
        hit->fTheta = pad->getTheta();

	// here i write the track number with its weight
        Int_t k = 0;
        while(ring.iRingTrack[k]){
	  
	  if (hit->track1==0) {
	    hit->track1 = ring.iRingTrack[k];
	    hit->flag1 =  ring.iRingFlag[k];
	    (hit->weigTrack1)++;
	  } else{
	    if(ring.iRingTrack[k]== hit->track1){
	      (hit->weigTrack1)++;

	    } else {
	      if(hit->track2==0) {
		hit->track2 = ring.iRingTrack[k];
		hit->flag2 =  ring.iRingFlag[k];
		(hit->weigTrack2)++;
	      } else{
		if(ring.iRingTrack[k]== hit->track2){
		  (hit->weigTrack2)++;

		} else {
		  if (hit->track3==0) {
		    hit->track3 = ring.iRingTrack[k];
		    hit->flag3 =  ring.iRingFlag[k];
		    (hit->weigTrack3)++;
		  }
		  else (hit->weigTrack3)++;
		}
	      }
	    }  
	  }
	  k++;
	  
        }//end loop on track array
	
      }
  }
}      
//============================================================================

//----------------------------------------------------------------------------
Int_t  HRichAnalysisIPUSim::getPadsNTrack1(Int_t padx, Int_t pady,Int_t sec){
    //     This function returns for each pad the corresponding
    //     NTrack1 value (cfr. HRichCalSim, HRichDigitizer::execute).
    //     All track numbers are stored during digitization  in the
    //     catRichTrack Linear Category.
    //     NTrack1 is the index in the Track array corresponding to the
    //     first track for each pad.  
    //     This function is called from HRichRingFindIPUSim::LookForTrack

 HLocation loc1;
 loc1.set(3, sec, pady, padx);

 pRichCalSim = (HRichCalSim*)((HMatrixCategory*)getRawCat())->getObject(loc1);
 if (pRichCalSim) return pRichCalSim->getNTrack1();   
 return 0;
}
//============================================================================

//----------------------------------------------------------------------------
Int_t  HRichAnalysisIPUSim::getPadsNTrack2(){

    //   This functions returns NTrack2, which is the index in 
    //   the Track Array corresponding to the last track for each pad.
    //   Pad must be identified by calling getPadsNTrack1(...) first. 

 if (pRichCalSim) return pRichCalSim->getNTrack2();
 return 0;
}
//============================================================================

//----------------------------------------------------------------------------
Int_t HRichAnalysisIPUSim::getFlag(Int_t index) {
  //  it is called from HRichRingFindIPUSim::LookForTrack().
  //  This function returns the flag contained in the
  //  catRichTrack container at the position index.
  //  This flag is 0 for Cherenkov photons and 1 for IP.
  
    HRichTrack *t = (HRichTrack*)((HLinearCategory*)m_pTrackCat)->getObject(index);
    Int_t i = -1;
    if (t) i=t->getFlag();
    return  i;
}
//============================================================================

//----------------------------------------------------------------------------
Int_t HRichAnalysisIPUSim::getTrack(Int_t index) {
    //  This function returns the track number contained in the
    //  catRichTrack container at the position index.

    HRichTrack *trk = ((HRichTrack*)((HLinearCategory*)m_pTrackCat)
		       ->getObject(index));
    Int_t track=-1;
    if(trk)  track=trk->getTrack();
    return track;

}
 //============================================================================

//----------------------------------------------------------------------------

ClassImp(HRichAnalysisIPUSim)


















































































