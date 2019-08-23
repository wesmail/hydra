//*-- AUTHOR :  Pablo Cabanelas
//*-- Created : 17/10/2005
//*-- Modified: 12/09/2006
//*-- Modified: 26/09/2006
//*-- Modified: 08/09/2006 by D. Gonzalez-Diaz
                                                                                              
/////////////////////////////////////////////////////////////
// HRpcParAsciiFileIo
//
// Class for RPC parameter input/output from/into Ascii file
//
/////////////////////////////////////////////////////////////
                                                                                              
#include "hrpcparasciifileio.h"
#include "hrpcdetector.h"
#include "hrpclookup.h"
#include "hrpcdigipar.h"
#include "hades.h"
#include "hspectrometer.h"
#include "hruntimedb.h"
                                                                                              
ClassImp(HRpcParAsciiFileIo)
                                                                                              
HRpcParAsciiFileIo::HRpcParAsciiFileIo(fstream* f) : HDetParAsciiFileIo(f) {
  // constructor calls the base class constructor
  fName="HRpcParIo";
  //HDetector* det=gHades->getSetup()->getDetector("Tof");  Por si queremos especificar
  //setSize=(det->getMaxSectors())*(det->getMaxModules());   el numero de modulos
  //  variable setSize  ->  public Int_t  en el header
}
                                                                                              
Bool_t HRpcParAsciiFileIo::init(HParSet* pPar,Int_t* set) {
  // calls the appropriate read function for the container
  const  Text_t* name=pPar->GetName();
  if (pFile) {
    pFile->clear();
    pFile->seekg(0,ios::beg);
    if (!strcmp(name,"RpcLookup")) return read((HRpcLookup*)pPar,set,kTRUE);
    if (!strcmp(name,"RpcDigiPar")) return read((HRpcDigiPar*)pPar,set);
    cerr<<"initialization of "<<name<<" not possible from Ascii file!"<<endl;
    return kFALSE;
  }
  cerr<<"no input file open"<<endl;
  return kFALSE;
}
                                                                                              
Int_t HRpcParAsciiFileIo::write(HParSet* pPar) {
  // calls the appropriate write function for the container
  if (pFile) {
    const  Text_t* name=pPar->GetName();
    if (!strcmp(name,"RpcLookup")) return writeFile1((HRpcLookup*)pPar);
    if (!strcmp(name,"RpcDigiPar")) return writeFile3((HRpcDigiPar*)pPar);
    cerr<<name<<" could not be written to Ascii file"<<endl;
    return -1;
  }
  cerr<<"no output file open"<<endl;
  return -1;
}
                                                                                              
template<class T> Int_t HRpcParAsciiFileIo::writeFile3(T* pPar) {
  // template function for all parameter containers with 3 levels
  // writes the header, loops over the container and calls its member
  //   function writeline(...)
  pPar->putAsciiHeader(fHeader);
  writeHeader(pPar->GetName());

  Text_t buf[155];
  Int_t n0 = (*pPar).getSize();
  for(Int_t i0=0; i0<n0; i0++) {
    Int_t n1 = (*pPar)[i0].getSize();
    for(Int_t i1=0; i1<n1; i1++) {
      Int_t n2 = (*pPar)[i0][i1].getSize();
      for(Int_t i2=0; i2<n2; i2++) {
        if (pPar->writeline(buf, i0, i1, i2))
          pFile->write(buf,strlen(buf));
      }
    }
  }
  pPar->setChanged(kFALSE);
  pFile->write(sepLine,strlen(sepLine));
  return 1;
}


template<class T> Int_t HRpcParAsciiFileIo::writeFile1(T* pPar) {
  // template function for all parameter containers with 1 level
  // writes the header, loops over the container and calls its member
  //   function writeline(...)
  pPar->putAsciiHeader(fHeader);
  writeHeader(pPar->GetName());
  Text_t buf[155];
  Int_t n0 = (*pPar).getSize();
  for(Int_t i0=0; i0<n0; i0++) {
     if (pPar->writeline(buf, i0))
       pFile->write(buf,strlen(buf));
  }
  pPar->setChanged(kFALSE);
  pFile->write(sepLine,strlen(sepLine));
  return 1;
}

                                                                                              
template<class T> Bool_t HRpcParAsciiFileIo::read(T* pPar, Int_t* set,
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
    if (buf[0]!='/' && buf[0]!='\0') pPar->readline(buf, set);
  }
  pPar->setInputVersion(1,inputNumber);
  pPar->setChanged();
  Bool_t allFound=kTRUE;
  if (!needsClear) {
    printf("%s: module(s) initialized from Ascii file:\n",name);
    allFound=checkAllFound(set,24);
  } else {
    for(Int_t i=0; i<1; i++) {
      if (set[i]) {
        if (set[i]==999) set[i]=1;
        else allFound=kFALSE;
      }
    }
  }
  if (allFound) printf("%s initialized from Ascii file\n",name);
  return allFound;
}
                                                                                              

