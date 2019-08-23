// $Id: hrichhistofiller.cc,v 1.2 2002-09-25 16:19:53 eberl Exp $
// Last update by Thomas Eberl: 02/09/25 17:35:14
//
#include "hrichhistofiller.h"
#include "hrichhit.h"
#include "hruntimedb.h"
#include "hevent.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hrichdetector.h"
#include "hcategory.h"
#include "hiterator.h"
#include "hmatrixcatiter.h"
#include "hlocation.h"
#include "hrichraw.h"
#include "hrichcalpar.h"
#include "hrichcal.h"
#include "hdebug.h"
#include "hades.h"
#include "richdef.h"
#include <iostream.h>

ClassImp(HRichHistoFiller)

HRichHistoFiller::HRichHistoFiller(Text_t *name,Text_t *title) :
  HReconstructor(name,title)
{
    fIter = NULL;
}

HRichHistoFiller::HRichHistoFiller()
{
    fIter = NULL;
}


HRichHistoFiller::~HRichHistoFiller(void) {
   if (fIter) delete fIter;
}


Bool_t HRichHistoFiller::init() {

    HRichDetector *pRichDet = (HRichDetector*)gHades->getSetup()
                                                  ->getDetector("Rich");

    fHitCat=gHades->getCurrentEvent()->getCategory(catRichHit);
    if (!fHitCat) {
      fHitCat=pRichDet->buildCategory(catRichHit);

      if (!fHitCat) return kFALSE;
      else gHades->getCurrentEvent()
                         ->addCategory(catRichHit, fHitCat, "Rich");
    }

    fIter = (HMatrixCatIter*)getHitCat()->MakeIterator(); 

    outfile = new TFile("richhistos.root","RECREATE");
    richtheta = new TH1F("RICH_theta","RICH_theta",90,1,90);

    return kTRUE;
}

Bool_t HRichHistoFiller::finalize() {
    outfile->cd();
    richtheta->Write();
    outfile->Close();
    
    return kTRUE;
}


Int_t HRichHistoFiller::execute()
{
  HRichHit *pHit;
  fIter->Reset(); 
  while((pHit = (HRichHit *)fIter->Next()))
  {
      richtheta->Fill(pHit->getTheta());
  }
  
  return 0;
}
