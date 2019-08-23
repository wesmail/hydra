
#include "hmdcfunc1.h"
#include "hpiddilepton.h"
#include "hpidparticle.h"
#include "hpidparticlesim.h"
#include "hpidtrackcand.h"
#include "hpidcandidate.h"
#include "hpiddilepton.h"
#include "hpidfl.h"
#include "hmdcseg.h"
#include "hmdcsegsim.h"
#include "hmdchit.h"
#include "hmdchitsim.h"
#include "hmdcclusinf.h"
#include "hmdcclusinfsim.h"

#include "hmdcdef.h"
#include "hmdctrackddef.h"
#include "richdef.h"
#include "hgeantdef.h"

#include "hrichhit.h"
#include "hrichhitsim.h"
//#include "hshowerhittof.h"
//#include "htofhit.h"
#include "hgeantkine.h"
#include "hgeantmdc.h"
#include "hgeantshower.h"
#include "hgeanttof.h"
#include "hgeantrich.h"
#include "hiterator.h"
//#include "hgeomvector.h"
#include "TArrayI.h"
#include "TVector3.h"

#include "hpidcpparam.h"

Bool_t HMdcFunc1::initCategoryPointers() {

    // Init the the category pointers for the given event in Hades.
    // Needs to called before one can retrieve objects

    fMdcSegCat=gHades->getCurrentEvent()->getCategory(catMdcSeg);
    if (!fMdcSegCat) {
	::Error("initCategories()","There is no catMdcSeg !");
	return kFALSE;
    }

    fRichHitCat=gHades->getCurrentEvent()->getCategory(catRichHit);
    if (!fRichHitCat) {
	::Error("initCategories()","There is no catRichHit !");
	return kFALSE;
    }


    fMdcHitCat=gHades->getCurrentEvent()->getCategory(catMdcHit);
    if (!fMdcHitCat) {
	::Error("initCategories()","There is no catMdcHit !");
	return kFALSE;
    }

    fClusInf=gHades->getCurrentEvent()->getCategory(catMdcClusInf);
    if(!fClusInf) {
	::Error("initCategories()","There is no catMdcClusInf !");
	return kFALSE;
    }

    fGeantKineCat=gHades->getCurrentEvent()->getCategory(catGeantKine);
    if (!fGeantKineCat) {
	::Error("initCategories()","There is no catGeantKine !");
    }
    if(fGeantKineCat)
    {
	// simulation or embedding !
	fGeantMdcCat=gHades->getCurrentEvent()->getCategory(catMdcGeantRaw);
	if (!fGeantMdcCat) {
	    ::Error("initCategories()","There is no catMdcGeantRaw !");
	    return kFALSE;
	}

	fGeantTofCat=gHades->getCurrentEvent()->getCategory(catTofGeantRaw);
	if (!fGeantTofCat) {
	    ::Error("initCategories()","There is no catTofGeantRaw !");
	    return kFALSE;
	}

	fGeantShowCat=gHades->getCurrentEvent()->getCategory(catShowerGeantRaw);
	if (!fGeantShowCat) {
	    ::Error("initCategories()","There is no catShowerGeantRaw !");
	    return kFALSE;
	}

	fGeantRichMirrorCat=gHades->getCurrentEvent()->getCategory(catRichGeantRaw+2);
	if (!fGeantRichMirrorCat) {
	    ::Error("initCategories()","There is no catRichGeantRaw+2 !");
	    return kFALSE;
	}


    }
    return kTRUE;
}

HMdcSeg*  HMdcFunc1::getMdcSegFromPidTrackCand(HPidTrackCand* cand)
{
    // Retrieve the Pointer of the inner segment corresponding
    // to the HPidTrackCand object. Return NULL if something wrong
    if(cand)
    {
	HPidHitData* hitdata = cand->getHitData();
	if(hitdata)
	{
	    HMdcSeg* seg=(HMdcSeg*)fMdcSegCat->getObject(hitdata->getIndInnerSeg());
	    if(seg){ return seg; }
	    else
	    {
		::Error("getMdcSegFromPidTrackCand()","Zero pointer for MdcSeg retrieved!");
		return 0;
	    }
	}
	else
	{
	    ::Error("getMdcSegFromPidTrackCand()","Zero pointer for PidHitData retrieved!");
	    return 0;
	}
    }
    else
    {
	::Error("getMdcSegFromPidTrackCand()","Zero pointer for PidTrackCand retrieved!");
	return 0;
    }
    return 0;
}
HRichHit*  HMdcFunc1::getRichHitFromPidTrackCand(HPidTrackCand* cand)
{
    // Retrieve the Pointer of the RichHit corresponding
    // to the HPidTrackCand object. Return NULL if something wrong
   if(cand)
    {
	HPidHitData* hitdata = cand->getHitData();
	if(hitdata)
	{
	    HRichHit* rich=(HRichHit*)fMdcSegCat->getObject(hitdata->getIndRICH());
	    if(rich){ return rich; }
	    else
	    {
		::Error("getRichHitFromPidTrackCand()","Zero pointer for RichHit retrieved!");
		return 0;
	    }
	}
	else
	{
	    ::Error("getRichHitFromPidTrackCand()","Zero pointer for PidHitData retrieved!");
	    return 0;
	}
    }
    else
    {
	::Error("getRichHitFromPidTrackCand()","Zero pointer for PidTrackCand retrieved!");
	return 0;
    }
    return 0;
}

Int_t  HMdcFunc1::getMdcClsSize(HPidParticle* part,Int_t module)
{
    // return the clustersize from HMdcClusInf for the
    // inner segment of a HPidParticle.
    // module == 0 -> first module in segment
    // module == 1 -> second module in segment
    // return 0 if something wrong

    HMdcSeg*        pMdcSeg      =NULL;
    HPidTrackCand*  pPidTrackCand=NULL;
    Int_t cls                    =0;
    if(part)
    {
	pPidTrackCand=getPidTrackCand(part);
	if(pPidTrackCand)
	{
	    pMdcSeg=getMdcSegFromPidTrackCand(pPidTrackCand);
	    if(pMdcSeg)
	    {
		cls=getMdcClsSize(pMdcSeg,module);
	    }
	}
    }
    return cls;
}

Int_t  HMdcFunc1::getMdcNWires(HPidParticle* part,Int_t module)
{
    // return the number of wires from HMdcClusInf for the
    // inner segment of a HPidParticle.
    // module == 0 -> first module in segment
    // module == 1 -> second module in segment
    // return 0 if something wrong

    HMdcSeg*        pMdcSeg      =NULL;
    HPidTrackCand*  pPidTrackCand=NULL;
    Int_t nwires                 =0;
    if(part)
    {
	pPidTrackCand=getPidTrackCand(part);
	if(pPidTrackCand)
	{
	    pMdcSeg=getMdcSegFromPidTrackCand(pPidTrackCand);
	    if(pMdcSeg)
	    {
		nwires=getMdcNWires(pMdcSeg,module);
	    }
	}
    }
    return nwires;
}

Int_t  HMdcFunc1::getMdcLevelCls(HPidParticle* part,Int_t module)
{
    // return the level of cluster finding from HMdcClusInf for the
    // inner segment of a HPidParticle.
    // module == 0 -> first module in segment
    // module == 1 -> second module in segment
    // return 0 if something wrong

    HMdcSeg*        pMdcSeg      =NULL;
    HPidTrackCand*  pPidTrackCand=NULL;
    Int_t level                  =0;
    if(part)
    {
	pPidTrackCand=getPidTrackCand(part);
	if(pPidTrackCand)
	{
	    pMdcSeg=getMdcSegFromPidTrackCand(pPidTrackCand);
	    if(pMdcSeg)
	    {
		level=getMdcLevelCls(pMdcSeg,module);
	    }
	}
    }
    return level;
}

