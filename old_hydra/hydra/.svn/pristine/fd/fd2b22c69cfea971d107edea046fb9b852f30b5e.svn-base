#include "hrichdigitizer.h"

#include "hcategory.h"
#include "hlinearcatiter.h"
#include "hlocation.h"
#include "hrichdigidetpar.h"
#include "hdebug.h"
#include "hades.h"
#include "richdef.h"


ClassImp(HRichDigitizer)

HRichDigitizer::HRichDigitizer(Text_t *name,Text_t *title) :
  HReconstructor(name,title)
{
  init();
  fIter = NULL;
}

HRichDigitizer::HRichDigitizer()
{
  init();
  fIter = NULL;
}


HRichDigitizer::~HRichDigitizer(void) {
  if (fIter) delete fIter;
}

Bool_t HRichDigitizer::init() {
  lNrEvent = 0;
  m_pDigiParSet=NULL;

  fIter = (HLinearCatIter*)getInCat()->MakeIterator();
  return kTRUE;
}

HRichDigitizer& HRichDigitizer::operator=(HRichDigitizer &c) {
  return c;
}

Int_t HRichDigitizer::execute()
{
  HDataObject *pHit;

  lNrEvent++;
  Int_t n = 0;

  fIter->Reset();
  while((pHit = (HDataObject*)fIter->Next()))
  {
     digitize(pHit);
     n++;
  }
  return n;
}


