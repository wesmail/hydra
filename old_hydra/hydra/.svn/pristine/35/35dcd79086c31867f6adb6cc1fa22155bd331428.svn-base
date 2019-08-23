//Task for all Matchings, fills hmetamatch
//*-- Author : Anar Rustamov (27.08.2003)
#include <iostream>
#include "hades.h"
#include "hcategory.h"
#include "hdetector.h"
#include "hevent.h"
#include "htofhitsim.h"
#include "hiterator.h"
#include "hgeomvolume.h"
#include "hgeomcompositevolume.h"
#include "hspectrometer.h"
#include "hruntimedb.h"
#include "hmdcgeompar.h"
#include "hspecgeompar.h" 
#include "tofdef.h"
#include "showerdef.h"
#include "hmdctrackgcorrpar.h"
#include "hmdctrackgfieldpar.h"
#include "hmatrixcategory.h"
#include "hbasetrack.h"
#include "showertofinodef.h"
#include "hgeantmdc.h"
#include "hmetamatchF.h"
#include "hmetamatch.h"
   //#inlcude "hsplinetrackf.h"
#include "hgeantkine.h"
#include "hgeomtransform.h"
#include "hgeomvolume.h"
#include "hmdctrackddef.h"
#include "hmdctrackgdef.h"
#include "hmdcsegsim.h"
#include "hruntimedb.h"
#include "hkickplane2.h"
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
ClassImp(HMetaMatchF)
   HMetaMatchF::HMetaMatchF()
{
   fCatTrkCand=0;
   fCatMdcSegSim=0;
   fMdcGeometry=0;
   fSpecGeomPar=0;
   Spline=0;
   fTrkCandIter=0;
   condGeant=kFALSE;
}
HMetaMatchF::HMetaMatchF(Text_t *name, Text_t *title):
   HReconstructor(name,title)
{
   fCatTrkCand=0;
   fCatMdcSegSim=0;
   fMdcGeometry=0;
   fSpecGeomPar=0;
   Spline=0;
   fTrkCandIter=0;
   condGeant=kFALSE;
}
HMetaMatchF::~HMetaMatchF()
{
   ;
}
Bool_t HMetaMatchF::init()
{
   if (gHades)
      {
	 HRuntimeDb *rtdb=gHades->getRuntimeDb();
	 HSpectrometer *spec=gHades->getSetup();
	 HEvent *event=gHades->getCurrentEvent();
	 if(rtdb && spec && event)
	    {
	       fSpecGeomPar=(HSpecGeomPar*)(rtdb->getContainer("SpecGeomPar"));
	       fGetCont=HMdcGetContainers::getObject();
	       fGetCont->getMdcGeomPar();
	       fGetCont->getSpecGeomPar();
	       kickplane=(HKickPlane2*)(rtdb->getContainer("KickPlane2MDC3"));
	       if(!kickplane) cout<<"YESSS DO IT!!!"<<endl;
	    }
	 Spline=new HMdcTrackGSpline("Spline","Spline"); 
	 fCatTrkCand=event->getCategory(catMdcTrkCand);
	 if (!fCatTrkCand) return kFALSE;
	 fTrkCandIter=(HIterator*)fCatTrkCand->MakeIterator();
	 if(!fTrkCandIter) return kFALSE;
	 fCatMdcSegSim=event->getCategory(catMdcSeg);
	 fCatTof=event->getCategory(catTofHit);
	 if(!fCatTof) return kFALSE;
	 IterTof=(HIterator*)fCatTof->MakeIterator();
	 fCatKine=event->getCategory(catGeantKine);
	 if(!fCatKine)
	    {
	       
	       fCatShower=event->getCategory(catShowerHitTof);
	       if(!fCatShower) return kFALSE;
	    }
	 else
	    {
	       condGeant=kTRUE;
	       fCatShower=event->getCategory(catShowerHitTofTrack);
	       if(!fCatShower) return kFALSE;
	    }
         IterShower=(HIterator*)fCatShower->MakeIterator();
	 fCatMetaMatch=event->getCategory(catMetaMatch);
	 if(!fCatMetaMatch)
	    {
	       Int_t size[2]={6,100};
	       fCatMetaMatch=new HMatrixCategory("HMetaMatch",2,size,0.5);  
	       if(fCatMetaMatch) 
		  event->addCategory(catMetaMatch,fCatMetaMatch,"MetaMatching");
	    }
      }
   return kTRUE;
}
Bool_t HMetaMatchF::reinit()
{
   Spline->setKickPointer(kickplane);
   for(Int_t i=0; i<6; i++)
      tRans[i]=0;
   if(condGeant)
      {
	 sigmaTofX=26*26;
	 sigmaTofY=22*22;
	 sigmaShowerX=12*12;
	 sigmaShowerY=14*14;
	 sTofX=0.;
	 sTofY=0.;
	 sShowerX=0.;
	 sShowerY=0.;
      }
   else
      {
	 sigmaTofX=43*43;
	 sigmaTofY=52*52;
	 sigmaShowerX=24*24;
	 sigmaShowerY=26*26;
	 sTofX=0.3;
	 sTofY=-13;
	 sShowerX=-4.5;
	 sShowerY=38.;
      }
   
   return kTRUE;
}
Bool_t HMetaMatchF::finalize()
{
   return kTRUE;
}
Int_t HMetaMatchF::execute()
{
   return matching1234(); //Mathing in the case of 3 and 4 chambers
}
Int_t HMetaMatchF::matching1234()
{
   candidatesNumber=fCatTof->getEntries()+fCatShower->getEntries();
   fTrkCandIter->Reset();
   system=-1;
   totalNumber=0;
   firstCandIndex=-1;
   while((pTrkCand=(HMdcTrkCand*)(fTrkCandIter->Next()))!=0)
      {
	 index1=pTrkCand->getSeg1Ind();
	 index2=pTrkCand->getSeg2Ind();
	 tempcounter=0;
    	 if(index1>=0 && index2>=0) 
	    { 
	       trkCandIndex=fCatTrkCand->getIndex(pTrkCand);
	       sector=pTrkCand->getSec();
	       sectorloc.set(1,sector);
	       segments[0]=(HMdcSegSim*)fCatMdcSegSim->getObject(index1);
	       segments[1]=(HMdcSegSim*)fCatMdcSegSim->getObject(index2);
	       if(!segments[0] || !segments[1]) continue;
	       if(segments[1]->getHitInd(0)==-1 && segments[1]->getHitInd(1)==-1 ||
		  segments[1]->getHitInd(0)==-1) continue;
	       if(segments[1]->getHitInd(1)==-1) 
		  Spline->calcSegPoints123(segments,POINTS);
	       else
		  Spline->calcSegPoints(segments,POINTS);  
	       IterTof->Reset();
	       while((pTofHit=(HTofHitSim*)(IterTof->Next()))!=0 )
		  {
		     system=1; 
		     totalNumber++;
		     metaHitIndex=fCatTof->getIndex(pTofHit);
		     pTofHit->getXYZLab(Xtof,Ytof,Ztof);
		     point.setXYZ(Xtof,Ytof,Ztof);
		     if(tRans[sector]==0)
			{
			   tRans[sector]=new HGeomTransform();
			   if(!fGetCont->getLabTransSec(*(tRans[sector]),sector))
			      continue; 
			} 
		     point=tRans[sector]->transTo(point);
		     Xtof=point.getX();
		     Ytof=point.getY();
		     Ztof=point.getZ();
		     metaDistance=Spline->getMetaDistance(0.1*Xtof,0.1*Ytof,0.1*Ztof);
		     Spline->equationofLine3D(POINTS[2],POINTS[3],0.1*Ztof,Xcalc,Ycalc);
		     qualityTof=(Xtof-10*Xcalc-sTofX)*(Xtof-10*Xcalc-sTofX)/sigmaTofX+(Ytof-10*Ycalc-sTofY)*(Ytof-10*Ycalc-sTofY)/sigmaTofY;
		     if(qualityTof<9)
			fillMeta(kTRUE);
		  }
	       IterShower->Reset();
	       while((pShowerHitTofTrack=(HShowerHitTofTrack*)(IterShower->Next()))!=0)
		  { //loop over shower
		     system=0;
		     totalNumber++;
		     metaHitIndex=fCatShower->getIndex(pShowerHitTofTrack);
		     pShowerHitTofTrack->getLabXYZ(&Xtof,&Ytof,&Ztof);
		     point.setXYZ(Xtof,Ytof,Ztof);
		     if(tRans[sector]==0)
			{
			   tRans[sector]=new HGeomTransform();
			   if(!fGetCont->getLabTransSec(*(tRans[sector]),sector))
			      continue; 
			} 
		     point=tRans[sector]->transTo(point);
		     Xtof=point.getX();
		     Ytof=point.getY();
		     Ztof=point.getZ();
		     metaDistance=Spline->getMetaDistance(0.1*Xtof,0.1*Ytof,0.1*Ztof);
		     Spline->equationofLine3D(POINTS[2],POINTS[3],0.1*Ztof,Xcalc,Ycalc);
		     qualityShower=(Xtof-10*Xcalc-sShowerX)*(Xtof-10*Xcalc-sShowerX)/sigmaShowerX+(Ytof-10*Ycalc-sShowerY)*(Ytof-10*Ycalc-sShowerY)/sigmaShowerY;
		     if(qualityShower<9)
			fillMeta(kTRUE);
		  }
	    }
	 
	 if(candidatesNumber==0)
	    fillMeta(kFALSE);
      }
   if(firstCandIndex!=-1)
      fillMeta1();
   return 1;
}
HMetaMatch *HMetaMatchF::fillMeta(Bool_t condition)
{
   Int_t index=-1;
   HMetaMatch * meta=(HMetaMatch*)fCatMetaMatch->
      getNewSlot(sectorloc,&index);
   if(!meta)
      {
	 cout<<"No slot available,,,"<<endl;
	 return 0;
      }
   meta=(HMetaMatch*)(new(meta) HMetaMatch);
   meta->setTrkCandInd(trkCandIndex);
   meta->setSector(sector);
   if(condition)
      {
	 meta->setMetaHitInd(metaHitIndex);
	 if(firstCandIndex!=-1)
	    {
	       HMetaMatch *metaold=(HMetaMatch*)(fCatMetaMatch->getObject(indexprevios));
	       metaold->setNextCandForMeta(index);
	    }
	 meta->setSystem(system);
      }
   if(firstCandIndex==-1)
      firstCandIndex=index;
   meta->setFirstCandForMeta(firstCandIndex);
   indexprevios=index;
   return meta;
}
void HMetaMatchF::fillMeta1()
{
   HMetaMatch *tempMetaMatch;
   Int_t Index=-10;
   tempMetaMatch=(HMetaMatch*)(fCatMetaMatch->getObject(firstCandIndex));
   tempMetaMatch->setNCandForMeta(totalNumber); 
   while(Index!=-1)
      {
	 Index=tempMetaMatch->getNextCandForMeta();
	 if(Index=-1) break;
	 tempMetaMatch=(HMetaMatch*)(fCatMetaMatch->getObject(Index));
	 tempMetaMatch->setNCandForMeta(totalNumber);
      }
}


