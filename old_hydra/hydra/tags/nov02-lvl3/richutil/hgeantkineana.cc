// $Id: hgeantkineana.cc,v 1.9 2003-01-20 16:16:25 halo Exp $
// Last update by Thomas Eberl: 02/10/15 19:55:10
//
#include "hgeantkineana.h"
#include "hgeantrich.h"
#include "htrackinfo.h"
#include "hgeantkine.h"
#include "hhitmatchsim.h"
#include "hdihitmatch.h"
#include "hconstant.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hrichdetector.h"
#include "hparset.h"
#include "hrichhitsim.h"
#include "hrichutilfunc.h"
#include "hruntimedb.h"
#include "hevent.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hrichdetector.h"
#include "hcategory.h"
#include "hiterator.h"
#include "hdebug.h"
#include "hades.h"
#include "richdef.h"
#include "hlinearcategory.h"
#include "hmatrixcategory.h"
#include "hgeomvector2.h"
#include "hrichgeometrypar.h"
#include "hrichcut.h"
#include <fstream>
///////////////////////////////////////////////////////////
// HGeantKineAna
//
// this class analyses HGeantKine
// and fills histograms
///////////////////////////////////////////////////////////

ClassImp(HGeantKineAna)

HGeantKineAna::HGeantKineAna(Text_t *name,Text_t *title) :
  HReconstructor(name,title)
{
     
}
HGeantKineAna::HGeantKineAna(Text_t *name,Text_t *title, char* filename, char* evtgen) :
  HReconstructor(name,title)
{
    pFileName  = filename;
    pEvtGen    = evtgen;
}
HGeantKineAna::HGeantKineAna()
{

}


HGeantKineAna::~HGeantKineAna(void) 
{

}

Bool_t HGeantKineAna::init() {
    if (gHades) {
	HEvent *event=gHades->getCurrentEvent();
	HRuntimeDb *rtdb=gHades->getRuntimeDb();
	HSpectrometer *spec=gHades->getSetup();
	HRichDetector *rich = (HRichDetector*)spec->getDetector("Rich");
	if (event && rtdb && rich) {
	    	    
	    HRichGeometryPar *pGeoParl =
		(HRichGeometryPar*)rtdb->getContainer("RichGeometryParameters");
	    setGeoPar(pGeoParl);
            if (!pGeoPar) Error("init","no geometry cntr defined");
	    
	    fRichPID=gHades->getCurrentEvent()->getCategory(catRichHit);
	    if (!fRichPID) Warning("init","no HRichHit cat");
    
	    if (fRichPID) fRichIter = (HIterator*) fRichPID->MakeIterator();
	    if(!fRichIter) Warning("init","no RichHitSim iter defined");

	    // HGEANT RICH MIRROR INFO
	    fGeantRichMirrorCat = (HMatrixCategory*)(gHades->getCurrentEvent()
                             ->getCategory(catRichGeantRaw+2));  
	    if (!fGeantRichMirrorCat)
	    {
		Error("HRichCorrelatorSim::init",
		      "no GEANT RICH MIRROR category available");
	    }
	    iter_mirror = (HIterator *)fGeantRichMirrorCat->MakeIterator("native");
	    // HGEANT MDC RAW INFO
	    fGeantMdcCat = (HMatrixCategory*)(gHades->getCurrentEvent()
                             ->getCategory(catMdcGeantRaw));  
	    if (!fGeantMdcCat)
	    {
		Error("HRichCorrelatorSim::init",
		      "no GEANT MDC RAW category available");
	    }
	    iter_mdcgeant = (HIterator *)fGeantMdcCat->MakeIterator("native");

	    // HGEANT KINE INFO
	    fGeantKineCat =(HLinearCategory*) event->getCategory(catGeantKine);
	    if (!fGeantKineCat) 
	    { 
		Error("HGeantKineAna::init",
		      "no GEANT Kine category available");
	    }  
	    setGeantKineCat(fGeantKineCat);
	    //	    iter_kine = (HIterator *)fGeantKineCat ->MakeIterator("native");
	    iter_kine = (HIterator *) getGeantKineCat()->MakeIterator("native");

	    // TRACKS
	    pHitMatchCat=event->getCategory(catMatchHit);
	    if (!pHitMatchCat) {
		Warning("init","No HIT MATCH category defined");
		return kFALSE;
	    }
	    if (pHitMatchCat) pIterMatchHit = (HIterator*)getHitMatchCat()->MakeIterator("native");
	    // PAIRS
  	    pDiHitMatchCat=event->getCategory(catDiMatchHit);
  	    if (!pDiHitMatchCat) {
  		Warning("init","No DI HIT MATCH category defined");
  		return kFALSE;
  	    }
	    if (pDiHitMatchCat) pIterDiHitMatch = (HIterator*)getDiHitMatchCat()->MakeIterator("native");
	    
	  
	} else {
	    Error ("init","! (event && rtdb)");
	    return kFALSE; //! (event && rtdb)
	}
	
//  	iniCuts();
  	iniSwitches();
	iniControlHistos();
  	iniCounters();
	pFileOut = new TFile(pFileName,"RECREATE"); // histo file
	el = new TEventList("rings","rings",100);
	
	return kTRUE;
    } else {
	Error ("init","! (gHades)");
	return kFALSE; //! (gHades)		
    }
}
void HGeantKineAna::iniSwitches()
{
    kDumpIt = kTRUE; //activate dumping to stdout of kine
}
void HGeantKineAna::iniCounters()
{
    //nCounterProcessedNbEvents=0;
    nEvtsProcessed=0;
    nbRings=0;
    nbLeptons=0;
    nbDalitzPairs=0;
    nbDalitzSingle=0;
    nbTracks=0;
    nbTrackPairs=0;
    nbgMatchedPairs=0;
    nbgMatchedTracks=0;
    nbgMatchedGPairs=0;
    nbgMatchedGTracks=0;
}

void HGeantKineAna::iniControlHistos()
{
    pHistArray = new TObjArray(12);
    med_conv = new TH1D("Medium_conv","Medium_conv",41,0,42);
    pHistArray->Add(med_conv);
    mech_conv = new TH1D("Mechanism_conv","Mechanism_conv",36,0,35);
    pHistArray->Add(mech_conv);
    par_conv = new TH1D("Parent_conv","Parent_conv",56,0,55);
    pHistArray->Add(par_conv);
    par_med_conv = new TH2D("parent_medium_conv","parent_medium_conv",56,0,55,46,0,45);
    pHistArray->Add(par_med_conv);
    par_mech_conv = new TH2D("parent_mechanism_conv","parent_mechanism_conv",56,0,55,36,0,35);
    pHistArray->Add(par_mech_conv);
    mech_mom_conv = new TH2D("mechanism_momentum_conv","mechanism_momentum_conv",36,0,35,100,0,1000);
    pHistArray->Add(mech_mom_conv);
    g_conv_opangle = new TH1F("pi0_2gamma_opangle","pi0->2*gamma_opangle",180,0,180);
    pHistArray->Add(g_conv_opangle);
    deltaP_opangle = new TH2F("delta_AbsP_opangle","delta_Abs(P)__opangle",100,0,1000,180,0,180);
    pHistArray->Add(deltaP_opangle);
    // *********************************************************
    med_dalitz = new TH1D("Medium_dalitz","Medium_dalitz",41,0,42);
    pHistArray->Add(med_dalitz);
    mech_dalitz = new TH1D("Mechanism_dalitz","Mechanism_dalitz",36,0,35);
    pHistArray->Add(mech_dalitz);
    par_dalitz = new TH1D("Parent_dalitz","Parent_dalitz",56,0,55);
    pHistArray->Add(par_dalitz);
    par_med_dalitz = new TH2D("parent_medium_dalitz","parent_medium_dalitz",56,0,55,46,0,45);
    pHistArray->Add(par_med_dalitz);
    par_mech_dalitz = new TH2D("parent_mechanism_dalitz","parent_mechanism_dalitz",56,0,55,36,0,35);
    pHistArray->Add(par_mech_dalitz);
    mech_mom_dalitz = new TH2D("mechanism_momentum_dalitz","mechanism_momentum_dalitz",36,0,35,100,0,1000);
    pHistArray->Add(mech_mom_dalitz);
    // ***********************************************************
    // *** Kine histograms ***
    hidkine = new TH1D("ID_kine","ID_kine",50,0,50);
    pHistArray->Add(hidkine);
    hmedkine = new TH1D("Medium_kine","Medium_kine",41,0,42);
    pHistArray->Add(hmedkine);
    hmechkine = new TH1D("Mechanism_kine","Mechanism_kine",36,0,35);
    pHistArray->Add(hmechkine);
    hmomkine = new TH1D("Momentum_kine","Momentum_kine",25,0,500);
    pHistArray->Add(hmomkine);
    hmomthetakine = new TH2D("Momentum_theta_kine","Momentum_theta_kine",180,0,180,100,0,500);
    pHistArray->Add(hmomthetakine);
    hmompairkine = new TH2D("Momentum_pair_kine","Momentum_pair_kine",100,0,500,100,0,500);
    pHistArray->Add(hmompairkine);
    hthetapairkine = new TH2D("polangle_pair_kine","polangle_pair_kine",180,0,180,180,0,180);
    pHistArray->Add(hthetapairkine);
    hvertexkine = new TH3D("Vertex_kine","Vertex_kine",100,-10,10,
			   100,-10,10,100,-10,10);
    pHistArray->Add(hvertexkine);
    hparentkine  = new TH1D("parent_kine","parent_kine",50,0,50);
    pHistArray->Add(hparentkine);
    // tmp histo to check dist of center
    hcntdistx = new TH1D("xdist","xdist",21,-10,10);
    pHistArray->Add(hcntdistx);
    hcntdisty = new TH1D("ydist","ydist",21,-10,10);
    pHistArray->Add(hcntdisty);
    hcntdistxy = new TH2D("xydist","xydist",21,-10,10,21,-10,10);
    pHistArray->Add(hcntdistxy);
    hweight = new TH1D("weight","weight",40,0,1.1);
    pHistArray->Add(hweight);
    hweightdist = new TH2D("weightdist","weightdist",40,0,1.1,22,-1,10.);
    pHistArray->Add(hweightdist);

    hpi0invmass = new TH1D("pi0mass","pi0mass",150,0,149);
    pHistArray->Add(hpi0invmass);
    hdalitzopang = new TH1D("Dalitzopang","Dalitzopang",101,0,100);
    pHistArray->Add(hdalitzopang);
    hdalitzinvmass = new TH1D("Dalitzinvmass","Dalitzinvmass",151,0,150);
    pHistArray->Add(hdalitzinvmass);

    hgammathetatheta = new TH2D("pi0gammatheta_theta","pi0gammatheta_theta",
				180,0,180,180,0,180);
    pHistArray->Add(hgammathetatheta);
    hgammamommom = new TH2D("pi0gammamom_mom","pi0gammamom_mom",
				100,0,500,100,0,500);
    pHistArray->Add(hgammamommom);
    hgammamom1theta2 = new TH2D("pi0gammamom1_theta2","pi0gammamom1_theta2",
				100,0,500,180,0,180);
    pHistArray->Add(hgammamom1theta2);
    hgammamomtheta = new TH2D("pi0gammamom_theta","pi0gammamom_theta",
				100,0,500,180,0,180);
    pHistArray->Add(hgammamomtheta);

    // histos for efficiency
    hmomthetaallsingle = new TH2D("Momentum_theta_allsingle","Momentum_theta_allsingle",30,0,300,20,0,90);
    pHistArray->Add(hmomthetaallsingle);
    hmomthetareconsingle = new TH2D("Momentum_theta_reconsingle","Momentum_theta_reconsingle",30,0,300,20,0,90);
    pHistArray->Add(hmomthetareconsingle);

    hmomthetaallpair = new TH2D("Momentum_theta_allpair","Momentum_theta_allpair",30,0,300,20,0,90);
    pHistArray->Add(hmomthetaallpair);
    hmomthetareconpair = new TH2D("Momentum_theta_reconpair","Momentum_theta_reconpair",30,0,300,20,0,90);
    pHistArray->Add(hmomthetareconpair);

}

