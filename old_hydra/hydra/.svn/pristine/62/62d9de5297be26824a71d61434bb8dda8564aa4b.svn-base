// $Id: hrichcuttrackletsim.cc,v 1.2 2003-01-24 18:25:44 halo Exp $
// Last update by Thomas Eberl: 03/01/17 19:46:48
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

HRichCutTrackletSim::HRichCutTrackletSim(Text_t *name, Text_t *title)
         : HRichCutTracklet(name, title) 
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

void HRichCutTrackletSim::switchTo(char *s)
{
    TString state(s);
    HRichCutTracklet::switchTo(s);
    // compare to available inner states of the cut
    // and to switch to state
    if (!state.CompareTo("isDalitz"))
    {
	Int_t swt[] = {1,1,1,1,1,1,1,1};
	Float_t val[] = {4.,5.,2.8,200.,1.7,1.8};
	setState(swt,val);
    } 
    else if (!state.CompareTo("isConv")) 
    {
	Int_t swt[] = {0,1,0,0,0,0,1,1};
	Float_t val[] = {0,0,0,0,1.7,1.8};
	setState(swt,val);
    }
    else cout<<"requested state "<<state.Data()<<" for cut: "<<this->GetName()<<" not found"<<endl;

    
}

void HRichCutTrackletSim::setStandardCuts()
{
    Int_t   swt[]={};
    Float_t val[]={};

    setState(swt,val);
}

void HRichCutTrackletSim::setState(Int_t *swt,Float_t *val)
{


}



Bool_t HRichCutTrackletSim::check(HHitMatchSim* h)
{
    // there must be a switch for every function
    if (
	kTRUE

       ) return kTRUE;


    else return kFALSE;

}

void HRichCutTrackletSim::reset() 
{
    // set to standard values, but all inactive !
    Int_t   swt[]={};
    Float_t val[]={};

    setState(swt,val);

}

void HRichCutTrackletSim::printCutList(char *s) {
// Prints a table of enabled cuts to stdio
    if (s) switchTo(s);
    else setStandardCuts();
    HRichCutTracklet::printCutList(s);
    
  printf("\n-----------------------------------------------\n");
  printf("               HRichCutTrackletSim cuts               \n");
  printf("               ---------------------               \n");


  printf("----------------------------------------------\n\n");
  return;
}


ClassImp(HRichCutTrackletSim)
