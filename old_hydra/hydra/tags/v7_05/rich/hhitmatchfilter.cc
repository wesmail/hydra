// $Id: hhitmatchfilter.cc,v 1.5 2004-10-15 08:21:47 eberl Exp $
// Last update by Thomas Eberl: 03/07/15 17:38:47
//
using namespace std;
#include "hdebug.h"
#include "hhitmatchfilter.h"
#include "hrichcut.h"
#include "hrichcuttracklet.h"
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
    HRichCutTracklet c("default","default");
    c.switchTo("angularMatch");
    if(
       // 2 sigma matching for exp nov01
       c.check((HHitMatch*)r)
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
