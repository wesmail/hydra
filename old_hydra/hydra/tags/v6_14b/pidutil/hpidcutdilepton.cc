// $Id: hpidcutdilepton.cc,v 1.1 2003-02-26 08:50:23 eberl Exp $
// Last update by Thomas Eberl: 03/02/25 15:40:40
//

using namespace std;
#include <iostream>
#include <iomanip>
#include <TDatime.h>
#include <TString.h>
#include <TNamed.h>
#include "hpidparticle.h"
#include "hpiddilepton.h"
#include "hpidcutdilepton.h"
#include "hpidcuto.h"

HPidCutDilepton::HPidCutDilepton() : HPidCutO() 
{
}

HPidCutDilepton::HPidCutDilepton(Text_t *name, Text_t *title)
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

void HPidCutDilepton::switchTo(char *s)
{
    TString state(s);

    // compare to available inner states of the cut
    // and to switch to state
    if (!state.CompareTo("unlike")) 
    {
	Int_t swt[]={0,0,0,0,0,0,1,0,0,0,0,0};
	Float_t val[]={0.,0.,0.};
	setState(swt,val);
    }
    else if (!state.CompareTo("like+")) 
    {
	Int_t swt[]={0,0,0,0,0,0,0,1,0,0,0,0};
	Float_t val[]={0.,0.,0.};
	setState(swt,val);
    }
    else if (!state.CompareTo("like-")) 
    {
	Int_t swt[]={0,0,0,0,0,0,0,0,1,0,0,0};
	Float_t val[]={0.,0.,0.};
	setState(swt,val);
    }
    else if (!state.CompareTo("opang>")) 
    {
	Int_t swt[]={1,0,0,0,0,0,0,0,0,0,0,0};
	Float_t val[]={4.,0.,0.};
	setState(swt,val);
    }
    else if (!state.CompareTo("unlike.opang>")) 
    {
	Int_t swt[]={1,0,0,0,0,0,1,0,0,0,0,0};
	Float_t val[]={4.,0.,0.};
	setState(swt,val);
    }
    else if (!state.CompareTo("unlike.2hit")) 
    {
	Int_t swt[]={0,0,0,0,0,0,1,0,0,0,0,1};
	Float_t val[]={4.,0.,0.};
	setState(swt,val);
    }
    else if (!state.CompareTo("unlike.1richhit")) 
    {
	Int_t swt[]={0,0,0,0,0,0,1,0,0,0,1,0};
	Float_t val[]={4.,0.,0.};
	setState(swt,val);
    }
    else if (!state.CompareTo("unlike.1mdchit")) 
    {
	Int_t swt[]={0,0,0,0,0,0,1,0,0,1,0,0};
	Float_t val[]={4.,0.,0.};
	setState(swt,val);
    }
    else cout<<"requested state "<<state.Data()<<" for cut: "<<this->GetName()<<" not found"<<endl;
}

void HPidCutDilepton::reset() 
{
    Int_t swt[]={0,0,0,0,0,0,0,0,0,0,0,0};
    Float_t val[]={0.,0.,0.};
    setState(swt,val);
}

