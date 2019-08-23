#include "hshowerhistogram.h"
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


ClassImp(HShowerHistogram)

HShowerHistogram::HShowerHistogram(Text_t *name,Text_t *title) :
  HReconstructor(name,title)
{
  fIter = NULL;
}

HShowerHistogram::HShowerHistogram()
{
  fIter = NULL;
}


HShowerHistogram::~HShowerHistogram(void) {
  if (fIter) delete fIter;
}

Bool_t HShowerHistogram::init() {
  
  return kTRUE; 
}

Bool_t HShowerHistogram::finalize(void) {
        printf("Finalize()\n");
        return kTRUE;
}                  

Int_t HShowerHistogram::execute()
{
  return 0;
}

Int_t HShowerHistogram::draw()
{
printf("The function draw() is still under construction\n" ); 
return 0;
}


