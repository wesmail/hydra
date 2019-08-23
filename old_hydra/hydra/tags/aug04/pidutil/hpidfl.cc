// $Id: hpidfl.cc,v 1.6 2004-07-08 13:46:50 christ Exp $
// Last update by Thomas Eberl: 03/02/25 14:55:41
//
#include "hpidfl.h"
#include "hruntimedb.h"
#include "hades.h"
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
#include "hlinearcategory.h"
#include "hmatrixcategory.h"
#include "hmatrixcatiter.h"
#include "hiterator.h"
#include "hrichraw.h"
#include "hrichcal.h"
#include "richdef.h"
#include "hgeantdef.h"
#include "hkicktrack.h"
#include "hkicktracksim.h"
#include "hrichgeometrypar.h"
#include "hrichpad.h"
#include "hpidcuto.h"
#include "hpidparticle.h"
#include "hpidparticlehit.h"
#include "hpidparticlesim.h"
#include "hpidparticlesimext.h"
#include "hpidparticlesimexthit.h"
#include "hpidtrackcand.h"
#include "hpidhitdata.h"
#include <TH1.h>
#include <TH2F.h>
#include <TVector3.h>
#include <TFile.h>

///////////////////////////////////////////////////////////
// HPidFL
//
// library of static functions
// look here for auxiliary functions for:
// physics analysis, simulation analysis, ...
///////////////////////////////////////////////////////////

ClassImp(HPidFL)

HPidFL::HPidFL()
{

}


HPidFL::~HPidFL(void) 
{


}

Float_t HPidFL::getKineMomentum(HPidTrackCand* pTrackCand)
{

  Char_t ListOfTracks[1];
  Short_t IdsOfTracks[1];

  ListOfTracks[0]=-99;
  IdsOfTracks[0]=-99;


  HGeantKine* theKine;
  HLinearCategory* fGeantKineCat = (HLinearCategory*) getCategory(catGeantKine);
  static HIterator* pIteratorGeantKine = (HIterator *)fGeantKineCat ->MakeIterator("native");

  
  //This function checks, how many matches have been found in the common track set 
  pTrackCand->getGeantTrackSet()->fillCommonTracks(ListOfTracks,IdsOfTracks,1);
  pIteratorGeantKine->Reset();

  while((theKine=(HGeantKine *)pIteratorGeantKine->Next())!=0)
    {
      Int_t aTrack, aID;
      theKine->getParticle(aTrack,aID);
      if (aTrack==IdsOfTracks[0] && 
	  (ListOfTracks[0]==HPidCommonTrack::kTrackKickPlane ||
	   ListOfTracks[0]==HPidCommonTrack::kTrackFull)
	  )
	{
	  return theKine->getTotalMomentum();
	}
    } // end while kine loop
  return -99.0;
}

//This function returns the probability that a track segment contains more than one electron!
//It does not say anything about whether the track is purely electronic or not!
Float_t HPidFL::computeCloseTrackProbability(HPidHitData* pHitData, Int_t useChiSquare)
{
  Int_t wait;
  static Bool_t HistosInitialized=kFALSE;
  //static TH2F* P_closeTrackPtr1;
  //static TH2F* P_closeTrackPtr2;
  static TH2F* P_closeTrack1;
  static TH2F* P_closeTrack2;

  if(!HistosInitialized)
    {
      //cout << "Initializing histograms!" << endl;
      TFile* histfile = TFile::Open("CloseTrackParams.root");
      P_closeTrack1 = (TH2F*)histfile->FindObjectAny("P_double_nochi2");
      P_closeTrack2 = (TH2F*)histfile->FindObjectAny("P_double_chi2");
      P_closeTrack1->SetDirectory(NULL);
      P_closeTrack2->SetDirectory(NULL);
      //cout << "Int1: " <<P_closeTrack1->Integral()<<endl;
      //cout << "Int2: " <<P_closeTrack2->Integral()<<endl;
      histfile->Close();
      HistosInitialized=kTRUE;
    }
  //cout << "Pointer 1:" << P_closeTrack1 << endl;
  //cout << "Pointer 2:" << P_closeTrack2 << endl;
  Float_t chi2=pHitData->iMdcChiSquare;
  Float_t nwiresmdc1=pHitData->iMdcNWires1;
  Float_t clussizemdc1=pHitData->iMdcClsSize1;
  
  /*  cout << "clussize:" << clussizemdc1 <<endl;
  cout << "nwires:" << nwiresmdc1 <<endl;
  cout << "chi2:" << chi2 <<endl;
  cout <<"Int1: " <<P_closeTrack1->Integral();
  cout <<"Int2: " <<P_closeTrack2->Integral();
  cin >> wait;*/
  Int_t histBin;
  switch(useChiSquare)
    {
    case 0: //We use just the track observables and ignore chisquare
      histBin=P_closeTrack1->FindBin(clussizemdc1,nwiresmdc1);
      return P_closeTrack1->GetBinContent(histBin);
    case 1:
      //We use the track observables but only for particles with NEGATIVE chi square;
      //For the others we return the default value!
      if(chi2>=0)
	return 0.06;
      else 
	{
	  histBin=P_closeTrack2->FindBin(clussizemdc1,nwiresmdc1);
	  return P_closeTrack2->GetBinContent(histBin);
	}
    case 2: //We use only chi2
      if(chi2>=0)
	return 0.06;
      else 
	return 0.72;

    default:
      return -1.0;
    }

}

