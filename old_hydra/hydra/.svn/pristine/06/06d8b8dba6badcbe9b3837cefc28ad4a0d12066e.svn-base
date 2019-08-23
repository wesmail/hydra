// File: hrichcorrcountersim.cc
//
// Author: Thomas Eberl <Thomas.Eberl@physik.tu-muenchen.de>
// Last update by Thomas Eberl: 02/02/09 19:18:34
//
#include "hrichcorrcountersim.h"
#include "hruntimedb.h"
#include "hevent.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hcategory.h"
#include "hiterator.h"
#include <iostream.h>
#include "hdebug.h"
#include "hades.h"
#include "richdef.h"
#include "hhitmatchheadersim.h"
#include "hhitmatchsim.h"
#include "hlinearcategory.h"
#include "hrichutilfunc.h"
ClassImp(HRichCorrCounterSim)

HRichCorrCounterSim::HRichCorrCounterSim(Text_t *name,Text_t *title) :
  HRichCorrCounter(name,title)
{

}

HRichCorrCounterSim::HRichCorrCounterSim()
{

}


HRichCorrCounterSim::~HRichCorrCounterSim(void) {

}


Bool_t HRichCorrCounterSim::init() {
        if (gHades) {
	HEvent *event=gHades->getCurrentEvent();
	HRuntimeDb *rtdb=gHades->getRuntimeDb();
	HSpectrometer *spec=gHades->getSetup();
	if (event && rtdb) {
	    HDetector *rich = spec->getDetector("Rich");
	    
	    if (rich) { //Has the user set up a RICH?
		// Tracklet
		pHitMatchCat=event->getCategory(catMatchHit);
		if (!pHitMatchCat) 
		{
		    pHitMatchCat=rich->buildCategory(catMatchHit);
		    
		    if (!pHitMatchCat) {
			Error("init","No HIT MATCH category defined");
			return kFALSE;
		    }
		    else event->addCategory(catMatchHit, pHitMatchCat, "Rich");
		}
		
		pIterMatchHit = (HIterator*)getHitMatchCat()->MakeIterator("native");
		// Header
		pHitMatchHeaderCat=event->getCategory(catMatchHitHeader);
		if (!pHitMatchHeaderCat) 
		{
		    pHitMatchHeaderCat=rich->buildCategory(catMatchHitHeader);
		    cout<<"i have built hhitmatchheader"<<endl;
		    if (!pHitMatchHeaderCat) {
			Error("init","No HIT MATCH HEADER category defined");
			return kFALSE;
		    }
		    else event->addCategory(catMatchHitHeader, pHitMatchHeaderCat, "Rich");
		}
		
		pIterMatchHitHeader = (HIterator*)getHitMatchHeaderCat()
		    ->MakeIterator("native");
	    }
	}
	}
	iniCounters();

    return kTRUE;
}

void HRichCorrCounterSim::iniCounters()
{
    HRichCorrCounter::iniCounters();
    ng_Part=0;
    ngNb_ConfTracklet=0;
    ngNb_Fakes=0;
    fAvrgRichWeight=0.;
    fAvrgMdcWeight=0.;
    ngGNbRMonly=0;
    ngGNbRTSonly=0;
    ngGNbMTSonly=0;
    ngGNbRMTSonly=0;
    ngNb_RW=0;
    ngNb_MW=0;
    for(Int_t n=0;n<MAXPARIDS;n++) 
	{
	 ng_ParId[n]=0;
	}
}

Int_t HRichCorrCounterSim::execute()
{
    if(HRichCorrCounter::execute())
    {

//  	HHitMatchSim* pHM= 0;
//  	pIterMatchHit->Reset();
//  	while(( pHM= (HHitMatchSim *)pIterMatchHit->Next()))
//  	{
//  	    //pHM->dumpToStdoutSim();
	    

//  	}

	HHitMatchHeaderSim *pHMH = 0;
	pIterMatchHitHeader->Reset();

	while((pHMH = (HHitMatchHeaderSim *)pIterMatchHitHeader->Next()))
	{
	    pHMH->resetSim();

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
		//diagOut(pHitMatch,t);
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
		fAvrgRichWeight+=fl_RW;//add avg Rich weight for this evt
		ngNb_RW++;//count that there was a particle in Rich
	    }
	    pHMH->setAvrgMdcWeight(fl_MW);
	    if(fl_MW>0.)
	    {
		fAvrgMdcWeight+=fl_MW;
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

//       HHitMatchHeaderSim *pHMH = 0;
//       pIterMatchHitHeader->Reset();


//       while(( pHMH= (HHitMatchHeaderSim *)pIterMatchHitHeader->Next()))
//       {
//  	ng_Part+=pHMH->getNbPart();
//  	ngNb_ConfTracklet+=pHMH->getNbConfTracklets();
//  	ngNb_Fakes+=pHMH->getNbFakes();
//  	if(pHMH->getAvrgRichWeight()>0.) ngNb_RW++;
//  	if(pHMH->getAvrgMdcWeight()>0.) ngNb_MW++;
//  	fAvrgRichWeight+=pHMH->getAvrgRichWeight();
//  	fAvrgMdcWeight+=pHMH->getAvrgMdcWeight();
//  	ngGNbRMonly+=pHMH->getGeantNb_RM();
//          ngGNbRTSonly+=pHMH->getGeantNb_RT_S();
//          ngGNbMTSonly+=pHMH->getGeantNb_MT_S();
//          ngGNbRMTSonly+=pHMH->getGeantNb_RMT_S();
//  	for(Int_t n=0;n<MAXPARIDS;n++) 
//  	{
//  	    if(pHMH->getParId(n)>0) ng_ParId[n]+=pHMH->getParId(n);
//  	}
//      }
//      return kTRUE;
//      }
    }
    return kFALSE;
}

Bool_t HRichCorrCounterSim::finalize() {
    
    dumpCorrelationStatus();
    
    return kTRUE;
}
void HRichCorrCounterSim::dumpCorrelationStatus()
{
    HRichCorrCounter::dumpCorrelationStatus();

    cout<<"--- SIM INFO ---"<<endl;
    cout<<"Nb of GEANT particles :"<<ng_Part<<endl;
    cout<<"Particle ID/# ... ";
    for(Int_t n=0;n<MAXPARIDS;n++) 
    {
	if(ng_ParId[n]>0) cout<<n<<"/"<<ng_ParId[n]<<"  ";
    }
    cout<<endl;
    if(ngNb_RW>0.) cout<<"Avrg Rich Weight :"<<fAvrgRichWeight/ngNb_RW<<endl;
    if(ngNb_MW>0.) cout<<"Avrg Mdc Weight :"<<fAvrgMdcWeight/ngNb_MW<<endl;
    cout<<"Nb of GEANT confirmed tracklets: "<<ngNb_ConfTracklet<<endl;
    cout<<"Nb of fake tracklets: "<<ngNb_Fakes<<endl;
    cout<<"Nb of Geant confirmed RM tracklets: "<<ngGNbRMonly<<endl;
    cout<<"Nb of Geant confirmed R (T||S) tracklets: "<<ngGNbRTSonly<<endl;
    cout<<"Nb of Geant confirmed M (T||S) tracklets: "<<ngGNbMTSonly<<endl;
    cout<<"Nb of Geant confirmed RM(T||S) tracklets: "<<ngGNbRMTSonly<<endl;

}








