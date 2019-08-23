//*-- modified : 06/10/2000 by I.Koenig

///////////////////////////////////////////////////////////////
// HStartParAsciiFileIo
//
// Class for Start parameter input/output from/into Ascii file
//
///////////////////////////////////////////////////////////////

#include "hstartparasciifileio.h"
#include "hstartlookup.h"
#include "hstartcalpar.h"
#include "hstartgeompar.h"

ClassImp(HStartParAsciiFileIo)

HStartParAsciiFileIo::HStartParAsciiFileIo(fstream* f) : HDetParAsciiFileIo(f) {
  // constructor calls the base class constructor
  fName="HStartParIo";
}

Bool_t HStartParAsciiFileIo::init(HParSet* pPar,Int_t* set) {
  // calls the appropriate read function for the container
  const  Text_t* name=pPar->GetName();
  if (pFile) {
    pFile->clear();
    pFile->seekg(0,ios::beg);
    if (!strcmp(name,"StartLookup")) return read((HStartLookup*)pPar,set);
    if (!strcmp(name,"StartCalPar")) return read((HStartCalPar*)pPar,set);
    if (!strcmp(name,"StartGeomPar"))
              return HDetParAsciiFileIo::read((HDetGeomPar*)pPar,set);
     cerr<<"initialization of "<<name<<" not possible from file!"<<endl;
    return kFALSE;
  }
  cerr<<"no input file open"<<endl;
  return kFALSE;
}

Int_t HStartParAsciiFileIo::write(HParSet* pPar) {
  // calls the appropriate write function for the container
  if (pFile) {
    const  Text_t* name=pPar->GetName();
    if (!strcmp(name,"StartLookup")) return writeFile2((HStartLookup*)pPar);
    if (!strcmp(name,"StartCalPar")) return writeFile2((HStartCalPar*)pPar);
    if (!strcmp(name,"StartGeomPar"))
        return HDetParAsciiFileIo::writeFile((HDetGeomPar*)pPar);
     cerr<<name<<" could not be written to Ascii file"<<endl;
    return -1;
  }
  cerr<<"no output file open"<<endl;
  return -1;
}

template<class T> Bool_t HStartParAsciiFileIo::read(T* pPar, Int_t* set) {
  // template function for all parameter containers
  // searches the container in the file, reads the data line by line and
  //   called the member function readline(...) of the container class
  const  Text_t* name=pPar->GetName();
  if (!findContainer(name)) return kFALSE;
  const Int_t maxbuf=155;
  Text_t buf[maxbuf];
  while (!pFile->eof()) {
    Char_t c=pFile->get();
    if (c!='#' && c!='/') {
      pFile->putback(c);
      pFile->getline(buf,maxbuf);
      pPar->readline(buf,set);
    } else {
      pPar->setInputVersion(1,inputNumber);
      pPar->setChanged();
      printf("%s: module(s) initialized from Ascii file:",name);
      return checkAllFound(set,2);
    }
  }
  return kFALSE;
}

template<class T> Int_t HStartParAsciiFileIo::writeFile2(T* pPar) {
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