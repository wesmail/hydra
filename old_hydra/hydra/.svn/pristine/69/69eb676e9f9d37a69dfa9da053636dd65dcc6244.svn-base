//*-- AUTHOR : Ilse Koenig
//*-- Modified : 02/11/98
//*-- Modified : 10/08/2001 by J. Otwinowski 
//*-- Modified : 30/08/2001 by T. Wojcik
//*-- Modified : 28/02/2002 by J. Otwinowski 

/////////////////////////////////////////////////////////////
//HShowerParRootFileIo
//
//  Class for Shower parameter input/output from/into ROOT file
//
//  It is derived from the base class HDetParFileIo and
//  inherits from it basic functions e.g. write(...)
//
/////////////////////////////////////////////////////////////

#include "hshowerparrootfileio.h"
#include "hparrootfileio.h"
#include "hades.h"
#include "hspectrometer.h"
#include "hruntimedb.h"
#include "hdetparrootfileio.h"
#include "hshowergeometry.h"
#include "hshowerdigidetpar.h"
#include "hshowerhitfpar.h"
#include "hshowercalpar.h"
#include "hshowerhist.h"
#include "hshowerunpackparam.h"
#include "hshowergeompar.h"
#include "hshowerparset.h"

#include <iostream.h>

ClassImp(HShowerParRootFileIo)

HShowerParRootFileIo::HShowerParRootFileIo(HParRootFile* f) : HDetParRootFileIo(f) {
  // constructor sets the name of the detector I/O "HShowerParIo"
  fName="HShowerParIo";
  initModules=new TArrayI(18);
}


HShowerParRootFileIo::~HShowerParRootFileIo() {
  // destructor
  if (modulesFound) {
    delete modulesFound;
    modulesFound=0;
  }
  if (initModules) {
    delete initModules;
    initModules=0;
  }
}


Bool_t HShowerParRootFileIo::init(HParSet* pPar,Int_t* set) {
  // initializes a container called by name, but only the modules
  // defined in the array 'set'
  // calls the special read function for this container
  // If it is called the first time it reads the setup found in the file

  if (!isActiv) readModules("Shower");
  const Text_t* name = ((TNamed*)pPar)->GetName();
  printf("Shower par file io init - %s\n", name);
  if (pFile) {
    if (strcmp(name,"ShowerGeometry")==0) {
			if (HDetParRootFileIo::read((HShowerGeometry*)pPar)){
				((HShowerGeometry*)pPar)->attachTables();
				return kTRUE;
			}
			
		return kFALSE;
		}
		//return read((HShowerGeometry*)pPar,set);
  //  if (strncmp(name,"ShowerDigiPar",strlen("ShowerDigiPar"))==0) return read((HShowerDigiDetPar*)pPar,set);
    if (strcmp(name,"ShowerDigiPar")==0) return read((HShowerDigiDetPar*)pPar,set);
    if (strcmp(name,"ShowerHitFinderParams")==0) return read((HShowerHitFPar*)pPar,set);
    if (strcmp(name,"ShowerCalPar")==0) return read((HShowerCalPar*)pPar,set);
    if (strcmp(name,"ShowerHist")==0) return read((HShowerHist*)pPar,set);
    if (strcmp(name,"ShowerUnpackParam")==0) return read((HShowerUnpackParam*)pPar,set);
    if (strcmp(name,"ShowerGeomPar")==0)
        return HDetParRootFileIo::read((HShowerGeomPar*)pPar,set);
   }
  cerr<<"initialization of "<<name<<" not possible from ROOT file!"<<endl;
  return kFALSE;
}
/*
Bool_t HShowerParRootFileIo::readObject(HShowerParSet *pPar) {
  const Text_t* name = ((TNamed*)pPar)->GetName();
  TKey *key = (TKey*)gDirectory->GetListOfKeys()->FindObject(name);

  if (key) {
         pPar->Read(name);
         pPar->setStatic(); 
  }
  return (key != NULL);
}
*/

Bool_t HShowerParRootFileIo::readObject(HShowerParSet *pPar) {

  Text_t* name = (char*)pPar->GetName();
  Int_t version = findInputVersion(name);

  if(version<=0){    // not in ROOT file
	printf("Container %s  not found in RuntimeDB in the %d ROOT input\n",name,inputNumber);
    pPar->setInputVersion(-1,inputNumber);
	return kFALSE;
  }
  if (pPar->getInputVersion(inputNumber)==version
        && pPar->getInputVersion(inputNumber)!=-1) return kTRUE;

  //TKey *key = (TKey*)gDirectory->GetListOfKeys()->FindObject(name);
  TKey *key = (TKey*)gDirectory->GetKey(name,version);

          // need reinizalization 
  
      if(key)
      { 

      key->Read(pPar); 
	  pPar->setInputVersion(version,inputNumber);
	  pPar->setChanged();

      } else {
 
	  pPar->setInputVersion(-1,inputNumber);
	//  pPar->setChanged();

      }
	  return (key != NULL);
}

	Bool_t HShowerParRootFileIo::read(HShowerGeometry* pPar,Int_t* set) {
	  //if (!readObject(pPar))
	     return kFALSE;

	  pPar->attachTables();
	//  pPar->transToLab();

	  printf("Container ShowerGeometry initialized from the %d ROOT input\n",inputNumber); 
	  return kTRUE;
	}


	Bool_t HShowerParRootFileIo::read(HShowerDigiDetPar* pPar,Int_t* set) {
	  if (!readObject(pPar))
	     return kFALSE;
	  
	  printf("Container ShowerDigiDetPar initialized from the %d ROOT input\n",inputNumber); 
	  return kTRUE;
	}

	Bool_t HShowerParRootFileIo::read(HShowerHitFPar* pPar,Int_t* set) {
	  if (!readObject(pPar))
	     return kFALSE;

	  printf("Container ShowerHitFPar initialized from the %d ROOT input\n",inputNumber); 
	  return kTRUE;
	}

	Bool_t HShowerParRootFileIo::read(HShowerCalPar* pPar,Int_t* set) {
	  if (!readObject(pPar))
	     return kFALSE;

	  printf("Container ShowerCalPar initialized from the %d ROOT input\n",inputNumber); 
	  return kTRUE;
	}

	Bool_t HShowerParRootFileIo::read(HShowerHist* pPar,Int_t* set) {
	  pPar->defaultInit(); //nothing to read
	  return kTRUE;  
	}

	Bool_t HShowerParRootFileIo::read(HShowerUnpackParam* pPar,Int_t* set) {
	  if (!readObject(pPar))
	     return kFALSE;

	  printf("Container ShowerUnpackParam initialized from %d ROOT input\n",inputNumber); 
	  return kTRUE;
	}

void  HShowerParRootFileIo::readContainer(HShowerParSet *pPar,char *name,Int_t version)
{
    char buff[256];
    sprintf(buff,"%s;%i",name,version);
    pPar->Read(buff);
}

	  

