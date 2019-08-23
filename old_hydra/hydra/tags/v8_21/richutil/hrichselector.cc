// Last update by Thomas Eberl: 04/09/23 14:05:33
//
using namespace std;
#include "hrichselector.h"
#include "hrichcut.h"
#include "hrichcutpairsim.h"
#include "hrichutilfunc.h"
#include "hlinearcategory.h"
#include "hhitmatch.h"
#include "hdihitmatch.h"
#include "hhitmatchsim.h"
#include "hdihitmatchsim.h"
#include "htrackinfo.h"
#include "hcategory.h"
#include "hevent.h"
#include "heventheader.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hrichdetector.h"
#include "hiterator.h"
#include "hades.h"
#include "richdef.h"
#include <iostream> 
ClassImp(HRichSelector)

HRichSelector::HRichSelector(const Text_t *name,const Text_t *title,const  Char_t *fname, const Char_t* cEvtType) :
  HReconstructor(name,title)
{
    // set a string that switches on the requested filter 
    pFileName = new TString(fname);
    TString sEvtType(cEvtType);
    if (sEvtType.Contains("real")) evtType=0;
    else if (sEvtType.Contains("simulation")) evtType=1;

}

HRichSelector::HRichSelector()
{

}

HRichSelector::~HRichSelector(void) 
{
 
}

Bool_t HRichSelector::init() 
{
    if (gHades) 
    {
	HEvent *event=gHades->getCurrentEvent();
	HSpectrometer *spec=gHades->getSetup();
	if (event) {
	    // data cat input
	    HDetector *rich = spec->getDetector("Rich");
	    if (rich) 
	    {
		    pHitMatchCat=gHades->getCurrentEvent()->getCategory(catMatchHit);
		    if (!pHitMatchCat) 
		    {
			Error("init()","no Match hit cat found");
			return kFALSE;
		    }
		    pHitMatchIter = (HIterator*) pHitMatchCat->MakeIterator();

		    pDiHitMatchCat=gHades->getCurrentEvent()->getCategory(catDiMatchHit);
		    if (!pDiHitMatchCat) 
		    {
			Error("init()","no Match hit cat found");
			return kFALSE;
		    }
		    pDiHitMatchIter = (HIterator*) pDiHitMatchCat->MakeIterator();
	    }	
	}
    }

    pFileOut = new TFile(pFileName->Data(),"RECREATE");
    track = 0;
    pair = 0;
    tracksim = 0;
    pairsim = 0;
    outTreeTracks = new TTree("Tracks","Tracks");
    outTreePairs = new TTree("Pairs","Pairs");
    outTreePairsTracks = new TTree("TracksInPairs","TracksInPairs");
    outTreeTracksRings = new TTree("RingsInTracks","RingsInTracks");
    outTreePairsTracksRings = new TTree("RingsInTracksInPairs","RingsInTracksInPairs");

    outNtupleS = new TNtuple("Tracks","Tracks",
			    "nbParticles:pid1:pid2:med1:med2:orig1:orig2:richweight1:richweight2:mdcweight1:mdcweight2:corrcode1:corrcode2:mdcseen1:mdcseen2:gmom1:gmom2:nphot1:nphot2:mirrseen:ering:sec:mom:chrg:corrcode:rt:rp:mt:mp:beta:ampl:houtra:patmat:padnr:locmax4:centroid:flagio:clussize:clushit:r:z:goodCorr:goodRing:goodShower:goodBetaMom");

    outNtuplePairsS = new TNtuple("Pairs","Pairs",
				 "mass:opang:charge:nbDmdc:nbDrich:nbDmeta:goodPair:truepair:orig:fake:med:1truetrack:1recCorr:1single:1double:1nbpart:1pid1:1corr1:1orig1:1cpid1:1ctrk1:1mech1:1med1:1pid2:1corr2:1orig2:1cpid2:1ctrk2:1mech2:1med2:1goodCorr:1goodRing:1goodShower:1goodBetaMom:1recCharge:2truetrack:2recCorr:2single:2double:2nbpart:2pid1:2corr1:2orig1:2cpid1:2ctrk1:2mech1:2med1:2pid2:2corr2:2orig2:2cpid2:2ctrk2:2mech2:2med2:2goodCorr:2goodRing:2goodShower:2goodBetaMom:2recCharge:recCharge:1chi2fit:2chi2fit:1fake:2fake:faketype:betadiff:tofdiff:1mom:1ringavgchrg:1ringpatmat:1ringpads:1ringampl:1ringcentr:1gmom1:1gmom2:2mom:2ringavgchrg:2ringpatmat:2ringpads:2ringampl:2ringcentr:2gmom1:2gmom2:rap:pt:1verx1:1very1:1verz1:2verx1:2very1:2verz1:1tof:1beta:2tof:2beta:1sec:2sec:1ktheta:1kphi:2ktheta:2kphi:1metatheta:1metaphi:2metatheta:2metaphi:1sys:2sys:1verx2:1very2:1verz2:2verx2:2very2:2verz2");

    outNtupleE = new TNtuple("Tracks","Tracks",
			    "sec:mom:chrg:corrcode:rt:rp:mt:mp:beta:ampl:houtra:patmat:padnr:locmax4:centroid:flagio:chi2:clushit:r:z:goodCorr:goodRing:goodShower:goodBetaMom:downscaling:downscalingFlag:triggerDec:seqnum:metatheta:metaphi:tof");

    outNtuplePairsE = new TNtuple("Pairs","Pairs",
				 "mass:opang:nbDmdc:nbDrich:nbDmeta:goodPair:1recCorr:1single:1double:1pid:1goodCorr:1goodRing:1goodShower:1goodBetaMom:1recCharge:2recCorr:2single:2double:2pid:2goodCorr:2goodRing:2goodShower:2goodBetaMom:2recCharge:recCharge:1chi2fit:2chi2fit:betadiff:tofdiff:1mom:1ringavgchrg:1ringpatmat:1ringpads:1ringampl:1ringcentr:2mom:2ringavgchrg:2ringpatmat:2ringpads:2ringampl:2ringcentr:rap:pt:1verx:1very:1verz:2verx:2very:2verz:1tof:1beta:2tof:2beta:1sec:2sec:1ktheta:1kphi:2ktheta:2kphi:1metatheta:1metaphi:2metatheta:2metaphi:1sys:2sys:downscaling:downscalingFlag:triggerDec:seqnum:1rtheta:1rphi:2rtheta:2rphi");
    Int_t split = 3;
    Int_t bsize = 64000;

    if (evtType)
    {
	outTreePairsTracksRings->Branch("HHitMatchSim", "HHitMatchSim", &tracksim, bsize,split);
    }
    else
    {
	outTreePairsTracksRings->Branch("HHitMatch", "HHitMatch", &track, bsize,split);
    }

    if (evtType)
    {
	outTreeTracksRings->Branch("HHitMatchSim", "HHitMatchSim", &tracksim, bsize,split);
    }
    else
    {
	outTreeTracksRings->Branch("HHitMatch", "HHitMatch", &track, bsize,split);
    }

    if (evtType)
    {
	outTreeTracks->Branch("HHitMatchSim", "HHitMatchSim", &tracksim, bsize,split);
    }
    else
    {
	outTreeTracks->Branch("HHitMatch", "HHitMatch", &track, bsize,split);
    }

    if (evtType)
    {
	outTreePairs -> Branch("HDiHitMatchSim", "HDiHitMatchSim", &pairsim, bsize,split);
    }
    else
    {
	outTreePairs -> Branch("HDiHitMatch", "HDiHitMatch", &pair, bsize,split);
    }

    if (evtType)
    {
	outTreePairsTracks -> Branch("HHitMatchSim", "HHitMatchSim", &tracksim, bsize,split);
    }
    else
    {
	outTreePairsTracks -> Branch("HHitMatch", "HHitMatch", &track, bsize,split);
    }


    return kTRUE;
   
}

Bool_t HRichSelector::finalize() 
{
    pFileOut->cd();
//     outTreePairsTracksRings->Write();
//     outTreeTracksRings->Write();
//     outTreeTracks->Write();
//     outTreePairs->Write();
//     outTreePairsTracks->Write();
//    outNtuple->Write();
    if (evtType) outNtuplePairsS->Write();
    else  outNtuplePairsE->Write();
    if (evtType) outNtupleS->Write();
    else  outNtupleE->Write();
    pFileOut->Close();

    return kTRUE;
}

