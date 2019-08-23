// $Id: hrichcorrcountersim.cc,v 1.7 2002-09-25 16:19:52 eberl Exp $
// Last update by Thomas Eberl: 02/09/25 17:30:38
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
#include "kickdef.h"
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

HRichCorrCounterSim::HRichCorrCounterSim(Text_t *name,Text_t *title, char* filename) :
  HRichCorrCounter(name,title)
{
    pFileName  = filename;// output filename for diagnostic histos
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

	    // --------- Tracks from Kickplane --------------
	    fKickTrackCat = gHades->getCurrentEvent()->getCategory(catKickTrack);
	    if (!fKickTrackCat) {
		Error("init","KickTrack not found in input file");
		return kFALSE;
	    }
	    iterTracks = (HIterator *)fKickTrackCat->MakeIterator("native");
	    cout<<"itertracks :"<<iterTracks<<endl;

	}
	}
	iniCounters();
	pFileOut = new TFile(pFileName,"RECREATE");
	iniHistos();
    return kTRUE;
}

void HRichCorrCounterSim::iniHistos()
{
    HRichCorrCounter::iniHistos();
    pHistArray2 = new TObjArray(10);

    Int_t dTl=-20;
    Int_t dTh= 20;
    Int_t dTn= 40;

    Int_t opl= 0;
    Int_t oph= 50;
    Int_t opn= 200;


    Int_t medl=1;
    Int_t medh=20;
    Int_t medn=20;

    Int_t mechl=1;
    Int_t mechh=15;
    Int_t mechn=15;

    Int_t moml=1;
    Int_t momh=1000;
    Int_t momn=100;

    Int_t idl=1;
    Int_t idh=15;
    Int_t idn=15;

    //MDC-TOF-dTheta
    pH_ParIdMdcTofdTheta = new TH2F("pH_ParIdMdcTofdTheta","pH_ParIdMdcTofdTheta",
			      idn,idl,idh,dTn,dTl,dTh);
    pHistArray2->Add(pH_ParIdMdcTofdTheta);

    pH_MechMdcTofdTheta = new TH2F("pH_MechMdcTofdTheta","pH_MechMdcTofdTheta",
  			      mechn,mechl,mechh,dTn,dTl,dTh);
    pHistArray2->Add(pH_MechMdcTofdTheta);
    
    pH_MedMdcTofdTheta = new TH2F("pH_MedMdcTofdTheta","pH_MedMdcTofdTheta",
  			      medn,medl,medh,dTn,dTl,dTh);
    pHistArray2->Add(pH_MedMdcTofdTheta);

    pH_MomMdcTofdTheta = new TH2F("pH_MomMdcTofdTheta","pH_MomMdcTofdTheta",
  			      momn,moml,momh,dTn,dTl,dTh);
    pHistArray2->Add(pH_MomMdcTofdTheta);

    pH_CreaIdMdcTofdTheta = new TH2F("pH_CreaIdMdcTofdTheta","pH_CreaIdMdcTofdTheta",
  			      idn,idl,idh,dTn,dTl,dTh);
    pHistArray2->Add(pH_CreaIdMdcTofdTheta);

    //MDC-SHOWER-dTheta
    pH_ParIdMdcShowerdTheta = new TH2F("pH_ParIdMdcShowerdTheta","pH_ParIdMdcShowerdTheta",
			      idn,idl,idh,dTn,dTl,dTh);
    pHistArray2->Add(pH_ParIdMdcShowerdTheta);

    pH_MechMdcShowerdTheta = new TH2F("pH_MechMdcShowerdTheta","pH_MechMdcShowerdTheta",
  			      mechn,mechl,mechh,dTn,dTl,dTh);
    pHistArray2->Add(pH_MechMdcShowerdTheta);
    
    pH_MedMdcShowerdTheta = new TH2F("pH_MedMdcShowerdTheta","pH_MedMdcShowerdTheta",
  			      medn,medl,medh,dTn,dTl,dTh);
    pHistArray2->Add(pH_MedMdcShowerdTheta);

    pH_MomMdcShowerdTheta = new TH2F("pH_MomMdcShowerdTheta","pH_MomMdcShowerdTheta",
  			      momn,moml,momh,dTn,dTl,dTh);
    pHistArray2->Add(pH_MomMdcShowerdTheta);

    pH_CreaIdMdcShowerdTheta = new TH2F("pH_CreaIdMdcShowerdTheta","pH_CreaIdMdcShowerdTheta",
  			      idn,idl,idh,dTn,dTl,dTh);
    pHistArray2->Add(pH_CreaIdMdcShowerdTheta);

    //GEANT KINE vs GEANT KINE
    pH_ParIdMech = new TH2F("pH_ParIdMech","pH_ParIdMech",idn,idl,idh,mechn,mechl,mechh);
    pHistArray2->Add(pH_ParIdMech);

    pH_ParIdMom = new TH2F("pH_ParIdMom","pH_ParIdMom",idn,idl,idh,momn,moml,momh);
    pHistArray2->Add(pH_ParIdMom);

    pH_ParIdMed = new TH2F("pH_ParIdMed","pH_ParIdMed",idn,idl,idh,medn,medl,medh);
    pHistArray2->Add(pH_ParIdMed);

    pH_ParIdCreaId = new TH2F("pH_ParIdCreaId","pH_ParIdCreaId",idn,idl,idh,idn,idl,idh);
    pHistArray2->Add(pH_ParIdCreaId);

    //MDC-SHOWER-dTheta - only electrons
    pH_ele_MechMdcShowerdTheta = new TH2F("pH_ele_MechMdcShowerdTheta","pH_ele_MechMdcShowerdTheta",
					  mechn,mechl,mechh,dTn,dTl,dTh);
    pHistArray2->Add(pH_ele_MechMdcShowerdTheta);
    pH_ele_MedMdcShowerdTheta = new TH2F("pH_ele_MedMdcShowerdTheta","pH_ele_MedMdcShowerdTheta",
					 medn,medl,medh,dTn,dTl,dTh);
    pHistArray2->Add(pH_ele_MedMdcShowerdTheta);
    pH_ele_MomMdcShowerdTheta = new TH2F("pH_ele_MomMdcShowerdTheta","pH_ele_MomMdcShowerdTheta",
					 momn,moml,momh,dTn,dTl,dTh);
    pHistArray2->Add(pH_ele_MomMdcShowerdTheta);
    pH_ele_CreaIdMdcShowerdTheta = new TH2F("pH_ele_CreaIdMdcShowerdTheta","pH_ele_CreaIdMdcShowerdTheta",
					   idn,idl,idh,dTn,dTl,dTh); 
    //MDC-SHOWER-dTheta - only positrons
    pHistArray2->Add(pH_ele_CreaIdMdcShowerdTheta);
    pH_pos_MechMdcShowerdTheta = new TH2F("pH_pos_MechMdcShowerdTheta","pH_pos_MechMdcShowerdTheta",
					  mechn,mechl,mechh,dTn,dTl,dTh);
    pHistArray2->Add(pH_pos_MechMdcShowerdTheta);
    pH_pos_MedMdcShowerdTheta = new TH2F("pH_pos_MedMdcShowerdTheta","pH_pos_MedMdcShowerdTheta",
					 medn,medl,medh,dTn,dTl,dTh);
    pHistArray2->Add(pH_pos_MedMdcShowerdTheta);
    pH_pos_MomMdcShowerdTheta = new TH2F("pH_pos_MomMdcShowerdTheta","pH_pos_MomMdcShowerdTheta",
					 momn,moml,momh,dTn,dTl,dTh);
    pHistArray2->Add(pH_pos_MomMdcShowerdTheta);
    pH_pos_CreaIdMdcShowerdTheta = new TH2F("pH_pos_CreaIdMdcShowerdTheta","pH_pos_CreaIdMdcShowerdTheta",
					   idn,idl,idh,dTn,dTl,dTh); 
    pHistArray2->Add(pH_pos_CreaIdMdcShowerdTheta);

    //MDC-TOF-dTheta - only electrons
    pH_ele_MechMdcTofdTheta = new TH2F("pH_ele_MechMdcTofdTheta","pH_ele_MechMdcTofdTheta",
				       mechn,mechl,mechh,dTn,dTl,dTh);
    pHistArray2->Add(pH_ele_MechMdcTofdTheta);
    pH_ele_MedMdcTofdTheta = new TH2F("pH_ele_MedMdcTofdTheta","pH_ele_MedMdcTofdTheta",
				      medn,medl,medh,dTn,dTl,dTh);
    pHistArray2->Add(pH_ele_MedMdcTofdTheta);
    pH_ele_MomMdcTofdTheta = new TH2F("pH_ele_MomMdcTofdTheta","pH_ele_MomMdcTofdTheta",
				      momn,moml,momh,dTn,dTl,dTh);
    pHistArray2->Add(pH_ele_MomMdcTofdTheta);
    pH_ele_CreaIdMdcTofdTheta = new TH2F("pH_ele_CreaIdMdcTofdTheta","pH_ele_CreaIdMdcTofdTheta",
					 idn,idl,idh,dTn,dTl,dTh);
    pHistArray2->Add(pH_ele_CreaIdMdcTofdTheta);


    //MDC-TOF-dTheta - only positrons
    pH_pos_MechMdcTofdTheta = new TH2F("pH_pos_MechMdcTofdTheta","pH_pos_MechMdcTofdTheta",
				       mechn,mechl,mechh,dTn,dTl,dTh);
    pHistArray2->Add(pH_pos_MechMdcTofdTheta);
    pH_pos_MedMdcTofdTheta = new TH2F("pH_pos_MedMdcTofdTheta","pH_pos_MedMdcTofdTheta",
				      medn,medl,medh,dTn,dTl,dTh);
    pHistArray2->Add(pH_pos_MedMdcTofdTheta);
    pH_pos_MomMdcTofdTheta = new TH2F("pH_pos_MomMdcTofdTheta","pH_pos_MomMdcTofdTheta",
				      momn,moml,momh,dTn,dTl,dTh);
    pHistArray2->Add(pH_pos_MomMdcTofdTheta);
    pH_pos_CreaIdMdcTofdTheta = new TH2F("pH_pos_CreaIdMdcTofdTheta","pH_pos_CreaIdMdcTofdTheta",
					 idn,idl,idh,dTn,dTl,dTh);
    pHistArray2->Add(pH_pos_CreaIdMdcTofdTheta);

    // opening angles

    pH_opangleMDCa = new TH1F("pH_opangleMDCa","pH_opangleMDCa",opn,opl,oph);
    pHistArray->Add(pH_opangleMDCa);

    pH_opangleMDCDalitz = new TH1F("pH_opangleMDCDalitz","pH_opangleMDCDalitz",opn,opl,oph);
    pHistArray->Add(pH_opangleMDCDalitz);
    
    pH_opangleMDCConv = new TH1F("pH_opangleMDCConv","pH_opangleMDCConv",opn,opl,oph);
    pHistArray->Add(pH_opangleMDCConv);

    pH_opangleMETADalitz = new TH1F("pH_opangleMETADalitz","pH_opangleMETADalitz",opn,opl,oph);
    pHistArray->Add(pH_opangleMETADalitz);
    
    pH_opangleMETAConv = new TH1F("pH_opangleMETAConv","pH_opangleMETAConv",opn,opl,oph);
    pHistArray->Add(pH_opangleMETAConv);

    pH_opangleMETAdThetaDalitz = new TH2F("pH_opangleMETAdThetaDalitz","pH_opangleMETAdThetaDalitz",opn,opl,oph,dTn,dTl,dTh);
    pHistArray->Add(pH_opangleMETAdThetaDalitz);

    pH_opangleMETAdPhiDalitz = new TH2F("pH_opangleMETAdPhiDalitz","pH_opangleMETAdPhiDalitz",opn,opl,oph,10,-5,5);
    pHistArray->Add(pH_opangleMETAdPhiDalitz);

    pH_opangleMETAdThetaConv = new TH2F("pH_opangleMETAdThetaConv", "pH_opangleMETAdThetaConv",opn,opl,oph,40,-20,20);
    pHistArray->Add(pH_opangleMETAdThetaConv);

    pH_opangleMETAdPhiConv = new TH2F("pH_opangleMETAdPhiConv", "pH_opangleMETAdPhiConv",opn,opl,oph,10,-5,5);
    pHistArray->Add(pH_opangleMETAdPhiConv);
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
    //cout<<"new evt ------------------------------------------------------"<<endl;
    ////////////////////////////////////////////////////////////////////
    //////////////// Update Tracklet Header ////////////////////////////
    //////////////// which might be out of sync ////////////////////////
    //////////////// after object filtering ////////////////////////////
    ////////////////////////////////////////////////////////////////////
    if(HRichCorrCounter::execute())
    {

	HHitMatchHeaderSim *pHMH = 0;
	pIterMatchHitHeader->Reset();

	while((pHMH = (HHitMatchHeaderSim *)pIterMatchHitHeader->Next()))
	{// there is only one header object !
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
		//pHitMatch->resetSim();
		Int_t isFirstGeantNbInTracklet=1;
		// loop over GEANT particles in corr object
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
	    pHMH->setNbPart(nl_Part);//nb of diff particles in tracklets in this event
	    ng_Part+=nl_Part;//nb of particles in file
	    ngNb_ConfTracklet+=nlNb_ConfTracklet;//nb of confirmed tracklets
	    pHMH->setNbConfTracklets(nlNb_ConfTracklet);// ... in this event
	    ngNb_Fakes+=nlNb_Fakes;//number of fake tracklets
	    pHMH->setNbFakes(nlNb_Fakes);// in this event
	    pHMH->setAvrgRichWeight(fl_RW);// ratio of fired pads from particle
	    // in correlation to total nb of fired pads
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


	    pHMH->setGeantNb_RM(nGNbRMonly);//for this event
	    ngGNbRMonly+=nGNbRMonly;//for the processed file
	    pHMH->setGeantNb_RT_S(nGNbRTSonly);
	    ngGNbRTSonly+=nGNbRTSonly;
	    pHMH->setGeantNb_MT_S(nGNbMTSonly);
	    ngGNbMTSonly+=nGNbMTSonly;
	    pHMH->setGeantNb_RMT_S(nGNbRMTSonly);
	    ngGNbRMTSonly+=nGNbRMTSonly;

	    //pHMH->dumpToStdoutSim();
	}//this hitmatch header

    }//endif HRichCorrCounter::execute
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    /////////////// Fill histograms 
    //////////////////////
    ///                !!!
    fillHistos();      
    ///                !!!
    //////////////////////
    return kFALSE;
}

