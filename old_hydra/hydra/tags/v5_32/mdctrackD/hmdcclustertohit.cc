//*-- Author : V.Pechenov
//*--Modified : 09.03.2001
//*-- Modified : 10.08.2000

///////////////////////////////////////////////////////////////////////////////
//
//  HMdcClusterToHit("","",Int_t flag)
//
//  fills the data objects of HMdcHit and HMdcSeg 
//  after track candidate search task (HMdcTrackFinder).
//
// flag=
//   -1 - don't fill HMdcSeg
//   0 (or 1) - at the typeClustFinder=1 (see hmdctrackfinder.cc) 
//     HMdcSeg will filled by hits from MDC1 (or MDC2)
// If in segment 1 is only one MDC (1 or 2) and
// flag=0 or 1  HMdcSeg will filled by Hit in this MDC.
//
//  HMdcHit and HMdcSeg:
//  Chi2 - size of cluster (in proj. plot bins)
//  user flag - number of wires in cluster
//
//  Using:
//  Insert this task in task list after HMdcTrackFinder.
//
///////////////////////////////////////////////////////////////////////////////

#include "hmdcclustertohit.h"
#include "hmdcdef.h"
#include "hades.h"
#include "hmdchit.h"
#include "hmdcseg.h"
#include "hruntimedb.h"
#include "hmdcdetector.h"
#include "hspectrometer.h"
#include "hmdcgetcontainers.h"
#include "hiterator.h"
#include "hcategory.h"
#include "hmdcclus.h"

HMdcClusterToHit::HMdcClusterToHit(Int_t flag) {
  tFillSeg=flag;
  if(tFillSeg<-1 || tFillSeg>1) tFillSeg=1;
}

HMdcClusterToHit::HMdcClusterToHit(Text_t *name, Text_t *title, Int_t flag) : 
                  HReconstructor(name,title) {
  tFillSeg=flag;
  if(tFillSeg<-1 || tFillSeg>1) tFillSeg=1;
}


Bool_t HMdcClusterToHit::init(void) {
  fGetCont=HMdcGetContainers::getObject();
  if( !fGetCont ) return kFALSE;
  fmdc=fGetCont->getMdcDetector();
  fClustCat = gHades->getCurrentEvent()->getCategory(catMdcClus);
  if( !fmdc || !fClustCat) return kFALSE;
  fHitCat = gHades->getCurrentEvent()->getCategory(catMdcHit);
  if (!fHitCat) {
     fHitCat = gHades->getSetup()->getDetector("Mdc")
                                 ->buildCategory(catMdcHit);
     if (!fHitCat) return kFALSE;
     else gHades->getCurrentEvent()->addCategory(catMdcHit,fHitCat,"Mdc");
  }
  fSegCat = gHades->getCurrentEvent()->getCategory(catMdcSeg);
  if (!fSegCat) {
     fSegCat = gHades->getSetup()->getDetector("Mdc")
                                 ->buildCategory(catMdcSeg);
     if (!fSegCat) return kFALSE;
     else gHades->getCurrentEvent()->addCategory(catMdcSeg,fSegCat,"Mdc");
  }
  iter=(HIterator *)fClustCat->MakeIterator("native");

  locClust.set(3,0,0,0);
  locHit.set(3,0,0,0);
  locSeg.set(3,0,0,0);
  return kTRUE;
}

Bool_t HMdcClusterToHit::reinit(void) {
  return kTRUE;
}