Int_t HRichSelector::execute()
{

  if (evtType) fillTrackNtupleS();
  else fillTrackNtupleE();
  if (evtType) fillPairNtupleS();
  else fillPairNtupleE();

//     if (evtType) selectPairsS();
//     else selectPairsE();

//     if (evtType) selectTracksS();
//     else selectTracksE();

//     if (evtType) selectRingsInTracksS();
//     else selectRingsInTracksE();
    
//     if (evtType) selectRingsInTracksInPairsS();
//     else selectRingsInTracksInPairsE();
    
//     if (evtType) selectTracksInPairsS();
//     else selectTracksInPairsE();
    

    // ***************************
    // *     process control     *
    // ***************************

    Int_t kReturnValue=0;//proceed normally
    //    if (isEmptyEvt()) kReturnValue = kSkipEvent;
    return kReturnValue;
}

UInt_t HRichSelector::getDownscalingFlag()
{
  return ((HEventHeader*)((HEvent*)gHades->getCurrentEvent())->getHeader())->getDownscalingFlag();

}
UInt_t HRichSelector::getDownscaling()
{
  return ((HEventHeader*)((HEvent*)gHades->getCurrentEvent())->getHeader())->getDownscaling();

}
UInt_t HRichSelector::getTriggerDecision()
{
  return ((HEventHeader*)((HEvent*)gHades->getCurrentEvent())->getHeader())->getTriggerDecision();

}
UInt_t HRichSelector::getSeqNumber()
{
  return ((HEventHeader*)((HEvent*)gHades->getCurrentEvent())->getHeader())->getEventSeqNumber();

}
void HRichSelector::classifyTrackE(HHitMatch *h,TrackE &track)
{
    // classify track according to rules
    // store classification in flags

     track.goodCorr = isGoodCorrelation(h);
     track.goodRing = isGoodRing(h);
     track.goodShower = isGoodShower(h);
     track.goodBetaMom = isBetaMom3s(h);
     track.recCharge = h->getKickCharge();
     track.chi2fit = h->getMdcClusterSize();
     track.mom = h->getKickMom();
     track.avgChrg = ((Float_t)h->getRingAmplitude())/((Float_t) h->getRingPadNr());
     track.patmat = h->getRingPatMat();
     track.nrpads = h->getRingPadNr();
     track.ampl = h->getRingAmplitude();
     track.centr = h->getCentroid();

     if (h->getShowInd()>-1 && h->getTofInd()==-1) track.sys=0;
     else if (h->getShowInd()==-1 && h->getTofInd()>-1) track.sys=1;

     track.ktheta = h->getMdcTheta();
     track.kphi = h->getMdcPhi();

     track.rtheta = h->getRichTheta();
     track.rphi = h->getRichPhi();

     if (track.sys==0) {track.metatheta = h->getShowerTheta();
     track.metaphi = h->getShowerPhi(); track.tof=h->getTofinoTof();}
     else if (track.sys==1) {track.metatheta = h->getTofTheta();
     track.metaphi = h->getTofPhi();track.tof=h->getTofTof();}
     
     track.sec = h->getSector();
     track.beta=h->getKickBeta();

    Int_t recCorr  = getRecCorrCode(h);
    track.recCorr = (Float_t) recCorr;

}
void HRichSelector::classifyTrackS(HHitMatchSim *h,TrackS &track)
{
    // classify track according to rules
    // store classification in flags
    // rules are cuts on compound sim info like 
    // isSingle, isHadronFake, etc ...

     track.goodCorr = isGoodCorrelation(h);
     track.goodRing = isGoodRing(h);
     track.goodShower = isGoodShower(h);
     track.goodBetaMom = isBetaMom3s(h);
     track.recCharge = h->getKickCharge();
     track.chi2fit = h->getMdcClusterSize();
     track.mom = h->getKickMom();
     track.avgChrg = ((Float_t)h->getRingAmplitude())/((Float_t) h->getRingPadNr());
     track.patmat = h->getRingPatMat();
     track.nrpads = h->getRingPadNr();
     track.ampl = h->getRingAmplitude();
     track.centr = h->getCentroid();

     if (h->getShowInd()>-1 && h->getTofInd()==-1) track.sys=0;
     else if (h->getShowInd()==-1 && h->getTofInd()>-1) track.sys=1;

     track.ktheta = h->getMdcTheta();
     track.kphi = h->getMdcPhi();

     if (track.sys==0) {track.metatheta = h->getShowerTheta();
     track.metaphi = h->getShowerPhi(); track.tof=h->getTofinoTof();}
     else if (track.sys==1) {track.metatheta = h->getTofTheta();
     track.metaphi = h->getTofPhi();track.tof=h->getTofTof();}
     
     track.sec = h->getSector();
     track.beta=h->getKickBeta();


     HTrackInfo* t = h->getTrackInfoObj();
     if (t == NULL) return;

    // decide whether track is true or fake
    Bool_t truetrack = kFALSE;
    Int_t maxcorr=-1;
    Int_t recCorr  = getRecCorrCode(h);
    if (recCorr==4)
    {
	for (Int_t i=0;i<MAXPARTICLES;i++)
	{
	    maxcorr=t->calcCorrCode(i);
	    if (maxcorr==4) 
		{
		    truetrack=kTRUE;
		    track.corr1=(Float_t) maxcorr;
		    track.pid1=(Float_t) t->getParId(i);
		    Float_t orig1=-1;
		    if (t->getConvLep(i) == 1 ) orig1=0;
		    if (t->getPi0Dalitz(i) == 1 ) orig1=1;
		    if (t->getConvLep(i) == -1 && 
			t->getPi0Dalitz(i) == -1) orig1=2;
		    track.orig1 = orig1;
		    track.cpid1=(Float_t) t->getCreaId(i);
		    track.ctrk1=(Float_t) t->getCreaTrkNb(i);
		    track.mech1=(Float_t) t->getMech(i);
		    track.med1=(Float_t) t->getMed(i);
		    track.gmom1= t->getTotMom(i);
		    track.verx1= t->getVertx(i);
		    track.very1= t->getVerty(i);
		    track.verz1= t->getVertz(i);
		    break;
		}
	}
	if (maxcorr<4) truetrack=kFALSE;
    }
    else truetrack=kFALSE;

    track.truetrack = (Float_t) truetrack;
    track.recCorr = (Float_t) recCorr;

    // store the 2nd particle
    for (Int_t i=0;i<MAXPARTICLES;i++)
    {
	maxcorr=t->calcCorrCode(i);
	if (maxcorr>=0 && maxcorr!=4) //a fake 
	{
	    track.corr2 = (Float_t) maxcorr;
	    track.pid2  = (Float_t) t->getParId(i);
	    Float_t orig2=-1;
	    if (t->getConvLep(i) == 1 ) orig2=0;
	    if (t->getPi0Dalitz(i) == 1 ) orig2=1;
	    if (t->getConvLep(i) == -1 && 
		t->getPi0Dalitz(i) == -1) orig2=2;
	    track.orig2 = orig2;
	    track.cpid2=(Float_t) t->getCreaId(i);
	    track.ctrk2=(Float_t) t->getCreaTrkNb(i);
	    track.mech2=(Float_t) t->getMech(i);
	    track.med2=(Float_t) t->getMed(i);
	    track.gmom2=t->getTotMom(i);
	    track.verx2= t->getVertx(i);
	    track.very2= t->getVerty(i);
	    track.verz2= t->getVertz(i);
	}
    }
    
    
    //--------------------------------------

    if (track.truetrack==0)
    {
	//h->dumpToStdoutSim();
	if (track.corr2>=0&&track.corr2<4)
	{
	    
	    if (track.pid2==2 || track.pid2==3) track.fake=0;
	    else if (track.pid2==8 || track.pid2==9) track.fake=1;
	    else if (track.pid2==14 ) track.fake=2;
	    else track.fake=4;
	}
	//cout<<track.corr2<<"  "<<track.pid2<<"  "<<track.fake<<endl;
	//cout<<"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"<<endl;
    }

    //---------------------------------------
    Int_t nbParticles = t->getPartNr();
    track.nbpart=(Float_t) nbParticles;
    // single
    Float_t single=-1;
    if (nbParticles==1)
    {
	Int_t index = getSingleParticleIndexFromTrackInfo(t);
	Float_t richweight = t->getRichWeight(index);
	Float_t mdcweight  = t->getMdcWeight(index);
	Int_t parid = t->getParId(index);
	Int_t gcorr = t->calcCorrCode(index);
	Int_t mirrseen = h->getLeptonOnMirror();//true single if yes
	if ( richweight>0.99 && mdcweight>0.99 &&
	     (parid==2 || parid==3) && gcorr==4 && recCorr==4 &&
	     mirrseen==1) single=1;
    }
    //    if (single==1) {cout<<"is single"<<endl; h->dumpToStdoutSim(); cout<<endl;}


    if (single==1) track.single=1;
    
    Float_t Double = -1.;
    if (nbParticles==2)
    {
	//Float_t richweight1 = t->getRichWeight(0);
	//Float_t mdcweight1  = t->getMdcWeight(0);
	Int_t parid1 = t->getParId(0);
	Int_t gcorr1 = t->calcCorrCode(0);
	Int_t mdcseen = h->getGLepInMDC();//true single if yes

	//Float_t richweight2 = t->getRichWeight(1);
	//Float_t mdcweight2  = t->getMdcWeight(1);
	Int_t parid2 = t->getParId(1);
	Int_t gcorr2 = t->calcCorrCode(1);
	Int_t mirrseen = h->getLeptonOnMirror();//true single if yes

	if ( (parid1+parid2==5) && (recCorr==1 || recCorr==4) &&
	     (gcorr1==1 || gcorr1==4) && (gcorr2==1 || gcorr2==4) &&
	     mirrseen==1 && mdcseen==1) Double=1;
    }
    //    if (Double==1) {cout<<"is double"<<endl; h->dumpToStdoutSim(); cout<<endl;}
    if (Double==1) track.Double=1;
    

}

