// $Id: hrichcut.cc,v 1.4 2002-09-25 16:19:53 eberl Exp $
// Last update by Thomas Eberl: 02/09/25 17:31:48
//
#include "hrichcut.h"
#include "hrichhitsim.h"
#include "hhitmatch.h"
#include "hhitmatchsim.h"
#include "hdihitmatch.h"
#include "hdihitmatchsim.h"
#include "hlinearcategory.h"
#include <iostream.h>
///////////////////////////////////////////////////////////
// HRichCut
//
// this class is a helper that contains functionality
// that can be shared by different reconstructors
// it provides cuts and conditions on HHitMatch objects
///////////////////////////////////////////////////////////

ClassImp(HRichCut)

HRichCut::HRichCut()
{

}


HRichCut::~HRichCut(void) 
{

}

Bool_t HRichCut::isRICHBetween(HHitMatch* track,Float_t low,Float_t high)
{
    //polar angle cut for Rich ring in tracklet
    Bool_t k = kFALSE;
    Float_t ringtheta=track->getRichTheta();
    if(ringtheta<=high&&ringtheta>=low) k=kTRUE;
    return k;
}
Bool_t HRichCut::isTOFBetween(HHitMatch* track,Float_t low,Float_t high)
{
    Bool_t k = kFALSE;
    Float_t toftof, tofinotof;
    toftof=tofinotof=-1;
    toftof=track->getTofTof();
    tofinotof=track->getTofinoTof();
    if (toftof>-1 && toftof>=low && toftof<=high) return kTRUE;
    else if (tofinotof>-1 && tofinotof>=low && tofinotof<=high) return kTRUE;
    return k;

}

Bool_t HRichCut::isRMThetaDiff(HHitMatch* track,Float_t limit)
{
    Bool_t k = kFALSE;
    Float_t richtheta = track->getRichTheta();
    Float_t mdctheta  = track->getMdcTheta();
    if ((TMath::Abs(richtheta-mdctheta))<limit) 
    {
	//cout<<TMath::Abs(richtheta-mdctheta)<<"   "<<limit<<endl;
	k=kTRUE;
    }
    return k;
}

Bool_t HRichCut::isInKickTrack(HDiHitMatch* pair)
{
    if (pair->getOpangKICK()>0.) return kTRUE;
    else return kFALSE;

}

Bool_t HRichCut::isOpangGreaterThan(HDiHitMatch* pair,Float_t th)
{
    Bool_t k = kFALSE;
    Float_t opang = pair->getOpangMDC();
    if (opang>th) k = kTRUE;
    return k;

}
Bool_t HRichCut::isOpangBetween(HDiHitMatch* pair,Float_t low,Float_t high)
{
    Bool_t k = kFALSE;
    Float_t opang = pair->getOpangMDC();
    if (opang>=low&&opang<=high) k = kTRUE;
    return k;

}
Bool_t HRichCut::isFullRichMdcMetaTracklet(HHitMatch* track)
{
    Bool_t k = kFALSE;
    Int_t nRInd = track->getRichInd();
    Int_t nMInd = track->getMdcInd();
    Int_t nTInd = track->getTofInd();
    Int_t nSInd = track->getShowInd();

    Int_t nRM = track->getMatchedRichMdc();
    Int_t nRT = track->getMatchedRichTof();
    Int_t nMT = track->getMatchedMdcTof();
    
    Int_t nRS = track->getMatchedRichShower();
    Int_t nMS = track->getMatchedMdcShower();

    if (nRInd!=-1 && nTInd!=-1 && nMInd!=-1
	&& nRM==1 &&  nRT==1 &&  nMT==1 )
    {
	
	k=kTRUE;
    }

    else if (nRInd!=-1 && nSInd!=-1 && nMInd!=-1
	     && nRM==1 &&  nRS==1 &&  nMS==1 )
    {
	
	k=kTRUE;
    }

    return k;
}

