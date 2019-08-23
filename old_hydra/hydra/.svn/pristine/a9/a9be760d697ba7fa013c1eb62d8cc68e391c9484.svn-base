///////////////////////////////////////////////////////////////////////////////
// HRichIPUParThresholds
//
// Parameters for RICH IPU Thresholds
//
///////////////////////////////////////////////////////////////////////////////
#include "hrichIPUparthresholds.h"
#include "hades.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "hrichdetector.h"
#include "hpario.h"
#include "hdetpario.h"
#include <iostream.h>

ClassImp(HRichIPUParThresholds)



void HRichIPUParThresholds::setDefaults(void) {
  // sets the default values
  for (Int_t xx=0; xx < 2; xx++) {
    for (Int_t yy=0; yy < 6; yy++) {
	thresholds[xx][yy]=0;
    }
  }
}


HRichIPUParThresholds::HRichIPUParThresholds(const char* name,const char* title,
                       const char* context)
           : HParSet(name,title,context) {
  // The container name is set to "RichIPUParThresholds"
  strcpy(detName,"Trigger");
}


HRichIPUParThresholds::~HRichIPUParThresholds(void) {
  // destructor
}
 
Bool_t HRichIPUParThresholds::init(HParIo* inp,Int_t* set) {
  // intitializes the container from an input
  HDetParIo* input=inp->getDetParIo("HTriggerParIo");
  if (input) return (input->init(this,set));
  cout << "Did not get input >HTriggerParIo< " << endl;
  return kFALSE;
}
 
Int_t HRichIPUParThresholds::write(HParIo* output) {
  // writes the container to an output
  HDetParIo* out=output->getDetParIo("HTriggerParIo");
  if (out) return out->write(this);
  return -1;
}

void HRichIPUParThresholds::clear(){
  // sets the default values
  setDefaults();
  status=kFALSE;
  resetInputVersions();
}


void HRichIPUParThresholds::readline(const char *buf, Int_t *set) {
  // decodes one line read from ascii file I/O
  Int_t sec,re;
  Int_t thr;


  sscanf(buf,"%i%i%i",&re,&sec,&thr);
  
  if( (re >= 0) && (re <= 1 ) 
   && (sec>= 0) && (sec<= 5 ) ) {
    setThresholds(re,sec,thr);
  } 
  else {
    cout << "variables not properly set " << endl;
  }
  
}



void HRichIPUParThresholds::putAsciiHeader(TString& header) {
  // puts the ascii header to the string used in HRichIPUParAsciiFileIo
  header=
    "# Parameters for RICH IPU Thresholds\n"
    "# Format:\n"
    "# region sector  threshold\n";
}

Bool_t HRichIPUParThresholds::writeline(char *buf,Int_t s, Int_t m) {
  // writes one line to the buffer used by ascii file I/O
 	sprintf(buf,"%i %i %i\n",s,m,getThresholds(s,m));	
  return kTRUE;
}

void HRichIPUParThresholds::printPause(void) {
  // pauses printing
  printf("Press any key to continue");
  getchar();
  printf("\n\n");
}

void HRichIPUParThresholds::printParam(void) {
  // prints the parameters

  printf("\n Parameters for RICH IPU Thresholds\n");
  printf(" Format:\n");
  printf(" region  sector  threshold\n");
  printf("==============================================================\n");

   for (Int_t xx=0; xx < 2; xx++) {
      for (Int_t yy=0; yy < 6; yy++) {
	  cout << "PRINTPARAM " << xx << "   " << yy << "   " << getThresholds(xx,yy) << endl;
      }
    }
  printf("==============================================================\n");
}



