Int_t HRichSelector::getSingleParticleIndexFromTrackInfo(HTrackInfo* t)
{
    if (t->getPartNr()!=1) Warning("getSingleParticleIndexFromTrackInfo()","not a single particle track");
    Int_t j=-1;
    for (Int_t i=0; i<MAXPARTICLES; i++)
    {
	if(t->getParId(i)>0) {j=i; break;}
    }
    return j;
}

Float_t HRichSelector::getMaxRichWeight(HTrackInfo* t)
{
    Float_t maxWeight=-1.;
    for (Int_t i=0;i<MAXPARTICLES;i++) 
	if (t->getRichWeight(i)>maxWeight) maxWeight=t->getRichWeight(i);
    return  maxWeight;
}

Float_t HRichSelector::getMaxMdcWeight(HTrackInfo* t)
{
    Float_t maxWeight=-1.;
    for (Int_t i=0;i<MAXPARTICLES;i++) 
	if (t->getMdcWeight(i)>maxWeight) maxWeight=t->getMdcWeight(i);
    return  maxWeight;
}

HHitMatch* HRichSelector::get1stTrackE(HDiHitMatch* hdi)
{
    return (HHitMatch*)pHitMatchCat->getObject(hdi->getIndTrk1());
}

HHitMatch* HRichSelector::get2ndTrackE(HDiHitMatch* hdi)
{
    return (HHitMatch*)pHitMatchCat->getObject(hdi->getIndTrk2());
}

HHitMatchSim* HRichSelector::get1stTrackS(HDiHitMatchSim* hdi)
{
    return (HHitMatchSim*)pHitMatchCat->getObject(hdi->getIndTrk1());
}

HHitMatchSim* HRichSelector::get2ndTrackS(HDiHitMatchSim* hdi)
{
    return (HHitMatchSim*)pHitMatchCat->getObject(hdi->getIndTrk2());
}

Bool_t HRichSelector::isGoodTrack(HHitMatch *h)
{
    Bool_t ret_val = kFALSE;

    if (isGoodCorrelation(h) && isGoodRing(h) && isGoodShower(h) && isBetaMom3s(h)) ret_val = kTRUE;

    return ret_val;

}

Bool_t HRichSelector::isSingleTrack(HHitMatchSim *h)
{
    Bool_t ret_val = kFALSE;

    HTrackInfo * t = h->getTrackInfoObj();
    if (t && t->getPartNr()==1 ) 
    {
	ret_val = kTRUE;
	//	h->dumpToStdoutSim();
    }
//     for (Int_t part=0;part<MAXPARTICLES;part++) 
//     { 
		    
// 	if (t->calcCorrCode(part)==4) 
// 		    {
// 			fullyTracked=kTRUE;
// 			//			cout<<"fully tracked particle "<<endl;
// 			if (t->getParId(part)==2) {nTrackCounterep4++;pSampleNameFake = &samplenamefulltrackep;}
    return ret_val;

}

Bool_t HRichSelector::isGoodCorrelation(HHitMatch *h)
{
    Bool_t ret_val = kFALSE;
    
    Float_t rt = h->getRichTheta();
    Float_t rp = h->getRichPhi();

    Float_t kt = h->getMdcTheta();
    Float_t kp = h->getMdcPhi();

    Float_t rad2deg = 180./TMath::Pi();

    Float_t fDeltaTheta = rt - kt;
    Float_t fDeltaPhi = (rp - kp)*TMath::Sin(kt/rad2deg);

    Float_t fRichMdcThetaMinCut  = -3.5;
    Float_t fRichMdcThetaMaxCut  =  0.5;
    Float_t fRichMdcPhiMinCut    = -2.;
    Float_t fRichMdcPhiMaxCut    =  2.;

    if ( fRichMdcThetaMinCut<fDeltaTheta &&
	 fRichMdcThetaMaxCut>fDeltaTheta &&
	 fRichMdcPhiMinCut<fDeltaPhi     &&
	 fRichMdcPhiMaxCut>fDeltaPhi        
	 ) ret_val = kTRUE;

    //    return kTRUE; //////////////////////!!!!!!!!!!!!!!!!!!!!!!!
    return ret_val; //////////////////////!!!!!!!!!!!!!!!!!!!!!!!


}

Bool_t HRichSelector::isGoodPair(HDiHitMatch *hdi)
{
    
    Bool_t ret_val = kFALSE;

    if ( 
	//hdi->getCharge()     == 0   &&
	//hdi->getOpangKICK()  >  9.  &&
	 hdi->getNbDMdcHit()  == 2   &&
	 hdi->getNbDRichHit() == 2   &&
	 hdi->getNbDMetaHit() == 2
	 )
	ret_val = kTRUE;
    
    return ret_val;
    
}

Bool_t HRichSelector::isGoodPairWithGoodTracks(HDiHitMatch* hdi)
{
    Bool_t ret_val = kFALSE;
    if (isGoodPair(hdi))
    {

	HHitMatch *h1 = (HHitMatch*)pHitMatchCat->getObject(hdi->getIndTrk1());
	HHitMatch *h2 = (HHitMatch*)pHitMatchCat->getObject(hdi->getIndTrk2());
	
	if (isGoodTrack(h1) && isGoodTrack(h2) ) ret_val = kTRUE;
    }
    return ret_val;
}

Bool_t HRichSelector::isGoodRing(HHitMatch *h)
{
    if ( h->getRingPatMat()                                            > 200   &&
	 h->getRingPadNr()                                             >   4   &&
	 ((Float_t)h->getRingAmplitude())/((Float_t)h->getRingPadNr()) >   5.  &&
	 h->getCentroid()                                              < 2.8
       )
	return kTRUE;
    else return kFALSE;
}


Bool_t HRichSelector::isGoodShower(HHitMatch *h)
{
    // check only in case of SHOWER was hit !
    if (h->getShowInd() < 0) return kTRUE;

    Float_t fF10 = h->getShowerfSum1()/h->getShowerfSum0();
    Float_t fF20 = h->getShowerfSum2()/h->getShowerfSum0();

    Float_t p = h->getKickMom();
    // Parameter and formula provided by J. Otwinowski, 2003-07-04
    // VALID FOR NOV01 !!!
    Double_t pPar1[]={-0.011, 0.00138, -0.000001027, 0.000000000266};
    Double_t pPar2[]={-0.1057, 0.001178, -0.0000005453, 0.0000000000893};
    
    Float_t fThr10 = 4.0 * (pPar1[0]+pPar1[1]*p+pPar1[2]*p*p+pPar1[3]*p*p*p);
    Float_t fThr20 = 4.0 * (pPar2[0]+pPar2[1]*p+pPar2[2]*p*p+pPar2[3]*p*p*p);

    if( fF10 <= fThr10 && fF20 <= fThr20 ) return kFALSE;
    else return kTRUE; 
}