Int_t  HMdcFunc1::getIntCharge(HPidParticle* part)
{
    // return the integrated charge from HRichHit for the
    // of a HPidParticle.
    // return 0 if something wrong

    HRichHit*       pRichHit     =NULL;
    HPidTrackCand*  pPidTrackCand=NULL;
    Int_t int_charge             =0;
    if(part)
    {
	pPidTrackCand=getPidTrackCand(part);
	if(pPidTrackCand)
	{
	    pRichHit=getRichHitFromPidTrackCand(pPidTrackCand);
	    if(pRichHit) {
		int_charge=pRichHit->getRingAmplitude();
	    }
	}
    }
    return int_charge;
}


Int_t  HMdcFunc1::getMdcClsSize(HMdcSeg* seg,Int_t module)
{
    // return the clustersize from HMdcClusInf for the
    // segment.
    // module == 0 -> first module in segment
    // module == 1 -> second module in segment
    // return 0 if something wrong

    Int_t    cls=0;
    HMdcHit* hit=0;
    if(seg)
    {
	hit        =getMdcHit(seg,module);
	if(hit) cls=getMdcClsSize(hit);
    }
    return cls;
}

Int_t  HMdcFunc1::getMdcNWires(HMdcSeg* seg,Int_t module)
{
    // return the number of wires from HMdcClusInf for the
    // segment.
    // module == 0 -> first module in segment
    // module == 1 -> second module in segment
    // return 0 if something wrong

    Int_t nwires=0;
    HMdcHit* hit=0;
    if(seg)
    {
	hit           =getMdcHit(seg,module);
	if(hit) nwires=getMdcNWires(hit);
    }
    return nwires;
}

Int_t  HMdcFunc1::getMdcLevelCls(HMdcSeg* seg,Int_t module)
{
    // return the level of cluster finding from HMdcClusInf for the
    // segment.
    // module == 0 -> first module in segment
    // module == 1 -> second module in segment
    // return 0 if something wrong

    Int_t  level=0;
    HMdcHit* hit=0;
    if(seg)
    {
	hit          =getMdcHit(seg,module);
	if(hit) level=getMdcLevelCls(hit);
    }
    return level;
}


HPidTrackCand* HMdcFunc1::getPidTrackCand(HPidParticle* part)
{
    // return HPidTrackCand form an HPidParticle
    // using the indices HPidParticel->HPidCandidate->HPidTrackCand
    // return 0 if something wrong
    HPidCandidate* pidcand     =NULL;
    HPidTrackCand* pidtrackcand=NULL;

    if(part)
    {
	pidcand=part->getPidCandidate();
	if(pidcand)
	{
	    pidtrackcand=pidcand->getTrackCandidate();
	}
    }
    return pidtrackcand;
}

HMdcHit* HMdcFunc1::getMdcHit(HMdcSeg* seg,Int_t module)
{
    // return the HMfdcHit for the
    // segment.
    // module == 0 -> first module in segment
    // module == 1 -> second module in segment
    // return 0 if something wrong

    HMdcHit*  hit=NULL;
    Int_t hit_inx=-1;
    if(seg)
    {
	hit_inx=seg->getHitInd(module);
	if(hit_inx>-1)
	{
	    if(getMdcHitCat())
	    {
		hit=(HMdcHit *)getMdcHitCat()->getObject(hit_inx);
		return hit;
	    } else {
		::Error("getMdcHit()","******MdcHitCat is NULL********use HMdcFunc1::setMdcHitCat()");
	    }
	}
    }
    return hit;
}

Int_t HMdcFunc1::getMdcClsSize(HMdcHit* hit)
{
    // return the clustersize from HMdcClusInf for the
    // HMdcHit.
    // return -1 if something wrong

    Int_t clssize            =-1;
    Int_t clsindex           =-1;
    HMdcClusInf* pObj_clusinf=NULL;

    clsindex    =hit->getClusInfIndex();
    pObj_clusinf=getClusInfObj(clsindex);
    if(pObj_clusinf)
    {
	clssize=pObj_clusinf->getClusSize();
    }
    return clssize;
}

Int_t HMdcFunc1::getMdcNWires(HMdcHit* hit)
{
    // return the number of wires from HMdcClusInf for the
    // HMdcHit.
    // return -1 if something wrong

    Int_t nwires             =-1;
    Int_t clsindex           =-1;
    HMdcClusInf* pObj_clusinf=NULL;

    clsindex    =hit->getClusInfIndex();
    pObj_clusinf=getClusInfObj(clsindex);
    if(pObj_clusinf)
    {
	nwires=pObj_clusinf->getNDrTimes();
    }
    return nwires;
}
Int_t HMdcFunc1::getMdcLevelCls(HMdcHit* hit)
{
    // return the level of cluster finding from HMdcClusInf for the
    // HMdcHit.
    // return -1 if something wrong

    Int_t level              =-1;
    Int_t clsindex           =-1;
    HMdcClusInf* pObj_clusinf=NULL;

    clsindex    =hit->getClusInfIndex();
    pObj_clusinf=getClusInfObj(clsindex);
    if(pObj_clusinf)
    {
	level=pObj_clusinf->getLevelClFinding();
    }
    return level;
}

HMdcClusInf* HMdcFunc1::getClusInfObj(Int_t clsindex)
{
    //get ClusInfObj from category using index

    HMdcClusInf* pObj_clusinf=NULL;
    if(getClusInfCat())
    {
	pObj_clusinf=(HMdcClusInf* )getClusInfCat()->getObject(clsindex);
    } else {
	::Error("getClusInfObj()","******MdcClusInfCat is NULL********use HMdcFunc1::setMdcClusInfCat()");
    }
    return pObj_clusinf;
}

Int_t HMdcFunc1::calculateLevelBin(Int_t level)
{
    // calculates the bin of the the level parameter
    // access the corrections

    Int_t mdc_lev_bin =-1;
    if (level>=4&&level<=6) { mdc_lev_bin=(Int_t )level-4;}
    return mdc_lev_bin;
}

Int_t HMdcFunc1::calculateBin(Float_t mdc_angle,Int_t option)
{
    // calculate the the bin of the angle (option=0 ... theta; =1 ... phi)

    Int_t mdc_theta_bin =-9;
    Int_t mdc_phi_bin   =-9;
    Int_t theta_bin_size=10;
    Int_t phi_bin_size  =10;

    Float_t theta_min   =10.0;
    Float_t theta_max   =90.0;

    Float_t phi_min     =0.0;
    Float_t phi_max     =360.0;

    switch (option)
    {
    case 0:
	// case theta
	if(mdc_angle>=theta_min&&
	   mdc_angle<=theta_max)
	{
	    mdc_theta_bin=((Int_t)(mdc_angle-theta_min))/theta_bin_size;
	    return mdc_theta_bin;
	}
	else
	{
	    ::Error("calculateBin()","theta out of range");
	    return -2;
	}
    case 1:
	if(mdc_angle>=phi_min&&
	   mdc_angle<=phi_max)
	{
	    mdc_phi_bin=(((Int_t) mdc_angle)%60)/phi_bin_size;
	    if(mdc_phi_bin>2) { mdc_phi_bin=5-mdc_phi_bin ;}//e.g. 10-10 and 50-60 is bin 0
	    return mdc_phi_bin;
	}
	else
	{
	    ::Error("calculateBin()","phi out of range");
	    return -2;
	}

    default:
	::Error("calculateBin()","Third argument should be 0-1 for theta-phi angle\n");
	return -1;
    }
}


Float_t HMdcFunc1::getNormalMdcPhi(Int_t iSector, Float_t fPhiMdc)
{
    // Convert MDC's phi angle to the coordinate system used
    // in other detectors.  input mdc phi in rad and sector
    // coordinate system => output in deg and hades coordinate system

    Float_t fPhi = TMath::RadToDeg() * fPhiMdc;

    switch(iSector)
    {
    case 0:
	break;

    case 1:
    case 2:
    case 3:
    case 4:
	fPhi += 60.0f * iSector;
	break;

    default:
	fPhi -= 60.0f;
	break;
    }

    return fPhi;
}

// -----------------------------------------------------------------------------

Float_t HMdcFunc1::getNormalMdcTheta(Float_t fThetaMdc)
{
    // Convert MDC's theta angle to the coordinate system
    // used in other detectors
    return  TMath::RadToDeg() * fThetaMdc;
}