void HGeantKineAna::findPionDalitzLeptons()
{
    HGeantKine * kine =0;
    // loop over kine container
    iter_kine->Reset();
    while((kine=(HGeantKine *)iter_kine->Next())!=0)
    {
	Int_t aTrack, aID;
	Int_t aPar, aMed, aMech;
	//Float_t ax, ay, az;
	//Float_t apx, apy, apz;
	//Float_t aInfo, aWeight;
	Float_t ptot;
	kine->getParticle(aTrack,aID);
	kine->getCreator(aPar,aMed,aMech);
	//kine->getVertex(ax,ay,az);
	//kine->getMomentum(apx,apy,apz);
	//kine->getGenerator(aInfo,aWeight);
	ptot=kine->getTotalMomentum();

	// aID==2 is e+
	// aID==3 is e-
	// aMech==6 is photo pair production
	// aMed==8 is RICH radiator ?? aMed==14 is target ??
	// aMed==9 is RICH window ?? aMed==10 is RICH carb shell ??
	// aMed==13 is RICH glass mirror ??
//  	if ( aID == 2 && aMech == 6 && 
//  	     //  (aMed == 8 || aMed == 14 || aMed == 9 
//  	     // || aMed == 10 || aMed == 13)) pPositrons->Add(kine);
//  	     aMed == 9) pPositrons->Add(kine);
//  	if ( aID == 3 && aMech == 6 && 
//  	     //(aMed == 8 || aMed == 14 || aMed == 9
//  	     // || aMed == 10 || aMed == 13)) pElectrons->Add(kine);
//  	     aMed == 9) pElectrons->Add(kine);
	//	if ( ( aID == 2 || aID==3 ) && aMech != 10 )  dumpKine(kine);


	if ( aMech==5 && (aID == 2 || aID==3) && (aMed>=8 && aMed<=19)) // direct particle decay
	{
	    HGeantKine *kine_parent1 = HRichUtilFunc::findParent(kine,(HLinearCategory*) getGeantKineCat());
	    if(kine_parent1){//parent should be pion
		Float_t theta1,phi1;
		theta1=phi1=0.;
		HRichUtilFunc::calcParticleAnglesKine(kine_parent1,theta1,phi1);
		Int_t aTrackp1, aIDp1;
		kine_parent1->getParticle(aTrackp1,aIDp1);
		//if(aIDp1==7 && (theta1>=10. && theta1<=90.))//pion
		if(aIDp1==7)//pion, but no emission angle cut
		{ //in acceptance
		    HGeantKine *secondlepton = 
			HRichUtilFunc::getSecondPionDalitzLepton(kine,(HLinearCategory*) getGeantKineCat());
		    if (secondlepton)
		    {
			Float_t deltaptot = 
			    TMath::Abs(ptot-secondlepton->getTotalMomentum());
			Float_t opangle = 
			    HRichUtilFunc::calcOpeningAngleKine(kine,secondlepton);
			deltaP_opangle->Fill(deltaptot,opangle);
		    }
		    med_dalitz->Fill(aMed);
		    mech_dalitz->Fill(aMech);
		    par_dalitz->Fill(aIDp1);
		    par_med_dalitz->Fill(aID,aMed);
		    par_mech_dalitz->Fill(aID,aMech);
		    mech_mom_dalitz->Fill(aMech,ptot);
		}//pi0
	    }//kine_parent1
	}//Mec==5 and lepton
	//	    if (kDumpIt) dumpKine(kine);
	//  if (kDumpIt) dumpKine(findParent(kine));

    } // end while kine loop


}
void HGeantKineAna::findPionGammaConversionLeptons()
{

    HGeantKine * kine =0;
    // loop over kine container
    iter_kine->Reset();
    while((kine=(HGeantKine *)iter_kine->Next())!=0)
    {
	Int_t aTrack, aID;
	Int_t aPar, aMed, aMech;
	//Float_t ax, ay, az;
	//Float_t apx, apy, apz;
	//Float_t aInfo, aWeight;
	Float_t ptot;
	kine->getParticle(aTrack,aID);
	kine->getCreator(aPar,aMed,aMech);
	//kine->getVertex(ax,ay,az);
	//kine->getMomentum(apx,apy,apz);
	//kine->getGenerator(aInfo,aWeight);
	ptot=kine->getTotalMomentum();

	// aID==2 is e+
	// aID==3 is e-
	// aMech==6 is photo pair production
	// aMed==8 is RICH radiator ?? aMed==14 is target ??
	// aMed==9 is RICH window ?? aMed==10 is RICH carb shell ??
	// aMed==13 is RICH glass mirror ??
//  	if ( aID == 2 && aMech == 6 && 
//  	     //  (aMed == 8 || aMed == 14 || aMed == 9 
//  	     // || aMed == 10 || aMed == 13)) pPositrons->Add(kine);
//  	     aMed == 9) pPositrons->Add(kine);
//  	if ( aID == 3 && aMech == 6 && 
//  	     //(aMed == 8 || aMed == 14 || aMed == 9
//  	     // || aMed == 10 || aMed == 13)) pElectrons->Add(kine);
//  	     aMed == 9) pElectrons->Add(kine);
	//	if ( ( aID == 2 || aID==3 ) && aMech != 10 )  dumpKine(kine);


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
			//  	    cout<<"before dumpKine "<<kine_parent2<<endl;
			//			  	    dumpKine(kine_parent2);
			kine_parent2->getParticle(aTrackp2,aIDp2);
			Float_t theta2,phi2;
			theta2=phi2=0.;
			HRichUtilFunc::calcParticleAnglesKine(kine_parent2,theta2,phi2);
			if(aIDp2==7 && (theta2>=10. && theta2<=90.))
			    //if(aIDp2==7)
			{//pi0 in acceptance
			    HGeantKine *secondgamma = HRichUtilFunc::getSecondPionDecayGamma(kine_parent1,(HLinearCategory*) getGeantKineCat());
			    
			    if (secondgamma) g_conv_opangle->Fill(HRichUtilFunc::calcOpeningAngleKine(kine_parent1,secondgamma));

			    med_conv->Fill(aMed);
			    mech_conv->Fill(aMech);
			    par_conv->Fill(aIDp2);
			    par_med_conv->Fill(aID,aMed);
			    par_mech_conv->Fill(aID,aMech);
			    mech_mom_conv->Fill(aMech,ptot);
			}//pi0
		    }//kine_parent2
		}//gamma
	    }//kine_parent1
	}//Mec==6 and lepton
	//	    if (kDumpIt) dumpKine(kine);
	//  if (kDumpIt) dumpKine(findParent(kine));

    } // end while kine loop


}

