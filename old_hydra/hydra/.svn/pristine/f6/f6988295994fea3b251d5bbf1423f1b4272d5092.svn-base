//*-- Author : Laura Fabbietti
//*-- Modified : 08/05/2000 L.Fabbietti
//*-- Modified : Oct. 2000 W. Koenig (reduced content of HRichPadSignal)
//*-- Modified : Tue Feb 12 16:54:10 CET 2002 teberl@ph.tum.de
//*--            introduced ctr flag to skip events
///////////////////////////////////////////////////////
//______________________________________________________
//  HRichAnalysisSim
//  This class besides the Rich Analysis contains
//  the necessary functions to retrieve the parent track 
//  number corresponding to each pad that belongs to
//  a Ring structure.
//  The Ring Finding algorithm is executed by the class
//  HRichRingFindSim. This class contains 2 different
//  algorithms to find a ring, once a ring is found
//  the track numbers of the particles that have hit
//  the pads belonging to the ring structure are memorized
//  in an array. (Int_t *iRingTrack). 
//  Once all track numbers are collected, 
//  3 tracks number are saved ( HRichHitSim:track1....)
//  for each Hit with the corresponding weights (cfr. updateHits).
//  (The weight
//  is the number of pads a photon or a Ip has fired.)
//  A flag for each of the three track numbers is saved too,
//  to be able to distinguish between a Chrenkov photon
//  and a direct hit.  
//
//  use kSkip in the constructor to skip writing of 
//  event if no ring was found

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
#include "hruntimedb.h"

ClassImp(HRichAnalysisSim)

