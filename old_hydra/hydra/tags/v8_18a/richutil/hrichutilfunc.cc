// $Id: hrichutilfunc.cc,v 1.19 2008-09-18 13:05:36 halo Exp $
// Last update by Thomas Eberl: 03/07/07 12:58:11
//
#include "hrichutilfunc.h"
#include "hrichcut.h"
#include "hruntimedb.h"
#include "hgeantkine.h"
#include "hgeantrich.h"
#include "hgeantmdc.h"
#include "hhitmatchsim.h"
#include "hrichhitsim.h"
#include "hdebug.h"
#include "hrichcuto.h"
#include "hades.h"
#include "hgeomvector2.h"
#include "hgeomvector.h"
#include "hconstant.h"
#include "hlinearcategory.h"
#include "hmatrixcategory.h"
#include "hmatrixcatiter.h"
#include "hiterator.h"
#include "hrichraw.h"
#include "hrichcal.h"
#include "richdef.h"
#include "hkicktrack.h"
#include "hparset.h"
#include "hcategory.h"
#include "htrackinfo.h"
#include "hgeantparticleinfo.h"
#include "TH1.h"
#include "TVector3.h"
//_HADES_CLASS_DESCRIPTION 
///////////////////////////////////////////////////////////
// HRichUtilFunc
//
// this class is a helper that contains functionality
// that can be shared by different reconstructors
///////////////////////////////////////////////////////////

ClassImp(HRichUtilFunc)

HRichUtilFunc::HRichUtilFunc()
{

}


HRichUtilFunc::~HRichUtilFunc(void) 
{

}

