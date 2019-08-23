//*-- AUTHOR Rainer Schicker
//*-- modified: 18/12/2001 by Ilse Koenig
//*-- modified: 06/03/2000 by Ilse Koenig
//*-- created : 18/02/00

//_HADES_CLASS_DESCRIPTION 
/////////////////////////////////////////////////////////////
// HStartCalPar
//
// Container class for Start calibration parameters
//
/////////////////////////////////////////////////////////////
using namespace std;
#include "hstartcalpar.h"
#include "hades.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "hstartdetector.h"
#include "hpario.h"
#include "hdetpario.h"
#include <iostream> 
#include <iomanip>

ClassImp(HStartCalPar)
ClassImp(HStartCalParMod)
ClassImp(HStartCalParChan)

void HStartCalParChan::Streamer(TBuffer &R__b)
{
	// Stream an object of class HStartCalParChan.

	UInt_t R__s, R__c;
	if (R__b.IsReading()) {
		Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
		TObject::Streamer(R__b);
	if(R__v==1){
		R__b >> slope;
		R__b >> offset;
	}
	else{
		R__b >> slope;
      	R__b >> offset;
      	R__b >> slopeL;
      	R__b >> slopeR;
      	R__b >> slopeM;
      	R__b >> vGroup;
      	R__b >> posOffset;
	}
	R__b.CheckByteCount(R__s, R__c, HStartCalParChan::IsA());
	} else {
		R__c = R__b.WriteVersion(HStartCalParChan::IsA(), kTRUE);
		TObject::Streamer(R__b);
		R__b << slope;
		R__b << offset;
		R__b << slopeL;
		R__b << slopeR;
		R__b << slopeM;
		R__b << vGroup;
		R__b << posOffset;
		R__b.SetByteCount(R__c, kTRUE);
	}
}

HStartCalParMod::HStartCalParMod(Int_t n) {
  // constructor creates an array of pointers of type
  // HStartCalParChan
  array = new TObjArray(n);
  for(Int_t i=0 ; i<n ; i++)
    array->AddAt( new HStartCalParChan(),i);
}

HStartCalParMod::~HStartCalParMod() {
  // destructor
  array->Delete();
  delete array;
}

HStartCalPar::HStartCalPar(const Char_t* name,const Char_t* title,
                           const Char_t* context)
             : HParSet(name,title,context) {
  // constructor creates an array of pointers of type HStartCalParMod
  strcpy(detName,"Start");
  HStartDetector* det = 
	(HStartDetector*)gHades->getSetup()->getDetector("Start");
  Int_t nMod=det->getMaxModInSetup();
  Int_t nComp=det->getMaxComponents(); 
  array = new TObjArray(nMod);
  for (Int_t i = 0; i < nMod; i++) {
    if (det->getModule(-1,i)>0) array->AddAt( new HStartCalParMod(nComp),i);
    else array->AddAt( new HStartCalParMod(0),i);
  }
}

HStartCalPar::~HStartCalPar() {
  // destructor
  array->Delete();
  delete array;
}

Bool_t HStartCalPar::init(HParIo* inp,Int_t* set) {
  // intitializes the container from an input
  HDetParIo* input=inp->getDetParIo("HStartParIo");
  if (input) return (input->init(this,set));
  return kFALSE;
}

Int_t HStartCalPar::write(HParIo* output) {
  // writes the container to an output
  HDetParIo* out=output->getDetParIo("HStartParIo");
  if (out) return out->write(this);
  return -1;
}

void HStartCalPar::clear() {
  // clears the container
  for(Int_t i=0;i<getSize();i++) {
    HStartCalParMod& mod=(*this)[i];
    for(Int_t j=0;j<mod.getSize();j++)
          mod[j].clear();
  }
  status=kFALSE;
  resetInputVersions();
}

void HStartCalPar::printParam() {
  // prints the calibration parameters
  printf("Calibration parameters for the START detector\n");
  printf("module  strip  slope  tim.offset slopeL slopeR slopeM vGroup posOffset\n");
  for(Int_t i=0;i<getSize();i++) {
    HStartCalParMod& mod=(*this)[i];
    for(Int_t j=0;j<mod.getSize();j++) {
      HStartCalParChan& chan=mod[j];
      printf("%4i%4i%10.5f%9.3f%10.5f%10.5f%10.5f%10.3f%8.3f\n",i,j,chan.getSlope(),chan.getOffset(),chan.getSlopeL(),chan.getSlopeR(),chan.getSlopeM(),chan.getVGroup(),chan.getPosOffset());
    }
  }
}

void HStartCalPar::readline(const Char_t *buf, Int_t *set) {
  // decodes one line read from ascii file I/O
  Int_t mod, strip;
	Float_t f[7]={-1,-1,-1,-1,-1,-1,-1}; // slope, offset,slopeL,slopeR,slopeM,vGroup,posOffset;
  sscanf(buf,"%i%i%f%f%f%f%f%f%f", &mod, &strip, &f[0],&f[1],&f[2],&f[3],&f[4],&f[5],&f[6]);
  if (!set[mod]) return;
  (*this)[mod][strip].fill(f);
  set[mod]=999;
}

void HStartCalPar::putAsciiHeader(TString& header) {
  // puts the ascii header to the string used in HStartParAsciiFileIo
  header=
    "# Calibration parameters for the START detector\n"
    "# Format:\n"
    "# module   strip   slope   tim.offset slopeL slopeR slopeM vGroup pos.Offset\n";
}

Bool_t HStartCalPar::writeline(Char_t *buf, Int_t mod, Int_t strip) {
  // writes one line to the buffer used by ascii file I/O
  HStartCalParChan& chan=(*this)[mod][strip];
  sprintf(buf,"%4i%4i%10.5f%9.3f%10.5f%10.5f%10.5f%10.3f%8.3f\n",
              mod, strip, chan.getSlope(),chan.getOffset(),chan.getSlopeL(),chan.getSlopeR(),chan.getSlopeM(),chan.getVGroup(),chan.getPosOffset());
  return kTRUE;
}
  






