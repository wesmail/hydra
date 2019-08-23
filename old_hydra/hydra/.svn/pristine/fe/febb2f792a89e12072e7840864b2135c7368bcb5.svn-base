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
  HTriggerParMomMap& par=(*this);  
  for (Int_t xx=0; xx < par.getTheta_bins(); xx++) {
    for (Int_t yy=0; yy < par.getDeltatheta_bins(); yy++) {
      for (Int_t zz=0; zz < par.getPhi_bins(); zz++) {
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
   clear();
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

  HTriggerParMomMap& par=(*this);
  
  mom = -111; // dummy value
  sscanf(buf,"%i%i%i%f",&th,&Delta_th,&ph,&mom);
  
  if (mom == -111)  {
    theta =th;
    phi=ph;
    Delta_theta=th;
    par.setTheta_bins(th);      
    par.setDeltatheta_bins(Delta_th);
    par.setPhi_bins(ph);
    cout << "READLINE theta bins " << th << "  delta theta bins " << Delta_th << "  phi bins " << ph << endl;
  } else {
    if( (Delta_th > -1) && (Delta_th <  par.getTheta_bins() ) 
       && (th > -1) && (th < par.getDeltatheta_bins() ) 
       && (ph > -1) && (ph < par.getPhi_bins() ) ) {
      par.setMom(th,Delta_th,ph,mom);
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
  HTriggerParMomMap& par=(*this);
  cout << "WRITELINE theta bins " << par.getTheta_bins() << "  delta theta bins " << par.getDeltatheta_bins() 
       << "  phi bins " << par.getPhi_bins() << endl;
 
  for (Int_t xx=0; xx < par.getTheta_bins(); xx++) {
    for (Int_t yy=0; yy < par.getDeltatheta_bins(); yy++) {
      for (Int_t zz=0; zz < par.getPhi_bins(); zz++) {
	sprintf(buf,"%i%i%i%f\n",
		xx,yy,zz,par.getMom(xx,yy,zz));	
      }
    }
  }
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
  HTriggerParMomMap& par=(*this);

  printf("\n Parameters for the MU Momentun Mapping\n");
  printf(" Format:\n");
  printf(" theta bins  Delta_theta bins  phi bins\n");
  printf(" theta       Delta_theta       phi       momentum\n");
  printf("==============================================================\n");

  cout << "PRINTPARAM theta bins " << par.getTheta_bins() << "  delta theta bins " << par.getDeltatheta_bins() 
       << "  phi bins " << par.getPhi_bins() << endl;
  cout << "mom[31,34,6] " << par.getMom(31,34,6) << endl;
   for (Int_t xx=0; xx < 1; xx++) {
      for (Int_t yy=0; yy< 1; yy++) {
        for (Int_t zz=0; zz< 1; zz++) {
	  cout << xx << "   " << yy << "   " << zz << "   " << par.getMom(xx,yy,zz) << endl;
        }
      }
    }
  printf("==============================================================\n");
}



































