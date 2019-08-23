#include "hmdc12fit.h"
#include "hmdcdef.h"
#include "hdebug.h"
#include "hades.h"
#include "hiterator.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hevent.h"
#include "hcategory.h"
#include "hlocation.h"
#include <iostream.h>
#include "TRandom.h"
#include "hmdcsizescells.h"
#include "hgeomvector.h"
#include "hmdc12cellprojec.h"
#include "hgeantmdc.h"
#include "hruntimedb.h"
#include "hgeomcompositevolume.h"
#include "hgeomvolume.h"
#include "hgeantkine.h"
#include "hmdcseg.h"
#include "hmdcclus.h"
#include "hmdcgetcontainers.h"
#include "hmdcclussim.h"
#include "hgeantkine.h"
#include "hgeantmdc.h"

//*-- Author : V.Pechenov
//*-- Modified: 17.10.00 A.Nekhaev

HMdc12Fit::HMdc12Fit() {
  fCalCat=0;
  fLookUpTb=0;
  fSizesCells=0;
  setParContainers();
  iter=0;
  fprint = kFALSE;
//   fprint = kTRUE;

}

HMdc12Fit::HMdc12Fit(Text_t *name,Text_t *title) : HReconstructor(name,title) {

  fCalCat=0;
  fLookUpTb=0;
  fSizesCells=0;
  setParContainers();
  iter=0;    
    fprint = kFALSE;
  //  fprint = kTRUE;
}

void HMdc12Fit::setParContainers() {
  fSizesCells=(HMdcSizesCells*)gHades->getRuntimeDb()->getContainer("MdcSizesCells");
  if (!fSizesCells) {
   printf("Error: MdcSizesCells is absent\n");
   return;
  }
  fGeomPar=(HMdcGeomPar*)gHades->getRuntimeDb()->getContainer("MdcGeomPar");
  if (!fGeomPar) {
    printf("Error: MdcGeomPar is absent\n");
    return;
  }
}

Bool_t HMdc12Fit::init(void) {
  
  fCalCat=gHades->getCurrentEvent()->getCategory(catMdcCal1);
  if (!fCalCat) return kFALSE; 
  fClusCat=HMdcGetContainers::getCatMdcClus();
  if( !fClusCat ) return kFALSE;
  
  fSegCat = gHades->getCurrentEvent()->getCategory(catMdcSeg);
  if (!fSegCat) {
     fSegCat = gHades->getSetup()->getDetector("Mdc")
                                 ->buildCategory(catMdcSeg);
     if (!fSegCat) return kFALSE;
     else gHades->getCurrentEvent()->addCategory(catMdcSeg,fSegCat,"Mdc");
  }

  iter=(HIterator *)fCalCat->MakeIterator();
  iterClus=(HIterator *)fClusCat->MakeIterator();
  fEventId=0;
  clSeqNum=0;
  cal=0;

  loc.set(4,0,0,0,0);

//Hists:
  setHists();

  HGeomVector vtmp;
 for(Int_t i=0; i<12; i++) {
   sysLayers[i]=new HGeomTransform();
   Int_t is=getTransfMat(i/6,i%6,sysLayers[i]);        // mod=0
     if(is==0) sysLayers[i]=0;    
  }
  
  return kTRUE;
}