Bool_t HRichSelector::isBetaMom3s(HHitMatch *h)
{

    Bool_t ret_val = kFALSE;
    Float_t *bl0p=0;
    Float_t *bl0m=0;
    Float_t *bl1p=0;
    Float_t *bl1m=0;
    Float_t *bh1p=0;
    Float_t *bh1m=0;
    // TOFINO resolution is 0,15c, TOF is 0,03 roughly
    Float_t ebl0p[]={0.83,  0.815, 0.81,  0.81,  0.81,  0.808};//+TOFINO
    Float_t ebl0m[]={0.82,  0.815, 0.805, 0.8,   0.798, 0.798};//-TOFINO
    Float_t ebl1p[]={0.89,  0.9,   0.89,  0.88,  0.885, 0.89 };//+TOF
    Float_t ebl1m[]={0.88,  0.9,   0.9,   0.89,  0.89,  0.89 };//-TOF
    Float_t ebh1p[]={1.155, 1.145, 1.145, 1.15,  1.145, 1.145};//+TOF
    Float_t ebh1m[]={1.155, 1.145, 1.145, 1.15,  1.145, 1.15 };//-TOF

    Float_t sbl0p[]={0.82,  0.83,  0.835, 0.835, 0.835, 0.835};//+TOFINO
    Float_t sbl0m[]={0.825, 0.815, 0.82,  0.83,  0.84,  0.84 };//-TOFINO
    Float_t sbl1p[]={0.94,  0.94,  0.94,  0.94,  0.94,  0.935};//+TOF
    Float_t sbl1m[]={0.945, 0.942, 0.94,  0.94,  0.94,  0.94 };//-TOF
    Float_t sbh1p[]={1.09,  1.085, 1.08,  1.08,  1.08,  1.1  };//+TOF
    Float_t sbh1m[]={1.1,   1.09,  1.09,  1.08,  1.08,  1.075};//-TOF

    if (evtType==0)//exp
    {
	//cout<<"is exp"<<endl;
	bl0p = ebl0p;
	bl0m = ebl0m;
	bl1p = ebl1p;
	bl1m = ebl1m;
	bh1p = ebh1p;
	bh1m = ebh1m;

    }
    else if (evtType==1)//sim
    {
	//cout<<"is sim"<<endl;
	bl0p = sbl0p;
	bl0m = sbl0m;
	bl1p = sbl1p;
	bl1m = sbl1m;
	bh1p = sbh1p;
	bh1m = sbh1m;

	
    }
    else cout<<"Evt type not set for cut"<<endl;

    Float_t mom = h->getKickMom();
    Float_t ch  = h->getKickCharge();
    Float_t beta= h->getKickBeta();
    Int_t i = -1;
    if (mom<=100) i=0; 
    else if (mom<=200) i=1; 
    else if (mom<=300) i=2; 
    else if (mom<=400) i=3; 
    else if (mom<=500) i=4; 
    else if (mom<=600) i=5; 
    else i=5;
    Int_t s,t;
    s=t=-1;
    s=h->getShowInd();
    t=h->getTofInd();
    Int_t lSys=-1; 
    if (s>-1) lSys = 0;
    else if (t>-1) lSys = 1;

    if (i!=-1)
    {
	if (lSys==0)
	{
	    if (ch>0 && beta>bl0p[i]) ret_val=kTRUE;
	    else if (ch<0 && beta>bl0m[i]) ret_val=kTRUE;
	}
	else if (lSys==1)
	{
	    //cout<<i<<"  "<<bl1p[i]<<"  "<<bh1p[i]<<"  "<<beta<<endl;
	    if (ch>0 && beta>bl1p[i] && beta<bh1p[i]) ret_val=kTRUE;
	    else if (ch<0 && beta>bl1m[i] && beta<bh1m[i]) ret_val=kTRUE;
	}
	else cout<<"no valid system in mombeta3s cut determined !"<<endl;
    }
    else cout<<"no valid mombeta index assigned !"<<endl;

    return ret_val;
}

Bool_t HRichSelector::selectTracksE()
{

    Int_t maxNrTrackEntries = pHitMatchCat->getEntries();
    Int_t *kicks = new Int_t[maxNrTrackEntries];
    for (Int_t j=0;j<maxNrTrackEntries;j++) kicks[j]=-2;

    pHitMatchIter->Reset();
    HHitMatch *h = 0;
    while( (h = (HHitMatch *)pHitMatchIter->Next()) ) 
    {
	//h->Dump();
	if (isGoodTrack(h)) 
	{
	    if ( HRichCut::isNewIndex(h->getKickInd(),kicks,maxNrTrackEntries) )
	    {// store only different tracks, redundant here
		track = h; 
		outTreeTracks->Fill();
	    }
	}
    }
    delete [] kicks;
return kTRUE;
}

Bool_t HRichSelector::selectTracksS()
{

    Int_t maxNrTrackEntries = pHitMatchCat->getEntries();
    Int_t *kicks = new Int_t[maxNrTrackEntries];
    for (Int_t j=0;j<maxNrTrackEntries;j++) kicks[j]=-2;

    pHitMatchIter->Reset();
    HHitMatchSim *h = 0;
    while( (h = (HHitMatchSim *)pHitMatchIter->Next()) ) 
    {
	//h->Dump();
	if (isGoodTrack(h)) 
	{
	    if ( HRichCut::isNewIndex(h->getKickInd(),kicks,maxNrTrackEntries) )
	    {// store only different tracks, redundant here
		tracksim = h; 
		outTreeTracks->Fill();
	    }
	}
    }
    delete [] kicks;
return kTRUE;
}

