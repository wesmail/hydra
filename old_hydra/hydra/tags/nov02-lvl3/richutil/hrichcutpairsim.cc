// $Id: hrichcutpairsim.cc,v 1.2 2003-01-24 18:25:44 halo Exp $
// Last update by Thomas Eberl: 03/01/17 19:47:30
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
#include "phyanadef.h"
#include "hrichcutpairsim.h"
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

HRichCutPairSim::HRichCutPairSim() : HRichCutPair() 
{
}

HRichCutPairSim::HRichCutPairSim(Text_t *name, Text_t *title)
         : HRichCutPair(name, title) 
{
    setStandardCuts();
    TDatime dt;
    dt.Set();

     TString stitle(title);
     stitle.Append("_");
     stitle+=dt.GetDate();
     stitle.Append("_");
     stitle+=dt.GetTime();
     SetTitle(stitle.Data());
//     TString sname(name);
//     sname.Append("_");
//     sname+=dt.GetDate();
//     sname.Append("_");
//     sname+=dt.GetTime();
//     SetName(sname.Data());
}

void HRichCutPairSim::switchTo(char *s)
{
    TString state(s);

    // compare to available inner states of the cut
    // and to switch to state
    if (!state.CompareTo("unlike")) 
    {
	Int_t swt[]={};
	Float_t val[]={};
	setState(swt,val);
    }
    else cout<<"requested state "<<state.Data()<<" for cut: "<<this->GetName()<<" not found"<<endl;
}

void HRichCutPairSim::reset() 
{
    Int_t swt[]={};
    Float_t val[]={};
    setState(swt,val);
}

void HRichCutPairSim::printCutList(char* s) {
    if (s) switchTo(s);
    else setStandardCuts();
    HRichCutPair::printCutList(s);

// Prints a table of enabled cuts to stdio
  printf("\n-----------------------------------------------\n");
  printf("               HRichCutPairSim cuts               \n");
  printf("               -----------------               \n");




  printf("-------------------------------------------------\n\n");
  return;
}

void HRichCutPairSim::setStandardCuts()
{

    Int_t swt[]={};
    Float_t val[]={};
    setState(swt,val);

}

void HRichCutPairSim::setState(Int_t *swt,Float_t *val)
{


}

Bool_t HRichCutPairSim::check(HDiHitMatchSim* h)
{
    // there must be a switch for every function
    if (

	kTRUE
       ) return kTRUE;


    else return kFALSE;

}


ClassImp(HRichCutPairSim)
