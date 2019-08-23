// $Id: hrichcorrelatorsim.cc,v 1.22 2009-07-23 14:52:20 halo Exp $
// Last update by Thomas Eberl: 04/08/02 14:22:26
//
#include "hrichcorrelatorsim.h"
#include "hrichcorrelatorpar.h"
#include "hrichgeometrypar.h"
#include "hrichutilfunc.h"
#include "hgeantkine.h"
#include "hgeantrich.h"
#include "hgeantmdc.h"
#include "hruntimedb.h"
#include "hevent.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hrichdetector.h"
#include "hmdcdetector.h"
#include "htofdetector.h"
#include "hshowerdetector.h"
#include "hshowertrack.h"
#include "hcategory.h"
#include "hiterator.h"
#include "hmatrixcatiter.h"
#include "hrichhit.h"
#include "hrichhitsim.h"
#include "hmdcseg.h"
#include "hmdcsegsim.h"
#include "htofhit.h"
#include "htofhitsim.h"
#include "hshowerhittof.h"
#include "tofdef.h"
#include "hmdcdef.h"
#include "showertofinodef.h"
#include "showerdef.h"
#include "hdebug.h"
#include "hades.h"
#include "richdef.h"
#include "hhitmatchsim.h"
#include "hhitmatchheadersim.h"
#include "hlinearcategory.h"
#include "kickdef.h"
#include <fstream>
#include "hgeantheader.h"
#include "hrecevent.h"
#include "hpartialevent.h"
//_HADES_CLASS_DESCRIPTION 
///////////////////////////////////////////////////////////
// HRichCorrelatorSim
//
// executes HRichCorrelator and uses additional info from GEANT 
// fills HHitMatchSim
///////////////////////////////////////////////////////////
ClassImp(HRichCorrelatorSim)

HRichCorrelatorSim::HRichCorrelatorSim(const Text_t *name,const Text_t *title) :
  HRichCorrelator(name,title)
{
     
}
HRichCorrelatorSim::HRichCorrelatorSim(const Text_t *name,const Text_t *title,const Char_t* filename, Bool_t style) :
  HRichCorrelator(name,title,filename,style)
{

    pFileName  = filename;
    isComplex = style;

}
HRichCorrelatorSim::HRichCorrelatorSim()
{

}


HRichCorrelatorSim::~HRichCorrelatorSim(void) 
{

}

Bool_t HRichCorrelatorSim::init() {
    if (gHades) {
	HEvent *event=gHades->getCurrentEvent();
	HRuntimeDb *rtdb=gHades->getRuntimeDb();
	HSpectrometer *spec=gHades->getSetup();
	if (event && rtdb) {
	    HTofDetector *tof=(HTofDetector*)spec->getDetector("Tof");
	    HShowerDetector *shower = (HShowerDetector*)spec->getDetector("Shower");
	    HMdcDetector *mdc =(HMdcDetector*)(spec->getDetector("Mdc"));
	    HRichDetector *rich = (HRichDetector*)spec->getDetector("Rich");
	    // init paramters for this task
            HRichCorrelatorPar *pCorrPar =
               (HRichCorrelatorPar*)rtdb->getContainer("RichCorrelatorParameters");
            if (!pCorrPar)
            {
                pCorrPar = new HRichCorrelatorPar;
                rtdb->addContainer(pCorrPar);
            }
            setCorrelationPar(pCorrPar);
            if (!pCorrPar) return kFALSE;

// 	    HRichGeometryPar *pGeoPar =
//                (HRichGeometryPar*)rtdb->getContainer("RichGeometryParameters");
//             if (!pGeoPar)
//             {
//                 pGeoPar = new HRichGeometryPar;
//                 rtdb->addContainer(pGeoPar);
//             }
//             setGeometryPar(pGeoPar);
//             if (!pGeoPar) return kFALSE;
	    // HGEANT KINE INFO
	    fGeantKineCat =(HLinearCategory*) event->getCategory(catGeantKine);
	    if (!fGeantKineCat) 
	    { 
		Error("HRichCorrelatorSim::init",
		      "no GEANT Kine category available");
	    }                
	    iter_kine = (HIterator *)fGeantKineCat ->MakeIterator("native");
	    // HGEANT RICH MIRROR INFO
	    fGeantRichMirrorCat = (HMatrixCategory*)(gHades->getCurrentEvent()
                             ->getCategory(catRichGeantRaw+2));  
	    if (!fGeantRichMirrorCat)
	    {
		Warning("HRichCorrelatorSim::init",
		      "no GEANT RICH MIRROR category available");
	    }
		if (fGeantRichMirrorCat) iter_mirror = (HIterator *)fGeantRichMirrorCat->MakeIterator("native");
	    // HGEANT MDC RAW INFO
	    fGeantMdcCat = (HMatrixCategory*)(gHades->getCurrentEvent()
                             ->getCategory(catMdcGeantRaw));  
	    if (!fGeantMdcCat)
	    {
		Warning("HRichCorrelatorSim::init",
		      "no GEANT MDC RAW category available");
	    }
	if (fGeantMdcCat)	iter_mdcgeant = (HIterator *)fGeantMdcCat->MakeIterator("native");
	    // HGEANT RICH RAW INFO
// 	    fGeantRichCat =(HLinearCategory*) event
// 		->getCategory(catRichGeantRaw);
// 	    if (!fGeantRichCat) 
// 	    { 
// 		Error("HRichCorrelatorSim::init",
// 		      "no GEANT RICH RAW category available");
// 	    }                
// 	    iter_geantrichraw = (HIterator *) fGeantRichCat
// 		->MakeIterator("native");
    
	    if (rich) { //Has the user set up a RICH?
		// RICH HIT container
		fRichPID=gHades->getCurrentEvent()->getCategory(catRichHit);
		if (!fRichPID) {
		    fRichPID=rich->buildMatrixCat("HRichHitSim",1);
		    //cout<<" i have built hrichhitsim "<<endl;
		    if (!fRichPID) return kFALSE;
		    else gHades->getCurrentEvent()
			     ->addCategory(catRichHit, fRichPID, "Rich");
		}
    
		fRichIter = (HIterator*) fRichPID->MakeIterator();
		if(!fRichIter) Error("init","no RichHitSim iter defined");

		pRichHitFitCat=gHades->getCurrentEvent()->getCategory(catRichHitFit);
		if (!pRichHitFitCat){
		    Warning("init()","no Rich hit fit category in input");
		}
		if (pRichHitFitCat) pRichHitFitIter = 
			(HIterator*) pRichHitFitCat->MakeIterator();

		//        //Setup output
		pHitMatchCat=event->getCategory(catMatchHit);
		if (!pHitMatchCat) {
		    pHitMatchCat=rich->buildLinearCat("HHitMatchSim");
		    // cout<<"i have built hhitmatchsim"<<endl;
		    if (!pHitMatchCat) {
			Error("init","No HIT MATCH category defined");
			return kFALSE;
		    }
		    else event->addCategory(catMatchHit, pHitMatchCat, "Rich");
		}
		
		pIterMatchHit = (HIterator*)getHitMatchCat()->MakeIterator("native");
		
		pHitMatchHeaderCat=event->getCategory(catMatchHitHeader);
		if (!pHitMatchHeaderCat) {
		    pHitMatchHeaderCat=rich->buildLinearCat("HHitMatchHeaderSim");
		    //cout<<"i have built hhitmatchheadersim"<<endl;
		    if (!pHitMatchHeaderCat) {
			Error("init","No HIT MATCH HEADER SIM category defined");
			return kFALSE;
		    }
		    else event->addCategory(catMatchHitHeader, pHitMatchHeaderCat
					    , "Rich");
		}
		
		pIterMatchHitHeader = (HIterator*)getHitMatchHeaderCat()
		    ->MakeIterator("native");
	    }
	    
	    if (mdc) { //Has the user set up a MDC?
		fMdcSeg = gHades->getCurrentEvent()->getCategory(catMdcSeg);
		if (!fMdcSeg){
		    fMdcSeg = mdc->buildMatrixCategory("HMdcSegSim",0.5F);
		    //cout<<"i have built a hmdcsegsim"<<endl;
		    if (!fMdcSeg) {
			Error("init","No MDC segment sim category defined");
			return kFALSE;
		    }
		}
		else {
		    gHades->getCurrentEvent()
			->addCategory(catMdcSeg,fMdcSeg,"Mdc");
		    fMdcSegIter=(HIterator *)fMdcSeg->MakeIterator();
		}
	    }
  	    if (tof) { //Has the user set up a TOF?
  		fTofHits=event->getCategory(catTofHit);
  		if (!fTofHits) {
  		    fTofHits=tof->buildMatrixCategory("HTofHitSim",0.5F);
		    //cout<<"i have built ftofhits"<<endl;
  		    if (!fTofHits) {
  			Error("init","No TOF input");
  			return kFALSE;
  		    }else{
			gHades->getCurrentEvent()
			    ->addCategory(catTofHit,fTofHits,"Tof");
		    }

  		}
  		fTofIter=(HIterator *)fTofHits->MakeIterator();
  	    }
	    
  	    if (shower) { //Has the user set up a Shower?

//   		  fShowerHits=event->getCategory(catShowerHit);
//   		if (!fShowerHits) {
//   		    fShowerHits = shower->buildCategory(catShowerHitTof);
//   		    if (!fShowerHits) {
//   			Error("init","No Shower input available");
//   			return kFALSE;
//  		    }
//  		}
//  		fShowerIter = (HIterator *)fShowerHits->MakeIterator();

//   		fShowerTofHits=event->getCategory(catShowerHitTof);
//    		if (!fShowerTofHits) {
//    		    fShowerTofHits = shower->buildCategory(catShowerHitTof);
//    		    if (!fShowerTofHits) {
//    			Error("init","No Shower input available");
//    			return kFALSE;
//    		    }
//    		}
//    		fShowerTofIter = (HIterator *)fShowerTofHits->MakeIterator();

		fShowerTrack=gHades->getCurrentEvent()->getCategory(catShowerTrack);
		 if (!fShowerTrack) {
		     fShowerTrack=shower->buildCategory(catShowerTrack);
		     //cout<<"i have built hshowertrack "<<endl;
		     if (!fShowerTrack) return kFALSE;
		     else gHades->getCurrentEvent()
			      ->addCategory(catShowerTrack, fShowerTrack, "Shower");
		 }
		 iter_showertrack = (HIterator*)fShowerTrack->MakeIterator();
 		fShowerTofHits=event->getCategory(catShowerHitTofTrack);
 		if (!fShowerTofHits)
 		{
 		   fShowerTofHits  = new HLinearCategory("HShowerHitTofTrack", 1000);
 		   //cout<<"i have built hshowerhittoftrack"<<endl;
 		   if (!fShowerTofHits) return kFALSE;
 		   else gHades->getCurrentEvent()
 			    ->addCategory(catShowerHitTofTrack,fShowerTofHits, "Tofino");
 		}
 		fShowerTofIter = (HIterator *)fShowerTofHits->MakeIterator();
  	    }
	    
	    // --------- Tracks from Kickplane --------------
	    fKickTrackCat = gHades->getCurrentEvent()->getCategory(catKickTrack);
	    if (!fKickTrackCat) {
		Error("init","KickTrack not found in input file");
		return kFALSE;
	    }
	    iterTracks = (HIterator *)fKickTrackCat->MakeIterator("native");
	    //cout<<"itertracks :"<<iterTracks<<endl;

	    
	  
	} else {
	    Error ("init","! (event && rtdb)");
	    return kFALSE; //! (event && rtdb)
	}
	iniCounters();
	pFileOut = new TFile(pFileName,"RECREATE");
	//cout<<"end of init()"<<endl;
	return kTRUE;
    } else {
	Error ("init","! (gHades)");
	return kFALSE; //! (gHades)		
    }
}

