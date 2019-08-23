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
#include "hgeantmdc.h"
#include "hruntimedb.h"
#include "hgeomcompositevolume.h"
#include "hgeomvolume.h"
#include "hgeantkine.h"
#include "hmdcseg.h"
#include "hmdchit.h"
#include "hmdcclus.h"
#include "hmdcgetcontainers.h"
#include "hmdcclussim.h"
#include "hgeantkine.h"
#include "hgeantmdc.h"
#include "hmdcgeompar.h"
#include "hmdcdetector.h"

//*-- Author : V.Pechenov
//*-- Modified: 17.10.00 A.Nekhaev

HMdc12Fit::HMdc12Fit() {
  setParContainers();
}

HMdc12Fit::HMdc12Fit(Text_t *name,Text_t *title) : HReconstructor(name,title) {
  setParContainers();
}

HMdc12Fit::~HMdc12Fit() {
  HMdcSizesCells::deleteCont();
}

void HMdc12Fit::setParContainers() {
  fCalCat=0;
  fSizesCells=0;
    iter=0;
  fprint = kFALSE;
//    fprint = kTRUE;
   fhists = kFALSE;
//  fhists = kTRUE;
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

  fHitCat = gHades->getCurrentEvent()->getCategory(catMdcHit);
  if (!fHitCat) {
     fHitCat = gHades->getSetup()->getDetector("Mdc")
                                 ->buildCategory(catMdcHit);
     if (!fHitCat) return kFALSE;
     else gHades->getCurrentEvent()->addCategory(catMdcHit,fHitCat,"Mdc");
  }

  iter=(HIterator *)fCalCat->MakeIterator();
  iterClus=(HIterator *)fClusCat->MakeIterator();
  
  fSizesCells=HMdcSizesCells::getObject();;
  if (!fSizesCells) {
   printf("Error: MdcSizesCells is absent\n");
   return kFALSE;
  }
  
  fEventId=0;
  clSeqNum=0;
  cal=0;

  loc.set(4,0,0,0,0);

//Hists:
  if(fhists) setHists();

  fGetCont=HMdcGetContainers::getObject();
  fmdc= fGetCont->getMdcDetector();
  if(!fmdc) return kFALSE;
  locSeg.set(2,0,0);
  locHit.set(3,0,0,0);
  locClus.set(2,0,0);
  return kTRUE;
}

Bool_t HMdc12Fit::reinit(void) {
  if(!fSizesCells) return kFALSE;
  return fSizesCells->initContainer();
}


