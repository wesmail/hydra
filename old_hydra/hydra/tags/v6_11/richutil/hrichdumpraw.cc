// $Id: hrichdumpraw.cc,v 1.2 2002-09-25 16:19:53 eberl Exp $
// Last update by Thomas Eberl: 02/09/25 17:34:02
//
#include "hrichdumpraw.h"
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

ClassImp(HRichDumpRaw)

HRichDumpRaw::HRichDumpRaw(Text_t *name,Text_t *title) :
  HReconstructor(name,title)
{
    fIter = NULL;
    nCounter = 0;
}

HRichDumpRaw::HRichDumpRaw()
{
    fIter = NULL;
    nCounter = 0;
}


HRichDumpRaw::~HRichDumpRaw(void) {
   if (fIter) delete fIter;
}


Bool_t HRichDumpRaw::init() {

    HRichDetector *pRichDet = (HRichDetector*)gHades->getSetup()
                                                  ->getDetector("Rich");

    fRawCat=gHades->getCurrentEvent()->getCategory(catRichRaw);
    if (!fRawCat) {
      fRawCat=pRichDet->buildCategory(catRichRaw);

      if (!fRawCat) return kFALSE;
      else gHades->getCurrentEvent()
                         ->addCategory(catRichRaw, fRawCat, "Rich");
    }

    fIter = (HMatrixCatIter*)getRawCat()->MakeIterator(); 
    ff = new TFile("hldrawhist.root","RECREATE");
    h1 = new TH2F("colvsrow","colvsrow",100,0,100,100,0,100); 
    return kTRUE;
}

Bool_t HRichDumpRaw::finalize() {
    cout<<nCounter<<" non-empty events processed"<<endl; 
    ff->cd();
    h1->Write();
    ff->Close();
    return kTRUE;
}


Int_t HRichDumpRaw::execute()
{
    cout<<"PROCESSING RICH EVENT : "<<nCounter<<endl;
  Bool_t kValidData = kFALSE;
  HRichRaw *pRaw;
  fIter->Reset(); 
  while((pRaw = (HRichRaw *)fIter->Next()))
  {
      kValidData = kTRUE;

      //      cout<<"evt no:"<<pRaw->getEventNr()<<" charge:"<<pRaw->getCharge()<<" row:"<<pRaw->getRow()<<" col:"<<pRaw->getCol()<<" sec:"<<pRaw->getSector()<<endl;
      pRaw->dumpToStdout();
      h1->Fill(pRaw->getRow(),pRaw->getCol(),pRaw->getCharge());
  }
  if(kValidData) nCounter++;
  return 0;
}
