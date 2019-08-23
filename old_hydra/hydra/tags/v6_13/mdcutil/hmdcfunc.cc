#include "hmdcfunc.h"
#include "hpidparticle.h"
#include "hpidparticlesim.h"
#include "hpidtrackcand.h"
#include "hpidcandidate.h"
#include "hmdcseg.h"
#include "hmdchit.h"
#include "hmdcclusinf.h"
#include "hrichhit.h"
#include "hrichhitsim.h"
#include "hkicktracksim.h"
#include "hgeantkine.h"
#include "hgeantmdc.h"
#include "hiterator.h"

Int_t  HMdcFunc::getMdcClsSize(HPidParticle *part,Int_t module){
  HMdcSeg * pMdcSeg=NULL;
  HPidTrackCand * pPidTrackCand=NULL;
  Int_t cls=0;
  if(part) {
    pPidTrackCand=getPidTrackCand(part);
    if(pPidTrackCand){
      pMdcSeg=(HMdcSeg*)pPidTrackCand->getMdcSeg();
      if(pMdcSeg) {
	cls=getMdcClsSize(pMdcSeg,module);
      }
    }
  }
  return cls; 
}

Int_t  HMdcFunc::getMdcNWires(HPidParticle *part,Int_t module){
  HMdcSeg * pMdcSeg=NULL;
  HPidTrackCand * pPidTrackCand=NULL;
  Int_t nwires=0;
  if(part) {
    pPidTrackCand=getPidTrackCand(part);
    if(pPidTrackCand){
      pMdcSeg=(HMdcSeg*)pPidTrackCand->getMdcSeg();
      if(pMdcSeg) {
	nwires=getMdcNWires(pMdcSeg,module);
      }
    }
  }
  return nwires; 
}

Int_t  HMdcFunc::getMdcLevelCls(HPidParticle *part,Int_t module){
  HMdcSeg * pMdcSeg=NULL;
  HPidTrackCand * pPidTrackCand=NULL;
  Int_t level=0;
  if(part) {
    pPidTrackCand=getPidTrackCand(part);
    if(pPidTrackCand){
      pMdcSeg=(HMdcSeg*)pPidTrackCand->getMdcSeg();
      if(pMdcSeg) {
	level=getMdcLevelCls(pMdcSeg,module);
      }
    }
  }
  return level; 
}

Int_t  HMdcFunc::getIntCharge(HPidParticle *part){
  HRichHit * pRichHit=NULL;
  HPidTrackCand * pPidTrackCand=NULL;
  Int_t int_charge=0;
  if(part) {
    pPidTrackCand=getPidTrackCand(part);
    if(pPidTrackCand){
      pRichHit=(HRichHit*)pPidTrackCand->getRichHit();
      if(pRichHit) {
	int_charge=pRichHit->getRingAmplitude();	
      }
    }
  }
  return int_charge; 

}


Int_t  HMdcFunc::getMdcClsSize(HMdcSeg *seg,Int_t module){
 Int_t cls=0;
 HMdcHit *hit;
 if(seg) {
   hit=getMdcHit(seg,module);
   if(hit) cls=getMdcClsSize(hit);
   
 }
 return cls;
}

Int_t  HMdcFunc::getMdcNWires(HMdcSeg *seg,Int_t module){
 Int_t nwires=0;
 HMdcHit *hit;
 if(seg) {
   hit=getMdcHit(seg,module);
   if(hit) nwires=getMdcNWires(hit);
   
 }
 return nwires;
}

Int_t  HMdcFunc::getMdcLevelCls(HMdcSeg *seg,Int_t module){
 Int_t level=0;
 HMdcHit *hit;
 if(seg) {
   hit=getMdcHit(seg,module);
   if(hit) level=getMdcLevelCls(hit);
   
 }
 return level;
}


HPidTrackCand* HMdcFunc::getPidTrackCand(HPidParticle *part){
  HPidCandidate* pidcand=NULL;
  HPidTrackCand* pidtrackcand=NULL;
  
  if(part) {
    pidcand=part->getPidCandidate();
    if(pidcand) {
      pidtrackcand=pidcand->getCandidate();
    }
  }
  return pidtrackcand;
}

