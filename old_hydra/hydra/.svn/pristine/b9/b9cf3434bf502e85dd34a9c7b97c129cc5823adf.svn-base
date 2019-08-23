// $Id: hrichcutpairsim.cc,v 1.6 2008-05-09 16:16:32 halo Exp $
// Last update by Thomas Eberl: 04/07/12 18:19:21
//

using namespace std;
#include <TObject.h>
#include <TRandom.h>
#include <time.h>
#include <iostream>
#include <iomanip>
#include <math.h>
#include <TMath.h>
#include <TDatime.h>
#include "hdihitmatchsim.h"
#include "hhitmatchsim.h"
#include "phyanadef.h"
#include "hrichcutpairsim.h"
#include "hrichcuto.h"
#include "hkicktrack.h"
#include "kickdef.h"
#include "hlinearcategory.h"
#include "hrichutilfunc.h"
#include "richdef.h"
#include "htrackinfo.h"
#include "hparticle.h"
#include "hrichhit.h"
#include "hades.h"
#include "hiterator.h"
#include "hcategory.h"
#include "hphysicsconstants.h"
#include "hrecevent.h"
#include "hlocation.h"

HRichCutPairSim::HRichCutPairSim() : HRichCutPair() 
{
}

HRichCutPairSim::HRichCutPairSim(Text_t *name, Text_t *title)
         : HRichCutPair(name, title) 
{
    isExp=kFALSE;
    isSim=kFALSE;
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

Bool_t HRichCutPairSim::switchTo(const Char_t *s,Bool_t clear)
{
    //cout<<"RichCutPairSim::switchTo: "<<s<<endl;
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
		if (!(ret_val = switchTo(tmp.Data(),kFALSE))) return ret_val;
		tmp="";
	    }
	    else
	    {
		tmp.Append(st);
	    }
	    if (i==len-1) 
	    {
		tmp.Remove(0,1);
		if (!(ret_val = switchTo(tmp.Data(),kFALSE))) return ret_val;
	    }
	}
	return ret_val;
    }
    
    // compare to available inner states of the cut
    // and to switch to state
    if (!state.CompareTo("isConvPair")) 
    {
	kisConvPair = 1;
	isSim= kTRUE;
	ret_val = kTRUE;
	//cout<<"isConvPair"<<endl;
    }
    else if (!state.CompareTo("isFakePair")) 
    {
	kisFakePair = 1;
	ret_val = kTRUE;
	isSim= kTRUE;
	//cout<<"isDalitzPair"<<endl;
    }
    else if (!state.CompareTo("isPhysCBPair")) 
    {
	kisPhysCBPair = 1;
	ret_val = kTRUE;
	isSim= kTRUE;
	//cout<<"isDalitzPair"<<endl;
    }
    else if (!state.CompareTo("isDalitzPair")) 
    {
	kisDalitzPair = 1;
	ret_val = kTRUE;
	isSim= kTRUE;
	//cout<<"isDalitzPair"<<endl;
    }
    else if (!state.CompareTo("isEtaDalitzPair")) 
    {
	kisEtaDalitzPair = 1;
	ret_val = kTRUE;
	isSim= kTRUE;
	//cout<<"isDalitzPair"<<endl;
    }
    else if (!state.CompareTo("isAllCombiPair")) 
    {
	kisAllCombiPair = 1;
	ret_val = kTRUE;
	isSim= kTRUE;
	//cout<<"isDalitzPair"<<endl;
    }
    else if (!state.CompareTo("isLepCombiPair")) 
    {
	kisLepCombiPair = 1;
	ret_val = kTRUE;
	isSim= kTRUE;
	//cout<<"isDalitzPair"<<endl;
    }
    else if (!state.CompareTo("nocutpairsim"))
    {
	isSim= kFALSE;
	ret_val = kTRUE;
    }
    else 
    {
	isExp = HRichCutPair::switchTo(s,clear);

	if (!isExp) 
	{
	    //cout<<"requested state "<<state.Data()<<" for cut: "
	    //	<<this->GetName()<<" not found"<<endl;
	    //Error("switchTo()","no valid cut found");
	}
	ret_val = isExp;
	
    }

    return ret_val;
}