Int_t HRichCut::NbRingsPerMDCUNLIKEPair(HDiHitMatch* pair)
{
       if(isUnlikeSignPair(pair) && pair->getNbDMdcHit()==2)
       {
	   return pair->getNbDRichHit();

       }
       else return -1;
}
Bool_t HRichCut::isGEANTPair(HDiHitMatchSim* pair, HLinearCategory* cat,
			     Char_t* opt)
{
        Bool_t isPair=kFALSE;

	// retrieve tracks that formed pair
	HHitMatchSim *track1 = (HHitMatchSim*)cat->getObject(pair->getIndTrk1());
	HHitMatchSim *track2 = (HHitMatchSim*)cat->getObject(pair->getIndTrk2());
	
	// retrieve GEANT information within tracks
	HTrackInfo *trackG1 = track1->getTrackInfoObj();
	HTrackInfo *trackG2 = track2->getTrackInfoObj();

	Int_t mech=-1;
	TString mechanism(opt);
	if (!mechanism.CompareTo("Conversion")) mech=6;
	else if (!mechanism.CompareTo("pi0Dalitz")) mech=5;

	// retrieve non-GEANT track information
	Int_t trackcorr1 = pair->getCorrCode1();
	Int_t trackcorr2 = pair->getCorrCode2();
	
	// retrieve pair information

	// loop over different particles in tracks
	for(Int_t i=0;i<MAXPARTICLES;i++)
	{
	    // track 1
	    Int_t nPart1  = trackG1->getPartNr();
	    Int_t nTrkNb1 = trackG1->getTrkNb(i);
	    if (nTrkNb1<=-1) break; 
	    Int_t parID1  = trackG1->getParId(i);
	    Int_t mech1   = trackG1->getMech(i);
	    Int_t med1    = trackG1->getMed(i);
	    Int_t creaID1 = trackG1->getCreaId(i);
	    Int_t cTrkNb1 = trackG1->getCreaTrkNb(i);
	    Int_t corr1   = trackG1->calcCorrCode(i);
	    Int_t srcLep1 = -1;
	    if (!mechanism.CompareTo("Conversion")) 
		srcLep1=trackG1->getConvLep(i);
	    else if(!mechanism.CompareTo("pi0Dalitz")) 
		srcLep1=trackG1->getPi0Dalitz(i);

	    // loop over particles in 2nd track
	    for(Int_t j=0;j<MAXPARTICLES;j++)
	    {
		
		Int_t nPart2  = trackG2->getPartNr();
		Int_t nTrkNb2 = trackG2->getTrkNb(j);
		if (nTrkNb2<=-1) break;
		Int_t parID2  = trackG2->getParId(j);
		Int_t mech2   = trackG2->getMech(j);
		Int_t med2    = trackG2->getMed(j);
		Int_t creaID2 = trackG2->getCreaId(j);
		Int_t cTrkNb2 = trackG2->getCreaTrkNb(j);
		Int_t corr2   = trackG2->calcCorrCode(j);
		Int_t srcLep2 = -1;
		if (!mechanism.CompareTo("Conversion")) 
		    srcLep2=trackG2->getConvLep(j);
		else if(!mechanism.CompareTo("pi0Dalitz")) 
		    srcLep2=trackG2->getPi0Dalitz(j);


		// cut on pair !!!!

		if (nTrkNb1!=nTrkNb2 && // diff particle in diff tracks
		    srcLep1==1 && srcLep2==1 && //conversion or pi0Dalitz
		    creaID1==creaID2 && cTrkNb1==cTrkNb2 && // same parent
		    mech1==mech && // 
		    mech1==mech2 && med1==med2 && // same crea mech and medium 
		    corr1==7 && corr2==7 && // both particles seen by 
		                            // all detectors in GEANT
		    nPart1==1 && nPart2==1 && // number of particles per track 
		    (parID1==2 || parID1==3) && (parID1+parID2==5) &&
		    trackcorr1==7 && trackcorr2==7) // both particles seen by all
		                                    // real detectors
		{

		    isPair=kTRUE;
		
		}
	    }
	}
	return isPair;	
}


Bool_t HRichCut::isUnlikeSignPair(HDiHitMatch* pair)
{
    Bool_t isPair=kFALSE;
    
    if (pair->getCharge()==0) isPair=kTRUE; 

    return isPair;


}

Bool_t HRichCut::isNew2Tuple(Int_t i,Int_t j,Int_t *tuple,Int_t max)
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

Bool_t HRichCut::isNewIndex(Int_t i,Int_t* iarr,Int_t max)
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



Bool_t HRichCut::isGoodRing(HRichHitSim *r)
{
     Bool_t kTEST = kFALSE;

    Int_t nPatMatCut   = 200;
    Int_t nHouTraCut   = 20;
    Int_t nRingAmplCut = 100;
    Int_t nPadNrCut    = 5;
    Int_t nLocMax4Cut  = 3;

    
    if(
       r->getRingPatMat()    >= nPatMatCut   &&
       r->getRingHouTra()    >= nHouTraCut   &&
       r->getRingAmplitude() >= nRingAmplCut &&
       r->getRingPadNr()     >= nPadNrCut    &&
       r->getRingLocalMax4() >= nLocMax4Cut     
       ) 
    {
	    kTEST = kTRUE;
    }
    
    return kTEST;
}

Bool_t HRichCut::isGoodTrack(HHitMatchSim *t)
{
     Bool_t kTEST = kFALSE;

     Int_t kickind = t->getKickInd();
     if (kickind > -1) kTEST = kTRUE;

     return kTRUE;
}




