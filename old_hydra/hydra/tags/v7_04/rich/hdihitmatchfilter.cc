// $Id: hdihitmatchfilter.cc,v 1.1 2004-10-15 08:15:35 eberl Exp $
// Last update by Thomas Eberl: 03/07/15 17:48:48
//
using namespace std;
#include "hdebug.h"
#include "hdihitmatchfilter.h"
#include "hrichcut.h"
#include "hrichcutpair.h"
#include <iostream>
#include <iomanip>
ClassImp(HDiHitMatchFilter)


    ///////////////////////////////////////////////////
    // 
    // HDiHitMatchFilter is a HFilter to select pairs
    // according to some quality criteria
    //
    //////////////////////////////////////////////////

void HDiHitMatchFilter::initCuts(HParSet* pPar)
{

}

Bool_t HDiHitMatchFilter::check(TObject* r)
{
  // returns kTRUE if the hit has not been deleted. 
  // otherwise, returns kFALSE

    Bool_t kRet =kFALSE;
    HRichCutPair c("default","default");
    c.switchTo("2matched");
    if(
       // 2 sigma matching for exp nov01
       c.check((HDiHitMatch*)r)
       ) 
    {

	kRet=kTRUE;
    }
    else 
    {
	kRet=kFALSE;
    }

    return kRet;
}
