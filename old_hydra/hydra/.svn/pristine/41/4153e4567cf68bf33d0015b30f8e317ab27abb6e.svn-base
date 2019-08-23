// $Id: hrichcuttracklet.cc,v 1.3 2003-04-04 13:13:42 eberl Exp $
// Last update by Thomas Eberl: 03/03/13 09:56:38
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
	(  !kBetaLT                      ||       isBetaLT(h)                    )

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

	kGoodRing      = 0;
	kAngularMatch  = 0;
	kRingPadNr     = 0;
	kRingAvChrg    = 0;
	kRingCentroid  = 0;
	kRingPatMat    = 0;
	kRichMdcTheta  = 0;
	kRichMdcPhi    = 0;
	kSector        = 0;
	kBetaGT        = 1;
	kBetaLT        = 1;

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
ClassImp(HRichCutTracklet)
