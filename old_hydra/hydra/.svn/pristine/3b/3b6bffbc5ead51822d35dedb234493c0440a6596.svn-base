//*-- Author : Laura Fabbietti
//*-- Modified : 08/05/2000 L.Fabbietti
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
#include "hrichdetector.h"
#include "hades.h"
#include "hrichcalsim.h"
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
  HRichAnalysis::clear();
 
 pPadClean = new HRichPadClean;
 pPadLabel = new HRichPadLabel;
 pRingFind = new HRichRingFindSim;

 pPads = NULL;
 fIter = NULL;
 
 cont = 0;
 secCount = 0;
 pRings = NULL;
}
HRichAnalysisSim::HRichAnalysisSim(Text_t *name,Text_t *title)
                                  : HRichAnalysis(name, title) {
 HRichAnalysis::clear();

 pPadClean = new HRichPadClean;
 pPadLabel = new HRichPadLabel;
 pRingFind = new HRichRingFindSim;

 pRings = NULL;
 pPads = NULL;
 fIter = NULL;
  secCount = 0;
}
HRichAnalysisSim::~HRichAnalysisSim(void){

 if (pRingFind) delete pRingFind;
 pRingFind = NULL;
 if (pRings) delete [] pRings;
 pRings = NULL;


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

  // now creating pads array

    Int_t i,j;
    pPads = new HRichPadSignal * [6];
    for (j = 0; j < 6; j++) {
      pPads[j] = new HRichPadSignal[((HRichGeometryPar*)fpGeomPar)
                                                     ->getPadsNr()];
      for (i = 0; i < ((HRichGeometryPar*)fpGeomPar)->getPadsNr(); i++) {
        pPads[j][i].setSector(j);
        pPads[j][i].setPadNr(i);
        pPads[j][i].CalcNrtoXY(((HRichGeometryPar*)fpGeomPar)->getColumns());
      }
    }

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

 HRichCalSim * pRichCalSim =  NULL;
 HLocation loc1;
 loc1.set(3, sec, pady, padx);


  pRichCalSim = (HRichCalSim*)((HMatrixCategory*)getCalCat())->getObject(loc1);
  if (pRichCalSim){
      return ((HRichCalSim*)((HMatrixCategory*)getCalCat())->getObject(loc1))->getNTrack1();   
  }
  else return 0;
 return 0;
}
//----------------------------------------------------------------------------
Int_t  HRichAnalysisSim::getPadsNTrack2(Int_t padx, Int_t pady,Int_t sec){

    //   This functions returns NTrack2, which is the index in 
    //   the Track Array corresponding to the last track for each pad. 

 HRichCalSim * pRichCalSim =  NULL;
 HLocation loc1;
 loc1.set(3, sec, pady, padx);


  pRichCalSim = (HRichCalSim*)((HMatrixCategory*)getCalCat())->getObject(loc1);
  if (pRichCalSim){
      return ((HRichCalSim*)((HMatrixCategory*)getCalCat())->getObject(loc1))->getNTrack2();
  }
  else return 0;
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
 if (((HRichTrack*)((HLinearCategory*)m_pTrackCat)->getObject(index))->getTrack()==0 ) cout<<"track in ringfinder is 0 "<<endl;
  return  ((HRichTrack*)((HLinearCategory*)m_pTrackCat)->getObject(index))->getTrack();
}
 
