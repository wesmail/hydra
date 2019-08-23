#include "hmdc12fit.h"
#include "hdebug.h"
#include "hiterator.h"
#include "hspectrometer.h"
#include "hmatrixcategory.h"
#include "hlocation.h"
#include <iostream.h>
#include "hmdcsizescells.h"
#include "hgeomvector.h"
#include "hmdcsegsim.h"
#include "hmdchitsim.h"
#include "hmdcgetcontainers.h"
#include "hmdcclussim.h"
#include "hmdcdetector.h"

//*-- Author : V.Pechenov
//*-- Modified: 16.08.01 V.Pechenov
//*-- Modified: 17.10.00 A.Nekhaev

HMdc12Fit::HMdc12Fit(Bool_t hst, Bool_t prnt, Int_t ver) { 
  fprint = prnt;
  fntuple = hst;
  version = ver;
  setParContainers();
}

HMdc12Fit::HMdc12Fit(Text_t *name, Text_t *title, Bool_t hst, Bool_t prnt,
    Int_t ver) : HReconstructor(name,title) {  
  fprint = prnt;
  fntuple = hst;
  version = ver;
  setParContainers();
}

HMdc12Fit::~HMdc12Fit() {
  HMdcSizesCells::deleteCont();
}

void HMdc12Fit::setParContainers() {
  trackFitter.setPrintFlag(fprint);
  trackFitter.setVersion(version);
  fCalCat=0;
  fSegCat=0;
  fSizesCells=0;
  modForSeg[0]=1;
  modForSeg[1]=2;
  trackFitter.setHitStatus (hitStatus);
  trackFitter.setTDCTime(TDCTime);
  trackFitter.setQualityWeight(qualityWeight);
  trackFitter.setXLeft(xLeft);
  trackFitter.setYLeft(yLeft);
  trackFitter.setZLeft(zLeft);
  trackFitter.setXRight(xRight);
  trackFitter.setYRight(yRight);
  trackFitter.setZRight(zRight);
  trackFitter.setSizeCellsLayer(fSizeCellsLayer);
  trackFitter.setListLayers(layIndx);
  trackFitter.setListModules(modIndx);
  trackFitter.setListCells(cellIndx);
}

void HMdc12Fit::fillSegByMod(Int_t ms1, Int_t ms2) {
  // It's will used for independent track finder mode
  // ms1=0,1 - if track finder have independend mode the HMdcSeg will
  //           filled via hit in MDC1 or MDC2
  // ms1<0 -the HMdcSeg will not filled
  // ms2=2,3 - MDC3 or MDC4 ...
  modForSeg[0]=(ms1>1) ? 1:ms1;
  modForSeg[1]=(ms2>3) ? 3:ms1;
  if(modForSeg[1]>=0 && modForSeg[1]<2) modForSeg[1]=2;
}

Bool_t HMdc12Fit::init(void) {
  fCalCat=HMdcGetContainers::getCatMdcCal1();
  if (!fCalCat) return kFALSE;
  fClusCat=HMdcGetContainers::getCatMdcClus();
  if( !fClusCat ) return kFALSE;

  isGeant=HMdcGetContainers::isGeant();
  HMdcDetector* fMdcDet=(HMdcDetector*)(gHades->getSetup()->getDetector("Mdc"));      
  if(modForSeg[0]>=0 && modForSeg[1]>0) {
    fSegCat = gHades->getCurrentEvent()->getCategory(catMdcSeg);
    if(!fSegCat) {
      if(isGeant) fSegCat = fMdcDet->buildMatrixCategory("HMdcSegSim",0.5F);
      else fSegCat = fMdcDet->buildCategory(catMdcSeg);
      if (!fSegCat) return kFALSE;
      else gHades->getCurrentEvent()->addCategory(catMdcSeg,fSegCat,"Mdc");
    }
  }

  fHitCat = gHades->getCurrentEvent()->getCategory(catMdcHit);
  if (!fHitCat) {
    if(isGeant) fHitCat = fMdcDet->buildMatrixCategory("HMdcHitSim",0.5F);          
    else fHitCat = fMdcDet->buildCategory(catMdcHit);                               
    if (!fHitCat) return kFALSE;                                                    
    else gHades->getCurrentEvent()->addCategory(catMdcHit,fHitCat,"Mdc");
  }

//  iter=(HIterator *)fCalCat->MakeIterator();
  iterClus=(HIterator *)fClusCat->MakeIterator();
  
  fSizesCells=HMdcSizesCells::getObject();
  if (!fSizesCells) {
   Error("init","HMdcSizesCells is absent");
   return kFALSE;
  }

  loc.set(4,0,0,0,0);

//Hists:
  if(fntuple) setNTuples();
  else {
    hfile=0;
    hitTuple=trackTuple=0;
  }

  fGetCont=HMdcGetContainers::getObject();
  fMdcDetector= fGetCont->getMdcDetector();
  if(!fMdcDetector) return kFALSE;
  locSeg.set(2,0,0);
  locHit.set(3,0,0,0);
  locClus.set(2,0,0);
  trackFitter.init();
  return kTRUE;
}