HRichAnalysisSim::HRichAnalysisSim(void){
 pRingFindSim = new HRichRingFindSim;
 
 cont = 0;
 secCount = 0;
 // pRings = NULL; // set by ringfinder
 pRichCalSim =  NULL;
}
HRichAnalysisSim::HRichAnalysisSim(Text_t *name,Text_t *title, Bool_t kSkip)
                                  : HRichAnalysis(name, title, kSkip) {
 pRingFindSim = new HRichRingFindSim;
 kSkipEvtIfNoRing = kSkip;
 secCount = 0;
 pRichCalSim =  NULL;
}
HRichAnalysisSim::~HRichAnalysisSim(void){

 if (pRingFindSim) delete pRingFindSim;
 pRingFindSim = NULL;

}
Bool_t HRichAnalysisSim::init() {  

//allocate input/output categories

  
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
	
	// ????????????????????????????????????????????????????????????????????
	m_pHitCat=pRichDet->buildMatrixCat("HRichHitSim",1); //et why Matrix ?!
	// ????????????????????????????????????????????????????????????????????
	// why does it crash (in updateHits: getNewSlot)
	// when a Linear Cat is used ?
	// The structure is the same as for HRichHit and there a lin cat 
	// is used
	
	//cout<<"address of cat rich hit sim"<<m_pHitCat<<endl;
	
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
     
     HRuntimeDb* rtdb=gHades->getRuntimeDb();
     HRichAnalysisPar *pAnalysisPar = (HRichAnalysisPar*)rtdb->
	 getContainer("RichAnalysisParameters");
     if (pAnalysisPar == NULL) {
	 pAnalysisPar = new HRichAnalysisPar;
      rtdb->addContainer(pAnalysisPar);
     }
     setAnalysisPar(pAnalysisPar);
     if (pAnalysisPar == NULL) return kFALSE;
     
     HRichGeometryPar *pGeomPar = (HRichGeometryPar*)rtdb->
	 getContainer("RichGeometryParameters");
     
     if (pGeomPar == NULL) {
	 pGeomPar = new HRichGeometryPar;
	 rtdb->addContainer(pGeomPar);
     }
     setGeomPar(pGeomPar);
     if (pGeomPar == NULL) return kFALSE;
     if (initParameters() == kFALSE) return kFALSE;
     
     return kTRUE;
     
}
Bool_t HRichAnalysisSim::initParameters() {

//allocate non event by event classes
       HRichGeometryPar *pGeomPar = getGeometryPar();                                  
    
    iRingNrTot=0;
    allPairNrTot=0;
    sectorPairNrTot=0;
    maxFiredTotalPads = 3000; // upper limit of number of fired pads
    maxFiredSectorPads =1000; // hardwired, needs to be set by an access function
    
    maxCols = pGeomPar->getColumns();
    maxRows = pGeomPar->getRows();
    //  maxPads = ((HRichGeometryPar*)fpGeomPar)->getPadsNr();
    if(pLeftBorder) delete [] pLeftBorder;
    pLeftBorder = new short[maxRows];
    if(pRightBorder) delete [] pRightBorder;
    pRightBorder = new short[maxRows];
    
    for (int row=0;row<maxRows;++row) {
    Int_t col=0;
    Int_t padOffset = row*maxCols;
    while(!pGeomPar->fPads.getPad(col+padOffset)->getPadActive() && col<maxCols) ++col;
    if(col==maxCols) {
      maxRows=row;
      break;
    }
    pLeftBorder[row]=col;
    while(pGeomPar->fPads.getPad(col+padOffset)->getPadActive() && col<maxCols) ++col;
    pRightBorder[row]=col-1;
    }
    maxPads=maxRows*maxCols;
    
    
    // now creating pads array
    
    Int_t i;
    pPads = new HRichPadSignal * [6];
    for (Int_t j = 0; j < 6; j++) {
     pSectorPads = pPads[j] = new HRichPadSignal[maxPads];
     for (i = 0; i < maxPads; pSectorPads[i++].clear());
    }
    pSectorPads=pPads[iActiveSector];
    
    return kTRUE;
}
Bool_t HRichAnalysisSim::reinit() {
       initParameters();

    pRingFindSim->init(this);


    iPadActive.Set(maxCols*maxRows);
    for (int i=0 ; i<maxCols*maxRows; i++)
     if (((HRichGeometryPar*)fpGeomPar)->getPadsPar()->getPad(i)->getPadActive() >0)
        iPadActive[i] = 1; else iPadActive[i] = 0;

  Int_t i,j,k,m,n;

    Int_t iMatrixSurface=0, iPartSurface=0;
    Int_t iMaskSize = ((HRichAnalysisPar*)fpAnalysisPar)->iRingMaskSize;
    Int_t iMaskSizeSquared = iMaskSize*iMaskSize;
    for (k = 0; k < iMaskSizeSquared; k++)
     if (((HRichAnalysisPar*)fpAnalysisPar)->iRingMask[k] == 1) iMatrixSurface++;


      for (j = 0; j < maxRows; j++)
       for (i = 0; i < maxCols; i++)   {
        iPartSurface = 0;
        for (k = 0; k < iMaskSizeSquared; k++) {
         m = (k % iMaskSize) - iMaskSize/2;
         n = (k / iMaskSize) - iMaskSize/2;
         if (!IsOut(i,j,m,n))
          if (((HRichAnalysisPar*)fpAnalysisPar)->iRingMask[k] == 1) iPartSurface++;
        }
        ((HRichGeometryPar*)fpGeomPar)->getPadsPar()->
          getPad(i,j)->setAmplitFraction((Float_t)iPartSurface/(Float_t)iMatrixSurface);
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
    //  it is called from HRichRingFindSim::LookForTrack().
     //  This function returns the flag contained in the
   //  catRichTrack container at the position index.
  // This flag is 0 for Chrenkov photons and 1 for IP.

    HRichTrack *t = (HRichTrack*)((HLinearCategory*)m_pTrackCat)->getObject(index);
    Int_t i = -1;
    if (t) i=t->getFlag();
    return  i;
}
 
//----------------------------------------------------------------------------
Int_t HRichAnalysisSim::getTrack(Int_t index) {
    //  This function returns the track number contained in the
    //  catRichTrack container at the position index.

    HRichTrack *trk = ((HRichTrack*)((HLinearCategory*)m_pTrackCat)
		       ->getObject(index));
    Int_t track=-1;
    if(trk)  track=trk->getTrack();
    return track;

}
 
//----------------------------------------------------------------------------
Int_t HRichAnalysisSim::execute() {
 
       // count total nb of rings in event
  Int_t iRingNrTotEvt;
  iRingNrTotEvt = 0;

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

  fIter->Reset();
  while((pCal = (HRichCal *)fIter->Next())) {
      
    if ( (ampl = (Int_t)pCal->getCharge()) > 0){
	  sec = pCal->getSector();
	  fPadFired[sec]++;
	  padnr = pCal->getCol() + pCal->getRow()*maxCols;
	  pPads[sec][padnr].setAmplitude(ampl);  
     
   }
  }
 
  iEventNr++;
  // if ( iEventNr%1000==0)   cout << "Next rich event goes... " << iEventNr << endl;

  Int_t fPadFiredTot=0;
  for(i=6;i--;fPadFiredTot+=fPadFired[i]);
  if(fPadFiredTot >  maxFiredTotalPads) return 0;
  for (i=0; i<6; i++)
      if (fPadFired[i] >= maxFiredSectorPads) {
	  Warning("execute","To many fired pads in sector %i. %i/%i",i,fPadFired[i],maxFiredSectorPads);
	  return 0;
      }
  // **************************************************************
  // ------- loop over sectors --- begin ---
  for(secCount = 0; secCount < 6; secCount++) 
      if (((HRichGeometryPar*)fpGeomPar)->getSector(secCount) > 0) {
	  
	  Reset();
	  SetActiveSector(secCount);
	  iPadFiredNr = fPadFired[secCount];
	 
	  
	  
      iPadCleanedNr=pPadClean->Execute(this);
      iLabelNr=pPadLabel->Execute(this);
      if (((HRichAnalysisPar*)fpAnalysisPar)->isActiveLabelPads == 0) {
	  iLabelNr = 1;
	  pLabelArea = new HRichLabel[1];
	  pLabelArea[0].iLeftX = 0;
	  pLabelArea[0].iRightX = maxCols;
	  pLabelArea[0].iLowerY = 0;
	  pLabelArea[0].iUpperY = maxRows;
	  pLabelArea[0].iLabeledPadsNr = maxPads;
	  pLabelArea[0].iFiredPadsNr = iPadFiredNr;
	  pLabelArea[0].iSignature = 0;
      }

      iRingNr=pRingFindSim->Execute(this);
      iRingNrTot+=iRingNr;
      // increment total nb of rings in evt
      iRingNrTotEvt+=iRingNr;
      updateHeaders(secCount, iEventNr);
      updateHits(secCount);
	
  }
  // ------- loop over sectors --- end ---
  // modification to skip event which does not contain any ring
  if (kSkipEvtIfNoRing && iRingNrTotEvt==0) return kSkipEvent;

  return 0;
   
}

void HRichAnalysisSim::updateHits(Int_t nSec) {
    // Each Hit is stored, in addition to real data for simulated one
    // the tracks number corresponding to each ring are stored too.
    // Maximal 3 tracks are stored for each ring.
   HRichHitSim *hit=NULL;
  for (int i = 0; i < iRingNr; i++) {

      loc.set(1, nSec);


  

      hit=(HRichHitSim *)m_pHitCat->getNewSlot(loc,NULL);
      if (hit!=NULL) {
        hit=new(hit) HRichHitSim;

	HRichHitSim & ring = pRings[i];
 
        *hit = ring;
        hit->setSector(nSec);

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
	
	//if(hit->track1==0) Error("updateHits(Int_t nSec)","rich hit contains no GEANT track nb information");
      }
      
  }//end loop on rings
            
}
      