//----------------------------------------------------------------------------
Int_t HRichAnalysisSim::execute() {
 
//   cont++;
  //cout<<"Executing Analysis at evt =  "<<cont<<endl; 

 HRichCal *pCal;
  Int_t nEvNr=0, i, j, ampl;
  Int_t sec, col, row, padnr;
  
  Bool_t kSecTest=kFALSE; // modification Jan 09 2000 E.T.
  Reset();

  for (i = 0; i < 6; i++) 
    if (fPadFired[i] > 0) {
      for (j = 0; j < ((HRichGeometryPar*)fpGeomPar)->getPadsNr(); j++)
        pPads[i][j].clear();
      fPadFired[i] = 0;
    }

  fIter->Reset();

  while((pCal = (HRichCal *)fIter->Next())) {

   nEvNr = pCal->getEventNr();
   //   loc = fIter->getLocation();  // native iterator does not know about loc
   //   sec = loc[0];
   //   col = loc[2];
   //   row = loc[1];
   sec = pCal->getSector();
   if (sec == 2) kSecTest=kTRUE; // in how many events the Dec99 sector fired?
   col = pCal->getCol();
   row = pCal->getRow();
   //   loc.set(3,sec,row,col);
   if ( (ampl = (Int_t)pCal->getCharge()) > 
        ((HRichAnalysisPar*)fpAnalysisPar)->iCutOffThresheold ) {
       padnr = col + row*((HRichGeometryPar*)fpGeomPar)->getColumns();
       fPadFired[sec]++;
       pPads[sec][padnr].setEventNr(nEvNr);
       pPads[sec][padnr].setSector(sec);
       pPads[sec][padnr].setCol(col);
       pPads[sec][padnr].setRow(row);
       pPads[sec][padnr].CalcXYtoNr(((HRichGeometryPar*)fpGeomPar)->getColumns());
       pPads[sec][padnr].setAmplitude(ampl);
       
   }
   
   
  }
  iEventNr++;
  if ( iEventNr%1000==0)   cout << "Next rich event goes... " << iEventNr << endl;
  
  // *************** modification L.F. && T.E. : Jan 05 2000 **************
  Int_t nPadThreshold = 1000; // upper limit of number of fired pads
  if( (fPadFired[0]+fPadFired[1]+fPadFired[2]+fPadFired[3]+fPadFired[4]+fPadFired[5]) > nPadThreshold) {
      return 0;
  }
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
	      pLabelArea[0].iRightX = ((HRichGeometryPar*)fpGeomPar)->getColumns();
	      pLabelArea[0].iLowerY = 0;
	      pLabelArea[0].iUpperY = ((HRichGeometryPar*)fpGeomPar)->getRows();
	      pLabelArea[0].iLabeledPadsNr = ((HRichGeometryPar*)fpGeomPar)->getPadsNr();
	      pLabelArea[0].iFiredPadsNr = iPadFiredNr;
	  }
	  
	  
	  pPadClean->Execute(this);
	  pPadLabel->Execute(this);
	  pRingFind->Execute(this);
	  
	  updateHeaders(secCount, nEvNr);
	  updateHits(secCount, nEvNr);
	
  }
  // ------- loop over sectors --- end ---

  return 0;
   
}

void HRichAnalysisSim::updateHits(Int_t nSec, Int_t nEvNr) {
    // Each Hit is stored, in addition to real data for simulated one
    // the tracks number corresponding to each ring are stored too.
    // Maximal 4 tracks are stored for each ring.
   HRichHitSim *hit=NULL;

  for (int i = 0; i < iRingNr; i++) {

      // HLocation loc;
      loc.set(1, 0);
  

      hit=(HRichHitSim *)m_pHitCat->getNewSlot(loc);
      if (hit!=NULL) {

        hit=new(hit) HRichHitSim;
 
        *hit = pRings[i];
        hit->setSector(nSec);

        hit->setEventNr(nEvNr);
	hit->setTheta( ((HRichGeometryPar*)fpGeomPar)-> getPadsPar()-> getPad(pRings[i].iRingX,pRings[i].iRingY)-> getTheta() );


        Float_t phiRot = (((HRichGeometryPar*)fpGeomPar)->getPadsPar()->getPad(pRings[i].iRingX,pRings[i].iRingY)->getPhi(nSec)); 
        hit->setPhi(phiRot);
        

        Int_t k = 0;
        while(hit->iRingTrack[k]){
	  
	  if (hit->track1==0) {
	    hit->track1 = hit->iRingTrack[k];
	    hit->flag1 =  hit->iRingFlag[k];
	    (hit->weigTrack1)++;
	  }  
	  else{
	    if(hit->iRingTrack[k]== hit->track1){
	      (hit->weigTrack1)++;

              
	    }
	    else {
	      if(hit->track2==0) {
		hit->track2 = hit->iRingTrack[k];
		hit->flag2 =  hit->iRingFlag[k];
		(hit->weigTrack2)++;
	      }
	      else{
		if(hit->iRingTrack[k]== hit->track2){
		  (hit->weigTrack2)++;

		}
		else {
		  if (hit->track3==0) {
		    hit->track3 = hit->iRingTrack[k];
		    hit->flag3 =  hit->iRingFlag[k];
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
      


