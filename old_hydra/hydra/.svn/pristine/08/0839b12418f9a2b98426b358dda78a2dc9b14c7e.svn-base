//*-- AUTHOR : Denis Bertini
//*-- Last modified : 28/11/98 by Ilse Koenig

/////////////////////////////////////////////////////////////////////////
//HParAsciFileIo                                                       //
//                                                                     //
// Interface class ascii file                                          //
// derived from "abstact" interface class HParIo                       //
//                                                                     //
// contains pointers to interface classes for every detector and to    //
// the connection class                                                //
//                                                                     //
/////////////////////////////////////////////////////////////////////////

#include "hparascifileio.h"
#include "hades.h"
#include "hspectrometer.h"
#include <string.h>

ClassImp(HParAsciFileIo)

HParAsciFileIo::HParAsciFileIo() {
  // default destructor
  file=0;
  detParIoList=new TList();
}

HParAsciFileIo::~HParAsciFileIo() {
  // default desstructor closes an open file and deletes list of I/Os
  close();
  delete detParIoList;
}

Bool_t HParAsciFileIo::open(Text_t* fname, Text_t* status) {
  // opens file
  // if a file is already open, this file will be closed
  // activates detector I/Os
  close();
  if(!((strcmp(status,"in")==0) || (strcmp(status,"out")==0))){
  printf(" put the right stream option for file :%2s\n",fname);
  printf(" writing state : in  \n");
  printf(" reading state : out \n");
  printf(" open  Aborted \n");
  return kFALSE;}

  file=new fstream();
  if(strcmp(status,"in")==0){file->open( fname, ios::in);};
  if(strcmp(status,"out")==0){file->open( fname, ios::out);};

  filebuf* buf = file->rdbuf();
  if (file && (buf->is_open()==1)) {
    gHades->getSetup()->activateParIo(this);
    return kTRUE;
  }
  return kFALSE;
}

void HParAsciFileIo::close() {
  // closes file
  if (file) {
    file->close();
    delete file;
    file=0;
  }
  if (detParIoList) detParIoList->Delete();
}

void HParAsciFileIo::print() {
  // prints information about the file and the detector I/Os
  if (check()) printf("Asci I/O is open\n");
  else printf("No pointer to file\n");
  detParIoList->ls();
}

fstream* HParAsciFileIo::getFile() {
  return file;
}

