#include "hrichanalysis.h"
#include "hruntimedb.h"
#include "hevent.h"
#include "hspectrometer.h"
#include "hcategory.h"
#include "hiterator.h"
#include "hdebug.h"
#include "hades.h"
#include "hrichcal.h"
#include "hrichhit.h"
#include "hrichhitheader.h"
#include "richdef.h"
#include "hrichpadclean.h"
#include "hrichpadlabel.h"
#include "hrichringfind.h"


#include <iostream.h>

ClassImp(HRichAnalysis)

// Modified Oct. 2000 by W. Koenig (W.Koenig@gsi.de)
//----------------------------------------------------------------------------
HRichAnalysis::HRichAnalysis() : HReconstructor() {

 clear();
 
 pPadClean = new HRichPadClean;
 pPadLabel = new HRichPadLabel;
 pRingFind = new HRichRingFind;

}
//============================================================================

//----------------------------------------------------------------------------
HRichAnalysis::HRichAnalysis(Text_t *name,Text_t *title)
                                  : HReconstructor(name, title) {
 clear();

 pPadClean = new HRichPadClean;
 pPadLabel = new HRichPadLabel;
 pRingFind = new HRichRingFind;

}
//============================================================================

//----------------------------------------------------------------------------
HRichAnalysis::~HRichAnalysis() {
 if (pPadClean) delete pPadClean;
 pPadClean = NULL;
 if (pPadLabel) delete pPadLabel;
 pPadLabel = NULL;
 if (pRingFind) delete pRingFind;
 pRingFind = NULL;

 if (pLabelArea) delete [] pLabelArea;
 pLabelArea = NULL;

 if (pPads) {
  for (Int_t i = 0; i < 6; i++)
   if (pPads[i]) delete [] pPads[i]; 
  delete [] pPads;
 }
 if(pLeftBorder) delete [] pLeftBorder;
 if(pRightBorder) delete [] pRightBorder;

 if (fIter) delete fIter;
}
//============================================================================

//----------------------------------------------------------------------------
void HRichAnalysis::clear() {
 iActiveAnalysis = 1;
 iActiveSector = 0;

 pLabelArea = NULL;
 pRings = NULL;
 pPads = NULL;
 fIter = NULL;
 pLeftBorder = NULL;
 pRightBorder = NULL;

 iPadFiredNr = 0;
 for (Int_t i = 0; i < 6; i++) fPadFired[i] = 0;
 iPadCleanedNr = 0;
 iClusterCleanedNr = 0;
 iLabelNr = 0;
 iRingNr = 0;
 iRingNrTot = 0;
 sectorPairNrTot = 0;
 allPairNrTot = 0;
 iEventNr = 0;

 iFakePad = 0;
 iFakeLocalMax4 = 0;
 iFakeLocalMax8 = 0;

}
//============================================================================

//----------------------------------------------------------------------------
HRichAnalysis::HRichAnalysis(const HRichAnalysis& source) {
 cerr << "HRichAnalysis object can not be initialized with values of another object!\n";
// cerr << "Default constructor will be called.\n";
// HRichAnalysisPar();
// HRichAnalysis();
}
//============================================================================