void HPidFL::calcParticleAnglesKine(HGeantKine *kine,Float_t &theta, Float_t &phi)
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

Float_t HPidFL::calcInvMassKine(HGeantKine *kine1,HGeantKine *kine2)
{

    Float_t opang = TMath::DegToRad()*
	calcOpeningAngleKine(kine1,kine2);
    Float_t p1 = kine1->getTotalMomentum();
    Float_t p2 = kine2->getTotalMomentum();
    return 2.*sin(opang/2.)*sqrt(p1*p2);
}

Float_t HPidFL::calcOpeningAngleKine(HGeantKine *kine1,HGeantKine *kine2)
{
    //input 2 kine objects
    //output opening angle of trajectories

    Double_t rad2deg = TMath::RadToDeg();

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

Float_t HPidFL::calcOpeningAngleVectors(HGeomVector &v1,HGeomVector &v2)
{
    //input two vectors, output opening angle
    Double_t rad2deg = TMath::RadToDeg();

    v1/=v1.length();//norm
    v2/=v2.length();//norm

    Float_t cosfOpeningAngle = v1.scalarProduct(v2);
    Float_t fOpeningAngle=0.;
    if (TMath::Abs(cosfOpeningAngle) <= 1) 
	fOpeningAngle=TMath::ACos(cosfOpeningAngle) * rad2deg;
    
    return fOpeningAngle;
}

Float_t HPidFL::openAngleDeg(Float_t th1, Float_t ph1, Float_t th2, Float_t ph2) 
{
// Returns angle between two directions in DEGREES (input angles in degrees)
  Float_t r2d = TMath::RadToDeg();
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

Bool_t HPidFL::getGeantMatches(HPidTrackCand* giveCandidate,Int_t& nMatchesMdcMeta, Int_t& nMatchesFull,
					 Int_t& nLeptonicMatchesMdcMeta, Int_t& GeantPid)
{
  Char_t ListOfTracks[10];
  Short_t IdsOfTracks[10];
  Int_t aTrack, aID;
  for (Int_t i =0;i< 10;i++){
    ListOfTracks[i]=-99;
    IdsOfTracks[i]=-99;
  }
  nMatchesMdcMeta=0;
  nMatchesFull=0;
  nLeptonicMatchesMdcMeta=0;
  GeantPid=-99;

  HGeantKine* theKine;
  HLinearCategory* fGeantKineCat = (HLinearCategory*) getCategory(catGeantKine);
  static HIterator* pIteratorGeantKine = (HIterator *)fGeantKineCat ->MakeIterator("native");

  //This function checks, how many matches have been found in the common track set 
  giveCandidate->getGeantTrackSet()->fillCommonTracks(ListOfTracks,IdsOfTracks,10);

  Int_t i=0;
  while(ListOfTracks[i]>=0)
    {
      if(ListOfTracks[i]==HPidCommonTrack::kTrackFull){
	nMatchesFull++;
	nMatchesMdcMeta++;
      }
      if(ListOfTracks[i]==HPidCommonTrack::kTrackKickPlane){
	nMatchesMdcMeta++;
      }
      pIteratorGeantKine->Reset();

      while((theKine=(HGeantKine *)pIteratorGeantKine->Next())!=0)
	{
	  theKine->getParticle(aTrack,aID);
	  if(i==0 && (ListOfTracks[0]==HPidCommonTrack::kTrackKickPlane || 
		      ListOfTracks[0]==HPidCommonTrack::kTrackFull))
	    {
	      if (aTrack==IdsOfTracks[0])
		GeantPid=aID; //set geant PID to most common track ID if it was seen at least in MDC and META
	    }

	  if (aTrack==IdsOfTracks[i] && (aID==2 ||aID==3))
	    {
	      if(ListOfTracks[i]==HPidCommonTrack::kTrackKickPlane || 
		 ListOfTracks[i]==HPidCommonTrack::kTrackFull)
		{
		  nLeptonicMatchesMdcMeta++;
		}
	    }
	} // end while kine loop
      
      i++;
    }
  return kTRUE;
}

Bool_t HPidFL::getGeantMatches(HPidTrackCand* giveCandidate, Int_t& nMatchesRichMdc,
			       Int_t& nMatchesMdcMeta, Int_t& nMatchesFull,
			       Int_t& nLeptonicMatchesFull, Int_t& nLeptonicMatchesMdcMeta, Int_t& GeantPid)
{
  Char_t ListOfTracks[10];
  Short_t IdsOfTracks[10];
  for (Int_t i =0;i< 10;i++){
    ListOfTracks[i]=-99;
    IdsOfTracks[i]=-99;
  }
  nMatchesMdcMeta=0;
  nMatchesRichMdc=0;
  nMatchesFull=0;
  nLeptonicMatchesFull=0;
  nLeptonicMatchesMdcMeta=0;
  GeantPid=-99;

  HGeantKine* theKine;
  HLinearCategory* fGeantKineCat = (HLinearCategory*) getCategory(catGeantKine);
  static HIterator* pIteratorGeantKine = (HIterator *)fGeantKineCat ->MakeIterator("native");

  //This function checks, how many matches have been found in the common track set 
  giveCandidate->getGeantTrackSet()->fillCommonTracks(ListOfTracks,IdsOfTracks,10);
  Int_t i=0;
  while(ListOfTracks[i]>=0)
    {
      if(ListOfTracks[i]==HPidCommonTrack::kTrackFull){
	nMatchesFull++;
	nMatchesRichMdc++;
	nMatchesMdcMeta++;
      }
      if(ListOfTracks[i]==HPidCommonTrack::kTracklet){
	nMatchesRichMdc++;
      }
      if(ListOfTracks[i]==HPidCommonTrack::kTrackKickPlane){
	nMatchesMdcMeta++;
      }

      pIteratorGeantKine->Reset();
      while((theKine=(HGeantKine *)pIteratorGeantKine->Next())!=0)
	{
	  Int_t aTrack, aID;
	  theKine->getParticle(aTrack,aID);
	  if(i==0 && (ListOfTracks[0]==HPidCommonTrack::kTrackKickPlane || 
		      ListOfTracks[0]==HPidCommonTrack::kTrackFull))
	    {
	      if (aTrack==IdsOfTracks[0])
		  GeantPid=aID; //set geant PID to most common track ID if it was seen at least in MDC and META
	    }

	  if (aTrack==IdsOfTracks[i] && (aID==2 ||aID==3))
	    {
	      if(ListOfTracks[i]==HPidCommonTrack::kTrackFull)
		{
		  nLeptonicMatchesFull++;
		  nLeptonicMatchesMdcMeta++;
		}
	      if(ListOfTracks[i]==HPidCommonTrack::kTrackKickPlane)
		{
		  nLeptonicMatchesMdcMeta++;
		}
	      
	    }
	} // end while kine loop
      
      i++;
    }
  return kTRUE;
}

Float_t HPidFL::calcOpeningAngleT(Float_t t1,Float_t p1,
				      Float_t t2,Float_t p2)
{
    //input theta and phi of two trajectories
    //returns opening angle 
    Float_t eps=1.e-4;
    Float_t fOpeningAngle=-1.;
    if (TMath::Abs(t1-t2)<eps && TMath::Abs(p1-p2)<eps)
    {
	fOpeningAngle = 0.;
    }
    else
    {
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
    }
    return fOpeningAngle;
}

Int_t HPidFL::countLeptonsInKickTrack(HKickTrackSim* theKickTrack)
{
  Int_t leptonCounter=0;
  Int_t MdcTrackId=-1;
  HGeantKine* theKine;
  HLinearCategory* fGeantKineCat = (HLinearCategory*) getCategory(catGeantKine);
  static HIterator* pIteratorGeantKine = (HIterator *)fGeantKineCat ->MakeIterator("native");
  for(Int_t i=0;i< theKickTrack->getNMdcTracks();i++)
    {
      MdcTrackId = theKickTrack->getMdcTrack(i);

      // loop over kine container
      pIteratorGeantKine->Reset();
      while((theKine=(HGeantKine *)pIteratorGeantKine->Next())!=0)
	{
	  Int_t aTrack, aID;
	  theKine->getParticle(aTrack,aID);
	  if (aTrack==MdcTrackId && (aID==2 ||aID==3))
	    {
	      leptonCounter++;
	    }
	} // end while kine loop
    }
  return leptonCounter;
}


void HPidFL::dumpKickTrack(HKickTrack *track)
{
    Float_t r2d = TMath::RadToDeg();
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

void HPidFL::dumpKine(HGeantKine *kine)
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

HGeantKine* HPidFL::findGeantParent(HGeantKine *kine)
{
    HLinearCategory * cat = (HLinearCategory*) getCategory(catGeantKine);
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
		//if (kDumpIt) dumpKine(findGeantParentFast(kine2));//recursive research for relatives
		return kine2;
	    }
	}
    }
    return 0;
}
HGeantKine* HPidFL::findGeantParentFast(HGeantKine *kine)
{
    HLinearCategory * cat = (HLinearCategory*) getCategory(catGeantKine);
    Int_t aPar, aMed, aMech;
    kine->getCreator(aPar,aMed,aMech);
    if (aPar <=0)
    {
        return NULL;
    }
    else
    {
	if(cat)
	{
    	    return (HGeantKine *) cat->getObject(aPar - 1);
	}
    }
    return NULL;
}


