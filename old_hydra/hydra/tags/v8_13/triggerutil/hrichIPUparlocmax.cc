//_HADES_CLASS_DESCRIPTION 
///////////////////////////////////////////////////////////////////////////////
// HRichIPUParLocMax
//
// Parameters for RICH IPU LocMax
//
///////////////////////////////////////////////////////////////////////////////
using namespace std;
#include "hrichIPUparlocmax.h"
#include "hades.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "hrichdetector.h"
#include "hpario.h"
#include "hdetpario.h"
#include "hparamlist.h"
#include <iostream>
#include <iomanip>

ClassImp(HRichIPUParLocMax)



void HRichIPUParLocMax::setDefaults(void) {
  // sets the default values
  locmax=0;
  locmax_flag=0;
}


HRichIPUParLocMax::HRichIPUParLocMax(const char* name,const char* title,
                      const char* context)
          : HParCond(name,title,context) {
  // The container name is set to "RichIPUParLocMax"
  strcpy(detName,"Trigger");
}


HRichIPUParLocMax::~HRichIPUParLocMax(void) {
  // destructor
}

Bool_t HRichIPUParLocMax::init(HParIo* inp,Int_t* set) {
  // intitializes the container from an input
  HDetParIo* input=inp->getDetParIo("HTriggerParIo");
  if (input) return (input->init(this,set));
  cout << "Did not get input >HTriggerParIo< " << endl;
  return kFALSE;
}

Int_t HRichIPUParLocMax::write(HParIo* output) {
  // writes the container to an output
  HDetParIo* out=output->getDetParIo("HTriggerParIo");
  if (out) return out->write(this);
  return -1;
}

void HRichIPUParLocMax::putParams(HParamList* l) {
  if (!l) return;
  l->add("locmax",locmax);
  l->add("locmax_flag",locmax_flag);
}

Bool_t HRichIPUParLocMax::getParams(HParamList* l) {
  if (!l) return kFALSE;
  if (!l->fill("locmax",&locmax)) return kFALSE;
  if (!l->fill("locmax_flag",&locmax_flag)) return kFALSE;
  return kTRUE;
}


void HRichIPUParLocMax::clear(){
  // sets the default values
  setDefaults();
  status=kFALSE;
  resetInputVersions();
}


void HRichIPUParLocMax::readline(const char *buf, Int_t *set) {
  // decodes one line read from ascii file I/O
  Int_t fl,lm;
  sscanf(buf,"%i%i",&fl,&lm);
  setLocMax(lm);
  setLocMaxFlag(fl);
}


void HRichIPUParLocMax::putAsciiHeader(TString& header) {
  // puts the ascii header to the string used in HRichIPUParAsciiFileIo
  header=
    "# Parameters for RICH IPU LocMax\n"
    "# Format:\n"
    "# locmax_flag   locmax\n";
}

// Bool_t HRichIPUParLocMax::writeline(char *buf,Int_t s, Int_t m, Int_t c) {
//   // writes one line to the buffer used by ascii file I/O
//   sprintf(buf,"%i%i\n",getLocMaxFlag(),getLocMax());
//   return kTRUE;
// }

void HRichIPUParLocMax::printPause(void) {
  // pauses printing
  printf("Press any key to continue");
  getchar();
  printf("\n\n");
}

void HRichIPUParLocMax::printParam(void) {
  // prints the parameters

  printf("\n Parameters for RICH IPU LocMax\n");
  printf(" Format:\n");
  printf(" locmax_flag   locmax\n");
  printf("==============================================================\n");

  cout << "PRINTPARAM " << getLocMaxFlag() << "   " << getLocMax() << endl;
  printf("==============================================================\n");
}
