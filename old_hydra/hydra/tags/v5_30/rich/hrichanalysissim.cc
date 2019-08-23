//*-- Author : Laura Fabbietti
//*-- Modified : 08/05/2000 L.Fabbietti
//*-- Modified : Oct. 2000 W. Koenig (reduced content of HRichPadSignal)
///////////////////////////////////////////////////////
//______________________________________________________
//  HRichAnalysisSim
//  This class besides the Rich Analysis contains
//  the necessary functions to retrieve the parent track 
//  number corresponding to each pad that belongs to
//  a Ring structure.
//  The Ring Finding algorhytm is executed by the class
//  HRichRingFindSim. This class contains 2 different
//  algorhytms to find a ring, once a ring is found
//  the track numbers of the particles that have hit
//  the pads belonging to the ring structure are memorized
//  in an array. (Int_t *iRingTrack). 
//  Once all track numbers are collected, 
//  3 tracks number are saved ( HRichHitSim:track1....)
//  for each Hit with the corrispondent weights (cfr. updateHits).
//  (The weight
//  is the number of pads a photon or a Ip has fired.)
//  A flag for each of the three track numbers is saved too,
//  to be able to distinguish between a Chrenkov photon
//  and a direct hit.  
//////////////////////////////////////////////////////


#include "hrichanalysissim.h"
#include <iostream.h>
#include "hlocation.h"
#include "hades.h"
#include "hrichhitsim.h"
#include "hspectrometer.h"
#include "richdef.h"
#include "hevent.h"
#include "hiterator.h"
#include "hmatrixcategory.h"
#include "hlinearcategory.h"
#include "hrichringfindsim.h"
#include "hrichpadclean.h"
#include "hrichpadlabel.h"
#include "hrichcalsim.h"
#include "hrichtrack.h"

ClassImp(HRichAnalysisSim)

HRichAnalysisSim::HRichAnalysisSim(void){
 pRingFind = new HRichRingFindSim;
 
 cont = 0;
 secCount = 0;
 // pRings = NULL; // set by ringfinder
 pRichCalSim =  NULL;
}
HRichAnalysisSim::HRichAnalysisSim(Text_t *name,Text_t *title)
                                  : HRichAnalysis(name, title) {
 pRingFind = new HRichRingFindSim;

 secCount = 0;
 pRichCalSim =  NULL;
}
HRichAnalysisSim::~HRichAnalysisSim(void){

 if (pRingFind) delete pRingFind;
 pRingFind = NULL;

}
Bool_t HRichAnalysisSim::init() {  

    HRichDetector *pRichDet = (HRichDetector*)gHades->getSetup()
                                                    ->getDetector("Rich");

    m_pCalCat=gHades->getCurrentEvent()->getCategory(catRichCal);
    if (!m_pCalCat) {
      m_pCalCat=pRichDet->buildMatrixCat("HRichCalSim",1);

      if (!m_pCalCat) return kFALSE;
      else gHades->getCurrentEvent()
                        ->addCategory(catRichCal, m_pCalCat, "Rich");
    }
  
   fIter = (HIterator*)m_pCalCat->MakeIterator();
  
    m_pHitCat=gHades->getCurrentEvent()->getCategory(catRichHit);
    if (!m_pHitCat) {
      m_pHitCat=pRichDet->buildMatrixCat("HRichHitSim",1);

      if (!m_pHitCat) return kFALSE;
      else gHades->getCurrentEvent()
                        ->addCategory(catRichHit, m_pHitCat, "Rich");
    }

    m_pHitHdrCat=gHades->getCurrentEvent()->getCategory(catRichHitHdr);
    if (!m_pHitHdrCat) {
      m_pHitHdrCat=pRichDet->buildCategory(catRichHitHdr);

      if (!m_pHitHdrCat) return kFALSE;
      else gHades->getCurrentEvent()
                        ->addCategory(catRichHitHdr, m_pHitHdrCat, "Rich");
    }
     m_pTrackCat = gHades->getCurrentEvent()->getCategory(catRichTrack);
  if (!m_pTrackCat) {
      m_pTrackCat = pRichDet->buildCategory(catRichTrack);
     if (!m_pTrackCat) return kFALSE;
     else gHades->getCurrentEvent()->addCategory(catRichTrack,m_pTrackCat , "Rich");
  }

  if (initParameters() == kFALSE) return kFALSE;

  pRingFind->init(this);

  return kTRUE;
}

Int_t  HRichAnalysisSim::getPadsNTrack1(Int_t padx, Int_t pady,Int_t sec){
    //     This function returns for each pad the corresponding
    //     NTrack1 value (cfr. HRichCalSim, HRichDigitizer::execute).
    //     All track numbers are stored during digitization  in the
    //     catRichTrack Linear Category.
    //     NTrack1 is the index in the Track array corresponding to the
    //     first track for each pad.  

    //     This function is called from HRichRingFindSim::CalcRingParamters.

 HLocation loc1;
 loc1.set(3, sec, pady, padx);


 pRichCalSim = (HRichCalSim*)((HMatrixCategory*)getCalCat())->getObject(loc1);
 if (pRichCalSim) return pRichCalSim->getNTrack1();   
 return 0;
}
//----------------------------------------------------------------------------
Int_t  HRichAnalysisSim::getPadsNTrack2(){

    //   This functions returns NTrack2, which is the index in 
    //   the Track Array corresponding to the last track for each pad.
    //   Pad must be identified by calling getPadsNTrack1(...) first. 

 if (pRichCalSim) return pRichCalSim->getNTrack2();
 return 0;
}
//----------------------------------------------------------------------------
Int_t HRichAnalysisSim::getFlag(Int_t index) {
    //  This function returns the track number contained in the
    //  catRichTrack container at the position index.

    //  it is called from HRichRingFindSim::LookForTrack().
  
return  ((HRichTrack*)((HLinearCategory*)m_pTrackCat)->getObject(index))->getFlag();
}
 
