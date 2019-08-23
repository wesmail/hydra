// File: hrichutilfunc.cc
//
// Author: Thomas Eberl <Thomas.Eberl@physik.tu-muenchen.de>
// Last update by Thomas Eberl: 01/09/18 17:17:03
//

#include "hrichutilfunc.h"
#include "hgeantkine.h"
#include "hdebug.h"
#include "hades.h"
#include "hgeomvector2.h"
#include "hgeomvector.h"
#include "hconstant.h"
#include <TH1.h>
///////////////////////////////////////////////////////////
// HRichUtilFunc
//
// this class is a helper that contains functionality
// that can be shared by different reonstructors
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

Float_t HRichUtilFunc::calcOpeningAngleT(Float_t t1,Float_t p1,
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
	HConstant hconst;
	Double_t rad2deg = hconst.rad2deg();
	
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

void HRichUtilFunc::saveHistos(TFile* pFileOut, TObjArray* pHistArray)
{
    // use this function to save histos every n events 
    // assumes that you have opened a file and that your
    // histograms are stored in a TObjArray
    pFileOut->cd();
    // write histograms
    for (Int_t i=0;i<(pHistArray->GetLast()+1);i++)
    {
	( (TH1*)(*pHistArray)[i] )->Write();
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