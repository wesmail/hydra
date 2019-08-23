// $Id: hpidfilter.cc,v 1.1 2003-02-26 08:52:14 eberl Exp $
// Last update by Thomas Eberl: 03/02/26 09:21:42
//
using namespace std;
#include "hdebug.h"
#include "hpidfilter.h"
#include "hpidcuto.h"
#include <iostream>
#include <iomanip>
ClassImp(HPidFilter)


    ///////////////////////////////////////////////////
    // 
    // HPidFilter is a HFilter to select identified particles
    // according to some quality criteria
    //
    //////////////////////////////////////////////////

void HPidFilter::initCuts(HParSet* pPar)
{

}
void HPidFilter::initCuts(HPidCutO* c)
{
    pcut = c;
}

Bool_t HPidFilter::check(TObject* r)
{
  // returns kTRUE if the hit has not been deleted. 
  // otherwise, returns kFALSE

    Bool_t kRet =kFALSE;

    if(pcut->check(r)) kRet=kTRUE;

    return kRet;
}