Int_t HGeantKineAna::execute()
{
    //cout<<" **** Event number "<<nEvtsProcessed<<" -----------------"<<endl;
    nEvtsProcessed++;
    //cout<<"in execute"<<endl;
    if ( nEvtsProcessed%5000==0) 
    {

	cout<<nEvtsProcessed
	    <<" EVTS PROCESSED ***** saving histos"<<endl;
	HRichUtilFunc::saveHistos(pFileOut,pHistArray);
    }
//      TObjArray *pElectrons = new TObjArray(25);
//      TObjArray *pPositrons = new TObjArray(25);

//      findPionGammaConversionLeptons();
//      findPionDalitzLeptons();

    TObjArray dalitzleptons(1);
    if(findAcceptedDalitzLeptons(dalitzleptons)) 
	checkAcceptedDalitzLeptons(dalitzleptons);


    //showLeptonsFromPi0Decays("dalitz");
    //    showGammasFromPi0Decays();
    // ===========================================================


    // loop over selected e- and e+ (kine objs) and fill histos 

//      Int_t aParE,aParP,aMedE,aMedP,aMechE,aMechP,aMedG,aMechG,aParG;
//      aParE=aParP=aMedE=aMedP=aMechE=aMechP=aMedG=aMechG=aParG=-1;
//      Float_t theta,phi;
//      theta=phi =-1.;
//      for (Int_t i=0;i<(pPositrons->GetLast()+1);i++)
//      {
//  	HGeantKine *positron = (HGeantKine*)(*pPositrons)[i];
//  	if (positron) 
//  	{
//  	    positron->getCreator(aParP,aMedP,aMechP);
//  	    for (Int_t j=0;j<(pElectrons->GetLast()+1);j++)
//  	    {
//  		HGeantKine *electron = (HGeantKine*)(*pElectrons)[j];
//  		if (electron)  
//  		{
//  		    electron->getCreator(aParE,aMedE,aMechE);
		
//  		    if (aParE==aParP && aMedE==aMedP && aMechE==aMechP)
//  		    { 
//  			//cout<<"!!! Conversion pair found !!!"<<endl;
//  			//dumpKine(electron);
//  			//dumpKine(positron);
//  			pHistConvPairSource->Fill(aMedP);
//  			pHistElePosMomConv->Fill(electron->getTotalMomentum(),
//  						 positron->getTotalMomentum());
//  			pHistOpenAngle->Fill(calcOpeningAngleV(electron,
//  							       positron));
//  			calcParticleAnglesV(electron,theta,phi);
//  			pHistElePosMomThetaConv->
//  			    Fill(electron->getTotalMomentum(),
//  				 positron->getTotalMomentum(),
//  				 theta);
			
		       
			
//  		    } // end pair check
//  		} // if electron
//  	    }//end for electrons
//  	}//if positron
//      } //end for positrons

//      aParE=aParP=aMedE=aMedP=aMechE=aMechP=aMedG=aMechG=aParG=-1;
//      Int_t aTrack,aID;
//      aTrack=aID=-1;
//      Float_t vx,vy,vz;
//      vx=vy=vz=-10000.;
//      theta=phi=-1.;
//      for (Int_t k=0;k<(pElectrons->GetLast()+1);k++)
//      {
//  	HGeantKine *electron = (HGeantKine*)(*pElectrons)[k];
//  	if (electron)  
//  	{
//  	    electron->getCreator(aParE,aMedE,aMechE);
//  	    electron->getVertex(vx,vy,vz);
//  	    calcParticleAnglesV(electron,theta,phi);
//  	    pHistConvEleMom->Fill(theta,electron->getTotalMomentum());
//  	    pHistConvPairVertex->Fill(vx,vy,vz);
//  	    HGeantKine *gamma = findParent(electron);
//  	    if (gamma) 
//  	    {
//  		gamma->getParticle(aTrack,aID);
//  		if (aID == 1 && aMechE == 6 && aParE == aTrack) // gamma
//  		{
//  		   calcParticleAnglesV(gamma,theta,phi);
//  		   pHistConvGammaMom->Fill(theta,gamma->getTotalMomentum()); 
//  		}
//  	    }
//  	}
//      }
//      vx=vy=vz=-10000.;
//      for (Int_t l=0;l<(pPositrons->GetLast()+1);l++)
//      {
//  	HGeantKine *positron = (HGeantKine*)(*pPositrons)[l];
//  	if (positron)  
//  	{
//  	    positron->getCreator(aParP,aMedP,aMechP);
//  	    positron->getVertex(vx,vy,vz);
//  	    calcParticleAnglesV(positron,theta,phi);
//  	    pHistConvPosMom->Fill(theta,positron->getTotalMomentum());
//  	    pHistConvPairVertex->Fill(vx,vy,vz);
//  	    HGeantKine *gamma = findParent(positron);
//  	    if (gamma) 
//  	    {
//  		gamma->getParticle(aTrack,aID);
//  		if (aID == 1 && aMechP == 6 && aParP == aTrack) // gamma
//  		{
//  		   calcParticleAnglesV(gamma,theta,phi);
//  		   pHistConvGammaMom->Fill(theta,gamma->getTotalMomentum()); 
//  		}
//  	    }
//  	}
//      }
//      nCounterProcessedNbEvents++;
    
//      delete pElectrons;
//      delete pPositrons;
    return kSkipEvent; //skips writing out event to file


}

