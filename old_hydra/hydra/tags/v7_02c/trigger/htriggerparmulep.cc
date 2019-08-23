///////////////////////////////////////////////////////////////////////////////
// HTriggerParMuLep
//
// Parameters for MU Lepton Emulation
//
///////////////////////////////////////////////////////////////////////////////
using namespace std;
#include "htriggerparmulep.h"
#include "hades.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "htriggerdetector.h"
#include "hpario.h"
#include "hdetpario.h"
#include "hparamlist.h"  
#include <iostream> 
#include <iomanip>

ClassImp(HTriggerParMuLep)


void HTriggerParMuLep::setDefaults(void) {
  // sets the default values
  MUtheta=255;
  MUphi_offset=34;
  MUphi_slope=4;
  MUfield=500;
  MUtof=20;
}

HTriggerParMuLep::HTriggerParMuLep(const char* name,const char* title,
                       const char* context)
           : HParCond(name,title,context) {
  // The container name is set to "TriggerParMuLep"
  strcpy(detName,"Trigger");
}


HTriggerParMuLep::~HTriggerParMuLep(void) {
  // destructor
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


void HTriggerParMuLep::putParams(HParamList* l) {   
if (!l) return;   
l->add("MUtheta",MUtheta); 
l->add("MUphi_offset",MUphi_offset); 
l->add("MUphi_slope",MUphi_slope); 
l->add("MUfield",MUfield); 
l->add("MUtof",MUtof); 
}

Bool_t HTriggerParMuLep::getParams(HParamList* l) {
  if (!l) return kFALSE;
  if (!l->fill("MUtheta",&MUtheta)) return kFALSE;  
  if (!l->fill("MUphi_offset",&MUphi_offset)) return kFALSE;  
  if (!l->fill("MUphi_slope",&MUphi_slope)) return kFALSE;  
  if (!l->fill("MUfield",&MUfield)) return kFALSE;  
  if (!l->fill("MUtof",&MUtof)) return kFALSE;  
  return kTRUE;
}


void HTriggerParMuLep::clear() {
  // sets the default values
  setDefaults();
  status=kFALSE;
  resetInputVersions();
}

void HTriggerParMuLep::readline(const char *buf, Int_t *set) {
  // decodes one line read from ascii file I/O
  Int_t th,phO,phS,ff;
  Float_t tof;
  sscanf(buf,"%i%i%i%i%f",&th,&phO,&phS,&ff,&tof);
  setMUtheta(th);
  setMUphiOffset(phO);
  setMUphiSlope(phS);
  setMUfield(ff);
  setMUtof(tof);
}

void HTriggerParMuLep::putAsciiHeader(TString& header) {
  // puts the ascii header to the string used in HTriggerParAsciiFileIo
  header=
    "# Parameters for the MU Lepton emulation\n"
    "# Format:\n"
    "# MUtheta  MUphi_offset  MUphi_slope  MUfield  MUtof\n";
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
  printf(" MUtheta  MUphi_offset  MUphi_slope  MUfield  MUtof\n");
  printf("==============================================================\n");

  printf("TRIGGER PARAMETERS for the MU Lepton Emulation \
              MUtheta        = %i \
              MUphi_offset   = %i \
              MUphi_slope    = %i \
              MUfield        = %i \
              MUtof          = %f\n",
	 MUtheta,
	 MUphi_offset,
	 MUphi_slope,
         MUfield,
	 MUtof);
  printf("==============================================================\n");
}






















