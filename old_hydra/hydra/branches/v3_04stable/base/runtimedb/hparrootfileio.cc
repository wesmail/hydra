//*-- AUTHOR : Ilse Koenig
//*-- Modified : 04/09/99 by Ilse Koenig

//////////////////////////////////////////////////////////////////////////////
// HParRootFileIo
//
// Interface class for parameter I/O from/to ROOT file
// derived from interface base class HParIo.
//
// It contains a pointer to the "connection class" of type HParRootFile
// inherited from TFile.  HParRootFile contains an object of type HEventFile
// which holds the list of container versions for an event file.
//
// It contains also a list of detector interface classes all having the common
// base type HDetParRootFileIo. Every detector has its own interface class.
//
// This class replaces the former class HParFileIo.
// 
//////////////////////////////////////////////////////////////////////////////

#include <TROOT.h>  
#include "hparrootfileio.h"
#include "hades.h"
#include "hspectrometer.h"
#include "heventfile.h"
#include <stdio.h>
#include <iostream.h>

ClassImp(HParRootFile)
ClassImp(HParRootFileIo)

HParRootFile::HParRootFile(Text_t* fname, Option_t* option="READ",
                           Text_t* ftitle="", Int_t compress=0)
              : TFile(fname,option,ftitle,compress) {
  // constructor opens a ROOT file
  eventFile=0;
}

HParRootFile::~HParRootFile() {
  // destructor
  if (eventFile) delete eventFile;
  eventFile=0;
}

void HParRootFile::readVersions(HEventFile* currentFile) {
  // finds the current parameter event file containing the parameter container
  // versions in the ROOT file
  if (eventFile) delete eventFile;
  eventFile=(HEventFile*)Get(((char*)currentFile->GetName()));
  if (eventFile) currentFile->setFileId(eventFile->getFileId());
}



HParRootFileIo::HParRootFileIo() {
  // constructor
  file=0;
}

HParRootFileIo::~HParRootFileIo() {
  // destructor closes an open file
  close();
}

Bool_t HParRootFileIo::open(Text_t* fname, Option_t* option="READ",
                        Text_t* ftitle="", Int_t compress=0) {
  // It opens a ROOT file (default option "READ"). An open file will be closed.
  // The detector I/Os for all detectors defined in the setup are activated.
  close();
  file=new HParRootFile(fname,option,ftitle,compress);
  if (file && file->IsOpen()) {
    gHades->getSetup()->activateParIo(this);
    return kTRUE;
  }
  return kFALSE;
}

void HParRootFileIo::close() {
  // closes an open ROOT file and deletes the detector I/Os
  if (file) {
    file->Close();
    delete file;
    file=0;
  }
  if (detParIoList) detParIoList->Delete();
}

void HParRootFileIo::print() {
  // prints the content of a open ROOT file and the list of detector I/Os
  if (file) file->ls();
  else cout<<"No pointer to ROOT file"<<'\n';
  cout<<"input/output for detectors: "<<'\n';
  detParIoList->ls();
}

HParRootFile* HParRootFileIo::getParRootFile() {
  // returns a pointer to the current ROOT file
  return file;
}

void HParRootFileIo::readVersions(HEventFile* currentFile) {
  // reads the parameter container versions for the current event file from
  // the ROOT file
  if (file) file->readVersions(currentFile);
}

TList* HParRootFileIo::getList() {
  // returns the list of keys found in the ROOT file
  if (file) return file->GetListOfKeys();
  return 0;
}
