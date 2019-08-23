///////////////////////////////////////////////////////////////////////////////
// HTriggerParShowerMap
//
// Parameters for TRIGGER Emulation
//
///////////////////////////////////////////////////////////////////////////////
#include "htriggerparshowermap.h"
#include "hades.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "htriggerdetector.h"
#include "hpario.h"
#include "hdetpario.h"
#include <iostream.h>

ClassImp(HTriggerParShowerMap)



void HTriggerParShowerMap::setDefaults(void) {
  // sets the default values
  for (Int_t xx=0; xx < 32; xx++) {
    for (Int_t yy=0; yy < 32; yy++) {
      shower_theta[xx][yy]=0;
      shower_phi[xx][yy]=0;
    }
  }
}



HTriggerParShowerMap::HTriggerParShowerMap(const char* name,const char* title,
                       const char* context)
           : HParSet(name,title,context) {
  // The container name is set to "TriggerParShowerMap"
  strcpy(detName,"Trigger");
}


HTriggerParShowerMap::~HTriggerParShowerMap(void) {
  // destructor
}
 
Bool_t HTriggerParShowerMap::init(HParIo* inp,Int_t* set) {
  // intitializes the container from an input
  HDetParIo* input=inp->getDetParIo("HTriggerParIo");
  if (input) return (input->init(this,set));
  cout << "Did not get input >HTriggerParIo< " << endl;
  return kFALSE;
}
 
Int_t HTriggerParShowerMap::write(HParIo* output) {
  // writes the container to an output
  HDetParIo* out=output->getDetParIo("HTriggerParIo");
  if (out) return out->write(this);
  return -1;
}

void HTriggerParShowerMap::clear(){
  // sets the default values
  setDefaults();
  status=kFALSE;
  resetInputVersions();
}


void HTriggerParShowerMap::readline(const char *buf, Int_t *set) {
  // decodes one line read from ascii file I/O
  Int_t row,col;
  Float_t theta;  
  Float_t phi;

  sscanf(buf,"%i%i%f%f",&col,&row,&phi,&theta);
 
//      if (phi>300.0) {
//        phi-=330.0;
//      } else if(phi<40) {
//        phi+=30; 
//      } else {
//        cout << "ERROR: Shower phi= " << phi << endl;
//      }

    if( (row > -1) && (row < 32 ) 
	&& (col > -1) && (col < 32 ) ) {
      setShowerTheta(col,row,theta);
      setShowerPhi(col,row,phi);
    }
    else {
      cout << "variables not properly set " << endl;
    }
  
}

void HTriggerParShowerMap::putAsciiHeader(TString& header) {
  // puts the ascii header to the string used in HTriggerParAsciiFileIo
  header=
    "# Parameters for the MU Shower Mapping\n"
    "# Format:\n"
    "# col     row     phi    theta\n";
}

Bool_t HTriggerParShowerMap::writeline(char *buf,Int_t c, Int_t r) {
  // writes one line to the buffer used by ascii file I/O
      sprintf(buf,"%i %i %f %f\n", c,r,getShowerPhi(c,r),getShowerTheta(c,r));
  return kTRUE;
}

void HTriggerParShowerMap::printPause(void) {
  // pauses printing
  printf("Press any key to continue");
  getchar();
  printf("\n\n");
}

void HTriggerParShowerMap::printParam(void) {
  // prints the parameters

  printf("\n Parameters for the MU Shower Mapping\n");
  printf(" Format:\n");
  printf(" col     row     phi     theta\n");
  printf("==============================================================\n");

   for (Int_t xx=0; xx < 32; xx++) {
      for (Int_t yy=0; yy< 32; yy++) {
	cout << xx << "   " << yy << "   " << getShowerPhi(xx,yy) << "   " << getShowerTheta(xx,yy) << endl;
        }
      }
    
   printf("==============================================================\n");
}



