/*
 Int_t  HMdcFunc1::getGeantId(HRichHitSim * pRichHitSim,HKickTrackSim * pKickTrack){
 //this calculated GeantId in a way I consider it correct
 //return 2,3 or 102 and 103 if e+ e-
 //      -2 ring-pi
 //      -3 ring-p
 //      -5 ring-another lepton
 //      -4 ring-mdc corr ele-meta not
 //      -1 ring-non sence in meta
 HGeantKine *kine=NULL;
 Int_t richTr1=-1;
 Int_t richTr2=-1;
 Int_t richTr3 =-1;
 Int_t iTrack=-1, iId=-1;
 Int_t flag_richmdcele=0;
 Int_t flag_missinmdc=0;



 if(pKickTrack!=NULL&&pRichHitSim!=NULL) {
 if(pRichHitSim->weigTrack1>0) richTr1=pRichHitSim->track1;
 if(pRichHitSim->weigTrack2>0) richTr2=pRichHitSim->track2;
 if(pRichHitSim->weigTrack3>0) richTr3=pRichHitSim->track3;

 //printf("THIS TRACK: RICH %i %i %i MDC (num %i) %i %i %i %i %i META (num %i) %i %i %i\n",richTr1,richTr2,richTr3,pKickTrack->getNMdcTracks(),pKickTrack->getMdcTrack(0),pKickTrack->getMdcTrack(1),pKickTrack->getMdcTrack(2),pKickTrack->getMdcTrack(3),pKickTrack->getMdcTrack(4),pKickTrack->getNMetaTracks(),pKickTrack->getMetaTrack(0),pKickTrack->getMetaTrack(1),pKickTrack->getMetaTrack(2));

 flag_richmdcele=0;
 for (Int_t j=0;j<pKickTrack->getNMdcTracks();j++){
 //go over mdc
 iId=-1;

 if(pKickTrack->getMdcTrack(j)>0&&pKickTrack->getMdcTrack(j)<10000){

 for (Int_t k=0;k<pKickTrack->getNMetaTracks();k++){
 //iId=-1;
 //completely new
 if(pKickTrack->getMdcTrack(j)==pKickTrack->getMetaTrack(k)){
 //common MDC-META
 kine=HPidFL::getGeantKineObjFromTrkNbFast(pKickTrack->getMdcTrack(j));
 kine->getParticle(iTrack, iId);
 if((pKickTrack->getMdcTrack(j)==richTr1)||(pKickTrack->getMdcTrack(j)==richTr2)||(pKickTrack->getMdcTrack(j)==richTr3)){
 //common RING-MDC-META
 //iId=-1;
 //kine=HPidFL::getGeantKineObjFromTrkNbFast(pKickTrack->getMdcTrack(j));;
 //kine->getParticle(iTrack, iId);

 //printf("Rich-Meta-Mdc common track=%i part=%i\n",pKickTrack->getMdcTrack(j),iId);
 //printf("return %i\n",iId);
 return  iId;
 }

 if(iId==9||iId==8) {
 //printf("Ring corr Meta-Mdc common track=%i part=%i\n",pKickTrack->getMdcTrack(j),iId);
 //printf("return -2 (pion)\n");
 return -2;
 }

 if(iId==14){
 // printf("Ring corr Meta-Mdc common track=%i part=%i\n",pKickTrack->getMdcTrack(j),iId);
 //printf("return -3 (proton)\n");
 return -3;
 }

 if(iId==3||iId==2){

 // printf("Ring corr Meta-Mdc common track=%i part=%i\n",pKickTrack->getMdcTrack(j),iId);
 //printf("return -5 (another lepton)\n");
 return -5;
 }
 }

 if((pKickTrack->getMdcTrack(j)==richTr1)||(pKickTrack->getMdcTrack(j)==richTr2)||(pKickTrack->getMdcTrack(j)==richTr3)){
 //common RING-MDC but not META because otherwise it is already return before

 kine=HPidFL::getGeantKineObjFromTrkNbFast(pKickTrack->getMdcTrack(j));;
 kine->getParticle(iTrack, iId);
 if(iId==2||iId==3){
 //common RING-MDC electron (but not META)
 // printf("common RING-MDC lepton (but not META) tr=%i \n",pKickTrack->getMdcTrack(j));
 //printf("set iId=-4\n");
 flag_richmdcele=1;
 } else {
 printf("Strange RING-MDC is not lepton\n");
 }
 }
 //one has to treat separately case that in MDC I do not see a second pair from conv but in META yes
 if((pKickTrack->getMetaTrack(k)==richTr1)||(pKickTrack->getMetaTrack(k)==richTr2)||(pKickTrack->getMetaTrack(k)==richTr3)){
 for (Int_t l=0;l<pKickTrack->getNMdcTracks();l++){
 if(TMath::Abs(pKickTrack->getMdcTrack(l)-pKickTrack->getMetaTrack(k))==1){


 //	printf("common RING-MDC lepton1 ,common RING-META lepton2 from pair  tr=%i in mdc lost \n",pKickTrack->getMetaTrack(l));
 kine=HPidFL::getGeantKineObjFromTrkNbFast(pKickTrack->getMetaTrack(k));
 kine->getParticle(iTrack, iId);


 //printf("common miss mdc track iTrack=%i set flag_missinmdc=%i\n ",pKickTrack->getMetaTrack(l),(100+iId));
 //return (100+iId);
 //I do not return because it could still be double-double-single case
 flag_missinmdc=(100+iId);

 }
 }
 }
 }
 }
 }
 //printf("THIS TRACK: RICH %i %i %i MDC (num %i) %i %i %i %i %i META (num %i) %i %i %i\n",richTr1,richTr2,richTr3,pKickTrack->getNMdcTracks(),pKickTrack->getMdcTrack(0),pKickTrack->getMdcTrack(1),pKickTrack->getMdcTrack(2),pKickTrack->getMdcTrack(3),pKickTrack->getMdcTrack(4),pKickTrack->getNMetaTracks(),pKickTrack->getMetaTrack(0),pKickTrack->getMetaTrack(1),pKickTrack->getMetaTrack(2));
 if(flag_missinmdc!=0){
 return flag_missinmdc;
 }

 if(flag_richmdcele==1){
 // printf("return %i\n",-4);
 return -4;
 }
 //printf("return %i\n",-1);
 return -1;

 }
 return 0;

 }
 */
