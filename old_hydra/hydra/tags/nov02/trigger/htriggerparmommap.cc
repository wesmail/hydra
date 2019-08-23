///////////////////////////////////////////////////////////////////////////////
// HTriggerParMomMap
//
// Parameters for TRIGGER Emulation
//
///////////////////////////////////////////////////////////////////////////////
#include "htriggerparmommap.h"
#include "hades.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "htriggerdetector.h"
#include "hpario.h"
#include "hdetpario.h"
#include <iostream.h>

ClassImp(HTriggerParMomMap)



void HTriggerParMomMap::setDefaults(void) {
  // sets the default values
  for (Int_t xx=0; xx < getTheta_bins(); xx++) {
    for (Int_t yy=0; yy < getDeltatheta_bins(); yy++) {
      for (Int_t zz=0; zz < getPhi_bins(); zz++) {
	mu_mom[xx][yy][zz]=0;
      }
    }
  }
}


HTriggerParMomMap::HTriggerParMomMap(const char* name,const char* title,
                       const char* context)
           : HParSet(name,title,context) {
  // The container name is set to "TriggerParMomMap"
  strcpy(detName,"Trigger");
}


HTriggerParMomMap::~HTriggerParMomMap(void) {
  // destructor
}
 
Bool_t HTriggerParMomMap::init(HParIo* inp,Int_t* set) {
  // intitializes the container from an input
  HDetParIo* input=inp->getDetParIo("HTriggerParIo");
  if (input) return (input->init(this,set));
  cout << "Did not get input >HTriggerParIo< " << endl;
  return kFALSE;
}
 
Int_t HTriggerParMomMap::write(HParIo* output) {
  // writes the container to an output
  HDetParIo* out=output->getDetParIo("HTriggerParIo");
  if (out) return out->write(this);
  return -1;
}

void HTriggerParMomMap::clear(){
  // sets the default values
  setDefaults();
  status=kFALSE;
  resetInputVersions();
}


void HTriggerParMomMap::readline(const char *buf, Int_t *set) {
  // decodes one line read from ascii file I/O
  Int_t th,ph,Delta_th;
  Int_t theta,phi,Delta_theta;
  Float_t mom;

  
  mom = -111; // dummy value
  sscanf(buf,"%i%i%i%f",&th,&Delta_th,&ph,&mom);
  
  if (mom == -111)  {
    theta =th;
    phi=ph;
    Delta_theta=th;
    setTheta_bins(th);      
    setDeltatheta_bins(Delta_th);
    setPhi_bins(ph);
    cout << "READLINE theta bins " << th << "  delta theta bins " << Delta_th << "  phi bins " << ph << endl;
  } else {
    if( (Delta_th > -1) && (Delta_th <  getTheta_bins() ) 
       && (th > -1) && (th < getDeltatheta_bins() ) 
       && (ph > -1) && (ph < getPhi_bins() ) ) {
      setMom(th,Delta_th,ph,mom);
    } 
    else {
      cout << "variables not properly set " << endl;
    }
  }
}

void HTriggerParMomMap::putAsciiHeader(TString& header) {
  // puts the ascii header to the string used in HTriggerParAsciiFileIo
  header=
    "# Parameters for the MU Momentum Mapping\n"
    "# Format:\n"
    "# theta bins  Delta_theta bins  phi bins\n"
    "# theta       Delta_theta       phi       momentum\n";
}

Bool_t HTriggerParMomMap::writeline(char *buf,Int_t s, Int_t m, Int_t c) {
  // writes one line to the buffer used by ascii file I/O
	sprintf(buf,"%i %i %i %f\n",s,m,c,getMom(s,m,c));	
  return kTRUE;
}

Bool_t HTriggerParMomMap::writesize(char *buf) {
  // writes one line to the buffer used by ascii file I/O
	sprintf(buf,"%i %i %i\n",getTheta_bins(),getDeltatheta_bins(),getPhi_bins());	
  return kTRUE;
}

void HTriggerParMomMap::printPause(void) {
  // pauses printing
  printf("Press any key to continue");
  getchar();
  printf("\n\n");
}

void HTriggerParMomMap::printParam(void) {
  // prints the parameters

  printf("\n Parameters for the MU Momentun Mapping\n");
  printf(" Format:\n");
  printf(" theta bins  Delta_theta bins  phi bins\n");
  printf(" theta       Delta_theta       phi       momentum\n");
  printf("==============================================================\n");

  cout << "PRINTPARAM theta bins " << getTheta_bins() << "  delta theta bins " << getDeltatheta_bins() 
       << "  phi bins " << getPhi_bins() << endl;

   for (Int_t xx=0; xx < getTheta_bins(); xx++) {
      for (Int_t yy=0; yy< getDeltatheta_bins(); yy++) {
        for (Int_t zz=0; zz< getPhi_bins(); zz++) {
	  cout << xx << "   " << yy << "   " << zz << "   " << getMom(xx,yy,zz) << endl;
        }
      }
    }
  printf("==============================================================\n");
}



