Bool_t HPidFL::isLepFromPi0DalitzDecay(Int_t trk, Char_t *style)
{
    // returns 1 if track nb corresponds to a lepton from pi0 Dalitz decay
    // else returns zero

    HLinearCategory * cat = (HLinearCategory*) getCategory(catGeantKine);
    HGeantKine * kine =0;
    // loop over kine container
    HIterator* iter_kine = (HIterator*)(cat->MakeIterator("native"));
    iter_kine->Reset();
    while((kine=(HGeantKine *)iter_kine->Next())!=0)
    {
	Int_t aTrack, aID;
	kine->getParticle(aTrack,aID);
	if (aTrack==trk) return isLepFromPi0DalitzDecay(kine,style);
    } // end while kine loop
    
    return kFALSE;    
}
Bool_t HPidFL::isLepFromPi0DalitzDecay(HGeantKine* kine,Char_t *style)
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

    Int_t aMechCut=0;
    Int_t aParCut=0;

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
	    if (opt.Contains("pluto") && aPar==aParCut) return kTRUE;
	    else if (opt.Contains("urqmd"))
	      {
		// only in case that generator was UrQMD
		HGeantKine *kine_parent1 = findGeantParentFast(kine);
		if(kine_parent1)
		  {//parent should be pion
		    //  		Float_t theta1,phi1;
		    //  		theta1=phi1=0.;
		    //  		   
		    //HPidFL::calcParticleAnglesKine(kine_parent1,theta1,phi1);
		    Int_t aTrackp1, aIDp1;
		    kine_parent1->getParticle(aTrackp1,aIDp1);
		    //if(aIDp1==aParCut && (theta1>=10. && theta1<=90.))//pion
		    if(aIDp1==aParCut)//pion, but no emission angle cut
		      {
			return kTRUE;
		      }
		    else return kFALSE;
		    
		    
		  }//kine_parent1
	      }//
	    
	  }//Mec==5 and lepton
	
	return kFALSE;    
}

