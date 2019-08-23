//*-- AUTHOR : Ilse Koenig
//*-- Modified : 02/11/98

/////////////////////////////////////////////////////////////
//HEventFile
//
//  class for event file
//
//  Each event file has a name and an id number. The name is
//  used to find the parameters valid for this file in Oracle
//  and in the ROOT file. The id number identifies the event
//  file. Each event has this number in the event header. It
//  is also stored in Oracle and can be used to find the
//  name of the corresponding event file and the valid
//  parameter versions.
//  Associated with the event file is a list of container
//  names with the versions of the containers in the two
//  possible inputs and the output (class HParVersions).
//  The input versions are used during the initialisation
//  used during the initialisation of the containers.
//
/////////////////////////////////////////////////////////////

#include "heventfile.h"
#include <iostream.h>

ClassImp(HParVersion)
ClassImp(HEventFile)

HParVersion::HParVersion(Text_t* name) : TNamed(name,"version info") {
  // constructor with the name of the container
  rootVersion=0;
  for(Int_t i=0;i<3;i++) {inputVersions[i]=-1;}
}


HEventFile::HEventFile(Text_t* name,Text_t* refName="")
              : TNamed(name,"event file") {
  // constructor with the name of the event file
  fileId=-1L;
  parVersions=new TList();
  refFileName=refName;
}

HEventFile::HEventFile(HEventFile &ef) {
  // copy constructor
  SetName(ef.GetName());
  fileId=ef.getFileId();
  parVersions=new TList();
  TList* lv=ef.getParVersions();
  TIter next(lv);
  HParVersion* pv;
  while ((pv=(HParVersion*)next())) {
    parVersions->Add(pv);
  }
  refFileName=ef.refFileName;
}

HEventFile::~HEventFile() {
  // destructor
  parVersions->Delete();
  parVersions=0;
}

void HEventFile::addParVersion(HParVersion *pv) {
  // adds a container version object to the list
  parVersions->Add(pv);
}

HParVersion* HEventFile::getParVersion(Text_t* name) {
  // return a container version object called by the name of
  // the container
  return (HParVersion*)parVersions->FindObject(name);
}

void HEventFile::resetInputVersions() {
  TIter next(parVersions);
  HParVersion* v;
  while ((v=(HParVersion*)next())) {
    v->resetInputVersions();
  }
}

void HEventFile::resetOutputVersions() {
  TIter next(parVersions);
  HParVersion* v;
  while ((v=(HParVersion*)next())) {
    v->setRootVersion(0);
  }
}

void HEventFile::print() {
  // prints the list of container versions for this event file
  cout<<fName<<"   run id "<<fileId<<'\n';
  TIter next(parVersions);
  HParVersion* v;
  while ((v=(HParVersion*)next())) {
    cout<<"     "<<v->GetName()<<"        "<<v->getInputVersion(1)<<"            "
        <<v->getInputVersion(2)<<"            "<<v->getRootVersion()<<'\n';
  }
}