void HRichCutPairSim::setStandardCuts()
{

    reset();
    kisFakePair = 0;
    kisConvPair = 0;
    kisPhysCBPair = 0;
    kisDalitzPair = 0;
    kisAllCombiPair = 0;
    kisLepCombiPair = 0;
    kisEtaDalitzPair=0;
}

void HRichCutPairSim::reset() 
{
    HRichCutPair::reset();
    kisFakePair = 0;
    kisConvPair = 0;
    kisPhysCBPair = 0;
    kisDalitzPair = 0;
    kisAllCombiPair = 0;
    kisLepCombiPair = 0;
    kisEtaDalitzPair = 0;
}

void HRichCutPairSim::printCutList(Char_t* s) 
{

    if (s) switchTo(s);
    else {setStandardCuts(); HRichCutPair::setStandardCuts();}
    
    if (!isExp)
    {
	
	printf("\n-----------------------------------------------\n");
	printf("               HRichCutPairSim cuts          \n");
	printf("               ------------------------          \n");
	if (kisDalitzPair) printf("cut on pi0-Dalitz lepton pair active  \n");
	if (kisAllCombiPair) printf("cut on combinatorial pair active  \n");
	if (kisLepCombiPair) printf("cut on combinatorial lepton pair active  \n");
	if (kisConvPair)   printf("cut on pair conversion lepton pair (from pi0) active\n");
	if (kisFakePair)   printf("cut on pair fake lepton pair (from pi0) active\n");
	if (kisPhysCBPair)   printf("cut on pair physCB lepton pair (from pi0) active\n");
	printf("-------------------------------------------------\n\n");
    }
    else HRichCutPair::printCutList(s);
    return;
}

void HRichCutPairSim::printCutList() 
{
    printf("\n-----------------------------------------------\n");
    printf("               HRichCutPairSim cuts          \n");
    printf("               ------------------------          \n");
    if (kisDalitzPair) printf("cut on pi0-Dalitz lepton pair active  \n");
    if (kisAllCombiPair) printf("cut on combinatorial pair active  \n");
    if (kisLepCombiPair) printf("cut on combinatorial lepton pair active  \n");
    if (kisConvPair)   printf("cut on pair conversion lepton pair (from pi0) active\n");
    if (kisFakePair)   printf("cut on pair fake lepton pair (from pi0) active\n");
    if (kisPhysCBPair)   printf("cut on pair phys cb lepton pair (from pi0) active\n");
    printf("-------------------------------------------------\n\n");
    HRichCutPair::printCutList();
}


Bool_t HRichCutPairSim::check(HDiHitMatchSim* h)
{
    // there must be a switch for every function


    Bool_t ret_val=kFALSE;
    if (!listCut)
    {
	if ( !isExp ){ 
	    // the cut was switched to pure sim
	    if (  
		(!kisDalitzPair     ||       isDalitzPair(h)     )  &&
		(!kisFakePair       ||       isFakePair(h)     )  &&
		(!kisPhysCBPair       ||       isPhysCBPair(h)     )  &&
		(!kisEtaDalitzPair  ||       isEtaDalitzPair(h)     )  &&
		(!kisAllCombiPair   ||       isAllCombiPair(h)     )  &&
		(!kisLepCombiPair   ||       isLepCombiPair(h)     )  &&
		(!kisConvPair       ||       isConvPair(h)       )   
		
		) ret_val=kTRUE;
	    
	}
	// the sim cut was switched with a exp keyword
	// call the exp cut check function
	else if (isExp) ret_val=HRichCutPair::check(h);
    }
    else
    {
	if (isExp && isSim)
	{
	    if (  
		(!kisDalitzPair   ||       isDalitzPair(h)     )  &&
		(!kisFakePair   ||       isFakePair(h)     )  &&
		(!kisPhysCBPair   ||       isPhysCBPair(h)     )  &&
		(!kisEtaDalitzPair  ||       isEtaDalitzPair(h)     )  &&
		(!kisAllCombiPair   ||       isAllCombiPair(h)     )  &&
		(!kisLepCombiPair   ||       isLepCombiPair(h)     )  &&
		(!kisConvPair     ||       isConvPair(h)       )   
		) ret_val=kTRUE;
	    Bool_t ret_val_exp = HRichCutPair::check(h);
	    if (ret_val && ret_val_exp) ret_val = kTRUE;
	}
	
	else {listCut=kFALSE;ret_val = check(h);}
    }
    
    return ret_val;
}

