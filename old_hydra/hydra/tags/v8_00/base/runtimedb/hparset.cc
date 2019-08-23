//*-- AUTHOR : Ilse Koenig
//*-- Last modified : 13/12/2001 by Ilse Koenig

//_HADES_CLASS_DESCRIPTION 
/////////////////////////////////////////////////////////////
//  HParSet
//
//  Base class for all parameter containers
//
/////////////////////////////////////////////////////////////
using namespace std;
#include "hparset.h"
#include "hades.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include <iostream> 
#include <iomanip>

ClassImp(HParSet)

HParSet::HParSet(const char* name,const char* title,const char* context)
        : TNamed(name,title) {
  // constructor sets default values of data elements
  paramContext=context;
  for(Int_t i=0;i<3;i++) {versions[i]=-1;}
  status=kFALSE;
  changed=kFALSE;
  detName[0]='\0';
}

Bool_t HParSet::init(HParIo* io) {
  // intitializes the container from an input in run time
  // database. If this is not successful it is initialized from
  // the second input. If this failes too, it returns an error.
  // (calls internally the init function in the derived class)
  HRuntimeDb* rtdb=gHades->getRuntimeDb();
  Int_t* set=0;
  if (strlen(detName)>0) {
    HDetector* det=gHades->getSetup()->getDetector(detName);
    if (det) {
      Int_t len=0;
      Int_t nSec=det->getMaxSectors();
      if (nSec<0) len=det->getMaxModules();
      else len=det->getMaxModules()*nSec;
      Int_t* s=det->getModules();
      set=new Int_t[len];
      for(Int_t i=0;i<len;i++) set[i]=s[i];
    } else {
      cerr<<"********  "<<GetName()<<"  not initialized  ********"<<endl;
      cerr<<"********  Detector "<<detName<<" not in setup  ********"<<endl;
      return kFALSE;
    }
  }
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
    } else setInputVersion(-1,2);
  }
  if (set) delete[] set;
  if (allFound) return kTRUE;
  cerr<<"********  "<<GetName()<<"  not initialized  ********"<<endl;
  return kFALSE;
}

Int_t HParSet::write() {
  // writes the container to the output defined in the runtime database
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
  if (!paramContext.IsNull()) cout<<"Context/Purpose:       "<<paramContext<<'\n';
  if (!author.IsNull()) cout<<"Author:                "<<author<<'\n';
  if (!description.IsNull()) cout<<"Description:           "<<description<<'\n';
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
    changed=kFALSE;
  }
}

void HParSet::setParamContext(const char* context) {
  // Sets the context
  // The context is normally set via the constructor and this function is somehow dangerous.
  // It should be only used, when a container is initialized from old parameter files,
  // when a container is initialized from old parameter files, which do not yet support the
  // context.
  paramContext=context;
}

void HParSet::Streamer(TBuffer &R__b) {
  // Streams an object of class HParSet.
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(); if (R__v) { }
      TNamed::Streamer(R__b);
      if (R__v > 1) {
        paramContext.Streamer(R__b);
        author.Streamer(R__b);
        description.Streamer(R__b);
      }
   } else {
      R__b.WriteVersion(HParSet::IsA());
      TNamed::Streamer(R__b);
      paramContext.Streamer(R__b);
      author.Streamer(R__b);
      description.Streamer(R__b);
   }
}
