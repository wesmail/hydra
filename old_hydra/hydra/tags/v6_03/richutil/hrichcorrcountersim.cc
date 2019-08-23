// File: hrichcorrcountersim.cc
//
// Author: Thomas Eberl <Thomas.Eberl@physik.tu-muenchen.de>
// Last update by Thomas Eberl: 01/11/26 14:55:36
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
	HHitMatchHeaderSim *pHMH = 0;
    pIterMatchHitHeader->Reset();

    while(( pHMH= (HHitMatchHeaderSim *)pIterMatchHitHeader->Next()))
    {
	ng_Part+=pHMH->getNbPart();
	ngNb_ConfTracklet+=pHMH->getNbConfTracklets();
	ngNb_Fakes+=pHMH->getNbFakes();
	if(pHMH->getAvrgRichWeight()>0.) ngNb_RW++;
	if(pHMH->getAvrgMdcWeight()>0.) ngNb_MW++;
	fAvrgRichWeight+=pHMH->getAvrgRichWeight();
	fAvrgMdcWeight+=pHMH->getAvrgMdcWeight();
	ngGNbRMonly+=pHMH->getGeantNb_RM();
        ngGNbRTSonly+=pHMH->getGeantNb_RT_S();
        ngGNbMTSonly+=pHMH->getGeantNb_MT_S();
        ngGNbRMTSonly+=pHMH->getGeantNb_RMT_S();
	for(Int_t n=0;n<MAXPARIDS;n++) 
	{
	    if(pHMH->getParId(n)>0) ng_ParId[n]+=pHMH->getParId(n);
	}
    }
    return kTRUE;
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








