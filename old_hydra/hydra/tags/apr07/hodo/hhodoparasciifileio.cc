//*-- AUTHOR : Ilse Koenig
//*-- Created : 18/05/2005

//_HADES_CLASS_DESCRIPTION 
///////////////////////////////////////////////////////////////
//
//  HHodoParAsciiFileIo
//
//  Parameter I/O from ASCII file for the Pion Hodoscopes
//
///////////////////////////////////////////////////////////////

#include "hhodoparasciifileio.h"
#include "hhodolookup.h"
#include "hhodocalpar.h"
#include "hhodorefhitpar.h"
#include "hades.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hhododetector.h"

ClassImp(HHodoParAsciiFileIo)

HHodoParAsciiFileIo::HHodoParAsciiFileIo(fstream* f) : HDetParAsciiFileIo(f) {
  // constructor calls the base class constructor
  fName="HHodoParIo";
}

Bool_t HHodoParAsciiFileIo::init(HParSet* pPar,Int_t* set) {
  // calls the appropriate read function for the container
  const  Text_t* name=pPar->GetName();
  if (pFile) {
    if (!strcmp(name,"HodoLookup")) return read((HHodoLookup*)pPar,set,kTRUE);
    if (!strcmp(name,"HodoCalPar")) return read((HHodoCalPar*)pPar,set,kTRUE);
    if (!strcmp(name,"HodoRefHitPar")) return read((HHodoRefHitPar*)pPar,set,kTRUE);
    cerr<<"initialization of "<<name<<" not possible from file!"<<endl;
    return kFALSE;
  }
  cerr<<"no input file open"<<endl;
  return kFALSE;
}

Int_t HHodoParAsciiFileIo::write(HParSet* pPar) {
  // calls the appropriate write function for the container
  if (pFile) {
    const  Text_t* name=pPar->GetName();
    if (!strcmp(name,"HodoLookup")) return writeFile3((HHodoLookup*)pPar);
    if (!strcmp(name,"HodoCalPar")) return writeFile2((HHodoCalPar*)pPar);
    if (!strcmp(name,"HodoRefHitPar")) return writeFile1((HHodoRefHitPar*)pPar);
    cerr<<name<<" could not be written to Ascii file"<<endl;
    return -1;
  }
  cerr<<"no output file open"<<endl;
  return -1;
}

template<class T> Bool_t HHodoParAsciiFileIo::read(T* pPar, Int_t* set,
                                                    Bool_t needsClear) {
  // template function for all parameter containers
  // searches the container in the file, reads the data line by line and
  //   called the member function readline(...) of the container class
  const  Text_t* name=pPar->GetName();
  HHodoDetector* det=
	 (HHodoDetector*)(gHades->getSetup()->getDetector("Hodo"));
  Int_t nSize = det->getMaxModInSetup();
  if (!findContainer(name)) return kFALSE;
  if (needsClear) pPar->clear();
  const Int_t maxbuf=155;
  Text_t buf[maxbuf];
  while (!pFile->eof()) {
    pFile->getline(buf, maxbuf);
    if (buf[0]=='#') break;
    if (buf[0]!='/' && buf[0]!='\0') pPar->readline(buf,set);
  }
  pPar->setInputVersion(1,inputNumber);
  pPar->setChanged();
  Bool_t allFound=kTRUE;
  if (!needsClear) {
    printf("%s: module(s) initialized from Ascii file:\n",name);
    allFound=checkAllFound(set,nSize);
  } else {
    for(Int_t i=0; i<nSize; i++) {
      if (set[i]) {
        if (set[i]==999) set[i]=1;
        else allFound=kFALSE;
      }
    }
    if (allFound) printf("%s initialized from Ascii file\n",name);
  }
  return allFound;
}

template<class T> Int_t HHodoParAsciiFileIo::writeFile3(T* pPar) {
  // template function for all parameter containers with 3 levels
  // writes the header, loops over the container and calls its member
  //   function writeline(...)
  pPar->putAsciiHeader(fHeader);
  writeHeader(pPar->GetName());
  Text_t buf[155];
  Int_t n0=(*pPar).getSize();
  for(Int_t i0=0; i0<n0; i0++) {
    Int_t n1=(*pPar)[i0].getSize();
    for(Int_t i1=0; i1<n1; i1++) {
      Int_t n2=(*pPar)[i0][i1].getSize();
      for(Int_t i2=0; i2<n2; i2++) {
        if (pPar->writeline(buf, i0, i1, i2)) pFile->write(buf,strlen(buf));
      }
    }
  }
  pPar->setChanged(kFALSE);
  pFile->write(sepLine,strlen(sepLine));
  return 1;
}

template<class T> Int_t HHodoParAsciiFileIo::writeFile2(T* pPar) {
  // template function for all parameter containers with 2 levels
  // writes the header, loops over the container and calls its member
  //   function writeline(...)
  pPar->putAsciiHeader(fHeader);
  writeHeader(pPar->GetName());
  Text_t buf[155];
  Int_t n0 = (*pPar).getSize();
  for(Int_t i0=0; i0<n0; i0++) {
    Int_t n1 = (*pPar)[i0].getSize();
    for(Int_t i1=0; i1<n1; i1++) {
      if (pPar->writeline(buf,i0,i1))
        pFile->write(buf,strlen(buf));
    }
  }
  pPar->setChanged(kFALSE);
  pFile->write(sepLine,strlen(sepLine));
  return 1;
}

template<class T> Int_t HHodoParAsciiFileIo::writeFile1(T* pPar) {
  // template function for all parameter containers with 1 levels
  // writes the header, loops over the container and calls its member
  //   function writeline(...)
  pPar->putAsciiHeader(fHeader);
  writeHeader(pPar->GetName());
  Text_t buf[155];
  Int_t n0 = (*pPar).getSize();
  for(Int_t i0=0; i0<n0; i0++) {
    if (pPar->writeline(buf,i0))
      pFile->write(buf,strlen(buf));
  }
  pPar->setChanged(kFALSE);
  pFile->write(sepLine,strlen(sepLine));
  return 1;
}
