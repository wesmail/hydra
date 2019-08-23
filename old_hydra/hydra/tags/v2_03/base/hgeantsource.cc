#include "hgeantsource.h"
#include <TROOT.h>
#include "hgeantreader.h"
#include "hrecevent.h"

//*-- Author : Manuel Sanchez
//*-- Modified : 2/11/98 by Manuel Sanchez 
//*-- Modified : /30/09/1998 by Manuel Sanchez
//*-- Modified : 17/06/98 by Manuel Sanchez
//*-- Modified : 26/02/99 by Ilse Koenig

//////////////////////////////
// HGeantSource
//
//  This class is a data source to read the simulated data obtained from
//GEANT.
//
//  HGeantSource acts as a container of different HGeantReader objects, which
//do the actual work by getting the data from a file and putting those data
//in the corresponding categories when their "execute" function is called
//
//  This kind of design, as in HLmdSource, allows the user to specifically 
//read only those parts of the event in which is interested.
////////////////////////////

HGeantSource::HGeantSource(void) {
  //Default constructor
}

HGeantSource::HGeantSource(HGeantSource &s) {
  fReaderList.Delete();
  fReaderList.AddAll(&s.fReaderList);
  fFileTable.Delete();
  fFileTable.AddAll(&s.fFileTable);
}

HGeantSource::~HGeantSource(void) {
  //Destructor. Deletes the readers
  fReaderList.Delete();
}

Bool_t HGeantSource::addGeantReader(HGeantReader *r,Text_t *inputFile) {
  //This method allows to specify which readers will be used to get the data.
  //The first input is the actual reader to be used, and the second input is the
  //name of the file from where this reader will read data; specifying the name
  //here allows having several readers each of them taking data from a different file.
  //
  // The situation of two or more readers taking data from the same file is foreseen, 
  //the user just gives the same name for the input file when adding both readers.
  TFile *f=(TFile *)fFileTable.FindObject(inputFile);
  if (f==NULL) {
    f=new TFile(inputFile);
    if (f==NULL) Error("HGeantSource::addGeantReader","Specified file doesn't exist");
    fFileTable.Add(f);
    gROOT->cd();
  }
  r->setInput(f);
  fReaderList.Add(r);
  return kTRUE;
}

Bool_t HGeantSource::init(void) {
  // calls the init() functions of all readers
  TIter next(&fReaderList);
  HGeantReader *greader;
  if (!(*fEventAddr)) {
    (*fEventAddr)=new HRecEvent;
  }
  while ( (greader=(HGeantReader *)next())!=NULL) {
    if (!greader->init()) return kFALSE;
  }
  return kTRUE;
}

EDsState HGeantSource::getNextEvent(void) {
  //Tries to read a new event into the HEvent 
  //structure:
  //
  TIter next(&fReaderList);
  HGeantReader *reader;
  while ( (reader=(HGeantReader *)next())!=NULL) {
    if (!reader->execute()) return kDsError;
  }
  return kDsOk;
}  

ClassImp(HGeantSource)
