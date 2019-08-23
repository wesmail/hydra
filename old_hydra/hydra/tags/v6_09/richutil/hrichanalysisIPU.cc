#include "hrichanalysisIPU.h"
#include "hdebug.h"
#include "hades.h"
#include "hcategory.h"
#include "hiterator.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "hevent.h"
#include "hrichraw.h"
#include "hrichhit.h"
#include "hmatrixcatiter.h"
#include "hrichringfindIPU.h"
#include "richutildef.h"
#include "hruntimedb.h"

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
  HRichDetector * pRichDet = (HRichDetector*)gHades->getSetup()->getDetector("Rich");
  
  //allocate input categories
  m_pRawCat=gHades->getCurrentEvent()->getCategory(catRichRaw);
  // cout << "input category exists" << endl;
  if (!m_pRawCat) {
    // cout << "no input category, build new" << endl;
    m_pRawCat=pRichDet->buildCategory(catRichRaw);
    
    if (!m_pRawCat) return kFALSE;
    else gHades->getCurrentEvent()
	   ->addCategory(catRichRaw, m_pRawCat, "Rich");
  }
  fIter = (HIterator*)m_pRawCat->MakeIterator("native");
    
  
  //allocate output categories
  m_pHardHitCat=gHades->getCurrentEvent()->getCategory(catHardRichHit);
  // cout << "output category exists" << endl;
  if (!m_pHardHitCat) {
    // cout << "no output category, build new" << endl;
    m_pHardHitCat=pRichDet->buildCategory(catHardRichHit);
    if (!m_pHardHitCat) return kFALSE;
    else gHades->getCurrentEvent()
	   ->addCategory(catHardRichHit, m_pHardHitCat, "Rich");
  }
  fIterH = (HIterator*)m_pHardHitCat->MakeIterator("native");
 
  HRuntimeDb* rtdb=gHades->getRuntimeDb();
  
  if(rtdb){  
    HRichGeometryPar *pGeomPar = (HRichGeometryPar*)rtdb->getContainer("RichGeometryParameters");
    setGeomPar(pGeomPar);
    if (pGeomPar == NULL) return kFALSE;
  }
  
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
   HRichRaw *pRaw; 
   HRichHitIPU *pHardHit; 

   //   cout << "start the execute" << endl;
////  this is just to see if the container is empty at the beginning 
//      nCounterH=0;
//       fIterH->Reset();
//       while((pHardHit = (HRichHitIPU *)fIterH->Next()))   {
//         nCounterH++;
//       } 
//       nEvtCounter++;
//       cout << "evt nb: " << nEvtCounter << " # of rings in pHardHit " << nCounterH << endl;


   pHardRingFind->ZeroArray();


   nCounter=0;
   fIter->Reset();
   while((pRaw = (HRichRaw *)fIter->Next()))   {
     nCounter++;
//       cout << "evt no: " << nEvtCounter << "  sec = " << pRaw->getSector() 
//       	  << "  pad fired nb: " << nCounter << endl;
//       cout << "r: " <<  pRaw->getRow() << "  c: " << pRaw->getCol() << endl;
     pHardRingFind->FiredPad(pRaw->getSector(),pRaw->getRow(),pRaw->getCol());
   }
    nEvtCounter++;
   

   iRingNrTot=0;
   // ------- loop over sectors --- begin ---
   for(int i = 0; i < 6; i++) {
     //  cout << "active sector: " << i << " *******" << endl; 
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



      // mapping according to RICH 
        HRichPad * pad = ((HRichGeometryPar*)fpGeomPar)->getPadsPar()->
                         getPad(pRings[i].iRingX,pRings[i].iRingY);

        hit->fPhi = pad->getPhi(nSec);
        hit->fTheta = pad->getTheta();

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


















































































