#include "hshowerdigitizer.h"

#include "hcategory.h"
#include "hlinearcatiter.h"
#include "hlocation.h"
#include "hshowerdigidetpar.h"
#include "hdebug.h"
#include "hades.h"
#include "showerdef.h"


ClassImp(HShowerDigitizer)

HShowerDigitizer::HShowerDigitizer(Text_t *name,Text_t *title) :
  HReconstructor(name,title)
{
  lNrEvent = 0;
  m_pDigiParSet=NULL;
}

HShowerDigitizer::HShowerDigitizer()
{
  lNrEvent = 0;
  m_pDigiParSet=NULL;
}


HShowerDigitizer::~HShowerDigitizer(void) {
}

Bool_t HShowerDigitizer::init() {
  return kTRUE;
}

HShowerDigitizer& HShowerDigitizer::operator=(HShowerDigitizer &c) {
  return c;
}

Int_t HShowerDigitizer::execute()
{
  HLinearCatIter *iter;
  HDataObject *pHit;

  lNrEvent++;
  iter = (HLinearCatIter*)getInCat()->MakeIterator();
  Int_t n = 0;

  while((pHit = (HDataObject*)iter->Next()))
  {
     digitize(pHit);
     n++;
  }

  return n;
}


