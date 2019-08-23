//*-- AUTHOR : Jochen Markert
//*-- Modified : 18/06/99 by Ilse Koenig

//_HADES_CLASS_DESCRIPTION 
///////////////////////////////////////////////////////////////////////////////
//
// HMdcWireInfo
//
// Data object for a Mdc cell containing indices of
// hit,seg,clusinf,clusfit,wirefit,clus and kickplane
// where this wire has been used
//
///////////////////////////////////////////////////////////////////////////////
#include "hmdcwireinfo.h"
#include "hmdcdef.h"
#include "hmdctrackddef.h"
#include "kickdef.h"
#include "hkicktrack.h"
#include "hmdcseg.h"
#include "hmdchit.h"
#include "hmdcclusinf.h"
#include "hmdcclusfit.h"
#include "hmdcwirefit.h"
#include "hmdcclus.h"
#include "hmdccal1.h"
#include "hmatrixcategory.h"
#include "hlinearcategory.h"
#include "hevent.h"
#include "hlocation.h"

ClassImp(HMdcWireInfo)

HMdcWireInfo::HMdcWireInfo()
{
    clear();
}
HMdcWireInfo::~HMdcWireInfo() {}

HKickTrack*  HMdcWireInfo::getKickTrack(Int_t slot)
{
    HMatrixCategory* kickcat=(HMatrixCategory*) gHades->getCurrentEvent()->getCategory(catKickTrack);
    HKickTrack* track=0;
    Int_t index=getIndex(HMdcWireInfo::kick,slot);
    if(kickcat)
    {
	if(index!=-1)track=(HKickTrack*)kickcat->getObject(index);
	else Warning("getKickTrack()","Index=-1!");
    }  else {
	Warning("getKickTrack()","No HKickTrack Catogory in Current Event!");
    }
    return track;
}
HMdcSeg*     HMdcWireInfo::getMdcSeg(Int_t slot)
{
    HMatrixCategory* segcat=(HMatrixCategory*) gHades->getCurrentEvent()->getCategory(catMdcSeg);
    HMdcSeg* seg=0;
    Int_t index=getIndex(HMdcWireInfo::seg,slot);
    if(segcat)
    {
	if(index!=-1)seg=(HMdcSeg*)segcat->getObject(index);
	//else Warning("getMdcSeg()","Index=-1!");
    } else {
	Warning("getMdcSeg()","No HMdcSeg Catogory in Current Event!");
    }
    return seg;
}
HMdcHit*     HMdcWireInfo::getMdcHit(Int_t slot)
{
    HMatrixCategory* hitcat=(HMatrixCategory*) gHades->getCurrentEvent()->getCategory(catMdcHit);
    HMdcHit* hit=0;
    Int_t index=getIndex(HMdcWireInfo::hit,slot);
    if(hitcat)
    {
	if(index!=-1)hit=(HMdcHit*)hitcat->getObject(index);
	//else Warning("getMdcHit()","Index=-1!");
    } else {
	Warning("getMdcHit()","No HMdcHit Catogory in Current Event!");
    }
    return hit;
}
HMdcClusInf* HMdcWireInfo::getMdcClusInf(Int_t slot)
{
    HLinearCategory* clusinfcat=(HLinearCategory*) gHades->getCurrentEvent()->getCategory(catMdcClusInf);
    HMdcClusInf* clusinf=0;
    Int_t index=getIndex(HMdcWireInfo::clusinf,slot);
    if(clusinfcat)
    {
	if(index!=-1)clusinf=(HMdcClusInf*)clusinfcat->getObject(index);
        //else Warning("getMdcClusInf()","Index=-1!");
    } else {
	Warning("getMdcClusInf()","No HMdcClusInf Catogory in Current Event!");
    }
    return clusinf;
}
HMdcClusFit* HMdcWireInfo::getMdcClusFit(Int_t slot)
{
    HLinearCategory* clusfitcat=(HLinearCategory*) gHades->getCurrentEvent()->getCategory(catMdcClusFit);
    HMdcClusFit* clusfit=0;
    Int_t index=getIndex(HMdcWireInfo::clusfit,slot);
    if(clusfitcat)
    {
	if(index!=-1)clusfit=(HMdcClusFit*)clusfitcat->getObject(index);
	//else Warning("getMdcClusFit()","Index=-1!");
    } else {
	Warning("getMdcClusFit()","No HMdcClusFit Catogory in Current Event!");
    }
    return clusfit;
}
HMdcWireFit* HMdcWireInfo::getMdcWireFit(Int_t slot)
{
    HLinearCategory* wirefitcat=(HLinearCategory*) gHades->getCurrentEvent()->getCategory(catMdcWireFit);
    HMdcWireFit* wirefit=0;
    Int_t index=getIndex(HMdcWireInfo::wirefit,slot);
    if(wirefitcat)
    {
	if(index!=-1)wirefit=(HMdcWireFit*)wirefitcat->getObject(index);
        //else Warning("getMdcWireFit()","Index=-1!");
    } else {
	Warning("getMdcWireFit()","No HMdcWireFit Catogory in Current Event!");
    }
    return wirefit;
}
HMdcClus*    HMdcWireInfo::getMdcClus(Int_t slot)
{
    HMatrixCategory* cluscat=(HMatrixCategory*) gHades->getCurrentEvent()->getCategory(catMdcClus);
    HMdcClus* clus=0;
    Int_t index=getIndex(HMdcWireInfo::clus,slot);
    if(cluscat)
    {
	if(index!=-1)clus=(HMdcClus*)cluscat->getObject(index);
        //else Warning("getMdcClus()","Index=-1!");
    } else {
	Warning("getMdcClus()","No HMdcClus Catogory in Current Event!");
    }
    return clus;
}
//HMdcWireInfo::getPid(Int_t slot){;}
HMdcCal1*    HMdcWireInfo::getMdcCal1(Int_t slot)
{
    HMatrixCategory* cal1cat=(HMatrixCategory*) gHades->getCurrentEvent()->getCategory(catMdcCal1);
    HMdcCal1* cal1=0;
    if(cal1cat){
	HLocation loccal;
	loccal.set(4,sector,module,layer,cell);
	cal1=(HMdcCal1*)cal1cat->getObject(loccal);
    } else {
	Warning("getMdcCal1()","No HMdcCal1 Catogory in Current Event!");
    }
    return cal1;

}
  