Int_t HMdc12Fit::execute(void) {
  HMdcClus * fClst;
  fEventId++;
  loc.set(4,0,0,0,0);   // location used to fill the HMdcGeantCell category
  iter->Reset();
  iterClus->Reset();
  Int_t  sumTracks;
  Int_t foundTracks;
  Int_t maxNumSectors=((HMatrixCategory*)fClusCat)->getSize(0);
  Int_t maxNumSegments=((HMatrixCategory*)fClusCat)->getSize(1);

  for(locClus[0]=0; locClus[0]<maxNumSectors; locClus[0]++){
    Int_t sec=locClus[0];
    for(locClus[1]=0; locClus[1]<maxNumSegments; locClus[1]++) {
      iterClus->Reset();
      iterClus->gotoLocation(locClus);

      sumTracks = 0;
      foundTracks = 0;
      Int_t indexHit[4];
      indexHit[0]=indexHit[1]=indexHit[2]=indexHit[3]=0;
      while((fClst=(HMdcClus*)iterClus->Next())) {
        clSeqNum++;
        //- - - - - - - - -
        loc[0] = sec;

        if(!executeClust(fClst)) continue;
        //- - - - - - - - -
        Float_t threshold = 1000000000000000.;
// 	if (trackFitter.execute(threshold)) ???????
	trackFitter.execute(threshold);
	
	  foundTracks++;
	
	  int NPlanes = 0;
	  int oldPlane = -1;
	
	  int NWeight = 0;
	  for(int hit = 0; hit < trackFitter.getNMDC1Times() + trackFitter.getNMDC2Times(); hit++) {
	    if(trackFitter.getWeight(hit)>.1) NWeight++;
	  }
	
	  if(fhists) {
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
			      trackFitter.getIterationNumber(),
			      NWeight,
			      // 			    address[hit][1],
			      // 			    address[hit][2],
			      locClus[1]*2+address[hit][0],
			      //			    clSeqNum,
			      trackFitter.getXOutVertex(),
			      trackFitter.getYOutVertex(),
			      trackFitter.getZOutVertex(),
			      trackFitter.getXOutPlane(),
			      trackFitter.getYOutPlane(),
			      trackFitter.getZOutPlane(),
			      trackFitter.getPVelocity());
	    }
	  }         
// ??? proverka na horoshiy fit ???
// =====================================================
// filling HMdcSeg and HMdcHit
          if(NWeight>=5) { //>6) 
            for(int hit=0;hit<nClTimes;hit++) {
              Int_t mod=address[hit][0];
              Int_t lay=address[hit][1];
              Int_t cell=address[hit][2];
              Int_t time=address[hit][3];
              Float_t weight=trackFitter.getWeight(hit);
//               if(listCells.getTime(mod*6+lay,cell) == 3) {
//                 // new fit ???
//                 Int_t dHit=(weight < trackFitter.getWeight(hit+1)) ? hit:hit+1;
//                 listCells.delTime(address[hit][0]*6+address[hit][1],
//                 address[hit][2],address[hit][3]);
//               }
              if(weight<.1) {
                listCells.delTime(mod*6+lay,cell,time);
//                if(weight>0) Printf("!!! m%il%ic%it%i W=%g",mod,lay,cell,time,
//                  weight);
              }
            }
/*for(int lay=0;lay<12;lay++) {
  Int_t cell=listCells.getFirstCell(lay);
  if(listCells.getLastCell(lay)-listCells.getFirstCell(lay) > 1) {
    Printf("====== %i-%i+1=%i cells",listCells.getLastCell(lay),listCells.getFirstCell(lay),
        listCells.getLastCell(lay)-listCells.getFirstCell(lay)+1);
    for(int hit=0;hit<nClTimes;hit++) {
      if(trackFitter.getWeight(hit)<=0.1) continue;
      if(address[hit][0]==lay/6 && address[hit][1]==lay%6) printf("! ");
Printf("...m%il%ic%it%i W=%g",
address[hit][0],address[hit][1],address[hit][2],address[hit][3],trackFitter.getWeight(hit));
    }
  }
}*/
          
// Printf("----------------------++++++++++++++++++++++++++++++++++++");
// //--------test------------------------
//   HGeomVector v1(trackFitter.getXOutVertex(),
//                  trackFitter.getYOutVertex(),
//                  trackFitter.getZOutVertex());
//   HGeomVector v2(trackFitter.getXOutPlane(),
//                  trackFitter.getYOutPlane(),
//                  trackFitter.getZOutPlane());
// Printf("Track:---------------------------");
//   for(int hit=0;hit<nClTimes;hit++) {
//     if(trackFitter.getWeight(hit)<.1) continue;    
//     Int_t mod=locClus[1]*2+address[hit][0];
//     Int_t lay=address[hit][1];
//     Int_t cell=address[hit][2];
//     HMdcSizesCellsLayer& pSCLayer=(*fSizesCells)[sec][mod][lay];
//     Float_t alfa=pSCLayer.getAlfa(v1,v2);
//     Float_t dist=trackFitter.getDistWire(hit);
//     Float_t distNew=pSCLayer.getDist(v1,v2,cell);
// Printf("S%i M%i L%i C%i alfa=%f dist=%f(%f) time=%f",sec+1,mod+1,lay+1,cell+1,
//     alfa,dist,distNew,trackFitter.getTDCTime(hit));
//   }
// //------------------------------------

	    locSeg[0]=locClus[0];
	    locSeg[1]=locClus[1];
	    HMdcSeg *fSeg = (HMdcSeg*)fSegCat->getNewSlot(locSeg);
	    if(!fSeg) continue;
	    fSeg = new(fSeg) HMdcSeg;
	    fSeg->setIOSeg(fClst->getIOSeg());
	    fSeg->setSec(sec);
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
	    fSeg->setPar(zm, errZm, r0, errR0, theta, errTheta, phi, errPhi);
          
	    
	    locHit[0]=sec;
	    Short_t modCl=fClst->getMod();
	    for(Int_t imod=0; imod<2; imod++) {
	      Int_t mod=imod+locClus[1]*2;
	      if(modCl>=0 && modCl!=mod) continue;
	      if(!fmdc->getModule(sec,mod)) continue;
            
	      locHit[1]=mod;
	      locHit[2]=indexHit[mod]++; 
	      HMdcHit * fHit = (HMdcHit*)fHitCat->getSlot(locHit);                          
	      if(!fHit) {                                                                   
		Error("execute","S.%i M.%i No slot HMdcHit available",sec+1,mod+1);                    
		continue;                                                                   
	      }                                                                             
	      fHit = new(fHit) HMdcHit;                                                     
	      fHit->setSecMod(sec,mod);                                                     
	      if(!fillHit(sec,mod,fHit)) return 1;
	      
// 	       ????           Int_t flag=0;                                                                 
// 		 for(Int_t layer=0; layer<6; layer++) {                                        
// 		   Int_t iLayer=layer+imod*6;                                                  
// 		   Int_t nHits=fClst->getNCells(iLayer);                                       
// 		   if( nHits <= 0 ) continue;                                                  
// 		   Int_t firstCell=fClst->getFirstCell(iLayer);                                
// 		   Int_t lastCell=fClst->getLastCell(iLayer);                                  
// 		   Int_t nMax=firstCell-lastCell;                                              
// 		   if(lastCell-firstCell >3 ) {                                                
// 		     nMax=3;                                                                   
// 		     flag=flag & (1<<layer);                                                   
// 		   }                                                                           
// 		   Int_t t[4]={0,0,0,0};                                                       
// 		   for(Int_t n=0; n<=nMax; n++) t[n]=fClst->getTime(iLayer,n+firstCell);       
// 
// 		   fHit->setSignId(layer,firstCell,t[0],t[1],t[2],t[3]);                       
// 		   if(fSeg) fSeg->setSignId(iLayer,firstCell,t[0],t[1],t[2],t[3]);             
// 		   flagSeg=flagSeg & flag<<mod*6;                                              
// 		 }
// 		 fHit->setFlag(flag);                                                          
// 		 flagSeg=flagSeg & flag<<imod*6;
                                                                   
              if(fSeg) fSeg->setHitInd(imod,fHitCat->getIndex(fHit));
	    }
	//=========================================================
	  }
	  if(fhists) {
	    trackTuple->Fill( trackFitter.getFunMin(),
			      trackFitter.getIterationNumber(),
			      nClTimes,
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
  }
  return 0;

}

Bool_t HMdc12Fit::executeClust(HMdcClus * fClst) {
  Float_t xVertex, yVertex, zVertex;
  
  if(fprint) fClst->print();
  
  listCells=*fClst;
  xPlane = fClst->getX();
  yPlane = fClst->getY();
  zPlane = fClst->getZ();
  xVertex = fClst->getXTarg();
  yVertex = fClst->getYTarg();
  zVertex = fClst->getZTarg();

  NMDCTime[0] = NMDCTime[1] = 0;
  nClTimes = 0;
  Short_t nseg = fClst->getIOSeg();
  Short_t sec = fClst->getSec();
  HMdcSizesCellsSec& fSCSec=(*fSizesCells)[sec];
  Float_t tOffset = 6;
  if(nseg > 0) tOffset = 2;

  for(Int_t module=0;module<2;module++) {       // relative number of module in segment
    loc[1] = module  + nseg*2;
    for(loc[2]=0;loc[2]<6;loc[2]++) {      // nl - Num. of layers in MDC1+MDC2
      Int_t nLayer=module*6+loc[2];
      Int_t nWires=listCells.getNCells(nLayer); // num. cells in layer
      if(nWires<=0) continue;
      loc[3]=-1;
      while((loc[3]=listCells.next(nLayer,loc[3])) > -1) {
	Int_t nTimes=listCells.getTime(nLayer,loc[3]); // num. times for nWire
	cal=(HMdcCal1*)fCalCat->getSlot(loc);
	if(!cal) continue;
	HMdcSizesCellsCell& fSCCell=fSCSec[loc[1]][loc[2]][loc[3]];

	const HGeomVector* wP1 = fSCCell.getWirePoint(0);
	const HGeomVector* wP2 = fSCCell.getWirePoint(1);
//---------------------------------------------------------------
	NMDCTime[module]++;

	address[nClTimes][0] = module;
	address[nClTimes][1] = loc[2];
	address[nClTimes][2] = loc[3];

	TDCTime[nClTimes] = cal->getTime1() + tOffset;

// 		  qualityWeight[nClTimes] = (nWires < 3) ? 1 : 1./nWires;
	qualityWeight[nClTimes] = 1./nWires;

	xLeft [nClTimes] = wP1->getX();
	xRight[nClTimes] = wP2->getX();
	yLeft [nClTimes] = wP1->getY();
	yRight[nClTimes] = wP2->getY();
	zLeft [nClTimes] = wP1->getZ();
	zRight[nClTimes] = wP2->getZ();

	nClTimes++;

        if(nTimes != 2) {
          TDCTime[nClTimes-1] = cal->getTime1() + tOffset;
          address[nClTimes-1][3]=1;
          if(nTimes == 3) {
	    if(cal->getTime2() - cal->getTime1() <= 50.) {
              listCells.delTime(loc[2]+module*2,loc[3],2); //???
              continue;
            }
	    NMDCTime[module]++;

	    TDCTime[nClTimes] = cal->getTime2() + tOffset;
            address[nClTimes][3]=2;

	    address[nClTimes][0] = module;
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
        } else {
          TDCTime[nClTimes-1] = cal->getTime2() + tOffset;
          address[nClTimes-1][3]=2;
        }
//----------------------------------------------------------------
      } // end of hit loop
    }// end of layer loop
  }// end of MDC loop

  if(NMDCTime[0] < 6 && NMDCTime[1] < 6) return kFALSE;

  trackFitter.setNSegment(nseg);

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

  trackFitter.setXVertex(xVertex);
  trackFitter.setYVertex(yVertex);
  trackFitter.setZVertex(zVertex);

  HGeomVector vtmp;

  HGeomVector trP1(0., 0., 0.);
  HGeomVector trP2(xPlane, yPlane, zPlane);

  HGeomVector pointCros1, pointCros2;
  HGeomVector pointCros3, pointCros4;
  Double_t dist1, dist2, dist3, dist4;

  if(&(fSCSec[0])) {
    calcCross( &trP1, &trP2, fSCSec[0].getSecTrans(), &pointCros1);
    dist1 = sqrt(pointCros1.getX()*pointCros1.getX()+pointCros1.getY()*
		 pointCros1.getY()+pointCros1.getZ()*pointCros1.getZ());
  }
  else dist1 = 0;

  if(&(fSCSec[1])) {
    calcCross( &trP1, &trP2, fSCSec[1].getSecTrans(), &pointCros2);
    dist2 =  sqrt(pointCros2.getX()*pointCros2.getX()+pointCros2.getY()*
		  pointCros2.getY()+pointCros2.getZ()*pointCros2.getZ());
  }
  else dist2 = 0;

  if(&(fSCSec[2])) {
    calcCross( &trP1, &trP2, fSCSec[2].getSecTrans(), &pointCros3);
    dist3 =  sqrt(pointCros3.getX()*pointCros3.getX()+pointCros3.getY()*
		  pointCros3.getY()+pointCros3.getZ()*pointCros3.getZ());
  }
  else dist3 = 0;

  if(&(fSCSec[3])) {
    calcCross( &trP1, &trP2, fSCSec[3].getSecTrans(), &pointCros4);
    dist4 =  sqrt(pointCros4.getX()*pointCros4.getX()+pointCros4.getY()*
		  pointCros4.getY()+pointCros4.getZ()*pointCros4.getZ());
  }
  else dist4 = 0;

  trackFitter.setDistVertexMDC1(dist1);
  trackFitter.setDistVertexMDC2(dist2);
  trackFitter.setDistVertexMDC3(dist3);
  trackFitter.setDistVertexMDC4(dist4);

  return kTRUE;

}

void HMdc12Fit::setHists(void) {
  for(Int_t j=0; j<6; j++) {
   for(Int_t i=0; i<200; i++) plots[i][j]=0;
  }
  for(Int_t i=0; i<200; i++) hist1[i]=0;
  for(Int_t i=0; i<200; i++) hist2[i]=0;
  hfile = new TFile("fitTuples.root","RECREATE","MDC hit and track NTuples");

//  hitTuple = new TNtuple("hit", "hits", "funMin:nHits:dev:weight:time:plane:cell:clnum:xV:yV:zV:xP:yP:zP:pV");
//   hitTuple = new TNtuple("hit", "hits", "funMin:nHits:dev:weight:time:plane:cell:mod:xV:yV:zV:xP:yP:zP:pV");
  hitTuple = new TNtuple("hit", "hits", "funMin:nHits:dev:weight:time:iter:nW:mod:xV:yV:zV:xP:yP:zP:pV");

  trackTuple = new TNtuple("track", "tracks", "funMin:iter:nHits:nPlanes:xV:yV:zV:xP:yP:zP:pV");

}


Bool_t HMdc12Fit::finalize(void) {
 
  if(fhists) {
    hfile->Write();
    hfile->Close();
  }
  return kTRUE;
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

Bool_t HMdc12Fit::fillHit(Int_t sec, Int_t mod, HMdcHit* fHit) {
  HGeomTransform trMdc;
  if( !fGetCont->getSecTransMod(trMdc,sec,mod) ) return kFALSE;
  HGeomVector cl(trackFitter.getXOutPlane(),trackFitter.getYOutPlane(),
      trackFitter.getZOutPlane());
  HGeomVector tg(trackFitter.getXOutVertex(),trackFitter.getYOutVertex(),
      trackFitter.getZOutVertex());
     
  cl    = trMdc.transTo(cl);
  tg    = trMdc.transTo(tg);
  
  Float_t x,y,l,m;
  x=(cl(0)*tg(2)-cl(2)*tg(0))/(tg(2)-cl(2));
  y=(cl(1)*tg(2)-cl(2)*tg(1))/(tg(2)-cl(2)); 
  Float_t dx=(cl(0)-tg(0));
  Float_t dy=(cl(1)-tg(1));
  Float_t dz=(cl(2)-tg(2));
  Float_t length=sqrt( dx*dx + dy*dy + dz*dz );
  l=dx/length; 
  m=dy/length;
       
  fHit->setXY(x,0.0,y,0.0);
  fHit->setXYDir(l,0.0,m,0.0);
  fHit->setChi2(trackFitter.getFunMin());
  
  return kTRUE;
}

ClassImp(HMdc12Fit)