Bool_t HPidFL::isLepFromPi0GammaConv(Int_t trk)
{
    // returns 1 if track nb corresponds to a lepton from gamma conv
    // else returns zero

    HLinearCategory * cat = (HLinearCategory*) getCategory(catGeantKine);

    HGeantKine * kine =0;
    // loop over kine container
    HIterator* iter_kine = (HIterator*)(cat->MakeIterator("native"));
    iter_kine->Reset();
    while((kine=(HGeantKine *)iter_kine->Next())!=0)
    {
	Int_t aTrack, aID;
	kine->getParticle(aTrack,aID);
	if (aTrack==trk) return isLepFromPi0GammaConv(kine);
    } // end while kine loop
    
    return kFALSE;
}
Bool_t HPidFL::isLepFromPi0GammaConv(HGeantKine* kine)
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
	    HGeantKine *kine_parent1 = findGeantParentFast(kine);
	    if(kine_parent1){
		
		Int_t aTrackp1, aIDp1;
		Int_t aParp1,aMedp1,aMechp1;
		kine_parent1->getParticle(aTrackp1,aIDp1);
		kine_parent1->getCreator(aParp1,aMedp1,aMechp1);
		if(aIDp1==1 && aMechp1==5)//gamma
		{
		    
		    HGeantKine *kine_parent2 = findGeantParentFast(kine_parent1);
		    if(kine_parent2){
			Int_t aTrackp2, aIDp2;
			kine_parent2->getParticle(aTrackp2,aIDp2);
			//  Float_t theta2,phi2;
			//  			theta2=phi2=0.;
			//  			HPidFL::calcParticleAnglesKine(kine_parent2,theta2,phi2);
			//if(aIDp2==7 && (theta2>=10. && theta2<=90.))
			if(aIDp2==7)
			{//pi0 in acceptance
			    return kTRUE;
			}
			else return kFALSE;
			
			
		    }//kine_parent2
		}//gamma
	    }//kine_parent1
	}//Mec==6 and lepton
	
	return kFALSE;
}
Bool_t HPidFL::isGamma(HGeantKine *kine)
{
    Bool_t ret_val=kFALSE;

    Int_t aTrack, aID;
    kine->getParticle(aTrack,aID);
    if (aID==1) ret_val=kTRUE;

    return ret_val;

}
Bool_t HPidFL::isGammaFromPi0Decay(HGeantKine *kine)
{
    Bool_t ret_val=kFALSE;
    Int_t aPar, aMed, aMech;
    kine->getCreator(aPar,aMed,aMech);
    Int_t aTrack, aID;
    kine->getParticle(aTrack,aID);
    if (getGeantParentParID(aTrack)==7 && aMech==5 && aID==1) ret_val=kTRUE;

    return ret_val;
}