Bool_t HRichCutPairSim::isConvPair(HDiHitMatchSim* h)
{
    Bool_t ret_val=kFALSE;
    HLinearCategory * cat = (HLinearCategory*) HRichUtilFunc::getCategory(catMatchHit);
    HHitMatchSim *t1 = 0;
    HHitMatchSim *t2 = 0;
    HTrackInfo * tt1 = 0;
    HTrackInfo * tt2 = 0;
    if (cat)
    {
	 t1 = (HHitMatchSim*)cat->getObject(h->getIndTrk1());
	 if (!t1) Error("isConvPair","HHitMatchSim object 1 not found");
	 t2 = (HHitMatchSim*)cat->getObject(h->getIndTrk2());
	 if (!t2) Error("isConvPair","HHitMatchSim object 2 not found");
    }
    else Error("isConvPair","no category found");
    if (t1) tt1 = t1->getTrackInfoObj();
    if (t2) tt2 = t2->getTrackInfoObj();

    if (tt1&&tt2)
    {
	for (Int_t i=0;i<MAXPARTICLES;i++)
	{
	    if (tt1->getConvLep(i)==1)
	    {
		Int_t c1 = tt1->calcCorrCode(i);
		// first particle is conv
		for (Int_t j=0;j<MAXPARTICLES;j++)
		{
		    Int_t c2 = tt2->calcCorrCode(j);
		    if (
			c1==4 && c2==4 &&
			tt2->getConvLep(j)==1 && // 2nd part is conv
			tt2->getParId(j)+tt1->getParId(i)==5 && // is e+ e-
			tt1->getCreaTrkNb(i)!=-1 &&
			tt2->getCreaTrkNb(j)!=-1 &&
			tt1->getCreaTrkNb(i)==tt2->getCreaTrkNb(j) &&// same mother
			tt1->getCreaId(i) == 1 // mother is gamma
			)
		    {
			ret_val = kTRUE;
			break;
		    }
		}
	    }
	}
    }
    else Error("isConvPair","HTrackInfo objects not found");

    return ret_val;
    
}


Bool_t HRichCutPairSim::isDalitzPair(HDiHitMatchSim* h)
{

    Bool_t ret_val=kFALSE;

HLinearCategory * cat = (HLinearCategory*) HRichUtilFunc::getCategory(catMatchHit);
    HHitMatchSim *t1 = 0;
    HHitMatchSim *t2 = 0;
    HTrackInfo * tt1 = 0;
    HTrackInfo * tt2 = 0;
    if (cat)
    {
	 t1 = (HHitMatchSim*)cat->getObject(h->getIndTrk1());
	 if (!t1) Error("isConvPair","HHitMatchSim object 1 not found");
	 t2 = (HHitMatchSim*)cat->getObject(h->getIndTrk2());
	 if (!t2) Error("isConvPair","HHitMatchSim object 2 not found");
    }
    else Error("isDalitzPair","no category found");
    if (t1) tt1 = t1->getTrackInfoObj();
    if (t2) tt2 = t2->getTrackInfoObj();

    if (tt1&&tt2)
    {

	for (Int_t i=0;i<MAXPARTICLES;i++)
	{
	    if (tt1->getPi0Dalitz(i)==1)
	    {
		Int_t c1 = tt1->calcCorrCode(i);
		// first particle is dalitz
		for (Int_t j=0;j<MAXPARTICLES;j++)
		{
		    Int_t c2 = tt2->calcCorrCode(j);
		    if (
			c1==4 && c2==4 &&
			tt2->getPi0Dalitz(j)==1 && // 2nd part is dalitz
			tt2->getParId(j)+tt1->getParId(i)==5 && // is e+ e-
			tt1->getCreaTrkNb(i)!=-1 &&
			tt2->getCreaTrkNb(j)!=-1 &&
			tt1->getCreaTrkNb(i)==tt2->getCreaTrkNb(j) &&// same mother
			tt1->getCreaId(i) == 7 // mother is pi0
			)
		    {
			ret_val = kTRUE;
			break;
		    }
		}
		
	    }
	}
    }
    else Error("isConvPair","HTrackInfo objects not found");
    
    return ret_val;
    
}
Bool_t HRichCutPairSim::isEtaDalitzPair(HDiHitMatchSim* h)
{

    Bool_t ret_val=kFALSE;

    HLinearCategory * cat = (HLinearCategory*) HRichUtilFunc::getCategory(catMatchHit);
    HHitMatchSim *t1 = 0;
    HHitMatchSim *t2 = 0;
    HTrackInfo * tt1 = 0;
    HTrackInfo * tt2 = 0;
    if (cat)
    {
	 t1 = (HHitMatchSim*)cat->getObject(h->getIndTrk1());
	 if (!t1) Error("isConvPair","HHitMatchSim object 1 not found");
	 t2 = (HHitMatchSim*)cat->getObject(h->getIndTrk2());
	 if (!t2) Error("isConvPair","HHitMatchSim object 2 not found");
    }
    else Error("isDalitzPair","no category found");
    if (t1) tt1 = t1->getTrackInfoObj();
    if (t2) tt2 = t2->getTrackInfoObj();

    if (tt1&&tt2)
    {

	for (Int_t i=0;i<MAXPARTICLES;i++)
	{
	    if (tt1->getCreaId(i) == 17)
	    {
		Int_t c1 = tt1->calcCorrCode(i);
		// first particle is eta dalitz
		for (Int_t j=0;j<MAXPARTICLES;j++)
		{
		    Int_t c2 = tt2->calcCorrCode(j);
		    if (
			c1==4 && c2==4 &&
			tt2->getParId(j)+tt1->getParId(i)==5 && // is e+ e-
			tt1->getCreaTrkNb(i)!=-1 &&
			tt2->getCreaTrkNb(j)!=-1 &&
			tt1->getCreaTrkNb(i)==tt2->getCreaTrkNb(j) &&// same mother
			tt2->getCreaId(j) == 17 // mother is pi0
			)
		    {
			ret_val = kTRUE;
			break;
		    }
		}
		
	    }
	}
    }
    else Error("isConvPair","HTrackInfo objects not found");
    
    return ret_val;
    
}