Int_t HMdcClusterToHit::execute(void) {
  iter->Reset();
  HMdcClus *fClst=0;
  Int_t indexHit[6][4];
  Int_t indexSeg[6][2];
  for(Int_t s=0; s<6; s++) {
    indexSeg[s][0]=indexSeg[s][1]=0;
    for(Int_t m=0; m<4; m++) indexHit[s][m]=0;
  }
  while ((fClst=(HMdcClus *)iter->Next())!=0) {
    if( !fClst->getStatus() ) continue;
    Short_t sec=fClst->getSec();
    Short_t modCl=fClst->getMod();
    Short_t seg=fClst->getIOSeg();
    Short_t nMods=0;
    if(fmdc->getModule(sec,seg*2)) nMods++;
    if(fmdc->getModule(sec,seg*2+1)) nMods++;
    locHit[0]=sec;
    Int_t typeClFn=fClst->getTypeClFinder();
    HMdcSeg *fSeg = 0;
    //         -               -          -     poka tol'ko dlya Seg.1
    if(tFillSeg!=-1 && (typeClFn==0 || nMods==1 || modCl==tFillSeg)) {
      locSeg[0]=sec;
      locSeg[1]=seg;
      locSeg[2]=indexSeg[sec][seg]++;
      fSeg = (HMdcSeg*)fSegCat->getSlot(locSeg);
      if(!fSeg) {
        Error("execute","S.%i Cluster %i No slot HMdcSeg available",
            sec+1,indexSeg[sec][seg]+1);
        continue;
      }
      fSeg = new(fSeg) HMdcSeg;
      fSeg->setSec(sec);
      fSeg->setIOSeg(seg);
      fSeg->setChi2(fClst->getNBins());  // NBins in clust. --> chi2 in seg.
      fSeg->setFlag(fClst->getNCells()); // NWires in clust. --> user flag in seg.
      fillSeg(fClst,fSeg);
    }
//    Int_t flagSeg=0;           
    for(Int_t imod=0; imod<2; imod++) {
      Int_t mod=imod+seg*2;
      if(modCl>=0 && modCl!=mod) continue;
      if(!fmdc->getModule(sec,mod)) continue;

      locHit[1]=mod;
      locHit[2]=indexHit[sec][mod]++;
      HMdcHit *fHit = (HMdcHit*)fHitCat->getSlot(locHit);
      if(!fHit) {
        Error("execute","S.%i No slot HMdcHit available",sec+1);
        continue;
      }
      fHit = new(fHit) HMdcHit;
      fHit->setSecMod(sec,mod); 
      fHit->setChi2(fClst->getNBins());  // NBins in clust. --> chi2 in hit.
      if(!fillHit(fClst,mod,fHit)) return 1;
      
      Int_t flag=0;
      for(Int_t layer=0; layer<6; layer++) {
        Int_t iLayer=layer+imod*6;
        Int_t nHits=fClst->getNCells(iLayer);
        if( nHits <= 0 ) continue;
        Int_t firstCell=fClst->getFirstCell(iLayer);
        Int_t lastCell=fClst->getLastCell(iLayer);
        Int_t nMax=lastCell-firstCell;
        if(lastCell-firstCell >3 ) {
          nMax=3;
//          flag=flag & (1<<layer);
        }
        flag+=fClst->getNCells(iLayer);
        Int_t t[4]={0,0,0,0};
        for(Int_t n=0; n<=nMax; n++) t[n]=fClst->getTime(iLayer,n+firstCell);
        
        fHit->setSignId(layer,firstCell,t[0],t[1],t[2],t[3]);
        if(fSeg) fSeg->setSignId(iLayer,firstCell,t[0],t[1],t[2],t[3]);
//        flagSeg=flagSeg & flag<<mod*6;
      }
      fHit->setFlag(flag);
//      flagSeg=flagSeg & flag<<imod*6; 
      if(fSeg) fSeg->setHitInd(imod,fHitCat->getIndex(fHit));
    }
//    if(fSeg) fSeg->setFlag(flagSeg);
  }
  return 0;
}

void HMdcClusterToHit::calcSeg(Double_t xTag, Double_t yTag, Double_t zTag, 
                               Double_t xCl,  Double_t yCl,  Double_t zCl,
         Double_t &theta, Double_t &phi, Double_t &zm, Double_t &r0) {
  Double_t xCxT=xCl-xTag;
  Double_t yCyT=yCl-yTag;
  Double_t zCzT=zCl-zTag;
  theta=acos(zCzT/sqrt(xCxT*xCxT+yCyT*yCyT+zCzT*zCzT));
  phi=TMath::ATan2(yCyT,xCxT);
  if( theta == 0.0 ) theta = 1.e-5;  // ??? !!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  zm=zTag - cos(theta)/sin(theta) * (xTag*cos(phi)+yTag*sin(phi));
  r0=yTag*cos(phi)-xTag*sin(phi);
}

