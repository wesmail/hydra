#include "hshowerhit.h"
#include "hshowerpid.h"
#include "hshowerhitheader.h"
#include "hshowercriterium.h"
#include "TArrayI.h"
#include "hruntimedb.h"
#include "hevent.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hshowerdetector.h"
#include "hratreeext.h"
#include "hcategory.h"
#include "hmatrixcategory.h"
#include "hlinearcatiter.h"
#include "hlocation.h"
#include "hshowercal.h"
#include "hshowerhitfpar.h"
#include "hshowergeometry.h"
#include "hshowerpad.h"
#include "hiterator.h"
#include "hdebug.h"
#include "hades.h"
#include "hgeomvector.h"
#include "hgeomvector2.h"
#include "showerdef.h"
#include "hshowertofpidfinder.h"
#include "hshowertofpid.h"
#include "hshowerhittof.h"
#include "showertofinodef.h"
#include "kickdef.h"
#include "htofinocalpar.h"
#include "htofinocalparcell.h"

ClassImp(HShowerTofPIDFinder)

///////////////////////////////////////////////////////////////////////
//HShowerTofPIDFinder
//
// HShowerTofPIDFinder reconstructor is looking for good lepton candidate 
// in shower-tofino detector. It takes information from HKickTrack and
// HShowerHitTof categories
// Author: J.O.
// Date: 08.12.2001
// Modified:
//////////////////////////////////////////////////////////////////////

HShowerTofPIDFinder::HShowerTofPIDFinder(Text_t *name,Text_t *title) :
  HReconstructor(name,title)
{
   fKickTrackIter = NULL; 
   m_pKickTrackCat=NULL;
   m_pHitTofCat=NULL;
   m_pPIDCat=NULL;
   
   m_Loc.set(0);
   setFillPID();
   setSortFlag(kTRUE);

}

HShowerTofPIDFinder::HShowerTofPIDFinder()
{
   fKickTrackIter = NULL; 
   m_pKickTrackCat=NULL;
   m_pHitTofCat=NULL;
   m_pPIDCat=NULL;

   m_Loc.set(0);
   setFillPID();
   setSortFlag(kTRUE);
}

HShowerTofPIDFinder::~HShowerTofPIDFinder(void) {
}

Bool_t HShowerTofPIDFinder::init() {
    printf("initialization of shower tofino  pid  finder\n");

    HShowerDetector *pShowerDet = (HShowerDetector*)gHades->getSetup()
                                                  ->getDetector("Shower");
												  
    m_pHitTofCat=gHades->getCurrentEvent()->getCategory(catShowerHitTof);
    if (!m_pHitTofCat) {
      m_pHitTofCat=pShowerDet->buildCategory(catShowerHitTof);

      if (!m_pHitTofCat) return kFALSE;
      else gHades->getCurrentEvent()
                        ->addCategory(catShowerHitTof,m_pHitTofCat , "Shower");
    }
    m_pKickTrackCat=gHades->getCurrentEvent()->getCategory(catKickTrack);
    if (!m_pKickTrackCat) {
      //m_pKickTrackCat=pShowerDet->buildCategory(catKickTrack);
      Error("init","No kick plane input available"); 

      if (!m_pKickTrackCat) return kFALSE;
      else gHades->getCurrentEvent()
                        ->addCategory(catKickTrack, m_pKickTrackCat, "KickTrack");
    }
      m_pPIDCat=gHades->getCurrentEvent()->getCategory(catShowerTofPID);
      if (!m_pPIDCat) {
       // m_pPIDCat=pShowerDet->buildCategory(catShowerTofPID);
        m_pPIDCat= new HLinearCategory("HShowerTofPID",1000);
  
        if (!m_pPIDCat) return kFALSE;
        else gHades->getCurrentEvent()
                        ->addCategory(catShowerTofPID, m_pPIDCat, "ShowerTofino");
      }

    fKickTrackIter = (HIterator*)getKickTrackCat()->MakeIterator(); 
    return initParameters();
}

Bool_t HShowerTofPIDFinder::initParameters() {
    HRuntimeDb* rtdb=gHades->getRuntimeDb();

    HShowerGeometry *pGeom = (HShowerGeometry*)rtdb->
                                               getContainer("ShowerGeometry");
    setGeometry(pGeom);
    if (!pGeom) return kFALSE;

    HShowerHitFPar *pHitFPar = (HShowerHitFPar*)rtdb->
                                         getContainer("ShowerHitFinderParams");
    setHitFPar(pHitFPar);
    if (!pHitFPar) return kFALSE;

	HTofinoCalPar *pTofinoCalPar = 
	                     (HTofinoCalPar*)rtdb->getContainer("TofinoCalPar");
	setTofinoCalPar(pTofinoCalPar);					 
	if(!pTofinoCalPar) return kFALSE;					 

    return kTRUE;

}

