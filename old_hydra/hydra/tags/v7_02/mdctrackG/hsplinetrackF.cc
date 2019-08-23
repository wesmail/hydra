//*-- Author : Anar Rustamov (27.08.2003)
#include <iostream>
#include "hades.h"
//#include "hmetamatchF.h"
#include "hmetamatch.h"
#include "hcategory.h"
#include "hdetector.h"
#include "hevent.h"
#include "htofhitsim.h"
#include "hsplinetrackF.h"
#include "hiterator.h"
#include "hgeomvolume.h"
#include "hgeomcompositevolume.h"
#include "hspectrometer.h"
#include "hruntimedb.h"
#include "hmdcgeompar.h"
#include "hspecgeompar.h" 
#include "tofdef.h"
#include "hmdcseg.h"
#include "showerdef.h"
#include "hmdctrackgcorrpar.h"
#include "hmdctrackgfieldpar.h"
#include "hmatrixcategory.h"
#include "hbasetrack.h"
#include "showertofinodef.h"
#include "hgeantmdc.h"
#include "hkickplane2.h"
#include "hgeantkine.h"
#include "hgeomtransform.h"
#include "hgeomvolume.h"
#include "hmdctrackddef.h"
#include "hmdctrackgdef.h"
#include "hruntimedb.h"
#include "hevent.h"
#include "hmdctrkcand.h"
#include "hmdctrackgspline.h"
#include "hmdctrackgcorrpar.h"
#include "hgeomvector.h"
#include "TNtuple.h"
#include "hlocation.h"
#include "hmdctrackgsplinecorr.h"
#include "hsplinetrack.h"
#include "hshowerhittoftrack.h"
#include "TFile.h"
  using namespace std;