void HRichCorrelatorSim::iniCounters(void)
{
    HRichCorrelator::iniCounters();
    //    cout<<"in iniCountersSim"<<endl;
    ng_Part=ngGNb_RM=ngGNb_RT_S=ngGNb_MT_S=ngGNb_RMT_S=0;
    ngGNbRMonly=ngGNbRTSonly=ngGNbMTSonly=ngGNbRMTSonly=0;
    ngNb_RW=ngNb_MW=0;
    fg_RW=fg_MW=0.;
    ngNb_Fakes=0;
    ngNb_ConfTracklet=0;
    for(Int_t k=0;k<MAXPARIDS;k++) ng_ParId[k]=0;
}

Bool_t HRichCorrelatorSim::reinit()
{
    //  cout<<"in reinit function"<<endl;
    HRichCorrelatorPar*   pCorrPar = (HRichCorrelatorPar*)getCorrelationPar();
//      cout<<pCorrPar<<" is address"<<endl;
//      cout<<pCorrPar->hasChanged()<<" has changed"<<endl;
    //    HRichGeometryPar* pGeo = (HRichGeometryPar*)getGeometryPar();
    //   if (pCorrPar->hasChanged())
    //{
        iniCuts();
        iniSwitches();
        //iniControlHistos();
	//	histArray = new TObjArray(5);
	//histArray->Add(new TH1D("corr_obj_multi","corr_obj_multi",20,0,20));
        pCorrPar -> printParam();
	//}
    return kTRUE;
}

void HRichCorrelatorSim::diag(HHitMatchSim * pHM)
{
    // function that prints the found GEANT track numbers 
    // for different detector hits in a HHitMatchSim obj
    cout<<"*** HRichCorrelatorSim::diag *********************** "<<endl;
    


	HMdcSegSim *m=0;
	HRichHitSim *r=0;
	HShowerHitTof *st=0;
	HTofHitSim *t=0;
	// get objs from categories according to their index
	// which is stored in HHitMatchSim
	if(pHM->getMdcInd()>-1) m   = ((HMdcSegSim*)getMdcSegCat()
				       ->getObject(pHM->getMdcInd()));
	if(pHM->getRichInd()>-1) r  = ((HRichHitSim*)getRichHitCat()
				       ->getObject(pHM->getRichInd()));
	if(pHM->getShowInd()>-1) st = ((HShowerHitTof*)
				       getShowerTofHitCat()
				       ->getObject(pHM->getShowInd()));
	if(pHM->getTofInd()>-1) t   = ((HTofHitSim*)getTofHitCat()
				       ->getObject(pHM->getTofInd()));
	
	if(r)
	{
	    cout<<"RICH track/weight"<<endl;
	    cout<<r->track1<<"/"<<r->weigTrack1<<" ";
	    cout<<r->track2<<"/"<<r->weigTrack2<<" ";
	    cout<<r->track3<<"/"<<r->weigTrack3<<endl;
	}
	if(m)
	{ 
	    Int_t nNbMdcSegTr=m->getNTracks();
	    cout<<"MDC track/weight"<<endl;
	    for (Int_t i=0;i<nNbMdcSegTr;i++)
	    {
		cout<<m->getTrack(i)<<"/"<<(Int_t)m->getNTimes(i)<<" ";
	    }
	    cout<<endl;
	}
	if(st)
	{
	    cout<<"SHOWER track/"<<endl;
	    for (Int_t i=0;i<SHOWERMAXTRACKS;i++)
	    {
		cout<<showerTracks[i]<<"/"<<"   ";
	    }
	    cout<<endl;
	}
	
	if(t)
	{
	    cout<<"TOF track/"<<endl; 
	    cout<<t->getNTrack1()<<"/"<<"    ";
	    cout<<t->getNTrack2()<<"/"<<"    ";
	    cout<<endl;
	}
    
}

