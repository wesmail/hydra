
#include "hrichcategoryviewer.h"
#include "hrichutilfunc.h"
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
#include <stdlib.h>

ClassImp(HRichCategoryViewer)

HRichCategoryViewer::HRichCategoryViewer(const Text_t *name,const Text_t *title,Int_t category) :
  HReconstructor(name,title)
{
  cout << "constructing category viewer for category: " <<category<< endl;
  fCategory = category;
}

HRichCategoryViewer::HRichCategoryViewer()
{
  
}


HRichCategoryViewer::~HRichCategoryViewer(void) 
{
}

Bool_t HRichCategoryViewer::init() {
    printf("initialization of rich categoryviewer\n");
    return kTRUE;
}

Bool_t HRichCategoryViewer::finalize() {
  return kTRUE;
}

Int_t HRichCategoryViewer::execute()
{
  HRichUtilFunc::dumpCategory(fCategory);
  return 0;
}

