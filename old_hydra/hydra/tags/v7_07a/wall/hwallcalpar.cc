//*-- AUTHOR Rainer Schicker
//*-- modified: 15/08/2005 by Filip Krizek
//*-- modified: 18/12/2001 by Ilse Koenig
//*-- modified: 06/03/2000 by Ilse Koenig
//*-- created : 18/02/00

/////////////////////////////////////////////////////////////
// HStartCalPar
//
// Container class for Start calibration parameters
//
/////////////////////////////////////////////////////////////
#include "hwallcalpar.h"
#include "hades.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "hwalldetector.h"
#include "hpario.h"
#include "hdetpario.h"
#include <iostream.h>

ClassImp(HWallCalPar)
ClassImp(HWallCalParFW)//??????????????????????
ClassImp(HWallCalParCell)

void HWallCalParCell::Streamer(TBuffer &R__b)
{
	// Stream an object of class HStartCalParChan.

	UInt_t R__s, R__c;
	if (R__b.IsReading()) {
		Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
		TObject::Streamer(R__b);
	if(R__v==1){
		R__b >> TDC_slope;
		R__b >> TDC_offset;
                R__b >> ADC_slope;
		R__b >> ADC_offset;
	}
	else{
		R__b >> TDC_slope;
      	R__b >> TDC_offset;
	R__b >> ADC_slope;
      	R__b >> ADC_offset;
	}
	R__b.CheckByteCount(R__s, R__c, HWallCalParCell::IsA());
	} else {
		R__c = R__b.WriteVersion(HWallCalParCell::IsA(), kTRUE);
		TObject::Streamer(R__b);
		R__b << TDC_slope;
		R__b << TDC_offset;
   	        R__b << ADC_slope;
		R__b << ADC_offset;      
		R__b.SetByteCount(R__c, kTRUE);
	}
}


HWallCalParFW::HWallCalParFW(Int_t n) {
  // constructor creates an array of pointers of type
  // HStartCalParChan
  array = new TObjArray(n);
  for(Int_t i=0 ; i<n ; i++)
    array->AddAt( new HWallCalParCell(),i);
}

HWallCalParFW::~HWallCalParFW() {
  // destructor
  array->Delete();
  delete array;
}

HWallCalPar::HWallCalPar(const char* name,const char* title,
                           const char* context)
             : HParSet(name,title,context) {
  // constructor creates an array of pointers of type HWallCalParFW
  strcpy(detName,"Wall");
  HDetector* det= gHades->getSetup()->getDetector("Wall");
  Int_t nComp=det->getMaxComponents(); //384 cells
  array = new TObjArray(1); //????????????????
  array->AddAt( new HWallCalParFW(nComp),0);     
}

HWallCalPar::~HWallCalPar() {
  // destructor
  array->Delete();
  delete array;
}

Bool_t HWallCalPar::init(HParIo* inp,Int_t* set) {
  // intitializes the container from an input
  HDetParIo* input=inp->getDetParIo("HWallParIo");
  if (input) return (input->init(this,set));
  return kFALSE;
}

Int_t HWallCalPar::write(HParIo* output) {
  // writes the container to an output
  HDetParIo* out=output->getDetParIo("HWallParIo");
  if (out) return out->write(this);
  return -1;
}

void HWallCalPar::clear() {
  // clears the container
  //for(Int_t i=0;i<getSize();i++) {
  HWallCalParFW& fw=(*this)[0];
    for(Int_t j=0;j<fw.getSize();j++)
      fw[j].clear();
    // }
  status=kFALSE;
  resetInputVersions();
}

void HWallCalPar::printParam() {
  // prints the calibration parameters
  printf("Calibration parameters for the Wall detector\n");
  printf("cell TDC_K TDC_offset ADC_K ADC_offset\n");
  //for(Int_t i=0;i<getSize();i++) {
  HWallCalParFW& fw=(*this)[0];
     for(Int_t j=0;j<fw.getSize();j++) {
      HWallCalParCell& cell=fw[j];
      printf("%i  %8.3f %8.3f  %8.3f %8.3f \n",j,cell.getTDC_Slope(),cell.getTDC_Offset(),cell.getADC_Slope(),cell.getADC_Offset());
    }
  }


void HWallCalPar::readline(const char *buf, Int_t *set) {
  // decodes one line read from ascii file I/O
  Int_t  cell;
  Float_t TDC_slope, TDC_offset, ADC_slope, ADC_offset;
  sscanf(buf,"%i%f%f%f%f", &cell, &TDC_slope, &TDC_offset, &ADC_slope, &ADC_offset);
  if (!set[0]) return;
  (*this)[0][cell].fill(TDC_slope,TDC_offset,ADC_slope,ADC_offset);
  //(*this)[cell].fill(TDC_slope,TDC_offset,ADC_slope,ADC_offset);
  //printf("ADC_slope=%f\n",(*this)[0][cell].getADC_Slope());
  set[0]=999;
}

void HWallCalPar::putAsciiHeader(TString& header) {
  // puts the ascii header to the string used in HStartParAsciiFileIo
  header=
    "# Calibration parameters for the WALL detector\n"
    "# Format:\n"
    "# cell  TDC_slope   TDC_offset   ADC_slope   ADC_offset\n";
}

Bool_t HWallCalPar::writeline(char *buf, Int_t cell) {
  // writes one line to the buffer used by ascii file I/O
    HWallCalParCell& celll=(*this)[0][cell];
  sprintf(buf,"%i%8.3f%8.3f%8.3f%8.3f\n",
               cell, celll.getTDC_Slope(),celll.getTDC_Offset(),celll.getADC_Slope(),celll.getADC_Offset());
  return kTRUE;
}
  