void HRichUtilFunc::calcParticleAnglesKine(HGeantKine *kine,Float_t &theta, Float_t &phi)
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
Float_t HRichUtilFunc::calcInvMassKine(HGeantKine *kine1,HGeantKine *kine2)
{

    Float_t opang = (1./HConstant::rad2deg())*
	calcOpeningAngleKine(kine1,kine2);
    Float_t p1 = kine1->getTotalMomentum();
    Float_t p2 = kine2->getTotalMomentum();
    return 2.*sin(opang/2.)*sqrt(p1*p2);
}
Float_t HRichUtilFunc::calcOpeningAngleKine(HGeantKine *kine1,HGeantKine *kine2)
{
    //input 2 kine objects
    //output opening angle of trajectories
    HConstant hconst;
    Double_t rad2deg = hconst.rad2deg();

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
Float_t HRichUtilFunc::calcOpeningAngleVectors(HGeomVector &v1,HGeomVector &v2)
{
    //input two vectors, output opening angle
    Double_t rad2deg = HConstant::rad2deg();

    v1/=v1.length();//norm
    v2/=v2.length();//norm

    Float_t cosfOpeningAngle = v1.scalarProduct(v2);
    Float_t fOpeningAngle=0.;
    if (TMath::Abs(cosfOpeningAngle) <= 1) 
	fOpeningAngle=TMath::ACos(cosfOpeningAngle) * rad2deg;
    
    return fOpeningAngle;
}

void HRichUtilFunc::calcParticleAnglesT(HGeantKine *kine,Float_t &fpt, Float_t &fpp)
{
    // works only correctly for 0 to 90 degrees polar angle

    //input kine object
    //output theta and phi of trajectory
    
    //dumpKine(kine);
    HConstant hconst;
    Double_t rad2deg = hconst.rad2deg();
    
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
Float_t HRichUtilFunc::openAngleDeg(Float_t th1, Float_t ph1, Float_t th2, Float_t ph2) 
{
// Returns angle between two directions in DEGREES (input angles in degrees)
  Float_t r2d = 57.29578;
  TVector3 v1(1,1,1);
  TVector3 v2(1,1,1);
  v1.SetMag(1.);
  v2.SetMag(1.);
  v1.SetTheta(th1/r2d);
  v1.SetPhi(ph1/r2d);
  v2.SetTheta(th2/r2d);
  v2.SetPhi(ph2/r2d);

  return v1.Angle(v2)*r2d;
}


Float_t HRichUtilFunc::calcOpeningAngleT(Float_t t1,Float_t p1,
				      Float_t t2,Float_t p2)
{
    //input theta and phi of two trajectories
    //returns opening angle 
//     Float_t eps=1.e-4;
     Float_t fOpeningAngle=-1.;
//     if (TMath::Abs(t1-t2)<eps && TMath::Abs(p1-p2)<eps)
//     {
// 	fOpeningAngle = 0.;
//     }
//     else
//     {

	Double_t rad2deg = TMath::RadToDeg();
	
	HGeomVector vec1,vec2;
	
	vec1.setX(TMath::Sin(t1/rad2deg) * TMath::Cos(p1/rad2deg));
	vec1.setY(TMath::Sin(t1/rad2deg) * TMath::Sin(p1/rad2deg));
	vec1.setZ(TMath::Cos(t1/rad2deg));

	vec2.setX(TMath::Sin(t2/rad2deg) * TMath::Cos(p2/rad2deg));
	vec2.setY(TMath::Sin(t2/rad2deg) * TMath::Sin(p2/rad2deg));
	vec2.setZ(TMath::Cos(t2/rad2deg));

    
	Float_t cosfOpeningAngle = vec1.scalarProduct(vec2);
	//	cout<<cosfOpeningAngle<<endl;
	
	if (TMath::Abs(cosfOpeningAngle) <= 1) 
	    fOpeningAngle=TMath::ACos(cosfOpeningAngle) * rad2deg;
	//    }
    return fOpeningAngle;
}

void HRichUtilFunc::dumpKickTrack(HKickTrack *track)
{
    Float_t r2d = 57.29578;
    Float_t tout = track->getTheta()*r2d;
    Float_t pout = track->getPhi()*r2d + (track->getSector()*60.);
    if(pout > 360.) pout-= 360.;

    cout<<"*** HKickTrack object ***"<<endl;
    TString tof="tof";
    TString sho="shower";
    TString sys;
    if (track->getSystem()==0) sys=sho;
    else sys=tof;
    cout<<"###################################"<<endl;
    cout<<"sec:"<<track->getSector()<<"  ";
    cout<<"theta:"<<tout<<" err:"<<track->getErrTheta()<<"  ";
    cout<<"phi:"<<pout<<" err:"<<track->getErrPhi()<<endl;
    cout<<"%%%"<<endl;
    cout<<"PID:"<<(Int_t)track->getPID()<<" ";
    cout<<"m:"<<TMath::Sqrt(track->getMass())<<" err^2:"<<track->getErrMass()<<endl;
    cout<<"%%%"<<endl;

    cout<<"quality:"<<(Int_t)track->getQuality()<<" sys:"<<sys.Data()<<endl;
    cout<<"pull:"<<track->getPull()<<endl;
    cout<<"%%%"<<endl;

    cout<<"P(tof):"<<track->getPTof()<<" err:"<<track->getErrPTof()<<endl;
    cout<<"P(bend):"<<track->getP()<<" err:"<<track->getErrP()<<endl;
    cout<<"%%%"<<endl;

    cout<<"Z:"<<track->getZ()<<" err:"<<track->getErrZ()<<endl;
    cout<<"R:"<<track->getR()<<" err:"<<track->getErrR()<<endl;
    cout<<"%%%"<<endl;

    cout<<"charge:"<<track->getCharge()<<endl;
    cout<<"tof:"<<track->getTof()<<endl;
    cout<<"beta:"<<track->getBeta()<<endl;;
    cout<<"%%%"<<endl;

    cout<<"seg id:"<<track->getSegment()<<endl;
    cout<<"ring id:"<<track->getRingId()<<endl;
    cout<<"out hit id:"<<track->getOuterHitId()<<endl;
    cout<<"***********************************"<<endl;
}

void HRichUtilFunc::dumpKine(HGeantKine *kine)
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
    cout<<"----------------------GEANT KINE---------------------------"<<endl;
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
    Float_t theta,phi;
    calcParticleAnglesKine(kine,theta,phi);
    cout<<"--- theta,phi                                  : "<<theta
	<<" "<<phi<<endl;
    cout<<"--- event generator info: "<<aInfo<<endl;
    cout<<"--- associated weight: "<<aWeight<<endl;
    cout<<"--- "<<endl;
    cout<<"--- "<<endl;
    cout<<"--- "<<endl;
    cout<<"--- "<<endl;
    cout<<"-----------------------------------------------------------"<<endl;
    }
}

