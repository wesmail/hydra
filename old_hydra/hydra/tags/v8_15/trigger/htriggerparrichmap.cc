//*-- Author: Alberica Toia
//*-- Modified: Joern Wuestenfeld 05/11/2005
//*-- Version: $Id: htriggerparrichmap.cc,v 1.6 2006-08-12 13:05:57 halo Exp $

//_HADES_CLASS_DESCRIPTION 
///////////////////////////////////////////////////////////////////////////////
// HTriggerParRichMap
//
// Parameters for TRIGGER Emulation.
// Mapping RICH pad numbers to theta and phi.
//
///////////////////////////////////////////////////////////////////////////////
using namespace std;
#include "htriggerparrichmap.h"
#include "hades.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "htriggerdetector.h"
#include "hpario.h"
#include "hdetpario.h"
#include <iostream> 
#include <iomanip>

ClassImp(HTriggerParRichMap)



void HTriggerParRichMap::setDefaults(void) {
  // sets the default values
  for (Int_t xx=0; xx < 96; xx++) {
    for (Int_t yy=0; yy < 96; yy++) {
      rich_theta[xx][yy]=0.;
      rich_phi[xx][yy]=0.;
    }
  }
}



HTriggerParRichMap::HTriggerParRichMap(const char* name,const char* title,
                       const char* context)
           : HParSet(name,title,context) {
  // The container name is set to "TriggerParRichMap"
  strcpy(detName,"Trigger");
  setDefaults();
}


HTriggerParRichMap::~HTriggerParRichMap(void) {
  // destructor
}
 
Bool_t HTriggerParRichMap::init(HParIo* inp,Int_t* set) {
  // intitializes the container from an input
  HDetParIo* input=inp->getDetParIo("HTriggerParIo");
  if (input) return (input->init(this,set));
  cout << "Did not get input >HTriggerParIo< " << endl;
  return kFALSE;
}
 
Int_t HTriggerParRichMap::write(HParIo* output) {
  // writes the container to an output
  HDetParIo* out=output->getDetParIo("HTriggerParIo");
  if (out) return out->write(this);
  return -1;
}

void HTriggerParRichMap::clear(){
  // sets the default values
  setDefaults();
  status=kFALSE;
  resetInputVersions();
}


void HTriggerParRichMap::readline(const char *buf, Int_t *set) {
  // decodes one line read from ascii file I/O
  Int_t row,col,counts;
  Float_t theta;  
  Float_t phi;

  sscanf(buf,"%i%i%i%f%f",&row,&col,&counts,&theta,&phi);

  if (counts == 0) {
    setRichTheta(row,col,0.);
    setRichPhi(row,col,0.);
  } else {
    if( (row > -1) && (row < 96 ) 
        && (col > -1) && (col < 96 ) ) {
      setRichTheta(row,col,theta);
      //setRichPhi(row,col,30+phi);
      setRichPhi(row,col,phi);
    }
    else {
    }
  }
}

void HTriggerParRichMap::putAsciiHeader(TString& header) {
  // puts the ascii header to the string used in HTriggerParAsciiFileIo
  header=
    "# Parameters for the MU Rich Mapping\n"
    "# Format:\n"
    "# row     col     counts     phi    theta\n";
}

Bool_t HTriggerParRichMap::writeline(char *buf,Int_t s, Int_t m) {
  // writes one line to the buffer used by ascii file I/O
      sprintf(buf,"%i %i %i %f %f\n",s,m,10,getRichTheta(s,m),getRichPhi(s,m));  
  return kTRUE;
}

void HTriggerParRichMap::printPause(void) {
  // pauses printing
  printf("Press any key to continue");
  getchar();
  printf("\n\n");
}

void HTriggerParRichMap::printParam(void) {
  // prints the parameters

  printf("\n Parameters for the MU Rich Mapping\n");
  printf(" Format:\n");
  printf(" col     row     phi     theta\n");
  printf("==============================================================\n");

   for (Int_t xx=0; xx < 96; xx++) {
      for (Int_t yy=0; yy< 96; yy++) {
          cout << xx << "   " << yy << "   " << getRichTheta(xx,yy) << "   " << getRichPhi(xx,yy) << endl;
        }
      }
    
   printf("==============================================================\n");
}



