HMdcHit* HMdcFunc::getMdcHit(HMdcSeg *seg,Int_t module){
  HMdcHit *hit=NULL;
  Int_t hit_inx=-1;  
  if(seg) {
    hit_inx=seg->getHitInd(module);

       //  Int_t seginx=getMdcSegCat()->getIndex(seg);

//      printf("seg=%i mod=%i hitinx=%i\n",seginx,module,hit_inx);
    if(hit_inx>-1){
      if(getMdcHitCat()) { 
	hit=(HMdcHit *)getMdcHitCat()->getObject(hit_inx);
	return hit;
      } else {
	printf("******MdcHitCat is NULL********\n");
	printf("******use HMdcFunc::setMdcHitCat()********\n");
      }
    } 
  }
  return hit;
}

Int_t   HMdcFunc::getMdcClsSize(HMdcHit *hit){
  Int_t clssize=-1;
  Int_t clsindex=-1;
  HMdcClusInf *pObj_clusinf=NULL;
  
  clsindex=hit->getClusInfIndex();
  pObj_clusinf=getClusInfObj(clsindex);
  if(pObj_clusinf){
    clssize=pObj_clusinf->getClusSize();
  } 
  return clssize;
}

Int_t   HMdcFunc::getMdcNWires(HMdcHit *hit){
  Int_t nwires=-1;
  Int_t clsindex=-1;
  HMdcClusInf *pObj_clusinf=NULL;
  
  clsindex=hit->getClusInfIndex();
  pObj_clusinf=getClusInfObj(clsindex);
  if(pObj_clusinf){
     nwires=pObj_clusinf->getNDrTimes();
  } 
  return nwires;
}
Int_t   HMdcFunc::getMdcLevelCls(HMdcHit *hit){
  Int_t level=-1;
  Int_t clsindex=-1;
  HMdcClusInf *pObj_clusinf=NULL;
  
  clsindex=hit->getClusInfIndex();
  pObj_clusinf=getClusInfObj(clsindex);
  if(pObj_clusinf){
     level=pObj_clusinf->getLevelClFinding();
  } 
  return level;
}

HMdcClusInf* HMdcFunc::getClusInfObj(Int_t clsindex){
  //get ClusInfObj from category using index
 
  HMdcClusInf *pObj_clusinf=NULL; 
  if(getClusInfCat()){
    pObj_clusinf=(HMdcClusInf* )getClusInfCat()->getObject(clsindex);
  } else {
    printf("******MdcClusInfCat is NULL********\n");
    printf("******use HMdcFunc::setMdcClusInfCat()********\n");
  }
  return pObj_clusinf;
}

Int_t HMdcFunc::calculateLevelBin(Int_t level) {
  Int_t mdc_lev_bin =-1;
  if (level>=4&&level<=6) mdc_lev_bin=(Int_t )level-4;
  return mdc_lev_bin;
}

Int_t HMdcFunc::calculateBin(Float_t mdc_angle,Int_t option) {
  // calculate the the bin of the angle (option=0 ... theta; =1 ... phi)
  
  Int_t mdc_theta_bin=-9;
  Int_t mdc_phi_bin=-9;
  Int_t theta_bin_size=10;
  Int_t phi_bin_size=10;
 
  Float_t theta_min=10.0;
  Float_t theta_max=90.0;

  Float_t phi_min=0.0;
  Float_t phi_max=360.0;
  
  switch (option) {
  case 0:
    // case theta
    if(mdc_angle>=theta_min&&mdc_angle<=theta_max) {
      mdc_theta_bin=((Int_t)(mdc_angle-theta_min))/theta_bin_size;
      //  printf("mdc_angle=%f bin=%i \n",mdc_angle,mdc_theta_bin);
      return mdc_theta_bin;
    } else {
      printf ("theta out of range\n");
      return -2;
    }
  case 1:
    if(mdc_angle>=phi_min&&mdc_angle<=phi_max){
      mdc_phi_bin=(((Int_t) mdc_angle)%60)/phi_bin_size; 
      if(mdc_phi_bin>2) mdc_phi_bin=5-mdc_phi_bin ;//e.g. 10-10 and 50-60 is bin 0 
      return mdc_phi_bin;
    }else{
      printf ("phi out of range\n");
      return -2;
    }
    
  
  default: 
    printf("Third argument should be 0-1 for theta-phi angle\n");
    return -1;
  }
}


