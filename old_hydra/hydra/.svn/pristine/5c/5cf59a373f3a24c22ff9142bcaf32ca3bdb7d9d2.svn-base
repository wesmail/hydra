#include "hshowertofinocorrelator.h"
#include "hruntimedb.h"
#include "hevent.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hshowerdetector.h"
#include "htofinodetector.h"
#include "hcategory.h"
#include "hmatrixcatiter.h"
#include "hlocation.h"
#include "hshowerhit.h"
#include "htofinocal.h"
#include "hshowerhittof.h"
#include "hdebug.h"
#include "hades.h"
#include "hiterator.h"
#include "showerdef.h"
#include "htofinocalpar.h"
#include "htofinoshowermap.h"
#include "hlinearcategory.h"
#include "tofinodef.h"
#include "showertofinodef.h"
#include "iostream.h"

ClassImp(HShowerTofinoCorrelator)

///////////////////////////////////////////////////////////////////////
//HShowerTofinoCorrelator
//
// HShowerTofinoCorrelator reconstructor searches data describing
// the same pad in HShowerHit and HTofinoCal categories
// and joins them into one object. 
// Results are stored in HShowerHitTof category.
//
//////////////////////////////////////////////////////////////////////

HShowerTofinoCorrelator::HShowerTofinoCorrelator(Text_t *name,Text_t *title) :
  HReconstructor(name,title)
{
   fHitIter=NULL;
   fTofinoIter=NULL;
   fHitTofinoIter=NULL;
   m_pTofinoCalPar = NULL;
   m_pTofShowerMap = NULL;
   m_zeroLoc.set(0);
}

HShowerTofinoCorrelator::HShowerTofinoCorrelator()
{
   fHitIter=NULL;
   fTofinoIter=NULL;
   fHitTofinoIter=NULL;
   m_pTofinoCalPar = NULL;
   m_pTofShowerMap = NULL;
   m_zeroLoc.set(0);
}


HShowerTofinoCorrelator::~HShowerTofinoCorrelator(void) {
     if (fHitIter) delete fHitIter;
     if (fTofinoIter) delete fTofinoIter;
     if (fHitTofinoIter) delete fHitTofinoIter;
}

Bool_t HShowerTofinoCorrelator::init() {
  // creates the ShowerHit, ShowerTof and HTofinoCal categories 
  // and adds them to the current event,
  // creates an iterator which loops over all local maxima
  // and an iterator which loops over Tofino information.

    printf("initialization of Shower/Tofino correlator\n");
    
    HShowerDetector *pShowerDet = (HShowerDetector*)gHades->getSetup()
                                                  ->getDetector("Shower");
    HTofinoDetector *pTofinoDet = (HTofinoDetector*)gHades->getSetup()
                                                  ->getDetector("Tofino");
    HRuntimeDb* rtdb=gHades->getRuntimeDb();

    m_pTofinoCat=gHades->getCurrentEvent()->getCategory(catTofinoCal);
    if (!m_pTofinoCat) {
      m_pTofinoCat=pTofinoDet->buildCategory(catTofinoCal);

      if (!m_pTofinoCat) return kFALSE;
      else gHades->getCurrentEvent()
                 ->addCategory(catTofinoCal, m_pTofinoCat, "Tofino");
    }

    m_pHitCat=gHades->getCurrentEvent()->getCategory(catShowerHit);
    if (!m_pHitCat) {
      m_pHitCat=pShowerDet->buildCategory(catShowerHit);

      if (!m_pHitCat) return kFALSE;
      else gHades->getCurrentEvent()
                 ->addCategory(catShowerHit, m_pHitCat, "Shower");
    }

    m_pHitTofCat=gHades->getCurrentEvent()->getCategory(catShowerHitTof);
    if (!m_pHitTofCat) {
      m_pHitTofCat = new HLinearCategory("HShowerHitTof", 1000);
      if (!m_pHitTofCat) return kFALSE;
      else gHades->getCurrentEvent()
                 ->addCategory(catShowerHitTof, m_pHitTofCat, "Tofino");
    }

    m_pTofinoCalPar = rtdb->getContainer("TofinoCalPar");
    if (!m_pTofinoCalPar) return kFALSE;
    m_pTofShowerMap = rtdb->getContainer("TofinoShowerMap");
    if (!m_pTofShowerMap) return kFALSE;

    fHitIter=(HIterator*)m_pHitCat->MakeIterator();
    fTofinoIter=(HIterator*)m_pTofinoCat->MakeIterator();
    fHitTofinoIter=(HIterator*)m_pHitTofCat->MakeIterator();

    return kTRUE;
}

Bool_t HShowerTofinoCorrelator::finalize(void) {
   return kTRUE;
}


HShowerHitTof* HShowerTofinoCorrelator::addHitTof(HShowerHit* pHit) {
// create one object, which contains data
// from HShowerHit and connect with it data from Tofino
  
    HShowerHitTof *pHitTof;

    pHitTof=(HShowerHitTof *)m_pHitTofCat->getNewSlot(m_zeroLoc);
    if (pHitTof!=NULL) {
      pHitTof=new(pHitTof) HShowerHitTof;

      *pHitTof = *pHit;
      pHitTof->setTof(-200.);
      pHitTof->setADC(-200.);
      pHitTof->setDriftTime(-200.);
      pHitTof->setTofinoCell(9);
      pHitTof->setTofinoMult(0.);
    }
  
    return pHitTof;
}