void HMdcClusterToHit::fillSeg(HMdcClus* fClst,HMdcSeg* fSeg) {
  Double_t xC,yC,zC,eXC,eYC;  // Cluster.
  xC=fClst->getX();
  yC=fClst->getY();
  zC=fClst->getZ();
  eXC=fClst->getErrX();
  eYC=fClst->getErrY();
  Double_t xT,yT,zT,eXT,eYT,eZT;  // Target (source)
  xT=fClst->getXTarg();
  yT=fClst->getYTarg();
  zT=fClst->getZTarg();
  eXT=fClst->getErrXTarg();
  eYT=fClst->getErrYTarg();
  eZT=fClst->getErrZTarg();
  Double_t theta, phi, zm, r0;
  calcSeg(xT,yT,zT,xC,yC,zC,theta,phi,zm,r0);
  Double_t eth[8], ephi[8], ezm[8], er0[8];
  calcSeg(xT,yT,zT,xC+eXC,yC,fClst->getZOnPlane(xC+eXC,yC),
      eth[0],ephi[0],ezm[0],er0[0]);
  calcSeg(xT,yT,zT,xC-eXC,yC,fClst->getZOnPlane(xC-eXC,yC),
      eth[1],ephi[1],ezm[1],er0[1]);
  calcSeg(xT,yT,zT,xC,yC+eYC,fClst->getZOnPlane(xC,yC+eYC),
      eth[2],ephi[2],ezm[2],er0[2]);
  calcSeg(xT,yT,zT,xC,yC-eYC,fClst->getZOnPlane(xC,yC-eYC),
      eth[3],ephi[3],ezm[3],er0[3]);
  Int_t nPoints;
  if(fClst->getIOSeg() == 0) {
    // Seg.0:  uchitiyvaem tol'ko Z ...
    calcSeg(xT,yT,zT+eZT,xC,yC,zC,eth[4],ephi[4],ezm[4],er0[4]);
    calcSeg(xT,yT,zT-eZT,xC,yC,zC,eth[5],ephi[5],ezm[5],er0[5]);
    nPoints=6;
  } else {
    // Seg.1:  schitaem, chto kick plane ploskost'
    calcSeg(xT+eXT,yT,zT,xC,yC,zC,eth[4],ephi[4],ezm[4],er0[4]);
    calcSeg(xT-eXT,yT,zT,xC,yC,zC,eth[5],ephi[5],ezm[5],er0[5]);
    calcSeg(xT,yT+eYT,zT-eZT,xC,yC,zC,eth[6],ephi[6],ezm[6],er0[6]);
    calcSeg(xT,yT-eYT,zT+eZT,xC,yC,zC,eth[7],ephi[7],ezm[7],er0[7]);
    nPoints=8;
  }
  Double_t errTheta,errPhi,errZm,errR0;
  errTheta=errPhi=errZm=errR0=0.0;
  for(Int_t i=0; i<nPoints; i+=2) {
    eth[i]=(TMath::Abs(eth[i]-theta)+TMath::Abs(eth[i+1]-theta))*0.5;
    ephi[i]=(TMath::Abs(ephi[i]-phi)+TMath::Abs(ephi[i+1]-phi))*0.5;
    ezm[i]=(TMath::Abs(ezm[i]-zm)+TMath::Abs(ezm[i+1]-zm))*0.5;
    er0[i]=(TMath::Abs(er0[i]-r0)+TMath::Abs(er0[i+1]-r0))*0.5;
    errTheta += eth[i]*eth[i];
    errPhi += ephi[i]*ephi[i];
    errZm += ezm[i]*ezm[i];
    errR0 += er0[i]*er0[i];    
  }
  fSeg->setPar(zm, sqrt(errZm), r0, sqrt(errR0), theta, sqrt(errTheta), 
      phi, sqrt(errPhi));
  //Printf("theta=%g+/-%g, phi=%g+/-%g, zm=%g+/-%g, r0=%g+/-%g",theta,errTheta, phi,errPhi, zm,errZm, r0,errR0);
}