Bool_t HShowerTofPIDFinder::finalize() {
    return kTRUE;
}

HShowerTofPIDFinder& HShowerTofPIDFinder::operator=(HShowerTofPIDFinder &c) {

  return c;
}

Int_t HShowerTofPIDFinder::execute() 
{
  Int_t nIndex = -1,nMult[24],isLep[24][20],isInTimeWindow[24][20]; // up to 20 hits per paddle 
  Float_t fTof[24][20]; 
  Float_t dist0; //min distance to the tofino padle 
  Float_t dist; //distance to the tofino padle 
  
  HKickTrack *pKickTrack = 0;
  HShowerHitTof *pShower = 0 ;
  HShowerTofPID  *pid = 0;
  HLocation loc;

  loc.set(2,0,0);
  
  for(int i = 0; i < 24; i++) {  // clear tables before next event
    nMult[i] = 0;
	for (int j = 0; j < 20; j++) {
      fTof[i][j] = 0;
	  isLep[i][j] = 0;
	  isInTimeWindow[i][j] = 0;
	  }
  }
  
  fKickTrackIter->Reset();
  while((pKickTrack = (HKickTrack*)fKickTrackIter->Next()) != 0) {
    if (pKickTrack) {
      if(pKickTrack->getSystem() == 0) {
        if( (pShower = 
	      (HShowerHitTof*)m_pHitTofCat->getObject(pKickTrack->getOuterHitId())) != 0) {  
		  nIndex = 4*pShower->getSector()+pShower->getTofinoCell(); 
          loc[0]=pShower->getSector();
          loc[1]=pShower->getTofinoCell();

		  dist = 300*pKickTrack->getBeta()*pKickTrack->getTof(); // dist from target (300 to get it in mm)
		  dist0 = ((HTofinoCalPar*)m_pTofinoCalPar)->getMinLength(loc); // distance from target
		
          if(pShower->getTofinoMult()>0) {
	      nMult[nIndex]++;
	      fTof[nIndex][nMult[nIndex]] = pShower->getTof()*dist0/dist;  

          if(IsShower(pKickTrack)) isLep[nIndex][nMult[nIndex]] = 1;
		  else  isLep[nIndex][nMult[nIndex]] = 0;
			
		  if(IsTofInWindow(fTof[nIndex][nMult[nIndex]])) isInTimeWindow[nIndex][nMult[nIndex]] = 1; 
		  else  isInTimeWindow[nIndex][nMult[nIndex]] = 0; 

	  }		
	 }		
    }
   }  
  }
  
  for(int i = 0; i < 24; i++ ) {
   for(int j = 0; j < 20; j++) { 
	 if(isInTimeWindow[i][j] ==0 ) continue;
	 
	   if(nMult[i] > 0) { 
         if(isLep[i][j] ==1) isLep[i][j] = 1;              
		 else isLep[i][j] = 0;
	   }
    }
  }
  
  for(int i = 0 ; i < 24;i++) nMult[i] = 0; 
  fKickTrackIter->Reset();
  pKickTrack=NULL;
  pid=NULL;

  while((pKickTrack = (HKickTrack*)fKickTrackIter->Next()) != 0) {
    if(pKickTrack->getSystem()==0) {
      if((pShower=
	    (HShowerHitTof*)m_pHitTofCat->getObject(pKickTrack->getOuterHitId())) != 0) {
		
		nIndex = 4*pShower->getSector()+pShower->getTofinoCell();
		nMult[nIndex]++; 
		
		if(isLep[nIndex][nMult[nIndex]] && isInTimeWindow[nIndex][nMult[nIndex]]) {
	      if (m_pPIDCat) {
             pid=(HShowerTofPID *)m_pPIDCat->getNewSlot(m_Loc);
             if (pid) {
                 pid=new(pid) HShowerTofPID;
                 fillPID(pKickTrack, pid);
             }
          }
        }
      }
    }
  }

  if (IsSortFlagSet()) {
     m_pPIDCat->sort();
  }

  return 0;
}

Bool_t HShowerTofPIDFinder::IsLepton(HKickTrack* pTrack) {
#if DEBUG_LEVEL>2
  gDebuger->enterFunc("HShowerTofPIDFinder::execute");
  gDebuger->message("KickTrack cat points to %p",fKickTrack);
#endif
  Bool_t isLep = kFALSE;
  HShowerHitTof *pShower = 0; 

  if (pTrack) {
  if(pTrack->getSystem() == 0) {
    pShower =(HShowerHitTof*)m_pHitTofCat->getObject(pTrack->getOuterHitId());  
    if(pShower) { 
		  if(IsShower(pTrack) && IsTofInWindow(pShower)) 
		  isLep = kTRUE; 
	    }
      }
 }
 return isLep;
}

