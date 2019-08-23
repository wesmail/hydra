///////////////////////////////////////////////////////////////////////////////
// HTriggerParMuDilep
//
// Parameters for TRIGGER Emulation
//
///////////////////////////////////////////////////////////////////////////////

#include "htriggerparmudilep.h"
#include "hades.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "htriggerdetector.h"
#include "hpario.h"
#include "hdetpario.h"
#include "hparamlist.h"
#include <iostream.h>

ClassImp(HTriggerParMuDilep)

void HTriggerParMuDilep::setDefaults(void) {
  // sets the default values
  MUcutoff=16;
}

HTriggerParMuDilep::HTriggerParMuDilep(const char* name,const char* title,
                       const char* context)
           : HParCond(name,title,context) {
  // The container name is set to "TriggerParMuDilep"
  strcpy(detName,"Trigger");
}


HTriggerParMuDilep::~HTriggerParMuDilep(void) {
  // destructor
}
 
Bool_t HTriggerParMuDilep::init(HParIo* inp,Int_t* set) {
  // intitializes the container from an input
  HDetParIo* input=inp->getDetParIo("HTriggerParIo");
  if (input) return (input->init(this,set));
  cout << "Did not get input >HTriggerParIo< " << endl;
  return kFALSE;
}
 
Int_t HTriggerParMuDilep::write(HParIo* output) {
  // writes the container to an output
  HDetParIo* out=output->getDetParIo("HTriggerParIo");
  if (out) return out->write(this);
  return -1;
}

void HTriggerParMuDilep::putParams(HParamList* l) {   
if (!l) return;   
l->add("MUcutoff",MUcutoff); 
}

Bool_t HTriggerParMuDilep::getParams(HParamList* l) {
  if (!l) return kFALSE;
  if (!l->fill("MUcutoff",&MUcutoff)) return kFALSE;  
  return kTRUE;
}



void HTriggerParMuDilep::clear() {
  // sets the default values
  setDefaults();
  status=kFALSE;
  resetInputVersions();
}

void HTriggerParMuDilep::readline(const char *buf, Int_t *set) {
  // decodes one line read from ascii file I/O
  Int_t ct;
  sscanf(buf,"%i",&ct);
  setMUcutoff(ct);
}

void HTriggerParMuDilep::putAsciiHeader(TString& header) {
  // puts the ascii header to the string used in HTriggerParAsciiFileIo
  header=
    "# Parameters for the MU Dilepton emulation\n"
    "# Format:\n"
    "# MUcutoff\n";
}

// Bool_t HTriggerParMuDilep::writeline(char *buf,Int_t s, Int_t m, Int_t c) {
//   // writes one line to the buffer used by ascii file I/O
//   sprintf(buf,"%i\n",
//               getMUcutoff());
//   return kTRUE;
// }

void HTriggerParMuDilep::printPause(void) {
  // pauses printing
  printf("Press any key to continue");
  getchar();
  printf("\n\n");
}

void HTriggerParMuDilep::printParam(void) {
  // prints the parameters
  printf("\n Parameters for the Mu Dilepton emulation\n");
  printf(" Format:\n");
  printf(" MUcutoff\n");
  printf("==============================================================\n");

  printf("TRIGGER PARAMETERS for the MU Dilepton Emulation
             MUcutoff   = %i\n",
             MUcutoff);
  printf("==============================================================\n");
}