/*
Float_t  HMdcFunc1::getKickAngle(HPidParticle* part)
{
    ::Warning("getKickAngle()","This function return hard wired numbers!");
    //return META-MDC theta diff
    //HMdcSeg * pMdcSeg=NULL;
    //HPidTrackCand * pPidTrackCand=NULL;
    Float_t meta_theta=-100.0;
    Float_t mdc_the=100.0;
    //  Int_t cls=0;
    // if(part) {
    //     pPidTrackCand=getPidTrackCand(part);
    //     if(pPidTrackCand){
    //       pMdcSeg=(HMdcSeg*)pPidTrackCand->getMdcSeg();
    //        mdc_the=getNormalMdcTheta(pMdcSeg->getTheta());


    //       if(pPidTrackCand->getSystem()==0){
    // 	((HShowerHitTof*) pPidTrackCand->getShowerHitTof())->getSphereCoord(dummy,dummy,meta_theta);
    //       } else {
    // 	((HTofHit*) pPidTrackCand->getTofHit())->getTheta(meta_theta);



    //       }




    //     }

    //   }
    return (meta_theta - mdc_the);

}
*/
HGeantKine* HMdcFunc1::getKineObj(Int_t pTrack)
{
    // return the HGenatKine object pointer
    // for a given track number. returns NULL
    // if something wrong

    if(fGeantKineCat!=NULL||pTrack<1)
    {
	return (HGeantKine*)(fGeantKineCat->getObject(pTrack-1));
    } else {
	::Error("getKineObj()","HMdcFunc1::getKineObj there is no fGeantKineCat defined please use HMdcFunc1::setGeantKineCat before\n");
	return NULL;
    }
}
Int_t HMdcFunc1::getGeantCommonTrack(HPidParticleSim* part)
{
    //this calculated CommonGeantTrack in a way I consider it is correct
    //return positive value for  e+ e- common track and:
    //      -2 ring-pi
    //      -3 ring-p
    //      -5 ring-another lepton
    //      -4 ring-mdc corr ele-meta not
    //      -1 ring-non sence in meta
    //      -10 richhit is null pointer
    HPidTrackCand* pPidTrackCand=NULL;
    HRichHitSim*   pRichHitSim  =NULL;

    if(part!=NULL)
    {
	const HPidGeantTrackSet* pidset=part->getGeantTrackSet();
	pPidTrackCand=getPidTrackCand(part);
	if(pPidTrackCand!=NULL)
	{
	    HGeantKine *kine     =NULL;
	    Int_t richTr1        = -1;
	    Int_t richTr2        = -1;
	    Int_t richTr3        = -1;
	    Int_t iTrack         = -1;
	    Int_t iId            = -1;
	    Int_t flag_richmdcele= 0;
	    Int_t flag_missinmdc = 0;

	    pRichHitSim=(HRichHitSim*) getRichHitFromPidTrackCand(pPidTrackCand);

	    if(pRichHitSim!=NULL)
	    {
		if(pRichHitSim->weigTrack1>0) richTr1=pRichHitSim->track1;
		if(pRichHitSim->weigTrack2>0) richTr2=pRichHitSim->track2;
		if(pRichHitSim->weigTrack3>0) richTr3=pRichHitSim->track3;


		flag_richmdcele=0;
		Int_t system   =(Int_t)part->getSystem();
		for(UInt_t j=0;j<pidset->getNInnerMdcTracks();j++)
		{
		    //go over mdc
		    iId=-1;
		    if(pidset->getInnerMdcTrack(j)>0)
		    {
			Int_t nmetatracks=0;
			if(system==0){
			    nmetatracks=pidset->getNShowerTracks();
			}
			else if (system==1){
			    nmetatracks=pidset->getNTOFTracks();
			}
			else
			{
			    ::Error("getGeantCommonTrack()","System is not 0 or 1!");
			    return -10;
			}
			for(Int_t k=0;k<nmetatracks;k++)
			{
			    //completely new
			    Int_t metatrack=-1;
			    (system==0)? metatrack=pidset->getShowerTrack(k) : metatrack=pidset->getTOFTrack(k);
			    if(pidset->getInnerMdcTrack(j)==metatrack)
			    {
				//common MDC-META
				kine=HPidFL::getGeantKineObjFromTrkNbFast(pidset->getInnerMdcTrack(j));
				kine->getParticle(iTrack, iId);
				if((pidset->getInnerMdcTrack(j)==richTr1)||
				   (pidset->getInnerMdcTrack(j)==richTr2)||
				   (pidset->getInnerMdcTrack(j)==richTr3))
				{
				    //common RING-MDC-META
				    return  pidset->getInnerMdcTrack(j);
				}

				if(iId==9||iId==8) { return -2; }
				if(iId==14)        { return -3; }
				if(iId==3||iId==2) { return -5; }
			    }

			    if((pidset->getInnerMdcTrack(j)==richTr1)||
			       (pidset->getInnerMdcTrack(j)==richTr2)||
			       (pidset->getInnerMdcTrack(j)==richTr3))
			    {
				//common RING-MDC but not META because otherwise it is already return before

				kine=HPidFL::getGeantKineObjFromTrkNbFast(pidset->getInnerMdcTrack(j));
				kine->getParticle(iTrack, iId);
				if(iId==2||iId==3){
				    //common RING-MDC electron (but not META)
				    flag_richmdcele=1;
				}
				else
				{
				    ::Error("getGeantCommonTrack()","Strange RING-MDC is not lepton");
				}
			    }
			    //one has to treat separately case that in MDC I do not see a second pair from conv but in META yes
			    if((metatrack==richTr1)||
			       (metatrack==richTr2)||
			       (metatrack==richTr3))
			    {
				for(UInt_t l=0;l<pidset->getNInnerMdcTracks();l++)
				{
				    if(TMath::Abs(pidset->getInnerMdcTrack(l)-metatrack)==1)
				    {
					kine=HPidFL::getGeantKineObjFromTrkNbFast(metatrack);
					kine->getParticle(iTrack, iId);
					//I do not return because it could still be double-double-single case
					// flag_missinmdc=(100+iId);
					flag_missinmdc=metatrack;
				    }
				}
			    }
			}
		    }
		}
		if(flag_missinmdc!=0) { return flag_missinmdc; }
		if(flag_richmdcele==1){ return -4; }
		return -1;
	    }
	}
    }
    return -10;
}
Bool_t HMdcFunc1::isCompt(HGeantKine* kine)
{
    // returns kTRUE if kine is a lepton
    // originating from gama->e- gama
    Int_t pTrack_par     =-1;
    Int_t pMed           =-1;
    Int_t pMech          =-1;
    Int_t pID_par        = 0;
    Int_t pTrack_particle=-1;
    Int_t pID_particle   = 0;

    if(isLepton(kine))
    {
	kine->getParticle(pTrack_particle,pID_particle);
	kine->getCreator(pTrack_par,pMed,pMech);

	if(pTrack_par!=0)
	{
	    getKineObj(pTrack_par)->getParticle(pTrack_par,pID_par);
	    //pMech==7 means that it was  compt gama->e- gama
	    if(pID_par==1&&pMech==7){ return kTRUE ; }
	}
    }
    return kFALSE;
}


Bool_t HMdcFunc1::isPi0Conv(HGeantKine* kine)
{
    // returns kTRUE if kine is a lepton
    // originating from pi0 ->gama->e+e-

    Int_t pTrack_par     =-1;
    Int_t pMed           =-1;
    Int_t pMech          =-1;
    Int_t pID_par        = 0;
    Int_t pTrack_par_par =-1;
    Int_t pMed_par       =-1;
    Int_t pMech_par      =-1;
    Int_t pID_par_par    =-1;
    Int_t pTrack_particle=-1;
    Int_t pID_particle   = 0;

    if(isLepton(kine))
    {
	kine->getParticle(pTrack_particle,pID_particle);
	kine->getCreator(pTrack_par,pMed,pMech);

	if(pTrack_par!=0)
	{
	    getKineObj(pTrack_par)->getParticle(pTrack_par,pID_par);
	    //pMech==6 means that it was pair creation gama->e+e-
	    if(pID_par==1&&pMech==6)
	    {
		getKineObj(pTrack_par)->getCreator(pTrack_par_par,pMed_par,pMech_par);
		if(pTrack_par_par!=0)
		{
		    getKineObj(pTrack_par_par)->getParticle(pTrack_par_par,pID_par_par);
		    if(pID_par_par==7){ return kTRUE ; }
		}
	    }
	}
    }
    return  kFALSE;
}

Bool_t HMdcFunc1::isConv(HGeantKine* kine)
{
    // returns kTRUE if kine is a lepton
    // originating from gama->e+e-
    Int_t pTrack_par     =-1;
    Int_t pMed           =-1;
    Int_t pMech          =-1;
    Int_t pID_par        = 0;
    Int_t pTrack_particle=-1;
    Int_t pID_particle   = 0;

    if(isLepton(kine))
    {
	kine->getParticle(pTrack_particle,pID_particle);
	kine->getCreator(pTrack_par,pMed,pMech);

	if(pTrack_par!=0)
	{
	    getKineObj(pTrack_par)->getParticle(pTrack_par,pID_par);
	    //pMech==6 means that it was pair creation gama->e+e-
	    if(pID_par==1&&pMech==6){ return kTRUE ; }
	}
    }
    return  kFALSE;
}

Bool_t HMdcFunc1::isPi0Dalitz(HGeantKine* kine)
{
    // returns kTRUE if kine is a lepton
    // originating from pi0

    Int_t pTrack_par     =-1;
    Int_t pMed           =-1;
    Int_t pMech          =-1;
    Int_t pID_par        = 0;
    Int_t pTrack_particle=-1;
    Int_t pID_particle   = 0;

    if(isLepton(kine))
    {
	kine->getParticle(pTrack_particle,pID_particle);
	kine->getCreator(pTrack_par,pMed,pMech);

	if(pTrack_par!=0)
	{
	    getKineObj(pTrack_par)->getParticle(pTrack_par,pID_par);
	    if(pID_par==7){ return kTRUE; }
	}
    }
    return  kFALSE;
}

