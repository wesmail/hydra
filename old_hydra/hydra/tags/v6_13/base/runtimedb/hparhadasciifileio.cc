//*-- AUTHOR : Denis Bertini
//*-- Last modified : 21/05/99 by Ilse Koenig
//*-- Last modified : 2000/02/11 by Witold Przygoda

///////////////////////////////////////////////////////////////////////////////
// HParHadAsciiFileIo
//
// Interface class for parameter I/O from ascii file
// derived from the interface base class HParIo
// 
// It contains pointers to the ascii file and to the interface classes for all
// detectors defined in the actual setup.
//
// This class was created by renaming the former class HParAsciFileIo.
// 
///////////////////////////////////////////////////////////////////////////////

#include "hades.h"
#include "hpario.h"
#include "hparhadasciifileio.h"
#include "hspectrometer.h"
#include <string.h>

ClassImp(HParHadAsciiFileIo)

HParHadAsciiFileIo::HParHadAsciiFileIo() {
 fActive = 0;
}

HParHadAsciiFileIo::HParHadAsciiFileIo(Int_t typ) {
 fActive = 0;
 SetAsciiType(typ);
}

HParHadAsciiFileIo::HParHadAsciiFileIo(const Char_t* typ) {
 fActive = 0;
 SetAsciiType(typ);
}

HParHadAsciiFileIo::~HParHadAsciiFileIo() {
  // default destructor closes an open file and deletes list of I/Os
}

HAsciiRaw* HParHadAsciiFileIo::GetRawAscii() {
 return &fRaw;
}

HAsciiKey* HParHadAsciiFileIo::GetKeyAscii() {
 return &fKey;
}

void HParHadAsciiFileIo::SetAsciiType(Int_t typ) {
 if (typ > 0) fActive = 1;
 else fActive = 0;
}

void HParHadAsciiFileIo::SetAsciiType(const Char_t* typ) {
 using namespace HadAsciiAuxiliaryFunctions;
 Char_t buf1[10];
 Char_t buf2[10];
 strtrunc(buf1,typ);
 strtolower(buf2,buf1);
 if (strcompare(buf2,"key") == 0) fActive = 0;
 else if (strcompare(buf2,"raw") == 0) fActive = 1;
 else {
  cerr << "Error in <HParHadAsciiFileIo::SetAsciiType>: "
  << "ASCII data type not recognised (now set to default \'key\').\n";
  fActive = 0;
 }
}


Int_t HParHadAsciiFileIo::GetAsciiType() {
 return fActive;
}


Bool_t HParHadAsciiFileIo::open(Text_t* fname, Text_t* status) {

 switch(fActive) {
  case 0: fKey.OpenFile(fname,status);
  case 1: fRaw.OpenFile(fname,status);
 }

 gHades->getSetup()->activateParIo(this);
return kTRUE;
}


void HParHadAsciiFileIo::close() {
  // closes the file and deletes the detector I/Os
 fKey.CloseFile();
 fRaw.CloseFile();
 if (detParIoList) detParIoList->Delete();
}

void HParHadAsciiFileIo::print() {
  // prints information about the file and the detector I/Os
  if (check()) {
    printf("Ascii I/O is opened\n");
    detParIoList->ls();
  }
  else printf("No pointer to file\n");
  detParIoList->ls();
}

void HParHadAsciiFileIo::openfilelist(const Char_t* filename) {
 (fActive) ? fRaw.OpenFileList(filename) : fKey.OpenFileList(filename);

 gHades->getSetup()->activateParIo(this);
}



