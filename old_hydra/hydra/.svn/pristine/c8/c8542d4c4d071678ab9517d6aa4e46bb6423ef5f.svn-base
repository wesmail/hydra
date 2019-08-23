// $Id: hrichcuttrackletsim.cc,v 1.8 2009-07-15 11:39:21 halo Exp $
// Last update by Thomas Eberl: 03/07/15 15:30:28
//

using namespace std;
#include "TObject.h"
#include "TRandom.h"
#include <time.h>
#include <iostream>
#include <iomanip>
#include <math.h>
#include "TMath.h"
#include "TDatime.h"
#include "hhitmatchsim.h"
#include "phyanadef.h"
#include "hrichcuttrackletsim.h"
#include "hrichcuto.h"
#include "hrichcut.h"
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

HRichCutTrackletSim::HRichCutTrackletSim() : HRichCutTracklet() 
{
}

HRichCutTrackletSim::HRichCutTrackletSim(const Text_t *name,const Text_t *title)
         : HRichCutTracklet(name, title) 
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
     setEvtType(1);//is sim
}

Bool_t HRichCutTrackletSim::switchTo(const Char_t *s,Bool_t clear)
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
		//cout<<"before switchTo "<<tmp.Data()<<endl;
		//if (!(ret_val = switchTo(tmp.Data(),kFALSE))) ret_val=HRichCutTracklet::switchTo(tmp.Data(),kFALSE);
		if (!(ret_val = switchTo(tmp.Data(),kFALSE))) return ret_val;
		//if (!ret_val) return ret_val;
		tmp="";
	    }
	    else
	    {
		tmp.Append(st);
	    }
	    if (i==len-1) 
	    {
		tmp.Remove(0,1);
		//if (!(ret_val = switchTo(tmp.Data(),kFALSE))) ret_val=HRichCutTracklet::switchTo(tmp.Data(),kFALSE);
		if (!(ret_val = switchTo(tmp.Data(),kFALSE))) return ret_val;
		//		if (!ret_val) return ret_val;
	    }
	}
	return ret_val;
    }
    
    // compare to available inner states of the cut
    // and to switch to state
    if (!state.CompareTo("isDalitz"))
    {
	kisDalitz = 1;
	ret_val = kTRUE;
	isSim = kTRUE;
    } 
    else if (!state.CompareTo("isConv")) 
    {
	//cout<<"isConv"<<endl;
	kisConv = 1;
	ret_val = kTRUE;
	isSim = kTRUE;
    }
    else if (!state.CompareTo("nocuttrackletsim")) 
    {
	ret_val = kTRUE;isSim = kFALSE;
    }
    else 
    {
	// no sim cut keyword found
	// try exp cut

	isExp=HRichCutTracklet::switchTo(s,clear);

	// still no valid keyword found
	if (!isExp) 
	{
// 	    cout<<"requested state "<<state.Data()<<" for cut: "
// 		<<this->GetName()<<" not found"<<endl;
// 	    Error("switchTo()","no valid cut found");
	}
	ret_val = isExp;
	
    }   

    return ret_val; 
}

void HRichCutTrackletSim::setStandardCuts()
{
    // switch all cuts of by default
    reset();
    kisConv = 0;
    kisDalitz = 0;
    //    isExp = kFALSE;
}


Bool_t HRichCutTrackletSim::check(HHitMatchSim* h)
{
    
    // there must be a switch for every function

    Bool_t ret_val=kFALSE;
    //printCutList();
    if (!listCut)
    {
	if ( !isExp ){ 
	    // the cut was switched to pure sim
	    if (  
		(!kisDalitz   ||       isDalitz(h)     )  &&
		(!kisConv     ||       isConv(h)       )   
		 
		) ret_val=kTRUE;

	}
	// the sim cut was switched with a exp keyword
	// call the exp cut check function
	else if (isExp) ret_val=HRichCutTracklet::check(h);
    }
    else 
    {
	if (isExp && isSim)
	{
	    if (  
		(!kisDalitz   ||       isDalitz(h)     )  &&
		(!kisConv     ||       isConv(h)       )   
		) ret_val=kTRUE;
	    Bool_t ret_val_exp = HRichCutTracklet::check(h);
	    if (ret_val && ret_val_exp) ret_val = kTRUE;
	    else ret_val=kFALSE;
	}
	// 
	else {listCut=kFALSE;ret_val = check(h);}
    }
    return ret_val;

}

void HRichCutTrackletSim::reset() 
{
    // set to standard values, but all inactive !

    HRichCutTracklet::reset();
    //    isExp = kFALSE;
    kisConv = 0;
    kisDalitz = 0; 
}

void HRichCutTrackletSim::printCutList(const Char_t *s) {
// Prints a table of enabled cuts to stdio

    if (s) switchTo(s);
    else {setStandardCuts(); HRichCutTracklet::setStandardCuts();}

    if (!isExp)
    {
	
	printf("\n-----------------------------------------------\n");
	printf("               HRichCutTrackletSim cuts          \n");
	printf("               ------------------------          \n");
	if (kisDalitz) printf("cut on pi0-Dalitz leptons active  \n");
	if (kisConv)   printf("cut on pair conversion leptons (from pi0) active\n");
	printf("-------------------------------------------------\n\n");
    }
    else HRichCutTracklet::printCutList(s);
    return;
}

void HRichCutTrackletSim::printCutList() {
// Prints a table of enabled cuts to stdio

	printf("\n-----------------------------------------------\n");
	printf("               HRichCutTrackletSim cuts          \n");
	printf("               ------------------------          \n");
	if (kisDalitz) printf("cut on pi0-Dalitz leptons active  \n");
	if (kisConv)   printf("cut on pair conversion leptons (from pi0) active\n");
	printf("-------------------------------------------------\n\n");
	HRichCutTracklet::printCutList();
}

Bool_t HRichCutTrackletSim::isConv(HHitMatchSim* h)
{
    // check 3 conditions:
    // - lepton went through mirror in correct position of ring
    // - lepton in tracklet comes from gamma conversion (gamma from pi0), can also be two leptons in tracklet here !
    // - at least one of the leptons in the tracklet must be seen by all subdetectors 
    Bool_t ret_val=kFALSE;
    if (h->getLeptonOnMirror()<1) return ret_val;
    HTrackInfo * t = h->getTrackInfoObj();
    for (Int_t i=0;i<MAXPARTICLES;i++)
    {
	if (t->getConvLep(i)==1 && t->getMatchedRMT_S(i)==1)
	{
	    ret_val = kTRUE;
	    break;
	}

    }

    return ret_val;
    
}
Bool_t HRichCutTrackletSim::isDalitz(HHitMatchSim* h)
{
    // check 3 conditions:
    // - lepton went through mirror in correct position of ring
    // - lepton in tracklet comes from pi0-Dalitz, can also be two leptons in tracklet here !
    // - at least one of the leptons in the tracklet must be seen by all subdetectors 
    Bool_t ret_val=kFALSE;
    if (h->getLeptonOnMirror()<1) return ret_val;
    HTrackInfo * t = h->getTrackInfoObj();
    for (Int_t i=0;i<MAXPARTICLES;i++)
    {
	if (t->getPi0Dalitz(i)==1 && t->getMatchedRMT_S(i)==1)
	{
	    ret_val = kTRUE;
	    break;
	}

    }

    return ret_val;
    
}

ClassImp(HRichCutTrackletSim)