HRichHitSim* HGeantKineAna::checkRings(HGeantKine* kine)
{
    // check whether the kine lepton was seen as a ring
    // or at least as part of it.
    Int_t ret_val=0;
    fRichIter->Reset();
    HRichHitSim *r = 0;
    Int_t aTrk, aID;
    kine->getParticle(aTrk,aID);

    Int_t richInd[20];
    Int_t xdistArr[20];
    Int_t ydistArr[20];
    Float_t distArr[20];
    for (Int_t i=0;i<20;i++) richInd[i]=xdistArr[i]=ydistArr[i]=-1;
    for (Int_t j=0;j<20;j++) distArr[j]=-1.;
    Int_t ringCnt=0;
    while((r = (HRichHitSim *)fRichIter->Next()))
    {
	Int_t t1,t2,t3;
	t1=t2=t3=-1;
	t1=r->track1;t2=r->track2;t3=r->track3;
	if ((t1==aTrk) || (t2==aTrk) || (t3==aTrk) ) // the ring contains same track number as the lepton
	{
	    // check whether the lepton went through the mirror where the ring sits.
	        HGeantRichMirror *mirr=0;
		iter_mirror->Reset();
		HRichGeometryPar* pGeo = (HRichGeometryPar*) pGeoPar;
		Float_t fYShift = pGeo->getSectorShift(); // to account for shiftewd volumes in HGEANT ?!
		fYShift = fYShift/TMath::Cos(20./57.3);
		HRichPadTab* pPadsTab = ((HRichGeometryPar*)pGeo)->getPadsPar();
		HRichPad *pPad = 0;
		Int_t xRing=r->getRingCenterX();
		Int_t yRing=r->getRingCenterY();
		//	Int_t secRing=r->getSector();
		// iterate over hits on mirror and compare hit ring x,y with
		// center of reflected photons, calculate pad hit by photon ctr
		while((mirr=(HGeantRichMirror *)iter_mirror->Next())!=0)
		{
		    Int_t mirrTrack,aID;
		    mirr->getTrack(mirrTrack,aID);
		    if (mirrTrack==aTrk) //lepton passed mirror
		    {
			ret_val=1;
			//return 1;
			Float_t x=mirr->getYRing()/10.;//in mm of the padplane
			Float_t y=mirr->getXRing()/10. + fYShift;
			pPad=pPadsTab->getPad(x, // they are swapped, sic !
					      y );// divide by 10 to account for mm->cm
			if (!pPad) Error("checkRings","no pad found for coord");
			//			Int_t sec=mirr->getSector();
			Int_t xMirrPhotCM=pPad->getPadX();
			Int_t yMirrPhotCM=pPad->getPadY();
			Float_t distance = TMath::Sqrt(1.*(xMirrPhotCM-xRing)*(xMirrPhotCM-xRing)+
						       1.*(yMirrPhotCM-yRing)*(yMirrPhotCM-yRing));
			Int_t richind = getRichHitCat()->getIndex(r);
			richInd[ringCnt]=richind;
			distArr[ringCnt]=distance;
			xdistArr[ringCnt]=xMirrPhotCM-xRing;
			ydistArr[ringCnt]=yMirrPhotCM-yRing;
			ringCnt++;

			Float_t padsum = r->weigTrack1+r->weigTrack2+r->weigTrack3;
			Float_t weight =-1.;
			if (aTrk==r->track1) weight = ((Float_t)r->weigTrack1)/padsum;
			else if (aTrk==r->track2) weight = ((Float_t)r->weigTrack2)/padsum;
			else if (aTrk==r->track3) weight = ((Float_t)r->weigTrack3)/padsum;
			//if (distance>5. && weight==1.)
			//{
			    //el->Enter(nEvtsProcessed-1);//account for start event
			    //el->Print("all");
			    //r->dumpToStdout();
			    //}
			//cout<<weight<<endl;
//  			hweight->Fill(weight);
//  			hweightdist->Fill(weight,distance);
//    			hcntdistx->Fill(xMirrPhotCM-xRing);
//      			hcntdisty->Fill(yMirrPhotCM-yRing);
//      			hcntdistxy->Fill(xMirrPhotCM-xRing,yMirrPhotCM-yRing);

			// matching condition via distance
			    //if (TMath::Abs(xMirrPhotCM-xRing)<=2 && 
			    //TMath::Abs(yMirrPhotCM-yRing)<=2 && 
			    //sec==secRing)
			    //{
//  			    cout<<"*********************************"<<endl;
//  			    cout<<"xmirr: "<<xMirrPhotCM<<" ymirr: ";
//  			    cout<<yMirrPhotCM<<" sec: "<<sec<<endl;
//  			    cout<<"xring: "<<xRing<<" yring: "<<yRing<<" sec: "<<sec<<endl;
//  			    cout<<"*********************************"<<endl;
			    // ring position and cm of photons from this lepton agree
			    // this lepton was seen as a ring
			    //return a positive decision for this lepton
			    //return 1;

			    //}


		    }
		}
	}
    }// end while over rich hits

    // now find ring with minimum distance to photon center

    HRichHitSim *rs = 0;
    Int_t sorted[20]; for (Int_t k=0;k<20;k++) sorted[k]=-1;
    TMath::Sort(20, distArr, sorted,kFALSE); //increasing order

//      for (Int_t k=0;k<20;k++) cout<<distArr[sorted[k]]<<" ";
//      cout<<endl;

    Int_t nn=0;
    while (nn<19&&distArr[sorted[nn]]==-1) nn++;
    //    cout<<nn<<endl;
    if (richInd[sorted[nn]]!=-1)
    {
//  	cout<<nn<<" "<<distArr[sorted[nn]]<<endl;
//  	cout<<"rich index: "<<richInd[sorted[nn]]<<endl;

  	rs =(HRichHitSim*)getRichHitCat()
  	    ->getObject(richInd[sorted[nn]]);
	
	Float_t padsum = rs->weigTrack1+rs->weigTrack2+rs->weigTrack3;
	Float_t weight =-1.;
	if (aTrk==rs->track1) weight = ((Float_t)rs->weigTrack1)/padsum;
	else if (aTrk==rs->track2) weight = ((Float_t)rs->weigTrack2)/padsum;
	else if (aTrk==rs->track3) weight = ((Float_t)rs->weigTrack3)/padsum;
	//cout<<weight<<endl;
  	hweight->Fill(weight);
  	hweightdist->Fill(weight,distArr[sorted[nn]]);
  	hcntdistx->Fill(xdistArr[sorted[nn]]);
    	hcntdisty->Fill(ydistArr[sorted[nn]]);
  	hcntdistxy->Fill(xdistArr[sorted[nn]],ydistArr[sorted[nn]]);
    }
    return rs;

}
void HGeantKineAna::showGammasFromPi0Decays()
{

    HGeantKine * kine =0;
    // loop over kine container
    iter_kine->Reset();
    Int_t size = 20;
    Int_t *gammapairs = new Int_t[size];
    for (Int_t k=0;k<size;k++) gammapairs[k]=-2;
    while((kine=(HGeantKine *)iter_kine->Next())!=0)
    {
	Int_t i = getGeantKineCat()->getIndex(kine);
	HGeantKine *secondgamma=0;
  	if(HRichUtilFunc::isGammaFromPi0(kine,
  				  (HLinearCategory*)getGeantKineCat())
	)
	    //if(HRichUtilFunc::isGamma(kine))

	{
	    secondgamma = HRichUtilFunc::getSecondPionDecayGamma(kine,
				  (HLinearCategory*)getGeantKineCat());
	    //secondgamma = kine;
	    Int_t j = getGeantKineCat()->getIndex(secondgamma);
	    
	    if (HRichCut::isNew2Tuple(i,j,gammapairs,size) &&
		secondgamma
		) histoGammas(kine,secondgamma);


	}
    }
    delete [] gammapairs;
}
void HGeantKineAna::histoGammas(HGeantKine* gamma1,HGeantKine* gamma2)
{
    
    Float_t momgamma1 = gamma1->getTotalMomentum();
    Float_t momgamma2 = gamma2->getTotalMomentum();
    
    Float_t theta_gamma1,phi_gamma1;
    Float_t theta_gamma2,phi_gamma2;
    
    HRichUtilFunc::calcParticleAnglesKine(gamma1,theta_gamma1,phi_gamma1);
    HRichUtilFunc::calcParticleAnglesKine(gamma2,theta_gamma2,phi_gamma2);
    hgammathetatheta->Fill(theta_gamma1,theta_gamma2);//account for double counting
    hgammamommom->Fill(momgamma1,momgamma2);//account for double counting
    hgammamomtheta->Fill(momgamma1,theta_gamma1);    
    hgammamomtheta->Fill(momgamma2,theta_gamma2);    
    hgammamom1theta2->Fill(momgamma1,theta_gamma2);

}

void HGeantKineAna::showLeptonsFromPi0Decays(char *swt)
{

    TObjArray leps(4);


    // loop over all particles provided by GEANT
    // and sort out different kinds
    HGeantKine * kine =0;
    iter_kine->Reset();
    while((kine=(HGeantKine *)iter_kine->Next())!=0)
    {


	TString decaymode(swt);
  	if (!decaymode.CompareTo("dalitz") &&
	    HRichUtilFunc::isPi0DalitzLep(kine,
					  (HLinearCategory*)getGeantKineCat(),pEvtGen) )
  	{
  	    histoKine(kine);
  	    leps.Add(kine);
  	}

	if (!decaymode.CompareTo("conv") &&
	    HRichUtilFunc::isPi0ConvLep(kine,
					(HLinearCategory*)getGeantKineCat()) )
	{
	    histoKine(kine);
	    leps.Add(kine);
	}

    	if (!decaymode.CompareTo("lepinMDC") &&
	    HRichUtilFunc::isPi0DalitzLep(kine,(HLinearCategory*)getGeantKineCat(),
					  pEvtGen) &&
      	    HRichUtilFunc::isLepOnMDC(kine,(HMatrixCategory*)getGeantMDCCat())     )
    	{
    	    histoKine(kine);
    	}

  	if (!decaymode.CompareTo("leponmirror") &&
	    HRichUtilFunc::isPi0DalitzLep(kine,(HLinearCategory*)getGeantKineCat(),
					  pEvtGen) &&
    	    HRichUtilFunc::isLepOnMirror(kine,(HMatrixCategory*)getGeantMirrCat())     )
  	{
  	    histoKine(kine);
  	}


	if (!decaymode.CompareTo("dalitzlepinacc") &&
	    HRichUtilFunc::isPi0DalitzLep(kine,(HLinearCategory*)getGeantKineCat(),pEvtGen) &&
  	    HRichUtilFunc::isLepOnMirror(kine,(HMatrixCategory*)getGeantMirrCat())  &&
  	    HRichUtilFunc::isLepOnMDC(kine,(HMatrixCategory*)getGeantMDCCat())     )
  	{
	    histoKine(kine);
	    leps.Add(kine); // store lepton for further analysis
	    cout<<" **** lepton *************************************************"<<endl;
	    HRichUtilFunc::dumpKine(kine);
	    HRichHitSim* rs=0;
  	    if ((rs=checkRings(kine))) // ring with min dist to lepton is returned 
	    {
		
		cout<<" **** corresponding min dist ring ****************************"<<endl; 
		//rs->dumpToStdout();
		
		TObjArray tracks(5);
		// this function finds tracks for this ring and applies a quality cut 
		// to the track, they are provided in array tracks 
		Int_t nbTracks = checkRingCorrelation(rs,tracks);
		if (nbTracks)
		{
		    cout<<" ***** constructed tracks for this ring *********************"<<endl;
		    for (Int_t i=0;i<(tracks.GetLast()+1);i++)
		    {
			((HHitMatchSim*)tracks[i])->dumpToStdoutSim();
		    }

		}

		// if there were any tracks, try to find pairs
		// and provide them in the array pairs
		TObjArray pairs(3);
		if (nbTracks)
		{
                    Int_t nbpairs =0;
		    nbpairs = checkTrackPairs(tracks,pairs);
		}
		
		cout<<" ****  -------------------- ***************************************"<<endl;
	    }
	    else
	    {
		// this lepton was not recognized in the RICH
		// as no ring was found with photons originating from
		// this lepton !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

		//  		HRichUtilFunc::dumpKine(kine);
		//  		el->Enter(nEvtsProcessed-1);
		
	    } // end if ring check

  	} //end if decay mode
	
    }
    
    checkForPair(leps,swt);
    
    
}



Int_t HGeantKineAna::findAcceptedDalitzLeptons(TObjArray& dalitzleps)
{
    // this function fills Dalitz leptons from HGEantKine in dalitzleps 
    Int_t n=0; 
    HGeantKine * kine =0;
    iter_kine->Reset();
    while((kine=(HGeantKine *)iter_kine->Next())!=0)
    {
	//HRichUtilFunc::dumpKine(kine);
	if (HRichUtilFunc::isPi0DalitzLep(kine,(HLinearCategory*)getGeantKineCat(),pEvtGen) &&
	    HRichUtilFunc::isLepOnMirror(kine,(HMatrixCategory*)getGeantMirrCat())  &&
	    HRichUtilFunc::isLepOnMDC(kine,(HMatrixCategory*)getGeantMDCCat())     )
	{
	    if (kine->getTotalMomentum()>=50.){
	    //	    HRichUtilFunc::dumpKine(kine);
		n++;
		dalitzleps.Add(kine); // store lepton for further analysis 
		histoLepton(kine,hmomthetaallsingle);
	    }
	}
	
    }
    nbLeptons+=n;
    return n;//number of found and stored dalitz leptons in event
    
    
}

