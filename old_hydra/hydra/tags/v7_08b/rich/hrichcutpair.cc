// $Id: hrichcutpair.cc,v 1.1 2004-10-15 08:19:23 eberl Exp $
// Last update by Thomas Eberl: 03/08/06 13:24:43
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
#include "hhitmatch.h"
#include "phyanadef.h"
#include "hrichcutpair.h"
#include "hrichcuttracklet.h"
#include "hrichutilfunc.h"
#include "hlinearcategory.h"
#include "hrichcuto.h"
#include "hkicktrack.h"
#include "kickdef.h"
#include "richdef.h"
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
    isExp=kTRUE;
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

Bool_t HRichCutPair::switchTo(const char *s, Bool_t clear)
{
    TString state(s);
    //cout<<state.Data()<<endl;
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

    if (!state.CompareTo("unlike")) 
    {
	kUnlikePair=1;
	ret_val=kTRUE;
	isExp = kTRUE;
    }
    else if (!state.CompareTo("like+")) 
    {
	kLikePPair=1;
	isExp = kTRUE;
	ret_val=kTRUE;
    }
    else if (!state.CompareTo("like-")) 
    {
	kLikeMPair=1;
	isExp = kTRUE;
	ret_val=kTRUE;
    }
    else if (!state.CompareTo("opangKICKgt12")) 
    {
	kPairOpangKICKGreaterThan=1;
	nPairOpangKICK=12.;
	isExp = kTRUE;
	ret_val=kTRUE;
    }
    else if (!state.CompareTo("opangKICKlt12")) 
    {
	kPairOpangKICKLowerThan=1;
	nPairOpangKICK=12.;
	isExp = kTRUE;
	ret_val=kTRUE;
    }
    else if (!state.CompareTo("opangKICKgt4")) 
    {
	kPairOpangKICKGreaterThan=1;
	nPairOpangKICK=4.;
	isExp = kTRUE;
	ret_val=kTRUE;
    }
    else if (!state.CompareTo("opangKICKgt4lt15")) 
    {
	kPairOpangKICKGTLT=1;
	nPairOpangKICK1=4.;
	nPairOpangKICK2=15.;
	isExp = kTRUE;
	ret_val=kTRUE;
    }
    else if (!state.CompareTo("opangKICKgt6lt18")) 
    {
	kPairOpangKICKGTLT=1;
	nPairOpangKICK1=6.;
	nPairOpangKICK2=18.;
	isExp = kTRUE;
	ret_val=kTRUE;
    }
    else if (!state.CompareTo("opangKICKgt8lt25")) 
    {
	kPairOpangKICKGTLT=1;
	nPairOpangKICK1=8.;
	nPairOpangKICK2=25.;
	isExp = kTRUE;
	ret_val=kTRUE;
    }
    else if (!state.CompareTo("opangKICKlt4")) 
    {
	kPairOpangKICKLowerThan=1;
	nPairOpangKICK=4.;
	isExp = kTRUE;
	ret_val=kTRUE;
    }
    else if (!state.CompareTo("2hit")) 
    {
	kPair2HIT=1;
	isExp = kTRUE;
	ret_val=kTRUE;
    }
    else if (!state.CompareTo("1richhit")) 
    {
	kPair1RICH=1;
	isExp = kTRUE;
	ret_val=kTRUE;
    }
    else if (!state.CompareTo("no1rich1mdc")) 
    {
	kPair1RICH1MDC=1;
	isExp = kTRUE;
	ret_val=kTRUE;
    }
    else if (!state.CompareTo("1mdchit")) 
    {
	kPair1MDC=1;
	isExp = kTRUE;
	ret_val=kTRUE;
    }
    else if (!state.CompareTo("nosamemeta"))
    {
	kNoSameMETA=1;
	isExp = kTRUE;
	ret_val=kTRUE;
    }
    else if (!state.CompareTo("2matchedGoodRing"))
    {
	k2MatchedGoodRing=1;
	isExp = kTRUE;
	ret_val=kTRUE;
    }
    else if (!state.CompareTo("2shower"))
    {
	k2Shower=1;
	isExp = kTRUE;
	ret_val=kTRUE;
    }
    else if (!state.CompareTo("2mdcchi2"))
    {
	k2MdcChi2=1;
	isExp = kTRUE;
	ret_val=kTRUE;
    }
    else if (!state.CompareTo("2matched"))
    {
	k2Matched=1;
	isExp = kTRUE;
	ret_val=kTRUE;
    }
    else if (!state.CompareTo("2goodRing"))
    {
	k2GoodRing=1;
	isExp = kTRUE;
	ret_val=kTRUE;
    }
    else if (!state.CompareTo("2fast"))
    {
	k2Fast=1;
	isExp = kTRUE;
	ret_val=kTRUE;
    }
    else if (!state.CompareTo("2tof"))
    {
	k2Tof=1;
	isExp = kTRUE;
	ret_val=kTRUE;
    }
    else if (!state.CompareTo("2pull"))
    {
	k2Pull=1;
	isExp = kTRUE;
	ret_val=kTRUE;
    }
    else if (!state.CompareTo("2pullmom"))
    {
	k2PullMom=1;
	isExp = kTRUE;
	ret_val=kTRUE;
    }
    else if (!state.CompareTo("2betamom"))
    {
	k2BetaMom=1;
	isExp = kTRUE;
	ret_val=kTRUE;
    }
    else if (!state.CompareTo("nocutpair")) 
    {
	ret_val=kTRUE;
    }
    else
    {
// 	cout<<"requested state "<<state.Data()<<" for cut: "
// 	  <<this->GetName()<<" not found"<<endl;
// 	Warning("switchTo","requested cut keyword not found");
// 	ret_val=kFALSE;
    }
    return ret_val;
}