Bool_t HRichSelector::fillTrackNtupleS()
{

    Int_t maxNrTrackEntries = pHitMatchCat->getEntries();
    Int_t *kicks = new Int_t[maxNrTrackEntries];
    for (Int_t j=0;j<maxNrTrackEntries;j++) kicks[j]=-2;

    pHitMatchIter->Reset();
    HHitMatchSim *h = 0;
    while( (h = (HHitMatchSim *)pHitMatchIter->Next()) ) 
    {
	//h->Dump();
	//	if (isSingleTrack(h)) 
	//{
	    if ( HRichCut::isNewIndex(h->getKickInd(),kicks,maxNrTrackEntries) )
	    {// store only different tracks, redundant here
		fillNtupleS(h);
	    }
	    //}
    }
    delete [] kicks;
return kTRUE;
}
Bool_t HRichSelector::fillNtupleS(HHitMatchSim *h)
{

    Float_t goodCorr = isGoodCorrelation(h);
    Float_t goodRing = isGoodRing(h);
    Float_t goodShower = isGoodShower(h);
    Float_t goodBetaMom = isBetaMom3s(h);

    HTrackInfo * t = h->getTrackInfoObj();
    Float_t nbParticles = t->getPartNr();

    Float_t pid1 = t->getParId(0); 
    Float_t pid2 = t->getParId(1); 

    Float_t med1 = t->getMed(0); 
    Float_t med2 = t->getMed(1); 

    Float_t orig1=-1;
    if (t->getConvLep(0) == 1 ) orig1=0;
    if (t->getPi0Dalitz(0) == 1 ) orig1=1;
    if (t->getConvLep(0) == -1 && 
	t->getPi0Dalitz(0) == -1) orig1=2;

    Float_t orig2=-1;
    if (t->getConvLep(1) == 1 ) orig2=0;
    if (t->getPi0Dalitz(1) == 1 ) orig2=1;
    if (t->getConvLep(1) == -1 && 
	t->getPi0Dalitz(1) == -1) orig2=2;

    Float_t richweight1 = t->getRichWeight(0); 
    Float_t richweight2 = t->getRichWeight(1); 

    Float_t mdcweight1 = t->getMdcWeight(0); 
    Float_t mdcweight2 = t->getMdcWeight(1); 

    Float_t corrcode1 = t->calcCorrCode(0);
    Float_t corrcode2 = t->calcCorrCode(1);

    Float_t mdcseen1 = t->getGCLepInMDC(0);
    Float_t mdcseen2 = t->getGCLepInMDC(1);

    Float_t gmom1 = t->getTotMom(0);
    Float_t gmom2 = t->getTotMom(1);

    Float_t nphot1 = h->getNumPhot1();
    Float_t nphot2 = h->getNumPhot2();

    Float_t mirrseen = h->getLeptonOnMirror();
    Float_t ering = h->isGCLepRing();

    Float_t sec = h->getSector();
    Float_t mom = h->getKickMom();
    Float_t chrg= h->getKickCharge();
    Float_t corrcode = getRecCorrCode(h);
    Float_t rt = h->getRichTheta();
    Float_t rp = h->getRichPhi();
    Float_t mt = h->getMdcTheta();
    Float_t mp = h->getMdcPhi();
    Float_t beta = h->getKickBeta();

    Float_t ampl = h->getRingAmplitude();
    Float_t houtra=h->getRingHouTra();
    Float_t patmat=h->getRingPatMat();
    Float_t padnr=h->getRingPadNr();
    Float_t locmax4 = h->getRingLocalMax4();
    Float_t centroid = h->getCentroid();

    Float_t flagio = h->getMdcFlagIO();
    Float_t clussize = h->getMdcClusterSize();
    Float_t clushit = h->getMdcClusterHit();
    Float_t r = h->getMdcR();
    Float_t z = h->getMdcZ();

    Float_t args[45];
    args[0]=nbParticles;
    args[1]=pid1;
    args[2]=pid2;
    args[3]=med1;
    args[4]=med2;
    args[5]=orig1;
    args[6]=orig2;
    args[7]=richweight1;
    args[8]=richweight2;
    args[9]=mdcweight1;
    args[10]=mdcweight2;
    args[11]=corrcode1;
    args[12]=corrcode2;
    args[13]=mdcseen1;
    args[14]=mdcseen2;
    args[15]=gmom1;
    args[16]=gmom2;
    args[17]=nphot1;
    args[18]=nphot2;
    args[19]=mirrseen;
    args[20]=ering;
    args[21]=sec;
    args[22]=mom;
    args[23]=chrg;
    args[24]=corrcode;
    args[25]=rt;
    args[26]=rp;
    args[27]=mt;
    args[28]=mp;
    args[29]=beta;
    args[30]=ampl;
    args[31]=houtra;
    args[32]=patmat;
    args[33]=padnr;
    args[34]=locmax4;
    args[35]=centroid;
    args[36]=flagio;
    args[37]=clussize;
    args[38]=clushit;
    args[39]=r;
    args[40]=z;
    args[41]=goodCorr;
    args[42]=goodRing;
    args[43]=goodShower;
    args[44]=goodBetaMom;
//     outNtuple->Fill(nbParticles,pid1,pid2,med1,med2,orig1,orig2,
// 		    richweight1,richweight2,mdcweight1,mdcweight2,
// 		    corrcode1,corrcode2,mdcseen1,mdcseen2,
// 		    gmom1,gmom2,nphot1,nphot2,mirrseen,ering,
// 		    sec,mom,chrg,corrcode,rt,rp,mt,mp,beta,
// 		    ampl,houtra,patmat,padnr,locmax4,centroid,
// 		    flagio,clussize,clushit,r,z);

    outNtupleS->Fill(args);

    return kTRUE;
}
Bool_t HRichSelector::fillTrackNtupleE()
{

    Int_t maxNrTrackEntries = pHitMatchCat->getEntries();
    Int_t *kicks = new Int_t[maxNrTrackEntries];
    for (Int_t j=0;j<maxNrTrackEntries;j++) kicks[j]=-2;

    pHitMatchIter->Reset();
    HHitMatch *h = 0;
    while( (h = (HHitMatch *)pHitMatchIter->Next()) ) 
    {
	//h->Dump();
	//	if (isSingleTrack(h)) 
	//{
	    if ( HRichCut::isNewIndex(h->getKickInd(),kicks,maxNrTrackEntries) )
	    {// store only different tracks, redundant here
		fillNtupleE(h);
	    }
	    //}
    }
    delete [] kicks;
return kTRUE;
}
Bool_t HRichSelector::fillNtupleE(HHitMatch *h)
{


    Float_t goodCorr = isGoodCorrelation(h);
    Float_t goodRing = isGoodRing(h);
    Float_t goodShower = isGoodShower(h);
    Float_t goodBetaMom = isBetaMom3s(h);

    Float_t sec = h->getSector();
    Float_t mom = h->getKickMom();
    Float_t chrg= h->getKickCharge();
    Float_t corrcode = getRecCorrCode(h);
    Float_t rt = h->getRichTheta();
    Float_t rp = h->getRichPhi();
    Float_t mt = h->getMdcTheta();
    Float_t mp = h->getMdcPhi();
    Float_t beta = h->getKickBeta();

    Float_t ampl = h->getRingAmplitude();
    Float_t houtra=h->getRingHouTra();
    Float_t patmat=h->getRingPatMat();
    Float_t padnr=h->getRingPadNr();
    Float_t locmax4 = h->getRingLocalMax4();
    Float_t centroid = h->getCentroid();

    Float_t flagio = h->getMdcFlagIO();
    Float_t clussize = h->getMdcClusterSize();
    Float_t clushit = h->getMdcClusterHit();
    Float_t r = h->getMdcR();
    Float_t z = h->getMdcZ();

    Float_t sys=-1.;
    if (h->getShowInd()>-1 && h->getTofInd()==-1) sys=0;
    else if (h->getShowInd()==-1 && h->getTofInd()>-1) sys=1;
    
    Float_t metatheta=-1.;
    Float_t metaphi=-1.;
    Float_t tof=-1.;
    if (sys==0) {metatheta = h->getShowerTheta();
    metaphi = h->getShowerPhi(); tof=h->getTofinoTof();}
    else if (sys==1) {metatheta = h->getTofTheta();
    metaphi = h->getTofPhi();tof=h->getTofTof();}
    
    //downscaling:downscalingFlag:triggerDec:seqnum
    Float_t downscaling=(Float_t) getDownscaling();
    Float_t downscalingFlag=(Float_t) getDownscalingFlag();
    Float_t triggerDecision=(Float_t) getTriggerDecision();
    Float_t seqnum=(Float_t) getSeqNumber();
    Float_t args[31];
    args[0]=sec;
    args[1]=mom;
    args[2]=chrg;
    args[3]=corrcode;
    args[4]=rt;
    args[5]=rp;
    args[6]=mt;
    args[7]=mp;
    args[8]=beta;
    args[9]=ampl;
    args[10]=houtra;
    args[11]=patmat;
    args[12]=padnr;
    args[13]=locmax4;
    args[14]=centroid;
    args[15]=flagio;
    args[16]=clussize;
    args[17]=clushit;
    args[18]=r;
    args[19]=z;
    args[20]=goodCorr;
    args[21]=goodRing;
    args[22]=goodShower;
    args[23]=goodBetaMom;
    args[24]=downscaling;
    args[25]=downscalingFlag;
    args[26]=triggerDecision;
    args[27]=seqnum;
    args[28]=metatheta;
    args[29]=metaphi;
    args[30]=tof;
//     outNtuple->Fill(nbParticles,pid1,pid2,med1,med2,orig1,orig2,
// 		    richweight1,richweight2,mdcweight1,mdcweight2,
// 		    corrcode1,corrcode2,mdcseen1,mdcseen2,
// 		    gmom1,gmom2,nphot1,nphot2,mirrseen,ering,
// 		    sec,mom,chrg,corrcode,rt,rp,mt,mp,beta,
// 		    ampl,houtra,patmat,padnr,locmax4,centroid,
// 		    flagio,clussize,clushit,r,z);

    outNtupleE->Fill(args);

    return kTRUE;
}
Bool_t HRichSelector::fillPairNtupleE()
{

    pDiHitMatchIter->Reset();
    HDiHitMatch *h = 0;

    while( (h = (HDiHitMatch *)pDiHitMatchIter->Next()) ) 
    {
	fillNtuplePairE(h);
    }

    return kTRUE;
}

