// $Id: hhitmatchfilter.cc,v 1.4 2003-01-24 18:00:59 halo Exp $
// Last update by Thomas Eberl: 03/01/13 17:34:30
//
using namespace std;
#include "hdebug.h"
#include "hhitmatchfilter.h"
#include "hrichcut.h"
#include <iostream>
#include <iomanip>
ClassImp(HHitMatchFilter)


    ///////////////////////////////////////////////////
    // 
    // HHitMatchFilter is a HFilter to select tracklets
    // according to some quality criteria
    //
    //////////////////////////////////////////////////

void HHitMatchFilter::initCuts(HParSet* pPar)
{

}

Bool_t HHitMatchFilter::check(TObject* r)
{
  // returns kTRUE if the hit has not been deleted. 
  // otherwise, returns kFALSE

    Bool_t kRet =kFALSE;
    if(
       HRichCut::isGoodRing(((HHitMatch*)r))                &&
       // 2 sigma matching for exp nov01
       HRichCut::isRMThetaDiff(((HHitMatch*)r),1.7)         &&
       HRichCut::isRMPhiDiff(((HHitMatch*)r),1.8)
       // in principle one should *not* cut on TOF
       // HRichCut::isTOFBetween(((HHitMatch*)r),5.,10.)      
       ) 
    {

	kRet=kTRUE;
    }
    else 
    {
	//((HHitMatch*)r)->dumpToStdout();
	//	cout<<"THROW IT !!!!!!"<<endl;
	kRet=kFALSE;
    }

    return kRet;
}
