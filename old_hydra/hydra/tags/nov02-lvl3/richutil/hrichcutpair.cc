// $Id: hrichcutpair.cc,v 1.2 2003-01-24 18:25:44 halo Exp $
// Last update by Thomas Eberl: 03/01/21 13:55:25
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
#include "hdihitmatch.h"
#include "phyanadef.h"
#include "hrichcutpair.h"
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

HRichCutPair::HRichCutPair() : HRichCutO() 
{
}

HRichCutPair::HRichCutPair(Text_t *name, Text_t *title)
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

void HRichCutPair::switchTo(char *s)
{
    TString state(s);

    // compare to available inner states of the cut
    // and to switch to state
    if (!state.CompareTo("unlike")) 
    {
	Int_t swt[]={0,0,0,0,0,0,0,0,1,0,0,0,0,0};
	Float_t val[]={0.,0.,0.,0.};
	setState(swt,val);
    }
    else if (!state.CompareTo("like+")) 
    {
	Int_t swt[]={0,0,0,0,0,0,0,0,0,1,0,0,0,0};
	Float_t val[]={0.,0.,0.,0.};
	setState(swt,val);
    }
    else if (!state.CompareTo("like-")) 
    {
	Int_t swt[]={0,0,0,0,0,0,0,0,0,0,1,0,0,0};
	Float_t val[]={0.,0.,0.,0.};
	setState(swt,val);
    }
    else if (!state.CompareTo("opangKICK>")) 
    {
	Int_t swt[]={1,0,0,0,0,0,0,0,0,0,0,0,0,0};
	Float_t val[]={4.,0.,0.,0.};
	setState(swt,val);
    }
    else if (!state.CompareTo("unlike.opangKICK>")) 
    {
	Int_t swt[]={1,0,0,0,0,0,0,0,1,0,0,0,0,0};
	Float_t val[]={4.,0.,0.,0.};
	setState(swt,val);
    }
    else if (!state.CompareTo("unlike.2hit")) 
    {
	Int_t swt[]={0,0,0,0,0,0,0,0,1,0,0,0,0,1};
	Float_t val[]={4.,0.,0.,0.};
	setState(swt,val);
    }
    else if (!state.CompareTo("unlike.1richhit")) 
    {
	Int_t swt[]={0,0,0,0,0,0,0,0,1,0,0,0,1,0};
	Float_t val[]={4.,0.,0.,0.};
	setState(swt,val);
    }
    else if (!state.CompareTo("unlike.1mdchit")) 
    {
	Int_t swt[]={0,0,0,0,0,0,0,0,1,0,0,1,0,0};
	Float_t val[]={4.,0.,0.,0.};
	setState(swt,val);
    }
    else cout<<"requested state "<<state.Data()<<" for cut: "<<this->GetName()<<" not found"<<endl;
}