Bool_t HMdc12Fit::reinit(void) {
  for(Int_t sec=0; sec<6; sec++) {
    nModsSeg[sec][0]=nModsSeg[sec][1]=0;
    for(Int_t mod=0; mod<4; mod++)
     if(fMdcDetector->getModule(sec,mod)) nModsSeg[sec][mod/2]++;
  }
  trackFitter.reinit();
  if(!fSizesCells) return kFALSE;
  return fSizesCells->initContainer();
}


Int_t HMdc12Fit::execute(void) {
//  iter->Reset();
  iterClus->Reset();
  Int_t maxNumSectors=((HMatrixCategory*)fClusCat)->getSize(0);
  Int_t maxNumSegments=((HMatrixCategory*)fClusCat)->getSize(1);
  for(locClus[0]=0; locClus[0]<maxNumSectors; locClus[0]++){
    for(locClus[1]=0; locClus[1]<maxNumSegments; locClus[1]++) {
//      iterClus->Reset();
      iterClus->gotoLocation(locClus);
      indexHit[0]=indexHit[1]=indexHit[2]=indexHit[3]=0;
      while((fClst=(HMdcClus*)iterClus->Next())) {
        if(!fClst->getStatus()) continue;
        HMdcList12GroupCells listCellsOrg=*fClst;
        if(fprint) fClst->print();
        if(!fillArrays(listCellsOrg)) return kFALSE;
        nFitedTimes=nClTimes;
        //                     MDC1(3)                      MDC2(4)  ???
        while(listCellsOrg.getNCells(0,5)>4 || listCellsOrg.getNCells(6,11)>4) {
          listCells=listCellsOrg;
//          calcMinFunctional();
          if(!fitCluster(1000000000000000.)) break; // (threshold)       
//???          unsetHits(listCellsOrg);
          break; //???
        }
      }
    }
  }
  return 0;
}

void HMdc12Fit::calcMinFunctional(void) {
  Float_t xCl=fClst->getX();
  Float_t yCl=fClst->getY();
  Float_t errX=fClst->getErrX()*2.; // +/-dX
  Float_t errY=fClst->getErrY()*2.; // +/-dX 
  Int_t nBinsX=TMath::Max(Int_t(errX*2./2.5),1);  //  / 2.5 mm
  Int_t nBinsY=TMath::Max(Int_t(errY*2./1.0),1);  //  / 1.0 mm
  if(nBinsX%2 == 0) nBinsX++;
  if(nBinsY%2 == 0) nBinsY++;
  Float_t stepX=errX/nBinsX;
  Float_t stepY=errY/nBinsY;
  Float_t xStart=xCl-nBinsX/2*stepX;
  Float_t yStart=yCl-nBinsY/2*stepY;
  Float_t minFunc=1.e+20;
Float_t xMin=xCl;
Float_t yMin=yCl;
Printf("xCl=%f, yCl=%f x:%i %f  y:%i %f",xCl,yCl,nBinsX,stepX,nBinsY,stepY);
  for(Int_t ny=0; ny<nBinsY; ny++) {
    Float_t yNew=yStart+stepY*ny;
printf("Y=%7.6g", yNew);
    for(Int_t nx=0; nx<nBinsX; nx++) {
      Float_t xNew=xStart+stepX*nx;
      zPlane = fClst->getZ();
      trackFitter.setXInitPlane(xNew);
      trackFitter.setYInitPlane(yNew);
      trackFitter.setZInitPlane(fClst->getZOnPlane(xNew,yNew));
      Double_t func=trackFitter.getFunctional();
printf(" %7.6g",func);
      if(func<minFunc) {
        minFunc=func;
        xMin=xNew;
        yMin=yNew;
      }
    }
printf("\n");
  }
printf("..... xMin=%f, yMin=%f funMin=%f\n\n",xMin,yMin,minFunc);
trackFitter.setXInitPlane(xMin);
trackFitter.setYInitPlane(yMin);
trackFitter.setZInitPlane(fClst->getZOnPlane(xMin,yMin));
//   trackFitter.setXInitPlane(xPlane);
//   trackFitter.setYInitPlane(yPlane);
//   trackFitter.setZInitPlane(zPlane);
}

