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
#include "hmdctdcthreshold.h"
#include "hmdctdcchannel.h"
#include "hmdccal2par.h"
#include "hmdccal2parsim.h"
#include "hmdccelleff.h"
#include "hmdcwirestat.h"
#include "hmdctrackfitpar.h"
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
#include "hmdctimecut.h"
#include "hmdcsetup.h"
#include "hmessagemgr.h"
#include "hmdcclosepaircutspar.h"

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
    if (!strncmp(name,"MdcCalParRaw",strlen("MdcCalParRaw")))
        return read((HMdcCalParRaw*)pPar,set);
    if (!strncmp(name,"MdcTdcThreshold",strlen("MdcTdcThreshold")))
        return read((HMdcTdcThreshold*)pPar,set);
    if (!strncmp(name,"MdcTdcChannel",strlen("MdcTdcChannel")))
        return read((HMdcTdcChannel*)pPar,set);
    if (!strncmp(name,"MdcTrackFitPar",strlen("MdcTrackFitPar")))
	return HDetParAsciiFileIo::readCond((HParCond*)pPar);
    if (!strcmp(name,"MdcCal2ParSim"))
        return read((HMdcCal2ParSim*)pPar,set);
    if (!strncmp(name,"MdcCellEff",strlen("MdcCellEff")))
        return read((HMdcCellEff*)pPar,set,4);
    if (!strncmp(name,"MdcWireStat",strlen("MdcWireStat")))
	return HDetParAsciiFileIo::readCond((HParCond*)pPar);
    if (!strcmp(name,"MdcLookupGeom"))
        return read((HMdcLookupGeom*)pPar,set);
    if (!strncmp(name,"MdcDigitPar",strlen("MdcDigitPar")))
	return HDetParAsciiFileIo::readCond((HParCond*)pPar);
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
    if (!strncmp(name,"MdcTimeCut",strlen("MdcTimeCut")))
      return read((HMdcTimeCut*)pPar,set);
    if (!strncmp(name,"MdcSetup",strlen("MdcSetup")))
      return HDetParAsciiFileIo::readCond((HParCond*)pPar);
    if (!strcmp(name,"MdcClosePairCutsPar"))
      return read((HMdcClosePairCutsPar*)pPar,set,0);

    gHades->getMsg()->error(1,DET_MDC,GetName(),"initialization of %s not possible from file!",name);
   
    return kFALSE;
  }
  ERROR_msg(DET_MDC,"no input file open");
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
    if (!strncmp(name,"MdcCalParRaw",strlen("MdcCalParRaw")))
        return writeFile4((HMdcCalParRaw*)pPar);
    if (!strncmp(name,"MdcTdcThreshold",strlen("MdcTdcThreshold")))
        return writeFile4((HMdcTdcThreshold*)pPar);
    if (!strncmp(name,"MdcTdcChannel",strlen("MdcTdcChannel")))
        return writeFile4((HMdcTdcChannel*)pPar);
    if (!strcmp(name,"MdcCal2Par"))
        return writeFile3_x((HMdcCal2Par*)pPar);
    if (!strcmp(name,"MdcCal2ParSim"))
        return writeFile3_x((HMdcCal2ParSim*)pPar);
    if (!strncmp(name,"MdcCellEff",strlen("MdcCellEff")))
        return writeFile2((HMdcCellEff*)pPar);
    if (!strncmp(name,"MdcWireStat",strlen("MdcWireStat")))
        return HDetParAsciiFileIo::writeCond((HParCond*)pPar);
    if (!strncmp(name,"MdcTrackFitPar",strlen("MdcTrackFitPar")))
        return HDetParAsciiFileIo::writeCond((HParCond*)pPar);
    if (!strcmp(name,"MdcLookupGeom"))
        return writeFile4((HMdcLookupGeom*)pPar);
    if (!strcmp(name,"MdcLookupRaw"))
        return writeFile4((HMdcLookupRaw*)pPar);
    if (!strncmp(name,"MdcDigitPar",strlen("MdcDigitPar")))
	return HDetParAsciiFileIo::writeCond((HParCond*)pPar);
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
    if(!strncmp(name,"MdcTimeCut",strlen("MdcTimeCut")))
      return writeFile2((HMdcTimeCut*)pPar);
    if (strncmp(name,"MdcSetup",strlen("MdcSetup"))==0)
	return HDetParAsciiFileIo::writeCond((HParCond*)pPar);
    if (!strcmp(name,"MdcClosePairCutsPar"))
      return writeFile4((HMdcClosePairCutsPar*)pPar);

    gHades->getMsg()->error(1,DET_MDC,GetName(),"%s could not be written to Ascii file",name);
    return -1;
  }
  ERROR_msg(DET_MDC,"no output file open");
  return -1;
}

