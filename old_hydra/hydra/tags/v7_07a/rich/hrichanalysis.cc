//*-- Author : W. Przygoda
//*-- Modified : Tue Feb 15 18:50:45 CET 2005 martin.jurkovic@ph.tum.de
//*-- Modified : Tue Feb 12 16:54:10 CET 2002 teberl@ph.tum.de
//*--            introduced ctr flag to skip events
using namespace std;
#include "hades.h"
#include "hcategory.h"
#include "hdebug.h"
#include "hevent.h"
#include "heventheader.h"
#include "hlinearcategory.h"
#include "hrichanalysis.h"
#include "hrichcal.h"
#include "hrichhit.h"
#include "hrichhitheader.h"
#include "hrichpadclean.h"
#include "hrichpadlabel.h"
#include "hrichringfind.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "richdef.h"

#include <iostream> 
#include <iomanip>

ClassImp(HRichAnalysis)

// Modified Oct. 2000 by W. Koenig (W.Koenig@gsi.de)
//*-- Modified : 13/05/2001 by Laura Fabbietti
//----------------------------------------------------------------------------
HRichAnalysis::HRichAnalysis() : HReconstructor() {

 clear();
 
 pPadClean = new HRichPadClean;
 pPadLabel = new HRichPadLabel;
 pRingFind = new HRichRingFind;

}
//============================================================================