void HRichCutPair::reset() 
{
    Int_t swt[]={0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    Float_t val[]={0.,0.,0.,0.};
    setState(swt,val);
}

void HRichCutPair::printCutList(char* s) {
    if (s) switchTo(s);
    else setStandardCuts();
// Prints a table of enabled cuts to stdio
  printf("\n-----------------------------------------------\n");
  printf("               HRichCutPair cuts               \n");
  printf("               -----------------               \n");


  if(kPairOpangKICKGreaterThan) printf("pair opening angle KICK >   %4.1f\n",nPairOpangKICK  );
  if(kPairOpangKICKLowerThan)   printf("pair opening angle KICK <   %4.1f\n",nPairOpangKICK  );
  if(kPairOpangMDCGreaterThan)  printf("pair opening angle MDC  >   %4.1f\n",nPairOpangMDC   ) ;
  if(kPairOpangMDCLowerThan)    printf("pair opening angle MDC  <   %4.1f\n",nPairOpangMDC   );
  if(kPairMinMom1GreaterThan)   printf("Trk1 momentum > \t %4.1f\n",nPairMinMom1    );
  if(kPairMinMom1LowerThan)     printf("Trk1 momentum < \t %4.1f\n",nPairMinMom1    );
  if(kPairMinMom2GreaterThan)   printf("Trk2 momentum > \t %4.1f\n",nPairMinMom2    );
  if(kPairMinMom2LowerThan)     printf("Trk2 momentum < \t %4.1f\n",nPairMinMom2    );
  if(kUnlikePair)               printf("Unlike sign pair \t %4.1i\n",kUnlikePair     );
  if(kLikePPair)                printf("Like sign pair + \t %4.1i\n",kLikePPair      );
  if(kLikeMPair)                printf("Like sign pair - \t %4.1i\n",kLikeMPair      );


  printf("-------------------------------------------------\n\n");
  return;
}

void HRichCutPair::setStandardCuts()
{

    Int_t swt[]={1,0,     //opang KICK
		 1,0,     //opang MDC
		 1,0,     //mom1
		 1,0,     //mom2
		 1,0,0,   //pairtype u,+,-
		 0,0,0};  //1mdc,1rich,2:2
    Float_t val[]={4.,4.,10.,10.};
    setState(swt,val);

}

void HRichCutPair::setState(Int_t *swt,Float_t *val)
{

     nPairOpangKICK  = val[0];
     nPairOpangMDC   = val[1];
     nPairMinMom1    = val[2];
     nPairMinMom2    = val[3];
    
     kPairOpangKICKGreaterThan  = swt[0];
     kPairOpangKICKLowerThan    = swt[1];
     kPairOpangMDCGreaterThan   = swt[2];
     kPairOpangMDCLowerThan     = swt[3];
     kPairMinMom1GreaterThan    = swt[4];
     kPairMinMom1LowerThan      = swt[5];
     kPairMinMom2GreaterThan    = swt[6];
     kPairMinMom2LowerThan      = swt[7];
     kUnlikePair                = swt[8];
     kLikePPair                 = swt[9];
     kLikeMPair                 = swt[10];
     kPair1MDC                  = swt[11];
     kPair1RICH                 = swt[12];
     kPair2HIT                  = swt[13];

}

Bool_t HRichCutPair::check(HDiHitMatch* h)
{
    // there must be a switch for every function
    if (

	(  !kUnlikePair                  ||       isUnLikePair(h)                )  &&
	(  !kLikePPair                   ||       isLikePPair(h)                 )  &&
	(  !kLikeMPair                   ||       isLikeMPair(h)                 )  &&
	(  !kPair1MDC                    ||       is1MDCPair(h)                  )  &&
	(  !kPair1RICH                   ||       is1RICHPair(h)                 )  &&
	(  !kPair2HIT                    ||       is2HITPair(h)                  )  &&
	(  !kPairOpangKICKGreaterThan    ||       isOpangKICKGreaterThan(h)      )  &&
	(  !kPairOpangKICKLowerThan      ||       isOpangKICKLowerThan(h)        )  &&
	(  !kPairOpangMDCGreaterThan     ||       isOpangMDCGreaterThan(h)       )  &&
	(  !kPairOpangMDCLowerThan       ||       isOpangMDCLowerThan(h)         )  &&
	(  !kPairMinMom1GreaterThan      ||       isMom1GreaterThan(h)           )  &&
	(  !kPairMinMom1LowerThan        ||       isMom1LowerThan(h)             )  &&
	(  !kPairMinMom2GreaterThan      ||       isMom2GreaterThan(h)           )  &&
	(  !kPairMinMom2LowerThan        ||       isMom2LowerThan(h)             )  

       ) return kTRUE;


    else return kFALSE;

}

Bool_t HRichCutPair::isOpangKICKGreaterThan(HDiHitMatch* h)
{
    if (h->getOpangKICK()>nPairOpangKICK) return kTRUE;
    return kFALSE;
}

Bool_t HRichCutPair::isOpangKICKLowerThan(HDiHitMatch* h)
{
    if (h->getOpangKICK()<nPairOpangKICK) return kTRUE;
    return kFALSE;
}
Bool_t HRichCutPair::isOpangMDCGreaterThan(HDiHitMatch* h)
{
    if (h->getOpangMDC()>nPairOpangMDC) return kTRUE;
    return kFALSE;
}

Bool_t HRichCutPair::isOpangMDCLowerThan(HDiHitMatch* h)
{
    if (h->getOpangMDC()<nPairOpangMDC) return kTRUE;
    return kFALSE;
}

Bool_t HRichCutPair::isLikePPair(HDiHitMatch* h)
{
    if (h->getCharge()==2) return kTRUE;
    return kFALSE;
}

Bool_t HRichCutPair::isLikeMPair(HDiHitMatch* h)
{
    if (h->getCharge()==-2) return kTRUE;
    return kFALSE;
}

Bool_t HRichCutPair::isUnLikePair(HDiHitMatch* h)
{
    if (h->getCharge()==0) return kTRUE;
    return kFALSE;
}

Bool_t HRichCutPair::isMom1GreaterThan(HDiHitMatch* h)
{
    if (h->getMomTrk1()>nPairMinMom1) return kTRUE;
    return kFALSE;
}
Bool_t HRichCutPair::isMom1LowerThan(HDiHitMatch* h)
{
    if (h->getMomTrk1()<nPairMinMom1) return kTRUE;
    return kFALSE;
}

Bool_t HRichCutPair::isMom2GreaterThan(HDiHitMatch* h)
{
    if (h->getMomTrk2()>nPairMinMom2) return kTRUE;
    return kFALSE;
}
Bool_t HRichCutPair::isMom2LowerThan(HDiHitMatch* h)
{
    if (h->getMomTrk2()<nPairMinMom2) return kTRUE;
    return kFALSE;
}
Bool_t HRichCutPair::is1MDCPair(HDiHitMatch* h)
{
    if (h->getNbDMdcHit() == 1 ) return kTRUE;
    return kFALSE;
}
Bool_t HRichCutPair::is1RICHPair(HDiHitMatch* h)
{
    if (h->getNbDRichHit() == 1 ) return kTRUE;
    return kFALSE;
}
Bool_t HRichCutPair::is2HITPair(HDiHitMatch* h)
{
    if (h->getNbDMdcHit() == 2 && h->getNbDRichHit() == 2) return kTRUE;
    return kFALSE;
}

ClassImp(HRichCutPair)
