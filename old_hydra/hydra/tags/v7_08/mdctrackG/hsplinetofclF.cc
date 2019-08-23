//Task for Tof clustering for Spline
//*-- Author : Anar Rustamov
//This task adds a flag to HMetaMatch
//The flag is : isTofCluster.
//+100 --means the cluster was accepted
//-100 --means the cluster was not accepted
using namespace std;
#include "hmetamatchF.h"
#include "hevent.h"
#include "hcategory.h"
#include "hiterator.h"
#include "hspectrometer.h"
#include "hruntimedb.h"
#include "hmdcdetector.h"
#include "hspecgeompar.h"
#include "tofdef.h"
#include "richdef.h"
#include "triggerinfodef.h"
#include "hmatrixcategory.h"
#include "hbasetrack.h"
#include "showertofinodef.h"
#include "hmetamatch.h"
#include "hgeomtransform.h"
#include "hmdctrackddef.h"
#include "hmdctrackgdef.h"
#include "hmdcseg.h"
#include "hkickplane2.h"
#include "hmdctrkcand.h"
#include "hmdctrackgspline.h"
#include "hgeomvector.h"
#include "hlocation.h"
#include "hshowerhittoftrack.h"
#include "hmdcgetcontainers.h"
#include "htofhit.h"
#include "hrichhit.h"
#include "hrichhitIPU.h"
#include <iostream>
#include "hkicktrack.h"
#include "hkicktrackB.h"
#include "kickdef.h"
#include "hsplinetofclF.h"

ClassImp(HSplineTofClF)
   
   HSplineTofClF::HSplineTofClF()
{
   ;
}

HSplineTofClF::HSplineTofClF(Text_t name[], Text_t title[],Int_t MODE):
   HReconstructor(name,title) {
   mode=MODE;
}

HSplineTofClF::~HSplineTofClF()
{
   ;   
}

Bool_t HSplineTofClF::init()
{
   if (!gHades) return kFALSE;
   HRuntimeDb *rtdb=gHades->getRuntimeDb();
   HSpectrometer *spec=gHades->getSetup();
   HEvent *event=gHades->getCurrentEvent();
   if(rtdb && spec && event)
      {
         fCatMetaMatch=event->getCategory(catMetaMatch);
         if(!fCatMetaMatch) return kFALSE;
	 iterMetaMatch1=(HIterator*)fCatMetaMatch->MakeIterator();
	 if(!iterMetaMatch1) return kFALSE;
         iterMetaMatch2=(HIterator*)fCatMetaMatch->MakeIterator();
	 if(!iterMetaMatch2) return kFALSE;
         fCatTrkCand=event->getCategory(catMdcTrkCand);
	 if(!fCatTrkCand) return kFALSE;
	 
      }
   return kTRUE;
}
Bool_t HSplineTofClF::reinit()
{
   return kTRUE;  
}

Bool_t HSplineTofClF::finalize()
{
   return kTRUE;
}

Int_t HSplineTofClF::execute()
{
  
  if(mode==0)
    {
      checkShowerOverlap();
      return unsetTofHitsFromCluster();
    }
  else 
    {
   iterMetaMatch1->Reset();
   step=0;
   while((pMetaMatch1=(HMetaMatch*)(iterMetaMatch1->Next()))!=0)
      {
         if((Int_t)(pMetaMatch1->getTofClusterSize())==2)
	    {
	    //cout<<"tof cluster size is 2"<<endl;
	       checkCluster(pMetaMatch1);
	    }
	 step++;
      }
   
    }
    return 1;
}

void HSplineTofClF::checkCluster(HMetaMatch *p1)
{
   iterMetaMatch2->Reset();
   step1=0;
   while((pMetaMatch2=(HMetaMatch*)(iterMetaMatch2->Next()))!=0)
      {
	 if(step1==step) continue;
	 if(condMeta(pMetaMatch2,p1))
	    {
	       if(checkForSeg(p1,pMetaMatch2))
		  {
		     p1->unsetSplineAccept();
		     p1->unsetRungeKuttaAccept();
		     
		  }
	    }
	 step1++;
      }
}

Bool_t HSplineTofClF::condMeta(HMetaMatch *p1,HMetaMatch *p2)
{
   if(p1->getTofHitInd()==p2->getTofHitInd())
      return kTRUE;
   else return kFALSE;
}