HGeantKine* HRichUtilFunc::findParent(HGeantKine *kine,HLinearCategory* cat)
{
    Int_t aPar, aMed, aMech;
    kine->getCreator(aPar,aMed,aMech);
    if (aPar){
	HIterator* iter_kine2 = (HIterator*)(cat->MakeIterator("native"));
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

Int_t HRichUtilFunc::isLepOnMirror(HGeantKine* kine,HMatrixCategory* cat)
{
    // this function returns 1 in case that 
    // the kine obj is a lepton that passed the mirror else 0
    Int_t aKineTrack, aKineID;
    kine->getParticle(aKineTrack,aKineID);
    HGeantRichMirror *mirr=0;
    HIterator *iter_mirror = (HIterator *)(cat->MakeIterator("native"));
    iter_mirror->Reset();
    while((mirr=(HGeantRichMirror *)iter_mirror->Next())!=0)
    {
	Int_t aTrk,aID;
	mirr->getTrack(aTrk,aID);
	if (aTrk==aKineTrack && aID==aKineID) return 1;

    } 
    return 0;
}

Int_t HRichUtilFunc::isLepOnMDC(HGeantKine* kine,HMatrixCategory* cat)
{
    Int_t aKineTrack, aKineID;
    kine->getParticle(aKineTrack,aKineID);
    //    cout<<"--------------kine track: "<<aKineTrack<<endl;
    HGeantMdc *gmdc = 0;
    HIterator *iter_mdcgeant = (HIterator *)(cat->MakeIterator("native"));
    iter_mdcgeant->Reset();
    while((gmdc=(HGeantMdc *)iter_mdcgeant->Next())!=0)
    {
//  	cout<<gmdc->getTrack()<<"  "<<(Int_t)gmdc->getSector()<<" ";
//  	cout<<(Int_t)gmdc->getModule()<<" "<<(Int_t)gmdc->getLayer()<<endl;

//  	cout<<"MDC track:"<<gmdc->getTrack()<<endl;

  	    Int_t mod = (Int_t) gmdc->getModule();
  	    Int_t lay = (Int_t) gmdc->getLayer();
  	    if (gmdc->getTrack()==aKineTrack && mod==0 && lay==0) 
	    {
//  		cout<<gmdc->getTrack()<<"  "<<(Int_t)gmdc->getSector()<<" ";
//  		cout<<(Int_t)gmdc->getModule()<<" "<<(Int_t)gmdc->getLayer()<<endl;
		return 1;
	    }
    }
    return 0;
}

Int_t HRichUtilFunc::isPi0DalitzLep(Int_t trk,HLinearCategory* cat,Char_t *style)
{
    // returns 1 if track nb corresponds to a lepton from pi0 Dalitz decay
    // else returns zero

    HGeantKine * kine =0;
    // loop over kine container
    HIterator* iter_kine = (HIterator*)(cat->MakeIterator("native"));
    iter_kine->Reset();
    while((kine=(HGeantKine *)iter_kine->Next())!=0)
    {
	Int_t aTrack, aID;
	kine->getParticle(aTrack,aID);
	if (aTrack==trk) return isPi0DalitzLep(kine,cat,style);
    } // end while kine loop
    
    return 0;    
}
Int_t HRichUtilFunc::isPi0DalitzLep(HGeantKine* kine,HLinearCategory* cat,Char_t *style)
{
    // the cuts that need to be applied here depend on the used generator
    // of the pi0 Dalitz leptons.
    // in case Pluto++ was used to directly decay pi0 in 2 leps and a gamma
    // the parent info concerning the pi0 is lost. USE 0 as par part id instead !
    // in case that UrQMD has generated the pions, GEANT has decayed them
    // and the parent particle id of the leps can be used to check the origin
    // returns 1 if track nb corresponds to a lepton from pi0 Dalitz decay
    // else returns zero

    TString opt(style);
    const Int_t aMechPluto=0;
    const Int_t aParPluto=0;
    const Int_t aMechUrQMD=5;
    const Int_t aParUrQMD=7;
// # warning "isPi0DalitzLep(): aMechCut, aParCut might be used uninitialized! fix me."
    Int_t aMechCut = -1;
    Int_t aParCut = -1;

    if (opt.Contains("pluto"))
    {
	aMechCut = aMechPluto;
	aParCut  = aParPluto;
    }
    else if (opt.Contains("urqmd"))
    {
	aMechCut = aMechUrQMD;
	aParCut  = aParUrQMD;
    }

	Int_t aTrack, aID;
	Int_t aPar, aMed, aMech;
	Float_t ptot;
	kine->getParticle(aTrack,aID);
	kine->getCreator(aPar,aMed,aMech);
	ptot=kine->getTotalMomentum();
	if ( aMech==aMechCut && (aID == 2 || aID==3) && (aMed>=8 && aMed<=19)) // direct particle decay
	{
	    if (opt.Contains("pluto") && aPar==aParCut) return 1;
	    else return 0;
	    // only in case that generator was UrQMD
	    HGeantKine *kine_parent1 = findParent(kine,cat);
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
		
		
	    }//kine_parent1
	}//Mec==5 and lepton
	
    return 0;    
}

Int_t HRichUtilFunc::isPi0ConvLep(Int_t trk ,HLinearCategory* cat)
{
    // returns 1 if track nb corresponds to a lepton from gamma conv
    // else returns zero

    HGeantKine * kine =0;
    // loop over kine container
    HIterator* iter_kine = (HIterator*)(cat->MakeIterator("native"));
    iter_kine->Reset();
    while((kine=(HGeantKine *)iter_kine->Next())!=0)
    {
	Int_t aTrack, aID;
	kine->getParticle(aTrack,aID);
	if (aTrack==trk) return isPi0ConvLep(kine,cat);
    } // end while kine loop
    
    return 0;
}
Int_t HRichUtilFunc::isPi0ConvLep(HGeantKine* kine ,HLinearCategory* cat)
{
    // returns 1 if kine obj corresponds to a lepton from gamma conv
    // gammas came from a pi0
    // else returns zero

	Int_t aTrack, aID;
	Int_t aPar, aMed, aMech;
	Float_t ptot;
	kine->getParticle(aTrack,aID);
	kine->getCreator(aPar,aMed,aMech);
	ptot=kine->getTotalMomentum();

	if ( aMech==6 && (aID == 2 || aID==3) && (aMed>=8 && aMed<=19)) // photon pair production in target or Rich
	{
	    HGeantKine *kine_parent1 = findParent(kine,cat);
	    if(kine_parent1){
		
		Int_t aTrackp1, aIDp1;
		Int_t aParp1,aMedp1,aMechp1;
		kine_parent1->getParticle(aTrackp1,aIDp1);
		kine_parent1->getCreator(aParp1,aMedp1,aMechp1);
		if(aIDp1==1 && aMechp1==5)//gamma
		{
		    
		    HGeantKine *kine_parent2 = findParent(kine_parent1, cat);
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
			
			
		    }//kine_parent2
		}//gamma
	    }//kine_parent1
	}//Mec==6 and lepton
	
	return 0;
}
Bool_t HRichUtilFunc::isGamma(HGeantKine *kine)
{
    Bool_t ret_val=kFALSE;

    Int_t aTrack, aID;
    kine->getParticle(aTrack,aID);
    if (aID==1) ret_val=kTRUE;

    return ret_val;

}
Bool_t HRichUtilFunc::isGammaFromPi0(HGeantKine *kine,HLinearCategory* cat)
{
    Bool_t ret_val=kFALSE;
    Int_t aPar, aMed, aMech;
    kine->getCreator(aPar,aMed,aMech);
    Int_t aTrack, aID;
    kine->getParticle(aTrack,aID);
    if (getParentParID(aTrack,cat)==7 && aMech==5 && aID==1) ret_val=kTRUE;

    return ret_val;
}