void HRichCutPairSim::getDalitzPairTrkNb(HDiHitMatchSim* h,Int_t &a, Int_t &b)
{


    HLinearCategory * cat = (HLinearCategory*) HRichUtilFunc::getCategory(catMatchHit);
    HHitMatchSim *t1 = 0;
    HHitMatchSim *t2 = 0;
    HTrackInfo * tt1 = 0;
    HTrackInfo * tt2 = 0;
    if (cat)
    {
	t1 = (HHitMatchSim*)cat->getObject(h->getIndTrk1());
	if (!t1) Error("isConvPair","HHitMatchSim object 1 not found");
	t2 = (HHitMatchSim*)cat->getObject(h->getIndTrk2());
	if (!t2) Error("isConvPair","HHitMatchSim object 2 not found");
    }
    else Error("isDalitzPair","no category found");
    if (t1) tt1 = t1->getTrackInfoObj();
    if (t2) tt2 = t2->getTrackInfoObj();
    
    if (tt1&&tt2)
    {
	
	for (Int_t i=0;i<MAXPARTICLES;i++)
	{
	    if (tt1->getPi0Dalitz(i)==1)
	    {
		Int_t c1 = tt1->calcCorrCode(i);
		// first particle is dalitz
		for (Int_t j=0;j<MAXPARTICLES;j++)
		{
		    Int_t c2 = tt2->calcCorrCode(j);
		    if (
			c1==4 && c2==4 &&
			tt2->getPi0Dalitz(j)==1 && // 2nd part is dalitz
			tt2->getParId(j)+tt1->getParId(i)==5 && // is e+ e-
			tt1->getCreaTrkNb(i)!=-1 &&
			tt2->getCreaTrkNb(j)!=-1 &&
			tt1->getCreaTrkNb(i)==tt2->getCreaTrkNb(j) &&// same mother
			tt1->getCreaId(i) == 7 // mother is pi0
			)
		    {
			a = tt1->getTrkNb(i);
			b = tt2->getTrkNb(j);
			break;
		    }
		}
		
	    }
	}
    }
    else Error("isConvPair","HTrackInfo objects not found");
    
    
}
void HRichCutPairSim::getConvPairTrkNb(HDiHitMatchSim* h,Int_t &a, Int_t &b)
{


    HLinearCategory * cat = (HLinearCategory*) HRichUtilFunc::getCategory(catMatchHit);
    HHitMatchSim *t1 = 0;
    HHitMatchSim *t2 = 0;
    HTrackInfo * tt1 = 0;
    HTrackInfo * tt2 = 0;
    if (cat)
    {
	t1 = (HHitMatchSim*)cat->getObject(h->getIndTrk1());
	if (!t1) Error("isConvPair","HHitMatchSim object 1 not found");
	t2 = (HHitMatchSim*)cat->getObject(h->getIndTrk2());
	if (!t2) Error("isConvPair","HHitMatchSim object 2 not found");
    }
    else Error("isDalitzPair","no category found");
    if (t1) tt1 = t1->getTrackInfoObj();
    if (t2) tt2 = t2->getTrackInfoObj();
    
    if (tt1&&tt2)
    {
	
	for (Int_t i=0;i<MAXPARTICLES;i++)
	{
	    if (tt1->getConvLep(i)==1)
	    {
		Int_t c1 = tt1->calcCorrCode(i);
		// first particle is conv
		for (Int_t j=0;j<MAXPARTICLES;j++)
		{
		    Int_t c2 = tt2->calcCorrCode(j);
		    if (
			c1==4 && c2==4 &&
			tt2->getConvLep(j)==1 && // 2nd part is dalitz
			tt2->getParId(j)+tt1->getParId(i)==5 && // is e+ e-
			tt1->getCreaTrkNb(i)!=-1 &&
			tt2->getCreaTrkNb(j)!=-1 &&
			tt1->getCreaTrkNb(i)==tt2->getCreaTrkNb(j) &&// same mother
			tt1->getCreaId(i) == 1 // mother is pi0
			)
		    {
			a = tt1->getTrkNb(i);
			b = tt2->getTrkNb(j);
			break;
		    }
		}
		
	    }
	}
    }
    else Error("isConvPair","HTrackInfo objects not found");
    
    
}

