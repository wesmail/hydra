// $Id: hhitmatchfilter.cc,v 1.1 2002-11-28 15:47:26 eberl Exp $
// Last update by Thomas Eberl: 02/11/26 21:39:14
//

#include "hdebug.h"
#include "hhitmatchfilter.h"
#include "hrichcut.h"
#include <iostream.h>
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
       HRichCut::isGoodRing(((HHitMatch*)r))               &&               
       HRichCut::isRMThetaDiff(((HHitMatch*)r),5.)         &&
       HRichCut::isTOFBetween(((HHitMatch*)r),5.,10.)      
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
