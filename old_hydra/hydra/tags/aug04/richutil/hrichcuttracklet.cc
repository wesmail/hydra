// $Id: hrichcuttracklet.cc,v 1.5 2003-05-05 08:22:53 eberl Exp $
// Last update by Thomas Eberl: 03/05/05 10:13:29
//

using namespace std;
#include <TObject.h>
#include <TRandom.h>
#include <time.h>
#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <math.h>
#include <TMath.h>
#include <TDatime.h>
#include "hhitmatch.h"
#include "phyanadef.h"
#include "hrichcuttracklet.h"
#include "hrichcuto.h"
#include "hkicktrack.h"
#include "kickdef.h"
#include "hparticle.h"
#include "hrichhit.h"
#include "hades.h"
#include "hiterator.h"
#include "hcategory.h"
#include "hphysicsconstants.h"
#include "hrecevent.h"
#include "hlocation.h"

HRichCutTracklet::HRichCutTracklet() : HRichCutO() 
{
}

HRichCutTracklet::HRichCutTracklet(Text_t *name, Text_t *title)
         : HRichCutO(name, title) 
{
    isExp=kFALSE;
    setStandardCuts();
    TDatime dt;
    dt.Set();

     TString stitle(title);
     stitle.Append("_");
     stitle+=dt.GetDate();
     stitle.Append("_");
     stitle+=dt.GetTime();
     SetTitle(stitle.Data());
}

Bool_t HRichCutTracklet::switchTo(const char *s,Bool_t clear)
{
    TString state(s);
    Bool_t ret_val = kFALSE;
    if (clear) reset();
    if (state.Contains("."))
    {
	listCut=kTRUE;
	Int_t len = state.Length();
	TString tmp; tmp="";
	for (Int_t i=0;i<len;i++)
	{
	    TString st(state[i]);
	    if (!st.CompareTo("."))
	    { 
		tmp.Remove(0,1);
		ret_val = switchTo(tmp.Data(),kFALSE);
		if (!ret_val) return ret_val;
		tmp="";
	    }
	    else
	    {
		tmp.Append(st);
	    }
	    if (i==len-1) 
	    {
		tmp.Remove(0,1);
		ret_val = switchTo(tmp.Data(),kFALSE);
		if (!ret_val) return ret_val;
	    }
	}
	return ret_val;
    }
    
    // compare to available inner states of the cut
    // and to switch to state
    // isExp is set in setState functions
    if (!state.CompareTo("matchedGoodRing"))
    {
	setMatchedGoodRing();
	ret_val=kTRUE;
    } 
    else if (!state.CompareTo("angularMatch")) 
    {
	setAngularMatch();
	ret_val=kTRUE;
    }
    else if (!state.CompareTo("goodRing")) 
    {
	//	cout<<"goodRing"<<endl;
	setGoodRing();
	ret_val=kTRUE;
    }
    else if (!state.CompareTo("tofgt5d0")) 
    {
	kTofGT=1;
	nTofGT=5.0;
	ret_val=kTRUE;
    }
    else if (!state.CompareTo("toflt9d0")) 
    {
	kTofLT=1;
	nTofLT=9.0;
	ret_val=kTRUE;
    }
    else if (!state.CompareTo("betagt0d8")) 
    {
	kBetaGT=1;
	nBetaGT=0.8;
	ret_val=kTRUE; 
    }
    else if (!state.CompareTo("betalt1d2")) 
    {
	kBetaLT=1;
	nBetaLT=1.2;
	ret_val=kTRUE;
    }
    else if (state.Contains("sec")) 
    {
	state.Remove(0,3);
	setSector(atoi(state.Data()));
	ret_val=kTRUE;
    }
    else if (state.Contains("sys")) 
    {
	kSys = 1;
	if (!state.CompareTo("sys0")) nSys=0;
	else if (!state.CompareTo("sys1")) nSys=1;
	ret_val=kTRUE;
    }
    else if (!state.CompareTo("electron")) 
    {
	kEle=1;
	ret_val=kTRUE;
    }
    else if (!state.CompareTo("positron")) 
    {
	kPos=1;
	ret_val=kTRUE;
    }
    else if (!state.CompareTo("nocuttracklet"))
    {
      ret_val=kTRUE;
	//	isExp = kTRUE;
    }
    else 
    {
	cout<<"requested state "<<state.Data()<<" for cut: "
	    <<this->GetName()<<" not found"<<endl;
	Warning("switchTo","requested cut keyword not found");
	ret_val=kFALSE;
    }
    return ret_val;
}