Bool_t HRichSelector::fillNtuplePairE(HDiHitMatch *h)
{

    HLinearCategory * cat = (HLinearCategory*) HRichUtilFunc::getCategory(catMatchHit);
    HHitMatch *t1 = 0;HHitMatch *t2 = 0;

    if (cat)
    {
	 t1 = (HHitMatch*)cat->getObject(h->getIndTrk1());
	 if (!t1) Error("fillTuple","HHitMatch object 1 not found");
	 t2 = (HHitMatch*)cat->getObject(h->getIndTrk2());
	 if (!t2) Error("fillTuple","HHitMatch object 2 not found");
    }
    else Error("fillTuple","no category found");

    TrackE track1; resetTrackE(track1);
    TrackE track2; resetTrackE(track2);
    //track.truetrack=-1; track.recCorr=-1.;
    if (t1) classifyTrackE(t1,track1);
    if (t2) classifyTrackE(t2,track2);

    //    if (t1) tt1 = t1->getTrackInfoObj();
    //if (t2) tt2 = t2->getTrackInfoObj();

    PairE pair; resetPairE(pair);
    pair.mass = h->getInvMass();
    pair.rap = h->getPairRap();
    pair.pt = h->getPairPt();
    pair.opang= h->getOpangKICK();
    pair.charge=h->getKICKCharge();
    pair.nbDmdc = h->getNbDMdcHit();
    pair.nbDrich= h->getNbDRichHit();
    pair.nbDmeta= h->getNbDMetaHit();

    pair.betadiff=t1->getKickBeta() - t2->getKickBeta();
    Float_t tof1=-1000., tof2=-1000.;  
    if (t1->getShowInd()!=-1) tof1 = t1->getTofinoTof();
    else if(t1->getTofInd()!=-1) tof1 = t1->getTofTof();
    else Warning("tofdiff","t1: no non-zero META index");
    if (t2->getShowInd()!=-1) tof2 = t2->getTofinoTof();
    else if(t2->getTofInd()!=-1) tof2 = t2->getTofTof();
    else Warning("tofdiff","t1: no non-zero META index");
    pair.tofdiff=tof1-tof2;

    //    pair.recCharge = track1.recCharge+track2.recCharge;
    if (pair.opang>9. && pair.nbDmdc==2 && pair.nbDrich==2 && pair.nbDmeta==2) pair.goodPair=1;;
    //downscaling:downscalingFlag:triggerDec:seqnum
    Float_t downscaling=(Float_t) getDownscaling();
    Float_t downscalingFlag=(Float_t) getDownscalingFlag();
    Float_t triggerDecision=(Float_t) getTriggerDecision();
    Float_t seqnum=(Float_t) getSeqNumber();
    Float_t args[73];
    args[0] = pair.mass;
    args[1] = pair.opang;
    args[2] = pair.nbDmdc;
    args[3] = pair.nbDrich;
    args[4] = pair.nbDmeta;
    args[5] = pair.goodPair;

    args[6] = track1.recCorr;
    args[7] = track1.single;
    args[8] = track1.Double;

    args[9] = track1.pid;

    args[10] = track1.goodCorr;
    args[11] = track1.goodRing;
    args[12] = track1.goodShower;
    args[13] = track1.goodBetaMom;
    args[14] = track1.recCharge;
    //
    args[15] = track2.recCorr;
    args[16] = track2.single;
    args[17] = track2.Double;

    args[18] = track2.pid;

    args[19] = track2.goodCorr;
    args[20] = track2.goodRing;
    args[21] = track2.goodShower;
    args[22] = track2.goodBetaMom;
    args[23] = track2.recCharge;

    args[24] = pair.charge;
    args[25] = track1.chi2fit;
    args[26] = track2.chi2fit;
    args[27] = pair.betadiff;
    args[28] = pair.tofdiff;

    args[29] = track1.mom;
    args[30] = track1.avgChrg;
    args[31] = track1.patmat;
    args[32] = track1.nrpads;
    args[33] = track1.ampl;
    args[34] = track1.centr;
    args[35] = track2.mom;
    args[36] = track2.avgChrg;
    args[37] = track2.patmat;
    args[38] = track2.nrpads;
    args[39] = track2.ampl;
    args[40] = track2.centr;

    args[41] = pair.rap;
    args[42] = pair.pt;

    args[43] = track1.verx;
    args[44] = track1.very;
    args[45] = track1.verz;
    args[46] = track2.verx;
    args[47] = track2.very;
    args[48] = track2.verz;
    args[49] = track1.tof;
    args[50] = track1.beta;
    args[51] = track2.tof;
    args[52] = track2.beta;
    args[53] = track1.sec;
    args[54] = track2.sec;
    args[55] = track1.ktheta;
    args[56] = track1.kphi;
    args[57] = track2.ktheta;
    args[58]= track2.kphi;
    args[59] = track1.metatheta;
    args[60] = track1.metaphi;
    args[61] = track2.metatheta;
    args[62]= track2.metaphi;
    args[63]= track1.sys;
    args[64]= track2.sys;
    args[65]= downscaling;
    args[66]= downscalingFlag;
    args[67]= triggerDecision;
    args[68]= seqnum;
    args[69] = track1.rtheta;
    args[70] = track1.rphi;
    args[71] = track2.rtheta;
    args[72]= track2.rphi;
    outNtuplePairsE->Fill(args);
    //    h->dumpToStdoutSim();
    return kTRUE;
}

Bool_t HRichSelector::fillPairNtupleS()
{

    pDiHitMatchIter->Reset();
    HDiHitMatchSim *h = 0;
    HRichCutPairSim c("default","default");
    c.switchTo("isFakePair");
    while( (h = (HDiHitMatchSim *)pDiHitMatchIter->Next()) ) 
    {
	fillNtuplePairS(h);
	//if (isGoodPairWithGoodTracks(h))
	//{
	    //	    h->dumpToStdoutSim();
	//  if (c.check(h))  fillNtuplePairS(h);
	//}
    }

    return kTRUE;
}