Bool_t HMdcFunc1::isEtaDalitz(HGeantKine* kine)
{
    // returns kTRUE if kine is a lepton
    // originating from eta dalitz

    Int_t pTrack_par     =-1;
    Int_t pMed           =-1;
    Int_t pMech          =-1;
    Int_t pID_par        = 0;
    Int_t pTrack_particle=-1;
    Int_t pID_particle   = 0;

    if(isLepton(kine))
    {
	kine->getParticle(pTrack_particle,pID_particle);
	kine->getCreator(pTrack_par,pMed,pMech);

	if(pTrack_par!=0)
	{
	    getKineObj(pTrack_par)->getParticle(pTrack_par,pID_par);
	    if(pID_par==17){ return  kTRUE; }
	}
    }
    return  kFALSE;
}


Bool_t HMdcFunc1::isLepton(HGeantKine* kine)
{
    // returns kTRUE if kine is a lepton

    Int_t pTrack_particle=-1;
    Int_t pID_particle   = 0;
    kine->getParticle(pTrack_particle,pID_particle);
    if (pID_particle==2||pID_particle==3) return kTRUE;
    return  kFALSE;
}

Bool_t HMdcFunc1::isSingle(HMdcSegSim* seg)
{
    // The method operates only on simulation data.
    // Using the geant track numbers of the tracks
    // in mdcseg (5 tracks stored) decide whether
    // it is a single tracks from lepton or not
    // Check also whether it is inner segment. If not
    // returns false.
    // look only at inner segments :very important
    if(seg->getIOSeg()==0)
    {
	Int_t track_mul=seg->getNTracks();
	// it gives number of tracks (max) stored for this segment
	if(track_mul==1)
	{
	    // only segment that consist of one track can be a
	    // ideal single (MAYBE TO STRONG HAS TO BE RECONSIDER)
	    if(isLepton(getKineObj(seg->getTrack(0))))
	    {
		// this is segment that consist only from one
		// geant track that is an lepton => we call it SINGLE
		return kTRUE;
	    }
	}
    }
    return kFALSE;
}

Bool_t HMdcFunc1::isSingle(HMdcHitSim* hit)
{
    // returns kTRUE if hit has only one track
    // conribution and is a lepton

    Int_t track_mul             = 0;
    Int_t clsindex              =-1;
    HMdcClusInfSim *pObj_clusinf=NULL;

    clsindex=hit->getClusInfIndex();
    pObj_clusinf=(HMdcClusInfSim*) getClusInfObj(clsindex);
    if(pObj_clusinf)
    {
	track_mul = pObj_clusinf->getNTracks();
	if(track_mul==1)
	{
	    if(isLepton(getKineObj(pObj_clusinf->getTrack(0))))
	    {
		return kTRUE;
	    }
	}
    }
    return kFALSE;
}

Int_t HMdcFunc1::isSingleDouble(HMdcSegSim* seg,Int_t track,Int_t conv_mode)
{
    //this method have following return values
    // 0 ... normal single
    // 1 ... normal double
    // 2 ... double => because possition of pair
    //                 lepton to track in MdcGeant too
    //                 close to MdcHit possition
    //-1 ... nothing of previous
    // return value 2 was introduced because the lack of
    // complete propagation  of all contributing track numbers
    // from contributing wires to segment for fitted one.
    // This is in version v6_11

    Int_t track_in_pair=-1;

    if(track>0)
    {
	if(isDouble(seg,conv_mode)) return 1;

	if(isSingle(seg))
	{
	    // here I have check position of pair
	    // lepton => if too close than I consider
	    // it as double, because lost info after
	    // fitter remove some wires
	    track_in_pair=getPairTrack(track);
	    if(isMdcSegInGeant(seg,track_in_pair)) return 2;
	    return 0;
	}
    }
    return -1;
}



Bool_t HMdcFunc1::isDouble(HMdcSegSim* seg,Int_t conv_mode)
{
    // The method operates only on simulated data.
    // Using the geant track numbers of the tracks in
    // mdcseg (5 tracks stored) decide whether
    // it is a double tracks from lepton or not
    // conv_more =0 seg is DOUBLE if there are
    //            2 different leptons contributing to it
    // conv_more =1 seg is DOUBLE if there are 2 different
    //            leptons contributing to it and leptons
    // are conversion pair
    Int_t track_mul  = 0;
    Int_t pID2       = 0;
    Int_t pID1       =-1;
    Int_t pTrack_par1=-1;
    Int_t pTrack_par2=-2;
    Int_t dummy      =-1;
    //look only at inner segments :very important
    if(seg->getIOSeg()==0)
    {
	track_mul=seg->getNTracks();
	//it gives number of tracks (max) stored for this segment
	if(track_mul>1)
	{
	    //only segment that consist more than one track can be a ideal double
	    for(Int_t i=0;i<track_mul;i++)
	    {
		for(Int_t j=i+1;j<track_mul;j++)
		{
		    // check i-th track in seg
		    if(isLepton(getKineObj(seg->getTrack(i))))
		    {
			if(conv_mode==1)
			{
			    //in this mode I check whether it is conversion lepton
			    if(isPi0Conv(getKineObj(seg->getTrack(i))))
			    {
				getKineObj(seg->getTrack(i))->getParticle(dummy,pID1);
				getKineObj(seg->getTrack(i))->getCreator(pTrack_par1,dummy,dummy);
			    }
			}
			else
			{
			    getKineObj(seg->getTrack(i))->getParticle(dummy,pID1);
			    getKineObj(seg->getTrack(i))->getCreator(pTrack_par1,dummy,dummy);
			}
		    }

		    // check j-th track in seg
		    if(isLepton(getKineObj(seg->getTrack(j))))
		    {
			if(conv_mode==1)
			{
			    //in this mode I check whether it is conversion lepton
			    if(isPi0Conv(getKineObj(seg->getTrack(j))))
			    {
				getKineObj(seg->getTrack(j))->getParticle(dummy,pID2);
				getKineObj(seg->getTrack(j))->getCreator(pTrack_par2,dummy,dummy);
			    }
			}
			else
			{
			    getKineObj(seg->getTrack(j))->getParticle(dummy,pID2);
			    getKineObj(seg->getTrack(j))->getCreator(pTrack_par2,dummy,dummy);
			}
		    }

		    if( ( ( (pID1==2&&pID2==3)      ||
			 (pID2==2&&pID1==3) )       &&
			(pTrack_par2==pTrack_par1) )||
		       (conv_mode!=1))
		    {
			// This is a DOUBLE mdcseg (either 2 leptons or conversion
			// pair (depending on the conv_mode)) if conv_mode!=1 then I
			// do not check weather both leptons are e+e- from one mother
			// particle check for leptons or conv is done already above
			return kTRUE;
		    }
		}
	    }
	}
    }
    return kFALSE;
}

