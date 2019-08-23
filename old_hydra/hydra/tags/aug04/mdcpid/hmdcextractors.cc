//*-- AUTHOR : J.Markert

///////////////////////////////////////////////////////////////////////////////
//
// HMdcCal1Extractor
//
// Extracts a Mdc objects
//
///////////////////////////////////////////////////////////////////////////////
using namespace std;
#include <iostream>
#include <iomanip>

#include "hmdcextractors.h"
#include "hmdccal1.h"
#include "hmdcwireinfo.h"
#include "hmdchit.h"
#include "hmdcclusinf.h"
#include "hmdcclusfit.h"
#include "hmdcwirefit.h"
#include "hmdcclus.h"
#include "hmdcseg.h"
#include "hmdctrkcand.h"
#include "hkicktrack.h"
#include "hcategory.h"
#include "hrecevent.h"
#include "hmatrixcategory.h"
#include "hlinearcategory.h"
#include "hmdcdef.h"
#include "hmdcpiddef.h"
#include "kickdef.h"
#include "hmdctrackddef.h"
#include "hiterator.h"

ClassImp(HMdcCal1Extractor)
ClassImp(HMdcClusInfExtractor)
ClassImp(HMdcClusFitExtractor)
ClassImp(HMdcWireFitExtractor)
ClassImp(HMdcWireInfoExtractor)
ClassImp(HMdcHitExtractor)
ClassImp(HMdcSegExtractor)
ClassImp(HMdcTrkCandExtractor)

//----------------------------------------CAL1-------------------------------------------------------
HMdcCal1Extractor::HMdcCal1Extractor(HRecEvent* cpev)
{
    if(cpev==0)
    {
	Error("HMdcCal1Extractor()","ZERO pointer for event received!");
        exit(1);
    }
    cpevent=cpev;
    loccal.set(4,0,0,0,0);
}
HMdcCal1Extractor::~HMdcCal1Extractor()
{
}

void  HMdcCal1Extractor::extract(HMdcSeg* seg,Int_t index)
{
    HMatrixCategory* cal1Cat  =(HMatrixCategory*)((HRecEvent*)(gHades->getCurrentEvent()))->getCategory(catMdcCal1);

    if(cal1Cat)
    {
        HMatrixCategory* cal1Catcp=(HMatrixCategory*)cpevent->getCategory(catMdcCal1);

        if(cal1Catcp)
        {
            for(Int_t l=0;l<12;l++)
            {
                Int_t nCell=seg->getNCells(l);
                for(Int_t i=0;i<nCell;i++)
                {
                    loccal[0]=seg->getSec();
                    Int_t ioseg= seg->getIOSeg();
                    if(ioseg==0){
                        (l<6)? loccal[1]=0 : loccal[1]=1;
                    }else if(ioseg==1){
                        (l<6)? loccal[1]=2 : loccal[1]=3;
                    }
                    (l<6)? loccal[2]=l : loccal[2]=l-6;
                    loccal[3]=seg->getCell(l,i);

                    if(((HMdcCal1 *)cal1Cat->getObject(loccal))!=0)
                    {
                        HMdcCal1* cal1=(HMdcCal1 *)cal1Cat->getObject(loccal);

                        if(!cal1Catcp->getObject(loccal))
                        {
                            HMdcCal1* cal1cp=(HMdcCal1 *)cal1Catcp->getSlot(loccal);
                            if (cal1cp)
                            {
                                cal1cp=new (cal1cp) HMdcCal1(*cal1);
                            } else Warning("extract()","No slot for HMdcCal1 obj!");
                        } //else Warning("extract()","HMdcCal1 obj already exists in output!");
		    } else {
			Warning("extract()","No HMdcCal1 obj for %i %i %i %i!",loccal[0],loccal[1],loccal[2],loccal[3]);
		    }
		}
            }
        } else Warning("extract()","No HMdcCal1 category in output!");
    } else Warning("extract()","No HMdcCal1 category in input!");
}
void HMdcCal1Extractor::clear(){}

//----------------------------------------WIREINFO-----------------------------------------------------