Bool_t HShowerTofPIDFinder::IsShower(HKickTrack *pKick) {
Bool_t isShower = kFALSE;
float fact10 =  0.0, fact20 = 0.0;
float lepThr10 = 0.0, lepThr20 = 0.0;
float showScaleFact;

 showScaleFact = ((HShowerHitFPar*)m_pHitFPar)->getScaleFactor();  
// if(pShower->getSum(1))  fact10 =  pShower->getSum(0)/pShower->getSum(1);
// if(pShower->getSum(2))  fact20 =  pShower->getSum(0)/pShower->getSum(2);
 fact10 =  pKick->getShowerSum10(1.);
 fact20 =  pKick->getShowerSum20(2.);
 
 lepThr10 = 
      getShowFactor(((HShowerHitFPar*)m_pHitFPar)->getFirstFitParam(),pKick->getP(),showScaleFact);

 lepThr20 = 
     getShowFactor(((HShowerHitFPar*)m_pHitFPar)->getSecondFitParam(),pKick->getP(),showScaleFact);
	 
 //cout << "lepThr10  " << lepThr10 <<"lepThr20  " << lepThr20 << endl;
 if(lepThr10<=0 && lepThr20<=0) return isShower;
 if(fact10>=lepThr10 || fact20>=lepThr20) isShower = kTRUE;

return isShower;
}

Float_t HShowerTofPIDFinder::getShowFactor(Float_t *pPar,Float_t mom,Float_t fParam) {
float fFactor = -1;

fFactor = calcFromFit(pPar,mom);
return fFactor*fParam;
}

Float_t HShowerTofPIDFinder::calcFromFit(float *pPar, float fMom)
{
 return (pPar[0]+pPar[1]*fMom+pPar[2]*fMom*fMom+pPar[3]*fMom*fMom*fMom);
}

Bool_t HShowerTofPIDFinder::IsTofInWindow(HShowerHitTof *pShower) {
Bool_t isTime = kFALSE;
Float_t  minTime ,maxTime ;
   
  minTime = ((HShowerHitFPar*)m_pHitFPar)->getMinTof();
  maxTime =((HShowerHitFPar*)m_pHitFPar)->getMaxTof();
  
  if(pShower->getTof() > minTime && pShower->getTof()<maxTime ) isTime = kTRUE;
  
return isTime;
}

Bool_t HShowerTofPIDFinder::IsTofInWindow(Float_t time) {
Bool_t isTime = kFALSE;
Float_t  minTime,maxTime ;

  minTime = ((HShowerHitFPar*)m_pHitFPar)->getMinTof();
  maxTime =((HShowerHitFPar*)m_pHitFPar)->getMaxTof();

  //cout << " minTime "<< minTime << " maxTime " << maxTime  << endl;
  
  if(time > minTime && time<maxTime ) isTime = kTRUE;
  
return isTime;
}

void HShowerTofPIDFinder::fillPID(HKickTrack *pKick , HShowerTofPID* pid) {
   HShowerHitTof *hit = 0 ;
   Float_t fR, fPhi, fTheta;
   Float_t d0 ;   
   Float_t  d;
   HLocation loc;
   loc.set(2,0,0);
   
   if( (hit=(HShowerHitTof*)m_pHitTofCat->getObject(pKick->getOuterHitId())) !=0) { 
   loc[0]=hit->getSector();
   loc[1]=hit->getTofinoCell();
   d = 300*pKick->getBeta()*pKick->getTof(); // dist from target (300 to get it in mm)
   d0 = ((HTofinoCalPar*)m_pTofinoCalPar)->getMinLength(loc); // min distance from target 
   pid->setSector(hit->getSector());
   pid->setModule(hit->getModule());
   pid->setRow(hit->getRow());
   pid->setCol(hit->getCol());
   pid->setAddress(hit->getAddress());

   hit->getSphereCoord(&fR, &fPhi, &fTheta);
   pid->setRadius(fR);
   pid->setPhi(fPhi);
   pid->setTheta(fTheta);
   
   pid->setTof(hit->getTof());
   //pid->setTof(hit->getTof()*d0/d);
   //pid->setFact10(hit->getSum(1)/hit->getSum(0));
   //pid->setFact20(hit->getSum(2)/hit->getSum(0));
   pid->setFact10( pKick->getShowerSum10(1.));
   pid->setFact20( pKick->getShowerSum20(2.));
   pid->setTofinoCell(hit->getTofinoCell());
   pid->setTofinoMult(hit->getTofinoMult());
   pid->setOuterHitId(m_pHitTofCat->getIndex(hit));
   pid->setTrackLength(d);
   }
}