Int_t HRichCorrelatorSim::execute()
{

//      cout<<"%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% "
//     <<nCounterProcessedNbEvents<<endl;
    if (!HRichCorrelator::execute()) //treat sim data like exp data
	                            //execute correlation search w/o 
	                            //GEANT info first 
    {// now add info from GEANT to the found corr objs
	HHitMatchSim *pHitMatch=0;
	pIterMatchHit->Reset();
	// loop over corr objs in category
	while((pHitMatch = (HHitMatchSim *)pIterMatchHit->Next()))
	{
	    //get pointer to track info obj inside HHitMatchSim
	    //this obj stores all additional info from GEANT
	    (pHitMatch->getTrackInfoObj())->reset();
	    cleanTracks();//clean track helper arrays
	    // store the GEANT track numbers of the detector hits
	    // in temp helper arrays
	    if(pHitMatch->getRichInd()>=0) assignRichTracks(pHitMatch->getRichInd());
	    if(pHitMatch->getMdcInd()>=0) assignMdcTracks(pHitMatch->getMdcInd()); 
	    if(pHitMatch->getShowInd()>=0) assignShowerTrack(pHitMatch->getShowInd());
	    if(pHitMatch->getTofInd()>=0) assignTofTracks(pHitMatch->getTofInd());
	    // try to match the GEANT track numbers of the different detector hits
	    matchTracks(pHitMatch);
	    if ((richTracks[0] == -5 || richTracks[0] == -1) && richTracks[1] >0) 
	    {
		//cout<<"ALERT, ALERT ******************************************"
		//  <<nCounterProcessedNbEvents<<endl;
		//diag(pHitMatch);
		//pHitMatch->dumpToStdoutSim();
		//cout<<"////////////////////////////////////////////////////////////////////////"<<endl;
	    }

	}
   
    // now fill one header object per event
	// check for fakes
	HHitMatchHeaderSim *pHMH = 0;
	pIterMatchHitHeader->Reset();
	//Int_t nnn=0;
	while((pHMH = (HHitMatchHeaderSim *)pIterMatchHitHeader->Next()))
	{
	    pHMH->resetSim();
	    //cout<<"evt nb "<<nCounterProcessedNbEvents<<" pHMH # "<<nnn<<endl;
	    //nnn++;
	    HHitMatchSim *pHitMatch=0;
	    pIterMatchHit->Reset();
	    Int_t nMaxParticles=pHMH->getNbCorrObjs()*MAXPARTICLES;
	    Int_t *nDParticles=new Int_t[nMaxParticles];
	    for(Int_t i=0;i<nMaxParticles;i++) nDParticles[i]=-2;
	    // local (for this evt) counter variables
	    Int_t nl_Part=0;
	    Int_t nl_RM=0;
	    Int_t nl_RT_S=0;
	    Int_t nl_MT_S=0;
	    Int_t nl_RMT_S=0;
	    Int_t nGNbRMonly=0;
	    Int_t nGNbRTSonly=0;
	    Int_t nGNbMTSonly=0;
	    Int_t nGNbRMTSonly=0;
	    Float_t fl_RW=0.;
	    Float_t fl_MW=0.;
	    Int_t nlNb_Fakes=0;
	    Int_t nlNb_ConfTracklet=0;
	    Int_t nl_ParId[MAXPARIDS];
	    for(Int_t k=0;k<MAXPARIDS;k++) nl_ParId[k]=0;


	    // loop over corr objs in category
	    while((pHitMatch = (HHitMatchSim *)pIterMatchHit->Next()))
	    {   
		HTrackInfo *t = pHitMatch->getTrackInfoObj();
		pHitMatch->resetSim();
		// check if ring is made by lepton
		pHitMatch->setGLepRing(isGRing(pHitMatch));
		// check if ring is made by lepton which is seen
		// by other subdetectors
		pHitMatch->setGCLepRing(isGCorrRing(t));
		// number of different GEANT particles stored in
		// tracklet that were seen by at least by 2 subdet
		pHitMatch->setNbGPart(t->getPartNr());
		//check if tracklet contains
		//a ring that stems from a lepton that can be further 
		//correlated and went through the mirror
		//		Int_t nMatchedMirrorRingTrkNb = isLepOnMirror(pHitMatch);
		Int_t nMatchedMirrorRingTrkNb = -3;
		//check if particle was seen by GEANT in first layer of MDC
		//meaning that it didn't perish somewhere in the Rich
		if (nMatchedMirrorRingTrkNb>-1) isLepOnMDC(pHitMatch,nMatchedMirrorRingTrkNb);
		Int_t isFirstGeantNbInTracklet=1;
		for(Int_t i=0;i<MAXPARTICLES;i++)
		{
		    Int_t nTrkNb=t->getTrkNb(i);
		    if (nTrkNb>-1)
		    {
			if(isFirstGeantNbInTracklet) nlNb_ConfTracklet++;
			// two tracklets can contain the same GEANT particle !
			isFirstGeantNbInTracklet=0;
			pHitMatch->setFakeFlag(0);
			Int_t nParId=t->getParId(i);
			if(!pHitMatch->isFake() && (nParId==2 || nParId==3) )
			{
			    pHitMatch->setLeptonFlag(1);
			}
			Int_t index=fillUniqueIndex(nTrkNb,
						    nMaxParticles,
						    nDParticles);
			if(index>0)
			{//it is a new GEANT particle in this evt!!
			    // index was incremented by 1 in fillUnique....!
			    // only tracklets containing different GEANT
			    // particles are counted here !!!
			    
			    //Int_t nParId=t->getParId(i);
			    // cout<<"it's a new particle: trk: "<<nTrkNb
// 				<<" part id: "<<nParId<<endl;
			    Float_t fRW=t->getRichWeight(i);
			    Float_t fMW=t->getMdcWeight(i);
			    Int_t nMatchRM=t->getMatchedRM(i);
			    Int_t nMatchRT_S=t->getMatchedRT_S(i);
			    Int_t nMatchMT_S=t->getMatchedMT_S(i);
			    Int_t nMatchRMT_S=t->getMatchedRMT_S(i);
			    nl_Part++;// count new part in this evt
			    if(nParId<MAXPARIDS) nl_ParId[nParId]+=1;
			    if(fRW>-1) fl_RW+=fRW;
			    if(fMW>-1) fl_MW+=fMW;
			    if(nMatchRM    != -1) nl_RM++;
			    if(nMatchRT_S  != -1) nl_RT_S++;
			    if(nMatchMT_S  != -1) nl_MT_S++;
			    if(nMatchRMT_S != -1) nl_RMT_S++;
			    if(nMatchRM!=-1   && nMatchRT_S==-1 &&
			       nMatchMT_S==-1 && nMatchRMT_S==-1  ) nGNbRMonly++;
			    if(nMatchRM==-1   && nMatchRT_S!=-1 &&
			       nMatchMT_S==-1 && nMatchRMT_S==-1  ) nGNbRTSonly++;
			    if(nMatchRM==-1   && nMatchRT_S==-1 &&
			       nMatchMT_S!=-1 && nMatchRMT_S==-1  ) nGNbMTSonly++;
			    if(nMatchRM!=-1   && nMatchRT_S!=-1 &&
			       nMatchMT_S!=-1 && nMatchRMT_S!=-1  ) nGNbRMTSonly++;
			}// endif new particle
		    }// endif valid trk nb
		}//endfor particle in track info object
		if(pHitMatch->isFake()) nlNb_Fakes++;


		// DIAGNOSTICS
// 		diagOut(pHitMatch,t);
		// -------------------

	    }// end while hhitmatchsim



	    //Int_t nParttmp=countDiscreteIndexes(nDParticles,nMaxParticles);
	    delete nDParticles;
	    if(nl_Part>0)
	    {
		fl_RW/=nl_Part;//avrg Rich weight
		fl_MW/=nl_Part;//avrg Mdc weight
	    }
	    pHMH->setNbPart(nl_Part);
	    ng_Part+=nl_Part;
	    ngNb_ConfTracklet+=nlNb_ConfTracklet;
	    pHMH->setNbConfTracklets(nlNb_ConfTracklet);
	    ngNb_Fakes+=nlNb_Fakes;
	    pHMH->setNbFakes(nlNb_Fakes);
	    pHMH->setAvrgRichWeight(fl_RW);
	    if(fl_RW>0.)
	    {
		fg_RW+=fl_RW;//add avg Rich weight for this evt
		ngNb_RW++;//count that there was a particle in Rich
	    }
	    pHMH->setAvrgMdcWeight(fl_MW);
	    if(fl_MW>0.)
	    {
		fg_MW+=fl_MW;
		ngNb_MW++;
	    }
	    for(Int_t n=0;n<MAXPARIDS;n++)
	    {
		ng_ParId[n]+=nl_ParId[n];
		if(nl_ParId[n]>0) pHMH->setParId(n,nl_ParId[n]);
	    }

// 	    pHMH->setGeantNb_RM(nl_RM);
// 	    ngGNb_RM+=nl_RM;
// 	    pHMH->setGeantNb_RT_S(nl_RT_S);
// 	    ngGNb_RT_S+=nl_RT_S;
// 	    pHMH->setGeantNb_MT_S(nl_MT_S);
// 	    ngGNb_MT_S+=nl_MT_S;
// 	    pHMH->setGeantNb_RMT_S(nl_RMT_S);
// 	    ngGNb_RMT_S+=nl_RMT_S;

	    pHMH->setGeantNb_RM(nGNbRMonly);
	    ngGNbRMonly+=nGNbRMonly;
	    pHMH->setGeantNb_RT_S(nGNbRTSonly);
	    ngGNbRTSonly+=nGNbRTSonly;
	    pHMH->setGeantNb_MT_S(nGNbMTSonly);
	    ngGNbMTSonly+=nGNbMTSonly;
	    pHMH->setGeantNb_RMT_S(nGNbRMTSonly);
	    ngGNbRMTSonly+=nGNbRMTSonly;

	    HRecEvent* pGeantEvent       = (HRecEvent*)(gHades->getCurrentEvent());
	    HPartialEvent* pPartialEvent = pGeantEvent->getPartialEvent(catSimul);
	    HGeantHeader* pGeantHeader   = (HGeantHeader*)(pPartialEvent->getSubHeader());

	    pHMH->setImpactParam(pGeantHeader->getImpactParameter());

	    //pHMH->dumpToStdoutSim();
	}//this hitmatch header, there is only one !
    }//end of base execute

    //    cout<<"%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% "<<endl;
    return 0;
}
void HRichCorrelatorSim::diagOut(HHitMatchSim* pHitMatch,HTrackInfo* t)
{
    cout<<"%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% "
	<<nCounterProcessedNbEvents<<endl;
    pHitMatch->dumpToStdoutSim();
    diag(pHitMatch);
    //    t->dumpToStdout();
    for(Int_t i=0;i<MAXPARTICLES;i++)
    {
	if (t->getTrkNb(i)>-1)
	{
	    HRichUtilFunc::dumpKine(HRichUtilFunc::
				    getKineObj(t->getTrkNb(i),
					       (HLinearCategory*)
					       getGeantKineCat()));
	}
    }
    cout<<"%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% "<<endl;
}
void HRichCorrelatorSim::cleanTracks()
{
    // temp helper arrays to hold GEANT track numbers and their weights
    // need to be reset as they are data members
    // GEANT starts track numbers at 1, therefore init them to 0
    for(Int_t i = 0;i<RICHMAXTRACKS;i++){
	richTracks[i] = 0;
	richWeights[i] = 0;
    }
    for(Int_t i = 0;i<MDCMAXTRACKS;i++){
	mdcTracks[i] = 0;
	mdcWeights[i] = 0;
    }

    for(Int_t i = 0;i<TOFMAXTRACKS;i++) tofTracks[i]=0;

    for(Int_t i = 0;i<SHOWERMAXTRACKS;i++) showerTracks[i] = 0;
    
}
void  HRichCorrelatorSim::assignRichTracks(Int_t richInd)
{
    // copy GEANT track info from the hit  
    HRichHitSim *pRichHit = ((HRichHitSim*)getRichHitCat()
			     ->getObject(richInd));
    
    richTracks[0] = pRichHit->track1;
    richWeights[0] = pRichHit->weigTrack1;
    richTracks[1] = pRichHit->track2;
    richWeights[1] = pRichHit->weigTrack2;
    richTracks[2] = pRichHit->track3;
    richWeights[2] = pRichHit->weigTrack3;
}