HMdcWireInfoExtractor::HMdcWireInfoExtractor(HRecEvent* cpev)
{
    if(cpev==0)
    {
	Error("HMdcWireInfoExtractor()","ZERO pointer for event received!");
	exit(1);
    }
    cpevent=cpev;
    loccal.set(4,0,0,0,0);
    kicklevel   =kFALSE;
    seglevel    =kFALSE;
    hitlevel    =kFALSE;
    clusinflevel=kFALSE;
    clusfitlevel=kFALSE;
    wirefitlevel=kFALSE;
}
void HMdcWireInfoExtractor::setDataLevels(TString levels)
{
    if(levels.Contains("kick")==1)    kicklevel=kTRUE;
    if(levels.Contains("seg")==1)     seglevel=kTRUE;
    if(levels.Contains("hit")==1)     hitlevel=kTRUE;
    if(levels.Contains("clusinf")==1) clusinflevel=kTRUE;
    if(levels.Contains("clusfit")==1) clusfitlevel=kTRUE;
    if(levels.Contains("wirefit")==1) wirefitlevel=kTRUE;
}
HMdcWireInfoExtractor::~HMdcWireInfoExtractor()
{
}
void  HMdcWireInfoExtractor::extract(HKickTrack* kick,Int_t index)
{
    Int_t kickindex=-1;
    Int_t sector=-1,module=-1,layer=-1,cell=-1;
    HMdcSeg* segcp=0;
    HMatrixCategory* segcatcp  =(HMatrixCategory*)cpevent->getCategory(catMdcSeg);
    if(kicklevel)
    {
	HMatrixCategory* kickcatcp =(HMatrixCategory*)cpevent->getCategory(catKickTrack);
        kickindex= kickcatcp->getIndex(kick);



	if(seglevel)
	{
	    HLocation locseg;
            locseg.set(3,kick->getSector(),0,kick->getSegment());
	    segcp=(HMdcSeg*)segcatcp->getObject(locseg);
            Int_t innersegindex=segcatcp->getIndex(locseg);

            HMatrixCategory* trkcandCat =(HMatrixCategory*)((HRecEvent*)(gHades->getCurrentEvent()))->getCategory(catMdcTrkCand);
            HIterator* itertrkcand=(HIterator *)((HCategory*)trkcandCat)->MakeIterator("native");
            HMdcTrkCand* trkcand;

            while ((trkcand=(HMdcTrkCand *)itertrkcand->Next())!=0)
            { // loop over HMdcTrkCand to find corresponding object
                Int_t inner  =trkcand->getSeg1Ind();
                Int_t outer  =trkcand->getSeg2Ind();
                if(inner==innersegindex)
                { // inner segment matchs witch HMdcTrkCand
                    for(Int_t ioseg=0;ioseg<2;ioseg++)
                    {
                        if(ioseg==0)                 segcp=(HMdcSeg*)segcatcp->getObject(inner);
                        else if(ioseg==1&&outer!=-1) segcp=(HMdcSeg*)segcatcp->getObject(outer);
                        else if(ioseg==1&&outer==-1) segcp=0;

                        if(segcp)
                        {
                            for(Int_t l=0;l<12;l++)
                            {
                                loccal[0]  = segcp->getSec();
                                Int_t ioseg= segcp->getIOSeg();

                                if(ioseg==0){
                                    (l<6)? loccal[1]=0 : loccal[1]=1;
                                }else {
                                    (l<6)? loccal[1]=2 : loccal[1]=3;
                                }
                                (l<6)? loccal[2]=l : loccal[2]=l-6;

                                Int_t nCell=segcp->getNCells(l);
                                for(Int_t i=0;i<nCell;i++)
                                {
                                    loccal[3]=segcp->getCell(l,i);

                                    HMatrixCategory* wireinfocatcp=(HMatrixCategory*)cpevent->getCategory(catMdcWireInfo);
                                    if(wireinfocatcp)
                                    {
                                        HMdcWireInfo* wireinfo=(HMdcWireInfo*)wireinfocatcp->getObject(loccal);
                                        if(!wireinfo)
                                        {
                                            wireinfo=(HMdcWireInfo *)wireinfocatcp->getSlot(loccal);
                                            wireinfo=new (wireinfo) HMdcWireInfo();
                                            wireinfo->setAddress(loccal[0],loccal[1],loccal[2],loccal[3]);
                                        }
                                        if(wireinfo)
                                        {
                                            Int_t segindex=segcatcp->getIndex(segcp);
                                            wireinfo->setListIndex(HMdcWireInfo::seg,segindex);
                                            wireinfo->setListIndex(HMdcWireInfo::kick,kickindex);

                                            Int_t hitindex=-1;
                                            if(loccal[1]==0 || loccal[1]==2 )
                                            {
                                                hitindex=segcp->getHitInd(0);
                                            } else {
                                                hitindex=segcp->getHitInd(1);
                                            }

                                            if(hitindex!=-1)
                                            {
                                                wireinfo->setListIndex(HMdcWireInfo::hit    ,hitindex);
                                                wireinfo->setListIndex(HMdcWireInfo::clusinf,hitindex);

                                                if(clusinflevel)
                                                {
                                                    HLinearCategory* clusinfcatcp =(HLinearCategory*)cpevent->getCategory(catMdcClusInf);

                                                    if(clusinfcatcp)
                                                    {
                                                        HMdcClusInf* clusinf=(HMdcClusInf*)clusinfcatcp->getObject(hitindex);
                                                        Int_t clusfitindex=clusinf->getClusFitIndex();

                                                        wireinfo->setListIndex(HMdcWireInfo::clusfit,clusfitindex);

                                                        if(clusfitlevel)
                                                        {
                                                            if(clusfitindex!=-1)
                                                            {
                                                                HLinearCategory* clusfitcatcp =(HLinearCategory*)cpevent->getCategory(catMdcClusFit);
                                                                if(clusfitcatcp)
                                                                {
                                                                    HMdcClusFit* clusfit=(HMdcClusFit*)clusfitcatcp->getObject(clusfitindex);
                                                                    if(clusfit)
                                                                    {
                                                                        Int_t indf=clusfit->getIndf();
                                                                        Int_t indl=clusfit->getIndl();

                                                                        if(wirefitlevel)
                                                                        {
                                                                            HLinearCategory* wirefitcatcp =(HLinearCategory*)cpevent->getCategory(catMdcWireFit);
                                                                            if(wirefitcatcp)
                                                                            {
                                                                                for(Int_t wirefitindex=indf; wirefitindex<=indl; wirefitindex++)
                                                                                {
                                                                                    HMdcWireFit* wirefitcp=(HMdcWireFit*)wirefitcatcp->getObject(wirefitindex);
                                                                                    wirefitcp->getAddress(sector,module,layer,cell);
                                                                                    if(sector==loccal[0] && module==loccal[1] && layer==loccal[2] && cell==loccal[3])
                                                                                    {
                                                                                        wireinfo->setListIndex(HMdcWireInfo::wirefit,wirefitindex);
                                                                                    }
                                                                                }
                                                                            } else {
                                                                                wireinfo->setListIndex(HMdcWireInfo::wirefit,-1);
                                                                                Warning("extract()","No HMdcWireFit category in output!");
                                                                            }
                                                                        }
                                                                    } else {
                                                                        wireinfo->setListIndex(HMdcWireInfo::clusfit,-1);
                                                                        wireinfo->setListIndex(HMdcWireInfo::wirefit,-1);
                                                                        Warning("extract()","Zero Pointer for HMdcClusFit retrieved!");
                                                                    }

                                                                } else {
                                                                    wireinfo->setListIndex(HMdcWireInfo::clusfit,-1);
                                                                    wireinfo->setListIndex(HMdcWireInfo::wirefit,-1);
                                                                    Warning("extract()","No HMdcClusFit category in output!");
                                                                }
                                                            } else {
                                                                wireinfo->setListIndex(HMdcWireInfo::clusfit,-1);
                                                                wireinfo->setListIndex(HMdcWireInfo::wirefit,-1);
                                                            }
                                                        }
                                                    } else {
                                                        wireinfo->setListIndex(HMdcWireInfo::clusinf,-1);
                                                        wireinfo->setListIndex(HMdcWireInfo::clusfit,-1);
                                                        wireinfo->setListIndex(HMdcWireInfo::wirefit,-1);
                                                        Warning("extract()","No HMdcClusInf category in output!");
                                                    }
                                                }
                                            } else {
                                                wireinfo->setListIndex(HMdcWireInfo::hit,-1);
                                                wireinfo->setListIndex(HMdcWireInfo::clusinf,-1);
                                                wireinfo->setListIndex(HMdcWireInfo::clusfit,-1);
                                                wireinfo->setListIndex(HMdcWireInfo::wirefit,-1);
                                            }
                                            wireinfo->setList();
                                        } else Warning("extract()","No slot for HMdcWireInfo obj!");
                                    } else Warning("extract()","No HMdcWireInfo category in output!");
                                }
                            }
                        }//  else Warning("extract()","ZERO pointer received for HMdcSeg!");

                    }
                }
            }
            itertrkcand->Reset();
            delete itertrkcand;
        }
    }

}
//----------------------------------------HIT--------------------------------------------------------