Bool_t HMdcFunc1::isDouble(HMdcHitSim* hit,Int_t conv_mode)
{
    // The method operates only on simulated data.
    // Using the geant track numbers of the tracks in mdcseg
    // (5 tracks stored) decide whether it is a double tracks
    // from lepton or not conv_more =0 seg is DOUBLE if there
    // are 2 different leptons contributing to it and they are pair
    // conv_more =1 seg is DOUBLE if there are 2 different leptons
    // contributing to it and leptons are conversion pair. Update
    // from v6_12 for complete list of track num related to hit
    // one has to look at MdcClusInf
    Int_t debug      = 0;
    Int_t clsindex   =-1;
    Int_t track_mul  = 0;
    Int_t pID2       = 0;
    Int_t pID1       =-1;
    Int_t pTrack_par1=-1;
    Int_t pTrack_par2=-2;
    Int_t dummy      =-1;

    HMdcClusInfSim* pObj_clusinf=NULL;
    clsindex=hit->getClusInfIndex();
    pObj_clusinf=(HMdcClusInfSim*) getClusInfObj(clsindex);
    if(pObj_clusinf)
    {
	track_mul=pObj_clusinf->getNTracks();
	//it gives number of tracks (max) stored for this segment
	if(track_mul>1&&pObj_clusinf->getNTimesInTrack(1)>2)
	{
	    //only segment that consist more than one track can be a ideal double
	    if (debug==1)
	    {
		if (track_mul==2) printf("mul=%i t0=%i(%i) t1=%i(%i)\n",
					 track_mul,
					 pObj_clusinf->getTrack(0),
					 pObj_clusinf->getNTimesInTrack(0),
					 pObj_clusinf->getTrack(1),
					 pObj_clusinf->getNTimesInTrack(1));

		if (track_mul==3) printf("mul=%i t0=%i(%i) t1=%i(%i) t2=%i(%i) \n",
					 track_mul,
					 pObj_clusinf->getTrack(0),
					 pObj_clusinf->getNTimesInTrack(0),
					 pObj_clusinf->getTrack(1),
					 pObj_clusinf->getNTimesInTrack(1),
					 pObj_clusinf->getTrack(2),
					 pObj_clusinf->getNTimesInTrack(2));

		if (track_mul==4) printf("mul=%i t0=%i(%i) t1=%i(%i) t2=%i(%i) t3=%i(%i)\n",
					 track_mul,pObj_clusinf->getTrack(0),
					 pObj_clusinf->getNTimesInTrack(0),
					 pObj_clusinf->getTrack(1),
					 pObj_clusinf->getNTimesInTrack(1),
					 pObj_clusinf->getTrack(2),
					 pObj_clusinf->getNTimesInTrack(2),
					 pObj_clusinf->getTrack(3),
					 pObj_clusinf->getNTimesInTrack(3));

		if (track_mul==5) printf("mul=%i t0=%i(%i) t1=%i(%i) t2=%i(%i) t3=%i(%i) t4=%i(%i)\n",
					 track_mul,
					 pObj_clusinf->getTrack(0),
					 pObj_clusinf->getNTimesInTrack(0),
					 pObj_clusinf->getTrack(1),
					 pObj_clusinf->getNTimesInTrack(1),
					 pObj_clusinf->getTrack(2),
					 pObj_clusinf->getNTimesInTrack(2),
					 pObj_clusinf->getTrack(3),
					 pObj_clusinf->getNTimesInTrack(3),
					 pObj_clusinf->getTrack(4),
					 pObj_clusinf->getNTimesInTrack(4));
	    }


	    for(Int_t i=0;i<track_mul;i++)
	    {
		for(Int_t j=i+1;j<track_mul;j++)
		{
		    // check i-th track in seg
		    if(isLepton(getKineObj(pObj_clusinf->getTrack(i))))
		    {
			if(conv_mode==1)
			{
			    //in this mode I check whether it is conversion lepton
			    if(isPi0Conv(getKineObj(pObj_clusinf->getTrack(i))))
			    {
				getKineObj(pObj_clusinf->getTrack(i))->getParticle(dummy,pID1);
				getKineObj(pObj_clusinf->getTrack(i))->getCreator(pTrack_par1,dummy,dummy);
			    }
			}
			else
			{
			    getKineObj(pObj_clusinf->getTrack(i))->getParticle(dummy,pID1);
			    getKineObj(pObj_clusinf->getTrack(i))->getCreator(pTrack_par1,dummy,dummy);
			}
		    }

		    // check j-th track in seg
		    if(isLepton(getKineObj(pObj_clusinf->getTrack(j))))
		    {
			if(conv_mode==1)
			{
			    //in this mode I check whether it is conversion lepton
			    if(isPi0Conv(getKineObj(pObj_clusinf->getTrack(j))))
			    {
				getKineObj(pObj_clusinf->getTrack(j))->getParticle(dummy,pID2);
				getKineObj(pObj_clusinf->getTrack(j))->getCreator(pTrack_par2,dummy,dummy);
			    }
			}
			else
			{
			    getKineObj(pObj_clusinf->getTrack(j))->getParticle(dummy,pID2);
			    getKineObj(pObj_clusinf->getTrack(j))->getCreator(pTrack_par2,dummy,dummy);
			}
		    }

		    if(((pID1==2&&pID2==3)||(pID2==2&&pID1==3))&&
		       (pTrack_par2==pTrack_par1))
		    {
			//This is a DOUBLE mdcseg (either 2 leptons from the same mother or conversion pair
			//(depending on the conv_mode))

			if(debug==1)   printf(" Module=%i DOUBLE %i %i \n",
					      hit->getModule(),
					      pObj_clusinf->getTrack(i),
					      pObj_clusinf->getTrack(j));
			return kTRUE;
		    }
		}
	    }
	}
    }
    return kFALSE;
}

Bool_t HMdcFunc1::isMdcSegInGeant(HMdcSegSim* seg,Int_t track)
{
    // this method look if mdcseg that corresponds to
    // geant tracknumber track if found at the same place in GeantMdc
    Float_t xgeant    = 1000.0;
    Float_t ygeant    = 1000.0;
    Float_t dummy     = 1000.0;
    Float_t x_hit     =-1000.0;
    Float_t y_hit     =-1000.0;
    Float_t Xdiff     = 3*2.5;
    Float_t Ydiff     = 3*1.5;
    Int_t module_geant=-1;

    HGeantMdc *pObj_geantmdc=NULL;
    if(getGeantMdcCat())
    {
	HIterator *iterator_mdcgeant=(HIterator*)getGeantMdcCat()->MakeIterator();
	iterator_mdcgeant->Reset();
	while ((pObj_geantmdc=(HGeantMdc*)iterator_mdcgeant->Next())!= 0)
	{
	    if(pObj_geantmdc->getLayer()  !=6||
	       (pObj_geantmdc->getModule()==2||
		pObj_geantmdc->getModule()==3)) continue;
	    if(pObj_geantmdc->getTrack()==track)
	    {
		//this pObj_geantmdc object has the same track number as this segment
		pObj_geantmdc->getHit(xgeant,ygeant,dummy,dummy);
		module_geant=pObj_geantmdc->getModule();
		if(getMdcHit(seg,module_geant))
		{
		    x_hit=getMdcHit(seg,module_geant)->getX();
		    y_hit=getMdcHit(seg,module_geant)->getY();
		}
		if(TMath::Abs(x_hit-xgeant)<Xdiff&&TMath::Abs(y_hit-ygeant)<Ydiff)
		{
		    delete iterator_mdcgeant;
		    return kTRUE;
		}
	    }
	}
	delete iterator_mdcgeant;
    } else {
	::Error("isMdcSegInGeant()","To Call HMdcFunc1::isMdcSegInGeant(HMdcSegSim *seg,Int_t track) one has to set GeomMdc category");
    }
    return kFALSE;
}


Int_t  HMdcFunc1::getPairTrack(Int_t track1)
{
    //this method finds a pair lepton to lepton with track1
    //I use the fact that it is either track1+1 or track1+1
    Int_t pTrack_par1=-1;
    Int_t pTrack_par2=-2;
    Int_t dummy      = 0;
    Int_t aTrack2    =-1;
    Int_t aID2       =-2;

    if(track1>=0)
    {
	getKineObj(track1)->getCreator(pTrack_par1,dummy,dummy);

	getKineObj(track1+1)->getCreator(pTrack_par2,dummy,dummy);
	getKineObj(track1+1)->getParticle(aTrack2,aID2);

	if(pTrack_par2==pTrack_par1) return (track1+1);

	getKineObj(track1-1)->getCreator(pTrack_par2,dummy,dummy);
	if(pTrack_par2==pTrack_par1) return (track1-1);

    }
    return -1;
}