//----------------------------------------------------------------------------
Int_t HRichAnalysisSim::getTrack(Int_t index) {
   //  This function returns the flag contained in the
   //  catRichTrack container at the position index.
  // This flag is 0 for Chrenkov photons and 1 for IP.
 Int_t track=((HRichTrack*)((HLinearCategory*)m_pTrackCat)->getObject(index))->getTrack();
 if (track == 0 ) cout<<"track in ringfinder is 0 "<<endl;
 return track;
}
 
//----------------------------------------------------------------------------
Int_t HRichAnalysisSim::execute() {
 
//   cont++;
//cout<<"Executing Analysis at evt =  "<<cont<<endl; 

 HRichCal *pCal;
  Int_t i, j, ampl;
  Int_t sec, padnr;
  
  Reset();

  for (i = 0; i < 6; i++) {
   if (fPadFired[i] > 0) {
    fPadFired[i] = 0;
    HRichPadSignal * pSecPads=pPads[i];
    for (j = 0; j < maxPads; pSecPads[j++].clear());
   }
  }

  Int_t iCutOffThresh=((HRichAnalysisPar*)fpAnalysisPar)->iCutOffThresheold;
  fIter->Reset();
  while((pCal = (HRichCal *)fIter->Next())) {
   if ( (ampl = (Int_t)pCal->getCharge()) > iCutOffThresh) {
       sec = pCal->getSector();
       fPadFired[sec]++;
       padnr = pCal->getCol() + pCal->getRow()*maxCols;
       pPads[sec][padnr].setAmplitude(ampl);       
   }
  }
  iEventNr++;
  if ( iEventNr%1000==0)   cout << "Next rich event goes... " << iEventNr << endl;
  
  // *************** modification L.F. && T.E. : Jan 05 2000 **************
  // modified by W. Koenig Oct. 2000, introducing the class member maxFiredTotalPads
  Int_t fPadFiredTot=0;
  for(i=6;i--;fPadFiredTot+=fPadFired[i]);
  if(fPadFiredTot >  maxFiredTotalPads) return 0;

  // **************************************************************
  // ------- loop over sectors --- begin ---
  for(secCount = 0; secCount < 6; secCount++) 
      if (((HRichGeometryPar*)fpGeomPar)->getSector(secCount) > 0) {
	  
	  Reset();
	  SetActiveSector(secCount);
	  iPadFiredNr = fPadFired[secCount];
	  
	  if (((HRichAnalysisPar*)fpAnalysisPar)->isActiveLabelPads == 0) {
	      iLabelNr = 1;
	      pLabelArea = new HRichLabel[1];
	      pLabelArea[0].iLeftX = 0;
	      pLabelArea[0].iRightX = maxCols;
	      pLabelArea[0].iLowerY = 0;
	      pLabelArea[0].iUpperY = maxRows;
	      pLabelArea[0].iLabeledPadsNr = maxPads;
	      pLabelArea[0].iFiredPadsNr = iPadFiredNr;
	  }
	  
	  
      iPadCleanedNr=pPadClean->Execute(this);
      iLabelNr=pPadLabel->Execute(this);
      iRingNr=pRingFind->Execute(this);
      iRingNrTot+=iRingNr;
      //if (iLabelNr > 0) {
      // cout<<iPadFiredNr<<" pads stored, "<<"cleaned="<<iPadCleanedNr<<" labels="<<iLabelNr<<" rings="<<iRingNr<<endl;
      // if(iRingNr>0) cout<<"Event "<<iEventNr<<" rings: "<<iRingNrTot<<endl;
      //}
	  
      updateHeaders(secCount, iEventNr);
      updateHits(secCount);
	
  }
  // ------- loop over sectors --- end ---

  return 0;
   
}

void HRichAnalysisSim::updateHits(Int_t nSec) {
    // Each Hit is stored, in addition to real data for simulated one
    // the tracks number corresponding to each ring are stored too.
    // Maximal 3 tracks are stored for each ring.
   HRichHitSim *hit=NULL;

  for (int i = 0; i < iRingNr; i++) {

      // HLocation loc;
      loc.set(1, nSec);
  

      hit=(HRichHitSim *)m_pHitCat->getNewSlot(loc);
      if (hit!=NULL) {
	HRichHitSim & ring = pRings[i];
        hit=new(hit) HRichHitSim;
 
        *hit = ring;
        hit->setSector(nSec);
//        hit->setEventNr(nEvNr);

        HRichPad * pad = ((HRichGeometryPar*)fpGeomPar)->getPadsPar()->
                         getPad(ring.iRingX,ring.iRingY);

	hit->setTheta(pad-> getTheta() );
        hit->setPhi(pad->getPhi(nSec));

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
	
	if(hit->track1==0) cout<<" Rich Track error"<<endl;
      }
      
  }//end loop on rings
            
}
      