HGeantKine* HRichUtilFunc::getSecondPionDecayGamma(HGeantKine *kine,HLinearCategory* cat)
{
    // input first gamma 
    Int_t aPar, aMed, aMech;
    kine->getCreator(aPar,aMed,aMech);
    Int_t aTrack, aID;
    kine->getParticle(aTrack,aID);

    if (getParentParID(aTrack,cat)==7 && aMech==5 && aID==1)
    {
	//dumpKine(kine);
	HIterator* iter_kine2 = (HIterator*)(cat->MakeIterator("native"));
	iter_kine2->Reset();
	HGeantKine *kine2=0;
	Int_t aTrackSister,aIDSister;
	Int_t aParSister,aMedSister,aMechSister;
	while((kine2=(HGeantKine *)iter_kine2->Next())!=0)
	{
	    kine2->getParticle(aTrackSister,aIDSister);
	    kine2->getCreator(aParSister,aMedSister,aMechSister);
	    if (aTrack!=aTrackSister)
	    {
		if (getParentParID(aTrackSister,cat)==7 && aMechSister==5)
		{
		    //dumpKine(kine2);
		    if (aPar == aParSister && aIDSister==1) return kine2;
		} 
	    }
	}
    }
    return 0;
}

HGeantKine* HRichUtilFunc::getSecondPionDalitzLepton(HGeantKine *kine,HLinearCategory* cat)
{
    Int_t aPar, aMed, aMech;
    kine->getCreator(aPar,aMed,aMech);
    Int_t aTrack, aID;
    kine->getParticle(aTrack,aID);
    if (getParentParID(aTrack,cat)==7 && aMech==5 && (aID==2 || aID==3))
    {
	HIterator* iter_kine2 = (HIterator*)(cat->MakeIterator("native"));
	iter_kine2->Reset();
	HGeantKine *kine2=0;
	Int_t aTrackSister,aIDSister;
	Int_t aParSister,aMedSister,aMechSister;
	while((kine2=(HGeantKine *)iter_kine2->Next())!=0)
	{
	    kine2->getParticle(aTrackSister,aIDSister);
	    kine2->getCreator(aParSister,aMedSister,aMechSister);
	    if (aTrack!=aTrackSister)
	    {
		if (getParentParID(aTrackSister,cat)==7 && aMechSister==5)
		{
		    if (aPar == aParSister && (aIDSister==2 || aIDSister==3) ) return kine2;
		} 
	    }
	}
    }
    return 0;
}