HMdcHitExtractor::HMdcHitExtractor(HRecEvent* cpev)
{
    if(cpev==0)
    {
	Error("HMdcHitExtractor()","ZERO pointer for event received!");
	exit(1);
    }
    cpevent=cpev;
    clusinfext=new HMdcClusInfExtractor(cpev);
    initLookup();
    resetCounters();
    lochit.set(3,0,0,0);
    clusinflevel=kFALSE;
}
HMdcHitExtractor::~HMdcHitExtractor()
{
   if(clusinfext) delete clusinfext;
}
void HMdcHitExtractor::setDataLevels(TString levels)
{
   if(levels.Contains("clusinf")==1) clusinflevel=kTRUE;
   clusinfext->setDataLevels(levels);
}
void  HMdcHitExtractor::extract(HMdcSeg* seg,HMdcSeg* segcp,Int_t index)
{
    HMatrixCategory* cathit  =(HMatrixCategory*)((HRecEvent*)(gHades->getCurrentEvent()))->getCategory(catMdcHit);

    if(cathit)
    {
        HMatrixCategory* cathitcp=(HMatrixCategory*)cpevent->getCategory(catMdcHit);
        if(cathitcp)
        {
            for(Int_t i=0;i<2;i++)
            {
                Int_t hitindex=seg->getHitInd(i);
                if(hitindex!=-1)
                {
                    if(!checkHitIndex(hitindex))
                    {
                        HMdcHit* hit=(HMdcHit*)cathit->getObject(hitindex);
                        hit->getSecMod(lochit[0],lochit[1]);
                        hitct[lochit[0]][lochit[1]]++;
                        lochit[2]=hitct[lochit[0]][lochit[1]];

                        HMdcHit* hitcp=(HMdcHit *)cathitcp->getSlot(lochit);
                        if (hitcp)
			{
			    hitlookup[hitctr++]=hitindex;

                            hitcp=new (hitcp) HMdcHit(*hit);
                            Int_t hitindexcp=cathitcp->getIndex(hitcp);
                            segcp->setHitInd(i,hitindexcp);
                            //-----------------------------CLUSINF---------------------------------------------------------
                            if(clusinflevel) clusinfext->extract(hit,hitcp,index);
                            //---------------------------------------------------------------------------------------------

                        } else Warning("extract()","no HMdcHit category in output!");
                    }
                }
            }
        } else Warning("extract()","No HMdcHit category in output!");
    } else Warning("extract()","No HMdcHit category in input!");
}
void HMdcHitExtractor::clear()
{
    clusinfext->clear();
    resetLookup();
    resetCounters();
}

