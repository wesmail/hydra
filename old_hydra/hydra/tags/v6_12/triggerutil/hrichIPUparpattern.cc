///////////////////////////////////////////////////////////////////////////////
// HRichIPUParPattern
//
// Parameters for RICH IPU Pattern
//
///////////////////////////////////////////////////////////////////////////////
using namespace std;
#include "hrichIPUparpattern.h"
#include "hades.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "hrichdetector.h"
#include "hpario.h"
#include "hdetpario.h"
#include <iostream> 
#include <iomanip>

ClassImp(HRichIPUParPattern)



void HRichIPUParPattern::setDefaults(void) {
  // sets the default values
  for (Int_t xx=0; xx < getNGroups(); xx++) {
    for (Int_t yy=0; yy < getNPadsinGroup(); yy++) {
      for (Int_t zz=0; zz < getNPadDimensions(); zz++) {
	pattern[xx][yy][zz]=0;
      }
    }
  }
}


HRichIPUParPattern::HRichIPUParPattern(const char* name,const char* title,
                       const char* context)
           : HParSet(name,title,context) {
  // The container name is set to "RichIPUParPattern"
  strcpy(detName,"Trigger");
}


HRichIPUParPattern::~HRichIPUParPattern(void) {
  // destructor
}
 
Bool_t HRichIPUParPattern::init(HParIo* inp,Int_t* set) {
  // intitializes the container from an input
  HDetParIo* input=inp->getDetParIo("HTriggerParIo");
  if (input) return (input->init(this,set));
  cout << "Did not get input >HTriggerParIo< " << endl;
  return kFALSE;
}
 
Int_t HRichIPUParPattern::write(HParIo* output) {
  // writes the container to an output
  HDetParIo* out=output->getDetParIo("HTriggerParIo");
  if (out) return out->write(this);
  return -1;
}

void HRichIPUParPattern::clear(){
  // sets the default values
  setDefaults();
  status=kFALSE;
  resetInputVersions();
}


void HRichIPUParPattern::readline(const char *buf, Int_t *set) {
  // decodes one line read from ascii file I/O
  Int_t gr,pg,re;
  Int_t pat;

  pat = -111; // dummy value
  sscanf(buf,"%i%i%i%i",&gr,&pg,&re,&pat);
  
  if (pat == -111)  {
    setNGroups(gr);      
    setNPadsinGroup(pg);
    setNPadDimensions(re);
  } else {
    if( (gr > -1) && (gr <  getNGroups() ) 
	&& (pg > -1) && (pg < getNPadsinGroup() ) 
	&& (re > -1) && (re < getNPadDimensions() ) ) {
      setPattern(gr,pg,re,pat);
    } else {
      cout << "variables not properly set " << endl;
    }
  }
}



void HRichIPUParPattern::putAsciiHeader(TString& header) {
  // puts the ascii header to the string used in HRichIPUParAsciiFileIo
  header=
    "# Parameters for RICH IPU Pattern\n"
    "# Format:\n"
    "# number_of_groups number_of_pads_in_group number_of_regions\n"
    "# group  pad  region  pattern\n";
}

Bool_t HRichIPUParPattern::writeline(char *buf,Int_t s, Int_t m, Int_t c) {
  // writes one line to the buffer used by ascii file I/O
  sprintf(buf,"%i %i %i %i\n",s,m,c,getPattern(s,m,c));
  return kTRUE;
}

Bool_t HRichIPUParPattern::writesize(char *buf) {
  // writes one line to the buffer used by ascii file I/O
  sprintf(buf,"%i %i %i\n",getNGroups(),getNPadsinGroup(),getNPadDimensions());
  return kTRUE;
}

void HRichIPUParPattern::printPause(void) {
  // pauses printing
  printf("Press any key to continue");
  getchar();
  printf("\n\n");
}

void HRichIPUParPattern::printParam(void) {
  // prints the parameters

  printf("\n Parameters for RICH IPU Pattern\n");
  printf(" Format:\n");
  printf(" number_of_groups number_of_pads_in_group number_of_regions\n");
  printf(" group  pad  region  pattern\n");
  printf("==============================================================\n");

  cout << " number of groups " << getNGroups()
       << "     number of pads in group " << getNPadsinGroup()
       << "     number of regions " << getNPadDimensions() << endl;
   for (Int_t xx=0; xx < getNGroups(); xx++) {
      for (Int_t yy=0; yy < getNPadsinGroup(); yy++) {
        for (Int_t zz=0; zz< getNPadDimensions(); zz++) {
	  cout << "PRINTPARAM " << xx << "   " << yy << "   " << zz << "   " << getPattern(xx,yy,zz) << endl;
        }
      }
    }
  printf("==============================================================\n");
}



































