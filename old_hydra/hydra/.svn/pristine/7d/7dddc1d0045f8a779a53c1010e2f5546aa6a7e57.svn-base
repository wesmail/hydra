//*-- Author : V.Pechenov
//*-- Modified : 10.08.2000

///////////////////////////////////////////////////////////////////////////////
//
//  HMdcClusterToHit
//
//  fills the data objects of HMdcHit and HMdcSeg 
//  after track candidate search task (HMdcTrackFinder).
//
//  Using:
//  Insert this task in task list after HMdcTrackFinder.
//
//  Remarks:
//  If user flag in HMdcHit and HMdcSeg non equal zero it means
//  that full information about cells numbers can not be geting from HMdcHit
//  and HMdcSeg data objects.
//
///////////////////////////////////////////////////////////////////////////////

#include "hmdcclustertohit.h"
#include "hmdcdef.h"
#include "hades.h"
#include "hmdchit.h"
#include "hmdcseg.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "hevent.h"
#include "hgeomcompositevolume.h" 


HMdcClusterToHit::HMdcClusterToHit() {
  fLookUpTb=0;
}

HMdcClusterToHit::HMdcClusterToHit(Text_t *name,Text_t *title) : 
                  HReconstructor(name,title) {
  fLookUpTb=0;   
}


Bool_t HMdcClusterToHit::init(void) {
  fmdc=(HMdcDetector*)gHades->getSetup()->getDetector("Mdc");
  if(!fmdc) return kFALSE;
  fLookUpTb=(HMdcLookUpTb*)gHades->getRuntimeDb()->getContainer("MdcLookUpTb");
  if(!fLookUpTb) return kFALSE;
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
  fGeomPar=(HMdcGeomPar*)gHades->getRuntimeDb()->getContainer("MdcGeomPar");
  for(Int_t ns=0; ns<6; ns++) {
    HMdcLookUpTbSec* plSec=&(*fLookUpTb)[ns];
    if( !plSec ) continue;
    targetM[ns]=plSec->getTargetF()+plSec->getTargetL();
    targetM[ns]/=2.;
    HGeomVector tgF=plSec->getTargetF();
    HGeomVector tgL=plSec->getTargetL();
    dZt[ns]=TMath::Abs(tgL(2)-tgF(2))*0.5;
    tAx[ns]=(tgF(2)-tgL(2) != 0.) ? (tgF(0)-tgL(0))/(tgF(2)-tgL(2)) : 0.;
    tBx[ns]=tgL(0)-tgL(2)*tAx[ns];
    tAy[ns]=(tgF(2)-tgL(2) != 0.) ? (tgF(1)-tgL(1))/(tgF(2)-tgL(2)) : 0.;
    tBy[ns]=tgL(1)-tgL(2)*tAy[ns];
    const HGeomTransform* prPlane=&plSec->getParPlane();
    Double_t pC=(prPlane->getTransVector()).getZ();
    pA[ns]=(prPlane->getTransVector()).getX()/pC;
    pB[ns]=(prPlane->getTransVector()).getY()/pC;
    pD[ns]=(pA[ns]*pA[ns]+pB[ns]*pB[ns]+1.)*pC;
  } 
  return kTRUE;
}

