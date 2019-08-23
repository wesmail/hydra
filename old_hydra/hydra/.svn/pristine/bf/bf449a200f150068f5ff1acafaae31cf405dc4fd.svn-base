/////////////////////////////////////////////////////////////
// HTriggerParAsciiFileIo
//
// Class for Trigger parameter input/output from/into Ascii file
//
/////////////////////////////////////////////////////////////

#include "htriggerparasciifileio.h"
#include "htriggerdetector.h"
#include "htriggerparmulep.h"
#include "htriggerparmudilep.h"
#include "htriggerparmommap.h"
#include "htriggerparshowermap.h"
#include "hades.h"
#include "hspectrometer.h"
#include "hruntimedb.h"

ClassImp(HTriggerParAsciiFileIo)

HTriggerParAsciiFileIo::HTriggerParAsciiFileIo(fstream* f) : HDetParAsciiFileIo(f) {
  // constructor calls the base class constructor
  fName="HTriggerParIo";
  HDetector* det=gHades->getSetup()->getDetector("Trigger");
}

Bool_t HTriggerParAsciiFileIo::init(HParSet* pPar,Int_t* set) {
  // calls the appropriate read function for the container
  const  Text_t* name=pPar->GetName();
  if (pFile) {
    pFile->clear();
    pFile->seekg(0,ios::beg);
    if (!strcmp(name,"TriggerParMuLep")) return read((HTriggerParMuLep*)pPar,set);
    if (!strcmp(name,"TriggerParMuDilep")) return read((HTriggerParMuDilep*)pPar,set);
    if (!strcmp(name,"TriggerParMomMap")) return read((HTriggerParMomMap*)pPar,set);
    if (!strcmp(name,"TriggerParShowerMap")) return read((HTriggerParShowerMap*)pPar,set);

    cerr<<"initialization of "<<name<<" not possible from Ascii file!"<<endl;
    return kFALSE;
  }
  cerr<<"no input file open"<<endl;
  return kFALSE;
}

Int_t HTriggerParAsciiFileIo::write(HParSet* pPar) {
  // calls the appropriate write function for the container
  if (pFile) {
    const  Text_t* name=pPar->GetName();
    if (!strcmp(name,"TriggerParMuLep")) return writeFile3((HTriggerParMuLep*)pPar);
    if (!strcmp(name,"TriggerParMuDilep")) return writeFile3((HTriggerParMuDilep*)pPar);
    if (!strcmp(name,"TriggerParMomMap")) return writeFile3((HTriggerParMomMap*)pPar);
    if (!strcmp(name,"TriggerParShowerMap")) return writeFile3((HTriggerParShowerMap*)pPar);

    cerr<<name<<" could not be written to Ascii file"<<endl;
    return -1;
  }
  cerr<<"no output file open"<<endl;
  return -1;
}

  template<class T> Int_t HTriggerParAsciiFileIo::writeFile3(T* pPar) {
  // template function for all parameter containers with 3 levels
  // writes the header, loops over the container and calls its member
  //   function writeline(...)
  pPar->putAsciiHeader(fHeader);
  writeHeader(pPar->GetName());
  Text_t buf[155];
 
  pFile->write(buf,strlen(buf));

  pPar->setChanged(kFALSE);
  pFile->write(sepLine,strlen(sepLine));
  return 1;
}

template<class T> Bool_t HTriggerParAsciiFileIo::read(T* pPar, Int_t* set,
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

  return kTRUE;
}