//----------------------------------------CLUSFIT---------------------------------------------------

HMdcClusFitExtractor::HMdcClusFitExtractor(HRecEvent* cpev)
{
    if(cpev==0)
    {
	Error("HMdcClusFitExtractor()","ZERO pointer for event received!");
	exit(1);
    }
    cpevent=cpev;

    locclusfit.set(1,0);
    wirefitext=new HMdcWireFitExtractor(cpev);
    wirefitlevel=kFALSE;
    initLookups();
}
HMdcClusFitExtractor::~HMdcClusFitExtractor()
{
}
void HMdcClusFitExtractor::setDataLevels(TString levels)
{
  if(levels.Contains("wirefit")==1)wirefitlevel=kTRUE;
}

void  HMdcClusFitExtractor::extract(HMdcClusInf* clusinf,HMdcClusInf* clusinfcp,Int_t index)
{
    Int_t hitindex = clusinf->getClusFitIndex();
    if(hitindex!=-1)
    {
        Int_t hitindexcp=checkHitIndex(hitindex);
	if(hitindexcp==-1)
	{
	    HLinearCategory* catclusfit=(HLinearCategory*)((HRecEvent*)(gHades->getCurrentEvent()))->getCategory(catMdcClusFit);

	    if(catclusfit)
	    {
		Int_t clusfitindex=clusinf->getClusFitIndex();
		HMdcClusFit* clusfit=(HMdcClusFit*)catclusfit->getObject(clusfitindex);
		HLinearCategory* catclusfitcp=(HLinearCategory*)cpevent->getCategory(catMdcClusFit);

		if(catclusfitcp)
		{
		    if(clusfit)
		    {
                        Int_t clusfitcpindex;
                        HMdcClusFit* clusfitcp=(HMdcClusFit*)catclusfitcp->getNewSlot(locclusfit,&clusfitcpindex);

			if(clusfitcp)
			{
			    hitlookup  [hitctr]=clusfitindex;
			    hitcplookup[hitctr]=clusfitcpindex;
			    hitctr++;
			    clusfitcp=new (clusfitcp) HMdcClusFit(*clusfit);
			    clusinfcp->setClusFitIndex(clusfitcpindex);
			    //-------------------------WIREFIT----------------------------------------
			    wirefitext->extract(clusfit,clusfitcp,index);
			    //------------------------------------------------------------------------
			} else Warning("extract()","No HMdcClusFit slot in output!");
		    } else Warning("extract()","No HMdcClusFit object in input!");
		} else Warning("extract()","No HMdcClusFit category in output!");
	    } else Warning("extract()","No HMdcClusFit category in input!");
	} else {
	    clusinfcp->setClusFitIndex(hitindexcp);
	}
    }
}
void HMdcClusFitExtractor::clear()
{
    wirefitext->clear();
    resetLookups();
}

