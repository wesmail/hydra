//*-- Author : Anar Rustamov (27.08.2003)
#include <iostream>
#include "hades.h"
//#include "hmetamatchF.h"
#include "hmetamatch.h"
#include "hcategory.h"
#include "hdetector.h"
#include "hevent.h"
#include "htofhit.h"
#include "htofcluster.h"
#include "hsplinetrackF.h"
#include "hiterator.h"
#include "hgeomvolume.h"
#include "hgeomcompositevolume.h"
#include "hspectrometer.h"
#include "htofinogeompar.h"
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
#include "hmagnetpar.h"
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
  fCatMdcTrkCand=0;
  fCatTof=0;
  fCatTofCluster=0;
  field=0;
  corr=0;
  qSpline=-1;
  fScal=0;
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
  fCatMdcTrkCand=0;
  fCatTof=0;
  fCatTofCluster=0;
  field=0;
  corr=0;
  qSpline=-1;
  fScal=0;
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
	  pMagnet=(HMagnetPar*)(rtdb->getContainer("MagnetPar"));
	  fGetCont=HMdcGetContainers::getObject();
	  fGetCont->getMdcGeomPar();
	  fGetCont->getSpecGeomPar();
	  kickplane=(HKickPlane2*)rtdb->getContainer("KickPlane2MDC3");

	  fTofinoGeomPar=(HTofinoGeomPar*)rtdb->getContainer("TofinoGeomPar");
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
      fCatTofCluster=event->getCategory(catTofCluster);
      if(!fCatTofCluster) Warning("init","No catTofClustter in input!");
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

  evHeader=gHades->getCurrentEvent()->getHeader();
  for(Int_t i=0; i<6; i++)
     tRans[i]=0;
  Spline=corr->getSPline();
  Spline->initMiddleParams(Spline->takeMiddleParams(fGetCont,0,2)); 
  Spline->setDataPointer(field->getPointer(),corr->getCorr1());
  Spline->setKickPointer(kickplane);
  if(pMagnet->getPolarity()>=0)
  {
  Spline->setMagnetScaling(pMagnet->getScalingFactor());
  fScal=pMagnet->getScalingFactor();
  }
  else 
  {Spline->setMagnetScaling(pMagnet->getScalingFactor()*(-1.));
   fScal=pMagnet->getScalingFactor()*(-1.);
  }	 

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
	 firstCandIndex=pMetaMatch->getFirstCandForMeta();
	 if(firstCandIndex==-1) continue;
         if(pMetaMatch->getNCandForMeta()<0) continue;
	 if((doMomentum(pMetaMatch,condition)))
	    fillData(segments[0],condition);
	 else
	    continue;
	 Int_t inndex;
	 while((inndex=pMetaMatch->getNextCandForMeta())>=0)
	    {
	       pMetaMatch=(HMetaMatch*)(fCatMetaMatch->getObject(inndex));
	       if(pMetaMatch->getTrkCandInd()==-1) continue;
	       if(doMassStuff(pMetaMatch))
		  fillData(segments[0],kTRUE);     
	    }
      }
   return 0;
}

Bool_t HSplineTrackF::doMomentum(HMetaMatch *pMetaMatch, Bool_t &condition)
{ //Impuls hesblamasini bashla()
   condition=kFALSE; //if there were meta hits or NOT!
   sector=pMetaMatch->getSector();
   sectorloc.set(1,(Int_t)sector);
   indTrkCand=pMetaMatch->getTrkCandInd();
   metaInd=pMetaMatch->getMetaHitInd();
   pMdcTrkCand=(HMdcTrkCand*)fCatMdcTrkCand->getObject(indTrkCand);
   if(!pMdcTrkCand) return kFALSE;
   index1=pMdcTrkCand->getSeg1Ind();
   index2=pMdcTrkCand->getSeg2Ind();
   if(index1<0 || index2<0) return kFALSE;
   metaInd=pMetaMatch->getMetaHitInd();
   segments[0]=(HMdcSeg*)fCatMdcSegSim->getObject(index1);
   segments[1]=(HMdcSeg*)fCatMdcSegSim->getObject(index2);
   system=pMetaMatch->getSystem();
   if(!segments[0] || !segments[1]) return kFALSE;
   polarity=Spline->getPolarity();
   if(metaInd<0) //This means there were no meta Hits
      {
	 calcMomentum(segments); 
	 condition=kFALSE;
	 return kTRUE;
      }
   else
      {
	 calcMomentum(segments,pMetaMatch);
	 doMassStuff(pMetaMatch);
	 condition=kTRUE;
      }
   
   return kTRUE;
}

void HSplineTrackF::calcMomentum(HMdcSeg *segments[])
{
   if(segments[1]->getHitInd(0)==-1 || segments[1]->getHitInd(1)==-1)
      { 
         //cout<<"3 chamber hali"<<endl;
	 Momentum= Spline->calcMomentum123(segments,kTRUE,0.);
	 numChambers=3;
      }
   else
      {
        //cout<<"4 chamber hali"<<endl;
	Momentum= Spline->calcMomentum(segments,kTRUE,0.);
	 numChambers=4;
      }
   Momentum/=(0.7215/fScal);
   qSpline=Spline->getqSpline();
   
}