HGeantKine* HPidFL::getSecondGammaFromPionDecay(HGeantKine *kine)
{
    HLinearCategory * cat = (HLinearCategory*) getCategory(catGeantKine);

    // input first gamma 
    Int_t aPar, aMed, aMech;
    kine->getCreator(aPar,aMed,aMech);
    Int_t aTrack, aID;
    kine->getParticle(aTrack,aID);

    if (getGeantParentParID(aTrack)==7 && aMech==5 && aID==1)
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
		if (getGeantParentParID(aTrackSister)==7 && aMechSister==5)
		{
		    //dumpKine(kine2);
		    if (aPar == aParSister && aIDSister==1) return kine2;
		} 
	    }
	}
    }
    return 0;
}

HGeantKine* HPidFL::getSecondLepFromPionDalitzDecay(HGeantKine *kine)
{
    HLinearCategory * cat = (HLinearCategory*) getCategory(catGeantKine);

    Int_t aPar, aMed, aMech;
    kine->getCreator(aPar,aMed,aMech);
    Int_t aTrack, aID;
    kine->getParticle(aTrack,aID);
    if (getGeantParentParID(aTrack)==7 && aMech==5 && (aID==2 || aID==3))
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
		if (getGeantParentParID(aTrackSister)==7 && aMechSister==5)
		{
		    if (aPar == aParSister && (aIDSister==2 || aIDSister==3) ) return kine2;
		} 
	    }
	}
    }
    return 0;
}

HGeantKine* HPidFL::getGammaFromPionDalitzDecay(HGeantKine *kine)
{
    // kine is a lepton from a pi0-Dalitz decay
    // now find gamma which was 3rd particle in decay

    HLinearCategory * cat = (HLinearCategory*) getCategory(catGeantKine);
    Int_t aParLepton, aMedLepton, aMechLepton;
    kine->getCreator(aParLepton,aMedLepton,aMechLepton);
    Int_t aTrackLepton, aIDLepton;
    kine->getParticle(aTrackLepton,aIDLepton);
    
    // verify Dalitz decay origin of provided lepton  
    if (
	getGeantParentParID(aTrackLepton)==7 && 
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
		getGeantParentParID(aTrackGamma)==7 &&
		aMechGamma==5 && 
		aIDGamma==1
		) return gamma;
	}
    }
    return 0;


}
HGeantKine* HPidFL::getGeantKineObjFromTrkNb(Int_t nTrkNb)
{
    HLinearCategory * cat = (HLinearCategory*) getCategory(catGeantKine);
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
HGeantKine* HPidFL::getGeantKineObjFromTrkNbFast(Int_t nTrkNb)
{
   HLinearCategory * cat = (HLinearCategory*) getCategory(catGeantKine);
   if(nTrkNb <= 0)
        return NULL;

    if(cat == NULL)
    {
        ::Error("HPidFL::getGeantKineObjFromTrkNbFast", "No catGeantKine category");
        return NULL;
    }

    return (HGeantKine *) cat->getObject(nTrkNb - 1);
}
 
Int_t HPidFL::getGeantParID(Int_t nTrkNb)
{
    HLinearCategory * cat = (HLinearCategory*) getCategory(catGeantKine);
    Int_t dummy,aID;
    aID=0;
    if (!cat) return aID;
    HGeantKine *kine = getGeantKineObjFromTrkNbFast(nTrkNb);
    if (kine)
    {
	kine->getParticle(dummy,aID);
    }
    return aID;
}

Int_t HPidFL::getGeantParentParID(Int_t nTrkNb)
{
    Int_t dummy,aID;
    aID=0;
    HLinearCategory * cat = (HLinearCategory*) getCategory(catGeantKine);
    if (!cat) return aID;
    HGeantKine *kine = getGeantKineObjFromTrkNbFast(nTrkNb);
    if (kine)
    {
	HGeantKine *parent = findGeantParent(kine);
	if (parent) parent->getParticle(dummy,aID);
    }
    return aID;

}
Int_t HPidFL::getGeantParentParID(HGeantKine* kine)
{

    HLinearCategory * cat = (HLinearCategory*) getCategory(catGeantKine);

    Int_t dummy,aID;
    aID=0;
    if (!kine || !cat) return aID;
    HGeantKine *parent = findGeantParentFast(kine);
    if (parent) parent->getParticle(dummy,aID);
    return aID;

}

void HPidFL::saveHistos(TFile* pFileOut, TObjArray* pHistArray)
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

void HPidFL::saveCuts(TFile* pFileOut, TObjArray* pArray)
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
	( (HPidCutO*)(*pArray)[i] )->Write();
    }
}