//----------------------------------------WIREFIT---------------------------------------------------

HMdcWireFitExtractor::HMdcWireFitExtractor(HRecEvent* cpev)
{
    if(cpev==0)
    {
	Error("HMdcWireFitExtractor()","ZERO pointer for event received!");
	exit(1);
    }
    cpevent=cpev;
    locwirefit.set(1,0);
}
HMdcWireFitExtractor::~HMdcWireFitExtractor()
{
}
void  HMdcWireFitExtractor::extract(HMdcClusFit* clusfit,HMdcClusFit* clusfitcp,Int_t index)
{
    HLinearCategory* catwirefitcp=(HLinearCategory*)cpevent->getCategory(catMdcWireFit);

    HLocation loccal;
    loccal.set(4,0,0,0,0);

    if(catwirefitcp)
    {
        HLinearCategory* catwirefit=(HLinearCategory*)((HRecEvent*)(gHades->getCurrentEvent()))->getCategory(catMdcWireFit);
        HMatrixCategory* catcal1   =(HMatrixCategory*)((HRecEvent*)(gHades->getCurrentEvent()))->getCategory(catMdcCal1);
        HMatrixCategory* catcal1cp =(HMatrixCategory*)cpevent->getCategory(catMdcCal1);

        if(catwirefit)
        {
            Int_t indf=clusfit->getIndf();
            Int_t indl=clusfit->getIndl();
            Int_t first=-1,last=-1;
            Int_t wirefitindex=-1;
            for(Int_t ind=indf; ind<=indl; ind++)
            {
                HMdcWireFit* wirefitcp=(HMdcWireFit*)catwirefitcp->getNewSlot(locwirefit,&wirefitindex);
                if(wirefitcp)
                {
                    if(ind==indf)first=wirefitindex;
                    last=wirefitindex;
                    HMdcWireFit* wirefit  =(HMdcWireFit*)catwirefit->getObject(ind);
                    if(wirefitcp)
                    {
                        wirefitcp=new (wirefitcp) HMdcWireFit(*wirefit);
                    } else Warning("extract()","No HMdcWireFit object in input!");

                    // copying the the cal1 objects missing in HMdcSegment
                    if(catcal1&&catcal1cp)
                    {
                        wirefit->getAddress(loccal[0],loccal[1],loccal[2],loccal[3]);

                        if(((HMdcCal1 *)catcal1cp->getObject(loccal))==0)
                        {
                            HMdcCal1* cal1=(HMdcCal1 *)catcal1->getObject(loccal);

                            if(cal1)
                            {
                                HMdcCal1* cal1cp=(HMdcCal1 *)catcal1cp->getSlot(loccal);
                                if (cal1cp)
                                {
                                    cal1cp=new (cal1cp) HMdcCal1(*cal1);
                                } else Warning("extract()","No slot for HMdcCal1 obj in output!");
                            } else Warning("extract()","HMdcCal1 obj missing in input!");
                        } //else Warning("extract()","No HMdcCal1 obj!");
                    }
                } else Warning("extract()","No HMdcWireFit slot in output!");
            }
            clusfitcp->setIndf(first);
            clusfitcp->setIndl(last);
        } else Warning("extract()","No HMdcWireFit category in input!");
    } else Warning("extract()","No HMdcWireFit category in output!");
}
void HMdcWireFitExtractor::clear()
{
}