Int_t HMdc12Fit::execute(void) {
  locSeg.set(2,0,0);
  locClus.set(2,0,0);
  HMdcClus * clst;
  fEventId++;
  loc.set(4,0,0,0,0);   // location used to fill the HMdcGeantCell category
  iter->Reset();
  iterClus->Reset();
  Int_t numcl;
  Int_t  sumTracks;
  Int_t foundTracks;
  
  for(loc[0]=0; loc[0]<6; loc[0]++){

    locClus[0] = loc[0];
    locClus[1] = 0;
    iterClus->gotoLocation(locClus);
      
    locSeg[0]=loc[0];
    locSeg[1]=0;
    sumTracks = 0;
    foundTracks = 0;
        
    while((clst=(HMdcClus*)iterClus->Next())) {
      clSeqNum++;
      //- - - - - - - - - 
      if(!executeClust(clst)) continue;
      //- - - - - - - - -
      Float_t threshold = 10000000000.;
      if (trackFitter.execute(threshold)) {
	
	foundTracks++;
	
	int NPlanes = 0;
	int oldPlane = -1;
	
	for(int hit = 0; hit < trackFitter.getNMDC1Times() + trackFitter.getNMDC2Times(); hit++) {
	  if(address[hit][1] != oldPlane) {
	    NPlanes++;
	    oldPlane = address[hit][1];
	  } 
	  
	  hitTuple->Fill( trackFitter.getFunMin(),
			  //			  trackFitter.getIterationNumber(),
			  trackFitter.getNMDC1Times() + trackFitter.getNMDC2Times(),
			  trackFitter.getDeviation(hit),
			  trackFitter.getWeight(hit),
			  TDCTime[hit],
			  address[hit][1],
			  address[hit][2],
			  clSeqNum,
			  trackFitter.getXOutVertex(),
			  trackFitter.getYOutVertex(),
			  trackFitter.getZOutVertex(),
			  trackFitter.getXOutPlane(),
			  trackFitter.getYOutPlane(),
			  trackFitter.getZOutPlane(),
			  trackFitter.getPVelocity());
	}
	
	HMdcSeg *seg = (HMdcSeg*)fSegCat->getNewSlot(locSeg);      
	if(!seg) continue;
	seg = new(seg) HMdcSeg;
	seg->setIOSeg(0);
	seg->setSec(loc[0]);
	Double_t theta, phi, zm, r0, xTag, yTag, zTag, xPl, yPl, zPl;
	Double_t errZm, errR0, errTheta, errPhi;
	errZm=errR0=errTheta=errPhi=0.;
	xTag = trackFitter.getXOutVertex();
	yTag = trackFitter.getYOutVertex();
	zTag = trackFitter.getZOutVertex();
	xPl = trackFitter.getXOutPlane();
	yPl = trackFitter.getYOutPlane();
	zPl = trackFitter.getZOutPlane();

	Double_t xPxT=xPl-xTag;
	Double_t yPyT=yPl-yTag;
	Double_t zPzT=zPl-zTag;

	theta=acos(zPzT/sqrt(xPxT*xPxT+yPyT*yPyT+zPzT*zPzT));
	phi=TMath::ATan2(yPyT,xPxT);
	if( theta == 0.0 ) theta = 1.e-5;  // ??? !!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	zm=zTag - cos(theta)/sin(theta) * (xTag*cos(phi)+yTag*sin(phi));
	r0=yTag*cos(phi)-xTag*sin(phi);
	seg->setPar(zm, errZm, r0, errR0, theta, errTheta, phi, errPhi);

	trackTuple->Fill( trackFitter.getFunMin(),
			  trackFitter.getIterationNumber(),
			  trackFitter.getNMDC1Times() + trackFitter.getNMDC2Times(),
			  NPlanes,
			  trackFitter.getXOutVertex(),
			  trackFitter.getYOutVertex(),
			  trackFitter.getZOutVertex(),
			  trackFitter.getXOutPlane(),
			  trackFitter.getYOutPlane(),
			  trackFitter.getZOutPlane(),
			  trackFitter.getPVelocity());
		
      }
    }
    
  }
  return 0;
  
}