HGeantKine*  HMdcFunc1::getPairTrack(HGeantKine* kine1)
{
    //this method finds a pair lepton to lepton with track1
    //I use the fact that it is either track1+1 or track1+1
    Int_t pTrack_par1=-1;
    Int_t pTrack_par2=-2;
    Int_t dummy      = 0;
    Int_t track1     =-1;
    if(kine1)
    {
	kine1->getParticle(track1,dummy);
	kine1->getCreator(pTrack_par1,dummy,dummy);
	if(getKineObj(track1+1)) getKineObj(track1+1)->getCreator(pTrack_par2,dummy,dummy);
	if(pTrack_par2==pTrack_par1) return  getKineObj(track1+1);
	if(getKineObj(track1-1)) getKineObj(track1-1)->getCreator(pTrack_par2,dummy,dummy);
	if(pTrack_par2==pTrack_par1) return  getKineObj(track1-1);
    }
    return NULL;
}

Float_t HMdcFunc1::getPairOpenAngle(HGeantKine* kine1,HGeantKine* kine2)
{
    //input 2 kine objects
    //output opening angle of trajectories
    Double_t rad2deg = TMath::RadToDeg();

    HGeomVector vec1;
    if (kine1)
    {
	Float_t xMom1,yMom1,zMom1;
	kine1->getMomentum(xMom1,yMom1,zMom1);
	vec1.setX(xMom1);
	vec1.setY(yMom1);
	vec1.setZ(zMom1);

	vec1/=vec1.length();//norm
    }
    HGeomVector vec2;
    if (kine2)
    {
	Float_t xMom2,yMom2,zMom2;
	kine2->getMomentum(xMom2,yMom2,zMom2);
	vec2.setX(xMom2);
	vec2.setY(yMom2);
	vec2.setZ(zMom2);

	vec2/=vec2.length();//norm
    }

    Float_t cosfOpeningAngle = vec1.scalarProduct(vec2);
    Float_t fOpeningAngle=0.;
    if (TMath::Abs(cosfOpeningAngle) <= 1) {
	fOpeningAngle=TMath::ACos(cosfOpeningAngle) * rad2deg;
    }

    Float_t xMom1,yMom1,zMom1;
    Float_t xMom2,yMom2,zMom2;
    kine1->getMomentum(xMom1,yMom1,zMom1);
    kine2->getMomentum(xMom2,yMom2,zMom2);
    TVector3 vvec1(xMom1,yMom1,zMom1);
    TVector3 vvec2(xMom2,yMom2,zMom2);

    fOpeningAngle=vvec1.Angle(vvec2)* rad2deg;

    return fOpeningAngle;
}

Float_t HMdcFunc1::getPairInvMass(HGeantKine* kine1,HGeantKine* kine2)
{
    //copy from HPidFL
    if(kine1&&kine2)
    {
	Float_t opang = TMath::DegToRad()*getPairOpenAngle(kine1,kine2);
	Float_t p1    = kine1->getTotalMomentum();
	Float_t p2    = kine2->getTotalMomentum();
	return 2.*sin(opang/2.)*sqrt(p1*p2);
    }
    return -1;
}

void HMdcFunc1::putInArray(Int_t cls,TArrayI* arr,Int_t index)
{
    //filling the array arr
    if (index<arr->GetSize()){
	arr->AddAt(cls,index);
    } else {
	arr->Set(2*arr->GetSize());
	arr->AddAt(cls,index);
    }
}

Bool_t HMdcFunc1::isInArray(Int_t cls,TArrayI* arr)
{
    //checking if number cls is already in arr
    for(Int_t i=0;i<arr->GetSize();i++){
	if(arr->At(i)==cls) return kTRUE;
    }
    return kFALSE;
}

void HMdcFunc1::resetArray(TArrayI* arr)
{
    //put all entries to -1
    for(Int_t i=0;i<arr->GetSize();i++) arr->AddAt(-1,i);
}

Bool_t HMdcFunc1::isInGeantMdc(Int_t tr)
{
    //check whether track tr is in GeantMdc inner modules
    HGeantMdc *pObj_geantmdc=NULL;
    if(getGeantMdcCat())
    {
	HIterator *iterator_mdcgeant= (HIterator*)getGeantMdcCat()->MakeIterator();
	iterator_mdcgeant->Reset();
	while ((pObj_geantmdc=(HGeantMdc*)iterator_mdcgeant->Next())!= 0)
	{
	    if(pObj_geantmdc->getModule()==2||pObj_geantmdc->getModule()==3) continue;
	    if(pObj_geantmdc->getTrack()==tr){
                delete iterator_mdcgeant;
		return kTRUE;
	    }
	}
	delete iterator_mdcgeant;
    } else {
	::Error("isInGeantMdc()","To Call HMdcFunc1::isInGeantMdc(Int tr)one has to set HMdcFunc1::GeomMdc category");
    }

    return kFALSE;
}


Bool_t HMdcFunc1::isInGeantRichMirror(Int_t tr)
{
    //check whether track tr is in GeantMdc inner modules
    HGeantRichMirror* pObj_geantrichmirr=NULL;
    Int_t aTrk,aID;
    if(getGeantRichMirrorCat())
    {
	HIterator *iterator_geant= (HIterator*)getGeantRichMirrorCat()->MakeIterator();
	iterator_geant->Reset();
	while ((pObj_geantrichmirr=(HGeantRichMirror*)iterator_geant->Next())!= 0)
	{
	    pObj_geantrichmirr->getTrack(aTrk,aID);
	    if(aTrk==tr){
		delete iterator_geant;
		return kTRUE;
	    }
	}
	delete iterator_geant;
    } else {
	::Error("isInGeantRichMirror()","To Call HMdcFunc1::isInGeantRichMirr(Int tr)one has to set HMdcFunc1::GeomRichMirr category");
    }
    return kFALSE;
}

Bool_t HMdcFunc1::isInGeantTof(Int_t tr)
{
    //check whether track tr is in GeantTof
    HGeantTof* pObj_geant=NULL;

    if(getGeantTofCat())
    {
	HIterator *iterator_geant= (HIterator*)getGeantTofCat()->MakeIterator();
	iterator_geant->Reset();
	while ((pObj_geant=(HGeantTof*)iterator_geant->Next())!= 0)
	{
	    if(pObj_geant->getModule()>21) continue; //this would be TOFINO
	    if(pObj_geant->getTrack()==tr){
                delete iterator_geant;
		return kTRUE;
	    }
	}
	delete iterator_geant;
    } else {
	::Error("isInGeantTof()","To Call HMdcFunc1::isInGeantTof(Int tr)one has to set HMdcFunc1::GeomTof category");
    }
    return kFALSE;
}

Bool_t HMdcFunc1::isInGeantShower(Int_t tr)
{
    //check whether track tr is in GeantShower
    HGeantShower* pObj_geant=NULL;

    if(getGeantShowCat())
    {
	HIterator *iterator_geant= (HIterator*)getGeantShowCat()->MakeIterator();
	iterator_geant->Reset();
	while ((pObj_geant=(HGeantShower*)iterator_geant->Next())!= 0)
	{

	    if(pObj_geant->getTrack()==tr){
                delete iterator_geant;
		return kTRUE;
	    }
	}
	delete iterator_geant;
    } else {
	::Error("isInGeantShower()","To Call HMdcFunc1::isInGeantShower(Int tr)one has to set HMdcFunc1::GeomShower category");
    }
    return kFALSE;
}

Bool_t HMdcFunc1::isHadesAccepted(Int_t tr)
{
    //look whether this track is seen in rich-mdc tof||show
    if(isInGeantRichMirror(tr))
    {
	if(isInGeantMdc(tr))
	{
	    if(isInGeantTof(tr))
	    {
		return kTRUE;
	    }
	    if(isInGeantShower(tr))
	    {
		return kTRUE;
	    }
	}
    }
    return kFALSE;
}
/*
Bool_t HMdcFunc1::couldBeAccepted(Int_t tr)
{
    if(tr>=0) return couldBeAccepted(getKineObj(tr));

    return kFALSE;
}
*/
/*
Bool_t HMdcFunc1::couldBeAccepted(HGeantKine* kine)
{
    // check whether this particle was created before rich mirror
    // that it could be possible flying to HADES acceptance and
    // should be taken in an account for acceptance studies
    ::Warning("couldBeAccepted()","problem: materials could change! check the use of the function!");
    Float_t verX,verY,verZ;
    Int_t material,dum;
    if(kine)
    {
	kine->getVertex(verX,verY,verZ); //mirror max radius is 715 mm
	if(sqrt(verX*verX+verY*verY+verZ*verZ)>715) return kFALSE;
	kine->getCreator(dum,material,dum);

	if(material==3  ||
	   material==4  ||
	   material==8  ||
	   material==9  ||
	   material==10 ||
	   material==17 ||
	   material==19) return kTRUE;
	// materials Al,Steel,C4F10,CaF2,CKFshell,RichMirr,CarbonTarget could change in various gen of sim data
	// this is valid for gen5 Nov01

    }
    return kFALSE;
}
*/

