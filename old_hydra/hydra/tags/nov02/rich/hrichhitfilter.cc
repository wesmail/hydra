// $Id: hrichhitfilter.cc,v 1.1 2002-11-28 15:48:00 eberl Exp $
// Last update by Thomas Eberl: 02/11/07 16:22:48
//

#include "hdebug.h"
#include "hrichhitfilter.h"
#include "hrichcorrelatorpar.h"
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

	if(((HRichHit*)r)->getRingPatMat()    >= nPatMatCut    &&
	   ((HRichHit*)r)->getRingHouTra()    >= nHouTraCut    &&
	   ((HRichHit*)r)->getRingAmplitude() >= nRingAmplCut  &&
	   ((HRichHit*)r)->getRingPadNr()     >= nPadNrCut     &&
	   ((HRichHit*)r)->getRingLocalMax4() >= nLocMax4Cut      ) 
	{
	    return kTRUE;
	}
	else 
	{
	    //	cout<<"THROW IT !!!!!!"<<endl;
	    return kFALSE;
	}
}