Bool_t HRichCutPairSim::isAllCombiPair(HDiHitMatchSim* h)
{
    HRichCutPair c("default","default");
    c.switchTo("2matchedGoodRing");c.switchTo("unlike",kFALSE);
    c.switchTo("2fast",kFALSE);
    if (h)
    {
	if (!c.check((HDiHitMatch*)h)) return kFALSE;
    }
    else Error("isCombiPair","HDiHitMatch object not found");

    Bool_t ret_val=kTRUE;

    HLinearCategory * cat = (HLinearCategory*) HRichUtilFunc::getCategory(catMatchHit);
    HHitMatchSim *t1 = 0;
    HHitMatchSim *t2 = 0;
    HTrackInfo * tt1 = 0;
    HTrackInfo * tt2 = 0;
    if (cat)
    {
	t1 = (HHitMatchSim*)cat->getObject(h->getIndTrk1());
	if (!t1) Error("isCombiPair","HHitMatchSim object 1 not found");
	t2 = (HHitMatchSim*)cat->getObject(h->getIndTrk2());
	if (!t2) Error("isCombiPair","HHitMatchSim object 2 not found");
    }
    else Error("isCombiPair","no category found");
    if (t1) tt1 = t1->getTrackInfoObj();
    if (t2) tt2 = t2->getTrackInfoObj();
    
    if (tt1&&tt2)
    {
	Int_t cc1=-1;
	Int_t cc2=-1;
	for (Int_t i=0;i<MAXPARTICLES;i++) 
	{
	    if (tt1->getTrkNb(i)!=-1 && tt1->calcCorrCode(i)==4) cc1=i;
	    if (tt2->getTrkNb(i)!=-1 && tt2->calcCorrCode(i)==4) cc2=i;
	}	    
	if ( cc1 != -1 && cc2 != -1) // two fully GEANT confirmed tracks
	{
	    //	    tt1->dumpToStdout();
	    //	    tt2->dumpToStdout();
	    if (tt1->getCreaTrkNb(cc1) == tt2->getCreaTrkNb(cc2) && //same parent
		tt1->getTrkNb(cc1)     != tt2->getTrkNb(cc2) )      //diff particles
	    {
// 		cout<<"///////////////////////////"<<endl;
// 		//tt1->dumpToStdout();
// 		t1->dumpToStdoutSim();
// 		//tt2->dumpToStdout();
// 		t2->dumpToStdoutSim();
// 		cout<<"///////////////////////////"<<endl;
		return kFALSE;

	    }
	} 
	// rest are combinations of tracks that do not contain fully-tracked
	// particles. Therefore they are combinations of two fake tracks or of a
	// good track with a fake track. These combinations add to the CB.



	//	cout<<tt1->getPartNr()<<"   "<<tt2->getPartNr()<<endl;
	//if (tt1->getPartNr()!=1 && tt2->getPartNr()!=1) return kFALSE;

	// 	if (h->getInvMass()<1) return kFALSE;

//  	for (Int_t i=0;i<MAXPARTICLES;i++)
//  	{
//  	    for (Int_t j=0;j<MAXPARTICLES;j++)
//  		{
// // 		    //if (tt1->getMatchedMT_S(i)==1 && tt2->getMatchedMT_S(j)==1) // one track is a fake kicktrack
// // 		    //{
//  			if(
//  			   tt1->getCreaTrkNb(i)==-1 ||
//  			   tt2->getCreaTrkNb(j)==-1
//  			   ) ret_val=kFALSE;
//  			else
//  			{
//  			    if(tt1->getCreaTrkNb(i)!=-1 && tt2->getCreaTrkNb(j)!=-1 && 
// 			       tt1->getCreaTrkNb(i)==tt2->getCreaTrkNb(j)) // same mother
//  			    {
//  				return kFALSE;
//  			    }
//  			    else if (tt1->getCreaTrkNb(i)!=-1 && tt2->getCreaTrkNb(j)!=-1 && 
// 				     tt1->getCreaTrkNb(i)!=tt2->getCreaTrkNb(j))
//  			    {
//  				//h->dumpToStdoutSim();
//  				//tt1->dumpToStdout();
//  				//tt2->dumpToStdout();
// 				return kTRUE;
// 				//if (tt1->getMatchedRMT_S(i)==1 && tt2->getMatchedRMT_S(j)==1) return kTRUE;
// 				//if (tt1->getMatchedRM(i)==1 && tt2->getMatchedRM(j)==1) return kTRUE;
// 				//				if (tt1->getParId(i)>3 || tt2->getParId(j)>3) return kFALSE;
// 				//else return kFALSE;
// // // 				else ret_val=kFALSE;// one track is a fake kicktrack
// // 				cout<<"&&&&&&&&&&&&&&&&&&&&&&&&&"<<endl;
// // 				tt1->dumpToStdout();
// // 				tt2->dumpToStdout();
// // 				cout<<"&&&&&&&&&&&&&&&&&&&&&&&&&"<<endl;

//  			    }
//  			}
			
//  		}
	    
//  	}
    }
    else Error("isCombiPair","HTrackInfo objects not found");
    return ret_val;
}

