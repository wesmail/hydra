using namespace std;
#include "TRandom.h"
#include <time.h>
#include <iostream> 
#include <iomanip>
#include "hwallhitf.h" //FK//
#include "hwallhitfsim.h"
#include "walldef.h"   //FK//
#include "hdebug.h"
#include "hades.h"
#include "hiterator.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hwalldetector.h" //FK//
#include "hwallraw.h"   //FK// 
#include "hwallrawsim.h"   
#include "hevent.h"
#include "hpartialevent.h"
#include "hcategory.h"
#include "hmatrixcategory.h"
#include "hrecevent.h"
#include "hlocation.h"
#include "hwallcalpar.h" //FK//
#include "hwalldigipar.h"
#include "hwallhit.h"   //FK//
#include "hwallhitsim.h"  
#include "hstartdef.h"
#include "hstarthit.h"
#include "hwallgeompar.h"        //FK//
#include "hdetgeompar.h"         //FK//
#include "hgeomvector.h"         //FK//
#include "hspecgeompar.h"        //FK//
#include "hgeomvolume.h"           //FK//
#include "hgeomcompositevolume.h"  //FK//
#include "hgeomtransform.h"        //FK//

//*-- Created : 24/06/2000 by M.Golubeva
//*-- Modified : 27/04/2001 by T.Wojcik 
//*-- Modified: 11/8/2005 by F. Krizek
// Modified by M.Golubeva 01.11.2006
//_HADES_CLASS_DESCRIPTION 
/////////////////////////////////////////////////////////////////////
//
//  HWallHitf calibrates raw Wall data and puts output 
//  values into cal data category
//
/////////////////////////////////////////////////////////////////////

void HWallHitFSim::initParContainer() {

  pWallDigiPar = (HWallDigiPar *)gHades->getRuntimeDb()
                                         ->getContainer("WallDigiPar");
  pWallGeometry=(HWallGeomPar *)gHades->getRuntimeDb()->getContainer("WallGeomPar");
  pSpecGeometry = (HSpecGeomPar *)gHades->getRuntimeDb()->getContainer("SpecGeomPar");
}

HWallHitFSim::HWallHitFSim(void) {

  fHitCat=NULL;
  fRawCat=NULL;
  pWallDigiPar = NULL;
  pWallGeometry = NULL;
  fLoc.set(1,-1);
  iter=NULL;    

}

HWallHitFSim::HWallHitFSim(Text_t *name,Text_t *title) : 
  //HReconstructor(name,title) {
  HWallHitF(name,title) {

  fHitCat=NULL;
  fRawCat=NULL;
  pWallDigiPar = NULL;
  pWallGeometry = NULL;
  pSpecGeometry = NULL;
  fLoc.set(1,-1);
  iter=NULL;    
}

HWallHitFSim::~HWallHitFSim(){
    if(iter){delete iter; iter=NULL;}
}

void HWallHitFSim::fillHit(HWallHitSim *hit, HWallRawSim *raw) {
}

void HWallHitFSim::fillHitTrack(HWallHitSim *hit, HWallRawSim *raw) {
  HWallHitSim *hs=(HWallHitSim *)hit;
  HWallRawSim *rs=(HWallRawSim *)raw;
  hs->setNTrack1(rs->getNTrack1());
  hs->setNTrack2(rs->getNTrack2());
}

