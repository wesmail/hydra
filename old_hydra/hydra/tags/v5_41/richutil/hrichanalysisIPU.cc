#include "hrichanalysisIPU.h"
#include "hdebug.h"
#include "hades.h"
#include "hcategory.h"
#include "hiterator.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "hevent.h"
#include "hrichcal.h"
#include "hrichhit.h"
#include "hmatrixcatiter.h"
#include "hrichringfindIPU.h"
#include "richutildef.h"

#include <iostream.h>         

HRichAnalysisIPU::HRichAnalysisIPU() { 
    fIter = NULL;
    fIterH = NULL;
    nCounter = 0;
    nEvtCounter = 0;
    nCounterH = 0;
    nEvtCounterH = 0;
    iActiveSector = 0;

    pRings = NULL;
    iRingNr = 0;
    iRingNrTot = 0;
   
    pHardRingFind = new HRichRingFindIPU;
}       
//============================================================================
 
//---------------------------------------------------------------------------- 

HRichAnalysisIPU::HRichAnalysisIPU(Text_t *name,Text_t *title)
               : HReconstructor(name, title) {
    fIter = NULL;
    fIterH = NULL;
    nCounter = 0;
    nEvtCounter = 0;
    nCounterH = 0;
    nEvtCounterH = 0;
    iActiveSector = 0;

    pRings = NULL;
    iRingNr = 0;
    iRingNrTot = 0;
   
    pHardRingFind = new HRichRingFindIPU;
}  
//============================================================================
 
//---------------------------------------------------------------------------- 

HRichAnalysisIPU::~HRichAnalysisIPU(void) {
 if (pHardRingFind) delete pHardRingFind;
 pHardRingFind = NULL;

 if (fIter) delete fIter; 
 if (fIterH) delete fIterH;
}
//============================================================================
 
//---------------------------------------------------------------------------- 
HRichAnalysisIPU::HRichAnalysisIPU(const HRichAnalysisIPU& source) {
 cerr << "HRichAnalysisIPU object can not be initialized with values of another object!\n";
}
//============================================================================

//----------------------------------------------------------------------------
HRichAnalysisIPU& HRichAnalysisIPU::operator=(const HRichAnalysisIPU& source) {
 if (this != &source) {
  cerr << "HRichAnalysisIPU object can not be assigned!\n";
 }
return *this;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HRichAnalysisIPU::init() { 
//allocate input categories
      HRichDetector * pRichDet = (HRichDetector*)gHades->getSetup()->getDetector("Rich");

    m_pCalCat=gHades->getCurrentEvent()->getCategory(catRichCal);
    if (!m_pCalCat) {
      m_pCalCat=pRichDet->buildCategory(catRichCal);

      if (!m_pCalCat) return kFALSE;
      else gHades->getCurrentEvent()
                        ->addCategory(catRichCal, m_pCalCat, "Rich");
    }
     fIter = (HIterator*)m_pCalCat->MakeIterator("native");

//allocate output categories
    m_pHardHitCat=gHades->getCurrentEvent()->getCategory(catHardRichHit);
    if (!m_pHardHitCat) {
      m_pHardHitCat=pRichDet->buildCategory(catHardRichHit);
      if (!m_pHardHitCat) return kFALSE;
      else gHades->getCurrentEvent()
	     ->addCategory(catHardRichHit, m_pHardHitCat, "Rich");
    }
    fIterH = (HIterator*)m_pHardHitCat->MakeIterator("native");
 
    pHardRingFind->init(this);

    return kTRUE;
}             
//============================================================================
 
//---------------------------------------------------------------------------- 

Bool_t HRichAnalysisIPU::finalize() {
    return kTRUE;
}     
//============================================================================
 
//---------------------------------------------------------------------------- 

Int_t HRichAnalysisIPU::execute() {  
   HRichCal *pCal; 
   HRichHitIPU *pHardHit; 

////  this is just to see if the container is empty at the beginning 
//    nCounterH=0;
//     fIterH->Reset();
//     while((pHardHit = (HRichHitIPU *)fIterH->Next()))   {
//       nCounterH++;
//     } 
//     nEvtCounter++;
//     cout << "evt nb: " << nEvtCounter << " # of rings in pHardHit " << nCounterH << endl;


   pHardRingFind->ZeroArray();


   nCounter=0;
   fIter->Reset();
   while((pCal = (HRichCal *)fIter->Next()))   {
     nCounter++;
     //cout << "evt no: " << nEvtCounter << "  sec = " << pCal->getSector() 
     //	  << "  pad fired nb: " << nCounter << endl;
     //cout << "r: " <<  pCal->getRow() << "  c: " << pCal->getCol() << endl;
     pHardRingFind->FiredPad(pCal->getSector(),pCal->getRow(),pCal->getCol()); 
   }
   //cout << "---" << endl; 
    nEvtCounter++;
   

   iRingNrTot=0;
   // ------- loop over sectors --- begin ---
   for(int i = 0; i < 6; i++) {
     //     cout << "active sector: " << i << " *******" << endl; 
     pHardRingFind->SetSector(i);
     iRingNr = pHardRingFind->Execute();
     iRingNrTot+=iRingNr;  
     if (iRingNr) updateHits(i);
   }
   // ------- loop over sectors --- end ---
   // cout << "total rings: " << iRingNrTot << endl;
   
   nCounterH=0;
   fIterH->Reset();
   while((pHardHit = (HRichHitIPU *)fIterH->Next()))   {
     // if (pHardHit->getSector()>5 || pHardHit->getSector() < 0) continue;
     nCounterH++;
//       cout << "tr evt no: " << nEvtCounterH << "  sec = " 
//  	  << pHardHit->getSector() << "  ring nb: " << nCounterH << endl;
//       cout << "xtr: " << pHardHit->getX() << "    ytr: " 
//  	  << pHardHit->getY()<< "   ring quality: " 
//  	  << pHardHit->getQuality() << endl ;
   }
   nEvtCounterH++;
   // cout << "===============================================" << endl;
   
   return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
void HRichAnalysisIPU::updateHits(Int_t nSec) {

  HRichHitIPU *hit=NULL; 

  //  cout << " HardRich::updateHits " <<  iRingNr << endl;
  
  for (int i = 0; i < iRingNr; i++) {
      loc.set(1, nSec);

      hit=(HRichHitIPU *)m_pHardHitCat->getNewSlot(loc);

      if (hit!=NULL) {
        hit=new(hit) HRichHitIPU;

        *hit = pRings[i];
	hit->setSector(nSec);
 //cout << "ring nr: " << i << "   sec nr: " << nSec << " ---*---" << endl;
      }
  }
}      
//============================================================================

//----------------------------------------------------------------------------
void HRichAnalysisIPU::SetActiveSector(Int_t sectornr) {

  if (sectornr == iActiveSector) return;
  if (sectornr > 5 || sectornr < 0) {
    cerr << "Error in <HRichAnalysis::SetActiveSector>: "
	 << "Sector number = " << sectornr << " out of range (0..5)!\n";
    return;
  } 
  else {
    iActiveSector = sectornr;
  }
}
//============================================================================

//----------------------------------------------------------------------------

ClassImp(HRichAnalysisIPU)


















































