Bool_t HMdcClusterToHit::fillHit(HMdcClus* fClst,Short_t mod,HMdcHit* fHit) {
  HGeomVector cl,clSh1,clSh2,clErr;
  HGeomVector tg,tgSh1,tgSh2,tgErr;
  HGeomTransform trMdc;
  if( !fGetCont->getSecTransMod(trMdc,fClst->getSec(),mod) ) return kFALSE;
  fClst->getPoint(cl);
  // clErrZ & tgErrZ so znakom!
  clErr.setXYZ(fClst->getErrX(),fClst->getErrY(),
      cl(2)-fClst->getZOnPlane(cl(0)+fClst->getErrX(),cl(1)+fClst->getErrY()));
  clSh1=cl+clErr;
  clSh2=cl-clErr;
  fClst->getTarg(tg);
  // Seg.0:  uchitiyvaem tol'ko Z ...
  if(fClst->getIOSeg() == 0) tgErr.setXYZ(0.,0.,fClst->getErrZTarg());
  // Seg.1:  schitaem, chto kick plane ploskost'
  else tgErr.setXYZ(fClst->getErrXTarg(),fClst->getErrYTarg(),
        -fClst->getErrZTarg());
  tgSh1=tg+tgErr;
  tgSh2=tg-tgErr;
     
  cl    = trMdc.transTo(cl);
  clSh1 = trMdc.transTo(clSh1);
  clSh2 = trMdc.transTo(clSh2);
  tg    = trMdc.transTo(tg);
  tgSh1   = trMdc.transTo(tgSh1);
  tgSh2   = trMdc.transTo(tgSh2);
  
  Float_t xCl,yCl,lCl,mCl;
  calcHit(cl, tg,xCl,yCl,lCl,mCl);
  Float_t xClSh[4],yClSh[4],lClSh[4],mClSh[4];
  calcHit(clSh1, tg,xClSh[0],yClSh[0],lClSh[0],mClSh[0]);
  calcHit(clSh2, tg,xClSh[1],yClSh[1],lClSh[1],mClSh[1]);
  calcHit(cl, tgSh1,xClSh[2],yClSh[2],lClSh[2],mClSh[2]);
  calcHit(cl, tgSh2,xClSh[3],yClSh[3],lClSh[3],mClSh[3]);
  // nado proverit' oshibki !!!???
  Float_t dXdXclErrX=(TMath::Abs(xClSh[0]-xCl)+TMath::Abs(xClSh[1]-xCl))*0.5;
  Float_t dXdZtgErrT=(TMath::Abs(xClSh[2]-xCl)+TMath::Abs(xClSh[3]-xCl))*0.5;
  Float_t errX=sqrt(dXdXclErrX*dXdXclErrX+dXdZtgErrT*dXdZtgErrT);
  Float_t dYdYclErrY=(TMath::Abs(yClSh[0]-yCl)+TMath::Abs(yClSh[1]-yCl))*0.5;
  Float_t dYdZtgErrT=(TMath::Abs(yClSh[2]-yCl)+TMath::Abs(yClSh[3]-yCl))*0.5;
  Float_t errY=sqrt(dYdYclErrY*dYdYclErrY+dYdZtgErrT*dYdZtgErrT);
  
   // nado proverit'  osobenno vnimatel'no oshibki !!!???
  Float_t dLdXclErrX=(TMath::Abs(lClSh[0]-lCl)+TMath::Abs(lClSh[1]-lCl))*0.5;
  Float_t dLdZtgErrT=(TMath::Abs(lClSh[2]-lCl)+TMath::Abs(lClSh[3]-lCl))*0.5;
  Float_t errL=sqrt(dLdXclErrX*dLdXclErrX+dLdZtgErrT*dLdZtgErrT);
  Float_t dMdYclErrY=(TMath::Abs(mClSh[0]-mCl)+TMath::Abs(mClSh[1]-mCl))*0.5;
  Float_t dMdZtgErrT=(TMath::Abs(mClSh[2]-mCl)+TMath::Abs(mClSh[3]-mCl))*0.5;
  Float_t errM=sqrt(dMdYclErrY*dMdYclErrY+dMdZtgErrT*dMdZtgErrT);
       
  fHit->setXY(xCl,errX,yCl,errY);
  fHit->setXYDir(lCl,errL,mCl,errM);
  return kTRUE;
}

void HMdcClusterToHit::calcHit(HGeomVector& p1, HGeomVector& p2,
    Float_t &x, Float_t &y, Float_t &l, Float_t &m) {
  x=(p1(0)*p2(2)-p1(2)*p2(0))/(p2(2)-p1(2));
  y=(p1(1)*p2(2)-p1(2)*p2(1))/(p2(2)-p1(2)); 
  Float_t dx=(p1(0)-p2(0));
  Float_t dy=(p1(1)-p2(1));
  Float_t dz=(p1(2)-p2(2));
  Float_t length=sqrt( dx*dx + dy*dy + dz*dz );
  l=dx/length; 
  m=dy/length;
}

ClassImp(HMdcClusterToHit)