Bool_t HRichSelector::fillNtuplePairS(HDiHitMatchSim *h)
{

    HLinearCategory * cat = (HLinearCategory*) HRichUtilFunc::getCategory(catMatchHit);
    HHitMatchSim *t1 = 0;HHitMatchSim *t2 = 0;

    if (cat)
    {
	 t1 = (HHitMatchSim*)cat->getObject(h->getIndTrk1());
	 if (!t1) Error("fillTuple","HHitMatchSim object 1 not found");
	 t2 = (HHitMatchSim*)cat->getObject(h->getIndTrk2());
	 if (!t2) Error("fillTuple","HHitMatchSim object 2 not found");
    }
    else Error("fillTuple","no category found");

    TrackS track1; resetTrackS(track1);
    TrackS track2; resetTrackS(track2);
    //track.truetrack=-1; track.recCorr=-1.;
    if (t1) classifyTrackS(t1,track1);
    if (t2) classifyTrackS(t2,track2);

    //    if (t1) tt1 = t1->getTrackInfoObj();
    //if (t2) tt2 = t2->getTrackInfoObj();

    PairS pair; resetPairS(pair);
    pair.mass = h->getInvMass();
    pair.rap = h->getPairRap();
    pair.pt = h->getPairPt();
    pair.opang= h->getOpangKICK();
    pair.charge=h->getKICKCharge();
    pair.nbDmdc = h->getNbDMdcHit();
    pair.nbDrich= h->getNbDRichHit();
    pair.nbDmeta= h->getNbDMetaHit();

    pair.betadiff=t1->getKickBeta() - t2->getKickBeta();
    Float_t tof1=-1000., tof2=-1000.;  
    if (t1->getShowInd()!=-1) tof1 = t1->getTofinoTof();
    else if(t1->getTofInd()!=-1) tof1 = t1->getTofTof();
    else Warning("tofdiff","t1: no non-zero META index");
    if (t2->getShowInd()!=-1) tof2 = t2->getTofinoTof();
    else if(t2->getTofInd()!=-1) tof2 = t2->getTofTof();
    else Warning("tofdiff","t1: no non-zero META index");
    pair.tofdiff=tof1-tof2;

//     if (track1.pid1==2 && track2.pid1==3) pair.charge=0;
//     else if (track1.pid1==3 && track2.pid1==2) pair.charge=0;
//     else if (track1.pid2==2 && track2.pid1==3) pair.charge=0;
//     else if (track1.pid2==3 && track2.pid1==2) pair.charge=0;
//     else if (track1.pid1==2 && track2.pid2==3) pair.charge=0;
//     else if (track1.pid1==3 && track2.pid2==2) pair.charge=0;

//      if (track1.pid1==3 && track2.pid1==3) pair.charge=-2;
//     else if (track1.pid2==3 && track2.pid2==3) pair.charge=-2;
//     else if (track1.pid1==3 && track2.pid2==3) pair.charge=-2;
//     else if (track1.pid2==3 && track2.pid1==3) pair.charge=-2;

//      if (track1.pid1==2 && track2.pid1==2) pair.charge=2;
//     else if (track1.pid2==2 && track2.pid2==2) pair.charge=2;
//     else if (track1.pid1==2 && track2.pid2==2) pair.charge=2;
//     else if (track1.pid2==2 && track2.pid1==2) pair.charge=2;

    if (track1.truetrack==1 && track2.truetrack==1)
    {
	if (track1.pid1+track2.pid1==5) pair.charge=0;
	if (track1.pid1+track2.pid1==4) pair.charge=2;
	if (track1.pid1+track2.pid1==6) pair.charge=-2;
    }
    

    // is it a true signal pair ?
    if (pair.charge==0)
    {
	if (track1.truetrack==1 && track2.truetrack==1)  // 2 true tracks                            full Geant correlation
	{
	    
	    if (track1.ctrk1>-1 && track1.ctrk1==track2.ctrk1) //same mother track
	    {
		if (track1.cpid1==17 && track1.mech1==5 && track2.cpid1==17 && track2.mech1==5)  pair.orig=2;
		if (track1.cpid1==1  && track1.mech1==6 && track2.cpid1==1  && track2.mech1==6)  pair.orig=0;
		if (track1.cpid1==7  && track1.mech1==5 && track2.cpid1==7  && track2.mech1==5)  pair.orig=1;
		pair.truepair=1;
		pair.med=track1.med1;
	    }
	    else pair.fake=0; //true CB pair.
	} 
	else //1 track is a fake
	{
	    pair.fake=1;
	    if (track1.fake>-1) pair.faketype = track1.fake;
	    else if (track2.fake>-1) pair.faketype = track2.fake;
	    // further split up in fake categories goes here !!!!!
	}
    }

    pair.recCharge = track1.recCharge+track2.recCharge;
    if (pair.opang>9. && pair.nbDmdc==2 && pair.nbDrich==2 && pair.nbDmeta==2) pair.goodPair=1;;
    Float_t args[113];
    args[0] = pair.mass;
    args[1] = pair.opang;
    args[2] = pair.charge;
    args[3] = pair.nbDmdc;
    args[4] = pair.nbDrich;
    args[5] = pair.nbDmeta;
    args[6] = pair.goodPair;
    args[7] = pair.truepair;
    args[8] = pair.orig;
    args[9] = pair.fake;
    args[10] = pair.med;

    args[11] = track1.truetrack;
    args[12] = track1.recCorr;
    args[13] = track1.single;
    args[14] = track1.Double;
    args[15] = track1.nbpart;

    args[16] = track1.pid1;
    args[17] = track1.corr1;
    args[18] = track1.orig1;
    args[19] = track1.cpid1;
    args[20] = track1.ctrk1;
    args[21] = track1.mech1;
    args[22] = track1.med1;

    args[23] = track1.pid2;
    args[24] = track1.corr2;
    args[25] = track1.orig2;
    args[26] = track1.cpid2;
    args[27] = track1.ctrk2;
    args[28] = track1.mech2;
    args[29] = track1.med2;

    args[30] = track1.goodCorr;
    args[31] = track1.goodRing;
    args[32] = track1.goodShower;
    args[33] = track1.goodBetaMom;
    args[34] = track1.recCharge;
    //
    args[35] = track2.truetrack;
    args[36] = track2.recCorr;
    args[37] = track2.single;
    args[38] = track2.Double;
    args[39] = track2.nbpart;

    args[40] = track2.pid1;
    args[41] = track2.corr1;
    args[42] = track2.orig1;
    args[43] = track2.cpid1;
    args[44] = track2.ctrk1;
    args[45] = track2.mech1;
    args[46] = track2.med1;

    args[47] = track2.pid2;
    args[48] = track2.corr2;
    args[49] = track2.orig2;
    args[50] = track2.cpid2;
    args[51] = track2.ctrk2;
    args[52] = track2.mech2;
    args[53] = track2.med2;

    args[54] = track2.goodCorr;
    args[55] = track2.goodRing;
    args[56] = track2.goodShower;
    args[57] = track2.goodBetaMom;
    args[58] = track2.recCharge;
    args[59] = pair.recCharge;
    args[60] = track1.chi2fit;
    args[61] = track2.chi2fit;
    args[62] = track1.fake;
    args[63] = track2.fake;
    args[64] = pair.faketype;
    args[65] = pair.betadiff;
    args[66] = pair.tofdiff;
    args[67] = track1.mom;
    args[68] = track1.avgChrg;
    args[69] = track1.patmat;
    args[70] = track1.nrpads;
    args[71] = track1.ampl;
    args[72] = track1.centr;
    args[73] = track1.gmom1;
    args[74] = track1.gmom2;
    args[75] = track2.mom;
    args[76] = track2.avgChrg;
    args[77] = track2.patmat;
    args[78] = track2.nrpads;
    args[79] = track2.ampl;
    args[80] = track2.centr;
    args[81] = track2.gmom1;
    args[82] = track2.gmom2;
    args[83] = pair.rap;
    args[84] = pair.pt;
    args[85] = track1.verx1;
    args[86] = track1.very1;
    args[87] = track1.verz1;
    args[88] = track2.verx1;
    args[89] = track2.very1;
    args[90] = track2.verz1;
    args[91] = track1.tof;
    args[92] = track1.beta;
    args[93] = track2.tof;
    args[94] = track2.beta;
    args[95] = track1.sec;
    args[96] = track2.sec;
    args[97] = track1.ktheta;
    args[98] = track1.kphi;
    args[99] = track2.ktheta;
    args[100]= track2.kphi;
    args[101] = track1.metatheta;
    args[102] = track1.metaphi;
    args[103] = track2.metatheta;
    args[104]= track2.metaphi;
    args[105]= track1.sys;
    args[106]= track2.sys;
    args[107] = track1.verx2;
    args[108] = track1.very2;
    args[109] = track1.verz2;
    args[110] = track2.verx2;
    args[111] = track2.very2;
    args[112] = track2.verz2;

    outNtuplePairsS->Fill(args);
    //    h->dumpToStdoutSim();
    return kTRUE;
}

Int_t HRichSelector::getRecCorrCode(HHitMatch* h)
{

    // this is rather redundant and dumb
    Int_t RM   = h->getMatchedRichMdc();

    Int_t RT_S = -1;
    if (h->getMatchedRichTof()==1 || h->getMatchedRichShower()==1) 
	RT_S=1;

    Int_t MT_S = -1;
    if (h->getMatchedMdcTof()==1 || h->getMatchedMdcShower()==1) 
	MT_S=1;

    Int_t RMT_S = -1;
    if (RM==1 && RT_S==1 && MT_S==1) RMT_S=1;

    if (RM==-1 && RT_S==-1 && MT_S==-1 && RMT_S == -1) return -1;

    if (RM!=1 && RT_S!=1 && MT_S!=1 && RMT_S != 1) return 0;

    if (RM==1 && RT_S!=1 && MT_S!=1 && RMT_S != 1) return 1;

    if (RM!=1 && RT_S==1 && MT_S!=1 && RMT_S != 1) return 2;

    if (RM!=1 && RT_S!=1 && MT_S==1 && RMT_S != 1) return 3;

    if (RM==1 && RT_S==1 && MT_S==1 && RMT_S == 1) return 4;

    return -1;
}

Bool_t HRichSelector::selectPairsE()
{
    pDiHitMatchIter->Reset();
    HDiHitMatch *hdi = 0;

    while( (hdi = (HDiHitMatch *)pDiHitMatchIter->Next()) ) 
    {
	//hdi->Dump();
	if ( isGoodPair(hdi) )
	{
	    pair = hdi; outTreePairs->Fill();

	}
    }
return kTRUE;
}

Bool_t HRichSelector::selectPairsS()
{
    pDiHitMatchIter->Reset();
    HDiHitMatchSim *hdi = 0;

    while( (hdi = (HDiHitMatchSim *)pDiHitMatchIter->Next()) ) 
    {
	//hdi->Dump();
	if ( isGoodPair(hdi) )
	{
	    pairsim = hdi; outTreePairs->Fill();

	}
    }
return kTRUE;
}

Bool_t HRichSelector::selectRingsInTracksE()
{
    Int_t maxNrTrackEntries = pHitMatchCat->getEntries();
    Int_t *rings = new Int_t[maxNrTrackEntries];
    for (Int_t j=0;j<maxNrTrackEntries;j++) rings[j]=-2;

    pHitMatchIter->Reset();
    HHitMatch *h = 0;
    while( (h = (HHitMatch *)pHitMatchIter->Next()) ) 
    {
	//h->Dump();
	if (isGoodTrack(h)) 
	{
	    if ( HRichCut::isNewIndex(h->getRichInd(),rings,maxNrTrackEntries) )
	    {// store only tracks with different rings. 1st one is taken: only good for
	     // plotting ring properties afterwards, otherwise choose best one !
		track = h; 
		outTreeTracksRings->Fill();
	    }
	}

    }
    delete [] rings;
return kTRUE;
}

