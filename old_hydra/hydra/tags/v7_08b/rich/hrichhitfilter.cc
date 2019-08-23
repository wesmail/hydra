// $Id: hrichhitfilter.cc,v 1.2 2003-01-24 15:43:46 eberl Exp $
// Last update by Thomas Eberl: 03/01/09 19:11:14
//

#include "hdebug.h"
#include "hrichhitfilter.h"
#include "hrichcorrelatorpar.h"
#include "hrichcut.h"
ClassImp(HRichHitFilter)


    ///////////////////////////////////////////////////
    // 
    // HRichHitFilter is a HFilter to select rich hits
    // according to some quality criteria
    //
    //////////////////////////////////////////////////

void HRichHitFilter::initCuts(HParSet* pPar)
{
    nPatMatCut=0;
    // RICH RING QUALITY CUTS
    nPatMatCut   = ((HRichCorrelatorPar*)pPar)->getRingPatMatCut();
    nHouTraCut   = ((HRichCorrelatorPar*)pPar)->getRingHouTraCut();
    nPadNrCut    = ((HRichCorrelatorPar*)pPar)->getRingPadNrCut();
    nRingAmplCut = ((HRichCorrelatorPar*)pPar)->getRingAmplCut();
    nLocMax4Cut  = ((HRichCorrelatorPar*)pPar)->getRingLocMax4Cut();


}

Bool_t HRichHitFilter::check(TObject* r)
{
  // returns kTRUE if the hit has not been deleted. 
  // otherwise, returns kFALSE

// 	if(((HRichHit*)r)->getRingPatMat()    >= nPatMatCut    &&
// 	   ((HRichHit*)r)->getRingHouTra()    >= nHouTraCut    &&
// 	   ((HRichHit*)r)->getRingAmplitude() >= nRingAmplCut  &&
// 	   ((HRichHit*)r)->getRingPadNr()     >= nPadNrCut     &&
// 	   ((HRichHit*)r)->getRingLocalMax4() >= nLocMax4Cut      ) 

// 
    if ( HRichCut::isGoodRing(((HRichHit*)r)) )      

	{
	    return kTRUE;
	}
	else 
	{
	    //	cout<<"THROW IT !!!!!!"<<endl;
	    return kFALSE;
	}
}