void HPidFL::dumpCategory(Cat_t iCategory)
{
    // each data category should have a function that dumps an obj to stdout
    // for easy inspection by the user. If this was forgotten use this
  HCategory* fCategory  = getCategory(iCategory);
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
void HPidFL::rotateTrackToLab(Int_t s,Float_t tin,Float_t pin,Float_t &tout,Float_t &pout)
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
    Float_t r2d = TMath::RadToDeg();;
    tout = tin*r2d;
    pout = pin*r2d + (s*60.);
    if(pout > 360.) pout-= 360.;
		

}

Float_t HPidFL::getDeflectionAngle(HHitMatch* hi)
{
    Float_t mt=hi->getMdcTheta();
    Float_t mm=-1.;

    // whether track was used before is not checked here !
    // check whether META was TOF or SHOWER
    if (hi->getTofTheta()>0.) mm=hi->getTofTheta();
    else if (hi->getShowerTheta()>0.) mm=hi->getShowerTheta();
    if (mm<0.||mt<0.) cout<<"HPidFL::getDeflectionAngle:  meta or mdc polar angle not initialized !"<<endl;
    return mt-mm;
}

Bool_t HPidFL::isNew2Tuple(Int_t i,Int_t j,Int_t *tuple,Int_t max)
{
    //this function projects (i,j)-->(n) {N2-->N1}
    //according to the function ax+by
    //and decides whether this value is already stored or not
    //the array tuple is assumed to be init to -2

    //dynamically choose multiplicator
    Int_t base=10;
    Int_t cnt=1;
    while ((max%cnt)<max)
    {
	cnt=cnt*base;
    }
    Int_t f1=cnt*i+j;
    Int_t f2=cnt*j+i;
    //    cout<<"number of entries is: "<<max<<"  and multiplicator is: "<<cnt<<endl;
    Int_t index=-1;
    //  cout<<"Index: ("<<i<<","<<j<<") ==> "<<f1<<" = "<<f2<<endl;
    for (Int_t k=0;k<max;k++)
    {

	if (tuple[k]==-2) 
	{
	    //    cout<<"this is tuple "<<tuple[k]<<endl;
	    index=k;
	    //      cout<<"only default found , break!"<<endl;
	    break;
	}//not found in not default
	if (f1==tuple[k] || f2==tuple[k]) 
	{
	    //cout<<"already stored: "<<tuple[k]<<endl;
	    return kFALSE;//already stored
	}
    }
    //    cout<<index<<" is index"<<endl;
    if (index>-1) 
    {
	tuple[index]=f1;//store new 2-tuple
	//cout<<"newly stored: "<<tuple[index]<<endl;
    }
    else cout<<"(isNew2Tuple,index of new slot not set)"<<endl;
    return kTRUE;
}

Bool_t HPidFL::isNewIndex(Int_t i,Int_t* iarr,Int_t max)
{
    // compare int to content of given array iarr
    // and decide if it is already stored or not
    // if not stored, remember the int

    Int_t n=0;

    do{
	if(i==iarr[n]) break;//i already stored
	else if(iarr[n]==-2) //new slot
	{
	    iarr[n]=i;//store value i in new slot
	    return kTRUE;
	}
	n++;
    } while(n<max);

    return kFALSE;

}


//		    isPair=kTRUE;
		    //		    arr->Add(pair);
		    // this is a true pair
//  		    cout<<"**************************"<<endl;
//  		    pair->dumpToStdoutSim();
//  		    if (trackG1->getPi0Dalitz(i) && trackG2->getPi0Dalitz(j) )
//  		    {
//  			cout<<"Pi0Dalitz pair "<<endl;
			
//  		    }
//  		    if (trackG1->getConvLep(i) && trackG2->getConvLep(j) )
//  		    {
//  			cout<<"Conversion  pair "<<endl;
			
//  		    }
		    
//  		    trackG1->dumpToStdout();
//  		    trackG2->dumpToStdout();
//  		    cout<<"Dalitz 1: "<<trackG1->getPi0Dalitz(i)
//  			<<"  Conversion 1: "<<trackG1->getConvLep(i)
//  			<<"  **** Dalitz 2: "<<trackG2->getPi0Dalitz(j)
//  			<<"  Conversion 2: "<<trackG2->getConvLep(j)<<endl;
//                  cout<<"**************************"<<endl;


