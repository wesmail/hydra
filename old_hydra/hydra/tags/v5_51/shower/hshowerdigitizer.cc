#include "hiterator.h"
#include "hruntimedb.h"
#include "hevent.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hshowerdetector.h"
#include "hcategory.h"
#include "hlinearcatiter.h"
#include "hlocation.h"
#include "hshowerdigidetpar.h"
#include "hshowergeometry.h"
#include "hdebug.h"
#include "hades.h"
#include "showerdef.h"
#include "hshowerdigitizer.h"


ClassImp(HShowerDigitizer)

HShowerDigitizer::HShowerDigitizer(Text_t *name,Text_t *title) :
  HReconstructor(name,title)
{
  lNrEvent = 0;
  m_pDigiParSet=NULL;
  m_pGeometry=NULL;
  fIter = NULL;
}

HShowerDigitizer::HShowerDigitizer()
{
  lNrEvent = 0;
  m_pDigiParSet=NULL;
  m_pGeometry=NULL;
  fIter = NULL;
}


HShowerDigitizer::~HShowerDigitizer(void) {
  if (fIter) delete fIter;
}

Bool_t HShowerDigitizer::init() {
  fIter = (HIterator*)getInCat()->MakeIterator("native");
  
  return initParameters();
}

Bool_t HShowerDigitizer::initParameters() {
    HRuntimeDb* rtdb=gHades->getRuntimeDb();

    HShowerGeometry *pGeom = (HShowerGeometry*)rtdb->
                                               getContainer("ShowerGeometry");
    setGeometry(pGeom);
    if (!pGeom) return kFALSE;

    HShowerDigiDetPar *pDigiDetPar = (HShowerDigiDetPar*)rtdb->
                                               getContainer("ShowerDigiPar");
    setDigiParSet(pDigiDetPar);
    if (!pDigiDetPar) return kFALSE;

    return kTRUE;
}

HShowerDigitizer& HShowerDigitizer::operator=(HShowerDigitizer &c) {
  return c;
}

Int_t HShowerDigitizer::execute()
{
  TObject *pHit;

  lNrEvent++;
  fIter->Reset();
  Int_t n = 0;

  while((pHit = fIter->Next()))
  {
     digitize(pHit);
     n++;
  }

  sort();   // this is used to call the track sort() in pad digitizer

  return 0;
}