Bool_t HWallHitFSim::init(void) { 

  initParContainer();

 fRawCat =gHades->getCurrentEvent()->getCategory(catWallRaw);
  if (!fRawCat) {
    HWallDetector* wall=(HWallDetector*)(gHades->getSetup()->getDetector("Wall"));
    fRawCat=wall->buildMatrixCategory("HWallRawSim",0.5F);
    if (!fRawCat) return kFALSE;
    else gHades->getCurrentEvent()->addCategory(catWallRaw,fRawCat,"Wall");
  } else {
    if (fRawCat->getClass()!=HWallRawSim::Class()) {
      Error("HWallHitFSim::init()","Misconfigured input category");
      return kFALSE;
    }
  }
  iter = (HIterator*)fRawCat->MakeIterator();  // cannot be "native", because
                                               // iter->getLocation() is used

  fHitCat=gHades->getCurrentEvent()->getCategory(catWallHit);
  if (!fHitCat) {
    HWallDetector* wall=(HWallDetector*)(gHades->getSetup()->getDetector("Wall"));
    fHitCat=wall->buildMatrixCategory("HWallHitSim",0.5F);
    if (!fHitCat) return kFALSE;
    else gHades->getCurrentEvent()->addCategory(catWallHit,fHitCat,"Wall");
  } else {
    if (fHitCat->getClass()!=HWallHitSim::Class()) {
      Error("HWallHitfSim::init()","Misconfigured output category");
      return kFALSE;
    }
  }

  /* 
     // Get Start Hit category. If StartHit is not defined or the start detector
    // is not in the setup a Warning is displayed and fStartHitCat is set to 0
 
   if (start) {
      fStartHitCat =gHades->getCurrentEvent()->getCategory(catStartHit);
      if (!fStartHitCat)
        Warning("init","Start Hit level not defining: setting start time to 0");
    } else {
      Warning("init","Start detector not found. Setting start time to 0");
      fStartHitCat = 0;
    }
  */

  return kTRUE;
}

void HWallHitFSim::fillGeometry(HWallHitSim *hit) {
  // Fills in the LAB position parameters for the given HTofHit.
  //
  // This is done by transforming the point (x,0,0) in the ROD coordinate
  // system to the LAB coordinate system. Where x is the reconstructed
  // x position inside the hit.
  
  HGeomVector rLocal,r;
  Float_t d,phi,theta,xx,yy,zz;
  Float_t rad2deg = 180./TMath::Pi(); 

  // HModGeomPar *mother=pWallGeometry->getModule(0);
  // HGeomTransform &trans = mother->getLabTransform();
  //HGeomVolume *cellVol=mother->getRefVolume()->getComponent(hit->getCell());
  //HGeomTransform &cellTrans=cellVol->getTransform();
  
   HModGeomPar *module= pWallGeometry->getModule(0);   
   HGeomTransform &trans = module->getLabTransform();

   /*
   //TEST of     wall_newgeom_trb_lookup_test.txt

   HGeomVolume *cellVol1;        

   for(Int_t ic=0; ic<303; ic++) {
     cellVol1 = module->getRefVolume()->getComponent(ic);        
     HGeomTransform &cellTrans1=cellVol1->getTransform();

     r.setX(0.);
     r.setY(0.);
     r.setZ(0.);
     
     rLocal=cellTrans1.transFrom(r);  // transform to module coordinate system
     r=trans.transFrom(rLocal);     // transform from module to LAB system
     
     d = r.length();
     theta = (d>0.) ? (rad2deg * TMath::ACos(r.getZ() / d)) : 0.;
     phi = rad2deg * TMath::ATan2( r.getY(), r.getX());
     if (phi < 0.) phi += 360.;
     
     //cout <<"cell,x,y,z " <<ic <<" " <<r.getX() <<" " <<r.getY() <<" " <<r.getZ() <<endl;
     cout <<"cell,x,y,z " <<ic <<" " <<rLocal.getX() <<" " <<rLocal.getY() <<" " <<rLocal.getZ() <<endl;
     
     }//for(Int_t ic=0; ic<301; ic++)
   */

   HGeomVolume *cellVol = module->getRefVolume()->getComponent(hit->getCell());        
   HGeomTransform &cellTrans=cellVol->getTransform();

  // Fill r with the hit coordinates in the cell coordinate system
  // Since we do not have information about x,y,z coordinates of impact
  //  hi occures in centre thus z=0, x=0 and y=0.
  //  Note that (0,0,0) corresponds to the rod center.


  r.setX(0.);
  r.setY(0.);
  r.setZ(0.);

  rLocal=cellTrans.transFrom(r);  // transform to module coordinate system
  r=trans.transFrom(rLocal);     // transform from module to LAB system

  HGeomVolume *tv=pSpecGeometry->getTarget(0);
  if (tv) r -= tv->getTransform().getTransVector();   // correct for target position

  // Fill output
  d = r.length();
  theta = (d>0.) ? (rad2deg * TMath::ACos(r.getZ() / d)) : 0.;
  phi = rad2deg * TMath::ATan2( r.getY(), r.getX());
  if (phi < 0.) phi += 360.;
  if (tv) r += tv->getTransform().getTransVector();   // correct for target position

  hit->setXYZLab(r.getX(), r.getY(), r.getZ());
  hit->setDistance( d );  
  hit->setTheta(theta);
  hit->setPhi(phi);  
  hit->getXYZLab(xx,yy,zz);

}


