// $Id: hrichcuttracklet.cc,v 1.2 2008-05-09 16:16:19 halo Exp $
// Last update by Thomas Eberl: 04/04/16 13:34:31
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
     setEvtType(0);//is exp
}

Bool_t HRichCutTracklet::switchTo(const Char_t *s,Bool_t clear)
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
    else if (!state.CompareTo("shower")) 
    {
	kShower=1;
	ret_val=kTRUE;
    }
    else if (!state.CompareTo("mdcchi2")) 
    {
	kMdcChi2=1;
	ret_val=kTRUE;
    }
    else if (!state.CompareTo("betamom3s")) 
    {
	kBetaMom3s=1;
	ret_val=kTRUE;
    }
    else if (!state.CompareTo("pullmom3s")) 
    {
	kPullMom3s=1;
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
    else if (!state.CompareTo("betagt0d935")) 
    {
	kBetaGT=1;
	nBetaGT=0.935;
	ret_val=kTRUE; 
    }
    else if (!state.CompareTo("betalt1d2")) 
    {
	kBetaLT=1;
	nBetaLT=1.2;
	ret_val=kTRUE;
    }
    else if (!state.CompareTo("pulllt2")) 
    {
	kPull=1;
	nPull=2.;
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
    kTofGT=0;
    kTofLT=0;
    kPull=0;
    nPull=0.;
    kShower=0;
    kBetaMom3s=0;
    kPullMom3s=0;
    isExp = kTRUE;

}




Bool_t HRichCutTracklet::check(HHitMatch* h)
{
    // there must be a switch for every function
    if (

	(  !kAngularMatch    ||       isAngMatch(h)                  )  &&
	(  !kGoodRing        ||       isGoodRing(h)                  )  &&
	(  !kSector          ||       isSector(h)                    )  &&
	(  !kBetaGT          ||       isBetaGT(h)                    )  &&
	(  !kBetaLT          ||       isBetaLT(h)                    )  &&
	(  !kTofGT           ||       isTofGT(h)                     )  &&
	(  !kTofLT           ||       isTofLT(h)                     )  &&
	(  !kPull            ||       isPull(h)                      )  &&
	(  !kShower          ||       isShowerCond(h)                )  &&
	(  !kMdcChi2         ||       isMdcChi2(h)                   )  &&
	(  !kBetaMom3s       ||       isBetaMom3s(h)                 )  &&
	(  !kPullMom3s       ||       isPullMom3s(h)                 )  &&
	(  !kSys             ||       isSys(h)                       )  &&
	(  !kEle             ||       isEle(h)                       )  &&
	(  !kPos             ||       isPos(h)                       )

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
	nRichMdcTheta =   10.;
	nRichMdcPhi   =   10.;
	nBetaGT       =   0.8;
	nBetaLT       =   1.2;
	nTofGT        =   5.0;
	nTofLT        =   9.0;
	nPull         =   2.;

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
	kPull          = 0;
	kSys           = 0;
	kEle           = 0;
	kPos           = 0;
	kShower        = 0;
	kMdcChi2       = 0;
	kBetaMom3s     = 0;
	kPullMom3s     = 0;

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

    nRichMdcTheta=10;
    nRichMdcPhi=10;
    isExp = kTRUE;
}

void HRichCutTracklet::setSector(Int_t i)
{
    kSector = 1;
    nSector = i;
    isExp = kTRUE;
}

void HRichCutTracklet::printCutList(Char_t *s) {
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
Bool_t HRichCutTracklet::isPull(HHitMatch *h)
{
    Bool_t ret_val=kFALSE;
    Float_t p  = h->getKickPull();
    if (

	(!kPull || p < nPull)

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
Bool_t HRichCutTracklet::isMdcChi2(HHitMatch *h)
{
    Bool_t ret_val=kFALSE;
    Float_t chi2 = h->getMdcClusterSize();
    // how nice to have meaningful names
    if (

	(!kMdcChi2 || chi2 > 0.)

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

Bool_t HRichCutTracklet::isShowerCond(HHitMatch *h)
{

    // check only in case of SHOWER was hit !
    if (h->getShowInd() < 0) return kTRUE;

    Float_t fF10 = h->getShowerfSum1()/h->getShowerfSum0();
    Float_t fF20 = h->getShowerfSum2()/h->getShowerfSum0();

    Float_t p = h->getKickMom();
    // Parameter and formula provided by J. Otwinowski, 2003-07-04
    Double_t pPar1[]={-0.011, 0.00138, -0.000001027, 0.000000000266};
    Double_t pPar2[]={-0.1057, 0.001178, -0.0000005453, 0.0000000000893};
    
    Float_t fThr10 = 4.0 * (pPar1[0]+pPar1[1]*p+pPar1[2]*p*p+pPar1[3]*p*p*p);
    Float_t fThr20 = 4.0 * (pPar2[0]+pPar2[1]*p+pPar2[2]*p*p+pPar2[3]*p*p*p);

    if( fF10 <= fThr10 && fF20 <= fThr20 ) return kFALSE;
    else return kTRUE; 
}

Bool_t HRichCutTracklet::isBetaMom3s(HHitMatch *h)
{

    Bool_t ret_val = kFALSE;
    Float_t *bl0p=0;
    Float_t *bl0m=0;
    Float_t *bl1p=0;
    Float_t *bl1m=0;
    Float_t *bh1p=0;
    Float_t *bh1m=0;
    // TOFINO resolution is 0,15c, TOF is 0,03 roughly
    Float_t ebl0p[]={0.83,  0.815, 0.81,  0.81,  0.81,  0.808};//+TOFINO
    Float_t ebl0m[]={0.82,  0.815, 0.805, 0.8,   0.798, 0.798};//-TOFINO
    Float_t ebl1p[]={0.89,  0.9,   0.89,  0.88,  0.885, 0.89 };//+TOF
    Float_t ebl1m[]={0.88,  0.9,   0.9,   0.89,  0.89,  0.89 };//-TOF
    Float_t ebh1p[]={1.155, 1.145, 1.145, 1.15,  1.145, 1.145};//+TOF
    Float_t ebh1m[]={1.155, 1.145, 1.145, 1.15,  1.145, 1.15 };//-TOF

    Float_t sbl0p[]={0.82,  0.83,  0.835, 0.835, 0.835, 0.835};//+TOFINO
    Float_t sbl0m[]={0.825, 0.815, 0.82,  0.83,  0.84,  0.84 };//-TOFINO
    Float_t sbl1p[]={0.94,  0.94,  0.94,  0.94,  0.94,  0.935};//+TOF
    Float_t sbl1m[]={0.945, 0.942, 0.94,  0.94,  0.94,  0.94 };//-TOF
    Float_t sbh1p[]={1.09,  1.085, 1.08,  1.08,  1.08,  1.1  };//+TOF
    Float_t sbh1m[]={1.1,   1.09,  1.09,  1.08,  1.08,  1.075};//-TOF

    if (evtType==0)//exp
    {
	//cout<<"is exp"<<endl;
	bl0p = ebl0p;
	bl0m = ebl0m;
	bl1p = ebl1p;
	bl1m = ebl1m;
	bh1p = ebh1p;
	bh1m = ebh1m;

    }
    else if (evtType==1)//sim
    {
	//cout<<"is sim"<<endl;
	bl0p = sbl0p;
	bl0m = sbl0m;
	bl1p = sbl1p;
	bl1m = sbl1m;
	bh1p = sbh1p;
	bh1m = sbh1m;

	
    }
    else cout<<"Evt type not set for cut"<<endl;

    Float_t mom = h->getKickMom();
    Float_t ch  = h->getKickCharge();
    Float_t beta= h->getKickBeta();
    Int_t i = -1;
    if (mom<=100) i=0; 
    else if (mom<=200) i=1; 
    else if (mom<=300) i=2; 
    else if (mom<=400) i=3; 
    else if (mom<=500) i=4; 
    else if (mom<=600) i=5; 
    else i=5;
    Int_t s,t;
    s=t=-1;
    s=h->getShowInd();
    t=h->getTofInd();
    Int_t lSys=-1; 
    if (s>-1) lSys = 0;
    else if (t>-1) lSys = 1;

    if (i!=-1)
    {
	if (lSys==0)
	{
	    if (ch>0 && beta>bl0p[i]) ret_val=kTRUE;
	    else if (ch<0 && beta>bl0m[i]) ret_val=kTRUE;
	}
	else if (lSys==1)
	{
	    //cout<<i<<"  "<<bl1p[i]<<"  "<<bh1p[i]<<"  "<<beta<<endl;
	    if (ch>0 && beta>bl1p[i] && beta<bh1p[i]) ret_val=kTRUE;
	    else if (ch<0 && beta>bl1m[i] && beta<bh1m[i]) ret_val=kTRUE;
	}
	else cout<<"no valid system in mombeta3s cut determined !"<<endl;
    }
    else cout<<"no valid mombeta index assigned !"<<endl;

//     delete [] bl0p;
//     delete [] bl0m;
//     delete [] bl1p;
//     delete [] bl1m;
//     delete [] bh1p;
//     delete [] bh1m;

    return ret_val;
}

Bool_t HRichCutTracklet::isPullMom3s(HHitMatch *h)
{

    Bool_t ret_val = kFALSE;
    Float_t *bl0p=0;
    Float_t *bl0m=0;
    Float_t *bl1p=0;
    Float_t *bl1m=0;
    Float_t *bh0p=0;
    Float_t *bh0m=0;
    Float_t *bh1p=0;
    Float_t *bh1m=0;

    Float_t sbl1m[]={-8.5, -6.5, -5.5, -5.5, -5.,  -5. };//-TOF
    Float_t sbh1m[]={ 8.5,  6.5,  5.5,  5.5,  5.,   5.  };//-TOF

    Float_t sbl0m[]={-6.6, -5.8, -5.3, -5.,  -4.8, -4.5};//-TOFINO
    Float_t sbh0m[]={ 6.,   5.8,  5.3,  5.,   4.8,  4.5 };//-TOFINO

    Float_t sbl1p[]={-7.5, -6.,  -5.5, -5.,  -5.,  -5. };//+TOF
    Float_t sbh1p[]={15.,  12.,  11.,  10.5, 10.5, 10.5};//+TOF

    Float_t sbl0p[]={-9.,  -6.5, -5.5, -5.,  -4.8, -4.5};//+TOFINO
    Float_t sbh0p[]={18.5, 13.,  11.,  10.5, 10.,   9.7 };//+TOFINO

    if (evtType==0)//exp
    {
	// use sim for exp as well
	//cout<<"is exp"<<endl;
	bl0p = sbl0p;
	bl0m = sbl0m;
	bl1p = sbl1p;
	bl1m = sbl1m;
	bh0p = sbh0p;
	bh0m = sbh0m;
	bh1p = sbh1p;
	bh1m = sbh1m;

    }
    else if (evtType==1)//sim
    {

	bl0p = sbl0p;
	bl0m = sbl0m;
	bl1p = sbl1p;
	bl1m = sbl1m;
	bh0p = sbh0p;
	bh0m = sbh0m;
	bh1p = sbh1p;
	bh1m = sbh1m;
    }
    else cout<<"Evt type not set for cut"<<endl;

    Float_t mom = h->getKickMom();
    Float_t ch  = h->getKickCharge();
    Float_t pull= h->getKickPull();
    Int_t i = -1;
    if (mom<=100) i=0; 
    else if (mom<=200) i=1; 
    else if (mom<=300) i=2; 
    else if (mom<=400) i=3; 
    else if (mom<=500) i=4; 
    else if (mom<=600) i=5; 
    else i=5;
    Int_t s,t;
    s=t=-1;
    s=h->getShowInd();
    t=h->getTofInd();
    Int_t lSys=-1; 
    if (s>-1) lSys = 0;
    else if (t>-1) lSys = 1;

    if (i!=-1)
    {
	if (lSys==0)
	{
	    if (ch>0 && pull>bl0p[i] && pull<bh0p[i]) ret_val=kTRUE;
	    else if (ch<0 && pull>bl0m[i] && pull<bh0m[i]) ret_val=kTRUE;
	}
	else if (lSys==1)
	{
	    //cout<<i<<"  "<<bl1p[i]<<"  "<<bh1p[i]<<"  "<<beta<<endl;
	    if (ch>0 && pull>bl1p[i] && pull<bh1p[i]) ret_val=kTRUE;
	    else if (ch<0 && pull>bl1m[i] && pull<bh1m[i]) ret_val=kTRUE;
	}
	else cout<<"no valid system in mombeta3s cut determined !"<<endl;
    }
    else cout<<"no valid mombeta index assigned !"<<endl;

    return ret_val;
}

ClassImp(HRichCutTracklet)