Int_t HShowerTofinoCorrelator::execute()
{
  HShowerHit *pHit;
  HShowerHitTof *pHitTof;
  HTofinoCal *pTCal;
  HTofinoCal *pTofCalTab[24];
  Int_t nMult[24]; //multiplicity in shower detector covered by one Tofino cell
  Int_t nRightMax = 31; // max correlated Shower col with Tofino cell
  Int_t nLeftMin = 0; // min corelated Shower col with Tofino cell
  Int_t nSector, nRow, nTofCell; 
  Int_t nTofIndex;
  Int_t nTofCellLeft, nTofCellRight, nNC;
  Float_t fTime, fDriftTime, fTof, fAdc;

  fHitIter->Reset();
  fTofinoIter->Reset();

  //clearing tables before analysis next event
  for (Int_t i = 0; i < 24; i++) {
    nMult[i] = 0;
    pTofCalTab[i] = NULL;
  }

  //mapping data to table indexed by sector,cell combination
  while((pTCal = (HTofinoCal*)fTofinoIter->Next())) {
    pTofCalTab[pTCal->getSector()*4+pTCal->getCell()] = pTCal;
  }

  while((pHit = (HShowerHit*)fHitIter->Next())) {
    if (pHit->getModule()!=0) continue; //correlation only for first module

     //looking for Tofino cell covering pad row, col
    	nTofCell = ((HTofinoShowerMap*)m_pTofShowerMap)->
    	getCellNumber(pHit->getRow(), pHit->getCol());
    	if (nTofCell==9) continue; //empty entry in map

          //cout <<"nTofCell = " <<nTofCell <<endl;
		  
    nNC = 1;
    nTofCellRight = nTofCellLeft = nTofCell;
    nSector = pHit->getSector();
	

    if (!pTofCalTab[nSector*4 + nTofCell]) {
     //try in cells on the right
	    if(pHit->getCol() < nRightMax)
     		nTofCellRight = ((HTofinoShowerMap*)m_pTofShowerMap)->
				getCellNumber(pHit->getRow(), pHit->getCol()+1);
        
     //try in cells on the left
	  	if(pHit->getCol() > nLeftMin) 
      		nTofCellLeft = ((HTofinoShowerMap*)m_pTofShowerMap)->
				getCellNumber(pHit->getRow(), pHit->getCol()-1);
    }

    if ((nTofCellRight != 9) && (nTofCellRight != nTofCell)) {
    	if (pTofCalTab[nSector*4 + nTofCellRight]) {
			nTofCell = nTofCellRight;
			nNC = -1;
      }
    }
 
    if ((nTofCellLeft != 9) && (nTofCellLeft != nTofCell)) {
    	if (pTofCalTab[nSector*4 + nTofCellLeft]) {
			nTofCell = nTofCellLeft;
			nNC = -1;
      }
    }

    nSector = pHit->getSector();
    nRow = pHit->getRow();
    nTofIndex = nSector*4 + nTofCell; 
    fDriftTime = ((HTofinoCalPar*)m_pTofinoCalPar)->
    calcDriftTime(nSector, nTofCell, (Float_t)nRow);
    if(fDriftTime <= 0.) continue;

    if (pTofCalTab[nTofIndex]) {
		fTime = pTofCalTab[nTofIndex]->getTime();
		if(fTime <= 0.) continue;
		fAdc = pTofCalTab[nTofIndex]->getCharge();

			if((fTime - fDriftTime) > 0.) {
				HShowerHitTof *pHitTof;
				pHitTof=(HShowerHitTof *)m_pHitTofCat->getNewSlot(m_zeroLoc);

				if (pHitTof!=NULL) {
					pHitTof=new(pHitTof) HShowerHitTof;
					*pHitTof = *pHit;

					pHitTof->setTof(fTime - fDriftTime);
					pHitTof->setDriftTime(fDriftTime);
					pHitTof->setADC(fAdc);
					pHitTof->setSector(pHit->getSector());
					pHitTof->setTofinoCell(nTofCell);
					nMult[nTofIndex]++;
				} //if (pHitTof!=NULL) 
      		} //if(fTime > 0. ...
    } //if (pTofCalTab[nTofIndex])
  } // while((pHit = (HShowerHit*)fHitIter->Next()))

  //updating multiplicity information
  fHitTofinoIter->Reset();
  while((pHitTof = (HShowerHitTof *)fHitTofinoIter->Next())) {
    fTof = pHitTof->getTof();
    nTofIndex = pHitTof->getSector()*4 +  pHitTof->getTofinoCell();
    pHitTof->setTofinoMult(nMult[nTofIndex]);
  } //while((pHitTof = (HShowerHitTof *)fHitTofinoIter->Next()))

  return 0;
}






