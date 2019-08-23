// File: hparhadasciifileiio.h

/////////////////////////////////////////////////////////////////////////
// Interface class ascii file                                          //
// derived from "abstact" interface class HParIo                       //
//                                                                     //
// contains pointers to interface classes for every detector and to    //
// the file                                                            //
/////////////////////////////////////////////////////////////////////////

#ifndef HPARHADASCIFILEIIO_H
#define HPARHADASCIFILEIIO_H
using namespace std;
#include <iostream> 
#include <iomanip>
#include <fstream> 
#include <string.h>
#include "hpario.h"
#include "hadascii.h"
#include "TList.h"


class HParHadAsciiFileIo : public HParIo {
protected:

 Int_t fActive;
 HAsciiKey fKey; // type 0
 HAsciiRaw fRaw; // type 1

public:

  HParHadAsciiFileIo();
  HParHadAsciiFileIo(Int_t typ);
  HParHadAsciiFileIo(const Char_t* typ);

  // default destructor closes an open file and deletes list of I/Os
  ~HParHadAsciiFileIo();

  HAsciiRaw* GetRawAscii();
  HAsciiKey* GetKeyAscii(); 
  void SetAsciiType(Int_t typ = 0);
  void SetAsciiType(const Char_t* typ);
  Int_t GetAsciiType();

  // opens file
  // if a file is already open, this file will be closed
  // activates detector I/Os
  Bool_t open(Text_t* fname, Text_t* status="in");

  void openfilelist(const Char_t* filename);

  HParHadAsciiFileIo* getHadAscii(){return this;}
  // closes file
  void close();

  // returns kTRUE if file is open
  Bool_t check() {
   switch (fActive) {
    case 0: if (fKey.IsReadFile() || fKey.IsWriteFile()) return kTRUE;
    case 1: if (fRaw.IsReadFile() || fRaw.IsWriteFile()) return kTRUE;
   }
   return kFALSE;
  }
   
  // prints information about the file and the detector I/Os
  void print();


  ClassDef(HParHadAsciiFileIo,0) // Parameter I/O from Ascii files
};

#endif  /* !HPARHADASCIIFILEIO_H */