void HGeantKineAna::histoLepton(HGeantKine* lep,TH2D* h)
{
    if (lep)
    {
  	Float_t ptot=lep->getTotalMomentum();
	Float_t theta,phi;
	HRichUtilFunc::calcParticleAnglesKine(lep,theta,phi);
	h->Fill(ptot,theta);

    }

}
void HGeantKineAna::histoLeptonPair(TObjArray& p,TH2D* h)
{
	histoLepton((HGeantKine*)p[0],h);
	histoLepton((HGeantKine*)p[1],h);

}
void HGeantKineAna::checkAcceptedDalitzLeptons(TObjArray& dalitzleps)
{
    // !!! TObjArrays are auto-expanding !!!

    Int_t nbfoundRings=0;
    Int_t nbfoundGEANTDalitzPairs=0;
    Int_t nbfoundDetTracks = 0;
    Int_t nbfoundDetPairs = 0;
    //Int_t nbfoundGEANTLeptons = dalitzleps.GetLast()+1; // unused
    Int_t nbfoundGEANTSingleDalitzLeptons = 0;
    Int_t nbMatchedPairs = 0;
    Int_t nbMatchedTracks = 0;
    Int_t nbMatchedGPairs = 0;
    Int_t nbMatchedGTracks = 0;
    // unlikely to find more than one pair in event
    // starting from zero, two consecutive leptons are always a pair
    TObjArray dalitzpairs(1);
    
    if (dalitzleps.GetLast()==0) // there is only one lepton !
    {
	// check lepton: has ring(s) ? has track(s) ?
	nbfoundGEANTSingleDalitzLeptons++;
	nbDalitzSingle++;
    }
    else
    {
	// search for pair
	nbfoundGEANTDalitzPairs = fillDalitzPairs(dalitzleps,dalitzpairs);
	nbDalitzPairs+=nbfoundGEANTDalitzPairs;
	if (nbfoundGEANTDalitzPairs)
	{
	    histoLeptonPair(dalitzpairs,hmomthetaallpair);
	    // pair found

	}
	else {} // still no pair found
    }

    // array that holds for each lepton the corresponding 
    // ring whose recognized center is closer to the 
    // center of all reflected photons than for any other
    // ring
    TObjArray mindistrings(1);
    for (Int_t i=0;i<(dalitzleps.GetLast()+1);i++)
    {
	HRichHitSim *r=checkRings((HGeantKine*)dalitzleps[i]);
	if (r)
	{
	    nbfoundRings++;
	    mindistrings.Add(r);
	}
    }
    nbRings+=nbfoundRings;
    if (nbfoundRings)
    {
	// find all tracks that were found in the detectors w/o 
	// additional information from GEANT and that end in
	// the respective ring
	// array to hold track arrays for each ring
	TObjArray foundTracks(1);
	nbfoundDetTracks = getTracksForRings(mindistrings,foundTracks);
	nbTracks+=nbfoundDetTracks;
	nbMatchedTracks = matchGEANTwithFoundTracks(dalitzleps,foundTracks);
	nbgMatchedTracks+=nbMatchedTracks;

	nbMatchedGTracks = matchFoundTrackswithGEANT(dalitzleps,foundTracks);
	nbgMatchedGTracks+=nbMatchedGTracks;
	if (nbfoundDetTracks)
	{
	    // find all pairs formed with the above found tracks in this event
	    TObjArray foundPairs(1);
	    nbfoundDetPairs = getPairsForTracks(foundTracks,foundPairs);
	    nbTrackPairs+=nbfoundDetPairs;
	    nbMatchedPairs = matchGEANTwithFoundPair(dalitzpairs,foundPairs);
	    nbgMatchedPairs+=nbMatchedPairs;
	    nbMatchedGPairs = matchFoundPairwithGEANT(dalitzpairs,foundPairs);
	    nbgMatchedGPairs+=nbMatchedGPairs;
	}// endif found det tracks
    }//end if found rings

    // print statistics about found objects to screen
    

//      cout<<"Evt rings:"<<nbfoundRings<<" leptons:"<<nbfoundGEANTLeptons<<" dal pairs:"<<nbfoundGEANTDalitzPairs
//  	<<" tracks:"<<nbfoundDetTracks<<" pairs:"<<nbfoundDetPairs
//  	<<" singles:"<<nbfoundGEANTSingleDalitzLeptons<<endl;
//      cout<<"Sum rings:"<<nbRings<<" leptons:"<<nbLeptons<<" dal pairs:"<<nbDalitzPairs
//  	<<" tracks:"<<nbTracks<<" pairs:"<<nbTrackPairs
//  	<<" singles:"<<nbDalitzSingle<<endl;
//      cout<<"Nb GEANT confirmed reconstructed  pairs -- in Evt:"<<nbMatchedPairs
//  	<<"  Sum:"<<nbgMatchedPairs<<endl;
//      cout<<"Nb GEANT confirmed reconstructed tracks -- in Evt:"<<nbMatchedTracks
//  	<<"  Sum:"<<nbgMatchedTracks<<endl;
//      cout<<"Nb reconstructed  GEANT pairs -- in Evt:"<<nbMatchedGPairs
//  	<<"  Sum:"<<nbgMatchedGPairs<<endl;
//      cout<<"Nb reconstructed GEANT leptons -- in Evt:"<<nbMatchedGTracks
//  	<<"  Sum:"<<nbgMatchedGTracks<<endl;
}