//----------------------------------------CLUSINF---------------------------------------------------

HMdcClusInfExtractor::HMdcClusInfExtractor(HRecEvent* cpev)
{
    if(cpev==0)
    {
	Error("HMdcClusInfExtractor()","ZERO pointer for event received!");
	exit(1);
    }
    cpevent=cpev;
    clusfitext=new HMdcClusFitExtractor(cpev);
    locclusinf.set(1,0);
    clusfitlevel=kFALSE;
    clusterlevel=kFALSE;
}
HMdcClusInfExtractor::~HMdcClusInfExtractor()
{
    if(clusfitext) delete clusfitext;
}
void HMdcClusInfExtractor::setDataLevels(TString levels)
{
   if(levels.Contains("clusfit")==1)clusfitlevel=kTRUE;
   if(levels.Contains("cluster")==1)clusterlevel=kTRUE;
   clusfitext->setDataLevels(levels);
}
void  HMdcClusInfExtractor::extract(HMdcHit* hit,HMdcHit* hitcp,Int_t index)
{
    Int_t clusinfindex=0;

    HLinearCategory* catclusinf  =(HLinearCategory*)((HRecEvent*)(gHades->getCurrentEvent()))->getCategory(catMdcClusInf);

    if(catclusinf)
    {
        HMdcClusInf* clusinf=(HMdcClusInf*)catclusinf->getObject(hit->getClusInfIndex());
        HLinearCategory* catclusinfcp=(HLinearCategory*)cpevent->getCategory(catMdcClusInf);

        if(catclusinfcp)
        {
            if(clusinf)
            {
                HMdcClusInf* clusinfcp=(HMdcClusInf*)catclusinfcp->getNewSlot(locclusinf,&clusinfindex);
                if(clusinfcp)
                {
                    clusinfcp=new (clusinfcp) HMdcClusInf(*clusinf);
                    hitcp->setClusInfIndex(clusinfindex);

                    //-------------------------CLUSFIT--------------------------------------
		    if(clusfitlevel) clusfitext->extract(clusinf,clusinfcp,index);
                    //----------------------------------------------------------------------

                } else Warning("extract()","No HMdcClusInf slot in output!");
            } else Warning("extract()","No HMdcClusInf object in input!");
        } else Warning("extract()","No HMdcClusInf category in output!");
    } else Warning("extract()","No HMdcClusInf category in input!");
}
void HMdcClusInfExtractor::clear()
{
    clusfitext->clear();
}

