//*-- AUTHOR : ???
//*-- modified : 08/03/2000 by Ilse Koenig

/////////////////////////////////////////////////////////////
// HMdcParAsciiFileIo
//
// Class for Mdc parameter input/output from/into Ascii file
//
/////////////////////////////////////////////////////////////

#include "hmdcparasciifileio.h"
#include "hmdcrawstruct.h"
#include "hmdcgeomstruct.h"
#include "hmdcmboreadout.h"
#include "hmdccalpar.h"
#include "hmdccalparraw.h"
#include "hmdclookupgeom.h"
#include "hmdclookupraw.h"
#include "hmdcdigitpar.h"
#include "hmdclayergeompar.h"
#include "hmdcgeompar.h"
#include "hmdcmodulegeometry.h"
#include "hmdchitfpar.h"
#include "hmdctargetgeometry.h"
#include "hmdcsegmentfpar.h"
#include "hmdcleverarmgeometry.h"

ClassImp(HMdcParAsciiFileIo)

HMdcParAsciiFileIo::HMdcParAsciiFileIo(fstream* f) : HDetParAsciiFileIo(f) {
  // constructor calls the base class constructor
  fName="HMdcParIo";
}

Bool_t HMdcParAsciiFileIo::init(HParSet* pPar,Int_t* set) {
  // calls the appropriate read function for the container
  const  Text_t* name=pPar->GetName();
  if (pFile) {
    pFile->clear();
    pFile->seekg(0,ios::beg);
    if (!strcmp(name,"MdcRawStruct"))
        return read((HMdcRawStruct*)pPar,set);
    if (!strcmp(name,"MdcGeomStruct"))
        return read((HMdcGeomStruct*)pPar,set);
    if (!strcmp(name,"MdcMboReadout"))
        return read((HMdcMboReadout*)pPar,set);
    if (!strcmp(name,"MdcCalParRaw"))
        return read((HMdcCalParRaw*)pPar,set);
    if (!strcmp(name,"MdcLookupGeom"))
        return read((HMdcLookupGeom*)pPar,set);
    if (!strcmp(name,"MdcDigitPar"))
        return read((HMdcDigitPar*)pPar,set);
    if (!strcmp(name,"MdcLayerGeomPar"))
        return read((HMdcLayerGeomPar*)pPar,set);
    if (!strcmp(name,"MdcGeomPar"))
        return HDetParAsciiFileIo::read((HDetGeomPar*)pPar,set);
    if(!strcmp(name,"MdcHitFPar")) 
        return ((HMdcHitFPar*)pPar)->read(pFile,set,inputNumber);
    if(!strcmp(name,"MdcModuleGeometry")) 
        return ((HMdcModuleGeometry*)pPar)->read(pFile,set,inputNumber);
    if(!strcmp(name,"MdcTargetGeometry")) 
        return ((HMdcTargetGeometry*)pPar)->read(pFile,set,inputNumber);
    if (!strcmp(name,"MdcSegmentFPar")) 
        return ((HMdcSegmentFPar*)pPar)->read(pFile,set,inputNumber);
    if (!strcmp(name,"MdcLeverArmGeometry")) 
        return ((HMdcLeverArmGeometry*)pPar)->read(pFile,set,inputNumber);

    cerr<<"initialization of "<<name<<" not possible from file!"<<endl;
    return kFALSE;
  }
  cerr<<"no input file open"<<endl;
  return kFALSE;
}

Int_t HMdcParAsciiFileIo::write(HParSet* pPar) {
  // calls the appropriate write function for the container
  if (pFile) {
    const  Text_t* name=pPar->GetName();
    if (!strcmp(name,"MdcRawStruct"))
        return writeFile3((HMdcRawStruct*)pPar);
    if (!strcmp(name,"MdcGeomStruct"))
        return writeFile3((HMdcGeomStruct*)pPar);
    if (!strcmp(name,"MdcMboReadout"))
        return writeFile3((HMdcMboReadout*)pPar);
    if (!strcmp(name,"MdcCalPar"))
        return writeFile4((HMdcCalPar*)pPar);
    if (!strcmp(name,"MdcCalParRaw"))
        return writeFile4((HMdcCalParRaw*)pPar);
    if (!strcmp(name,"MdcLookupGeom"))
        return writeFile4((HMdcLookupGeom*)pPar);
    if (!strcmp(name,"MdcLookupRaw"))
        return writeFile4((HMdcLookupRaw*)pPar);
    if (!strcmp(name,"MdcDigitPar"))
        return writeFile3((HMdcDigitPar*)pPar);
    if (!strcmp(name,"MdcLayerGeomPar"))
        return writeFile3((HMdcLayerGeomPar*)pPar);
    if (!strcmp(name,"MdcGeomPar"))
        return HDetParAsciiFileIo::writeFile((HDetGeomPar*)pPar);
    if(!strcmp(name,"MdcHitFPar"))
        return ((HMdcHitFPar*)pPar)->writeFile(pFile);
    if(!strcmp(name,"MdcModuleGeometry"))
        return ((HMdcModuleGeometry*)pPar)->writeFile(pFile);
    if(!strcmp(name,"MdcTargetGeometry"))
        return ((HMdcTargetGeometry*)pPar)->writeFile(pFile);
    if(!strcmp(name,"MdcSegmentFPar"))
        return ((HMdcSegmentFPar*)pPar)->writeFile(pFile);
    if(!strcmp(name,"MdcLeverArmGeometry"))
        return ((HMdcLeverArmGeometry*)pPar)->writeFile(pFile);

    cerr<<name<<" could not be written to Ascii file"<<endl;
    return -1;
  }
  cerr<<"no output file open"<<endl;
  return -1;
}

template<class T> Int_t HMdcParAsciiFileIo::writeFile3(T* pPar) {
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

template<class T> Int_t HMdcParAsciiFileIo::writeFile4(T* pPar) {
  // template function for all parameter containers with 4 levels
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
        Int_t n3 = (*pPar)[i0][i1][i2].getSize();
        for(Int_t i3=0; i3<n3; i3++) {
          if (pPar->writeline(buf, i0, i1, i2, i3))
            pFile->write(buf,strlen(buf));
        }
      }
    }
  }
  pPar->setChanged(kFALSE);
  pFile->write(sepLine,strlen(sepLine));
  return 1;
}

template<class T> Bool_t HMdcParAsciiFileIo::read(T* pPar, Int_t* set) {
  // template function for all parameter containers
  // searches the container in the file, reads the data line by line and
  //   called the member function readline(...) of the container class
  cout << "read " << endl;
  const  Text_t* name=pPar->GetName();
  if (!findContainer(name)) return kFALSE;
  const Int_t maxbuf=155;
  Text_t buf[maxbuf];
  while (!pFile->eof()) {
    Char_t c=pFile->get();
    if (c!='#' && c!='/') {
      pFile->putback(c);
      pFile->getline(buf, maxbuf);
      pPar->readline(buf, set);
    } else {
      pPar->setInputVersion(1,inputNumber);
      pPar->setChanged();
      printf("%s: module(s) initialized from Ascii file:",name);
      return checkAllFound(set,24);
    }
  }
  cout << "end of reading" << endl;
  return kFALSE;
}
