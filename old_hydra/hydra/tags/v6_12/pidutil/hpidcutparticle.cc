// $Id: hpidcutparticle.cc,v 1.1 2003-02-26 08:50:45 eberl Exp $
// Last update by Thomas Eberl: 03/02/26 08:37:38
//

using namespace std;
#include <iostream>
#include <iomanip>
#include <TDatime.h>
#include <TString.h>
#include <TNamed.h>
#include "hpidparticle.h"
#include "hpidcutparticle.h"
#include "hpidcuto.h"

HPidCutParticle::HPidCutParticle() : HPidCutO() 
{
}

HPidCutParticle::HPidCutParticle(Text_t *name, Text_t *title)
         : HPidCutO(name, title) 
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
}

void HPidCutParticle::switchTo(char *s)
{
    TString state(s);

    // compare to available inner states of the cut
    // and switch to state

    
    if (!state.CompareTo("leptonbeta"))
    {
	Int_t   swt[] = {1,0};
	Float_t val[] = {0.9};
	setState(swt,val);
    } 
    else if (!state.CompareTo("lepton"))
    {
	Int_t   swt[] = {0,1};
	Float_t val[] = {0.};
	setState(swt,val);
    } 
    else cout<<"requested state "<<state.Data()<<" for cut: "<<this->GetName()<<" not found"<<endl;
    
    
}

void HPidCutParticle::setStandardCuts()
{
   
    Int_t   swt[]={1,1};
    Float_t val[]={0.9};

    setState(swt,val);
   
}

void HPidCutParticle::setState(Int_t *swt,Float_t *val)
{
    nPartBetaExpMin = val[0];
    kPartBetaExpMin = swt[0];
    kPartLepton     = swt[1];
}



Bool_t HPidCutParticle::check(HPidParticle* h)
{
    // there must be a switch for each function
    if (
	
	(  !kPartBetaExpMin                ||       isBetaExpMin(h)                  ) &&
	(  !kPartLepton                    ||       isLepton(h)                      )
	
	
	) return kTRUE;

    
    else return kFALSE;
    
}

void HPidCutParticle::reset() 
{


    // set to standard values, but all inactive !
    Int_t   swt[]={0,0};
    Float_t val[]={0.9};

    setState(swt,val);

}

void HPidCutParticle::printCutList(char *s) {
// Prints a table of enabled cuts to stdio
    if (s) switchTo(s);
    else setStandardCuts();
  printf("\n-----------------------------------------------\n");
  printf("               HPidCutParticle cuts               \n");
  printf("               --------------------              \n");
  if (kPartBetaExpMin) printf("Particle Beta exp > \t %4.1f\n",nPartBetaExpMin   );
  if (kPartLepton    ) printf("Check for Lepton"                                 );
  printf("----------------------------------------------\n\n");
  return;
}

Bool_t HPidCutParticle::isBetaExpMin(HPidParticle *p)
{
    if (p->getBetaExp() > nPartBetaExpMin) return kTRUE;
    return kFALSE;
}

Bool_t HPidCutParticle::isLepton(HPidParticle *p)
{
    Int_t pid = (Int_t)p->getPid();
    if ( pid == 2 || pid == 3 ) return kTRUE;
    return kFALSE;

}
ClassImp(HPidCutParticle)