ClassImp(HSplineTrackF)
   HSplineTrackF::HSplineTrackF()
{
   C=299792458;
   fCatSplineTrack=0;
   fCatMdcSegSim=0;
   fMdcGeometry=0;
   fSpecGeomPar=0;
   Spline=0;
   fMetaMatchIter=0;
   field=0;
   corr=0;
}
HSplineTrackF::HSplineTrackF(Text_t *name, Text_t *title):
   HReconstructor(name,title)
{
   C=299792458;
   fCatSplineTrack=0;
   fCatMdcSegSim=0;
   fMdcGeometry=0;
   fSpecGeomPar=0;
   Spline=0;
   fMetaMatchIter=0;
   field=0;
   corr=0;
}
HSplineTrackF::~HSplineTrackF()
{
   ;
}
Bool_t HSplineTrackF::init()
{
   
   
   if (gHades)
      {
	 for(Int_t i=0; i<6;i++)
	    
	       tRans[i]=0;
	   
	 HRuntimeDb *rtdb=gHades->getRuntimeDb();
	 HSpectrometer *spec=gHades->getSetup();
	 HEvent *event=gHades->getCurrentEvent();
	 if(rtdb && spec && event)
	    {
	       field=(HMdcTrackGFieldPar*)(rtdb->getContainer("MdcTrackGFieldPar"));
	       corr=(HMdcTrackGCorrPar*)(rtdb->getContainer("MdcTrackGCorrPar"));
	       fSpecGeomPar=(HSpecGeomPar*)(rtdb->getContainer("SpecGeomPar"));
	       fGetCont=HMdcGetContainers::getObject();
	       fGetCont->getMdcGeomPar();
	       fGetCont->getSpecGeomPar();
	       kickplane=(HKickPlane2*)rtdb->getContainer("KickPlane2MDC3");
	    } 
	 fCatMdcTrkCand=event->getCategory(catMdcTrkCand);
	 if(!fCatMdcTrkCand) return kFALSE;
	 fCatMetaMatch=event->getCategory(catMetaMatch);
	 if (!fCatMetaMatch) return kFALSE;
	 fMetaMatchIter=(HIterator*)fCatMetaMatch->MakeIterator();
	 if(!fMetaMatchIter) return kFALSE;
	 fCatMdcSegSim=event->getCategory(catMdcSeg);
	 fCatTof=event->getCategory(catTofHit);
	 if(!fCatTof) return kFALSE;
	 fCatKine=event->getCategory(catGeantKine);
	 if(!fCatKine)
	    {
	       fCatShower=event->getCategory(catShowerHitTof);
	       if(!fCatShower) return kFALSE;
	    }
	 else
	    {
	       fCatShower=event->getCategory(catShowerHitTofTrack);
	       if(!fCatShower) return kFALSE;
	    }
	 fCatSplineTrack=event->getCategory(catSplineTrack);
	 if(!fCatSplineTrack)
	    {
	       Int_t size[2]={6,600};
	       fCatSplineTrack=new HMatrixCategory("HSplineTrack",2,size,0.5);  
	       if(fCatSplineTrack) 
		  event->addCategory(catSplineTrack,fCatSplineTrack,"SplineTrack");
	    }
      }
   return kTRUE;
}
Bool_t HSplineTrackF::reinit()
{
   for(Int_t i=0; i<6; i++)
      tRans[i]=0;
   Spline=corr->getSPline(); 
   Spline->setDataPointer(field->getPointer(),corr->getCorr());
   Spline->setKickPointer(kickplane);
   return kTRUE;
}
Bool_t HSplineTrackF::finalize()
{
   return kTRUE;
}
Int_t HSplineTrackF::execute()
{
   return mdc1234();
}
Int_t HSplineTrackF::mdc1234()
{
   fMetaMatchIter->Reset();
   while((pMetaMatch=(HMetaMatch*)(fMetaMatchIter->Next()))!=0)
      {
	 sector=pMetaMatch->getSector();
	 indTrkCand=pMetaMatch->getTrkCandInd();
	 metaInd=pMetaMatch->getMetaHitInd();
	 pMdcTrkCand=(HMdcTrkCand*)fCatMdcTrkCand->getObject(indTrkCand);
	 if(!pMdcTrkCand) continue;
	 index1=pMdcTrkCand->getSeg1Ind();
	 index2=pMdcTrkCand->getSeg2Ind(); 
	 metaInd=pMetaMatch->getMetaHitInd();
	 sectorloc.set(1,(Int_t)sector);
	 segments[0]=(HMdcSeg*)fCatMdcSegSim->getObject(index1);
	 segments[1]=(HMdcSeg*)fCatMdcSegSim->getObject(index2);
	 system=pMetaMatch->getSystem();
	 if(!segments[0] || !segments[1]) continue; // only if outer mdcs 
	 if(metaInd<0)
	 { // if no meta hit
	     if(segments[1]->getHitInd(0)==-1 || segments[1]->getHitInd(1)==-1)
	     { // if one out of outer is missing
		 Momentum=corr->calcMomentum123(segments,kTRUE,segments[0]->getZ());
	     } else {
		 Momentum=corr->calcMomentum(segments,kTRUE,segments[0]->getZ());
	     }
	    
		 polarity=Spline->getPolarity();
		 fillData(segments[0],kFALSE);
                  
	     
	     continue; // if no meta stop here
	 }
	 if (system==1)
	 {   // for tof
	     pTofHit=(HTofHitSim*)fCatTof->getObject(metaInd);
	     tof=pTofHit->getTof();
	     pTofHit->getXYZLab(xTof,yTof,zTof);
	 }
	 if(system==0)
	 {   // for shower
	     pShowerHitTofTrack=(HShowerHitTofTrack*)fCatShower->getObject(metaInd);
	     tof=pShowerHitTofTrack->getTof();
	     pShowerHitTofTrack->getLabXYZ(&xTof,&yTof,&zTof);
	 }
	 pointMeta.setXYZ(xTof,yTof,zTof);
	 if(tRans[sector]==0)
	 {
	     tRans[sector]=new HGeomTransform();
	     if(!fGetCont->getLabTransSec(*(tRans[sector]),sector))
	     {
		 continue;
	     }
	 }
	 pointMeta=tRans[sector]->transTo(pointMeta);
	 xTof=pointMeta.getX();
	 yTof=pointMeta.getY();
	 zTof=pointMeta.getZ();

	 if(segments[1]->getHitInd(0)==-1 || segments[1]->getHitInd(1)==-1)
	 {   // if only one mdc in outer seg
	     Momentum=corr->calcMomentum123(segments,kTRUE,segments[0]->getZ());
	 }
	 else
	 {   // 2 mdc in outer seg
	     Momentum=corr->calcMomentum(segments,kTRUE,segments[0]->getZ());
	 }
	 distanceTof=Spline->getMetaDistance(xTof*0.1,yTof*0.1,zTof*0.1);
	 const HGeomVector &targetPos = fSpecGeomPar->getTarget(0)->getTransform().getTransVector();
	 HGeomVector POINT1=Spline->getPointOne();
	 Double_t XXX=POINT1.getX();
	 Double_t YYY=POINT1.getY();
	 Double_t ZZZ=POINT1.getZ();
	 POINT1.setXYZ(10*XXX,10*YYY,10*ZZZ);
	 Double_t dist = (POINT1 - targetPos).length();
	 TOFdistance=dist+distanceTof*10; //in mm
	 beta=TOFdistance/tof/C;
	 beta*=1e6;
	 mass2=Momentum*Momentum*(1-beta*beta)/(beta*beta);
	 polarity=Spline->getPolarity();

	
	     fillData(segments[0],kTRUE);
	
	 
      }
   return 0;
}

HSplineTrack *  HSplineTrackF::fillData(HMdcSeg *segment,Bool_t condition)
{
   Int_t indexspline;
   HSplineTrack *sp=(HSplineTrack*)fCatSplineTrack->
      getNewSlot(sectorloc,&indexspline);
   if(!sp)
      {
         Error("fillData","No slot available");
	 return 0;
      }
   sp=(HSplineTrack*)(new(sp)HSplineTrack);
   if(sp)
      {
	 sp->setP(Momentum,0);
	 sp->setZ(segment->getZ(),segment->getErrZ());
	 sp->setR(segment->getR(),segment->getErrR());
	 sp->setTheta(segment->getTheta(),segment->getErrTheta());
	 sp->setPhi(segment->getPhi(),segment->getErrPhi());
	 sp->setPolarity(polarity);
	 sp->setSector(sector);
	 pMetaMatch->setSplineInd(indexspline);
         sp->setTofHitInd(pMetaMatch->getTofHitInd());
         sp->setShowerHitInd(pMetaMatch->getShowerHitInd());
	 if(condition)
	    {
	      // sp->setP(Momentum,0);
	       sp->setTofDist(TOFdistance);
	       sp->setBeta(beta);
//	       sp->setBetaCalc(betaCalc);
	       sp->setMass2(mass2,0);
	       sp->setTof(tof);
	    }
      }
   else
      Error("FillData","No slots free");
   return sp;
}
