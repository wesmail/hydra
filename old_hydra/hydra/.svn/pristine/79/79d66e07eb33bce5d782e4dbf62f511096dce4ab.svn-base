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


//*-- Author : V.Pechenov
//*-- Modified: 17.10.00 A.Nekhaev

HMdc12Fit::HMdc12Fit() {
  fCalCat=0;
  fLookUpTb=0;
  fSizesCells=0;
  setParContainers();
  iter=0;
  fprint = kFALSE;
  //  fprint = kTRUE;

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
  fLookUpTb=(HMdcLookUpTb*)gHades->getRuntimeDb()->getContainer("MdcLookUpTb");
  if(!fLookUpTb) {
   printf("Error: MdcLookUpTb is absent\n");
   return;
  }
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
  if (!fCalCat) {
    return kFALSE;
  } 

 fSegCat = gHades->getCurrentEvent()->getCategory(catMdcSeg);
  if (!fSegCat) {
     fSegCat = gHades->getSetup()->getDetector("Mdc")
                                 ->buildCategory(catMdcSeg);
     if (!fSegCat) return kFALSE;
     else gHades->getCurrentEvent()->addCategory(catMdcSeg,fSegCat,"Mdc");
  }

  iter=(HIterator *)fCalCat->MakeIterator();
  fEventId=0;
  clSeqNum=0;
  cal=0;

  loc.set(4,0,0,0,0);

//Hists:
  setHists();

  HGeomVector vtmp;
//  for(Int_t i=0; i<12; i++) {
  for(Int_t i=6; i<12; i++) {
   sysLayers[i]=new HGeomTransform();
   Int_t is=getTransfMat(i/6,i%6,sysLayers[i]);        // mod=0
     if(is==0) {sysLayers[i]=0;continue;}    
     vtmp=sysLayers[i]->getTransVector();
     vtmp/=10.;                                 // mm -> cm
     sysLayers[i]->setTransVector(vtmp);  
  }
  return kTRUE;
}


