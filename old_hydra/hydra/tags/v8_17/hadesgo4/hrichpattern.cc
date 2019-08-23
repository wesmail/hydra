using namespace std;
#include "hrichpattern.h"
#include "hrichcal.h"
#include <iostream> 
#include <iomanip>
#include "TObjString.h"

ClassImp(HRichPattern)


HRichPattern::HRichPattern(const Char_t* name,
		       const Char_t* title,
		       Int_t active,Int_t resetable): TNamed(name,title)
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


