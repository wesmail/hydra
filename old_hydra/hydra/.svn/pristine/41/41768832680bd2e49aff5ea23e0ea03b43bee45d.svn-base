#include "hrichpattern.h"
#include "hrichcal.h"
#include "iostream.h"
#include "TObjString.h"

ClassImp(HRichPattern)


HRichPattern::HRichPattern(const char* name   ="",
		       const char* title  ="",
		       Int_t active=0,Int_t resetable=0): TNamed(name,title) 
{
  fIsActive=active;
  fisResetable=resetable;
  refreshrate=500;
  newPattern = new TList();
  oldPattern = new TList();
  newPattern->SetOwner(kTRUE);
  oldPattern->SetOwner(kTRUE);
}

void HRichPattern::reset()
{
  oldPattern->Clear();
  delete oldPattern;
  oldPattern = newPattern;
  oldPattern->SetOwner(kTRUE);
  newPattern = new TList();
  newPattern->SetOwner(kTRUE);
}

HRichPattern::~HRichPattern(){
  delete oldPattern;
  delete newPattern;
}