Bool_t HRichCutPairSim::isLepCombiPair(HDiHitMatchSim* h)
{

    Bool_t ret_val=kTRUE;

    HLinearCategory * cat = (HLinearCategory*) HRichUtilFunc::getCategory(catMatchHit);
    HHitMatchSim *t1 = 0;
    HHitMatchSim *t2 = 0;
    HTrackInfo * tt1 = 0;
    HTrackInfo * tt2 = 0;
    if (cat)
    {
	t1 = (HHitMatchSim*)cat->getObject(h->getIndTrk1());
	if (!t1) Error("isCombiPair","HHitMatchSim object 1 not found");
	t2 = (HHitMatchSim*)cat->getObject(h->getIndTrk2());
	if (!t2) Error("isCombiPair","HHitMatchSim object 2 not found");
    }
    else Error("isCombiPair","no category found");
    if (t1) tt1 = t1->getTrackInfoObj();
    if (t2) tt2 = t2->getTrackInfoObj();
    
    if (tt1&&tt2)
    {
	if(h->getInvMass()<1) return kFALSE;
	for (Int_t i=0;i<MAXPARTICLES;i++)
	{
	    for (Int_t j=0;j<MAXPARTICLES;j++)
	    {
		if (
		    (tt2->getParId(j)==2 && tt1->getParId(i)==3) ||
		    (tt2->getParId(j)==3 && tt1->getParId(i)==2) ||
		    (tt2->getParId(j)==2 && tt1->getParId(i)==2) ||
		    (tt2->getParId(j)==3 && tt1->getParId(i)==3) 
		    ) // is lepton pair
		{
		    if(
		       tt1->getCreaTrkNb(i)!=-1 &&
		       tt2->getCreaTrkNb(j)!=-1 &&
		       tt1->getCreaTrkNb(i)==tt2->getCreaTrkNb(j) // same mother
		       )
		    {
			ret_val = kFALSE;//lepton pair with same parent, signal !, not CB
			break;
		    }
		}
		else ret_val = kFALSE; // not a lepton pair, not part of CB
	    }
	}
    }
    else Error("isCombiPair","HTrackInfo objects not found");
    return ret_val;
}

