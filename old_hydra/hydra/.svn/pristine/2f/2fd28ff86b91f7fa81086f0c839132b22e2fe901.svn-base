//*-- AUTHOR : Ilse Koenig
//*-- Modified : 02/11/98

/////////////////////////////////////////////////////////////
//HParFileIo
//
//  Interface class to ROOT file
//  derived from interface base class HParIo
//                                                                     //
//  It contains pointers to a list of interface classes
//  (inherited from the base class) and to the connection
//  class (HParRootFile inherited from TFile)
//  Every detector has its own interface class.
//
/////////////////////////////////////////////////////////////
#include <TROOT.h>  
#include "hparfileio.h"
#include "hades.h"
#include "hspectrometer.h"
#include "heventfile.h"
#include <stdio.h>
#include <iostream.h>

ClassImp(HParRootFile)
ClassImp(HParFileIo)

HParRootFile::HParRootFile(Text_t* fname, Option_t* option="READ",
                           Text_t* ftitle="", Int_t compress=0)
              : TFile(fname,option,ftitle,compress) {
  // constructor
  eventFile=0;
}

HParRootFile::~HParRootFile() {
  // destructor
  if (eventFile) delete eventFile;
  eventFile=0;
}

void HParRootFile::readVersions(HEventFile* currentFile) {
  // finds the current event file versions in the ROOT file
  if (eventFile) delete eventFile;
  Text_t* name=currentFile->GetName();
  cd();
  eventFile=(HEventFile*)gROOT->FindObject(name);
  currentFile->setFileId(eventFile->getFileId());
}



HParFileIo::HParFileIo() {
  // constructor creates an empty list of detector I/Os
  file=0;
  detParIoList=new TList();
}

HParFileIo::~HParFileIo() {
  // destructor closes an open file
  close();
  delete detParIoList;
}

Bool_t HParFileIo::open(Text_t* fname, Option_t* option="READ",
                        Text_t* ftitle="", Int_t compress=0) {
  // opens a ROOT file with default option "READ"
  // activates detector I/Os for all detectors defined in the setup
  close();
  file=new HParRootFile(fname,option,ftitle,compress);
  if (file && file->IsOpen()) {
    gHades->getSetup()->activateParIo(this);
    return kTRUE;
  }
  return kFALSE;
}

void HParFileIo::close() {
  // closes the ROOT file
  if (file) {
    file->Close();
    delete file;
    file=0;
  }
  if (detParIoList) detParIoList->Delete();
}

void HParFileIo::print() {
  // prints the content of a open ROOT file and the list
  // of detector I/Os
  if (file) file->ls();
  else cout<<"No pointer to ROOT file"<<'\n';
  cout<<"input/output for detectors: "<<'\n';
  detParIoList->ls();
}

HParRootFile* HParFileIo::getParRootFile() {
  // returns a pointer of the current ROOT file
  return file;
}

void HParFileIo::readVersions(HEventFile* currentFile) {
  // reads the current event file versions in the ROOT file
  if (file) file->readVersions(currentFile);
}

TList* HParFileIo::getList() {
  // returns the list of keys found in the ROOT file
  if (file) return file->GetListOfKeys();
  return 0;
}