void HPidCutDilepton::printCutList(char* s) {
    if (s) switchTo(s);
    else setStandardCuts();
// Prints a table of enabled cuts to stdio
  printf("\n-----------------------------------------------\n");
  printf("               HPidCutDilepton cuts               \n");
  printf("               -----------------               \n");


  if(kPairOpangGreaterThan) printf("pair opening angle >   %4.1f\n",nPairOpang  );
  if(kPairOpangLowerThan)   printf("pair opening angle <   %4.1f\n",nPairOpang  );
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

void HPidCutDilepton::setStandardCuts()
{

    Int_t swt[]={1,0,     //opang
		 1,0,     //mom1
		 1,0,     //mom2
		 1,0,0,   //pairtype u,+,-
		 0,0,0};  //1mdc,1rich,2:2
    Float_t val[]={4.,10.,10.};
    setState(swt,val);

}

void HPidCutDilepton::setState(Int_t *swt,Float_t *val)
{

     nPairOpang      = val[0];
     nPairMinMom1    = val[1];
     nPairMinMom2    = val[2];
    
     kPairOpangGreaterThan  = swt[0];
     kPairOpangLowerThan    = swt[1];
     kPairMinMom1GreaterThan    = swt[2];
     kPairMinMom1LowerThan      = swt[3];
     kPairMinMom2GreaterThan    = swt[4];
     kPairMinMom2LowerThan      = swt[5];
     kUnlikePair                = swt[6];
     kLikePPair                 = swt[7];
     kLikeMPair                 = swt[8];
     kPair1MDC                  = swt[9];
     kPair1RICH                 = swt[10];
     kPair2HIT                  = swt[11];

}

Bool_t HPidCutDilepton::check(HPidDilepton* h)
{
    // there must be a switch for every function
    if (

	(  !kUnlikePair                  ||       isUnLikePair(h)                )  &&
	(  !kLikePPair                   ||       isLikePPair(h)                 )  &&
	(  !kLikeMPair                   ||       isLikeMPair(h)                 )  &&
	(  !kPair1MDC                    ||       is1MDCPair(h)                  )  &&
	(  !kPair1RICH                   ||       is1RICHPair(h)                 )  &&
	(  !kPair2HIT                    ||       is2HITPair(h)                  )  &&
	(  !kPairOpangGreaterThan        ||       isOpangGreaterThan(h)          )  &&
	(  !kPairOpangLowerThan          ||       isOpangLowerThan(h)            )  &&
	(  !kPairMinMom1GreaterThan      ||       isMom1GreaterThan(h)           )  &&
	(  !kPairMinMom1LowerThan        ||       isMom1LowerThan(h)             )  &&
	(  !kPairMinMom2GreaterThan      ||       isMom2GreaterThan(h)           )  &&
	(  !kPairMinMom2LowerThan        ||       isMom2LowerThan(h)             )  

       ) return kTRUE;


    else return kFALSE;

}

Bool_t HPidCutDilepton::isOpangGreaterThan(HPidDilepton* h)
{
    if (h->getOpeningAngleDeg()>nPairOpang) return kTRUE;
    return kFALSE;
}

Bool_t HPidCutDilepton::isOpangLowerThan(HPidDilepton* h)
{
    if (h->getOpeningAngleDeg()<nPairOpang) return kTRUE;
    return kFALSE;
}

Bool_t HPidCutDilepton::isLikePPair(HPidDilepton* h)
{
    if ((Int_t)h->getCharge()==2) return kTRUE;
    return kFALSE;
}

Bool_t HPidCutDilepton::isLikeMPair(HPidDilepton* h)
{
    if ((Int_t)h->getCharge()==-2) return kTRUE;
    return kFALSE;
}

Bool_t HPidCutDilepton::isUnLikePair(HPidDilepton* h)
{
    if ((Int_t)h->getCharge()==0) return kTRUE;
    return kFALSE;
}

Bool_t HPidCutDilepton::isMom1GreaterThan(HPidDilepton* h)
{
    if ((h->getParticle1())->P()>nPairMinMom1) return kTRUE;
    return kFALSE;
}
Bool_t HPidCutDilepton::isMom1LowerThan(HPidDilepton* h)
{
    if ((h->getParticle1())->P()<nPairMinMom1) return kTRUE;
    return kFALSE;
}

Bool_t HPidCutDilepton::isMom2GreaterThan(HPidDilepton* h)
{
    if ((h->getParticle2())->P()>nPairMinMom2) return kTRUE;
    return kFALSE;
}
Bool_t HPidCutDilepton::isMom2LowerThan(HPidDilepton* h)
{
    if ((h->getParticle2())->P()<nPairMinMom2) return kTRUE;
    return kFALSE;
}
Bool_t HPidCutDilepton::is1MDCPair(HPidDilepton* h)
{
    //    if (h->getNbDMdcHit() == 1 ) return kTRUE;
    return kFALSE;
}
Bool_t HPidCutDilepton::is1RICHPair(HPidDilepton* h)
{
    
    //    if (h->getNbDRichHit() == 1 ) return kTRUE;
    return kFALSE;
}
Bool_t HPidCutDilepton::is2HITPair(HPidDilepton* h)
{
    //    if (h->getNbDMdcHit() == 2 && h->getNbDRichHit() == 2) return kTRUE;
    return kFALSE;
}

ClassImp(HPidCutDilepton)