Int_t HWallHitFSim::execute(void) {

  HWallRawSim* raw = NULL;
  HWallHitSim* hit = NULL;
  //Bool_t startFlag;
  //Float_t startTime = 0, 
  Float_t calTime = 0, adc = 0;
  //Int_t startStrip, startModule;
  Int_t cell=-1;
  fLoc.set(1,-1);

  /*
  if (fStartHitCat) {
    HStartHit *h=NULL;
    if((h=(HStartHit *)fStartHitCat->getObject(0))!=NULL){
      startTime = h->getTime();
      startFlag = h->getFlag();    
      startStrip = h->getStrip();    
      startModule = h->getModule();
    }    
    else{
      startTime =0.0;
      startFlag = kFALSE;
    }
    
    cout <<"startTime,strip,module = " <<startTime <<" " <<startStrip <<" " <<startModule <<endl;
    if(startFlag)
      cout <<"startFlag = kTRUE"  <<endl;
    if(!startFlag)
      cout <<"startFlag = kFALSE"  <<endl;    
    
  } else {
    startTime = 0.0;
    startFlag = kFALSE;
  }
  */ 
  
  iter->Reset();   // this works only in split mode=2
                   // (for 0 and 1 the iterator must be recreated)
  while ((raw=(HWallRawSim *)iter->Next())!=0) {

    fLoc[0] =  raw->getCell();
    if(fLoc[0] == -1) continue;
    
    if(raw->getTime() > 0.) {
      fLoc[0] = raw->getCell();
      cell=fLoc[0];
      //HWallDigiParCell &pPar=(*pWallDigiPar)[fLoc[0]];
      //calTime =  pPar.getTDC_Slope()*raw->getTime()-pPar.getTDC_Offset();
      calTime =  pWallDigiPar->getTDC_Slope(cell)*raw->getTime()-pWallDigiPar->getTDC_Offset(cell);
      
      //if(startFlag) calTime -=  startTime;
      
      adc = 1.8*(raw->getCharge())/256.;
      //cout <<raw->getCharge() <<" " <<adc <<endl;
      hit = (HWallHitSim*) fHitCat->getSlot(fLoc);
      if(hit != NULL) {
	hit = new(hit) HWallHitSim;
	hit->setTime(calTime);	 
	//hit->setCharge(pPar.getADC_Slope()*(raw->getCharge())-pPar.getADC_Offset());
	hit->setCharge(adc);
	hit->setAddress(fLoc[0]);
	fillHit(hit,raw);
	fillGeometry(hit);  //FK// TADY TO KRACHUJE
	fillHitTrack(hit,raw);
      } //if(hit != NULL) 
    } //if(raw->getTime() > 0.) 
    
  } // while ((geant=(HGeantTof *)iter->Next())!=0) 
  return 0;
  
} // hwallhitfsim


ClassImp(HWallHitFSim)