template<class T> Int_t HMdcParAsciiFileIo::writeFile2(T* pPar) {
  // template function for all parameter containers with 2 levels
  // writes the header, loops over the container and calls its member
  //   function writeline(...)
  pPar->putAsciiHeader(fHeader);
  writeHeader(pPar->GetName());
  Text_t buf[800];
  Int_t n0 = (*pPar).getSize();
  for(Int_t i0=0; i0<n0; i0++) {
    Int_t n1 = (*pPar)[i0].getSize();
    for(Int_t i1=0; i1<n1; i1++) {
      if (pPar->writeline(buf, i0, i1))
	pFile->write(buf,strlen(buf));
    }
  }
  pPar->setChanged(kFALSE);
  pFile->write(sepLine,strlen(sepLine));
  return 1;
}

template<class T> Int_t HMdcParAsciiFileIo::writeFile2_x(T* pPar) {
  // template function for all parameter containers with 2 levels
  // writes the header, loops over the container and calls its member
  //   function writeline(...)
  pPar->putAsciiHeader(fHeader);
  writeHeader(pPar->GetName());
  Text_t buf[800];
  Int_t n0 = (*pPar).getSize();
  Int_t numberOfLines=(*pPar).getNumberOfLines();
  for(Int_t i0=0; i0<n0; i0++) {
      Int_t n1 = (*pPar)[i0].getSize();
      for(Int_t i1=0; i1<n1; i1++) {
	  for(Int_t i2=0; i2<numberOfLines; i2++) {
	      if (pPar->writeline(buf, i0,i1))
		  pFile->write(buf,strlen(buf));
	  }
      }
  }
  pPar->setChanged(kFALSE);
  pFile->write(sepLine,strlen(sepLine));
  return 1;
}


template<class T> Int_t HMdcParAsciiFileIo::writeFile3(T* pPar) {
  // template function for all parameter containers with 3 levels
  // writes the header, loops over the container and calls its member
  //   function writeline(...)
  pPar->putAsciiHeader(fHeader);
  writeHeader(pPar->GetName());
  Text_t buf[800];
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

template<class T> Int_t HMdcParAsciiFileIo::writeFile3_x(T* pPar) {
  // template function for all parameter containers with 3 levels
  // writes the header, loops over the container and calls its member
  //   function writeline(...)
  pPar->putAsciiHeader(fHeader);
  writeHeader(pPar->GetName());
  Text_t buf[800];
  Int_t n0 = (*pPar).getSize();
  Int_t numberOfLines=(*pPar).getNumberOfLines();
  for(Int_t i0=0; i0<n0; i0++) {
    Int_t n1 = (*pPar)[i0].getSize();
    for(Int_t i1=0; i1<n1; i1++) {
      Int_t n2 = (*pPar)[i0][i1].getSize();
      for(Int_t i2=0; i2<n2; i2++) {
	  for(Int_t i3=0; i3<numberOfLines; i3++) {
	      if (pPar->writeline(buf, i0, i1, i2))
		  pFile->write(buf,strlen(buf));
	  }
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
  Text_t buf[800];
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

template<class T> Bool_t HMdcParAsciiFileIo::read(T* pPar, Int_t* set,Int_t num) {
  // template function for all parameter containers
  // searches the container in the file, reads the data line by line and
  //   called the member function readline(...) of the container class
  const  Text_t* name=pPar->GetName();
  if (!findContainer(name)) return kFALSE;
  const Int_t maxbuf=800;
  Text_t buf[maxbuf];
  while (!pFile->eof()) {
      pFile->getline(buf, maxbuf);
      if (buf[0]=='#') break;
      if (buf[0]!='/' && buf[0]!='\0') pPar->readline(buf, set);
  }
  pPar->setInputVersion(1,inputNumber);
  pPar->setChanged();
  gHades->getMsg()->info(10,DET_MDC,GetName(),"%s: module(s) initialized from Ascii file:",name);
  return checkAllFound(set,num);
  INFO_msg(10,DET_MDC,"end of reading");
  return kFALSE;
}
