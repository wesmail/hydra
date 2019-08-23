//*-- Author : Anar Rustamov (27.08.2003)
#include <iostream>
#include "hades.h"
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
#include "showertofinodef.h"
#include "hspecgeompar.h" 
#include "tofdef.h"
#include "showerdef.h"
#include "hmdctrackgcorrpar.h"
#include "hmdctrackgfieldpar.h"
#include "hmatrixcategory.h"
#include "hbasetrack.h"
#include "showertofinodef.h"
#include "hgeantmdc.h"
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
   fCatMdcSegsMatch=0;
   fCatSplineTrack=0;
   fCatMdcSegSim=0;
   fMdcGeometry=0;
   fSpecGeomPar=0;
   Spline=0;
   fMdcSegsMatchIter=0;
   field=0;
   corr=0;
}
HSplineTrackF::HSplineTrackF(Text_t *name, Text_t *title,Int_t flag1):
   HReconstructor(name,title)
{
   flag=flag1;
   C=299792458;
   fCatMdcSegsMatch=0;
   fCatSplineTrack=0;
   fCatMdcSegSim=0;
   fMdcGeometry=0;
   // fSpecGeometry=0;
   fSpecGeomPar=0;
   Spline=0;
   fMdcSegsMatchIter=0;
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
	 histoXtof=new TH1F("histoXtof","histoXtof",100,-100,100);
	 histoYtof=new TH1F("histoYtof","histoYtof",100,-100,100);
	 histoXshower=new TH1F("histoXshower","histoXshower",100,-100,100);
	 histoYshower=new TH1F("histoYshower","histoYshower",100,-100,100);
	 
	 HRuntimeDb *rtdb=gHades->getRuntimeDb();
	 HSpectrometer *spec=gHades->getSetup();
	 HEvent *event=gHades->getCurrentEvent();
	 if(rtdb && spec && event)
	    {
	       field=(HMdcTrackGFieldPar*)(rtdb->getContainer("MdcTrackGFieldPar"));
	       corr=(HMdcTrackGCorrPar*)(rtdb->getContainer("MdcTrackGCorrPar"));
	       	         fSpecGeomPar=(HSpecGeomPar*)(rtdb->getContainer("SpecGeomPar"));
	       //fMdcGeometry=(HMdcGeomPar*)(rtdb->getContainer("MdcGeomPar"));
               fGetCont=HMdcGetContainers::getObject();
               fGetCont->getMdcGeomPar();
               fGetCont->getSpecGeomPar();
	       //	       targetPosition=fSpecGeomPar->getTarget(0)->getTransform().getTransVector();
	       //	       target=targetPosition.getZ();
	       
	    } 
	 fCatMdcSegsMatch=event->getCategory(catMdcTrkCand);
	 if (!fCatMdcSegsMatch) return kFALSE;
	 fMdcSegsMatchIter=(HIterator*)fCatMdcSegsMatch->MakeIterator();
	 if(!fMdcSegsMatchIter) return kFALSE;
	 fCatMdcSegSim=event->getCategory(catMdcSeg);
	 
         fCatTof=event->getCategory(catTofHit);
         if(!fCatTof) return kFALSE;
	 
         IterTof=(HIterator*)fCatTof->MakeIterator();
         
	 if(flag==1)
	 {
	 fCatShower=event->getCategory(catShowerHitTof);
	 
	 
         if(!fCatShower) return kFALSE;
         IterShower=(HIterator*)fCatShower->MakeIterator();
	 }
	 if(flag==0)
	 {
	 fCatShower=event->getCategory(catShowerHitTofTrack);
	 if(!fCatShower) return kFALSE;
         IterShower=(HIterator*)fCatShower->MakeIterator();
	 }
	 
	 
	 
	 fCatSplineTrack=event->getCategory(catSplineTrack);
	 if(!fCatSplineTrack)
	    {
	       Int_t size[2]={6,100};
	       fCatSplineTrack=new HMatrixCategory("HSplineTrack",2,size,0.5);  
	       if(fCatSplineTrack) 
		  event->addCategory(catSplineTrack,fCatSplineTrack,"SplineTrack");
	       
	    }
      }
   for(Int_t i=0;i<6;i++)
      tRans[i]=0;
   return kTRUE;
}
Bool_t HSplineTrackF::reinit()
{
   corr->getSPline()->setDataPointer(field->getPointer(),corr->getCorr());
   return kTRUE;
}
Bool_t HSplineTrackF::finalize()
{
   //TFile f("test.root","recreate");
   //histoXtof->Write();
   //histoYtof->Write();
   //histoXshower->Write();
   //histoYshower->Write();
   //f.Close();
   return kTRUE;
}
Int_t HSplineTrackF::execute()
{
   return mdc1234();
}
Int_t HSplineTrackF::mdc1234()
{
   //   sectorloc.set(1,0);
   HMdcTrkCand *pMdcSegs=0;
   Short_t index1,index2;
   fMdcSegsMatchIter->Reset();
   while((pMdcSegs=(HMdcTrkCand*)(fMdcSegsMatchIter->Next()))!=0)
      {   //while in segs
	 HMdcSegSim *segments[2]={0,0};
	 index1=pMdcSegs->getSeg1Ind();
	 index2=pMdcSegs->getSeg2Ind();
	 Short_t nCandForSeg1=pMdcSegs->getNCandForSeg1();
	 if(index1>=0 && index2>=0) 
	    { //if index1>=0
	       IndTrkCand=fCatMdcSegsMatch->getIndex(pMdcSegs);
	       sectorloc.set(1,(int)pMdcSegs->getSec());
	       sector=(Int_t)pMdcSegs->getSec();
	       segments[0]=(HMdcSegSim*)fCatMdcSegSim->getObject(index1);
	       segments[1]=(HMdcSegSim*)fCatMdcSegSim->getObject(index2);
	       if(!segments[0] || !segments[1]) continue;
	       ZET=segments[1]->getZ();
	       RO=segments[1]->getR();
	       THETA=segments[1]->getTheta();
	       PHI=segments[1]->getPhi();
	       temp1[0]=RO*cos(PHI+acos(-1.)/2);
	       temp1[1]=RO*sin(PHI+acos(-1.)/2);
	       temp1[2]=ZET;
	       temp2[0]=temp1[0]+cos(PHI)*sin(THETA);
	       temp2[1]=temp1[1]+sin(PHI)*sin(THETA);
	       temp2[2]=temp1[2]+cos(THETA);
	       point1.setXYZ(temp1[0],temp1[1],temp1[2]);
	       point2.setXYZ(temp2[0],temp2[1],temp2[2]);
	       //Tof and shower part
	       HTofHitSim *pTofHit=0;
	       IterTof->Reset();
                //cout<<"Sector=  "<<(Int_t)sector<<endl;
	       while((pTofHit=(HTofHitSim*)(IterTof->Next()))!=0 )
		  {  //begin tof loop
		     pTofHit->getXYZLab(Xtof,Ytof,Ztof);
		     point.setXYZ(Xtof,Ytof,Ztof);
		     if(tRans[(Int_t)sector]==0)
			{
			   tRans[(Int_t)sector]=new HGeomTransform();
			   if(!fGetCont->getLabTransSec(*(tRans[(Int_t)sector]),(Int_t)sector))
			      continue; 
			}
			
			
			//HGeomTransform test=fGetCont->getLabTransSec(1);
			//tRans[(Int_t)sector]->print();
			
			//cout<<"SECTORSHOWR="<<(Int_t)sector<<endl;
			 
		     point=tRans[sector]->transTo(point);
		     Xtof=point.getX();
		     Ytof=point.getY();
		     Ztof=point.getZ();
		     corr->getSPline()->equationofLine3D(point1,point2,Ztof,Xcalc,Ycalc);
		     quality=(Xtof-Xcalc)*(Xtof-Xcalc)/(23*23)+(Ytof-Ycalc)*(Ytof-Ycalc)/(23*23);
		     tofflag=0;
		     if(quality<18)
			{ 
			tofflag=1;
			//cout<<"TOF"<<endl;
			   outerHitInd=fCatTof->getIndex(pTofHit);
			   tof=pTofHit->getTof();
			   distanceTof=corr->getSPline()->getMetaDistance(0.1*Xtof,0.1*Ytof,0.1*Ztof);//in cm
			   const HGeomVector &targetPos = fSpecGeomPar->getTarget(0)->getTransform().getTransVector();
			   HGeomVector POINT1=corr->getSPline()->getPointOne();
			   Double_t XXX=POINT1.getX();
			   Double_t YYY=POINT1.getY();
			   Double_t ZZZ=POINT1.getZ();
			   POINT1.setXYZ(10*XXX,10*YYY,10*ZZZ);
			   Double_t dist = (POINT1 - targetPos).length();
			   TOFdistance=dist+distanceTof*10; //in mm
			   beta=TOFdistance/tof/C;

			  // cout<<"TOFdistance"<<TOFdistance<<endl;
			   
			   beta*=1e6;
			   Momentum=corr->calcMomentum(segments,kTRUE,segments[0]->getZ());
			   mass2=Momentum*Momentum*(1-beta*beta)/beta/beta/C/C;
			   polarity=corr->getSPline()->getPolarity();
			   system=1;
			   if(Momentum<3000) fillData(segments[0],kTRUE);
			   
			}
		  }
	       HShowerHitTofTrack * pShowerHitTofTrack;
	       IterShower->Reset();
	       while((pShowerHitTofTrack=(HShowerHitTofTrack*)(IterShower->Next()))!=0)
		  { //loop over shower
		     pShowerHitTofTrack->getLabXYZ(&Xtof,&Ytof,&Ztof);
		     point.setXYZ(Xtof,Ytof,Ztof);
		     if(tRans[(Int_t)sector]==0)
			{
			   tRans[(Int_t)sector]=new HGeomTransform();
			   if(!fGetCont->getLabTransSec(*(tRans[(Int_t)sector]),(Int_t)sector))
			      continue; 
			} 
		     point=tRans[sector]->transTo(point);
		     Xtof=point.getX();
		     Ytof=point.getY();
		     Ztof=point.getZ();
		     corr->getSPline()->equationofLine3D(point1,point2,Ztof,Xcalc,Ycalc);
		     quality=(Xtof-Xcalc);
		     if (quality<0) quality*=-1;
		     //*(Xtof-Xcalc)/(12*12);
		     
		     qualityY=Ytof-Ycalc;
		    // cout<<"SHOWSEC"<<(Int_t)sector<<endl;
		     if(qualityY<0) qualityY*=-1;
		     showerflag=0;
		     if(qualityY<70 && quality<70)
			{ 
			  showerflag=1;
			   outerHitInd=fCatShower->getIndex(pShowerHitTofTrack);
			   tof=pShowerHitTofTrack->getTof();
			   distanceTof=corr->getSPline()->getMetaDistance(0.1*Xtof,0.1*Ytof,0.1*Ztof);//in cm
			   const HGeomVector &targetPos = fSpecGeomPar->getTarget(0)->getTransform().getTransVector();
			 
			   //cout<<"SHOWVER"<<endl;
			   HGeomVector POINT1=corr->getSPline()->getPointOne();
			   Double_t XXX=POINT1.getX();
			   Double_t YYY=POINT1.getY();
			   Double_t ZZZ=POINT1.getZ();
			   POINT1.setXYZ(10*XXX,10*YYY,10*ZZZ);
			   Double_t dist = (POINT1 - targetPos).length();
			   TOFdistance=dist+distanceTof*10; //in mm
			   
			   
			   
			   beta=TOFdistance/tof/C;

                         //  cout<<"Showerdistance"<<TOFdistance<<endl;
			   
			   beta*=1e6;
			   Momentum=corr->calcMomentum(segments,kTRUE,segments[0]->getZ());
			   mass2=Momentum*Momentum*(1-beta*beta)/beta/beta;
			   polarity=corr->getSPline()->getPolarity();
			   system=0;
			   if(Momentum<3000) fillData(segments[0],kTRUE);
			   
			}
			if(!showerflag && !tofflag) fillData(segments[0],kFALSE);
		  }
		  
	    }
      } 
   return 1;
   
}
HSplineTrack *  HSplineTrackF::fillData(HMdcSegSim *segment,Bool_t condition)
{
   slot=fCatSplineTrack->getNewSlot(sectorloc);
   if(slot)
      sp= new(slot) HSplineTrack;
   if(sp)
      {
	 sp->setP(Momentum,0);
	 sp->setZ(segment->getZ(),segment->getErrZ());
	 sp->setR(segment->getR(),segment->getErrR());
	 sp->setTheta(segment->getTheta(),segment->getErrTheta());
	 sp->setPhi(segment->getPhi(),segment->getErrPhi());
	 sp->setTrkCandInd(IndTrkCand);
	 sp->setPolarity(polarity);
	 sp->setSector(segment->getSec());
	 if(condition)
	 {
	 sp->setBeta(beta);
	 sp->setMass2(mass2,0);
	 sp->setTof(tof);
	  sp->setSystem(system);
	 sp->setOuterInd(outerHitInd);
	 
	 }
      }
   else
      Error("FillData","No slots free");
   return sp;
}