void HRichCutTracklet::setStandardCuts()
{

    reset();
    setAngularMatch();
    setGoodRing();
    nBetaGT=0.8;
    nBetaLT=1.2;
    kBetaGT=1;
    kBetaLT=1;
    nTofGT=0.8;
    nTofLT=1.2;
    kTofGT=1;
    kTofLT=1;
    isExp = kTRUE;

}




Bool_t HRichCutTracklet::check(HHitMatch* h)
{
    // there must be a switch for every function
    if (

	(  !kAngularMatch                ||       isAngMatch(h)                  )  &&
	(  !kGoodRing                    ||       isGoodRing(h)                  )  &&
	(  !kSector                      ||       isSector(h)                    )  &&
	(  !kBetaGT                      ||       isBetaGT(h)                    )  &&
	(  !kBetaLT                      ||       isBetaLT(h)                    )  &&
	(  !kTofGT                       ||       isTofGT(h)                     )  &&
	(  !kTofLT                       ||       isTofLT(h)                     )  &&
	(  !kSys                         ||       isSys(h)                       )  &&
	(  !kEle                         ||       isEle(h)                       )  &&
	(  !kPos                         ||       isPos(h)                       )

       ) return kTRUE;


    else return kFALSE;

}

void HRichCutTracklet::reset() 
{
    // set to standard values, but all inactive !
 
	nRingPadNr    =   4.;
	nRingAvChrg   =   5.;
	nRingCentroid =   2.8;
	nRingPatMat   =   200;
	nRichMdcTheta =   1.7;
	nRichMdcPhi   =   1.8;
	nBetaGT       =   0.8;
	nBetaLT       =   1.2;
	nTofGT        =   5.0;
	nTofLT        =   9.0;

	kGoodRing      = 0;
	kAngularMatch  = 0;
	kRingPadNr     = 0;
	kRingAvChrg    = 0;
	kRingCentroid  = 0;
	kRingPatMat    = 0;
	kRichMdcTheta  = 0;
	kRichMdcPhi    = 0;
	kSector        = 0;
	kBetaGT        = 0;
	kBetaLT        = 0;
	kTofGT         = 0;
	kTofLT         = 0;
	kSys           = 0;
	kEle           = 0;
	kPos           = 0;

	isExp = kFALSE;
	listCut = kFALSE;

}

void HRichCutTracklet::setMatchedGoodRing()
{
    setGoodRing();
    setAngularMatch();
}

void HRichCutTracklet::setGoodRing()
{
    kGoodRing      = 1;
    kRingPadNr     = 1;
    kRingAvChrg    = 1;
    kRingCentroid  = 1;
    kRingPatMat    = 1;

    nRingPadNr    =   4.;
    nRingAvChrg   =   5.;
    nRingCentroid =   2.8;
    nRingPatMat   =   200;
    isExp = kTRUE;
}

void HRichCutTracklet::setAngularMatch()
{
    kAngularMatch=1;
    kRichMdcTheta=1;
    kRichMdcPhi=1;

    nRichMdcTheta=1.7;
    nRichMdcPhi=1.8;
    isExp = kTRUE;
}

void HRichCutTracklet::setSector(Int_t i)
{
    kSector = 1;
    nSector = i;
    isExp = kTRUE;
}

void HRichCutTracklet::printCutList(char *s) {
// Prints a table of enabled cuts to stdio
    if (s) switchTo(s);
    else setStandardCuts();
  printf("\n-----------------------------------------------\n");
  printf("               HRichCutTracklet cuts               \n");
  printf("               ---------------------               \n");

  if(kGoodRing)     printf("cut on Good Ring active\n"); 
  if(kAngularMatch) printf("cut on RICH_MDC matching active\n"); 
  if(kRingPadNr)    printf(" nRingPadNr > %4.1f\n",   nRingPadNr);
  if(kRingAvChrg)   printf(" nRingAvChrg > %4.1f\n",  nRingAvChrg);
  if(kRingCentroid) printf(" nRingCentroid > %4.1f\n",nRingCentroid);
  if(kRingPatMat)   printf(" nRingPatMat > %4.1f\n",  nRingPatMat);
  if(kRichMdcTheta) printf(" nRichMdcTheta < %4.1f\n",nRichMdcTheta);
  if(kRichMdcPhi)   printf(" nRichMdcPhi < %4.1f\n",  nRichMdcPhi);

  printf("----------------------------------------------\n\n");
  return;
}
void HRichCutTracklet::printCutList() {
// Prints a table of enabled cuts to stdio
  printf("\n-----------------------------------------------\n");
  printf("               HRichCutTracklet cuts               \n");
  printf("               ---------------------               \n");

  if(kGoodRing)     printf("cut on Good Ring active\n"); 
  if(kAngularMatch) printf("cut on RICH_MDC matching active\n"); 
  if(kRingPadNr)    printf(" nRingPadNr > %4.1f\n",   nRingPadNr);
  if(kRingAvChrg)   printf(" nRingAvChrg > %4.1f\n",  nRingAvChrg);
  if(kRingCentroid) printf(" nRingCentroid > %4.1f\n",nRingCentroid);
  if(kRingPatMat)   printf(" nRingPatMat > %4.1f\n",  nRingPatMat);
  if(kRichMdcTheta) printf(" nRichMdcTheta < %4.1f\n",nRichMdcTheta);
  if(kRichMdcPhi)   printf(" nRichMdcPhi < %4.1f\n",  nRichMdcPhi);

  printf("----------------------------------------------\n\n");

}

