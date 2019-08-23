// File: hdihitmatch.cc
//
// Author: Thomas Eberl <Thomas.Eberl@physik.tu-muenchen.de>
// Last update by Thomas Eberl: 02/04/09 17:41:04
//
//
#include "hdihitmatch.h"
#include "hhitmatch.h"
#include <iostream.h>
#include <TLorentzVector.h>
#include <TVector3.h>
#include "hrichutilfunc.h"
ClassImp(HDiHitMatch)

HDiHitMatch::HDiHitMatch() {
    reset();//default init of data members
    
}
HDiHitMatch::HDiHitMatch(HHitMatch* trk1, HHitMatch* trk2) {
    reset();//default init of data members
    setIndHitsTrk1(trk1->getRichInd(),
		   trk1->getMdcInd(),
		   trk1->getTofInd(),
		   trk1->getShowInd(),
		   trk1->getKickInd());
    setIndHitsTrk2(trk2->getRichInd(),
		   trk2->getMdcInd(),
		   trk2->getTofInd(),
		   trk2->getShowInd(),
		   trk2->getKickInd());
    if      (trk1->getRichInd()==-1 && trk2->getRichInd()!=-1) setNbDRichHit(0);
    else if (trk1->getRichInd()!=-1 && trk2->getRichInd()==-1) setNbDRichHit(0);
    else if (trk1->getRichInd()==-1 && trk2->getRichInd()==-1) setNbDRichHit(-2); 
    else if (trk1->getRichInd()==trk2->getRichInd()) setNbDRichHit(1);
    else if (trk1->getRichInd()!=trk2->getRichInd()) setNbDRichHit(2);

    if      (trk1->getMdcInd()==-1 && trk2->getMdcInd()!=-1) setNbDMdcHit(0);
    else if (trk1->getMdcInd()!=-1 && trk2->getMdcInd()==-1) setNbDMdcHit(0);
    else if (trk1->getMdcInd()==-1 && trk2->getMdcInd()==-1) setNbDMdcHit(-2); 
    else if (trk1->getMdcInd()==trk2->getMdcInd()) setNbDMdcHit(1);
    else if (trk1->getMdcInd()!=trk2->getMdcInd()) setNbDMdcHit(2);


    Int_t t1 = trk1->getTofInd();
    Int_t s1 = trk1->getShowInd();
    Int_t t2 = trk2->getTofInd();
    Int_t s2 = trk2->getShowInd();
    if      (t1 == -1 && t2 == -1) setNbDMetaHit(-2); // no META hit
    else if (t1 != -1 && t2 == -1) setNbDMetaHit(0); //one trk had META, the other not
    else if (t2 != -1 && t1 == -1) setNbDMetaHit(0);
    else if (t2 == t1            ) setNbDMetaHit(1); // twice the same
    else if (t2 != t1            ) setNbDMetaHit(2); // two different hits in META

    if (getNbDMetaHit()<0)
    {
	if      (s1 == -1 && s2 == -1) setNbDMetaHit(-2);
	else if (s1 != -1 && s2 == -1) setNbDMetaHit(0);
	else if (s2 != -1 && s1 == -1) setNbDMetaHit(0);
	else if (s2 == s1            ) setNbDMetaHit(1);
	else if (s2 != s1            ) setNbDMetaHit(2);
	// mixed case
	if (getNbDMetaHit()<0)
	{
	    if      (s1 == -1 && t2 == -1) setNbDMetaHit(-2);
	    else if (s1 != -1 && t2 == -1) setNbDMetaHit(0);
	    else if (t2 != -1 && s1 == -1) setNbDMetaHit(0);
	    else if (t2 == s1            ) setNbDMetaHit(2);
	    else if (t2 != s1            ) setNbDMetaHit(2);
	    
	    if (getNbDMetaHit()<0)
	    {
		if      (s2 == -1 && t1 == -1) setNbDMetaHit(-2);
		else if (s2 != -1 && t1 == -1) setNbDMetaHit(0);
		else if (t1 != -1 && s2 == -1) setNbDMetaHit(0);
		else if (t1 == s2            ) setNbDMetaHit(2);
		else if (t1 != s2            ) setNbDMetaHit(2);
	    }
	}
    }
    setIndHitsTrk1( trk1->getRichInd(),
		    trk1->getMdcInd(),
		    trk1->getTofInd(),
		    trk1->getShowInd(),
		    trk1->getKickInd()  );

    setIndHitsTrk2( trk2->getRichInd(),
		    trk2->getMdcInd(),
		    trk2->getTofInd(),
		    trk2->getShowInd(),
		    trk2->getKickInd()  );

    setMomTrk1(trk1->getKickMom());
    setMomTrk2(trk2->getKickMom());
    setCorrCode1(calcCorrCode(trk1));
    setCorrCode2(calcCorrCode(trk2));
//      Int_t c1=getCorrCode1();
//      Int_t c2=getCorrCode2();
    //cout<<"corr code 1: "<<c1<<" -- corr code 2: "<<c2<<endl;
    //    if ( c1==7 && c2==7) // both tracks have full correlation
    //    {
	//trk1->dumpToStdout();
	//cout<<"*******************"<<endl;
	//trk2->dumpToStdout();
    setInvMass(calcInvMass(trk1,trk2));
    setCharge(calcCharge(trk1,trk2));
    setOpangKICK(calcOpangKICK(trk1,trk2));
    setOpangMDC(calcOpangMDC(trk1,trk2));
    setOpangMETA(calcOpangMETA(trk1,trk2));
    setPt(trk1,trk2);
	//    }
}