void HRichCutPair::reset() 
{
     nPairOpangKICK  = 0.;
     nPairOpangKICK1 = 0.;
     nPairOpangKICK2 = 0.;
     nPairOpangMDC   = 0.;
     nPairMinMom1    = 0.;
     nPairMinMom2    = 0.;
    
     kPairOpangKICKGreaterThan  = 0;
     kPairOpangKICKGTLT         = 0;
     kPairOpangKICKLowerThan    = 0;
     kPairOpangMDCGreaterThan   = 0;
     kPairOpangMDCLowerThan     = 0;
     kPairMinMom1GreaterThan    = 0;
     kPairMinMom1LowerThan      = 0;
     kPairMinMom2GreaterThan    = 0;
     kPairMinMom2LowerThan      = 0;
     kUnlikePair                = 0;
     kLikePPair                 = 0;
     kLikeMPair                 = 0;
     kPair1MDC                  = 0;
     kPair1RICH                 = 0;
     kPair2HIT                  = 0;
     kPair1RICH1MDC             = 0;
     kNoSameMETA                = 0;
     k2MatchedGoodRing          = 0;
     k2GoodRing                 = 0;
     k2Matched                  = 0;
     k2Fast                     = 0;
     k2Tof                      = 0;
     k2Pull                     = 0;
     k2Shower                   = 0;
     k2MdcChi2                  = 0;
     k2PullMom                  = 0;
     k2BetaMom                  = 0;
     listCut = kFALSE;
     isExp=kFALSE;
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
void HRichCutPair::printCutList() {

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

    reset();

     nPairOpangKICK  = 12.;
     nPairOpangMDC   = 12.;
     nPairMinMom1    = 10.;
     nPairMinMom2    = 10.;
    
     kPairOpangKICKGreaterThan  = 0;
     kPairOpangMDCGreaterThan   = 0;
     kPairMinMom1GreaterThan    = 0;
     kPairMinMom2GreaterThan    = 0;
     kUnlikePair                = 0;
     kNoSameMETA                = 0;
     k2MatchedGoodRing          = 0;
     k2Matched                  = 0;
     k2GoodRing                 = 0;
     k2Fast                     = 0;
     k2Tof                      = 0;
     k2Pull                     = 0;
     k2Shower                   = 0;
     k2PullMom                  = 0;
     k2BetaMom                  = 0;
     isExp = kTRUE;
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
	(  !kPair1RICH1MDC               ||       isNOT1RICH1MDCPair(h)          )  &&
	(  !kPair2HIT                    ||       is2HITPair(h)                  )  &&
	(  !kPairOpangKICKGTLT           ||       isOpangKICKGTLT(h)             )  &&
	(  !kPairOpangKICKGreaterThan    ||       isOpangKICKGreaterThan(h)      )  &&
	(  !kPairOpangKICKLowerThan      ||       isOpangKICKLowerThan(h)        )  &&
	(  !kPairOpangMDCGreaterThan     ||       isOpangMDCGreaterThan(h)       )  &&
	(  !kPairOpangMDCLowerThan       ||       isOpangMDCLowerThan(h)         )  &&
	(  !kPairMinMom1GreaterThan      ||       isMom1GreaterThan(h)           )  &&
	(  !kPairMinMom1LowerThan        ||       isMom1LowerThan(h)             )  &&
	(  !kPairMinMom2GreaterThan      ||       isMom2GreaterThan(h)           )  &&
	(  !kPairMinMom2LowerThan        ||       isMom2LowerThan(h)             )  &&
	(  !kNoSameMETA                  ||       isNoSameMETA(h)                )  &&
	(  !k2MatchedGoodRing            ||       is2MatchedGoodRing(h)          )  &&
	(  !k2GoodRing                   ||       is2GoodRing(h)                 )  &&
	(  !k2Matched                    ||       is2Matched(h)                  )  &&
	(  !k2Fast                       ||       is2Fast(h)                     )  &&
	(  !k2Pull                       ||       is2Pull(h)                     )  &&
	(  !k2Shower                     ||       is2Shower(h)                   )  &&
	(  !k2MdcChi2                    ||       is2MdcChi2(h)                  )  &&
	(  !k2PullMom                    ||       is2PullMom(h)                  )  &&
	(  !k2BetaMom                    ||       is2BetaMom(h)                  )  &&
	(  !k2Tof                        ||       is2TofWin(h)                   )  

       ) return kTRUE;


    else return kFALSE;

}

