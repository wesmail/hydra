#include "hrichparticleidentifier.h"
#include "hrichhitselection.h"

#include "hruntimedb.h"
#include "hevent.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hrichdetector.h"
#include "hcategory.h"
#include "hmatrixcatiter.h"
#include "hlocation.h"
#include "hrichhit.h"
#include "hrichanalysispar.h"
#include "hrichpid.h"
#include "hdebug.h"
#include "hades.h"
#include "hiterator.h"
#include "richdef.h"


ClassImp(HRichParticleIdentifier)

//_HADES_CLASS_DESCRIPTION 
///////////////////////////////////////////////////////////////////////
//HRichParticleIdentifier
//
//HRichParticleIdentifier selects hits, which passed through
//rich criterium defined by HRichHitSelection
//
//////////////////////////////////////////////////////////////////////

HRichParticleIdentifier::HRichParticleIdentifier(Text_t *name,Text_t *title) : HReconstructor(name,title)
{
   fIter=NULL;
   m_zeroLoc.set(0);
   m_pHitSelection = new HRichHitSelection;
}

HRichParticleIdentifier::HRichParticleIdentifier()
{
  m_zeroLoc.set(0);
  fIter=NULL;
  m_pHitSelection = new HRichHitSelection;
}

HRichParticleIdentifier::~HRichParticleIdentifier(void) {
     if (fIter) delete fIter;
     if (m_pHitSelection) delete m_pHitSelection;
}

Bool_t HRichParticleIdentifier::init() {
  //initailization of pid and hit levels 

    printf("initialization of rich particle identifier\n");
    
    HRichDetector *pRichDet = (HRichDetector*)gHades->getSetup()
                                                  ->getDetector("Rich");

    m_pHitCat=gHades->getCurrentEvent()->getCategory(catRichHit);
    if (!m_pHitCat) {
      m_pHitCat=pRichDet->buildCategory(catRichHit);

      if (!m_pHitCat) return kFALSE;
      else gHades->getCurrentEvent()
                         ->addCategory(catRichHit, m_pHitCat, "Rich");
    }

    m_pPIDCat=gHades->getCurrentEvent()->getCategory(catRichPID);
    if (!m_pPIDCat) {
      m_pPIDCat=pRichDet->buildCategory(catRichPID);

      if (!m_pPIDCat) return kFALSE;
      else gHades->getCurrentEvent()
                        ->addCategory(catRichPID, m_pPIDCat, "Rich");
    }

    fIter=(HIterator*)m_pHitCat->MakeIterator("native");
    initParameters();

    return kTRUE;
}



Bool_t HRichParticleIdentifier::initParameters() {
    HRuntimeDb* rtdb=gHades->getRuntimeDb();

    HRichAnalysisPar *pAnalysisPar = (HRichAnalysisPar*)rtdb->
                                       getContainer("RichAnalysisParameters");
    setAnalysisPar(pAnalysisPar);
    if (!pAnalysisPar) return kFALSE;
    m_pHitSelection->setParams(pAnalysisPar);

    return kTRUE;
}


void HRichParticleIdentifier::setHitSelection(HRichHitSelection* pCrit) {
   if (m_pHitSelection) delete m_pHitSelection;
   m_pHitSelection = pCrit;
}


Bool_t HRichParticleIdentifier::finalize(void) {
   return kTRUE;
}

Int_t HRichParticleIdentifier::execute()
{
 //it selects hits, which passed rich criterium 
  Float_t fRich = 0.0;
  Int_t ret = 0;
  HRichHit *pHit;
  HRichPID *pPID;

  fIter->Reset();
  while((pHit = (HRichHit *)fIter->Next()))
  {
    //loop on local maxima
    fRich = m_pHitSelection->richHitSelection(pHit, ret,
                             (HRichAnalysisPar*)m_pAnalysisPar);

    if (fRich > 0.0) {
      pPID=(HRichPID *)m_pPIDCat->getNewSlot(m_zeroLoc);
      if (pPID) {
        pPID=new(pPID) HRichPID;
        fillPID(pHit, pPID);
      }
    }
  }

  return 0;
}

void HRichParticleIdentifier::fillPID(HRichHit* hit, HRichPID* pid) {
   Float_t fX, fY;
   // Float_t fPhi, fTheta;

   pid->setSector(hit->getSector());
//   pid->setEventNr(hit->getEventNr());

   switch (((HRichAnalysisPar*)m_pAnalysisPar)->iSuperiorAlgorithmID) {
    case 1: pid->setRingQuality(hit->iRingPatMat);
            break;
    case 2: pid->setRingQuality(hit->iRingHouTra);
            break;
    default: pid->setRingQuality(hit->iRingPatMat);
             break; 
   }
   pid->setRingAmplitude(hit->iRingAmplitude);
   pid->setRingPadNr(hit->iRingPadNr);

   hit->getXY(&fX, &fY);
   pid->setXY(fX, fY);
   pid->setPhi(hit->getPhi());
   pid->setTheta(hit->getTheta());

//   getPIDCat()->sort();
}