Bool_t HRichCorrCounterSim::finalize() {
    HRichCorrCounter::finalize();
    dumpCorrelationStatus();
    pFileOut->cd();
    HRichUtilFunc::saveHistos(pFileOut,pHistArray2);
    pFileOut->Close();
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

Int_t HRichCorrCounterSim::fillHistos()
{
    Int_t nHM = ((HLinearCategory*)getHitMatchCat())->getEntries();
    //cout<<"fill Histos in SIM "<<nHM<<endl;
    if (nHM==0) return 0; // nothing to be done

    pIterMatchHit->Reset();
    HHitMatchSim* pHM=0;

     Int_t *MdcTofUniqueCombiMdc=new Int_t[nHM];
     Int_t *MdcTofUniqueCombiTof=new Int_t[nHM];
     Int_t *MdcShowerUniqueCombiMdc=new Int_t[nHM];
     Int_t *MdcShowerUniqueCombiShower=new Int_t[nHM];
     for (Int_t i=0;i<nHM;i++) MdcTofUniqueCombiMdc[i]=MdcTofUniqueCombiTof[i]=MdcShowerUniqueCombiMdc[i]=MdcShowerUniqueCombiShower[i]=-2;

     TObjArray *dalitz = new TObjArray(50);
     TObjArray *conv   = new TObjArray(50);
     TObjArray *all   = new TObjArray(50);
     //cout<<"******"<<endl;
    while(( pHM= (HHitMatchSim *)pIterMatchHit->Next()))
    {
	fillGeantMdcTofdTheta(pHM,nHM,
		      MdcTofUniqueCombiMdc,MdcTofUniqueCombiTof);
	//fillGeantMdcTofdThetanoGCond(pHM,nHM,
	//	      MdcTofUniqueCombiMdc,MdcTofUniqueCombiTof);
	fillGeantMdcShowerdTheta(pHM,nHM,
		      MdcShowerUniqueCombiMdc,MdcShowerUniqueCombiShower);

	// select objs for opening angle distribution
	
	selectConvTrack(conv,pHM);
	selectDalitzTrack(dalitz,pHM);
	selectAll(all,pHM);
	
    }
    
    calcOpeningAngleMDC(conv,pH_opangleMDCConv);
    calcOpeningAngleMDC(dalitz,pH_opangleMDCDalitz);
    //calcOpeningAngleMDC(all,pH_opangleMDCa);
    //cout<<"*** end ***"<<endl;
    //calcOpeningAngleMETA(conv,pH_opangleMETAConv,pH_opangleMETAdThetaConv,pH_opangleMETAdPhiConv);
    //calcOpeningAngleMETA(dalitz,pH_opangleMETADalitz,pH_opangleMETAdThetaDalitz,pH_opangleMETAdPhiDalitz);
    delete all;
    delete dalitz;
    delete conv;
    delete [] MdcTofUniqueCombiMdc;
    delete [] MdcTofUniqueCombiTof;
    delete [] MdcShowerUniqueCombiMdc;
    delete [] MdcShowerUniqueCombiShower;
    
    return 1;
}

Int_t HRichCorrCounterSim::selectAll(TObjArray* arr,HHitMatchSim* h)
{
    Int_t nRInd = h->getRichInd();
    Int_t nMInd = h->getMdcInd();
    Int_t nTInd = h->getTofInd();
    Int_t nSInd = h->getShowInd();

    Int_t nRM = h->getMatchedRichMdc();
    Int_t nRT = h->getMatchedRichTof();
    Int_t nMT = h->getMatchedMdcTof();
    Int_t nRS = h->getMatchedRichShower();
    Int_t nMS = h->getMatchedMdcShower();
    Float_t dThetaRM = TMath::Abs(h->getRichTheta()-h->getMdcTheta());
 
   if (nRInd!=-1 && nTInd!=-1 && nMInd!=-1
	&& nRM==1 &&  nRT==1 &&  nMT==1 )
    {
	if (dThetaRM<2.) 
	{
	    arr->Add(h);
	    //h->dumpToStdoutSim();
	    //cout<<"all: "<<nMInd<<endl;
	}
    }

    else if (nRInd!=-1 && nSInd!=-1 && nMInd!=-1
	     && nRM==1 &&  nRS==1 &&  nMS==1 )
    {
	
	if (dThetaRM<2.) 
	{
	    arr->Add(h);
	    //h->dumpToStdoutSim();
	    //cout<<"all: "<<nMInd<<endl;
	}
    }
    return 0;
}
Int_t HRichCorrCounterSim::selectDalitzTrack(TObjArray* arr,HHitMatchSim* h)
{
    // a pion dalitz lepton and the MDC has seen a segment
    HTrackInfo *trk = h->getTrackInfoObj();
    Int_t nRInd = h->getRichInd();
    Int_t nMInd = h->getMdcInd();
    Int_t nTInd = h->getTofInd();
    Int_t nSInd = h->getShowInd();

    Int_t nRM = h->getMatchedRichMdc();
    Int_t nRT = h->getMatchedRichTof();
    Int_t nMT = h->getMatchedMdcTof();
    Int_t nRS = h->getMatchedRichShower();
    Int_t nMS = h->getMatchedMdcShower();
    Float_t dThetaRM = TMath::Abs(h->getRichTheta()-h->getMdcTheta());
    if (nRInd!=-1 && nTInd!=-1 && nMInd!=-1
	&& nRM==1 &&  nRT==1 &&  nMT==1 )
   	
    {
	for (Int_t k=0;k<MAXPARTICLES;k++)
	{
	    if (trk->getPi0Dalitz(k)==1 && trk->getMatchedRMT_S(k)==1) 
	    {
		if (dThetaRM<2.) 
		{
		    arr->Add(h);
		    //h->dumpToStdoutSim();
		    //cout<<"dal: "<<nMInd<<endl;
		}
		return 0;
	    }
	}
    }

    else if (nRInd!=-1 && nSInd!=-1 && nMInd!=-1
	     && nRM==1 &&  nRS==1 &&  nMS==1 )
   	
    {
	for (Int_t k=0;k<MAXPARTICLES;k++)
	{
	    if (trk->getPi0Dalitz(k)==1 && trk->getMatchedRMT_S(k)==1) 
	    {
		if (dThetaRM<2.) 
		{
		    arr->Add(h);
		    //h->dumpToStdoutSim();
		    //cout<<"dal: "<<nMInd<<endl;
		}
		return 0;
	    }
	}
    }

    return 0;
}

Int_t HRichCorrCounterSim::selectConvTrack(TObjArray* arr,HHitMatchSim* h)
{
    // a conv lepton  and the MDC has seen a segment 
    HTrackInfo *trk = h->getTrackInfoObj();
    Int_t nRInd = h->getRichInd();
    Int_t nMInd = h->getMdcInd();
    Int_t nTInd = h->getTofInd();
    Int_t nSInd = h->getShowInd();

    Int_t nRM = h->getMatchedRichMdc();
    Int_t nRT = h->getMatchedRichTof();
    Int_t nMT = h->getMatchedMdcTof();
    Int_t nRS = h->getMatchedRichShower();
    Int_t nMS = h->getMatchedMdcShower();
    Float_t dThetaRM = TMath::Abs(h->getRichTheta()-h->getMdcTheta());
    if (nRInd!=-1 && nTInd!=-1 && nMInd!=-1
	&& nRM==1 &&  nRT==1 &&  nMT==1 )
    {   	
	for (Int_t k=0;k<MAXPARTICLES;k++)
	{
	    if (trk->getConvLep(k)==1 && trk->getMatchedRMT_S(k)==1) 
	    {
		//cout<<"TOF conv added"<<endl;
		if (dThetaRM<2.) 
		{
		    arr->Add(h);
		    //h->dumpToStdoutSim();
		    //cout<<"conv: "<<nMInd<<endl;
		}
		return 0;
	    }
	}
    }

    else if (nRInd!=-1 && nSInd!=-1 && nMInd!=-1
	     && nRM==1 &&  nRS==1 &&  nMS==1 )
   	
    {
	for (Int_t k=0;k<MAXPARTICLES;k++)
	{
	    if (trk->getConvLep(k)==1 && trk->getMatchedRMT_S(k)==1) 
	    {
		//cout<<"SHOWER conv added"<<endl;
		if (dThetaRM<2.) 
		{
		    arr->Add(h);
		    //h->dumpToStdoutSim();
		    //cout<<"conv: "<<nMInd<<endl;
		}
		return 0;
	    }
	}
    }

    return 0;
}
Int_t HRichCorrCounterSim::fillGeantMdcTofdTheta(HHitMatchSim* h,Int_t evtsize,Int_t* m,Int_t* t)
{

  Int_t mi,ti;
    mi=ti=-1;
    //check if it is a valid MDC-TOF matched tracklet
    if (h->getMdcInd()  > -1 && h->getTofInd()  > -1 && 
	h->getMatchedMdcTof() == 1 && h->getMatchedRichMdc()==1 &&
	h->getMatchedRichTof()==1)
    {
	mi = h->getMdcInd();
	ti = h->getTofInd();
    }
    else return 0;

    // check whether this bi-corr was used before to fill histo 
    if (checkCombination(mi,ti,m,t,evtsize)==0) return 0;

    // calculate values and fill them in histo
    Float_t mt,tt,rt;
    mt=tt=rt=0.;
    mt=h->getMdcTheta();
    tt=h->getTofTheta();
    rt=h->getRichTheta();
    Float_t dThetaMT=mt-tt;//MDC-TOF theta
    Float_t dThetaRM=TMath::Abs(mt-rt);//RICH-MDC theta
    if (dThetaRM>2.) return 0;

    HTrackInfo *trk = h->getTrackInfoObj();
    Bool_t isRealTracklet=kFALSE;
    Int_t nPartInd=-1;
    //Int_t maxNbGPart = trk->getPartNr();
    //    cout<<maxNbGPart<<endl;


    // more than one entry is allowed
    // as more than one GEANT particle may be stored in this tracklet
    // they could not be resolved by the detectors
    for (Int_t k=0;k<MAXPARTICLES;k++)
    {
	//if (trk->getMatchedRM(k) > 0 && trk->getMatchedMT_S(k) > 0)
	if (trk->getPi0Dalitz(k)==1 || trk->getConvLep(k)==1)
	{
	    isRealTracklet=kTRUE;
	    nPartInd=k;
	}
    
	if (isRealTracklet)
	{
	    if (nPartInd>-1)
	    {
		Int_t nPartId = trk->getParId(nPartInd);
		Int_t mech    = trk->getMech(nPartInd);
		Int_t med     = trk->getMed(nPartInd);
		Float_t mom   = trk->getTotMom(nPartInd);
		Int_t cId     = trk->getCreaId(nPartInd);
		
		

		pH_ParIdMdcTofdTheta  -> Fill(nPartId,dThetaMT);
		pH_MechMdcTofdTheta   -> Fill(mech,dThetaMT);
		pH_MedMdcTofdTheta    -> Fill(med,dThetaMT);
		pH_MomMdcTofdTheta    -> Fill(mom,dThetaMT);
		pH_CreaIdMdcTofdTheta -> Fill(cId,dThetaMT);
		pH_ParIdMech          -> Fill(nPartId,mech);
		pH_ParIdMom           -> Fill(nPartId,mom);
		pH_ParIdMed           -> Fill(nPartId,med);
		pH_ParIdCreaId        -> Fill(nPartId,cId);
		//split it up
		if (nPartId==2)//positrons
		{
		    pH_pos_MechMdcTofdTheta  -> Fill(mech,dThetaMT);
		    pH_pos_MedMdcTofdTheta   -> Fill(med,dThetaMT);
		    pH_pos_MomMdcTofdTheta   -> Fill(mom,dThetaMT);
		    pH_pos_CreaIdMdcTofdTheta-> Fill(cId,dThetaMT);
		}
		if (nPartId==3)//electrons
		{
		    pH_ele_MechMdcTofdTheta  -> Fill(mech,dThetaMT);
		    pH_ele_MedMdcTofdTheta   -> Fill(med,dThetaMT);
		    pH_ele_MomMdcTofdTheta   -> Fill(mom,dThetaMT);
		    pH_ele_CreaIdMdcTofdTheta-> Fill(cId,dThetaMT);
		}
		
	    }//end if valid index
	}//end if isRealTracklet
	isRealTracklet=kFALSE;
    }//end particle array loop
    return 1;

}

Int_t HRichCorrCounterSim::fillGeantMdcShowerdTheta(HHitMatchSim* h,Int_t evtsize,Int_t* m,Int_t* s)
{

  Int_t mi,si;
    mi=si=-1;
    //check if it is a valid MDC-TOF matched tracklet
    if (h->getMdcInd()  > -1 && h->getShowInd()  > -1 && 
	h->getMatchedMdcShower() == 1 && h->getMatchedRichMdc()==1 &&
	h->getMatchedRichShower()==1)
    {
	mi = h->getMdcInd();
	si = h->getShowInd();
    }
    else return 0;

    // check whether this bi-corr was used before to fill histo 
    if (checkCombination(mi,si,m,s,evtsize)==0) return 0;

    // calculate values and fill them in histo
    Float_t mt,st,rt;
    mt=st=rt=0.;
    mt=h->getMdcTheta();
    st=h->getShowerTheta();
    rt=h->getRichTheta();
    Float_t dThetaMS=mt-st;//MDC-TOF theta
    Float_t dThetaRM=TMath::Abs(mt-rt);//RICH-MDC theta
    if (dThetaRM>2.) return 0;
    
    HTrackInfo *trk = h->getTrackInfoObj();
    Bool_t isRealTracklet=kFALSE;
    Int_t nPartInd=-1;
    Int_t maxNbGPart = trk->getPartNr();
    //    cout<<maxNbGPart<<endl;
    for (Int_t k=0;k<MAXPARTICLES;k++)
    {
	//	if (trk->getMatchedRM(k) > 0 && trk->getMatchedMT_S(k) > 0) 
	if (trk->getPi0Dalitz(k)==1 || trk->getConvLep(k)==1)
	    {
		isRealTracklet=kTRUE;
		nPartInd=k;
	    }
    
	if (isRealTracklet)
	{
	    if (nPartInd>-1)
	    {
		Int_t nPartId = trk->getParId(nPartInd);
		Int_t mech    = trk->getMech(nPartInd);
		Int_t med     = trk->getMed(nPartInd);
		Float_t mom   = trk->getTotMom(nPartInd);
		Int_t cId     = trk->getCreaId(nPartInd);
	  	
	
		pH_ParIdMdcShowerdTheta -> Fill(nPartId,dThetaMS);
		pH_MechMdcShowerdTheta  -> Fill(mech,dThetaMS);
		pH_MedMdcShowerdTheta   -> Fill(med,dThetaMS);
		pH_MomMdcShowerdTheta   -> Fill(mom,dThetaMS);
		pH_CreaIdMdcShowerdTheta-> Fill(cId,dThetaMS);
		pH_ParIdMech            -> Fill(nPartId,mech);
		pH_ParIdMom             -> Fill(nPartId,mom);
		pH_ParIdMed             -> Fill(nPartId,med);
		pH_ParIdCreaId          -> Fill(nPartId,cId);
		if (nPartId==2)//positrons
		{
		    pH_pos_MechMdcShowerdTheta  -> Fill(mech,dThetaMS);
		    pH_pos_MedMdcShowerdTheta   -> Fill(med,dThetaMS);
		    pH_pos_MomMdcShowerdTheta   -> Fill(mom,dThetaMS);
		    pH_pos_CreaIdMdcShowerdTheta-> Fill(cId,dThetaMS);
		}
		if (nPartId==3)//electrons
		{
		    pH_ele_MechMdcShowerdTheta  -> Fill(mech,dThetaMS);
		    pH_ele_MedMdcShowerdTheta   -> Fill(med,dThetaMS);
		    pH_ele_MomMdcShowerdTheta   -> Fill(mom,dThetaMS);
		    pH_ele_CreaIdMdcShowerdTheta-> Fill(cId,dThetaMS);
		}
	    
	    }//end if valid index
	}//end if isRealTracklet
	isRealTracklet=kFALSE;
    }//end loop over GEANT particles

    return 1;
}

Int_t HRichCorrCounterSim::fillGeantMdcTofdThetanoGCond(HHitMatchSim* h,Int_t evtsize,Int_t* m,Int_t* t)
{
    //do not require a condition on Geant info
    //in order to use the Geant info
  Int_t mi,ti;
    mi=ti=-1;
    //check if it is a valid MDC-TOF matched tracklet
    if (h->getMdcInd()  > -1 && h->getTofInd()  > -1 && 
	h->getMatchedMdcTof() == 1 && h->getMatchedRichMdc()==1 &&
	h->getMatchedRichTof()==1)
    {
	mi = h->getMdcInd();
	ti = h->getTofInd();
    }
    else return 0;

    // check whether this bi-corr was used before to fill histo 
    if (checkCombination(mi,ti,m,t,evtsize)==0) return 0;

    // calculate values and fill them in histo
    Float_t mt,tt,rt;
    mt=tt=rt=0.;
    mt=h->getMdcTheta();
    tt=h->getTofTheta();
    rt=h->getRichTheta();
    //Int_t maxNbGPart = h->getNbGPart();//FIXME: gave always -1, why ??
    HTrackInfo *trk = h->getTrackInfoObj();
    Int_t maxNbGPart = trk->getPartNr();
    //cout<<"Max nb of particles: "<<maxNbGPart<<endl;

    for (Int_t k=0;k<maxNbGPart;k++)
    {
	if (trk->getTrkNb(k) == -1) Error("fillGeantMdcTofdThetanoGCond","wrong particle number");
	Int_t nPartId = trk->getParId(k);
	Int_t mech    = trk->getMech(k);
	Int_t med     = trk->getMed(k);
	Float_t mom   = trk->getTotMom(k);
	Int_t cId     = trk->getCreaId(k);
	Float_t dThetaMT=mt-tt;//MDC-TOF theta
	Float_t dThetaRM=TMath::Abs(mt-rt);//RICH-MDC theta
	
	//cout<<dTheta<<" ////// "<<mi<<"::"<<ti<<" ////// "<<nCounterProcessedNbEvents<<"*********"<<endl;
	if (dThetaRM>2.) return 0;
	pH_ParIdMdcTofdTheta->Fill(nPartId,dThetaMT);
	pH_MechMdcTofdTheta->Fill(mech,dThetaMT);
	pH_MedMdcTofdTheta->Fill(med,dThetaMT);
	pH_MomMdcTofdTheta->Fill(mom,dThetaMT);
	pH_CreaIdMdcTofdTheta->Fill(cId,dThetaMT);
    }
    
    return 1;

}