void HMdc12Fit::unsetHits(HMdcList12GroupCells& lCells) {
  for(Int_t lay=0;lay<12;lay++) {
    Int_t cell=-1;
    while((cell=listCells.next(lay,cell)) > -1) {
      Int_t time=listCells.getTime(lay,cell); // time=1||2, not 3 !!!
      // ??? check on wight ???
      if(time>0 && time<3) {
        hitStatus[hitIndex[lay][cell][time-1]]=0;
        lCells.delTime(lay,cell,time);
        nFitedTimes--;
      }
      else if(time == 3) {
        hitStatus[hitIndex[lay][cell][0]]=0;
        hitStatus[hitIndex[lay][cell][1]]=0;
        lCells.delTime(lay,cell,3);
        nFitedTimes-=2;
      }
    }
  }
}
        
Bool_t HMdc12Fit::fillArrays(HMdcList12GroupCells& lCells) {
  Float_t xVertex, yVertex, zVertex;
  
  memset(hitIndex,-1,sizeof(hitIndex));
  
  xPlane = fClst->getX();
  yPlane = fClst->getY();
  zPlane = fClst->getZ();
  xVertex = fClst->getXTarg();
  yVertex = fClst->getYTarg();
  zVertex = fClst->getZTarg();

  NMDCTime[0] = NMDCTime[1] = 0;
  nClTimes = 0;
  segment = fClst->getIOSeg();
  sector = fClst->getSec();
  HMdcSizesCellsSec& fSCSec=(*fSizesCells)[sector];

  loc[0] = sector;
  for(Int_t mod=0; mod<2; mod++) {       // relative number of module in segment
    loc[1] = mod  + segment*2;
    for(loc[2]=0;loc[2]<6;loc[2]++) {      // nl - Num. of layers in MDC1+MDC2
      Int_t nLayer=mod*6+loc[2];
      Int_t nWires=lCells.getNCells(nLayer); // num. cells in layer
      if(nWires<=0) continue;
      loc[3]=-1;
      while((loc[3]=lCells.next(nLayer,loc[3])) > -1) {
	Int_t nTimes=lCells.getTime(nLayer,loc[3]); // num. times for nWire
	HMdcCal1* cal=(HMdcCal1*)fCalCat->getObject(loc);
	if(!cal) {
          Error("fillArrays","S.%i M.%i L.%i  Cell %i don't exist",
              loc[0]+1,loc[1]+1,loc[2]+1,loc[3]+1);
          continue;
        }
        HMdcSizesCellsLayer& fSCLay=fSCSec[loc[1]][loc[2]];
        HMdcSizesCellsCell& fSCCell=fSCLay[loc[3]];

	const HGeomVector* wP1 = fSCCell.getWirePoint(0);
	const HGeomVector* wP2 = fSCCell.getWirePoint(1);
//---------------------------------------------------------------
        for(Int_t time=1; time<3; time++) {
          if((time&nTimes) == 0) continue;
          if(time==1) TDCTime[nClTimes] = cal->getTime1();
          else {
            if(isGeant && cal->getTime2() - cal->getTime1() <= 50.) {
              lCells.delTime(loc[2]+mod*2,loc[3],2); //???
              continue;
            }
            TDCTime[nClTimes] = cal->getTime2();
          }
          NMDCTime[mod]++;
          fSizeCellsLayer[nClTimes]=&fSCLay;
          modIndx[nClTimes]=loc[1];
          layIndx[nClTimes]=loc[2];
          cellIndx[nClTimes]=loc[3];
          timeIndx[nClTimes]=time;
          hitIndex[mod*6+loc[2]][loc[3]][time-1]=nClTimes;
          
          qualityWeight[nClTimes] = 1./nWires;
          
          xLeft [nClTimes] = wP1->getX();
	  xRight[nClTimes] = wP2->getX();
	  yLeft [nClTimes] = wP1->getY();
	  yRight[nClTimes] = wP2->getY();
	  zLeft [nClTimes] = wP1->getZ();
	  zRight[nClTimes] = wP2->getZ();
          hitStatus[nClTimes] = 1;
          
          nClTimes++;
        }
//---------------------------------------------------------------
      }  // end of hit loop
    }    // end of layer loop
  }      // end of MDC loop

  trackFitter.setAddress(sector,segment);

  trackFitter.setNMDC1Times(NMDCTime[0]);
  trackFitter.setNMDC2Times(NMDCTime[1]);

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

Bool_t HMdc12Fit::fitCluster(Float_t threshold) {
  Int_t iter=0;
  do if(trackFitter.execute(threshold,iter++)==0) return kFALSE; //???
  while ( testFitResult() > 1.e+20 && // 1. && ???
          (listCells.getNCells(0,5)>4 || listCells.getNCells(6,11)>4) );
  for(Int_t n=0; n<nClTimes; n++) if(hitStatus[n]<0) hitStatus[n]=1;
  if(listCells.getNDrTimes()>=5) fillMdcHitSeg();  // ???
  else return kFALSE;
  if(fntuple) fillNTuples();
  return kTRUE;
}

Float_t HMdc12Fit::testFitResult() {
  // Selection of hits (needs in checking ???). 
  Float_t sumDelWeight=0;
  for(int hit=0;hit<nClTimes;hit++) {
    if(hitStatus[hit] != 1) continue;
    Int_t mod=modIndx[hit]%2;
    Int_t lay=layIndx[hit];  
    Int_t cell=cellIndx[hit];
    Int_t time=timeIndx[hit];
    Float_t weight=trackFitter.getWeight(hit);

    if(weight<.9) {                   // selecting of hits by weight
      // new fit ???
      listCells.delTime(mod*6+lay,cell,time);
      sumDelWeight+=weight;
      hitStatus[hit]=-1;
    } 
    if(listCells.getTime(mod*6+lay,cell) == 3) {  // Duble hit ...
      // new fit ???
      Int_t dHit=(weight < trackFitter.getWeight(hit+1)) ? hit:hit+1;
      listCells.delTime(mod*6+lay,cell,timeIndx[dHit]);
      sumDelWeight+=trackFitter.getWeight(dHit);
      hitStatus[dHit]=-1;
    }
  }
  for(Int_t lay=0;lay<12;lay++) {
    while(listCells.getLastCell(lay)-listCells.getFirstCell(lay)>1) {
      // new fit ???
      Int_t fCell=listCells.getFirstCell(lay);
      Int_t lCell=listCells.getLastCell(lay);
      Warning("execute","The region of fired wires in layer =%i > 2",
          lCell-fCell+1);
      Int_t t1=listCells.getTime(lay,fCell);
      Int_t t2=listCells.getTime(lay,lCell);
      Int_t hit1=hitIndex[lay][fCell][t1-1];
      Int_t hit2=hitIndex[lay][lCell][t2-1];
      if(trackFitter.getWeight(hit1)<trackFitter.getWeight(hit2)) {
        listCells.delTime(lay,fCell,t1);  // new fit ???
        sumDelWeight+=trackFitter.getWeight(hit1);
        hitStatus[hit1]=-1;
      } else {
        listCells.delTime(lay,lCell,t2);  // new fit ???
        sumDelWeight+=trackFitter.getWeight(hit2);
        hitStatus[hit2]=-1;
      }
    }
  }
  return sumDelWeight;
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

void HMdc12Fit::fillMdcHitSeg() {
  // filling HMdcSeg and HMdcHit
  locSeg[0]=locClus[0];
  locSeg[1]=locClus[1];
  Short_t modCl=fClst->getMod();
  HMdcSeg *fSeg = 0;
  if(modForSeg[segment]>=0 && (modCl<0 || nModsSeg[sector][segment]==1 || 
      modCl==modForSeg[segment])) {
    fSeg = (HMdcSeg*)fSegCat->getNewSlot(locSeg);
    if(!fSeg) return;
    fSeg = new(fSeg) HMdcSeg;
  }
  if(fSeg) fillSeg(fSeg);
  
  locHit[0]=sector;
  for(Int_t imod=0; imod<2; imod++) {
    Int_t mod=imod+locClus[1]*2;
    if(modCl>=0 && modCl!=mod) continue;
    if(!fMdcDetector->getModule(sector,mod)) continue;

    locHit[1]=mod;
    locHit[2]=indexHit[mod]++; 
    HMdcHit* fHit = (HMdcHit*)fHitCat->getSlot(locHit);                          
    if(!fHit) {                                                                   
      Error("execute","S.%i M.%i No slot HMdcHit available",sector+1,mod+1);                    
      continue;                                                                   
    }                                                                             
    fHit = new(fHit) HMdcHit;                                                     
    fHit->setSecMod(sector,mod);                                                     
    if(!fillHit(mod,fHit)) return; //????

    Int_t t[4];
    for(Int_t layer=0;layer<6;layer++) {
      Int_t lay=layer+imod*6;
      Int_t cell=listCells.getFirstCell(lay);
      if(listCells.getLastCell(lay)-cell >= 4) {
        Warning("execute","The region of fired wires in layer > 4.");
      }
      for(Int_t i=0; i<4; i++) t[i]=listCells.getTime(lay,cell+i);
      fHit->setSignId(layer,cell,t[0],t[1],t[2],t[3]);
      if(fSeg) fSeg->setSignId(lay,cell,t[0],t[1],t[2],t[3]);
    }
    if(isGeant) ((HMdcHitSim*)fHit)->calcNTracks();
    if(fSeg) fSeg->setHitInd(imod,fHitCat->getIndex(fHit));
  }
  if(isGeant && fSeg) ((HMdcSegSim*)fSeg)->calcNTracks();
}

void HMdc12Fit::fillSeg(HMdcSeg* fSeg) {
  fSeg->setIOSeg(fClst->getIOSeg());
  fSeg->setSec(locSeg[0]);
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
  fSeg->setChi2(trackFitter.getFunMin());  
}

Bool_t HMdc12Fit::fillHit(Int_t mod, HMdcHit* fHit) {
  const HGeomTransform* trMdc=(*fSizesCells)[sector][mod].getSecTrans();
  if( !trMdc ) return kFALSE;
  HGeomVector cl(trackFitter.getXOutPlane(),trackFitter.getYOutPlane(),
      trackFitter.getZOutPlane());
  HGeomVector tg(trackFitter.getXOutVertex(),trackFitter.getYOutVertex(),
      trackFitter.getZOutVertex());
     
  cl    = trMdc->transTo(cl);
  tg    = trMdc->transTo(tg);
  
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

void HMdc12Fit::setNTuples(void) {
  hfile = new TFile("fitTuples.root","RECREATE","MDC hit and track NTuples");

  hitTuple = new TNtuple("hit", "hits", "funMin:nHits:dev:weight:time:iter:nW:mod:xV:yV:zV:xP:yP:zP:pV");

  trackTuple = new TNtuple("track", "tracks", "funMin:iter:nHits:nPlanes:xV:yV:zV:xP:yP:zP:pV");

}

Bool_t HMdc12Fit::finalize(void) {
  if(fntuple) {
    hfile->Write();
    hfile->Close();
  }
  return kTRUE;
}

void HMdc12Fit::fillNTuples() {
  int NWeight = 0;
  for(int hit = 0; hit < nClTimes; hit++) {
    if(hitStatus[hit] != 1) continue;
    if(trackFitter.getWeight(hit)>.9) NWeight++;  // 0.9 ???
  }
  for(int hit = 0; hit < nClTimes; hit++) {
    if(hitStatus[hit] != 1) continue;
    hitTuple->Fill( trackFitter.getFunMin(),
		//			  trackFitter.getIterationNumber(),
		    nClTimes,
		    trackFitter.getDeviation(hit),
		    trackFitter.getWeight(hit),
		    trackFitter.getTDCTime(hit),
		    trackFitter.getIterationNumber(),
		    NWeight,
		    // 			    layIndx[hit],
		    // 			    cellIndx[hit],
		    modIndx[hit],
		    trackFitter.getXOutVertex(),
		    trackFitter.getYOutVertex(),
		    trackFitter.getZOutVertex(),
		    trackFitter.getXOutPlane(),
		    trackFitter.getYOutPlane(),
		    trackFitter.getZOutPlane(),
		    trackFitter.getPVelocity());
  }

  trackTuple->Fill( trackFitter.getFunMin(),
		    trackFitter.getIterationNumber(),
		    nFitedTimes,
                    listCells.getNLayers(),
		    trackFitter.getXOutVertex(),
		    trackFitter.getYOutVertex(),
		    trackFitter.getZOutVertex(),
		    trackFitter.getXOutPlane(),
		    trackFitter.getYOutPlane(),
		    trackFitter.getZOutPlane(),
		    trackFitter.getPVelocity());
}

ClassImp(HMdc12Fit)                  