Float_t HMdcFunc::getNormalMdcPhi(Int_t iSector, Float_t fPhiMdc)
{
  // Convert MDC's phi angle to the coordinate system used in other detectors
  // input mdc phi in rad and sector coordinate system => output in deg and hades coordinate system

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

Float_t HMdcFunc::getNormalMdcTheta(Float_t fThetaMdc)
{
// Convert MDC's theta angle to the coordinate system used in other detectors

    return  TMath::RadToDeg() * fThetaMdc;
}


Int_t  HMdcFunc::getGeantId(HRichHitSim * pRichHitSim,HKickTrackSim * pKickTrack){
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
	    kine=HPidParticleSim::getGeantKine(pKickTrack->getMdcTrack(j));
	    kine->getParticle(iTrack, iId); 
	    if((pKickTrack->getMdcTrack(j)==richTr1)||(pKickTrack->getMdcTrack(j)==richTr2)||(pKickTrack->getMdcTrack(j)==richTr3)){
	      //common RING-MDC-META
	      //iId=-1;
	      //kine=HPidParticleSim::getGeantKine(pKickTrack->getMdcTrack(j));
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
	    
	    kine=HPidParticleSim::getGeantKine(pKickTrack->getMdcTrack(j));
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
		kine=HPidParticleSim::getGeantKine(pKickTrack->getMetaTrack(k));
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
    //  printf("THIS TRACK: RICH %i %i %i MDC (num %i) %i %i %i %i %i META (num %i) %i %i %i\n",richTr1,richTr2,richTr3,pKickTrack->getNMdcTracks(),pKickTrack->getMdcTrack(0),pKickTrack->getMdcTrack(1),pKickTrack->getMdcTrack(2),pKickTrack->getMdcTrack(3),pKickTrack->getMdcTrack(4),pKickTrack->getNMetaTracks(),pKickTrack->getMetaTrack(0),pKickTrack->getMetaTrack(1),pKickTrack->getMetaTrack(2));  
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

Int_t  HMdcFunc::getGeantCommonTrack(HRichHitSim * pRichHitSim,HKickTrackSim * pKickTrack){
  //this calculated CommonGeantTrack in a way I consider it is correct
  //return positive value for  e+ e- common track and:
  //      -2 ring-pi
  //      -3 ring-p
  //      -5 ring-another lepton
  //      -4 ring-mdc corr ele-meta not 
  //      -1 ring-non sence in meta
  //      -10 richhit is null pointer          
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
	    kine=HPidParticleSim::getGeantKine(pKickTrack->getMdcTrack(j));
	    kine->getParticle(iTrack, iId); 
	    if((pKickTrack->getMdcTrack(j)==richTr1)||(pKickTrack->getMdcTrack(j)==richTr2)||(pKickTrack->getMdcTrack(j)==richTr3)){
	      //common RING-MDC-META
	      //iId=-1;
	      //kine=HPidParticleSim::getGeantKine(pKickTrack->getMdcTrack(j));
	      //kine->getParticle(iTrack, iId);
	      
	      //printf("Rich-Meta-Mdc common track=%i part=%i\n",pKickTrack->getMdcTrack(j),iId); 
              //printf("return %i\n",iId);
	      return  pKickTrack->getMdcTrack(j);
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
	    
	    kine=HPidParticleSim::getGeantKine(pKickTrack->getMdcTrack(j));
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
		kine=HPidParticleSim::getGeantKine(pKickTrack->getMetaTrack(k));
		kine->getParticle(iTrack, iId); 
		
		
		//printf("common miss mdc track iTrack=%i set flag_missinmdc=%i\n ",pKickTrack->getMetaTrack(l),(100+iId)); 
		//return (100+iId);
                //I do not return because it could still be double-double-single case
		// flag_missinmdc=(100+iId); 
		flag_missinmdc=pKickTrack->getMetaTrack(k);
		
	      } 	    	      
	    }
	  }
	}        
      }
    }
    //  printf("THIS TRACK: RICH %i %i %i MDC (num %i) %i %i %i %i %i META (num %i) %i %i %i\n",richTr1,richTr2,richTr3,pKickTrack->getNMdcTracks(),pKickTrack->getMdcTrack(0),pKickTrack->getMdcTrack(1),pKickTrack->getMdcTrack(2),pKickTrack->getMdcTrack(3),pKickTrack->getMdcTrack(4),pKickTrack->getNMetaTracks(),pKickTrack->getMetaTrack(0),pKickTrack->getMetaTrack(1),pKickTrack->getMetaTrack(2));  
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
  return -10;

}


