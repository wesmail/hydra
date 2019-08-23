#include "hshowerparticleidentifier.h"
#include "hshowercriterium.h"

#include "hruntimedb.h"
#include "hevent.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hshowerdetector.h"
#include "hcategory.h"
#include "hmatrixcatiter.h"
#include "hlocation.h"
#include "hshowerhit.h"
#include "hshowerhitfpar.h"
#include "hshowerpid.h"
#include "hdebug.h"
#include "hades.h"
#include "hiterator.h"
#include "showerdef.h"


ClassImp(HShowerParticleIdentifier)

///////////////////////////////////////////////////////////////////////
//HShowerParticleIdentifier
//
//HShowerParticleIdentifier selects hits, which passed through
//shower criterium defined by HShowerCriterium
//
//////////////////////////////////////////////////////////////////////

HShowerParticleIdentifier::HShowerParticleIdentifier(Text_t *name,Text_t *title) : HReconstructor(name,title)
{
   fIter=NULL;
   m_zeroLoc.set(0);
   m_pCriterium = new HShowerCriterium;
}

HShowerParticleIdentifier::HShowerParticleIdentifier()
{
  m_zeroLoc.set(0);
  fIter=NULL;
  m_pCriterium = new HShowerCriterium;
}

HShowerParticleIdentifier::~HShowerParticleIdentifier(void) {
     if (fIter) delete fIter;
     if (m_pCriterium) delete m_pCriterium;
}

Bool_t HShowerParticleIdentifier::init() {
  //initailization of pid and hit levels 

    printf("initialization of shower particle identifier\n");
    
    HShowerDetector *pShowerDet = (HShowerDetector*)gHades->getSetup()
                                                  ->getDetector("Shower");

    m_pHitCat=gHades->getCurrentEvent()->getCategory(catShowerHit);
    if (!m_pHitCat) {
      m_pHitCat=pShowerDet->buildCategory(catShowerHit);

      if (!m_pHitCat) return kFALSE;
      else gHades->getCurrentEvent()
                         ->addCategory(catShowerHit, m_pHitCat, "Shower");
    }

    m_pPIDCat=gHades->getCurrentEvent()->getCategory(catShowerPID);
    if (!m_pPIDCat) {
      m_pPIDCat=pShowerDet->buildCategory(catShowerPID);

      if (!m_pPIDCat) return kFALSE;
      else gHades->getCurrentEvent()
                        ->addCategory(catShowerPID, m_pPIDCat, "Shower");
    }

    fIter=(HIterator*)m_pHitCat->MakeIterator("native");
    initParameters();

    return kTRUE;
}

Bool_t HShowerParticleIdentifier::initParameters() {
    HRuntimeDb* rtdb=gHades->getRuntimeDb();

    HShowerHitFPar *pHitFPar = (HShowerHitFPar*)rtdb->
                                         getContainer("ShowerHitFinderParams");
    setHitFPar(pHitFPar);
    if (!pHitFPar) return kFALSE;
    m_pCriterium->setParams(pHitFPar);

    return kTRUE;
}

void HShowerParticleIdentifier::setCriterium(HShowerCriterium* pCrit) {
   if (m_pCriterium) delete m_pCriterium;
   m_pCriterium = pCrit;
}


Bool_t HShowerParticleIdentifier::finalize(void) {
   return kTRUE;
}

Int_t HShowerParticleIdentifier::execute()
{
 //it selects hits, which passed shower criterium 
  Float_t fShower = 0.0;
  Int_t ret = 0;
  HShowerHit *pHit;
  HShowerPID *pPID;

  fIter->Reset();
  while((pHit = (HShowerHit *)fIter->Next()))
  {
    //loop on local maxima
    fShower = m_pCriterium->showerCriterium(pHit, ret,
                             (HShowerHitFPar*)m_pHitFPar);

    if (fShower > 0.0) {
      pPID=(HShowerPID *)m_pPIDCat->getNewSlot(m_zeroLoc);
      if (pPID) {
        pPID=new(pPID) HShowerPID;
        fillPID(pHit, pPID, fShower);
      }
    }
  }

  return 0;
}

void HShowerParticleIdentifier::fillPID(HShowerHit* hit, HShowerPID* pid,
                                                          Float_t fShower) {
   Float_t fX, fY, fZ;
   Float_t fR, fPhi, fTheta;

   pid->setCharge(hit->getCharge());
   pid->setSector(hit->getSector());
   pid->setModule(hit->getModule());
   pid->setRow(hit->getRow());
   pid->setCol(hit->getCol());
   pid->setAddress(hit->getAddress());
   pid->setShower(fShower);

   hit->getLabXYZ(&fX, &fY, &fZ);
   pid->setXYZ(fX, fY, fZ);
   hit->getSphereCoord(&fR, &fPhi, &fTheta);
   pid->setSphereCoord(fR, fPhi, fTheta);

   getPIDCat()->sort();
}

