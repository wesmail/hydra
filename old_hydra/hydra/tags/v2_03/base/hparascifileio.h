// File: hparascifileio.h

/////////////////////////////////////////////////////////////////////////
// Interface class ascii file                                          //
// derived from "abstact" interface class HParIo                       //
//                                                                     //
// contains pointers to interface classes for every detector and to    //
// the file                                                            //
/////////////////////////////////////////////////////////////////////////

#ifndef HPARASCIFILEIO_H
#define HPARASCIFILEIO_H

#include <iostream.h>
#include <string.h>
#include "hpario.h"
#include "TList.h"

//class HSpectrometer;
//class HEventFile;

#include <iostream.h>
#include <fstream.h>

class HParAsciFileIo : public HParIo {
protected:
  fstream* file;  // pointer to a file
public:
  HParAsciFileIo();

  // default desstructor closes an open file and deletes list of I/Os
  ~HParAsciFileIo();

  // opens file
  // if a file is already open, this file will be closed
  // activates detector I/Os
  Bool_t open(Text_t* fname, Text_t* status);

  // closes file
  void close();

  // returns kTRUE if file is open
  Bool_t check() {
    if (file) return (file->rdbuf()->is_open()==1);
    else return kFALSE;
  }
   
  // prints information about the file and the detector I/Os
  void print();

  fstream* getFile();

  ClassDef(HParAsciFileIo,0) // Parameter I/O from Ascii files
};

#endif  /* !HPARASCIFILEIO_H */