HGeantKine* HMdcFunc::getKineObj(Int_t pTrack){
  if(fGeantKineCat!=NULL) {
    return (HGeantKine*)(fGeantKineCat->getObject(pTrack-1));
  } else {
    printf("HMdcFunc::getKineObj there is no fGeantKineCat defined please use HMdcFunc::setGeantKineCat before\n");
    return NULL;
  }
}

Int_t HMdcFunc::getGeantCommonTrack(HPidParticleSim *part){
  HPidTrackCand* pPidTrackCand=NULL;
  HKickTrackSim *pKickTrack=NULL;
  HRichHitSim *pRichHitSim=NULL;
  
  if (part!=NULL) {
    pPidTrackCand=getPidTrackCand(part);
    if (pPidTrackCand!=NULL) {
      pKickTrack=(HKickTrackSim *) pPidTrackCand->getKickTrack();
      pRichHitSim=(HRichHitSim *) pPidTrackCand->getRichHit();
      return getGeantCommonTrack(pRichHitSim,pKickTrack); 
    } 
  } 
  return -10;
}

Bool_t HMdcFunc::isPi0Conv(HGeantKine *kine){
  Int_t pTrack_par=-1,pMed=-1,pMech=-1,pID_par=0,pTrack_par_par=-1,pMed_par=-1,pMech_par=-1,pID_par_par=-1;
  Int_t pTrack_particle=-1,pID_particle=0;
  
  if(isLepton(kine)) {
    kine->getParticle(pTrack_particle,pID_particle);
    kine->getCreator(pTrack_par,pMed,pMech);
    
    if(pTrack_par!=0){
      getKineObj(pTrack_par)->getParticle(pTrack_par,pID_par);
      if(pID_par==1){
	getKineObj(pTrack_par)->getCreator(pTrack_par_par,pMed_par,pMech_par);
	if(pTrack_par_par!=0){
	  getKineObj(pTrack_par_par)->getParticle(pTrack_par_par,pID_par_par);
	  //	cout<<"in checkConv prarodic ="<<pID_par_par<<endl;
	  if(pID_par_par==7){
	    return kTRUE ;
	  }
	}
      }
    }
  }  
  return  kFALSE;
}
Bool_t HMdcFunc::isPi0Dalitz(HGeantKine *kine){
  Int_t pTrack_par=-1,pMed=-1,pMech=-1,pID_par=0;
  Int_t pTrack_particle=-1,pID_particle=0;
  
  if(isLepton(kine)) {
    kine->getParticle(pTrack_particle,pID_particle);
    kine->getCreator(pTrack_par,pMed,pMech);
    
    if(pTrack_par!=0){
      getKineObj(pTrack_par)->getParticle(pTrack_par,pID_par);
      if(pID_par==7){
	return  kTRUE;
      }
    }
  }  
  return  kFALSE;
}