//----------------------------------------------------------------------------
HRichAnalysis::HRichAnalysis(Text_t *name,Text_t *title,Bool_t kSkip)
                                  : HReconstructor(name, title) {
 clear();

 pPadClean = new HRichPadClean;
 pPadLabel = new HRichPadLabel;
 pRingFind = new HRichRingFind;
 kSkipEvtIfNoRing = kSkip;
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
 iClustersCleaned.Reset();
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
Bool_t HRichAnalysis::reinit() {

    initParameters();

    pRingFind->init(this);


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

    fIterHitHeader = (HIterator*)m_pHitHdrCat->MakeIterator("native");
    //cout << "Made IterHitHeader: " << fIterHitHeader << endl;
    HRuntimeDb* rtdb=gHades->getRuntimeDb();
    HRichAnalysisPar *pAnalysisPar = (HRichAnalysisPar*)rtdb->
                                         getContainer("RichAnalysisParameters");
    setAnalysisPar(pAnalysisPar);
    if (pAnalysisPar == NULL) return kFALSE;

    HRichGeometryPar *pGeomPar = (HRichGeometryPar*)rtdb->
                                  getContainer("RichGeometryParameters");

    setGeomPar(pGeomPar);
    if (pGeomPar == NULL) return kFALSE;

    pPadClean->init();

    return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HRichAnalysis::initParameters() {

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
//    //cout<<"row="<<row<<" min,max="<<pLeftBorder[row]<<", "<<pRightBorder[row]<<"\n";
  }
  maxPads=maxRows*maxCols;

  //  //cout<<"maxCols="<<maxCols<<", maxRows="<<maxRows<<", maxPads="<<maxPads<<endl;

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

  secWithCurrent = -1;
  hithdrLoop = NULL;
  Int_t iRingNrTotEvt;
  iRingNrTotEvt = 0;

  HRichCal *pCal;
  Int_t i, j, ampl;
  Int_t sec, padnr;

  //Bool_t kSecTest=kFALSE; 
  Reset();

  //cout << "In execute after reset" << endl;

  for (i = 0; i < 6; i++) {
   if (fPadFired[i] > 0) {
    fPadFired[i] = 0;
    HRichPadSignal * pSecPads=pPads[i];
    for (j = 0; j < maxPads; pSecPads[j++].clear());
   }
  }

   fIter->Reset();
  
   //cout << "In execute after iter->reset" << endl;

  while((pCal = (HRichCal *)fIter->Next())) {

    if ( (ampl = (Int_t)pCal->getCharge()) > 0){
	  sec = pCal->getSector();
	  fPadFired[sec]++;
	  padnr = pCal->getCol() + pCal->getRow()*maxCols;
	  pPads[sec][padnr].setAmplitude(ampl);
      }
      // remember that columns are x and rows are y

  }
  //cout << "In execute after setamplitude" << endl;

  Int_t fPadFiredTot=0;
  for(i=6;i--;fPadFiredTot+=fPadFired[i]);

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
  //cout << "In execute before sector loop" << endl;
  for(i = 0; i < 6; i++) 
   if (((HRichGeometryPar*)fpGeomPar)->getSector(i) > 0) {

   Reset();
   SetActiveSector(i);
   iPadFiredNr = fPadFired[i];

  
   //for each sector the cleaning, labeling, ring finding procedure are
   // executed
   iPadCleanedNr=pPadClean->Execute(this);
   iLabelNr=pPadLabel->Execute(this);

  //cout << "In execute after padlabel->execute" << endl;

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

   iRingNr=pRingFind->Execute(this);
   //cout << "In execute after ringfinder" << endl;
   iRingNrTot+=iRingNr;
   // increment total nb of rings in evt
   iRingNrTotEvt+=iRingNr;
   //  cout <<"Event "<<iEventNr<<" pads: "<<iPadFiredNr<<" rings: "<<iRingNrTot<<endl;
   if (iRingNr > 0) {
     
     if (iRingNr>1) {
       ++sectorPairNrTot;
     }
   }
   //the found rings are stored in the hit container 
//    updateHeaders(i, iEventNr);
   //cout << "In execute before header update" << endl;
   updateHeaders(i, gHades->getCurrentEvent()->getHeader()->getEventSeqNumber());
   updateHits(i);
  }
  //cout << "In execute after hit update" << endl;
  // ------- loop over sectors --- end ---
  if(iRingNrTot>lastRingNr+1) {
    ++allPairNrTot;
    //cout<<"total pairs: "<<allPairNrTot<<endl;
  }
// modification to skip event which does not contain any ring
  if (kSkipEvtIfNoRing && iRingNrTotEvt==0) return kSkipEvent;
  return 0;
}
//============================================================================

//----------------------------------------------------------------------------
void HRichAnalysis::updateHits(Int_t nSec) {

  HRichHit *hit=NULL;

  for (int i = 0; i < iRingNr; i++) {
      //HLocation loc;
      loc.set(1, nSec);

      hit=(HRichHit *)m_pHitCat->getNewSlot(loc);

      if (hit!=NULL) {
        hit=new(hit) HRichHit;

        *hit = pRings[i];
//      hit->setEventNr(nEvNr);
        hit->setSector(nSec);
	if (hit->fX >1000.)
	  cout << "fX : " <<hit->fX<<", "<<hit->iRingX<<" fY : "<<hit->fY<<", "<<hit->iRingY<<endl;

        HRichPad * pad = ((HRichGeometryPar*)fpGeomPar)->getPadsPar()->
                         getPad(pRings[i].iRingX,pRings[i].iRingY);

        // now fX and fY is something else - mean cluster x,y
//        pad->getXY(&hit->fX, &hit->fY);
//        pad->getXYZlab(nSec, &hit->fLabX, &hit->fLabY, &hit->fLabZ);
        hit->fPhi = pad->getPhi(nSec);
        hit->fTheta = pad->getTheta();

      }
  }
}      
//============================================================================

//----------------------------------------------------------------------------
void HRichAnalysis::updateHeaders(const Int_t nSec, Int_t nEvNr) {
  HRichHitHeader *hithdr = NULL;
  //cout << "fiterhitheader: " << fIterHitHeader << endl;
  fIterHitHeader->Reset();
  hithdrLoop = NULL;
 
  while((hithdrLoop = (HRichHitHeader *)fIterHitHeader->Next())) {
    secWithCurrent = hithdrLoop->getSector();
    if(nSec==secWithCurrent) {
      hithdr = hithdrLoop;
      break;
    }
  }

  if(hithdr==NULL){
    HLocation loc1;
    loc1.set(1, nSec);
    hithdr=(HRichHitHeader *)(m_pHitHdrCat)->getNewSlot(loc1);
    if (hithdr!=NULL)  hithdr=new(hithdr) HRichHitHeader;
  }

  if (hithdr!=NULL) {
      hithdr->setSector(nSec);
      hithdr->setEventNr(gHades->getCurrentEvent()->getHeader()->getEventSeqNumber());
      hithdr->setExecutedAnalysis(1);
      hithdr->iPadFiredNr = iPadFiredNr;
      hithdr->iPadCleanedNr = iPadCleanedNr;
      hithdr->iClusterCleanedNr = iClusterCleanedNr;
      hithdr->iClustersCleaned = iClustersCleaned;
      hithdr->iRingNr = iRingNr;
      hithdr->iFakePad = iFakePad;
      hithdr->iFakeLocalMax4 = iFakeLocalMax4;
      hithdr->iFakeLocalMax8 = iFakeLocalMax8;
  }
  else cout<< "No Header Object could be created"<<endl;
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
      R__b >> pRings;
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
      R__b << pRings;
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
//============================================================================

//----------------------------------------------------------------------------
Int_t HRichAnalysis::IsOut(Int_t nowPad, Int_t dx, Int_t dy) {
 if (nowPad <= 0) return 1;
 Int_t x = nowPad % maxCols;
 Int_t y = nowPad / maxCols;

 if ((x+dx) >= 0 && (x+dx) < maxCols &&
     (y+dy) >= 0 && (y+dy) < maxRows &&
     iPadActive[nowPad+dx + maxCols*dy]) {
  return 0; }
 else {
  return 1; }
}
//============================================================================

//----------------------------------------------------------------------------
Int_t HRichAnalysis::IsOut(Int_t x, Int_t y, Int_t dx, Int_t dy) {
 if ((x+dx) >= 0 && (x+dx) < maxCols &&
     (y+dy) >= 0 && (y+dy) < maxRows &&
     iPadActive[x+dx + maxCols*(y+dy)]) {
  return 0; }
 else {
  return 1; }
}
//============================================================================












