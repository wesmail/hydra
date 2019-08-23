#include "hrichanalysis.h"
#include "hruntimedb.h"
#include "hevent.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hcategory.h"
#include "hiterator.h"
#include "hdebug.h"
#include "hades.h"
#include "hrichdetector.h"
#include "hrichcal.h"
#include "hrichhit.h"
#include "hrichhitheader.h"
#include "richdef.h"
#include "hrichpadsignal.h"
#include "hrichpadclean.h"
#include "hrichpadlabel.h"
#include "hrichringfind.h"


#include <iostream.h>

ClassImp(HRichAnalysis)


//----------------------------------------------------------------------------
HRichAnalysis::HRichAnalysis() : HReconstructor() {

 clear();
 
 pPadClean = new HRichPadClean;
 pPadLabel = new HRichPadLabel;
 pRingFind = new HRichRingFind;

 pPads = NULL;
 fIter = NULL;
}
//============================================================================

//----------------------------------------------------------------------------
HRichAnalysis::HRichAnalysis(Text_t *name,Text_t *title)
                                  : HReconstructor(name, title) {
 clear();

 pPadClean = new HRichPadClean;
 pPadLabel = new HRichPadLabel;
 pRingFind = new HRichRingFind;

 pPads = NULL;
 fIter = NULL;
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
 if (pRings) delete [] pRings;
 pRings = NULL;
 if (pPads) {
   for (Int_t i = 0; i < 6; i++) if (pPads[i]) delete [] pPads[i]; 
   delete [] pPads;
 }
 delete rootTheta;
 delete theta;
 if (fIter) delete fIter;
}
//============================================================================

//----------------------------------------------------------------------------
void HRichAnalysis::clear() {
 iActiveAnalysis = 1;
 iActiveSector = 0;

 pLabelArea = NULL;
 pRings = NULL;

 iPadFiredNr = 0;
 for (Int_t i = 0; i < 6; i++) fPadFired[i] = 0;
 iPadCleanedNr = 0;
 iClusterCleanedNr = 0;
 iLabelNr = 0;
 iRingNr = 0;
 iEventNr = 0;

 iFakePad = 0;
 iFakeLocalMax4 = 0;
 iFakeLocalMax8 = 0;

 pRichDet = NULL;
}
//============================================================================

//----------------------------------------------------------------------------
HRichAnalysis::HRichAnalysis(const HRichAnalysis& source) {
 cerr << "HRichAnalysis object cannot be initialized with values of another object!\n";
// cerr << "Default constructor will be called.\n";
// HRichAnalysisPar();
// HRichAnalysis();
}
//============================================================================