//----------------------------------------SEGMENT---------------------------------------------------
HMdcSegExtractor::HMdcSegExtractor(HRecEvent* cpev)
{
    if(cpev==0)
    {
	Error("HMdcSegExtractor()","ZERO pointer for event received!");
	exit(1);
    }
    cpevent=cpev;
    locseg.set(3,0,0,0);
    lockick.set(2,0,0);
    hitext=new HMdcHitExtractor(cpev);
    initLookup();
    resetCounters();
    counter=0;
    cal1ext=new HMdcCal1Extractor(cpev);
    callevel =kFALSE;
    hitlevel =kFALSE;
    kicklevel=kFALSE;
}
HMdcSegExtractor::~HMdcSegExtractor()
{
   if(hitext) delete hitext;
   if(cal1ext) delete cal1ext;
}
void HMdcSegExtractor::setDataLevels(TString levels)
{
    if(levels.Contains("cal")==1) callevel =kTRUE;
    if(levels.Contains("hit")==1) hitlevel =kTRUE;
    if(levels.Contains("kick")==1)kicklevel=kTRUE;
    hitext->setDataLevels(levels);
}
void  HMdcSegExtractor::extract(HKickTrack* kick,HMdcSeg* seg,Int_t index,Int_t& segcpindex)
{
    HMatrixCategory* kickCatcp =(HMatrixCategory*)cpevent->getCategory(catKickTrack);
    HMatrixCategory* segCat    =(HMatrixCategory*)((HRecEvent*)(gHades->getCurrentEvent()))->getCategory(catMdcSeg);
    if(segCat)
    {
        HMatrixCategory* segCatcp=(HMatrixCategory*)cpevent->getCategory(catMdcSeg);
        if(segCatcp)
        {
            locseg[0]=seg->getSec();
            locseg[1]=seg->getIOSeg();

            segct[locseg[0]][locseg[1]]++;
            locseg[2]=segct[locseg[0]][locseg[1]];
            Int_t segindex=segCat->getIndex(seg);
            if(checkSegIndex(segindex)==-1)
            {
		HMdcSeg* segcp=(HMdcSeg *)segCatcp->getSlot(locseg);

		if (segcp)
                {
		    segcp=new (segcp) HMdcSeg(*seg);

		    segcplookup[segctr]=segCatcp->getIndex(segcp);
		    seglookup  [segctr]=segindex;
                    segcpindex=segcplookup[segctr];
                    segctr++;

		    //kickct[locseg[0]]++;

		    //-------------------------------CAL1-----------------------------
		    if(callevel) cal1ext->extract(seg,index);
                    //-------------------------------HIT------------------------------
                    if(hitlevel) hitext->extract(seg,segcp,index);
		    //-------------------------------KICK------------------------------

		    if(kicklevel&&locseg[1]==0)
		    { // only for inner segment
			if(kickCatcp)
			{
			    if(kick)
			    {
				lockick[0]=kick->getSector();
                                Int_t indkick;
				HKickTrack* kickcp=(HKickTrack *)kickCatcp->getSlot(lockick,&indkick);
				if(kickcp)
				{

				    kickct[locseg[0]]++;
				    kickcp=new (kickcp) HKickTrack(*kick,1);
				    kickcp->setSegment(kickct[locseg[0]]);
				} else Warning("extract()","No slot for HKickTrack object in output!");
			    } else Warning("extract()","Zero pointer for HKickTrack object in input!");
			}
		    }
		    //----------------------------------------------------------------


		} else Warning("extract()","No slot for HMdcSeg object in output!");
            } else Warning("extract()","Segment index already exixts!");
        } else Warning("extract()","No HMdcSeg category in output!");
    } else Warning("extract()","No HMdcSeg category in input!");

}
void  HMdcSegExtractor::extract(HMdcSeg* seg,Int_t index)
{
    HMatrixCategory* segCat    =(HMatrixCategory*)((HRecEvent*)(gHades->getCurrentEvent()))->getCategory(catMdcSeg);
    if(segCat)
    {
        HMatrixCategory* segCatcp=(HMatrixCategory*)cpevent->getCategory(catMdcSeg);
        if(segCatcp)
        {
            locseg[0]=seg->getSec();
            locseg[1]=seg->getIOSeg();

            segct[locseg[0]][locseg[1]]++;
            locseg[2]=segct[locseg[0]][locseg[1]];
            Int_t segindex=segCat->getIndex(seg);
            if(checkSegIndex(segindex)==-1)
            {
		HMdcSeg* segcp=(HMdcSeg *)segCatcp->getSlot(locseg);

		if (segcp)
                {
		    segcp=new (segcp) HMdcSeg(*seg);

		    segcplookup[segctr]=segCatcp->getIndex(segcp);
		    seglookup  [segctr]=segindex;
                    segctr++;

		    //-------------------------------CAL1-----------------------------
		    if(callevel) cal1ext->extract(seg,index);
                    //-------------------------------HIT------------------------------
                    if(hitlevel) hitext->extract(seg,segcp,index);
		    //-------------------------------KICK------------------------------
		} else Warning("extract()","No slot for HMdcSeg object in output!");
            } else Warning("extract()","Segment index already exixts!");
        } else Warning("extract()","No HMdcSeg category in output!");
    } else Warning("extract()","No HMdcSeg category in input!");

}
void HMdcSegExtractor::clear()
{
  resetLookup();
  resetCounters();
  hitext->clear();
  cal1ext->clear();
}
//----------------------------------------TRKCAND---------------------------------------------------
HMdcTrkCandExtractor::HMdcTrkCandExtractor(HRecEvent* cpev)
{
    if(cpev==0)
    {
	Error("HMdcTrkCandExtractor()","ZERO pointer for event received!");
	exit(1);
    }
    cpevent=cpev;
    loctrkcand.set(2,0,0);
    segext=new HMdcSegExtractor(cpev);
    initLookup();
    resetCounters();
    counter=0;
}
HMdcTrkCandExtractor::~HMdcTrkCandExtractor()
{
   if(segext) delete segext;
}
void HMdcTrkCandExtractor::setDataLevels(TString levels)
{
    segext->setDataLevels(levels);
}
void  HMdcTrkCandExtractor::extract(HKickTrack* kick,HMdcTrkCand* trkcand,Int_t index)
{
    HMatrixCategory* trkcandCat=(HMatrixCategory*)((HRecEvent*)(gHades->getCurrentEvent()))->getCategory(catMdcTrkCand);
    HMatrixCategory* segCat    =(HMatrixCategory*)((HRecEvent*)(gHades->getCurrentEvent()))->getCategory(catMdcSeg);
    if(trkcandCat)
    {
        HMatrixCategory* trkcandCatcp=(HMatrixCategory*)cpevent->getCategory(catMdcTrkCand);

        if(trkcandCatcp)
        {

            loctrkcand[0]=trkcand->getSec();
            trkcandct[loctrkcand[0]]++;
            loctrkcand[1]=trkcandct[loctrkcand[0]];

            Int_t trkcandindex=trkcandCat->getIndex(trkcand);
            if(checkTrkCandIndex(trkcandindex)==-1)
            {
		HMdcTrkCand* trkcandcp=(HMdcTrkCand *)trkcandCatcp->getSlot(loctrkcand);

		if (trkcandcp)
                {
		    trkcandcp=new (trkcandcp) HMdcTrkCand(*trkcand);

		    trkcandcplookup[trkcandctr]=trkcandCatcp->getIndex(trkcandcp);
		    trkcandlookup  [trkcandctr]=trkcandindex;
		    trkcandctr++;

                    Int_t inner  =trkcand->getSeg1Ind();
                    Int_t outer  =trkcand->getSeg2Ind();

                    HMdcSeg* seginner=0;
                    HMdcSeg* segouter=0;

                    Int_t indexseg=-1;

		    if(inner!=-1)
                    {
                        seginner=(HMdcSeg*)segCat->getObject(inner);
                        segext->extract(kick,seginner,index,indexseg);
                        trkcandcp->setSeg1Ind(indexseg);
                    }
                    if(outer!=-1)
                    {
                        segouter=(HMdcSeg*)segCat->getObject(outer);
                        segext->extract(kick,segouter,index,indexseg);
                        trkcandcp->setSeg2Ind(indexseg);
                    }


                } else Warning("extract()","No slot for HMdcTrkCand object in output!");
            } else Warning("extract()","TrkCand index already exixts!");
        } else Warning("extract()","No HMdcTrkCand category in output!");
    } else Warning("extract()","No HMdcTrkCand category in input!");

}
void HMdcTrkCandExtractor::clear()
{
  resetLookup();
  resetCounters();
  segext->clear();
}