Bool_t HMdc12Fit::executeClust(HMdcClus * clst) {
 
   xPlane = clst->getX();
   yPlane = clst->getY();
   zPlane = clst->getZ();
   
   NMDCTime[0] = NMDCTime[1] = 0;
   nClTimes = 0;
   //   for(loc[1]=0;loc[1]<2;loc[1]++) {
//    for(loc[1]=1;loc[1]<2;loc[1]++) {
   for(loc[1]=0;loc[1]<2;loc[1]++) {
      
      for(loc[2]=0;loc[2]<6;loc[2]++) {      // nl - Num. of layers in MDC1+MDC2
	 
	 Int_t nLayer=loc[1]*6+loc[2];
	 loc[3]=-1;
	 while((loc[3]=clst->next(nLayer,loc[3])) > -1) {
	    Int_t nTimes=clst->getTime(nLayer,loc[3]); // num. times for nWire    
	    Int_t nWires=clst->getNCells(nLayer); // num. times for nWire    
	    cal=(HMdcCal1*)fCalCat->getSlot(loc);
	    if(!cal) {
	    } else {  
      
	       HMdcSizesCellsCell& pSCCell=(*fSizesCells)[loc[0]][loc[1]][loc[2]][loc[3]];

	       const HGeomVector* wP1 = pSCCell.getWirePoint(0);
	       const HGeomVector* wP2 = pSCCell.getWirePoint(1);
//---------------------------------------------------------------      
// 	       Int_t nTr1 = cal->getNTrack1();
//      	       if(nTr1 == nTrList[0]) {
// 	       if(cal->getTime1() < 75) {
		  NMDCTime[loc[1]]++;
			
		  address[nClTimes][0] = loc[1];
		  address[nClTimes][1] = loc[2];
		  address[nClTimes][2] = loc[3];

		  TDCTime[nClTimes] = cal->getTime1() + 6;

// 		  qualityWeight[nClTimes] = (nWires < 3) ? 1 : 1./nWires;
		  qualityWeight[nClTimes] = 1./nWires;
		  
		  xLeft [nClTimes] = wP1->getX();
		  xRight[nClTimes] = wP2->getX();
		  yLeft [nClTimes] = wP1->getY();
		  yRight[nClTimes] = wP2->getY();
		  zLeft [nClTimes] = wP1->getZ();
		  zRight[nClTimes] = wP2->getZ();
		  
		  nClTimes++;
// 	       }
//      	       }
		  
	       if(nTimes == 3) {  

// 		  Int_t nTr2 = cal->getNTrack2();

//      		  if(nTr2 == nTrList[0]) {
		    if(cal->getTime2() - cal->getTime1() > 50) {
		      NMDCTime[loc[1]]++;
			
		      TDCTime[nClTimes] = cal->getTime2() + 6;
		     
		      address[nClTimes][0] = loc[1];
		      address[nClTimes][1] = loc[2];
		      address[nClTimes][2] = loc[3];
		      
		      qualityWeight[nClTimes] = 1./nWires;
		      qualityWeight[nClTimes] = (nWires < 3) ? 1 : 1./nWires;

		      xLeft [nClTimes] = wP1->getX();
		      xRight[nClTimes] = wP2->getX();
		      yLeft [nClTimes] = wP1->getY();
		      yRight[nClTimes] = wP2->getY();
		      zLeft [nClTimes] = wP1->getZ();
		      zRight[nClTimes] = wP2->getZ();
		      nClTimes++;
		    }
//      		  }
	       }
//----------------------------------------------------------------
	    }
	 } // end of hit loop    
      }// end of layer loop
   }// end of MDC loop

   if(NMDCTime[0] < 6 && NMDCTime[1] < 6) return kFALSE; 
    
   trackFitter.setNMDC1Times(NMDCTime[0]);
   trackFitter.setNMDC2Times(NMDCTime[1]);
		    
   trackFitter.setTDCTime(TDCTime);

   trackFitter.setQualityWeight(qualityWeight);
		    
   trackFitter.setXLeft(xLeft);
   trackFitter.setYLeft(yLeft);
   trackFitter.setZLeft(zLeft);
   trackFitter.setXRight(xRight);
   trackFitter.setYRight(yRight);
   trackFitter.setZRight(zRight);
		    
   trackFitter.setXInitPlane(xPlane);
   trackFitter.setYInitPlane(yPlane);
   trackFitter.setZInitPlane(zPlane);		    

   HGeomVector vtmp;
  
   HGeomVector trP1(0., 0., 0.);
   HGeomVector trP2(xPlane, yPlane, zPlane);
   
   HGeomVector pointCros1, pointCros2;
   Double_t dist1, dist2;
   
   if(sysLayers[0]) {
     calcCross( &trP1, &trP2, sysLayers[0], &pointCros1);
     dist1 = sqrt(pointCros1.getX()*pointCros1.getX()+pointCros1.getY()*
		  pointCros1.getY()+pointCros1.getZ()*pointCros1.getZ());
   }
   else dist1 = 0;
   
   if(sysLayers[6]) {
     calcCross( &trP1, &trP2, sysLayers[6], &pointCros2);
     dist2 =  sqrt(pointCros2.getX()*pointCros2.getX()+pointCros2.getY()*
		   pointCros2.getY()+pointCros2.getZ()*pointCros2.getZ());
   }
   else dist2 = 0;
   
   trackFitter.setDistVertexMDC1(dist1);
   trackFitter.setDistVertexMDC2(dist2);

   return kTRUE;

}