void  HRichCorrelatorSim::assignMdcTracks(Int_t mdcInd)
{
    // according to info from Vladimir Pechenov
    // copy GEANT track info from the hit
    HMdcSegSim *pMdcSeg = ((HMdcSegSim*)getMdcSegCat()
			   ->getObject(mdcInd));
  
    for(Int_t i =0;i<pMdcSeg->getNTracks();i++)
    {
	mdcTracks[i] = pMdcSeg->getTrack(i);
	mdcWeights[i] = (Int_t)pMdcSeg->getNTimes(i);
    }
    
}

void HRichCorrelatorSim::assignShowerTrack(Int_t showerInd)
{
    // look in HShowerTrack and HShowerHitTof
    // and match the info
    // basically what is done in HShowerHitTofMatcher
    // but there for each GEANT track number one clone is created
    // which is not useful here --- > we do it ourselves
    HShowerTrack *pTrack=0;
    iter_showertrack->Reset();
    HShowerHitTof * pHit = ((HShowerHitTof*)  getShowerTofHitCat()
					  ->getObject(showerInd));
    pTrack = (HShowerTrack *)iter_showertrack->Next();

    do {
	if (pTrack && pHit->getAddress()==pTrack->getAddress()) { 
	    *showerTracks = pTrack->getTrack(); 
	    break; 
	}
    }
    while((pTrack = (HShowerTrack *)iter_showertrack->Next())) ;

    //searching other tracks with the same address number
    Int_t i=0;
    while((pTrack = (HShowerTrack *)iter_showertrack->Next())) {
       if (i<SHOWERMAXTRACKS-1 && pHit->getAddress()==pTrack->getAddress()) { 
	  showerTracks[i++] = pTrack->getTrack();
       } else break;
    }

}

void HRichCorrelatorSim::assignTofTracks(Int_t tofInd)
{

    HTofHitSim *pTofHit = ((HTofHitSim*)getTofHitCat()
			   ->getObject(tofInd));
	    tofTracks[0] = pTofHit ->getNTrack1();
	    tofTracks[1] = pTofHit ->getNTrack2();
}
void  HRichCorrelatorSim::matchTracks(HHitMatchSim *pHitMatch)
{
    
    Bool_t kRICH   = kFALSE;
    Bool_t kMDC    = kFALSE;
    Bool_t kTOF    = kFALSE;
    Bool_t kSHOWER = kFALSE;
    for(Int_t i = 0;i<RICHMAXTRACKS;i++)
    {
	if (richTracks[i] > 0) {kRICH=kTRUE; break;}
    }

    for(Int_t i = 0;i<MDCMAXTRACKS;i++)
    {
	if (mdcTracks[i] > 0) {kMDC=kTRUE; break;}
    }

    for(Int_t i = 0;i<TOFMAXTRACKS;i++)
    {
	if (tofTracks[i] > 0) {kTOF=kTRUE; break;}
    }

    for(Int_t i = 0;i<SHOWERMAXTRACKS;i++)
    {
	if (showerTracks[i] > 0) {kSHOWER=kTRUE; break;}
    }


    // case RICH-MDC only
    if(kRICH && kMDC && !kSHOWER && !kTOF)
    {
	corrTracksRM(pHitMatch);
    }
    // case MDC and TOF or SHOWER
    if(!kRICH && kMDC &&  (kSHOWER || kTOF) )
    {
	corrTracksMT_S(pHitMatch);
    }
    // case RICH and TOF or SHOWER
    if(kRICH && !kMDC && (kSHOWER || kTOF) )
    {
	corrTracksRT_S(pHitMatch);
    }
    // case RICH - MDC and TOF or SHOWER
    if(kRICH && kMDC && (kSHOWER || kTOF) )
    {
	corrTracksRM(pHitMatch);
	corrTracksRT_S(pHitMatch);
	corrTracksMT_S(pHitMatch);
	evalBiCorrs(pHitMatch);
    }
}
void HRichCorrelatorSim::evalBiCorrs(HHitMatchSim *pHitMatch)
{
    // all possible RICH MDC META bi correlations are done
    // now check whether there is a tri correlation of GEANT track numbers
    HTrackInfo* t = pHitMatch->getTrackInfoObj();
    for (Int_t i=0;i<MAXPARTICLES;i++)
    {
	Int_t nRM   = t->getMatchedRM(i);
	Int_t nRT_S = t->getMatchedRT_S(i);
	Int_t nMT_S = t->getMatchedMT_S(i);
	if (nRM==1 && nMT_S==1 && nRT_S==1) t->setMatchedRMT_S(i);
    }

}
void HRichCorrelatorSim::corrTracksRT_S(HHitMatchSim *pHitMatch)
{
    // look if we find the same GEANT track number for 
    // the rich hit and the hit in shower or tof 
    HTrackInfo* t = pHitMatch->getTrackInfoObj();
    for (Int_t i=0;i<RICHMAXTRACKS;i++)
    {
	    for (Int_t j=0;j<SHOWERMAXTRACKS;j++)
	    {
		// store same GEANT nb found in RICH Hit and SHOWER Hit 
		if(richTracks[i]>0 && richTracks[i] == showerTracks[j])
		{
	  
		    setGeantTrackInfoRT_S(i,t);
		    break;
		}
	    }

	    for (Int_t j=0;j<TOFMAXTRACKS;j++)
	    {
		if(richTracks[i]>0 && richTracks[i] == tofTracks[j])
		{
		    setGeantTrackInfoRT_S(i,t);
		    break;
		}
	    }
    }
}
void HRichCorrelatorSim::corrTracksMT_S(HHitMatchSim *pHitMatch){

    HTrackInfo* t = pHitMatch->getTrackInfoObj();
    for (Int_t i=0;i<MDCMAXTRACKS;i++)
    {
	    for (Int_t j=0;j<SHOWERMAXTRACKS;j++)
	    {
		if(mdcTracks[i]>0 && mdcTracks[i] == showerTracks[j])
		{
		    setGeantTrackInfoMT_S(i,t);
		    break;
		}
	    }

	    for (Int_t j=0;j<TOFMAXTRACKS;j++)
	    {
		if(mdcTracks[i]>0 && mdcTracks[i] == tofTracks[j])
		{
		    setGeantTrackInfoMT_S(i,t);
		    break;
		}
	    }
    }
}