Int_t HGeantKineAna::matchGEANTwithFoundTracks(TObjArray& Gl,TObjArray& Rt)
{
    // Gl: GEANT leptons
    // Rt: reconstructed tracks


    Int_t confirmedtracks = 0;
    Int_t *trackinds=new Int_t[Rt.GetLast()+1];
    for (Int_t kk=0;kk<(Rt.GetLast()+1);kk++) trackinds[kk]=-2;
    if ((Gl.GetLast()+1)>=1)
    {
	for (Int_t i=0;i<(Gl.GetLast()+1);i++)
	{
	    Int_t aTrack,aID;
	    ((HGeantKine*)Gl[i])->getParticle(aTrack, aID);
	    if ((Rt.GetLast()+1)>=1)
	    {
		for (Int_t j=0;j<(Rt.GetLast()+1);j++)
		{
		    HTrackInfo *t = ((HHitMatchSim*)Rt[j])->getTrackInfoObj();
		    for (Int_t k=0;k<MAXPARTICLES;k++)
		    {
			// count each track only once, even if
			// it contains more than one lepton
			if (t->getTrkNb(k)==aTrack &&
			    HRichCut::isNewIndex(getHitMatchCat()->
						 getIndex((HHitMatchSim*)Rt[j]),
						 trackinds,Rt.GetLast()+1) 
			    )
			{
			    confirmedtracks++;
			    break;
			}
		    }
		}
	    }
	}
    }
    delete [] trackinds;
    return confirmedtracks;
}
Int_t HGeantKineAna::matchFoundTrackswithGEANT(TObjArray& Gl,TObjArray& Rt)
{
    // Gl: GEANT leptons
    // Rt: reconstructed tracks


    Int_t confirmedlepton = 0;
    Int_t *trackinds=new Int_t[Rt.GetLast()+1];
    for (Int_t kk=0;kk<(Rt.GetLast()+1);kk++) trackinds[kk]=-2;
    if ((Gl.GetLast()+1)>=1)
    {
	for (Int_t i=0;i<(Gl.GetLast()+1);i++)
	{
	    Int_t aTrack,aID;
	    ((HGeantKine*)Gl[i])->getParticle(aTrack, aID);
	    if ((Rt.GetLast()+1)>=1)
	    {
		Bool_t lepisSeen=kFALSE;
		for (Int_t j=0;j<(Rt.GetLast()+1);j++)
		{
		    HTrackInfo *t = ((HHitMatchSim*)Rt[j])->getTrackInfoObj();
		    for (Int_t k=0;k<MAXPARTICLES;k++)
		    {
			// accept lepton only if it has its own track
			// the same track for two diff leps is forbidden
			// as this doesn't allow to reconstr and separate
			// the particle
			if (!lepisSeen &&
			    t->getTrkNb(k)==aTrack &&
			    HRichCut::isNewIndex(getHitMatchCat()->
						 getIndex((HHitMatchSim*)Rt[j]),
						 trackinds,Rt.GetLast()+1) 

			    )
			{
			    lepisSeen=kTRUE;
			    confirmedlepton++;
			    //Sl.Add((HGeantKine*)Gl[i]); // store reconstr. lepton
			    histoLepton((HGeantKine*)Gl[i],hmomthetareconsingle);
			    break;
			}
		    }
		}
	    }
	}
    }
    delete [] trackinds;
    return confirmedlepton;
}
Int_t HGeantKineAna::matchGEANTwithFoundPair(TObjArray& Gp,TObjArray& Rp)
{
    // Gp: GEANT pairs
    // Rp: reconstructed pairs
    
    Int_t confirmedpair = 0;

	if ((Gp.GetLast()+1)==2) // simple case with one pair
	{
	    // retrieve track numbers of leptons in pair
	    Int_t aTrack1, aTrack2;
	    Int_t aID1, aID2;
	    ((HGeantKine*)Gp[0])->getParticle(aTrack1, aID1);
	    ((HGeantKine*)Gp[1])->getParticle(aTrack2, aID2);
	    // loop over found pairs
	    for (Int_t i=0;i<(Rp.GetLast()+1);i++)
	    {
		// get indexes of reconst. tracks in pair
		Int_t ind1=((HDiHitMatch*)Rp[i])->getIndTrk1();
		Int_t ind2=((HDiHitMatch*)Rp[i])->getIndTrk2();
		// get tracks
		HHitMatchSim *trk1 = (HHitMatchSim*)(getHitMatchCat()
						     ->getObject(ind1));
		HHitMatchSim *trk2 = (HHitMatchSim*)(getHitMatchCat()
						     ->getObject(ind2));

		// get simulation part of reconstructed track
		HTrackInfo *t1 = trk1->getTrackInfoObj();
		HTrackInfo *t2 = trk2->getTrackInfoObj();

		Bool_t found11=kFALSE;
		Bool_t found12=kFALSE;
		Bool_t found21=kFALSE;
		Bool_t found22=kFALSE;
		// compare up-to MAXPARTICLES trk numbers
		// per reconstructed track to the given two
		for (Int_t k=0;k<MAXPARTICLES;k++)
		{
		    // if both trk numbers are found in one
		    // and none in the other, it is a double
		    // with a fake !! exclude this pair !!
		    if (t1->getTrkNb(k)==aTrack1) found11=kTRUE;
		    if (t1->getTrkNb(k)==aTrack2) found12=kTRUE;
		    if (t2->getTrkNb(k)==aTrack1) found21=kTRUE;
		    if (t2->getTrkNb(k)==aTrack2) found22=kTRUE;
		}
		// good combinations
		if (
		    !( (found11&&found12&&!found21&&!found22) ||
		       (found21&&found22&&!found11&&!found12)
		     )   
		    )   
		{
		    // pair is confirmed now !!
		    confirmedpair++;
		    // here the pair could be added to arr !!
		}
		else
		{
		    trk1->dumpToStdoutSim();
		    trk2->dumpToStdoutSim();
		}
		
	    }//endfor
	}
	else if((Gp.GetLast()+1)<2)
	{
	    //no GEANT pair found, possible recon. fake pair
	}
	else if((Gp.GetLast()+1)>2)
	{
	    //more complicated, for Dalitz not needed
	}
	

	return confirmedpair;


}
Int_t HGeantKineAna::matchFoundPairwithGEANT(TObjArray& Gp,TObjArray& Rp)
{
    // Gp: GEANT pairs
    // Rp: reconstructed pairs
    
    Int_t confirmedpair = 0;

	if ((Gp.GetLast()+1)==2) // simple case with one pair
	{
	    // retrieve track numbers of leptons in pair
	    Int_t aTrack1, aTrack2;
	    Int_t aID1, aID2;
	    ((HGeantKine*)Gp[0])->getParticle(aTrack1, aID1);
	    ((HGeantKine*)Gp[1])->getParticle(aTrack2, aID2);
	    // loop over reconstructed pairs
	    for (Int_t i=0;i<(Rp.GetLast()+1);i++)
	    {
		// get indexes of reconst. tracks in pair
		Int_t ind1=((HDiHitMatch*)Rp[i])->getIndTrk1();
		Int_t ind2=((HDiHitMatch*)Rp[i])->getIndTrk2();
		// get tracks
		HHitMatchSim *trk1 = (HHitMatchSim*)(getHitMatchCat()
						     ->getObject(ind1));
		HHitMatchSim *trk2 = (HHitMatchSim*)(getHitMatchCat()
						     ->getObject(ind2));

		// get simulation part of reconstructed track
		HTrackInfo *t1 = trk1->getTrackInfoObj();
		HTrackInfo *t2 = trk2->getTrackInfoObj();

		Bool_t found11=kFALSE;
		Bool_t found12=kFALSE;
		Bool_t found21=kFALSE;
		Bool_t found22=kFALSE;
		// compare up-to MAXPARTICLES trk numbers
		// per reconstructed track to the given two from the leps
		for (Int_t k=0;k<MAXPARTICLES;k++)
		{
		    // if both trk numbers are found in one
		    // and none in the other, it is a double
		    // with a fake !! exclude this pair !!
		    if (t1->getTrkNb(k)==aTrack1) found11=kTRUE;
		    if (t1->getTrkNb(k)==aTrack2) found12=kTRUE;
		    if (t2->getTrkNb(k)==aTrack1) found21=kTRUE;
		    if (t2->getTrkNb(k)==aTrack2) found22=kTRUE;
		}
		// good combinations
		if (
		    !( (found11&&found12&&!found21&&!found22) ||
		       (found21&&found22&&!found11&&!found12)
		     )   
		    )   
		{
		    // pair is confirmed now !!
		    confirmedpair++;
		    histoLeptonPair(Gp,hmomthetareconpair);
		    break; // stop looping over reconstructed pairs
		    // as the "real" pair was already found
		    // here the pair could be added to an arr !!
		}
		else
		{
		    trk1->dumpToStdoutSim();
		    trk2->dumpToStdoutSim();
		}
		
	    }//endfor reconstr. pairs
	}
	else if((Gp.GetLast()+1)<2)
	{
	    //no GEANT pair found, possible recon. fake pair
	}
	else if((Gp.GetLast()+1)>2)
	{
	    //more complicated, for Dalitz not needed
	}
	

	return confirmedpair;


}

Int_t HGeantKineAna::getPairsForTracks(TObjArray& tracks,TObjArray& pairs)
{

    HDiHitMatch *h=0;
    pIterDiHitMatch->Reset();
    Int_t nbFoundPairs=0;
    while(( h= (HDiHitMatch *)pIterDiHitMatch->Next())) 
    {
	//h->dumpToStdout();
	// indexes that made pair
	Int_t ind1=h->getIndTrk1();
	Int_t ind2=h->getIndTrk2();
	Int_t nbfoundtracksforpair=0;
	Int_t *trkinds = new Int_t[tracks.GetLast()+1];
	for (Int_t kk=0;kk<(tracks.GetLast()+1);kk++)trkinds[kk]=-2; 
//  	cout<<"ind1: "<<ind1<<"  ind2: "<<ind2<<endl;
//  	cout<<"track index: ";
	for (Int_t i=0;i<(tracks.GetLast()+1);i++)
	{
	    //	    ((HHitMatchSim*)tracks[i])->dumpToStdout();
	    // track index in track category
	    Int_t trkind=getHitMatchCat()->getIndex((HHitMatchSim*)tracks[i]);
	    //cout<<trkind<<" , ";
	    // check only against an index which did not occur before 
	    
	    if(
	       (trkind==ind1 || trkind==ind2) &&
	       HRichCut::isNewIndex(trkind,trkinds,tracks.GetLast()+1)
	       ) 
	    {
		//cout<<"found: "<<trkind<<" "<<ind1<<" "<<ind2<<endl;
		nbfoundtracksforpair++;
	    
	    
		if (nbfoundtracksforpair==2) 
		{
		    //cout<<"pair found"<<endl;
		    nbFoundPairs++;
		    pairs.Add(h);
		    break;
		}
	    }
	}
	//cout<<"pair loop"<<endl;
	//cout<<endl;
	delete [] trkinds;

    }
    return nbFoundPairs;
}
Int_t HGeantKineAna::getTracksForRings(TObjArray& rings,TObjArray& tracks)
{
    Int_t n=0;
    for (Int_t i=0;i<(rings.GetLast()+1);i++)
    {
	HHitMatchSim *track =0;
	pIterMatchHit->Reset();
	Int_t richind = getRichHitCat()->getIndex((HRichHitSim*)rings[i]);

	while((track=(HHitMatchSim *)pIterMatchHit->Next())!=0)
	{
	    if (
		HRichCut::isGoodRing((HRichHitSim*)rings[i]) &&
		HRichCut::isGoodTrack(track) &&
		track->getRichInd()==richind
		) 
	    {
		n++;
		tracks.Add(track);
	    }
	    
	}
    }
    return n;
}
Int_t HGeantKineAna::fillDalitzPairs(TObjArray& dalitzleps,TObjArray& dalitzpairs)
{
    Int_t n=0;
    for (Int_t i=0;i<(dalitzleps.GetLast()+1);i++)
    {
	for (Int_t j=i+1;j<(dalitzleps.GetLast()+1);j++) 
	{
	    Int_t aPar1, aMed1, aMech1;
	    Int_t aPar2, aMed2, aMech2;
	    Int_t aTrack1, aTrack2;
	    Int_t aID1, aID2;
	    
	    ((HGeantKine*)dalitzleps[i])->getParticle(aTrack1, aID1);
	    ((HGeantKine*)dalitzleps[j])->getParticle(aTrack2, aID2);
	    
	    ((HGeantKine*)dalitzleps[i])->getCreator(aPar1, aMed1, aMech1);
	    ((HGeantKine*)dalitzleps[j])->getCreator(aPar2, aMed2, aMech2);
	    
	    if (aPar1==aPar2 && aMed1==aMed2 && aMech1==aMech2)
	    {
		// pair found
		dalitzpairs.Add((HGeantKine*)dalitzleps[i]);
		dalitzpairs.Add((HGeantKine*)dalitzleps[j]);
		n++; //count pair
	    }
	}
    }
    return n;
}
Int_t HGeantKineAna::checkTrackPairs(TObjArray &t, TObjArray &p)
{
     
    HDiHitMatch *h=0;
    pIterDiHitMatch->Reset();
    Int_t nbFoundPairs=0;
    while(( h= (HDiHitMatch *)pIterDiHitMatch->Next())) 
    {
	Int_t ind1=h->getIndTrk1();
	Int_t ind2=h->getIndTrk2();
	Int_t foundtracks=0;
	Int_t *trkinds = new Int_t[t.GetLast()+1];
	for (Int_t j=0;j<(t.GetLast()+1);j++) trkinds[j]=-2;
	for (Int_t i=0;i<(t.GetLast()+1);i++)
	{
	    Int_t trkind=getHitMatchCat()->getIndex(t[i]);
	    if ((trkind==ind1 || trkind==ind2)&&
		HRichCut::isNewIndex(trkind,trkinds,t.GetLast()+1)) foundtracks++;

	}
	delete [] trkinds;
	if (foundtracks>=2) {nbFoundPairs++;p.Add(h);}
    }
    return nbFoundPairs;
}
Int_t HGeantKineAna::checkRingCorrelation(HRichHitSim* r,TObjArray& tracks)
{
    
    HHitMatchSim *track =0;
    pIterMatchHit->Reset();
    Int_t richind = getRichHitCat()->getIndex(r);
    Int_t n=0;
    while((track=(HHitMatchSim *)pIterMatchHit->Next())!=0)
    {
	if (
	    HRichUtilFunc::isGoodRing(r) &&
	    HRichUtilFunc::isGoodTrack(track) &&
	    track->getRichInd()==richind
	    ) 
	{
	    n++;
	    tracks.Add(track);
	}
	
    }
    return n;
}