HDiHitMatch::~HDiHitMatch() {}

HDiHitMatch::HDiHitMatch(const HDiHitMatch& source) {

}

HDiHitMatch& HDiHitMatch::operator=(const HDiHitMatch& source) {
 if (this != &source) {
 }
 
 return *this;
}
void HDiHitMatch::dumpToStdout()
{
    cout<<"***********************************"<<endl;
    cout<<"Track1: code: "<<nCorrCode1<<" - index: "<<indTrk1<<" - hits [RMTSK]: ";
	for (Int_t i=0;i<MAXIND;i++)
	{
	    cout<<indHitsTrk1[i]<<" ";
	}
    cout<<endl;

    cout<<"Track2: code: "<<nCorrCode2<<" - index: "<<indTrk2<<" - hits [RMTSK]: ";
	for (Int_t i=0;i<MAXIND;i++)
	{
	    cout<<indHitsTrk2[i]<<" ";
	}
    cout<<endl;
    cout<<"nb diff Hits Rich: "<<nbDRichHit<<" - MDC: "<<nbDMdcHit
	<<" - META: "<<nbDMetaHit<<endl;
    cout<<"Chrg: "<<charge<<" - OpA MDC: "<<opangMDC<<" - OpA KICK: "<<opangKICK
	<<" - OpA META: "<<opangMETA<<endl;
    cout<<"Inv Mass: "<<invMass<<" - Mom1: "<<mom1<<" - Mom2: "<<mom2<<endl;
}

void HDiHitMatch::reset(void) 
{
    nbDRichHit = -1;
    nbDMdcHit  = -1;
    nbDMetaHit = -1;
    opangMDC   = -1.;
    opangKICK   = -1.;
    opangMETA  = -1.;
    charge     = -1;
    indTrk1    = -1;
    indTrk2    = -1;
    invMass    = -1.;
    for (Int_t i=0;i<MAXIND;i++) indHitsTrk1[i]=-1;
    for (Int_t i=0;i<MAXIND;i++) indHitsTrk2[i]=-1;
}

void HDiHitMatch::setPt(HHitMatch* h1,HHitMatch* h2)
{
    if (h1 && h1->getKickInd()>=0)
    {
	TLorentzVector* v1 = ((TLorentzVector*)h1->getLVec());
	momt1 = (Float_t) v1->Pt();
    }
    if (h2 && h2->getKickInd()>=0)
    {
	TLorentzVector* v2 = ((TLorentzVector*)h2->getLVec());
	momt2 = (Float_t) v2->Pt();
    }
}

