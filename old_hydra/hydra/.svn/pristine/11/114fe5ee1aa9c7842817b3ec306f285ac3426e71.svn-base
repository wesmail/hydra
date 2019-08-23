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
#include "richdef.h"

#include <iostream.h>         

HRichAnalysisIPUSim::HRichAnalysisIPUSim() { 
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
   
    pHardRingFind = new HRichRingFindIPUSim;
}       
//============================================================================
 //  is the number of pads a photon or a Ip has fired.)
//  A flag for each of the three track numbers is saved too,
//  to be able to distinguish between a Chrenkov photon

//---------------------------------------------------------------------------- 

HRichAnalysisIPUSim::HRichAnalysisIPUSim(Text_t *name,Text_t *title)
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
   
    pHardRingFind = new HRichRingFindIPUSim;
}  
//============================================================================
 
//---------------------------------------------------------------------------- 

HRichAnalysisIPUSim::~HRichAnalysisIPUSim(void) {
 if (pHardRingFind) delete pHardRingFind;
 pHardRingFind = NULL;

 if (fIter) delete fIter; 
 if (fIterH) delete fIterH;
}
//============================================================================
 
//---------------------------------------------------------------------------- 
HRichAnalysisIPUSim::HRichAnalysisIPUSim(const HRichAnalysisIPUSim& source) {
 cerr << "HRichAnalysisIPUSim object can not be initialized with values of another object!\n";
}
//============================================================================

//----------------------------------------------------------------------------
HRichAnalysisIPUSim& HRichAnalysisIPUSim::operator=(const HRichAnalysisIPUSim& source) {
 if (this != &source) {
  cerr << "HRichAnalysisIPUSim object can not be assigned!\n";
 }
return *this;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HRichAnalysisIPUSim::init() { 
  HRichDetector * pRichDet = (HRichDetector*)gHades->getSetup()->getDetector("Rich");
  
  //allocate input categories
  m_pCalCat=gHades->getCurrentEvent()->getCategory(catRichCal);
  //cout << "input category exists" << endl;
  if (!m_pCalCat) {
    //cout << "no input category, build new" << endl;
     m_pCalCat=pRichDet->buildMatrixCat("HRichCalSim",1);
     
    if (!m_pCalCat) return kFALSE;
    else gHades->getCurrentEvent()
	   ->addCategory(catRichCal, m_pCalCat, "Rich");
  }
  fIter = (HIterator*)m_pCalCat->MakeIterator("native");
    
  
  //allocate output categories
  m_pHardHitCat=gHades->getCurrentEvent()->getCategory(catHardRichHit);
  //cout << "output category exists" << endl;
  if (!m_pHardHitCat) {
    //cout << "no output category, build new" << endl;
     // m_pHardHitCat=pRichDet->buildCategory(catHardRichHit);
    m_pHardHitCat=pRichDet->buildLinearCat("HRichHitIPUSim");
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

Bool_t HRichAnalysisIPUSim::finalize() {
    return kTRUE;
}     
//============================================================================
 
//---------------------------------------------------------------------------- 

Int_t HRichAnalysisIPUSim::execute() {  
   HRichCalSim *pCalSim; 
   HRichHitIPUSim *pHardHit; 

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
   while((pCalSim = (HRichCalSim *)fIter->Next()) )   {
     nCounter++;
//       cout << "evt no: " << nEvtCounter << "  sec = " << pCalSim->getSector() 
//       	  << "  pad fired nb: " << nCounter << endl;
//       cout << "r: " <<  pCalSim->getRow() << "  c: " << pCalSim->getCol() << endl;
     pHardRingFind->FiredPad(pCalSim->getSector(),pCalSim->getRow(),pCalSim->getCol());
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
   while((pHardHit = (HRichHitIPUSim *)fIterH->Next()))   {
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
void HRichAnalysisIPUSim::updateHits(Int_t nSec) {

  HRichHitIPUSim *hit=NULL; 

  //  cout << " HardRich::updateHits " <<  iRingNr << endl;
  
  for (int i = 0; i < iRingNr; i++) {
      loc.set(1, nSec);

      hit=(HRichHitIPUSim *)m_pHardHitCat->getNewSlot(loc);

      if (hit!=NULL) {
        hit=new(hit) HRichHitIPUSim;

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
void HRichAnalysisIPUSim::SetActiveSector(Int_t sectornr) {

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

ClassImp(HRichAnalysisIPUSim)


















































































