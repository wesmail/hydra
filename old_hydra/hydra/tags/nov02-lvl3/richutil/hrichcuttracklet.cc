// $Id: hrichcuttracklet.cc,v 1.2 2003-01-24 18:25:44 halo Exp $
// Last update by Thomas Eberl: 03/01/17 15:20:16
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

void HRichCutTracklet::switchTo(char *s)
{
    TString state(s);

    // compare to available inner states of the cut
    // and to switch to state
    if (!state.CompareTo("matchedGoodRing"))
    {
	Int_t swt[] = {1,1,1,1,1,1,1,1};
	Float_t val[] = {4.,5.,2.8,200.,1.7,1.8};
	setState(swt,val);
    } 
    else if (!state.CompareTo("angularMatch")) 
    {
	Int_t swt[] = {0,1,0,0,0,0,1,1};
	Float_t val[] = {0,0,0,0,1.7,1.8};
	setState(swt,val);
    }
    else cout<<"requested state "<<state.Data()<<" for cut: "<<this->GetName()<<" not found"<<endl;

    
}

void HRichCutTracklet::setStandardCuts()
{
    Int_t   swt[]={1,1,1,1,1,1,1,1};
    Float_t val[]={4.,5.,2.8,200.,1.7,1.8};

    setState(swt,val);
}

void HRichCutTracklet::setState(Int_t *swt,Float_t *val)
{
    nRingPadNr    =   val[0];
    nRingAvChrg   =   val[1];
    nRingCentroid =   val[2];
    nRingPatMat   =   val[3];
    nRichMdcTheta =   val[4];
    nRichMdcPhi   =   val[5];


    kGoodRing      = swt[0];
    kAngularMatch  = swt[1];
    kRingPadNr     = swt[2];
    kRingAvChrg    = swt[3]; 
    kRingCentroid  = swt[4];
    kRingPatMat    = swt[5];
    kRichMdcTheta  = swt[6];
    kRichMdcPhi    = swt[7];
}



Bool_t HRichCutTracklet::check(HHitMatch* h)
{
    // there must be a switch for every function
    if (

	(  !kAngularMatch                ||       isAngMatch(h)                  )  &&
	(  !kGoodRing                    ||       isGoodRing(h)                  )  

       ) return kTRUE;


    else return kFALSE;

}

void HRichCutTracklet::reset() 
{
    // set to standard values, but all inactive !
    Int_t   swt[]={0,0,0,0,0,0,0,0};
    Float_t val[]={4.,5.,2.8,200.,1.7,1.8};

    setState(swt,val);

}

void HRichCutTracklet::printCutList(char *s) {
// Prints a table of enabled cuts to stdio
    if (s) switchTo(s);
    else setStandardCuts();
  printf("\n-----------------------------------------------\n");
  printf("               HRichCutTracklet cuts               \n");
  printf("               ---------------------               \n");

  if(kRingPadNr)    printf(" nRingPadNr = %4.1f\n",   nRingPadNr);
  if(kRingAvChrg)   printf(" nRingAvChrg = %4.1f\n",  nRingAvChrg);
  if(kRingCentroid) printf(" nRingCentroid = %4.1f\n",nRingCentroid);
  if(kRingPatMat)   printf(" nRingPatMat = %4.1f\n",  nRingPatMat);
  if(kRichMdcTheta) printf(" nRichMdcTheta = %4.1f\n",nRichMdcTheta);
  if(kRichMdcPhi)   printf(" nRichMdcPhi = %4.1f\n",  nRichMdcPhi);

  printf("----------------------------------------------\n\n");
  return;
}
Bool_t HRichCutTracklet::isAngMatch(HHitMatch *h)
{

   Bool_t ret_val=kFALSE;
    if (
	
	(!kRichMdcTheta || TMath::Abs(h->getRichTheta() - h->getMdcTheta()) < nRichMdcTheta) &&
	(!kRichMdcPhi   || TMath::Abs(h->getRichPhi()   - h->getMdcPhi())   < nRichMdcPhi) 
	
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


ClassImp(HRichCutTracklet)