Bool_t HPidFL::isLepOnMirror(HRichHitSim* r,Int_t &nTrkNbOnMirror)
{

    HMatrixCategory * mirrcat = (HMatrixCategory*) getCategory(catRichGeantRaw+2);

    HRichGeometryPar* pGeo = (HRichGeometryPar*) getParamContainer("RichGeometryParameters");
    Float_t fYShift = pGeo->getSectorShift(); // to account for shifted volumes in HGEANT !
    fYShift = fYShift/TMath::Cos( 20. * TMath::DegToRad() );//20 degrees is padplane tilt
    HRichPadTab* pPadsTab = ((HRichGeometryPar*)pGeo)->getPadsPar();
    if (!pPadsTab) return kFALSE;
    HRichPad *pPad = 0;

    if (!r) return kFALSE;
    if (r)
    {
	Int_t xRing=r->getRingCenterX();
	Int_t yRing=r->getRingCenterY();
	Int_t secRing=r->getSector();
	// iterate over hits on mirror and compare hit ring x,y with
	// center of reflected photons, calculate pad hit by photon ctr
	HGeantRichMirror *mirr=0;
	HIterator *iter_mirror = (HIterator *)(mirrcat->MakeIterator("native"));
	iter_mirror->Reset();
	while((mirr=(HGeantRichMirror *)iter_mirror->Next())!=0)
	{
	    
	    Int_t aTrk,aID;
	    mirr->getTrack(aTrk,aID);
	    if(matchRingMirrTracks(r,aTrk,aID)>0) // ring has track of kine obj seen traversing the mirror
	      {

		// project back from mirror to padplane
		Float_t x=mirr->getYRing()/10.;//in mm of the padplane
		Float_t y=mirr->getXRing()/10. + fYShift;
		pPad=pPadsTab->getPad(x, // they are swapped, sic !
				      y );// divide by 10 to account for mm->cm
		if (!pPad) return kFALSE;
		Int_t sec=mirr->getSector();
		Int_t xMirrPhotCM=pPad->getPadX();
		Int_t yMirrPhotCM=pPad->getPadY();

		// check position of ring cntr and photon cntr
		if (
		    TMath::Abs(xMirrPhotCM-xRing) <  2 && 
		    TMath::Abs(yMirrPhotCM-yRing) <  2 && 
		    sec == secRing
		    )
		{
		    nTrkNbOnMirror = aTrk;
		    return kTRUE; // ring stems from lepton that made photons with cm
		                  // in position found by pattern recognition in correct place.
		} 
	    }
	}
    }
    return kFALSE;
}

Bool_t HPidFL::isDoubleLepOnMirror(HRichHitSim* r,Int_t &nTrkNbOnMirror1, Int_t &nTrkNbOnMirror2)
{

    HMatrixCategory * mirrcat = (HMatrixCategory*) getCategory(catRichGeantRaw+2);

    HRichGeometryPar* pGeo = (HRichGeometryPar*) getParamContainer("RichGeometryParameters");
    Float_t fYShift = pGeo->getSectorShift(); // to account for shifted volumes in HGEANT !
    fYShift = fYShift/TMath::Cos( 20. * TMath::DegToRad() );//20 degrees is padplane tilt
    HRichPadTab* pPadsTab = ((HRichGeometryPar*)pGeo)->getPadsPar();
    if (!pPadsTab) return kFALSE;
    HRichPad *pPad = 0;

    Bool_t kFirstLepton,kSecondLepton;kFirstLepton = kSecondLepton = kFALSE;

    if (!r) return kFALSE;
    if (r)
    {
	Int_t xRing=r->getRingCenterX();
	Int_t yRing=r->getRingCenterY();
	Int_t secRing=r->getSector();
	// iterate over hits on mirror and compare hit ring x,y with
	// center of reflected photons, calculate pad hit by photon ctr
	HGeantRichMirror *mirr=0;
	HIterator *iter_mirror = (HIterator *)(mirrcat->MakeIterator("native"));
	iter_mirror->Reset();
	while((mirr=(HGeantRichMirror *)iter_mirror->Next())!=0)
	{

	  Int_t aTrk,aID;
	  mirr->getTrack(aTrk,aID);
	  if(matchRingMirrTracks(r,aTrk,aID)>0) // ring has track of kine obj seen traversing the mirror
	    {
	      // project back from mirror to padplane
	      Float_t x=mirr->getYRing()/10.;//in mm of the padplane
	      Float_t y=mirr->getXRing()/10. + fYShift;
	      pPad=pPadsTab->getPad(x, // they are swapped, sic !
				    y );// divide by 10 to account for mm->cm
	      //cout << "pPad:" <<pPad << endl;
	      if (!pPad) return kFALSE;
	      //cout << "pad reconstructed" << endl;
	      Int_t sec=mirr->getSector();
	      Int_t xMirrPhotCM=pPad->getPadX();
	      Int_t yMirrPhotCM=pPad->getPadY();
	      
	      if (
		  TMath::Abs(xMirrPhotCM-xRing) <  2 && 
		  TMath::Abs(yMirrPhotCM-yRing) <  2 && 
		  sec == secRing                     &&
		  !kFirstLepton
		  )
		{
		  nTrkNbOnMirror1 = aTrk; 
		  kFirstLepton = kTRUE;
		  //cout << "first lepton found" << endl;
		} 
	      else if (
		       TMath::Abs(xMirrPhotCM-xRing) <  4 && 
		       TMath::Abs(yMirrPhotCM-yRing) <  4 && 
		       sec == secRing                     &&
		       !kSecondLepton
		       ) 
		{
		  nTrkNbOnMirror2 = aTrk;
		  kSecondLepton = kTRUE;
		  //cout << "second lepton found" << endl;
		}
	    }
	}
	if ( kFirstLepton && kSecondLepton ) return kTRUE;
    }

    return kFALSE;
}