Bool_t HRichCutTracklet::isAngMatch(HHitMatch *h)
{
    // solid angle cut !
   Bool_t ret_val=kFALSE;
    if (
	
	(!kRichMdcTheta || TMath::Abs(h->getRichTheta() - h->getMdcTheta()) < nRichMdcTheta) &&
	(!kRichMdcPhi   || TMath::Abs(h->getRichPhi()   - h->getMdcPhi())*
	 TMath::Sin(TMath::DegToRad()*h->getMdcTheta())   < nRichMdcPhi) 
	
	) ret_val=kTRUE;

    return ret_val;


}
Bool_t HRichCutTracklet::isGoodRing(HHitMatch *h) 
{
    Bool_t ret_val=kFALSE;
    if (

	(!kRingPatMat   || h->getRingPatMat()                                            > nRingPatMat)    &&
	(!kRingPadNr    || h->getRingPadNr()                                             > nRingPadNr)     &&
	(!kRingCentroid || h->getCentroid()                                              < nRingCentroid)  &&
	(!kRingAvChrg   || ((Float_t)h->getRingAmplitude())/((Float_t)h->getRingPadNr()) > nRingAvChrg)    
	
	) ret_val=kTRUE;

    return ret_val;

}

Bool_t HRichCutTracklet::isSector(HHitMatch *h)
{
    Bool_t ret_val=kFALSE;
    if (

	(!kSector || h->getSector() == nSector)

	) ret_val=kTRUE;

    return ret_val;
}
Bool_t HRichCutTracklet::isSys(HHitMatch *h)
{
    Bool_t ret_val=kFALSE;
    Int_t s,t;
    s=t=-1;
    s=h->getShowInd();
    t=h->getTofInd();
    Int_t lSys=-1; 
    if (s>-1) lSys = 0;
    else if (t>-1) lSys = 1;
    else Error("HRichCutTracklet::isSys","no system in kicktrack");
    if (

	(!kSys || lSys == nSys)

	) ret_val=kTRUE;

    return ret_val;
}
Bool_t HRichCutTracklet::isEle(HHitMatch *h)
{
    Bool_t ret_val=kFALSE;
    Int_t c = h->getKickCharge();
    if (

	(!kEle || c == -1)

	) ret_val=kTRUE;

    return ret_val;
}
Bool_t HRichCutTracklet::isPos(HHitMatch *h)
{
    Bool_t ret_val=kFALSE;
    Int_t c = h->getKickCharge();
    if (

	(!kPos || c == 1)

	) ret_val=kTRUE;

    return ret_val;
}
Bool_t HRichCutTracklet::isBetaGT(HHitMatch *h)
{
    Bool_t ret_val=kFALSE;
    if (

	(!kBetaGT || h->getKickBeta() > nBetaGT)

	) ret_val=kTRUE;

    return ret_val;

}
Bool_t HRichCutTracklet::isBetaLT(HHitMatch *h)
{
    Bool_t ret_val=kFALSE;
    if (

	(!kBetaLT || h->getKickBeta() < nBetaLT)

	) ret_val=kTRUE;

    return ret_val;

}
Bool_t HRichCutTracklet::isTofGT(HHitMatch *h)
{
    Bool_t ret_val=kFALSE;
    Float_t tof    = h->getTofTof(); 
    Float_t tofino = h->getTofinoTof();
    Float_t tmp = 0.;
    if (tof>0.) tmp = tof;
    else if (tofino>0.) tmp = tofino;
    if (

	(!kTofGT || tmp > nTofGT)

	) ret_val=kTRUE;

    return ret_val;

}
Bool_t HRichCutTracklet::isTofLT(HHitMatch *h)
{
    Bool_t ret_val=kFALSE;
    Float_t tof    = h->getTofTof(); 
    Float_t tofino = h->getTofinoTof();
    Float_t tmp = 0.;
    if (tof>0.) tmp = tof;
    else if (tofino>0.) tmp = tofino;
    if (

	(!kTofLT || tmp < nTofLT)

	) ret_val=kTRUE;

    return ret_val;

}
ClassImp(HRichCutTracklet)