Int_t HGeantKineAna::checkForPair(TObjArray& l,char *swt)
{
    if (l.GetLast()==0)
    {
	cout<<" **** pi0-Dalitz single lepton **** "<<endl;
	HRichUtilFunc::dumpKine((HGeantKine*)l[0]);
    }
    for (Int_t i=0;i<(l.GetLast()+1);i++)
    {
	for (Int_t j=i+1;j<(l.GetLast()+1);j++)
	{
	
	    Int_t aPar1, aMed1, aMech1;
	    Int_t aPar2, aMed2, aMech2;
	    Int_t aTrack1, aTrack2;
	    Int_t aID1, aID2;
	    
	    ((HGeantKine*)l[i])->getParticle(aTrack1, aID1);
	    ((HGeantKine*)l[j])->getParticle(aTrack2, aID2);

	    ((HGeantKine*)l[i])->getCreator(aPar1, aMed1, aMech1);
	    ((HGeantKine*)l[j])->getCreator(aPar2, aMed2, aMech2);

	    if (aPar1==aPar2 && aMed1==aMed2 && aMech1==aMech2)
	    {
		Float_t opang = HRichUtilFunc::calcOpeningAngleKine((HGeantKine*)l[i],(HGeantKine*)l[j]);
		Float_t invMass=HRichUtilFunc::calcInvMassKine((HGeantKine*)l[i],(HGeantKine*)l[j]);
		HGeantKine *gamma = 0;
		HGeantKine *pi0 = 0;
		HGeantKine *gamma1 =0;
		TString mode(swt);
		if (!mode.CompareTo("dalitz"))
		{
		// dalitz case
		    gamma = HRichUtilFunc::
			getPionDalitzGamma((HGeantKine*)l[i],
					   (HLinearCategory*)getGeantKineCat());
		    
		    if (gamma) pi0 =  HRichUtilFunc::
			findParent(gamma,(HLinearCategory*)getGeantKineCat());
		}
		else if (!mode.CompareTo("conv"))
		{
		// conversion case
		    gamma1 =  HRichUtilFunc::
			findParent((HGeantKine*)l[i],
				   (HLinearCategory*)getGeantKineCat());
		    if (gamma1) gamma = HRichUtilFunc::
			getSecondPionDecayGamma(gamma1,
						(HLinearCategory*)getGeantKineCat());
		     if (gamma1) pi0 =  HRichUtilFunc::
			findParent(gamma1,(HLinearCategory*)getGeantKineCat());
		}

		Bool_t validDecay = kFALSE;
		if (!mode.CompareTo("conv") && gamma1) validDecay=kTRUE;
		else if (!mode.CompareTo("dalitz")) validDecay = kTRUE;


		if (gamma&&pi0&&validDecay){
		
		// pi0 momentum
		Float_t ppi0X,ppi0Y,ppi0Z;
		pi0->getMomentum(ppi0X,ppi0Y,ppi0Z);


		// vector sum of leptons
		Float_t plep1X,plep1Y,plep1Z;
		Float_t plep2X,plep2Y,plep2Z;
		((HGeantKine*)l[i])->getMomentum(plep1X,plep1Y,plep1Z);
		((HGeantKine*)l[j])->getMomentum(plep2X,plep2Y,plep2Z);
		Float_t plepX = plep1X+plep2X;
		Float_t plepY = plep1Y+plep2Y;
		Float_t plepZ = plep1Z+plep2Z;
		Float_t pleptotal = TMath::Sqrt( plepX*plepX+
						 plepY*plepY+
						 plepZ*plepZ );
		HGeomVector psumleptons;
		psumleptons.setX(plepX);
		psumleptons.setY(plepY);
		psumleptons.setZ(plepZ);
		Float_t totMom1 = ((HGeantKine*)l[i])->getTotalMomentum();
		Float_t totMom2 = ((HGeantKine*)l[j])->getTotalMomentum();
		hmompairkine->Fill(totMom1,totMom2);
		//		Float_t momgamma1 = gamma1->getTotalMomentum();


		Float_t theta1,phi1;
		Float_t theta2,phi2;
		HRichUtilFunc::calcParticleAnglesKine((HGeantKine*)l[i],theta1,phi1);
		HRichUtilFunc::calcParticleAnglesKine((HGeantKine*)l[j],theta2,phi2);
		hthetapairkine->Fill(theta1,theta2);
		// take care of gamma
		Float_t momgamma = gamma->getTotalMomentum();
		HGeomVector pgamma;
		Float_t pgammaX,pgammaY,pgammaZ;
		
		gamma->getMomentum(pgammaX,pgammaY,pgammaZ);
		pgamma.setX(pgammaX);
		pgamma.setY(pgammaY);
		pgamma.setZ(pgammaZ);
		
		// take care of second gamma
		//  if (!mode.CompareTo("conv"))
//  		{
//  		    Float_t momgamma1 = gamma1->getTotalMomentum();
//  		    Float_t theta_gamma,phi_gamma;
//  		    Float_t theta_gamma1,phi_gamma1;
		    
//  		    HRichUtilFunc::calcParticleAnglesKine(gamma,theta_gamma,phi_gamma);
//  		    HRichUtilFunc::calcParticleAnglesKine(gamma1,theta_gamma1,phi_gamma1);
//  		    hgammathetatheta->Fill(theta_gamma,theta_gamma1);
//  		    hgammamommom->Fill(momgamma,momgamma1);
//  		}

		// check momentum conservation
		Float_t psumX,psumY,psumZ;
		psumX = plep1X+plep2X+pgammaX-ppi0X;
		psumY = plep1Y+plep2Y+pgammaY-ppi0Y;
		psumZ = plep1Z+plep2Z+pgammaZ-ppi0Z;
		
		// calc opening angle between gamma mom 3vector 
		// and sum of lepton mom 3vectors 
		Float_t opanglepsgamma = HRichUtilFunc::calcOpeningAngleVectors(pgamma,psumleptons);
		// calc inv mass of decayed pion0 (134.976 MeV, c.f.:data booklet)
		Float_t invMassPion = 
		    2.*
		    sin( (1./HConstant::rad2deg()) *opanglepsgamma/2. )*
		    sqrt(momgamma*pleptotal);


		// dump to screen
//  		if (!mode.CompareTo("dalitz"))
//  		    cout<<" **** pi0-Dalitz decay (";
//  		else if (!mode.CompareTo("conv"))
//  		    cout<<" **** pi0->2-gamma decay (";
//  		cout<<"opang:"<<opang
//  		    <<"; lep pair inv. mass:"<<invMass;
//  		//cout<<"; trk nb1:"<<aTrack1<<"; trk nb2:"<<aTrack2;
//  		cout<<")"<<endl;
//    		cout<<" **** mom_gamma:"<<momgamma
//    		    <<" pion mass:"<<invMassPion<<endl;

  		//cout<<" **** mom sum 3-vector p("<<psumX<<","<<psumY<<","<<psumZ<<")  "

//              HRichUtilFunc::dumpKine((HGeantKine*)l[i]);
//  		HRichUtilFunc::dumpKine((HGeantKine*)l[j]);
//  		HRichUtilFunc::dumpKine(gamma);
//  		HRichUtilFunc::dumpKine(pi0);
		hpi0invmass->Fill(invMassPion);
		hdalitzopang->Fill(opang);
		hdalitzinvmass->Fill(invMass);
		}
	    }
	}

    }
    return 1;
}