Float_t HDiHitMatch::calcInvMass(HHitMatch* h1,HHitMatch* h2)
{
    if (h1->getKickInd()>=0 && h2->getKickInd()>=0 )
    {
	TLorentzVector* v1 = ((TLorentzVector*)h1->getLVec());
	TLorentzVector* v2 = ((TLorentzVector*)h2->getLVec());
	Float_t p1 = (v1->Vect()).Mag();
	Float_t p2 = (v2->Vect()).Mag();
	Float_t eps=1e-3;
	if (p1>0. && p2>0.)
	{

	    if ((h1->getKickMom() - p1) > eps) // consistency check
	    {
		cout<<"mom from kick track: "<<h1->getKickMom()
		    <<" from Lorentz vector: "<<p1<<" diff "
	     <<h1->getKickMom() - p1<<endl;
		Error("HDiHitMatch::calcInvMass","wrong mom");
	    }

	    TVector3 pvec1 = v1->Vect();
	    TVector3 pvec2 = v2->Vect();
	    Float_t opang=pvec1.Angle(pvec2);//in radians
	    Float_t invMass=2.*sin(opang/2.)*sqrt(p1*p2);
	    if (invMass>0.) return invMass;
	    else return -1.;
	    
	}else return -1.;
    }
    else return -1.;
}
Float_t HDiHitMatch::calcOpangMDC(HHitMatch* h1,HHitMatch* h2)
{

    Float_t th1 = h1->getMdcTheta();
    Float_t ph1 = h1->getMdcPhi();
    Float_t th2 = h2->getMdcTheta();
    Float_t ph2 = h2->getMdcPhi();
    Float_t myopangle = HRichUtilFunc::calcOpeningAngleT(th1,ph1,th2,ph2);

    if (myopangle>0.)
    {

//  	TLorentzVector* v1 = ((TLorentzVector*)h1->getLVec());
//  	TLorentzVector* v2 = ((TLorentzVector*)h2->getLVec());
//  	Float_t p1 = (v1->Vect()).Mag();
//  	Float_t p2 = (v2->Vect()).Mag();
//  	if (p1>0. && p2>0.)
//  	{
//  	    TVector3 pvec1 = v1->Vect();
//  	    TVector3 pvec2 = v2->Vect();
//  	    Float_t opang=pvec1.Angle(pvec2);//in radians
//  	    opang=opang*57.29578;//in degrees
//  	    if (opang>0.) 
//  	    {
//  		if (opang-myopangle>0.01) 
//  		{
//  		    cout<<"ERROR in HDiHitMatch::calcOpangMDC"<<endl;
//  		    cout<<"opang MDC: "<<myopangle<<" -- opang KICK: "<<opang<<endl;
//  		}
		return myopangle;
//  	    }
//  	    else return -1.;
//  	} else return -1.;
      } else return -1.;
}
Float_t HDiHitMatch::calcOpangKICK(HHitMatch* h1,HHitMatch* h2)
{
    if (h1->getKickInd()>=0 && h2->getKickInd()>=0 )
    {
	TLorentzVector* v1 = ((TLorentzVector*)h1->getLVec());
	TLorentzVector* v2 = ((TLorentzVector*)h2->getLVec());
	Float_t p1 = (v1->Vect()).Mag();
	Float_t p2 = (v2->Vect()).Mag();
	if (p1>0. && p2>0.)
	{
	    TVector3 pvec1 = v1->Vect();
	    TVector3 pvec2 = v2->Vect();
	    Float_t opang=pvec1.Angle(pvec2);//in radians
	    opang=opang*57.29578;//in degrees
	    if (opang>0.) return opang;
	    else return -1.;
	} else return -1.;
    }
    else return -1.;
}
Float_t HDiHitMatch::calcOpangMETA(HHitMatch* h1,HHitMatch* h2)
{
    
    Int_t nTInd1 = h1->getTofInd();
    Int_t nSInd1 = h1->getShowInd();

    Int_t nTInd2 = h2->getTofInd();
    Int_t nSInd2 = h2->getShowInd();

    Float_t h1Theta,h1Phi,h2Theta,h2Phi;

    if (nTInd1>-1) 
    {
	h1Theta= h1->getTofTheta();
	h1Phi  = h1->getTofPhi();
    }
    else if (nSInd1>-1)
    {
	h1Theta= h1->getShowerTheta();
	h1Phi  = h1->getShowerPhi();
    }
    if (nTInd2>-1) 
    {
	h2Theta= h2->getTofTheta();
	h2Phi  = h2->getTofPhi();
    }
    else if (nSInd2>-1)
    {
	h2Theta= h2->getShowerTheta();
	h2Phi  = h2->getShowerPhi();
    }

    Float_t myopangle = HRichUtilFunc::calcOpeningAngleT(h1Theta,h1Phi,
							 h2Theta,h2Phi);

    if (myopangle>0.)
    {
	return myopangle;
    } else return -1.;
    return -1.;
}
Int_t HDiHitMatch::calcCharge(HHitMatch* h1,HHitMatch* h2)
{
    Int_t c=-1;
    Int_t kc1=h1->getKickCharge();
    Int_t kc2=h2->getKickCharge();
    setKICKCharge(kc1+kc2);
    //calc "deflection angle" using MDC and META info
    Float_t d1=-1.;
    Float_t d2=-1.;

    if (h1->getMatchedMdcTof()==1) 
    {
	d1=h1->getMdcTheta() - h1->getTofTheta();
	//cout<<"tof dtheta1: "<<d1<<endl;
    }
    else if (h1->getMatchedMdcShower()==1) 
    {
	d1=h1->getMdcTheta() - h1->getShowerTheta();
	//cout<<"shower dtheta1: "<<d1<<endl;
    }
    
    
    if (h2->getMatchedMdcTof()==1)
    {
	d2=h2->getMdcTheta() - h2->getTofTheta();
	//cout<<"tof dtheta2: "<<d2<<endl;
    }
    else if (h2->getMatchedMdcShower()==1) 
    {
	d2=h2->getMdcTheta() - h2->getShowerTheta();
	//cout<<"tof dshower2: "<<d2<<endl;
    }
    
	// neg value for deflection is electron
    if (d1*d2>0.)//like sign pair
    {
	//cout<<"like sign pair"<<endl;
	if (d1<0. && d2<0.) c=-2;//2 e-
	if (d1>0. && d2>0.) c= 2;//2 e+
    }
    else if (d1*d2<0.)//unlike sign pair
    {
	//cout<<"unlike sign pair"<<endl;
	c=0;
    }
    if (h1->getKickInd()>=0 && h2->getKickInd()>=0)
    {
	if (c != kc1+kc2) 
	{
	    c = kc1+kc2; // believe in kick track 
//  	    cout<<"kc1: "<<kc1<<" / kc2: "<<kc2<<"  -- c: "<<c
//  		<<"  -- d1:"<<d1<<" / d2:"<<d2<<endl;
//  	    Error ("calcCharge","inconsistent charge KickTrack<->Correlator");
	}
    }
    return c;
}

Int_t HDiHitMatch::calcCorrCode(HHitMatch* h)
{
    Int_t c=-1;
    Int_t rm=h->getMatchedRichMdc();
    Int_t rt=h->getMatchedRichTof();
    Int_t rs=h->getMatchedRichShower();
    Int_t mt=h->getMatchedMdcTof();
    Int_t ms=h->getMatchedMdcShower();
    //cf. look-up table in .h file
    if (rm!=1 && (rt!=1 && rs!=1) && (mt==1 || ms==1) ) c=1;
    if (rm!=1 && (rt==1 || rs==1) && (mt!=1 && ms!=1) ) c=2;
    if (rm!=1 && (rt==1 || rs==1) && (mt==1 || ms==1) ) c=3;
    if (rm==1 && (rt!=1 && rs!=1) && (mt!=1 && ms!=1) ) c=4;
    if (rm==1 && (rt!=1 && rs!=1) && (mt==1 || ms==1) ) c=5;
    if (rm==1 && (rt==1 || rs==1) && (mt!=1 && ms!=1) ) c=6;
    if (rm==1 && (rt==1 || rs==1) && (mt==1 || ms==1) ) c=7;
    return c;
}
