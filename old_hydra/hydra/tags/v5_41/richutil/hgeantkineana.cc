// File: hgeantkineana.cc
//
// Author: Thomas Eberl <Thomas.Eberl@physik.tu-muenchen.de>
// Last update by Thomas Eberl: 01/08/29 22:00:27
//

#include "hgeantkineana.h"
#include "hgeantkine.h"
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
#include "hgeomvector2.h"
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
HGeantKineAna::HGeantKineAna(Text_t *name,Text_t *title, char* filename) :
  HReconstructor(name,title)
{
    pFileName  = filename;

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
	//	HSpectrometer *spec=gHades->getSetup();
	if (event && rtdb) {
	    
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
	  
	} else {
	    Error ("init","! (event && rtdb)");
	    return kFALSE; //! (event && rtdb)
	}
	
//  	iniCuts();
  	iniSwitches();
	iniControlHistos();
  	iniCounters();
	pFileOut = new TFile(pFileName,"RECREATE"); // histo file
	
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
    nCounterProcessedNbEvents=0;
}

void HGeantKineAna::iniControlHistos()
{
    pHistArray = new TObjArray(10);


    pHistElePosMomConv = new TH2D("e-_e+_convpair_mom","e-_e+_convpair_mom"
			    ,80,0,800,80,0,800);
    pHistArray->Add(pHistElePosMomConv);
    pHistOpenAngle = new TH1F("e+_e-_convpair_opening_angle",
			      "e+_e-_convpair_opening_angle",100,0,10);
    pHistArray->Add(pHistOpenAngle);
    pHistConvEleMom = new TH2D("e-_conv_mom_theta","e-_conv_mom_theta"
			       ,180,0,180,80,0,800);
    pHistArray->Add(pHistConvEleMom);
    pHistConvPosMom = new TH2D("e+_conv_mom_theta","e+_conv_mom_theta"
			       ,180,0,180,80,0,800);
    pHistArray->Add(pHistConvPosMom);
    pHistConvGammaMom = new TH2D("gamma_conv_mom_theta","gamma_conv_mom_theta"
			       ,180,0,180,80,0,800);
    pHistArray->Add(pHistConvGammaMom);
    pHistConvPairSource = new TH1F("conv_pair_source","conv_pair_source"
				   ,10,6,16);
    pHistArray->Add(pHistConvPairSource);
    pHistElePosMomThetaConv = new TH3D("e-_e+_convpair_mom_theta"
				       ,"e-_e+_convpair_mom_theta",
				       80,0,800,80,0,800,180,0,180);
    pHistArray->Add(pHistElePosMomThetaConv);
    pHistConvPairVertex = new TH3D("conv_pair_vertex","conv_pair_vertex",
				   100,-1000,1000,100,-1000,1000,100,-1000,1000);
    pHistArray->Add(pHistConvPairVertex);
}