Bool_t HRichCutPair::isOpangKICKGTLT(HDiHitMatch* h)
{
    if (h->getOpangKICK()>nPairOpangKICK1 && 
	h->getOpangKICK()<nPairOpangKICK2) return kTRUE;
    return kFALSE;
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
Bool_t HRichCutPair::isNOT1RICH1MDCPair(HDiHitMatch* h)
{
    if (h->getNbDMdcHit() == 1 && h->getNbDRichHit() == 1) return kFALSE;
    return kTRUE;
}
Bool_t HRichCutPair::isNoSameMETA(HDiHitMatch* h)
{
    if (h->getNbDMetaHit() == 2) return kTRUE;
    return kFALSE;
}
Bool_t HRichCutPair::is2MatchedGoodRing(HDiHitMatch* h)
{
    HRichCutTracklet c("default","default");
    HLinearCategory * cat = (HLinearCategory*)HRichUtilFunc::getCategory(catMatchHit);
    HHitMatch *h1 = (HHitMatch*)cat->getObject(h->getIndTrk1());
    HHitMatch *h2 = (HHitMatch*)cat->getObject(h->getIndTrk2());
    c.switchTo("matchedGoodRing");
    if (c.check(h1) && c.check(h2)) return kTRUE;
    return kFALSE;

}
Bool_t HRichCutPair::is2GoodRing(HDiHitMatch* h)
{
    HRichCutTracklet c("default","default");
    HLinearCategory * cat = (HLinearCategory*)HRichUtilFunc::getCategory(catMatchHit);
    HHitMatch *h1 = (HHitMatch*)cat->getObject(h->getIndTrk1());
    HHitMatch *h2 = (HHitMatch*)cat->getObject(h->getIndTrk2());
    c.switchTo("goodRing");
    if (c.check(h1) && c.check(h2)) return kTRUE;
    return kFALSE;

}
Bool_t HRichCutPair::is2Matched(HDiHitMatch* h)
{
    HRichCutTracklet c("default","default");
    HLinearCategory * cat = (HLinearCategory*)HRichUtilFunc::getCategory(catMatchHit);
    HHitMatch *h1 = (HHitMatch*)cat->getObject(h->getIndTrk1());
    HHitMatch *h2 = (HHitMatch*)cat->getObject(h->getIndTrk2());
    c.switchTo("angularMatch");
    if (c.check(h1) && c.check(h2)) return kTRUE;
    return kFALSE;

}
Bool_t HRichCutPair::is2Fast(HDiHitMatch* h)
{
    HRichCutTracklet c("default","default");
    HLinearCategory * cat = (HLinearCategory*)HRichUtilFunc::getCategory(catMatchHit);
    HHitMatch *h1 = (HHitMatch*)cat->getObject(h->getIndTrk1());
    HHitMatch *h2 = (HHitMatch*)cat->getObject(h->getIndTrk2());
    c.switchTo("betagt0d935");c.switchTo("betalt1d2",kFALSE);
    if (c.check(h1) && c.check(h2)) return kTRUE;
    return kFALSE;

}
Bool_t HRichCutPair::is2Pull(HDiHitMatch* h)
{
    HRichCutTracklet c("default","default");
    HLinearCategory * cat = (HLinearCategory*)HRichUtilFunc::getCategory(catMatchHit);
    HHitMatch *h1 = (HHitMatch*)cat->getObject(h->getIndTrk1());
    HHitMatch *h2 = (HHitMatch*)cat->getObject(h->getIndTrk2());
    c.switchTo("pulllt2");
    if (c.check(h1) && c.check(h2)) return kTRUE;
    return kFALSE;

}
Bool_t HRichCutPair::is2Shower(HDiHitMatch* h)
{
    HRichCutTracklet c("default","default");
    HLinearCategory * cat = (HLinearCategory*)HRichUtilFunc::getCategory(catMatchHit);
    HHitMatch *h1 = (HHitMatch*)cat->getObject(h->getIndTrk1());
    HHitMatch *h2 = (HHitMatch*)cat->getObject(h->getIndTrk2());
    c.switchTo("shower");
    if (c.check(h1) && c.check(h2)) return kTRUE;
    return kFALSE;

}
Bool_t HRichCutPair::is2MdcChi2(HDiHitMatch* h)
{
    HRichCutTracklet c("default","default");
    HLinearCategory * cat = (HLinearCategory*)HRichUtilFunc::getCategory(catMatchHit);
    HHitMatch *h1 = (HHitMatch*)cat->getObject(h->getIndTrk1());
    HHitMatch *h2 = (HHitMatch*)cat->getObject(h->getIndTrk2());
    c.switchTo("mdcchi2");
    if (c.check(h1) && c.check(h2)) return kTRUE;
    return kFALSE;

}
Bool_t HRichCutPair::is2PullMom(HDiHitMatch* h)
{
    HRichCutTracklet c("default","default");
    HLinearCategory * cat = (HLinearCategory*)HRichUtilFunc::getCategory(catMatchHit);
    HHitMatch *h1 = (HHitMatch*)cat->getObject(h->getIndTrk1());
    HHitMatch *h2 = (HHitMatch*)cat->getObject(h->getIndTrk2());
    c.switchTo("pullmom3s");
    if (c.check(h1) && c.check(h2)) return kTRUE;
    return kFALSE;

}
Bool_t HRichCutPair::is2BetaMom(HDiHitMatch* h)
{
    HRichCutTracklet c("default","default");
    HLinearCategory * cat = (HLinearCategory*)HRichUtilFunc::getCategory(catMatchHit);
    HHitMatch *h1 = (HHitMatch*)cat->getObject(h->getIndTrk1());
    HHitMatch *h2 = (HHitMatch*)cat->getObject(h->getIndTrk2());
    c.switchTo("betamom3s");
    if (c.check(h1) && c.check(h2)) return kTRUE;
    return kFALSE;

}
Bool_t HRichCutPair::is2TofWin(HDiHitMatch* h)
{
    HRichCutTracklet c("default","default");
    HLinearCategory * cat = (HLinearCategory*)HRichUtilFunc::getCategory(catMatchHit);
    HHitMatch *h1 = (HHitMatch*)cat->getObject(h->getIndTrk1());
    HHitMatch *h2 = (HHitMatch*)cat->getObject(h->getIndTrk2());
    c.switchTo("tofgt5d0");c.switchTo("toflt9d0",kFALSE);
    if (c.check(h1) && c.check(h2)) return kTRUE;
    return kFALSE;

}
ClassImp(HRichCutPair)