HGeantKine* HRichUtilFunc::getPionDalitzGamma(HGeantKine *kine,HLinearCategory* cat)
{
    // kine is a lepton from a pi0-Dalitz decay
    // now find gamma which was 3rd particle in decay
    Int_t aParLepton, aMedLepton, aMechLepton;
    kine->getCreator(aParLepton,aMedLepton,aMechLepton);
    Int_t aTrackLepton, aIDLepton;
    kine->getParticle(aTrackLepton,aIDLepton);
    
    // verify Dalitz decay origin of provided lepton  
    if (
	getParentParID(aTrackLepton,cat)==7 && 
	aMechLepton==5 && 
	(aIDLepton==2 || aIDLepton==3)
	)
    {
	// iterate over GEANT particles and search corresponding gamma
	// we know the track number of the parent particle aParLepton
	HIterator* iter_kine = (HIterator*)(cat->MakeIterator("native"));
	iter_kine->Reset();
	HGeantKine *gamma=0;
	Int_t aTrackGamma,aIDGamma;
	Int_t aParGamma,aMedGamma,aMechGamma;
	while((gamma=(HGeantKine *)iter_kine->Next())!=0)
	{
	    gamma->getParticle(aTrackGamma,aIDGamma);
	    gamma->getCreator(aParGamma,aMedGamma,aMechGamma);
	    if (
		aParLepton==aParGamma &&
		getParentParID(aTrackGamma,cat)==7 &&
		aMechGamma==5 && 
		aIDGamma==1
		) return gamma;
	}
    }
    return 0;


}
HGeantKine* HRichUtilFunc::getKineObj(Int_t nTrkNb,HLinearCategory* cat)
{
    HIterator* iter_kine = (HIterator*)(cat->MakeIterator("native"));
    iter_kine->Reset();
    HGeantKine *kine=0;
    Int_t aTrack,aID;
    while((kine=(HGeantKine *)iter_kine->Next())!=0)
    {
	kine->getParticle(aTrack,aID);
	if (aTrack==nTrkNb) return kine;
    }
    return 0;
}

Int_t HRichUtilFunc::getParID(Int_t nTrkNb,HLinearCategory* cat)
{
    Int_t dummy,aID;
    aID=0;
    HGeantKine *kine = getKineObj(nTrkNb,cat);
    if (kine)
    {
	kine->getParticle(dummy,aID);
    }
    return aID;
}

Int_t HRichUtilFunc::getParentParID(Int_t nTrkNb,HLinearCategory* cat)
{
    HGeantKine *kine = getKineObj(nTrkNb,cat);
    Int_t dummy,aID;
    aID=0;
    if (kine)
    {
	HGeantKine *parent = findParent(kine,cat);
	if (parent) parent->getParticle(dummy,aID);
    }
    return aID;

}
Int_t HRichUtilFunc::getParentParID(HGeantKine* kine,HLinearCategory* cat)
{
    Int_t dummy,aID;
    aID=0;
    HGeantKine *parent = findParent(kine,cat);
    if (parent) parent->getParticle(dummy,aID);
    return aID;

}

