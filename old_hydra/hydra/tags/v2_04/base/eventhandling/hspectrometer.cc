//*-- AUTHOR : Ilse Koenig
//*-- Modified : 27/05/99

/////////////////////////////////////////////////////////////
//HSpectrometer
//
//  class for the actual setup of the HADES spectrometer used
//  in the analysis.
//  It contains the list of defined detectors.
//
/////////////////////////////////////////////////////////////

#include "hspectrometer.h"
#include "hdetector.h"
#include "hades.h"
#include "hruntimedb.h"
#include "hgeomshapes.h"
#include <iostream.h>

ClassImp(HSpectrometer)

HSpectrometer::HSpectrometer(void) : TNamed("Hades","The Hades spectrometer") {
  // constructor creates an empty list of detectors
  detectors=new TList();
  changed=kFALSE;
}

HSpectrometer::~HSpectrometer(void) {
  // destructor
  detectors->Delete();
  delete detectors;
  if (shapes) delete shapes;
}

void HSpectrometer::addDetector(HDetector* det) {
  // adds a detector to the list of detectors
  detectors->Add(det);
  changed=kTRUE;
}

HDetector* HSpectrometer::getDetector(Text_t* name) {
  // returns a pointer to the detector called by name
  return ((HDetector*)detectors->FindObject(name));
}

Bool_t HSpectrometer::init(Text_t* level="raw") {
  // Calls the init function of each detector in the spectrometer.
  // The string level indicates the start level of the analysis
  // Each detector (as e.g. MDC) which needs special initialization
  // before creating the tasks must have an init(...)-function
  TIter next(detectors);
  HDetector *det=0;
  while ((det=(HDetector *)next())) {
    if (!det->init(level)) return kFALSE;
  }
  return kTRUE;
}

void HSpectrometer::activateParIo(HParIo* io) {
  // loops over the list of detectors and activates the
  // corresponding detector I/Os
  TIter next(detectors);
  HDetector* det;
  while ((det=(HDetector*)next())) {
    det->activateParIo(io);
  }
}

Bool_t HSpectrometer::write() {
  // writes the actual setup to the output defined in the
  // runtime database
  HParIo* io=gHades->getRuntimeDb()->getOutput();
  if (io) return write(io);
  cerr<<"actual setup couldn't be written to output"<<endl;
  return kFALSE;
}

Bool_t HSpectrometer::write(HParIo* io) {
  // writes the actual setup to the output
  TIter next(detectors);
  HDetector* det=0;
  Bool_t rc=kTRUE;
  while ((det=(HDetector*)next())) {
    if (det->write(io)==kFALSE) rc=kFALSE;
  }
  if (rc==kTRUE) changed=kFALSE;
  else cerr<<"actual setup couldn't be written to output"<<endl;
  return rc;
}

HDetGeomPar* HSpectrometer::createDetGeomPar(Text_t* name) {
  // Creates the geometry parameter container for the detector given by name.
  // Calls the function createDetGeomPar(HGeomShapes*) in the detector class.
  // If the shape class object does not exist, it is created.
  HDetector* det=getDetector(name);
  if (det) {
    if (shapes==0) shapes=new HGeomShapes();
    return det->createDetGeomPar(shapes);
  }
  cerr<<"Detector "<<name<<" not found"<<endl;
  cerr<<"Geometry parameter container not created"<<endl;
  return 0;
}

void HSpectrometer::print() {
  // prints the actual setup
  cout<<"Actual setup for Hades Spectrometer:"<<"\n";
  TIter next(detectors);
  HDetector* det=0;
  while ((det=(HDetector*)next())) {
    det->print();
  }
}
