// $Id: hrichdumpcal.cc,v 1.4 2003-01-24 18:24:25 halo Exp $
// Last update by Thomas Eberl: 02/09/25 17:32:59
//
using namespace std;
#include "hrichdumpcal.h"
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
#include <iostream> 
#include <iomanip>

ClassImp(HRichDumpCal)

HRichDumpCal::HRichDumpCal(Text_t *name,Text_t *title) :
  HReconstructor(name,title)
{
    fIter = NULL;
    nCounter = 0;
}

HRichDumpCal::HRichDumpCal()
{
    fIter = NULL;
    nCounter = 0;
}


HRichDumpCal::~HRichDumpCal(void) {
   if (fIter) delete fIter;
}


Bool_t HRichDumpCal::init() {

    HRichDetector *pRichDet = (HRichDetector*)gHades->getSetup()
                                                  ->getDetector("Rich");

    fCalCat=gHades->getCurrentEvent()->getCategory(catRichCal);
    if (!fCalCat) {
      fCalCat=pRichDet->buildCategory(catRichCal);

      if (!fCalCat) return kFALSE;
      else gHades->getCurrentEvent()
                         ->addCategory(catRichCal, fCalCat, "Rich");
    }

    fIter = (HMatrixCatIter*)getCalCat()->MakeIterator(); 
    return kTRUE;
}

Bool_t HRichDumpCal::finalize() {

    return kTRUE;
}


Int_t HRichDumpCal::execute()
{
    cout<<"PROCESSING RICH EVENT : "<<nCounter<<endl;
  Bool_t kValidData = kFALSE;
  HRichCal *pCal;
  fIter->Reset(); 
  while((pCal = (HRichCal *)fIter->Next()))
  {
      kValidData = kTRUE;
      cout<<"evt no:"<<pCal->getEventNr()<<" charge:"<<pCal->getCharge()<<" row:"<<pCal->getRow()<<" col:"<<pCal->getCol()<<" sec:"<<pCal->getSector()<<endl;
  }
  if(kValidData) nCounter++;
  return 0;
}