void HGeantKineAna::calcParticleAnglesV(HGeantKine *kine,Float_t &theta, Float_t &phi)
{
    // input kine object with momentum vector
    // output theta and phi of trajectory
    Float_t xMom,yMom,zMom;
    kine->getMomentum(xMom,yMom,zMom);
    HGeomVector2 vec;
    vec.setX(xMom);
    vec.setY(yMom);
    vec.setZ(zMom);
    
    vec/=vec.length();//norm

    Float_t rad;
    vec.sphereCoord(rad,theta,phi);
    //cout<<"theta: "<<theta<<" phi: "<<phi<<endl;
}

Float_t HGeantKineAna::calcOpeningAngleV(HGeantKine *kine1,HGeantKine *kine2)
{
    //input 2 kine objects
    //output opening angle of trajectories
    Float_t rad2deg = 180./TMath::Pi();
    HGeomVector vec1;
    if (kine1){
    Float_t xMom1,yMom1,zMom1;
    kine1->getMomentum(xMom1,yMom1,zMom1);
    vec1.setX(xMom1);
    vec1.setY(yMom1);
    vec1.setZ(zMom1); 
    
    vec1/=vec1.length();//norm
    }
    HGeomVector vec2;
    if (kine2){
    Float_t xMom2,yMom2,zMom2;
    kine2->getMomentum(xMom2,yMom2,zMom2);
    vec2.setX(xMom2);
    vec2.setY(yMom2);
    vec2.setZ(zMom2);
    
    vec2/=vec2.length();//norm
    }
    
    Float_t cosfOpeningAngle = vec1.scalarProduct(vec2);
    //cout<<cosfOpeningAngle<<endl;
    Float_t fOpeningAngle=0.;
    if (TMath::Abs(cosfOpeningAngle) <= 1) 
	fOpeningAngle=TMath::ACos(cosfOpeningAngle) * rad2deg;
    
    return fOpeningAngle;
}

void HGeantKineAna::calcParticleAnglesT(HGeantKine *kine,Float_t &fpt, Float_t &fpp)
{
    //input kine object
    //output theta and phi of trajectory

    //dumpKine(kine);
    Float_t rad2deg = 180./TMath::Pi();
    Float_t pX,pY,pZ;
    pX=pY=pZ=0;
    kine->getMomentum(pX,pY,pZ);
    Float_t pT = TMath::Sqrt(pX*pX+pY*pY);
    Float_t pTot = kine->getTotalMomentum();
    fpt = TMath::ASin(pT/pTot) * rad2deg; //theta (polar) of particle
    fpp = TMath::ASin(pY/pT) *rad2deg; //phi (azimuth) of particle
    if (pX<0.) fpp = 180.-fpp;
    else if (pY<0.) fpp+=360.;
    //cout<<" Geant theta: "<<fpt
    //<< "Geant phi: "<<fpp<<endl;

}

Float_t HGeantKineAna::calcOpeningAngleT(Float_t t1,Float_t p1,
				      Float_t t2,Float_t p2)
{
    //input theta and phi of two trajectories
    //returns opening angle 
    Float_t rad2deg = 180./TMath::Pi();
    HGeomVector vec1,vec2;

    vec1.setX(TMath::Sin(t1/rad2deg) * TMath::Cos(p1/rad2deg));
    vec1.setY(TMath::Sin(t1/rad2deg) * TMath::Sin(p1/rad2deg));
    vec1.setZ(TMath::Cos(t1/rad2deg));

    vec2.setX(TMath::Sin(t2/rad2deg) * TMath::Cos(p2/rad2deg));
    vec2.setY(TMath::Sin(t2/rad2deg) * TMath::Sin(p2/rad2deg));
    vec2.setZ(TMath::Cos(t2/rad2deg));

    
    Float_t cosfOpeningAngle = vec1.scalarProduct(vec2);
    cout<<cosfOpeningAngle<<endl;
    Float_t fOpeningAngle=0.;
    if (TMath::Abs(cosfOpeningAngle) <= 1) 
	fOpeningAngle=TMath::ACos(cosfOpeningAngle) * rad2deg;
    
    return fOpeningAngle;
}

void HGeantKineAna::dumpKine(HGeantKine *kine)
{
    if (kine)
    {
    Int_t aTrack, aID;
    Int_t aPar, aMed, aMech;
    Float_t ax, ay, az;
    Float_t apx, apy, apz;
    Float_t aInfo, aWeight;
    Float_t ptot;
    kine->getParticle(aTrack,aID);
    kine->getCreator(aPar,aMed,aMech);
    kine->getVertex(ax,ay,az);
    kine->getMomentum(apx,apy,apz);
    kine->getGenerator(aInfo,aWeight);
    ptot=kine->getTotalMomentum();
    cout<<"-----------------------------------------------------------"<<endl;
    cout<<"--- GEANT track number: "<<aTrack<<endl;
    cout<<"--- track number of parent particle: "<<aPar<<endl;
    cout<<"--- GEANT particle ID: "<<aID<<endl;
    cout<<"--- GEANT medium of creation: "<<aMed<<endl;
    cout<<"--- GEANT creation mechanism: "<<aMech<<endl;
    cout<<"--- x of track vertex (in mm): "<<ax<<endl;
    cout<<"--- y                        : "<<ay<<endl;
    cout<<"--- z                        : "<<az<<endl;
    cout<<"--- x component of particle momentum (in MeV/c): "<<apx<<endl;
    cout<<"--- y                                          : "<<apy<<endl;
    cout<<"--- z                                          : "<<apz<<endl;
    cout<<"--- total momentum                             : "<<ptot<<endl;
    cout<<"--- event generator info: "<<aInfo<<endl;
    cout<<"--- associated weight: "<<aWeight<<endl;
    cout<<"--- "<<endl;
    cout<<"--- "<<endl;
    cout<<"--- "<<endl;
    cout<<"--- "<<endl;
    cout<<"-----------------------------------------------------------"<<endl;
    }
}
void HGeantKineAna::histoKine(HGeantKine *kine)
{
    if (kine)
    {
	Int_t aTrack, aID;
	Int_t aPar, aMed, aMech;
	Float_t ax, ay, az;
	Float_t apx, apy, apz;
	Float_t aInfo, aWeight;
	Float_t ptot;
	kine->getParticle(aTrack,aID);
	kine->getCreator(aPar,aMed,aMech);
	kine->getVertex(ax,ay,az);
	kine->getMomentum(apx,apy,apz);
	kine->getGenerator(aInfo,aWeight);
  	ptot=kine->getTotalMomentum();
	Float_t theta,phi;
	HRichUtilFunc::calcParticleAnglesKine(kine,theta,phi);
	hidkine->Fill(aID);
	hmedkine->Fill(aMed);
	hmechkine->Fill(aMech);
	hmomkine->Fill(ptot);
	hmomthetakine->Fill(theta,ptot);
	hparentkine->Fill(aPar);
	hvertexkine->Fill(ax,ay,az);
    }
}

HGeantKine* HGeantKineAna::findParent(HGeantKine *kine)
{
    Int_t aPar, aMed, aMech;
    kine->getCreator(aPar,aMed,aMech);
    if (aPar){
	HIterator* iter_kine2 = (HIterator*)(((HLinearCategory*)
					      getGeantKineCat())
					     ->MakeIterator("native"));
	iter_kine2->Reset();
	HGeantKine *kine2=0;
	Int_t aTrackParent,aIDParent;
	while((kine2=(HGeantKine *)iter_kine2->Next())!=0)
	{
	    kine2->getParticle(aTrackParent,aIDParent);;
	    if (aPar == aTrackParent) 
	    {
		//if (kDumpIt) dumpKine(findParent(kine2));//recursive research for relatives
		return kine2;
	    }
	}
    }
    return 0;
}

Bool_t HGeantKineAna::finalize() {
    //cout<<"in finalize"<<endl;
//      TFile ff("largedistevtlist.root","RECREATE");
//      ff.cd();
//      el->Print("all");
//      el->Write();
//      ff.Close();
    HRichUtilFunc::saveHistos(pFileOut,pHistArray);
    pFileOut->Close();

    cout<<"Sum rings:"<<nbRings<<" leptons:"<<nbLeptons<<" dal pairs:"<<nbDalitzPairs
	<<" tracks:"<<nbTracks<<" pairs:"<<nbTrackPairs
	<<" singles:"<<nbDalitzSingle<<endl;
    cout<<"Nb GEANT confirmed reconstructed pairs -- Sum:"<<nbgMatchedPairs<<endl;
    cout<<"Nb of reconstructed GEANT pairs -- Sum:"<<nbgMatchedGPairs<<endl;
    cout<<"Nb GEANT confirmed reconstructed tracks -- Sum:"<<nbgMatchedTracks<<endl;
    cout<<"Nb of reconstructed GEANT leptons -- Sum:"<<nbgMatchedGTracks<<endl;
    //    delete el;
    return kTRUE;
}

void HGeantKineAna::saveHistos()
{
    pFileOut->cd();
    // write histograms
    for (Int_t i=0;i<(pHistArray->GetLast()+1);i++)
    {
	( (TH1*)(*pHistArray)[i] )->Write();
    }
    //pHistElePosMomConv->Write();
    //pHistOpenAngle->Write();

}