Int_t HMdcClusterToHit::execute(void) {
  locHit.set(2,0,0);
  locSeg.set(2,0,0);
  HGeomVector tg, tgF, tgL;
  HGeomVector cl,clSh1,clSh2;

  for(Int_t ns=0; ns<6; ns++) {
    HMdcLookUpTbSec* plSec=&(*fLookUpTb)[ns];
    if( !plSec ) continue;
    HMdc12Clusters& clusters = (*fLookUpTb)[ns].getClusters();
    Int_t nclst = clusters.getNClusters();
    for(Int_t nc=0; nc<nclst; nc++) {
      HMdcCluster& clst = clusters[nc];
      locSeg[0]=ns;
      locSeg[1]=0;
      HMdcSeg *seg = (HMdcSeg*)fSegCat->getNewSlot(locSeg);      
      if(!seg) continue;
      seg = new(seg) HMdcSeg;
      seg->setSec(ns);
      seg->setIOSeg(0);
      HGeomVector vTr(clst.getX()-targetM[ns](0),clst.getY()-targetM[ns](1),
                      clst.getZ()-targetM[ns](2));
      
      Double_t theta, phi, zm, r0;
      calcSeg(ns,targetM[ns](2),clst.getX(),clst.getY(),theta,phi,zm,r0);
      Double_t eth[6], ephi[6], ezm[6], er0[6];
      calcSeg(ns,targetM[ns](2)+dZt[ns],clst.getX(),clst.getY(),
              eth[0],ephi[0],ezm[0],er0[0]);
      calcSeg(ns,targetM[ns](2)-dZt[ns],clst.getX(),clst.getY(),
              eth[1],ephi[1],ezm[1],er0[1]);
      calcSeg(ns,targetM[ns](2),clst.getX()+clst.getErrX(),clst.getY(),
              eth[2],ephi[2],ezm[2],er0[2]);
      calcSeg(ns,targetM[ns](2),clst.getX()-clst.getErrX(),clst.getY(),
              eth[3],ephi[3],ezm[3],er0[3]);
      calcSeg(ns,targetM[ns](2),clst.getX(),clst.getY()+clst.getErrY(),
              eth[4],ephi[4],ezm[4],er0[4]);
      calcSeg(ns,targetM[ns](2),clst.getX(),clst.getY()-clst.getErrY(),
              eth[5],ephi[5],ezm[5],er0[5]);
      for(Int_t i=0; i<6; i+=2) {
        eth[i]=(TMath::Abs(eth[i]-theta)+TMath::Abs(eth[i+1]-theta))*0.5;
        ephi[i]=(TMath::Abs(ephi[i]-phi)+TMath::Abs(ephi[i+1]-phi))*0.5;
	ezm[i]=(TMath::Abs(ezm[i]-zm)+TMath::Abs(ezm[i+1]-zm))*0.5;
	er0[i]=(TMath::Abs(er0[i]-r0)+TMath::Abs(er0[i+1]-r0))*0.5;
      }
      Double_t errTheta=sqrt(eth[0]*eth[0]+eth[2]*eth[2]+eth[4]*eth[4]);
      Double_t errPhi=sqrt(ephi[0]*ephi[0]+ephi[2]*ephi[2]+ephi[4]*ephi[4]);
      Double_t errZm=sqrt(ezm[0]*ezm[0]+ezm[2]*ezm[2]+ezm[4]*ezm[4]);
      Double_t errR0=sqrt(er0[0]*er0[0]+er0[2]*er0[2]+er0[4]*er0[4]);
      seg->setPar(zm, errZm, r0, errR0, theta, errTheta, phi, errPhi);
//Printf("theta=%g+/-%g, phi=%g+/-%g, zm=%g+/-%g, r0=%g+/-%g",theta,errTheta, phi,errPhi, zm,errZm, r0,errR0);
            
      for(Int_t mod=0; mod<2; mod++) {
        if(!fmdc->getModule(ns,mod)) continue;
        HGeomCompositeVolume* pComVol=fGeomPar->getRefVolume(mod);
        if(!pComVol) return 1;
        const HGeomTransform* trMdc = & pComVol->getTransform();
        cl.setX(clst.getX());
        cl.setY(clst.getY());
        cl.setZ(clst.getZ());
        clSh1.setX(clst.getX()+clst.getErrX());
        clSh1.setY(clst.getY()+clst.getErrY());
        clSh1.setZ(clst.getZ()-clst.getErrZ());
        clSh2.setX(clst.getX()-clst.getErrX());
        clSh2.setY(clst.getY()-clst.getErrY());
        clSh2.setZ(clst.getZ()+clst.getErrZ());
        tg=targetM[ns];
        tgF=plSec->getTargetF();
        tgL=plSec->getTargetL();
        cl    = trMdc->transTo(cl);
        clSh1 = trMdc->transTo(clSh1);
        clSh2 = trMdc->transTo(clSh2);
        tg    = trMdc->transTo(tg);
        tgF   = trMdc->transTo(tgF);
        tgL   = trMdc->transTo(tgL);
        
        Float_t xCl=(cl(0)*tg(2)-cl(2)*tg(0))/(tg(2)-cl(2));
        Float_t yCl=(cl(1)*tg(2)-cl(2)*tg(1))/(tg(2)-cl(2));         
        Float_t xClSC1=(clSh1(0)*tg(2)-clSh1(2)*tg(0))/(tg(2)-clSh1(2));
        Float_t yClSC1=(clSh1(1)*tg(2)-clSh1(2)*tg(1))/(tg(2)-clSh1(2));
        Float_t xClSC2=(clSh2(0)*tg(2)-clSh2(2)*tg(0))/(tg(2)-clSh2(2));
        Float_t yClSC2=(clSh2(1)*tg(2)-clSh2(2)*tg(1))/(tg(2)-clSh2(2));
        Float_t xClST1=(cl(0)*tgF(2)-cl(2)*tgF(0))/(tgF(2)-cl(2));
        Float_t yClST1=(cl(1)*tgF(2)-cl(2)*tgF(1))/(tgF(2)-cl(2));
        Float_t xClST2=(cl(0)*tgL(2)-cl(2)*tgL(0))/(tgL(2)-cl(2));
        Float_t yClST2=(cl(1)*tgL(2)-cl(2)*tgL(1))/(tgL(2)-cl(2));
        Float_t dXdXclErrX=(TMath::Abs(xClSC1-xCl)+TMath::Abs(xClSC2-xCl))*0.5;
        Float_t dXdZtgErrT=(TMath::Abs(xClST1-xCl)+TMath::Abs(xClST2-xCl))*0.5;
        Float_t errX=sqrt(dXdXclErrX*dXdXclErrX+dXdZtgErrT*dXdZtgErrT);
        Float_t dYdYclErrY=(TMath::Abs(yClSC1-yCl)+TMath::Abs(yClSC2-yCl))*0.5;
        Float_t dYdZtgErrT=(TMath::Abs(yClST1-yCl)+TMath::Abs(yClST2-yCl))*0.5;
        Float_t errY=sqrt(dYdYclErrY*dYdYclErrY+dYdZtgErrT*dYdZtgErrT);
               
        Float_t dx=(xCl-tg(0));
        Float_t dy=(yCl-tg(1));
        Float_t length=sqrt( dx*dx + dy*dy + tg(2)*tg(2) );
        Float_t lCl=dx/length; 
        Float_t mCl=dy/length;

        Float_t ln32=1./TMath::Power(length,3);
        Float_t ax=(tgF(0)-tgL(0))/(tgF(2)-tgL(2));
        Float_t ay=(tgF(1)-tgL(1))/(tgF(2)-tgL(2));
        Float_t dLdXcl=(dy*dy+tg(2)*tg(2))*ln32;
        Float_t dLdYcl=-dx*dy*ln32;
        Float_t dLdZtg=dx*(dx*ax+dy*ay+tg(2))*ln32;
        Float_t dMdXcl=-dy*dx*ln32;
        Float_t dMdYcl=(dx*dx+tg(2)*tg(2))*ln32;
        Float_t dMdZtg=dy*(dx*ax+dy*ay+tg(2))*ln32;
        Float_t errZtg=TMath::Abs((tgF(2)-tgL(2)))*0.5;
        Float_t errL=sqrt(TMath::Power(dLdXcl*errX,2)+
                          TMath::Power(dLdYcl*errY,2)+
                          TMath::Power(dLdZtg*errZtg,2));
        Float_t errM=sqrt(TMath::Power(dMdXcl*errX,2)+
                          TMath::Power(dMdYcl*errY,2)+
                          TMath::Power(dMdZtg*errZtg,2));
        
	locHit[0]=ns;
	locHit[1]=mod;
        HMdcHit *hit = (HMdcHit*)fHitCat->getNewSlot(locHit);
        if(!hit) continue;
        hit = new(hit) HMdcHit;
        hit->setSecMod(ns,mod);        
        hit->setXY(xCl,errX,yCl,errY);
        hit->setXYDir(lCl,errL,mCl,errM);
	Int_t flagSeg=0;
	for(Int_t layer=0; layer<6; layer++) {
	  Int_t nHits=clst.getNumHits(mod*6+layer);
	  if( nHits <= 0 ) continue;
	  Int_t t[4]={0,0,0,0};
	  Int_t flag=0;
	  Int_t fcell=clst.getHit(mod*6+layer,0);
	  for(Int_t nHit=0; nHit<nHits; nHit++) {
	    Int_t cell=clst.getHit(mod*6+layer,nHit)-fcell;
	    Int_t nTimes=clst.getTimes(mod*6+layer,nHit);
	    if( nTimes == 2 ) nTimes=3;
	    if( cell < 4 ) t[cell]=nTimes;
	    else flag= flag & 1<<layer;
	  }
	  hit->setSignId(layer,fcell,t[0],t[1],t[2],t[3]);
	  hit->setFlag(flag);
	  seg->setSignId(layer,fcell,t[0],t[1],t[2],t[3]);
	  flagSeg=flagSeg & flag<<mod*6;
	}
	seg->setFlag(flagSeg);
	seg->setHitInd(mod,fHitCat->getIndex(hit));
      }
    }
  }
  return 0;
}

void HMdcClusterToHit::calcSeg(Int_t ns, 
         Double_t zTag, Double_t xCl, Double_t yCl,
         Double_t &theta, Double_t &phi, Double_t &zm, Double_t &r0) {
  Double_t zCzT=pD[ns]-pA[ns]*xCl-pB[ns]*yCl-zTag;
  Double_t xTag=tAx[ns]*zTag+tBx[ns];
  Double_t yTag=tAy[ns]*zTag+tBy[ns];
  Double_t xCxT=xCl-xTag;
  Double_t yCyT=yCl-yTag;
  theta=acos(zCzT/sqrt(xCxT*xCxT+yCyT*yCyT+zCzT*zCzT));
  phi=TMath::ATan2(yCyT,xCxT);
  if( theta == 0.0 ) theta = 1.e-5;  // ??? !!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  zm=zTag - cos(theta)/sin(theta) * (xTag*cos(phi)+yTag*sin(phi));
  r0=yTag*cos(phi)-xTag*sin(phi);
}
 
ClassImp(HMdcClusterToHit)
