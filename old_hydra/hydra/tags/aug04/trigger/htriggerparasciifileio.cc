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
#include "htriggerparrichmap.h"
#include "hrichIPUparpattern.h"
#include "hrichIPUparthresholds.h"
#include "hrichIPUparlocmax.h"
#include "hades.h"
#include "hspectrometer.h"
#include "hruntimedb.h"

ClassImp(HTriggerParAsciiFileIo)

HTriggerParAsciiFileIo::HTriggerParAsciiFileIo(fstream* f) : HDetParAsciiFileIo(f) {
  // constructor calls the base class constructor
  fName="HTriggerParIo";
}

Bool_t HTriggerParAsciiFileIo::init(HParSet* pPar,Int_t* set) {
  // calls the appropriate read function for the container
  const  Text_t* name=pPar->GetName();
  if (pFile) {
    pFile->clear();
    pFile->seekg(0,ios::beg);
    if (!strcmp(name,"TriggerParMomMap")) return read((HTriggerParMomMap*)pPar,set);
    if (!strcmp(name,"TriggerParShowerMap")) return read((HTriggerParShowerMap*)pPar,set);
    if (!strcmp(name,"TriggerParRichMap")) return read((HTriggerParRichMap*)pPar,set);

    if (strncmp(name,"TriggerParMuLep",strlen("TriggerParMuLep"))==0)
        return HDetParAsciiFileIo::readCond((HParCond*)pPar); 
    if (strncmp(name,"TriggerParMuDilep",strlen("TriggerParMuDilep"))==0)
        return HDetParAsciiFileIo::readCond((HParCond*)pPar); 
    if (!strcmp(name,"RichIPUParPattern")) return read((HRichIPUParPattern*)pPar,set);
    if (!strcmp(name,"RichIPUParThresholds")) return read((HRichIPUParThresholds*)pPar,set);
    if (strncmp(name,"RichIPUParLocMax",strlen("RichIPUParLocMax"))==0)
        return HDetParAsciiFileIo::readCond((HParCond*)pPar);
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
    if (!strcmp(name,"TriggerParMomMap")) return writeFile((HTriggerParMomMap*)pPar);
    if (!strcmp(name,"TriggerParShowerMap")) return writeFile((HTriggerParShowerMap*)pPar);
    if (!strcmp(name,"TriggerParRichMap")) return writeFile((HTriggerParRichMap*)pPar);

    if (strncmp(name,"TriggerParMuLep",strlen("TriggerParMuLep"))==0)
      return HDetParAsciiFileIo::writeCond((HParCond*)pPar);  
    if (strncmp(name,"TriggerParMuDilep",strlen("TriggerParMuDilep"))==0)
      return HDetParAsciiFileIo::writeCond((HParCond*)pPar);  
    if (!strcmp(name,"RichIPUParPattern")) return writeFile((HRichIPUParPattern*)pPar);
    if (!strcmp(name,"RichIPUParThresholds")) return writeFile((HRichIPUParThresholds*)pPar);
    if (strncmp(name,"RichIPUParLocMax",strlen("RichIPUParLocMax"))==0)
      return HDetParAsciiFileIo::writeCond((HParCond*)pPar);
    cerr<<name<<" could not be written to Ascii file"<<endl;
    return -1;
  }
  cerr<<"no output file open"<<endl;
  return -1;
}

Int_t HTriggerParAsciiFileIo::writeFile(HTriggerParMomMap* pPar) {
  // writes the header, loops over the container and calls its member
  //   function writeline(...)
  pPar->putAsciiHeader(fHeader);
  writeHeader(pPar->GetName());
  Text_t buf[155];
 
  if (pPar->writesize(buf)) 
    pFile->write(buf,strlen(buf));

  for (Int_t xx=0; xx < (*pPar).getTheta_bins(); xx++) {
    for (Int_t yy=0; yy < (*pPar).getDeltatheta_bins(); yy++) {
      for (Int_t zz=0; zz < (*pPar).getPhi_bins(); zz++) {
	if (pPar->writeline(buf, xx, yy, zz))
	  pFile->write(buf,strlen(buf));  
      }
    }
  }

  pPar->setChanged(kFALSE);
  pFile->write(sepLine,strlen(sepLine));
  return 1;
}


Int_t HTriggerParAsciiFileIo::writeFile(HTriggerParShowerMap* pPar) {
  // writes the header, loops over the container and calls its member
  //   function writeline(...)
  pPar->putAsciiHeader(fHeader);
  writeHeader(pPar->GetName());
  Text_t buf[155];
 
   for (Int_t xx=0; xx < 32; xx++) {
      for (Int_t yy=0; yy< 32; yy++) {
        if (pPar->writeline(buf, xx, yy))
          pFile->write(buf,strlen(buf));  
        }
      }

  pPar->setChanged(kFALSE);
  pFile->write(sepLine,strlen(sepLine));
  return 1;
}

Int_t HTriggerParAsciiFileIo::writeFile(HTriggerParRichMap* pPar) {
  // writes the header, loops over the container and calls its member
  //   function writeline(...)
  pPar->putAsciiHeader(fHeader);
  writeHeader(pPar->GetName());
  Text_t buf[155];
 
  for (Int_t xx=0; xx < 96; xx++) {
      for (Int_t yy=0; yy< 96; yy++) {
        if (pPar->writeline(buf, xx, yy))
          pFile->write(buf,strlen(buf));  
        }
      }

  pPar->setChanged(kFALSE);
  pFile->write(sepLine,strlen(sepLine));
  return 1;
}

Int_t HTriggerParAsciiFileIo::writeFile(HRichIPUParPattern* pPar) {
  // writes the header, loops over the container and calls its member
  //   function writeline(...)
  pPar->putAsciiHeader(fHeader);
  writeHeader(pPar->GetName());
  Text_t buf[155];

  if (pPar->writesize(buf))
    pFile->write(buf,strlen(buf));

  for (Int_t xx=0; xx < (*pPar).getNGroups(); xx++) {
    for (Int_t yy=0; yy < (*pPar).getNPadsinGroup(); yy++) {
      for (Int_t zz=0; zz < (*pPar).getNPadDimensions(); zz++) {
        if (pPar->writeline(buf, xx, yy, zz))
          pFile->write(buf,strlen(buf));
      }
    }
  }
  pPar->setChanged(kFALSE);
  pFile->write(sepLine,strlen(sepLine));
  return 1;
}

Int_t HTriggerParAsciiFileIo::writeFile(HRichIPUParThresholds* pPar) {
  // writes the header, loops over the container and calls its member
  //   function writeline(...)
  pPar->putAsciiHeader(fHeader);
  writeHeader(pPar->GetName());
  Text_t buf[155];

  for (Int_t xx=0; xx < 2; xx++) {
    for (Int_t yy=0; yy < 6; yy++) {
      if (pPar->writeline(buf, xx, yy))
        pFile->write(buf,strlen(buf));
    }
  }

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