//----------------------------------------------------------------------------
HRichAnalysis& HRichAnalysis::operator=(const HRichAnalysis& source) {
 if (this != &source) {
  cerr << "HRichAnalysis object can not be assigned!\n";
//  cerr << "Default constructor will be called.\n";
//  HRichAnalysisPar();
//  HRichAnalysis();
 }
return *this;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HRichAnalysis::init() {
  // ****************
    rootTheta= new TFile("richTheta.root","READ","");
    theta =(TVector*)rootTheta->Get("thetaVec");
    // ***************   
    HRichDetector *pRichDet = (HRichDetector*)gHades->getSetup()
                                                    ->getDetector("Rich");

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

    nSecCounter=0;//counts how many times Dec99 sector was fired;
    return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HRichAnalysis::initParameters() {

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

return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HRichAnalysis::finalize() {
    // cout<<"Sector 3 (in GEANT Sector 2) which was active in the Dec99 beamtime was fired in "<<nSecCounter<<" events"<<endl;// switch on only for ana of sim data
    rootTheta->Close();
    return kTRUE;
    
}
//============================================================================

//----------------------------------------------------------------------------
Int_t HRichAnalysis::execute() {
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
      // remember that columns are x and rows are y
      // cout << " DEBUG : loc 0, 1, 2: " << loc[0] << "," << loc[1] 
      //     << "," << loc[2] << " == " << pCal->getCharge() << endl;

  }

 //  if ((fPadFired[0]+fPadFired[1]+fPadFired[2]+fPadFired[3]+fPadFired[4]+fPadFired[5]) > 0)
//    cout << "PADS fired: " 
//     << (fPadFired[0]+fPadFired[1]+fPadFired[2]+fPadFired[3]+fPadFired[4]+fPadFired[5])
//      << endl;
  if ( kSecTest)  nSecCounter++;
  iEventNr++;
  if ( iEventNr%1000==0) cout << "Next rich event goes... " << iEventNr << endl;

  // *************** modification L.F. && T.E. : Jan 05 2000 **************
  // intended to skip events which have to many pads fired to be an interesting "ring candidate" event
  Int_t nPadThreshold = 1000; // upper limit of number of fired pads
  if( (fPadFired[0]+fPadFired[1]+fPadFired[2]+fPadFired[3]+fPadFired[4]+fPadFired[5]) > nPadThreshold) {
      // cout<<"Analysis of event skipped : too many pads fired (threshold : "<<nPadThreshold<<" )"<<endl;
    return 0;
  }
  // **************************************************************
  // ------- loop over sectors --- begin ---
  for(i = 0; i < 6; i++) 
   if (((HRichGeometryPar*)fpGeomPar)->getSector(i) > 0) {

   Reset();
   SetActiveSector(i);
   iPadFiredNr = fPadFired[i];

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

   // calculation of LAB position
   for (j = 0; j < iRingNr; j++) {
    loc.set(3, i, pRings[j].iRingY, pRings[j].iRingX);
    calcLabCoordinates(&pRings[j], loc);
   }
   updateHeaders(i, nEvNr);
   updateHits(i, nEvNr);
  }
  // ------- loop over sectors --- end ---

  return 0;
}
//============================================================================

//----------------------------------------------------------------------------
void HRichAnalysis::updateHits(Int_t nSec, Int_t nEvNr) {
  HRichHit *hit=NULL;

  for (int i = 0; i < iRingNr; i++) {
      // HLocation loc;
      loc.set(1, 0);

      hit=(HRichHit *)m_pHitCat->getNewSlot(loc);
      if (hit!=NULL) {
        hit=new(hit) HRichHit;

        *hit = pRings[i];
        hit->setSector(nSec);
        hit->setEventNr(nEvNr);
	hit->setTheta(theta);
      }
  }
}      
//============================================================================

//----------------------------------------------------------------------------
void HRichAnalysis::updateHeaders(Int_t nSec, Int_t nEvNr) {
  HRichHitHeader *hithdr = NULL;

  // HLocation loc;
  loc.set(1, 0);

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
Int_t HRichAnalysis::getPadsIndex(Int_t nSec, Int_t nRow, Int_t nCol) {
  HRichDetector *pRD = (HRichDetector*)pRichDet;  

  int offset = nSec * pRD->getRows() * pRD->getColumns();
  offset += nRow  * pRD->getColumns();
  offset += nCol;

  return offset;
}
//============================================================================

//----------------------------------------------------------------------------
Int_t* HRichAnalysis::getPadsPointer(Int_t* pPads, Int_t nSec) {
  int offset = getPadsIndex(nSec, 0, 0);
  return pPads + offset;
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
 if (pRings) {
  delete [] pRings;
  pRings = NULL;
 }
}
//============================================================================

//----------------------------------------------------------------------------
void HRichAnalysis::SetActive() { iActiveAnalysis = 1; }
//============================================================================

//----------------------------------------------------------------------------
Int_t HRichAnalysis::IsActive() { return iActiveAnalysis; }
//============================================================================

//----------------------------------------------------------------------------
Int_t HRichAnalysis::GetPadNr() { return ((HRichGeometryPar*)fpGeomPar)->getPadsNr(); }
//============================================================================
 
//----------------------------------------------------------------------------
Int_t HRichAnalysis::GetPadsXNr() { return ((HRichGeometryPar*)fpGeomPar)->getColumns(); }
//============================================================================

//----------------------------------------------------------------------------
Int_t HRichAnalysis::GetPadsYNr() { return ((HRichGeometryPar*)fpGeomPar)->getRows(); }
//============================================================================

//----------------------------------------------------------------------------
Int_t HRichAnalysis::GetActiveSector() { return iActiveSector; }
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
 } 
}
//============================================================================

//----------------------------------------------------------------------------
Int_t HRichAnalysis::SetNextSector() {
 Int_t i = iActiveSector;
 while (i < 6) {
  i++;
  if (((HRichGeometryPar*)fpGeomPar)->getSector(i) > 0) return (iActiveSector = i);
 }
 cerr << "No more sectors (last sector = " << iActiveSector << ")!\n";
return iActiveSector;
}
//============================================================================

//----------------------------------------------------------------------------
HRichPadSignal* HRichAnalysis::GetPad(Int_t padnr) {
 return &pPads[iActiveSector][padnr];
}
//============================================================================

//----------------------------------------------------------------------------
HRichPadSignal* HRichAnalysis::GetPad(Int_t padx, Int_t pady) {
 return &pPads[iActiveSector][padx + pady*((HRichGeometryPar*)fpGeomPar)->getColumns()]; 
}
//============================================================================

//----------------------------------------------------------------------------
Int_t HRichAnalysis::GetLabelNr() {
 return iLabelNr;
}
//============================================================================

//----------------------------------------------------------------------------
HRichLabel* HRichAnalysis::GetLabel(Int_t labelnr) {
 return &pLabelArea[labelnr];
}
//============================================================================

//----------------------------------------------------------------------------
Int_t HRichAnalysis::GetRingNr() {
 return iRingNr;
}
//============================================================================

//----------------------------------------------------------------------------
HRichHit* HRichAnalysis::GetRing(Int_t ringnr) {
 return &pRings[ringnr];
}
//============================================================================

//----------------------------------------------------------------------------
Int_t HRichAnalysis::IsOut(Int_t nowPad, Int_t dx = 0, Int_t dy = 0) {
 if (nowPad <= 0) return 1;
 Int_t x = nowPad % ((HRichGeometryPar*)fpGeomPar)->getColumns();
 Int_t y = nowPad / ((HRichGeometryPar*)fpGeomPar)->getColumns();

 if ((x+dx) >= 0 && (x+dx) < ((HRichGeometryPar*)fpGeomPar)->getColumns() && 
     (y+dy) >= 0 && (y+dy) < ((HRichGeometryPar*)fpGeomPar)->getRows() && 
     ((HRichGeometryPar*)getGeomPar())
      ->fPads.getPad(nowPad+dx + ((HRichGeometryPar*)fpGeomPar)->getColumns()*dy)
      ->getPadActive()) {
  return 0; }
 else {
  return 1; }
}
//============================================================================

//----------------------------------------------------------------------------
Int_t HRichAnalysis::IsOut(Int_t x, Int_t y, Int_t dx, Int_t dy) {
 if ((x+dx) >= 0 && (x+dx) < ((HRichGeometryPar*)fpGeomPar)->getColumns() && 
     (y+dy) >= 0 && (y+dy) < ((HRichGeometryPar*)fpGeomPar)->getRows() && 
     ((HRichGeometryPar*)getGeomPar())
      ->fPads.getPad(x+dx+((HRichGeometryPar*)fpGeomPar)->getColumns()*(y+dy))
      ->getPadActive()) {
  return 0; }
 else {
  return 1; }
}
//============================================================================

//----------------------------------------------------------------------------
void HRichAnalysis::calcLabCoordinates(HRichHit* hit, HLocation &fLoc) {
  HGeomVector vLocalCoord;
  HGeomVector vLabCoord;

  ((HRichGeometryPar*)fpGeomPar)->getLocalCoord(fLoc, vLocalCoord);
  ((HRichGeometryPar*)fpGeomPar)->getLabCoord(fLoc, vLabCoord);

  hit->setLocalXY(vLocalCoord.getX(), vLocalCoord.getY());
  hit->setLabXYZ(vLabCoord.getX(), vLabCoord.getY(), vLabCoord.getZ());
}
//============================================================================







