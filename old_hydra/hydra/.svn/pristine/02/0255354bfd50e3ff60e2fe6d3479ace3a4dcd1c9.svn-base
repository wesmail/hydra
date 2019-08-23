// File: hrichcorrelatorsim.cc
//
// Author: Thomas Eberl <Thomas.Eberl@physik.tu-muenchen.de>
// Last update by Thomas Eberl: 01/10/31 15:56:51
//
#include "hrichcorrelatorsim.h"
#include "hrichcorrelatorpar.h"
#include "hrichutilfunc.h"
#include "hgeantkine.h"
#include "hgeantrich.h"
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
#include <fstream> 
///////////////////////////////////////////////////////////
// HRichCorrelatorSim
//
// executes HRichCorrelator and uses additional info from GEANT 
// fills HHitMatchSim
///////////////////////////////////////////////////////////
ClassImp(HRichCorrelatorSim)

HRichCorrelatorSim::HRichCorrelatorSim(Text_t *name,Text_t *title) :
  HRichCorrelator(name,title)
{
     
}
HRichCorrelatorSim::HRichCorrelatorSim(Text_t *name,Text_t *title, char* filename) :
  HRichCorrelator(name,title)
{

    pFileName  = filename;

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

	    // HGEANT KINE INFO
	    fGeantKineCat =(HLinearCategory*) event->getCategory(catGeantKine);
	    if (!fGeantKineCat) 
	    { 
		Error("HRichCorrelatorSim::init",
		      "no GEANT Kine category available");
	    }                
	    iter_kine = (HIterator *)fGeantKineCat ->MakeIterator("native");
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
		    cout<<" i have built hrichhitsim "<<endl;
		    if (!fRichPID) return kFALSE;
		    else gHades->getCurrentEvent()
			     ->addCategory(catRichHit, fRichPID, "Rich");
		}
    
		fRichIter = (HIterator*) fRichPID->MakeIterator();
		if(!fRichIter) Error("init","no RichHitSim iter defined");

		//        //Setup output
		pHitMatchCat=event->getCategory(catMatchHit);
		if (!pHitMatchCat) {
		    pHitMatchCat=rich->buildLinearCat("HHitMatchSim");
		    cout<<"i have built hhitmatchsim"<<endl;
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
		    cout<<"i have built hhitmatchheadersim"<<endl;
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
		    cout<<"i have built a hmdcsegsim"<<endl;
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
		    cout<<"i have built ftofhits"<<endl;
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
		     cout<<"i have built hshowertrack "<<endl;
		     if (!fShowerTrack) return kFALSE;
		     else gHades->getCurrentEvent()
			      ->addCategory(catShowerTrack, fShowerTrack, "Shower");
		 }
		 iter_showertrack = (HIterator*)fShowerTrack->MakeIterator();
 		fShowerTofHits=event->getCategory(catShowerHitTofTrack);
 		if (!fShowerTofHits)
 		{
 		   fShowerTofHits  = new HLinearCategory("HShowerHitTofTrack", 1000);
 		   cout<<"i have built hshowerhittoftrack"<<endl;
 		   if (!fShowerTofHits) return kFALSE;
 		   else gHades->getCurrentEvent()
 			    ->addCategory(catShowerHitTofTrack,fShowerTofHits, "Tofino");
 		}
 		fShowerTofIter = (HIterator *)fShowerTofHits->MakeIterator();
  	    }
	    

	    
	  
	} else {
	    Error ("init","! (event && rtdb)");
	    return kFALSE; //! (event && rtdb)
	}
	iniCounters();
	pFileOut = new TFile(pFileName,"RECREATE");
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
    //cout<<"in reinit function"<<endl;
    HRichCorrelatorPar*   pCorrPar = (HRichCorrelatorPar*)getCorrelationPar();
    if (pCorrPar->hasChanged())
    {
        iniCuts();
        iniSwitches();
        iniControlHistos();
	//	histArray = new TObjArray(5);
	//histArray->Add(new TH1D("corr_obj_multi","corr_obj_multi",20,0,20));
        pCorrPar -> printParam();
    }
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
	if(r)
	{
	    cout<<"RICH track/weight"<<endl;
	    cout<<r->track1<<"/"<<r->weigTrack1<<" ";
	    cout<<r->track2<<"/"<<r->weigTrack2<<" ";
	    cout<<r->track3<<"/"<<r->weigTrack3<<endl;
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
    //cout<<"%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% "
    //	<<nCounterProcessedNbEvents<<endl;
    if (HRichCorrelator::execute()) //treat sim data like exp data
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
		Int_t isFirstGeantNbInTracklet=1;
		for(Int_t i=0;i<MAXPARTICLES;i++)
		{
		    Int_t nTrkNb=t->getTrkNb(i);
		    if (nTrkNb>-1)
		    {
			if(isFirstGeantNbInTracklet) nlNb_ConfTracklet++;
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
		diagOut(pHitMatch,t);
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

	    //pHMH->dumpToStdoutSim();
	}//this hitmatch header
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
    t->dumpToStdout();
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
    
    
    
    // case RICH-MDC only
    if(richTracks[0] && mdcTracks[0] && !showerTracks[0] && !tofTracks[0])
    {
	corrTracksRM(pHitMatch);
    }
    // case MDC and TOF or SHOWER
    if(!richTracks[0] && mdcTracks[0] &&  (showerTracks[0] || tofTracks[0]))
    {
	corrTracksMT_S(pHitMatch);
    }
    // case RICH and TOF or SHOWER
    if(richTracks[0] && !mdcTracks[0] && (showerTracks[0] || tofTracks[0]) )
    {
	corrTracksRT_S(pHitMatch);
    }
    // case RICH - MDC and TOF or SHOWER
    if(richTracks[0] && mdcTracks[0] && (showerTracks[0] || tofTracks[0]) )
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
	if(showerTracks[0]){
	    for (Int_t j=0;j<SHOWERMAXTRACKS;j++)
	    {
		// store same GEANT nb found in RICH Hit and SHOWER Hit 
		if(richTracks[i]!=0 && richTracks[i] == showerTracks[j])
		{
		    setGeantTrackInfoRT_S(i,t);
		    break;
		}
	    }
	}
	if(tofTracks[0]){
	    for (Int_t j=0;j<TOFMAXTRACKS;j++)
	    {
		if(richTracks[i]!=0 && richTracks[i] == tofTracks[j])
		{
		    setGeantTrackInfoRT_S(i,t);
		    break;
		}
	    }
	}
    }
}
void HRichCorrelatorSim::corrTracksMT_S(HHitMatchSim *pHitMatch){

    HTrackInfo* t = pHitMatch->getTrackInfoObj();
    for (Int_t i=0;i<MDCMAXTRACKS;i++)
    {
	if(showerTracks[0]){
	    for (Int_t j=0;j<SHOWERMAXTRACKS;j++)
	    {
		if(mdcTracks[i]!=0 && mdcTracks[i] == showerTracks[j])
		{
		    setGeantTrackInfoMT_S(i,t);
		    break;
		}
	    }
	}
	if(tofTracks[0]){
	    for (Int_t j=0;j<TOFMAXTRACKS;j++)
	    {
		if(mdcTracks[i]!=0 && mdcTracks[i] == tofTracks[j])
		{
		    setGeantTrackInfoMT_S(i,t);
		    break;
		}
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
	    if(richTracks[i]!=0 && richTracks[i] == mdcTracks[j]) 
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
	t->setTrkNb(newSlot,trknb);
	t->setParId(newSlot,HRichUtilFunc::getParID(trknb,(HLinearCategory*)getGeantKineCat()));
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
	t->setTrkNb(newSlot,trknb);
	t->setParId(newSlot,HRichUtilFunc::getParID(trknb,
                           (HLinearCategory*)getGeantKineCat()));
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
	t->setTrkNb(newSlot,trknb);
	t->setParId(newSlot,HRichUtilFunc::getParID(trknb,
		           (HLinearCategory*)getGeantKineCat()));
	t->setRichWeight(newSlot,calcRichWeight(richindex));
	t->setMdcWeight(newSlot,calcMdcWeight(mdcindex));
	t->setMatchedRM(newSlot);
    }
}
Bool_t HRichCorrelatorSim::finalize() {
    HRichCorrelator::finalize();
    TString filename(((TFile*)gHades->getOutputFile())->GetName());
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


