Bool_t HPidFL::isLepOnMirror(HGeantKine* kine)
{

    HMatrixCategory * cat = (HMatrixCategory*) getCategory(catRichGeantRaw+2);
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

Bool_t HPidFL::isLepOnMDC(HGeantKine* kine)
{
      //check whether the particle with GEANT track number trk
      //has reached the MDC module 0, layer 0
      //this means that it didn't hit a spoke or edge in the RICH
      //and can therefore be correlated and detected, finally going 
      //in efficiency.


    HMatrixCategory * cat = (HMatrixCategory*) getCategory(catMdcGeantRaw);

    Int_t aKineTrack, aKineID;
    if (kine) kine->getParticle(aKineTrack,aKineID);
    else return kFALSE;
    HGeantMdc *gmdc = 0;
    HIterator *iter_mdcgeant = (HIterator *)(cat->MakeIterator("native"));
    if (iter_mdcgeant) iter_mdcgeant->Reset();
    else return kFALSE;
    while((gmdc=(HGeantMdc *)iter_mdcgeant->Next())!=0)
    {
	Int_t mod = (Int_t) gmdc->getModule();
	Int_t lay = (Int_t) gmdc->getLayer();
	if (gmdc->getTrack()==aKineTrack && mod==0 && lay==0) 
	{
	    return kTRUE;
	}
    }
    return kFALSE;
}

Bool_t HPidFL::isRingFromLeptonOnMirrorAndMdc(HRichHitSim *r)
{
    Bool_t kReturnValue = kFALSE;
    Int_t nTrkNb = 0;
    Bool_t kLeptonRing = isLepOnMirror(r,nTrkNb);
    if (kLeptonRing) kReturnValue = isLepOnMDC(getGeantKineObjFromTrkNbFast(nTrkNb));
    return kReturnValue;

}

Float_t HPidFL::calcRichWeight(HRichHitSim* r, Int_t ind)
{
  // use the number of fired pads in a ring per tracknumber as 
  // a confidence value for this track number
  Float_t sumWeightsRich=0.;
  sumWeightsRich += (float) r->weigTrack1;
  sumWeightsRich += (float) r->weigTrack2;
  sumWeightsRich += (float) r->weigTrack3;
  switch (ind)
    {
    case 1:  return (float)r->weigTrack1/sumWeightsRich;
      break;
    case 2:  return (float)r->weigTrack2/sumWeightsRich;
      break;
    case 3:  return (float)r->weigTrack3/sumWeightsRich;
      break;
    default : return 0.0;
      break;
    }
}

Int_t HPidFL::matchRingMirrTracks(HRichHitSim *r,Int_t trackMirr,Int_t idMirr)
{

    // return weight of photon contribution of track on mirror and in ring
    Int_t t1,t2,t3;
    t1=t2=t3=-1;
    t1=r->track1;t2=r->track2;t3=r->track3;
    // get particle id of 
    Int_t p1,p2,p3;p1=p2=p3=-1;
    if (t1>-1) p1=getGeantParID(t1);
    if (t2>-1) p2=getGeantParID(t2);
    if (t3>-1) p3=getGeantParID(t3);


    if (t1==trackMirr && idMirr==p1) return r->weigTrack1;
    if (t2==trackMirr && idMirr==p2) return r->weigTrack2;
    if (t3==trackMirr && idMirr==p3) return r->weigTrack3;
    return 0; 
}


HCategory* HPidFL::getCategory(Cat_t cat, Bool_t bWarning)
{
// Find HCategory if gHades->getCurrentEvent() is accessible

HEvent    *pEvent;
HCategory *pCat;

    if((gHades == NULL) || ((pEvent = gHades->getCurrentEvent()) == NULL))
    {
        if(bWarning)
        {
            ::Error("HPidFL::getCategory",
                            "Cannot access current event");
        }

        return NULL;
    }

    if((pCat = pEvent->getCategory(cat)) == NULL)
    {
        if(bWarning)
            ::Error("HPidFL::getCategory", "No category: %d", cat);
    }

    return pCat;
}
HParSet* HPidFL::getParamContainer(Char_t * contname)
{
	return (HParSet*)((gHades->getRuntimeDb())->getContainer(contname));
}