Bool_t HSplineTofClF::checkForSeg(HMetaMatch *p1, HMetaMatch *p2)
{
   HMdcTrkCand *tr1 ,*tr2;
   Int_t tr1Ind,tr2Ind;
   tr1Ind=p1->getTrkCandInd();
   tr2Ind=p2->getTrkCandInd();
   if(tr1Ind<0 || tr2Ind<0) return kFALSE;	
   tr1=(HMdcTrkCand*)(fCatTrkCand->getObject(tr1Ind));
   tr2=(HMdcTrkCand*)(fCatTrkCand->getObject(tr2Ind));
   if(!tr1 || !tr2) return kFALSE;
   ind1=tr1->getSeg1Ind();
   ind2=tr1->getSeg2Ind();
   ind3=tr2->getSeg1Ind();
   ind4=tr2->getSeg2Ind();
   if(ind2<0 || ind4<0) return kFALSE;
   if(tr1Ind!=tr2Ind && ind1!=ind3) return kTRUE;
   else return kFALSE;
}

Bool_t HSplineTofClF::checkCandForSameMeta(HMetaMatch *pMetaMatch)
   
{
   HMetaMatch *pMetaMatch2=0;
   Int_t index; 
   if(pMetaMatch) pMetaMatch2=pMetaMatch;
   while((index=pMetaMatch2->getNextMMForSameMeta())>=0)
      {
	 pMetaMatch2=(HMetaMatch*)(fCatMetaMatch->getObject(index));
	 if(!pMetaMatch2) return kFALSE;
	 pMetaMatch2->unsetSplineAccept();
	 pMetaMatch2->unsetRungeKuttaAccept();
      }
   
   return kTRUE;
}
Int_t  HSplineTofClF::unsetTofHitsFromCluster()
   
{
   HMetaMatch *pMetaMatch;
   iterMetaMatch1->Reset();
   
   while((pMetaMatch=(HMetaMatch*)(iterMetaMatch1->Next()))!=0)
      {
	 if(pMetaMatch->getNumMMForSameMeta()<0) continue;
	 if(pMetaMatch->getTofClusterSize()==2 && pMetaMatch->getNextMMForTofHit()>=0)
	    {
	       if(!checkNextTofHits(pMetaMatch)) continue;
	    }	
      }
   return 1;
}
Bool_t  HSplineTofClF::checkNextTofHits(HMetaMatch *pMetaMatch)
   
{
   Int_t index;
//   if (!(pMetaMatch->isSplineAccepted())) return kFALSE;
   while((index=pMetaMatch->getNextMMForTofHit())>=0)
      {
	 pMetaMatch=(HMetaMatch*)(fCatMetaMatch->getObject(index));
	 pMetaMatch->unsetSplineAccept();
	 pMetaMatch->unsetRungeKuttaAccept();
	 if(!pMetaMatch) return kFALSE;
	 if(!checkCandForSameMeta(pMetaMatch)) continue;
      }
   return kTRUE;
}


void HSplineTofClF::checkShowerOverlap()
{
   HMetaMatch *pMetaMatch,*pMetaMatch1;
   Int_t index;
   Int_t firstCandIndex;  
   iterMetaMatch1->Reset();
   while((pMetaMatch=(HMetaMatch*)(iterMetaMatch1->Next()))!=0)
      {

        firstCandIndex=pMetaMatch->getFirstCandForMeta();
	if(firstCandIndex==-1) continue;
		       
	 if(pMetaMatch->getNCandForMeta()<0) continue;

	 while((index=pMetaMatch->getNextCandForMeta())>=0)
	   {
	     //pMetaMatch=(HMetaMatch*)(fCatMetaMatch->getObject(index));
	     
	     if(pMetaMatch->getTofHitInd()>=0)
	       {
		 index=pMetaMatch->getNextCandForMeta();
		 if(index>=0)
		   {
		     pMetaMatch=(HMetaMatch*)(fCatMetaMatch->getObject(index));  
		     if(pMetaMatch->getShowerHitInd()>=0)
		     {
		     pMetaMatch->unsetSplineAccept();
		     pMetaMatch->unsetRungeKuttaAccept();
		     }
		     
		   }
	       }
	     else if(pMetaMatch->getShowerHitInd()>=0)
		{
		
	          pMetaMatch1=pMetaMatch;	
		  index=pMetaMatch->getNextCandForMeta();
		  if(index>=0)
		    {
		      pMetaMatch=(HMetaMatch*)(fCatMetaMatch->getObject(index));  
		      if(pMetaMatch->getTofHitInd()>=0)
		      {
		      pMetaMatch1->unsetSplineAccept();
		      pMetaMatch1->unsetRungeKuttaAccept();
		      }
		      
		    }
		}
	      
	   }
	 
	 
      }
}

	 