Int_t HMdc12Fit::execute(void) {
  locSeg.set(2,0,0);
  fEventId++;
  //  Printf("=== EventId=%i",fEventId);
  loc.set(4,0,0,0,0);   // location used to fill the HMdcGeantCell category
  iter->Reset();
  Int_t numcl;
  Int_t  sumTracks;
  Int_t foundTracks;
  
  for(loc[0]=2; loc[0]<3; loc[0]++){
    locSeg[0]=loc[0];
    locSeg[1]=0;
//     HMdcSeg *seg = (HMdcSeg*)fSegCat->getNewSlot(locSeg);      
//     if(!seg) continue;
//     seg = new(seg) HMdcSeg;
//     seg->setIOSeg(0);
//     seg->setSec(loc[0]);
  
    //--------------------------------------
    HMdcLookUpTbSec& lookutsec = (*fLookUpTb)[loc[0]];
    //--------------------------------------------
    coordSys=&(lookutsec.getParPlane());
    
    // --- Fill plots:
    //   Char_t name[10],title[20];
    //   sprintf(name,"ev%3is%i%c",fEventId-1,loc[0],0);  
    //   if( name[2] == ' ') name[2]='0';
    //   if( name[3] == ' ') name[3]='0';
    //   sprintf(title,"Event %i Sector %i%c",fEventId-1,loc[0],0);
    //   plots[fEventId-1][loc[0]]=lookutsec.fillTH2C(name,title);
    
    //--------------------------------------------
    
    sumTracks = 0;
    foundTracks = 0;
    //     for(int nPlane = 12; nPlane > 7; nPlane--) {
    //     for(int nPlane = 6; nPlane > 3; nPlane--) {
    
    //       Float_t threshold = 170. * float(nPlane) / 12.;
    //       Float_t threshold = 500. * float(nPlane) / 12.;
    //        cout << " ======# Plane ====== " << nPlane << " threshold " << threshold << endl;
    //       Int_t findIter = 0;
    //       int nPlane = 6;
    //       while(lookutsec.findClusters(nPlane)) {
    
    // 	findIter++;
    //  	cout << " -------------------------findIter " << findIter << endl;
    
    HMdc12Clusters& clusters=lookutsec.getClusters();  
    Int_t nclst=clusters.getNClusters(); // Num. of clusters 
    // 	if (findIter == 1 && nPlane == 6) numcl = nclst;
    //         numcl+=nclst;
    if (fprint) Printf("\nSec.%i Num.Clusters=%i",loc[0],nclst);
    
    // 	Int_t gutHits = 0;
    
    for(Int_t nc=0; nc<nclst; nc++) {
      clSeqNum++;
      HMdcCluster& clst=clusters[nc];
      //- - - - - - - - - 
      executeClust(nc,clst);
      //- - - - - - - - -
      Float_t threshold = 10000000000.;
      // 	  if (trackFitter.getNMDC2Times() > 5 && 
      if (trackFitter.execute(threshold)) {
	
	foundTracks++;
	
	int NPlanes = 0;
	int oldPlane = -1;
	
	for(int hit = 0; hit < trackFitter.getNMDC2Times(); hit++) {
	  if(address[hit][1] != oldPlane) {
	    NPlanes++;
	    oldPlane = address[hit][1];
	  } 
	  
	  hitTuple->Fill( trackFitter.getFunMin(),
			  //			  trackFitter.getIterationNumber(),
			  trackFitter.getNMDC2Times(),
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
			  trackFitter.getNMDC2Times(),
			  NPlanes,
			  clst.getPrDb(),
			  trackFitter.getXOutVertex(),
			  trackFitter.getYOutVertex(),
			  trackFitter.getZOutVertex(),
			  trackFitter.getXOutPlane(),
			  trackFitter.getYOutPlane(),
			  trackFitter.getZOutPlane(),
			  trackFitter.getPVelocity());
		
	
      }
    }
    
    //  	cout << " foundTracks " << foundTracks << " gutHits " << gutHits << endl;
    
    //       }
    
  }
  return 0;
  // Save all objects in the file
  
}

void HMdc12Fit::executeClust(Int_t nc, HMdcCluster& clst) {

      printClust(nc, clst);
    //    if(fprint) printClust(nc, clst);
   
//    Int_t q1=clst.getQuality1();
//    Int_t q2=clst.getQuality2();

//    if( (q1+q2) > 2 ) return;
 
   xPlane = clst.getX();
   yPlane = clst.getY();
   zPlane = clst.getZ();
//    Printf("Cluster %i; Q:%i-%i; X=%f Y=%f Z=%f",nc,q1,q2,xPlane,yPlane,zPlane);
   //   Printf("Cluster %i; X=%f Y=%f Z=%f",nc,xPlane,yPlane,zPlane);

//    if(q1>q2) {Int_t qq=q1; q1=q2; q2=qq;}

//    Float_t dens=(Float_t)totalNhits/12.;

//    HGeomVector clV(clst.getXmid(),clst.getYmid(),clst.getZmid());
//    clV=coordSys->transTo(clV);
//------------------------------------------
//     Int_t nTrList[50];
//     Int_t nHitsTr[50];
//     Int_t nTracksClust=analClust(clst, nTrList, nHitsTr);
//-------------------------------------------
//    if(nTracksClust==1) {
//     Int_t nTr=nTrList[0];
//     Int_t indK;
//     Int_t ind=getInd(nTr, indK);
//     if(nTimesInLayer[ind][1][0] == 1) {
//      Int_t indM=indMdc[ind][1][0][0];
//      Float_t dX=mdc_root.xHit[indM]-(Float_t)clV.getX();
//      Float_t dY=mdc_root.yHit[indM]-(Float_t)clV.getY();
//      hist1[30]->Fill(dX,1.);
//      hist1[31]->Fill(dY,1.);
//      hist1[42]->Fill((Float_t)clst.getErrXmid(),1.);  // System ??? ->
//      hist1[43]->Fill((Float_t)clst.getErrYmid(),1.);
//      if(q1==1 && q2==1) {
//       hist1[32]->Fill(dX,1.);
//       hist1[33]->Fill(dY,1.);
//      } else if(q1==1 && q2==2) {
//       hist1[34]->Fill(dX,1.);
//       hist1[35]->Fill(dY,1.);
//      } else if(q1==1 && q2==3) {
//       hist1[36]->Fill(dX,1.);
//       hist1[37]->Fill(dY,1.);
//      } else if(q1==2 && q2==2) {
//       hist1[38]->Fill(dX,1.);
//       hist1[39]->Fill(dY,1.);
//      } else {
//       hist1[40]->Fill(dX,1.);
//       hist1[41]->Fill(dY,1.);
//      }
//     }
//    }
   
//    if (nTracksClust != 1) return;
   
//     Int_t nTr = nTrList[0];
//     Int_t indDigi = getIndDigi(nTr);
//     Int_t indK;
//     Int_t ind=getInd(nTr, indK);
   
//    if(qualTr[indDigi] != 0) return;
   
//    float px = kine_root.xMonTr-1];
//    float py = kine_root.yMom[nTr-1];
//    float pz = kine_root.zMom[nTr-1];
//    float p = sqrt(px*px + py*py + pz*pz);
//    if(sqrt(px*px + py*py + pz*pz) < 500) return;
   
   NMDCTime[0] = NMDCTime[1] = 0;
   nClTimes = 0;
   //   for(loc[1]=0;loc[1]<2;loc[1]++) {
   for(loc[1]=1;loc[1]<2;loc[1]++) {
      
      for(loc[2]=0;loc[2]<6;loc[2]++) {      // nl - Num. of layers in MDC1+MDC2
	 
	 Int_t nLayer=loc[1]*6+loc[2];
	 Int_t nWires=clst.getNumHits(nLayer);   // num. fired wires in this clusters for loc
	 for(Int_t nW=0; nW<nWires; nW++) {   
	    loc[3]=clst.getHit(nLayer,nW);         // num. fired wire
	    Int_t nTimes=clst.getTimes(nLayer,nW); // num. times for nWire    
	    cal=(HMdcCal1*)fCalCat->getSlot(loc);
	    if(!cal) {
// 	       Printf("Error: Hit S%i M%i L%i C%i is absent",loc[0],loc[1],loc[2],loc[3]);
	    } else {  
      
	       HMdcSizesCellsCell& pSCCell=(*fSizesCells)[loc[0]][loc[1]][loc[2]][loc[3]];

	       const HGeomVector* wP1 = pSCCell.getWirePoint(0);
	       const HGeomVector* wP2 = pSCCell.getWirePoint(1);
//---------------------------------------------------------------      
// 	       Int_t nTr1 = cal->getNTrack1();
//      	       if(nTr1 == nTrList[0]) {
		  NMDCTime[loc[1]]++;
			
		  address[nClTimes][0] = loc[1];
		  address[nClTimes][1] = loc[2];
		  address[nClTimes][2] = loc[3];

		  TDCTime[nClTimes] = cal->getTime1();

// 		  qualityWeight[nClTimes] = (nWires < 3) ? 1 : 1./nWires;
		  qualityWeight[nClTimes] = 1./nWires;
		  
		  xLeft [nClTimes] = wP1->getX();
		  xRight[nClTimes] = wP2->getX();
		  yLeft [nClTimes] = wP1->getY();
		  yRight[nClTimes] = wP2->getY();
		  zLeft [nClTimes] = wP1->getZ();
		  zRight[nClTimes] = wP2->getZ();
		  nClTimes++;
//      	       }

	       if(nTimes == 2) {  

// 		  Int_t nTr2 = cal->getNTrack2();

//      		  if(nTr2 == nTrList[0]) {
		    if(cal->getTime2() - cal->getTime1() > 50) {
		      NMDCTime[loc[1]]++;
			
		      TDCTime[nClTimes] = cal->getTime2();
		     
		      address[nClTimes][0] = loc[1];
		      address[nClTimes][1] = loc[2];
		      address[nClTimes][2] = loc[3];
		      
		      qualityWeight[nClTimes] = 1./nWires;
// 		      qualityWeight[nClTimes] = (nWires < 3) ? 1 : 1./nWires;

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
   //   for(Int_t i=0; i<12; i++) {
   for(Int_t i=6; i<12; i++) {
     //     sysLayers[i]=new HGeomTransform();
     Int_t is=getTransfMat(i/6,i%6,sysLayers[i]);        // mod=0
     if(is==0) sysLayers[i]=0;
     vtmp=sysLayers[i]->getTransVector();
     //     vtmp/=10.;                                 // mm -> cm
     sysLayers[i]->setTransVector(vtmp);  
   }
  
   HGeomVector trP1(0., 0., 0.);
   HGeomVector trP2(xPlane, yPlane, zPlane);

   
   HGeomVector pointCros;
   calcCross( &trP1, &trP2, sysLayers[6], &pointCros);
   //   pointCros=sysLayers[6]->transTo(pointCros);
   Double_t dist =  sqrt(pointCros.getX()*pointCros.getX()+pointCros.getY()*
			 pointCros.getY()+pointCros.getZ()*pointCros.getZ());
   trackFitter.setDistVertexMDC1(dist);
   trackFitter.setDistVertexMDC2(dist);

   trackFitter.distVertex();

//    if (trackFitter.execute()) {
   
//    Float_t funMin;
//    //   Int_t   status;

//    funMin = (Float_t)trackFitter.getFunMin();
//    //   status = trackFitter.getStatus();
//                //              if (status < 3 || funMin > 500) continue; 
             
//    hist1[195]->Fill(funMin, 1.);
//    hist2[0]->Fill(funMin, (NMDCTime[0]+NMDCTime[1]), 1.);
//    if(funMin > 500) return;
//    //   h6->Fill(status, 1.);
// // Calculating crosses track with layers in MDC1&MDC2
// //    cout << " xyz plane " << trackFitter.getXOutPlane() << " " << trackFitter.getYOutPlane() << " " << trackFitter.getZOutPlane() <<endl;
// //    cout << " xyz vertex " << trackFitter.getXOutVertex() << " " << trackFitter.getYOutVertex() << " " << trackFitter.getZOutVertex() <<endl;
//      HGeomVector trP2(trackFitter.getXOutPlane(), trackFitter.getYOutPlane(), trackFitter.getZOutPlane());
//      HGeomVector trP1(trackFitter.getXOutVertex(), trackFitter.getYOutVertex(), trackFitter.getZOutVertex());
//      HGeomVector pointCros;
     
//      for(Int_t i=0; i<12; i++) {
//       int indM = indMdc[ind][i/6][i%6][0];
//       calcCross( &trP1, &trP2, sysLayers[i], &pointCros);
//       pointCros=sysLayers[i]->transTo(pointCros);

//       Float_t dXi = mdc_root.xHit[indM]-(Float_t)pointCros.getX();
//       Float_t dYi = mdc_root.yHit[indM]-(Float_t)pointCros.getY();
// //       cout << " indM " << indM << " dXi " << dXi << " dYi " << dYi <<
// // 	" mdc_root.xHit[indM]  " << mdc_root.xHit[indM] << " pointCros.getX() " << pointCros.getX() <<
// // 	" mdc_root.yHit[indM]  "<< mdc_root.yHit[indM] << " pointCros.getY() " << pointCros.getY() << endl;
      
//       hist1[101+i]->Fill(dXi,1.);
//       hist1[121+i]->Fill(dYi,1.);
//      }
//    }
   

//    for(loc[1]=0;loc[1]<2;loc[1]++) { 
//     for(loc[2]=0;loc[2]<6;loc[2]++) {      // nl - Num. of layers in MDC1+MDC2
//      Int_t nLayer=loc[1]*6+loc[2];
//      Int_t nWires=clst.getNumHits(nLayer);   // num. fired wires in this clusters for loc
//      for(Int_t nW=0; nW<nWires; nW++) {   
//       loc[3]=clst.getHit(nLayer,nW);         // num. fired wire
//       Int_t nTimes=clst.getTimes(nLayer,nW); // num. times for nWire         
//       cal=(HMdcCal1Sim*)fCalCat->getSlot(loc);
//       if(!cal) {
//        Printf("Error: Hit S%i M%i L%i C%i is absent",loc[0],loc[1],loc[2],loc[3]);
//       } else {  
      
//       HMdcSizesCellsCell& pSCCell=(*fSizesCells)[loc[0]][loc[1]][loc[2]][loc[3]];

// //---------------------------------------------------------------      
//        Float_t time1=cal->getTime1();
//        Int_t nTr1=cal->getNTrack1();
//        if(nTimes==2) {  
//         Float_t time2=cal->getTime2();     
//         Int_t nTr2=cal->getNTrack2();
//        }      
// //----------------------------------------------------------------
//       }
//      }     
//     }
//    } 
}

void HMdc12Fit::setHists(void) {
  for(Int_t j=0; j<6; j++) {
   for(Int_t i=0; i<200; i++) plots[i][j]=0;
  }
  for(Int_t i=0; i<200; i++) hist1[i]=0;
  for(Int_t i=0; i<200; i++) hist2[i]=0;
  hfile = new TFile("fitTuples_48mm.root","RECREATE","MDC hit and track NTuples");

  //  hitTuple = new TNtuple("hit", "hits", "funMin:iter:nHits:dev:weight:time:plane:cell:xV:yV:zV:xP:yP:zP:pV");
  hitTuple = new TNtuple("hit", "hits", "funMin:nHits:dev:weight:time:plane:cell:clnum:xV:yV:zV:xP:yP:zP:pV");

  trackTuple = new TNtuple("track", "tracks", "funMin:iter:nHits:nPlanes:prdv:xV:yV:zV:xP:yP:zP:pV");
  
//   hist1[0] = new TH1F("h0","Min of Functional",300,0,3000);
//   hist1[1] = new TH1F("h1","Min of Functional",300,0,3000);
//   hist1[2] = new TH1F("h2","Min of Functional",300,0,3000);
//   hist1[3] = new TH1F("h3","Min of Functional",300,0,3000);
//   hist1[4] = new TH1F("h4","Min of Functional",300,0,3000);
//   hist1[5] = new TH1F("h5","Min of Functional",300,0,3000);
//   hist1[6] = new TH1F("h6","Min of Functional",300,0,3000);
//   hist1[7] = new TH1F("h7","Min of Functional",300,0,3000);
//   hist1[8] = new TH1F("h8","Min of Functional",300,0,3000);
//   hist1[9] = new TH1F("h9","Min of Functional",300,0,3000);
//   hist1[10] = new TH1F("h10","Min of Functional",300,0,3000);
//   hist1[11] = new TH1F("h11","Min of Functional",300,0,3000);
//   hist1[12] = new TH1F("h12","Min of Functional",300,0,3000);
//   hist1[13] = new TH1F("h13","Min of Functional",300,0,3000);
//   hist1[14] = new TH1F("h14","Min of Functional",300,0,3000);
//   hist1[100] = new TH1F("h100"," Efficiency ",50,0,1);
//   hist1[101] = new TH1F("h101"," XVertex resolution ",100, -.5, .5);
//   hist1[102] = new TH1F("h102"," YVertex resolution ",100, -.5, .5);
//   hist1[103] = new TH1F("h103"," ZVertex resolution ",100, -.5, .5);
//   hist2[0] = new TH2F("h200"," Trackfinder efficiency vs NClst ",50,0,1,50,0,50);
//   hist2[1] = new TH2F("h201"," Trackfinder efficiency vs P ",50,0,1, 50,0,4000);
  
//   hist2[  0] = new TH2F("2h0","Min of Functional vs number of times",100,0,15000,100,0,100);
/*  hist1[ 121]=new TH1F("h121","Xgeant-Xtrack (MDC1,layer 1)",200,-.4,.4);
  hist1[ 122]=new TH1F("h122","Xgeant-Xtrack (MDC1,layer 2)",200,-.4,.4);
  hist1[ 123]=new TH1F("h123","Xgeant-Xtrack (MDC1,layer 3)",200,-.4,.4);
  hist1[ 124]=new TH1F("h124","Xgeant-Xtrack (MDC1,layer 4)",200,-.4,.4);
  hist1[ 125]=new TH1F("h125","Xgeant-Xtrack (MDC1,layer 5)",200,-.4,.4);
  hist1[ 126]=new TH1F("h126","Xgeant-Xtrack (MDC1,layer 6)",200,-.4,.4);
  hist1[ 127]=new TH1F("h127","Xgeant-Xtrack (MDC2,layer 1)",200,-.4,.4);
  hist1[ 128]=new TH1F("h128","Xgeant-Xtrack (MDC2,layer 2)",200,-.4,.4);
  hist1[ 129]=new TH1F("h129","Xgeant-Xtrack (MDC2,layer 3)",200,-.4,.4);
  hist1[ 130]=new TH1F("h130","Xgeant-Xtrack (MDC2,layer 4)",200,-.4,.4);
  hist1[ 131]=new TH1F("h131","Xgeant-Xtrack (MDC2,layer 5)",200,-.4,.4);
  hist1[ 132]=new TH1F("h132","Xgeant-Xtrack (MDC2,layer 6)",200,-.2,.2);
  
  hist1[ 141]=new TH1F("h141","Ygeant-Ytrack (MDC1,layer 1)",200,-.2,.2);
  hist1[ 142]=new TH1F("h142","Ygeant-Ytrack (MDC1,layer 2)",200,-.2,.2);
  hist1[ 143]=new TH1F("h143","Ygeant-Ytrack (MDC1,layer 3)",200,-.2,.2);
  hist1[ 144]=new TH1F("h144","Ygeant-Ytrack (MDC1,layer 4)",200,-.2,.2);
  hist1[ 145]=new TH1F("h145","Ygeant-Ytrack (MDC1,layer 5)",200,-.2,.2);
  hist1[ 146]=new TH1F("h146","Ygeant-Ytrack (MDC1,layer 6)",200,-.2,.2);
  hist1[ 147]=new TH1F("h147","Ygeant-Ytrack (MDC2,layer 1)",200,-.2,.2);
  hist1[ 148]=new TH1F("h148","Ygeant-Ytrack (MDC2,layer 2)",200,-.2,.2);
  hist1[ 149]=new TH1F("h149","Ygeant-Ytrack (MDC2,layer 3)",200,-.2,.2);
  hist1[ 150]=new TH1F("h150","Ygeant-Ytrack (MDC2,layer 4)",200,-.2,.2);
  hist1[ 151]=new TH1F("h151","Ygeant-Ytrack (MDC2,layer 5)",200,-.2,.2);
  hist1[ 152]=new TH1F("h152","Ygeant-Ytrack (MDC2,layer 6)",200,-.2,.2); */
  
//  hist1[160] = new TH1F("h160"," Min. dist. between track and GEANT vertex ",100, -5., 5.);
  
//   hist1[170] = new TH1F("h170"," number of hits from track in cluster ",100, 0., 50.);
//   hist1[171] = new TH1F("h171"," number of background hits in cluster ",100, 0., 50.);

//   hist1[180] = new TH1F("h180"," particle velocity ",100, 0., 150.);
//  hist2[3] = new TH2F("h203"," particle velocity vs momentum ",50,25,30,25,0,5000);
}

void HMdc12Fit::writeHists(void) {
//Writing hists:
/* TFile *fh=new TFile("fulltest.root","RECREATE");
 for(Int_t i=0; i<200; i++) if(hist1[i]) hist1[i]->Write();
 for(Int_t i=0; i<200; i++) if(hist2[i]) hist2[i]->Write();
 fh->Close();
//Write plots:
 TFile *plot=new TFile("plots.root","RECREATE");
 for(Int_t i=0; i<200; i++) {
  for(Int_t j=0; j<6; j++) if(plots[i][j]) plots[i][j]->Write();
 }
 plot->Close();
*/
}

Bool_t HMdc12Fit::finalize(void) {
  hfile->Write();
  hfile->Close();
  return kTRUE;
}

void HMdc12Fit::printClust(Int_t nc, HMdcCluster& clst) {
  Bool_t pr=kFALSE;
  // Bool_t pr=kTRUE;
 if(pr) {
   Printf("\nCluster %i; Q:%i-%i; X=%f+/-%f Y=%f+/-%f Z=%f+/-%f",
   nc,clst.getQuality1(),clst.getQuality2(),
   clst.getX(),clst.getErrX(),
   clst.getY(),clst.getErrY(),
   clst.getZ(),clst.getErrZ());
  Printf("Cluster's window,  LeftUp:   X=%f Y=%f Z=%f",
   clst.getXleft(),clst.getYup(),clst.getZup());
  Printf("                   RightDown X=%f Y=%f Z=%f",
   clst.getXright(),clst.getYdown(),clst.getZdown());
 }
//------------------------------------------
/*   Int_t nTrList[50];
   Int_t nHitsTr[50];
   Int_t nTracksClust=analClust(clst, nTrList, nHitsTr);
if(pr) Printf("%i tracks in clusters nTr[nHits(nHitsTot)]qual x,y,z(in Mdc1, Layer0):",
                                                         nTracksClust); */
/*for(Int_t i=0; i<nTracksClust; i++) {
 Int_t nTrI=nTrList[i];
 Int_t indDigi=getIndDigi(nTrI);
if(pr) {
 Int_t indKn;
 Int_t indGn=getInd(nTrI, indKn);
 Int_t nTmmm=nTimesInLayer[indGn][1][0];
 printf(" %i[%i(%i)]%i",
  nTrList[i],nHitsTr[i],nDigHits[indDigi],qualTr[indDigi]);
 if(nTmmm == 1) {
  Int_t j=indMdc[indGn][1][0][0];
//  HGeomVector hit(mdc_root.xHit[j],mdc_root.yHit[j],0.0);
//  hit=coordSys->transFrom(hit);
//  printf("   x=%f  y=%f z=%f\n", hit.getX(),hit.getY(),hit.getZ());
  
//  xPlane = hit.getX();
//  yPlane = hit.getY(); 
//  zPlane = hit.getZ(); 

  }
 else
  printf("  Track crossed Layer 0 in MDC1 %i times !!!!\n", nTmmm);
}
findedTr[indDigi]++;
}*/
//-------------------------------------------
 //   for(loc[1]=0;loc[1]<2;loc[1]++) { 
 trackFitter.setFlag(0);
   for(loc[1]=1;loc[1]<2;loc[1]++) { 
    for(loc[2]=0;loc[2]<6;loc[2]++) {      // nl - Num. of layers in MDC1+MDC2
     Int_t nLayer=loc[1]*6+loc[2];
     Int_t nWires=clst.getNumHits(nLayer);   // num. fired wires in this clusters for loc
if(pr)printf("%2i) M%i L%i C(nT):", nLayer,loc[1],loc[2]);
     for(Int_t nW=0; nW<nWires; nW++) {   
      loc[3]=clst.getHit(nLayer,nW);         // num. fired wire
      Int_t nTimes=clst.getTimes(nLayer,nW); // num. times for nWire
if(pr)printf(" %i(%i)", loc[3],nTimes);  
// ANB
 if(loc[2]==2||loc[2]==5) {
   if(loc[3]==44||loc[3]==45||loc[3]==46||loc[3]==47||loc[3]==48||loc[3]==49||loc[3]==50||loc[3]==51||loc[3]==52||loc[3]==53) {
     cal=(HMdcCal1*)fCalCat->getSlot(loc);     
     if(!cal) {
       Printf("Error: Hit S%i M%i L%i C%i is absent",loc[0],loc[1],loc[2],loc[3]);
     } else {  
//        cout << endl << "layer 6, cell 47" << endl;    
//        cout << "for track s:" << loc[0] << " m: " << loc[1] << " l: " << loc[2] << " c: " << loc[3] << endl;
//        cout << "time1 = " << cal->getTime1() << endl;
//        cout << "time2 = " << cal->getTime2() << endl;
//        cout << "nW = " << (nW+1) << endl;
       trackFitter.setFlag(nW+1);
       trackFitter.setLayCell(loc[2],loc[3]);
     }
   }
 }

// ANE      
      cal=(HMdcCal1*)fCalCat->getSlot(loc);
      if(!cal) {
       Printf("Error: Hit S%i M%i L%i C%i is absent",loc[0],loc[1],loc[2],loc[3]);
      } else {  

//---------------------------------------------------------------
/*       Int_t nTr1=cal->getNTrack1();
if(pr)printf("[%i",nTr1);
       if(nTimes==2) {     
        Int_t nTr2=cal->getNTrack2();
if(pr)printf(";%i",nTr2);
       }
if(pr)printf("]");
*/   
//----------------------------------------------------------------

      }
     }
if(pr)printf("\n");     
    }
   } 
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