void HRichUtilFunc::saveHistos(TFile* pFileOut, TObjArray* pHistArray)
{
    // use this function to save histos every n events 
    // assumes that you have opened a file and that your
    // histograms are stored in a TObjArray
    if(!pFileOut->IsOpen())
    {
	TString filename(pFileOut->GetName());
	pFileOut=TFile::Open(filename.Data(),"UPDATE");
	cout<<"Warning: File "<<pFileOut->GetName()<<" was reopened."<<endl;
	pFileOut->Print();
    }
    pFileOut->cd();
    // write histograms
    for (Int_t i=0;i<(pHistArray->GetLast()+1);i++)
    {
	( (TH1*)(*pHistArray)[i] )->Write();
    }
}
void HRichUtilFunc::saveCuts(TFile* pFileOut, TObjArray* pArray)
{
    if(!pFileOut->IsOpen())
    {
	TString filename(pFileOut->GetName());
	pFileOut=TFile::Open(filename.Data(),"UPDATE");
	cout<<"Warning: File "<<pFileOut->GetName()<<" was reopened."<<endl;
	pFileOut->Print();
    }
    pFileOut->cd();

    for (Int_t i=0;i<(pArray->GetLast()+1);i++)
    {
	( (HRichCutO*)(*pArray)[i] )->Write();
    }
}

TCanvas* HRichUtilFunc::makeMicroCanvas(TObjArray* pHistArray)
{
    TH1 *h1;
    Int_t cntr = pHistArray->GetLast()+1;
    TCanvas * c1 = new TCanvas("microCanvas","microCanvas",10,10,1200,1000); 
    Float_t help = cntr;
    Int_t plusone=0;
    if (TMath::Sqrt(help)-TMath::Nint(TMath::Sqrt(help)) >0) plusone=1;
    Int_t div = TMath::Nint(TMath::Sqrt(help));
    c1->Divide(div,div+plusone);
    for (Int_t i=0;i<cntr;i++)
    {

	h1 = (TH1*)(*pHistArray)[i];
	c1->cd(i+1);
	if ( 
	    !strcmp((h1->IsA())->GetName(),"TH2F") || 
	    !strcmp((h1->IsA())->GetName(),"TH2D") ||
	    !strcmp((h1->IsA())->GetName(),"TH2S")   )
	{
	    h1->DrawCopy("colz");
	}else{
	    h1->DrawCopy();
	}
    }
    c1->Modified();
    c1->Update();
    return c1;
}
void HRichUtilFunc::dumpCategory(Cat_t iCategory)
{
  HCategory* fCategory = gHades->getCurrentEvent()->getCategory(iCategory);
  HIterator* fIter;
  HLocation loc;
  switch (iCategory) {
  case catRichRaw: 
    {
      cout << "Dumping rich raw category to screen:" << endl;
      fIter = (HMatrixCatIter*)fCategory->MakeIterator();
      fIter->Reset(); 
      while(fIter->Next())
	{      
	  loc = fIter->getLocation();
	  HRichRaw* raw = (HRichRaw*)fCategory->getObject(loc); 
	  cout << "***********************************" << endl;
	  cout << "Sector: \t" << raw->getSector()<< endl;
	  cout << "Row: \t\t" << raw->getRow()<< endl;
	  cout << "Col: \t\t" << raw->getCol()<< endl;
	  cout << "EventNr: \t" << raw->getEventNr()<< endl;
	  cout << "Charge: \t" << raw->getCharge() << endl;
	  cout << "***********************************" << endl;
	  cout << endl;
	}
      break;
    }
  case catRichCal: 
    {
      cout << "Dumping rich cal category to screen:" << endl;
      fIter = (HMatrixCatIter*)fCategory->MakeIterator();
      fIter->Reset();
      while(fIter->Next())
	{
	  loc = fIter->getLocation();
	  HRichCal* cal  = (HRichCal*)fCategory->getObject(loc);
	  cout << "***********************************" << endl;
	  cout << "Sector: \t" << cal->getSector()<< endl;
	  cout << "Row: \t\t" << cal->getRow() << endl;
	  cout << "Col: \t\t" << cal->getCol() << endl;
	  cout << "EventNr: \t" << cal->getEventNr() << endl;
	  cout << "Charge: \t" << cal->getCharge() << endl;
	  cout << "***********************************" << endl;
	  cout << endl;
	}
      break;
    }
  default: cout << "Unknown category!" << endl;
    
  }
}
void HRichUtilFunc::rotateTrackToLab(Int_t s,Float_t tin,Float_t pin,Float_t &tout,Float_t &pout)
{
    // this should be done using the HSpecGeomPar
    // this should be done when alignment for real data
    // is used !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

// HSpecGeomPar version
//      HGeomVector2 alpha;
//      HGeomVector2 alphaLocal;
//      HGeomTransform &transSec = ((HSpecGeomPar*)getSpecGeomPar())
//  	->getSector( s )
//  	->getTransform();
    
//      alphaLocal.setX(sin(tin)*cos(pin));
//      alphaLocal.setY(sin(tin)*sin(pin));
//      alphaLocal.setZ(cos(tin));
    
//      alpha = transSec.getRotMatrix() * alphaLocal;
//      tout = (Float_t) alpha.getTheta();
//      pout = (Float_t) alpha.getPhi();



    // simple minded version
    Float_t r2d = 180./TMath::Pi();//57.29578;
    tout = tin*r2d;
    pout = pin*r2d + (s*60.);
    if(pout > 360.) pout-= 360.;
		

}
Float_t HRichUtilFunc::getDeflectionAngle(HHitMatch* hi)
{
    Float_t mt=hi->getMdcTheta();
    Float_t mm=-1.;

    // whether track was used before is not checked here !
    // check whether META was TOF or SHOWER
    if (hi->getTofTheta()>0.) mm=hi->getTofTheta();
    else if (hi->getShowerTheta()>0.) mm=hi->getShowerTheta();
    if (mm<0.||mt<0.) cout<<"HRichUtilFunc::getDeflectionAngle:  meta or mdc polar angle not initialized !"<<endl;
    return mt-mm;
}
HCategory* HRichUtilFunc::getCategory(Cat_t cat, Bool_t bWarning)
{
// Find HCategory if gHades->getCurrentEvent() is accessible

HEvent    *pEvent;
HCategory *pCat;

    if((gHades == NULL) || ((pEvent = gHades->getCurrentEvent()) == NULL))
    {
        if(bWarning)
        {
            ::Error("HRichUtilFunc::getCategory",
                            "Cannot access current event");
        }

        return NULL;
    }

    if((pCat = pEvent->getCategory(cat)) == NULL)
    {
        if(bWarning)
            ::Error("HRichUtilFunc::getCategory", "No category: %d", cat);
    }

    return pCat;
}
HParSet* HRichUtilFunc::getParamContainer(Char_t * contname)
{
	return (HParSet*)((gHades->getRuntimeDb())->getContainer(contname));
}

