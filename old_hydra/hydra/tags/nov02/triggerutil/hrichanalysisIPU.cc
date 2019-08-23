#include "hrichanalysisIPU.h"
#include "hdebug.h"
#include "hades.h"
#include "hcategory.h"
#include "hiterator.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "hevent.h"
#include "hrichraw.h"
#include "hrichhitIPU.h"
#include "hmatrixcatiter.h"
#include "hrichringfindIPU.h"
#include "triggerinfodef.h"
#include "htriggerdetector.h"
#include "hlocation.h"

#include <iostream.h>         


//---------------------------------------------------------------------------- 

HRichAnalysisIPU::HRichAnalysisIPU() { 
    fIter = NULL;
    fIterH = NULL;
    nCounter = 0;
    nEvtCounter = 0;
    nCounterH = 0;
    nEvtCounterH = 0;

    pRings = NULL;
    psortRings = NULL;
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

    pRings = NULL;
    psortRings = NULL;
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
  fIter = NULL; 
  if (fIterH) delete fIterH;
  fIterH = NULL;

  if (psortRings) delete [] psortRings;
  psortRings = NULL;
 
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
  HRuntimeDb *rtdb = gHades->getRuntimeDb();
  HRichDetector * pRichDet = (HRichDetector*)gHades->getSetup()->getDetector("Rich");
  HTriggerDetector * pTriggerDet = (HTriggerDetector*)gHades->getSetup()->getDetector("Trigger");
  
  //allocate input categories
  m_pRawCat=gHades->getCurrentEvent()->getCategory(catRichRaw);
  if (!m_pRawCat) {
    m_pRawCat=pRichDet->buildCategory(catRichRaw);
    
    if (!m_pRawCat) return kFALSE;
    else gHades->getCurrentEvent()
	   ->addCategory(catRichRaw, m_pRawCat, "Rich");
  }
  fIter = (HIterator*)m_pRawCat->MakeIterator("native");
    
  
  //allocate output categories
  m_pHardHitCat=gHades->getCurrentEvent()->getCategory(catHardRichHit);
  if (!m_pHardHitCat) {
    m_pHardHitCat=pTriggerDet->buildCategory(catHardRichHit);
    if (!m_pHardHitCat) return kFALSE;
    else gHades->getCurrentEvent()
	   ->addCategory(catHardRichHit, m_pHardHitCat, "Trigger");
  }
  fIterH = (HIterator*)m_pHardHitCat->MakeIterator("native");
 
  
  if(rtdb){  
    HRichGeometryPar *pGeomPar = (HRichGeometryPar*)rtdb->getContainer("RichGeometryParameters");
    setGeomPar(pGeomPar);
    if (pGeomPar == NULL) { 
      cout << " rich geometry parameters not found " << endl;
      return kFALSE;
    }
  }
  
  pHardRingFind->init(this);

 if (psortRings) delete [] psortRings;
  psortRings=new HRichHitIPU[1000];

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

   pHardRingFind->ZeroArray();

   nCounter=0;
   fIter->Reset();
   while((pRaw = (HRichRaw *)fIter->Next()))   {
     nCounter++;
     pHardRingFind->FiredPad(pRaw->getSector(),pRaw->getRow(),pRaw->getCol());
   }
    nEvtCounter++;
    //cout << "Evt. " << nEvtCounter << " -- " ;   

   iRingNrTot=0;

   // ------- loop over sectors --- begin ---
   for(int i = 0; i < 6; i++) {
     pHardRingFind->SetSector(i);
     iRingNr = pHardRingFind->Execute();
     iRingNrTot+=iRingNr;  
     if (iRingNr) {
       DataWordCut(i);
       updateHits(i);
     }
   }  // ------- loop over sectors --- end ---

   nCounterH=0;
   fIterH->Reset();
   while((pHardHit = (HRichHitIPU *)fIterH->Next()))   {
     nCounterH++;
   }
   nEvtCounterH++;

   return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
void HRichAnalysisIPU::updateHits(Int_t nSec) {
  HRichHitIPU *hit=NULL; 

  for (int i = 0; i < iRingNr; i++) {
      loc.set(1, nSec);

      hit=(HRichHitIPU *)m_pHardHitCat->getNewSlot(loc);

      if (hit!=NULL) {
        hit=new(hit) HRichHitIPU;
        *hit = pRings[i];
	
	// here we set the hit
	hit->setSector(nSec);

	// mapping according to RICH 
        HRichPad * pad = ((HRichGeometryPar*)fpGeomPar)->getPadsPar()->
                         getPad(pRings[i].iRingX,pRings[i].iRingY);

        hit->fPhi = pad->getPhi(nSec);
        hit->fTheta = pad->getTheta();
	
      } 
  }
}      
//============================================================================

//----------------------------------------------------------------------------
void HRichAnalysisIPU::DataWordCut(Int_t nSec) { 
  Int_t currentNumberOfWords = 0;  
  
  // Sort pRings into psortRings ( 100*X + Y)
  Int_t kk;     
  
  for(Int_t jj=0; jj<1000;jj++) {  
  psortRings[jj].iRingX=0; 
  psortRings[jj].iRingY=0; 
  }
  
  //  cout << " Number of rings " << iRingNr ; 
    for(Int_t jj=0; jj<iRingNr;jj++) {
      kk=0;
      pRings[jj].setY(100*pRings[jj].iRingY + pRings[jj].iRingX);

      if ( jj == 0 ) {
	psortRings[kk]=pRings[jj];
      } else {
	while  ( (pRings[jj].iRingY > psortRings[kk].iRingY) && (kk<jj )) 
	  kk++;

	for(Int_t ll=jj;ll>=kk;ll--)
	  psortRings[ll+1]=psortRings[ll];
	psortRings[kk]=pRings[jj];
      }
    }
    
    currentNumberOfWords = 0;

    Int_t newX, newY, oldX, oldY;
    oldX = -111; oldY = -111;

    for (    Int_t jj = 0;  jj <iRingNr; jj++  ) {
      newX = psortRings[jj].iRingX;
      newY = (Int_t) psortRings[jj].iRingY/100;

      if ( !( (newY==oldY) && (newX/8==oldX/8)) )
	currentNumberOfWords++;

      pRings[jj] = psortRings[jj];
      pRings[jj].iRingY = newY;
      pRings[jj].iRingX = newX;
      oldX = newX; oldY = newY;
      //cout << " Sec, X, Y, CurrDataWord " << nSec << "  " << newX << " " <<  newY << " " << currentNumberOfWords << endl;
      pRings[jj].DataWord = currentNumberOfWords;
      if (pRings[jj].DataWord==999) cout << "AIUTOOOOO !!!!! DataW " << currentNumberOfWords << endl;
    }
    //    cout << " and number of rings at the end " << jj << endl;     
            
}
//============================================================================
ClassImp(HRichAnalysisIPU)


















































































