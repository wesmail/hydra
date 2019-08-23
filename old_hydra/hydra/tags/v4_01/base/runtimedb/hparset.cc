//*-- AUTHOR : Ilse Koenig
//*-- Modified : 02/11/98

/////////////////////////////////////////////////////////////
//HParSet
//
//  Base class for all parameter containers
//
/////////////////////////////////////////////////////////////

#include "hparset.h"
#include "hades.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include <iostream.h>

ClassImp(HParSet)

HParSet::HParSet() {
  // constructor sets default values of data elements
  for(Int_t i=0;i<3;i++) {versions[i]=-1;}
  status=kFALSE;
  changed=kFALSE;
}

Bool_t HParSet::init(HParIo* io) {
  // intitializes the container from an input in run time
  // database. If this is not successful it is initialized from
  // the second input. If this failes too, it returns an error.
  // (calls internally the init function in the derived class)
  HRuntimeDb* rtdb=gHades->getRuntimeDb();
  HDetector* det=gHades->getSetup()->getDetector(detName);
  if (det) {
    Int_t len=det->getMaxModules()*6;
    Int_t* s=det->getModules();
    Int_t* set=new Int_t[len];
    for(Int_t i=0;i<len;i++) set[i]=s[i];
    Bool_t allFound=kFALSE;
    if (versions[1]!=-1 && versions[2]!=-1) resetInputVersions();
    if (io) {
      allFound=init(io,set);
    } else {
      io=rtdb->getFirstInput();
      if (io) allFound=init(io,set);
      if (!allFound) {
        io=rtdb->getSecondInput();
        if (io) allFound=init(io,set);
      }
    }
    delete[] set;
    if (allFound) {
      changed=kTRUE;
      return kTRUE;
    }
  }
  cerr<<"********  "<<GetName()<<"  not initialized  ********"<<endl;
  return kFALSE;
}

Int_t HParSet::write() {
  // writes the container to the output defined in the runtime
  // database
  // returns the output version in the ROOT file
  // returns -1 if error occured
  // (calls internally the init function in the derived class)
  HParIo* output=gHades->getRuntimeDb()->getOutput();
  if (output) return write(output);
  cerr<<GetName()<<" could not be written to output"<<endl;
  return -1;
}

void HParSet::print() {
  // prints information about container (versions,status,hasChanged...)
  cout<<"-----  "<<GetName()<<"  -----"<<'\n';
  cout<<"first input version:   "<<versions[1]<<'\n';
  cout<<"second input version:  "<<versions[2]<<'\n';
  if (changed) cout<<"has changed"<<'\n';
  else cout<<"has not changed"<<'\n';
  if (status) cout<<"is static"<<'\n';
  else cout<<"is not static"<<'\n';
}

void HParSet::resetInputVersions() {
  // resets the input versions if the container is not static
  if (!status) {
    for(Int_t i=0;i<3;i++) {versions[i]=-1;}
    changed=kTRUE;
  }
}