Bool_t HRichSelector::selectRingsInTracksS()
{
    Int_t maxNrTrackEntries = pHitMatchCat->getEntries();
    Int_t *rings = new Int_t[maxNrTrackEntries];
    for (Int_t j=0;j<maxNrTrackEntries;j++) rings[j]=-2;

    pHitMatchIter->Reset();
    HHitMatchSim *h = 0;
    while( (h = (HHitMatchSim *)pHitMatchIter->Next()) ) 
    {
	//h->Dump();
	if (isGoodTrack(h)) 
	{
	    if ( HRichCut::isNewIndex(h->getRichInd(),rings,maxNrTrackEntries) )
	    {// store only tracks with different rings. 1st one is taken: only good for
	     // plotting ring properties afterwards, otherwise choose best one !
		tracksim = h; 
		outTreeTracksRings->Fill();
	    }
	}

    }
    delete [] rings;
return kTRUE;
}


Bool_t HRichSelector::selectRingsInTracksInPairsE()
{
    Int_t maxNrTrackEntries = pDiHitMatchCat->getEntries();
    Int_t *rings = new Int_t[maxNrTrackEntries];
    for (Int_t j=0;j<maxNrTrackEntries;j++) rings[j]=-2;
    
    pDiHitMatchIter->Reset();
    HDiHitMatch *hdi = 0;
    
    while( (hdi = (HDiHitMatch *)pDiHitMatchIter->Next()) ) 
    {
	//hdi->Dump();
	if ( isGoodPairWithGoodTracks(hdi) )
	{
	    if (HRichCut::isNewIndex(get1stTrackE(hdi)->getRichInd(),rings,maxNrTrackEntries) )  
	    {track = get1stTrackE(hdi); outTreePairsTracksRings->Fill();}
	    if (HRichCut::isNewIndex(get2ndTrackE(hdi)->getRichInd(),rings,maxNrTrackEntries) ) 
	    {track = get2ndTrackE(hdi); outTreePairsTracksRings->Fill();}
	}
    }
    
    delete [] rings;
return kTRUE;
}
Bool_t HRichSelector::selectRingsInTracksInPairsS()
{
    Int_t maxNrTrackEntries = pDiHitMatchCat->getEntries();
    Int_t *rings = new Int_t[maxNrTrackEntries];
    for (Int_t j=0;j<maxNrTrackEntries;j++) rings[j]=-2;
    
    pDiHitMatchIter->Reset();
    HDiHitMatchSim *hdi = 0;
    
    while( (hdi = (HDiHitMatchSim *)pDiHitMatchIter->Next()) ) 
    {
	//hdi->Dump();
	if ( isGoodPairWithGoodTracks(hdi) )
	{
	    if (HRichCut::isNewIndex(get1stTrackS(hdi)->getRichInd(),rings,maxNrTrackEntries) )  
	    {tracksim = get1stTrackS(hdi); outTreePairsTracksRings->Fill();}
	    if (HRichCut::isNewIndex(get2ndTrackS(hdi)->getRichInd(),rings,maxNrTrackEntries) ) 
	    {tracksim = get2ndTrackS(hdi); outTreePairsTracksRings->Fill();}
	}
    }
    
    delete [] rings;
return kTRUE;
}

Bool_t HRichSelector::selectTracksInPairsE()
{
    Int_t maxNrTrackEntries = pDiHitMatchCat->getEntries();
    Int_t *tracks = new Int_t[2*maxNrTrackEntries];
    for (Int_t j=0;j<2*maxNrTrackEntries;j++) tracks[j]=-2;
    
    pDiHitMatchIter->Reset();
    HDiHitMatch *hdi = 0;
    
    while( (hdi = (HDiHitMatch *)pDiHitMatchIter->Next()) ) 
    {
	//hdi->Dump();
	if ( isGoodPairWithGoodTracks(hdi) )
	{
	    if (HRichCut::isNewIndex(hdi->getIndTrk1(),tracks,2*maxNrTrackEntries) )  
	    {track = get1stTrackE(hdi); outTreePairsTracks->Fill();}
	    if (HRichCut::isNewIndex(hdi->getIndTrk1(),tracks,2*maxNrTrackEntries) ) 
	    {track = get2ndTrackE(hdi); outTreePairsTracks->Fill();}
	}
    }
    
    delete [] tracks;
return kTRUE;
}
Bool_t HRichSelector::selectTracksInPairsS()
{
    Int_t maxNrTrackEntries = pDiHitMatchCat->getEntries();
    Int_t *tracks = new Int_t[2*maxNrTrackEntries];
    for (Int_t j=0;j<2*maxNrTrackEntries;j++) tracks[j]=-2;
    
    pDiHitMatchIter->Reset();
    HDiHitMatchSim *hdi = 0;
    
    while( (hdi = (HDiHitMatchSim *)pDiHitMatchIter->Next()) ) 
    {
	//hdi->Dump();
	if ( isGoodPairWithGoodTracks(hdi) )
	{
	    if (HRichCut::isNewIndex(hdi->getIndTrk1(),tracks,2*maxNrTrackEntries) )  
	    {tracksim = get1stTrackS(hdi); outTreePairsTracks->Fill();}
	    if (HRichCut::isNewIndex(hdi->getIndTrk1(),tracks,2*maxNrTrackEntries) ) 
	    {tracksim = get2ndTrackS(hdi); outTreePairsTracks->Fill();}
	}
    }
    
    delete [] tracks;
    return kTRUE;
}

void HRichSelector::resetTrackS(TrackS& track)
{
	track.truetrack=-1;
	track.recCorr=-1;
	track.single=-1;
	track.Double=-1;
	track.nbpart=-1;
	// maxcorrelated particle 
	track.pid1=-1;
	track.corr1=-1;
	track.orig1=-1;
	track.cpid1=-1;
	track.ctrk1=-1;
	track.mech1=-1;
	track.med1=-1;
	track.gmom1=-1;
	track.verx1=-100000;
	track.very1=-100000;
	track.verz1=-100000;
	// particle 2
	track.pid2=-1;
	track.corr2=-1;
	track.orig2=-1;
	track.cpid2=-1;
	track.ctrk2=-1;
	track.mech2=-1;
	track.med2=-1;
	track.gmom2=-1;
	track.verx2=-100000;
	track.very2=-100000;
	track.verz2=-100000;
	// reconstructed info
	track.goodCorr=-1;
	track.goodRing=-1;
	track.goodShower=-1;
	track.goodBetaMom=-1;
	track.recCharge=-5;
	track.chi2fit=-1;
	track.fake=-1;
	track.avgChrg=-1;
	track.patmat=-1;
	track.nrpads=-1;
	track.ampl=-1;
	track.centr=-1;
	track.mom=-1;
	track.tof=-1;;
	track.beta=-1;;
	track.sec=-1;
	track.ktheta=-1;
	track.kphi=-1;
	track.metatheta=-1;
	track.metaphi=-1;
	track.sys=-1;
}
void HRichSelector::resetPairS(PairS& pair)
{
	pair.mass=-1;
	pair.rap=-1;
	pair.pt=-1;
	pair.opang=-1;
	pair.charge=-5;
	pair.nbDmdc=-1;
	pair.nbDrich=-1;
	pair.nbDmeta=-1;
	pair.goodPair=-1;
	// sim
	pair.orig=-1;//0 conv/1 pidal/2 etadal
	pair.truepair=-1;//y/n
	pair.fake=-1;//code 0:trueCB, 1:fakeCB
	pair.med=-1;
	pair.faketype=-1;
	pair.betadiff=-1000;
	pair.tofdiff=-1000;
}
void HRichSelector::resetTrackE(TrackE& track)
{
	track.recCorr=-1;
	track.single=-1;
	track.Double=-1;
	track.pid=-1;
	track.verx=-100000;
	track.very=-100000;
	track.verz=-100000;
	track.goodCorr=-1;
	track.goodRing=-1;
	track.goodShower=-1;
	track.goodBetaMom=-1;
	track.recCharge=-5;
	track.chi2fit=-1;
	track.avgChrg=-1;
	track.patmat=-1;
	track.nrpads=-1;
	track.ampl=-1;
	track.centr=-1;
	track.mom=-1;
	track.tof=-1;;
	track.beta=-1;;
	track.sec=-1;
	track.ktheta=-1;
	track.kphi=-1;
	track.rtheta=-1;
	track.rphi=-1;
	track.metatheta=-1;
	track.metaphi=-1;
	track.sys=-1;
}
void HRichSelector::resetPairE(PairE& pair)
{
	pair.mass=-1;
	pair.rap=-1;
	pair.pt=-1;
	pair.opang=-1;
	pair.charge=-5;
	pair.nbDmdc=-1;
	pair.nbDrich=-1;
	pair.nbDmeta=-1;
	pair.goodPair=-1;
	pair.betadiff=-1000;
	pair.tofdiff=-1000;
}