Bool_t HRichCutPairSim::isFakePair(HDiHitMatchSim* h)
{

    Bool_t ret_val=kTRUE;

HLinearCategory * cat = (HLinearCategory*) HRichUtilFunc::getCategory(catMatchHit);
    HHitMatchSim *t1 = 0;
    HHitMatchSim *t2 = 0;
    HTrackInfo * tt1 = 0;
    HTrackInfo * tt2 = 0;
    if (cat)
    {
	 t1 = (HHitMatchSim*)cat->getObject(h->getIndTrk1());
	 if (!t1) Error("isFakePair","HHitMatchSim object 1 not found");
	 t2 = (HHitMatchSim*)cat->getObject(h->getIndTrk2());
	 if (!t2) Error("isFakePair","HHitMatchSim object 2 not found");
    }
    else Error("isFakePair","no category found");
    if (t1) tt1 = t1->getTrackInfoObj();
    if (t2) tt2 = t2->getTrackInfoObj();

    if (tt1&&tt2)
    {

	for (Int_t i=0;i<MAXPARTICLES;i++)
	{
	    Int_t c1 = tt1->calcCorrCode(i);
	    // first particle is dalitz
	    for (Int_t j=0;j<MAXPARTICLES;j++)
	    {
		Int_t c2 = tt2->calcCorrCode(j);
		if (c1==4 && c2==4)
		{
		    ret_val = kFALSE;
		    break;
		}
		
	    }
	}
    }
    else Error("isFakePair","HTrackInfo objects not found");
    
    return ret_val;
    
}
Bool_t HRichCutPairSim::isPhysCBPair(HDiHitMatchSim* h)
{

    Bool_t ret_val=kFALSE;

    HLinearCategory * cat = (HLinearCategory*) HRichUtilFunc::getCategory(catMatchHit);
    HHitMatchSim *t1 = 0;
    HHitMatchSim *t2 = 0;
    HTrackInfo * tt1 = 0;
    HTrackInfo * tt2 = 0;
    if (cat)
    {
	 t1 = (HHitMatchSim*)cat->getObject(h->getIndTrk1());
	 if (!t1) Error("isPhysCBPair","HHitMatchSim object 1 not found");
	 t2 = (HHitMatchSim*)cat->getObject(h->getIndTrk2());
	 if (!t2) Error("isPhysCBPair","HHitMatchSim object 2 not found");
    }
    else Error("isPhysCBPair","no category found");
    if (t1) tt1 = t1->getTrackInfoObj();
    if (t2) tt2 = t2->getTrackInfoObj();

    if (tt1&&tt2)
    {

	for (Int_t i=0;i<MAXPARTICLES;i++)
	{
	    
	    Int_t c1 = tt1->calcCorrCode(i);
	    // first particle is eta dalitz
	    for (Int_t j=0;j<MAXPARTICLES;j++)
	    {
		Int_t c2 = tt2->calcCorrCode(j);
		if (
		    c1==4 && c2==4 &&
		    tt2->getParId(j)+tt1->getParId(i)==5 && // is e+ e-
		    tt1->getCreaTrkNb(i)!=-1 &&
		    tt2->getCreaTrkNb(j)!=-1 &&
		    tt1->getCreaTrkNb(i)!=tt2->getCreaTrkNb(j) // not same mother

		    )
		{
		    ret_val = kTRUE;
		    break;
		}
	    }
	    
	}
	
    }
    else Error("isConvPair","HTrackInfo objects not found");
    
    return ret_val;
    
}


ClassImp(HRichCutPairSim)