Bool_t HMdcFunc1::passCprPdfCuts(HPidDilepton* pDil,Float_t prob,const Char_t* name_of_pdf_file,Int_t mode_rs,const Option_t* rej_condition)
{
    //checking wheather both PidParticles fullfill CPR propabilities to be single
    if(passCprPdfCuts((HPidParticle *)pDil->getParticle1(),prob,name_of_pdf_file,mode_rs,rej_condition)==kFALSE) return kFALSE;
    if(passCprPdfCuts((HPidParticle *)pDil->getParticle2(),prob,name_of_pdf_file,mode_rs,rej_condition)==kFALSE) return kFALSE;
    return kTRUE;
}

Bool_t HMdcFunc1::passCprPdfCuts(HPidParticle* pPart,Float_t prob,const Char_t* name_of_pdf_file,Int_t mode_rs,const Option_t* rej_condition)
{
    // checking wheather HPidParticle pass a probability to be a single
    // mode_rs =0 for simulation data
    // mode_rs =1 for real data


    TString condition = rej_condition;
    condition.ToUpper();

    HPidCPParam par(name_of_pdf_file);

    Int_t sec          =-1;
    Int_t nwires_part_0=-1;
    Int_t nwires_part_1=-1;
    Int_t level_0      =-1;
    Int_t level_1      =-1;
    Int_t the_b        =-1;
    Int_t phi_b        =-1;
    Int_t level_b0     =-1;
    Int_t level_b1     =-1;
    Int_t cls_part_0   =-1;
    Int_t cls_part_1   =-1;
    Int_t charge_part  =-1;
    Float_t mdc_the    =-1.0;
    Float_t mdc_phi    =-1.0;

    HMdcSeg* seg=getMdcSegFromPidTrackCand(getPidTrackCand(pPart));
    sec          =seg->getSec();

    mdc_the      =seg->getTheta();
    mdc_phi      =seg->getPhi();
    mdc_the      =getNormalMdcTheta(mdc_the);
    mdc_phi      =getNormalMdcPhi(sec,mdc_phi);
    the_b        =calculateBin(mdc_the,0);
    phi_b        =calculateBin(mdc_phi,1);

    level_0      =getMdcLevelCls(pPart,0);
    level_1      =getMdcLevelCls(pPart,1);
    level_b0     =calculateLevelBin(level_0);
    level_b1     =calculateLevelBin(level_1);

    cls_part_0   =getMdcClsSize(pPart,0);
    cls_part_1   =getMdcClsSize(pPart,1);

    nwires_part_0=getMdcNWires(pPart,0);
    nwires_part_1=getMdcNWires(pPart,1);

    charge_part  =getIntCharge(pPart);

    if(mode_rs==0)
    {
	//this is real data
	cls_part_0   =corect_cls(cls_part_0,the_b,phi_b,level_b0);
	cls_part_1   =corect_cls(cls_part_1,the_b,phi_b,level_b1);
	nwires_part_0=corect_nwires(nwires_part_0,the_b,phi_b,level_b0);
	nwires_part_1=corect_nwires(nwires_part_0,the_b,phi_b,level_b1);
    }

    Float_t prop_single0=par.getProbSingle(the_b,phi_b,level_b0,cls_part_0,nwires_part_0,0);
    Float_t prop_single1=par.getProbSingle(the_b,phi_b,level_b1,cls_part_1,nwires_part_1,1);

    //in a case having -1 made default

    if(strstr(condition.Data(),"OR"))
    {
	if (prop_single0>prob||prop_single1>prob) return kTRUE;
    }

    if(strstr(condition.Data(),"AND"))
    {
	if (prop_single0>prob&&prop_single1>prob) return kTRUE;
    }

    if(strstr(condition.Data(),"AND")==NULL &&
       strstr(condition.Data(),"OR")==NULL )
    {
	::Error("passCprPdfCuts()","THIS condition: %s is unknown possible: AND and OR",condition.Data());
    }

    return kFALSE;
}

Int_t HMdcFunc1::corect_cls(Int_t cls,Int_t the_bin,Int_t phi,Int_t level)
{
    //corect cls to keep sim/real correspondance
    //function the,phi,level
    //for the moment only the
    ::Warning("corect_cls()","Hardwired corrections!!!!check!");
    Float_t k[8]={1.18,1.17,1.10,1.10,1.13,1.12,1.15,1.10};
    if( the_bin>=0&&the_bin<8)
    {
	cls=(Int_t) (cls*k[the_bin]);
    }
    return cls;
}

Int_t HMdcFunc1::corect_nwires(Int_t nwires,Int_t the_bin,Int_t phi,Int_t level)
{
    //corect nwires to keep sim/real correspondance
    //function the,phi,level
    //for the moment only the
    ::Warning("corect_nwires()","Hardwired corrections!!!!check!");
    Float_t k[8]={1.06,1.04,1.03,1.04,1.05,1.04,1.04,0.99};
    if( the_bin>=0&&the_bin<8)
    {
	nwires=(Int_t) (nwires*k[the_bin]);
    }
    return nwires;
}

Int_t HMdcFunc1::whatCommonHits(HPidDilepton* dil)
{
    //this check common hits of leptons in pairs
    //return values:
    //0 : no common hits
    //1 : R-  common RICH hit
    //2 : M-  common MDC hit
    //3 : RM- common RICH and MDC hits
    //4 : Me- common META hit
    //5 : RMe-common RICH and META hits
    //6 : MMe-common MDC and META hits
    //7 : should not happend
    //-1: default return

    HPidParticle *pPart1=NULL;
    HPidParticle *pPart2=NULL;
    Int_t sum=-1;

    if(dil)
    {
	pPart1=(HPidParticle *)dil->getParticle1();
	pPart2=(HPidParticle *)dil->getParticle2();

	if(pPart1&& pPart2)
	{
	    HPidTrackCand* pTrackCand1=pPart1->getTrackCand();
	    HPidTrackCand* pTrackCand2=pPart2->getTrackCand();

	    Bool_t common_RICH=kFALSE;
	    Bool_t common_MDC=kFALSE;
	    Bool_t common_META=kFALSE;

	    //check ring ID
	    if(pTrackCand1&& pTrackCand2)
	    {

		HPidHitData* hitdata1=pTrackCand1->getHitData();
		HPidHitData* hitdata2=pTrackCand2->getHitData();

		if(hitdata1->getIndRICH()==hitdata1->getIndRICH()){
		    common_RICH=kTRUE;
		}

		//check MdcSeg Id
		if((hitdata1->getIndInnerSeg()==hitdata2->getIndInnerSeg())&&
		   (hitdata1->getSector()==hitdata2->getSector())){
		    common_MDC=kTRUE;
		}
		//check META Id
		Int_t system1=hitdata1->getSystem();
		Int_t system2=hitdata2->getSystem();

		Int_t ind1=-1;
		Int_t ind2=-1;

		if(system1==0){
		    ind1=hitdata1->getIndShower();
		}else{
		    ind1=hitdata1->getIndTOF();
		}

		if(system2==0){
		    ind2=hitdata2->getIndShower();
		}else{
		    ind2=hitdata2->getIndTOF();
		}


		if(ind1==ind2){
		    common_META=kTRUE;
		}

		sum=0;
		if(common_RICH) sum=sum+1;
		if(common_MDC)  sum=sum+2;
		if(common_META) sum=sum+4;
	    }
	}
    }
    return sum;
}



ClassImp(HMdcFunc1)
