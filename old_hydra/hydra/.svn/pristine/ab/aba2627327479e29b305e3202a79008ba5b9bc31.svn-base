// $Id: hrichcutpairsim.cc,v 1.3 2003-04-04 13:13:28 eberl Exp $
// Last update by Thomas Eberl: 03/03/10 16:32:20
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

Bool_t HRichCutPairSim::switchTo(const char *s,Bool_t clear)
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
    else if (!state.CompareTo("isDalitzPair")) 
    {
	kisDalitzPair = 1;
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
    kisConvPair = 0;
    kisDalitzPair = 0;

}

void HRichCutPairSim::reset() 
{
    HRichCutPair::reset();
    kisConvPair = 0;
    kisDalitzPair = 0;
}

void HRichCutPairSim::printCutList(char* s) 
{

    if (s) switchTo(s);
    else {setStandardCuts(); HRichCutPair::setStandardCuts();}
    
    if (!isExp)
    {
	
	printf("\n-----------------------------------------------\n");
	printf("               HRichCutPairSim cuts          \n");
	printf("               ------------------------          \n");
	if (kisDalitzPair) printf("cut on pi0-Dalitz lepton pair active  \n");
	if (kisConvPair)   printf("cut on pair conversion lepton pair (from pi0) active\n");
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
    if (kisConvPair)   printf("cut on pair conversion lepton pair (from pi0) active\n");
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
		(!kisDalitzPair   ||       isDalitzPair(h)     )  &&
		(!kisConvPair     ||       isConvPair(h)       )   
		
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
		// first particle is conv
		for (Int_t j=0;j<MAXPARTICLES;j++)
		{
		    if (tt2->getConvLep(j)==1 && // 2nd part is conv
			tt2->getParId(j)+tt1->getParId(i)==5 && // is e+ e-
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
    else Error("isConvPair","no category found");
    if (t1) tt1 = t1->getTrackInfoObj();
    if (t2) tt2 = t2->getTrackInfoObj();

    if (tt1&&tt2)
    {

	for (Int_t i=0;i<MAXPARTICLES;i++)
	{
	    if (tt1->getPi0Dalitz(i)==1)
	    {
		// first particle is dalitz
		for (Int_t j=0;j<MAXPARTICLES;j++)
		{
		    if (tt2->getPi0Dalitz(j)==1 && // 2nd part is dalitz
			tt2->getParId(j)+tt1->getParId(i)==5 && // is e+ e-
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

ClassImp(HRichCutPairSim)