void HRichCorrelatorSim::corrTracksRM(HHitMatchSim* pHitMatch)
{
    HTrackInfo* t = pHitMatch->getTrackInfoObj();
    for (Int_t i=0;i<RICHMAXTRACKS;i++)
    {
	for (Int_t j=0;j<MDCMAXTRACKS;j++)
	{
	    if(richTracks[i]>0 && richTracks[i] == mdcTracks[j]) 
	    {
		setGeantTrackInfoRM(i,j,t);
		break;
	    }
	}
    }
}
Float_t HRichCorrelatorSim::calcRichWeight(Int_t ind)
{
    // use the number of fired pads in a ring per tracknumber as 
    // a confidence value for this track number
    Float_t sumWeightsRich=0.;
    for(Int_t i=0;i<RICHMAXTRACKS;i++) sumWeightsRich += richWeights[i];
    Float_t f = (Float_t) richWeights[ind];
    return f/sumWeightsRich;
}
Float_t HRichCorrelatorSim::calcMdcWeight(Int_t ind)
{
    // use number of drift times that have contributed to this segment
    // and that come from one GEANT number as confidence value
    Float_t sumWeightsMdc=0.;
    for(Int_t j =0;j<MDCMAXTRACKS;j++) sumWeightsMdc += mdcWeights[j];
    Float_t f = (Float_t) mdcWeights[ind];
    return f/sumWeightsMdc;
}
void HRichCorrelatorSim::setGeantTrackInfoRT_S(Int_t index,HTrackInfo *t)
{
    //check if geant nb is already stored
    //we store only a maximum of MAXPARTICLES particles, 
    //the rest is discarded
    Int_t trknb = richTracks[index];
    if(trknb<=0) return;
    Int_t newSlot=-1;
    for (Int_t i=0;i<MAXPARTICLES;i++)
    {
	if(t->getTrkNb(i)<0) //assumes init of array elements to -1;
	{// array element i is still -1, we can use it for a new particle
	    newSlot=i;
	    break;
	}
	if(t->getTrkNb(i) == trknb) 
	{// GEANT nb is already stored in Trackinfo
	    t->setMatchedRT_S(i);
	    if(t->getRichWeight(i)==-1) t->setRichWeight(i,calcRichWeight(index));
	    newSlot=-1;
	    break;
	}
    }
    // newSlot is either -1 (nothing to be done)
    // or i<MAXPARTICLES indicating a new as yet unused slot in the particle array
    if (newSlot>=0) //insert new Geant Nb at newSlot
    {
	HGeantKine* kine=(HGeantKine*)HRichUtilFunc::getKineObj(trknb,(HLinearCategory*)getGeantKineCat());
	if (kine==0)
	{
	    Warning("setGeantTrackInfoRT_S","track number stored in rich hit not found in GEANT kine container");
	    cout<<"track number that creates problem: "<<trknb<<endl;
	    return;
	}

	Int_t aTrack, aID;
	Int_t aPar, aMed, aMech;
	Float_t ptot;
	kine->getParticle(aTrack,aID);
	kine->getCreator(aPar,aMed,aMech);
	ptot=kine->getTotalMomentum();
	if (aTrack!=trknb) Error("HRichCorrelatorSim::setGeantTrackInfoRT_S","track numbers not equal");
	t->setTrkNb(newSlot,trknb);
	t->setParId(newSlot,aID);
	t->setMech(newSlot,aMech);
	t->setCreatorId(newSlot,HRichUtilFunc::getParentParID(kine,(HLinearCategory*)getGeantKineCat()));
	t->setCreatorTrkNb(newSlot,aPar);
	t->setMed(newSlot,aMed);
	Float_t xvert,yvert,zvert;
	kine->getVertex(xvert,yvert,zvert);
	t->setVertx(newSlot,xvert);
	t->setVerty(newSlot,yvert);
	t->setVertz(newSlot,zvert);
	if (aID==2 || aID==3) 
	{
	    t->setPi0Dalitz(newSlot,isPi0Dalitz(trknb));
	    t->setConvLep(newSlot,isConvLep(trknb));
	}
	t->setTotMom(newSlot,ptot);
	t->setRichWeight(newSlot,calcRichWeight(index));
	t->setMatchedRT_S(newSlot);
    }
}
void HRichCorrelatorSim::setGeantTrackInfoMT_S(Int_t index,HTrackInfo *t)
{
    //check if geant nb is already stored
    Int_t trknb = mdcTracks[index];
    Int_t newSlot=-1;
    for (Int_t i=0;i<MAXPARTICLES;i++)
    {
	if(t->getTrkNb(i)<0) //assumes init of array elements to -1;
	{
	    newSlot=i;
	    break;
	}
	if(t->getTrkNb(i) == trknb) 
	{
	    t->setMatchedMT_S(i);
	    if(t->getMdcWeight(i)==-1) t->setMdcWeight(i,calcMdcWeight(index));
	    newSlot=-1;
	    break;
	}
    }
    if (newSlot>=0) //insert new Geant Nb 
    {
	HGeantKine* kine=(HGeantKine*)HRichUtilFunc::getKineObj(trknb,(HLinearCategory*)getGeantKineCat());
	Int_t aTrack, aID;
	Int_t aPar, aMed, aMech;
	Float_t ptot;
	kine->getParticle(aTrack,aID);
	kine->getCreator(aPar,aMed,aMech);
	ptot=kine->getTotalMomentum();
	if (aTrack!=trknb) Error("HRichCorrelatorSim::setGeantTrackInfoRT_S","track numbers not equal");

	t->setTrkNb(newSlot,trknb);
	t->setParId(newSlot,aID);
	t->setMech(newSlot,aMech);
	t->setCreatorId(newSlot,HRichUtilFunc::getParentParID(kine,(HLinearCategory*)getGeantKineCat()));
	t->setCreatorTrkNb(newSlot,aPar);
	Float_t xvert,yvert,zvert;
	kine->getVertex(xvert,yvert,zvert);
	t->setVertx(newSlot,xvert);
	t->setVerty(newSlot,yvert);
	t->setVertz(newSlot,zvert);
	t->setMed(newSlot,aMed);
	if (aID==2 || aID==3) 
	{
	    t->setPi0Dalitz(newSlot,isPi0Dalitz(trknb));
	    t->setConvLep(newSlot,isConvLep(trknb));
	}
	t->setTotMom(newSlot,ptot);
	t->setMdcWeight(newSlot,calcMdcWeight(index));
	t->setMatchedMT_S(newSlot);
    }
}
void HRichCorrelatorSim::setGeantTrackInfoRM(Int_t richindex,Int_t mdcindex,
					     HTrackInfo *t)
{
    //this func needs two indexes as we need to find the weight for both rich and mdc
    //check if geant nb is already stored
    Int_t trknb = richTracks[richindex];
    if (trknb<0) return;
    Int_t newSlot=-1;
    for (Int_t i=0;i<MAXPARTICLES;i++)
    {
	if(t->getTrkNb(i)<0) //assumes init of array elements to -1;
	{
	    newSlot=i;
	    break;
	}
	if(t->getTrkNb(i) == trknb) 
	{
	    t->setMatchedRM(i);
	    if(t->getMdcWeight(i)==-1) t->setMdcWeight(i,calcMdcWeight(mdcindex));
	    if(t->getRichWeight(i)==-1) t->setRichWeight(i,calcRichWeight(richindex));
	    newSlot=-1;
	    break;
	}
    }
    if (newSlot>=0) //insert new Geant Nb 
    {
	HGeantKine* kine=(HGeantKine*)HRichUtilFunc::getKineObj(trknb,(HLinearCategory*)getGeantKineCat());

	if (kine==0)
	{
	    Warning("setGeantTrackInfoRT_S","track number stored in rich hit not found in GEANT kine container");
	    cout<<"track number that creates problem: "<<trknb<<endl;
	    return;
	}

	Int_t aTrack, aID;
	Int_t aPar, aMed, aMech;
	Float_t ptot;
	kine->getParticle(aTrack,aID);
	kine->getCreator(aPar,aMed,aMech);
	ptot=kine->getTotalMomentum();
	if (aTrack!=trknb) Error("HRichCorrelatorSim::setGeantTrackInfoRT_S","track numbers not equal");

	t->setTrkNb(newSlot,trknb);
	t->setParId(newSlot,aID);
	t->setMech(newSlot,aMech);
	t->setCreatorId(newSlot,HRichUtilFunc::getParentParID(kine,(HLinearCategory*)getGeantKineCat()));
	t->setCreatorTrkNb(newSlot,aPar);
	Float_t xvert,yvert,zvert;
	kine->getVertex(xvert,yvert,zvert);
	t->setVertx(newSlot,xvert);
	t->setVerty(newSlot,yvert);
	t->setVertz(newSlot,zvert);
	t->setMed(newSlot,aMed);
	if (aID==2 || aID==3) 
	{
	    t->setPi0Dalitz(newSlot,isPi0Dalitz(trknb));
	    t->setConvLep(newSlot,isConvLep(trknb));
	}
	t->setTotMom(newSlot,ptot);
	t->setRichWeight(newSlot,calcRichWeight(richindex));
	t->setMdcWeight(newSlot,calcMdcWeight(mdcindex));
	t->setMatchedRM(newSlot);
    }
}

