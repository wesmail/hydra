///////////////////////////////////////////////////////////////////////////////
// HTriggerParMuLep
//
// Parameters for MU Lepton Emulation
//
///////////////////////////////////////////////////////////////////////////////

#include "htriggerparmulep.h"
#include "hades.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "htriggerdetector.h"
#include "hpario.h"
#include "hdetpario.h"
#include <iostream.h>

ClassImp(HTriggerParMuLep)

void HTriggerParMuLep::setDefaults(void) {
  // sets the default values
  MUtheta=90.;
  MUphi=15.;
}

HTriggerParMuLep::HTriggerParMuLep(const char* name,const char* title,
                       const char* context)
           : HParSet(name,title,context) {
  // The container name is set to "TriggerParMuLep"
  strcpy(detName,"Trigger");
}


HTriggerParMuLep::~HTriggerParMuLep(void) {
  // destructor
  clear();
}
 
Bool_t HTriggerParMuLep::init(HParIo* inp,Int_t* set) {
  // intitializes the container from an input
  HDetParIo* input=inp->getDetParIo("HTriggerParIo");
  if (input) return (input->init(this,set));
  cout << "Did not get input >HTriggerParIo< " << endl;
  return kFALSE;
}
 
Int_t HTriggerParMuLep::write(HParIo* output) {
  // writes the container to an output
  HDetParIo* out=output->getDetParIo("HTriggerParIo");
  if (out) return out->write(this);
  return -1;
}

void HTriggerParMuLep::clear() {
  // sets the default values
  setDefaults();
  status=kFALSE;
  resetInputVersions();
}

void HTriggerParMuLep::readline(const char *buf, Int_t *set) {
  // decodes one line read from ascii file I/O
  Float_t th,ph;
  sscanf(buf,"%f%f",&th,&ph);
  HTriggerParMuLep& par=(*this);
  par.setMUtheta(th);
  par.setMUphi(ph);
}

void HTriggerParMuLep::putAsciiHeader(TString& header) {
  // puts the ascii header to the string used in HTriggerParAsciiFileIo
  header=
    "# Parameters for the MU Lepton emulation\n"
    "# Format:\n"
    "# MUtheta  MUphi\n";
}

Bool_t HTriggerParMuLep::writeline(char *buf,Int_t s, Int_t m, Int_t c) {
  // writes one line to the buffer used by ascii file I/O
  HTriggerParMuLep& par=(*this);
  sprintf(buf,"%f %f\n",
              par.getMUtheta(),par.getMUphi());
  return kTRUE;
}

void HTriggerParMuLep::printPause(void) {
  // pauses printing
  printf("Press any key to continue");
  getchar();
  printf("\n\n");
}

void HTriggerParMuLep::printParam(void) {
  // prints the parameters
  printf("\n Parameters for the MU Lepton emulation\n");
  printf(" Format:\n");
  printf(" MUtheta  MUphi\n");
  printf("==============================================================\n");
 
  printf("TRIGGER PARAMETERS for the MU Lepton Emulation
             MUtheta = %f
             MUphi   = %f\n",
             MUtheta,
             MUphi);
  printf("==============================================================\n");
}





