// Int_t HRichUtilFunc::getTrackGeantCorrCode(HTrackInfo* t,Int_t index)
// {
//     // substituted by htrackinfo member function
//     // return t->calcCorrCode(part);
//     Int_t nCorrCode=-100;
//     Int_t RM   = t->getMatchedRM(index);
//     Int_t RTS  = t->getMatchedRT_S(index);
//     Int_t MTS  = t->getMatchedMT_S(index);
//     Int_t RMTS = t->getMatchedRMT_S(index);
//     if (RM==-1  && RTS==-1 && MTS==-1 && RMTS==-1) nCorrCode=0;//complete fake, does not occur
//     if (RM==1  && RTS==-1 && MTS==-1 && RMTS==-1) nCorrCode=1;//fake kicktrack
//     if (RTS==1 && RM==-1  && MTS==-1 && RMTS==-1) nCorrCode=2;//fake kicktrack, fake RICH-MDC, good RICH-META
//     if (MTS==1 && RM==-1  && RTS==-1 && RMTS==-1) nCorrCode=3;//good kicktrack, RICH didn't see track
//     if (RMTS==1) nCorrCode=4;//ideal tracking, fully tracked particle
//     return nCorrCode;
// }

TObjArray* HRichUtilFunc::getGeantParticleInfo()
{
    TObjArray* objs = new TObjArray(5);
    HLinearCategory * cat = (HLinearCategory*) HRichUtilFunc::getCategory(catMatchHit);
    HIterator* pIterMatchHit = 0;
    if (cat) pIterMatchHit = (HIterator*)cat->MakeIterator("native");

    Int_t *geantparticles = new Int_t[MAXPARTICLES*cat->getEntries()];
    for (Int_t ps=0;ps<MAXPARTICLES*cat->getEntries();ps++) geantparticles[ps]=-2;

    HHitMatchSim *h=0;
    pIterMatchHit->Reset();
    while(( h = (HHitMatchSim *)pIterMatchHit->Next()))
    {
	//	cout<<"in hitmatchsim while loop"<<endl;
	HTrackInfo * t = h->getTrackInfoObj();

	for (Int_t part=0;part<MAXPARTICLES;part++)
	{
	    if (t->getTrkNb(part)==-1) break;
	    Bool_t isNewPart = HRichCut::isNewIndex(t->getTrkNb(part),geantparticles,MAXPARTICLES*cat->getEntries());
	    if (isNewPart)
	    {
		//cout<<"is new particle "<<t->getTrkNb(part)<<endl;
		HGeantParticleInfo *info = new HGeantParticleInfo();
		info->setGeantTrackNr(t->getTrkNb(part));
		info->setReconTrk(0,cat->getIndex(h));
		info->setCorrCode(0,t->calcCorrCode(part));
		objs->Add(info);
	    }
	    else 
	    {
		//cout<<"nb of objects :"<<objs->GetLast()+1<<endl;
		for (Int_t ii=0;ii<objs->GetLast()+1;ii++)
		{
		    HGeantParticleInfo* infoi = (HGeantParticleInfo*)((*objs)[ii]);
		    if (infoi)
		    {
			if( infoi->getGeantTrackNr() == t->getTrkNb(part) )
			{
			    for (Int_t jj=0;jj<TRACKS;jj++)
			    {
				if (infoi->getReconTrk(jj)==cat->getIndex(h)) break;//index already stored
				if (infoi->getReconTrk(jj)==-1)
				{
				    infoi->setReconTrk(jj,cat->getIndex(h)); 
				    infoi->setCorrCode(jj,t->calcCorrCode(part)); 
				    break;
				}
			    }
			}
		    }
		    //else Error("getGeantParticleInfo","no geant particle info obj");
		    else cout<<"Error: hgeantparticleinfo, no such object in array found"<<endl;
		}//endfor info objects already created
	    }//endif new particle


	}//endfor particles
    }//end while


    delete [] geantparticles;
    return objs;
}
TObjArray* HRichUtilFunc::getGeantParticleInfo(TObjArray* arr)
{
    TObjArray* objs = new TObjArray(5);
    Int_t *geantparticles = new Int_t[(MAXPARTICLES*(arr->GetLast()+1))];
    for (Int_t ps=0;ps<MAXPARTICLES*(arr->GetLast()+1);ps++) geantparticles[ps]=-2;

    Int_t max = arr->GetLast()+1;
    for (Int_t track=0;track<max;track++)
    {
	HHitMatchSim *h= ((HHitMatchSim*)(*arr)[track]);
	if(h)
	{
	    HTrackInfo * t = h->getTrackInfoObj();

	    for (Int_t part=0;part<MAXPARTICLES;part++)
	    {
		if (t->getTrkNb(part)==-1) break;
		Bool_t isNewPart = HRichCut::isNewIndex(t->getTrkNb(part),geantparticles,MAXPARTICLES*max);
		if (isNewPart)
		{
		    //cout<<"is new particle "<<t->getTrkNb(part)<<endl;
		    HGeantParticleInfo *info = new HGeantParticleInfo();
		    info->setGeantTrackNr(t->getTrkNb(part));
		    info->setReconTrk(0,arr->IndexOf(h));//should be track, but still ...
		    info->setCorrCode(0,t->calcCorrCode(part));
		    objs->Add(info);
		}
		else 
		{
		    //cout<<"nb of objects :"<<objs->GetLast()+1<<endl;
		    for (Int_t ii=0;ii<objs->GetLast()+1;ii++)
		    {
			HGeantParticleInfo* infoi = (HGeantParticleInfo*)((*objs)[ii]);
			if (infoi)
			{
			    if( infoi->getGeantTrackNr() == t->getTrkNb(part) )
			    {
				for (Int_t jj=0;jj<TRACKS;jj++)
				{
				    if (infoi->getReconTrk(jj)==arr->IndexOf(h)) break;//index already stored
				    if (infoi->getReconTrk(jj)==-1)
				    {
					infoi->setReconTrk(jj,arr->IndexOf(h)); 
					infoi->setCorrCode(jj,t->calcCorrCode(part)); 
					break;
				    }
				}
			    }
			}
			//else Error("getGeantParticleInfo","no geant particle info obj");
			else cout<<"Error: hgeantparticleinfo, no such object in array found"<<endl;
		    }//endfor info objects already created
		}//endif new particle
		
		
	    }//endfor particles
	}//endif h
    }//endfor recon tracks array
    
    
    delete [] geantparticles;
    return objs;
}