Int_t HRichCorrelatorSim::isPi0Dalitz(Int_t trk)
{
    // returns 1 if track nb corresponds to a lepton from pi0 Dalitz decay
    // else returns zero

     HGeantKine * kine =0;
    // loop over kine container
    iter_kine->Reset();
    while((kine=(HGeantKine *)iter_kine->Next())!=0)
    {
	Int_t aTrack, aID;
	Int_t aPar, aMed, aMech;
	Float_t ptot;
	kine->getParticle(aTrack,aID);
	kine->getCreator(aPar,aMed,aMech);
	ptot=kine->getTotalMomentum();
	if (aTrack==trk)
	{
	    if ( aMech==5 && (aID == 2 || aID==3) && (aMed>=8 && aMed<=19)) // direct particle decay
	    {
		HGeantKine *kine_parent1 = HRichUtilFunc::findParent(kine,(HLinearCategory*) getGeantKineCat());
		if(kine_parent1){//parent should be pion
		    //  		Float_t theta1,phi1;
		    //  		theta1=phi1=0.;
		    //  		HRichUtilFunc::calcParticleAnglesKine(kine_parent1,theta1,phi1);
		    Int_t aTrackp1, aIDp1;
		    kine_parent1->getParticle(aTrackp1,aIDp1);
		    //if(aIDp1==7 && (theta1>=10. && theta1<=90.))//pion
		    if(aIDp1==7)//pion, but no emission angle cut
		    {
			return 1;
		    }
		    else return 0;
		    
///////////////////////////////////////////////////////////////////////////
//                    get Second Pion Dalitz Lepton
///////////////////////////////////////////////////////////////////////////
//  		{ //in acceptance
//  		    HGeantKine *secondlepton = 
//  			HRichUtilFunc::getSecondPionDalitzLepton(kine,(HLinearCategory*) getGeantKineCat());
//  		    if (secondlepton)
//  		    {
//  			Float_t deltaptot = 
//  			    TMath::Abs(ptot-secondlepton->getTotalMomentum());
//  			Float_t opangle = 
//  			    HRichUtilFunc::calcOpeningAngleKine(kine,secondlepton);
//  			deltaP_opangle->Fill(deltaptot,opangle);
//  		    }
//  		    med_dalitz->Fill(aMed);
//  		    mech_dalitz->Fill(aMech);
//  		    par_dalitz->Fill(aIDp1);
//  		    par_med_dalitz->Fill(aID,aMed);
//  		    par_mech_dalitz->Fill(aID,aMech);
//  		    mech_mom_dalitz->Fill(aMech,ptot);
//  		}//pi0
///////////////////////////////////////////////////////////////////////////



		}//kine_parent1
	    }//Mec==5 and lepton
	    //	    if (kDumpIt) dumpKine(kine);
	    //  if (kDumpIt) dumpKine(findParent(kine));
	}
    } // end while kine loop
    
    return 0;    
}