void HMdc12Fit::setHists(void) {
  for(Int_t j=0; j<6; j++) {
   for(Int_t i=0; i<200; i++) plots[i][j]=0;
  }
  for(Int_t i=0; i<200; i++) hist1[i]=0;
  for(Int_t i=0; i<200; i++) hist2[i]=0;
  hfile = new TFile("fitTuples.root","RECREATE","MDC hit and track NTuples");

  hitTuple = new TNtuple("hit", "hits", "funMin:nHits:dev:weight:time:plane:cell:clnum:xV:yV:zV:xP:yP:zP:pV");

  trackTuple = new TNtuple("track", "tracks", "funMin:iter:nHits:nPlanes:xV:yV:zV:xP:yP:zP:pV");
  
}


Bool_t HMdc12Fit::finalize(void) {
  hfile->Write();
  hfile->Close();
  return kTRUE;
}


Int_t HMdc12Fit::getTransfMat(Int_t mod, Int_t layer, HGeomTransform* sys) {
  HGeomCompositeVolume* pComVol=fGeomPar->getRefVolume(mod);
  if(!pComVol)  return 0;
  HGeomVolume* pGeomVol=pComVol->getComponent(layer); 
  if(!pGeomVol)  return 0; 
  sys->setTransform(pGeomVol->getTransform());
  sys->transFrom(pComVol->getTransform());
  return 1;
}

void HMdc12Fit::calcCross( HGeomVector* vc1, HGeomVector* vc2, 
            const HGeomTransform* trans, HGeomVector* vcNew){
// Calculeting a cross of line (vc1-vc2) with plane HGeomTransform
  Double_t x1,x2,y1,y2,z1,z2;
  x1=vc1->getX();   x2=vc2->getX();
  y1=vc1->getY();   y2=vc2->getY();
  z1=vc1->getZ();   z2=vc2->getZ();
  Double_t A,B,C,D;
  A=(trans->getTransVector()).getX();
  B=(trans->getTransVector()).getY();
  C=(trans->getTransVector()).getZ();
  D=-(A*A+B*B+C*C);
  Double_t point[3];
  point[0]=(x1/(x1-x2)*(B*(y1-y2)+C*(z1-z2))-B*y1-C*z1-D)/
           (A+(B*(y1-y2)+C*(z1-z2))/(x1-x2));
  point[1]=(point[0]-x1)*(y1-y2)/(x1-x2)+y1;
  point[2]=(-A*point[0]-B*point[1]-D)/C;
  vcNew->setVector(point);
}

Float_t HMdc12Fit::calcVertexDist( Float_t x0, Float_t y0, Float_t z0,
			      Float_t x1, Float_t y1, Float_t z1,
			      Float_t x2, Float_t y2, Float_t z2) {
  
// Calculeting a min distance betveen point x0, y0, z0 and line

  return sqrt( ((y2-y1)*(z1-z0) - (z2-z1)*(y1-y0))*((y2-y1)*(z1-z0) - (z2-z1)*(y1-y0)) +
	       ((z2-z1)*(x1-x0) - (x2-x1)*(z1-z0))*((z2-z1)*(x1-x0) - (x2-x1)*(z1-z0)) +
               ((x2-x1)*(y1-y0) - (y2-y1)*(x1-x0))*((x2-x1)*(y1-y0) - (y2-y1)*(x1-x0)) )/
    sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) + (z2-z1)*(z2-z1));
}

ClassImp(HMdc12Fit)