void HSplineTrackF::calcMomentum(HMdcSeg *segments[], HMetaMatch *pMetaMatch)
{
   calcMomentum(segments);
   doMassStuff(pMetaMatch);
}
Bool_t HSplineTrackF::doMassStuff(HMetaMatch *pMetaMatch)
{ //Bashla Mass stuff
    metaInd=pMetaMatch->getMetaHitInd();

    tofClSize=(Int_t)pMetaMatch->getTofClusterSize();
    
    system=pMetaMatch->getSystem();
   if(system==-1) return kFALSE;
   else if (system==1)
      {   // for tof
	 if(tofClSize<=0)
	 pTofHit=(HTofHit*)fCatTof->getObject(metaInd);
	 else
	 pTofHit=(HTofHit*)fCatTofCluster->getObject(metaInd);

	 tof=pTofHit->getTof();
	 pTofHit->getXYZLab(xTof,yTof,zTof);

	 pointMeta.setXYZ(xTof,yTof,zTof);
	 if(tRans[(Int_t)sector]==0)
	    {
	       tRans[(Int_t)sector]=new HGeomTransform();
	       if(!fGetCont->getLabTransSec(*(tRans[(Int_t)sector]),sector))
		  {
		     return kFALSE;
		  }
	    }
	 pointMeta=tRans[(Int_t)sector]->transTo(pointMeta);
	 xTof=pointMeta.getX();
	 yTof=pointMeta.getY();
	 zTof=pointMeta.getZ();
	 distanceTof=Spline->getMetaDistance(xTof*0.1,yTof*0.1,zTof*0.1);
      }
   else if(system==0)
      {   // for shower
	 pShowerHitTofTrack=(HShowerHitTofTrack*)fCatShower->getObject(metaInd);
	 tof=pShowerHitTofTrack->getTof();
	 distanceTof=calcTofinoDistance(pShowerHitTofTrack);
      }
   
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
   return kTRUE;
}


Double_t HSplineTrackF::calcTofinoDistance(HShowerHitTofTrack *hit)
{
   HGeomVector posTofino;
   HGeomVector out,in1,in2,segpoints[4];
   const HGeomTransform &transMod=fTofinoGeomPar->getModule(hit->getSector(),
                                                            hit->getModule())->getLabTransform();
   
   HGeomTransform &transSec = fSpecGeomPar->getSector(hit->getSector())->getTransform();
   HGeomTransform transTofino(transMod);
   transTofino.transTo(transSec);
   const HGeomRotation &rotTofino = transTofino.getRotMatrix();
   const HGeomVector& vcTofino=transTofino.getTransVector();
   
   Double_t parCTofino=rotTofino(0)*rotTofino(4)-rotTofino(3)*rotTofino(1);
   Double_t parATofino=(rotTofino(3)*rotTofino(7)-rotTofino(6)*rotTofino(4))/parCTofino;
   Double_t parBTofino=(rotTofino(6)*rotTofino(1)-rotTofino(0)*rotTofino(7))/parCTofino;
   Double_t parDTofino=parATofino*vcTofino(0)+parBTofino*vcTofino(1)+vcTofino(2);

   if(segments[1]->getHitInd(0)==-1 || segments[1]->getHitInd(1)==-1)
      {
	 Spline->calcSegPoints123(segments,segpoints);
      }
   else
      {
	 Spline->calcSegPoints(segments,segpoints);
      }
   
   in1=segpoints[2];
   in2=segpoints[3];
					  
   Spline->calcInter(parATofino,parBTofino,0.1*parDTofino,in1,in2,out);
   Double_t len=(out-in1).length()+Spline->getFieldDistance();
  return len;
    
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
	 sp->setP(Momentum,0.);
	 sp->setZ(segment->getZ(),segment->getErrZ());
	 sp->setR(segment->getR(),segment->getErrR());
	 sp->setTheta(segment->getTheta(),segment->getErrTheta());
	 sp->setPhi(segment->getPhi(),segment->getErrPhi());
	 sp->setPolarity(polarity);
	 sp->setSector(sector);
	 pMetaMatch->setSplineInd(indexspline);
         sp->setTofHitInd(pMetaMatch->getTofHitInd());
         sp->setShowerHitInd(pMetaMatch->getShowerHitInd());
	 sp->setNumOfChambers(numChambers);
         if(qSpline<5.)
	 sp->setQSpline(qSpline);
	 if(condition)
	   {
	     sp->setTofDist(TOFdistance);
	     sp->setBeta(beta);
	     sp->setMass2(mass2,0.);
	     sp->setTof(tof);
	   }
      }
   else
      Error("FillData","No slots free");
   return sp;
}