Int_t HRichCorrelatorSim::isConvLep(Int_t trk)
{
    // returns 1 if track nb corresponds to a lepton from gamma conv
    // else returns zero

      HGeantKine * kine =0;
    // loop over kine container
    iter_kine->Reset();
    while((kine=(HGeantKine *)iter_kine->Next())!=0)
    {
	Int_t aTrack, aID;
	Int_t aPar, aMed, aMech;
	Float_t ptot;
	kine->getParticle(aTrack,aID);
	kine->getCreator(aPar,aMed,aMech);
	ptot=kine->getTotalMomentum();

	if (aTrack==trk)
	{

	    if ( aMech==6 && (aID == 2 || aID==3) && (aMed>=8 && aMed<=19)) // photon pair production in target or Rich
	    {
		HGeantKine *kine_parent1 = HRichUtilFunc::findParent(kine,(HLinearCategory*) getGeantKineCat());
		if(kine_parent1){
		    
		    Int_t aTrackp1, aIDp1;
		    Int_t aParp1,aMedp1,aMechp1;
		    kine_parent1->getParticle(aTrackp1,aIDp1);
		    kine_parent1->getCreator(aParp1,aMedp1,aMechp1);
		    if(aIDp1==1 && aMechp1==5)//gamma
		    {
			
			HGeantKine *kine_parent2 = HRichUtilFunc::findParent(kine_parent1,(HLinearCategory*) getGeantKineCat());
			if(kine_parent2){
			    Int_t aTrackp2, aIDp2;
			    kine_parent2->getParticle(aTrackp2,aIDp2);
			    //  Float_t theta2,phi2;
			    //  			theta2=phi2=0.;
			    //  			HRichUtilFunc::calcParticleAnglesKine(kine_parent2,theta2,phi2);
			    //if(aIDp2==7 && (theta2>=10. && theta2<=90.))
			    if(aIDp2==7)
			    {//pi0 in acceptance
				return 1;
			    }
			    else return 0;
///////////////////////////////////////////////////////////////////////////
//                    get Second Pion Decay Gamma
///////////////////////////////////////////////////////////////////////////
//  			    HGeantKine *secondgamma = HRichUtilFunc::getSecondPionDecayGamma(kine_parent1,(HLinearCategory*) getGeantKineCat());
			    
//  			    if (secondgamma) g_conv_opangle->Fill(HRichUtilFunc::calcOpeningAngleKine(kine_parent1,secondgamma));

//  			    med_conv->Fill(aMed);
//  			    mech_conv->Fill(aMech);
//  			    par_conv->Fill(aIDp2);
//  			    par_med_conv->Fill(aID,aMed);
//  			    par_mech_conv->Fill(aID,aMech);
//  			    mech_mom_conv->Fill(aMech,ptot);
			    // }//pi0
////////////////////////////////////////////////////////////////////////////



			}//kine_parent2
		    }//gamma
		}//kine_parent1
	    }//Mec==6 and lepton
	    //	    if (kDumpIt) dumpKine(kine);
	    //  if (kDumpIt) dumpKine(findParent(kine));
	} // end if trk nb comparison
    } // end while kine loop
    
    return 0;
}
Int_t HRichCorrelatorSim::isLepOnMirror(HHitMatchSim *h)
{
    Int_t RingCanBeInCorrelation=-1;//return value
    HGeantRichMirror *mirr=0;
    iter_mirror->Reset();
    HRichGeometryPar* pGeo = (HRichGeometryPar*) getGeometryPar();
    Float_t fYShift = pGeo->getSectorShift(); // to account for shifted volumes in HGEANT ?!
    fYShift = fYShift/TMath::Cos(20./57.3);
    HRichPadTab* pPadsTab = ((HRichGeometryPar*)pGeo)->getPadsPar();
    HRichPad *pPad = 0;
    // x,y in pad units of ring recognized on padplane 
    HRichHitSim* r=0;
    Int_t lep1Count = 0;
    Int_t lep2Count = 0;
    if(h->getRichInd()>-1) r  = ((HRichHitSim*)getRichHitCat()
				 ->getObject(h->getRichInd()));
    else return -1;//no rich hit in tracklet
    if (r)
    {
	Int_t xRing=r->getRingCenterX();
	Int_t yRing=r->getRingCenterY();
	//	cout<<"  ++++++++++++++++++++++++++++++++++++++++++"<<endl;
	//cout<<"  ring X "<<r->getRingCenterX()<<"  ring Y  "<<r->getRingCenterY()<<endl;
	Int_t secRing=r->getSector();
	Float_t weight1 =0.;
	Float_t weight2 =0.;
	Float_t weightTemp = 0.;
	// iterate over hits on mirror and compare hit ring x,y with
	// center of reflected photons, calculate pad hit by photon ctr
	while((mirr=(HGeantRichMirror *)iter_mirror->Next())!=0)
	{
	    Int_t aTrk,aID;
	    mirr->getTrack(aTrk,aID);//tracknumber and pid from HGeant
	    //    cout<<" mirror track "<<aTrk<<endl;
	    weightTemp = 0;
	    weightTemp = matchRingMirrtracks(r,aTrk,aID);
	    if(weightTemp>0){
		Float_t x=mirr->getYRing()/10.;//in mm of the padplane
		Float_t y=mirr->getXRing()/10. + fYShift;
		pPad=pPadsTab->getPad(x, // they are swapped, sic !
				      y );// divide by 10 to account for mm->cm
		if (!pPad) return -1;
		Int_t sec=mirr->getSector();
		Int_t xMirrPhotCM=pPad->getPadX();
		Int_t yMirrPhotCM=pPad->getPadY();
		//		cout<<"  mirror X "<<xMirrPhotCM<<"  mirror Y "<<yMirrPhotCM<<endl;
		Int_t phot=mirr->getNumPhot();
		//cout<<" number of photons   "<<phot<<endl;
		if ((TMath::Abs(xMirrPhotCM-xRing)<2 && 
		      TMath::Abs(yMirrPhotCM-yRing)<2 && 
		      sec==secRing) && lep1Count==0){
		    RingCanBeInCorrelation=aTrk;
		    h->setLeptonOnMirror(1);
		    h->setNumPhot1(phot);
		    weight1 = weightTemp;
		    lep1Count++;
		    HTrackInfo* t=(HTrackInfo*) h->getTrackInfoObj();
		    for (Int_t i=0;i<MAXPARTICLES;i++)
		    {
			if( t->getTrkNb(i)>-1 && t->getTrkNb(i)==aTrk)
			{
			    if (t->getMatchedRM(i)==1 || t->getMatchedRT_S(i)==1 ||
				t->getMatchedRMT_S(i)==1)
			    {
				if (t->getParId(i)==2 || t->getParId(i)==3)
				{
				    // ring in tracklet was made by lepton
				    // in correlation with other detector
				    // store the number of photons
				    // before reflection on mirror
				    t->setNumPhot(i,phot);
				    //cout<<"photon nb set: "<<phot<<endl;
				} else Error("HRichCorrelatorSim::isLepOnMirror(HHitMatchSim *h)","it's not a lepton !!");
			    }
			} else break;
		    }
		}
		else{
		    if ((TMath::Abs(xMirrPhotCM-xRing)<4 && 
			  TMath::Abs(yMirrPhotCM-yRing)<4 && 
			  sec==secRing) && lep2Count==0){
			h->setLeptonOnMirror(2);
			h->setNumPhot2(phot);
			weight2 = weightTemp;
			lep2Count++;
		    }
		}    
	    }
	}
	//	cout<<" nr of lep1  "<<lep1Count<< " nr of lep2 "<<lep2Count<<endl;
	if (lep1Count==0){
	    h->setLeptonOnMirror(0);
	    h->setNumPhot2(0);
	}
	else{
	    if(lep2Count>0){
	    h->setWeightRatio(weight1>weight2?weight1/weight2:weight2/weight1);
	    //  cout<<" weight ratio  "<< h->getWeightRatio()<<endl;
	    }
	}
    }else Error("HRichCorrelatorSim::isLepOnMirror(HHitMatchSim *h)","no valid Rich hit found");
    //    cout<<"  RingCanBeInCorrelation  "<<RingCanBeInCorrelation<<endl;
    return RingCanBeInCorrelation;
}
Int_t HRichCorrelatorSim::matchRingMirrtracks(HRichHitSim *pH,Int_t trackMirr,Int_t idMirr){
    Int_t t1,t2,t3;
    t1=t2=t3=-1;
    t1=pH->track1;t2=pH->track2;t3=pH->track3;
    // cout<<"  rich track 1 "<<t1<< "  rich track2  "<<t2<<"  rich track3 "<<t3<<endl;
    //cout<<"  rich weight 1 "<<pH->weigTrack1<<" rich weight 2 "<<pH->weigTrack2<<" rich weight 3 "<<pH->weigTrack3<<endl; 
   
    
    Int_t p1,p2,p3;p1=p2=p3=-1;
    if (t1>-1) p1=HRichUtilFunc::getParID(t1,(HLinearCategory*)
					  getGeantKineCat());
    if (t2>-1) p2=HRichUtilFunc::getParID(t2,(HLinearCategory*)
					  getGeantKineCat());
    if (t3>-1) p3=HRichUtilFunc::getParID(t3,(HLinearCategory*)
						      getGeantKineCat());
    //cout<<"  rich ID 1  "<<p1<<"rich ID 2  "<<p2<<"  rich ID 3  "<<p3<<endl;  
    //cout<<"  mirror track  "<<trackMirr<<" mirror id "<<idMirr<<endl;
    if (t1==trackMirr && idMirr==p1) return pH->weigTrack1;
    if (t2==trackMirr && idMirr==p2) return pH->weigTrack2;
    if (t3==trackMirr && idMirr==p3) return pH->weigTrack3;
    return 0; 
}
Int_t HRichCorrelatorSim::isLepOnMDC(HHitMatchSim* h,Int_t trknb)
  {
      //check whether the particle with GEANT track number trk
      //has reached the MDC module 0, layer 0
      //this means that it didn't hit a spoke or edge in the RICH
      //and can therefore be correlated and detected, finally going 
      //in efficiency.
      //returns 1 if particle was seen in MDC, or 0 else
      //trknb is return value of isLepOnMirror, the track number of 
      //the first found lepton passing through the mirror
      //matching a track number in the ring
      Int_t ParticleSeenInMDC=-1;// -1: not seen in MDC
    
      if (h->getNumPhot1()>0) //has Rich ring made of lepton photons on mirr
      {
  	HGeantMdc *gmdc = 0;
  	iter_mdcgeant->Reset();
  	while((gmdc=(HGeantMdc *)iter_mdcgeant->Next())!=0)
  	{
  	    //cout<<gmdc->getTrack()<<" "<<(Int_t)gmdc->getSector()<<" ";
  	    //cout<<(Int_t)gmdc->getModule()<<" "<<(Int_t)gmdc->getLayer()<<endl;
	    
  	    Int_t mod = (Int_t) gmdc->getModule();
  	    Int_t lay = (Int_t) gmdc->getLayer();
  	    if (gmdc->getTrack()==trknb && mod==0 && lay==0) 
  	    {//same track number of particle that went through
  		//the mirror
  		ParticleSeenInMDC=1;//enough for return value
  		h->setGLepInMDC(ParticleSeenInMDC);
		//cout<<"flag indicating that lep reached MDC "<<h->getGLepInMDC()<<endl;
  		//check if this trk nb is in different subdetector hits of this tracklet
  		HTrackInfo* t=(HTrackInfo*) h->getTrackInfoObj();
  		for (Int_t i=0;i<MAXPARTICLES;i++)
  		{
  		    if( t->getTrkNb(i)>-1 && t->getTrkNb(i)==trknb) //trk nb occurred in a corr hit
  		    {
  			if (t->getMatchedRM(i)==1 || t->getMatchedRT_S(i)==1 ||
  			    t->getMatchedRMT_S(i)==1)//not a fake
  			{
  			    t->setGCLepInMDC(i,ParticleSeenInMDC);
  			    break;
  			}
  		    }
  		}
  	    }
  	}//end while
      }
    
      return ParticleSeenInMDC;
  }