//----------------------------------------------------------------------------
HRichAnalysis& HRichAnalysis::operator=(const HRichAnalysis& source) {
 if (this != &source) {
  cerr << "HRichAnalysis object can not be assigned!\n";
 }
return *this;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HRichAnalysis::init() {

//allocate input/output categories

    HRichDetector * pRichDet = (HRichDetector*)gHades->getSetup()->getDetector("Rich");

    m_pCalCat=gHades->getCurrentEvent()->getCategory(catRichCal);
    if (!m_pCalCat) {
      m_pCalCat=pRichDet->buildCategory(catRichCal);

      if (!m_pCalCat) return kFALSE;
      else gHades->getCurrentEvent()
                        ->addCategory(catRichCal, m_pCalCat, "Rich");
    }
    fIter = (HIterator*)m_pCalCat->MakeIterator("native");

    m_pHitCat=gHades->getCurrentEvent()->getCategory(catRichHit);
    if (!m_pHitCat) {
      m_pHitCat=pRichDet->buildCategory(catRichHit);

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

    if (initParameters() == kFALSE) return kFALSE;

    pRingFind->init(this);

 return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HRichAnalysis::initParameters() {

//allocate non event by event classes

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

  iRingNrTot=0;
  allPairNrTot=0;
  sectorPairNrTot=0;
  maxFiredTotalPads = 3000; // upper limit of number of fired pads
  maxFiredSectorPads =1000; // hardwired, needs to be set by an access function

  maxCols = ((HRichGeometryPar*)fpGeomPar)->getColumns();
  maxRows = ((HRichGeometryPar*)fpGeomPar)->getRows();
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
//    cout<<"row="<<row<<" min,max="<<pLeftBorder[row]<<", "<<pRightBorder[row]<<"\n";
  }
  maxPads=maxRows*maxCols;

  cout<<"maxCols="<<maxCols<<", maxRows="<<maxRows<<", maxPads="<<maxPads<<endl;

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
//============================================================================

//----------------------------------------------------------------------------
Bool_t HRichAnalysis::finalize() {
    return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
Int_t HRichAnalysis::execute() {
  HRichCal *pCal;
  Int_t i, j, ampl;
  Int_t sec, padnr;

  //Bool_t kSecTest=kFALSE; 
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

//   nEvNr = pCal->getEventNr();

      // all the pads fired overcoming the cutofftrheshold are stored.

   if ( (ampl = (Int_t)pCal->getCharge()) > iCutOffThresh) {
     sec = pCal->getSector();
     fPadFired[sec]++;
     padnr = pCal->getCol() + pCal->getRow()*maxCols;
     pPads[sec][padnr].setAmplitude(ampl);
   }
   // remember that columns are x and rows are y

  }

  Int_t fPadFiredTot=0;
  for(i=6;i--;fPadFiredTot+=fPadFired[i]);

  iEventNr++;

  if( fPadFiredTot > maxFiredTotalPads) {
       cout<<"Analysis of event skipped : too many pads fired (threshold : "<<maxFiredTotalPads<<" )"<<endl;
       return 0;
  }
  for (i=0; i<6; i++)
    if (fPadFired[i] >= maxFiredSectorPads) {
      Warning("execute","To many fired pads in sector %i. %i/%i",i,fPadFired[i],maxFiredSectorPads);
      return 0;
    }
  // **************************************************************

  // ------- loop over sectors --- begin ---
  Int_t lastRingNr=iRingNrTot;
  for(i = 0; i < 6; i++) 
   if (((HRichGeometryPar*)fpGeomPar)->getSector(i) > 0) {

   Reset();
   SetActiveSector(i);
   iPadFiredNr = fPadFired[i];

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

   //for each sector the cleaning, labeling, ring finding procedure are
   // executed
   iPadCleanedNr=pPadClean->Execute(this);
   iLabelNr=pPadLabel->Execute(this);
   iRingNr=pRingFind->Execute(this);
   iRingNrTot+=iRingNr;

   if (iRingNr > 0) {
//     cout <<"Event "<<iEventNr<<" pads: "<<iPadFiredNr<<" rings: "<<iRingNrTot<<endl;
     if (iRingNr>1) {
       ++sectorPairNrTot;
       Int_t dx = pRings[0].iRingX-pRings[1].iRingX;
       Int_t dy = pRings[0].iRingY-pRings[1].iRingY;
       float dQ =(float)(pRings[0].iRingQuality-pRings[1].iRingQuality)/(float)(pRings[0].iRingQuality+pRings[1].iRingQuality);
//	 cout<<" sector pair: "<<sectorPairNrTot<<" pads="<<pRings[0].iRingPadNr<<", "<<pRings[1].iRingPadNr<<" delta quality="<<dQ<<" dx="<<sqrt(dx*dx+dy*dy);
//	 cout<<" centroid: "<<pRings[0].centroid<<", "<<pRings[1].centroid<<endl; 
     }
   }
   //the found rings are stored in the hit container 
   updateHeaders(i, iEventNr);
   updateHits(i);
  }
  // ------- loop over sectors --- end ---
  if(iRingNrTot>lastRingNr+1) {
    ++allPairNrTot;
//    cout<<"total pairs: "<<allPairNrTot<<endl;
  }


  return 0;
}
//============================================================================

//----------------------------------------------------------------------------
void HRichAnalysis::updateHits(Int_t nSec) {

  HRichHit *hit=NULL;

  for (int i = 0; i < iRingNr; i++) {
      // HLocation loc;
      loc.set(1, nSec);

      hit=(HRichHit *)m_pHitCat->getNewSlot(loc);

      if (hit!=NULL) {
        hit=new(hit) HRichHit;

        *hit = pRings[i];
//      hit->setEventNr(nEvNr);
        hit->setSector(nSec);
        HRichPad * pad = ((HRichGeometryPar*)fpGeomPar)->getPadsPar()->
                         getPad(pRings[i].iRingX,pRings[i].iRingY);
        pad->getXY(&hit->fX, &hit->fY);
//        pad->getXYZlab(nSec, &hit->fLabX, &hit->fLabY, &hit->fLabZ);
        hit->fPhi = pad->getPhi(nSec);
        hit->fTheta = pad->getTheta();

      }
  }
}      
//============================================================================

//----------------------------------------------------------------------------
void HRichAnalysis::updateHeaders(Int_t nSec, Int_t nEvNr) {
  HRichHitHeader *hithdr = NULL;

  // HLocation loc;
  loc.set(1, nSec);

  hithdr=(HRichHitHeader *)m_pHitHdrCat->getNewSlot(loc);
  if (hithdr!=NULL) {
      hithdr=new(hithdr) HRichHitHeader;

      hithdr->setSector(nSec);
      hithdr->setEventNr(nEvNr);

      hithdr->iPadFiredNr = iPadFiredNr;
      hithdr->iPadCleanedNr = iPadCleanedNr;
      hithdr->iClusterCleanedNr = iClusterCleanedNr;
      hithdr->iClustersCleaned = iClustersCleaned;
      hithdr->iRingNr = iRingNr;

      hithdr->iFakePad = iFakePad;
      hithdr->iFakeLocalMax4 = iFakeLocalMax4;
      hithdr->iFakeLocalMax8 = iFakeLocalMax8;
  }
}

//============================================================================
  
//----------------------------------------------------------------------------
void HRichAnalysis::Streamer(TBuffer &R__b) {
// Stream an object of class HRichAnalysis.
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(); if (R__v) { }
      R__b >> iPadFiredNr;
      R__b >> iPadCleanedNr;
      R__b >> iRingNr;
      //R__b >> pRings;
      R__b >> iClusterCleanedNr;
      iClustersCleaned.Streamer(R__b);
      R__b >> iFakePad;
      R__b >> iFakeLocalMax4;
      R__b >> iFakeLocalMax8;
   } else {
      R__b.WriteVersion(HRichAnalysis::IsA());
      R__b << iPadFiredNr;
      R__b << iPadCleanedNr;
      R__b << iRingNr;
      //R__b << pRings;
      R__b << iClusterCleanedNr;
      iClustersCleaned.Streamer(R__b);
      R__b << iFakePad;
      R__b << iFakeLocalMax4;
      R__b << iFakeLocalMax8;
   }
}
//============================================================================

//----------------------------------------------------------------------------
void HRichAnalysis::Reset() {
 iPadFiredNr = 0;
 iPadCleanedNr = 0;
 iClusterCleanedNr = 0;
 iClustersCleaned.Reset();
 iLabelNr = 0;
 iRingNr = 0;
 iFakePad = 0;
 iFakeLocalMax4 = 0;
 iFakeLocalMax8 = 0;

 if (pLabelArea) {
  delete [] pLabelArea;
  pLabelArea = NULL; 
 }
}
//============================================================================

//----------------------------------------------------------------------------
void HRichAnalysis::SetActiveSector(Int_t sectornr) {
 if (sectornr == iActiveSector) return;
 if (sectornr > 5 || sectornr < 0) {
  cerr << "Error in <HRichAnalysis::SetActiveSector>: "
       << "Sector number = " << sectornr << " out of range (0..5)!\n";
  return;
 } else if (((HRichGeometryPar*)fpGeomPar)->getSector(sectornr) == 0) {
  cerr << "Error in <HRichAnalysis::SetActiveSector>: "
       << "Sector number  = " << sectornr << " is not present (and cannot be active)!\n";
  return;
 } else {
  iActiveSector = sectornr;
  pSectorPads=pPads[sectornr];
 } 
}
//============================================================================

//----------------------------------------------------------------------------
Int_t HRichAnalysis::SetNextSector() {
 Int_t i = iActiveSector;
 while (i < 6) {
  i++;
  if (((HRichGeometryPar*)fpGeomPar)->getSector(i) > 0) {
	  pSectorPads=pPads[i];
	  return (iActiveSector = i);
  }
 }
 cerr << "No more sectors (last sector = " << iActiveSector << ")!\n";
return iActiveSector;
}











