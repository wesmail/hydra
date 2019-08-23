//*-- AUTHOR : Ilse Koenig
//*-- Created : 30/11/2000

///////////////////////////////////////////////////////////////
// HTofinoParAsciiFileIo
//
// Class for Tofino parameter input/output from/into Ascii file
//
///////////////////////////////////////////////////////////////

#include "htofinoparasciifileio.h"
#include "htofinolookup.h"
#include "htofinogeompar.h"

ClassImp(HTofinoParAsciiFileIo)

HTofinoParAsciiFileIo::HTofinoParAsciiFileIo(fstream* f) : HDetParAsciiFileIo(f) {
  // constructor calls the base class constructor
  fName="HTofinoParIo";
}

Bool_t HTofinoParAsciiFileIo::init(HParSet* pPar,Int_t* set) {
  // calls the appropriate read function for the container
  const  Text_t* name=pPar->GetName();
  if (pFile) {
    if (!strcmp(name,"TofinoLookup")) {
      return read((HTofinoLookup*)pPar,set,kTRUE);
    }
    if (!strcmp(name,"TofinoGeomPar"))
              return HDetParAsciiFileIo::read((HDetGeomPar*)pPar,set);
    return kFALSE;
  }
  cerr<<"No input file open"<<endl;
  return kFALSE;
}

Int_t HTofinoParAsciiFileIo::write(HParSet* pPar) {
  // calls the appropriate write function for the container
  if (pFile) {
    const  Text_t* name=pPar->GetName();
    if (!strcmp(name,"TofinoLookup")) return writeFile2((HTofinoLookup*)pPar);
    if (!strcmp(name,"TofinoGeomPar"))
        return HDetParAsciiFileIo::writeFile((HDetGeomPar*)pPar);
    return -1;
  }
  cerr<<"No output file open"<<endl;
  return -1;
}

template<class T> Bool_t HTofinoParAsciiFileIo::read(T* pPar, Int_t* set,
                                                     Bool_t needsClear) {
  // template function for all parameter containers
  // searches the container in the file, reads the data line by line and
  //   called the member function readline(...) of the container class
  const  Text_t* name=pPar->GetName();
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
    allFound=checkAllFound(set,6);
  } else {
    for(Int_t i=0; i<6; i++) {
      if (set[i]) {
        if (set[i]==999) set[i]=1;
        else allFound=kFALSE;
      } 
    }
    if (allFound) printf("%s initialized from Ascii file\n",name);
  }
  return allFound;
}

template<class T> Int_t HTofinoParAsciiFileIo::writeFile2(T* pPar) {
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