Bool_t HMdcFunc::isLepton(HGeantKine *kine){
  
  Int_t pTrack_particle=-1,pID_particle=0;
  kine->getParticle(pTrack_particle,pID_particle);
  if (pID_particle==2||pID_particle==3) return kTRUE;
  return  kFALSE;
}

Bool_t HMdcFunc::isSingle(HMdcSegSim *seg){
  //The method operates only on simulation data.
  //Using the geant track numbers of the tracks in mdcseg (5 tracks stored) decide whether 
  //it is a single tracks from lepton or not
  //Check also whether it is inner segment. If not returns false.
  Int_t track_mul;
  //look only at inner segments :very important 
  if(seg->getIOSeg()==0){ 
    track_mul=seg->getNTracks();
    //it gives number of tracks (max) stored for this segment
    if(track_mul==1){
      //only segment that consist of one track can be a ideal single (MAYBE TO STRONG HAS TO BE RECONSIDER)
      if(isLepton(getKineObj(seg->getTrack(0)))) {
	//this is segment that consist only from one geant track that is an lepton => we call it SINGLE
	return kTRUE;
      } 
    }
  }
  return kFALSE;
}


Int_t  HMdcFunc::isSingleDouble(HMdcSegSim *seg,Int_t track,Int_t conv_mode){
  //this method have following return values
  // 0 ... normal single
  // 1 ... normal double
  // 2 ... double => because possition of pair lepton to track in MdcGeant too close to MdcHit possition
  //-1 ... nothing of previous
  // return value 2 was introduced because the lack of complete propagation  of all contributing track numbers
  // from contributing wires to segment for fitted one. This is in version v6_11
  
  Int_t track_in_pair=-1;
  
  if(track>0){
    if(isDouble(seg,conv_mode)) return 1; 
    
    if(isSingle(seg)){
      //here I have check position of pair lepton => if too close than I consider it as double, because
      //lost info after fitter remove some wires
      track_in_pair=getPairTrack(track);
      if(isMdcSegInGeant(seg,track_in_pair)) return 2;
      return 0; 
    }
  }
  return -1;
}



Bool_t HMdcFunc::isDouble(HMdcSegSim *seg,Int_t conv_mode){
  //The method operates only on simulated data.
  //Using the geant track numbers of the tracks in mdcseg (5 tracks stored) decide whether 
  //it is a double tracks from lepton or not
  //conv_more =0 seg is DOUBLE if there are 2 different leptons contributing to it
  //conv_more =1 seg is DOUBLE if there are 2 different leptons contributing to it and leptons are conversion pair
  Int_t track_mul=0;
  Int_t pID2=0,pID1=-1,pTrack_par1=-1,pTrack_par2=-2,dummy=-1;
  //look only at inner segments :very important 
  if(seg->getIOSeg()==0){ 
    track_mul=seg->getNTracks();
    //it gives number of tracks (max) stored for this segment
    if(track_mul>1){
      //only segment that consist more than one track can be a ideal double 
      
      for(Int_t i=0;i<track_mul;i++){
       
	for(Int_t j=i+1;j<track_mul;j++){
	 
	 //  printf("track_mul=%i i=%i j=%i\n",track_mul,i,j);
// 	  printf("isTr1Conv=%i isTr2Conv=%i\n",isPi0Conv(getKineObj(seg->getTrack(i))),isPi0Conv(getKineObj(seg->getTrack(j))));
          // check i-th track in seg 
	  if(isLepton(getKineObj(seg->getTrack(i)))) {
	    if(conv_mode==1){
	      //in this mode I check whether it is conversion lepton
	      if(isPi0Conv(getKineObj(seg->getTrack(i)))){
		getKineObj(seg->getTrack(i))->getParticle(dummy,pID1);
		getKineObj(seg->getTrack(i))->getCreator(pTrack_par1,dummy,dummy);
	      }
	    } else {
	      getKineObj(seg->getTrack(i))->getParticle(dummy,pID1);
	      getKineObj(seg->getTrack(i))->getCreator(pTrack_par1,dummy,dummy); 
	    }
	  }

	  // check j-th track in seg 
	  if(isLepton(getKineObj(seg->getTrack(j)))) {
	    if(conv_mode==1){
	      //in this mode I check whether it is conversion lepton
	      if(isPi0Conv(getKineObj(seg->getTrack(j)))){
		getKineObj(seg->getTrack(j))->getParticle(dummy,pID2);
		getKineObj(seg->getTrack(j))->getCreator(pTrack_par2,dummy,dummy); 
	      }
	    } else {
	      getKineObj(seg->getTrack(j))->getParticle(dummy,pID2);
	      getKineObj(seg->getTrack(j))->getCreator(pTrack_par2,dummy,dummy); 
	    }
	  }

	 //  printf("track_mul=%i i=%i j=%i pid1=%i pid2=%i pTrack_par1=%i pTrack_par2=%i \n",track_mul,i,j,pID1,pID2,pTrack_par1,pTrack_par2);
	  if((((pID1==2&&pID2==3)||(pID2==2&&pID1==3))&&(pTrack_par2==pTrack_par1))||(conv_mode!=1)){
	    //This is a DOUBLE mdcseg (either 2 leptons or conversion pair (depending on the conv_mode))
	    //if conv_mode!=1 then I do not check weather both leptons are e+e- from one mother particle 
            //check for leptons or conv is done already above
	    return kTRUE;    
	  }
	}
      }
    }
  }
  return kFALSE;
}