Bool_t HRichCorrelatorSim::finalize() {
    HRichCorrelator::finalize();
    TFile *f = (TFile*) gHades->getOutputFile();
    TString filename("testfile.stat");
    if (f){
	
	cout<<((TFile*)f)->GetName()<<endl;
	filename = ((TFile*)gHades->getOutputFile())->GetName();
    }
    filename.Remove(filename.Length()-5, filename.Length());
    filename.Append(".stat");
    std::ofstream statusOut(filename.Data(),ios::app);
    // dump to stdout
    cout<<"--- SIM INFO ---"<<endl;
    cout<<"Nb of GEANT particles :"<<ng_Part<<endl;
    cout<<"Particle ID/# ... ";
    for(Int_t n=0;n<MAXPARIDS;n++) 
    {
	if(ng_ParId[n]>0) cout<<n<<"/"<<ng_ParId[n]<<"  ";
    }
    cout<<endl;
    if(ngNb_RW>0.) cout<<"Avrg Rich Weight :"<<fg_RW/ngNb_RW<<endl;
    if(ngNb_MW>0.) cout<<"Avrg Mdc Weight :"<<fg_MW/ngNb_MW<<endl;
    cout<<"Nb of GEANT confirmed tracklets: "<<ngNb_ConfTracklet<<endl;
    cout<<"Nb of fake tracklets: "<<ngNb_Fakes<<endl;
//     cout<<"Nb of Geant confirmed RM tracklets: "<<ngGNb_RM<<endl;
//     cout<<"Nb of Geant confirmed R (T||S) tracklets: "<<ngGNb_RT_S<<endl;
//     cout<<"Nb of Geant confirmed M (T||S) tracklets: "<<ngGNb_MT_S<<endl;
//     cout<<"Nb of Geant confirmed RM(T||S) tracklets: "<<ngGNb_RMT_S<<endl;
    cout<<"Nb of Geant confirmed RM tracklets: "<<ngGNbRMonly<<endl;
    cout<<"Nb of Geant confirmed R (T||S) tracklets: "<<ngGNbRTSonly<<endl;
    cout<<"Nb of Geant confirmed M (T||S) tracklets: "<<ngGNbMTSonly<<endl;
    cout<<"Nb of Geant confirmed RM(T||S) tracklets: "<<ngGNbRMTSonly<<endl;

    // append to status file
    statusOut<<"--- SIM INFO ---"<<endl;
    statusOut<<"Nb of Geant particles :"<<ng_Part<<endl;
    statusOut<<"Particle ID/# ... ";
    for(Int_t n=0;n<MAXPARIDS;n++) 
    {
	if(ng_ParId[n]>0) statusOut<<n<<"/"<<ng_ParId[n]<<" ";
    }
    statusOut<<endl;
    if(ngNb_RW>0.) statusOut<<"Avrg Rich Weight :"<<fg_RW/ngNb_RW<<endl;
    if(ngNb_MW>0.) statusOut<<"Avrg Mdc Weight :"<<fg_MW/ngNb_MW<<endl;
    statusOut<<"Nb of GEANT confirmed tracklets: "<<ngNb_ConfTracklet<<endl;
    statusOut<<"Nb of fake tracklets: "<<ngNb_Fakes<<endl;
//     statusOut<<"Nb of Geant confirmed RM tracklets: "<<ngGNb_RM<<endl;
//     statusOut<<"Nb of Geant confirmed R (T||S) tracklets: "<<ngGNb_RT_S<<endl;
//     statusOut<<"Nb of Geant confirmed M (T||S) tracklets: "<<ngGNb_MT_S<<endl;
//     statusOut<<"Nb of Geant confirmed RM(T||S) tracklets: "<<ngGNb_RMT_S<<endl;
    statusOut<<"Nb of Geant confirmed RM tracklets: "<<ngGNbRMonly<<endl;
    statusOut<<"Nb of Geant confirmed R (T||S) tracklets: "<<ngGNbRTSonly<<endl;
    statusOut<<"Nb of Geant confirmed M (T||S) tracklets: "<<ngGNbMTSonly<<endl;
    statusOut<<"Nb of Geant confirmed RM(T||S) tracklets: "<<ngGNbRMTSonly<<endl;
    statusOut.close();

    return kTRUE;
}


Int_t HRichCorrelatorSim::isGCorrRing(HTrackInfo *t)
{
    // this function checks whether a GEANT particle correlation
    // occurs for min 1 ring generating lepton in this tracklet

    Int_t n=0;//counter for confirmed and min bi-correlated lepton rings
    Bool_t isCorrRing=kFALSE;
    for (Int_t i=0;i<MAXPARTICLES;i++)
    {
	if( t->getTrkNb(i)>-1 )
	{
	    if (t->getMatchedRM(i)==1 || t->getMatchedRT_S(i)==1 ||
		t->getMatchedRMT_S(i)==1)
	    {
		if (t->getParId(i)==2 || t->getParId(i)==3) n++;
		isCorrRing=kTRUE;
	    }
	} else break;
    }
    if (isCorrRing) return n;// can be 0 or >0; 0 => not a lepton ring !
    else return -1; //doesn't contain a rich hit in Geant correlation
}

Int_t HRichCorrelatorSim::isGRing(HHitMatchSim *h)
{
    // this function checks whether the ring that went in the
    // tracklet contains pads fired by photons that come from a lepton
    // one should also check whether the lepton reached the mirror
    // and the first MDC module

    Int_t n=0;//counter for confirmed lepton rings
    // n==0: this ring is a fake, no lepton contributed
    // n==-1: no Rich hit in tracklet
    // n>0: n>0 leptons really contributed photons to the hit pattern
    HRichHitSim* r=0;
    if(h->getRichInd()>-1) r  = ((HRichHitSim*)getRichHitCat()
				       ->getObject(h->getRichInd()));
    else return -1;

    Int_t t1,t2,t3,w1,w2,w3;
    t1=t2=t3=w1=w2=w3=-1;
    if(r)
    {
	t1=r->track1;t2=r->track2;t3=r->track3;
	w1=r->weigTrack1;w2=r->weigTrack2;w3=r->weigTrack3;
	Int_t p1,p2,p3;p1=p2=p3=-1;
	if (t1>-1) p1=HRichUtilFunc::getParID(t1,(HLinearCategory*)
					      getGeantKineCat());
	if (t2>-1) p2=HRichUtilFunc::getParID(t2,(HLinearCategory*)
					      getGeantKineCat());
	if (t3>-1) p3=HRichUtilFunc::getParID(t3,(HLinearCategory*)
					      getGeantKineCat());
	if (p1==2 || p1==3) n++;
	if (p2==2 || p2==3) n++;
	if (p3==2 || p3==3) n++;
    }
    else Error("HRichCorrelatorSim::isGRing","no rich hit found");
    return n;
}