Int_t HGeantKineAna::execute()
{
    //cout<<"in execute"<<endl;
    if (nCounterProcessedNbEvents != 0 &&
	nCounterProcessedNbEvents%50000==0) 
    {
	cout<<"********** "
	    <<nCounterProcessedNbEvents
	    <<" EVTS PROCESSED **********"<<endl;
	saveHistos();
    }
    TObjArray *pElectrons = new TObjArray(25);
    TObjArray *pPositrons = new TObjArray(25);
    HGeantKine * kine =0;
    

    // loop over kine container and select e- and e+
    iter_kine->Reset();
    while((kine=(HGeantKine *)iter_kine->Next())!=0)
    {
	Int_t aTrack, aID;
	Int_t aPar, aMed, aMech;
	//Float_t ax, ay, az;
	//Float_t apx, apy, apz;
	//Float_t aInfo, aWeight;
	//Float_t ptot;
	kine->getParticle(aTrack,aID);
	kine->getCreator(aPar,aMed,aMech);
	//kine->getVertex(ax,ay,az);
	//kine->getMomentum(apx,apy,apz);
	//kine->getGenerator(aInfo,aWeight);
	//ptot=kine->getTotalMomentum();
	// aID==2 is e+
	// aID==3 is e-
	// aMech==6 is photo pair production
	// aMed==8 is RICH radiator ?? aMed==14 is target ??
	// aMed==9 is RICH window ?? aMed==10 is RICH carb shell ??
	// aMed==13 is RICH glass mirror ??
	if ( aID == 2 && aMech == 6 && 
	     //  (aMed == 8 || aMed == 14 || aMed == 9 
	     // || aMed == 10 || aMed == 13)) pPositrons->Add(kine);
	     aMed == 9) pPositrons->Add(kine);
	if ( aID == 3 && aMech == 6 && 
	     //(aMed == 8 || aMed == 14 || aMed == 9
	     // || aMed == 10 || aMed == 13)) pElectrons->Add(kine);
	     aMed == 9) pElectrons->Add(kine);
	//	    if (kDumpIt) dumpKine(kine);
	//  if (kDumpIt) dumpKine(findParent(kine));

    } // end while kine loop


    // ===========================================================


    // loop over selected e- and e+ (kine objs) and fill histos 

    Int_t aParE,aParP,aMedE,aMedP,aMechE,aMechP,aMedG,aMechG,aParG;
    aParE=aParP=aMedE=aMedP=aMechE=aMechP=aMedG=aMechG=aParG=-1;
    Float_t theta,phi;
    theta=phi =-1.;
    for (Int_t i=0;i<(pPositrons->GetLast()+1);i++)
    {
	HGeantKine *positron = (HGeantKine*)(*pPositrons)[i];
	if (positron) 
	{
	    positron->getCreator(aParP,aMedP,aMechP);
	    for (Int_t j=0;j<(pElectrons->GetLast()+1);j++)
	    {
		HGeantKine *electron = (HGeantKine*)(*pElectrons)[j];
		if (electron)  
		{
		    electron->getCreator(aParE,aMedE,aMechE);
		
		    if (aParE==aParP && aMedE==aMedP && aMechE==aMechP)
		    { 
			//cout<<"!!! Conversion pair found !!!"<<endl;
			//dumpKine(electron);
			//dumpKine(positron);
			pHistConvPairSource->Fill(aMedP);
			pHistElePosMomConv->Fill(electron->getTotalMomentum(),
						 positron->getTotalMomentum());
			pHistOpenAngle->Fill(calcOpeningAngleV(electron,
							       positron));
			calcParticleAnglesV(electron,theta,phi);
			pHistElePosMomThetaConv->
			    Fill(electron->getTotalMomentum(),
				 positron->getTotalMomentum(),
				 theta);
			
		       
			
		    } // end pair check
		} // if electron
	    }//end for electrons
	}//if positron
    } //end for positrons

    aParE=aParP=aMedE=aMedP=aMechE=aMechP=aMedG=aMechG=aParG=-1;
    Int_t aTrack,aID;
    aTrack=aID=-1;
    Float_t vx,vy,vz;
    vx=vy=vz=-10000.;
    theta=phi=-1.;
    for (Int_t k=0;k<(pElectrons->GetLast()+1);k++)
    {
	HGeantKine *electron = (HGeantKine*)(*pElectrons)[k];
	if (electron)  
	{
	    electron->getCreator(aParE,aMedE,aMechE);
	    electron->getVertex(vx,vy,vz);
	    calcParticleAnglesV(electron,theta,phi);
	    pHistConvEleMom->Fill(theta,electron->getTotalMomentum());
	    pHistConvPairVertex->Fill(vx,vy,vz);
	    HGeantKine *gamma = findParent(electron);
	    if (gamma) 
	    {
		gamma->getParticle(aTrack,aID);
		if (aID == 1 && aMechE == 6 && aParE == aTrack) // gamma
		{
		   calcParticleAnglesV(gamma,theta,phi);
		   pHistConvGammaMom->Fill(theta,gamma->getTotalMomentum()); 
		}
	    }
	}
    }
    vx=vy=vz=-10000.;
    for (Int_t l=0;l<(pPositrons->GetLast()+1);l++)
    {
	HGeantKine *positron = (HGeantKine*)(*pPositrons)[l];
	if (positron)  
	{
	    positron->getCreator(aParP,aMedP,aMechP);
	    positron->getVertex(vx,vy,vz);
	    calcParticleAnglesV(positron,theta,phi);
	    pHistConvPosMom->Fill(theta,positron->getTotalMomentum());
	    pHistConvPairVertex->Fill(vx,vy,vz);
	    HGeantKine *gamma = findParent(positron);
	    if (gamma) 
	    {
		gamma->getParticle(aTrack,aID);
		if (aID == 1 && aMechP == 6 && aParP == aTrack) // gamma
		{
		   calcParticleAnglesV(gamma,theta,phi);
		   pHistConvGammaMom->Fill(theta,gamma->getTotalMomentum()); 
		}
	    }
	}
    }
    nCounterProcessedNbEvents++;
    
    delete pElectrons;
    delete pPositrons;
    return kSkipEvent; //skips writing out event to file


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

    pFileOut->cd();
    
    // create canvas
//      TCanvas *canvas1 = makeCanvas1();
//      canvas1->Write();
    // write histograms
    for (Int_t i=0;i<(pHistArray->GetLast()+1);i++)
    {
	( (TH1*)(*pHistArray)[i] )->Write();
    }
	//pHistElePosMomConv->Write();
	//pHistOpenAngle->Write();
    //close file 
    pFileOut->Close();

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