Bool_t HMdcFunc::isMdcSegInGeant(HMdcSegSim *seg,Int_t track){
  //this method look if mdcseg that corresponds to geant tracknumber track if found at the same place in GeantMdc 
  Float_t xgeant=1000.0,ygeant=1000.0,dummy=1000.0,x_hit=-1000.0,y_hit=-1000.0;
  Float_t Xdiff=3*2.5,Ydiff=3*1.5;
  Int_t module_geant=-1;
  HGeantMdc *pObj_geantmdc=NULL;
  if(getGeantMdcCat()){   
  HIterator *iterator_mdcgeant= (HIterator*)getGeantMdcCat()->MakeIterator();
  iterator_mdcgeant->Reset();
  while ((pObj_geantmdc=(HGeantMdc*)iterator_mdcgeant->Next())!= 0) { 
    if(pObj_geantmdc->getLayer()!=6||(pObj_geantmdc->getModule()==2||pObj_geantmdc->getModule()==3)) continue;
    if(pObj_geantmdc->getTrack()==track){
      //this pObj_geantmdc object has the same track number as this segment
      pObj_geantmdc->getHit(xgeant,ygeant,dummy,dummy);
      module_geant=pObj_geantmdc->getModule();
      if(getMdcHit(seg,module_geant)){ 
	x_hit=getMdcHit(seg,module_geant)->getX();
	y_hit=getMdcHit(seg,module_geant)->getY();
      }
      // printf("module=%i xgeant=%f ygeant=%f x_hit=%f y_hit=%f\n",module_geant,xgeant,ygeant,x_hit,y_hit);
      if(TMath::Abs(x_hit-xgeant)<Xdiff&&TMath::Abs(y_hit-ygeant)<Ydiff) return kTRUE;
    }
  } 
  
  } else {
    printf("To Call HMdcFunc::isMdcSegInGeant(HMdcSegSim *seg,Int_t track) one has to set GeomMdc category\n");
  }
  return kFALSE;
}


Int_t  HMdcFunc::getPairTrack(Int_t track1){
  //this method finds a pair lepton to lepton with track1  
  //I use the fact that it is either track1+1 or track1+1
  Int_t pTrack_par1=-1,pTrack_par2=-2,dummy=0;
  getKineObj(track1)->getCreator(pTrack_par1,dummy,dummy); 
  getKineObj(track1+1)->getCreator(pTrack_par2,dummy,dummy); 
  if(pTrack_par2==pTrack_par1) return (track1+1);
  getKineObj(track1-1)->getCreator(pTrack_par2,dummy,dummy); 
  if(pTrack_par2==pTrack_par1) return (track1-1);
  return -1;
}



ClassImp(HMdcFunc) 
